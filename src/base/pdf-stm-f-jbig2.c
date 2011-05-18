/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-jbig2.c
 *       Date:         Fri Nov 21 01:12:48 2008
 *
 *       GNU PDF Library - JBIG2 encoder/decoder
 *
 */

/* Copyright (C) 2008-2011 Free Software Foundation, Inc. */

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <config.h>


#include <string.h>
#include <stdint.h>

#include <jbig2.h>

#include <pdf-hash-helper.h>
#include <pdf-stm-f-jbig2.h>
#include <pdf-types.h>
#include <pdf-types-buffer.h>
#include <pdf-hash.h>

/* Internal state */
struct pdf_stm_f_jbig2dec_s
{
  Jbig2Allocator *jbig2_allocator;
  Jbig2Ctx *jbig2_context;
  Jbig2GlobalCtx *jbig2_global_context;
  Jbig2ErrorCallback jbig2_error_cb_fn;
  Jbig2Image *jbig2_page;
  pdf_size_t index;
  pdf_bool_t error_p;
  pdf_size_t data_size;
};

static int jbig2dec_error_cb (void          *data,
                              const char    *msg,
                              Jbig2Severity  severity,
                              int32_t        seg_idx);

static pdf_bool_t stm_f_jbig2dec_init (const pdf_hash_t  *params,
                                       void             **state,
                                       pdf_error_t      **error);

static void stm_f_jbig2dec_deinit (void *state);

static enum pdf_stm_filter_apply_status_e stm_f_jbig2dec_apply (void          *state,
                                                                pdf_buffer_t  *in,
                                                                pdf_buffer_t  *out,
                                                                pdf_bool_t     finish,
                                                                pdf_error_t  **error);

/* -- Filter implementations -- */

static const pdf_stm_filter_impl_t dec_impl = {
  .init_fn   = stm_f_jbig2dec_init,
  .apply_fn  = stm_f_jbig2dec_apply,
  .deinit_fn = stm_f_jbig2dec_deinit,
};

const pdf_stm_filter_impl_t *
pdf_stm_f_jbig2dec_get (void)
{
  return &dec_impl;
}

static pdf_bool_t
stm_f_jbig2dec_init (const pdf_hash_t  *params,
                     void             **state,
                     pdf_error_t      **error)
{
  struct pdf_stm_f_jbig2dec_s *filter_state;
  pdf_char_t *global_stream_buffer;
  pdf_size_t global_stream_size;

  /* Allocate the internal state structure */
  filter_state = pdf_alloc (sizeof (struct pdf_stm_f_jbig2dec_s));
  if (!filter_state)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "cannot create LZW encoder internal state: "
                     "couldn't allocate %lu bytes",
                     (unsigned long)sizeof (struct pdf_stm_f_jbig2dec_s));
      return PDF_FALSE;
    }

  /* Initialize fields */
  filter_state->jbig2_allocator = NULL; /* Use default */
  filter_state->jbig2_error_cb_fn = jbig2dec_error_cb; /* Use default */
  filter_state->jbig2_page = NULL;
  filter_state->index = 0;
  filter_state->error_p = PDF_FALSE;

  /* Get the global stream contents, if any */
  if (params &&
      pdf_hash_key_p (params, "GlobalStreamsBuffer") == PDF_TRUE &&
      pdf_hash_key_p (params, "GlobalStreamsSize") == PDF_TRUE)
    {
      /* Get the parameters from the hash table */
      global_stream_buffer = (pdf_char_t *) pdf_hash_get_string (params, "GlobalStreamsBuffer");
      global_stream_size = pdf_hash_get_size (params, "GlobalStreamsSize");

      /* Initialize the global context */
      filter_state->jbig2_context = jbig2_ctx_new (filter_state->jbig2_allocator,
                                                   JBIG2_OPTIONS_EMBEDDED,
                                                   NULL,
                                                   filter_state->jbig2_error_cb_fn,
                                                   (void *) filter_state);

      jbig2_data_in (filter_state->jbig2_context,
                     (pdf_uchar_t *) global_stream_buffer,
                     global_stream_size);
      filter_state->jbig2_global_context =
        jbig2_make_global_ctx (filter_state->jbig2_context);
    }
  else
    {
      /* Do not use a global context */
      filter_state->jbig2_global_context = NULL;
    }

  filter_state->jbig2_context = jbig2_ctx_new (filter_state->jbig2_allocator,
                                               JBIG2_OPTIONS_EMBEDDED,
                                               filter_state->jbig2_global_context,
                                               filter_state->jbig2_error_cb_fn,
                                               (void *) filter_state);

  *state = (void *) filter_state;
  return PDF_TRUE;
}

static void
stm_f_jbig2dec_deinit (void *state)
{
  struct pdf_stm_f_jbig2dec_s *filter_state = state;

  jbig2_ctx_free (filter_state->jbig2_context);
  pdf_dealloc (filter_state);
}

static enum pdf_stm_filter_apply_status_e
stm_f_jbig2dec_apply (void          *state,
                      pdf_buffer_t  *in,
                      pdf_buffer_t  *out,
                      pdf_bool_t     finish,
                      pdf_error_t  **error)
{
  struct pdf_stm_f_jbig2dec_s *filter_state = state;
  pdf_status_t ret;
  pdf_size_t bytes_to_copy;

  if (finish)
    {
      /* Get the page, if needed */
      if (filter_state->jbig2_page == NULL)
        {
          /* Get the page */
          jbig2_complete_page (filter_state->jbig2_context);
          filter_state->jbig2_page =
            jbig2_page_out (filter_state->jbig2_context);
        }

      if (filter_state->jbig2_page == NULL)
        return PDF_STM_FILTER_APPLY_STATUS_EOF;

      /* Write out the data in the jbig2 page, if any */
      bytes_to_copy = out->size - out->wp;
      if ((filter_state->index + bytes_to_copy) >
          (filter_state->jbig2_page->height *
           filter_state->jbig2_page->stride))
        {
          bytes_to_copy = ((filter_state->jbig2_page->height *
                            filter_state->jbig2_page->stride)
                           - filter_state->index);
        }

      memcpy (out->data + out->wp,
              filter_state->jbig2_page->data + filter_state->index,
              bytes_to_copy);

      out->wp += bytes_to_copy;
      filter_state->index += bytes_to_copy;

      /* Determine the output state */
      if (filter_state->index ==
          (filter_state->jbig2_page->height *
           filter_state->jbig2_page->stride))
        {
          jbig2_release_page(filter_state->jbig2_context,
                             filter_state->jbig2_page);
          return PDF_STM_FILTER_APPLY_STATUS_EOF;
        }

      return PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT;
    }

  /* Write input into the decoder */
  PDF_ASSERT (in->wp >= in->rp);
  bytes_to_copy = (in->wp - in->rp);
  jbig2_data_in (filter_state->jbig2_context,
                 (pdf_uchar_t *) (in->data + in->rp),
                 bytes_to_copy);

  /* If any error, propagate it */
  if (filter_state->error_p == PDF_TRUE)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ERROR,
                     "jbig2dec error detected");
      return PDF_STM_FILTER_APPLY_STATUS_ERROR;
    }

  in->rp += bytes_to_copy;

  return PDF_STM_FILTER_APPLY_STATUS_NO_INPUT;
}

/*
 * Private functions
 */

static int
jbig2dec_error_cb (void          *data,
                   const char    *msg,
                   Jbig2Severity  severity,
                   int32_t        seg_idx)
{
  struct pdf_stm_f_jbig2dec_s *filter_state = data;

  if (severity == JBIG2_SEVERITY_FATAL)
    filter_state->error_p = PDF_TRUE;

  return 0;
}

/* End of pdf-stm-f-jbig2.c */

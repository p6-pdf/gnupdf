/* -*- mode: C -*- Time-stamp: "09/09/05 22:52:24 jemarch"
 *
 *       File:         pdf-stm-f-jbig2.c
 *       Date:         Fri Nov 21 01:12:48 2008
 *
 *       GNU PDF Library - JBIG2 encoder/decoder
 *
 */

/* Copyright (C) 2008, 2009 Free Software Foundation, Inc. */

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

#include <pdf-hash-helper.h>
#include <pdf-stm-f-jbig2.h>

static int jbig2dec_error_cb (void *data,
                              const char *msg, Jbig2Severity severity,
                              int32_t seg_idx);


pdf_status_t
pdf_stm_f_jbig2dec_init (pdf_hash_t params,
                         void **state)
{
  pdf_stm_f_jbig2dec_t filter_state;
  pdf_char_t *global_stream_buffer;
  pdf_size_t global_stream_size;


  /* Allocate the internal state structure */
  filter_state = pdf_alloc (sizeof (struct pdf_stm_f_jbig2dec_s));
  if (filter_state == NULL)
    {
      return PDF_ERROR;
    }

  /* Initialize fields */
  filter_state->jbig2_allocator = NULL; /* Use default */
  filter_state->jbig2_error_cb_fn = jbig2dec_error_cb; /* Use default */
  filter_state->jbig2_page = NULL;
  filter_state->index = 0;
  filter_state->error_p = PDF_FALSE;

  /* Get the global stream contents, if any */
  if ((pdf_hash_key_p (params, "GlobalStreamsBuffer") == PDF_TRUE) &&
      (pdf_hash_key_p (params, "GlobalStreamsSize") == PDF_TRUE))
    {
      /* Get the parameters from the hash table */
      pdf_hash_get_string (params,
                           "GlobalStreamsBuffer",
                           &global_stream_buffer);
      pdf_hash_get_size (params,
                         "GlobalStreamsSize",
                         &global_stream_size);

      /* Initialize the global context */
      filter_state->jbig2_context = jbig2_ctx_new (filter_state->jbig2_allocator,
                                                   JBIG2_OPTIONS_EMBEDDED,
                                                   NULL,
                                                   filter_state->jbig2_error_cb_fn,
                                                   (void *) filter_state);

      jbig2_data_in (filter_state->jbig2_context,
                     global_stream_buffer,
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
  return PDF_OK;
}

pdf_status_t
pdf_stm_f_jbig2dec_apply (pdf_hash_t params,
                          void *state,
                          pdf_buffer_t in,
                          pdf_buffer_t out,
                          pdf_bool_t finish_p)
{
  pdf_status_t ret;
  pdf_stm_f_jbig2dec_t filter_state;
  pdf_size_t bytes_to_copy;
  filter_state = (pdf_stm_f_jbig2dec_t) state;

  ret = PDF_OK;

  if (finish_p)
    {
      /* Get the page, if needed */
      if (filter_state->jbig2_page == NULL)
        {
          /* Get the page */
          jbig2_complete_page (filter_state->jbig2_context);
          filter_state->jbig2_page =
            jbig2_page_out (filter_state->jbig2_context);
        }

      /* Write out the data in the jbig2 page, if any */
      if (filter_state->jbig2_page != NULL)
        {
          bytes_to_copy = out->size - out->wp;
          if ((filter_state->index + bytes_to_copy)
              > (filter_state->jbig2_page->height * filter_state->jbig2_page->stride))
            {
              bytes_to_copy = ((filter_state->jbig2_page->height * filter_state->jbig2_page->stride)
                               - filter_state->index);
            }
          
          memcpy (out->data + out->wp,
                  filter_state->jbig2_page->data + filter_state->index,
                  bytes_to_copy);
          
          out->wp += bytes_to_copy;
          filter_state->index += bytes_to_copy;

          /* Determine the output state */
          if (filter_state->index ==
              (filter_state->jbig2_page->height * filter_state->jbig2_page->stride))
            {
              jbig2_release_page(filter_state->jbig2_context,
                                 filter_state->jbig2_page);
              ret = PDF_EEOF;
            }
          else
            {
              ret = PDF_ENOUTPUT;
            }
        }
      else
        {
          ret = PDF_EEOF;
        }
    }
  else
    {
      /* Write input into the decoder */
      bytes_to_copy = (in->wp - in->rp);
      jbig2_data_in (filter_state->jbig2_context,
                     in->data + in->rp,
                     bytes_to_copy);
      if (filter_state->error_p == PDF_TRUE)
        {
          return PDF_ERROR;
        }

      in->rp += bytes_to_copy;

      ret = PDF_ENINPUT;
    }

  return ret;
}

pdf_status_t
pdf_stm_f_jbig2dec_dealloc_state (void *state)
{
  pdf_stm_f_jbig2dec_t filter_state;
  filter_state = (pdf_stm_f_jbig2dec_t) state;

  jbig2_ctx_free (filter_state->jbig2_context);
  pdf_dealloc (filter_state);
  
  return PDF_OK;
}

/*
 * Private functions
 */

static int 
jbig2dec_error_cb (void *data,
                   const char *msg,
                   Jbig2Severity severity,
                   int32_t seg_idx)
{
  pdf_stm_f_jbig2dec_t filter_state;

  filter_state = (pdf_stm_f_jbig2dec_t) data;

  if (severity == JBIG2_SEVERITY_FATAL)
    {
      filter_state->error_p = PDF_TRUE;
    }

  return 0;
}

/* End of pdf-stm-f-jbig2.c */

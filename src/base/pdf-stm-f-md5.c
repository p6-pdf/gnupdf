/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-md5.c
 *       Date:         Fri Dec  5 16:40:50 2008
 *
 *       GNU PDF Library - Message-digest stream filter
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

#include <pdf-types.h>
#include <pdf-types-buffer.h>
#include <pdf-hash.h>
#include <pdf-alloc.h>
#include <pdf-crypt.h>
#include <pdf-stm-f-md5.h>

#define MD5_OUTPUT_SIZE 16

/* Internal state */
struct pdf_stm_f_md5_s
{
  pdf_crypt_md_t md;
  pdf_buffer_t *cache;
};

static pdf_bool_t stm_f_md5_init (pdf_hash_t   *params,
                                  void        **state,
                                  pdf_error_t **error);

static void stm_f_md5_deinit (void *state);

static enum pdf_stm_filter_apply_status_e stm_f_md5enc_apply (pdf_hash_t    *params,
                                                              void          *state,
                                                              pdf_buffer_t  *in,
                                                              pdf_buffer_t  *out,
                                                              pdf_bool_t     finish,
                                                              pdf_error_t  **error);

/* Filter implementations */

static const pdf_stm_filter_impl_t enc_impl = {
  .init_fn   = stm_f_md5_init,
  .apply_fn  = stm_f_md5enc_apply,
  .deinit_fn = stm_f_md5_deinit,
};

const pdf_stm_filter_impl_t *
pdf_stm_f_md5enc_get (void)
{
  return &enc_impl;
}

static pdf_bool_t
stm_f_md5_init (pdf_hash_t   *params,
                void        **state,
                pdf_error_t **error)
{
  struct pdf_stm_f_md5_s *filter_state;

  filter_state = pdf_alloc (sizeof (struct pdf_stm_f_md5_s));
  if (!filter_state)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "cannot create V2 encoder/decoder internal state: "
                     "couldn't allocate %lu bytes",
                     (unsigned long)sizeof (struct pdf_stm_f_md5_s));
      return PDF_FALSE;
    }

  /* Initialize internal buffer */
  filter_state->cache = pdf_buffer_new (MD5_OUTPUT_SIZE, error);
  if (!(filter_state->cache))
    {
      stm_f_md5_deinit (filter_state);
      return PDF_FALSE;
    }

  if (pdf_crypt_md_new (PDF_CRYPT_MD_MD5,
                        &(filter_state->md)) != PDF_OK)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_EBADDATA,
                     "cannot initialize MD5 encoder: "
                     "couldn't setup cipher");
      stm_f_md5_deinit (filter_state);
      return PDF_FALSE;
    }

  *state = filter_state;
  return PDF_TRUE;
}

static void
stm_f_md5_deinit (void *state)
{
  struct pdf_stm_f_md5_s *filter_state = state;

  if (filter_state->md)
    pdf_crypt_md_destroy (filter_state->md);
  if (filter_state->cache)
    pdf_buffer_destroy (filter_state->cache);
  pdf_dealloc (state);
}

static enum pdf_stm_filter_apply_status_e
stm_f_md5enc_apply (pdf_hash_t    *params,
                    void          *state,
                    pdf_buffer_t  *in,
                    pdf_buffer_t  *out,
                    pdf_bool_t     finish,
                    pdf_error_t  **error)
{
  struct pdf_stm_f_md5_s *filter_state = state;
  pdf_buffer_t *cache = filter_state->cache;
  pdf_size_t in_size;
  pdf_size_t bytes_to_write;
  pdf_size_t cache_size;
  pdf_size_t out_size;

  PDF_ASSERT (in->wp >= in->rp);
  in_size = in->wp - in->rp;

  pdf_crypt_md_write (filter_state->md, (pdf_char_t *)in->data, in_size);
  in->rp += in_size;

  if (!finish)
    return PDF_STM_FILTER_APPLY_STATUS_NO_INPUT;

  if (pdf_buffer_eob_p (cache))
    {
      /* If we have reached the end, read the hash value in cache */
      pdf_crypt_md_read (filter_state->md, (pdf_char_t *)cache->data, cache->size);
      cache->wp = cache->size;
    }

  PDF_ASSERT (out->size >= out->wp);
  PDF_ASSERT (cache->wp >= cache->rp);

  out_size = out->size - out->wp;
  cache_size = cache->wp - cache->rp;
  bytes_to_write = PDF_MIN (out_size, cache_size);

  memcpy (out->data, cache->data + cache->rp, bytes_to_write);

  cache->rp += bytes_to_write;
  out->wp   += bytes_to_write;

  if (out_size >= cache_size)
    return PDF_STM_FILTER_APPLY_STATUS_EOF;

  return PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT;
}

/* End of pdf_stm_f_md5.c */

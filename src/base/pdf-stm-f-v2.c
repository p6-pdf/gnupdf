/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-v2.c
 *       Date:         Tue Jul 10 23:44:00 2007
 *
 *       GNU PDF Library - V2 stream filter
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

#include <pdf-types.h>
#include <pdf-types-buffer.h>
#include <pdf-hash.h>
#include <pdf-alloc.h>
#include <pdf-crypt.h>
#include <pdf-stm-f-v2.h>
#include <pdf-hash-helper.h>

/* Encryption and decryption  */
typedef enum {
  PDF_STM_F_V2_MODE_ENCODE,
  PDF_STM_F_V2_MODE_DECODE
} pdf_stm_f_v2_mode_e;

/* Internal state */
struct pdf_stm_f_v2_s
{
  pdf_crypt_cipher_t cipher;
};

static pdf_bool_t stm_f_v2_init (const pdf_hash_t  *params,
                                 void             **state,
                                 pdf_error_t      **error);

static void stm_f_v2_deinit (void *state);

static enum pdf_stm_filter_apply_status_e stm_f_v2enc_apply (void          *state,
                                                             pdf_buffer_t  *in,
                                                             pdf_buffer_t  *out,
                                                             pdf_bool_t     finish,
                                                             pdf_error_t  **error);

static enum pdf_stm_filter_apply_status_e stm_f_v2dec_apply (void          *state,
                                                             pdf_buffer_t  *in,
                                                             pdf_buffer_t  *out,
                                                             pdf_bool_t     finish,
                                                             pdf_error_t  **error);
/* Filter implementations */

static const pdf_stm_filter_impl_t enc_impl = {
  .init_fn   = stm_f_v2_init,
  .apply_fn  = stm_f_v2enc_apply,
  .deinit_fn = stm_f_v2_deinit,
};

static const pdf_stm_filter_impl_t dec_impl = {
  .init_fn   = stm_f_v2_init,
  .apply_fn  = stm_f_v2dec_apply,
  .deinit_fn = stm_f_v2_deinit,
};

const pdf_stm_filter_impl_t *
pdf_stm_f_v2enc_get (void)
{
  return &enc_impl;
}

const pdf_stm_filter_impl_t *
pdf_stm_f_v2dec_get (void)
{
  return &dec_impl;
}

/* Common implementation */

static pdf_bool_t
stm_f_v2_init (const pdf_hash_t  *params,
               void             **state,
               pdf_error_t      **error)
{
  struct pdf_stm_f_v2_s *filter_state;
  const pdf_char_t *key;
  pdf_size_t keysize;

  /* We demand all parameters are present */
  if (!params ||
      !pdf_hash_key_p (params, "Key") ||
      !pdf_hash_key_p (params, "KeySize"))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_EBADDATA,
                     "cannot initialize V2 encoder/decoder: "
                     "parameters missing ('Key': %s, 'KeySize': %s)",
                     (params && pdf_hash_key_p (params, "Key")) ? "available" : "missing",
                     (params && pdf_hash_key_p (params, "KeySize")) ? "available" : "missing");
      return PDF_FALSE;
    }

  filter_state = pdf_alloc (sizeof (struct pdf_stm_f_v2_s));
  if (!filter_state)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "cannot create V2 encoder/decoder internal state: "
                     "couldn't allocate %lu bytes",
                     (unsigned long)sizeof (struct pdf_stm_f_v2_s));
      return PDF_FALSE;
    }

  filter_state->cipher = NULL;

  /* Note that Key may NOT be NUL-terminated */
  key = pdf_hash_get_value (params, "Key");
  keysize = pdf_hash_get_size (params, "KeySize");

  if (pdf_crypt_cipher_new (PDF_CRYPT_CIPHER_ALGO_V2,
                            &(filter_state->cipher)) != PDF_OK ||
      pdf_crypt_cipher_setkey (filter_state->cipher, (pdf_char_t *)key, keysize) != PDF_OK)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_EBADDATA,
                     "cannot initialize V2 encoder/decoder: "
                     "couldn't setup cipher");
      stm_f_v2_deinit (filter_state);
      return PDF_FALSE;
    }

  *state = filter_state;
  return PDF_TRUE;
}

static void
stm_f_v2_deinit (void *state)
{
  struct pdf_stm_f_v2_s *filter_state = state;

  if (filter_state->cipher)
    pdf_crypt_cipher_destroy (filter_state->cipher);
  pdf_dealloc (state);
}

static enum pdf_stm_filter_apply_status_e
stm_f_v2_apply (pdf_stm_f_v2_mode_e   mode,
                void                 *state,
                pdf_buffer_t         *in,
                pdf_buffer_t         *out,
                pdf_bool_t            finish,
                pdf_error_t         **error)
{
  struct pdf_stm_f_v2_s *filter_state = state;
  pdf_size_t in_size;
  pdf_size_t out_size;
  pdf_size_t bytes_to_copy;
  pdf_size_t written;

  PDF_ASSERT (in->wp >= in->rp);
  PDF_ASSERT (out->size >= out->wp);

  /* TODO: Shouldn't this be like this?
   *   in_size = in->size - in->rp;
   */
  in_size = in->wp - in->rp;
  out_size = out->size - out->wp;

  bytes_to_copy = PDF_MIN (out_size, in_size);

  if (bytes_to_copy != 0)
    {
      switch (mode)
        {
        case PDF_STM_F_V2_MODE_ENCODE:
          {
            pdf_crypt_cipher_encrypt (filter_state->cipher,
                                      (pdf_char_t *)out->data,
                                      out_size,
                                      (pdf_char_t *)in->data,
                                      in_size,
                                      &written);
            break;
          }
        case PDF_STM_F_V2_MODE_DECODE:
          {
            pdf_crypt_cipher_decrypt (filter_state->cipher,
                                      (pdf_char_t *)out->data,
                                      out_size,
                                      (pdf_char_t *)in->data,
                                      in_size,
                                      &written);
            break;
          }
        }

      in->rp  += bytes_to_copy;
      out->wp += written;
    }

  return (in_size > out_size ?
          PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT :
          PDF_STM_FILTER_APPLY_STATUS_NO_INPUT);
}

/* Encode filter */

static enum pdf_stm_filter_apply_status_e
stm_f_v2enc_apply (void          *state,
                   pdf_buffer_t  *in,
                   pdf_buffer_t  *out,
                   pdf_bool_t     finish,
                   pdf_error_t  **error)
{
  return stm_f_v2_apply (PDF_STM_F_V2_MODE_ENCODE,
                         state,
                         in,
                         out,
                         finish,
                         error);
}

/* Decode filter  */

static enum pdf_stm_filter_apply_status_e
stm_f_v2dec_apply (void          *state,
                   pdf_buffer_t  *in,
                   pdf_buffer_t  *out,
                   pdf_bool_t     finish,
                   pdf_error_t  **error)
{
  return stm_f_v2_apply (PDF_STM_F_V2_MODE_DECODE,
                         state,
                         in,
                         out,
                         finish,
                         error);
}

/* End of pdf_stm_f_v2.c */

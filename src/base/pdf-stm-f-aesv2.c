/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-aesv2.c
 *       Date:         Sun Dec 14 20:13:53 2008
 *
 *       GNU PDF Library - AESV2 stream filter
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

#include <stdlib.h>
#include <string.h>

#include <pdf-types.h>
#include <pdf-types-buffer.h>
#include <pdf-hash.h>
#include <pdf-alloc.h>
#include <pdf-crypt.h>
#include <pdf-stm-f-aesv2.h>
#include <pdf-hash-helper.h>

/* Define AESv2 encoder */
PDF_STM_FILTER_DEFINE (pdf_stm_f_aesv2enc_get,
                       stm_f_aesv2_init,
                       stm_f_aesv2enc_apply,
                       stm_f_aesv2_deinit);

/* Define AESv2 decoder */
PDF_STM_FILTER_DEFINE (pdf_stm_f_aesv2dec_get,
                       stm_f_aesv2_init,
                       stm_f_aesv2dec_apply,
                       stm_f_aesv2_deinit);

#define AESV2_CACHE_SIZE     16
#define AESv2_PARAM_KEY      "Key"
#define AESv2_PARAM_KEY_SIZE "KeySize"

/* Encryption and decryption  */
typedef enum {
    PDF_STM_F_AESV2_MODE_ENCODE,
    PDF_STM_F_AESV2_MODE_DECODE
} pdf_stm_f_aesv2_mode_e;

/* Internal state */
struct pdf_stm_f_aesv2_s
{
  pdf_crypt_cipher_t cipher;

  pdf_buffer_t *in_cache;
  pdf_buffer_t *out_cache;

  pdf_char_t *key;
  pdf_size_t keysize;
};

/* Common implementation */

static pdf_bool_t
stm_f_aesv2_init (const pdf_hash_t  *params,
                  void             **state,
                  pdf_error_t      **error)
{
  struct pdf_stm_f_aesv2_s *filter_state;
  const pdf_char_t *key;
  pdf_size_t keysize;

  /* We demand all parameters are present */
  if (!params ||
      !pdf_hash_key_p (params, AESv2_PARAM_KEY) ||
      !pdf_hash_key_p (params, AESv2_PARAM_KEY))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_EBADDATA,
                     "cannot initialize AESv2 encoder/decoder: "
                     "parameters missing ('Key': %s, 'KeySize': %s)",
                     ((params && pdf_hash_key_p (params, AESv2_PARAM_KEY)) ?
                      "available" : "missing"),
                     ((params && pdf_hash_key_p (params, AESv2_PARAM_KEY_SIZE)) ?
                      "available" : "missing"));
      return PDF_FALSE;
    }

  filter_state = pdf_alloc (sizeof (struct pdf_stm_f_aesv2_s));
  if (!filter_state)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "cannot create AESv2 encoder/decoder internal state: "
                     "couldn't allocate %lu bytes",
                     (unsigned long)sizeof (struct pdf_stm_f_aesv2_s));
      return PDF_FALSE;
    }

  /* Initialize cache buffers */
  filter_state->in_cache = pdf_buffer_new (AESV2_CACHE_SIZE, error);
  if (!(filter_state->in_cache))
    {
      stm_f_aesv2_deinit (filter_state);
      return PDF_FALSE;
    }
  filter_state->out_cache = pdf_buffer_new (AESV2_CACHE_SIZE, error);
  if (!(filter_state->out_cache))
    {
      stm_f_aesv2_deinit (filter_state);
      return PDF_FALSE;
    }

  /* Note that Key may NOT be NUL-terminated */
  key = pdf_hash_get_value (params, AESv2_PARAM_KEY);
  keysize = pdf_hash_get_size (params, AESv2_PARAM_KEY_SIZE);

  /* Keep a copy of the key in the filter */
  filter_state->key = (pdf_char_t *)pdf_alloc (keysize);
  if (!filter_state->key)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "cannot copy AESv2 key: "
                     "couldn't allocate %lu bytes",
                     (unsigned long)keysize);
      stm_f_aesv2_deinit (filter_state);
      return PDF_FALSE;
    }
  filter_state->keysize = keysize;
  memcpy (filter_state->key, key, keysize);

  filter_state->cipher = NULL;
  if (pdf_crypt_cipher_new (PDF_CRYPT_CIPHER_ALGO_AESV2,
                            &(filter_state->cipher)) != PDF_OK ||
      pdf_crypt_cipher_setkey (filter_state->cipher,
                               filter_state->key,
                               filter_state->keysize) != PDF_OK)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_EBADDATA,
                     "cannot initialize AESv2 encoder/decoder: "
                     "couldn't setup cipher");
      stm_f_aesv2_deinit (filter_state);
      return PDF_FALSE;
    }

  *state = filter_state;
  return PDF_TRUE;
}

static void
stm_f_aesv2_deinit (void *state)
{
  struct pdf_stm_f_aesv2_s *filter_state = state;

  if (filter_state->cipher)
    pdf_crypt_cipher_destroy (filter_state->cipher);
  if (filter_state->in_cache)
    pdf_buffer_destroy (filter_state->in_cache);
  if (filter_state->out_cache)
    pdf_buffer_destroy (filter_state->out_cache);
  if (filter_state->key)
    pdf_dealloc (filter_state->key);
  pdf_dealloc (state);
}

static enum pdf_stm_filter_apply_status_e
stm_f_aesv2_apply (pdf_stm_f_aesv2_mode_e   mode,
                   void                    *state,
                   pdf_buffer_t            *in,
                   pdf_buffer_t            *out,
                   pdf_bool_t               finish,
                   pdf_error_t            **error)
{
  struct pdf_stm_f_aesv2_s *filter_state = state;
  pdf_crypt_cipher_t cipher = filter_state->cipher;
  pdf_buffer_t *in_cache = filter_state->in_cache;
  pdf_buffer_t *out_cache = filter_state->out_cache;

  while (PDF_TRUE)
    {
      pdf_size_t in_size;
      pdf_size_t out_size;
      pdf_size_t in_cache_size;
      pdf_size_t out_cache_size;
      pdf_size_t bytes_to_read;
      pdf_size_t bytes_to_write;

      PDF_ASSERT (in->wp >= in->rp);
      PDF_ASSERT (out->size >= out->wp);

      /* TODO: Shouldn't this be like this?
       *   in_size = in->size - in->rp;
       */
      in_size = in->wp - in->rp;
      in_cache_size = in_cache->size - in_cache->wp;
      bytes_to_read = PDF_MIN (in_size, in_cache_size);

      /* Read bytes from IN and fill IN_CACHE */
      memcpy (in_cache->data + in_cache->wp,
              in->data       + in->rp,
              bytes_to_read);

      in_cache->wp += bytes_to_read;
      in->rp       += bytes_to_read;

      /* If we cannot fill all IN_CACHE... */
      if (!pdf_buffer_full_p (in_cache))
        {
          if (finish &&
              mode == PDF_STM_F_AESV2_MODE_DECODE &&
              in_cache->wp > 0)
            {
              /* TODO: Better error explanation */
              pdf_set_error (error,
                             PDF_EDOMAIN_BASE_STM,
                             PDF_ERROR,
                             "");
              return PDF_STM_FILTER_APPLY_STATUS_ERROR;
            }

          /* ...pad the cache if we have reached EOD */
          if (finish &&
              mode == PDF_STM_F_AESV2_MODE_ENCODE &&
              !pdf_buffer_full_p (in_cache))
            {
              pdf_size_t padding;

              padding = in_cache->size - in_cache->wp;
              memset (in_cache->data + in_cache->wp,
                      padding,
                      padding);

              in_cache->wp += padding;
            }
          else
            {
              if (pdf_buffer_eob_p (out_cache))
                return PDF_STM_FILTER_APPLY_STATUS_NO_INPUT; /* ...ask more input */
            }
        }

      /* If OUT_CACHE is empty and IN_CACHE is full, then it is ready
       * to be processed. */
      if (pdf_buffer_full_p (in_cache) &&
          pdf_buffer_eob_p (out_cache))
        {
          switch (mode)
            {
            case PDF_STM_F_AESV2_MODE_ENCODE:
              pdf_crypt_cipher_encrypt (cipher,
                                        (pdf_char_t *)out_cache->data,
                                        out_cache->size,
                                        (pdf_char_t *)in_cache->data,
                                        in_cache->size,
                                        NULL);
              break;

            case PDF_STM_F_AESV2_MODE_DECODE:
              pdf_crypt_cipher_decrypt (cipher,
                                        (pdf_char_t *)out_cache->data,
                                        out_cache->size,
                                        (pdf_char_t *)in_cache->data,
                                        in_cache->size,
                                        NULL);
              break;

            }

          /* Both cache are full now */
          pdf_buffer_rewind (in_cache);
          out_cache->wp = out_cache->size;
        }

      /* When we are decrypting data, we need know what block is the
       * last. If both IN and IN_CACHE are empty, then OUT_CACHE could
       * hold it. So, we ask more input to we make sure.*/
      if (mode == PDF_STM_F_AESV2_MODE_DECODE &&
          pdf_buffer_eob_p (in) &&
          pdf_buffer_eob_p (in_cache))
        {
          /* When we know it is the last, remove the padding. */
          if (finish)
            {
              pdf_size_t padding;

              padding = out_cache->data[out_cache->size - 1];
              if (padding > AESV2_CACHE_SIZE)
                {
                  pdf_set_error (error,
                                 PDF_EDOMAIN_BASE_STM,
                                 PDF_ERROR,
                                 "Padding longer than AESv2 cache (%lu > %lu)",
                                 (unsigned long)padding,
                                 (unsigned long) AESV2_CACHE_SIZE);
                  return PDF_STM_FILTER_APPLY_STATUS_ERROR;
                }

              out_cache->wp = out_cache->size - padding;
            }
          else /* Ask for more input */
            return PDF_STM_FILTER_APPLY_STATUS_NO_INPUT;
        }

      /* Finally, we fill the OUT buffer */
      out_size = out->size - out->wp;
      out_cache_size = out_cache->wp - out_cache->rp;
      bytes_to_write = PDF_MIN (out_size, out_cache_size);

      memcpy (out->data + out->wp,
              out_cache->data + out_cache->rp,
              bytes_to_write);

      out_cache->rp += bytes_to_write;
      out->wp       += bytes_to_write;

      if (finish)
        return PDF_STM_FILTER_APPLY_STATUS_EOF;

      if (pdf_buffer_full_p (out))
        return PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT;

      /* Continue loop */
      pdf_buffer_rewind (out_cache);
    }
}

/* Encode filter */

static enum pdf_stm_filter_apply_status_e
stm_f_aesv2enc_apply (void          *state,
                      pdf_buffer_t  *in,
                      pdf_buffer_t  *out,
                      pdf_bool_t     finish,
                      pdf_error_t  **error)
{
  return stm_f_aesv2_apply (PDF_STM_F_AESV2_MODE_ENCODE,
                            state,
                            in,
                            out,
                            finish,
                            error);
}

/* Decode filter  */

static enum pdf_stm_filter_apply_status_e
stm_f_aesv2dec_apply (void          *state,
                      pdf_buffer_t  *in,
                      pdf_buffer_t  *out,
                      pdf_bool_t     finish,
                      pdf_error_t  **error)
{
  return stm_f_aesv2_apply (PDF_STM_F_AESV2_MODE_DECODE,
                            state,
                            in,
                            out,
                            finish,
                            error);
}

/* End of pdf_stm_f_aesv2.c */

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

#include <pdf-stm-f-aesv2.h>
#include <pdf-hash-helper.h>

#define AESV2_CACHE_SIZE 16


/* Internal state */
struct pdf_stm_f_aesv2_s
{
  pdf_crypt_cipher_t cipher;
  pdf_buffer_t *in_cache;
  pdf_buffer_t *out_cache;
};
typedef struct pdf_stm_f_aesv2_s * pdf_stm_f_aesv2_t;


/* Encryption and decryption  */
enum pdf_stm_f_aesv2_mode_e
  {
    PDF_STM_F_AESV2_MODE_ENCODE,
    PDF_STM_F_AESV2_MODE_DECODE
  };
typedef enum pdf_stm_f_aesv2_mode_e pdf_stm_f_aesv2_mode_t;



static inline pdf_status_t
pdf_stm_f_aesv2_init (pdf_hash_t  *params,
                      void       **state)
{
  pdf_status_t ret;
  pdf_stm_f_aesv2_t filter_state;

  filter_state = pdf_alloc (sizeof (struct pdf_stm_f_aesv2_s));

  if (filter_state == NULL)
    {
      ret = PDF_ENOMEM;
    }
  else if (state == NULL)
    {
      pdf_dealloc (filter_state);
      ret = PDF_EBADDATA;
    }
  else
    {
      /* We demand all parameters are present */
      if ((pdf_hash_key_p (params, "Key")) &&
          (pdf_hash_key_p (params, "KeySize")))
        {
          const pdf_char_t *key;
          pdf_size_t keysize;
          pdf_crypt_cipher_t cipher;

          /* Key may NOT be NUL-terminated */
          key = pdf_hash_get_value (params, "Key");
          keysize = pdf_hash_get_size (params, "KeySize");

          ret = pdf_crypt_cipher_new (PDF_CRYPT_CIPHER_ALGO_AESV2, &cipher);
          if (ret == PDF_OK)
            {
              ret = pdf_crypt_cipher_setkey (cipher, key, keysize);
              if (ret == PDF_OK)
                {
                  filter_state->cipher = cipher;

                  /* Initialize cache buffers */
                  filter_state->in_cache  = pdf_buffer_new (AESV2_CACHE_SIZE, NULL);
                  filter_state->out_cache = pdf_buffer_new (AESV2_CACHE_SIZE, NULL);
                  /* TODO: get and propagate errors */

                  if (filter_state->in_cache == NULL || filter_state->out_cache == NULL)
                    {
                      ret = PDF_ERROR;
                    }
                  else
                    {
                      ret = PDF_OK;
                      *state = filter_state;
                    }
                }
            }
        }
      else
        {
          ret = PDF_EBADDATA;
        }

      if (ret != PDF_OK)
        {
          pdf_dealloc (filter_state);
        }
    }

  return ret;
}




static inline pdf_status_t
pdf_stm_f_aesv2_apply (pdf_stm_f_aesv2_mode_t  mode,
                       pdf_hash_t             *params,
                       void                   *state,
                       pdf_buffer_t           *in,
                       pdf_buffer_t           *out,
                       pdf_bool_t              finish_p)
{
  pdf_stm_f_aesv2_t filter_state = state;
  pdf_crypt_cipher_t cipher   = filter_state->cipher;
  pdf_buffer_t *in_cache   = filter_state->in_cache;
  pdf_buffer_t *out_cache  = filter_state->out_cache;

  while(1)
    {
      pdf_size_t in_size;
      pdf_size_t out_size;
      pdf_size_t in_cache_size;
      pdf_size_t out_cache_size;
      pdf_size_t bytes_to_read;
      pdf_size_t bytes_to_write;

      /* Read bytes from IN and fill IN_CACHE*/
      in_size = in->wp - in->rp;
      in_cache_size = in_cache->size - in_cache->wp;
      bytes_to_read = PDF_MIN (in_size, in_cache_size);

      memcpy (in_cache->data + in_cache->wp,
              in->data       + in->rp,
              bytes_to_read);

      in_cache->wp += bytes_to_read;
      in->rp       += bytes_to_read;


      /* If we cannot fill all IN_CACHE...  */
      if (!pdf_buffer_full_p (in_cache))
        {
          if (finish_p && mode == PDF_STM_F_AESV2_MODE_DECODE
              && in_cache->wp > 0)
            return PDF_ERROR;

          /* ...pad the cache if we have reached EOD */
          if (finish_p
              && !pdf_buffer_full_p (in_cache)
              && mode == PDF_STM_F_AESV2_MODE_ENCODE)
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
                return PDF_ENINPUT; /* ...ask more input */
            }
        }


      /* If OUT_CACHE is empty and IN_CACHE is full, then it is ready
         to be processed. */
      if (pdf_buffer_full_p (in_cache) && pdf_buffer_eob_p (out_cache))
        {
          switch (mode)
            {
            case PDF_STM_F_AESV2_MODE_ENCODE:
              pdf_crypt_cipher_encrypt (cipher,
                                        out_cache->data,
                                        out_cache->size,
                                        in_cache->data,
                                        in_cache->size,
                                        NULL);
              break;

            case PDF_STM_F_AESV2_MODE_DECODE:
              pdf_crypt_cipher_decrypt (cipher,
                                        out_cache->data,
                                        out_cache->size,
                                        in_cache->data,
                                        in_cache->size,
                                        NULL);
              break;

            default: /* not reached */
              break;
            }

          /* Both cache are full now */
          pdf_buffer_rewind (in_cache);
          out_cache->wp = out_cache->size;
        }


      /* When we are decrypting data, we need know what block is the
         last. If both IN and IN_CACHE are empty, then OUT_CACHE could
         hold it. So, we ask more input to we make sure.*/
      if (mode == PDF_STM_F_AESV2_MODE_DECODE
          && pdf_buffer_eob_p (in) && pdf_buffer_eob_p (in_cache))
        {
          /* When we know it is the last, remove the padding. */
          if (finish_p)
            {
              pdf_size_t padding;
              padding = out_cache->data[out_cache->size - 1];

              if (padding > AESV2_CACHE_SIZE)
                return PDF_ERROR;

              out_cache->wp = out_cache->size - padding;
            }
          else
            return PDF_ENINPUT;
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


      if (finish_p)
        {
          return PDF_EEOF;
        }
      else if (pdf_buffer_full_p (out))
        {
          return PDF_ENOUTPUT;
        }
      else
        {
          pdf_buffer_rewind (out_cache);
        }
    }
}



static inline pdf_status_t
pdf_stm_f_aesv2_dealloc_state (void *state)
{
  pdf_stm_f_aesv2_t filter_state = state;
  pdf_crypt_cipher_destroy (filter_state->cipher);
  pdf_buffer_destroy (filter_state->in_cache);
  pdf_buffer_destroy (filter_state->out_cache);
  pdf_dealloc (state);
  return PDF_OK;
}




/* Encode filter */

pdf_status_t
pdf_stm_f_aesv2enc_init (pdf_hash_t  *params,
                         void       **state)
{
  return pdf_stm_f_aesv2_init (params, state);
}

pdf_status_t
pdf_stm_f_aesv2enc_apply (pdf_hash_t   *params,
                          void         *state,
                          pdf_buffer_t *in,
                          pdf_buffer_t *out,
                          pdf_bool_t    finish_p)
{
  return pdf_stm_f_aesv2_apply (PDF_STM_F_AESV2_MODE_ENCODE,
                                params, state, in, out, finish_p);
}

pdf_status_t
pdf_stm_f_aesv2enc_dealloc_state (void *state)
{
  return pdf_stm_f_aesv2_dealloc_state (state);
}



/* Decode filter  */

pdf_status_t
pdf_stm_f_aesv2dec_init (pdf_hash_t  *params,
                         void       **state)
{
  return pdf_stm_f_aesv2_init (params, state);
}


pdf_status_t
pdf_stm_f_aesv2dec_apply (pdf_hash_t   *params,
                          void         *state,
                          pdf_buffer_t *in,
                          pdf_buffer_t *out,
                          pdf_bool_t    finish_p)
{
  return pdf_stm_f_aesv2_apply (PDF_STM_F_AESV2_MODE_DECODE,
                                params, state, in, out, finish_p);
}


pdf_status_t
pdf_stm_f_aesv2dec_dealloc_state (void *state)
{
  return pdf_stm_f_aesv2_dealloc_state (state);
}



/* End of pdf_stm_f_aesv2.c */

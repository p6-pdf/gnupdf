/* -*- mode: C -*- Time-stamp: "2008-12-21 14:17:04 davazp"
 *
 *       File:         pdf-stm-f-aesv2.c
 *       Date:         Sun Dec 14 20:13:53 2008
 *
 *       GNU PDF Library - AESV2 stream filter
 *
 */

/* Copyright (C) 2007, 2008 Free Software Foundation, Inc. */

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

#include <pdf-stm-f-aesv2.h>
#include <stdlib.h>
#include <string.h>

#define AESV2_CACHE_SIZE 1024


/* Internal state */
struct pdf_stm_f_aesv2_s
{
  pdf_crypt_cipher_t cipher;
  pdf_stm_buffer_t cache;
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
pdf_stm_f_aesv2_init (pdf_hash_t params, void **state)
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
      pdf_char_t * key;
      pdf_size_t * keysize;
      pdf_crypt_cipher_t cipher;
      
      /* We demand all parameters are present */
      if ((( pdf_hash_key_p (params, "Key")     == PDF_TRUE))
          && pdf_hash_key_p (params, "KeySize") == PDF_TRUE)
        {
          pdf_hash_search (params, "Key",       (const void **)&key);
          pdf_hash_search (params, "KeySize",   (const void **)&keysize);

          if (pdf_crypt_cipher_new (PDF_CRYPT_CIPHER_ALGO_AESV2, &cipher) == PDF_OK
              && pdf_crypt_cipher_setkey (cipher, key, *keysize) == PDF_OK)
            {
              filter_state->cipher = cipher;

              /* Initialize the cache buffer */
              filter_state->cache = pdf_stm_buffer_new (AESV2_CACHE_SIZE);
              if (filter_state->cache == NULL)
                {
                  ret = PDF_ERROR;
                }
              else
                {
                  ret = PDF_OK;
                  *state = filter_state;
                }
                
            }
          else
            {
              pdf_dealloc (filter_state);
              ret = PDF_EBADDATA;
            }
        }
      else
        {
          pdf_dealloc (filter_state);
          ret = PDF_EBADDATA;
        }
    }
  
  return ret;
}



static inline pdf_status_t
pdf_stm_f_aesv2_apply (pdf_stm_f_aesv2_mode_t mode,
                       pdf_hash_t params, void *state, pdf_stm_buffer_t in,
                       pdf_stm_buffer_t out, pdf_bool_t finish_p)
{
  pdf_stm_f_aesv2_t filter_state = state;
  pdf_stm_buffer_t cache = filter_state->cache;
  pdf_size_t in_size;
  
  in_size = in->wp - in->rp;
  
  /* We need operate on whole buffer in a single function
     call. So, we must to cache all input before. */
  if (in_size != 0)
    {
      /* if CACHE is not too large, we must resize it in order to we
         can append the IN buffer. */
      if ((cache->size - cache->wp) < in_size)
        {
          if (pdf_stm_buffer_resize (cache, cache->size + in_size) != PDF_OK)
            return PDF_ERROR;
        }
      
      memcpy (cache->data + cache->wp, in->data, in_size);

      cache->wp += in_size;
      in->rp    += in_size;
    }


  if (finish_p == PDF_TRUE)
    {
      /* Once we have collected all input, then we can encrypt/decrypt. */
      pdf_size_t cache_size;
      pdf_size_t out_size;
      pdf_size_t written;

      cache_size  = cache->wp - cache->rp;
      out_size = out->size - out->wp;

      if (cache_size == 0)
        return PDF_ENINPUT;


      if (mode == PDF_STM_F_AESV2_MODE_ENCODE) /* encoding data */
        {
          pdf_size_t aux_size;
          aux_size = pdf_crypt_cipher_encrypt_size (filter_state->cipher, cache->data, cache_size);

          if (aux_size > out_size)
            {
              /* We should can return PDF_ENOUTPUT. However the
                 output buffer is assumed to be full if we return
                 PDF_ENOUTPUT, but we can not write it partially.  */
              return PDF_ERROR;
            }
          else
            {
              pdf_crypt_cipher_encrypt (filter_state->cipher,
                                        out->data,
                                        out_size,
                                        cache->data,
                                        cache_size,
                                        &written);
            }
        }
      else if (mode == PDF_STM_F_AESV2_MODE_DECODE) /* decoding data */
        {
          pdf_size_t aux_size;
          aux_size = pdf_crypt_cipher_decrypt_size (filter_state->cipher, cache->data, cache_size);

          if (aux_size > out_size)
            {
              return PDF_ERROR;
            }
          else
            {
              pdf_crypt_cipher_decrypt (filter_state->cipher,
                                        out->data,
                                        out_size,
                                        cache->data,
                                        cache_size,
                                        &written);
            }
        }

      /* Update output buffer */
      cache->rp += cache_size;
      out->wp += written;
    }

  return PDF_ENINPUT;
}



static inline pdf_status_t
pdf_stm_f_aesv2_dealloc_state (void *state)
{
  pdf_stm_f_aesv2_t filter_state = state;
  pdf_crypt_cipher_destroy (filter_state->cipher);
  pdf_stm_buffer_destroy (filter_state->cache);
  pdf_dealloc (state);
  return PDF_OK;
}




/* Encode filter */

pdf_status_t
pdf_stm_f_aesv2enc_init (pdf_hash_t params, void **state)
{
  return pdf_stm_f_aesv2_init (params, state);
}

pdf_status_t
pdf_stm_f_aesv2enc_apply (pdf_hash_t params, void *state, pdf_stm_buffer_t in,
                          pdf_stm_buffer_t out, pdf_bool_t finish_p)
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
pdf_stm_f_aesv2dec_init (pdf_hash_t params, void **state)
{
  return pdf_stm_f_aesv2_init (params, state);
}


pdf_status_t
pdf_stm_f_aesv2dec_apply (pdf_hash_t params, void *state, pdf_stm_buffer_t in,
                          pdf_stm_buffer_t out, pdf_bool_t finish_p)
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

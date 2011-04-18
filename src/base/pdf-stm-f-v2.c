/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-v2.c
 *       Date:         Tue Jul 10 23:44:00 2007
 *
 *       GNU PDF Library - V2 stream filter
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

#include <pdf-stm-f-v2.h>
#include <pdf-hash-helper.h>


/* Internal state */
struct pdf_stm_f_v2_s
{
  pdf_crypt_cipher_t cipher;
};
typedef struct pdf_stm_f_v2_s * pdf_stm_f_v2_t;


/* Encryption and decryption  */
enum pdf_stm_f_v2_mode_e
{
  PDF_STM_F_V2_MODE_ENCODE,
  PDF_STM_F_V2_MODE_DECODE
};
typedef enum pdf_stm_f_v2_mode_e pdf_stm_f_v2_mode_t;



static inline pdf_status_t
pdf_stm_f_v2_init (pdf_hash_t  *params,
                   void       **state)
{
  pdf_status_t ret;
  pdf_stm_f_v2_t filter_state;

  filter_state = pdf_alloc (sizeof (struct pdf_stm_f_v2_s));

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
          pdf_crypt_cipher_t cipher;
          const pdf_char_t *key;
          pdf_size_t keysize;

          /* Key may NOT be NUL-terminated */
          key = pdf_hash_get_value (params, "Key");
          keysize = pdf_hash_get_size (params, "KeySize");

          ret = pdf_crypt_cipher_new (PDF_CRYPT_CIPHER_ALGO_V2, &cipher);
          if (ret == PDF_OK)
            {
              ret = pdf_crypt_cipher_setkey (cipher, key, keysize);
              if (ret == PDF_OK)
                {
                  filter_state->cipher = cipher;
                  *state = filter_state;
                }
            }

          if (ret != PDF_OK)
            {
              pdf_dealloc (filter_state);
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
pdf_stm_f_v2_apply (pdf_stm_f_v2_mode_t  mode,
                    pdf_hash_t          *params,
                    void                *state,
                    pdf_buffer_t        *in,
                    pdf_buffer_t        *out,
                    pdf_bool_t           finish_p)
{
  pdf_stm_f_v2_t filter_state = state;

  pdf_size_t in_size;
  pdf_size_t out_size;
  pdf_size_t bytes_to_copy;
  pdf_size_t written;
  pdf_status_t ret;

  in_size = in->wp - in->rp;
  out_size = out->size - out->wp;

  bytes_to_copy = PDF_MIN(out_size, in_size);

  if (bytes_to_copy != 0)
    {
      if (mode == PDF_STM_F_V2_MODE_ENCODE)
        {
          pdf_crypt_cipher_encrypt (filter_state->cipher,
                                    out->data,
                                    out_size,
                                    in->data,
                                    in_size,
                                    &written);
        }
      else if (mode == PDF_STM_F_V2_MODE_DECODE)
        {
          pdf_crypt_cipher_decrypt (filter_state->cipher,
                                    out->data,
                                    out_size,
                                    in->data,
                                    in_size,
                                    &written);
        }
      else
        {
          /* This point should not be reached. */
        }

      in->rp  += bytes_to_copy;
      out->wp += written;
    }

  if (in_size > out_size)
    {
      /* We need more room in the output buffer */
      ret = PDF_ENOUTPUT;
    }
  else
    {
      /* We can process more input */
      ret = PDF_ENINPUT;
    }

  return ret;
}


static inline pdf_status_t
pdf_stm_f_v2_dealloc_state (void *state)
{
  pdf_stm_f_v2_t filter_state = state;
  pdf_crypt_cipher_destroy (filter_state->cipher);
  pdf_dealloc (state);
  return PDF_OK;
}



/* Encode filter */

pdf_status_t
pdf_stm_f_v2enc_init (pdf_hash_t  *params,
                      void       **state)
{
  return pdf_stm_f_v2_init (params, state);
}

pdf_status_t
pdf_stm_f_v2enc_apply (pdf_hash_t   *params,
                       void         *state,
                       pdf_buffer_t *in,
                       pdf_buffer_t *out,
                       pdf_bool_t    finish_p)
{
  return pdf_stm_f_v2_apply (PDF_STM_F_V2_MODE_ENCODE,
                             params, state, in, out, finish_p);
}

pdf_status_t
pdf_stm_f_v2enc_dealloc_state (void *state)
{
  return pdf_stm_f_v2_dealloc_state (state);
}


/* Decode filter  */

pdf_status_t
pdf_stm_f_v2dec_init (pdf_hash_t  *params,
                      void       **state)
{
  return pdf_stm_f_v2_init (params, state);
}


pdf_status_t
pdf_stm_f_v2dec_apply (pdf_hash_t   *params,
                       void         *state,
                       pdf_buffer_t *in,
                       pdf_buffer_t *out,
                       pdf_bool_t    finish_p)
{
  return pdf_stm_f_v2_apply (PDF_STM_F_V2_MODE_DECODE,
                             params, state, in, out, finish_p);
}


pdf_status_t
pdf_stm_f_v2dec_dealloc_state (void *state)
{
  return pdf_stm_f_v2_dealloc_state (state);
}


/* End of pdf_stm_f_v2.c */

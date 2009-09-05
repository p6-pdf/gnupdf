/* -*- mode: C -*- Time-stamp: "09/09/05 22:48:52 jemarch"
 *
 *       File:         pdf-crypt.c
 *       Date:         Fri Feb 22 21:05:05 2008
 *
 *       GNU PDF Library - AESV2 backend encryption module
 *
 */

/* Copyright (C) 2008 Free Software Foundation, Inc. */

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
#include <gcrypt.h>

#include <pdf-alloc.h>
#include <pdf-types.h>
#include <pdf-error.h>
#include <pdf-crypt-c-aesv2.h>
#include <pdf-hash-helper.h>

#define AESV2_BLKSIZE 16	/* Size of a block in AES128 */


struct aesv2_state_s
{
  gcry_cipher_hd_t cipher;
  pdf_bool_t first_block;
};

typedef struct aesv2_state_s * aesv2_state_t;
  

/* Creation and destruction of aesv2 ciphers */

pdf_status_t
pdf_crypt_cipher_aesv2_new (void ** out)
{
  aesv2_state_t state;

  state = pdf_alloc (sizeof(struct aesv2_state_s));

  if (state != NULL)
    {
      gcry_error_t err;

      err = gcry_cipher_open (&state->cipher, GCRY_CIPHER_AES128, GCRY_CIPHER_MODE_CBC, 0);
      state->first_block = PDF_TRUE;
      
      if (err == GPG_ERR_NO_ERROR)
	{
	  *out = state;
	  return PDF_OK;
	}
      else
	{
	  pdf_dealloc (state);
	  return PDF_ERROR;
	}
    }
  else
    {
      pdf_dealloc (state);
      return PDF_ENOMEM;
    }

  return PDF_OK;
}



pdf_status_t
pdf_crypt_cipher_aesv2_destroy (void * _state)
{
  aesv2_state_t state = _state;
  gcry_cipher_close (state->cipher);
  pdf_dealloc (state);
  return PDF_OK;
}



/* Encryption and decryption functions */

pdf_status_t
pdf_crypt_cipher_aesv2_setkey (void * _state,
			       pdf_char_t *key, pdf_size_t size)
{
  aesv2_state_t state = _state;
  
  if (gcry_cipher_setkey (state->cipher, key, size) == GPG_ERR_NO_ERROR)
    {
      return PDF_OK;
    }
  else
    {
      return PDF_EBADAESKEY;
    }
}



pdf_status_t
pdf_crypt_cipher_aesv2_encrypt (void * _state,
				pdf_char_t *out, pdf_size_t out_size,
				pdf_char_t *in,  pdf_size_t in_size,
				pdf_size_t *result_size)
{
  aesv2_state_t state = _state;
  pdf_char_t * output;
  pdf_size_t output_size;
  pdf_char_t * input;
  pdf_size_t input_size;
  
  if (in_size < AESV2_BLKSIZE || in_size % AESV2_BLKSIZE != 0)
    return PDF_EBADDATA;

  if (out_size < in_size)
    return PDF_EBADDATA;

  /* If we are at first block, then we have found the IV vector */
  if (state->first_block == PDF_TRUE)
    {
      pdf_char_t * iv = in;
      gcry_cipher_setiv (state->cipher, iv, AESV2_BLKSIZE);
      input = in + AESV2_BLKSIZE;
      input_size = in_size - AESV2_BLKSIZE;

      memcpy (out, iv, AESV2_BLKSIZE);

      output = out + AESV2_BLKSIZE;
      output_size = out_size - AESV2_BLKSIZE;

      state->first_block = PDF_FALSE;
    }
  else
    {
      output = out;
      output_size = out_size;
      input = in;
      input_size = in_size;
    }

  if (gcry_cipher_encrypt (state->cipher, output, output_size, input, input_size) != GPG_ERR_NO_ERROR)
    {
      return PDF_ERROR;
    }

  if (result_size != NULL)
    *result_size = in_size;

  return PDF_OK;
}




pdf_status_t
pdf_crypt_cipher_aesv2_decrypt (void * _state,
				pdf_char_t *out, pdf_size_t out_size,
				pdf_char_t *in,  pdf_size_t in_size,
				pdf_size_t *result_size)
{
  aesv2_state_t state = _state;
  pdf_char_t * output;
  pdf_size_t output_size;
  pdf_char_t * input;
  pdf_size_t input_size;
  
  if (in_size < AESV2_BLKSIZE || in_size % AESV2_BLKSIZE != 0)
    return PDF_EBADDATA;

  if (out_size < in_size)
    return PDF_EBADDATA;

  /* If we are at first block, then we have found the IV vector */
  if (state->first_block == PDF_TRUE)
    {
      pdf_char_t * iv = in;
      gcry_cipher_setiv (state->cipher, iv, AESV2_BLKSIZE);
      input = in + AESV2_BLKSIZE;
      input_size = in_size - AESV2_BLKSIZE;

      memcpy (out, iv, AESV2_BLKSIZE);

      output = out + AESV2_BLKSIZE;
      output_size = out_size - AESV2_BLKSIZE;

      state->first_block = PDF_FALSE;
    }
  else
    {
      output = out;
      output_size = out_size;
      input = in;
      input_size = in_size;
    }

  if (gcry_cipher_decrypt (state->cipher, output, output_size, input, input_size) != GPG_ERR_NO_ERROR)
    {
      return PDF_ERROR;
    }

  if (result_size != NULL)
    *result_size = in_size;

  return PDF_OK;
}





/* End of pdf-crypt-c-aesv2.c */

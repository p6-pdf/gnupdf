/* -*- mode: C -*- Time-stamp: "2008-08-25 17:48:42 davazp"
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

#include <stdlib.h>
#include <string.h>
#include <gcrypt.h>
#include <pdf-alloc.h>
#include <pdf-types.h>
#include <pdf-error.h>
#include <pdf-crypt-c-aesv2.h>

#define AESV2_BLKSIZE 16


static pdf_status_t
pdf_crypt_cipher_aesv2_new (void ** cipher)
{
  pdf_status_t status;
  gcry_cipher_hd_t * hd;

  hd = pdf_alloc (sizeof (gcry_cipher_hd_t));

  if (hd == NULL)
    {
      gcry_cipher_open (hd, GCRY_CIPHER_AES128, GCRY_CIPHER_MODE_CBC, 0);
      status = PDF_OK;
    }
  else
    status = PDF_ERROR;

  return status;
}


static pdf_status_t
pdf_crypt_cipher_aesv2_setkey (void * cipher,
			       pdf_char_t *key, pdf_size_t size)
{
  if (size == AESV2_BLKSIZE
      && gcry_cipher_setkey (cipher, key, size) != GPG_ERR_NO_ERROR)
    return PDF_EBADDATA;
  else
    return PDF_OK;
}


static pdf_size_t
pdf_crypt_cipher_aesv2_encrypt_size (void * cipher,
				     pdf_char_t *in, pdf_size_t in_size)
{
  return in_size + 2*AESV2_BLKSIZE - (in_size % AESV2_BLKSIZE);
}


static pdf_size_t
pdf_crypt_cipher_aesv2_decrypt_size (void * cipher,
				     pdf_char_t *in, pdf_size_t in_size)
{
  return in_size - AESV2_BLKSIZE;
}


static pdf_status_t
pdf_crypt_cipher_aesv2_encrypt (void * cipher,
				pdf_char_t *out, pdf_size_t out_size,
				pdf_char_t *in,  pdf_size_t in_size)
{
  pdf_char_t * padded;
  pdf_size_t padded_size;
  pdf_char_t iv[AESV2_BLKSIZE];
  pdf_size_t iv_size = sizeof(iv);

  padded_size  = pdf_crypt_cipher_aesv2_encrypt_size (cipher, in, in_size);
  padded_size -= iv_size;

  padded = pdf_alloc (padded_size);
  
  memcpy (padded, in, in_size);
  memset (padded, padded_size - in_size, padded_size - in_size);
  
  gcry_create_nonce (iv, iv_size);
  gcry_cipher_setiv (cipher, iv, iv_size);

  memcpy (out, iv, iv_size);
  gcry_cipher_encrypt (cipher,
		       out + iv_size,
		       out_size - iv_size,
		       in,
		       in_size);

  pdf_dealloc (padded);
  return PDF_OK;
}


static pdf_status_t
pdf_crypt_cipher_aesv2_decrypt (void * cipher,
				pdf_char_t *out, pdf_size_t out_size,
				pdf_char_t *in,  pdf_size_t in_size)
{
  return PDF_OK;
}


static pdf_status_t
pdf_crypt_cipher_aesv2_destroy (void * cipher)
{
  gcry_cipher_close (cipher);
  pdf_dealloc (cipher);
  return PDF_OK;
}



struct pdf_crypt_cipher_algo_s pdf_crypt_cipher_aesv2 = 
{
    pdf_crypt_cipher_aesv2_new,
    pdf_crypt_cipher_aesv2_setkey,
    pdf_crypt_cipher_aesv2_encrypt_size,
    pdf_crypt_cipher_aesv2_decrypt_size,
    pdf_crypt_cipher_aesv2_encrypt,  
    pdf_crypt_cipher_aesv2_decrypt,
    pdf_crypt_cipher_aesv2_destroy
};



/* End of pdf-crypt-c-aesv2.c */


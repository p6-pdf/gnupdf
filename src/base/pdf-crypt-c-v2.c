/* -*- mode: C -*- Time-stamp: "2008-08-31 02:44:31 davazp"
 *
 *       File:         pdf-crypt.c
 *       Date:         Fri Feb 22 21:05:05 2008
 *
 *       GNU PDF Library - V2 backend encryption module
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
#include <pdf-crypt-c-v2.h>


/* Creation and destruction of a v2 cipher */

static pdf_status_t
pdf_crypt_cipher_v2_new (void ** cipher)
{
  gcry_cipher_hd_t * hd;

  hd = pdf_alloc (sizeof (gcry_cipher_hd_t));

  if (hd != NULL)
    {
      gcry_error_t err;

      err = gcry_cipher_open (hd, GCRY_CIPHER_ARCFOUR, GCRY_CIPHER_MODE_STREAM, 0);

      if (err == GPG_ERR_NO_ERROR)
	{
	  *cipher = hd;
	  return PDF_OK;
	}
      else
	{
	  pdf_dealloc (hd);
	  return PDF_ERROR;
	}
    }
  else
    {
      pdf_dealloc (hd);
      return PDF_ENOMEM;
    }

  return PDF_OK;
}


static pdf_status_t
pdf_crypt_cipher_v2_destroy (void * cipher)
{
  gcry_cipher_hd_t * hd = cipher;
  gcry_cipher_close (*hd);
  pdf_dealloc (cipher);
  return PDF_OK;
}



/* Encryption and decryption functions */

static pdf_status_t
pdf_crypt_cipher_v2_setkey (void * cipher,
			    pdf_char_t *key, pdf_size_t size)
{
  if (gcry_cipher_setkey (cipher, key, size) != GPG_ERR_NO_ERROR)
    return PDF_EBADDATA;
  else
    return PDF_OK;
}


static pdf_size_t
pdf_crypt_cipher_v2_encrypt_size (void * cipher,
				  pdf_char_t *in, pdf_size_t in_size)
{
  return in_size;
}


static pdf_size_t
pdf_crypt_cipher_v2_decrypt_size (void * cipher,
				  pdf_char_t *in, pdf_size_t in_size)
{
  return in_size;
}



static pdf_status_t
pdf_crypt_cipher_v2_encrypt (void * cipher,
			     pdf_char_t *out, pdf_size_t out_size,
			     pdf_char_t *in,  pdf_size_t in_size,
			     pdf_size_t *result_size)
{
  gcry_cipher_encrypt (cipher, out, out_size, in, in_size);
  *result_size = in_size;
  return PDF_OK;
}



static pdf_size_t
pdf_crypt_cipher_v2_decrypt (void * cipher,
			     pdf_char_t *out, pdf_size_t out_size,
			     pdf_char_t *in,  pdf_size_t in_size,
			     pdf_size_t *result_size)
{
  gcry_cipher_decrypt (cipher, out, out_size, in, in_size);
  *result_size = in_size;
  return PDF_OK;
}



struct pdf_crypt_cipher_algo_s pdf_crypt_cipher_v2 = 
{
    pdf_crypt_cipher_v2_new,
    pdf_crypt_cipher_v2_setkey,
    pdf_crypt_cipher_v2_encrypt_size,
    pdf_crypt_cipher_v2_decrypt_size,
    pdf_crypt_cipher_v2_encrypt,  
    pdf_crypt_cipher_v2_decrypt,
    pdf_crypt_cipher_v2_destroy
};


/* End of pdf-crypt-c-v2.c */

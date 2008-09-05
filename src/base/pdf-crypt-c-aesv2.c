/* -*- mode: C -*- Time-stamp: "2008-09-05 04:37:42 david"
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

#define AESV2_BLKSIZE 16	/* Size of a block in AES128 */


/* Creation and destruction of aesv2 ciphers */

static pdf_status_t
pdf_crypt_cipher_aesv2_new (void ** cipher)
{
  gcry_cipher_hd_t * hd;

  hd = pdf_alloc (sizeof (gcry_cipher_hd_t));

  if (hd != NULL)
    {
      gcry_error_t err;

      err = gcry_cipher_open (hd, GCRY_CIPHER_AES128, GCRY_CIPHER_MODE_CBC, 0);

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
pdf_crypt_cipher_aesv2_destroy (void * cipher)
{
  gcry_cipher_hd_t * hd = cipher;
  gcry_cipher_close (*hd);
  pdf_dealloc (cipher);
  return PDF_OK;
}



/* Encryption and decryption functions */

static pdf_status_t
pdf_crypt_cipher_aesv2_setkey (void * cipher,
			       pdf_char_t *key, pdf_size_t size)
{
  gcry_cipher_hd_t * hd = cipher;
  
  if (gcry_cipher_setkey (*hd, key, size) == GPG_ERR_NO_ERROR)
    return PDF_OK;
  else
    return PDF_EBADDATA;
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
  if (in_size < 2*AESV2_BLKSIZE)
    {
      PDF_DEBUG_BASE ("Invalid input buffer size");
      return 0;
    }
  else
    {
      return in_size <= 2*AESV2_BLKSIZE ? 0: in_size - 2*AESV2_BLKSIZE;
    }
}



static pdf_status_t
pdf_crypt_cipher_aesv2_encrypt (void * cipher,
				pdf_char_t *out, pdf_size_t out_size,
				pdf_char_t *in,  pdf_size_t in_size,
				pdf_size_t *result_size)
{
  gcry_cipher_hd_t * hd = cipher;
  pdf_size_t   buffer_size;
  pdf_size_t   iv_size	     = AESV2_BLKSIZE;
  pdf_size_t   content_size  = in_size;
  pdf_size_t   padding_size;
  pdf_char_t * buffer	     = out;
  pdf_char_t * iv	     = &buffer[0];
  pdf_char_t * content	     = &buffer[iv_size];
  pdf_char_t * padding	     = &buffer[iv_size + content_size];

  buffer_size  = pdf_crypt_cipher_aesv2_encrypt_size (cipher, in, in_size);

  if (out_size < buffer_size)
    return PDF_ERROR;

  padding_size = buffer_size - iv_size - content_size;

  gcry_create_nonce (iv, iv_size);
  memcpy (content, in, in_size);
  memset (padding, padding_size, padding_size);
  
  gcry_cipher_setiv (*hd, iv, iv_size);
  if (gcry_cipher_encrypt (*hd, content, content_size + padding_size, NULL, 0) != GPG_ERR_NO_ERROR)
    {
      return PDF_ERROR;
    }

  *result_size = buffer_size;

  return PDF_OK;
}



static pdf_status_t
pdf_crypt_cipher_aesv2_decrypt (void * cipher,
				pdf_char_t *out, pdf_size_t out_size,
				pdf_char_t *in,  pdf_size_t in_size,
				pdf_size_t *result_size)
{
  gcry_cipher_hd_t * hd = cipher;
  pdf_size_t   buffer_size   = in_size;
  pdf_size_t   iv_size	     = AESV2_BLKSIZE;
  pdf_size_t   content_size;
  pdf_size_t   padding_size;
  pdf_char_t * buffer	     = in;
  pdf_char_t * iv	     = &buffer[0];
  pdf_char_t * content	     = &buffer[iv_size];
  
  gcry_cipher_setiv (*hd, iv, iv_size);
  if (gcry_cipher_decrypt (*hd, content, buffer_size - iv_size, NULL, 0) != GPG_ERR_NO_ERROR)
    {
      return PDF_ERROR;
    }

  padding_size = content[buffer_size - iv_size - 1];
  content_size = buffer_size - iv_size - padding_size;

  memcpy (out, content, content_size);

  *result_size = content_size;
  
  return PDF_OK;
}


/* Structure for crypt module */
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

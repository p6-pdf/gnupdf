/* -*- mode: C -*-
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

#define AESV2_BLKSIZE 16	/* Size of a block in AES128 */

struct pdf_crypt_cipher_aesv2_s {
  /* Implementation */
  const struct pdf_crypt_cipher_s *parent;
  /* Implementation-specific private data */
  gcry_cipher_hd_t hd;
  pdf_bool_t first_block;
};

static pdf_bool_t
aesv2_set_key (pdf_crypt_cipher_t  *cipher,
               const pdf_char_t    *key,
               pdf_size_t           size,
               pdf_error_t        **error)
{
  struct pdf_crypt_cipher_aesv2_s *aesv2 = (struct pdf_crypt_cipher_aesv2_s *)cipher;
  gcry_error_t gcry_error;

  gcry_error = gcry_cipher_setkey (aesv2->hd, key, size);
  if (gcry_error != GPG_ERR_NO_ERROR)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_ENCRYPTION,
                     PDF_ENOMEM,
                     "cannot set key in AESv2 cipher: '%s/%s'",
                     gcry_strsource (gcry_error),
                     gcry_strerror (gcry_error));
      return PDF_FALSE;
    }
  return PDF_TRUE;
}

static pdf_bool_t
aesv2_encrypt (pdf_crypt_cipher_t  *cipher,
               pdf_char_t          *out,
               pdf_size_t           out_size,
               const pdf_char_t    *in,
               pdf_size_t           in_size,
               pdf_size_t          *result_size,
               pdf_error_t        **error)
{
  struct pdf_crypt_cipher_aesv2_s *aesv2 = (struct pdf_crypt_cipher_aesv2_s *)cipher;
  gcry_error_t gcry_error;
  pdf_char_t *output;
  pdf_size_t output_size;
  const pdf_char_t *input;
  pdf_size_t input_size;

  if (in_size < AESV2_BLKSIZE ||
      in_size % AESV2_BLKSIZE != 0)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_ENCRYPTION,
                     PDF_EBADDATA,
                     "cannot encrypt in AESv2 cipher: "
                     "invalid input size (%lu), "
                     "must be multiple of block size (%lu)",
                     (unsigned long)in_size,
                     (unsigned long)AESV2_BLKSIZE);
      return PDF_FALSE;
    }

  if (out_size < in_size)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_ENCRYPTION,
                     PDF_EBADDATA,
                     "cannot encrypt in AESv2 cipher: "
                     "output size (%lu) cannot be smaller than input size (%lu)",
                     (unsigned long)output_size,
                     (unsigned long)in_size);
      return PDF_FALSE;
    }

  /* If we are at first block, then we have found the IV vector */
  if (aesv2->first_block == PDF_TRUE)
    {
      const pdf_char_t *iv = in;

      gcry_cipher_setiv (aesv2->hd, iv, AESV2_BLKSIZE);
      input = in + AESV2_BLKSIZE;
      input_size = in_size - AESV2_BLKSIZE;

      memcpy (out, iv, AESV2_BLKSIZE);

      output = out + AESV2_BLKSIZE;
      output_size = out_size - AESV2_BLKSIZE;

      aesv2->first_block = PDF_FALSE;
    }
  else
    {
      output = out;
      output_size = out_size;
      input = in;
      input_size = in_size;
    }

  gcry_error = gcry_cipher_encrypt (aesv2->hd, out, out_size, in, in_size);
  if (gcry_error != GPG_ERR_NO_ERROR)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_ENCRYPTION,
                     PDF_ENOMEM,
                     "cannot encrypt with AESv2 cipher: '%s/%s'",
                     gcry_strsource (gcry_error),
                     gcry_strerror (gcry_error));
      return PDF_FALSE;
    }

  if (result_size)
    *result_size = in_size;

  return PDF_TRUE;
}

static pdf_bool_t
aesv2_decrypt (pdf_crypt_cipher_t  *cipher,
               pdf_char_t          *out,
               pdf_size_t           out_size,
               const pdf_char_t    *in,
               pdf_size_t           in_size,
               pdf_size_t          *result_size,
               pdf_error_t        **error)
{
  struct pdf_crypt_cipher_aesv2_s *aesv2 = (struct pdf_crypt_cipher_aesv2_s *)cipher;
  gcry_error_t gcry_error;
  pdf_char_t *output;
  pdf_size_t output_size;
  const pdf_char_t *input;
  pdf_size_t input_size;

  if (in_size < AESV2_BLKSIZE ||
      in_size % AESV2_BLKSIZE != 0)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_ENCRYPTION,
                     PDF_EBADDATA,
                     "cannot decrypt in AESv2 cipher: "
                     "invalid input size (%lu), "
                     "must be multiple of block size (%lu)",
                     (unsigned long)in_size,
                     (unsigned long)AESV2_BLKSIZE);
      return PDF_FALSE;
    }

  if (out_size < in_size)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_ENCRYPTION,
                     PDF_EBADDATA,
                     "cannot decrypt in AESv2 cipher: "
                     "output size (%lu) cannot be smaller than input size (%lu)",
                     (unsigned long)out_size,
                     (unsigned long)in_size);
      return PDF_FALSE;
    }

  /* If we are at first block, then we have found the IV vector */
  if (aesv2->first_block == PDF_TRUE)
    {
      const pdf_char_t *iv = in;

      gcry_cipher_setiv (aesv2->hd, iv, AESV2_BLKSIZE);
      input = in + AESV2_BLKSIZE;
      input_size = in_size - AESV2_BLKSIZE;

      memcpy (out, iv, AESV2_BLKSIZE);

      output = out + AESV2_BLKSIZE;
      output_size = out_size - AESV2_BLKSIZE;

      aesv2->first_block = PDF_FALSE;
    }
  else
    {
      output = out;
      output_size = out_size;
      input = in;
      input_size = in_size;
    }

  gcry_error = gcry_cipher_decrypt (aesv2->hd, out, out_size, in, in_size);
  if (gcry_error != GPG_ERR_NO_ERROR)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_ENCRYPTION,
                     PDF_ENOMEM,
                     "cannot decrypt with AESv2 cipher: '%s/%s'",
                     gcry_strsource (gcry_error),
                     gcry_strerror (gcry_error));
      return PDF_FALSE;
    }

  if (result_size)
    *result_size = in_size;

  return PDF_TRUE;
}

static void
aesv2_destroy (pdf_crypt_cipher_t *cipher)
{
  struct pdf_crypt_cipher_aesv2_s *aesv2 = (struct pdf_crypt_cipher_aesv2_s *)cipher;

  gcry_cipher_close (aesv2->hd);
  pdf_dealloc (aesv2);
}

/* Implementation of the cipher module */
static const struct pdf_crypt_cipher_s implementation = {
  .set_key = aesv2_set_key,
  .encrypt = aesv2_encrypt,
  .decrypt = aesv2_decrypt,
  .destroy = aesv2_destroy
};

pdf_crypt_cipher_t *
pdf_crypt_cipher_aesv2_new (pdf_error_t **error)
{
  gcry_error_t gcry_error;
  struct pdf_crypt_cipher_aesv2_s *cipher;

  cipher = pdf_alloc (sizeof (struct pdf_crypt_cipher_aesv2_s));
  if (!cipher)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_ENCRYPTION,
                     PDF_ENOMEM,
                     "cannot create new AESv2 cipher object: "
                     "couldn't allocate %lu bytes",
                     (unsigned long)sizeof (struct pdf_crypt_cipher_aesv2_s));
      return NULL;
    }

  /* Set implementation API */
  cipher->parent = &implementation;

  /* Initialize cipher object */
  cipher->first_block = PDF_TRUE;
  gcry_error = gcry_cipher_open (&(cipher->hd),
                                 GCRY_CIPHER_AES128,
                                 GCRY_CIPHER_MODE_CBC,
                                 0);
  if (gcry_error != GPG_ERR_NO_ERROR)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_ENCRYPTION,
                     PDF_ENOMEM,
                     "cannot initialize AESv2 cipher: '%s/%s'",
                     gcry_strsource (gcry_error),
                     gcry_strerror (gcry_error));
      pdf_dealloc (cipher);
      return NULL;
    }

  return (pdf_crypt_cipher_t *)cipher;
}

/* End of pdf-crypt-c-aesv2.c */

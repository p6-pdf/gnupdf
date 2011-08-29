/* -*- mode: C -*-
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

#include <config.h>

#include <stdlib.h>
#include <string.h>
#include <gcrypt.h>

#include <pdf-alloc.h>
#include <pdf-types.h>
#include <pdf-error.h>
#include <pdf-crypt-c-v2.h>

struct pdf_crypt_cipher_v2_s {
  /* Implementation */
  struct pdf_crypt_cipher_s parent;
  /* Implementation-specific private data */
  gcry_cipher_hd_t hd;
};

static pdf_bool_t
v2_set_key (pdf_crypt_cipher_t  *cipher,
            const pdf_char_t    *key,
            pdf_size_t           size,
            pdf_error_t        **error)
{
  struct pdf_crypt_cipher_v2_s *v2 = (struct pdf_crypt_cipher_v2_s *)cipher;
  gcry_error_t gcry_error;

  gcry_error = gcry_cipher_setkey (v2->hd, key, size);
  if (gcry_error != GPG_ERR_NO_ERROR)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_ENCRYPTION,
                     PDF_EBADV2KEY,
                     "cannot set key in V2 cipher: '%s/%s'",
                     gcry_strsource (gcry_error),
                     gcry_strerror (gcry_error));
      return PDF_FALSE;
    }
  return PDF_TRUE;
}

static pdf_bool_t
v2_encrypt (pdf_crypt_cipher_t  *cipher,
            pdf_char_t          *out,
            pdf_size_t           out_size,
            const pdf_char_t    *in,
            pdf_size_t           in_size,
            pdf_size_t          *result_size,
            pdf_error_t        **error)
{
  struct pdf_crypt_cipher_v2_s *v2 = (struct pdf_crypt_cipher_v2_s *)cipher;
  gcry_error_t gcry_error;

  gcry_error = gcry_cipher_encrypt (v2->hd, out, out_size, in, in_size);
  if (gcry_error != GPG_ERR_NO_ERROR)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_ENCRYPTION,
                     PDF_ENOMEM,
                     "cannot set key in V2 cipher: '%s/%s'",
                     gcry_strsource (gcry_error),
                     gcry_strerror (gcry_error));
      return PDF_FALSE;
    }

  if (result_size)
    *result_size = in_size;

  return PDF_TRUE;
}

static pdf_bool_t
v2_decrypt (pdf_crypt_cipher_t  *cipher,
            pdf_char_t          *out,
            pdf_size_t           out_size,
            const pdf_char_t    *in,
            pdf_size_t           in_size,
            pdf_size_t          *result_size,
            pdf_error_t        **error)
{
  struct pdf_crypt_cipher_v2_s *v2 = (struct pdf_crypt_cipher_v2_s *)cipher;
  gcry_error_t gcry_error;

  gcry_error = gcry_cipher_decrypt (v2->hd, out, out_size, in, in_size);
  if (gcry_error != GPG_ERR_NO_ERROR)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_ENCRYPTION,
                     PDF_ENOMEM,
                     "cannot set key in V2 cipher: '%s/%s'",
                     gcry_strsource (gcry_error),
                     gcry_strerror (gcry_error));
      return PDF_FALSE;
    }

  if (result_size)
    *result_size = in_size;

  return PDF_TRUE;
}

static void
v2_destroy (pdf_crypt_cipher_t *cipher)
{
  struct pdf_crypt_cipher_v2_s *v2 = (struct pdf_crypt_cipher_v2_s *)cipher;

  gcry_cipher_close (v2->hd);
  pdf_dealloc (v2);
}

/* Implementation of the cipher module */
static const struct pdf_crypt_cipher_s implementation = {
  .set_key = v2_set_key,
  .encrypt = v2_encrypt,
  .decrypt = v2_decrypt,
  .destroy = v2_destroy
};

pdf_crypt_cipher_t *
pdf_crypt_cipher_v2_new (pdf_error_t **error)
{
  gcry_error_t gcry_error;
  struct pdf_crypt_cipher_v2_s *cipher;

  cipher = pdf_alloc (sizeof (struct pdf_crypt_cipher_v2_s));
  if (!cipher)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_ENCRYPTION,
                     PDF_ENOMEM,
                     "cannot create new V2 cipher object: "
                     "couldn't allocate %lu bytes",
                     (unsigned long)sizeof (struct pdf_crypt_cipher_v2_s));
      return NULL;
    }

  /* Set implementation API */
  cipher->parent = implementation;

  /* Initialize cipher object */
  gcry_error = gcry_cipher_open (&(cipher->hd),
                                 GCRY_CIPHER_ARCFOUR,
                                 GCRY_CIPHER_MODE_STREAM,
                                 0);
  if (gcry_error != GPG_ERR_NO_ERROR)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_ENCRYPTION,
                     PDF_ENOMEM,
                     "cannot initialize V2 cipher: '%s/%s'",
                     gcry_strsource (gcry_error),
                     gcry_strerror (gcry_error));
      pdf_dealloc (cipher);
      return NULL;
    }

  return (pdf_crypt_cipher_t *)cipher;
}

/* End of pdf-crypt-c-v2.c */

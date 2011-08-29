/* -*- mode: C -*-
 *
 *       File:         pdf-crypt.c
 *       Date:         Fri Feb 22 21:05:05 2008
 *
 *       GNU PDF Library - Encryption module
 *
 */

/* Copyright (C) 2008-2011 Free Software Foundation, Inc. */

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

#include <gcrypt.h>

#include <pdf-crypt.h>
#include <pdf-crypt-md-md5.h>
#include <pdf-crypt-c-aesv2.h>
#include <pdf-crypt-c-v2.h>

pdf_bool_t
pdf_crypt_init (pdf_error_t **error)
{
  if (!gcry_check_version (GCRYPT_VERSION))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_ENCRYPTION,
                     PDF_EBADDATA,
                     "cannot initialize crypt module: "
                     "minimum required version is '%s'",
                     GCRYPT_VERSION);
      return PDF_FALSE;
    }

  return PDF_TRUE;
}

pdf_char_t *
pdf_crypt_nonce (pdf_char_t *buffer,
                 pdf_size_t  size)
{
  gcry_create_nonce (buffer, size);
  return buffer;
}

pdf_crypt_cipher_t *
pdf_crypt_cipher_new (enum pdf_crypt_cipher_algo_e   algorithm,
                      pdf_error_t                  **error)
{
  PDF_ASSERT_RETURN_VAL (algorithm >= PDF_CRYPT_CIPHER_ALGO_AESV2 &&
                         algorithm <= PDF_CRYPT_CIPHER_ALGO_V2,
                         NULL);

  switch (algorithm)
    {
    case PDF_CRYPT_CIPHER_ALGO_AESV2:
      return pdf_crypt_cipher_aesv2_new (error);
    case PDF_CRYPT_CIPHER_ALGO_V2:
      return pdf_crypt_cipher_v2_new (error);
    }
}

pdf_crypt_md_t *
pdf_crypt_md_new (enum pdf_crypt_md_algo_e   algorithm,
                  pdf_error_t              **error)
{
  /* So far, only MD5 supported */
  PDF_ASSERT_RETURN_VAL (algorithm == PDF_CRYPT_MD_MD5, NULL);

  return pdf_crypt_md_md5_new (error);
}

/* End of pdf-crypt.c */

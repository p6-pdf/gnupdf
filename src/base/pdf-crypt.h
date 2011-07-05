/* -*- mode: C -*-
 *
 *       File:         pdf-crypt.c
 *       Date:         Fri Feb 22 21:05:05 2008
 *
 *       GNU PDF Library - Header file for encryption module
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

#ifndef PDF_CRYPT_H
#define PDF_CRYPT_H

#include <config.h>

#include <string.h>

#include <pdf-types.h>
#include <pdf-error.h>
#include <pdf-alloc.h>

/* BEGIN PUBLIC */

/* ------------------ Crypt Module Cipher API ---------------------- */

/* Encryption types */
enum pdf_crypt_cipher_algo_e
{
  PDF_CRYPT_CIPHER_ALGO_AESV2,
  PDF_CRYPT_CIPHER_ALGO_V2
};

typedef struct pdf_crypt_cipher_s pdf_crypt_cipher_t;
struct pdf_crypt_cipher_s
{
  pdf_bool_t (*set_key) (pdf_crypt_cipher_t  *cipher,
                         const pdf_char_t    *key,
                         pdf_size_t           size,
                         pdf_error_t        **error);

  pdf_bool_t (*encrypt) (pdf_crypt_cipher_t  *cipher,
                         pdf_char_t          *out,
                         pdf_size_t           out_size,
                         const pdf_char_t    *in,
                         pdf_size_t           in_size,
                         pdf_size_t          *result_size,
                         pdf_error_t        **error);

  pdf_bool_t (*decrypt) (pdf_crypt_cipher_t  *cipher,
                         pdf_char_t          *out,
                         pdf_size_t           out_size,
                         const pdf_char_t    *in,
                         pdf_size_t           in_size,
                         pdf_size_t          *result_size,
                         pdf_error_t        **error);

  void (*destroy) (pdf_crypt_cipher_t *cipher);
};

pdf_crypt_cipher_t *pdf_crypt_cipher_new (enum pdf_crypt_cipher_algo_e   algorithm,
                                          pdf_error_t                  **error);

#define pdf_crypt_cipher_set_key(cipher, key, size, error)              \
  ((pdf_crypt_cipher_t *)(cipher))->set_key (cipher, key, size, error)

#define pdf_crypt_cipher_encrypt(cipher, out, out_size, in, in_size, result_size, error) \
  ((pdf_crypt_cipher_t *)(cipher))->encrypt (cipher,                    \
                                             out,                       \
                                             out_size,                  \
                                             in,                        \
                                             in_size,                   \
                                             result_size,               \
                                             error)

#define pdf_crypt_cipher_decrypt(cipher, out, out_size, in, in_size, result_size, error) \
  ((pdf_crypt_cipher_t *)(cipher))->decrypt (cipher,                    \
                                             out,                       \
                                             out_size,                  \
                                             in,                        \
                                             in_size,                   \
                                             result_size,               \
                                             error)

#define pdf_crypt_cipher_destroy(cipher)        \
  ((pdf_crypt_cipher_t *)(cipher))->destroy (cipher)

/* ------------------ Crypt Module Message Digest API ---------------------- */

/* Hashing types */
enum pdf_crypt_md_algo_e
{
  PDF_CRYPT_MD_MD5,
};

typedef struct pdf_crypt_md_s pdf_crypt_md_t;
struct pdf_crypt_md_s
{
  pdf_bool_t (*write) (pdf_crypt_md_t    *md,
                       const pdf_char_t  *in,
                       pdf_size_t         in_size,
                       pdf_error_t      **error);

  pdf_bool_t (*read) (pdf_crypt_md_t  *md,
                      pdf_char_t      *out,
                      pdf_size_t       out_size,
                      pdf_error_t    **error);

  void (*destroy) (pdf_crypt_md_t *md);
};

pdf_crypt_md_t *pdf_crypt_md_new (enum pdf_crypt_md_algo_e   algorithm,
                                  pdf_error_t              **error);

#define pdf_crypt_md_write(md, in, in_size, error)  \
  ((pdf_crypt_md_t *)(md))->write (md,              \
                                   in,              \
                                   in_size,         \
                                   error)

#define pdf_crypt_md_read(md, out, out_size, error)  \
  ((pdf_crypt_md_t *)(md))->read (md,                \
                                  out,               \
                                  out_size,          \
                                  error)

#define pdf_crypt_md_destroy(md)                \
  ((pdf_crypt_md_t *)(md))->destroy (md)

/* ------------------ Crypt Module Other Utilities ---------------------- */

pdf_char_t *pdf_crypt_nonce (pdf_char_t *buffer,
                             pdf_size_t  size);

/* END PUBLIC */

pdf_bool_t pdf_crypt_init (pdf_error_t **error);

#endif /* PDF_CRYPT_H */

/* End of pdf-crypt.h */

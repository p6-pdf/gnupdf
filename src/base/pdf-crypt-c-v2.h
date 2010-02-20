/* -*- mode: C -*-
 *
 *       File:         pdf-crypt.c
 *       Date:         Fri Feb 22 21:05:05 2008
 *
 *       GNU PDF Library - Header file for V2 backend encryption module
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


#ifndef PDF_CRYPT_C_V2_H
#define PDF_CRYPT_C_V2_H

pdf_status_t pdf_crypt_cipher_v2_new (void ** cipher);
pdf_status_t pdf_crypt_cipher_v2_destroy (void * cipher);
pdf_status_t pdf_crypt_cipher_v2_setkey (void * cipher,
                                         pdf_char_t *key,
                                         pdf_size_t size);
pdf_size_t pdf_crypt_cipher_v2_encrypt_size (void * cipher,
                                             pdf_char_t *in,
                                             pdf_size_t in_size);
pdf_size_t pdf_crypt_cipher_v2_decrypt_size (void * cipher,
                                             pdf_char_t *in,
                                             pdf_size_t in_size);
pdf_status_t pdf_crypt_cipher_v2_encrypt (void * cipher,
                                          pdf_char_t *out, pdf_size_t out_size,
                                          pdf_char_t *in,  pdf_size_t in_size,
                                          pdf_size_t *result_size);
pdf_status_t pdf_crypt_cipher_v2_decrypt (void * cipher,
                                          pdf_char_t *out, pdf_size_t out_size,
                                          pdf_char_t *in,  pdf_size_t in_size,
                                          pdf_size_t *result_size);

#endif	/* PDF_CRYPT_C_V2_H */

/* End of pdf-crypt-c-v2.h */

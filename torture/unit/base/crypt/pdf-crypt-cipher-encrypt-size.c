/* -*- mode: C -*- Time-stamp: "2008-09-09 04:07:52 david"
 *
 *       File:         pdf-crypt-cipher-encrypt-size.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_crypt_cipher_encrypt_size
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

#include <stdbool.h>
#include <stdio.h>
#include <pdf.h>
#include <check.h>


/*
 * Test: pdf_crypt_cipher_encrypt_size_001
 * Description:
 *   Get the ciphered buffer size for an empty input buffer (AESV2).
 * Success condition:
 *   Returns 32
 */
START_TEST (pdf_crypt_cipher_encrypt_size_001)
{
  pdf_crypt_cipher_t cipher;
  pdf_crypt_init();
  pdf_crypt_cipher_new (PDF_CRYPT_CIPHER_ALGO_AESV2, &cipher);
  fail_if (pdf_crypt_cipher_encrypt_size (cipher, NULL, 0) != 32);
  pdf_crypt_cipher_destroy (cipher);
}
END_TEST


/*
 * Test: pdf_crypt_cipher_encrypt_size_002
 * Description:
 *   Get the ciphered buffer size for a 15 bytes buffer (AESV2).
 * Success condition:
 *   Returns 32
 */
START_TEST (pdf_crypt_cipher_encrypt_size_002)
{
  pdf_crypt_cipher_t cipher;
  pdf_crypt_init();
  pdf_crypt_cipher_new (PDF_CRYPT_CIPHER_ALGO_AESV2, &cipher);
  fail_if (pdf_crypt_cipher_encrypt_size (cipher, NULL, 15) != 32);
  pdf_crypt_cipher_destroy (cipher);

}
END_TEST


/*
 * Test: pdf_crypt_cipher_encrypt_size_003
 * Description:
 *   Get the ciphered buffer size for a empty buffer (V2).
 * Success condition:
 *   Returns 0
 */
START_TEST (pdf_crypt_cipher_encrypt_size_003)
{
  pdf_crypt_cipher_t cipher;
  pdf_crypt_init();
  pdf_crypt_cipher_new (PDF_CRYPT_CIPHER_ALGO_V2, &cipher);
  fail_if (pdf_crypt_cipher_encrypt_size (cipher, NULL, 0) != 0);
  pdf_crypt_cipher_destroy (cipher);
}
END_TEST



/*
 * Test: pdf_crypt_cipher_encrypt_size_004
 * Description:
 *   Get the ciphered buffer size for a 15 bytes buffer (V2).
 * Success condition:
 *   Returns 15
 */
START_TEST (pdf_crypt_cipher_encrypt_size_004)
{
  pdf_crypt_cipher_t cipher;
  pdf_crypt_init();
  pdf_crypt_cipher_new (PDF_CRYPT_CIPHER_ALGO_V2, &cipher);
  fail_if (pdf_crypt_cipher_encrypt_size (cipher, NULL, 15) != 15);
  pdf_crypt_cipher_destroy (cipher);
}
END_TEST




/*
 * Test case creation function
 */
TCase *
test_pdf_crypt_cipher_encrypt_size (void)
{
  TCase *tc = tcase_create("pdf_crypt_cipher_encrypt_size");
  tcase_add_test(tc, pdf_crypt_cipher_encrypt_size_001);
  tcase_add_test(tc, pdf_crypt_cipher_encrypt_size_002);
  tcase_add_test(tc, pdf_crypt_cipher_encrypt_size_003);
  tcase_add_test(tc, pdf_crypt_cipher_encrypt_size_004);
  return tc;
}


/* End of pdf-crypt-cipher-encrypt-size.c */

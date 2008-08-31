/* -*- mode: C -*- Time-stamp: "2008-08-31 02:54:14 davazp"
 *
 *       File:         pdf-crypt-cipher-decrypt-size.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_crypt_cipher_decrypt_size
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
 * Test: pdf_crypt_cipher_decrypt_size_001
 * Description:
 *   Compute the ouput buffer size for a empty input buffer in a AESV2 cipher.
 * Success condition:
 *   Returns 0
 */
START_TEST (pdf_crypt_cipher_decrypt_size_001)
{
  pdf_crypt_cipher_t cipher;
  pdf_crypt_cipher_new (PDF_CRYPT_CIPHER_ALGO_AESV2, &cipher);
  fail_if (pdf_crypt_cipher_decrypt_size (cipher, NULL, 0) != 0);
  pdf_crypt_cipher_destroy (cipher);
}
END_TEST


/*
 * Test: pdf_crypt_cipher_decrypt_size_002
 * Description:
 *   Compute the ouput buffer size for a 32 bytes buffer in a AESV2 cipher.
 * Success condition:
 *   Returns 0
 */
START_TEST (pdf_crypt_cipher_decrypt_size_002)
{
  pdf_crypt_cipher_t cipher;
  pdf_crypt_cipher_new (PDF_CRYPT_CIPHER_ALGO_AESV2, &cipher);
  fail_if (pdf_crypt_cipher_decrypt_size (cipher, NULL, 32) != 0);
  pdf_crypt_cipher_destroy (cipher);
}
END_TEST



/*
 * Test: pdf_crypt_cipher_decrypt_size_002
 * Description:
 *   Compute the ouput buffer size for a 32 bytes buffer in a AESV2 cipher.
 * Success condition:
 *   Returns 0
 */
START_TEST (pdf_crypt_cipher_decrypt_size_003)
{
  pdf_crypt_cipher_t cipher;
  pdf_crypt_cipher_new (PDF_CRYPT_CIPHER_ALGO_AESV2, &cipher);
  fail_if (pdf_crypt_cipher_decrypt_size (cipher, NULL, 31) != 0);
  pdf_crypt_cipher_destroy (cipher);
}
END_TEST



/*
 * Test: pdf_crypt_cipher_decrypt_size_003
 * Description:
 *   Compute the ouput buffer size for a 0 bytes buffer in a V2 cipher.
 * Success condition:
 *   Returns 0
 */
START_TEST (pdf_crypt_cipher_decrypt_size_004)
{
  pdf_crypt_cipher_t cipher;
  pdf_crypt_cipher_new (PDF_CRYPT_CIPHER_ALGO_V2, &cipher);
  fail_if (pdf_crypt_cipher_decrypt_size (cipher, NULL, 15) != 15);
  pdf_crypt_cipher_destroy (cipher);
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_crypt_cipher_decrypt_size (void)
{
  TCase *tc = tcase_create("pdf_crypt_cipher_decrypt_size");
  tcase_add_test(tc, pdf_crypt_cipher_decrypt_size_001);
  tcase_add_test(tc, pdf_crypt_cipher_decrypt_size_002);
  tcase_add_test(tc, pdf_crypt_cipher_decrypt_size_003);
  tcase_add_test(tc, pdf_crypt_cipher_decrypt_size_004);
  return tc;
}


/* End of pdf-crypt-cipher-decrypt-size.c */


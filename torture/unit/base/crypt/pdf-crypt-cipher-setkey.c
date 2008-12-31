/* -*- mode: C -*- Time-stamp: "2008-12-23 19:15:33 davazp"
 *
 *       File:         pdf-crypt-cipher-decrypt.c
 *       Date:         Tue Dec 23 17:36:21 2008
 *
 *       GNU PDF Library - Unit tests for pdf_crypt_cipher_setkey
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
 * Test: pdf_crypt_cipher_setkey_001
 * Description:
 *   Set a key of 16 bytes. (AESv2)
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_crypt_cipher_setkey_001)
{
  pdf_crypt_cipher_t cipher;
  pdf_crypt_init();
  pdf_crypt_cipher_new (PDF_CRYPT_CIPHER_ALGO_AESV2, &cipher);

  fail_if (pdf_crypt_cipher_setkey (cipher, "0123456789abcdef", 16) != PDF_OK);
  
  pdf_crypt_cipher_destroy (cipher);
}
END_TEST


/*
 * Test: pdf_crypt_cipher_setkey_002
 * Description:
 *   Set a key of 3 bytes (AESv2)
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_crypt_cipher_setkey_002)
{
  pdf_crypt_cipher_t cipher;
  pdf_crypt_init();
  pdf_crypt_cipher_new (PDF_CRYPT_CIPHER_ALGO_AESV2, &cipher);

  fail_if (pdf_crypt_cipher_setkey (cipher, "GNU", 3) == PDF_OK);
  
  pdf_crypt_cipher_destroy (cipher);
}
END_TEST


/*
 * Test: pdf_crypt_cipher_setkey_003
 * Description:
 *   Set a key of 6 bytes (V2)
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_crypt_cipher_setkey_003)
{
  pdf_crypt_cipher_t cipher;
  pdf_crypt_init();
  pdf_crypt_cipher_new (PDF_CRYPT_CIPHER_ALGO_V2, &cipher);

  fail_if (pdf_crypt_cipher_setkey (cipher, "GNUGNU", 6) != PDF_OK);
  
  pdf_crypt_cipher_destroy (cipher);
}
END_TEST




/*
 * Test case creation function
 */
TCase *
test_pdf_crypt_cipher_setkey (void)
{
  TCase *tc = tcase_create("pdf_crypt_cipher_setkey");
  tcase_add_test(tc, pdf_crypt_cipher_setkey_001);
  tcase_add_test(tc, pdf_crypt_cipher_setkey_002);
  tcase_add_test(tc, pdf_crypt_cipher_setkey_003);
  return tc;
}


/* End of pdf-crypt-cipher-setkey.c */


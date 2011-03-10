/* -*- mode: C -*-
 *
 *       File:         pdf-crypt-cipher-destroy.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_crypt_cipher_destroy
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

#include <stdbool.h>
#include <stdio.h>
#include <pdf.h>
#include <check.h>
#include <pdf-test-common.h>

/*
 * Test: pdf_crypt_cipher_destroy_001
 * Description:
 *   Destroy a AESV2 cipher.
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_crypt_cipher_destroy_001)
{
  pdf_crypt_cipher_t cipher;
    pdf_crypt_cipher_new (PDF_CRYPT_CIPHER_ALGO_AESV2, &cipher);
  fail_if (pdf_crypt_cipher_destroy (cipher) != PDF_OK);
}
END_TEST


/*
 * Test: pdf_crypt_cipher_destroy_002
 * Description:
 *   Destroy a V2 cipher.
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_crypt_cipher_destroy_002)
{
  pdf_crypt_cipher_t cipher;
    pdf_crypt_cipher_new (PDF_CRYPT_CIPHER_ALGO_V2, &cipher);
  fail_if (pdf_crypt_cipher_destroy (cipher) != PDF_OK);
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_crypt_cipher_destroy (void)
{
  TCase *tc = tcase_create("pdf_crypt_cipher_destroy");
  tcase_add_test(tc, pdf_crypt_cipher_destroy_001);
  tcase_add_test(tc, pdf_crypt_cipher_destroy_002);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}


/* End of pdf-crypt-cipher-destroy.c */

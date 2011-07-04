/* -*- mode: C -*-
 *
 *       File:         pdf-crypt-md-new.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_crypt_md_new
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
 * Test: pdf_crypt_md_new_001
 * Description:
 *   Create a new message-digester.
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_crypt_md_new_001)
{
  pdf_crypt_md_t *md;
  pdf_error_t *error = NULL;

  md = pdf_crypt_md_new (PDF_CRYPT_MD_MD5, &error);
  fail_unless (md != NULL);
  fail_if (error != NULL);

  pdf_crypt_md_destroy (md);
}
END_TEST

/*
 * Test: pdf_crypt_md_new_002
 * Description:
 *   Create a new nonce (random initialization vector).
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_crypt_md_new_002)
{
  pdf_char_t buffer[16];

  fail_unless (pdf_crypt_nonce (buffer, sizeof(buffer)) != NULL);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_crypt_md_new (void)
{
  TCase *tc = tcase_create ("pdf_crypt_md_new");

  tcase_add_test (tc, pdf_crypt_md_new_001);
  tcase_add_test (tc, pdf_crypt_md_new_002);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-crypt-md-new.c */

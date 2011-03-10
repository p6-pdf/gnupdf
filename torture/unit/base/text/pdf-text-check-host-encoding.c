/* -*- mode: C -*-
 *
 *       File:         pdf-text-check-host-encoding.c
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_check_host_encoding test case
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
#include <stdio.h>
#include <string.h>
#include <pdf.h>
#include <check.h>
#include <pdf-test-common.h>
/*
 * Test: pdf_text_check_host_encoding_001
 * Description:
 *   Check if a given valid host encoding is available in the system. ASCII-7
 *   is considered as the host encoding that every system should at least have,
 *   so the test will check for it.
 * Success conditions:
 *   1. The call to pdf_text_check_host_encoding should return PDF_OK.
 *   2. A non-empty pdf_text_host_encoding_t variable should be returned as
 *      well.
 */
START_TEST (pdf_text_check_host_encoding_001)
{
  pdf_text_host_encoding_t host_enc;




  /* 1. The call to pdf_text_check_host_encoding should return PDF_OK. */
#ifdef PDF_HOST_WIN32
  fail_unless(pdf_text_check_host_encoding((pdf_char_t *)"CP20127",
                                           &host_enc) == PDF_OK);
#else
  fail_unless(pdf_text_check_host_encoding((pdf_char_t *)"ascii",
                                           &host_enc) == PDF_OK);
#endif

  /* 2. A non-empty pdf_text_host_encoding_t variable should be returned as
   *      well. */
  fail_unless(strlen(host_enc.name) > 0);
}
END_TEST


/*
 * Test: pdf_text_check_host_encoding_002
 * Description:
 *   Check if a given invalid host encoding is available in the system. An
 *   inexistent host encoding will be requested.
 * Success conditions:
 *   1. The call to pdf_text_check_host_encoding should NOT return PDF_OK.
 *   2. The pdf_text_host_encoding_t variable should remain unchanged.
 */
START_TEST (pdf_text_check_host_encoding_002)
{
  pdf_text_host_encoding_t host_enc;

  /* Set old host encoding name */
  strncpy((&(host_enc.name[0])),"ascii",5);
  host_enc.name[5] = '\0';




  /* 1. The call to pdf_text_check_host_encoding should NOT return PDF_OK. */
#ifdef PDF_HOST_WIN32
  fail_unless(pdf_text_check_host_encoding((pdf_char_t *)"CP17",
                                           &host_enc) != PDF_OK);
#else
  fail_unless(pdf_text_check_host_encoding((pdf_char_t *)"invalid_host_encoding",
                                           &host_enc) != PDF_OK);
#endif

  /* 2. The pdf_text_host_encoding_t variable should remain unchanged. */
  fail_unless(strcmp(host_enc.name, "ascii") == 0);
}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_text_check_host_encoding (void)
{
  TCase *tc = tcase_create("pdf_text_check_host_encoding");
  tcase_add_test(tc, pdf_text_check_host_encoding_001);
  tcase_add_test(tc, pdf_text_check_host_encoding_002);

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-text-get-host-encoding.c */

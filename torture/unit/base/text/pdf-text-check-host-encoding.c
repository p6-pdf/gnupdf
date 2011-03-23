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
 *   1. The call to pdf_text_check_host_encoding should return PDF_TRUE.
 */
START_TEST (pdf_text_check_host_encoding_001)
{
  pdf_error_t *error = NULL;

  /* 1. The call to pdf_text_check_host_encoding should return PDF_TRUE. */
#ifdef PDF_HOST_WIN32
  fail_unless (pdf_text_check_host_encoding ("CP20127", &error) == PDF_TRUE);
#else
  fail_unless (pdf_text_check_host_encoding ("ascii", &error) == PDF_TRUE);
#endif

  fail_if (error != NULL);
}
END_TEST

/*
 * Test: pdf_text_check_host_encoding_002
 * Description:
 *   Check if a given invalid host encoding is available in the system. An
 *   inexistent host encoding will be requested.
 * Success conditions:
 *   1. The call to pdf_text_check_host_encoding should NOT return PDF_TRUE.
 */
START_TEST (pdf_text_check_host_encoding_002)
{
  pdf_error_t *error = NULL;

  /* 1. The call to pdf_text_check_host_encoding should NOT return PDF_OK. */
#ifdef PDF_HOST_WIN32
  fail_if (pdf_text_check_host_encoding ("CP17", &error) == PDF_TRUE);
#else
  fail_if (pdf_text_check_host_encoding ("invalid_host_encoding", &error) == PDF_TRUE);
#endif

  fail_if (error != NULL);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_text_check_host_encoding (void)
{
  TCase *tc = tcase_create ("pdf_text_check_host_encoding");

  tcase_add_test (tc, pdf_text_check_host_encoding_001);
  tcase_add_test (tc, pdf_text_check_host_encoding_002);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-text-check-host-encoding.c */

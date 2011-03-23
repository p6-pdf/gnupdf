/* -*- mode: C -*-
 *
 *       File:         pdf-text-get-host-encoding.c
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_get_host_encoding test case
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
 * Test: pdf_text_get_host_encoding_001
 * Description:
 *   Get the host encoding configured by the user. As this test really depends
 *   on what the user has configured in the system, the unit test will only
 *   check that a non-empty host encoding is returned.
 * Success conditions:
 *   1. The call to pdf_text_get_host_encoding should return a non-empty string.
 */
START_TEST (pdf_text_get_host_encoding_001)
{
  const pdf_char_t *host_enc;

  host_enc = pdf_text_get_host_encoding ();

  /* 1. The call to pdf_text_get_host_encoding should return a non-empty
   *    string. */
  fail_unless (strlen (host_enc) > 0);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_text_get_host_encoding (void)
{
  TCase *tc = tcase_create ("pdf_text_get_host_encoding");

  tcase_add_test (tc, pdf_text_get_host_encoding_001);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-text-get-host-encoding.c */

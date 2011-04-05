/* -*- mode: C -*-
 *
 *       File:         pdf-time-init.c
 *       Date:         Thu Mar 24 21:47:59 2011
 *
 *       GNU PDF Library - Unit tests for pdf_time_init
 *
 */

/* Copyright (C) 2011 Free Software Foundation, Inc. */

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
#include <check.h>
#include <pdf.h>
#include <stdlib.h>
#include <string.h>
#include <pdf-test-common.h>

/*
 * Test: pdf_time_init_001
 * Description:
 *   Create init pdf_time_t.
 *
 *Success condition:
 * 1. Function pdf_time_init should return a valid object
 * 2. Init pdf_time_t object is not NUL
 * 3. pdf_time_t is set to 1970-01-01 00:00:00 UTC
 */
START_TEST (pdf_time_init_001)
{
  const pdf_char_t *expected_time = "1970-01-01T00:00:00Z";
  pdf_char_t *time_str;
  pdf_error_t *error = NULL;
  pdf_time_t time_var;

  pdf_time_init (&time_var);

  /* Yeah, I know we shouldn't be using this here before unit-testing it, but
   * anyway... */
  time_str = pdf_time_to_string (&time_var,
                                 PDF_TIME_STRING_FORMAT_ISO_8601,
                                 PDF_TIME_STRING_NO_OPTION,
                                 &error);
  fail_unless (time_str != NULL);
  fail_if (error != NULL);

  fail_unless (memcmp (time_str, expected_time, strlen (expected_time)) == 0);

  pdf_dealloc (time_str);
  pdf_time_deinit (&time_var);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_time_init (void)
{
  TCase *tc = tcase_create ("pdf_time_init");

  tcase_add_test (tc, pdf_time_init_001);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-time-init.c */

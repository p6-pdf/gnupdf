/* -*- mode: C -*-
 *
 *       File:         pdf-set-error.c
 *       Date:         Fri Mar 04 23:48:29 2011
 *
 *       GNU PDF Library - Unit tests for pdf_set_error()
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

#include <stdio.h>
#include <pdf.h>
#include <check.h>
#include <pdf-test-common.h>

/*
 * Test: pdf_set_error_001
 * Description:
 *   Set an error with a fixed format string.
 * Success condition:
 *   The pdf_error_t is created, and its contens are the expected
 *   ones.
 */
START_TEST (pdf_set_error_001)
{
  const pdf_char_t *format = "Hello this is an error";
  pdf_error_t *error = NULL;

  pdf_init ();

  pdf_set_error (&error,
                 PDF_EDOMAIN_BASE_BASIC,
                 PDF_EAGAIN,
                 format);
  fail_if (!error);
  fail_if (pdf_error_get_status (error) != PDF_EAGAIN);
  fail_if (pdf_error_get_domain (error) != PDF_EDOMAIN_BASE_BASIC);
  fail_if (strcmp (pdf_error_get_message (error), format) != 0);

  pdf_error_destroy (error);
}
END_TEST

/*
 * Test: pdf_set_error_002
 * Description:
 *   Try to set an error with a fixed format string, but no error passed.
 * Success condition:
 *   Doesn't crash.
 */
START_TEST (pdf_set_error_002)
{
  const pdf_char_t *format = "Hello this is an error";

  pdf_init ();

  pdf_set_error (NULL,
                 PDF_EDOMAIN_BASE_BASIC,
                 PDF_EAGAIN,
                 format);
}
END_TEST

/*
 * Test: pdf_set_error_003
 * Description:
 *   Try to set an error with a printf-like message format and arguments.
 * Success condition:
 *   The pdf_error_t is created, and its contens are the expected
 *   ones.
 */
START_TEST (pdf_set_error_003)
{
  pdf_error_t *error = NULL;

  const pdf_char_t *format =
    "This is a message with a string: %s and "
    "a number: %d, and another unsigned long number: %lu";

  const pdf_char_t *string = "STRING";
  int number = -1234;
  unsigned long ulong = 123456789;

  const pdf_char_t *expected =
    "This is a message with a string: STRING and "
    "a number: -1234, and another unsigned long number: 123456789";

  pdf_init ();

  pdf_set_error (&error,
                 PDF_EDOMAIN_BASE_BASIC,
                 PDF_EAGAIN,
                 format,
                 string,
                 number,
                 ulong);
  fail_if (!error);
  fail_if (pdf_error_get_status (error) != PDF_EAGAIN);
  fail_if (pdf_error_get_domain (error) != PDF_EDOMAIN_BASE_BASIC);
  fail_if (strcmp (pdf_error_get_message (error), expected) != 0);

  pdf_error_destroy (error);
}
END_TEST

/*
 * Test: pdf_set_error_004
 * Description:
 *   Try to set an error with a printf-like message format and arguments, but no
 *   error passed.
 * Success condition:
 *   Doesn't crash.
 */
START_TEST (pdf_set_error_004)
{
  const pdf_char_t *format =
    "This is a message with a string: %s and "
    "a number: %d, and another unsigned long number: %lu";

  const pdf_char_t *string = "STRING";
  int number = -1234;
  unsigned long ulong = 123456789;

  pdf_init ();

  pdf_set_error (NULL,
                 PDF_EDOMAIN_BASE_BASIC,
                 PDF_EAGAIN,
                 format,
                 string,
                 number,
                 ulong);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_set_error (void)
{
  TCase *tc;

  tc = tcase_create("pdf_set_error");

  tcase_add_test(tc, pdf_set_error_001);
  tcase_add_test(tc, pdf_set_error_002);
  tcase_add_test(tc, pdf_set_error_003);
  tcase_add_test(tc, pdf_set_error_004);
  return tc;
}

/* End of pdf-set-error.c */

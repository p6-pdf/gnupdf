/* -*- mode: C -*-
 *
 *       File:         pdf-clear-error.c
 *       Date:         Fri Mar 04 23:57:00 2011
 *
 *       GNU PDF Library - Unit tests for pdf_clear_error()
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
 * Test: pdf_clear_error_001
 * Description:
 *   Clear a valid error.
 * Success condition:
 *   After clearing, error is NULL.
 */
START_TEST (pdf_clear_error_001)
{
  const pdf_char_t *format = "Hello this is an error";
  pdf_error_t *error;

  pdf_init ();

  error = pdf_error_new (PDF_EDOMAIN_BASE_BASIC,
                         PDF_EAGAIN,
                         format);
  pdf_clear_error (&error);

  fail_if (error != NULL);
}
END_TEST

/*
 * Test: pdf_clear_error_002
 * Description:
 *   Try to clear an error, but no error pointer given.
 * Success condition:
 *   Doesn't crash.
 */
START_TEST (pdf_clear_error_002)
{
  pdf_init ();

  pdf_clear_error (NULL);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_clear_error (void)
{
  TCase *tc;

  tc = tcase_create("pdf_clear_error");

  tcase_add_test(tc, pdf_clear_error_001);
  tcase_add_test(tc, pdf_clear_error_002);
  return tc;
}

/* End of pdf-clear-error.c */

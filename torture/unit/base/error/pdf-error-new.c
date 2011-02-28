/* -*- mode: C -*-
 *
 *       File:         pdf-error-new.c
 *       Date:         Mon Feb 28 19:57:49 2011
 *
 *       GNU PDF Library - Unit tests for pdf_error_new()
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
 * Test: pdf_error_new_001
 * Description:
 *   Create a pdf_error_t with a fixed format string.
 * Success condition:
 *   The pdf_error_t is created, and its contens are the expected
 *   ones.
 */
START_TEST (pdf_error_new_001)
{
  const pdf_char_t *format = "Hello this is an error";
  pdf_error_t *error;

  pdf_init ();

  error = pdf_error_new (PDF_EDOMAIN_BASE_BASIC,
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
 * Test case creation function
 */
TCase *
test_pdf_error_new (void)
{
  TCase *tc;

  tc = tcase_create("pdf_error_new");

  tcase_add_test(tc, pdf_error_new_001);
  return tc;
}

/* End of pdf-error-new.c */

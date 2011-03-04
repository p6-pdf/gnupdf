/* -*- mode: C -*-
 *
 *       File:         pdf-propagate-error.c
 *       Date:         Sat Mar 05 00:03:14 2011
 *
 *       GNU PDF Library - Unit tests for pdf_propagate_error()
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
 * Test: pdf_propagate_error_001
 * Description:
 *   Propagate a valid error.
 * Success condition:
 *   After propagateing, error is NULL.
 */
START_TEST (pdf_propagate_error_001)
{
  const pdf_char_t *format = "Hello this is an error";
  pdf_error_t *error;
  pdf_error_t *propagated = NULL;

  pdf_init ();

  error = pdf_error_new (PDF_EDOMAIN_BASE_BASIC,
                         PDF_EAGAIN,
                         format);

  pdf_propagate_error (&propagated, error);

  fail_if (!propagated);
  fail_if (pdf_error_get_status (propagated) != PDF_EAGAIN);
  fail_if (pdf_error_get_domain (propagated) != PDF_EDOMAIN_BASE_BASIC);
  fail_if (strcmp (pdf_error_get_message (propagated), format) != 0);

  pdf_error_destroy (propagated);
}
END_TEST

/*
 * Test: pdf_propagate_error_002
 * Description:
 *   Try to propagate an error, but no error pointer given.
 * Success condition:
 *   Doesn't crash.
 */
START_TEST (pdf_propagate_error_002)
{
  const pdf_char_t *format = "Hello this is an error";
  pdf_error_t *error;

  pdf_init ();

  error = pdf_error_new (PDF_EDOMAIN_BASE_BASIC,
                         PDF_EAGAIN,
                         format);

  pdf_propagate_error (NULL, error);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_propagate_error (void)
{
  TCase *tc;

  tc = tcase_create("pdf_propagate_error");

  tcase_add_test(tc, pdf_propagate_error_001);
  tcase_add_test(tc, pdf_propagate_error_002);
  return tc;
}

/* End of pdf-propagate-error.c */

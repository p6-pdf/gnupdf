/* -*- mode: C -*-
 *
 *       File:         pdf-time-copy.c
 *       Date:         Fri Feb 27 17:35:31 2008
 *
 *       GNU PDF Library - Unit tests for pdf_time_copy
 *
 */

/* Copyright (C) 2009-2011 Free Software Foundation, Inc. */

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
#include <stdint.h>
#include <check.h>
#include <pdf.h>
#include <stdlib.h>
#include <pdf-test-common.h>

/*
 * Test: pdf_time_copy_001
 * Description:
 *   Copy value of pdf_time_t object.
 * Success condition:
 *   1. Function pdf_time_new schould return PDF_OK.
 *   2. Function pdf_time_copy schould return PDF_OK.
 *   3. Structures pdf_time_s schould have the same data.
 */
START_TEST (pdf_time_copy_001)
{
  pdf_error_t *error = NULL;
  pdf_time_t *time1;
  pdf_time_t *time2;

  time1 = pdf_time_new (&error);
  fail_unless (time1 != NULL);
  fail_if (error != NULL);

  pdf_time_set_utc (time1, 1234567890);

  time2 = pdf_time_new (&error);
  fail_unless (time2 != NULL);
  fail_if (error != NULL);

  pdf_time_copy(time2, time1);

  fail_unless (pdf_time_cmp (time1, time2) == 0);

  pdf_time_destroy (time1);
  pdf_time_destroy (time2);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_time_copy (void)
{
  TCase *tc = tcase_create ("pdf_time_copy");

  tcase_add_test (tc, pdf_time_copy_001);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-time-copy.c */

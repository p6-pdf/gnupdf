/* -*- mode: C -*-
 *
 *       File:         pdf-time-add-span.c
 *       Date:         Fri Feb 27 17:35:31 2008
 *
 *       GNU PDF Library - Unit tests for pdf_time_add_span
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
#include <stdint.h>
#include <check.h>
#include <pdf.h>
#include <stdlib.h>
#include <pdf-test-common.h>

/*
 * Test: pdf_time_add_span
 * Description:
 *   Compare two pdf_time_t objects. First is
 *   initialized by adding pdf_time_span_t and second one
 *   by pdf_i64_t value. Both pdf_time_t objects
 *   must be equal.
 * Success condition:
 *   1. Function pdf_time_add_span schould return
 *      PDF_OK.
 *   2. Function pdf_time_cmp schould return 0.
 */
START_TEST (pdf_time_add_span_001)
{
  pdf_time_t time1;
  pdf_time_t time2;

  pdf_time_init (&time1);
  pdf_time_init (&time2);

  pdf_time_add_span (&time1, 1234567890);
  pdf_time_set_utc (&time2, 1234567890);

  fail_unless (pdf_time_cmp (&time1, &time2) == 0);

  pdf_time_deinit (&time1);
  pdf_time_deinit (&time2);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_time_add_span (void)
{
  TCase *tc = tcase_create ("pdf_time_add_span");

  tcase_add_test (tc, pdf_time_add_span_001);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-time-add-span.c */

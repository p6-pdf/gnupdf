/* -*- mode: C -*-
 *
 *       File:         pdf-time-span_new.c
 *       Date:         Fri Feb 27 17:35:31 2009
 *
 *       GNU PDF Library - Unit tests for pdf_time_span_new
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
#include <check.h>
#include <pdf.h>
#include <stdlib.h>
#include <pdf-test-common.h>
/*
 * Test: pdf_time_span_new_001
 * Description:
 *   Create new pdf_time_span_t. Get number of
 *   seconds in span. Number of seconds schould be
 *   0.
 *Success condition:
 * 1. Function pdf_time_span_new schould return PDF_OK
 * 2. pdf_time_span_t is set to 0.
 * 3. Fuction pdf_time_span_destroy schould return PDF_OK
 */
START_TEST (pdf_time_span_new_001)
{
  pdf_status_t status;
  pdf_i64_t sec;
  pdf_i64_t sec2;
  pdf_time_span_t span;

  pdf_init();

  span = pdf_time_span_new();

  sec = pdf_time_span_to_secs(span);
  sec2 = 0;

  fail_unless(memcmp(&sec,&sec2, sizeof(pdf_i64_t)) == 0);

  status = pdf_time_span_destroy(&span);
  fail_if(status != PDF_OK);

}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_time_span_new (void)
{
  TCase *tc = tcase_create ("pdf_time_span_new");

  tcase_add_test(tc, pdf_time_span_new_001);

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-time-span-new.c */

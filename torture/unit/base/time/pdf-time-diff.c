/* -*- mode: C -*-
 *
 *       File:         pdf-time-diff.c
 *       Date:         Fri Feb 27 17:35:31 2009
 *
 *       GNU PDF Library - Unit tests for pdf_time_diff
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
 * Test: pdf_time_diff_001
 * Description:
 *   Create two pdf_time_t objects and initialize
 *   them with the same value.
 *   Returned pdf_time_diff_t schould be equal to 0.
 *
 * Success condition:
 * 1. Function pdf_time_set_from_i64 schould return PDF_OK.
 * 2. Function pdf_time_diff  schould return PDF_OK.
 * 3. Diff returned by pdf_time_diff schould be equal to 0.
 */
START_TEST (pdf_time_diff_001)
{
  pdf_status_t status;
  pdf_time_t time1, time2;
  pdf_i64_t sec;
  pdf_i64_t sec2;
  pdf_time_span_t span;

  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);

  status = pdf_time_new(&time2);
  fail_if(status != PDF_OK);

  sec2 = INT64_C(0x0123456789ABCDEF);
  status = pdf_time_set_from_i64(time1, sec2);
  fail_if(status != PDF_OK);
  status = pdf_time_set_from_i64(time2, sec2);
  fail_if(status != PDF_OK);

  span = pdf_time_span_new();
  status = pdf_time_diff(time1, time2, &span);
  fail_if(status != PDF_OK);

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
test_pdf_time_diff (void)
{
  TCase *tc = tcase_create ("pdf_time_diff");

  tcase_add_test(tc, pdf_time_diff_001);

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-time-diff.c */

/* -*- mode: C -*-
 *
 *       File:         pdf-time-cal-span-cmp.c
 *       Date:         Fri Feb 27 17:35:31 2009
 *
 *       GNU PDF Library - Unit tests for pdf_time_cal_span_cmp
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
 * Test: pdf_time_cal_span_cmp_001
 * Description:
 *   Create two pdf_time_cal_span_s objects.
 *   First object has every field bigger then second
 *   one. Test makes fallowing sequences:
 *      1. Call pdf_time_cal_span_cmp
 *      2. swap arguments in pdf_time_cal_span_cmp
 *      2. Assign years field of first object to the
 *      value of the second object.
 *      3. until all fields are equal goto step1.
 * Success condition:
 *   1. Function pdf_time_cal_span_cmp schould return 1
 *   2. Function pdf_time_cal_span_cmp schould return -1
 *      after swapping arguments.
 *   3. Function pdf_time_cal_span_cmp schould return 0
 *      When both objects have all fields equal.
 */
START_TEST (pdf_time_cal_span_cmp_001)
{
  pdf_i64_t sec;
  pdf_u32_t i,j;
  pdf_time_t basetime;
  struct pdf_time_cal_span_s span1;
  struct pdf_time_cal_span_s span2;


  pdf_time_init (&basetime);

  pdf_time_set_utc (&basetime, 0x012FBBCC);

  span1.sign = PDF_FALSE;
  span1.years = 2;
  span1.months = 3;
  span1.days = 12;
  span1.hours = 10;
  span1.minutes = 31;
  span1.seconds = 12;

  span2.sign = PDF_FALSE;
  span2.years = 1;
  span2.months = 2;
  span2.days = 11;
  span2.hours = 9;
  span2.minutes = 30;
  span2.seconds = 10;

  fail_unless (pdf_time_cal_span_cmp (&span1, &span2, &basetime) > 0);
  /* Swap arguments */
  fail_unless (pdf_time_cal_span_cmp (&span2, &span1, &basetime) < 0);

  /* Make years field equal */
  span2.years = span1.years;
  fail_unless (pdf_time_cal_span_cmp(&span1, &span2, &basetime) > 0);
  /* Swap arguments */
  fail_unless (pdf_time_cal_span_cmp(&span2, &span1, &basetime) < 0);

  /* Make months field equal */
  span2.months = span1.months;
  fail_unless (pdf_time_cal_span_cmp (&span1, &span2, &basetime) > 0);
  /* Swap arguments */
  fail_unless (pdf_time_cal_span_cmp (&span2, &span1, &basetime) < 0);

  /* Make days field equal */
  span2.days = span1.days;
  fail_unless (pdf_time_cal_span_cmp (&span1, &span2, &basetime) > 0);
  /* Swap arguments */
  fail_unless (pdf_time_cal_span_cmp (&span2, &span1, &basetime) < 0);

  /* Make hours field equal */
  span2.hours = span1.hours;
  fail_unless (pdf_time_cal_span_cmp (&span1, &span2, &basetime) > 0);
  /* Swap arguments */
  fail_unless (pdf_time_cal_span_cmp (&span2, &span1, &basetime) < 0);

  /* Make minutes field equal */
  span2.minutes = span1.minutes;
  fail_unless (pdf_time_cal_span_cmp (&span1, &span2, &basetime) > 0);
  /* Swap arguments */
  fail_unless (pdf_time_cal_span_cmp (&span2, &span1, &basetime) < 0);

  /* Make seconds field equal, pdf_time_cal_span_s objects are equal now */
  span2.seconds = span1.seconds;
  fail_unless (pdf_time_cal_span_cmp (&span1, &span2, &basetime) == 0);
  /* Swap arguments */
  fail_unless (pdf_time_cal_span_cmp (&span2, &span1, &basetime) == 0);

  pdf_time_deinit (&basetime);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_time_cal_span_cmp (void)
{
  TCase *tc = tcase_create ("pdf_time_cal_span_cmp");

  tcase_add_test (tc, pdf_time_cal_span_cmp_001);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-time-cal-span-cmp.c */

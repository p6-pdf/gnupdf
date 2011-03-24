/* -*- mode: C -*-
 *
 *       File:         pdf-time-diff-cal.c
 *       Date:         Fri Feb 27 17:35:31 2008
 *
 *       GNU PDF Library - Unit tests for pdf_time_diff_cal
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
#include <time.h>
#include <string.h>
#include <base/time/pdf-time-test-common.h>
#include <pdf-test-common.h>

/*
 * Test: pdf_time_diff_cal_001
 * Description:
 *   Create two pdf_time_t objects.
 *   First is initialized with
 *   pdf_time_set_from_u32, and second one to the
 *   same number with function
 *   pdf_time_set_from_i64.
 *   seconds.
 *   Both object must be equal.
 *
 *Success condition:
 * 1. Function pdf_time_new schould return PDF_OK
 * 2. Function pdf_time_set_from_u32 schould return
 * PDF_OK.
 * 3. Function pdf_time_set_from_i64 schould return
 * PDF_OK.
 * 4. pdf_time_t objects are equal.
 *
 */
START_TEST (pdf_time_diff_cal_001)
{
  extern struct pdf_time_cal_span_s day_time_span[];
  extern  struct pdf_time_cal_span_s years_months[];
  extern pdf_u32_t days_in_seconds[];
  pdf_u32_t i;


  for (i = 0; i < YEAR_MONTH_SIZE; i++)
    {
      pdf_u32_t j;

      for(j = 0; j < DAY_TIME_SPAN_SIZE; j++)
        {
          pdf_time_t time1;
          pdf_time_t time2;
          struct pdf_time_cal_span_s calspan = { 0 };
          struct pdf_time_cal_span_s calspan2 = { 0 };
          pdf_u32_t seconds;


          pdf_time_init (&time1);
          pdf_time_init (&time2);


          calspan.sign = PDF_TRUE;

          calspan.years = years_months[i].years;
          calspan.months = years_months[i].months;

          calspan.days = day_time_span[j].days;
          calspan.hours = day_time_span[j].hours;
          calspan.minutes = day_time_span[j].minutes;
          calspan.seconds = day_time_span[j].seconds;

          seconds = days_in_seconds[i];
          seconds += day_time_span[j].days* SEC_IN_DAY;
          seconds += day_time_span[j].hours * 3600;
          seconds += day_time_span[j].minutes * 60;
          seconds += day_time_span[j].seconds;

          pdf_time_set_utc (&time1, seconds);
          pdf_time_diff_cal (&time2, &time1, &calspan2);

          fail_unless (memcmp (&calspan,
                               &calspan2,
                               sizeof (struct pdf_time_cal_span_s)) == 0);

          pdf_time_deinit (&time1);
          pdf_time_deinit (&time2);
        }
    }
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_time_diff_cal (void)
{
  TCase *tc = tcase_create ("pdf_time_diff_cal");

  tcase_add_test (tc, pdf_time_diff_cal_001);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-time-diff-cal.c */

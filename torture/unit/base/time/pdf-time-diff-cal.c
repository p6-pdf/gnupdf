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
  pdf_status_t status;
  pdf_time_t time1;
  pdf_time_t time2;
  pdf_u32_t i,j;
  pdf_u32_t seconds;

  extern struct pdf_time_cal_span_s day_time_span[];
  extern  struct pdf_time_cal_span_s years_months[];
  extern pdf_u32_t daysInSeconds[];
  struct pdf_time_cal_span_s calspan, calspan2;

  pdf_init();

  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);

  status = pdf_time_new(&time2);
  fail_if(status != PDF_OK);


  calspan.sign = PDF_TRUE;
  memset(&calspan,0,sizeof(struct pdf_time_cal_span_s));
  memset(&calspan2,0,sizeof(struct pdf_time_cal_span_s));

  for (i=0;i<YEAR_MONTH_SIZE;i++){
    for(j=0;j<DAY_TIME_SPAN_SIZE; j++){

        calspan.years = years_months[i].years;
        calspan.months = years_months[i].months;

        calspan.days = day_time_span[j].days;
        calspan.hours = day_time_span[j].hours;
        calspan.minutes = day_time_span[j].minutes;
        calspan.seconds = day_time_span[j].seconds;


        seconds = daysInSeconds[i];
        seconds += day_time_span[j].days* SEC_IN_DAY;
        seconds += day_time_span[j].hours * 3600;
        seconds += day_time_span[j].minutes * 60;
        seconds += day_time_span[j].seconds;


        status = pdf_time_set_from_u32(time1, seconds);
        fail_if(status != PDF_OK);
        status = pdf_time_diff_cal(time2, time1, &calspan2);
        fail_if(status != PDF_OK);

        fail_unless(memcmp(&calspan, &calspan2, sizeof(struct pdf_time_cal_span_s)) == 0 );


        status = pdf_time_clear(time2);
        fail_if(status != PDF_OK);

    }
  }

  status = pdf_time_destroy(time1);
  fail_if(status != PDF_OK);

  status = pdf_time_destroy(time2);
  fail_if(status != PDF_OK);


}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_time_diff_cal (void)
{
  TCase *tc = tcase_create ("pdf_time_diff_cal");

  tcase_add_test(tc, pdf_time_diff_cal_001);


  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-time-diff-cal.c */

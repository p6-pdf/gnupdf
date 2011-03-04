/* -*- mode: C -*-
 *
 *       File:         pdf-time-add-cal-span.c
 *       Date:         Sun Sep 21 16:37:27 2008
 *
 *       GNU PDF Library - Unit tests for pdf_time_sub_cal_span
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
#include <base/time/pdf-time-test-common.h>
#include <pdf-test-common.h>
#define INTERACTIVE_DEBUG 0

/*
 * Test: pdf_time_sub_cal_span_001
 * Description:
 *   Create two pdf_time_t objects.
 *   First is initialized with
 *   pdf_time_set_from_u32. Then sub cal span from
 *   that object. Returned time object schould
 *   points to 1970-01-01:00:00:00
 *
 * Success condition:
 *  1. Function pdf_time_new schould return PDF_OK
 *  2. Function pdf_time_set_from_u32 schould return
 *     PDF_OK.
 *  3. Function pdf_time_sub_cal_span schould return
 *     expected time.
 *
 */
START_TEST (pdf_time_sub_cal_span_001)
{
  pdf_status_t status;
  pdf_time_t time1;
  pdf_time_t time2;

  pdf_u32_t i,j;
  pdf_u32_t seconds;

  extern struct pdf_time_cal_span_s day_time_span[];
  extern  struct pdf_time_cal_span_s years_months[];
  extern pdf_u32_t daysInSeconds[];
  struct pdf_time_cal_span_s calspan;




  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);

  status = pdf_time_new(&time2);
  fail_if(status != PDF_OK);

  calspan.sign = PDF_FALSE;

  for (i=0;i<YEAR_MONTH_SIZE;i++){
    for(j=0;j<1; j++){
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

        struct pdf_time_cal_s cal;
        if (INTERACTIVE_DEBUG){
            printf("pdf_time_sub_cal_span_001 %d %d\n",i, j);
            printf("pdf_time_sub_cal_span_001 seconds=%d , calspan: %d-%d-%dT%d:%d:%d\n",seconds,
                    calspan.years, calspan.months, calspan.days, calspan.hours, calspan.minutes, calspan.seconds);


            pdf_time_get_utc_cal(time1, &cal);
            printf("pdf_time_sub_cal_span_001 time1 calendar %d-%d-%dT%d:%d:%d\n",
                    cal.year, cal.month, cal.day, cal.hour, cal.minute, cal.second);
        }

        status = pdf_time_sub_cal_span(time1, &calspan);
        fail_if(status != PDF_OK);

        if(INTERACTIVE_DEBUG){
            pdf_time_get_utc_cal(time1, &cal);
            printf("pdf_time_sub_cal_span_001 time1 after sub %d-%d-%dT%d:%d:%d\n",
                    cal.year, cal.month, cal.day, cal.hour, cal.minute, cal.second);
        }

        fail_unless(pdf_time_cmp(time1, time2) == 0);

        status = pdf_time_clear(time1);
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
test_pdf_time_sub_cal_span (void)
{
  TCase *tc = tcase_create ("pdf_time_sub_cal_span");

  tcase_add_test(tc, pdf_time_sub_cal_span_001);

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-time-sub-cal-span.c */

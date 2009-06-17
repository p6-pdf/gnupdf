/* -*- mode: C -*- Time-stamp: "09/04/17 00:05:32 jemarch"
 *
 *       File:         pdf-time-from-cal.c
 *       Date:         Fri Feb 27 17:35:31 2008
 *
 *       GNU PDF Library - Unit tests for pdf_time_from_cal
 *
 */

/* Copyright (C) 2009 Free Software Foundation, Inc. */

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

/*
 * Test: pdf_time_from_cal_001
 * Description:
 * Get pdf_time_t object from calendar object. Set
 * calendar date after 00:00:00 01.01.1970.
 *Success condition:
 * 1. pdf_time_set_from_u32 schould return PDF_OK.
 * 2. pdf_time_from_cal schould return PDF_OK.
 * 3. pdf_time_t objects schould be the same.
 */
START_TEST (pdf_time_from_cal_001)
{
  pdf_status_t status;
  pdf_time_t time1;
  pdf_time_t time2;
  pdf_u32_t i;
  pdf_i32_t gmt;
  pdf_u32_t seconds;
  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t datesInSeconds[];
  struct pdf_time_cal_s calendar;

  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  fail_if(time1 == NULL);

  status = pdf_time_new(&time2);
  fail_if(status != PDF_OK);
  fail_if(time2 == NULL);

  for (i=0; i<DATES_SIZE; i++){
        for (gmt =-12*3600; gmt <=12*3600; gmt+=1800){  /* Set various gmt_offsets. */

            seconds=datesInSeconds[i] -gmt;

            if (seconds +gmt < gmt) continue;

            pdf_time_set_from_u32(time1,seconds);

            calendar.year = dates[i].year;
            calendar.month= dates[i].month;
            calendar.day = dates[i].day;
            calendar.dow = dates[i].dow;
            calendar.hour = dates[i].hour;
            calendar.minute = dates[i].minute;
            calendar.second = dates[i].second;
            calendar.gmt_offset = +gmt;

            status = pdf_time_from_cal(time2, &calendar);
            fail_if(status != PDF_OK);
            fail_unless(pdf_time_cmp(time1, time2) == 0);
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
test_pdf_time_from_cal (void)
{
  TCase *tc = tcase_create ("pdf_time_from_cal");

  tcase_add_test(tc, pdf_time_from_cal_001);


  return tc;
}

/* End of pdf-time-from-cal.c */

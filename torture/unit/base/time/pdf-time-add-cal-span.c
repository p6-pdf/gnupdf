/* -*- mode: C -*-
 *
 *       File:         pdf-time-add-cal-span.c
 *       Date:         Sun Sep 21 16:37:27 2008
 *
 *       GNU PDF Library - Unit tests for pdf_time_add_cal_span
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

/*
 * Test: pdf_time_add_cal_span_001
 * Description:
 *   Create two pdf_time_t objects.
 *   First is initialized with
 *   pdf_time_set_from_u32 and second one to the
 *   same number with pdf_time_cal_span_s. Both
 *   objects schould be equal.
 *
 * Success conditions:
 *   1. Function pdf_time_new schould return PDF_OK
 *   2. Function pdf_time_set_from_u32 schould return
 *      PDF_OK.
 *   3. Function pdf_time_add_cal_span schould return
 *      PDF_OK.
 *   4. pdf_time_t objects are equal.
 */
START_TEST (pdf_time_add_cal_span_001)
{
  extern struct pdf_time_cal_span_s day_time_span[];
  extern struct pdf_time_cal_span_s years_months[];
  extern pdf_u32_t days_in_seconds[];

  pdf_time_t time1;
  pdf_time_t time2;
  pdf_u32_t i;
  pdf_u32_t seconds;

  pdf_time_init (&time1);
  pdf_time_init (&time2);

  for (i = 0; i < YEAR_MONTH_SIZE; i++)
    {
      pdf_u32_t j;

      for (j = 0; j < DAY_TIME_SPAN_SIZE; j++)
        {
          struct pdf_time_cal_span_s calspan;

          seconds = days_in_seconds[i];
          seconds += day_time_span[j].days * SEC_IN_DAY;
          seconds += day_time_span[j].hours * 3600;
          seconds += day_time_span[j].minutes * 60;
          seconds += day_time_span[j].seconds;
          pdf_time_set_utc (&time1, seconds);

          calspan.sign = PDF_TRUE;
          calspan.years = years_months[i].years;
          calspan.months = years_months[i].months;
          calspan.days = day_time_span[j].days;
          calspan.hours = day_time_span[j].hours;
          calspan.minutes = day_time_span[j].minutes;
          calspan.seconds = day_time_span[j].seconds;
          pdf_time_clear (&time2);
          pdf_time_add_cal_span (&time2, &calspan);

#ifdef TIME_MODULE_ADDITIONAL_TEST_TRACES
            {
              struct pdf_time_cal_s cal1;
              struct pdf_time_cal_s cal2;

              pdf_time_get_utc_cal (&time1, &cal1);
              pdf_time_get_utc_cal (&time2, &cal2);

              printf ("\n"
                      "pdf_time_add_cal_span_001[%u,%u] (seconds) %d\n",
                      i, j, seconds);

              printf ("pdf_time_add_cal_span_001[%u,%u] (cal1)    %d-%d-%d %d:%d:%d\n",
                      i, j,
                      cal1.year, cal1.month, cal1.day,
                      cal1.hour, cal1.minute, cal1.second);

              printf ("pdf_time_add_cal_span_001[%u,%u] (calspan) %d-%d-%d %d:%d:%d\n",
                      i, j,
                      calspan.years, calspan.months, calspan.days,
                      calspan.hours, calspan.minutes, calspan.seconds);
              printf ("pdf_time_add_cal_span_001[%u,%u] (cal2)    %d-%d-%d %d:%d:%d\n",
                      i, j,
                      cal2.year, cal2.month, cal2.day,
                      cal2.hour, cal2.minute, cal2.second);
              fflush (stdout);
            }
#endif /* TIME_MODULE_ADDITIONAL_TEST_TRACES */

            fail_unless (pdf_time_cmp (&time1, &time2) == 0);
        }
    }

  pdf_time_deinit (&time1);
  pdf_time_deinit (&time2);
}
END_TEST

/*
 * Test: pdf_time_add_cal_span_002
 * Description:
 *   Create pdf_time_t objects and add to them a
 *   pdf_time_cap_span_s initialized to 1 month and
 *   27, 28, 29, 30 days.
 *   All objcts schould be equal.
 * Success conditions:
 *   1. Function pdf_time_new schould return PDF_OK
 *   2. Function pdf_time_add_cal_span schould return
 *      PDF_OK.
 *   3. pdf_time_t objects are equal.
 */
START_TEST (pdf_time_add_cal_span_002)
{
  pdf_time_t time27, time28,time29, time30;
  struct pdf_time_cal_span_s calspan;
  struct pdf_time_cal_s cal;

  pdf_time_init (&time27);
  pdf_time_init (&time28);
  pdf_time_init (&time29);
  pdf_time_init (&time30);

  /* Init cal_span structure */
  calspan.years = 0;
  calspan.months = 1;
  calspan.hours = 0;
  calspan.minutes = 0;
  calspan.seconds = 0;

  calspan.days = 27;
  pdf_time_add_cal_span (&time27, &calspan);
  calspan.days = 28;
  pdf_time_add_cal_span (&time28, &calspan);
  calspan.days = 29;
  pdf_time_add_cal_span (&time29, &calspan);
  calspan.days = 30;
  pdf_time_add_cal_span (&time30, &calspan);

  fail_unless (pdf_time_cmp (&time27, &time28) == 0);
  fail_unless (pdf_time_cmp (&time28, &time29) == 0);
  fail_unless (pdf_time_cmp (&time29, &time30) == 0);

  pdf_time_deinit (&time27);
  pdf_time_deinit (&time28);
  pdf_time_deinit (&time29);
  pdf_time_deinit (&time30);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_time_add_cal_span (void)
{
  TCase *tc = tcase_create ("pdf_time_add_cal_span");

  tcase_add_test (tc, pdf_time_add_cal_span_001);
  tcase_add_test (tc, pdf_time_add_cal_span_002);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-time-add-cal-span.c */

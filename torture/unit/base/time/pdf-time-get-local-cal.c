/* -*- mode: C -*-
 *
 *       File:         pdf-time-get-local-cal.c
 *       Date:         Fri Feb 27 17:35:31 2008
 *
 *       GNU PDF Library - Unit tests for pdf_time_new
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
#include <base/time/pdf-time-test-common.h>
#include <pdf-test-common.h>

/*
 * Test: pdf_time_get_local_cal_001
 * Description:
 *   Create new pdf_time_t and initialize it with a
 *   date in utc. Returned localcal schould be
 *   expected one.
 *
 *Success condition:
 * 1. Function pdf_time_new schould return PDF_OK
 * 2. Function pdf_time_set_from_u32 schould return
 * PDF_OK.
 * 3. Function pdf_time_get_local_cal schould return
 * PDF_OK.
 * 4. Returned localcal must be expected one.
 */
START_TEST (pdf_time_get_local_cal_001)
{
  extern pdf_u32_t dates_in_seconds[];
  extern struct pdf_time_cal_s dates[];
  pdf_time_t time1;
  pdf_u32_t i;

  pdf_time_init (&time1);

  for (i = 0; i < DATES_SIZE; i++)
    {
      struct pdf_time_cal_s localcal;

      pdf_time_set_utc (&time1, dates_in_seconds[i]);
      pdf_time_get_local_cal (&time1, &localcal);
      fail_unless (memcmp (&localcal,
                           &dates[i],
                           sizeof (struct pdf_time_cal_s)) == 0);
    }

  pdf_time_deinit (&time1);
}
END_TEST

/*
 * Test: pdf_time_get_local_cal_002
 * Description:
 *   Create new pdf_time_t and initialize it with a
 *   date in utc. Returned localcal schould be
 *   expected one.
 *
 *Success condition:
 * 1. Function pdf_time_new schould return PDF_OK
 * 2. Function pdf_time_set_from_u32 schould return
 * PDF_OK.
 * 3. Function pdf_time_get_local_cal schould return
 * PDF_OK.
 * 4. Returned localcal must be expected one.
 */
START_TEST (pdf_time_get_local_cal_002)
{
  extern pdf_u32_t dates_in_seconds[];
  extern struct pdf_time_cal_s dates[];
  pdf_time_t time1;
  struct tm *time_struct;
  time_t tloc;
  pdf_i32_t localgmt;
  pdf_u32_t i;


  time (&tloc);
  time_struct = localtime (&tloc);

#if defined PDF_HOST_WIN32
  localgmt = _timezone;
#else
  localgmt = time_struct->tm_gmtoff;
#endif

  pdf_time_init (&time1);

  for (i = 0; i < DATES_SIZE; i++)
    {
      struct pdf_time_cal_s localcal;

      if (dates_in_seconds[i] < localgmt)
        continue;

      pdf_time_set_utc (&time1,
                        dates_in_seconds[i] - localgmt);
      pdf_time_set_local_offset (&time1);

      pdf_time_get_local_cal (&time1, &localcal);

      /* Compare offsets */
      fail_unless (localcal.gmt_offset == localgmt);

      /* Reset offset and compare structs */
      localcal.gmt_offset = 0;
      fail_unless (memcmp (&localcal,
                           &dates[i],
                           sizeof (struct pdf_time_cal_s)) == 0);

    }

  pdf_time_deinit (&time1);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_time_get_local_cal (void)
{
  TCase *tc = tcase_create ("pdf_time_get_local_cal");

  tcase_add_test (tc, pdf_time_get_local_cal_001);
  tcase_add_test (tc, pdf_time_get_local_cal_002);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-time-get-local-cal.c */

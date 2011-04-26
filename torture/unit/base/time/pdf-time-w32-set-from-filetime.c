/* -*- mode: C -*-
 *
 *       File:         pdf-time-w32-set-from-filetime.c
 *       Date:         Sun Sep 21 16:37:27 2008
 *
 *       GNU PDF Library - Unit tests for pdf_time_w32_set_from_filetime
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
 * Test: pdf_time_w32_set_from_filetime_001
 * Description:
 *   Invoke pdf_time_w32_set_from_filetime with
 *   specialy prepared FILETIME structure and check if
 *   returned pdf_time_t object points to right date.
 * Success condition:
 *   1. Function pdf_time_w32_set_from_filetime
 *      schould return PDF_OK.
 *   2. Function pdf_time_from_cal schould return
 *      PDF_OK;
 *   3. Function pdf_time_cmp schould return 0.
 */
START_TEST (pdf_time_w32_set_from_filetime_001)
{
#ifdef PDF_HOST_WIN32
  pdf_error_t *error = NULL;
  FILETIME fileTime;
  pdf_time_t time1;
  pdf_time_t time2;
  struct pdf_time_cal_s calendar;

  pdf_time_init (&time1);
  pdf_time_init (&time2);

  fileTime.dwLowDateTime = 0x8EA6C2F0;
  fileTime.dwHighDateTime = 0x01C9B81D;

  fail_unless (pdf_time_w32_set_from_filetime (&time1,
                                               &fileTime,
                                               &error) == PDF_TRUE);
  fail_if (error != NULL);

  calendar.year = 2009;
  calendar.month = 4;
  calendar.dow = 3;
  calendar. day = 8;
  calendar.hour = 7;
  calendar.minute = 42;
  calendar.second = 24;
  calendar.gmt_offset = 0;
  pdf_time_set_from_cal (&time2, &calendar);

  fail_unless (pdf_time_cmp (&time, &time2) == 0);

  pdf_time_deinit (&time1);
  pdf_time_deinit (&time2);
#endif /* PDF_HOST_WIN32 */
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_time_w32_set_from_filetime (void)
{
  TCase *tc = tcase_create ("pdf_time_w32_set_from_filetime");

  tcase_add_test (tc, pdf_time_w32_set_from_filetime_001);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-time-w32-set-from-filetime.c */

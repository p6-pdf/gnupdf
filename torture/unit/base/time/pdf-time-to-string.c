/* -*- mode: C -*-
 *
 *       File:         pdf-time-to-string.c
 *       Date:         Fri Feb 27 17:35:31 2008
 *
 *       GNU PDF Library - Unit tests for pdf_time_to_string
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
#include <stdio.h>
#include <string.h>
#include <base/time/pdf-time-test-common.h>
#include <pdf-test-common.h>

/*
 * Test: pdf_time_to_string_001
 * Description:
 *   Get string representation of pdf_time_t object in
 *   ISO_8601 format:  YYYY-MM-DDThh:mm:ss+hh:mm.
 *Success condition:
 * 1. Function pdf_time_to_string schould return
 * string in right format.
 * 2. Returned string schould be expected one.
 */
START_TEST (pdf_time_to_string_001)
{
  pdf_u32_t i;

  extern struct pdf_time_cal_s dates[];
  struct pdf_time_cal_s calendar;

  for (i = 0; i < DATES_SIZE; i++)
    {
      pdf_i32_t gmt;

      /* We won't test with strings in 1970-01-01 */
      if (dates[i].year == 1970 &&
          dates[i].month == 1 &&
          dates[i].day == 1)
        continue;

      for (gmt = -12 * 60; gmt <= 12 * 60; gmt += 30)
        {
          pdf_error_t *error = NULL;
          pdf_i32_t offset_hours;
          pdf_i32_t offset_minutes;
          pdf_time_t time1;
          pdf_char_t *test_string;
          pdf_char_t *date_string;

          pdf_time_init (&time1);

          /* YYYY-MM-DDThh:mm:ss+hh:mm  - lenght = 26  */
          test_string = pdf_alloc (26);
          memset (&test_string[0], 0, 26);

          calendar.year = dates[i].year;
          calendar.month = dates[i].month;
          calendar.day = dates[i].day;
          calendar.hour = dates[i].hour;
          calendar.minute = dates[i].minute;
          calendar.second = dates[i].second;
          calendar.dow = dates[i].dow;
          calendar.gmt_offset = gmt * 60;

#ifdef TIME_MODULE_ADDITIONAL_TEST_TRACES
          printf ("pdf_time_to_string_001 [%u,%+.3d] '%d-%d-%d %d:%d:%d'\n",
                  i, gmt,
                  calendar.year, calendar.month, calendar.day,
                  calendar.hour, calendar.minute, calendar.second);
#endif /* TIME_MODULE_ADDITIONAL_TEST_TRACES */

          pdf_time_set_from_cal (&time1, &calendar);

          offset_hours = (((gmt < 0) ? (-1) : (1)) * gmt) / 60;
          offset_minutes = (((gmt < 0) ? (-1) : (1)) *gmt ) % 60;

          sprintf (&test_string[0],
                   "%d-%02d-%02dT%02d:%02d:%02d%c%02d:%02d",
                   dates[i].year, dates[i].month, dates[i].day,
                   dates[i].hour, dates[i].minute, dates[i].second,
                   ((gmt < 0) ? '-' : '+'),
                   offset_hours, offset_minutes);

          if (gmt == 0)
            {
              /** Test YYYY-MM-DDThh:mm:ssZ format **/
              test_string[19] = 'Z';
              test_string[20] = '\0';
            }

#ifdef TIME_MODULE_ADDITIONAL_TEST_TRACES
          printf ("                       [%u,%+.3d] '%s'\n",
                  i, gmt, &test_string[0]);
#endif /* TIME_MODULE_ADDITIONAL_TEST_TRACES */

          date_string = pdf_time_to_string (&time1,
                                            PDF_TIME_STRING_FORMAT_ISO_8601,
                                            PDF_TIME_STRING_NO_OPTION,
                                            &error);
          fail_unless (date_string != NULL);
          fail_if (error != NULL);

#ifdef TIME_MODULE_ADDITIONAL_TEST_TRACES
          printf ("                       [%u,%+.3d] '%s'\n",
                  i, gmt, &date_string[0]);
#endif /* TIME_MODULE_ADDITIONAL_TEST_TRACES */

          fail_unless (strcmp (test_string, date_string) == 0);

          pdf_dealloc (date_string);
          pdf_dealloc (test_string);
        }
    }
}
END_TEST

/*
 * Test: pdf_time_to_string_002
 * Description:
 *   Get string representation of pdf_time_t object in
 *   PDF format:
 *   D:YYYYMMDDHHmmSSOHH'mm'
 *Success condition:
 * 1. Function pdf_time_to_string schould return
 * string in right format.
 * 2. Returned string schould be expected one.
 */
START_TEST (pdf_time_to_string_002)
{
  pdf_u32_t i;

  extern struct pdf_time_cal_s dates[];
  struct pdf_time_cal_s calendar;

  for (i = 0; i < DATES_SIZE; i++)
    {
      pdf_i32_t gmt;

      /* We won't test with strings in 1970-01-01 */
      if (dates[i].year == 1970 &&
          dates[i].month == 1 &&
          dates[i].day == 1)
        continue;

      for (gmt = -12 * 60; gmt <= 12 * 60; gmt += 30)
        {
          pdf_error_t *error = NULL;
          pdf_i32_t offset_hours;
          pdf_i32_t offset_minutes;
          pdf_time_t time1;
          pdf_char_t *date_string;
          pdf_char_t *date_string_na;
          pdf_char_t *test_string;
          pdf_char_t *test_string_na;

          pdf_time_init (&time1);

          /* D:YYYYMMDDHHmmSSOHH'mm' - lenght = 24  */
          test_string = pdf_alloc (24);
          memset (&test_string[0], 0, 24);

          /* D:YYYYMMDDHHmmSSOHH'mm - length = 23 */
          test_string_na = pdf_alloc (23);
          memset (&test_string_na[0], 0, 23);

          calendar.year = dates[i].year;
          calendar.month = dates[i].month;
          calendar.day = dates[i].day;
          calendar.hour = dates[i].hour;
          calendar.minute = dates[i].minute;
          calendar.second = dates[i].second;
          calendar.dow = dates[i].dow;
          calendar.gmt_offset = gmt * 60;

#ifdef TIME_MODULE_ADDITIONAL_TEST_TRACES
          printf ("pdf_time_to_string_002 [%u,%+.3d] '%d-%d-%d %d:%d:%d'\n",
                  i, gmt,
                  calendar.year, calendar.month, calendar.day,
                  calendar.hour, calendar.minute, calendar.second);
#endif /* TIME_MODULE_ADDITIONAL_TEST_TRACES */

          pdf_time_set_from_cal (&time1, &calendar);

          offset_hours = (((gmt < 0) ? (-1) : (1)) * gmt) / 60;
          offset_minutes = (((gmt < 0) ? (-1) : (1)) *gmt ) % 60;

          sprintf (&test_string[0],
                   "D:%d%02d%02d%02d%02d%02d%c%02d'%02d'",
                   dates[i].year, dates[i].month, dates[i].day,
                   dates[i].hour, dates[i].minute, dates[i].second,
                   ((gmt < 0) ? '-' : '+'),
                   offset_hours, offset_minutes);

          if (gmt == 0)
            {
              /** Test D:YYYYMMDDHHmmSSZ format **/
              test_string[16] = 'Z';
              test_string[17] = '\0';
            }

#ifdef TIME_MODULE_ADDITIONAL_TEST_TRACES
          printf ("                       [%u,%+.3d] '%s'\n",
                  i, gmt, &test_string[0]);
#endif /* TIME_MODULE_ADDITIONAL_TEST_TRACES */

          date_string = pdf_time_to_string (&time1,
                                            PDF_TIME_STRING_FORMAT_PDF,
                                            PDF_TIME_STRING_TRAILING_APOSTROPHE,
                                            &error);
          fail_unless (date_string != NULL);
          fail_if (error != NULL);

#ifdef TIME_MODULE_ADDITIONAL_TEST_TRACES
          printf ("                       [%u,%+.3d] '%s'\n",
                  i, gmt, &date_string[0]);
#endif /* TIME_MODULE_ADDITIONAL_TEST_TRACES */

          fail_unless (strcmp (test_string, date_string) == 0);

          sprintf (&test_string_na[0],
                   "D:%d%02d%02d%02d%02d%02d%c%02d'%02d",
                   dates[i].year, dates[i].month, dates[i].day,
                   dates[i].hour, dates[i].minute, dates[i].second,
                   ((gmt < 0) ? '-' : '+'),
                   offset_hours, offset_minutes);

          if (gmt == 0)
            {
              /** Test D:YYYYMMDDHHmmSSZ format **/
              test_string_na[16] = 'Z';
              test_string_na[17] = '\0';
            }

#ifdef TIME_MODULE_ADDITIONAL_TEST_TRACES
          printf ("                       [%u,%+.3d] '%s'\n",
                  i, gmt, &test_string_na[0]);
#endif /* TIME_MODULE_ADDITIONAL_TEST_TRACES */

          date_string_na = pdf_time_to_string (&time1,
                                               PDF_TIME_STRING_FORMAT_PDF,
                                               PDF_TIME_STRING_NO_OPTION,
                                               &error);
          fail_unless (date_string_na != NULL);
          fail_if (error != NULL);

#ifdef TIME_MODULE_ADDITIONAL_TEST_TRACES
          printf ("                       [%u,%+.3d] '%s'\n",
                  i, gmt, &date_string_na[0]);
#endif /* TIME_MODULE_ADDITIONAL_TEST_TRACES */

          fail_unless (strcmp (test_string_na, date_string_na) == 0);

          pdf_dealloc (date_string);
          pdf_dealloc (date_string_na);
          pdf_dealloc (test_string);
          pdf_dealloc (test_string_na);
        }
    }
}
END_TEST

/*
 * Test: pdf_time_to_string_003
 * Description:
 *   Get string representation of pdf_time_t object in
 *   PDF_TIME_FORMAT_GENERALIZED_ASN1 format:
 *   YYYYMMDDhhmmssTZD.
 *Success condition:
 * 1. Function pdf_time_to_string schould return
 * string in right format.
 * 2. Returned string schould be expected one.

 */
START_TEST (pdf_time_to_string_003)
{
  pdf_u32_t i;

  extern struct pdf_time_cal_s dates[];
  struct pdf_time_cal_s calendar;

  for (i = 0; i < DATES_SIZE; i++)
    {
      pdf_i32_t gmt;

      /* We won't test with strings in 1970-01-01 */
      if (dates[i].year == 1970 &&
          dates[i].month == 1 &&
          dates[i].day == 1)
        continue;

      for (gmt = -12 * 60; gmt <= 12 * 60; gmt += 30)
        {
          pdf_error_t *error = NULL;
          pdf_i32_t offset_hours;
          pdf_i32_t offset_minutes;
          pdf_time_t time1;
          pdf_char_t *test_string;
          pdf_char_t *date_string;

          pdf_time_init (&time1);

          /* YYYYMMDDhhmmss+hhmm  - lenght = 20 */
          test_string = pdf_alloc (20);
          memset (&test_string[0], 0, 20);

          calendar.year = dates[i].year;
          calendar.month = dates[i].month;
          calendar.day = dates[i].day;
          calendar.hour = dates[i].hour;
          calendar.minute = dates[i].minute;
          calendar.second = dates[i].second;
          calendar.dow = dates[i].dow;
          calendar.gmt_offset = gmt * 60;

#ifdef TIME_MODULE_ADDITIONAL_TEST_TRACES
          printf ("pdf_time_to_string_003 [%u,%+.3d] '%d-%d-%d %d:%d:%d'\n",
                  i, gmt,
                  calendar.year, calendar.month, calendar.day,
                  calendar.hour, calendar.minute, calendar.second);
#endif /* TIME_MODULE_ADDITIONAL_TEST_TRACES */

          pdf_time_set_from_cal (&time1, &calendar);

          offset_hours = (((gmt < 0) ? (-1) : (1)) * gmt) / 60;
          offset_minutes = (((gmt < 0) ? (-1) : (1)) *gmt ) % 60;

          sprintf (&test_string[0],
                   "%d%02d%02d%02d%02d%02d%c%02d%02d",
                   dates[i].year, dates[i].month, dates[i].day,
                   dates[i].hour, dates[i].minute,dates[i].second,
                   ((gmt < 0) ? '-' : '+'),
                   offset_hours, offset_minutes);

          if (gmt == 0)
            {
                /** Test YYYYMMDDhhmmssZ format **/
                test_string[14] = 'Z';
                test_string[15] = '\0';
            }

#ifdef TIME_MODULE_ADDITIONAL_TEST_TRACES
          printf ("pdf_time_to_string_003 [%u,%+.3d] '%s'\n",
                  i, gmt, &test_string[0]);
#endif /* TIME_MODULE_ADDITIONAL_TEST_TRACES */

          date_string = pdf_time_to_string (&time1,
                                            PDF_TIME_STRING_FORMAT_GENERALIZED_ASN1,
                                            PDF_TIME_STRING_NO_OPTION,
                                            &error);
          fail_unless (date_string != NULL);
          fail_if (error != NULL);

#ifdef TIME_MODULE_ADDITIONAL_TEST_TRACES
          printf ("                       [%u,%+.3d] '%s'\n",
                  i, gmt, &date_string[0]);
#endif /* TIME_MODULE_ADDITIONAL_TEST_TRACES */

          fail_unless (strcmp (test_string, date_string) == 0);

          pdf_dealloc (date_string);
          pdf_dealloc (test_string);
        }
    }
}
END_TEST

/*
 * Test: pdf_time_to_string_004
 * Description:
 *   Get string representation of pdf_time_t object in
 *   PDF_TIME_FORMAT_UTC_ASN1 format:
 *   yymmddhhmmss+hhmm
 *Success condition:
 * 1. Function pdf_time_to_string schould return
 * string in right format.
 * 2. Returned string schould be expected one.
 */
START_TEST (pdf_time_to_string_004)
{
  pdf_u32_t i;

  extern struct pdf_time_cal_s dates[];
  struct pdf_time_cal_s calendar;

  for (i = 0; i < DATES_SIZE; i++)
    {
      pdf_i32_t gmt;

      /* We won't test with strings in 1970-01-01 */
      if (dates[i].year == 1970 &&
          dates[i].month == 1 &&
          dates[i].day == 1)
        continue;

      for (gmt = -12 * 60; gmt <= 12 * 60; gmt += 30)
        {
          pdf_error_t *error = NULL;
          pdf_i32_t offset_hours;
          pdf_i32_t offset_minutes;
          pdf_time_t time1;
          pdf_char_t *test_string;
          pdf_char_t *date_string;
          int year;

          pdf_time_init (&time1);

          /* yymmddhhmmss+hhmm  - lenght = 18 */
          test_string = pdf_alloc (18);
          memset (&test_string[0], 0, 18);

          calendar.year   = dates[i].year;
          calendar.month  = dates[i].month;
          calendar.day    = dates[i].day;
          calendar.hour   = dates[i].hour;
          calendar.minute = dates[i].minute;
          calendar.second = dates[i].second;
          calendar.dow    = dates[i].dow;
          calendar.gmt_offset = gmt * 60;

#ifdef TIME_MODULE_ADDITIONAL_TEST_TRACES
          printf ("pdf_time_to_string_004 [%u,%+.3d] '%d-%d-%d %d:%d:%d'\n",
                  i, gmt,
                  calendar.year, calendar.month, calendar.day,
                  calendar.hour, calendar.minute, calendar.second);
#endif /* TIME_MODULE_ADDITIONAL_TEST_TRACES */

          pdf_time_set_from_cal (&time1, &calendar);

          offset_hours = (((gmt < 0) ? (-1) : (1)) * gmt) / 60;
          offset_minutes = (((gmt < 0) ? (-1) : (1)) *gmt ) % 60;

          year = dates[i].year;
          if (dates[i].year < 2000)
            year -= 1900;
          else
            year -= 2000;

          sprintf (&test_string[0],
                   "%02d%02d%02d%02d%02d%02d%c%02d%02d",
                   year, dates[i].month, dates[i].day, dates[i].hour,
                   dates[i].minute, dates[i].second,
                   ((gmt < 0) ? '-' : '+'),
                   offset_hours, offset_minutes);

          if (gmt == 0)
            {
                /** Test yymmddhhmmssZ format **/
                test_string[12] = 'Z';
                test_string[13] = '\0';
            }

#ifdef TIME_MODULE_ADDITIONAL_TEST_TRACES
          printf ("                       [%u,%+.3d] '%s'\n",
                  i, gmt, &test_string[0]);
#endif /* TIME_MODULE_ADDITIONAL_TEST_TRACES */

          date_string = pdf_time_to_string (&time1,
                                            PDF_TIME_STRING_FORMAT_UTC_ASN1,
                                            PDF_TIME_STRING_NO_OPTION,
                                            &error);
          fail_unless (date_string != NULL);
          fail_if (error != NULL);

#ifdef TIME_MODULE_ADDITIONAL_TEST_TRACES
          printf ("                       [%u,%+.3d] '%s'\n",
                  i, gmt, &date_string[0]);
#endif /* TIME_MODULE_ADDITIONAL_TEST_TRACES */

          fail_unless (strcmp (test_string, date_string) == 0);

          pdf_dealloc (date_string);
          pdf_dealloc (test_string);
        }
    }
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_time_to_string (void)
{
  TCase *tc = tcase_create ("pdf_time_to_string");

  tcase_add_test (tc, pdf_time_to_string_001);
  tcase_add_test (tc, pdf_time_to_string_002);
  tcase_add_test (tc, pdf_time_to_string_003);
  tcase_add_test (tc, pdf_time_to_string_004);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-time-to-string.c */

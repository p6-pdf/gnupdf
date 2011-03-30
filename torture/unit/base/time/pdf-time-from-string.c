/* -*- mode: C -*-
 *
 *       File:         pdf-time-from-string.c
 *       Date:         Fri Feb 27 17:35:31 2008
 *
 *       GNU PDF Library - Unit tests for pdf_time_from_string
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
 * Test: pdf_time_from_string_001
 * Description:
 *   Get pdf_time_t object from string in full ISO_8601:
 *   YYYY-MM-DDThh:mm:ss+hh:mm.
 *Success condition:
 * 1. Function pdf_time_from_string schould return
 * PDF_OK.
 * 2. pdf_time_t object schould point the same time
 * as pdf_time_t object created by pdf_time_from_u32_t.
 * 3. pdf_time_cmp schould return PDF_OK
 */
START_TEST (pdf_time_from_string_001)
{
  pdf_u32_t i;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t dates_in_seconds[];

  for (i = 0; i < DATES_SIZE; i++)
    {
      pdf_i32_t gmt;

      for (gmt = -12 * 60; gmt <= 12 * 60; gmt += 51)
        {
          pdf_error_t *error = NULL;
          pdf_time_t time1;
          pdf_time_t time2;
          pdf_i32_t offset_hours;
          pdf_i32_t offset_minutes;
          pdf_i32_t seconds;
          pdf_char_t *date_string;

          /* Set various gmt_offsets. */
          seconds = dates_in_seconds[i] - gmt * 60;
          if (seconds < 0)
            continue;

          pdf_time_init (&time1);
          pdf_time_init (&time2);

          /* YYYY-MM-DDThh:mm:ss+hh:mm  - length = 26  */
          date_string = pdf_alloc (26);
          memset (&date_string[0], 0, 26);

          pdf_time_set_utc (&time1, seconds);

          offset_hours = (((gmt < 0) ? (-1) : (1)) * gmt) / 60;
          offset_minutes = (((gmt < 0) ? (-1) : (1)) *gmt ) % 60;

          sprintf (&date_string[0],
                   "%d-%02d-%02dT%02d:%02d:%02d%c%02d:%02d",
                   dates[i].year, dates[i].month, dates[i].day,
                   dates[i].hour, dates[i].minute, dates[i].second,
                   ((gmt < 0) ? '-' : '+'),
                   offset_hours, offset_minutes);

          fail_if (pdf_time_set_from_string (&time2,
                                             date_string,
                                             PDF_TIME_STRING_FORMAT_ISO_8601,
                                             PDF_TIME_STRING_NO_OPTION,
                                             &error) != PDF_TRUE);
          fail_if (error != NULL);
          fail_unless (pdf_time_cmp (&time1, &time2) == 0);

          if (gmt == 0)
            {
              /** Test YYYY-MM-DDThh:mm:ssZ format **/
                date_string[19] = 'Z';
                date_string[20] = '\0';

                fail_if (pdf_time_set_from_string (&time2,
                                                   date_string,
                                                   PDF_TIME_STRING_FORMAT_ISO_8601,
                                                   PDF_TIME_STRING_NO_OPTION,
                                                   &error) != PDF_TRUE);
                fail_if (error != NULL);
                fail_unless (pdf_time_cmp (&time1, &time2) == 0);
            }

          pdf_dealloc (date_string);
          pdf_time_deinit (&time1);
          pdf_time_deinit (&time2);
        }
    }
}
END_TEST

/*
 * Test: pdf_time_from_string_002
 * Description:
 *   Get pdf_time_t object from string in ISO_8601
 *   with format YYYY-MM-DDThh:mm+hh:mm(without
 *   seconds)
 *Success condition:
 * 1. Function pdf_time_from_string schould return
 * PDF_OK.
 * 2. pdf_time_t object schould point the same time
 * as pdf_time_t object created by pdf_time_from_u32_t.
 * 3. pdf_time_cmp schould return PDF_OK
 */
START_TEST (pdf_time_from_string_002)
{
  pdf_u32_t i;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t dates_in_seconds[];

  for (i = 0; i < DATES_SIZE; i++)
    {
      pdf_i32_t gmt;

      for (gmt = -12 * 60; gmt <= 12 * 60; gmt += 51)
        {
          pdf_error_t *error = NULL;
          pdf_time_t time1;
          pdf_time_t time2;
          pdf_i32_t offset_hours;
          pdf_i32_t offset_minutes;
          pdf_i32_t seconds;
          pdf_char_t *date_string;

          /* Set various gmt_offsets. */
          seconds = dates_in_seconds[i] - gmt * 60;
          if (seconds < 0)
            continue;

          pdf_time_init (&time1);
          pdf_time_init (&time2);

          /* YYYY-MM-DDThh:mm+hh:mm  - length = 23  */
          date_string = pdf_alloc (23);
          memset (&date_string[0], 0, 23);

          pdf_time_set_utc (&time1, seconds);

          offset_hours = (((gmt < 0) ? (-1) : (1)) * gmt) / 60;
          offset_minutes = (((gmt < 0) ? (-1) : (1)) *gmt ) % 60;

          sprintf (&date_string[0],
                   "%d-%02d-%02dT%02d:%02d%c%02d:%02d",
                   dates[i].year, dates[i].month, dates[i].day,
                   dates[i].hour, dates[i].minute,
                   ((gmt < 0) ? '-' : '+'),
                   offset_hours, offset_minutes);

          fail_if (pdf_time_set_from_string (&time2,
                                             date_string,
                                             PDF_TIME_STRING_FORMAT_ISO_8601,
                                             PDF_TIME_STRING_NO_OPTION,
                                             &error) != PDF_TRUE);
          fail_if (error != NULL);
          fail_unless (pdf_time_cmp (&time1, &time2) == 0);

          if (gmt == 0)
            {
              /** Test YYYY-MM-DDThh:mm:ssZ format **/
                date_string[16] = 'Z';
                date_string[17] = '\0';

                fail_if (pdf_time_set_from_string (&time2,
                                                   date_string,
                                                   PDF_TIME_STRING_FORMAT_ISO_8601,
                                                   PDF_TIME_STRING_NO_OPTION,
                                                   &error) != PDF_TRUE);
                fail_if (error != NULL);
                fail_unless (pdf_time_cmp (&time1, &time2) == 0);
            }

          pdf_dealloc (date_string);
          pdf_time_deinit (&time1);
          pdf_time_deinit (&time2);
        }
    }
}
END_TEST

/*
 * Test: pdf_time_from_string_003
 * Description:
 *   Get pdf_time_t object from string in ISO_8601
 *   with format YYYY-MM-DD(without time)
 *Success condition:
 * 1. Function pdf_time_from_string schould return
 * PDF_OK.
 * 2. pdf_time_t object schould point the same time
 * as pdf_time_t object created by pdf_time_from_u32_t.
 * 3. pdf_time_cmp schould return PDF_OK
 */
START_TEST (pdf_time_from_string_003)
{
  pdf_u32_t i;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t dates_in_seconds[];

  for (i = 0; i < DATES_SIZE; i++)
    {
      pdf_error_t *error = NULL;
      pdf_time_t time1;
      pdf_time_t time2;
      pdf_i32_t seconds;
      pdf_char_t *date_string;

      seconds = (dates_in_seconds[i] -
                 dates[i].hour * 3600 -
                 dates[i].minute * 60 -
                 dates[i].second);
      if (seconds < 0)
        continue;

      pdf_time_init (&time1);
      pdf_time_init (&time2);

      /* YYYY-MM-DD  - length = 11 */
      date_string = pdf_alloc (11);
      memset (&date_string[0], 0, 11);

      pdf_time_set_utc (&time1, seconds);

      sprintf (&date_string[0],
               "%d-%02d-%02d",
               dates[i].year, dates[i].month, dates[i].day);

      fail_if (pdf_time_set_from_string (&time2,
                                         date_string,
                                         PDF_TIME_STRING_FORMAT_ISO_8601,
                                         PDF_TIME_STRING_NO_OPTION,
                                         &error) != PDF_TRUE);
      fail_if (error != NULL);
      fail_unless (pdf_time_cmp (&time1, &time2) == 0);

      pdf_dealloc (date_string);
      pdf_time_deinit (&time1);
      pdf_time_deinit (&time2);
    }
}
END_TEST

/*
 * Test: pdf_time_from_string_004
 * Description:
 *   Get pdf_time_t object from string in ISO_8601
 *   with format YYYY-MM(without time and day)
 *Success condition:
 * 1. Function pdf_time_from_string schould return
 * PDF_OK.
 * 2. pdf_time_t object schould point the same time
 * as pdf_time_t object created by pdf_time_from_u32_t.
 * 3. pdf_time_cmp schould return PDF_OK
 */
START_TEST (pdf_time_from_string_004)
{
  pdf_u32_t i;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t dates_in_seconds[];

  for (i = 0; i < DATES_SIZE; i++)
    {
      pdf_error_t *error = NULL;
      pdf_time_t time1;
      pdf_time_t time2;
      pdf_i32_t seconds;
      pdf_char_t *date_string;

      if (dates[i].day != 1)
        continue;

      seconds = (dates_in_seconds[i] -
                 dates[i].hour * 3600 -
                 dates[i].minute * 60 -
                 dates[i].second);
      if (seconds < 0)
        continue;

      /* YYYY-MM  - length = 8 */
      date_string = pdf_alloc (8);
      memset (&date_string[0], 0, 8);

      pdf_time_set_utc (&time1, seconds);

      sprintf (&date_string[0],
               "%d-%02d",
               dates[i].year, dates[i].month);

      fail_if (pdf_time_set_from_string (&time2,
                                         date_string,
                                         PDF_TIME_STRING_FORMAT_ISO_8601,
                                         PDF_TIME_STRING_NO_OPTION,
                                         &error) != PDF_TRUE);
      fail_if (error != NULL);
      fail_unless (pdf_time_cmp (&time1, &time2) == 0);

      pdf_dealloc (date_string);
      pdf_time_deinit (&time1);
      pdf_time_deinit (&time2);
    }
}
END_TEST

/*
 * Test: pdf_time_from_string_005
 * Description:
 *   Get pdf_time_t object from string in ISO_8601
 *   with format YYYY(only year)
 *Success condition:
 * 1. Function pdf_time_from_string schould return
 * PDF_OK.
 * 2. pdf_time_t object schould point the same time
 * as pdf_time_t object created by pdf_time_from_u32_t.
 * 3. pdf_time_cmp schould return PDF_OK
 */
START_TEST (pdf_time_from_string_005)
{
  pdf_u32_t i;
  pdf_i32_t seconds;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t dates_in_seconds[];

  for (i = 0, seconds = 0;
       i < DATES_SIZE;
       i++, seconds += SEC_IN_NOLEEP_YEAR)
    {
      pdf_error_t *error = NULL;
      pdf_time_t time1;
      pdf_time_t time2;
      pdf_char_t *date_string;

      /* add one day - 29 February after leap year */
      if ((i - 2) % 4 == 1)
        seconds += SEC_IN_DAY;

      pdf_time_init (&time1);
      pdf_time_init (&time2);

      /* YYYY  - length = 5 */
      date_string = pdf_alloc (5);
      memset (&date_string[0], 0, 5);

      pdf_time_set_utc (&time1, seconds);

      sprintf (&date_string[0], "%d", 1970 + i);

      fail_if (pdf_time_set_from_string (&time2,
                                         date_string,
                                         PDF_TIME_STRING_FORMAT_ISO_8601,
                                         PDF_TIME_STRING_NO_OPTION,
                                         &error) != PDF_TRUE);
      fail_if (error != NULL);
      fail_unless (pdf_time_cmp (&time1, &time2) == 0);

      pdf_dealloc (date_string);
      pdf_time_deinit (&time1);
      pdf_time_deinit (&time2);
    }
}
END_TEST

/*
 * Test: pdf_time_from_string_006
 * Description:
 *   Get pdf_time_t object from string in PDF
 *   format:
 *   D:YYYYMMDDHHmmSSOHH'mm'
 *   D:YYYYMMDDHHmmSSOHH'mm
 *   D:YYYYMMDDHHmmSSZ when gmt_offset == 0
 *Success condition:
 * 1. Function pdf_time_from_string should return PDF_OK.
 * 2. Returned pdf_time_t object schould point the same time
 * as pdf_time_t object created by pdf_time_from_u32_t.
 * 3. pdf_time_cmp schould return PDF_OK
 */
START_TEST (pdf_time_from_string_006)
{
  pdf_u32_t i;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t dates_in_seconds[];

  for (i = 0; i < DATES_SIZE; i++)
    {
      pdf_i32_t gmt;

      for (gmt = -12 * 60; gmt <= 12 * 60; gmt += 51)
        {
          pdf_error_t *error = NULL;
          pdf_i32_t offset_hours;
          pdf_i32_t offset_minutes;
          pdf_i32_t seconds;
          pdf_char_t *date_string;
          pdf_time_t time1;
          pdf_time_t time2;

          seconds = dates_in_seconds[i] - gmt * 60;
          if (seconds < 0)
            continue;

          pdf_time_init (&time1);
          pdf_time_init (&time2);

          /* D:YYYYMMDDHHmmSSOHH'mm'  - length = 24  */
          date_string = pdf_alloc (24);
          memset (&date_string[0], 0, 24);

          pdf_time_set_utc (&time1, seconds);

          offset_hours = (((gmt < 0) ? (-1) : (1)) * gmt) / 60;
          offset_minutes = (((gmt < 0) ? (-1) : (1)) *gmt ) % 60;

          sprintf (&date_string[0],
                   "D:%d%02d%02d%02d%02d%02d%c%02d'%02d'",
                   dates[i].year, dates[i].month, dates[i].day,
                   dates[i].hour, dates[i].minute, dates[i].second,
                   ((gmt < 0) ? '-' : '+'),
                   offset_hours, offset_minutes);

          fail_if (pdf_time_set_from_string (&time2,
                                             date_string,
                                             PDF_TIME_STRING_FORMAT_PDF,
                                             PDF_TIME_STRING_NO_OPTION,
                                             &error) != PDF_TRUE);
          fail_if (error != NULL);
          fail_if (pdf_time_set_from_string (&time2,
                                             date_string,
                                             PDF_TIME_STRING_FORMAT_PDF,
                                             PDF_TIME_STRING_TRAILING_APOSTROPHE,
                                             &error) != PDF_TRUE);
          fail_if (error != NULL);
          fail_unless (pdf_time_cmp (&time1, &time2) == 0);

          /* Check format without last "'"
           * (From ISO 32000)
           * */
          date_string[22] = '\0';

          fail_if (pdf_time_set_from_string (&time2,
                                             date_string,
                                             PDF_TIME_STRING_FORMAT_PDF,
                                             PDF_TIME_STRING_NO_OPTION,
                                             &error) != PDF_TRUE);
          fail_if (error != NULL);
          fail_if (pdf_time_set_from_string (&time2,
                                             date_string,
                                             PDF_TIME_STRING_FORMAT_PDF,
                                             PDF_TIME_STRING_TRAILING_APOSTROPHE,
                                             &error) != PDF_TRUE);
          fail_if (error != NULL);
          fail_unless (pdf_time_cmp (&time1, &time2) == 0);

          if (gmt == 0)
            {
              /** Test D:YYYYMMDDHHmmSSZ format **/
              date_string[16] = 'Z';
              date_string[17] = '\0';

              fail_if (pdf_time_set_from_string (&time2,
                                                 date_string,
                                                 PDF_TIME_STRING_FORMAT_PDF,
                                                 PDF_TIME_STRING_TRAILING_APOSTROPHE,
                                                 &error) != PDF_TRUE);
              fail_if (error != NULL);
              fail_unless (pdf_time_cmp (&time1, &time2) == 0);
            }

          pdf_dealloc (date_string);
          pdf_time_deinit (&time1);
          pdf_time_deinit (&time2);
        }
    }
}
END_TEST

/*
 * Test: pdf_time_from_string_007
 * Description:
 *   Get pdf_time_t object from string in full
 *   PDF_TIME_STRING_FORMAT_GENERALIZED_ASN1:
 *   YYYYMMDDhhmmssTZD.
 *Success condition:
 * 1. Function pdf_time_from_string schould return
 * PDF_OK.
 * 2. pdf_time_t object schould point the same time
 * as pdf_time_t object created by pdf_time_from_u32_t.
 * 3. pdf_time_cmp schould return PDF_OK
 */
START_TEST (pdf_time_from_string_007)
{
  pdf_u32_t i;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t dates_in_seconds[];

  for (i = 0; i < DATES_SIZE; i++)
    {
      pdf_i32_t gmt;

      for (gmt = -12 * 60; gmt <= 12 * 60; gmt += 51)
        {
          pdf_error_t *error = NULL;
          pdf_i32_t offset_hours;
          pdf_i32_t offset_minutes;
          pdf_i32_t seconds;
          pdf_char_t *date_string;
          pdf_time_t time1;
          pdf_time_t time2;

          /* Set various gmt_offsets. */
          seconds = dates_in_seconds[i] - gmt * 60;
          if (seconds < 0)
            continue;

          pdf_time_init (&time1);
          pdf_time_init (&time2);

          /* YYYYMMDDhhmmss+hhmm  - length = 20 */
          date_string = pdf_alloc (20);
          memset (&date_string[0], 0, 20);

          pdf_time_set_utc (&time1, seconds);

          offset_hours = (((gmt < 0) ? (-1) : (1)) * gmt) / 60;
          offset_minutes = (((gmt < 0) ? (-1) : (1)) *gmt ) % 60;

          sprintf (&date_string[0],
                   "%d%02d%02d%02d%02d%02d%c%02d%02d",
                   dates[i].year, dates[i].month, dates[i].day,
                   dates[i].hour, dates[i].minute, dates[i].second,
                   ((gmt < 0) ? '-' : '+'),
                   offset_hours, offset_minutes);

          fail_if (pdf_time_set_from_string (&time2,
                                             date_string,
                                             PDF_TIME_STRING_FORMAT_GENERALIZED_ASN1,
                                             PDF_TIME_STRING_NO_OPTION,
                                             &error) != PDF_TRUE);
          fail_if (error != NULL);
          fail_unless (pdf_time_cmp (&time1, &time2) == 0);

          if (gmt == 0)
            {
              /** Test YYYYMMDDhhmmssZ format **/
              date_string[14] = 'Z';
              date_string[15] = '\0';
              fail_if (pdf_time_set_from_string (&time2,
                                                 date_string,
                                                 PDF_TIME_STRING_FORMAT_GENERALIZED_ASN1,
                                                 PDF_TIME_STRING_NO_OPTION,
                                                 &error) != PDF_TRUE);
              fail_if (error != NULL);
              fail_unless (pdf_time_cmp (&time1, &time2) == 0);
            }

          pdf_dealloc (date_string);
          pdf_time_deinit (&time1);
          pdf_time_deinit (&time2);
        }
    }
}
END_TEST

/*
 * Test: pdf_time_from_string_008
 * Description:
 *   Get pdf_time_t object from string in full
 *   PDF_TIME_STRING_FORMAT_GENERALIZED_ASN1:
 *   YYYYMMDDhhmmTZD. (without sconds)
 *Success condition:
 * 1. Function pdf_time_from_string schould return
 * PDF_OK.
 * 2. pdf_time_t object schould point the same time
 * as pdf_time_t object created by pdf_time_from_u32_t.
 * 3. pdf_time_cmp schould return PDF_OK
 */
START_TEST (pdf_time_from_string_008)
{
  pdf_u32_t i;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t dates_in_seconds[];

  for (i = 0; i < DATES_SIZE; i++)
    {
      pdf_i32_t gmt;

      for (gmt = -12 * 60; gmt <= 12 * 60; gmt += 51)
        {
          pdf_error_t *error = NULL;
          pdf_i32_t offset_hours;
          pdf_i32_t offset_minutes;
          pdf_i32_t seconds;
          pdf_char_t *date_string;
          pdf_time_t time1;
          pdf_time_t time2;

          seconds = dates_in_seconds[i] - dates[i].second - gmt * 60;
          if (seconds < 0)
            continue;

          pdf_time_init (&time1);
          pdf_time_init (&time2);

          /* YYYYMMDDhhmm+hhmm  - length = 18  */
          date_string = pdf_alloc (18);
          memset (&date_string[0], 0, 18);

          pdf_time_set_utc (&time1, seconds);

          offset_hours = (((gmt < 0) ? (-1) : (1)) * gmt) / 60;
          offset_minutes = (((gmt < 0) ? (-1) : (1)) *gmt ) % 60;

          sprintf (&date_string[0],
                   "%d%02d%02d%02d%02d%c%02d%02d",
                   dates[i].year, dates[i].month, dates[i].day,
                   dates[i].hour, dates[i].minute,
                   ((gmt < 0) ? '-' : '+'),
                   offset_hours, offset_minutes);

          fail_if (pdf_time_set_from_string (&time2,
                                             date_string,
                                             PDF_TIME_STRING_FORMAT_GENERALIZED_ASN1,
                                             PDF_TIME_STRING_NO_OPTION,
                                             &error) != PDF_TRUE);
          fail_if (error != NULL);
          fail_unless (pdf_time_cmp (&time1, &time2) == 0);

          if (gmt == 0)
            {
              /** Test YYYYMMDDhhmmZ format **/
              date_string[12] = 'Z';
              date_string[13] = '\0';
              fail_if (pdf_time_set_from_string (&time2,
                                                 date_string,
                                                 PDF_TIME_STRING_FORMAT_GENERALIZED_ASN1,
                                                 PDF_TIME_STRING_NO_OPTION,
                                                 &error) != PDF_TRUE);
              fail_if (error != NULL);
              fail_unless (pdf_time_cmp (&time1, &time2) == 0);
            }

          pdf_dealloc (date_string);
          pdf_time_deinit (&time1);
          pdf_time_deinit (&time2);
        }
    }
}
END_TEST

/*
 * Test: pdf_time_from_string_009
 * Description:
 *   Get pdf_time_t object from string in full
 *   PDF_TIME_STRING_FORMAT_GENERALIZED_ASN1:
 *   YYYYMMDD. (without time)
 *
 *Success condition:
 * 1. Function pdf_time_from_string schould return
 * PDF_OK.
 * 2. pdf_time_t object schould point the same time
 * as pdf_time_t object created by pdf_time_from_u32_t.
 * 3. pdf_time_cmp schould return PDF_OK
 */
START_TEST (pdf_time_from_string_009)
{
  pdf_u32_t i;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t dates_in_seconds[];

  for (i = 0; i < DATES_SIZE; i++)
    {
      pdf_error_t *error = NULL;
      pdf_time_t time1;
      pdf_time_t time2;
      pdf_i32_t seconds;
      pdf_char_t *date_string;

      seconds = (dates_in_seconds[i] -
                 dates[i].hour * 3600 -
                 dates[i].minute * 60 -
                 dates[i].second);
      if (seconds < 0)
        continue;

      /* YYYYMMDD  - length = 9 */
      date_string = pdf_alloc(9);
      memset (&date_string[0], 0, 9);

      pdf_time_set_utc (&time1, seconds);

      sprintf (&date_string[0],
               "%d%02d%02d",
               dates[i].year, dates[i].month, dates[i].day);

      fail_if (pdf_time_set_from_string (&time2,
                                         date_string,
                                         PDF_TIME_STRING_FORMAT_GENERALIZED_ASN1,
                                         PDF_TIME_STRING_NO_OPTION,
                                         &error) != PDF_TRUE);
      fail_if (error != NULL);
      fail_unless (pdf_time_cmp (&time1, &time2) == 0);

      pdf_dealloc (date_string);
      pdf_time_deinit (&time1);
      pdf_time_deinit (&time2);
    }
}
END_TEST

/*
 * Test: pdf_time_from_string_010
 * Description:
 *   Get pdf_time_t object from string in full
 *   PDF_TIME_STRING_FORMAT_GENERALIZED_ASN1:
 *   YYYYMM. (without time and day)
 *Success condition:
 * 1. Function pdf_time_from_string schould return
 * PDF_OK.
 * 2. pdf_time_t object schould point the same time
 * as pdf_time_t object created by pdf_time_from_u32_t.
 * 3. pdf_time_cmp schould return PDF_OK
 */
START_TEST (pdf_time_from_string_010)
{
  pdf_u32_t i;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t dates_in_seconds[];

  for (i = 0; i < DATES_SIZE; i++)
    {
      pdf_error_t *error = NULL;
      pdf_time_t time1;
      pdf_time_t time2;
      pdf_i32_t seconds;
      pdf_char_t *date_string;

      if (dates[i].day != 1)
        continue;

      seconds = (dates_in_seconds[i] -
                 (dates[i].day - 1) * 24 * 3600 -
                 dates[i].hour * 3600 -
                 dates[i].minute * 60 -
                 dates[i].second);
      if (seconds < 0)
        continue;

      /* YYYYMM  - length = 7 */
      date_string = pdf_alloc (7);
      memset (&date_string[0], 0, 7);

      pdf_time_set_utc (&time1, seconds);

      sprintf(&date_string[0],
              "%d%02d",
              dates[i].year, dates[i].month);

      fail_if (pdf_time_set_from_string (&time2,
                                         date_string,
                                         PDF_TIME_STRING_FORMAT_GENERALIZED_ASN1,
                                         PDF_TIME_STRING_NO_OPTION,
                                         &error) != PDF_TRUE);
      fail_if (error != NULL);
      fail_unless (pdf_time_cmp (&time1, &time2) == 0);

      pdf_dealloc (date_string);
      pdf_time_deinit (&time1);
      pdf_time_deinit (&time2);
    }
}
END_TEST

/*
 * Test: pdf_time_from_string_011
 * Description:
 *   Get pdf_time_t object from string in full
 *   PDF_TIME_STRING_FORMAT_GENERALIZED_ASN1:
 *   YYYY. (only year)
  *Success condition:
 * 1. Function pdf_time_from_string schould return
 * PDF_OK.
 * 2. pdf_time_t object schould point the same time
 * as pdf_time_t object created by pdf_time_from_u32_t.
 * 3. pdf_time_cmp schould return PDF_OK
 */
START_TEST (pdf_time_from_string_011)
{
  pdf_i32_t seconds;
  pdf_u32_t i;

  for (i = 0, seconds = 0;
       i < 40;
       i++, seconds += SEC_IN_NOLEEP_YEAR)
    {
      pdf_error_t *error = NULL;
      pdf_time_t time1;
      pdf_time_t time2;
      pdf_char_t *date_string;

      /* add one day - 29 February after leap year */
      if ((i - 2) % 4 == 1)
        seconds += SEC_IN_DAY;

      /* YYYY  - length = 5  */
      date_string = pdf_alloc (5);
      memset (&date_string[0], 0, 5);

      pdf_time_set_utc (&time1, seconds);

      sprintf (&date_string[0],
               "%d",
               1970 + i);

      fail_if (pdf_time_set_from_string (&time2,
                                         date_string,
                                         PDF_TIME_STRING_FORMAT_GENERALIZED_ASN1,
                                         PDF_TIME_STRING_NO_OPTION,
                                         &error) != PDF_TRUE);
      fail_if (error != NULL);
      fail_unless (pdf_time_cmp (&time1, &time2) == 0);

      pdf_dealloc (date_string);
      pdf_time_deinit (&time1);
      pdf_time_deinit (&time2);
    }
}
END_TEST


/*
 * Test: pdf_time_from_string_012
 * Description:
 *   Get pdf_time_t object from string in full
 *   PDF_TIME_STRING_FORMAT_UTC_ASN1:
 *   yymmddhhmmss+hhmm
 *Success condition:
 * 1. Function pdf_time_from_string schould return
 * PDF_OK.
 * 2. pdf_time_t object schould point the same time
 * as pdf_time_t object created by pdf_time_from_u32_t.
 * 3. pdf_time_cmp schould return PDF_OK
 */
START_TEST (pdf_time_from_string_012)
{
  pdf_u32_t i;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t dates_in_seconds[];

  for (i = 0; i < DATES_SIZE; i++)
    {
      pdf_i32_t gmt;

      for (gmt = -12 * 60; gmt <= 12 * 60; gmt += 51)
        {
          pdf_error_t *error = NULL;
          pdf_time_t time1;
          pdf_time_t time2;
          pdf_i32_t offset_hours;
          pdf_i32_t offset_minutes;
          pdf_i32_t seconds;
          pdf_char_t *date_string;

          /* Set various gmt_offsets. */
          seconds = dates_in_seconds[i] - gmt * 60;
          if (seconds < 0)
            continue;

          pdf_time_init (&time1);
          pdf_time_init (&time2);

          /* yymmddhhmmss+hhmm  - length = 18 */
          date_string = pdf_alloc (18);
          memset (&date_string[0], 0, 18);

          pdf_time_set_utc (&time1, seconds);

          offset_hours = (((gmt < 0) ? (-1) : (1)) * gmt) / 60;
          offset_minutes = (((gmt < 0) ? (-1) : (1)) *gmt ) % 60;

          sprintf (&date_string[0],
                   "%d%02d%02d%02d%02d%02d%c%02d%02d",
                   dates[i].year, dates[i].month, dates[i].day,
                   dates[i].hour, dates[i].minute, dates[i].second,
                   ((gmt < 0) ? '-' : '+'),
                   offset_hours, offset_minutes);

          fail_if (pdf_time_set_from_string (&time2,
                                             date_string,
                                             PDF_TIME_STRING_FORMAT_UTC_ASN1,
                                             PDF_TIME_STRING_NO_OPTION,
                                             &error) != PDF_TRUE);
          fail_if (error != NULL);
          fail_unless (pdf_time_cmp (&time1, &time2) == 0);

          if (gmt == 0)
            {
              /** Test yymmddhhmmssZ format **/
              date_string[14] = 'Z';
              date_string[15] = '\0';
              fail_if (pdf_time_set_from_string (&time2,
                                                 date_string,
                                                 PDF_TIME_STRING_FORMAT_UTC_ASN1,
                                                 PDF_TIME_STRING_NO_OPTION,
                                                 &error) != PDF_TRUE);
              fail_if (error != NULL);
              fail_unless (pdf_time_cmp (&time1, &time2) == 0);
            }

          pdf_dealloc (date_string);
          pdf_time_deinit (&time1);
          pdf_time_deinit (&time2);
        }
    }
}
END_TEST

/*
 * Test: pdf_time_from_string_013
 * Description:
 *   Get pdf_time_t object from string in full
 *   PDF_TIME_STRING_FORMAT_UTC_ASN1:
 *   YYYYMMDDhhmmTZD. (without sconds)
 *Success condition:
 * 1. Function pdf_time_from_string schould return
 * PDF_OK.
 * 2. pdf_time_t object schould point the same time
 * as pdf_time_t object created by pdf_time_from_u32_t.
 * 3. pdf_time_cmp schould return PDF_OK
 */
START_TEST (pdf_time_from_string_013)
{
  pdf_u32_t i;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t dates_in_seconds[];

  for (i = 0; i < DATES_SIZE; i++)
    {
      pdf_i32_t gmt;

      for (gmt = -12 * 60; gmt <= 12 * 60; gmt += 51)
        {
          pdf_error_t *error = NULL;
          pdf_time_t time1;
          pdf_time_t time2;
          pdf_i32_t offset_hours;
          pdf_i32_t offset_minutes;
          pdf_i32_t seconds;
          pdf_char_t *date_string;

          seconds = dates_in_seconds[i] - dates[i].second - gmt * 60;
          if (seconds < 0)
            continue;

          pdf_time_init (&time1);
          pdf_time_init (&time2);

          /* YYMMDDhhmm+hhmm  - length = 16  */
          date_string = pdf_alloc (18);
          memset (&date_string[0], 0, 18);

          pdf_time_set_utc (&time1, seconds);

          offset_hours = (((gmt < 0) ? (-1) : (1)) * gmt) / 60;
          offset_minutes = (((gmt < 0) ? (-1) : (1)) *gmt ) % 60;

          sprintf (&date_string[0],
                   "%d%02d%02d%02d%02d%c%02d%02d",
                   dates[i].year, dates[i].month, dates[i].day,
                   dates[i].hour, dates[i].minute,
                   ((gmt < 0) ? '-' : '+'),
                   offset_hours, offset_minutes);

          fail_if (pdf_time_set_from_string (&time2,
                                             date_string,
                                             PDF_TIME_STRING_FORMAT_UTC_ASN1,
                                             PDF_TIME_STRING_NO_OPTION,
                                             &error) != PDF_TRUE);
          fail_if (error != NULL);
          fail_unless (pdf_time_cmp (&time1, &time2) == 0);

          if (gmt == 0)
            {
              /** Test YYMMDDhhmmZ format **/
              date_string[12] = 'Z';
              date_string[13] = '\0';
              fail_if (pdf_time_set_from_string (&time2,
                                                 date_string,
                                                 PDF_TIME_STRING_FORMAT_UTC_ASN1,
                                                 PDF_TIME_STRING_NO_OPTION,
                                                 &error) != PDF_TRUE);
              fail_if (error != NULL);
              fail_unless (pdf_time_cmp (&time1, &time2) == 0);
            }

          pdf_dealloc (date_string);
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
test_pdf_time_from_string (void)
{
  TCase *tc = tcase_create ("pdf_time_from_string");

  tcase_add_test (tc, pdf_time_from_string_001);
  tcase_add_test (tc, pdf_time_from_string_002);
  tcase_add_test (tc, pdf_time_from_string_003);
  tcase_add_test (tc, pdf_time_from_string_004);
  tcase_add_test (tc, pdf_time_from_string_005);
  tcase_add_test (tc, pdf_time_from_string_006);
  tcase_add_test (tc, pdf_time_from_string_007);
  tcase_add_test (tc, pdf_time_from_string_008);
  tcase_add_test (tc, pdf_time_from_string_009);
  tcase_add_test (tc, pdf_time_from_string_010);
  tcase_add_test (tc, pdf_time_from_string_011);
  tcase_add_test (tc, pdf_time_from_string_012);
  tcase_add_test (tc, pdf_time_from_string_013);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-time-from-string.c */

/* -*- mode: C -*- Time-stamp: "07/09/09 00:20:28 jemarch"
 *
 *       File:         test-date.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Sat Sep  8 22:56:54 2007
 *
 *       GNU PDF Library - Test suite for the date compound data type
 *
 */

/* Copyright (C) 2007 Jose E. Marchesi */

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
#include <malloc.h>
#include <xalloc.h>
#include <string.h>
#include <stdio.h>
#include <pdf_obj.h>
#include <pdf_date.h>

#define PRINT_DATE(date)                                                \
  do                                                                    \
    {                                                                   \
      printf("Year: %d\n", pdf_date_year ((date)));                     \
      printf("Month: %d\n", pdf_date_month ((date)));                   \
      printf("Day: %d\n", pdf_date_day ((date)));                       \
      printf("Hour: %d\n", pdf_date_hour ((date)));                     \
      printf("Minute: %d\n", pdf_date_minute ((date)));                 \
      printf("Second: %d\n", pdf_date_second ((date)));                 \
      printf("UT offset hours: %d\n", pdf_date_ut_offset_hour ((date)));     \
      printf("UT offset minutes: %d\n", pdf_date_ut_offset_minute ((date))); \
    }                                                                   \
  while (0)

int 
main ()
{
  int success;
  pdf_obj_t string;
  pdf_date_t date;

  /* Date creation */
  string = pdf_create_string ("D:19800813175500+01'03'", 23);
  date = pdf_date_new (string);
  if (date == NULL)
    {
      printf("Error parsing a date from a D:YYYYMMDDHHmmSS+HH'mm' string.\n");
      success = PDF_FALSE;
    }
  else
    {
      if ((pdf_date_year (date) != 1980) ||
          (pdf_date_month (date) != 8) ||
          (pdf_date_day (date) != 13) ||
          (pdf_date_hour (date) != 17) ||
          (pdf_date_minute (date) != 55) ||
          (pdf_date_second (date) != 0) ||
          (pdf_date_ut_offset_hour (date) != 1) ||
          (pdf_date_ut_offset_minute (date) != 3))
        {
          printf("Error values in a date created from a D:YYYYMMDDHHmmSS+HH'mm' string.\n");
          PRINT_DATE(date);
          success = PDF_FALSE;
        }

      pdf_date_destroy (date);
    }
  pdf_destroy_obj (string);

  string = pdf_create_string ("D:19800813175500-01'03'", 23);
  date = pdf_date_new (string);
  if (date == NULL)
    {
      printf("Error parsing a date from a D:YYYYMMDDHHmmSS-HH'mm' string.\n");
      success = PDF_FALSE;
    }
  else
    {
      if ((pdf_date_year (date) != 1980) ||
          (pdf_date_month (date) != 8) ||
          (pdf_date_day (date) != 13) ||
          (pdf_date_hour (date) != 17) ||
          (pdf_date_minute (date) != 55) ||
          (pdf_date_second (date) != 0) ||
          (pdf_date_ut_offset_hour (date) != -1) ||
          (pdf_date_ut_offset_minute (date) != -3))
        {
          printf("Error values in a date created from a D:YYYYMMDDHHmmSS-HH'mm' string.\n");
          PRINT_DATE(date);
          success = PDF_FALSE;
        }

      pdf_date_destroy (date);
    }
  pdf_destroy_obj (string);

  string = pdf_create_string ("D:19800813175500Z01'03'", 23);
  date = pdf_date_new (string);
  if (date == NULL)
    {
      printf("Error parsing a date from a D:YYYYMMDDHHmmSSZHH'mm' string.\n");
      success = PDF_FALSE;
    }
  else
    {
      if ((pdf_date_year (date) != 1980) ||
          (pdf_date_month (date) != 8) ||
          (pdf_date_day (date) != 13) ||
          (pdf_date_hour (date) != 17) ||
          (pdf_date_minute (date) != 55) ||
          (pdf_date_second (date) != 0) ||
          (pdf_date_ut_offset_hour (date) != 0) ||
          (pdf_date_ut_offset_minute (date) != 0))
        {
          printf("Error values in a date created from a D:YYYYMMDDHHmmSSZHH'mm' string.\n");
          PRINT_DATE(date);
          success = PDF_FALSE;
        }

      pdf_date_destroy (date);
    }
  pdf_destroy_obj (string);

  string = pdf_create_string ("19800813175500+01'03'", 21);
  date = pdf_date_new (string);
  if (date == NULL)
    {
      printf("Error parsing a date from a YYYYMMDDHHmmSSOHH'mm' string.\n");
      success = PDF_FALSE;
    }
  else
    {
      if ((pdf_date_year (date) != 1980) ||
          (pdf_date_month (date) != 8) ||
          (pdf_date_day (date) != 13) ||
          (pdf_date_hour (date) != 17) ||
          (pdf_date_minute (date) != 55) ||
          (pdf_date_second (date) != 0) ||
          (pdf_date_ut_offset_hour (date) != 1) ||
          (pdf_date_ut_offset_minute (date) != 3))
        {
          printf("Error values in a date created from a YYYYMMDDHHmmSSOHH'mm' string.\n");
          PRINT_DATE(date);
          success = PDF_FALSE;
        }

      pdf_date_destroy (date);
    }
  pdf_destroy_obj (string);

  string = pdf_create_string ("19800813175500+01'", 18);
  date = pdf_date_new (string);
  if (date == NULL)
    {
      printf("Error parsing a date from a YYYYMMDDHHmmSSOHH' string.\n");
      success = PDF_FALSE;
    }
  else
    {
      if ((pdf_date_year (date) != 1980) ||
          (pdf_date_month (date) != 8) ||
          (pdf_date_day (date) != 13) ||
          (pdf_date_hour (date) != 17) ||
          (pdf_date_minute (date) != 55) ||
          (pdf_date_second (date) != 0) ||
          (pdf_date_ut_offset_hour (date) != 1) ||
          (pdf_date_ut_offset_minute (date) != 0))
        {
          printf("Error values in a date created from a YYYYMMDDHHmmSSOHH' string.\n");
          PRINT_DATE(date);
          success = PDF_FALSE;
        }

      pdf_date_destroy (date);
    }
  pdf_destroy_obj (string);

  string = pdf_create_string ("19800813175500+", 15);
  date = pdf_date_new (string);
  if (date == NULL)
    {
      printf("Error parsing a date from a YYYYMMDDHHmmSSO string.\n");
      success = PDF_FALSE;
    }
  else
    {
      if ((pdf_date_year (date) != 1980) ||
          (pdf_date_month (date) != 8) ||
          (pdf_date_day (date) != 13) ||
          (pdf_date_hour (date) != 17) ||
          (pdf_date_minute (date) != 55) ||
          (pdf_date_second (date) != 0) ||
          (pdf_date_ut_offset_hour (date) != 0) ||
          (pdf_date_ut_offset_minute (date) != 0))
        {
          printf("Error values in a date created from a YYYYMMDDHHmmSSO without marker.\n");
          PRINT_DATE(date);
          success = PDF_FALSE;
        }

      pdf_date_destroy (date);
    }
  pdf_destroy_obj (string);

  string = pdf_create_string ("19800813175500", 14);
  date = pdf_date_new (string);
  if (date == NULL)
    {
      printf("Error parsing a date from a YYYYMMDDHHmmSS string.\n");
      success = PDF_FALSE;
    }
  else
    {
      if ((pdf_date_year (date) != 1980) ||
          (pdf_date_month (date) != 8) ||
          (pdf_date_day (date) != 13) ||
          (pdf_date_hour (date) != 17) ||
          (pdf_date_minute (date) != 55) ||
          (pdf_date_second (date) != 0) ||
          (pdf_date_ut_offset_hour (date) != -1) ||
          (pdf_date_ut_offset_minute (date) != -1))
        {
          printf("Error values in a date created from a YYYYMMDDHHmmSS without marker.\n");
          PRINT_DATE(date);
          success = PDF_FALSE;
        }

      pdf_date_destroy (date);
    }
  pdf_destroy_obj (string);

  string = pdf_create_string ("198008131755", 12);
  date = pdf_date_new (string);
  if (date == NULL)
    {
      printf("Error parsing a date from a YYYYMMDDHHmm string.\n");
      success = PDF_FALSE;
    }
  else
    {
      if ((pdf_date_year (date) != 1980) ||
          (pdf_date_month (date) != 8) ||
          (pdf_date_day (date) != 13) ||
          (pdf_date_hour (date) != 17) ||
          (pdf_date_minute (date) != 55) ||
          (pdf_date_second (date) != 0) ||
          (pdf_date_ut_offset_hour (date) != -1) ||
          (pdf_date_ut_offset_minute (date) != -1))
        {
          printf("Error values in a date created from a YYYYMMDDHHmm without marker.\n");
          PRINT_DATE(date);
          success = PDF_FALSE;
        }

      pdf_date_destroy (date);
    }
  pdf_destroy_obj (string);

  string = pdf_create_string ("1980081317", 10);
  date = pdf_date_new (string);
  if (date == NULL)
    {
      printf("Error parsing a date from a YYYYMMDDHH string.\n");
      success = PDF_FALSE;
    }
  else
    {
      if ((pdf_date_year (date) != 1980) ||
          (pdf_date_month (date) != 8) ||
          (pdf_date_day (date) != 13) ||
          (pdf_date_hour (date) != 17) ||
          (pdf_date_minute (date) != 0) ||
          (pdf_date_second (date) != 0) ||
          (pdf_date_ut_offset_hour (date) != -1) ||
          (pdf_date_ut_offset_minute (date) != -1))
        {
          printf("Error values in a date created from a YYYYMMDDHH without marker.\n");
          PRINT_DATE(date);
          success = PDF_FALSE;
        }

      pdf_date_destroy (date);
    }
  pdf_destroy_obj (string);

  string = pdf_create_string ("19800813", 8);
  date = pdf_date_new (string);
  if (date == NULL)
    {
      printf("Error parsing a date from a YYYYMMDD string.\n");
      success = PDF_FALSE;
    }
  else
    {
      if ((pdf_date_year (date) != 1980) ||
          (pdf_date_month (date) != 8) ||
          (pdf_date_day (date) != 13) ||
          (pdf_date_hour (date) != 0) ||
          (pdf_date_minute (date) != 0) ||
          (pdf_date_second (date) != 0) ||
          (pdf_date_ut_offset_hour (date) != -1) ||
          (pdf_date_ut_offset_minute (date) != -1))
        {
          printf("Error values in a date created from a YYYYMMDD without marker.\n");
          PRINT_DATE(date);
          success = PDF_FALSE;
        }

      pdf_date_destroy (date);
    }
  pdf_destroy_obj (string);

  string = pdf_create_string ("198008", 6);
  date = pdf_date_new (string);
  if (date == NULL)
    {
      printf("Error parsing a date from a YYYYMM string.\n");
      success = PDF_FALSE;
    }
  else
    {
      if ((pdf_date_year (date) != 1980) ||
          (pdf_date_month (date) != 8) ||
          (pdf_date_day (date) != 1) ||
          (pdf_date_hour (date) != 0) ||
          (pdf_date_minute (date) != 0) ||
          (pdf_date_second (date) != 0) ||
          (pdf_date_ut_offset_hour (date) != -1) ||
          (pdf_date_ut_offset_minute (date) != -1))
        {
          printf("Error values in a date created from a YYYYMM without marker.\n");
          PRINT_DATE(date);
          success = PDF_FALSE;
        }

      pdf_date_destroy (date);
    }
  pdf_destroy_obj (string);

  string = pdf_create_string ("1980", 4);
  date = pdf_date_new (string);
  if (date == NULL)
    {
      printf("Error parsing a date from a YYYY string.\n");
      success = PDF_FALSE;
    }
  else
    {
      if ((pdf_date_year (date) != 1980) ||
          (pdf_date_month (date) != 1) ||
          (pdf_date_day (date) != 1) ||
          (pdf_date_hour (date) != 0) ||
          (pdf_date_minute (date) != 0) ||
          (pdf_date_second (date) != 0) ||
          (pdf_date_ut_offset_hour (date) != -1) ||
          (pdf_date_ut_offset_minute (date) != -1))
        {
          printf("Error values in a date created from a YYYY without marker.\n");
          PRINT_DATE(date);
          success = PDF_FALSE;
        }

      pdf_date_destroy (date);
    }
  pdf_destroy_obj (string);


  string = pdf_create_string ("D:19800813175500+01'03", 22);
  date = pdf_date_new (string);
  if (date != NULL)
    {
      printf("Invalid success parsing a incorrect date D:YYYYMMDDHHmmSS+HH'mm\n");
      PRINT_DATE(date);
      pdf_date_destroy (date);
      success = PDF_FALSE;
    }
  pdf_destroy_obj (string);

  string = pdf_create_string ("D:19800813175500+a1'", 20);
  date = pdf_date_new (string);
  if (date != NULL)
    {
      printf("Invalid success parsing a incorrect date D:YYYYMMDDHHmmSS+aH'\n");
      PRINT_DATE(date);
      pdf_date_destroy (date);
      success = PDF_FALSE;
    }
  pdf_destroy_obj (string);

  string = pdf_create_string ("D:19z0", 6);
  date = pdf_date_new (string);
  if (date != NULL)
    {
      printf("Invalid success parsing a incorrect date D:YYzY\n");
      PRINT_DATE(date);
      pdf_date_destroy (date);
      success = PDF_FALSE;
    }
  pdf_destroy_obj (string);

  string = pdf_create_string ("", 0);
  date = pdf_date_new (string);
  if (date != NULL)
    {
      printf("Invalid success parsing the empty string.\n");
      PRINT_DATE(date);
      pdf_date_destroy (date);
      success = PDF_FALSE;
    }
  pdf_destroy_obj (string);


  if (success)
    {
      return 0;
    }
  else
    {
      return 1;
    }
}

/* End of test-date.c */

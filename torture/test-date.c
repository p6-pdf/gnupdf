/* -*- mode: C -*- Time-stamp: "07/10/31 20:08:10 jemarch"
 *
 *       File:         test-date.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Sat Sep  8 22:56:54 2007
 *
 *       GNU PDF Library - Test suite for the date common data type
 *
 */

/* Copyright (C) 2007 Free Software Foundation, Inc. */

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

#ifdef HAVE_MALLOC_H
 #include <malloc.h>
#else
 #include <stdlib.h>
#endif /* HAVE_MALLOC_H */

#include <xalloc.h>
#include <string.h>
#include <stdio.h>
#include <pdf_obj.h>
#include <pdf_date.h>
#include <check.h>


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



START_TEST(create_1)
{
  pdf_obj_t string;
  pdf_date_t date;


  /* Date creation */
  string = pdf_create_string ("D:19800813175500+01'03'", 23);
  date = pdf_create_date (string);
  fail_if(date == NULL);
  fail_if ((pdf_date_year (date) != 1980) ||
           (pdf_date_month (date) != 8) ||
           (pdf_date_day (date) != 13) ||
           (pdf_date_hour (date) != 17) ||
           (pdf_date_minute (date) != 55) ||
           (pdf_date_second (date) != 0) ||
           (pdf_date_ut_offset_hour (date) != 1) ||
           (pdf_date_ut_offset_minute (date) != 3));
  pdf_destroy_date (date);
  pdf_destroy_obj (string);
}
END_TEST




START_TEST(create_2)
{
  pdf_obj_t string;
  pdf_date_t date;

  string = pdf_create_string ("D:19800813175500-01'03'", 23);
  date = pdf_create_date (string);
  fail_if (date == NULL);
  fail_if ((pdf_date_year (date) != 1980) ||
           (pdf_date_month (date) != 8) ||
           (pdf_date_day (date) != 13) ||
           (pdf_date_hour (date) != 17) ||
           (pdf_date_minute (date) != 55) ||
           (pdf_date_second (date) != 0) ||
           (pdf_date_ut_offset_hour (date) != -1) ||
           (pdf_date_ut_offset_minute (date) != -3));
  pdf_destroy_date (date);
  pdf_destroy_obj (string);
}
END_TEST


START_TEST(create_3)
{
  pdf_obj_t string;
  pdf_date_t date;

  string = pdf_create_string ("D:19800813175500Z01'03'", 23);
  date = pdf_create_date (string);
  fail_if (date == NULL);
  fail_if ((pdf_date_year (date) != 1980) ||
           (pdf_date_month (date) != 8) ||
           (pdf_date_day (date) != 13) ||
           (pdf_date_hour (date) != 17) ||
           (pdf_date_minute (date) != 55) ||
           (pdf_date_second (date) != 0) ||
           (pdf_date_ut_offset_hour (date) != 0) ||
           (pdf_date_ut_offset_minute (date) != 0));
  pdf_destroy_date (date);
  pdf_destroy_obj (string);
}
END_TEST


START_TEST(create_4)
{
  pdf_obj_t string;
  pdf_date_t date;

  string = pdf_create_string ("19800813175500+01'03'", 21);
  date = pdf_create_date (string);
  fail_if (date == NULL);
  fail_if ((pdf_date_year (date) != 1980) ||
           (pdf_date_month (date) != 8) ||
           (pdf_date_day (date) != 13) ||
           (pdf_date_hour (date) != 17) ||
           (pdf_date_minute (date) != 55) ||
           (pdf_date_second (date) != 0) ||
           (pdf_date_ut_offset_hour (date) != 1) ||
           (pdf_date_ut_offset_minute (date) != 3));
  pdf_destroy_date (date);
  pdf_destroy_obj (string);
}
END_TEST


START_TEST(create_5)
{
  pdf_obj_t string;
  pdf_date_t date;

  string = pdf_create_string ("19800813175500+01'", 18);
  date = pdf_create_date (string);
  fail_if (date == NULL);
  fail_if ((pdf_date_year (date) != 1980) ||
           (pdf_date_month (date) != 8) ||
           (pdf_date_day (date) != 13) ||
           (pdf_date_hour (date) != 17) ||
           (pdf_date_minute (date) != 55) ||
           (pdf_date_second (date) != 0) ||
           (pdf_date_ut_offset_hour (date) != 1) ||
           (pdf_date_ut_offset_minute (date) != 0));
  pdf_destroy_date (date);
  pdf_destroy_obj (string);
}
END_TEST


START_TEST(create_6)
{
  pdf_obj_t string;
  pdf_date_t date;

  string = pdf_create_string ("19800813175500+", 15);
  date = pdf_create_date (string);
  fail_if (date == NULL);
  fail_if ((pdf_date_year (date) != 1980) ||
           (pdf_date_month (date) != 8) ||
           (pdf_date_day (date) != 13) ||
           (pdf_date_hour (date) != 17) ||
           (pdf_date_minute (date) != 55) ||
           (pdf_date_second (date) != 0) ||
           (pdf_date_ut_offset_hour (date) != 0) ||
           (pdf_date_ut_offset_minute (date) != 0));
  pdf_destroy_date (date);
  pdf_destroy_obj (string);
}
END_TEST


START_TEST(create_7)
{
  pdf_obj_t string;
  pdf_date_t date;

  string = pdf_create_string ("19800813175500", 14);
  date = pdf_create_date (string);
  fail_if (date == NULL);
  fail_if ((pdf_date_year (date) != 1980) ||
           (pdf_date_month (date) != 8) ||
           (pdf_date_day (date) != 13) ||
           (pdf_date_hour (date) != 17) ||
           (pdf_date_minute (date) != 55) ||
           (pdf_date_second (date) != 0) ||
           (pdf_date_ut_offset_hour (date) != -1) ||
           (pdf_date_ut_offset_minute (date) != -1));
  pdf_destroy_date (date);
  pdf_destroy_obj (string);
}
END_TEST




START_TEST(create_8)
{
  pdf_obj_t string;
  pdf_date_t date; 

  string = pdf_create_string ("198008131755", 12);
  date = pdf_create_date (string);
  fail_if (date == NULL);
  fail_if ((pdf_date_year (date) != 1980) ||
           (pdf_date_month (date) != 8) ||
           (pdf_date_day (date) != 13) ||
           (pdf_date_hour (date) != 17) ||
           (pdf_date_minute (date) != 55) ||
           (pdf_date_second (date) != 0) ||
           (pdf_date_ut_offset_hour (date) != -1) ||
           (pdf_date_ut_offset_minute (date) != -1));
  pdf_destroy_date (date);
  pdf_destroy_obj (string);
}
END_TEST



START_TEST(create_9)
{
  pdf_obj_t string;
  pdf_date_t date;

  string = pdf_create_string ("1980081317", 10);
  date = pdf_create_date (string);
  fail_if (date == NULL);
  fail_if ((pdf_date_year (date) != 1980) ||
           (pdf_date_month (date) != 8) ||
           (pdf_date_day (date) != 13) ||
           (pdf_date_hour (date) != 17) ||
           (pdf_date_minute (date) != 0) ||
           (pdf_date_second (date) != 0) ||
           (pdf_date_ut_offset_hour (date) != -1) ||
           (pdf_date_ut_offset_minute (date) != -1));
  pdf_destroy_date (date);
  pdf_destroy_obj (string);
}
END_TEST




START_TEST(create_10)
{
  pdf_obj_t string;
  pdf_date_t date;

  string = pdf_create_string ("19800813", 8);
  date = pdf_create_date (string);
  fail_if (date == NULL);
  fail_if ((pdf_date_year (date) != 1980) ||
           (pdf_date_month (date) != 8) ||
           (pdf_date_day (date) != 13) ||
           (pdf_date_hour (date) != 0) ||
           (pdf_date_minute (date) != 0) ||
           (pdf_date_second (date) != 0) ||
           (pdf_date_ut_offset_hour (date) != -1) ||
           (pdf_date_ut_offset_minute (date) != -1));
  pdf_destroy_date (date);
  pdf_destroy_obj (string);
}
END_TEST



START_TEST(create_11)
{
  pdf_obj_t string;
  pdf_date_t date;

  string = pdf_create_string ("198008", 6);
  date = pdf_create_date (string);
  fail_if (date == NULL);
  fail_if ((pdf_date_year (date) != 1980) ||
           (pdf_date_month (date) != 8) ||
           (pdf_date_day (date) != 1) ||
           (pdf_date_hour (date) != 0) ||
           (pdf_date_minute (date) != 0) ||
           (pdf_date_second (date) != 0) ||
           (pdf_date_ut_offset_hour (date) != -1) ||
           (pdf_date_ut_offset_minute (date) != -1));
  pdf_destroy_date (date);
  pdf_destroy_obj (string);
}
END_TEST



START_TEST(create_12)
{
  pdf_obj_t string;
  pdf_date_t date;


  string = pdf_create_string ("1980", 4);
  date = pdf_create_date (string);
  fail_if (date == NULL);
  fail_if ((pdf_date_year (date) != 1980) ||
           (pdf_date_month (date) != 1) ||
           (pdf_date_day (date) != 1) ||
           (pdf_date_hour (date) != 0) ||
           (pdf_date_minute (date) != 0) ||
           (pdf_date_second (date) != 0) ||
           (pdf_date_ut_offset_hour (date) != -1) ||
           (pdf_date_ut_offset_minute (date) != -1));
  pdf_destroy_date (date);
  pdf_destroy_obj (string);
}
END_TEST




START_TEST(create_13)
{
  pdf_obj_t string;
  pdf_date_t date;


  string = pdf_create_string ("D:19800813175500+01'03", 22);
  date = pdf_create_date (string);
  fail_if (date != NULL);
  pdf_destroy_date (date);
  pdf_destroy_obj (string);
}
END_TEST



START_TEST(create_14)
{
  pdf_obj_t string;
  pdf_date_t date;


  string = pdf_create_string ("D:19800813175500+a1'", 20);
  date = pdf_create_date (string);
  fail_if (date != NULL);
  pdf_destroy_date (date);
  pdf_destroy_obj (string);
}
END_TEST



START_TEST(create_15)
{
  pdf_obj_t string;
  pdf_date_t date;

  string = pdf_create_string ("D:19z0", 6);
  date = pdf_create_date (string);
  fail_if (date != NULL);
  pdf_destroy_date (date);
  pdf_destroy_obj (string);
}
END_TEST



START_TEST(create_16)
{
  pdf_obj_t string;
  pdf_date_t date;

  string = pdf_create_string ("", 0);
  date = pdf_create_date (string);
  fail_if (date != NULL);
  pdf_destroy_date (date);
  pdf_destroy_obj (string);
}
END_TEST



TCase* test_date_tests(void)
{
  TCase *tc = tcase_create("test-date");
  tcase_add_test(tc, create_1);
  tcase_add_test(tc, create_2);
  tcase_add_test(tc, create_3);
  tcase_add_test(tc, create_4);
  tcase_add_test(tc, create_5);
  tcase_add_test(tc, create_6);
  tcase_add_test(tc, create_7);
  tcase_add_test(tc, create_8);
  tcase_add_test(tc, create_9);
  tcase_add_test(tc, create_10);
  tcase_add_test(tc, create_11);
  tcase_add_test(tc, create_12);
  tcase_add_test(tc, create_13);
  tcase_add_test(tc, create_14);
  tcase_add_test(tc, create_15);
  tcase_add_test(tc, create_16);
  return tc;
}



/* End of test-date.c */

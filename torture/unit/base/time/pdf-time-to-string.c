/* -*- mode: C -*- Time-stamp: "09/06/16 21:32:33 jemarch"
 *
 *       File:         pdf-time-to-string.c
 *       Date:         Fri Feb 27 17:35:31 2008
 *
 *       GNU PDF Library - Unit tests for pdf_time_to_string
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
#include <stdio.h>
#include <string.h>
#include <base/time/pdf-time-test-common.h>

#define INTERACTIVE_DEBUG 0


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
  pdf_status_t status;
  pdf_time_t time1;
  pdf_u32_t i;
  pdf_i32_t gmt;
  pdf_i32_t offset_hours;
  pdf_i32_t offset_minutes;

  pdf_char_t *testString;
  pdf_char_t *dateString;

  extern struct pdf_time_cal_s dates[];
  struct pdf_time_cal_s calendar;


  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  fail_if(time1 == NULL);


  testString = pdf_alloc(26); /* YYYY-MM-DDThh:mm:ss+hh:mm  - lenght = 26  */

  for (i=0; i<DATES_SIZE; i++){
          for (gmt =-12*60; gmt <=12*60; gmt+=51){  /* Set various gmt_offsets. */
            calendar.year = dates[i].year;
            calendar.month = dates[i].month;
            calendar.day = dates[i].day;
            calendar.hour = dates[i].hour;
            calendar.minute = dates[i].minute;
            calendar.second = dates[i].second;
            calendar.dow = dates[i].dow;
            calendar.gmt_offset = gmt*60;

            memset(&testString[0], 0, 26);

            pdf_time_from_cal(time1, &calendar);

            offset_hours = (((gmt < 0) ? (-1) : (1)) * gmt) / 60;
            offset_minutes = (((gmt < 0) ? (-1) : (1)) *gmt ) % 60;

            sprintf(&testString[0],"%d-%02d-%02dT%02d:%02d:%02d%c%02d:%02d",
                            dates[i].year, dates[i].month, dates[i].day,
                            dates[i].hour, dates[i].minute, dates[i].second,
                            ((gmt < 0) ? '-' : '+'),
                            offset_hours, offset_minutes);


            dateString = pdf_time_to_string(time1, PDF_TIME_FORMAT_ISO_8601);


            if (gmt == 0){
                /** Test YYYY-MM-DDThh:mm:ssZ format **/
                testString[19]='Z';
                testString[20]='\0';
            }
            fail_unless(strcmp(testString, dateString) == 0);

            
            pdf_dealloc(dateString);
        }
  }

  pdf_dealloc(testString);
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
  pdf_status_t status;
  pdf_time_t time1;
  pdf_u32_t i;
  pdf_i32_t gmt;
  pdf_i32_t offset_hours;
  pdf_i32_t offset_minutes;
  pdf_char_t *dateString;
  pdf_char_t *testString;

  extern struct pdf_time_cal_s dates[];
  struct pdf_time_cal_s calendar;


  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  fail_if(time1 == NULL);


  testString = pdf_alloc(24); /* D:YYYYMMDDHHmmSSOHH'mm'  - lenght = 24  */

  for (i=0; i<DATES_SIZE; i++){
          for (gmt =-12*60; gmt <=12*60; gmt+=51){  /* Set various gmt_offsets. */

            memset(&testString[0], 0, 24);

            calendar.year = dates[i].year;
            calendar.month = dates[i].month;
            calendar.day = dates[i].day;
            calendar.hour = dates[i].hour;
            calendar.minute = dates[i].minute;
            calendar.second = dates[i].second;
            calendar.dow = dates[i].dow;
            calendar.gmt_offset = gmt*60;

            status = pdf_time_from_cal(time1, &calendar);
            fail_if(status != PDF_OK);

            offset_hours = (((gmt < 0) ? (-1) : (1)) * gmt) / 60;
            offset_minutes = (((gmt < 0) ? (-1) : (1)) *gmt ) % 60;

            sprintf(&testString[0],"D:%d%02d%02d%02d%02d%02d%c%02d'%02d'",
                            dates[i].year, dates[i].month, dates[i].day,
                            dates[i].hour, dates[i].minute, dates[i].second,
                            ((gmt < 0) ? '-' : '+'),
                            offset_hours, offset_minutes);

            
            if (INTERACTIVE_DEBUG) 
                printf("pdf_time_from_string_002 > %s \n", testString);

            dateString = pdf_time_to_string(time1, PDF_TIME_FORMAT_PDF);
    

            if (gmt == 0){
                /** Test D:YYYYMMDDHHmmSSZ format **/
                testString[16]='Z';
                testString[17]='\0';
            }

            fail_unless(strcmp(testString, dateString) == 0);
            pdf_dealloc(dateString);


         }
  }

  pdf_dealloc(testString);
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
  pdf_status_t status;
  pdf_time_t time1;
  pdf_u32_t i;
  pdf_i32_t gmt;
  pdf_i32_t offset_hours;
  pdf_i32_t offset_minutes;

  pdf_i32_t seconds;
  pdf_char_t *dateString;
  pdf_char_t *testString;

  extern struct pdf_time_cal_s dates[];
  struct pdf_time_cal_s calendar;


  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  fail_if(time1 == NULL);


  testString = pdf_alloc(20); /* YYYYMMDDhhmmss+hhmm  - lenght = 20 */

  for (i=0; i<DATES_SIZE; i++){
          for (gmt =-12*60; gmt <=12*60; gmt+=51){  /* Set various gmt_offsets. */

            memset(&testString[0], 0, 20);
            
            calendar.year = dates[i].year;
            calendar.month = dates[i].month;
            calendar.day = dates[i].day;
            calendar.hour = dates[i].hour;
            calendar.minute = dates[i].minute;
            calendar.second = dates[i].second;
            calendar.dow = dates[i].dow;
            calendar.gmt_offset = gmt*60;

            status = pdf_time_from_cal(time1, &calendar);
            fail_if(status != PDF_OK);


            offset_hours = (((gmt < 0) ? (-1) : (1)) * gmt) / 60;
            offset_minutes = (((gmt < 0) ? (-1) : (1)) *gmt ) % 60;

            sprintf(&testString[0],"%d%02d%02d%02d%02d%02d%c%02d%02d",
                            dates[i].year, dates[i].month, dates[i].day,
                            dates[i].hour, dates[i].minute,dates[i].second,
                            ((gmt < 0) ? '-' : '+'),
                            offset_hours, offset_minutes);


            dateString = pdf_time_to_string(time1, PDF_TIME_FORMAT_GENERALIZED_ASN1);
    

            if (gmt == 0){
                /** Test YYYYMMDDhhmmssZ format **/
                testString[14]='Z';
                testString[15]='\0';
            }

            fail_unless(strcmp(dateString, testString) == 0);
            pdf_dealloc(dateString);
        }
  }

  pdf_dealloc(testString);
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
  pdf_status_t status;
  pdf_time_t time1;
  pdf_u32_t i;
  pdf_i32_t gmt;
  pdf_i32_t offset_hours;
  pdf_i32_t offset_minutes;

  pdf_i32_t seconds;
  pdf_char_t *dateString;
  pdf_char_t *testString;

  extern struct pdf_time_cal_s dates[];
  struct pdf_time_cal_s calendar;


  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  fail_if(time1 == NULL);

  testString = pdf_alloc(18); /* yymmddhhmmss+hhmm  - lenght = 18 */

  for (i=0; i<DATES_SIZE; i++){
          for (gmt =-12*60; gmt <=12*60; gmt+=51){  /* Set various gmt_offsets. */

            memset(&testString[0], 0, 20);

            calendar.year = dates[i].year;
            calendar.month = dates[i].month;
            calendar.day = dates[i].day;
            calendar.hour = dates[i].hour;
            calendar.minute = dates[i].minute;
            calendar.second = dates[i].second;
            calendar.dow = dates[i].dow;
            calendar.gmt_offset = gmt*60;

            status = pdf_time_from_cal(time1, &calendar);
            fail_if(status != PDF_OK);


            offset_hours = (((gmt < 0) ? (-1) : (1)) * gmt) / 60;
            offset_minutes = (((gmt < 0) ? (-1) : (1)) *gmt ) % 60;
            
            int year = dates[i].year;
            if (dates[i].year <2000) year -=1900;
            else year -=2000;
            sprintf(&testString[0],"%02d%02d%02d%02d%02d%02d%c%02d%02d",
                            year, dates[i].month, dates[i].day, dates[i].hour,
                            dates[i].minute, dates[i].second,
                            ((gmt < 0) ? '-' : '+'),
                            offset_hours, offset_minutes);


            dateString = pdf_time_to_string(time1,  PDF_TIME_FORMAT_UTC_ASN1);
            fail_if(status != PDF_OK);
    

            if (gmt == 0){
                /** Test yymmddhhmmssZ format **/
                testString[12]='Z';
                testString[13]='\0';
            }
            int w =strcmp(dateString, testString);

            fail_unless(strcmp(dateString, testString) == 0);
            pdf_dealloc(dateString);

        }
  }

  pdf_dealloc(testString);
}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_time_to_string (void)
{
  TCase *tc = tcase_create ("pdf_time_to_string");

  tcase_add_test(tc, pdf_time_to_string_001);
  tcase_add_test(tc, pdf_time_to_string_002);
  tcase_add_test(tc, pdf_time_to_string_003);
  tcase_add_test(tc, pdf_time_to_string_004);

  return tc;
}

/* End of pdf-time-to-string.c */

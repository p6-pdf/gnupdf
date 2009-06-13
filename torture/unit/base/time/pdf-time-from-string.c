/* -*- mode: C -*- Time-stamp: "09/05/07 12:22:20 jemarch"
 *
 *       File:         pdf-time-from-string.c
 *       Date:         Fri Feb 27 17:35:31 2008
 *
 *       GNU PDF Library - Unit tests for pdf_time_from_string
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
#include <string.h>
#include <check.h>
#include <pdf.h>
#include <stdlib.h>
#include <stdio.h>
#include <base/time/pdf-time-test-common.h>

#define INTERACTIVE_DEBUG 0


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
  pdf_status_t status;
  pdf_time_t time1;
  pdf_time_t time2;
  pdf_u32_t i;
  pdf_i32_t gmt;
  pdf_i32_t offset_hours;
  pdf_i32_t offset_minutes;

  pdf_i32_t seconds;
  pdf_char_t *dateString;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t datesInSeconds[];
  struct pdf_time_cal_s calendar;


  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  fail_if(time1 == NULL);

  status = pdf_time_new(&time2);
  fail_if(status != PDF_OK);
  fail_if(time2 == NULL);

  dateString = pdf_alloc(26); /* YYYY-MM-DDThh:mm:ss+hh:mm  - length = 26  */

  for (i=0; i<DATES_SIZE; i++){
          for (gmt =-12*60; gmt <=12*60; gmt+=5){  /* Set various gmt_offsets. */
            seconds=datesInSeconds[i] + dates[i].second + dates[i].minute*60 + \
            dates[i].hour*3600 - gmt*60;

            if ( seconds < 0) continue;
            memset(&dateString[0], 0, 26);

            pdf_time_set_from_u32(time1,seconds);


            offset_hours = (((gmt < 0) ? (-1) : (1)) * gmt) / 60;
            offset_minutes = (((gmt < 0) ? (-1) : (1)) *gmt ) % 60;

            sprintf(&dateString[0],"%d-%s%d-%s%dT%s%d:%s%d:%s%d%c%s%d:%s%d",
                            dates[i].year, 
                            (dates[i].month < 10 ? "0":""), dates[i].month,
                            (dates[i].day < 10 ? "0":""), dates[i].day,
                            (dates[i].hour < 10 ? "0":""), dates[i].hour,
                            (dates[i].minute < 10 ? "0":""), dates[i].minute,
                            (dates[i].second < 10 ? "0":""), dates[i].second,
                            ((gmt < 0) ? '-' : '+'),
                            (offset_hours < 10 ? "0" : ""), offset_hours,
                            (offset_minutes < 10 ? "0" : ""), offset_minutes);


            status = pdf_time_from_string(time2,dateString, PDF_TIME_FORMAT_ISO_8601);
            fail_if(status != PDF_OK);
    
            fail_unless(pdf_time_cmp(time1, time2) == 0);


            if (gmt == 0){
                /** Test YYYY-MM-DDThh:mm:ssZ format **/
                dateString[19]='Z';
                dateString[20]='\0';
                status = pdf_time_from_string(time2,dateString, PDF_TIME_FORMAT_ISO_8601);
                fail_if(status != PDF_OK);
    
                fail_unless(pdf_time_cmp(time1, time2) == 0);

            }
        }
  }

  pdf_dealloc(dateString);
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
  pdf_status_t status;
  pdf_time_t time1;
  pdf_time_t time2;
  pdf_u32_t i;
  pdf_i32_t gmt;
  pdf_i32_t offset_hours;
  pdf_i32_t offset_minutes;

  pdf_i32_t seconds;
  pdf_char_t *dateString;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t datesInSeconds[];
  struct pdf_time_cal_s calendar;


  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  fail_if(time1 == NULL);

  status = pdf_time_new(&time2);
  fail_if(status != PDF_OK);
  fail_if(time2 == NULL);

  dateString = pdf_alloc(23); /* YYYY-MM-DDThh:mm+hh:mm  - length = 23  */

  for (i=0; i<DATES_SIZE; i++){
          for (gmt =-12*60; gmt <=12*60; gmt+=5){  /* Set various gmt_offsets. */
            seconds=datesInSeconds[i] +  dates[i].minute*60 + \
            dates[i].hour*3600 - gmt*60;

            if ( seconds < 0) continue;
            memset(&dateString[0], 0, 26);

            pdf_time_set_from_u32(time1,seconds);


            offset_hours = (((gmt < 0) ? (-1) : (1)) * gmt) / 60;
            offset_minutes = (((gmt < 0) ? (-1) : (1)) *gmt ) % 60;

            sprintf(&dateString[0],"%d-%s%d-%s%dT%s%d:%s%d%c%s%d:%s%d",
                            dates[i].year, 
                            (dates[i].month < 10 ? "0":""), dates[i].month,
                            (dates[i].day < 10 ? "0":""), dates[i].day,
                            (dates[i].hour < 10 ? "0":""), dates[i].hour,
                            (dates[i].minute < 10 ? "0":""), dates[i].minute,
                            ((gmt < 0) ? '-' : '+'),
                            (offset_hours < 10 ? "0" : ""), offset_hours,
                            (offset_minutes < 10 ? "0" : ""), offset_minutes);


/** Error in pdf_time_string which doesn't notice
 * that time is in format without seconds. */
            status = pdf_time_from_string(time2,dateString, PDF_TIME_FORMAT_ISO_8601);
            fail_if(status != PDF_OK);
    
            fail_unless(pdf_time_cmp(time1, time2) == 0);

            if (gmt == 0){
                /** Test YYYY-MM-DDThh:mmZ format **/
                dateString[16]='Z';
                dateString[17]='\0';
                status = pdf_time_from_string(time2,dateString, PDF_TIME_FORMAT_ISO_8601);
                fail_if(status != PDF_OK);
    
                fail_unless(pdf_time_cmp(time1, time2) == 0);

            }
 
        }
  }

  pdf_dealloc(dateString);
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
  pdf_status_t status;
  pdf_time_t time1;
  pdf_time_t time2;
  pdf_u32_t i;

  pdf_i32_t seconds;
  pdf_char_t *dateString;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t datesInSeconds[];


  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  fail_if(time1 == NULL);

  status = pdf_time_new(&time2);
  fail_if(status != PDF_OK);
  fail_if(time2 == NULL);

  dateString = pdf_alloc(11); /* YYYY-MM-DD  - length = 11 */

  for (i=0; i<DATES_SIZE; i++){
        seconds=datesInSeconds[i];

        if ( seconds < 0) continue;
        memset(&dateString[0], 0, 11);

        pdf_time_set_from_u32(time1,seconds);

        sprintf(&dateString[0],"%d-%s%d-%s%d",
                      dates[i].year, 
                     (dates[i].month < 10 ? "0":""), dates[i].month,
                     (dates[i].day < 10 ? "0":""), dates[i].day);


        if (INTERACTIVE_DEBUG)
           printf("pdf_time_from_string_003 %s %i\n",dateString,i);
        status = pdf_time_from_string(time2,dateString, PDF_TIME_FORMAT_ISO_8601);
       fail_if(status != PDF_OK);

    
       fail_unless(pdf_time_cmp(time1, time2) == 0);
  }

  pdf_dealloc(dateString);
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
  pdf_status_t status;
  pdf_time_t time1;
  pdf_time_t time2;
  pdf_u32_t i;

  pdf_i32_t seconds;
  pdf_char_t *dateString;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t datesInSeconds[];


  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  fail_if(time1 == NULL);

  status = pdf_time_new(&time2);
  fail_if(status != PDF_OK);
  fail_if(time2 == NULL);

  dateString = pdf_alloc(8); /* YYYY-MM  - length = 8 */

  for (i=0; i<DATES_SIZE; i++){
            seconds=datesInSeconds[i];

            if ( seconds < 0) continue;
            memset(&dateString[0], 0, 8);

            pdf_time_set_from_u32(time1,seconds);

            sprintf(&dateString[0],"%d-%s%d",
                            dates[i].year, 
                            (dates[i].month < 10 ? "0":""), dates[i].month);


 /**
 *  pdf_time_from_string returns PDF_EBADDATA
 *  when date string is in format "YYYY-MM".
 *  Function creates internal calendar which is
 *  initialized by date in string format. However
 *  functions forgets about setting month and day
 *  to one. In effect calandar points to
 *  YYYY-MM-00 which is wrong value. 
 */
            if (INTERACTIVE_DEBUG)
                printf("pdf_time_from_string_004 %s %i\n",dateString,i);
            status = pdf_time_from_string(time2,dateString, PDF_TIME_FORMAT_ISO_8601);
            fail_if(status != PDF_OK);

    
            fail_unless(pdf_time_cmp(time1, time2) == 0);
  }

  pdf_dealloc(dateString);
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
  pdf_status_t status;
  pdf_time_t time1;
  pdf_time_t time2;
  pdf_u32_t i;

  pdf_i32_t seconds;
  pdf_char_t *dateString;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t datesInSeconds[];


  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  fail_if(time1 == NULL);

  status = pdf_time_new(&time2);
  fail_if(status != PDF_OK);
  fail_if(time2 == NULL);

  dateString = pdf_alloc(5); /* YYYY  - length = 5  */

  for (i=0, seconds=0; i<DATES_SIZE; i++, seconds+=SEC_IN_NOLEEP_YEAR){
    if ((i-2)%4 == 0) seconds +=SEC_IN_DAY;     //add one day - 29 February

    
    memset(&dateString[0], 0, 5);

    pdf_time_set_from_u32(time1,seconds);

    sprintf(&dateString[0],"%d",1970+i);
    if (INTERACTIVE_DEBUG)
        printf("pdf_time_from_string_005 %s %d %d\n",dateString,i, seconds);


                             
/**
 *  pdf_time_from_string returns PDF_EBADDATA
 *  when date string is in format "YYYY".
 *  Function creates internal calendar which is
 *  initialized by date in string format. However
 *  functions forgets about setting month and day
 *  to one. In effect calandar points to
 *  YYYY-00-00 which is wrong calendar value. 
 */
    status = pdf_time_from_string(time2,dateString, PDF_TIME_FORMAT_ISO_8601);
    fail_if(status != PDF_OK);
    
    fail_unless(pdf_time_cmp(time1, time2) == 0);


  }

  pdf_dealloc(dateString);
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
 * 1. Function pdf_time_from_string schould return
 * PDF_OK.
 * 2. Returned pdf_time_t object schould point the same time
 * as pdf_time_t object created by pdf_time_from_u32_t.
 * 3. pdf_time_cmp schould return PDF_OK
 */
START_TEST (pdf_time_from_string_006)
{
  pdf_status_t status;
  pdf_time_t time1;
  pdf_time_t time2;
  pdf_u32_t i;
  pdf_i32_t gmt;
  pdf_i32_t offset_hours;
  pdf_i32_t offset_minutes;
  pdf_i32_t seconds;
  pdf_char_t *dateString;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t datesInSeconds[];
  struct pdf_time_cal_s calendar;


  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  fail_if(time1 == NULL);

  status = pdf_time_new(&time2);
  fail_if(status != PDF_OK);
  fail_if(time2 == NULL);

  dateString = pdf_alloc(24); /* D:YYYYMMDDHHmmSSOHH'mm'  - length = 24  */

  for (i=0; i<DATES_SIZE; i++){
          for (gmt =-12*60; gmt <=12*60; gmt+=9){  /* Set various gmt_offsets. */
            seconds=datesInSeconds[i] + dates[i].hour*3600 + dates[i].minute*60 + \
                                     dates[i].second - gmt*60;

            if ( seconds < 0) continue;
            memset(&dateString[0], 0, 24);

            pdf_time_set_from_u32(time1,seconds);


            offset_hours = (((gmt < 0) ? (-1) : (1)) * gmt) / 60;
            offset_minutes = (((gmt < 0) ? (-1) : (1)) *gmt ) % 60;

            sprintf(&dateString[0],"D:%d%s%d%s%d%s%d%s%d%s%d%c%s%d'%s%d'",
                            dates[i].year, 
                            (dates[i].month < 10 ? "0":""), dates[i].month,
                            (dates[i].day < 10 ? "0":""), dates[i].day,
                            (dates[i].hour < 10 ? "0":""), dates[i].hour,
                            (dates[i].minute < 10 ? "0":""), dates[i].minute,
                            (dates[i].second < 10 ? "0":""), dates[i].second,
                            ((gmt < 0) ? '-' : '+'),
                            (offset_hours < 10 ? "0" : ""), offset_hours,
                            (offset_minutes < 10 ? "0" : ""), offset_minutes);


            fail_if(PDF_OK != PDF_OK);
            
            if (INTERACTIVE_DEBUG) 
                printf("pdf_time_from_string_006 > %s \n", dateString);

            status = pdf_time_from_string(time2,dateString, PDF_TIME_FORMAT_PDF);
            fail_if(status != PDF_OK);
    
            fail_unless(pdf_time_cmp(time1, time2) == 0);

            /* Check format without last "'" 
             * (From ISO 32000 )
             * */ 
            dateString[22]='\0';
 
            if (INTERACTIVE_DEBUG) 
                printf("pdf_time_from_string_006 > %s \n", dateString);

/** Test fails because pdf_time_check_string_pdf
 * returns PDF_EBADDATA when last ' symbol is
 * missing. However comment of
 * pdf_time_from_string_pdf says that last ' is
 * optional */
            status = pdf_time_from_string(time2,dateString, PDF_TIME_FORMAT_PDF);
            fail_if(status != PDF_OK);
    
            fail_unless(pdf_time_cmp(time1, time2) == 0);

            if (gmt == 0){
                /** Test D:YYYYMMDDHHmmSSZ format **/
                dateString[16]='Z';
                dateString[17]='\0';

                status = pdf_time_from_string(time2,dateString, PDF_TIME_FORMAT_PDF);
                fail_if(status != PDF_OK);
    
                fail_unless(pdf_time_cmp(time1, time2) == 0);

            }


         }
  }

  pdf_dealloc(dateString);
}
END_TEST


/*
 * Test: pdf_time_from_string_007
 * Description:
 *   Get pdf_time_t object from string in full 
 *   PDF_TIME_FORMAT_GENERALIZED_ASN1:
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
  pdf_status_t status;
  pdf_time_t time1;
  pdf_time_t time2;
  pdf_u32_t i;
  pdf_i32_t gmt;
  pdf_i32_t offset_hours;
  pdf_i32_t offset_minutes;

  pdf_i32_t seconds;
  pdf_char_t *dateString;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t datesInSeconds[];
  struct pdf_time_cal_s calendar;


  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  fail_if(time1 == NULL);

  status = pdf_time_new(&time2);
  fail_if(status != PDF_OK);
  fail_if(time2 == NULL);

  dateString = pdf_alloc(20); /* YYYYMMDDhhmmss+hhmm  - length = 20 */

  for (i=0; i<DATES_SIZE; i++){
          for (gmt =-12*60; gmt <=12*60; gmt+=5){  /* Set various gmt_offsets. */
            seconds=datesInSeconds[i] + dates[i].second + dates[i].minute*60 + \
            dates[i].hour*3600 - gmt*60;

            if ( seconds < 0) continue;
            memset(&dateString[0], 0, 20);

            pdf_time_set_from_u32(time1,seconds);


            offset_hours = (((gmt < 0) ? (-1) : (1)) * gmt) / 60;
            offset_minutes = (((gmt < 0) ? (-1) : (1)) *gmt ) % 60;

            sprintf(&dateString[0],"%d%s%d%s%d%s%d%s%d%s%d%c%s%d%s%d",
                            dates[i].year, 
                            (dates[i].month < 10 ? "0":""), dates[i].month,
                            (dates[i].day < 10 ? "0":""), dates[i].day,
                            (dates[i].hour < 10 ? "0":""), dates[i].hour,
                            (dates[i].minute < 10 ? "0":""), dates[i].minute,
                            (dates[i].second < 10 ? "0":""), dates[i].second,
                            ((gmt < 0) ? '-' : '+'),
                            (offset_hours < 10 ? "0" : ""), offset_hours,
                            (offset_minutes < 10 ? "0" : ""), offset_minutes);


            status = pdf_time_from_string(time2,dateString, PDF_TIME_FORMAT_GENERALIZED_ASN1);
            fail_if(status != PDF_OK);
    
            fail_unless(pdf_time_cmp(time1, time2) == 0);


            if (gmt == 0){
                /** Test YYYYMMDDhhmmssZ format **/
                dateString[14]='Z';
                dateString[15]='\0';
                status = pdf_time_from_string(time2,dateString, PDF_TIME_FORMAT_GENERALIZED_ASN1);
                fail_if(status != PDF_OK);
    
                fail_unless(pdf_time_cmp(time1, time2) == 0);

            }
        }
  }

  pdf_dealloc(dateString);
}
END_TEST

/*
 * Test: pdf_time_from_string_008
 * Description:
 *   Get pdf_time_t object from string in full 
 *   PDF_TIME_FORMAT_GENERALIZED_ASN1:
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
  pdf_status_t status;
  pdf_time_t time1;
  pdf_time_t time2;
  pdf_u32_t i;
  pdf_i32_t gmt;
  pdf_i32_t offset_hours;
  pdf_i32_t offset_minutes;

  pdf_i32_t seconds;
  pdf_char_t *dateString;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t datesInSeconds[];
  struct pdf_time_cal_s calendar;


  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  fail_if(time1 == NULL);

  status = pdf_time_new(&time2);
  fail_if(status != PDF_OK);
  fail_if(time2 == NULL);

  dateString = pdf_alloc(18); /* YYYYMMDDhhmm+hhmm  - length = 18  */

  for (i=0; i<DATES_SIZE; i++){
          for (gmt =-12*60; gmt <=12*60; gmt+=5){  /* Set various gmt_offsets. */
            seconds=datesInSeconds[i] +  dates[i].minute*60 + \
            dates[i].hour*3600 - gmt*60;

            if ( seconds < 0) continue;
            memset(&dateString[0], 0, 18);

            pdf_time_set_from_u32(time1,seconds);


            offset_hours = (((gmt < 0) ? (-1) : (1)) * gmt) / 60;
            offset_minutes = (((gmt < 0) ? (-1) : (1)) *gmt ) % 60;

            sprintf(&dateString[0],"%d%s%d%s%d%s%d%s%d%c%s%d%s%d",
                            dates[i].year, 
                            (dates[i].month < 10 ? "0":""), dates[i].month,
                            (dates[i].day < 10 ? "0":""), dates[i].day,
                            (dates[i].hour < 10 ? "0":""), dates[i].hour,
                            (dates[i].minute < 10 ? "0":""), dates[i].minute,
                            ((gmt < 0) ? '-' : '+'),
                            (offset_hours < 10 ? "0" : ""), offset_hours,
                            (offset_minutes < 10 ? "0" : ""), offset_minutes);


            status = pdf_time_from_string(time2,dateString, PDF_TIME_FORMAT_GENERALIZED_ASN1);
            fail_if(status != PDF_OK);
    
            fail_unless(pdf_time_cmp(time1, time2) == 0);

            if (gmt == 0){
                /** Test YYYYMMDDhhmmZ format **/
                dateString[12]='Z';
                dateString[13]='\0';
                status = pdf_time_from_string(time2,dateString, PDF_TIME_FORMAT_GENERALIZED_ASN1);
                fail_if(status != PDF_OK);
    
                fail_unless(pdf_time_cmp(time1, time2) == 0);

            }
 
        }
  }

  pdf_dealloc(dateString);
}
END_TEST

/*
 * Test: pdf_time_from_string_009
 * Description:
 *   Get pdf_time_t object from string in full 
 *   PDF_TIME_FORMAT_GENERALIZED_ASN1:
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
  pdf_status_t status;
  pdf_time_t time1;
  pdf_time_t time2;
  pdf_u32_t i;

  pdf_i32_t seconds;
  pdf_char_t *dateString;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t datesInSeconds[];


  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  fail_if(time1 == NULL);

  status = pdf_time_new(&time2);
  fail_if(status != PDF_OK);
  fail_if(time2 == NULL);

  dateString = pdf_alloc(9); /* YYYYMMDD  - length = 9 */

  for (i=0; i<DATES_SIZE; i++){
    seconds=datesInSeconds[i];

    if ( seconds < 0) continue;
    memset(&dateString[0], 0, 9);

    pdf_time_set_from_u32(time1,seconds);

    sprintf(&dateString[0],"%d%s%d%s%d",
            dates[i].year, 
            (dates[i].month < 10 ? "0":""), dates[i].month,
            (dates[i].day < 10 ? "0":""), dates[i].day);


     status = pdf_time_from_string(time2,dateString, PDF_TIME_FORMAT_GENERALIZED_ASN1);
     fail_if(status != PDF_OK);

    
     fail_unless(pdf_time_cmp(time1, time2) == 0);
  }

  pdf_dealloc(dateString);
}
END_TEST

/*
 * Test: pdf_time_from_string_010
 * Description:
 *   Get pdf_time_t object from string in full 
 *   PDF_TIME_FORMAT_GENERALIZED_ASN1:
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
  pdf_status_t status;
  pdf_time_t time1;
  pdf_time_t time2;
  pdf_u32_t i;

  pdf_i32_t seconds;
  pdf_char_t *dateString;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t datesInSeconds[];


  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  fail_if(time1 == NULL);

  status = pdf_time_new(&time2);
  fail_if(status != PDF_OK);
  fail_if(time2 == NULL);

  dateString = pdf_alloc(7); /* YYYYMM  - length = 7 */

  for (i=0; i<DATES_SIZE; i++){
      seconds=datesInSeconds[i];

        if ( seconds < 0) continue;
        memset(&dateString[0], 0, 7);

        pdf_time_set_from_u32(time1,seconds);

        sprintf(&dateString[0],"%d%s%d",
                      dates[i].year, 
                     (dates[i].month < 10 ? "0":""), dates[i].month);


 /**
 *  pdf_time_from_string returns PDF_EBADDATA
 *  when date string is in format "YYYYMM".
 *  Function creates internal calendar which is
 *  initialized by date in string format. However
 *  functions forgets about setting month and day
 *  to one. In effect calandar points to
 *  YYYY-MM-00 which is wrong value. 
 */
        if (INTERACTIVE_DEBUG)
            printf("pdf_time_from_string_010 %s %i\n",dateString,i);
        status = pdf_time_from_string(time2,dateString, PDF_TIME_FORMAT_GENERALIZED_ASN1);
        fail_if(status != PDF_OK);

    
        fail_unless(pdf_time_cmp(time1, time2) == 0);
  }

  pdf_dealloc(dateString);
}
END_TEST


/*
 * Test: pdf_time_from_string_011
 * Description:
 *   Get pdf_time_t object from string in full 
 *   PDF_TIME_FORMAT_GENERALIZED_ASN1:
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
  pdf_status_t status;
  pdf_time_t time1;
  pdf_time_t time2;
  pdf_u32_t i;

  pdf_i32_t seconds;
  pdf_char_t *dateString;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t datesInSeconds[];


  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  fail_if(time1 == NULL);

  status = pdf_time_new(&time2);
  fail_if(status != PDF_OK);
  fail_if(time2 == NULL);

  dateString = pdf_alloc(5); /* YYYY  - length = 5  */

  for (i=0, seconds=0; i<40; i++, seconds+=SEC_IN_NOLEEP_YEAR){
    if ((i-2)%4 == 0) seconds +=SEC_IN_DAY;     //add one day - 29 February

    
    memset(&dateString[0], 0, 5);

    pdf_time_set_from_u32(time1,seconds);

    sprintf(&dateString[0],"%d",1970+i);
    if (INTERACTIVE_DEBUG) 
        printf("pdf_time_from_string_011 %s %d %d\n",dateString,i, seconds);


                             
/**
 *  pdf_time_from_string returns PDF_EBADDATA
 *  when date string is in format "YYYY".
 *  Function creates internal calendar which is
 *  initialized by date in string format. However
 *  functions forgets about setting month and day
 *  to one. In effect calandar points to
 *  YYYY-00-00 which is wrong value. 
 */
    status = pdf_time_from_string(time2,dateString, PDF_TIME_FORMAT_GENERALIZED_ASN1);
    fail_if(status != PDF_OK);
    
    fail_unless(pdf_time_cmp(time1, time2) == 0);


  }

  pdf_dealloc(dateString);
}
END_TEST


/*
 * Test: pdf_time_from_string_012
 * Description:
 *   Get pdf_time_t object from string in full 
 *   PDF_TIME_FORMAT_UTC_ASN1:
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
  pdf_status_t status;
  pdf_time_t time1;
  pdf_time_t time2;
  pdf_u32_t i;
  pdf_i32_t gmt;
  pdf_i32_t offset_hours;
  pdf_i32_t offset_minutes;

  pdf_i32_t seconds;
  pdf_char_t *dateString;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t datesInSeconds[];
  struct pdf_time_cal_s calendar;


  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  fail_if(time1 == NULL);

  status = pdf_time_new(&time2);
  fail_if(status != PDF_OK);
  fail_if(time2 == NULL);

  dateString = pdf_alloc(18); /* yymmddhhmmss+hhmm  - length = 18 */

  for (i=0; i<DATES_SIZE; i++){
          for (gmt =-12*60; gmt <=12*60; gmt+=5){  /* Set various gmt_offsets. */
            seconds=datesInSeconds[i] + dates[i].second + dates[i].minute*60 + \
            dates[i].hour*3600 - gmt*60;

            if ( seconds < 0) continue;
            memset(&dateString[0], 0, 20);

            pdf_time_set_from_u32(time1,seconds);


            offset_hours = (((gmt < 0) ? (-1) : (1)) * gmt) / 60;
            offset_minutes = (((gmt < 0) ? (-1) : (1)) *gmt ) % 60;

            sprintf(&dateString[0],"%d%s%d%s%d%s%d%s%d%s%d%c%s%d%s%d",
                            dates[i].year, 
                            (dates[i].month < 10 ? "0":""), dates[i].month,
                            (dates[i].day < 10 ? "0":""), dates[i].day,
                            (dates[i].hour < 10 ? "0":""), dates[i].hour,
                            (dates[i].minute < 10 ? "0":""), dates[i].minute,
                            (dates[i].second < 10 ? "0":""), dates[i].second,
                            ((gmt < 0) ? '-' : '+'),
                            (offset_hours < 10 ? "0" : ""), offset_hours,
                            (offset_minutes < 10 ? "0" : ""), offset_minutes);


            status = pdf_time_from_string(time2,&dateString[2], PDF_TIME_FORMAT_UTC_ASN1);
            fail_if(status != PDF_OK);
    
            fail_unless(pdf_time_cmp(time1, time2) == 0);


            if (gmt == 0){
                /** Test yymmddhhmmssZ format **/
                dateString[14]='Z';
                dateString[15]='\0';
                status = pdf_time_from_string(time2,&dateString[2], PDF_TIME_FORMAT_UTC_ASN1);
                fail_if(status != PDF_OK);
    
                fail_unless(pdf_time_cmp(time1, time2) == 0);

            }
        }
  }

  pdf_dealloc(dateString);
}
END_TEST

/*
 * Test: pdf_time_from_string_013
 * Description:
 *   Get pdf_time_t object from string in full 
 *   PDF_TIME_FORMAT_UTC_ASN1:
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
  pdf_status_t status;
  pdf_time_t time1;
  pdf_time_t time2;
  pdf_u32_t i;
  pdf_i32_t gmt;
  pdf_i32_t offset_hours;
  pdf_i32_t offset_minutes;

  pdf_i32_t seconds;
  pdf_char_t *dateString;

  extern struct pdf_time_cal_s dates[];
  extern pdf_u32_t datesInSeconds[];
  struct pdf_time_cal_s calendar;


  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  fail_if(time1 == NULL);

  status = pdf_time_new(&time2);
  fail_if(status != PDF_OK);
  fail_if(time2 == NULL);

  dateString = pdf_alloc(18); /* YYMMDDhhmm+hhmm  - length = 16  */

  for (i=0; i<DATES_SIZE; i++){
          for (gmt =-12*60; gmt <=12*60; gmt+=5){  /* Set various gmt_offsets. */
            seconds=datesInSeconds[i] +  dates[i].minute*60 + \
            dates[i].hour*3600 - gmt*60;

            if ( seconds < 0) continue;
            memset(&dateString[0], 0, 18);

            pdf_time_set_from_u32(time1,seconds);


            offset_hours = (((gmt < 0) ? (-1) : (1)) * gmt) / 60;
            offset_minutes = (((gmt < 0) ? (-1) : (1)) *gmt ) % 60;

            sprintf(&dateString[0],"%d%s%d%s%d%s%d%s%d%c%s%d%s%d",
                            dates[i].year, 
                            (dates[i].month < 10 ? "0":""), dates[i].month,
                            (dates[i].day < 10 ? "0":""), dates[i].day,
                            (dates[i].hour < 10 ? "0":""), dates[i].hour,
                            (dates[i].minute < 10 ? "0":""), dates[i].minute,
                            ((gmt < 0) ? '-' : '+'),
                            (offset_hours < 10 ? "0" : ""), offset_hours,
                            (offset_minutes < 10 ? "0" : ""), offset_minutes);


/** Error in pdf_time_string which doesn't notice
 * that time is in format without seconds. */
            status = pdf_time_from_string(time2,&dateString[2], PDF_TIME_FORMAT_UTC_ASN1);
            fail_if(status != PDF_OK);
    
            fail_unless(pdf_time_cmp(time1, time2) == 0);

            if (gmt == 0){
                /** Test YYMMDDhhmmZ format **/
                dateString[12]='Z';
                dateString[13]='\0';
                status = pdf_time_from_string(time2,&dateString[2], PDF_TIME_FORMAT_UTC_ASN1);
                fail_if(status != PDF_OK);
    
                fail_unless(pdf_time_cmp(time1, time2) == 0);

            }
 
        }
  }

  pdf_dealloc(dateString);
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_time_from_string (void)
{
  TCase *tc = tcase_create ("pdf_time_from_string");

  tcase_add_test(tc, pdf_time_from_string_001);
  tcase_add_test(tc, pdf_time_from_string_002);
  tcase_add_test(tc, pdf_time_from_string_003);
  tcase_add_test(tc, pdf_time_from_string_004);
  tcase_add_test(tc, pdf_time_from_string_005);
  tcase_add_test(tc, pdf_time_from_string_006);
  tcase_add_test(tc, pdf_time_from_string_007);
  tcase_add_test(tc, pdf_time_from_string_008);
  tcase_add_test(tc, pdf_time_from_string_009);
  tcase_add_test(tc, pdf_time_from_string_010);
  tcase_add_test(tc, pdf_time_from_string_011);
  tcase_add_test(tc, pdf_time_from_string_012);
  tcase_add_test(tc, pdf_time_from_string_013);


  return tc;
}

/* End of pdf-time-from-string.c */

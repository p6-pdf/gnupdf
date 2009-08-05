/* -*- mode: C -*- Time-stamp: "2009-08-05 22:05:41 davazp"
 *
 *       File:         pdf-time-set-local-offset.c
 *       Date:         Fri Feb 27 17:35:31 2008
 *
 *       GNU PDF Library - Unit tests for pdf_time_set_local_offset
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
#include <time.h>

/*
 * Test: pdf_time_set_local_offset_001
 * Description:
 * Invoke pdf_time_set_local_offset on newly created
 * pdf_time_t object. Than compare its gmt_offset
 * with offset taken from system.
 *   
 *Success condition:
 * 1. Function pdf_time_new schould return PDF_OK
 * 2. Function pdf_time_set_local_offset schould return
 * PDF_OK.
 * 3. Function pdf_time_get_utc_cal schould return
 * PDF_OK.
 * 4. In returned utccal field gmt_offset must be
 * equal to offset returned by system.
 */
START_TEST (pdf_time_set_local_offset_001)
{
  pdf_status_t status;

  pdf_time_t time1;
  struct pdf_time_cal_s utccal;
  pdf_u32_t localgmt;
  time_t tloc;
  struct tm* time_struct;
    
  pdf_init();

  time(&tloc);
  time_struct = localtime(&tloc);

#if defined(PDF_HOST_WIN32)
  localgmt = _timezone;
#else
  localgmt = time_struct->tm_gmtoff;
#endif

  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  
  status = pdf_time_set_local_offset(time1);
  fail_if(status != PDF_OK);

  status = pdf_time_get_utc_cal(time1, &utccal);
  fail_if(status != PDF_OK);

  fail_if(utccal.gmt_offset != localgmt);
 
  status = pdf_time_destroy(time1);
  fail_if(status != PDF_OK);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_time_set_local_offset (void)
{
  TCase *tc = tcase_create ("pdf_time_set_local_offset");

  tcase_add_test(tc, pdf_time_set_local_offset_001);


  return tc;
}

/* End of pdf-time-set-local-offset.c */

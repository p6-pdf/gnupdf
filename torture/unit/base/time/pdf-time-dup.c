/* -*- mode: C -*- Time-stamp: "09/04/17 00:05:28 jemarch"
 *
 *       File:         pdf-time-dup.c
 *       Date:         Fri Feb 27 17:35:31 2008
 *
 *       GNU PDF Library - Unit tests for pdf_time_dup
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
#include <base/time/pdf-time-test-common.h>

/*
 * Test: pdf_time_dup_001
 * Description:
 *   Duplicate initialized pdf_time_t object
 *Success condition:
 * 1. Function pdf_time_new schould return PDF_OK
 * 2. Function pdf_time_dup schould return pdf_time_t object.
 * 3. Structures pdf_time_s schould have the same data.
 */
START_TEST (pdf_time_dup_001)
{
  pdf_status_t status;
  pdf_time_t time1;
  pdf_time_t time2;
  pdf_i64_t seconds;
  pdf_u32_t i,j,sec;

  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  fail_if(time1 == NULL);

  seconds = pdf_i64_new(0x12345678,0x9ABCDEF0);

  status = pdf_time_set_from_i64(time1, seconds);
  fail_if(status != PDF_OK);

  time2 = pdf_time_dup(time1);
  fail_if(time2 == NULL);
  fail_unless(pdf_time_cmp(time1, time2) == 0);

  
  status = pdf_time_destroy(time1);
  fail_if(status !=PDF_OK);

  status = pdf_time_destroy(time2);
  fail_if(status !=PDF_OK);

}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_time_dup (void)
{
  TCase *tc = tcase_create ("pdf_time_dup");

  tcase_add_test(tc, pdf_time_dup_001);

  return tc;
}

/* End of pdf-time-dup.c */

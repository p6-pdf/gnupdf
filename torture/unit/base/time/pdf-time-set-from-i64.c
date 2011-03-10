/* -*- mode: C -*-
 *
 *       File:         pdf-time-set-from-u32.c
 *       Date:         Sun Sep 21 16:37:27 2008
 *
 *       GNU PDF Library - Unit tests for pdf_time_set_from_i64
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
#include <stdint.h>
#include <check.h>
#include <pdf.h>
#include <stdlib.h>
#include <pdf-test-common.h>

/*
 * Test: pdf_time_set_from_i64_001
 * Description:
 *   Create two pdf_time_t objects.
 *   First is initialized with pdf_time_set_from_u32
 *   and second one with the same number with function
 *   pdf_time_set_from_i64.
 *   Both object must be equal.
 *
 *Success condition:
 * 1. Function pdf_time_new schould return PDF_OK
 * 2. Function pdf_time_set_from_u32 schould return
 * PDF_OK.
 * 3. Function pdf_time_set_from_i64 schould return
 * PDF_OK.
 * 4. pdf_time_t objects are equal.
 *
 */
START_TEST (pdf_time_set_from_i64_001)
{
  pdf_status_t status;
  pdf_time_t time1;
  pdf_time_t time2;
  pdf_i64_t seconds;


  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);

  status = pdf_time_new(&time2);
  fail_if(status != PDF_OK);

  seconds = 0xF1234567;

  status = pdf_time_set_from_u32(time1, 0xF1234567);
  fail_if(status != PDF_OK);

  status = pdf_time_set_from_i64(time2, seconds);
  fail_if(status != PDF_OK);


  fail_unless(pdf_time_cmp(time1, time2) == 0 );


  status = pdf_time_destroy(time1);
  fail_if(status != PDF_OK);

  status = pdf_time_destroy(time2);
  fail_if(status != PDF_OK);

}
END_TEST

/*
 * Test: pdf_time_set_from_i64_002
 * Description:
 *   Create pdf_time_t and pdf_time_span_t objects
 *   and initialize them to with the same 64 bit number of
 *   seconds. After pdf_time_sub_span pdf_time_t
 *   object schould be equal to 0.
 *
 *Success condition:
 * 1. Function pdf_time_new schould return PDF_OK
 * 2. Function pdf_time_set_from_i64 schould return
 * PDF_OK.
 * 3. Function pdf_time_sub_span schould return
 * PDF_OK.
 * 4. pdf_time_t object is equal to 0.
 *
 */
START_TEST (pdf_time_set_from_i64_002)
{
  pdf_status_t status;
  pdf_time_t time;
  pdf_time_t zero;
  pdf_time_span_t span;
  pdf_i64_t sec;


  status = pdf_time_new(&time);
  fail_if(status != PDF_OK);

  status = pdf_time_new(&zero);
  fail_if(status != PDF_OK);


  span = pdf_time_span_new();

  sec = INT64_C(0x123456789ABCDEEF);
  status = pdf_time_span_set(&span, sec);
  fail_if(status != PDF_OK);

  status = pdf_time_set_from_i64(time, sec);
  fail_if(status != PDF_OK);

  status =  pdf_time_sub_span(time, span);

  fail_unless( pdf_time_cmp(time, zero) == 0 );


  status = pdf_time_destroy(time);
  fail_if(status != PDF_OK);

  status = pdf_time_destroy(zero);
  fail_if(status != PDF_OK);

  status = pdf_time_span_destroy(&span);
  fail_if(status != PDF_OK);
}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_time_set_from_i64 (void)
{
  TCase *tc = tcase_create ("pdf_time_set_from_i64");

  tcase_add_test(tc, pdf_time_set_from_i64_001);
  tcase_add_test(tc, pdf_time_set_from_i64_002);

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-time-set-from-i64.c */

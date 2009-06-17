/* -*- mode: C -*- Time-stamp: "09/04/17 00:06:20 jemarch"
 *
 *       File:         pdf-time-span-diff.c
 *       Date:         Fri Feb 27 17:35:31 2009
 *
 *       GNU PDF Library - Unit tests for pdf_time_span_diff
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

/*
 * Test: pdf_time_span_diff_001
 * Description:
 *   Create two pdf_time_span_t objects, and check
 *   if the diff of spans is expected one.
 *Success condition:
 * 1. Function pdf_time_span_new schould return PDF_OK
 * 2. Function pdf_time_span_diff schould return PDF_OK.
 * 3. Result schould be equal span1-span2
 */
START_TEST (pdf_time_span_diff_001)
{
  pdf_status_t status;
  pdf_i64_t sec;
  pdf_time_span_t span1;
  pdf_time_span_t span2;
  pdf_time_span_t result;

  span1 = pdf_time_span_new();
  span2 = pdf_time_span_new();
  result = pdf_time_span_new();

  

  pdf_time_span_set_from_i32(&span1,0x00FFFF11);
  pdf_time_span_set_from_i32(&span2,-0x00FFFF12);

  status = pdf_time_span_diff(span1,span2,&result);
  fail_if(status != PDF_OK);

  sec = pdf_time_span_to_secs(result);

  fail_if(pdf_i64_cmp_i32(sec,0x1FFFE23) != 0);

  status = pdf_time_span_destroy(&span1);
  fail_if(status != PDF_OK);
  status = pdf_time_span_destroy(&span2);
  fail_if(status != PDF_OK);
  status = pdf_time_span_destroy(&result);
  fail_if(status != PDF_OK);


}
END_TEST

/*
 * Test: pdf_time_span_diff_002
 * Description:
 *  Try to get diff of two pdf_time_span_t objects,
 *  where pointer to result is NUL.
 *Success condition:
 * 1. Function pdf_time_span_new schould return PDF_OK
 * 2. Function pdf_time_span_diff schould return PDF_ERROR.
 * 3. Result schould be equal span1-span2
 */
START_TEST (pdf_time_span_diff_002)
{
  pdf_status_t status;
  pdf_time_span_t span1;
  pdf_time_span_t span2;
  pdf_time_span_t *result;

  span1 = pdf_time_span_new();
  span2 = pdf_time_span_new();
  result = NULL; 

  pdf_time_span_set_from_i32(&span1,0x00FFFF11);
  pdf_time_span_set_from_i32(&span2,-0x00FFFF12);

  status = pdf_time_span_diff(span1,span2,result);
  fail_if(status != PDF_ERROR);

  status = pdf_time_span_destroy(&span1);
  fail_if(status != PDF_OK);
  status = pdf_time_span_destroy(&span2);
  fail_if(status != PDF_OK);

}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_time_span_diff (void)
{
  TCase *tc = tcase_create ("pdf_time_span_diff");

  tcase_add_test(tc, pdf_time_span_diff_001);
  tcase_add_test(tc, pdf_time_span_diff_002);


  return tc;
}

/* End of pdf-time-span-diff.c */

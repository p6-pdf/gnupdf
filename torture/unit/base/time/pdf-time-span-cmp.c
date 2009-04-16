/* -*- mode: C -*- Time-stamp: "09/04/17 00:06:09 jemarch"
 *
 *       File:         pdf-time-span-cmp.c
 *       Date:         Fri Feb 27 17:35:31 2009
 *
 *       GNU PDF Library - Unit tests for pdf_time_span_cmp
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
 * Test: pdf_time_span_cmp_001
 * Description:
 *   Create two pdf_time_span_t equal objects. 
 *Success condition:
 * 1. Function pdf_time_span_cmp schould return 1
 */
START_TEST (pdf_time_span_cmp_001)
{
  pdf_status_t status;
  pdf_i64_t sec;
  pdf_time_span_t span1;
  pdf_time_span_t span2;

  span1 = pdf_time_span_new();
  span2 = pdf_time_span_new();

  pdf_time_span_set(&span1,0x01234567,0x89ABCDEF);
  pdf_time_span_set(&span2,0x01234567,0x89ABCDEF);

  fail_unless (pdf_time_span_cmp(span1, span2) == 0);

  status = pdf_time_span_destroy(&span1);
  fail_if(status != PDF_OK);
  status = pdf_time_span_destroy(&span2);
  fail_if(status != PDF_OK);


}
END_TEST

/*
 * Test: pdf_time_span_cmp_002
 * Description:
 *   Create two pdf_time_span_t, first is argument
 *   is bigger. 
 *Success condition:
 * 1. Function pdf_time_span_cmp schould return -1
 */
START_TEST (pdf_time_span_cmp_002)
{
  pdf_status_t status;
  pdf_i64_t sec;
  pdf_time_span_t span1;
  pdf_time_span_t span2;

  span1 = pdf_time_span_new();
  span2 = pdf_time_span_new();

  pdf_time_span_set(&span1,0x01234567,0x89ABCDEF);
  pdf_time_span_set(&span2,-0x01234567,0x89ABCDEF);

  fail_unless (pdf_time_span_cmp(span1, span2) == 1);

  status = pdf_time_span_destroy(&span1);
  fail_if(status != PDF_OK);
  status = pdf_time_span_destroy(&span2);
  fail_if(status != PDF_OK);


}
END_TEST

/*
 * Test: pdf_time_span_cmp_003
 * Description:
 *   Create two pdf_time_span_t, first is argument
 *   is smaller than second one. 
 *Success condition:
 * 1. Function pdf_time_span_cmp schould return 0
 */
START_TEST (pdf_time_span_cmp_003)
{
  pdf_status_t status;
  pdf_i64_t sec;
  pdf_time_span_t span1;
  pdf_time_span_t span2;

  span1 = pdf_time_span_new();
  span2 = pdf_time_span_new();

  pdf_time_span_set(&span1,-0x01234567,0x89ABCDEF);
  pdf_time_span_set(&span2,0x01234567,0x89ABCDEF);

  fail_unless (pdf_time_span_cmp(span1, span2) == -1);

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
test_pdf_time_span_cmp (void)
{
  TCase *tc = tcase_create ("pdf_time_span_cmp");

  tcase_add_test(tc, pdf_time_span_cmp_001);
  tcase_add_test(tc, pdf_time_span_cmp_002);
  tcase_add_test(tc, pdf_time_span_cmp_003);



  return tc;
}

/* End of pdf-time-span-cmp.c */

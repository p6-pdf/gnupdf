/* -*- mode: C -*-
 *
 *       File:         pdf-time-sub-span.c
 *       Date:         Fri Feb 27 17:35:31 2008
 *
 *       GNU PDF Library - Unit tests for pdf_time_sub_span
 *
 */

/* Copyright (C) 2008-2011 Free Software Foundation, Inc. */

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
 * Test: pdf_time_sub_span
 * Description:
 *   Initialize pdf_time_t and pdf_time_span_t
 *   object with pdf_i64_t value.
 *   Then call pdf_time_sub_span and check if
 *   pdf_time_t object points to 1970-01-01.
 *
 *Success condition:
 * 1. Function pdf_time_sub_span schould return
 * PDF_OK.
 * 2. Function pdf_time_cmp schould return 0.
 */
START_TEST (pdf_time_sub_span_001)
{
  pdf_status_t status;

  pdf_time_t time1;
  pdf_time_t time2;

  pdf_time_span_t span;
  pdf_i64_t num64;


  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);

  status = pdf_time_new(&time2);
  fail_if(status != PDF_OK);


  span = pdf_time_span_new();

  num64= INT64_C(0x01234567AABBCCDD);
  pdf_time_span_set(&span, num64);


  status = pdf_time_set_from_i64(time1, num64);
  fail_if(status != PDF_OK);

  status = pdf_time_sub_span(time1, span);
  fail_if(status != PDF_OK);


  fail_unless(pdf_time_cmp(time1, time2) == 0);


  status = pdf_time_destroy(time1);
  fail_if(status != PDF_OK);

  status = pdf_time_destroy(time2);
  fail_if(status != PDF_OK);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_time_sub_span (void)
{
  TCase *tc = tcase_create ("pdf_time_sub_span");

  tcase_add_test(tc, pdf_time_sub_span_001);

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-time-sub-span.c*/

/* -*- mode: C -*- Time-stamp: "09/04/17 00:06:15 jemarch"
 *
 *       File:         pdf-time-span_new.c
 *       Date:         Fri Feb 27 17:35:31 2009
 *
 *       GNU PDF Library - Unit tests for pdf_time_span_destroy
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
 * Test: pdf_time_span_destroy_001
 * Description:
 *   Create new pdf_time_span_t and destroy it.
 *Success condition:
 * 1. Function pdf_time_span_new schould return PDF_OK
 * 2. Fuction pdf_time_span_destroy schould return PDF_OK
 */
START_TEST (pdf_time_span_destroy_001)
{
  pdf_status_t status;
  pdf_i64_t sec;
  pdf_i64_t sec2;
  pdf_time_span_t span;

  span = pdf_time_span_new();
  
  sec = pdf_time_span_to_secs(span);
  sec2 = pdf_i64_new(0, 0);
  
  fail_unless(memcmp(&sec, &sec2, sizeof(pdf_i64_t)) == 0);

  status = pdf_time_span_destroy(&span);
  fail_if(status != PDF_OK);

}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_time_span_destroy (void)
{
  TCase *tc = tcase_create ("pdf_time_span_destroy");

  tcase_add_test(tc, pdf_time_span_destroy_001);

  return tc;
}

/* End of pdf-time-span-destroy.c */

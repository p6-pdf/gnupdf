/* -*- mode: C -*-
 *
 *       File:         pdf-time-span-to-secs.c
 *       Date:         Fri Feb 27 17:35:31 2009
 *
 *       GNU PDF Library - Unit tests for pdf_time_span_to_secs
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
#include <stdint.h>
#include <check.h>
#include <pdf.h>
#include <stdlib.h>

/*
 * Test: pdf_time_span_to_secs_001
 * Description:
 *   Create new pdf_time_span_t object and init it
 *   with pdf_i64_t number. Seconds returned by
 *   pdf_time_span_to_secs schould be equal to init
 *   number.
 *Success condition:
 * 1. Function pdf_time_span_new schould return PDF_OK
 * 2. Function pdf_time_span_set  return PDF_OK.
 * 3. Seconds returned by pdf_time_span_to_secs
 * schould be equal to initial number.
 * schould return PDF_OK
 */
START_TEST (pdf_time_span_to_secs_001)
{
  pdf_status_t status;
  pdf_i64_t sec;
  pdf_i64_t sec2;
  pdf_time_span_t span;

  pdf_init();

  span = pdf_time_span_new();
  status = pdf_time_span_set(&span, INT64_C(0x0123456789ABCDEF));
  fail_if(status != PDF_OK);
  
  sec = pdf_time_span_to_secs(span);
  sec2 = INT64_C(0x0123456789ABCDEF);
  fail_unless(memcmp(&sec,&sec2, sizeof(pdf_i64_t)) == 0);

  status = pdf_time_span_destroy(&span);
  fail_if(status != PDF_OK);

}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_time_span_to_secs (void)
{
  TCase *tc = tcase_create ("pdf_time_span_to_secs");

  tcase_add_test(tc, pdf_time_span_to_secs_001);

  return tc;
}

/* End of pdf-time-span-to-secs.c */

/* -*- mode: C -*- Time-stamp: "2009-08-05 22:18:11 davazp"
 *
 *       File:         pdf-time-span-set-from-i32.c
 *       Date:         Sun Sep 21 16:37:27 2008
 *
 *       GNU PDF Library - Unit tests for pdf_time_span_set_from_i32
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
 * Test: pdf_time_span_set_from_i32_001
 * Description:
 *   Create new pdf_time_span_t object and init it
 *   with positive and negative number.
 *Success condition:
 * 1. Function pdf_time_span_new schould return PDF_OK
 * 2. Function pdf_time_span_set_from_i32 schould return PDF_OK
 * 3. Seconds returned from pdf_time_span_to_secs
 * schould be expected one.
 */
START_TEST (pdf_time_span_set_from_i32_001)
{
    pdf_status_t status;
    pdf_time_span_t span;
    pdf_i32_t seconds;
    pdf_i64_t seconds2;

    pdf_init();

    span=pdf_time_span_new();

    seconds = 0x01234567;
    status = pdf_time_span_set_from_i32(&span,seconds);
    fail_if(status != PDF_OK);

    seconds2 = pdf_time_span_to_secs(span);
    fail_unless(pdf_i64_cmp_i32(seconds2,seconds) == 0);

    /* Try with negative span */
    seconds = 0x12345678;
    status = pdf_time_span_set_from_i32(&span,seconds);
    fail_if(status != PDF_OK);

    seconds2 = pdf_time_span_to_secs(span);
    fail_if(status != PDF_OK);

    fail_unless(pdf_i64_cmp_i32(seconds2,seconds) == 0);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_time_span_set_from_i32 (void)
{
  TCase *tc = tcase_create ("pdf_time_span_set_from_i32");

  tcase_add_test(tc, pdf_time_span_set_from_i32_001);

  return tc;
}

/* End of pdf-time-span-set-from-i32.c */

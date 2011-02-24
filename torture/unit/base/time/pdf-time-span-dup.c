/* -*- mode: C -*-
 *
 *       File:         pdf-stm-write.c
 *       Date:         Sun Sep 21 16:37:27 2008
 *
 *       GNU PDF Library - Unit tests for pdf_time_span_dup
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
#include <check.h>
#include <pdf.h>
#include <stdlib.h>
#include <pdf-test-common.h>
/*
 * Test: pdf_time_span_dup_001
 * Description:
 *   Create pdf_time_span_t and initialize it.
 *   Create duplicate  and compare with first
 *   object.
 *
 *Success condition:
 * 1. Function memcmp schould return 0 if object are
 * equal.
 */
START_TEST (pdf_time_span_dup_001)
{
  pdf_status_t status;

    pdf_time_span_t span1;
    pdf_time_span_t span2;

    pdf_init();

    span1 = pdf_time_span_new();

    span1 = 0x55667788;

    span2 = pdf_time_span_dup(span1);

    fail_unless(memcmp(&span1, &span2, sizeof(pdf_time_span_t)) == 0);

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
test_pdf_time_span_dup (void)
{
  TCase *tc = tcase_create ("pdf_time_span_dup");

  tcase_add_test(tc, pdf_time_span_dup_001);

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-time-span-dup.c */

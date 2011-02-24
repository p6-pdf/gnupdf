/* -*- mode: C -*-
 *
 *       File:         pdf-time-span-copy.c
 *       Date:         Fri Feb 27 17:35:31 2009
 *
 *       GNU PDF Library - Unit tests for pdf_time_span_copy
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
 * Test: pdf_time_span_copy_001
 * Description:
 *   Create two pdf_time_span_t objects. Init first
 *   and copy it to second one.
 *Success condition:
 * 1. Function pdf_time_span_copy schould return
 * PDF_OK.
 * 2. Function pdf_time_span_cmp schould return 0
 * (objects must be equal).
 */
START_TEST (pdf_time_span_copy_001)
{
  pdf_status_t status;
  pdf_time_span_t span1;
  pdf_time_span_t span2;

  pdf_init();

  span1 = pdf_time_span_new();
  span2 = pdf_time_span_new();

  pdf_time_span_set(&span1,INT64_C(0x0123456789ABCDEF));

  status = pdf_time_span_copy(span1, &span2);
  fail_if(status != PDF_OK);

  fail_unless (pdf_time_span_cmp(span1, span2) == 0);

  status = pdf_time_span_destroy(&span1);
  fail_if(status != PDF_OK);
  status = pdf_time_span_destroy(&span2);
  fail_if(status != PDF_OK);
}
END_TEST

/*
 * Test: pdf_time_span_copy_002
 * Description:
 *   Create two pdf_time_span_t objects.
 *   Second argument (destination) points to NULL.
 *   Function schould return PDF_ERROR.
 *Success condition:
 * 1. Function pdf_time_span_copy schould return
 * PDF_ERROR.
 * 2. Function pdf_time_span_cmp schould return 0
 * (objects must be equal).
 */
START_TEST (pdf_time_span_copy_002)
{
  pdf_status_t status;
  pdf_time_span_t span1;
  pdf_time_span_t* span2;

  pdf_init();

  span1 = pdf_time_span_new();
  span2 = NULL;

  pdf_time_span_set(&span1,INT64_C(0x0123456789ABCDEF));

  status = pdf_time_span_copy(span1, span2);
  fail_if(status != PDF_EBADDATA);

  status = pdf_time_span_destroy(&span1);
  fail_if(status != PDF_OK);
}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_time_span_copy (void)
{
  TCase *tc = tcase_create ("pdf_time_span_copy");

  tcase_add_test(tc, pdf_time_span_copy_001);
  tcase_add_test(tc, pdf_time_span_copy_002);

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-time-span-copy.c */

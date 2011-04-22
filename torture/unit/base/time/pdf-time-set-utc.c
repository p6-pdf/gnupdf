/* -*- mode: C -*-
 *
 *       File:         pdf-time-set-utc.c
 *       Date:         Sun Sep 21 16:37:27 2008
 *
 *       GNU PDF Library - Unit tests for pdf_time_set_utc
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
 * Test: pdf_time_set_utc_001
 * Description:
 *   Create pdf_time_t and pdf_time_span_t objects
 *   and initialize them to with the same number of
 *   seconds. After calling pdf_time_sub_span pdf_time_t
 *   object schould be equal to 0.
 * Success condition:
 *   1. Function pdf_time_new schould return PDF_OK
 *   2. Function pdf_time_set_utc schould return
 *      PDF_OK.
 *   3. Function pdf_time_sub_span schould return
 *      PDF_OK.
 *   4. pdf_time_t object is equal to 0.
 */
START_TEST (pdf_time_set_utc_001)
{
  pdf_time_t time_var;
  pdf_time_t zero;
  pdf_i32_t sec;


  pdf_time_init (&time_var);
  pdf_time_init (&zero);

  sec = 1234567890;

  pdf_time_set_utc (&time_var, sec);
  pdf_time_sub_span (&time_var, sec);

  fail_unless (pdf_time_cmp (&time_var, &zero) == 0);

  pdf_time_deinit (&time_var);
  pdf_time_deinit (&zero);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_time_set_utc (void)
{
  TCase *tc = tcase_create ("pdf_time_set_utc");

  tcase_add_test (tc, pdf_time_set_utc_001);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-time-set-utc.c */

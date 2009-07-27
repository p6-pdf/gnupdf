/* -*- mode: C -*- Time-stamp: ""
 *
 *       File:         pdf-i64-abs.c
 *       Date:         Mon May 05 16:30:00 2008
 *
 *       GNU PDF Library - Types Module - pdf_i64_abs test case
 *
 */

/* Copyright (C) 2008 Free Software Foundation, Inc. */

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
#include <stdio.h>
#include <string.h>
#include <pdf.h>
#include <check.h>

#define INTERACTIVE_DEBUG 0

/*
 * Test: pdf_i64_abs_001
 * Description:
 *   Checks if the result is the absolute value of the input variable
 * Success conditions:
 * The call should produce a positive number from a negative argument
 */
START_TEST(pdf_i64_abs_001)
{
  pdf_status_t p_status = PDF_OK;
  int cmp_res;
  pdf_i64_t k,j;

#ifndef PDF_USE_BUILTIN_64BIT_SUPPORT
  pdf_i64_assign(&k,0xFFFFFFFF,0xFFFFFFFE, &p_status); /*-2*/
  pdf_i64_abs( &j, k , &p_status);
  fail_if(p_status != PDF_OK);
  fail_if(j.high != 0);
  fail_if(j.low != 2);
#else
  k = -2;
  pdf_i64_abs( &j, k , &p_status);
  fail_if(j != 2);
#endif

}
END_TEST

/*
 * Test: pdf_i64_abs_002
 * Description:
 *   Checks if result is absolute value of the input variable
 * Success conditions:
 * The call should produce a positive number from a positive argument
 */

START_TEST(pdf_i64_abs_002)
{
  pdf_status_t p_status = PDF_OK;
  int cmp_res;
  pdf_i64_t k,j;

#ifndef PDF_USE_BUILTIN_64BIT_SUPPORT
  pdf_i64_assign(&k,0,2, &p_status); /*2*/
  pdf_i64_abs( &j, k , &p_status);
  fail_if(p_status != PDF_OK);
  fail_if(j.high != 0);
  fail_if(j.low != 2);
#else
  k = 2;
  pdf_i64_abs( &j, k , &p_status);
  fail_if(j != 2);
#endif
}
END_TEST

/*
 * Test: pdf_i64_abs_003
 * Description:
 *   Checks if an invalid destination correctly throws an error
 * Success conditions:
 * The call should return PDF_ERROR
 */
START_TEST(pdf_i64_abs_003)
{
  pdf_status_t p_status = PDF_OK;
  pdf_i64_t src;
  pdf_i64_t *dest = NULL;

  pdf_i64_assign(&src,0, 3, &p_status);
  fail_if(p_status != PDF_OK);

  pdf_i64_abs(dest, src , &p_status);
  fail_unless(p_status == PDF_ERROR);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_i64_abs (void)
{
  TCase *tc = tcase_create("pdf_i64_abs");
  tcase_add_test(tc, pdf_i64_abs_001);
  tcase_add_test(tc, pdf_i64_abs_002);
  tcase_add_test(tc, pdf_i64_abs_003);
  return tc;
}

/* End of pdf-i64-abs.c */

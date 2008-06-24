/* -*- mode: C -*- Time-stamp: ""
 *
 *       File:         pdf-i64-new.c
 *       Date:         Mon May 05 10:30:01 2008
 *
 *       GNU PDF Library - Types Module - pdf_i64_new test case
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
 * Test: pdf_i64_new_001
 * Description:
 *   Checks if the the pdf_i64_new function properly assigns
 *  the high and low values to a pdf_i64_t type
 */
START_TEST(pdf_i64_new_001)
{

  pdf_i64_t k;

  k = pdf_i64_new(4,5);

#ifndef PDF_USE_BUILTIN_64BIT_SUPPORT 
  fail_if(k.low != 5);
  fail_if(k.high != 4);
#else
  fail_if(k != 0x400000005);
#endif
  

}
END_TEST




/*
 * Test case creation function
 */
TCase *
test_pdf_i64_new (void)
{
  TCase *tc = tcase_create("pdf_i64_new");
  tcase_add_test(tc, pdf_i64_new_001);
  return tc;
}

/* End of pdf-i64-new.c */

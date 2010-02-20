/* -*- mode: C -*-
 *
 *       File:         pdf-i64-cmp.c
 *       Date:         Mon May 05 16:30:00 2008
 *
 *       GNU PDF Library - Types Module - pdf_i64_cmp test case
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
 * Test: pdf_i64_cmp_001
 * Description:
 *   Checks if the comparison between a negative and positive
 * number is carried out properly
 * Success conditions:
 * The call should produce a -1
 */
START_TEST(pdf_i64_cmp_001)
{
  pdf_status_t p_status = PDF_OK;
  int cmp_res;
  pdf_i64_t k,j;
  pdf_init();

  pdf_i64_assign(&k,0xFFFFFFFF,0xFFFFFFFE,&p_status); /*-2*/
  pdf_i64_assign(&j,0,2,&p_status); /*2*/

#ifndef PDF_USE_BUILTIN_64BIT_SUPPORT
  cmp_res = pdf_i64_cmp( k, j );
  fail_if(cmp_res != -1);
#endif
  
  

}
END_TEST


/*
 * Test: pdf_i64_cmp_002
 * Description:
 * Checks if the comparison between a positive and negative
 * number is carried out properly
 * Success conditions:
 * The call should produce a 1
 */

START_TEST(pdf_i64_cmp_002)
{
  pdf_status_t p_status = PDF_OK;
  int cmp_res;
  pdf_i64_t k,j;
  pdf_init();

  pdf_i64_assign(&k,0xFFFFFFFF,0xFFFFFFFE,&p_status); /*-2*/
  pdf_i64_assign(&j,0,2,&p_status); /*2*/

#ifndef PDF_USE_BUILTIN_64BIT_SUPPORT
  cmp_res = pdf_i64_cmp( j, k );
  fail_if(cmp_res != 1);
#endif  
  
}
END_TEST

/*
 * Test: pdf_i64_add_003
 * Description:
 *    Checks if the comparison between two equal numbers
 *  is carried out properly
 * Success conditions:
 * The call should produce a 0
 */
START_TEST(pdf_i64_cmp_003)
{
  pdf_status_t p_status = PDF_OK;
  int cmp_res;
  pdf_i64_t k,j;
  pdf_init();

  pdf_i64_assign(&k,0,2,&p_status); /*2*/
  pdf_i64_assign(&j,0,2,&p_status); /*2*/

#ifndef PDF_USE_BUILTIN_64BIT_SUPPORT
  cmp_res = pdf_i64_cmp( j, k );
  fail_if(cmp_res != 0);
#endif  
  

}
END_TEST






/*
 * Test case creation function
 */
TCase *
test_pdf_i64_cmp (void)
{
  TCase *tc = tcase_create("pdf_i64_cmp");
  tcase_add_test(tc, pdf_i64_cmp_001);
  tcase_add_test(tc, pdf_i64_cmp_002);
  tcase_add_test(tc, pdf_i64_cmp_003);
  return tc;
}

/* End of pdf-i64-cmp.c */

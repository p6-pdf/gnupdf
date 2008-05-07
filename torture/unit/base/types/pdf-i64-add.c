/* -*- mode: C -*- Time-stamp: ""
 *
 *       File:         pdf-i64-add.c
 *       Date:         Mon May 05 10:30:01 2008
 *
 *       GNU PDF Library - Types Module - pdf_i64_add test case
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
 * Test: pdf_i64_add_001
 * Description:
 *   Checks if the carry number is properly carried
 * Success conditions:
 * The call should not produce an error
 */
START_TEST(pdf_i64_add_001)
{

  pdf_i64_t add1,add2,dest;
  add1 = pdf_i64_new(0,0xFFFFFFFF);
  add2 = pdf_i64_new(2147483646,1);

  


  fail_if(pdf_i64_add(&dest,add1,add2) != PDF_OK);
  
  fail_if(dest.low != 0);
  fail_if(dest.high != 2147483647);
  

}
END_TEST


/*
 * Test: pdf_i64_add_002
 * Description:
 *   Checks if function checks the proper initialisation of target
 * Success conditions:
 * The call should produce an error
 */

START_TEST(pdf_i64_add_002)
{
  
  pdf_i64_t add1,add2;
  pdf_i64_t *dest = NULL;
  add1 = pdf_i64_new(0,4);
  add2 = pdf_i64_new(5,0);

  


  fail_if(pdf_i64_add(dest,add1,add2) != PDF_ERROR);

  
  
}
END_TEST

/*
 * Test: pdf_i64_add_003
 * Description:
 *   Adds a positive and negative number
 * Success conditions:
 * The call should not produce an error
 */
START_TEST(pdf_i64_add_003)
{

  pdf_i64_t add1, add2, dest;
  add1 = pdf_i64_new(0xFFFFFFFF,0xFFFFFFFE); /*-2*/
  add2 = pdf_i64_new(0,3);

  
  fail_if(pdf_i64_add(&dest, add1, add2) != PDF_OK);
  
  fail_if(dest.low != 1);
  fail_if(dest.high != 0);
  

}
END_TEST



/*
 * Test: pdf_i64_add_004
 * Description:
 *   Adds two negative numbers
 * Success conditions:
 * The call should not produce an error
 */
START_TEST(pdf_i64_add_004)
{

  pdf_i64_t add1, add2, dest;
  add1 = pdf_i64_new(0xFFFFFFFF,0xFFFFFFFE); /*-2*/
  add2 = pdf_i64_new(0xFFFFFFFF,0xFFFFFFFE); /*-2*/

  
  fail_if(pdf_i64_add(&dest, add1, add2) != PDF_OK);
  
  fail_if(dest.low !=  0xFFFFFFFC); /*-4*/
  fail_if(dest.high != 0xFFFFFFFF);
  

}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_i64_add (void)
{
  TCase *tc = tcase_create("pdf_i64_add");
  tcase_add_test(tc, pdf_i64_add_001);
  tcase_add_test(tc, pdf_i64_add_002);
  tcase_add_test(tc, pdf_i64_add_003);
  tcase_add_test(tc, pdf_i64_add_004);
  return tc;
}

/* End of pdf-i64-add.c */

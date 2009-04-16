/* -*- mode: C -*- Time-stamp: "danividal"
 *
 *       File:         pdf-i64-div.c
 *       Date:         Mon May 05 10:30:01 2008
 *
 *       GNU PDF Library - Types Module - pdf_i64_div test case
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
 * Test: pdf_i64_div_001
 * Description:
 *   Checks if the division of two positive numbers 
 *   is carried out properly
 * Success conditions:
 * The call should not produce an error
 */
START_TEST(pdf_i64_div_001)
{
  pdf_status_t p_status = PDF_OK;
  pdf_i64_t div1,div2,dest;
  div1 = pdf_i64_new(0,8);
  div2 = pdf_i64_new(0,2);

  

#ifndef PDF_USE_BUILTIN_64BIT_SUPPORT  
  pdf_i64_div(&dest,div1,div2, &p_status);
  fail_if(p_status != PDF_OK);
  fail_if(dest.low != 4);
  fail_if(dest.high != 0);
#else
  pdf_i64_div(&dest,div1,div2, &p_status);
  fail_if(dest != 4);
#endif

}
END_TEST


/*
 * Test: pdf_i64_div_002
 * Description:
 * Checks if function checks the proper initialisation of target
 * Success conditions:
 * The call should produce an error
 */

START_TEST(pdf_i64_div_002)
{
  pdf_status_t p_status = PDF_OK;
  pdf_i64_t div1,div2;
  
  div1 = pdf_i64_new(0,4);
  div2 = pdf_i64_new(5,0);

  

#ifndef PDF_USE_BUILTIN_64BIT_SUPPORT 
  pdf_i64_t *dest = NULL;
  pdf_i64_div(dest,div1,div2, &p_status); 
  fail_if(p_status != PDF_ERROR);
#endif
  
  
}
END_TEST

/*
 * Test: pdf_i64_div_003
 * Description:
 *   Divs a positive and negative number
 * Success conditions:
 * The call should not produce an error
 */
START_TEST(pdf_i64_div_003)
{
  pdf_status_t p_status = PDF_OK;
  pdf_i64_t div1, div2, dest;

#ifndef PDF_USE_BUILTIN_64BIT_SUPPORT 
  div1 = pdf_i64_new(0xFFFFFFFF,0xFFFFFFFA); /*-6*/
  div2 = pdf_i64_new(0,3);
  pdf_i64_div(&dest, div1, div2, &p_status);
  
  fail_if(p_status != PDF_OK);
  
  fail_if(dest.low != 0xFFFFFFFE); /*-2*/
  fail_if(dest.high != 0xFFFFFFFF);
#else
  div1 = -6;
  div2 = 3;
  pdf_i64_div(&dest, div1, div2, &p_status);
  fail_if(dest != -2);
#endif
 

}
END_TEST



/*
 * Test: pdf_i64_div_004
 * Description:
 *   Divs two negative numbers
 * Success conditions:
 * The call should not produce an error
 */
START_TEST(pdf_i64_div_004)
{

  pdf_i64_t div1, div2, dest;
  pdf_status_t p_status = PDF_OK;
#ifndef PDF_USE_BUILTIN_64BIT_SUPPORT 
  div1 = pdf_i64_new(0xFFFFFFFF,0xFFFFFFFA); /*-6*/
  div2 = pdf_i64_new(0xFFFFFFFF,0xFFFFFFFE); /*-2*/

  pdf_i64_div(&dest, div1, div2, &p_status);
  
  fail_if(p_status != PDF_OK);
  
  fail_if(dest.low !=  3);
  fail_if(dest.high != 0);
#else
  div1 = -6;
  div2 = -2;
  pdf_i64_div(&dest, div1, div2, &p_status);
  fail_if(dest != 3);
#endif

}
END_TEST

/*
 * Test: pdf_i64_div_005
 * Description:
 *   Checks if the division 1/86400 gives 0.
 * Success conditions:
 * The call should not produce an error
 */
START_TEST(pdf_i64_div_005)
{
  pdf_status_t p_status = PDF_OK;
  pdf_i64_t div1,div2,dest;
  div1 = pdf_i64_new(0,1);
  div2 = pdf_i64_new(0,24*3600);

  

#ifndef PDF_USE_BUILTIN_64BIT_SUPPORT  
  pdf_i64_div(&dest,div1,div2, &p_status);
  fail_if(p_status != PDF_OK);
  fail_if(dest.low != 0);
  fail_if(dest.high != 0);
#else
  pdf_i64_div(&dest,div1,div2, &p_status);
  fail_if(dest != 0);
#endif

}
END_TEST




/*
 * Test case creation function
 */
TCase *
test_pdf_i64_div (void)
{
  TCase *tc = tcase_create("pdf_i64_div");
  tcase_add_test(tc, pdf_i64_div_001);
  tcase_add_test(tc, pdf_i64_div_002);
  tcase_add_test(tc, pdf_i64_div_003);
  tcase_add_test(tc, pdf_i64_div_004);
  tcase_add_test(tc, pdf_i64_div_005);
  return tc;
}

/* End of pdf-i64-div.c */

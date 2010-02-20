/* -*- mode: C -*-
 *
 *       File:         pdf-i64-mult.c
 *       Date:         Mon May 05 10:30:01 2008
 *
 *       GNU PDF Library - Types Module - pdf_i64_mult test case
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
 * Test: pdf_i64_mult_001
 * Description:
 *   Checks if the multiplication of two positive numbers 
 *   is carried out properly
 * Success conditions:
 * The call should not produce an error
 */
START_TEST(pdf_i64_mult_001)
{
  pdf_status_t p_status = PDF_OK;
  pdf_i64_t mult1,mult2,dest;
  pdf_init();

  mult1 = pdf_i64_new(0,2);
  mult2 = pdf_i64_new(0,2);

  

#ifndef PDF_USE_BUILTIN_64BIT_SUPPORT  
  pdf_i64_mult(&dest,mult1,mult2, &p_status);
  fail_if(p_status != PDF_OK);
  fail_if(dest.low != 4);
  fail_if(dest.high != 0);
#else
  pdf_i64_mult(&dest,mult1,mult2, &p_status);
  fail_if(dest != 4);
#endif

}
END_TEST


/*
 * Test: pdf_i64_mult_002
 * Description:
 * Checks if function checks the proper initialisation of target
 * Success conditions:
 * The call should produce an error
 */

START_TEST(pdf_i64_mult_002)
{
  pdf_status_t p_status = PDF_OK;
  pdf_i64_t mult1,mult2;
  
  pdf_init();

  mult1 = pdf_i64_new(0,4);
  mult2 = pdf_i64_new(5,0);

  

#ifndef PDF_USE_BUILTIN_64BIT_SUPPORT 
  pdf_i64_t *dest = NULL;
  pdf_i64_mult(dest,mult1,mult2, &p_status); 
  fail_if(p_status != PDF_ERROR);
#endif
  
  
}
END_TEST

/*
 * Test: pdf_i64_mult_003
 * Description:
 *   Mults a positive and negative number
 * Success conditions:
 * The call should not produce an error
 */
START_TEST(pdf_i64_mult_003)
{
  pdf_status_t p_status = PDF_OK;
  pdf_i64_t mult1, mult2, dest;

#ifndef PDF_USE_BUILTIN_64BIT_SUPPORT 
  pdf_init();

  mult1 = pdf_i64_new(0xFFFFFFFF,0xFFFFFFFE); /*-2*/
  mult2 = pdf_i64_new(0,3);
  pdf_i64_mult(&dest, mult1, mult2, &p_status);
  
  fail_if(p_status != PDF_OK);
 
  fail_if(dest.low != 0xFFFFFFFA); /*-6*/
  fail_if(dest.high != 0xFFFFFFFF);
#else
  mult1 = -2;
  mult2 = 3;
  pdf_i64_mult(&dest, mult1, mult2, &p_status);
  fail_if(dest != -6);
#endif
 

}
END_TEST



/*
 * Test: pdf_i64_mult_004
 * Description:
 *   Mults two negative numbers
 * Success conditions:
 * The call should not produce an error
 */
START_TEST(pdf_i64_mult_004)
{

  pdf_i64_t mult1, mult2, dest;
  pdf_status_t p_status = PDF_OK;
#ifndef PDF_USE_BUILTIN_64BIT_SUPPORT 
  pdf_init();

  mult1 = pdf_i64_new(0xFFFFFFFF,0xFFFFFFFE); /*-2*/
  mult2 = pdf_i64_new(0xFFFFFFFF,0xFFFFFFFE); /*-2*/

  pdf_i64_mult(&dest, mult1, mult2, &p_status);
  
  fail_if(p_status != PDF_OK);
  
  fail_if(dest.low !=  4);
  fail_if(dest.high != 0);
#else
  mult1 = -2;
  mult2 = -2;
  pdf_i64_mult(&dest, mult1, mult2, &p_status);
  fail_if(dest != 4);
#endif

}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_i64_mult (void)
{
  TCase *tc = tcase_create("pdf_i64_mult");
  tcase_add_test(tc, pdf_i64_mult_001);
  tcase_add_test(tc, pdf_i64_mult_002);
  tcase_add_test(tc, pdf_i64_mult_003);
  tcase_add_test(tc, pdf_i64_mult_004);
  return tc;
}

/* End of pdf-i64-mult.c */

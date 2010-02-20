/* -*- mode: C -*-
 *
 *       File:         pdf-i64-copy.c
 *       Date:         Mon May 05 10:30:01 2008
 *
 *       GNU PDF Library - Types Module - pdf_i64_copy test case
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
 * Test: pdf_i64_copy_001
 * Description:
 *   Checks if the pdf_i64_copy copies the values of one 
 * pdf_i64_t variable into another
 */
START_TEST(pdf_i64_copy_001)
{
  pdf_status_t p_status = PDF_OK;
  pdf_i64_t orig;
  pdf_i64_t copy;
  
  pdf_init();

  orig = pdf_i64_new(4,5);


  

#ifndef PDF_USE_BUILTIN_64BIT_SUPPORT 
  pdf_i64_copy(orig,&copy,&p_status);
  fail_if( p_status != PDF_OK);
  fail_if(copy.low != 5);
  fail_if(copy.high != 4);
#else
  pdf_i64_copy(orig,&copy,&p_status);
  fail_if(copy != 0x0000000400000005LL);
#endif
  

}
END_TEST


/*
 * Test: pdf_i64_copy_001
 * Description:
 *   Checks if the pdf_i64_copy function gets an error if
 *  the target pointer is not properly initialised
 */

START_TEST(pdf_i64_copy_002)
{
  pdf_status_t p_status = PDF_OK;
  pdf_i64_t orig;
  
  
  pdf_init();

  orig = pdf_i64_new(4,5);
  
#ifndef PDF_USE_BUILTIN_64BIT_SUPPORT 
  pdf_i64_t *copy = NULL;
  pdf_i64_copy(orig,copy,&p_status);
  fail_if( p_status != PDF_ERROR);
#endif
  
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_i64_copy (void)
{
  TCase *tc = tcase_create("pdf_i64_copy");
  tcase_add_test(tc, pdf_i64_copy_001);
  tcase_add_test(tc, pdf_i64_copy_002);
  return tc;
}

/* End of pdf-i64-copy.c */

/* -*- mode: C -*- Time-stamp: "danividal"
 *
 *       File:         pdf-i64-mod.c
 *       Date:         Mon May 05 10:30:01 2008
 *
 *       GNU PDF Library - Types Module - pdf_i64_mod test case
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
 * Test: pdf_i64_mod_001
 * Description:
 *   Checks if the modulus of two positive numbers 
 *   is carried out properly
 * Success conditions:
 * The call should not produce an error
 */
START_TEST(pdf_i64_mod_001)
{
  pdf_status_t p_status = PDF_OK;
  pdf_i64_t mod1,mod2,dest;
  mod1 = pdf_i64_new(0,7);
  mod2 = pdf_i64_new(0,2);

  

#ifndef PDF_USE_BUILTIN_64BIT_SUPPORT  
  pdf_i64_mod(&dest,mod1,mod2, &p_status);
  fail_if(p_status != PDF_OK);
  fail_if(dest.low != 1);
  fail_if(dest.high != 0);
#else
  pdf_i64_mod(&dest,mod1,mod2, &p_status);
  fail_if(dest != 1);
#endif

}
END_TEST


/*
 * Test: pdf_i64_mod_002
 * Description:
 * Checks if function checks the proper initialisation of target
 * Success conditions:
 * The call should produce an error
 */

START_TEST(pdf_i64_mod_002)
{
  pdf_status_t p_status = PDF_OK;
  pdf_i64_t mod1,mod2;
  
  mod1 = pdf_i64_new(0,4);
  mod2 = pdf_i64_new(5,0);

  

#ifndef PDF_USE_BUILTIN_64BIT_SUPPORT 
  pdf_i64_t *dest = NULL;
  pdf_i64_mod(dest,mod1,mod2, &p_status); 
  fail_if(p_status != PDF_ERROR);
#endif
  
  
}
END_TEST

/*
 * Test: pdf_i64_mod_003
 * Description:
 *   Mods a positive and negative number
 * Success conditions:
 * The call should not produce an error
 */
START_TEST(pdf_i64_mod_003)
{
  pdf_status_t p_status = PDF_OK;
  pdf_i64_t mod1, mod2, dest;

#ifndef PDF_USE_BUILTIN_64BIT_SUPPORT 
  mod1 = pdf_i64_new(0xFFFFFFFF,0xFFFFFFFB); /*-5*/
  mod2 = pdf_i64_new(0,3);
  pdf_i64_mod(&dest, mod1, mod2, &p_status);
  
  fail_if(p_status != PDF_OK);

  fail_if(dest.low != 0xFFFFFFFE); 
  fail_if(dest.high != 0xFFFFFFFF);
#else
  mod1 = -5;
  mod2 = 3;
  pdf_i64_mod(&dest, mod1, mod2, &p_status);
  fail_if(dest != -2);
#endif
 

}
END_TEST



/*
 * Test: pdf_i64_mod_004
 * Description:
 *   Mods two negative numbers
 * Success conditions:
 * The call should not produce an error
 */
START_TEST(pdf_i64_mod_004)
{

  pdf_i64_t mod1, mod2, dest;
  pdf_status_t p_status = PDF_OK;
#ifndef PDF_USE_BUILTIN_64BIT_SUPPORT 
  mod1 = pdf_i64_new(0xFFFFFFFF,0xFFFFFFFB); /*-5*/
  mod2 = pdf_i64_new(0xFFFFFFFF,0xFFFFFFFD); /*-3*/

  pdf_i64_mod(&dest, mod1, mod2, &p_status);
  
  fail_if(p_status != PDF_OK);
 
  fail_if(dest.low !=  0xFFFFFFFE);
  fail_if(dest.high != 0xFFFFFFFF);
#else
  mod1 = -5;
  mod2 = -3;
  pdf_i64_mod(&dest, mod1, mod2, &p_status);
  fail_if(dest != -2);
#endif

}
END_TEST

/*
 * Test: pdf_i64_neg_005
 * Description:
 *   Checks if an invalid destination correctly throws an error
 * Success conditions:
 * The call should return PDF_ERROR
 */
START_TEST(pdf_i64_mod_005)
{
  pdf_status_t p_status = PDF_OK;
  pdf_i64_t mod1, mod2;
  pdf_i64_t *dest = NULL;

  pdf_i64_assign(&mod1,0, 5, &p_status);
  fail_if(p_status != PDF_OK);
  pdf_i64_assign(&mod2,0, 2, &p_status);
  fail_if(p_status != PDF_OK);

  pdf_i64_mod(dest, mod1, mod2, &p_status);
  fail_unless(p_status == PDF_ERROR);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_i64_mod (void)
{
  TCase *tc = tcase_create("pdf_i64_mod");
  tcase_add_test(tc, pdf_i64_mod_001);
  tcase_add_test(tc, pdf_i64_mod_002);
  tcase_add_test(tc, pdf_i64_mod_003);
  tcase_add_test(tc, pdf_i64_mod_004);
  tcase_add_test(tc, pdf_i64_mod_005);
  return tc;
}

/* End of pdf-i64-mod.c */

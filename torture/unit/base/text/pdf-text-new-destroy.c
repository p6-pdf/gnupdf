/* -*- mode: C -*-
 *
 *       File:         pdf-text-new-destroy.c
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_new_destroy test case
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


/*
 * Test: pdf_text_new_destroy_001
 * Description:
 *   Create a text object without initializing the context
 * Success conditions:
 *   1. The call to @code{pdf_text_new} should return PDF_EBADCONTEXT
 */
START_TEST(pdf_text_new_destroy_001)
{
  pdf_text_t newtext = NULL;

  fail_unless(pdf_text_new (&newtext) == PDF_EBADCONTEXT);
}
END_TEST

/*
 * Test: pdf_text_new_destroy_002
 * Description:
 *   Create an empty text object and destroy it
 * Success conditions:
 *   1. The call to @code{pdf_text_new} should not return a  @code{NULL} pointer.
 *   2. The text data of the created object should be empty.
 *   3. The language code of the output object should be empty
 *   4. The country code of the output object should be empty 
 *   5. The call to @code{pdf_text_destroy} should return PDF_OK;
 */
START_TEST(pdf_text_new_destroy_002)
{
  pdf_text_t newtext = NULL;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);

  /* 1. The call to @code{pdf_text_new} should not return an error
     code. */
  fail_if(pdf_text_new (&newtext) != PDF_OK);

  /* 2. The text data of the created object should be empty. */
  fail_unless(pdf_text_empty_p(newtext) == PDF_TRUE);
  
  /* 3. The language code of the output object should be empty */
  fail_unless(strlen((char *)pdf_text_get_language(newtext)) == 0);
  
  /* 4. The country code of the output object should be empty */
  fail_unless(strlen((char *)pdf_text_get_country(newtext)) == 0);
  
  /* 5. The call to @code{pdf_text_destroy} should return PDF_OK. Again,
   *  probably this test is not that useful... */
  fail_if(pdf_text_destroy(newtext) != PDF_OK);
}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_text_new_destroy (void)
{
  TCase *tc = tcase_create("pdf_text_new_destroy");
  tcase_add_test(tc, pdf_text_new_destroy_001);
  tcase_add_test(tc, pdf_text_new_destroy_002);
  return tc;
}

/* End of pdf-text-new-destroy.c */

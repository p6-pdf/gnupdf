/* -*- mode: C -*-
 *
 *       File:         pdf-text-get-best-encoding.c
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_get_best_encoding test case
 *
 */

/* Copyright (C) 2008-2011 Free Software Foundation, Inc. */

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
 * Test: pdf_text_get_best_encoding_001
 * Description:
 *   Check if a best encoding is returned. If available, it must return a valid
 *   Unicode-based host encoding. If no Unicode encoding is available, it should
 *   return the preferred encoding. As this test really depends on what the user
 *   has configured in the system, the unit test will only check that a
 *   non-empty host encoding is returned.
 * Success conditions:
 *   1. The call to pdf_text_get_best_encoding should return a non-empty 
 *      pdf_text_host_encoding_t variable. 
 */
START_TEST(pdf_text_get_best_encoding_001)
{
  pdf_text_host_encoding_t host_enc;
  pdf_text_t text;

  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  fail_if(pdf_text_new (&text) != PDF_OK);

#ifdef PDF_HOST_WIN32
  fail_unless(pdf_text_check_host_encoding((pdf_char_t *)"CP20127",
                                           &host_enc) == PDF_OK);
#else
  fail_unless(pdf_text_check_host_encoding((pdf_char_t *)"ascii",
                                           &host_enc) == PDF_OK);
#endif

  /* 1. The call to pdf_text_get_best_encoding should return a non-empty 
   *      pdf_text_host_encoding_t variable.*/
  host_enc = pdf_text_get_best_encoding(text, host_enc);
  fail_unless(strlen(host_enc.name) > 0);
  
  pdf_text_destroy(text);
}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_text_get_best_encoding (void)
{
  TCase *tc = tcase_create("pdf_text_get_best_encoding");
  tcase_add_test(tc, pdf_text_get_best_encoding_001);
  
  return tc;
}

/* End of pdf-text-get-best-encoding.c */

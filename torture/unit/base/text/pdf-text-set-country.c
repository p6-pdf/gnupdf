/* -*- mode: C -*- Time-stamp: ""
 *
 *       File:         pdf-text-set-country.c
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_set_country test case
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
 * Test: pdf_text_set_country_001
 * Description:
 *   Set a valid two-character country ID
 * Success conditions:
 *   1. The call to pdf_text_set_country should return PDF_OK.
 *   2. The contents of the internal country ID representation should be the
 *      expected ones. 
 */
START_TEST(pdf_text_set_country_001)
{
  pdf_text_t text;
  const pdf_char_t *country = (pdf_char_t *)"GB";
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  text = pdf_text_new();
  fail_if(text == NULL);
  
  /* 1. The call to pdf_text_set_country should return PDF_OK. */
  fail_if(pdf_text_set_country(text, country) != PDF_OK);
  
  /* 2. The contents of the internal country ID representation should be the
   *      expected ones. */
  fail_unless(strcmp((char *)text->country, (char *)country)==0);
}
END_TEST


/*
 * Test: pdf_text_set_country_002
 * Description:
 *   Set an invalid one-character country ID
 * Success conditions:
 *   1. The call to pdf_text_set_country should NOT return PDF_OK. 
 */
START_TEST(pdf_text_set_country_002)
{
  pdf_text_t text;
  const pdf_char_t *country = (pdf_char_t *)"a";
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  text = pdf_text_new();
  fail_if(text == NULL);
  
  /* 1. The call to pdf_text_set_country should NOT return PDF_OK. */
  fail_if(pdf_text_set_country(text, country) == PDF_OK);
}
END_TEST


/*
 * Test: pdf_text_set_country_003
 * Description:
 *   Set an invalid empty country ID
 * Success conditions:
 *   1. The call to pdf_text_set_country should NOT return PDF_OK. 
 */
START_TEST(pdf_text_set_country_003)
{
  pdf_text_t text;
  const pdf_char_t *country = (pdf_char_t *)"";
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  text = pdf_text_new();
  fail_if(text == NULL);
  
  /* 1. The call to pdf_text_set_country should NOT return PDF_OK. */
  fail_if(pdf_text_set_country(text, country) == PDF_OK);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_text_set_country (void)
{
  TCase *tc = tcase_create("pdf_text_set_country");
  tcase_add_test(tc, pdf_text_set_country_001);
  tcase_add_test(tc, pdf_text_set_country_002);
  tcase_add_test(tc, pdf_text_set_country_003);

  return tc;
}

/* End of pdf-text-get-country.c */

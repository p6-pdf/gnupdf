/* -*- mode: C -*-
 *
 *       File:         pdf-text-get-language.c
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_get_language test case
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
#include <pdf-test-common.h>

/*
 * Test: pdf_text_get_language_001
 * Description:
 *   Get language ID when the stored country ID is valid
 * Success conditions:
 *   1. The call to pdf_text_get_language should return a valid pointer.
 *   2. The length of the returned string should be 2.
 */
START_TEST (pdf_text_get_language_001)
{



  pdf_text_t text;
  const pdf_char_t *language = NULL;

  fail_if(pdf_text_new (&text) != PDF_OK);

  /* Store language ID without using the API */
  fail_if(pdf_text_set_language(text, "en") != PDF_OK);

  /* Get country ID */
  language = pdf_text_get_language(text);

  /* 1. The call to pdf_text_get_language should return a valid pointer. */
  fail_if(language == NULL);

  /* 2. The length of the returned string should be 2. */
  fail_unless(strlen(language) == 2);

  pdf_text_destroy(text);

}
END_TEST


/*
 * Test: pdf_text_get_language_002
 * Description:
 *   Get language ID when the stored country ID is invalid
 * Success conditions:
 *   1. The call to pdf_text_get_language should return a valid pointer.
 *   2. The length of the returned string should be 0.
 */
START_TEST (pdf_text_get_language_002)
{
  pdf_text_t text;
  const pdf_char_t *language = NULL;

  fail_if(pdf_text_new (&text) != PDF_OK);

  /* Get language ID */
  language = pdf_text_get_language(text);

  /* 1. The call to pdf_text_get_language should return a valid pointer. */
  fail_if(language == NULL);

  /* 2. The length of the returned string should be 2. */
  fail_unless(strlen(language) == 0);

  pdf_text_destroy(text);
}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_text_get_language (void)
{
  TCase *tc = tcase_create("pdf_text_get_language");
  tcase_add_test(tc, pdf_text_get_language_001);
  tcase_add_test(tc, pdf_text_get_language_002);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-text-get-language.c */

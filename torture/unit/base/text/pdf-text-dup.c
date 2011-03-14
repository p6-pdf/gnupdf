/* -*- mode: C -*-
 *
 *       File:         pdf-text-dup.c
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_dup test case
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
 * Test: pdf_text_dup_001
 * Description:
 *   Duplicate an empty text object
 * Success conditions:
 *   1. The call to pdf_text_dup should not return a NULL pointer.
 *   2. The text data of the created object should be empty.
 *   3. The language code of the output object should be empty
 *   4. The country code of the output object should be empty
 */
START_TEST (pdf_text_dup_001)
{
  pdf_error_t *error = NULL;
  pdf_text_t *text;
  pdf_text_t *duplicate;

  text = pdf_text_new (&error);
  fail_unless (text != NULL);
  fail_if (error != NULL);

  /* 1. The call to pdf_text_dup should not return a NULL pointer.*/
  duplicate = pdf_text_dup (text, &error);
  fail_unless (duplicate != NULL);
  fail_if (error != NULL);

  /* 2. The text data of the created object should be empty. */
  fail_unless (pdf_text_empty_p (duplicate) == PDF_TRUE);

  /* 3. The language code of the output object should be empty */
  fail_unless (strlen (pdf_text_get_language (duplicate)) == 0);

  /* 4. The country code of the output object should be empty */
  fail_unless (strlen (pdf_text_get_country (duplicate)) == 0);

  pdf_text_destroy (text);
  pdf_text_destroy (duplicate);
}
END_TEST

/*
 * Test: pdf_text_dup_002
 * Description:
 *   Duplicate a text object with data contents but no language/country code
 * Success conditions:
 *   1. The call to pdf_text_dup should not return a NULL pointer.
 *   2. The text data of the output object should be equal to the text data
 *      of the input object.
 *   3. The language code of the output object should be empty
 *   4. The country code of the output object should be empty
 */
START_TEST (pdf_text_dup_002)
{
  pdf_error_t *error = NULL;
  pdf_text_t *text;
  pdf_text_t *duplicate;
  const pdf_char_t *sample_utf8 = (pdf_char_t *)"GNU's not Unix";

  text = pdf_text_new (&error);
  fail_unless (text != NULL);
  fail_if (error != NULL);

  /* Set some data contents */
  fail_unless (pdf_text_set_unicode (text,
                                     sample_utf8,
                                     strlen (sample_utf8),
                                     PDF_TEXT_UTF8,
                                     &error));
  fail_if (error != NULL);

  /* 1. The call to pdf_text_dup should not return a NULL pointer.*/
  duplicate = pdf_text_dup (text, &error);
  fail_unless (duplicate != NULL);
  fail_if (error != NULL);

  /* 2. The text data of the output object should be equal to the text data
   *     of the input object. */
  fail_unless (pdf_text_cmp (text, duplicate, PDF_TRUE, &error) == 0);
  fail_if (error != NULL);

  /* 3. The language code of the output object should be empty */
  fail_unless (strlen (pdf_text_get_language (duplicate)) == 0);

  /* 4. The country code of the output object should be empty */
  fail_unless (strlen (pdf_text_get_country (duplicate)) == 0);

  pdf_text_destroy (text);
  pdf_text_destroy (duplicate);
}
END_TEST

/*
 * Test: pdf_text_dup_003
 * Description:
 *   Duplicate a text object with data contents and language code
 * Success conditions:
 *   1. The call to pdf_text_dup should not return a NULL pointer.
 *   2. The text data of the output object should be equal to the text data
 *      of the input object.
 *   3. The language code of the output object should be equal to the language
 *      code of the input object.
 *   4. The country code of the output object should be empty
 */
START_TEST (pdf_text_dup_003)
{
  pdf_error_t *error = NULL;
  pdf_text_t *text;
  pdf_text_t *duplicate;
  const pdf_char_t *sample_utf8 = (pdf_char_t *)"GNU's not Unix";
  const pdf_char_t *sample_lang = (pdf_char_t *)"en";

  text = pdf_text_new (&error);
  fail_unless (text != NULL);
  fail_if (error != NULL);

  /* Set some data contents */
  fail_unless (pdf_text_set_unicode (text,
                                     sample_utf8,
                                     strlen (sample_utf8),
                                     PDF_TEXT_UTF8,
                                     &error));
  fail_if (error != NULL);

  /* Set language code */
  pdf_text_set_language (text, sample_lang);

  /* 1. The call to pdf_text_dup should not return a NULL pointer.*/
  duplicate = pdf_text_dup (text, &error);
  fail_unless (duplicate != NULL);
  fail_if (error != NULL);

  /* 2. The text data of the output object should be equal to the text data
   *     of the input object. */
  fail_unless (pdf_text_cmp (text, duplicate, PDF_TRUE, &error) == 0);
  fail_if (error != NULL);

  /* 3. The language code of the output object should be equal to the language
   *      code of the input object. */
  fail_unless (strcmp (pdf_text_get_language (duplicate), sample_lang) == 0);

  /* 4. The country code of the output object should be empty */
  fail_unless (strlen (pdf_text_get_country (duplicate)) == 0);

  pdf_text_destroy (text);
  pdf_text_destroy (duplicate);
}
END_TEST

/*
 * Test: pdf_text_dup_004
 * Description:
 *   Duplicate a text object with data contents, language code and country code
 * Success conditions:
 *   1. The call to pdf_text_dup should not return a NULL pointer.
 *   2. The text data of the output object should be equal to the text data
 *      of the input object.
 *   3. The language code of the output object should be equal to the language
 *      code of the input object.
 *   4. The country code of the output object should be equal to the country
 *      code of the input object.
 */
START_TEST (pdf_text_dup_004)
{
  pdf_error_t *error = NULL;
  pdf_text_t *text;
  pdf_text_t *duplicate;
  const pdf_char_t *sample_utf8 = (pdf_char_t *)"GNU's not Unix";
  const pdf_char_t *sample_lang = (pdf_char_t *)"en";
  const pdf_char_t *sample_country = (pdf_char_t *)"gb";

  text = pdf_text_new (&error);
  fail_unless (text != NULL);
  fail_if (error != NULL);

  /* Set some data contents */
  fail_unless (pdf_text_set_unicode (text,
                                     sample_utf8,
                                     strlen (sample_utf8),
                                     PDF_TEXT_UTF8,
                                     &error));
  fail_if (error != NULL);

  /* Set language code */
  pdf_text_set_language (text, sample_lang);

  /* Set country code */
  pdf_text_set_country (text, sample_country);

  /* 1. The call to pdf_text_dup should not return a NULL pointer.*/
  duplicate = pdf_text_dup (text, &error);
  fail_unless (duplicate != NULL);
  fail_if (error != NULL);

  /* 2. The text data of the output object should be equal to the text data
   *     of the input object. */
  fail_unless (pdf_text_cmp (text, duplicate, PDF_TRUE, &error) == 0);
  fail_if (error != NULL);

  /* 3. The language code of the output object should be equal to the language
   *      code of the input object. */
  fail_unless (strcmp (pdf_text_get_language (duplicate), sample_lang) == 0);

  /* 4. The country code of the output object should be empty */
  fail_unless (strcmp (pdf_text_get_country (duplicate), sample_country) == 0);

  pdf_text_destroy (text);
  pdf_text_destroy (duplicate);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_text_dup (void)
{
  TCase *tc = tcase_create("pdf_text_dup");

  tcase_add_test(tc, pdf_text_dup_001);
  tcase_add_test(tc, pdf_text_dup_002);
  tcase_add_test(tc, pdf_text_dup_003);
  tcase_add_test(tc, pdf_text_dup_004);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-text-dup.c */

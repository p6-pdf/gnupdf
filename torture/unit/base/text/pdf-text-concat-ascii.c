/* -*- mode: C -*-
 *
 *       File:         pdf-text-concat-ascii.c
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_concat_ascii test case
 *
 */

/* Copyright (C) 2010,2011 Free Software Foundation, Inc. */

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
 * Test: pdf_text_concat_ascii_001
 * Description:
 *   Concatenate text object and non-empty ascii string.
 * Success conditions:
 *   1. The call to  pdf_text_concat_ascii should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 *   3. The lang/country information in the output object must remain unchanged.
 */
START_TEST (pdf_text_concat_ascii_001)
{
  const pdf_char_t *country = (pdf_char_t *)"GB";
  const pdf_char_t *language = (pdf_char_t *)"en";
  pdf_error_t *error = NULL;
  pdf_char_t *output_data;
  pdf_text_t *text1;
  const pdf_char_t *text2 = " Unix";

  text1 = pdf_text_new (&error);
  fail_unless (text1 != NULL);
  fail_if (error != NULL);
  fail_unless (pdf_text_set_pdfdocenc (text1, (pdf_char_t *)"GNU's not", &error) == PDF_TRUE);
  fail_if (error != NULL);
  pdf_text_set_language (text1, language);
  pdf_text_set_country (text1, country);

  /* 1. The call to  pdf_text_concat_ascii should return PDF_OK. */
  fail_unless (pdf_text_concat_ascii (text1, text2, &error) == PDF_TRUE);
  fail_if (error != NULL);

  /* 2. The contents of the output text object must be the expected ones. */
  output_data = pdf_text_get_pdfdocenc (text1, &error);
  fail_unless (output_data != NULL);
  fail_unless (strlen (output_data) == strlen ("GNU's not Unix"));
  fail_unless (strcmp (output_data, "GNU's not Unix") == 0);

  /* 3. The lang/country information in the output object must remain
   *    unchanged. */
  fail_unless (strcmp (pdf_text_get_language (text1), language) == 0);
  fail_unless (strcmp (pdf_text_get_country (text1), country) == 0);

  pdf_text_destroy (text1);
  pdf_dealloc (output_data);
}
END_TEST

/*
 * Test: pdf_text_concat_ascii_002
 * Description:
 *   Concatenate text object without lang/country with non-empty ascii string.
 * Success conditions:
 *   1. The call to  pdf_text_concat_ascii should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 *   3. The lang/country information in the output object must be empty.
 */
START_TEST (pdf_text_concat_ascii_002)
{
  pdf_error_t *error = NULL;
  pdf_char_t *output_data;
  pdf_text_t *text1;
  const pdf_char_t *text2 = " Unix";

  text1 = pdf_text_new (&error);
  fail_unless (text1 != NULL);
  fail_if (error != NULL);
  fail_unless (pdf_text_set_pdfdocenc (text1, (pdf_char_t *)"GNU's not", &error) == PDF_TRUE);
  fail_if (error != NULL);

  /* 1. The call to  pdf_text_concat_ascii should return PDF_OK. */
  fail_unless (pdf_text_concat_ascii (text1, text2, &error) == PDF_TRUE);
  fail_if (error != NULL);

  /* 2. The contents of the output text object must be the expected ones. */
  output_data = pdf_text_get_pdfdocenc (text1, &error);
  fail_unless (output_data != NULL);
  fail_unless (strlen (output_data) == strlen ("GNU's not Unix"));
  fail_unless (strcmp (output_data, "GNU's not Unix") == 0);

  /* 3. The lang/country information in the output object must be empty. */
  fail_unless (strlen (pdf_text_get_language (text1)) == 0);
  fail_unless (strlen (pdf_text_get_country (text1)) == 0);

  pdf_text_destroy (text1);
  pdf_dealloc (output_data);
}
END_TEST

/*
 * Test: pdf_text_concat_ascii_003
 * Description:
 *   Concatenate text object with empty string.
 * Success conditions:
 *   1. The call to  pdf_text_concat_ascii should return PDF_OK.
 *   2. The contents of the output text object should be empty.
 *   3. The lang/country information in the output object must remain unchanged.
 */
START_TEST (pdf_text_concat_ascii_003)
{
  const pdf_char_t *country = (pdf_char_t *)"GB";
  const pdf_char_t *language = (pdf_char_t *)"en";
  pdf_error_t *error = NULL;
  pdf_char_t *output_data;
  pdf_text_t *text1;
  const pdf_char_t *text2 = "";

  text1 = pdf_text_new (&error);
  fail_unless (text1 != NULL);
  fail_if (error != NULL);
  fail_unless (pdf_text_set_pdfdocenc (text1, (pdf_char_t *)"GNU's not Unix", &error) == PDF_TRUE);
  fail_if (error != NULL);
  pdf_text_set_language (text1, language);
  pdf_text_set_country (text1, country);

  /* 1. The call to  pdf_text_concat_ascii should return PDF_OK. */
  fail_unless (pdf_text_concat_ascii (text1, text2, &error) == PDF_TRUE);
  fail_if (error != NULL);

  /* 2. The contents of the output text object must be the expected ones. */
  output_data = pdf_text_get_pdfdocenc (text1, &error);
  fail_unless (output_data != NULL);
  fail_unless (strlen (output_data) == strlen ("GNU's not Unix"));
  fail_unless (strcmp (output_data, "GNU's not Unix") == 0);

  /* 3. The lang/country information in the output object must remain
   *    unchanged. */
  fail_unless (strcmp (pdf_text_get_language (text1), language) == 0);
  fail_unless (strcmp (pdf_text_get_country (text1), country) == 0);

  pdf_text_destroy (text1);
  pdf_dealloc (output_data);
}
END_TEST

/*
 * Test: pdf_text_concat_ascii_004
 * Description:
 *   Concatenate empty text object with empty string.
 * Success conditions:
 *   1. The call to  pdf_text_concat_ascii should return PDF_OK.
 *   2. The contents of the output text object should be empty.
 *   3. The lang/country information in the output object must remain unchanged.
 */
START_TEST (pdf_text_concat_ascii_004)
{
  const pdf_char_t *country = (pdf_char_t *)"GB";
  const pdf_char_t *language = (pdf_char_t *)"en";
  pdf_error_t *error = NULL;
  pdf_char_t *output_data;
  pdf_text_t *text1;
  const pdf_char_t *text2 = "";

  text1 = pdf_text_new (&error);
  fail_unless (text1 != NULL);
  fail_if (error != NULL);
  pdf_text_set_language (text1, language);
  pdf_text_set_country (text1, country);

  /* 1. The call to  pdf_text_concat_ascii should return PDF_OK. */
  fail_unless (pdf_text_concat_ascii (text1, text2, &error) == PDF_TRUE);
  fail_if (error != NULL);

  /* 2. The contents of the output text object must be the expected ones. */
  output_data = pdf_text_get_pdfdocenc (text1, &error);
  fail_unless (output_data != NULL);
  fail_unless (strlen (output_data) == 0);

  /* 3. The lang/country information in the output object must remain
   *    unchanged. */
  fail_unless (strcmp (pdf_text_get_language (text1), language) == 0);
  fail_unless (strcmp (pdf_text_get_country (text1), country) == 0);

  pdf_text_destroy (text1);
  pdf_dealloc (output_data);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_text_concat_ascii (void)
{
  TCase *tc = tcase_create ("pdf_text_concat_ascii");

  tcase_add_test (tc, pdf_text_concat_ascii_001);
  tcase_add_test (tc, pdf_text_concat_ascii_002);
  tcase_add_test (tc, pdf_text_concat_ascii_003);
  tcase_add_test (tc, pdf_text_concat_ascii_004);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-text-concat-ascii.c */

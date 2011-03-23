/* -*- mode: C -*-
 *
 *       File:         pdf-text-cmp.c
 *       Date:         Mon Apr 13 21:08:34 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_cmp test case
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
#include <base/text/pdf-text-test-common.h>
#include <pdf-test-common.h>

/*
 * Test: pdf_text_cmp_001
 * Description:
 *   Compare two equal text objects, case-sensitive.
 * Success conditions:
 *   1. The call to  pdf_text_cmp should return 0.
 *   2. No error detected
 */
START_TEST (pdf_text_cmp_001)
{
  pdf_error_t *error = NULL;
  const pdf_char_t *input_data = "GNU's Not Unix. ";
  pdf_size_t input_size = strlen (input_data);
  pdf_text_t *text1;
  pdf_text_t *text2;

  /* Create text objects with same contents */
  text1 = pdf_text_new_from_unicode (input_data,
                                     input_size,
                                     PDF_TEXT_UTF8,
                                     &error);
  fail_unless (text1 != NULL);
  fail_if (error != NULL);
  text2 = pdf_text_new_from_unicode (input_data,
                                     input_size,
                                     PDF_TEXT_UTF8,
                                     &error);
  fail_unless (text2 != NULL);
  fail_if (error != NULL);

  /* 1. The call to  pdf_text_cmp should return 0. */
  fail_unless (pdf_text_cmp (text1, text2, PDF_TRUE, &error) == 0);
  fail_if (error != NULL);

  pdf_text_destroy (text1);
  pdf_text_destroy (text2);
}
END_TEST

/*
 * Test: pdf_text_cmp_002
 * Description:
 *   Compare two equal text objects, non-case-sensitive.
 * Success conditions:
 *   1. The call to  pdf_text_cmp should return 0.
 *   2. The returned status in  p_ret_code should be PDF_OK.
 */
START_TEST (pdf_text_cmp_002)
{
  pdf_error_t *error = NULL;
  const pdf_char_t *input_data = "GNU's Not Unix. ";
  pdf_size_t input_size = strlen (input_data);
  pdf_text_t *text1;
  pdf_text_t *text2;

  /* Create text objects with same contents */
  text1 = pdf_text_new_from_unicode (input_data,
                                     input_size,
                                     PDF_TEXT_UTF8,
                                     &error);
  fail_unless (text1 != NULL);
  fail_if (error != NULL);
  text2 = pdf_text_new_from_unicode (input_data,
                                     input_size,
                                     PDF_TEXT_UTF8,
                                     &error);
  fail_unless (text2 != NULL);
  fail_if (error != NULL);

  /* 1. The call to  pdf_text_cmp should return 0. */
  fail_unless (pdf_text_cmp (text1, text2, PDF_FALSE, &error) == 0);
  fail_if (error != NULL);

  pdf_text_destroy (text1);
  pdf_text_destroy (text2);
}
END_TEST

/*
 * Test: pdf_text_cmp_003
 * Description:
 *   Compare two equal text objects (one in uppercase, the other one in
 *   lowercase), case-sensitive.
 * Success conditions:
 *   1. The call to  pdf_text_cmp should NOT return 0.
 *   2. The returned status in  p_ret_code should be PDF_OK.
 */
START_TEST (pdf_text_cmp_003)
{
  pdf_error_t *error = NULL;
  const pdf_char_t *upper_data = "GNU'S NOT UNIX. ";
  const pdf_char_t *lower_data = "gnu's not unix. ";
  pdf_size_t input_size = strlen (upper_data);
  pdf_text_t *text1;
  pdf_text_t *text2;

  /* Create text objects with same contents */
  text1 = pdf_text_new_from_unicode (upper_data,
                                     input_size,
                                     PDF_TEXT_UTF8,
                                     &error);
  fail_unless (text1 != NULL);
  fail_if (error != NULL);
  text2 = pdf_text_new_from_unicode (lower_data,
                                     input_size,
                                     PDF_TEXT_UTF8,
                                     &error);
  fail_unless (text2 != NULL);
  fail_if (error != NULL);

  /* 1. The call to  pdf_text_cmp should return 0. */
  fail_unless (pdf_text_cmp (text1, text2, PDF_TRUE, &error) != 0);
  fail_if (error != NULL);

  pdf_text_destroy (text1);
  pdf_text_destroy (text2);
}
END_TEST

/*
 * Test: pdf_text_cmp_004
 * Description:
 *   Compare two equal text objects (one in uppercase, the other one in
 *   lowercase), non-case-sensitive.
 * Success conditions:
 *   1. The call to  pdf_text_cmp should return 0.
 *   2. The returned status in  p_ret_code should be PDF_OK.
 */
START_TEST (pdf_text_cmp_004)
{
  pdf_error_t *error = NULL;
  const pdf_char_t *upper_data = "GNU'S NOT UNIX. ";
  const pdf_char_t *lower_data = "gnu's not unix. ";
  pdf_size_t input_size = strlen (upper_data);
  pdf_text_t *text1;
  pdf_text_t *text2;

  /* Create text objects with same contents */
  text1 = pdf_text_new_from_unicode (upper_data,
                                     input_size,
                                     PDF_TEXT_UTF8,
                                     &error);
  fail_unless (text1 != NULL);
  fail_if (error != NULL);
  text2 = pdf_text_new_from_unicode (lower_data,
                                     input_size,
                                     PDF_TEXT_UTF8,
                                     &error);
  fail_unless (text2 != NULL);
  fail_if (error != NULL);

  /* 1. The call to  pdf_text_cmp should return 0. */
  fail_unless (pdf_text_cmp (text1, text2, PDF_FALSE, &error) == 0);
  fail_if (error != NULL);

  pdf_text_destroy (text1);
  pdf_text_destroy (text2);
}
END_TEST

/*
 * Test: pdf_text_cmp_005
 * Description:
 *   Compare two different text objects, case-sensitive.
 * Success conditions:
 *   1. The call to  pdf_text_cmp should NOT return 0.
 *   2. The returned status in  p_ret_code should be PDF_OK.
 */
START_TEST (pdf_text_cmp_005)
{
  pdf_error_t *error = NULL;
  const pdf_char_t *input_data1 = (pdf_char_t *) "GNU's Not Unix. ";
  const pdf_char_t *input_data2 = (pdf_char_t *) "Bad Vista. ";
  pdf_size_t input_size1 = strlen (input_data1);
  pdf_size_t input_size2 = strlen (input_data2);
  pdf_text_t *text1;
  pdf_text_t *text2;

  /* Create text objects with same contents */
  text1 = pdf_text_new_from_unicode (input_data1,
                                     input_size1,
                                     PDF_TEXT_UTF8,
                                     &error);
  fail_unless (text1 != NULL);
  fail_if (error != NULL);
  text2 = pdf_text_new_from_unicode (input_data2,
                                     input_size2,
                                     PDF_TEXT_UTF8,
                                     &error);
  fail_unless (text2 != NULL);
  fail_if (error != NULL);

  /* 1. The call to  pdf_text_cmp should NOT return 0. */
  fail_unless (pdf_text_cmp (text1, text2, PDF_TRUE, &error) != 0);
  fail_if (error != NULL);

  pdf_text_destroy (text1);
  pdf_text_destroy (text2);
}
END_TEST

/*
 * Test: pdf_text_cmp_006
 * Description:
 *   Compare two different text objects, non-case-sensitive.
 * Success conditions:
 *   1. The call to  pdf_text_cmp should NOT return 0.
 *   2. The returned status in  p_ret_code should be PDF_OK.
 */
START_TEST (pdf_text_cmp_006)
{
  pdf_error_t *error = NULL;
  const pdf_char_t *input_data1 = (pdf_char_t *) "GNU's Not Unix. ";
  const pdf_char_t *input_data2 = (pdf_char_t *) "Bad Vista. ";
  pdf_size_t input_size1 = strlen (input_data1);
  pdf_size_t input_size2 = strlen (input_data2);
  pdf_text_t *text1;
  pdf_text_t *text2;

  /* Create text objects with same contents */
  text1 = pdf_text_new_from_unicode (input_data1,
                                     input_size1,
                                     PDF_TEXT_UTF8,
                                     &error);
  fail_unless (text1 != NULL);
  fail_if (error != NULL);
  text2 = pdf_text_new_from_unicode (input_data2,
                                     input_size2,
                                     PDF_TEXT_UTF8,
                                     &error);
  fail_unless (text2 != NULL);
  fail_if (error != NULL);

  /* 1. The call to  pdf_text_cmp should NOT return 0. */
  fail_unless (pdf_text_cmp (text1, text2, PDF_FALSE, &error) != 0);
  fail_if (error != NULL);

  pdf_text_destroy (text1);
  pdf_text_destroy (text2);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_text_cmp (void)
{
  TCase *tc = tcase_create ("pdf_text_cmp");

  tcase_add_test (tc, pdf_text_cmp_001);
  tcase_add_test (tc, pdf_text_cmp_002);
  tcase_add_test (tc, pdf_text_cmp_003);
  tcase_add_test (tc, pdf_text_cmp_004);
  tcase_add_test (tc, pdf_text_cmp_005);
  tcase_add_test (tc, pdf_text_cmp_006);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-text-cmp.c */

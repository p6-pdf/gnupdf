/* -*- mode: C -*-
 *
 *       File:         pdf-text-new-from-u32.c
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_new_from_u32 test case
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
#include <pdf.h>
#include <check.h>
#include <base/text/pdf-text-test-common.h>
#include <pdf-test-common.h>

/*
 * Test: pdf_text_new_from_u32_001
 * Description:
 *   Create a text object given '0' as input number
 * Success conditions:
 *   1. The call to pdf_text_new_from_u32 should return PDF_OK.
 *   2. The contents of the text object must be the expected ones.
 */
START_TEST (pdf_text_new_from_u32_001)
{
  const pdf_char_t *expected_data_be = (pdf_char_t *)"\x00\x00\x00" "0";
  const pdf_char_t *expected_data_le = (pdf_char_t *)"0" "\x00\x00\x00";
  const pdf_char_t *expected_data;
  pdf_size_t expected_size = 4;
  pdf_char_t *actual_data;
  pdf_size_t actual_size;
  pdf_error_t *error = NULL;
  pdf_u32_t number = 0;
  pdf_text_t *text;

  expected_data = (!pdf_text_test_big_endian_system () ?
                   expected_data_le :
                   expected_data_be);

  /* 1. The call to pdf_text_new_from_u32 should return PDF_OK. */
  text = pdf_text_new_from_u32 (number, &error);
  fail_unless (text != NULL);
  fail_if (error != NULL);

  /* 2. The contents of the text object must be the expected ones. */
  actual_data = pdf_text_get_unicode (text,
                                      PDF_TEXT_UTF32_HE,
                                      PDF_TEXT_UNICODE_NO_OPTION,
                                      &actual_size,
                                      &error);
  fail_unless (actual_data != NULL);
  fail_if (error != NULL);

  PRINT_CONTENTS (__FUNCTION__, 0, text, expected_data, expected_size, 0);

  fail_unless (actual_size == expected_size);
  fail_unless (memcmp (actual_data, expected_data, expected_size) == 0);

  pdf_text_destroy (text);
  pdf_dealloc (actual_data);
}
END_TEST

/*
 * Test: pdf_text_new_from_u32_002
 * Description:
 *   Create a text object given a non-zero positive number
 * Success conditions:
 *   1. The call to pdf_text_new_from_u32 should return PDF_OK.
 *   2. The contents of the text object must be the expected ones.
 */
START_TEST (pdf_text_new_from_u32_002)
{
  const pdf_char_t *expected_data_be = (pdf_char_t *)"\x00\x00\x00" "1" \
                                                     "\x00\x00\x00" "2" \
                                                     "\x00\x00\x00" "3" \
                                                     "\x00\x00\x00" "4" \
                                                     "\x00\x00\x00" "5" \
                                                     "\x00\x00\x00" "0";
  const pdf_char_t *expected_data_le = (pdf_char_t *)"1" "\x00\x00\x00" \
                                                     "2" "\x00\x00\x00" \
                                                     "3" "\x00\x00\x00" \
                                                     "4" "\x00\x00\x00" \
                                                     "5" "\x00\x00\x00" \
                                                     "0" "\x00\x00\x00";
  const pdf_char_t *expected_data;
  pdf_size_t expected_size = 24;
  pdf_char_t *actual_data;
  pdf_size_t actual_size;
  pdf_error_t *error = NULL;
  pdf_u32_t number = 123450;
  pdf_text_t *text;

  expected_data = (!pdf_text_test_big_endian_system () ?
                   expected_data_le :
                   expected_data_be);

  /* 1. The call to pdf_text_new_from_u32 should return PDF_OK. */
  text = pdf_text_new_from_u32 (number, &error);
  fail_unless (text != NULL);
  fail_if (error != NULL);

  /* 2. The contents of the text object must be the expected ones. */
  actual_data = pdf_text_get_unicode (text,
                                      PDF_TEXT_UTF32_HE,
                                      PDF_TEXT_UNICODE_NO_OPTION,
                                      &actual_size,
                                      &error);
  fail_unless (actual_data != NULL);
  fail_if (error != NULL);

  PRINT_CONTENTS (__FUNCTION__, 0, text, expected_data, expected_size, 0);

  fail_unless (actual_size == expected_size);
  fail_unless (memcmp (actual_data, expected_data, expected_size) == 0);

  pdf_text_destroy (text);
  pdf_dealloc (actual_data);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_text_new_from_u32 (void)
{
  TCase *tc = tcase_create ("pdf_text_new_from_u32");

  tcase_add_test (tc, pdf_text_new_from_u32_001);
  tcase_add_test (tc, pdf_text_new_from_u32_002);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-text-new-from-u32.c */

/* -*- mode: C -*-
 *
 *       File:         pdf-text-new-from-unicode.c
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_new_from_unicode test case
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
 * Test: pdf_text_new_from_unicode_001-028
 * Description:
 *
 *   Create a text object with an input valid/invalid string encoded in the
 *   different supported Unicode encodings (UTF-8, UTF16_BE, UTF16_LE, UTF32_BE,
 *   UTF32_LE) with and without BOM.  See the definition of the 'test_params'
 *   array in pdf-text-new-from-unicode.c for detailed information about the
 *   parameters used in the tests.
 *
 * Success conditions:
 *    1. The call to  pdf_text_new_from_unicode should return a valid object.
 *    2. The contents of the text object must be the expected ones (with/without BOM).
 *    3. The language code within the text object must be empty or the expected one.
 *    4. The country code within the text object must be empty or the expected one.
 */

struct test_params {
  /* Test index */
  int idx;
  /* pdf_text_get_unicode params: */
  enum pdf_text_unicode_encoding_e enc;
  pdf_bool_t bom;
  pdf_bool_t valid_input;
};

static const struct test_params tests_params[] = {
  /* No   Encoding           BOM Valid */
  {	 1,   PDF_TEXT_UTF8,     0,  1 },
  {	 2,   PDF_TEXT_UTF16_BE, 0,  1 },
  {	 3,   PDF_TEXT_UTF16_LE, 0,  1 },
  {	 4,   PDF_TEXT_UTF16_HE, 0,  1 },
  {	 5,   PDF_TEXT_UTF32_BE, 0,  1 },
  {	 6,   PDF_TEXT_UTF32_LE, 0,  1 },
  {	 7,   PDF_TEXT_UTF32_HE, 0,  1 },
  {	 8,   PDF_TEXT_UTF8,     1,  1 },
  {	 9,   PDF_TEXT_UTF16_BE, 1,  1 },
  {	 10,  PDF_TEXT_UTF16_LE, 1,  1 },
  {	 11,  PDF_TEXT_UTF16_HE, 1,  1 },
  {	 12,  PDF_TEXT_UTF32_BE, 1,  1 },
  {	 13,  PDF_TEXT_UTF32_LE, 1,  1 },
  {	 14,  PDF_TEXT_UTF32_HE, 1,  1 },
  {	 15,  PDF_TEXT_UTF8,     0,  0 },
  {	 16,  PDF_TEXT_UTF16_BE, 0,  0 },
  {	 17,  PDF_TEXT_UTF16_LE, 0,  0 },
  {	 18,  PDF_TEXT_UTF16_HE, 0,  0 },
  {	 19,  PDF_TEXT_UTF32_BE, 0,  0 },
  {	 20,  PDF_TEXT_UTF32_LE, 0,  0 },
  {	 21,  PDF_TEXT_UTF32_HE, 0,  0 },
  {	 22,  PDF_TEXT_UTF8,     1,  0 },
  {	 23,  PDF_TEXT_UTF16_BE, 1,  0 },
  {	 24,  PDF_TEXT_UTF16_LE, 1,  0 },
  {	 25,  PDF_TEXT_UTF16_HE, 1,  0 },
  {	 26,  PDF_TEXT_UTF32_BE, 1,  0 },
  {	 27,  PDF_TEXT_UTF32_LE, 1,  0 },
  {	 28,  PDF_TEXT_UTF32_HE, 1,  0 }};

static void
test_one_string (struct test_params   params,
                 const test_string_t *test_string,
                 const pdf_char_t    *function_name,
                 pdf_u32_t            i)
{
  pdf_error_t *error = NULL;
  pdf_char_t *input_data;
  pdf_size_t input_size;
  pdf_char_t *expected_data;
  pdf_size_t expected_size;
  pdf_char_t *actual_data;
  pdf_size_t actual_size;
  pdf_text_t *text;
  const pdf_char_t *bom;
  int bom_size = 0;
  int str_start_index;
  enum pdf_text_unicode_encoding_e enc;

  /* Input string with BOM? */
  bom = pdf_text_test_get_bom (params.enc, &bom_size);
  fail_unless (bom != NULL);
  str_start_index = (params.bom || !params.valid_input ? 0 : bom_size);

  if (params.enc == PDF_TEXT_UTF16_HE)
    {
      enc = (pdf_text_test_big_endian_system () ?
              PDF_TEXT_UTF16_BE : PDF_TEXT_UTF16_LE);
    }
  else if (params.enc == PDF_TEXT_UTF32_HE)
    {
      enc = (pdf_text_test_big_endian_system () ?
             PDF_TEXT_UTF32_BE : PDF_TEXT_UTF32_LE);
    }
  else
    {
      enc = params.enc;
    }

  /* Setup input data */
  if (!params.valid_input && params.bom)
    {
      /* If no valid input but BOM, prepare a string with valid BOM */
      input_size = bom_size + test_string->size;
      input_data = pdf_alloc (input_size);
      fail_unless (input_data != NULL);
      memcpy (input_data, bom, bom_size);
      memcpy (&input_data[bom_size], test_string->data, test_string->size);
    }
  else
    {
      input_size = test_string->size - str_start_index;
      if (enc == PDF_TEXT_UTF8 ||
          enc == PDF_TEXT_UTF16_BE ||
          enc == PDF_TEXT_UTF32_BE)
        {
          input_data = pdf_alloc (input_size);
          fail_unless (input_data != NULL);
          memcpy (input_data, &test_string->data[str_start_index], input_size);
        }
      else if (enc == PDF_TEXT_UTF16_LE)
        {
          input_data = pdf_text_test_change_utf16_endianness (&test_string->data[str_start_index],
                                                              input_size);
        }
      else if (enc == PDF_TEXT_UTF32_LE)
        {
          input_data = pdf_text_test_change_utf32_endianness (&test_string->data[str_start_index],
                                                              input_size);
        }
    }

  /* 1. Check output of pdf_text_new_from_unicode */
  text = pdf_text_new_from_unicode (input_data,
                                    input_size,
                                    params.enc,
                                    &error);

  /* If no valid input, no valid text! */
  if (!params.valid_input)
    {
      fail_if (text != NULL);
      fail_unless (error != NULL);

      pdf_error_destroy (error);
      pdf_dealloc (input_data);
      return;
    }

  /* 2. The function should return a valid pointer to the new text object */
  fail_unless (text != NULL);
  fail_if (error != NULL);

  /* Set expected data */
  expected_size = test_string->utf32be_size;
  if (!pdf_text_test_big_endian_system ())
    {
      expected_data = pdf_text_test_change_utf32_endianness (test_string->utf32be_data,
                                                             expected_size);
      fail_unless (expected_data != NULL);
    }
  else
    {
      expected_data = pdf_alloc (expected_size);
      fail_unless (expected_data != NULL);
      memcpy (expected_data, test_string->utf32be_data, expected_size);
    }

  PRINT_CONTENTS (function_name, i, text, expected_data, expected_size, 0);

  /* 3. The contents of the text object must be the expected ones. */
  actual_data = pdf_text_get_unicode (text,
                                      PDF_TEXT_UTF32_HE,
                                      PDF_TEXT_UNICODE_NO_OPTION,
                                      &actual_size,
                                      &error);
  fail_unless (actual_data != NULL);
  fail_if (error != NULL);
  fail_unless (actual_size == expected_size);
  fail_unless (memcmp (actual_data, expected_data, expected_size) == 0);

  /* 4. The language code of the output object should be empty */
  fail_unless (strlen (pdf_text_get_language (text)) == 0);

  /* 5. The country code of the output object should be empty */
  fail_unless (strlen (pdf_text_get_country (text)) == 0);

  pdf_text_destroy (text);

  pdf_dealloc (expected_data);
  pdf_dealloc (actual_data);
  pdf_dealloc (input_data);
}

static void
common_test (const pdf_char_t *function_name,
             int               test_index)
{
  extern const test_string_t utf8_strings[];
  extern const test_string_t utf16be_strings[];
  extern const test_string_t utf32be_strings[];
  /* Get test params */
  struct test_params params = tests_params[test_index - 1];
  int i = 0;

  fail_if (params.idx != test_index);

  fail_if (params.enc < PDF_TEXT_UTF8);
  fail_if (params.enc >= PDF_TEXT_MAX_UNICODE_ENC);

  if (params.valid_input)
    {
      const test_string_t *strings;

      /* Tests all strings from pdf-text-test-data.c */
      switch (params.enc)
        {
        case PDF_TEXT_UTF8:
          strings = utf8_strings;
          break;
        case PDF_TEXT_UTF16_BE:
        case PDF_TEXT_UTF16_LE:
        case PDF_TEXT_UTF16_HE:
          strings = utf16be_strings;
          break;
        case PDF_TEXT_UTF32_BE:
        case PDF_TEXT_UTF32_LE:
        case PDF_TEXT_UTF32_HE:
          strings = utf32be_strings;
          break;
        default:
          strings = NULL;
        }

      while (strings && strings[i].data)
        {
          test_one_string (params, &strings[i], function_name, i);
          i++;
        }
    }
  else
    {
      if (params.enc == PDF_TEXT_UTF8)
        {
          /* Pass UTF16 strings as invalid UTF-8 */
          while (utf16be_strings[i].data)
            {
              test_one_string (params, &utf16be_strings[i], function_name, i);
              i++;
            }

          i = 0;
          /* Pass UTF32 strings as invalid UTF-8 */
          while (utf32be_strings[i].data)
            {
              test_one_string (params, &utf32be_strings[i], function_name, i);
              i++;
            }
        }
      else
        {
          /* Pass UTF8 strings as invalid UTF-16/UTF-32 */
          while (utf8_strings[i].data)
            {
              test_one_string (params, &utf8_strings[i], function_name, i);
              i++;
            }
        }
    }
}

START_TEST (pdf_text_new_from_unicode_001) { common_test(__FUNCTION__,  1); } END_TEST
START_TEST (pdf_text_new_from_unicode_002) { common_test(__FUNCTION__,  2); } END_TEST
START_TEST (pdf_text_new_from_unicode_003) { common_test(__FUNCTION__,  3); } END_TEST
START_TEST (pdf_text_new_from_unicode_004) { common_test(__FUNCTION__,  4); } END_TEST
START_TEST (pdf_text_new_from_unicode_005) { common_test(__FUNCTION__,  5); } END_TEST
START_TEST (pdf_text_new_from_unicode_006) { common_test(__FUNCTION__,  6); } END_TEST
START_TEST (pdf_text_new_from_unicode_007) { common_test(__FUNCTION__,  7); } END_TEST
START_TEST (pdf_text_new_from_unicode_008) { common_test(__FUNCTION__,  8); } END_TEST
START_TEST (pdf_text_new_from_unicode_009) { common_test(__FUNCTION__,  9); } END_TEST
START_TEST (pdf_text_new_from_unicode_010) { common_test(__FUNCTION__, 10); } END_TEST
START_TEST (pdf_text_new_from_unicode_011) { common_test(__FUNCTION__, 11); } END_TEST
START_TEST (pdf_text_new_from_unicode_012) { common_test(__FUNCTION__, 12); } END_TEST
START_TEST (pdf_text_new_from_unicode_013) { common_test(__FUNCTION__, 13); } END_TEST
START_TEST (pdf_text_new_from_unicode_014) { common_test(__FUNCTION__, 14); } END_TEST
START_TEST (pdf_text_new_from_unicode_015) { common_test(__FUNCTION__, 15); } END_TEST
START_TEST (pdf_text_new_from_unicode_016) { common_test(__FUNCTION__, 16); } END_TEST
START_TEST (pdf_text_new_from_unicode_017) { common_test(__FUNCTION__, 17); } END_TEST
START_TEST (pdf_text_new_from_unicode_018) { common_test(__FUNCTION__, 18); } END_TEST
START_TEST (pdf_text_new_from_unicode_019) { common_test(__FUNCTION__, 19); } END_TEST
START_TEST (pdf_text_new_from_unicode_020) { common_test(__FUNCTION__, 20); } END_TEST
START_TEST (pdf_text_new_from_unicode_021) { common_test(__FUNCTION__, 21); } END_TEST
START_TEST (pdf_text_new_from_unicode_022) { common_test(__FUNCTION__, 22); } END_TEST
START_TEST (pdf_text_new_from_unicode_023) { common_test(__FUNCTION__, 23); } END_TEST
START_TEST (pdf_text_new_from_unicode_024) { common_test(__FUNCTION__, 24); } END_TEST
START_TEST (pdf_text_new_from_unicode_025) { common_test(__FUNCTION__, 25); } END_TEST
START_TEST (pdf_text_new_from_unicode_026) { common_test(__FUNCTION__, 26); } END_TEST
START_TEST (pdf_text_new_from_unicode_027) { common_test(__FUNCTION__, 27); } END_TEST
START_TEST (pdf_text_new_from_unicode_028) { common_test(__FUNCTION__, 28); } END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_text_new_from_unicode (void)
{
  TCase *tc = tcase_create ("pdf_text_new_from_unicode");

  tcase_add_test (tc, pdf_text_new_from_unicode_001);
  tcase_add_test (tc, pdf_text_new_from_unicode_002);
  tcase_add_test (tc, pdf_text_new_from_unicode_003);
  tcase_add_test (tc, pdf_text_new_from_unicode_004);
  tcase_add_test (tc, pdf_text_new_from_unicode_005);
  tcase_add_test (tc, pdf_text_new_from_unicode_006);
  tcase_add_test (tc, pdf_text_new_from_unicode_007);
  tcase_add_test (tc, pdf_text_new_from_unicode_008);
  tcase_add_test (tc, pdf_text_new_from_unicode_009);
  tcase_add_test (tc, pdf_text_new_from_unicode_010);
  tcase_add_test (tc, pdf_text_new_from_unicode_011);
  tcase_add_test (tc, pdf_text_new_from_unicode_012);
  tcase_add_test (tc, pdf_text_new_from_unicode_013);
  tcase_add_test (tc, pdf_text_new_from_unicode_014);
  tcase_add_test (tc, pdf_text_new_from_unicode_015);
  tcase_add_test (tc, pdf_text_new_from_unicode_016);
  tcase_add_test (tc, pdf_text_new_from_unicode_017);
  tcase_add_test (tc, pdf_text_new_from_unicode_018);
  tcase_add_test (tc, pdf_text_new_from_unicode_019);
  tcase_add_test (tc, pdf_text_new_from_unicode_020);
  tcase_add_test (tc, pdf_text_new_from_unicode_021);
  tcase_add_test (tc, pdf_text_new_from_unicode_022);
  tcase_add_test (tc, pdf_text_new_from_unicode_023);
  tcase_add_test (tc, pdf_text_new_from_unicode_024);
  tcase_add_test (tc, pdf_text_new_from_unicode_025);
  tcase_add_test (tc, pdf_text_new_from_unicode_026);
  tcase_add_test (tc, pdf_text_new_from_unicode_027);
  tcase_add_test (tc, pdf_text_new_from_unicode_028);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-text-new-from-unicode.c */

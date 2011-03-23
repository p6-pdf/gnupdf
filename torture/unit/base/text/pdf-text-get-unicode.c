/* -*- mode: C -*-
 *
 *       File:         pdf-text-get-unicode.c
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_get_unicode test case
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

struct test_params {
  /* Test index */
  int idx;
  /* pdf_text_get_unicode params: */
  enum pdf_text_unicode_encoding_e enc;
  pdf_bool_t bom;
  pdf_bool_t empty;
  pdf_bool_t lang;
  pdf_bool_t country;
  pdf_bool_t nul;
  /* Test goal */
  pdf_bool_t pdf_ok_p;
};

/*
 * Test: pdf_text_get_unicode_001-108
 * Description:
 *
 *   Get the contents of a non-empty text object in the different
 *   supported Unicode encodings (UTF-8, UTF16_BE, UTF16_LE, UTF32_BE,
 *   UTF32_LE) with and without BOM.  See the definition of the
 *   'test_params' array in pdf-text-get-unicode.c for detailed
 *   information about the parameters used in the tests.
 *
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must be the expected one, not NUL terminated.
 *    3. The returned length must be the expected one.
 */

/* Here are all the parameters we'll test, and their different
   combinations.  Getting unicode data with bundled lang&country
   information is only supported for UTF-16BE, so the function will
   not return PDF_OK in that case for all other encodings. */
static const struct test_params tests_params[] = {
  /* UTF-8, no NUL */
  /* No   Encoding           BOM Empty Lang Country NUL PDF_OK? */
  {	 1,   PDF_TEXT_UTF8,     0,  0,    0,   0,      0,  1 },
  {	 2,   PDF_TEXT_UTF8,     1,  0,    0,   0,      0,  1 },
  {	 3,   PDF_TEXT_UTF8,     0,  1,    0,   0,      0,  1 },
  {	 4,   PDF_TEXT_UTF8,     1,  1,    0,   0,      0,  1 },
  {	 5,   PDF_TEXT_UTF8,     0,  0,    1,   1,      0,  0 },
  {	 6,   PDF_TEXT_UTF8,     1,  0,    1,   1,      0,  0 },

  /* UTF-16, no NUL */
  /* No   Encoding           BOM Empty Lang Country NUL PDF_OK? */
  {  7,   PDF_TEXT_UTF16_BE, 0,  0,    0,   0,      0,  1 },
  {	 8,   PDF_TEXT_UTF16_BE, 1,  0,    0,   0,      0,  1 },
  {	 9,   PDF_TEXT_UTF16_BE, 0,  1,    0,   0,      0,  1 },
  {	 10,  PDF_TEXT_UTF16_BE, 1,  1,    0,   0,      0,  1 },
  {	 11,  PDF_TEXT_UTF16_BE, 0,  0,    1,   1,      0,  1 },
  {	 12,  PDF_TEXT_UTF16_BE, 0,  0,    1,   0,      0,  1 },
  {	 13,  PDF_TEXT_UTF16_BE, 0,  1,    1,   1,      0,  1 },
  {	 14,  PDF_TEXT_UTF16_BE, 0,  1,    1,   0,      0,  1 },
  {	 15,  PDF_TEXT_UTF16_BE, 1,  0,    1,   1,      0,  1 },
  {	 16,  PDF_TEXT_UTF16_BE, 1,  0,    1,   0,      0,  1 },
  {	 17,  PDF_TEXT_UTF16_BE, 1,  1,    1,   1,      0,  1 },
  {	 18,  PDF_TEXT_UTF16_BE, 1,  1,    1,   0,      0,  1 },

  {	 19,  PDF_TEXT_UTF16_LE, 0,  0,    0,   0,      0,  1 },
  {	 20,  PDF_TEXT_UTF16_LE, 1,  0,    0,   0,      0,  1 },
  {	 21,  PDF_TEXT_UTF16_LE, 0,  1,    0,   0,      0,  1 },
  {	 22,  PDF_TEXT_UTF16_LE, 1,  1,    0,   0,      0,  1 },
  {	 23,  PDF_TEXT_UTF16_LE, 0,  0,    1,   1,      0,  0 },
  {	 24,  PDF_TEXT_UTF16_LE, 0,  0,    1,   0,      0,  0 },
  {	 25,  PDF_TEXT_UTF16_LE, 0,  1,    1,   1,      0,  0 },
  {	 26,  PDF_TEXT_UTF16_LE, 0,  1,    1,   0,      0,  0 },
  {	 27,  PDF_TEXT_UTF16_LE, 1,  0,    1,   1,      0,  0 },
  {	 28,  PDF_TEXT_UTF16_LE, 1,  0,    1,   0,      0,  0 },
  {	 29,  PDF_TEXT_UTF16_LE, 1,  1,    1,   1,      0,  0 },
  {	 30,  PDF_TEXT_UTF16_LE, 1,  1,    1,   0,      0,  0 },

  /* UTF-32, no NUL */
  /* No   Encoding           BOM Empty Lang Country NUL PDF_OK? */
  {  31,  PDF_TEXT_UTF32_BE, 0,  0,    0,   0,      0,  1 },
  {  32,  PDF_TEXT_UTF32_BE, 1,  0,    0,   0,      0,  1 },
  {  33,  PDF_TEXT_UTF32_BE, 0,  1,    0,   0,      0,  1 },
  {  34,  PDF_TEXT_UTF32_BE, 1,  1,    0,   0,      0,  1 },
  {  35,  PDF_TEXT_UTF32_BE, 0,  0,    1,   1,      0,  0 },
  {  36,  PDF_TEXT_UTF32_BE, 0,  0,    1,   0,      0,  0 },
  {  37,  PDF_TEXT_UTF32_BE, 0,  1,    1,   1,      0,  0 },
  {  38,  PDF_TEXT_UTF32_BE, 0,  1,    1,   0,      0,  0 },
  {  39,  PDF_TEXT_UTF32_BE, 1,  0,    1,   1,      0,  0 },
  {  40,  PDF_TEXT_UTF32_BE, 1,  0,    1,   0,      0,  0 },
  {  41,  PDF_TEXT_UTF32_BE, 1,  1,    1,   1,      0,  0 },
  {  42,  PDF_TEXT_UTF32_BE, 1,  1,    1,   0,      0,  0 },

  {  43,  PDF_TEXT_UTF32_LE, 0,  0,    0,   0,      0,  1 },
  {  44,  PDF_TEXT_UTF32_LE, 1,  0,    0,   0,      0,  1 },
  {  45,  PDF_TEXT_UTF32_LE, 0,  1,    0,   0,      0,  1 },
  {  46,  PDF_TEXT_UTF32_LE, 1,  1,    0,   0,      0,  1 },
  {  47,  PDF_TEXT_UTF32_LE, 0,  0,    1,   1,      0,  0 },
  {  48,  PDF_TEXT_UTF32_LE, 0,  0,    1,   0,      0,  0 },
  {  49,  PDF_TEXT_UTF32_LE, 0,  1,    1,   1,      0,  0 },
  {  50,  PDF_TEXT_UTF32_LE, 0,  1,    1,   0,      0,  0 },
  {  51,  PDF_TEXT_UTF32_LE, 1,  0,    1,   1,      0,  0 },
  {  52,  PDF_TEXT_UTF32_LE, 1,  0,    1,   0,      0,  0 },
  {  53,  PDF_TEXT_UTF32_LE, 1,  1,    1,   1,      0,  0 },
  {  54,  PDF_TEXT_UTF32_LE, 1,  1,    1,   0,      0,  0 },

  /* UTF-8, with NUL */
  /* No   Encoding           BOM Empty Lang Country NUL PDF_OK? */
  {  55,  PDF_TEXT_UTF8,     0,  0,    0,   0,      1,  1 },
  {  56,  PDF_TEXT_UTF8,     1,  0,    0,   0,      1,  1 },
  {  57,  PDF_TEXT_UTF8,     0,  1,    0,   0,      1,  1 },
  {  58,  PDF_TEXT_UTF8,     1,  1,    0,   0,      1,  1 },
  {  59,  PDF_TEXT_UTF8,     0,  0,    1,   1,      1,  0 },
  {  60,  PDF_TEXT_UTF8,     1,  0,    1,   1,      1,  0 },

  /* UTF-16, with NUL */
  /* No   Encoding           BOM Empty Lang Country NUL PDF_OK? */
  {  61,  PDF_TEXT_UTF16_BE, 0,  0,    0,   0,      1,  1 },
  {  62,  PDF_TEXT_UTF16_BE, 1,  0,    0,   0,      1,  1 },
  {  63,  PDF_TEXT_UTF16_BE, 0,  1,    0,   0,      1,  1 },
  {  64,  PDF_TEXT_UTF16_BE, 1,  1,    0,   0,      1,  1 },
  {  65,  PDF_TEXT_UTF16_BE, 0,  0,    1,   1,      1,  1 },
  {  66,  PDF_TEXT_UTF16_BE, 0,  0,    1,   0,      1,  1 },
  {  67,  PDF_TEXT_UTF16_BE, 0,  1,    1,   1,      1,  1 },
  {  68,  PDF_TEXT_UTF16_BE, 0,  1,    1,   0,      1,  1 },
  {  69,  PDF_TEXT_UTF16_BE, 1,  0,    1,   1,      1,  1 },
  {  70,  PDF_TEXT_UTF16_BE, 1,  0,    1,   0,      1,  1 },
  {  71,  PDF_TEXT_UTF16_BE, 1,  1,    1,   1,      1,  1 },
  {  72,  PDF_TEXT_UTF16_BE, 1,  1,    1,   0,      1,  1 },

  {  73,  PDF_TEXT_UTF16_LE, 0,  0,    0,   0,      1,  1 },
  {  74,  PDF_TEXT_UTF16_LE, 1,  0,    0,   0,      1,  1 },
  {  75,  PDF_TEXT_UTF16_LE, 0,  1,    0,   0,      1,  1 },
  {  76,  PDF_TEXT_UTF16_LE, 1,  1,    0,   0,      1,  1 },
  {  77,  PDF_TEXT_UTF16_LE, 0,  0,    1,   1,      1,  0 },
  {  78,  PDF_TEXT_UTF16_LE, 0,  0,    1,   0,      1,  0 },
  {  79,  PDF_TEXT_UTF16_LE, 0,  1,    1,   1,      1,  0 },
  {  80,  PDF_TEXT_UTF16_LE, 0,  1,    1,   0,      1,  0 },
  {  81,  PDF_TEXT_UTF16_LE, 1,  0,    1,   1,      1,  0 },
  {  82,  PDF_TEXT_UTF16_LE, 1,  0,    1,   0,      1,  0 },
  {  83,  PDF_TEXT_UTF16_LE, 1,  1,    1,   1,      1,  0 },
  {  84,  PDF_TEXT_UTF16_LE, 1,  1,    1,   0,      1,  0 },

  /* UTF-32, with NUL */
  /* No   Encoding           BOM Empty Lang Country NUL PDF_OK? */
  {  85,  PDF_TEXT_UTF32_BE, 0,  0,    0,   0,      1,  1 },
  {  86,  PDF_TEXT_UTF32_BE, 1,  0,    0,   0,      1,  1 },
  {  87,  PDF_TEXT_UTF32_BE, 0,  1,    0,   0,      1,  1 },
  {  88,  PDF_TEXT_UTF32_BE, 1,  1,    0,   0,      1,  1 },
  {  89,  PDF_TEXT_UTF32_BE, 0,  0,    1,   1,      1,  0 },
  {  90,  PDF_TEXT_UTF32_BE, 0,  0,    1,   0,      1,  0 },
  {  91,  PDF_TEXT_UTF32_BE, 0,  1,    1,   1,      1,  0 },
  {  92,  PDF_TEXT_UTF32_BE, 0,  1,    1,   0,      1,  0 },
  {  93,  PDF_TEXT_UTF32_BE, 1,  0,    1,   1,      1,  0 },
  {  94,  PDF_TEXT_UTF32_BE, 1,  0,    1,   0,      1,  0 },
  {  95,  PDF_TEXT_UTF32_BE, 1,  1,    1,   1,      1,  0 },
  {  96,  PDF_TEXT_UTF32_BE, 1,  1,    1,   0,      1,  0 },

  {  97,  PDF_TEXT_UTF32_LE, 0,  0,    0,   0,      1,  1 },
  {  98,  PDF_TEXT_UTF32_LE, 1,  0,    0,   0,      1,  1 },
  {  99,  PDF_TEXT_UTF32_LE, 0,  1,    0,   0,      1,  1 },
  {  100, PDF_TEXT_UTF32_LE, 1,  1,    0,   0,      1,  1 },
  {  101, PDF_TEXT_UTF32_LE, 0,  0,    1,   1,      1,  0 },
  {  102, PDF_TEXT_UTF32_LE, 0,  0,    1,   0,      1,  0 },
  {  103, PDF_TEXT_UTF32_LE, 0,  1,    1,   1,      1,  0 },
  {  104, PDF_TEXT_UTF32_LE, 0,  1,    1,   0,      1,  0 },
  {  105, PDF_TEXT_UTF32_LE, 1,  0,    1,   1,      1,  0 },
  {  106, PDF_TEXT_UTF32_LE, 1,  0,    1,   0,      1,  0 },
  {  107, PDF_TEXT_UTF32_LE, 1,  1,    1,   1,      1,  0 },
  {  108, PDF_TEXT_UTF32_LE, 1,  1,    1,   0,      1,  0 }};

static void
test_one_string (struct test_params   params,
                 pdf_bool_t           empty,
                 const test_string_t *test_string,
                 const pdf_char_t    *function_name,
                 pdf_u32_t            i)
{
  pdf_error_t *error = NULL;
  pdf_text_t *text;
  pdf_char_t *initial_utf_data = NULL;
  pdf_size_t  initial_utf_data_size = 0;
  const pdf_char_t *language = (pdf_char_t *)"en";
  const pdf_char_t *country = (pdf_char_t *)"GB";
  pdf_char_t langcode[8];
  pdf_char_t langcode_size = 0;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  pdf_u32_t options = PDF_TEXT_UNICODE_NO_OPTION;
  int expected_size = 0;
  int bom_size = 0;
  int nul_size = 0;

  /* - from strings if non-empty */
  if (!params.empty)
    {
      pdf_char_t *source;
      int initial_utf_bom_size = 0;

      if (params.enc == PDF_TEXT_UTF8)
        initial_utf_bom_size = 3;
      else if (params.enc == PDF_TEXT_UTF16_BE || params.enc == PDF_TEXT_UTF16_LE)
        initial_utf_bom_size = 2;
      else if (params.enc == PDF_TEXT_UTF32_BE || params.enc == PDF_TEXT_UTF32_LE)
        initial_utf_bom_size = 4;

      initial_utf_data_size = test_string->size - initial_utf_bom_size;
      source = (pdf_char_t *)&(test_string->data[initial_utf_bom_size]);
      /* change endianness if needed */
      if (params.enc == PDF_TEXT_UTF16_LE)
        {
          initial_utf_data = pdf_text_test_change_utf16_endianness (source,
                                                                    initial_utf_data_size);
        }
      else if (params.enc == PDF_TEXT_UTF32_LE)
        {
          initial_utf_data = pdf_text_test_change_utf32_endianness (source,
                                                                    initial_utf_data_size);
        }
      else
        {
          /* just strdup, this eases freeing memory at the end of the test */
          initial_utf_data = pdf_alloc (initial_utf_data_size);
          memcpy (initial_utf_data, source, initial_utf_data_size);
        }

      text = pdf_text_new_from_unicode (test_string->utf32be_data,
                                        test_string->utf32be_size,
                                        PDF_TEXT_UTF32_BE,
                                        &error);
    }
  /* - from scratch if empty */
  else
    {
      text = pdf_text_new (&error);
    }

  fail_unless (text != NULL);
  fail_if (error != NULL);

  /* Lang/country information */
  if (params.lang || params.country)
    {
      if (params.lang)
        pdf_text_set_language (text, language);
      if (params.country)
        pdf_text_set_country (text, country);

      memcpy (&langcode[langcode_size], "\x00\x1B", 2);
      langcode_size += 2;
      if (params.lang) {
        memcpy (&langcode[langcode_size], "en", 2);
        langcode_size += 2;
      }
      if (params.country) {
        memcpy (&langcode[langcode_size], "GB", 2);
        langcode_size += 2;
      }
      memcpy (&langcode[langcode_size], "\x00\x1B", 2);
      langcode_size += 2;
    }

  /* Get unicode from text object */
  if (params.bom)
    options |= PDF_TEXT_UNICODE_WITH_BOM;
  if (params.lang || params.country)
    options |= PDF_TEXT_UTF16BE_WITH_LANGCODE;
  if (params.nul)
    options |= PDF_TEXT_UNICODE_WITH_NUL_SUFFIX;

  /***********************************
   * THE 'pdf_text_get_unicode' CALL *
   *                                 *
   ***********************************/
  data = pdf_text_get_unicode (text,
                               params.enc,
                               options,
                               &size,
                               &error);

  /* 1. Fail depending on pdf_ok_p */
  if (params.pdf_ok_p)
    {
      if (params.empty && !params.bom && !params.lang && !params.nul)
        fail_unless (data == NULL);
      else
        fail_unless (data != NULL);
      fail_if (error != NULL);
    }
  else
    {
      fail_if (data != NULL);
      fail_unless (error != NULL);
      pdf_error_destroy (error);
    }

  /* - Stop if expected result is !PDF_OK */
  if (!params.pdf_ok_p)
    return;
  /* - Stop if empty && !bom && !lang */
  if (params.empty && !params.bom && !params.lang)
    return;

  /* 2. The returned string must be the expected one */
  PRINT_CONTENTS (function_name,
                  params.idx,
                  text,
                  initial_utf_data,
                  initial_utf_data_size,
                  0);

  /* - verify BOM if present */
  if (params.bom)
    {
      if (params.enc == PDF_TEXT_UTF8)
        {
          bom_size = 3;
          fail_unless (memcmp (&data[0], "\xEF\xBB\xBF", bom_size) == 0);
        }
      else if (params.enc == PDF_TEXT_UTF16_BE)
        {
          bom_size = 2;
          fail_unless (memcmp(&data[0], "\xFE\xFF", bom_size) == 0);
        }
      else if (params.enc == PDF_TEXT_UTF16_LE)
        {
          bom_size = 2;
          fail_unless (memcmp(&data[0], "\xFF\xFE", bom_size) == 0);
        }
      else if (params.enc == PDF_TEXT_UTF32_BE)
        {
          bom_size = 4;
          fail_unless (memcmp(&data[0], "\x00\x00\xFE\xFF", bom_size) == 0);
        }
      else if (params.enc == PDF_TEXT_UTF32_LE)
        {
          bom_size = 4;
          fail_unless (memcmp(&data[0], "\xFF\xFE\x00\x00", bom_size) == 0);
        }
    }
  expected_size += bom_size;

  /* - verify country/lang if present */
  fail_unless (memcmp (&data[bom_size], langcode, langcode_size) == 0);
  expected_size += langcode_size;

  /* - the returned string must be the expected one */
  /*  printf("test(%02d): bom_size = %d, langcode_size = %d\n", params.idx, bom_size, langcode_size); */
  fail_unless (memcmp (&data[bom_size + langcode_size],
                       initial_utf_data,
                       initial_utf_data_size) == 0);
  expected_size += initial_utf_data_size;

  /* - verify NUL if present */
  /*   Last NUL has different sizes depending the encoding: */
  /*   - 1 NUL byte for UTF-8 */
  /*   - 2 NUL bytes for all the UTF-16 encodings */
  /*   - 4 NUL bytes for all the UTF-32 encodings */

  if (params.nul)
    {
      if (params.enc == PDF_TEXT_UTF8)
        {
          nul_size = 1;
          fail_unless (data[size - 1] == '\0');
        }
      else if (params.enc == PDF_TEXT_UTF16_BE || params.enc == PDF_TEXT_UTF16_LE)
        {
          nul_size = 2;
          fail_unless (memcmp (&data[size - nul_size], "\x00\x00", nul_size) == 0);
        }
      else if (params.enc == PDF_TEXT_UTF32_BE || params.enc == PDF_TEXT_UTF32_LE)
        {
          nul_size = 4;
          fail_unless (memcmp (&data[size - nul_size], "\x00\x00\x00\x00", nul_size) == 0);
        }
    }
  expected_size += nul_size;

  /* 3. The returned length must be the expected one. */
  fail_unless (size == expected_size);

  /* Clean-up */
  pdf_text_destroy (text);
  pdf_dealloc (data);
  pdf_dealloc (initial_utf_data);
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

  fail_if (params.idx != test_index);

  if (params.empty)
    {
      /* Testing the empty text - only 1 possible test */
      test_one_string (params, 1, NULL, function_name, 0);
    }
  else
    {
      /* Tests all strings from pdf-text-test-data.c */
      int i = 0;

      if (params.enc == PDF_TEXT_UTF8)
        {
          while (utf8_strings[i].data)
            {
              test_one_string (params, 0, &utf8_strings[i], function_name, i);
              i++;
            }
        }
      else if (params.enc == PDF_TEXT_UTF16_BE || params.enc == PDF_TEXT_UTF16_LE)
        {
          while (utf16be_strings[i].data)
            {
              test_one_string (params, 0, &utf16be_strings[i], function_name, i);
              i++;
            }
        }
      else /* if (params.enc == PDF_TEXT_UTF32_BE || params.enc == PDF_TEXT_UTF32_LE) */
        {
          while (utf32be_strings[i].data)
            {
              test_one_string (params, 0, &utf32be_strings[i], function_name, i);
              i++;
            }
        }
    }
}

START_TEST (pdf_text_get_unicode_001) { common_test(__FUNCTION__,  1); } END_TEST
START_TEST (pdf_text_get_unicode_002) { common_test(__FUNCTION__,  2); } END_TEST
START_TEST (pdf_text_get_unicode_003) { common_test(__FUNCTION__,  3); } END_TEST
START_TEST (pdf_text_get_unicode_004) { common_test(__FUNCTION__,  4); } END_TEST
START_TEST (pdf_text_get_unicode_005) { common_test(__FUNCTION__,  5); } END_TEST
START_TEST (pdf_text_get_unicode_006) { common_test(__FUNCTION__,  6); } END_TEST
START_TEST (pdf_text_get_unicode_007) { common_test(__FUNCTION__,  7); } END_TEST
START_TEST (pdf_text_get_unicode_008) { common_test(__FUNCTION__,  8); } END_TEST
START_TEST (pdf_text_get_unicode_009) { common_test(__FUNCTION__,  9); } END_TEST
START_TEST (pdf_text_get_unicode_010) { common_test(__FUNCTION__, 10); } END_TEST
START_TEST (pdf_text_get_unicode_011) { common_test(__FUNCTION__, 11); } END_TEST
START_TEST (pdf_text_get_unicode_012) { common_test(__FUNCTION__, 12); } END_TEST
START_TEST (pdf_text_get_unicode_013) { common_test(__FUNCTION__, 13); } END_TEST
START_TEST (pdf_text_get_unicode_014) { common_test(__FUNCTION__, 14); } END_TEST
START_TEST (pdf_text_get_unicode_015) { common_test(__FUNCTION__, 15); } END_TEST
START_TEST (pdf_text_get_unicode_016) { common_test(__FUNCTION__, 16); } END_TEST
START_TEST (pdf_text_get_unicode_017) { common_test(__FUNCTION__, 17); } END_TEST
START_TEST (pdf_text_get_unicode_018) { common_test(__FUNCTION__, 18); } END_TEST
START_TEST (pdf_text_get_unicode_019) { common_test(__FUNCTION__, 19); } END_TEST
START_TEST (pdf_text_get_unicode_020) { common_test(__FUNCTION__, 20); } END_TEST
START_TEST (pdf_text_get_unicode_021) { common_test(__FUNCTION__, 21); } END_TEST
START_TEST (pdf_text_get_unicode_022) { common_test(__FUNCTION__, 22); } END_TEST
START_TEST (pdf_text_get_unicode_023) { common_test(__FUNCTION__, 23); } END_TEST
START_TEST (pdf_text_get_unicode_024) { common_test(__FUNCTION__, 24); } END_TEST
START_TEST (pdf_text_get_unicode_025) { common_test(__FUNCTION__, 25); } END_TEST
START_TEST (pdf_text_get_unicode_026) { common_test(__FUNCTION__, 26); } END_TEST
START_TEST (pdf_text_get_unicode_027) { common_test(__FUNCTION__, 27); } END_TEST
START_TEST (pdf_text_get_unicode_028) { common_test(__FUNCTION__, 28); } END_TEST
START_TEST (pdf_text_get_unicode_029) { common_test(__FUNCTION__, 29); } END_TEST
START_TEST (pdf_text_get_unicode_030) { common_test(__FUNCTION__, 30); } END_TEST
START_TEST (pdf_text_get_unicode_031) { common_test(__FUNCTION__, 31); } END_TEST
START_TEST (pdf_text_get_unicode_032) { common_test(__FUNCTION__, 32); } END_TEST
START_TEST (pdf_text_get_unicode_033) { common_test(__FUNCTION__, 33); } END_TEST
START_TEST (pdf_text_get_unicode_034) { common_test(__FUNCTION__, 34); } END_TEST
START_TEST (pdf_text_get_unicode_035) { common_test(__FUNCTION__, 35); } END_TEST
START_TEST (pdf_text_get_unicode_036) { common_test(__FUNCTION__, 36); } END_TEST
START_TEST (pdf_text_get_unicode_037) { common_test(__FUNCTION__, 37); } END_TEST
START_TEST (pdf_text_get_unicode_038) { common_test(__FUNCTION__, 38); } END_TEST
START_TEST (pdf_text_get_unicode_039) { common_test(__FUNCTION__, 39); } END_TEST
START_TEST (pdf_text_get_unicode_040) { common_test(__FUNCTION__, 40); } END_TEST
START_TEST (pdf_text_get_unicode_041) { common_test(__FUNCTION__, 41); } END_TEST
START_TEST (pdf_text_get_unicode_042) { common_test(__FUNCTION__, 42); } END_TEST
START_TEST (pdf_text_get_unicode_043) { common_test(__FUNCTION__, 43); } END_TEST
START_TEST (pdf_text_get_unicode_044) { common_test(__FUNCTION__, 44); } END_TEST
START_TEST (pdf_text_get_unicode_045) { common_test(__FUNCTION__, 45); } END_TEST
START_TEST (pdf_text_get_unicode_046) { common_test(__FUNCTION__, 46); } END_TEST
START_TEST (pdf_text_get_unicode_047) { common_test(__FUNCTION__, 47); } END_TEST
START_TEST (pdf_text_get_unicode_048) { common_test(__FUNCTION__, 48); } END_TEST
START_TEST (pdf_text_get_unicode_049) { common_test(__FUNCTION__, 49); } END_TEST
START_TEST (pdf_text_get_unicode_050) { common_test(__FUNCTION__, 50); } END_TEST
START_TEST (pdf_text_get_unicode_051) { common_test(__FUNCTION__, 51); } END_TEST
START_TEST (pdf_text_get_unicode_052) { common_test(__FUNCTION__, 52); } END_TEST
START_TEST (pdf_text_get_unicode_053) { common_test(__FUNCTION__, 53); } END_TEST
START_TEST (pdf_text_get_unicode_054) { common_test(__FUNCTION__, 54); } END_TEST
START_TEST (pdf_text_get_unicode_055) { common_test(__FUNCTION__, 55); } END_TEST
START_TEST (pdf_text_get_unicode_056) { common_test(__FUNCTION__, 56); } END_TEST
START_TEST (pdf_text_get_unicode_057) { common_test(__FUNCTION__, 57); } END_TEST
START_TEST (pdf_text_get_unicode_058) { common_test(__FUNCTION__, 58); } END_TEST
START_TEST (pdf_text_get_unicode_059) { common_test(__FUNCTION__, 59); } END_TEST
START_TEST (pdf_text_get_unicode_060) { common_test(__FUNCTION__, 60); } END_TEST
START_TEST (pdf_text_get_unicode_061) { common_test(__FUNCTION__, 61); } END_TEST
START_TEST (pdf_text_get_unicode_062) { common_test(__FUNCTION__, 62); } END_TEST
START_TEST (pdf_text_get_unicode_063) { common_test(__FUNCTION__, 63); } END_TEST
START_TEST (pdf_text_get_unicode_064) { common_test(__FUNCTION__, 64); } END_TEST
START_TEST (pdf_text_get_unicode_065) { common_test(__FUNCTION__, 65); } END_TEST
START_TEST (pdf_text_get_unicode_066) { common_test(__FUNCTION__, 66); } END_TEST
START_TEST (pdf_text_get_unicode_067) { common_test(__FUNCTION__, 67); } END_TEST
START_TEST (pdf_text_get_unicode_068) { common_test(__FUNCTION__, 68); } END_TEST
START_TEST (pdf_text_get_unicode_069) { common_test(__FUNCTION__, 69); } END_TEST
START_TEST (pdf_text_get_unicode_070) { common_test(__FUNCTION__, 70); } END_TEST
START_TEST (pdf_text_get_unicode_071) { common_test(__FUNCTION__, 71); } END_TEST
START_TEST (pdf_text_get_unicode_072) { common_test(__FUNCTION__, 72); } END_TEST
START_TEST (pdf_text_get_unicode_073) { common_test(__FUNCTION__, 73); } END_TEST
START_TEST (pdf_text_get_unicode_074) { common_test(__FUNCTION__, 74); } END_TEST
START_TEST (pdf_text_get_unicode_075) { common_test(__FUNCTION__, 75); } END_TEST
START_TEST (pdf_text_get_unicode_076) { common_test(__FUNCTION__, 76); } END_TEST
START_TEST (pdf_text_get_unicode_077) { common_test(__FUNCTION__, 77); } END_TEST
START_TEST (pdf_text_get_unicode_078) { common_test(__FUNCTION__, 78); } END_TEST
START_TEST (pdf_text_get_unicode_079) { common_test(__FUNCTION__, 79); } END_TEST
START_TEST (pdf_text_get_unicode_080) { common_test(__FUNCTION__, 80); } END_TEST
START_TEST (pdf_text_get_unicode_081) { common_test(__FUNCTION__, 81); } END_TEST
START_TEST (pdf_text_get_unicode_082) { common_test(__FUNCTION__, 82); } END_TEST
START_TEST (pdf_text_get_unicode_083) { common_test(__FUNCTION__, 83); } END_TEST
START_TEST (pdf_text_get_unicode_084) { common_test(__FUNCTION__, 84); } END_TEST
START_TEST (pdf_text_get_unicode_085) { common_test(__FUNCTION__, 85); } END_TEST
START_TEST (pdf_text_get_unicode_086) { common_test(__FUNCTION__, 86); } END_TEST
START_TEST (pdf_text_get_unicode_087) { common_test(__FUNCTION__, 87); } END_TEST
START_TEST (pdf_text_get_unicode_088) { common_test(__FUNCTION__, 88); } END_TEST
START_TEST (pdf_text_get_unicode_089) { common_test(__FUNCTION__, 89); } END_TEST
START_TEST (pdf_text_get_unicode_090) { common_test(__FUNCTION__, 90); } END_TEST
START_TEST (pdf_text_get_unicode_091) { common_test(__FUNCTION__, 91); } END_TEST
START_TEST (pdf_text_get_unicode_092) { common_test(__FUNCTION__, 92); } END_TEST
START_TEST (pdf_text_get_unicode_093) { common_test(__FUNCTION__, 93); } END_TEST
START_TEST (pdf_text_get_unicode_094) { common_test(__FUNCTION__, 94); } END_TEST
START_TEST (pdf_text_get_unicode_095) { common_test(__FUNCTION__, 95); } END_TEST
START_TEST (pdf_text_get_unicode_096) { common_test(__FUNCTION__, 96); } END_TEST
START_TEST (pdf_text_get_unicode_097) { common_test(__FUNCTION__, 97); } END_TEST
START_TEST (pdf_text_get_unicode_098) { common_test(__FUNCTION__, 98); } END_TEST
START_TEST (pdf_text_get_unicode_099) { common_test(__FUNCTION__, 99); } END_TEST
START_TEST (pdf_text_get_unicode_100) { common_test(__FUNCTION__,100); } END_TEST
START_TEST (pdf_text_get_unicode_101) { common_test(__FUNCTION__,101); } END_TEST
START_TEST (pdf_text_get_unicode_102) { common_test(__FUNCTION__,102); } END_TEST
START_TEST (pdf_text_get_unicode_103) { common_test(__FUNCTION__,103); } END_TEST
START_TEST (pdf_text_get_unicode_104) { common_test(__FUNCTION__,104); } END_TEST
START_TEST (pdf_text_get_unicode_105) { common_test(__FUNCTION__,105); } END_TEST
START_TEST (pdf_text_get_unicode_106) { common_test(__FUNCTION__,106); } END_TEST
START_TEST (pdf_text_get_unicode_107) { common_test(__FUNCTION__,107); } END_TEST
START_TEST (pdf_text_get_unicode_108) { common_test(__FUNCTION__,108); } END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_text_get_unicode (void)
{
  TCase *tc = tcase_create ("pdf_text_get_unicode");

  tcase_add_test (tc, pdf_text_get_unicode_001);
  tcase_add_test (tc, pdf_text_get_unicode_002);
  tcase_add_test (tc, pdf_text_get_unicode_003);
  tcase_add_test (tc, pdf_text_get_unicode_004);
  tcase_add_test (tc, pdf_text_get_unicode_005);
  tcase_add_test (tc, pdf_text_get_unicode_006);

  tcase_add_test (tc, pdf_text_get_unicode_007);
  tcase_add_test (tc, pdf_text_get_unicode_008);
  tcase_add_test (tc, pdf_text_get_unicode_009);
  tcase_add_test (tc, pdf_text_get_unicode_010);
  tcase_add_test (tc, pdf_text_get_unicode_011);
  tcase_add_test (tc, pdf_text_get_unicode_012);
  tcase_add_test (tc, pdf_text_get_unicode_013);
  tcase_add_test (tc, pdf_text_get_unicode_014);
  tcase_add_test (tc, pdf_text_get_unicode_015);
  tcase_add_test (tc, pdf_text_get_unicode_016);
  tcase_add_test (tc, pdf_text_get_unicode_017);
  tcase_add_test (tc, pdf_text_get_unicode_018);

  tcase_add_test (tc, pdf_text_get_unicode_019);
  tcase_add_test (tc, pdf_text_get_unicode_020);
  tcase_add_test (tc, pdf_text_get_unicode_021);
  tcase_add_test (tc, pdf_text_get_unicode_022);
  tcase_add_test (tc, pdf_text_get_unicode_023);
  tcase_add_test (tc, pdf_text_get_unicode_024);
  tcase_add_test (tc, pdf_text_get_unicode_025);
  tcase_add_test (tc, pdf_text_get_unicode_026);
  tcase_add_test (tc, pdf_text_get_unicode_027);
  tcase_add_test (tc, pdf_text_get_unicode_028);
  tcase_add_test (tc, pdf_text_get_unicode_029);
  tcase_add_test (tc, pdf_text_get_unicode_030);

  tcase_add_test (tc, pdf_text_get_unicode_031);
  tcase_add_test (tc, pdf_text_get_unicode_032);
  tcase_add_test (tc, pdf_text_get_unicode_033);
  tcase_add_test (tc, pdf_text_get_unicode_034);
  tcase_add_test (tc, pdf_text_get_unicode_035);
  tcase_add_test (tc, pdf_text_get_unicode_036);
  tcase_add_test (tc, pdf_text_get_unicode_037);
  tcase_add_test (tc, pdf_text_get_unicode_038);
  tcase_add_test (tc, pdf_text_get_unicode_039);
  tcase_add_test (tc, pdf_text_get_unicode_040);
  tcase_add_test (tc, pdf_text_get_unicode_041);
  tcase_add_test (tc, pdf_text_get_unicode_042);

  tcase_add_test (tc, pdf_text_get_unicode_043);
  tcase_add_test (tc, pdf_text_get_unicode_044);
  tcase_add_test (tc, pdf_text_get_unicode_045);
  tcase_add_test (tc, pdf_text_get_unicode_046);
  tcase_add_test (tc, pdf_text_get_unicode_047);
  tcase_add_test (tc, pdf_text_get_unicode_048);
  tcase_add_test (tc, pdf_text_get_unicode_049);
  tcase_add_test (tc, pdf_text_get_unicode_050);
  tcase_add_test (tc, pdf_text_get_unicode_051);
  tcase_add_test (tc, pdf_text_get_unicode_052);
  tcase_add_test (tc, pdf_text_get_unicode_053);
  tcase_add_test (tc, pdf_text_get_unicode_054);


  tcase_add_test (tc, pdf_text_get_unicode_055);
  tcase_add_test (tc, pdf_text_get_unicode_056);
  tcase_add_test (tc, pdf_text_get_unicode_057);
  tcase_add_test (tc, pdf_text_get_unicode_058);
  tcase_add_test (tc, pdf_text_get_unicode_059);
  tcase_add_test (tc, pdf_text_get_unicode_060);

  tcase_add_test (tc, pdf_text_get_unicode_061);
  tcase_add_test (tc, pdf_text_get_unicode_062);
  tcase_add_test (tc, pdf_text_get_unicode_063);
  tcase_add_test (tc, pdf_text_get_unicode_064);
  tcase_add_test (tc, pdf_text_get_unicode_065);
  tcase_add_test (tc, pdf_text_get_unicode_066);
  tcase_add_test (tc, pdf_text_get_unicode_067);
  tcase_add_test (tc, pdf_text_get_unicode_068);
  tcase_add_test (tc, pdf_text_get_unicode_069);
  tcase_add_test (tc, pdf_text_get_unicode_070);
  tcase_add_test (tc, pdf_text_get_unicode_071);
  tcase_add_test (tc, pdf_text_get_unicode_072);

  tcase_add_test (tc, pdf_text_get_unicode_073);
  tcase_add_test (tc, pdf_text_get_unicode_074);
  tcase_add_test (tc, pdf_text_get_unicode_075);
  tcase_add_test (tc, pdf_text_get_unicode_076);
  tcase_add_test (tc, pdf_text_get_unicode_077);
  tcase_add_test (tc, pdf_text_get_unicode_078);
  tcase_add_test (tc, pdf_text_get_unicode_079);
  tcase_add_test (tc, pdf_text_get_unicode_080);
  tcase_add_test (tc, pdf_text_get_unicode_081);
  tcase_add_test (tc, pdf_text_get_unicode_082);
  tcase_add_test (tc, pdf_text_get_unicode_083);
  tcase_add_test (tc, pdf_text_get_unicode_084);

  tcase_add_test (tc, pdf_text_get_unicode_085);
  tcase_add_test (tc, pdf_text_get_unicode_086);
  tcase_add_test (tc, pdf_text_get_unicode_087);
  tcase_add_test (tc, pdf_text_get_unicode_088);
  tcase_add_test (tc, pdf_text_get_unicode_089);
  tcase_add_test (tc, pdf_text_get_unicode_090);
  tcase_add_test (tc, pdf_text_get_unicode_091);
  tcase_add_test (tc, pdf_text_get_unicode_092);
  tcase_add_test (tc, pdf_text_get_unicode_093);
  tcase_add_test (tc, pdf_text_get_unicode_094);
  tcase_add_test (tc, pdf_text_get_unicode_095);
  tcase_add_test (tc, pdf_text_get_unicode_096);

  tcase_add_test (tc, pdf_text_get_unicode_097);
  tcase_add_test (tc, pdf_text_get_unicode_098);
  tcase_add_test (tc, pdf_text_get_unicode_099);
  tcase_add_test (tc, pdf_text_get_unicode_100);
  tcase_add_test (tc, pdf_text_get_unicode_101);
  tcase_add_test (tc, pdf_text_get_unicode_102);
  tcase_add_test (tc, pdf_text_get_unicode_103);
  tcase_add_test (tc, pdf_text_get_unicode_104);
  tcase_add_test (tc, pdf_text_get_unicode_105);
  tcase_add_test (tc, pdf_text_get_unicode_106);
  tcase_add_test (tc, pdf_text_get_unicode_107);
  tcase_add_test (tc, pdf_text_get_unicode_108);

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-text-get-unicode.c */

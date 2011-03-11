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
 * Test: pdf_text_new_from_unicode_001
 * Description:
 *   Create a text object with an input valid UTF-8 encoded string
 *   (containing 8-bit, 16-bit, 24-bit and 32-bit code points!) without BOM
 * Success conditions:
 *   1. The call to @code{pdf_text_new_from_unicode} should return PDF_OK.
 *   2. The function should return a valid pointer to the new text object
 *   3. The contents of the text object must be the expected ones (without BOM).
 *   4. The language code within the text object must be empty.
 *   5. The country code within the text object must be empty.
 */
START_TEST (pdf_text_new_from_unicode_001)
{
  extern const test_string_t utf8_strings[];
  int i;

  i = 0;
  while (utf8_strings[i].data)
    {
      pdf_error_t *error = NULL;
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data;
      const pdf_char_t *input_data;
      pdf_size_t input_size;
      pdf_size_t expected_size;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      pdf_text_t *text;

      /* Set input data */
      input_data = (pdf_char_t *)&(utf8_strings[i].data[3]);
      input_size = (pdf_size_t)utf8_strings[i].size - 3;

      /* Set expected data */
      expected_data = (pdf_char_t *)utf8_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf8_strings[i].utf32be_size;
      if (!pdf_text_test_big_endian_system ())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness (expected_data,
                                                                 expected_size);
          /* Just in case... */
          fail_unless (expected_data != NULL);
        }

      /* 1. The call to pdf_text_new_from_unicode should return PDF_OK. */
      text = pdf_text_new_from_unicode (input_data,
                                        input_size,
                                        PDF_TEXT_UTF8,
                                        &error);

      /* 2. The function should return a valid pointer to the new text object */
      fail_unless (text != NULL);
      fail_if (error != NULL);

      PRINT_CONTENTS (i, text, expected_data, expected_size, 0);

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

      if (expected_free)
        pdf_dealloc (expected_data);
      pdf_dealloc (actual_data);

      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_new_from_unicode_002
 * Description:
 *   Create a text object with an input valid UTF-16BE encoded string
 *    (16-bit and 32-bit code points!) without BOM
 * Success conditions:
 *   1. The call to pdf_text_new_from_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 *   3. The function should return a valid pointer to the new text object
 *   4. The language code within the text object must be empty.
 *   5. The country code within the text object must be empty.
 */
START_TEST (pdf_text_new_from_unicode_002)
{
  extern const test_string_t utf16be_strings[];
  int i;

  i = 0;
  while (utf16be_strings[i].data)
    {
      pdf_error_t *error = NULL;
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data;
      const pdf_char_t *input_data;
      pdf_size_t input_size;
      pdf_size_t expected_size;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      pdf_text_t *text;

      /* Set input data */
      input_data = (pdf_char_t *)&(utf16be_strings[i].data[2]);
      input_size = (pdf_size_t)utf16be_strings[i].size -2;

      /* Set expected data */
      expected_data = (pdf_char_t *)utf16be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf16be_strings[i].utf32be_size;
      if (!pdf_text_test_big_endian_system ())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness (expected_data,
                                                                 expected_size);
          /* Just in case... */
          fail_unless (expected_data != NULL);
        }

      /* 1. The call to pdf_text_new_from_unicode should return PDF_OK. */
      text = pdf_text_new_from_unicode (input_data,
                                        input_size,
                                        PDF_TEXT_UTF16_BE,
                                        &error);

      /* 2. The function should return a valid pointer to the new text object */
      fail_unless (text != NULL);
      fail_if (error != NULL);

      PRINT_CONTENTS (i, text, expected_data, expected_size, 0);

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

      if (expected_free)
        pdf_dealloc (expected_data);
      pdf_dealloc (actual_data);

      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_new_from_unicode_003
 * Description:
 *   Create a text object with an input valid UTF-16LE encoded string
 *    (16-bit and 32-bit code points!) without BOM
 * Success conditions:
 *   1. The call to pdf_text_new_from_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 *   3. The function should return a valid pointer to the new text object
 *   4. The language code within the text object must be empty.
 *   5. The country code within the text object must be empty.
 */
START_TEST (pdf_text_new_from_unicode_003)
{
  extern const test_string_t utf16be_strings[];
  int i;

  i = 0;
  while (utf16be_strings[i].data)
    {
      pdf_error_t *error = NULL;
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data;
      pdf_char_t *input_data;
      pdf_size_t input_size;
      pdf_size_t expected_size;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      pdf_text_t *text;

      /* Set input data (change endiannes to input UTF-16BE string) */
      input_size = (pdf_size_t)utf16be_strings[i].size - 2;
      input_data = pdf_text_test_change_utf16_endianness ((pdf_char_t *)&utf16be_strings[i].data[2],
                                                          input_size);

      /* Set expected data */
      expected_data = (pdf_char_t *)utf16be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf16be_strings[i].utf32be_size;
      if (!pdf_text_test_big_endian_system ())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness (expected_data,
                                                                 expected_size);
          /* Just in case... */
          fail_unless (expected_data != NULL);
        }

      /* 1. The call to pdf_text_new_from_unicode should return PDF_OK. */
      text = pdf_text_new_from_unicode (input_data,
                                        input_size,
                                        PDF_TEXT_UTF16_LE,
                                        &error);

      /* 2. The function should return a valid pointer to the new text object */
      fail_unless (text != NULL);
      fail_if (error != NULL);

      PRINT_CONTENTS (i, text, expected_data, expected_size, 0);

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

      if (expected_free)
        pdf_dealloc (expected_data);
      pdf_dealloc (actual_data);

      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_new_from_unicode_004
 * Description:
 *   Create a text object with an input valid UTF-16HE encoded string
 *    (16-bit and 32-bit code points!) without BOM
 * Success conditions:
 *   1. The call to pdf_text_new_from_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 *   3. The function should return a valid pointer to the new text object
 *   4. The language code within the text object must be empty.
 *   5. The country code within the text object must be empty.
 */
START_TEST (pdf_text_new_from_unicode_004)
{
  extern const test_string_t utf16be_strings[];
  int i;

  i = 0;
  while (utf16be_strings[i].data)
    {
      pdf_error_t *error = NULL;
      pdf_bool_t input_free = PDF_FALSE;
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data;
      pdf_char_t *input_data;
      pdf_size_t input_size;
      pdf_size_t expected_size;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      pdf_text_t *text;

      /* Set input data (change endiannes to input UTF-16BE string if required)
       */
      input_size = (pdf_size_t)utf16be_strings[i].size - 2;
      if (!pdf_text_test_big_endian_system ())
        {
          input_free = PDF_TRUE;
          input_data = pdf_text_test_change_utf16_endianness ((pdf_char_t *) &utf16be_strings[i].data[2],
                                                              input_size);
          /* Just in case... */
          fail_unless (input_data != NULL);
        }
      else
        {
          input_data = (pdf_char_t *)&utf16be_strings[i].data[2];
        }

      /* Set expected data */
      expected_data = (pdf_char_t *)utf16be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf16be_strings[i].utf32be_size;
      if (!pdf_text_test_big_endian_system ())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness (expected_data,
                                                                 expected_size);
          /* Just in case... */
          fail_unless (expected_data != NULL);
        }

      /* 1. The call to pdf_text_new_from_unicode should return PDF_OK. */
      text = pdf_text_new_from_unicode (input_data,
                                        input_size,
                                        PDF_TEXT_UTF16_HE,
                                        &error);

      /* 2. The function should return a valid pointer to the new text object */
      fail_unless (text != NULL);
      fail_if (error != NULL);

      PRINT_CONTENTS (i, text, expected_data, expected_size, 0);

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

      if (expected_free)
        pdf_dealloc (expected_data);
      if (input_free)
        pdf_dealloc (input_data);
      pdf_dealloc (actual_data);

      ++i;
    }
}
END_TEST


/*
 * Test: pdf_text_new_from_unicode_005
 * Description:
 *   Create a text object with an input valid UTF-32BE encoded string
 *    without BOM
 * Success conditions:
 *   1. The call to pdf_text_new_from_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 *   3. The function should return a valid pointer to the new text object
 *   4. The language code within the text object must be empty.
 *   5. The country code within the text object must be empty.
 */
START_TEST (pdf_text_new_from_unicode_005)
{
  extern const test_string_t utf32be_strings[];
  int i;

  i = 0;
  while (utf32be_strings[i].data)
    {
      pdf_error_t *error = NULL;
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data;
      const pdf_char_t *input_data;
      pdf_size_t input_size;
      pdf_size_t expected_size;
      pdf_char_t *actual_data;
      pdf_size_t actual_size;
      pdf_text_t *text;

      /* Set input data */
      input_size = (pdf_size_t)utf32be_strings[i].size -4;
      input_data = (pdf_char_t *)&utf32be_strings[i].data[4];

      /* Set expected data */
      expected_data = (pdf_char_t *)utf32be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf32be_strings[i].utf32be_size;
      if (!pdf_text_test_big_endian_system ())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness (expected_data,
                                                                 expected_size);
          /* Just in case... */
          fail_unless (expected_data != NULL);
        }

      /* 1. The call to pdf_text_new_from_unicode should return PDF_OK. */
      text = pdf_text_new_from_unicode (input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_BE,
                                        &error);

      /* 2. The function should return a valid pointer to the new text object */
      fail_unless (text != NULL);
      fail_if (error != NULL);

      PRINT_CONTENTS (i, text, expected_data, expected_size, 0);

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

      if (expected_free)
        pdf_dealloc (expected_data);
      pdf_dealloc (actual_data);

      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_new_from_unicode_006
 * Description:
 *   Create a text object with an input valid UTF-32LE encoded string
 *    without BOM
 * Success conditions:
 *   1. The call to pdf_text_new_from_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 *   3. The function should return a valid pointer to the new text object
 *   4. The language code within the text object must be empty.
 *   5. The country code within the text object must be empty.
 */
START_TEST (pdf_text_new_from_unicode_006)
{
  extern const test_string_t utf32be_strings[];
  int i;

  i = 0;
  while (utf32be_strings[i].data)
    {
      pdf_error_t *error = NULL;
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data;
      pdf_char_t *input_data;
      pdf_size_t input_size;
      pdf_char_t *actual_data;
      pdf_size_t actual_size;
      pdf_size_t expected_size;
      pdf_text_t *text;

      /* Set input data */
      input_size = (pdf_size_t)utf32be_strings[i].size -4;
      input_data = pdf_text_test_change_utf32_endianness ((pdf_char_t *) &utf32be_strings[i].data[4],
                                                          input_size);

      /* Set expected data */
      expected_data = (pdf_char_t *)utf32be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf32be_strings[i].utf32be_size;
      if (!pdf_text_test_big_endian_system ())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness (expected_data,
                                                                 expected_size);
          /* Just in case... */
          fail_unless (expected_data != NULL);
        }

      /* 1. The call to pdf_text_new_from_unicode should return PDF_OK. */
      text = pdf_text_new_from_unicode (input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_LE,
                                        &error);

      /* 2. The function should return a valid pointer to the new text object */
      fail_unless (text != NULL);
      fail_if (error != NULL);

      PRINT_CONTENTS (i, text, expected_data, expected_size, 0);

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

      if (expected_free)
        pdf_dealloc (expected_data);
      pdf_dealloc (actual_data);

      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_new_from_unicode_007
 * Description:
 *   Create a text object with an input valid UTF-32HE encoded string
 *    without BOM
 * Success conditions:
 *   1. The call to pdf_text_new_from_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 *   3. The function should return a valid pointer to the new text object
 *   4. The language code within the text object must be empty.
 *   5. The country code within the text object must be empty.
 */
START_TEST (pdf_text_new_from_unicode_007)
{
  extern const test_string_t utf32be_strings[];
  int i;

  i = 0;
  while (utf32be_strings[i].data)
    {
      pdf_error_t *error = NULL;
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_bool_t input_free = PDF_FALSE;
      pdf_char_t *expected_data;
      pdf_char_t *input_data;
      pdf_size_t input_size;
      pdf_size_t expected_size;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      pdf_text_t *text;

      /* Set input data (UTF-32HE) */
      input_size = (pdf_size_t)utf32be_strings[i].size - 4;
      if (!pdf_text_test_big_endian_system ())
        {
          input_free = PDF_TRUE;
          input_data = pdf_text_test_change_utf32_endianness ((pdf_char_t *) &utf32be_strings[i].data[4],
                                                              input_size);
          /* Just in case... */
          fail_unless (input_data != NULL);
        }
      else
        {
          input_data = (pdf_char_t *)&utf32be_strings[i].data[4];
        }

      /* Set expected data */
      expected_data = (pdf_char_t *)utf32be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf32be_strings[i].utf32be_size;
      if (!pdf_text_test_big_endian_system ())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness (expected_data,
                                                                 expected_size);
          /* Just in case... */
          fail_unless (expected_data != NULL);
        }

      /* 1. The call to pdf_text_new_from_unicode should return PDF_OK. */
      text = pdf_text_new_from_unicode (input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_HE,
                                        &error);

      /* 2. The function should return a valid pointer to the new text object */
      fail_unless (text != NULL);
      fail_if (error != NULL);

      PRINT_CONTENTS (i, text, expected_data, expected_size, 0);

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

      if (expected_free)
        pdf_dealloc (expected_data);
      if (input_free)
        pdf_dealloc(input_data);
      pdf_dealloc (actual_data);

      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_new_from_unicode_008
 * Description:
 *   Create a text object with an input valid UTF-8 encoded string
 *   (containing 8-bit, 16-bit, 24-bit and 32-bit code points!) with BOM
 * Success conditions:
 *   1. The call to @code{pdf_text_new_from_unicode} should return PDF_OK.
 *   2. The function should return a valid pointer to the new text object
 *   3. The contents of the text object must be the expected ones (without BOM).
 *   4. The language code within the text object must be empty.
 *   5. The country code within the text object must be empty.
 */
START_TEST (pdf_text_new_from_unicode_008)
{
  extern const test_string_t utf8_strings[];
  int i;

  i = 0;
  while (utf8_strings[i].data)
    {
      pdf_error_t *error = NULL;
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data;
      const pdf_char_t *input_data;
      pdf_size_t input_size;
      pdf_size_t expected_size;
      pdf_char_t *actual_data;
      pdf_size_t actual_size;
      pdf_text_t *text;

      /* Set input data */
      input_data = (pdf_char_t *)&(utf8_strings[i].data[0]);
      input_size = (pdf_size_t)utf8_strings[i].size;

      /* Set expected data */
      expected_data = (pdf_char_t *)utf8_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf8_strings[i].utf32be_size;
      if (!pdf_text_test_big_endian_system ())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness (expected_data,
                                                                 expected_size);
          /* Just in case... */
          fail_unless (expected_data != NULL);
        }

      /* 1. The call to pdf_text_new_from_unicode should return PDF_OK. */
      text = pdf_text_new_from_unicode (input_data,
                                        input_size,
                                        PDF_TEXT_UTF8,
                                        &error);

      /* 2. The function should return a valid pointer to the new text object */
      fail_unless (text != NULL);
      fail_if (error != NULL);

      PRINT_CONTENTS (i, text, expected_data, expected_size, 0);

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

      if (expected_free)
        pdf_dealloc (expected_data);
      pdf_dealloc (actual_data);

      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_new_from_unicode_009
 * Description:
 *   Create a text object with an input valid UTF-16BE encoded string
 *    (16-bit and 32-bit code points!) with BOM
 * Success conditions:
 *   1. The call to pdf_text_new_from_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 *   3. The function should return a valid pointer to the new text object
 *   4. The language code within the text object must be empty.
 *   5. The country code within the text object must be empty.
 */
START_TEST (pdf_text_new_from_unicode_009)
{
  extern const test_string_t utf16be_strings[];
  int i;

  i = 0;
  while (utf16be_strings[i].data)
    {
      pdf_error_t *error = NULL;
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data;
      const pdf_char_t *input_data;
      pdf_size_t input_size;
      pdf_size_t expected_size;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      pdf_text_t *text;

      /* Set input data */
      input_data = (pdf_char_t *)&(utf16be_strings[i].data[0]);
      input_size = (pdf_size_t)utf16be_strings[i].size;

      /* Set expected data */
      expected_data = (pdf_char_t *)utf16be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf16be_strings[i].utf32be_size;
      if (!pdf_text_test_big_endian_system ())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness (expected_data,
                                                                 expected_size);
          /* Just in case... */
          fail_unless (expected_data != NULL);
        }

      /* 1. The call to pdf_text_new_from_unicode should return PDF_OK. */
      text = pdf_text_new_from_unicode (input_data,
                                        input_size,
                                        PDF_TEXT_UTF16_BE,
                                        &error);

      /* 2. The function should return a valid pointer to the new text object */
      fail_unless (text != NULL);
      fail_if (error != NULL);

      PRINT_CONTENTS (i, text, expected_data, expected_size, 0);

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

      if (expected_free)
        pdf_dealloc (expected_data);
      pdf_dealloc (actual_data);

      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_new_from_unicode_010
 * Description:
 *   Create a text object with an input valid UTF-16LE encoded string
 *    (16-bit and 32-bit code points!) with BOM
 * Success conditions:
 *   1. The call to pdf_text_new_from_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 *   3. The function should return a valid pointer to the new text object
 *   4. The language code within the text object must be empty.
 *   5. The country code within the text object must be empty.
 */
START_TEST (pdf_text_new_from_unicode_010)
{
  extern const test_string_t utf16be_strings[];
  int i;

  i = 0;
  while (utf16be_strings[i].data)
    {
      pdf_error_t *error = NULL;
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data;
      pdf_char_t *input_data;
      pdf_size_t input_size;
      pdf_size_t expected_size;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      pdf_text_t *text;

      /* Set input data (change endiannes to input UTF-16BE string) */
      input_size = (pdf_size_t)utf16be_strings[i].size;
      input_data = pdf_text_test_change_utf16_endianness ((pdf_char_t *) &utf16be_strings[i].data[0],
                                                          input_size);

      /* Set expected data */
      expected_data = (pdf_char_t *)utf16be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf16be_strings[i].utf32be_size;
      if (!pdf_text_test_big_endian_system ())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness (expected_data,
                                                                 expected_size);
          /* Just in case... */
          fail_unless (expected_data != NULL);
        }

      /* 1. The call to pdf_text_new_from_unicode should return PDF_OK. */
      text = pdf_text_new_from_unicode (input_data,
                                        input_size,
                                        PDF_TEXT_UTF16_LE,
                                        &error);

      /* 2. The function should return a valid pointer to the new text object */
      fail_unless (text != NULL);
      fail_if (error != NULL);

      PRINT_CONTENTS (i, text, expected_data, expected_size, 0);

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

      if (expected_free)
        pdf_dealloc (expected_data);
      pdf_dealloc (actual_data);

      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_new_from_unicode_011
 * Description:
 *   Create a text object with an input valid UTF-16HE encoded string
 *    (16-bit and 32-bit code points!) with BOM
 * Success conditions:
 *   1. The call to pdf_text_new_from_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 *   3. The function should return a valid pointer to the new text object
 *   4. The language code within the text object must be empty.
 *   5. The country code within the text object must be empty.
 */
START_TEST (pdf_text_new_from_unicode_011)
{
  extern const test_string_t utf16be_strings[];
  int i;

  i = 0;
  while (utf16be_strings[i].data)
    {
      pdf_error_t *error = NULL;
      pdf_bool_t input_free = PDF_FALSE;
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data;
      pdf_char_t *input_data;
      pdf_size_t input_size;
      pdf_size_t expected_size;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      pdf_text_t *text;

      /* Set input data (change endiannes to input UTF-16BE string if required)
       */
      input_size = (pdf_size_t)utf16be_strings[i].size;
      if (!pdf_text_test_big_endian_system ())
        {
          input_free = PDF_TRUE;
          input_data = pdf_text_test_change_utf16_endianness ((pdf_char_t *) &utf16be_strings[i].data[0],
                                                              input_size);
          /* Just in case... */
          fail_unless (input_data != NULL);
        }
      else
        {
          input_data = (pdf_char_t *)&utf16be_strings[i].data[0];
        }

      /* Set expected data */
      expected_data = (pdf_char_t *)utf16be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf16be_strings[i].utf32be_size;
      if (!pdf_text_test_big_endian_system ())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness (expected_data,
                                                                 expected_size);
          /* Just in case... */
          fail_unless (expected_data != NULL);
        }

      /* 1. The call to pdf_text_new_from_unicode should return PDF_OK. */
      text = pdf_text_new_from_unicode (input_data,
                                        input_size,
                                        PDF_TEXT_UTF16_HE,
                                        &error);

      /* 2. The function should return a valid pointer to the new text object */
      fail_unless (text != NULL);
      fail_if (error != NULL);

      PRINT_CONTENTS (i, text, expected_data, expected_size, 0);

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

      if (expected_free)
        pdf_dealloc (expected_data);
      if (input_free)
        pdf_dealloc (input_data);
      pdf_dealloc (actual_data);

      ++i;
    }
}
END_TEST


/*
 * Test: pdf_text_new_from_unicode_012
 * Description:
 *   Create a text object with an input valid UTF-32BE encoded string
 *    with BOM
 * Success conditions:
 *   1. The call to pdf_text_new_from_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 *   3. The function should return a valid pointer to the new text object
 *   4. The language code within the text object must be empty.
 *   5. The country code within the text object must be empty.
 */
START_TEST (pdf_text_new_from_unicode_012)
{
  extern const test_string_t utf32be_strings[];
  int i;

  i = 0;
  while (utf32be_strings[i].data)
    {
      pdf_error_t *error = NULL;
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data;
      const pdf_char_t *input_data;
      pdf_size_t input_size;
      pdf_size_t expected_size;
      pdf_char_t *actual_data;
      pdf_size_t actual_size;
      pdf_text_t *text;

      /* Set input data */
      input_size = (pdf_size_t)utf32be_strings[i].size;
      input_data = (pdf_char_t *)&utf32be_strings[i].data[0];

      /* Set expected data */
      expected_data = (pdf_char_t *)utf32be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf32be_strings[i].utf32be_size;
      if (!pdf_text_test_big_endian_system ())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness (expected_data,
                                                                 expected_size);
          /* Just in case... */
          fail_unless (expected_data != NULL);
        }

      /* 1. The call to pdf_text_new_from_unicode should return PDF_OK. */
      text = pdf_text_new_from_unicode (input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_BE,
                                        &error);

      /* 2. The function should return a valid pointer to the new text object */
      fail_unless (text != NULL);
      fail_if (error != NULL);

      PRINT_CONTENTS (i, text, expected_data, expected_size, 0);

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

      if (expected_free)
        pdf_dealloc (expected_data);
      pdf_dealloc (actual_data);

      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_new_from_unicode_013
 * Description:
 *   Create a text object with an input valid UTF-32LE encoded string
 *    with BOM
 * Success conditions:
 *   1. The call to pdf_text_new_from_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 *   3. The function should return a valid pointer to the new text object
 *   4. The language code within the text object must be empty.
 *   5. The country code within the text object must be empty.
 */
START_TEST (pdf_text_new_from_unicode_013)
{
  extern const test_string_t utf32be_strings[];
  int i;

  i = 0;
  while (utf32be_strings[i].data)
    {
      pdf_error_t *error = NULL;
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data;
      pdf_char_t *input_data;
      pdf_size_t input_size;
      pdf_char_t *actual_data;
      pdf_size_t actual_size;
      pdf_size_t expected_size;
      pdf_text_t *text;

      /* Set input data */
      input_size = (pdf_size_t)utf32be_strings[i].size;
      input_data = pdf_text_test_change_utf32_endianness ((pdf_char_t *) &utf32be_strings[i].data[0],
                                                          input_size);

      /* Set expected data */
      expected_data = (pdf_char_t *)utf32be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf32be_strings[i].utf32be_size;
      if (!pdf_text_test_big_endian_system ())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness (expected_data,
                                                                 expected_size);
          /* Just in case... */
          fail_unless (expected_data != NULL);
        }

      /* 1. The call to pdf_text_new_from_unicode should return PDF_OK. */
      text = pdf_text_new_from_unicode (input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_LE,
                                        &error);

      /* 2. The function should return a valid pointer to the new text object */
      fail_unless (text != NULL);
      fail_if (error != NULL);

      PRINT_CONTENTS (i, text, expected_data, expected_size, 0);

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

      if (expected_free)
        pdf_dealloc (expected_data);
      pdf_dealloc (actual_data);

      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_new_from_unicode_014
 * Description:
 *   Create a text object with an input valid UTF-32HE encoded string
 *    with BOM
 * Success conditions:
 *   1. The call to pdf_text_new_from_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 *   3. The function should return a valid pointer to the new text object
 *   4. The language code within the text object must be empty.
 *   5. The country code within the text object must be empty.
 */
START_TEST (pdf_text_new_from_unicode_014)
{
  extern const test_string_t utf32be_strings[];
  int i;

  i = 0;
  while (utf32be_strings[i].data)
    {
      pdf_error_t *error = NULL;
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_bool_t input_free = PDF_FALSE;
      pdf_char_t *expected_data;
      pdf_char_t *input_data;
      pdf_size_t input_size;
      pdf_size_t expected_size;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      pdf_text_t *text;

      /* Set input data (UTF-32HE) */
      input_size = (pdf_size_t)utf32be_strings[i].size;
      if (!pdf_text_test_big_endian_system ())
        {
          input_free = PDF_TRUE;
          input_data = pdf_text_test_change_utf32_endianness ((pdf_char_t *) &utf32be_strings[i].data[0],
                                                              input_size);
          /* Just in case... */
          fail_unless (input_data != NULL);
        }
      else
        {
          input_data = (pdf_char_t *)&utf32be_strings[i].data[0];
        }

      /* Set expected data */
      expected_data = (pdf_char_t *)utf32be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf32be_strings[i].utf32be_size;
      if (!pdf_text_test_big_endian_system ())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness (expected_data,
                                                                 expected_size);
          /* Just in case... */
          fail_unless (expected_data != NULL);
        }

      /* 1. The call to pdf_text_new_from_unicode should return PDF_OK. */
      text = pdf_text_new_from_unicode (input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_HE,
                                        &error);

      /* 2. The function should return a valid pointer to the new text object */
      fail_unless (text != NULL);
      fail_if (error != NULL);

      PRINT_CONTENTS (i, text, expected_data, expected_size, 0);

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

      if (expected_free)
        pdf_dealloc (expected_data);
      if (input_free)
        pdf_dealloc(input_data);
      pdf_dealloc (actual_data);

      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_new_from_unicode_015
 * Description:
 *   Create a text object with an input invalid UTF-8 encoded string without BOM
 * Success conditions:
 *   1. The call to pdf_text_new_from_unicode should NOT return PDF_OK.
 *   2. Pointer to the text object must remain unchanged.
 */
START_TEST (pdf_text_new_from_unicode_015)
{
  extern const test_string_t utf16be_strings[];
  extern const test_string_t utf32be_strings[];
  int i;

  /* Try UTF-16BE strings */
  i = 0;
  while (utf16be_strings[i].data)
    {
      pdf_text_t *text;
      pdf_char_t *input_data;
      pdf_size_t input_size;
      pdf_error_t *error = NULL;

      input_data = (pdf_char_t *)utf16be_strings[i].data;
      input_size = (pdf_size_t)utf16be_strings[i].size;

      /* 1. The call to  pdf_text_new_from_host should NOT return PDF_OK. */
      text = pdf_text_new_from_unicode (input_data,
                                        input_size,
                                        PDF_TEXT_UTF8,
                                        &error);

      /* 2. Pointer to the text object must remain unchanged. */
      fail_if (text != NULL);
      fail_unless (error != NULL);

      pdf_error_destroy (error);

      ++i;
    }

  /* Try UTF-32BE strings */
  i = 0;
  while (utf32be_strings[i].data)
    {
      pdf_text_t *text;
      pdf_char_t *input_data;
      pdf_size_t input_size;
      pdf_error_t *error = NULL;

      input_data = (pdf_char_t *)utf32be_strings[i].data;
      input_size = (pdf_size_t)utf32be_strings[i].size;

      /* 1. The call to  pdf_text_new_from_host should NOT return PDF_OK. */
      text = pdf_text_new_from_unicode (input_data,
                                        input_size,
                                        PDF_TEXT_UTF8,
                                        &error);

      /* 2. Pointer to the text object must remain unchanged. */
      fail_if (text != NULL);
      fail_unless (error != NULL);

      pdf_error_destroy (error);

      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_new_from_unicode_016
 * Description:
 *   Create a text object with an input invalid UTF-16BE encoded string without
 *   BOM
 * Success conditions:
 *   1. The call to pdf_text_new_from_unicode should NOT return PDF_OK.
 *   2. Pointer to the text object must remain unchanged.
 */
START_TEST (pdf_text_new_from_unicode_016)
{
  extern const test_string_t utf8_strings[];
  int i;

  /* Try UTF-8 strings */
  i = 0;
  while (utf8_strings[i].data)
    {
      pdf_error_t *error = NULL;
      pdf_text_t *text;
      pdf_char_t *input_data;
      pdf_size_t input_size;

      input_data = (pdf_char_t *)utf8_strings[i].data;
      input_size = (pdf_size_t)utf8_strings[i].size;

      /* 1. The call to  pdf_text_new_from_host should NOT return PDF_OK. */
      text = pdf_text_new_from_unicode (input_data,
                                        input_size,
                                        PDF_TEXT_UTF16_BE,
                                        &error);

      /* 2. Pointer to the text object must remain unchanged. */
      fail_if (text != NULL);
      fail_unless (error != NULL);

      pdf_error_destroy (error);

      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_new_from_unicode_017
 * Description:
 *   Create a text object with an input invalid UTF-16LE encoded string without
 *   BOM
 * Success conditions:
 *   1. The call to pdf_text_new_from_unicode should NOT return PDF_OK.
 *   2. Pointer to the text object must remain unchanged.
 */
START_TEST (pdf_text_new_from_unicode_017)
{
  extern const test_string_t utf8_strings[];
  int i;

  /* Try UTF-8 strings */
  i = 0;
  while (utf8_strings[i].data)
    {
      pdf_text_t *text;
      pdf_char_t *input_data;
      pdf_size_t input_size;
      pdf_error_t *error = NULL;

      input_data = (pdf_char_t *)utf8_strings[i].data;
      input_size = (pdf_size_t)utf8_strings[i].size;

      /* 1. The call to  pdf_text_new_from_host should NOT return PDF_OK. */
      text = pdf_text_new_from_unicode (input_data,
                                        input_size,
                                        PDF_TEXT_UTF16_LE,
                                        &error);

      /* 2. Pointer to the text object must remain unchanged. */
      fail_if (text != NULL);
      fail_unless (error != NULL);

      pdf_error_destroy (error);

      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_new_from_unicode_018
 * Description:
 *   Create a text object with an input invalid UTF-16HE encoded string without
 *   BOM
 * Success conditions:
 *   1. The call to pdf_text_new_from_unicode should NOT return PDF_OK.
 *   2. Pointer to the text object must remain unchanged.
 */
START_TEST (pdf_text_new_from_unicode_018)
{
  extern const test_string_t utf8_strings[];
  int i;

  /* Try UTF-8 strings */
  i = 0;
  while (utf8_strings[i].data)
    {
      pdf_text_t *text;
      pdf_char_t *input_data;
      pdf_size_t input_size;
      pdf_error_t *error = NULL;

      input_data = (pdf_char_t *)utf8_strings[i].data;
      input_size = (pdf_size_t)utf8_strings[i].size;

      /* 1. The call to  pdf_text_new_from_host should NOT return PDF_OK. */
      text = pdf_text_new_from_unicode (input_data,
                                        input_size,
                                        PDF_TEXT_UTF16_HE,
                                        &error);

      /* 2. Pointer to the text object must remain unchanged. */
      fail_if (text != NULL);
      fail_unless (error != NULL);

      pdf_error_destroy (error);

      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_new_from_unicode_019
 * Description:
 *   Create a text object with an input invalid UTF-32BE encoded string without
 *   BOM
 * Success conditions:
 *   1. The call to pdf_text_new_from_unicode should NOT return PDF_OK.
 *   2. Pointer to the text object must remain unchanged.
 */
START_TEST (pdf_text_new_from_unicode_019)
{
  extern const test_string_t utf8_strings[];
  int i;

  /* Try UTF-8 strings */
  i = 0;
  while (utf8_strings[i].data)
    {
      pdf_text_t *text;
      pdf_char_t *input_data;
      pdf_size_t input_size;
      pdf_error_t *error = NULL;

      input_data = (pdf_char_t *)utf8_strings[i].data;
      input_size = (pdf_size_t)utf8_strings[i].size;

      /* 1. The call to  pdf_text_new_from_host should NOT return PDF_OK. */
      text = pdf_text_new_from_unicode (input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_BE,
                                        &error);

      /* 2. Pointer to the text object must remain unchanged. */
      fail_if (text != NULL);
      fail_unless (error != NULL);

      pdf_error_destroy (error);

      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_new_from_unicode_020
 * Description:
 *   Create a text object with an input invalid UTF-32LE encoded string without
 *   BOM
 * Success conditions:
 *   1. The call to pdf_text_new_from_unicode should NOT return PDF_OK.
 *   2. Pointer to the text object must remain unchanged.
 */
START_TEST (pdf_text_new_from_unicode_020)
{
  extern const test_string_t utf8_strings[];
  int i;

  /* Try UTF-8 strings */
  i = 0;
  while (utf8_strings[i].data)
    {
      pdf_text_t *text;
      pdf_char_t *input_data;
      pdf_size_t input_size;
      pdf_error_t *error = NULL;

      input_data = (pdf_char_t *)utf8_strings[i].data;
      input_size = (pdf_size_t)utf8_strings[i].size;

      /* 1. The call to  pdf_text_new_from_host should NOT return PDF_OK. */
      text = pdf_text_new_from_unicode (input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_LE,
                                        &error);

      /* 2. Pointer to the text object must remain unchanged. */
      fail_if (text != NULL);
      fail_unless (error != NULL);

      pdf_error_destroy (error);

      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_new_from_unicode_021
 * Description:
 *   Create a text object with an input invalid UTF-32HE encoded string without
 *   BOM
 * Success conditions:
 *   1. The call to pdf_text_new_from_unicode should NOT return PDF_OK.
 *   2. Pointer to the text object must remain unchanged.
 */
START_TEST (pdf_text_new_from_unicode_021)
{
  extern const test_string_t utf8_strings[];
  int i;

  /* Try UTF-8 strings */
  i = 0;
  while (utf8_strings[i].data)
    {
      pdf_text_t *text;
      pdf_char_t *input_data;
      pdf_size_t input_size;
      pdf_error_t *error = NULL;

      input_data = (pdf_char_t *)utf8_strings[i].data;
      input_size = (pdf_size_t)utf8_strings[i].size;

      /* 1. The call to  pdf_text_new_from_host should NOT return PDF_OK. */
      text = pdf_text_new_from_unicode (input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_HE,
                                        &error);

      /* 2. Pointer to the text object must remain unchanged. */
      fail_if (text != NULL);
      fail_unless (error != NULL);

      pdf_error_destroy (error);

      ++i;
    }
}
END_TEST

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
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-text-new-from-unicode.c */

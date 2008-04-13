/* -*- mode: C -*- Time-stamp: ""
 *
 *       File:         pdf-text-filter.c
 *       Date:         Thu Mar 27 20:25:17 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_filter test case
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
#include <base/text/pdf-text-test-common.h>

#define INTERACTIVE_DEBUG 0


/*
 * Test: pdf_text_filter_001
 * Description:
 *   Apply the `Normalize line endings' filter to a text object which contains
 *   different types of line endings.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones. 
 */
START_TEST(pdf_text_filter_001)
{
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "GNU's Not Unix" "\r""\n" \
                                                "GNU's Not Unix" "\r" \
                                                "GNU's Not Unix" "\n" \
                                                "GNU's Not Unix" "\xC2\x85";
  const pdf_char_t *expected_data = (pdf_char_t *) "GNU's Not Unix" "\n" \
                                                   "GNU's Not Unix" "\n" \
                                                   "GNU's Not Unix" "\n" \
                                                   "GNU's Not Unix" "\n";
  pdf_size_t input_size;
  pdf_size_t expected_size;
  pdf_char_t *output_data;
  pdf_size_t output_size;

  input_size = strlen((char *)input_data);
  expected_size = strlen((char *)expected_data);
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);

      
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF8) != PDF_OK);
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_001:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LINE_ENDINGS) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_001:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF8,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);

}
END_TEST


/*
 * Test: pdf_text_filter_002
 * Description:
 *   Apply the `Normalize line endings' filter to an empty text object.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must remain unchanged. 
 */
START_TEST(pdf_text_filter_002)
{
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "";
  const pdf_char_t *expected_data = input_data;
  pdf_size_t input_size = 0;
  pdf_size_t expected_size = 0;
  pdf_char_t *output_data = NULL;
  pdf_size_t output_size = 0;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF8) != PDF_OK);
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_002:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LINE_ENDINGS) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_002:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF8,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must remain unchanged. */
  fail_unless(output_size == expected_size);
  fail_unless(output_data == NULL);
  
}
END_TEST


/*
 * Test: pdf_text_filter_003
 * Description:
 *   Apply the `Remove line endings' filter to a text object which contains
 *   different types of line endings.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones. 
 */
START_TEST(pdf_text_filter_003)
{
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "GNU's Not Unix. " "\r""\n" \
                                                "GNU's Not Unix. " "\r" \
                                                "GNU's Not Unix. " "\n" \
                                                "GNU's Not Unix. " "\xC2\x85";
  const pdf_char_t *expected_data = (pdf_char_t *) "GNU's Not Unix. " \
                                                   "GNU's Not Unix. " \
                                                   "GNU's Not Unix. " \
                                                   "GNU's Not Unix. ";
  pdf_size_t input_size;
  pdf_size_t expected_size;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  input_size = strlen((char *)input_data);
  expected_size = strlen((char *)expected_data);
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF8) != PDF_OK);
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_003:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text,
                              PDF_TEXT_FILTER_REMOVE_LINE_ENDINGS) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_003:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF8,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_004
 * Description:
 *   Apply the `Remove line endings' filter to an empty text object.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must remain unchanged. 
 */
START_TEST(pdf_text_filter_004)
{
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "";
  const pdf_char_t *expected_data = input_data;
  pdf_size_t input_size = 0;
  pdf_size_t expected_size = 0;
  pdf_char_t *output_data = NULL;
  pdf_size_t output_size = 0;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF8) != PDF_OK);
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_004:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text,
                              PDF_TEXT_FILTER_REMOVE_LINE_ENDINGS) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_004:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF8,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must remain unchanged. */
  fail_unless(output_size == expected_size);
  fail_unless(output_data == NULL);
  
}
END_TEST


/*
 * Test: pdf_text_filter_005
 * Description:
 *   Apply the `Remove ampersands' filter to a text object which contains
 *   single and double ampersands.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones. 
 */
START_TEST(pdf_text_filter_005)
{
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "GNU & GNU && GNU";
  const pdf_char_t *expected_data = (pdf_char_t *) "GNU GNU & GNU";
  pdf_size_t input_size;
  pdf_size_t expected_size;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  input_size = strlen((char *)input_data);
  expected_size = strlen((char *)expected_data);
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF8) != PDF_OK);
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_005:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_REMOVE_AMP) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_005:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF8,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_006
 * Description:
 *   Apply the `Remove ampersands' filter to an empty text object.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must remain unchanged. 
 */
START_TEST(pdf_text_filter_006)
{
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "";
  const pdf_char_t *expected_data = input_data;
  pdf_size_t input_size = 0;
  pdf_size_t expected_size = 0;
  pdf_char_t *output_data = NULL;
  pdf_size_t output_size = 0;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF8) != PDF_OK);
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_006:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_REMOVE_AMP) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_006:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF8,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must remain unchanged. */
  fail_unless(output_size == expected_size);
  fail_unless(output_data == NULL);
  
}
END_TEST



/*
 * Test: pdf_text_filter_007
 * Description:
 *   Apply the `Normalize with full width' filter to a text object that contains
 *   code points that have a valid full width representation.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones. 
 */
START_TEST(pdf_text_filter_007)
{
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00"" " \
                                                "\x00\x00\x00""G" \
                                                "\x00\x00\x00""N" \
                                                "\x00\x00\x00""U" \
                                                "\x00\x00\x00"" ";
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00"" " \
                                                "\x00\x00\xFF\x27" \
                                                "\x00\x00\xFF\x2E" \
                                                "\x00\x00\xFF\x35" \
                                                "\x00\x00\x00"" ";
  pdf_size_t input_size = 20;
  pdf_size_t expected_size = 20;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_007:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text,
                              PDF_TEXT_FILTER_NORM_WITH_FULL_WIDTH) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_007:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_008
 * Description:
 *   Apply the `Normalize with full width' filter to a text object that contains
 *   code points that have a valid full width representation.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones. 
 */
START_TEST(pdf_text_filter_008)
{
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x6C\x34" \
                                                "\x00\x00\x00"" " \
                                                "\x00\x01\xD1\x1E";
  const pdf_char_t *expected_data = input_data;
  pdf_size_t input_size = 16;
  pdf_size_t expected_size = input_size;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_007:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text,
                              PDF_TEXT_FILTER_NORM_WITH_FULL_WIDTH) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_007:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_009
 * Description:
 *   Apply the `Normalize with full width' filter to an empty text object.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must remain unchanged. 
 */
START_TEST(pdf_text_filter_009)
{
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "";
  const pdf_char_t *expected_data = input_data;
  pdf_size_t input_size = 0;
  pdf_size_t expected_size = 0;
  pdf_char_t *output_data = NULL;
  pdf_size_t output_size = 0;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF8) != PDF_OK);
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_006:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, 
                              PDF_TEXT_FILTER_NORM_WITH_FULL_WIDTH) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_006:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF8,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must remain unchanged. */
  fail_unless(output_size == expected_size);
  fail_unless(output_data == NULL);
  
}
END_TEST


/*
 * Test: pdf_text_filter_010
 * Description:
 *   Apply the `Upper case' filter to a text object that contains
 *   simple-case-conversion characters.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 *   3. The length of the output text must be equal to the length of the
 *      input text.
 */
START_TEST(pdf_text_filter_010)
{
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00"" " \
                                                "\x00\x00\x00""g" \
                                                "\x00\x00\x00""n" \
                                                "\x00\x00\x00""u" \
                                                "\x00\x00\x00"" ";
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00"" " \
                                                   "\x00\x00\x00""G" \
                                                   "\x00\x00\x00""N" \
                                                   "\x00\x00\x00""U" \
                                                   "\x00\x00\x00"" ";
  pdf_size_t input_size = 20;
  pdf_size_t expected_size = input_size;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_010:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_010:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 3. The length of the output text must be equal to the length of the
   *      input text. */
  fail_unless(output_size == expected_size);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_011
 * Description:
 *   Apply the `Upper case' filter to a text object that contains
 *   special-case-conversion characters with no context condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_011)
{
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x1E\x98" \
                                                "\x00\x00\x1F\xE4" \
                                                "\x00\x00\x1F\x80" \
                                                "\x00\x00\x1F\xC4";
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                          "\x00\x00\x00\x57\x00\x00\x03\x0A" \
                                          "\x00\x00\x03\xA1\x00\x00\x03\x13" \
                                          "\x00\x00\x1F\x08\x00\x00\x03\x99" \
                                          "\x00\x00\x03\x89\x00\x00\x03\x99";
  pdf_size_t input_size = 20;
  pdf_size_t expected_size = 36;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_011:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_011:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_012
 * Description:
 *   Apply the `Upper case' filter to a text object that contains
 *   special-case-conversion characters with Final_Sigma context condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones. 
 */
START_TEST(pdf_text_filter_012)
{
  /* Final_Sigma condition in Special Case algorithms
   *  When the given character is preceded by a sequence consisting of a
   *  cased unicode point (with case information) and a case-ignorable
   *  (without case information) sequence of points. The given character
   *  must no be followed by a sequence consisting of a case-ignorable
   *  sequence and a cased letter.
   *
   * - 0x03A3 point is the one with the Final_Sigma condition
   * - 0x0055 point is the cased point
   * - 0x003A point is the case-ignorable point (MidLetter WB property)
   * - 0x0020 point (whitespace) is used as word separator to have three 
   *    different words.
   * If condition is fulfilled, expected uppercase character for 0x03A3 is
   *  the same 0x03A3.
   */
  
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);

  /* 1st check, the sequence of previous case-ignorable points is empty */
  if(1)
  {
    const pdf_char_t *input_data = (pdf_char_t *) \
                                   "\x00\x00\x00\x47" "\x00\x00\x00\x4E" \
                                   "\x00\x00\x00\x55" "\x00\x00\x00\x20" \
                                   "\x00\x00\x00\x55" \
                                   "\x00\x00\x03\xA3" "\x00\x00\x00\x20" \
                                   "\x00\x00\x00\x47" "\x00\x00\x00\x4E" \
                                   "\x00\x00\x00\x55";
    const pdf_char_t *expected_data = input_data;
    const pdf_size_t input_size = 40;
    pdf_char_t *output_data;
    pdf_size_t output_size;
    pdf_text_t text;

    /* Create text object with given contents */
    fail_if(pdf_text_new_from_unicode(&text,
                                      input_data,
                                      input_size,
                                      PDF_TEXT_UTF32_BE) != PDF_OK);
    
    /* 1. The call to  pdf_text_filter should return PDF_OK. */
    fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK);
    
    if(INTERACTIVE_DEBUG)
      {
        pdf_char_t *internal_hex = NULL;
        internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
        fail_if(internal_hex == NULL);
        printf("pdf_text_filter:12:1:Internal> '%s'\n", internal_hex);
        pdf_dealloc(internal_hex);
      }
    
    /* Get output as UTF-32BE */
    fail_unless(pdf_text_get_unicode(&output_data,
                                     &output_size,
                                     text,
                                     PDF_TEXT_UTF32_BE,
                                     PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
    
    /* 2. The contents of the output text object must be the expected ones. */
    fail_unless(output_size == input_size);
    fail_unless(memcmp(output_data, expected_data, input_size) == 0);
  }
  
  /* 2nd check, the sequence of previous case-ignorable points is NOT empty */
  if(1)
    {
      const pdf_char_t *input_data = (pdf_char_t *) \
                                     "\x00\x00\x00\x47" "\x00\x00\x00\x4E" \
                                     "\x00\x00\x00\x55" "\x00\x00\x00\x20" \
                                     "\x00\x00\x00\x55" "\x00\x00\x00\x3A" \
                                     "\x00\x00\x03\xA3" "\x00\x00\x00\x20" \
                                     "\x00\x00\x00\x47" "\x00\x00\x00\x4E" \
                                     "\x00\x00\x00\x55";
      const pdf_char_t *expected_data = input_data;
      const pdf_size_t input_size = 44;
      pdf_char_t *output_data;
      pdf_size_t output_size;
      pdf_text_t text;
      
      /* Create text object with given contents */
      fail_if(pdf_text_new_from_unicode(&text,
                                        input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_BE) != PDF_OK);
      
      /* 1. The call to  pdf_text_filter should return PDF_OK. */
      fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          internal_hex = pdf_text_test_get_hex(text->data,text->size,':');
          fail_if(internal_hex == NULL);
          printf("pdf_text_filter:12:2:Internal> '%s'\n", internal_hex);
          pdf_dealloc(internal_hex);
        }
      
      /* Get output as UTF-32BE */
      fail_unless(pdf_text_get_unicode(&output_data,
                                       &output_size,
                                       text,
                                       PDF_TEXT_UTF32_BE,
                                       PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
      
      /* 2. The contents of the output text object must be the expected ones. */
      fail_unless(output_size == input_size);
      fail_unless(memcmp(output_data, expected_data, input_size) == 0);
    }
}
END_TEST

/*
 * Test: pdf_text_filter_013
 * Description:
 *   Apply the `Upper case' filter to a text object that contains
 *   special-case-conversion characters with After_Soft_Dotted context
 *   condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_013)
{
  /* 0307; 0307; ; ; lt After_Soft_Dotted; # COMBINING DOT ABOVE */
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00" "i"
                                                "\x00\x00\x03\x07";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00" "I";
  pdf_size_t input_size = 12;
  pdf_size_t expected_size = 8;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'lt' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_013:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_013:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST

/*
 * Test: pdf_text_filter_014
 * Description:
 *   Apply the `Upper case' filter to a text object that contains
 *   special-case-conversion characters with More_Above context condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_014)
{
  /* Code:0049; Lower:0069 0307; Title:0049; Upper:0049;
   * lt More_Above; # LATIN CAPITAL LETTER I */

  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00\x49"
                                                "\x00\x00\x03\x0B";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00\x49"
                                                   "\x00\x00\x03\x0B";
  pdf_size_t input_size = 12;
  pdf_size_t expected_size = 12;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'lt' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_014:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_014:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_015
 * Description:
 *   Apply the `Upper case' filter to a text object that contains
 *   special-case-conversion characters with Before_Dot context condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_015)
{
  /* NOTE: Actually there is no special condition involving "Before_Dot" as is
   *       (there are a couple of conditions involving the negated one:
   *       Not_Before_Dot). But probably is a good idea to at least leave
   *       the test, just in case in future updates of the Unicode standard
   *       one of this type appears */
   fail_if(0);
}
END_TEST


/*
 * Test: pdf_text_filter_016
 * Description:
 *   Apply the `Upper case' filter to a text object that contains
 *   special-case-conversion characters with After_I context condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_016)
{
  /* Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""I" \
                                                "\x00\x00\x03\x07";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""I" \
                                                   "\x00\x00\x03\x07";
  pdf_size_t input_size = 12;
  pdf_size_t expected_size = 12;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_016:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_016:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_017
 * Description:
 *   Apply the `Upper case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent context
 *   condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_017)
{
  /* # When uppercasing, i turns into a dotted capital I
   * 0069; 0069; 0130; 0130; tr; # LATIN SMALL LETTER I */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00\x69";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x01\x30";
  pdf_size_t input_size = 8;
  pdf_size_t expected_size = 8;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_017:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_017:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_018
 * Description:
 *   Apply the `Upper case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent and After_I
 *    context condition (to check two context conditions at the same time).
 * Note: As long as After_I is always coming with a language condition (at least
 *   in the Unicode standard 5.0), this test is equal to pdf_text_filter_016
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_018)
{
  /* Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""I" \
                                                "\x00\x00\x03\x07";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""I" \
                                                   "\x00\x00\x03\x07";
  pdf_size_t input_size = 12;
  pdf_size_t expected_size = 12;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_016:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_016:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST



/*
 * Test: pdf_text_filter_019
 * Description:
 *   Apply the `Upper case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent and
 *   Not_Before_Dot context condition (to check two context conditions at the
 *   same time, where one of them is negated with Not_).
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_019)
{
  /* # When lowercasing, unless an I is before a dot_above, it turns into a
   *   dotless i.
   * 0049; 0131; 0049; 0049; tr Not_Before_Dot; # LATIN CAPITAL LETTER I */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00\x49" \
                                                "\x00\x00\x00\x50";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00\x49" \
                                                   "\x00\x00\x00\x50";
  pdf_size_t input_size = 12;
  pdf_size_t expected_size = 12;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_016:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_016:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);  
}
END_TEST

/*
 * Test: pdf_text_filter_020
 * Description:
 *   Apply the `Upper case' filter to a text object that contains
 *   special-case-conversion characters with Final_Sigma context condition,
 *   where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones. 
 */
START_TEST(pdf_text_filter_020)
{
  /* Final_Sigma condition in Special Case algorithms
   *  When the given character is preceded by a sequence consisting of a
   *  cased unicode point (with case information) and a case-ignorable
   *  (without case information) sequence of points. The given character
   *  must no be followed by a sequence consisting of a case-ignorable
   *  sequence and a cased letter.
   *
   * The condition won't be fulfilled because:
   *  1st check. The previous cased point is not available
   *  2nd check. A next cased point is available
   * 
   * - 0x03A3 point is the one with the Final_Sigma condition
   * - 0x0055 point is the cased point
   * - 0x0027 point is the case-ignorable point
   * - 0x0020 point (whitespace) is used as word separator to have three 
   *    different words.
   *
   * If condition is not fulfilled, expected uppercase character for 0x03A3 is
   *  the same 0x03A3.
   */
  
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* 1st check, the previous cased point is not available */
  if(1)
    {
      const pdf_char_t *input_data = (pdf_char_t *) \
                                     "\x00\x00\x00\x47" "\x00\x00\x00\x4E" \
                                     "\x00\x00\x00\x55" "\x00\x00\x00\x20" \
                                     "\x00\x00\x00\x27"
                                     "\x00\x00\x03\xA3" "\x00\x00\x00\x20" \
                                     "\x00\x00\x00\x47" "\x00\x00\x00\x4E" \
                                     "\x00\x00\x00\x55";
      const pdf_char_t *expected_data = input_data;
      const pdf_size_t input_size = 40;
      pdf_char_t *output_data;
      pdf_size_t output_size;
      pdf_text_t text;
      
      /* Create text object with given contents */
      fail_if(pdf_text_new_from_unicode(&text,
                                        input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_BE) != PDF_OK);
      
      /* 1. The call to  pdf_text_filter should return PDF_OK. */
      fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
          fail_if(internal_hex == NULL);
          printf("pdf_text_filter:20:1:Internal> '%s'\n", internal_hex);
          pdf_dealloc(internal_hex);
        }
      
      /* Get output as UTF-32BE */
      fail_unless(pdf_text_get_unicode(&output_data,
                                       &output_size,
                                       text,
                                       PDF_TEXT_UTF32_BE,
                                       PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
      
      /* 2. The contents of the output text object must be the expected ones. */
      fail_unless(output_size == input_size);
      fail_unless(memcmp(output_data, expected_data, input_size) == 0);
    }
  
  /* 2nd check, the sequence of previous case-ignorable points is NOT empty */
  if(1)
    {
      const pdf_char_t *input_data = (pdf_char_t *) \
                                     "\x00\x00\x00\x47" "\x00\x00\x00\x4E" \
                                     "\x00\x00\x00\x55" "\x00\x00\x00\x20" \
                                     "\x00\x00\x00\x55" "\x00\x00\x00\x27" \
                                     "\x00\x00\x03\xA3" "\x00\x00\x00\x55" \
                                     "\x00\x00\x00\x20" \
                                     "\x00\x00\x00\x47" "\x00\x00\x00\x4E" \
                                     "\x00\x00\x00\x55";
      const pdf_char_t *expected_data = input_data;
      const pdf_size_t input_size = 48;
      pdf_char_t *output_data;
      pdf_size_t output_size;
      pdf_text_t text;
      
      /* Create text object with given contents */
      fail_if(pdf_text_new_from_unicode(&text,
                                        input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_BE) != PDF_OK);
      
      /* 1. The call to  pdf_text_filter should return PDF_OK. */
      fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          internal_hex = pdf_text_test_get_hex(text->data,text->size,':');
          fail_if(internal_hex == NULL);
          printf("pdf_text_filter:20:2:Internal> '%s'\n", internal_hex);
          pdf_dealloc(internal_hex);
        }
      
      /* Get output as UTF-32BE */
      fail_unless(pdf_text_get_unicode(&output_data,
                                       &output_size,
                                       text,
                                       PDF_TEXT_UTF32_BE,
                                       PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
      
      /* 2. The contents of the output text object must be the expected ones. */
      fail_unless(output_size == input_size);
      fail_unless(memcmp(output_data, expected_data, input_size) == 0);
    }
}
END_TEST


/*
 * Test: pdf_text_filter_021
 * Description:
 *   Apply the `Upper case' filter to a text object that contains
 *   special-case-conversion characters with After_Soft_Dotted context
 *   condition, where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_021)
{
  /* 0x030C has combining class 230, so condition should not be fulfilled */
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00" "i" \
                                                "\x00\x00\x03\x0C" \
                                                "\x00\x00\x03\x07";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00" "I" \
                                                   "\x00\x00\x03\x0C" \
                                                   "\x00\x00\x03\x07";
  pdf_size_t input_size = 16;
  pdf_size_t expected_size = 16;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'lt' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_021:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_021:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST

/*
 * Test: pdf_text_filter_022
 * Description:
 *   Apply the `Upper case' filter to a text object that contains
 *   special-case-conversion characters with More_Above context condition,
 *   where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_022)
{
  /* Code:0049; Lower:0069 0307; Title:0049; Upper:0049;
   * lt More_Above; # LATIN CAPITAL LETTER I
   * As 0xC0 has combining class 0, the condition must not be fullfilled*/
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00\x49" \
                                                "\x00\x00\x00\xC0" \
                                                "\x00\x00\x03\x0B";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00\x49" \
                                                   "\x00\x00\x00\xC0" \
                                                   "\x00\x00\x03\x0B";
  pdf_size_t input_size = 16;
  pdf_size_t expected_size = 16;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'lt' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_022:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_022:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_023
 * Description:
 *   Apply the `Upper case' filter to a text object that contains
 *   special-case-conversion characters with Before_Dot context condition,
 *   where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_023)
{
  /* NOTE: Actually there is no special condition involving "Before_Dot" as is
   *       (there are a couple of conditions involving the negated one:
   *       Not_Before_Dot). But probably is a good idea to at least leave
   *       the test, just in case in future updates of the Unicode standard
   *       one of this type appears */
  fail_if(0);
}
END_TEST


/*
 * Test: pdf_text_filter_024
 * Description:
 *   Apply the `Upper case' filter to a text object that contains
 *   special-case-conversion characters with After_I context condition,
 *   where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_024)
{
  /* Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE */
  /* 0x030C has combining class 230, so condition should not be fulfilled */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""I" \
                                                "\x00\x00\x03\x0C" \
                                                "\x00\x00\x03\x07";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""I" \
                                                   "\x00\x00\x03\x0C" \
                                                   "\x00\x00\x03\x07";
  pdf_size_t input_size = 16;
  pdf_size_t expected_size = 16;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_024:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_024:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_025
 * Description:
 *   Apply the `Upper case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent context
 *   condition, where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_025)
{
  /* # When uppercasing, i turns into a dotted capital I
   * 0069; 0069; 0130; 0130; tr; # LATIN SMALL LETTER I */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""i";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""I";
  pdf_size_t input_size = 8;
  pdf_size_t expected_size = 8;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'en' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"en") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_025:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_025:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_026
 * Description:
 *   Apply the `Upper case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent and After_I
 *    context condition (to check two context conditions at the same time),
 *    where the condition is not fulfilled.
 * Note: As long as After_I is always coming with a language condition (at least
 *   in the Unicode standard 5.0), this test is equal to pdf_text_filter_024
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_026)
{
  /* Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE */
  /* 0x030C has combining class 230, so condition should not be fulfilled */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""I" \
                                                "\x00\x00\x03\x0C" \
                                                "\x00\x00\x03\x07";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""I" \
                                                   "\x00\x00\x03\x0C" \
                                                   "\x00\x00\x03\x07";
  pdf_size_t input_size = 16;
  pdf_size_t expected_size = 16;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_026:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_026:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST



/*
 * Test: pdf_text_filter_027
 * Description:
 *   Apply the `Upper case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent and
 *   Not_Before_Dot context condition (to check two context conditions at the
 *   same time, where one of them is negated with Not_), where the condition is
 *   not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_027)
{
  /* # When lowercasing, unless an I is before a dot_above, it turns into a
   *   dotless i.
   * 0049; 0131; 0049; 0049; tr Not_Before_Dot; # LATIN CAPITAL LETTER I
   * Not Not_Before_Dot... is equal to Before_Dot :-) */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00\x49" \
                                                "\x00\x00\x03\x07" \
                                                "\x00\x00\x00\x50";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00\x49" \
                                                   "\x00\x00\x03\x07" \
                                                   "\x00\x00\x00\x50";
  pdf_size_t input_size = 16;
  pdf_size_t expected_size = 16;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_027:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_027:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);  
}
END_TEST



/*
 * Test: pdf_text_filter_028
 * Description:
 *   Apply the `Lower case' filter to a text object that contains
 *   simple-case-conversion characters.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 *   3. The length of the output text must be equal to the length of the
 *      input text.
 */
START_TEST(pdf_text_filter_028)
{
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00"" " \
                                                "\x00\x00\x00""G" \
                                                "\x00\x00\x00""N" \
                                                "\x00\x00\x00""U" \
                                                "\x00\x00\x00"" ";
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00"" " \
                                                   "\x00\x00\x00""g" \
                                                   "\x00\x00\x00""n" \
                                                   "\x00\x00\x00""u" \
                                                   "\x00\x00\x00"" ";
  pdf_size_t input_size = 20;
  pdf_size_t expected_size = input_size;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_028:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_028:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 3. The length of the output text must be equal to the length of the
   *      input text. */
  fail_unless(output_size == expected_size);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_029
 * Description:
 *   Apply the `Lower case' filter to a text object that contains
 *   special-case-conversion characters with no context condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_029)
{
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x01\x30";
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                          "\x00\x00\x00\x69\x00\x00\x03\x07";
  pdf_size_t input_size = 8;
  pdf_size_t expected_size = 12;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_029:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_029:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST



/*
 * Test: pdf_text_filter_030
 * Description:
 *   Apply the `Lower case' filter to a text object that contains
 *   special-case-conversion characters with Final_Sigma context condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones. 
 */
START_TEST(pdf_text_filter_030)
{
  /* Final_Sigma condition in Special Case algorithms
   *  When the given character is preceded by a sequence consisting of a
   *  cased unicode point (with case information) and a case-ignorable
   *  (without case information) sequence of points. The given character
   *  must no be followed by a sequence consisting of a case-ignorable
   *  sequence and a cased letter.
   *
   * - 0x03A3 point is the one with the Final_Sigma condition
   * - 0x0055 point is the cased point
   * - 0x003A point is the case-ignorable point (MidLetter WB property)
   * - 0x0020 point (whitespace) is used as word separator to have three 
   *    different words.
   * If condition is fulfilled, expected smallcase character for 0x03A3 is
   *  different: 0x03C2.
   */
  
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* 1st check, the sequence of previous case-ignorable points is empty */
  if(1)
    {
      const pdf_char_t *input_data = (pdf_char_t *) \
                                     "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                     "\x00\x00\x00""U" "\x00\x00\x00"" " \
                                     "\x00\x00\x00""G" \
                                     "\x00\x00\x03\xA3" "\x00\x00\x00"" " \
                                     "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                     "\x00\x00\x00""U";
      const pdf_char_t *expected_data = (pdf_char_t *) \
                                        "\x00\x00\x00""g" "\x00\x00\x00""n" \
                                        "\x00\x00\x00""u" "\x00\x00\x00"" " \
                                        "\x00\x00\x00""g" \
                                        "\x00\x00\x03\xC2" "\x00\x00\x00"" " \
                                        "\x00\x00\x00""g" "\x00\x00\x00""n" \
                                        "\x00\x00\x00""u";
      const pdf_size_t input_size = 40;
      pdf_char_t *output_data;
      pdf_size_t output_size;
      pdf_text_t text;
      
      /* Create text object with given contents */
      fail_if(pdf_text_new_from_unicode(&text,
                                        input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_BE) != PDF_OK);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          internal_hex = pdf_text_test_get_hex(text->data,text->size,':');
          fail_if(internal_hex == NULL);
          printf("pdf_text_filter:30:1:Before> '%s'\n", internal_hex);
          pdf_dealloc(internal_hex);
        }
      
      /* 1. The call to  pdf_text_filter should return PDF_OK. */
      fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
          fail_if(internal_hex == NULL);
          printf("pdf_text_filter:30:1:After> '%s'\n", internal_hex);
          pdf_dealloc(internal_hex);
        }
      
      /* Get output as UTF-32BE */
      fail_unless(pdf_text_get_unicode(&output_data,
                                       &output_size,
                                       text,
                                       PDF_TEXT_UTF32_BE,
                                       PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
      
      /* 2. The contents of the output text object must be the expected ones. */
      fail_unless(output_size == input_size);
      fail_unless(memcmp(output_data, expected_data, input_size) == 0);
    }
  
  /* 2nd check, the sequence of previous case-ignorable points is NOT empty */
  if(1)
    {
      const pdf_char_t *input_data = (pdf_char_t *) \
                                     "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                     "\x00\x00\x00""U" "\x00\x00\x00"" " \
                                     "\x00\x00\x00""G" "\x00\x00\x00\x3A" \
                                     "\x00\x00\x03\xA3" "\x00\x00\x00"" " \
                                     "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                     "\x00\x00\x00""U";
      const pdf_char_t *expected_data = (pdf_char_t *) \
                                        "\x00\x00\x00""g" "\x00\x00\x00""n" \
                                        "\x00\x00\x00""u" "\x00\x00\x00"" " \
                                        "\x00\x00\x00""g" "\x00\x00\x00\x3A" \
                                        "\x00\x00\x03\xC2" "\x00\x00\x00"" " \
                                        "\x00\x00\x00""g" "\x00\x00\x00""n" \
                                        "\x00\x00\x00""u";
      const pdf_size_t input_size = 44;
      pdf_char_t *output_data;
      pdf_size_t output_size;
      pdf_text_t text;
      
      /* Create text object with given contents */
      fail_if(pdf_text_new_from_unicode(&text,
                                        input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_BE) != PDF_OK);
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          internal_hex = pdf_text_test_get_hex(text->data,text->size,':');
          fail_if(internal_hex == NULL);
          printf("pdf_text_filter:30:2:Before> '%s'\n", internal_hex);
          pdf_dealloc(internal_hex);
        }
      
      /* 1. The call to  pdf_text_filter should return PDF_OK. */
      fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          internal_hex = pdf_text_test_get_hex(text->data,text->size,':');
          fail_if(internal_hex == NULL);
          printf("pdf_text_filter:30:2:After> '%s'\n", internal_hex);
          pdf_dealloc(internal_hex);
        }
      
      /* Get output as UTF-32BE */
      fail_unless(pdf_text_get_unicode(&output_data,
                                       &output_size,
                                       text,
                                       PDF_TEXT_UTF32_BE,
                                       PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
      
      /* 2. The contents of the output text object must be the expected ones. */
      fail_unless(output_size == input_size);
      fail_unless(memcmp(output_data, expected_data, input_size) == 0);
    }
}
END_TEST


/*
 * Test: pdf_text_filter_031
 * Description:
 *   Apply the `Lower case' filter to a text object that contains
 *   special-case-conversion characters with After_Soft_Dotted context
 *   condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_031)
{
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00" "i" \
                                                "\x00\x00\x03\x07";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00" "i" \
                                                   "\x00\x00\x03\x07";
  pdf_size_t input_size = 12;
  pdf_size_t expected_size = 12;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'lt' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_031:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_031:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST

/*
 * Test: pdf_text_filter_032
 * Description:
 *   Apply the `Lower case' filter to a text object that contains
 *   special-case-conversion characters with More_Above context condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_032)
{
  /* Code:0049; Lower:0069 0307; Title:0049; Upper:0049;
   * lt More_Above; # LATIN CAPITAL LETTER I */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00\x49"
                                                "\x00\x00\x03\x0B";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                              "\x00\x00\x00\x69\x00\x00\x03\x07"
                                                   "\x00\x00\x03\x0B";
  pdf_size_t input_size = 12;
  pdf_size_t expected_size = 16;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'lt' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_032:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_032:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_033
 * Description:
 *   Apply the `Lower case' filter to a text object that contains
 *   special-case-conversion characters with Before_Dot context condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_033)
{
  /* NOTE: Actually there is no special condition involving "Before_Dot" as is
   *       (there are a couple of conditions involving the negated one:
   *       Not_Before_Dot). But probably is a good idea to at least leave
   *       the test, just in case in future updates of the Unicode standard
   *       one of this type appears */
  fail_if(0);
}
END_TEST


/*
 * Test: pdf_text_filter_034
 * Description:
 *   Apply the `Lower case' filter to a text object that contains
 *   special-case-conversion characters with After_I context condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_034)
{
  /* Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""I" \
                                                "\x00\x00\x03\x07";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""i";
  pdf_size_t input_size = 12;
  pdf_size_t expected_size = 8;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_034:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_034:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_035
 * Description:
 *   Apply the `Lower case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent context
 *   condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_035)
{
  /* 00CC; 0069 0307 0300; 00CC; 00CC; lt; # LATIN CAPITAL LETTER I WITH GRAVE*/
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00\xCC";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                  "\x00\x00\x00\x69\x00\x00\x03\x07\x00\x00\x03\x00";
  pdf_size_t input_size = 8;
  pdf_size_t expected_size = 16;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'lt' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_035:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_035:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_036
 * Description:
 *   Apply the `Lower case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent and After_I
 *    context condition (to check two context conditions at the same time).
 * Note: As long as After_I is always coming with a language condition (at least
 *   in the Unicode standard 5.0), this test is equal to pdf_text_filter_034
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_036)
{
  /* Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""I" \
                                                "\x00\x00\x03\x07";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""i";
  pdf_size_t input_size = 12;
  pdf_size_t expected_size = 8;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_016:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_016:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST



/*
 * Test: pdf_text_filter_037
 * Description:
 *   Apply the `Lower case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent and
 *   Not_Before_Dot context condition (to check two context conditions at the
 *   same time, where one of them is negated with Not_).
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_037)
{
  /* # When lowercasing, unless an I is before a dot_above, it turns into a
   *   dotless i.
   * 0049; 0131; 0049; 0049; tr Not_Before_Dot; # LATIN CAPITAL LETTER I */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00\x49" \
                                                "\x00\x00\x00\x50";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x01\x31" \
                                                   "\x00\x00\x00\x70";
  pdf_size_t input_size = 12;
  pdf_size_t expected_size = 12;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_037:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_037:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);  
}
END_TEST


/*
 * Test: pdf_text_filter_038
 * Description:
 *   Apply the `Lower case' filter to a text object that contains
 *   special-case-conversion characters with Final_Sigma context condition,
 *   where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones. 
 */
START_TEST(pdf_text_filter_038)
{
  /* Final_Sigma condition in Special Case algorithms
   *  When the given character is preceded by a sequence consisting of a
   *  cased unicode point (with case information) and a case-ignorable
   *  (without case information) sequence of points. The given character
   *  must no be followed by a sequence consisting of a case-ignorable
   *  sequence and a cased letter.
   *
   * The condition won't be fulfilled because:
   *  1st check. The previous cased point is not available
   *  2nd check. A next cased point is available
   * 
   * - 0x03A3 point is the one with the Final_Sigma condition
   * - 0x0055 point is the cased point
   * - 0x0027 point is the case-ignorable point
   * - 0x0020 point (whitespace) is used as word separator to have three 
   *    different words.
   *
   * If condition is not fulfilled, expected lowercase character for 0x03A3 is
   *  the non-final lowercase sigma 0x03C3.
   */
  
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* 1st check, the previous cased point is not available */
  if(1)
    {
      const pdf_char_t *input_data = (pdf_char_t *) \
                                     "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                     "\x00\x00\x00""U" "\x00\x00\x00"" " \
                                     "\x00\x00\x00\x27" \
                                     "\x00\x00\x03\xA3" "\x00\x00\x00"" " \
                                     "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                     "\x00\x00\x00""U";
      const pdf_char_t *expected_data = (pdf_char_t *) \
                                        "\x00\x00\x00""g" "\x00\x00\x00""n" \
                                        "\x00\x00\x00""u" "\x00\x00\x00"" " \
                                        "\x00\x00\x00\x27" \
                                        "\x00\x00\x03\xC3" "\x00\x00\x00"" " \
                                        "\x00\x00\x00""g" "\x00\x00\x00""n" \
                                        "\x00\x00\x00""u";
      const pdf_size_t input_size = 40;
      pdf_char_t *output_data;
      pdf_size_t output_size;
      pdf_text_t text;
      
      /* Create text object with given contents */
      fail_if(pdf_text_new_from_unicode(&text,
                                        input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_BE) != PDF_OK);
      
      /* 1. The call to  pdf_text_filter should return PDF_OK. */
      fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
          fail_if(internal_hex == NULL);
          printf("pdf_text_filter:38:1:Internal> '%s'\n", internal_hex);
          pdf_dealloc(internal_hex);
        }
      
      /* Get output as UTF-32BE */
      fail_unless(pdf_text_get_unicode(&output_data,
                                       &output_size,
                                       text,
                                       PDF_TEXT_UTF32_BE,
                                       PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
      
      /* 2. The contents of the output text object must be the expected ones. */
      fail_unless(output_size == input_size);
      fail_unless(memcmp(output_data, expected_data, input_size) == 0);
    }
  
  /* 2nd check, a next cased point is available */
  if(1)
    {
      const pdf_char_t *input_data = (pdf_char_t *) \
                                     "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                     "\x00\x00\x00""U" "\x00\x00\x00"" " \
                                     "\x00\x00\x00""G" "\x00\x00\x00\x27" \
                                     "\x00\x00\x03\xA3" "\x00\x00\x00""G" \
                                     "\x00\x00\x00"" " \
                                     "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                     "\x00\x00\x00""U";
      const pdf_char_t *expected_data = (pdf_char_t *) \
                                        "\x00\x00\x00""g" "\x00\x00\x00""n" \
                                        "\x00\x00\x00""u" "\x00\x00\x00"" " \
                                        "\x00\x00\x00""g" "\x00\x00\x00\x27" \
                                        "\x00\x00\x03\xC3" "\x00\x00\x00""g" \
                                        "\x00\x00\x00"" " \
                                        "\x00\x00\x00""g" "\x00\x00\x00""n" \
                                        "\x00\x00\x00""u";
      const pdf_size_t input_size = 48;
      pdf_char_t *output_data;
      pdf_size_t output_size;
      pdf_text_t text;
      
      /* Create text object with given contents */
      fail_if(pdf_text_new_from_unicode(&text,
                                        input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_BE) != PDF_OK);
      
      /* 1. The call to  pdf_text_filter should return PDF_OK. */
      fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          internal_hex = pdf_text_test_get_hex(text->data,text->size,':');
          fail_if(internal_hex == NULL);
          printf("pdf_text_filter:38:2:Internal> '%s'\n", internal_hex);
          pdf_dealloc(internal_hex);
        }
      
      /* Get output as UTF-32BE */
      fail_unless(pdf_text_get_unicode(&output_data,
                                       &output_size,
                                       text,
                                       PDF_TEXT_UTF32_BE,
                                       PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
      
      /* 2. The contents of the output text object must be the expected ones. */
      fail_unless(output_size == input_size);
      fail_unless(memcmp(output_data, expected_data, input_size) == 0);
    }
}
END_TEST



/*
 * Test: pdf_text_filter_039
 * Description:
 *   Apply the `Lower case' filter to a text object that contains
 *   special-case-conversion characters with After_Soft_Dotted context
 *   condition, where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_039)
{
  /* 0x030C has combining class 230, so condition should not be fulfilled */
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00" "i" \
                                                "\x00\x00\x03\x0C" \
                                                "\x00\x00\x03\x07";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00" "i" \
                                                   "\x00\x00\x03\x0C" \
                                                   "\x00\x00\x03\x07";
  pdf_size_t input_size = 16;
  pdf_size_t expected_size = 16;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'lt' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_039:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_039:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_040
 * Description:
 *   Apply the `Lower case' filter to a text object that contains
 *   special-case-conversion characters with More_Above context condition,
 *   where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_040)
{
  /* Code:0049; Lower:0069 0307; Title:0049; Upper:0049;
   * lt More_Above; # LATIN CAPITAL LETTER I
   * As 0xC0 has combining class 0, the condition must not be fullfilled */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00\x49" \
                                                "\x00\x00\x00\xC0" \
                                                "\x00\x00\x03\x0B";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00\x69" \
                                                   "\x00\x00\x00\xE0" \
                                                   "\x00\x00\x03\x0B";
  pdf_size_t input_size = 16;
  pdf_size_t expected_size = 16;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'lt' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_040:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_040:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_041
 * Description:
 *   Apply the `Lower case' filter to a text object that contains
 *   special-case-conversion characters with Before_Dot context condition,
 *   where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_041)
{
  /* NOTE: Actually there is no special condition involving "Before_Dot" as is
   *       (there are a couple of conditions involving the negated one:
   *       Not_Before_Dot). But probably is a good idea to at least leave
   *       the test, just in case in future updates of the Unicode standard
   *       one of this type appears */
  fail_if(0);
}
END_TEST


/*
 * Test: pdf_text_filter_042
 * Description:
 *   Apply the `Lower case' filter to a text object that contains
 *   special-case-conversion characters with After_I context condition,
 *   where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_042)
{
  /* Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE */
  /* 0x030C has combining class 230, so condition should not be fulfilled */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""I" \
                                                "\x00\x00\x03\x0C" \
                                                "\x00\x00\x03\x07";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x01\x31" \
                                                   "\x00\x00\x03\x0C" \
                                                   "\x00\x00\x03\x07";
  pdf_size_t input_size = 16;
  pdf_size_t expected_size = 16;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_042:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_042:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_043
 * Description:
 *   Apply the `Lower case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent context
 *   condition, where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_043)
{
  /* 00CC; 0069 0307 0300; 00CC; 00CC; lt; # LATIN CAPITAL LETTER I WITH GRAVE 
   * If condition not fulfilled, simple lowercase gives: 00EC */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00\xCC";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00\xEC";
  pdf_size_t input_size = 8;
  pdf_size_t expected_size = 8;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'en' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"en") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_043:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_043:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_044
 * Description:
 *   Apply the `Lower case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent and After_I
 *    context condition (to check two context conditions at the same time),
 *    where the condition is not fulfilled.
 * Note: As long as After_I is always coming with a language condition (at least
 *   in the Unicode standard 5.0), this test is equal to pdf_text_filter_042
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_044)
{
  /* Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE */
  /* 0x030C has combining class 230, so condition should not be fulfilled */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""I" \
                                                "\x00\x00\x03\x0C" \
                                                "\x00\x00\x03\x07";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x01\x31" \
                                                   "\x00\x00\x03\x0C" \
                                                   "\x00\x00\x03\x07";
  pdf_size_t input_size = 16;
  pdf_size_t expected_size = 16;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_044:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_044:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
}
END_TEST



/*
 * Test: pdf_text_filter_045
 * Description:
 *   Apply the `Lower case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent and
 *   Not_Before_Dot context condition (to check two context conditions at the
 *   same time, where one of them is negated with Not_), where the condition is
 *   not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_045)
{
  /* # When lowercasing, unless an I is before a dot_above, it turns into a
   *   dotless i.
   * 0049; 0131; 0049; 0049; tr Not_Before_Dot; # LATIN CAPITAL LETTER I
   * Not Not_Before_Dot... is equal to Before_Dot :-) */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00\x49" \
                                                "\x00\x00\x03\x07" \
                                                "\x00\x00\x00\x50";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00\x69" \
                                                   "\x00\x00\x00\x70";
  pdf_size_t input_size = 16;
  pdf_size_t expected_size = 12;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_045:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_045:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);  
}
END_TEST



/*
 * Test: pdf_text_filter_046
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   simple-case-conversion characters.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 *   3. The length of the output text must be equal to the length of the
 *      input text.
 */
START_TEST(pdf_text_filter_046)
{
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00"" " \
                                                "\x00\x00\x00""G" \
                                                "\x00\x00\x00""N" \
                                                "\x00\x00\x00""U" \
                                                "\x00\x00\x00"" ";
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00"" " \
                                                   "\x00\x00\x00""G" \
                                                   "\x00\x00\x00""n" \
                                                   "\x00\x00\x00""u" \
                                                   "\x00\x00\x00"" ";
  pdf_size_t input_size = 20;
  pdf_size_t expected_size = input_size;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_046:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_046:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 3. The length of the output text must be equal to the length of the
   *      input text. */
  fail_unless(output_size == expected_size);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_047
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with no context condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_047)
{
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""A" \
                                                "\x00\x00\x01\x30";
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""A" \
                                                "\x00\x00\x00\x69\x00\x00\x03\x07";
  pdf_size_t input_size = 12;
  pdf_size_t expected_size = 16;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_047:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_047:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST



/*
 * Test: pdf_text_filter_048
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with Final_Sigma context condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones. 
 */
START_TEST(pdf_text_filter_048)
{
  /* Final_Sigma condition in Special Case algorithms
   *  When the given character is preceded by a sequence consisting of a
   *  cased unicode point (with case information) and a case-ignorable
   *  (without case information) sequence of points. The given character
   *  must no be followed by a sequence consisting of a case-ignorable
   *  sequence and a cased letter.
   *
   * - 0x03A3 point is the one with the Final_Sigma condition
   * - 0x0055 point is the cased point
   * - 0x003A point is the case-ignorable point (MidLetter WB property)
   * - 0x0020 point (whitespace) is used as word separator to have three 
   *    different words.
   * If condition is fulfilled, expected titlecase character for 0x03A3 is
   *  the same 0x03A3. But WARNING!!! Only the first character in the word
   *  is titlecased, and due to the constraints of the Final_Sigma condition,
   *  the character with the Final_Sigma will always be at the end of the word,
   *  so the title case option will probably be NEVER used. In this case, the
   *  finalsigma lowercase unicode point will be used (0x03C2) !!.
   */
  
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* 1st check, the sequence of previous case-ignorable points is empty */
  if(1)
    {
      const pdf_char_t *input_data = (pdf_char_t *) \
                                     "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                     "\x00\x00\x00""U" "\x00\x00\x00"" " \
                                     "\x00\x00\x00""G" \
                                     "\x00\x00\x03\xA3" "\x00\x00\x00"" " \
                                     "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                     "\x00\x00\x00""U";
      const pdf_char_t *expected_data = (pdf_char_t *) \
                                     "\x00\x00\x00""G" "\x00\x00\x00""n" \
                                     "\x00\x00\x00""u" "\x00\x00\x00"" " \
                                     "\x00\x00\x00""G" \
                                     "\x00\x00\x03\xC2" "\x00\x00\x00"" " \
                                     "\x00\x00\x00""G" "\x00\x00\x00""n" \
                                     "\x00\x00\x00""u";
      const pdf_size_t input_size = 40;
      pdf_char_t *output_data;
      pdf_size_t output_size;
      pdf_text_t text;
      
      /* Create text object with given contents */
      fail_if(pdf_text_new_from_unicode(&text,
                                        input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_BE) != PDF_OK);
      
      /* 1. The call to  pdf_text_filter should return PDF_OK. */
      fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
          fail_if(internal_hex == NULL);
          printf("pdf_text_filter:48:1:Internal> '%s'\n", internal_hex);
          pdf_dealloc(internal_hex);
        }
      
      /* Get output as UTF-32BE */
      fail_unless(pdf_text_get_unicode(&output_data,
                                       &output_size,
                                       text,
                                       PDF_TEXT_UTF32_BE,
                                       PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
      
      /* 2. The contents of the output text object must be the expected ones. */
      fail_unless(output_size == input_size);
      fail_unless(memcmp(output_data, expected_data, input_size) == 0);
    }
  
  /* 2nd check, the sequence of previous case-ignorable points is NOT empty */
  if(1)
    {
      const pdf_char_t *input_data = (pdf_char_t *) \
                                        "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                        "\x00\x00\x00""U" "\x00\x00\x00"" " \
                                        "\x00\x00\x00""G" "\x00\x00\x00\x3A" \
                                        "\x00\x00\x03\xA3" "\x00\x00\x00"" " \
                                        "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                        "\x00\x00\x00""U";
      const pdf_char_t *expected_data = (pdf_char_t *) \
                                        "\x00\x00\x00""G" "\x00\x00\x00""n" \
                                        "\x00\x00\x00""u" "\x00\x00\x00"" " \
                                        "\x00\x00\x00""G" "\x00\x00\x00\x3A" \
                                        "\x00\x00\x03\xC2" "\x00\x00\x00"" " \
                                        "\x00\x00\x00""G" "\x00\x00\x00""n" \
                                        "\x00\x00\x00""u";
      
      const pdf_size_t input_size = 44;
      pdf_char_t *output_data;
      pdf_size_t output_size;
      pdf_text_t text;
      
      /* Create text object with given contents */
      fail_if(pdf_text_new_from_unicode(&text,
                                        input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_BE) != PDF_OK);
      
      /* 1. The call to  pdf_text_filter should return PDF_OK. */
      fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          internal_hex = pdf_text_test_get_hex(text->data,text->size,':');
          fail_if(internal_hex == NULL);
          printf("pdf_text_filter:48:2:Internal> '%s'\n", internal_hex);
          pdf_dealloc(internal_hex);
        }
      
      /* Get output as UTF-32BE */
      fail_unless(pdf_text_get_unicode(&output_data,
                                       &output_size,
                                       text,
                                       PDF_TEXT_UTF32_BE,
                                       PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
      
      /* 2. The contents of the output text object must be the expected ones. */
      fail_unless(output_size == input_size);
      fail_unless(memcmp(output_data, expected_data, input_size) == 0);
    }
}
END_TEST


/*
 * Test: pdf_text_filter_049
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with After_Soft_Dotted context
 *   condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_049)
{
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00" "i" \
                                                "\x00\x00\x03\x07";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00" "I" \
                                                   "\x00\x00\x03\x07";
  pdf_size_t input_size = 12;
  pdf_size_t expected_size = 12;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'lt' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_049:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_049:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST

/*
 * Test: pdf_text_filter_050
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with More_Above context condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_050)
{
  /* Code:0049; Lower:0069 0307; Title:0049; Upper:0049;
   * lt More_Above; # LATIN CAPITAL LETTER I */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""a" \
                                                "\x00\x00\x00\x49"
                                                "\x00\x00\x03\x0B";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""A" \
                                                "\x00\x00\x00\x69\x00\x00\x03\x07"
                                                "\x00\x00\x03\x0B";
  pdf_size_t input_size = 16;
  pdf_size_t expected_size = 20;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'lt' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_050:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_050:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_051
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with Before_Dot context condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_051)
{
  /* NOTE: Actually there is no special condition involving "Before_Dot" as is
   *       (there are a couple of conditions involving the negated one:
   *       Not_Before_Dot). But probably is a good idea to at least leave
   *       the test, just in case in future updates of the Unicode standard
   *       one of this type appears */
  fail_if(0);
}
END_TEST


/*
 * Test: pdf_text_filter_052
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with After_I context condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_052)
{
  /* Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""a" \
                                                "\x00\x00\x00""I" \
                                                "\x00\x00\x03\x07";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""A" \
                                                   "\x00\x00\x00""i";
  pdf_size_t input_size = 16;
  pdf_size_t expected_size = 12;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_052:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_052:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_053
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent context
 *   condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_053)
{
  /* 00CC; 0069 0307 0300; 00CC; 00CC; lt; # LATIN CAPITAL LETTER I WITH GRAVE*/
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""a" \
                                                "\x00\x00\x00\xCC";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""A" \
                                  "\x00\x00\x00\x69\x00\x00\x03\x07\x00\x00\x03\x00";
  pdf_size_t input_size = 12;
  pdf_size_t expected_size = 20;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'lt' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_053:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_053:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_054
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent and After_I
 *    context condition (to check two context conditions at the same time).
 * Note: As long as After_I is always coming with a language condition (at least
 *   in the Unicode standard 5.0), this test is equal to pdf_text_filter_034
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_054)
{
  /* Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""a" \
                                                "\x00\x00\x00""I" \
                                                "\x00\x00\x03\x07";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""A" \
                                                   "\x00\x00\x00""i";
  pdf_size_t input_size = 16;
  pdf_size_t expected_size = 12;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_054:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_054:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST



/*
 * Test: pdf_text_filter_055
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent and
 *   Not_Before_Dot context condition (to check two context conditions at the
 *   same time, where one of them is negated with Not_).
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_055)
{
  /* # When lowercasing, unless an I is before a dot_above, it turns into a
   *   dotless i.
   * 0049; 0131; 0049; 0049; tr Not_Before_Dot; # LATIN CAPITAL LETTER I */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""a" \
                                                "\x00\x00\x00\x49" \
                                                "\x00\x00\x00\x50";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""A" \
                                                   "\x00\x00\x01\x31" \
                                                   "\x00\x00\x00\x70";
  pdf_size_t input_size = 16;
  pdf_size_t expected_size = 16;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_055:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_055:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);  
}
END_TEST


/*
 * Test: pdf_text_filter_056
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with Final_Sigma context condition,
 *   where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones. 
 */
START_TEST(pdf_text_filter_056)
{
  /* Final_Sigma condition in Special Case algorithms
   *  When the given character is preceded by a sequence consisting of a
   *  cased unicode point (with case information) and a case-ignorable
   *  (without case information) sequence of points. The given character
   *  must no be followed by a sequence consisting of a case-ignorable
   *  sequence and a cased letter.
   *
   * The condition won't be fulfilled because:
   *  1st check. The previous cased point is not available
   *  2nd check. A next cased point is available
   * 
   * - 0x03A3 point is the one with the Final_Sigma condition
   * - 0x0055 point is the cased point
   * - 0x003A point is the case-ignorable point (MidLetter WB property)
   * - 0x0020 point (whitespace) is used as word separator to have three 
   *    different words.
   *
   * If condition is not fulfilled, expected titlecase character for 0x03A3 is
   *  the same 0x03A3. But WARNING!!! Only the first character in the word
   *  is titlecased, and due to the constraints of the Final_Sigma condition,
   *  the character with the Final_Sigma will always be at the end of the word,
   *  so the title case option will probably be NEVER used. In this case, the
   *  non-final lowercase sigma nd uppercase unicode points will be used!!.
   */
  
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* 1st check, the previous cased point is not available (so word starts in
   *  the sigma --> uppercase sigma */
  if(1)
    {
      const pdf_char_t *input_data = (pdf_char_t *) \
                                     "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                     "\x00\x00\x00""U" "\x00\x00\x00"" " \
                                     "\x00\x00\x00\x3A"
                                     "\x00\x00\x03\xA3" "\x00\x00\x00"" " \
                                     "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                     "\x00\x00\x00""U";
      const pdf_char_t *expected_data = (pdf_char_t *) \
                                     "\x00\x00\x00""G" "\x00\x00\x00""n" \
                                     "\x00\x00\x00""u" "\x00\x00\x00"" " \
                                     "\x00\x00\x00\x3A"
                                     "\x00\x00\x03\xA3" "\x00\x00\x00"" " \
                                     "\x00\x00\x00""G" "\x00\x00\x00""n" \
                                     "\x00\x00\x00""u";
      const pdf_size_t input_size = 40;
      pdf_char_t *output_data;
      pdf_size_t output_size;
      pdf_text_t text;
      
      /* Create text object with given contents */
      fail_if(pdf_text_new_from_unicode(&text,
                                        input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_BE) != PDF_OK);
      
      /* 1. The call to  pdf_text_filter should return PDF_OK. */
      fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
          fail_if(internal_hex == NULL);
          printf("pdf_text_filter:56:1:Internal> '%s'\n", internal_hex);
          pdf_dealloc(internal_hex);
        }
      
      /* Get output as UTF-32BE */
      fail_unless(pdf_text_get_unicode(&output_data,
                                       &output_size,
                                       text,
                                       PDF_TEXT_UTF32_BE,
                                       PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
      
      /* 2. The contents of the output text object must be the expected ones. */
      fail_unless(output_size == input_size);
      fail_unless(memcmp(output_data, expected_data, input_size) == 0);
    }
  
  /* 2nd check, the sequence of previous case-ignorable points is NOT empty, but
   *  the previous cased point is available --> non-final lowercase sigma is
   *  used */
  if(1)
    {
      const pdf_char_t *input_data = (pdf_char_t *) \
                                     "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                     "\x00\x00\x00""U" "\x00\x00\x00"" " \
                                     "\x00\x00\x00""G" "\x00\x00\x00\x3A" \
                                     "\x00\x00\x03\xA3" "\x00\x00\x00""G" \
                                     "\x00\x00\x00"" " \
                                     "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                     "\x00\x00\x00""U";
      const pdf_char_t *expected_data = (pdf_char_t *) \
                                        "\x00\x00\x00""G" "\x00\x00\x00""n" \
                                        "\x00\x00\x00""u" "\x00\x00\x00"" " \
                                        "\x00\x00\x00""G" "\x00\x00\x00\x3A" \
                                        "\x00\x00\x03\xC3" "\x00\x00\x00""g" \
                                        "\x00\x00\x00"" " \
                                        "\x00\x00\x00""G" "\x00\x00\x00""n" \
                                        "\x00\x00\x00""u";
      const pdf_size_t input_size = 48;
      pdf_char_t *output_data;
      pdf_size_t output_size;
      pdf_text_t text;
      
      /* Create text object with given contents */
      fail_if(pdf_text_new_from_unicode(&text,
                                        input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_BE) != PDF_OK);
      
      /* 1. The call to  pdf_text_filter should return PDF_OK. */
      fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          internal_hex = pdf_text_test_get_hex(text->data,text->size,':');
          fail_if(internal_hex == NULL);
          printf("pdf_text_filter:56:2:Internal> '%s'\n", internal_hex);
          pdf_dealloc(internal_hex);
        }
      
      /* Get output as UTF-32BE */
      fail_unless(pdf_text_get_unicode(&output_data,
                                       &output_size,
                                       text,
                                       PDF_TEXT_UTF32_BE,
                                       PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
      
      /* 2. The contents of the output text object must be the expected ones. */
      fail_unless(output_size == input_size);
      fail_unless(memcmp(output_data, expected_data, input_size) == 0);
    }
}
END_TEST


/*
 * Test: pdf_text_filter_057
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with After_Soft_Dotted context
 *   condition, where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_057)
{
  /* 0x030C has combining class 230, so condition should not be fulfilled */
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00" "a" \
                                                "\x00\x00\x00" "i" \
                                                "\x00\x00\x03\x0C" \
                                                "\x00\x00\x03\x07";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00" "A" \
                                                   "\x00\x00\x00" "i" \
                                                   "\x00\x00\x03\x0C" \
                                                   "\x00\x00\x03\x07";
  pdf_size_t input_size = 20;
  pdf_size_t expected_size = 20;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'lt' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_057:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_057:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_058
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with More_Above context condition,
 *   where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_058)
{
  /* Code:0049; Lower:0069 0307; Title:0049; Upper:0049;
   * lt More_Above; # LATIN CAPITAL LETTER I
   * As 0xC0 has combining class 0, the condition must not be fullfilled */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""a" \
                                                "\x00\x00\x00\x49" \
                                                "\x00\x00\x00\xC0" \
                                                "\x00\x00\x03\x0B";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""A" \
                                                   "\x00\x00\x00\x69" \
                                                   "\x00\x00\x00\xE0" \
                                                   "\x00\x00\x03\x0B";
  pdf_size_t input_size = 20;
  pdf_size_t expected_size = 20;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'lt' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_058:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_058:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_059
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with Before_Dot context condition,
 *   where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_059)
{
  /* NOTE: Actually there is no special condition involving "Before_Dot" as is
   *       (there are a couple of conditions involving the negated one:
   *       Not_Before_Dot). But probably is a good idea to at least leave
   *       the test, just in case in future updates of the Unicode standard
   *       one of this type appears */
  fail_if(0);
}
END_TEST


/*
 * Test: pdf_text_filter_060
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with After_I context condition,
 *   where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_060)
{
  /* Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE */
  /* 0x030C has combining class 230, so condition should not be fulfilled */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""a" \
                                                "\x00\x00\x00""I" \
                                                "\x00\x00\x03\x0C" \
                                                "\x00\x00\x03\x07";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""A" \
                                                   "\x00\x00\x01\x31" \
                                                   "\x00\x00\x03\x0C" \
                                                   "\x00\x00\x03\x07";
  pdf_size_t input_size = 20;
  pdf_size_t expected_size = 20;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_060:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_060:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_061
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent context
 *   condition, where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_061)
{
  /* 00CC; 0069 0307 0300; 00CC; 00CC; lt; # LATIN CAPITAL LETTER I WITH GRAVE 
   * If condition not fulfilled, simple lowercase gives: 00EC */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""a" \
                                                "\x00\x00\x00\xCC";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""A" \
                                                   "\x00\x00\x00\xEC";
  pdf_size_t input_size = 12;
  pdf_size_t expected_size = 12;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'en' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"en") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_061:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_061:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
  
}
END_TEST


/*
 * Test: pdf_text_filter_062
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent and After_I
 *    context condition (to check two context conditions at the same time),
 *    where the condition is not fulfilled.
 * Note: As long as After_I is always coming with a language condition (at least
 *   in the Unicode standard 5.0), this test is equal to pdf_text_filter_060
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_062)
{
  /* Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE */
  /* 0x030C has combining class 230, so condition should not be fulfilled */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""a" \
                                                "\x00\x00\x00""I" \
                                                "\x00\x00\x03\x0C" \
                                                "\x00\x00\x03\x07";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""A" \
                                                   "\x00\x00\x01\x31" \
                                                   "\x00\x00\x03\x0C" \
                                                   "\x00\x00\x03\x07";
  pdf_size_t input_size = 20;
  pdf_size_t expected_size = 20;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_062:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_062:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);
}
END_TEST



/*
 * Test: pdf_text_filter_063
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent and
 *   Not_Before_Dot context condition (to check two context conditions at the
 *   same time, where one of them is negated with Not_), where the condition is
 *   not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_filter_063)
{
  /* # When lowercasing, unless an I is before a dot_above, it turns into a
   *   dotless i.
   * 0049; 0131; 0049; 0049; tr Not_Before_Dot; # LATIN CAPITAL LETTER I
   * Not Not_Before_Dot... is equal to Before_Dot :-) */
  
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""a" \
                                                "\x00\x00\x00\x49" \
                                                "\x00\x00\x03\x07";
  
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""A" \
                                                   "\x00\x00\x00\x69";
  pdf_size_t input_size = 16;
  pdf_size_t expected_size = 12;
  pdf_char_t *output_data;
  pdf_size_t output_size;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(&text,
                                    input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);
  
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_063:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      internal_hex = pdf_text_test_get_hex(text->data, text->size, ':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_063:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, strlen((char *)internal_hex));
      pdf_dealloc(internal_hex);
    }
  
  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);  
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_text_filter (void)
{
  TCase *tc = tcase_create("pdf_text_filter");
  
  tcase_add_test(tc, pdf_text_filter_001); /* Normalize line endings */
  tcase_add_test(tc, pdf_text_filter_002); /* Normalize line endings */

  tcase_add_test(tc, pdf_text_filter_003); /* Remove line endings */
  tcase_add_test(tc, pdf_text_filter_004); /* Remove line endings */

  tcase_add_test(tc, pdf_text_filter_005); /* Remove ampersands */
  tcase_add_test(tc, pdf_text_filter_006); /* Remove ampersands */

  tcase_add_test(tc, pdf_text_filter_007); /* Normalize with full width */
  tcase_add_test(tc, pdf_text_filter_008); /* Normalize with full width */
  tcase_add_test(tc, pdf_text_filter_009); /* Normalize with full width */

  /* Upper case */
  tcase_add_test(tc, pdf_text_filter_010); /* Upper */
  tcase_add_test(tc, pdf_text_filter_011); /* Upper-Special */
  tcase_add_test(tc, pdf_text_filter_012); /* Upper-Special Final_Sigma */
  tcase_add_test(tc, pdf_text_filter_013); /* Upper-Special After_Soft_Dotted */
  tcase_add_test(tc, pdf_text_filter_014); /* Upper-Special More_Above */
  tcase_add_test(tc, pdf_text_filter_015); /* Upper-Special Before_Dot  */
  tcase_add_test(tc, pdf_text_filter_016); /* Upper-Special After_I  */
  tcase_add_test(tc, pdf_text_filter_017); /* Upper-Special 'tr'  */
  tcase_add_test(tc, pdf_text_filter_018); /* Upper-Special After_I and 'tr'  */
  tcase_add_test(tc, pdf_text_filter_019); /* Upper-Special Not_Before_Dot  */
  tcase_add_test(tc, pdf_text_filter_020); /* Upper-Special Not Final_Sigma */
  tcase_add_test(tc, pdf_text_filter_021); /* Upper-Special Not After_Soft_Dotted */
  tcase_add_test(tc, pdf_text_filter_022); /* Upper-Special Not More_Above */
  tcase_add_test(tc, pdf_text_filter_023); /* Upper-Special Not Before_Dot  */
  tcase_add_test(tc, pdf_text_filter_024); /* Upper-Special Not After_I  */
  tcase_add_test(tc, pdf_text_filter_025); /* Upper-Special Not 'tr'  */
  tcase_add_test(tc, pdf_text_filter_026); /* Upper-Special Not After_I and 'tr'  */
  tcase_add_test(tc, pdf_text_filter_027); /* Upper-Special Not Not_Before_Dot  */

  /* Lower case */
  tcase_add_test(tc, pdf_text_filter_028); /* Lower */
  tcase_add_test(tc, pdf_text_filter_029); /* Lower-Special */
  tcase_add_test(tc, pdf_text_filter_030); /* Lower-Special, Final_Sigma */
  tcase_add_test(tc, pdf_text_filter_031); /* Lower-Special After_Soft_Dotted */
  tcase_add_test(tc, pdf_text_filter_032); /* Lower-Special More_Above */
  tcase_add_test(tc, pdf_text_filter_033); /* Lower-Special Before_Dot  */
  tcase_add_test(tc, pdf_text_filter_034); /* Lower-Special After_I  */
  tcase_add_test(tc, pdf_text_filter_035); /* Lower-Special 'tr'  */
  tcase_add_test(tc, pdf_text_filter_036); /* Lower-Special After_I and 'tr'  */
  tcase_add_test(tc, pdf_text_filter_037); /* Lower-Special Not_Before_Dot  */
  tcase_add_test(tc, pdf_text_filter_038); /* Lower-Special Not Final_Sigma */
  tcase_add_test(tc, pdf_text_filter_039); /* Lower-Special Not After_Soft_Dotted */
  tcase_add_test(tc, pdf_text_filter_040); /* Lower-Special Not More_Above */
  tcase_add_test(tc, pdf_text_filter_041); /* Lower-Special Not Before_Dot  */
  tcase_add_test(tc, pdf_text_filter_042); /* Lower-Special Not After_I  */
  tcase_add_test(tc, pdf_text_filter_043); /* Lower-Special Not 'tr'  */
  tcase_add_test(tc, pdf_text_filter_044); /* Lower-Special Not After_I and 'tr'  */
  tcase_add_test(tc, pdf_text_filter_045); /* Lower-Special Not Not_Before_Dot  */

  /* Title case */
  tcase_add_test(tc, pdf_text_filter_046); /* Title */
  tcase_add_test(tc, pdf_text_filter_047); /* Title-Special */
  tcase_add_test(tc, pdf_text_filter_048); /* Title-Special, Final_Sigma */
  tcase_add_test(tc, pdf_text_filter_049); /* Title-Special After_Soft_Dotted */
  tcase_add_test(tc, pdf_text_filter_050); /* Title-Special More_Above */
  tcase_add_test(tc, pdf_text_filter_051); /* Title-Special Before_Dot  */
  tcase_add_test(tc, pdf_text_filter_052); /* Title-Special After_I  */
  tcase_add_test(tc, pdf_text_filter_053); /* Title-Special 'tr'  */
  tcase_add_test(tc, pdf_text_filter_054); /* Title-Special After_I and 'tr'  */
  tcase_add_test(tc, pdf_text_filter_055); /* Title-Special Not_Before_Dot  */
  tcase_add_test(tc, pdf_text_filter_056); /* Title-Special, Not Final_Sigma */
  tcase_add_test(tc, pdf_text_filter_057); /* Title-Special Not After_Soft_Dotted */
  tcase_add_test(tc, pdf_text_filter_058); /* Title-Special Not More_Above */
  tcase_add_test(tc, pdf_text_filter_059); /* Title-Special Not Before_Dot  */
  tcase_add_test(tc, pdf_text_filter_060); /* Title-Special Not After_I  */
  tcase_add_test(tc, pdf_text_filter_061); /* Title-Special Not 'tr'  */
  tcase_add_test(tc, pdf_text_filter_062); /* Title-Special Not After_I and 'tr'  */
  tcase_add_test(tc, pdf_text_filter_063); /* Title-Special Not Not_Before_Dot  */
  
  return tc;
}

/* End of pdf-text-filter.c */

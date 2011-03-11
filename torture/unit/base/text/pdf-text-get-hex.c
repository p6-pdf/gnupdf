/* -*- mode: C -*-
 *
 *       File:         pdf-text-get-hex.c
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_get_hex test case
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
 * Test: pdf_text_get_hex_001
 * Description:
 *   Get the contents of a non-empty text object in Hexadecimal representation
 *   in ASCII
 * Success conditions:
 *   1. The call to  pdf_text_get_hex should return a valid string, NUL
 *      terminated.
 *   2. The contents of the returned string must be the expected ones.
 *   3. The length of the string must be non-zero.
 */
START_TEST (pdf_text_get_hex_001)
{
  pdf_error_t *error = NULL;
  pdf_text_t *text;
  pdf_char_t *utf8data = (pdf_char_t *)"ab";
  pdf_char_t *expected_utf32le = (pdf_char_t *)"61:00:00:00:" "62:00:00:00";
  pdf_char_t *expected_utf32be = (pdf_char_t *)"00:00:00:61:" "00:00:00:62";
  pdf_char_t *expected;
  pdf_char_t *output_data;

  expected = (!pdf_text_test_big_endian_system () ?
              expected_utf32le :
              expected_utf32be);

  text = pdf_text_new_from_unicode (utf8data,
                                    strlen (utf8data),
                                    PDF_TEXT_UTF8,
                                    &error);
  fail_unless (text != NULL);
  fail_if (error != NULL);

  /* 1. The call to  pdf_text_get_hex should return a valid string, NUL
   *      terminated. */
  output_data = pdf_text_get_hex (text, ':', &error);
  fail_unless (output_data != NULL);
  fail_if (error != NULL);

  /* 2. The contents of the returned string must be the expected ones. */
  fail_unless (memcmp (output_data, expected, strlen (output_data)) == 0);

  /* 3. The length of the string must be non-zero.  */
  fail_unless (strlen (output_data) == strlen (expected));

  pdf_dealloc (output_data);
  pdf_text_destroy (text);
}
END_TEST

/*
 * Test: pdf_text_get_hex_002
 * Description:
 *   Get the contents of an empty text object in Hexadecimal representation in
 *   ASCII
 * Success conditions:
 *   1. The call to  pdf_text_get_hex should return a valid string, NUL
 *      terminated.
 *   2. The length of the string must be zero.
 */
START_TEST (pdf_text_get_hex_002)
{
  pdf_error_t *error = NULL;
  pdf_text_t *text;
  pdf_char_t *output_data;

  text = pdf_text_new (&error);

  /* 1. The call to  pdf_text_get_hex should return a valid string, NUL
   *      terminated. */
  output_data = pdf_text_get_hex (text, ':', &error);
  fail_unless (output_data != NULL);
  fail_if (error != NULL);

  /* 2. The length of the string must be zero. */
  fail_unless (strlen (output_data) == 0);

  pdf_dealloc (output_data);
  pdf_text_destroy (text);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_text_get_hex(void)
{
  TCase *tc = tcase_create ("pdf_text_get_hex");

  tcase_add_test (tc, pdf_text_get_hex_001);
  tcase_add_test (tc, pdf_text_get_hex_002);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-text-get-hex.c */

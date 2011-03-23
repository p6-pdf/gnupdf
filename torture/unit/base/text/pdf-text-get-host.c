/* -*- mode: C -*-
 *
 *       File:         pdf-text-get-host.c
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_get_host test case
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
 * Test: pdf_text_get_host_001
 * Description:
 *   Get the contents of a text object in a valid host encoding
 * Success conditions:
 *    1. The call to  pdf_text_get_host should return PDF_OK.
 *    2. The returned string must be the expected one.
 */
START_TEST (pdf_text_get_host_001)
{
#ifdef PDF_HOST_WIN32
  const pdf_char_t *host_enc = "CP20127"; /* us-ascii */
#else
  const pdf_char_t *host_enc = "us-ascii";
#endif
  extern const test_string_t ascii_strings[];
  int i;

  i = 0;
  while (ascii_strings[i].data)
    {
      pdf_error_t *error = NULL;
      pdf_text_t *text;
      pdf_char_t *data;
      pdf_size_t size;
      const pdf_char_t *expected_data;
      pdf_size_t expected_size;

      /* Set expected data and size */
      expected_data = (pdf_char_t *)ascii_strings[i].data;
      expected_size = ascii_strings[i].size;

      text = pdf_text_new_from_unicode ((pdf_char_t *) ascii_strings[i].utf32be_data,
                                        (pdf_size_t) ascii_strings[i].utf32be_size,
                                        PDF_TEXT_UTF32_BE,
                                        &error);
      fail_unless (text != NULL);
      fail_if (error != NULL);

      /* 1. The call to  pdf_text_get_host should return PDF_OK. */
      data = pdf_text_get_host (text,
                                host_enc,
                                &size,
                                &error);
      fail_unless (data != NULL);

      /* 2. The returned string must be the expected one. */
      fail_unless (size == expected_size);
      fail_unless (memcmp (expected_data, data, size) == 0);

      pdf_text_destroy (text);
      pdf_dealloc (data);

      ++i;
    }
}
END_TEST


/*
 * Test: pdf_text_get_host_002
 * Description:
 *   Get the contents of a text object in an invalid host encoding
 * Success conditions:
 *    1. The call to  pdf_text_get_host should NOT return PDF_OK.
 */
START_TEST (pdf_text_get_host_002)
{
#ifdef PDF_HOST_WIN32
  const pdf_char_t *host_enc = "CP17";
#else
  const pdf_char_t *host_enc = "invalid_host_enc";
#endif
  extern const test_string_t ascii_strings[];
  int i;

  i = 0;
  while (ascii_strings[i].data)
    {
      pdf_error_t *error = NULL;
      pdf_text_t *text;
      pdf_char_t *data;
      pdf_size_t size;
      const pdf_char_t *expected_data;
      pdf_size_t expected_size;

      /* Set expected data and size */
      expected_data = (pdf_char_t *)ascii_strings[i].data;
      expected_size = ascii_strings[i].size;

      text = pdf_text_new_from_unicode ((pdf_char_t *) ascii_strings[i].utf32be_data,
                                        (pdf_size_t) ascii_strings[i].utf32be_size,
                                        PDF_TEXT_UTF32_BE,
                                        &error);
      fail_unless (text != NULL);
      fail_if (error != NULL);

      /* 1. The call to  pdf_text_get_host should return PDF_OK. */
      data = pdf_text_get_host (text,
                                host_enc,
                                &size,
                                &error);
      fail_if (data != NULL);
      fail_unless (error != NULL);

      pdf_text_destroy (text);
      pdf_error_destroy (error);

      ++i;
    }
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_text_get_host (void)
{
  TCase *tc = tcase_create ("pdf_text_get_host");

  tcase_add_test (tc, pdf_text_get_host_001);
  tcase_add_test (tc, pdf_text_get_host_002);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-text-get-host.c */

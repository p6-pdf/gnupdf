/* -*- mode: C -*-
 *
 *       File:         pdf-text-set-pdfdocenc.c
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_set_pdfdocenc test case
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
 * Test: pdf_text_set_pdfdocenc_001
 * Description:
 *   Set the contents of a text object with an input valid PDF Doc Encoded
 *   string
 * Success conditions:
 *   1. The call to  pdf_text_set_pdfdocenc should return PDF_OK.
 *   2. The contents of the text object must be the expected ones.
 */
START_TEST (pdf_text_set_pdfdocenc_001)
{
  extern const test_string_t pdf_doc_encoding_strings[];
  int i;

  i = 0;
  while (pdf_doc_encoding_strings[i].data)
    {
      pdf_error_t *error = NULL;
      pdf_text_t *text;
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_size_t expected_size = 0;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;

      /* Set input data, NUL terminated!!!!! */
      input_size = (pdf_size_t)pdf_doc_encoding_strings[i].size + 1;
      input_data = (pdf_char_t *) pdf_alloc (input_size);
      fail_unless (input_data != NULL);
      memcpy (input_data, pdf_doc_encoding_strings[i].data, (input_size - 1));
      input_data[input_size-1] = '\0';

      /* Set expected data */
      expected_data = (pdf_char_t *)pdf_doc_encoding_strings[i].utf32be_data;
      expected_size = (pdf_size_t)pdf_doc_encoding_strings[i].utf32be_size;
      if (!pdf_text_test_big_endian_system ())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness (expected_data,
                                                                expected_size);
          /* Just in case... */
          fail_unless (expected_data != NULL);
        }

      text = pdf_text_new (&error);
      fail_unless (text != NULL);
      fail_if (error != NULL);

      /* 1. The call to pdf_text_set_pdfdocenc should return PDF_TRUE. */
      fail_unless (pdf_text_set_pdfdocenc (text,
                                           input_data,
                                           &error) == PDF_TRUE);
      fail_if (error != NULL);

      /* 2. The contents of the text object must be the expected ones. */
      actual_data = pdf_text_get_unicode (text,
                                          PDF_TEXT_UTF32_HE,
                                          PDF_TEXT_UNICODE_NO_OPTION,
                                          &actual_size,
                                          &error);
      fail_unless (actual_data != NULL);
      fail_if (error != NULL);

      PRINT_CONTENTS (__FUNCTION__, i, text, expected_data, expected_size, 0);

      fail_unless (actual_size == expected_size);
      fail_unless (memcmp (actual_data, expected_data, expected_size) == 0);

      pdf_text_destroy (text);

      if (expected_free)
        pdf_dealloc (expected_data);
      pdf_dealloc (actual_data);

      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_set_pdfdocenc_002
 * Description:
 *   Set the contents of a text object with an input empty PDF Doc Encoded
 *   string
 * Success conditions:
 *   1. The call to  pdf_text_set_pdfdocenc should return PDF_TRUE.
 *   2. The contents of the text object must be empty.
 */
START_TEST (pdf_text_set_pdfdocenc_002)
{
  pdf_error_t *error = NULL;
  pdf_text_t *text;

  text = pdf_text_new (&error);
  fail_unless (text != NULL);
  fail_if (error != NULL);

  /* 1. The call to  pdf_text_new_from_host should return PDF_OK. */
  fail_unless (pdf_text_set_pdfdocenc (text,
                                       (pdf_char_t *)"",
                                       &error) == PDF_TRUE);
  fail_if (error != NULL);

  /* 2. The contents of the text object must be empty. */
  fail_unless (pdf_text_empty_p (text) == PDF_TRUE);

  pdf_text_destroy (text);
}
END_TEST

/*
 * Test: pdf_text_set_pdfdocenc_003
 * Description:
 *   Set the contents of a text object with an input invalid PDF Doc Encoded
 *   string
 * Success conditions:
 *   1. The call to  pdf_text_set_pdfdocenc should NOT return PDF_OK.
 */
START_TEST (pdf_text_set_pdfdocenc_003)
{
  /* 0x9F is Undefined in PDF Doc Encoding */
  const pdf_char_t *invalid_pdfdocenc = (pdf_char_t *)"\x9D\x9E\x9F\x00";
  pdf_error_t *error = NULL;
  pdf_text_t *text;

  text = pdf_text_new (&error);
  fail_unless (text != NULL);
  fail_if (error != NULL);

  /* 1. The call to  pdf_text_set_pdfdocenc should NOT return PDF_TRUE. */
  fail_if (pdf_text_set_pdfdocenc (text,
                                       invalid_pdfdocenc,
                                       &error ) == PDF_TRUE);
  fail_unless (error != NULL);

  pdf_error_destroy (error);
  pdf_text_destroy (text);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_text_set_pdfdocenc (void)
{
  TCase *tc = tcase_create ("pdf_text_set_pdfdocenc");

  tcase_add_test (tc, pdf_text_set_pdfdocenc_001);
  tcase_add_test (tc, pdf_text_set_pdfdocenc_002);
  tcase_add_test (tc, pdf_text_set_pdfdocenc_003);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-text-set-pdfdocenc.c */

/* -*- mode: C -*-
 *
 *       File:         pdf-text-replace-ascii.c
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_replace_ascii test case
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

typedef struct {
  const pdf_char_t *input;
  const pdf_char_t *old_pattern;
  const pdf_char_t *new_pattern;
  const pdf_char_t *expected;
} test_data_t;

static const test_data_t test_strings[] = {
  {
    /* old > new, 1 replacement at least */
    "Linux rules Linux rules Linux",
    "Linux", "GNU",
    "GNU rules GNU rules GNU"
  },
  {
    /* old < new, 1 replacement at least */
    "Linux rules Linux rules Linux",
    "Linux", "GNU/Linux",
    "GNU/Linux rules GNU/Linux rules GNU/Linux"
  },
  {
    /* old = new, 1 replacement at least */
    "Windows7 sucks Windows7 sucks Windows7",
    "Windows7", "Mac OS X",
    "Mac OS X sucks Mac OS X sucks Mac OS X"
  },
  {
    /* new = 0, 1 replacement at least */
    "GNU rules GNU rules GNU",
    "GNU", "",
    " rules  rules "
  },
  {
    /* old > new, 0 replacements */
    "Simple test string with no old pattern",
    "Linux", "GNU",
    "Simple test string with no old pattern"
  },
  {
    /* old < new, 0 replacements */
    "Simple test string with no old pattern",
    "Linux", "GNU/Linux",
    "Simple test string with no old pattern"
  },
  {
    /* old = new, 0 replacements */
    "Simple test string with no old pattern",
    "Windows7", "Mac OS X",
    "Simple test string with no old pattern"
  },
  {
    /* new = 0, 0 replacements */
    "Simple test string with no old pattern",
    "GNU", "",
    "Simple test string with no old pattern"
  },
  { NULL, NULL, NULL, NULL }
};

/*
 * Test: pdf_text_replace_ascii_001
 * Description:
 *   Replace an old non-empty ASCII pattern with a new empty/non-empty ASCII
 *   pattern.
 * Success conditions:
 *   1. The call to  pdf_text_replace_ascii should return PDF_TRUE.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST (pdf_text_replace_ascii_001)
{
  pdf_u32_t i;

  i = 0;
  while (test_strings[i].input)
    {
      pdf_error_t *error = NULL;
      pdf_text_t *text;
      pdf_text_t *new_pattern;
      pdf_text_t *old_pattern;
      pdf_char_t *output_string;

      text = pdf_text_new_from_unicode (test_strings[i].input,
                                        strlen (test_strings[i].input),
                                        PDF_TEXT_UTF8,
                                        &error);

      /* 1. The call to  pdf_text_replace_ascii should return PDF_OK. */
      fail_unless (pdf_text_replace_ascii (text,
                                           test_strings[i].new_pattern,
                                           test_strings[i].old_pattern,
                                           &error) == PDF_TRUE);
      fail_if (error != NULL);

      /* 2. The contents of the output text object must be the expected ones. */
      output_string = pdf_text_get_unicode (text,
                                            PDF_TEXT_UTF8,
                                            PDF_TEXT_UNICODE_WITH_NUL_SUFFIX,
                                            NULL,
                                            &error);
      fail_unless (output_string != NULL);
      fail_if (error != NULL);
      fail_if (strlen (output_string) != strlen (test_strings[i].expected));
      fail_if (strcmp (output_string, test_strings[i].expected) != 0);

      pdf_text_destroy (text);
      pdf_dealloc (output_string);

      ++i;
    }
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_text_replace_ascii (void)
{
  TCase *tc = tcase_create ("pdf_text_replace_ascii");

  tcase_add_test (tc, pdf_text_replace_ascii_001);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-text-replace-ascii.c */

/* -*- mode: C -*-
 *
 *       File:         pdf-text-replace.c
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_replace test case
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

/*
 * Test: pdf_text_replace_001
 * Description:
 *   Replace an old non-empty pattern with a new non-empty pattern of
 *   different sizes (old > new). At least one replacement is done.
 * Success conditions:
 *   1. The call to  pdf_text_replace should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_replace_001)
{
  pdf_text_t text;
  pdf_text_t new_pattern;
  pdf_text_t old_pattern;
  const pdf_char_t *new_pattern_ascii = (pdf_char_t *)"GNU";
  const pdf_char_t *old_pattern_ascii = (pdf_char_t *)"Linux";
  const pdf_char_t *input_string = (pdf_char_t *) \
                                   "Linux rules Linux rules Linux";
  const pdf_char_t *expected_string = (pdf_char_t *) \
                                      "GNU rules GNU rules GNU";
  pdf_char_t *output_string = NULL;

  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);

  
  fail_if(pdf_text_new_from_pdf_string(input_string,
                                       strlen((char *)input_string),
                                       NULL, NULL,
                                       &text) != PDF_OK);
  fail_if(pdf_text_new_from_pdf_string(old_pattern_ascii,
                                       strlen((char *)old_pattern_ascii),
                                       NULL, NULL,
                                       &old_pattern) != PDF_OK);
  fail_if(pdf_text_new_from_pdf_string(new_pattern_ascii,
                                       strlen((char *)new_pattern_ascii),
                                       NULL, NULL,
                                       &new_pattern) != PDF_OK);

  /* 1. The call to  pdf_text_replace should return PDF_OK. */
  fail_unless(pdf_text_replace(text, new_pattern, old_pattern) == PDF_OK);

  /* 2. The contents of the output text object must be the expected ones. */
  fail_if(pdf_text_get_pdfdocenc(&output_string, text) != PDF_OK);
  fail_if(output_string == NULL);
  fail_if(strlen((char *)output_string) != strlen((char *)expected_string));
  fail_if(strcmp((char *)output_string, (char *)expected_string) != 0);
  
  pdf_text_destroy(text);
  pdf_text_destroy(new_pattern);
  pdf_text_destroy(old_pattern);
  pdf_dealloc(output_string);
}
END_TEST


/*
 * Test: pdf_text_replace_002
 * Description:
 *   Replace an old non-empty pattern with a new non-empty pattern of
 *   different sizes (old < new). At least one replacement is done.
 * Success conditions:
 *   1. The call to  pdf_text_replace should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_replace_002)
{
  pdf_text_t text;
  pdf_text_t new_pattern;
  pdf_text_t old_pattern;
  const pdf_char_t *new_pattern_ascii = (pdf_char_t *)"GNU/Linux";
  const pdf_char_t *old_pattern_ascii = (pdf_char_t *)"Linux";
  const pdf_char_t *input_string = (pdf_char_t *) \
                                   "Linux rules Linux rules Linux";
  const pdf_char_t *expected_string = (pdf_char_t *) \
                                      "GNU/Linux rules GNU/Linux rules "
                                      "GNU/Linux";
  pdf_char_t *output_string = NULL;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  
  fail_if(pdf_text_new_from_pdf_string(input_string,
                                       strlen((char *)input_string),
                                       NULL, NULL,
                                       &text) != PDF_OK);
  fail_if(pdf_text_new_from_pdf_string(old_pattern_ascii,
                                       strlen((char *)old_pattern_ascii),
                                       NULL, NULL,
                                       &old_pattern) != PDF_OK);
  fail_if(pdf_text_new_from_pdf_string(new_pattern_ascii,
                                       strlen((char *)new_pattern_ascii),
                                       NULL, NULL,
                                       &new_pattern) != PDF_OK);
  
  /* 1. The call to  pdf_text_replace should return PDF_OK. */
  fail_unless(pdf_text_replace(text, new_pattern, old_pattern) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_if(pdf_text_get_pdfdocenc(&output_string, text) != PDF_OK);
  fail_if(output_string == NULL);
  fail_if(strlen((char *)output_string) != strlen((char *)expected_string));
  fail_if(strcmp((char *)output_string, (char *)expected_string) != 0);
  
  pdf_text_destroy(text);
  pdf_text_destroy(new_pattern);
  pdf_text_destroy(old_pattern);
  pdf_dealloc(output_string);
}
END_TEST


/*
 * Test: pdf_text_replace_003
 * Description:
 *   Replace an old non-empty pattern with a new non-empty pattern of same
 *   sizes. At least one replacement is done.
 * Success conditions:
 *   1. The call to  pdf_text_replace should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_replace_003)
{
  pdf_text_t text;
  pdf_text_t new_pattern;
  pdf_text_t old_pattern;
  const pdf_char_t *old_pattern_ascii = (pdf_char_t *)"Windows";
  const pdf_char_t *new_pattern_ascii = (pdf_char_t *)"FreeBSD";
  const pdf_char_t *input_string = (pdf_char_t *) \
                                   "Windows rules Windows rules Windows"; 
  const pdf_char_t *expected_string = (pdf_char_t *) \
                                   "FreeBSD rules FreeBSD rules FreeBSD";
  pdf_char_t *output_string = NULL;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  
  fail_if(pdf_text_new_from_pdf_string(input_string,
                                       strlen((char *)input_string),
                                       NULL, NULL,
                                       &text) != PDF_OK);
  fail_if(pdf_text_new_from_pdf_string(old_pattern_ascii,
                                       strlen((char *)old_pattern_ascii),
                                       NULL, NULL,
                                       &old_pattern) != PDF_OK);
  fail_if(pdf_text_new_from_pdf_string(new_pattern_ascii,
                                       strlen((char *)new_pattern_ascii),
                                       NULL, NULL,
                                       &new_pattern) != PDF_OK);
  
  /* 1. The call to  pdf_text_replace should return PDF_OK. */
  fail_unless(pdf_text_replace(text, new_pattern, old_pattern) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_if(pdf_text_get_pdfdocenc(&output_string, text) != PDF_OK);
  fail_if(output_string == NULL);
  fail_if(strlen((char *)output_string) != strlen((char *)expected_string));
  fail_if(strcmp((char *)output_string, (char *)expected_string) != 0);
  
  pdf_text_destroy(text);
  pdf_text_destroy(new_pattern);
  pdf_text_destroy(old_pattern);
  pdf_dealloc(output_string);
}
END_TEST


/*
 * Test: pdf_text_replace_004
 * Description:
 *   Replace an old non-empty pattern with a new empty pattern (remove old
 *   pattern from text). At least one replacement is done.
 * Success conditions:
 *   1. The call to  pdf_text_replace should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST(pdf_text_replace_004)
{
  pdf_text_t text;
  pdf_text_t new_pattern;
  pdf_text_t old_pattern;
  const pdf_char_t *old_pattern_ascii = (pdf_char_t *)"GNU";
  const pdf_char_t *new_pattern_ascii = (pdf_char_t *)"";
  const pdf_char_t *input_string = (pdf_char_t *) "GNU rules GNU rules GNU"; 
  const pdf_char_t *expected_string = (pdf_char_t *) " rules  rules "; 
  pdf_char_t *output_string = NULL;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  
  fail_if(pdf_text_new_from_pdf_string(input_string,
                                       strlen((char *)input_string),
                                       NULL, NULL,
                                       &text) != PDF_OK);
  fail_if(pdf_text_new_from_pdf_string(old_pattern_ascii,
                                       strlen((char *)old_pattern_ascii),
                                       NULL, NULL,
                                       &old_pattern) != PDF_OK);
  fail_if(pdf_text_new_from_pdf_string(new_pattern_ascii,
                                       strlen((char *)new_pattern_ascii),
                                       NULL, NULL,
                                       &new_pattern) != PDF_OK);
  
  /* 1. The call to  pdf_text_replace should return PDF_OK. */
  fail_unless(pdf_text_replace(text, new_pattern, old_pattern) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_if(pdf_text_get_pdfdocenc(&output_string, text) != PDF_OK);
  fail_if(output_string == NULL);
  fail_if(strlen((char *)output_string) != strlen((char *)expected_string));
  fail_if(strcmp((char *)output_string, (char *)expected_string) != 0);
  
  pdf_text_destroy(text);
  pdf_text_destroy(new_pattern);
  pdf_text_destroy(old_pattern);
  pdf_dealloc(output_string);
}
END_TEST


/*
 * Test: pdf_text_replace_005
 * Description:
 *   Replace an old non-empty pattern with a new non-empty pattern of different
 *   sizes (old > new). No replacement is done.
 * Success conditions:
 *   1. The call to  pdf_text_replace should return PDF_OK.
 *   2. The contents of the output text object remain unchanged.
 */
START_TEST(pdf_text_replace_005)
{
  pdf_text_t text;
  pdf_text_t new_pattern;
  pdf_text_t old_pattern;
  const pdf_char_t *new_pattern_ascii = (pdf_char_t *)"GNU";
  const pdf_char_t *old_pattern_ascii = (pdf_char_t *)"Linux";
  const pdf_char_t *input_string = (pdf_char_t *) \
                                   "Simple test string with no old pattern";
  const pdf_char_t *expected_string = (pdf_char_t *) \
                                      "Simple test string with no old pattern";
  pdf_char_t *output_string = NULL;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  
  fail_if(pdf_text_new_from_pdf_string(input_string,
                                       strlen((char *)input_string),
                                       NULL, NULL,
                                       &text) != PDF_OK);
  fail_if(pdf_text_new_from_pdf_string(old_pattern_ascii,
                                       strlen((char *)old_pattern_ascii),
                                       NULL, NULL,
                                       &old_pattern) != PDF_OK);
  fail_if(pdf_text_new_from_pdf_string(new_pattern_ascii,
                                       strlen((char *)new_pattern_ascii),
                                       NULL, NULL,
                                       &new_pattern) != PDF_OK);
  
  /* 1. The call to  pdf_text_replace should return PDF_OK. */
  fail_unless(pdf_text_replace(text, new_pattern, old_pattern) == PDF_OK);
  
  /* 2. The contents of the output text object remain unchanged. */
  fail_if(pdf_text_get_pdfdocenc(&output_string, text) != PDF_OK);
  fail_if(output_string == NULL);
  fail_if(strlen((char *)output_string) != strlen((char *)expected_string));
  fail_if(strcmp((char *)output_string, (char *)expected_string) != 0);
  
  pdf_text_destroy(text);
  pdf_text_destroy(new_pattern);
  pdf_text_destroy(old_pattern);
  pdf_dealloc(output_string);
}
END_TEST


/*
 * Test: pdf_text_replace_006
 * Description:
 *   Replace an old non-empty pattern with a new non-empty pattern of different
 *   sizes (old < new). No replacement is done.
 * Success conditions:
 *   1. The call to  pdf_text_replace should return PDF_OK.
 *   2. The contents of the output text object remain unchanged.
 */
START_TEST(pdf_text_replace_006)
{
  pdf_text_t text;
  pdf_text_t new_pattern;
  pdf_text_t old_pattern;
  const pdf_char_t *new_pattern_ascii = (pdf_char_t *)"GNU/Linux";
  const pdf_char_t *old_pattern_ascii = (pdf_char_t *)"Linux";
  const pdf_char_t *input_string = (pdf_char_t *) \
                                   "Simple test string with no old pattern";
  const pdf_char_t *expected_string = (pdf_char_t *) \
                                      "Simple test string with no old pattern";
  pdf_char_t *output_string = NULL;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  
  fail_if(pdf_text_new_from_pdf_string(input_string,
                                       strlen((char *)input_string),
                                       NULL, NULL,
                                       &text) != PDF_OK);
  fail_if(pdf_text_new_from_pdf_string(old_pattern_ascii,
                                       strlen((char *)old_pattern_ascii),
                                       NULL, NULL,
                                       &old_pattern) != PDF_OK);
  fail_if(pdf_text_new_from_pdf_string(new_pattern_ascii,
                                       strlen((char *)new_pattern_ascii),
                                       NULL, NULL,
                                       &new_pattern) != PDF_OK);
  
  /* 1. The call to  pdf_text_replace should return PDF_OK. */
  fail_unless(pdf_text_replace(text, new_pattern, old_pattern) == PDF_OK);
  
  /* 2. The contents of the output text object remain unchanged. */
  fail_if(pdf_text_get_pdfdocenc(&output_string, text) != PDF_OK);
  fail_if(output_string == NULL);
  fail_if(strlen((char *)output_string) != strlen((char *)expected_string));
  fail_if(strcmp((char *)output_string, (char *)expected_string) != 0);
  
  pdf_text_destroy(text);
  pdf_text_destroy(new_pattern);
  pdf_text_destroy(old_pattern);
  pdf_dealloc(output_string);
}
END_TEST


/*
 * Test: pdf_text_replace_007
 * Description:
 *   Replace an old non-empty pattern with a new non-empty pattern of same
 *   sizes. No replacement is done.
 * Success conditions:
 *   1. The call to  pdf_text_replace should return PDF_OK.
 *   2. The contents of the output text object remain unchanged.
 */
START_TEST(pdf_text_replace_007)
{
  pdf_text_t text;
  pdf_text_t new_pattern;
  pdf_text_t old_pattern;
  const pdf_char_t *old_pattern_ascii = (pdf_char_t *)"Windows";
  const pdf_char_t *new_pattern_ascii = (pdf_char_t *)"FreeBSD";
  const pdf_char_t *input_string = (pdf_char_t *) \
                                   "Simple test string with no old pattern";
  const pdf_char_t *expected_string = (pdf_char_t *) \
                                   "Simple test string with no old pattern";
  pdf_char_t *output_string = NULL;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  
  fail_if(pdf_text_new_from_pdf_string(input_string,
                                       strlen((char *)input_string),
                                       NULL, NULL,
                                       &text) != PDF_OK);
  fail_if(pdf_text_new_from_pdf_string(old_pattern_ascii,
                                       strlen((char *)old_pattern_ascii),
                                       NULL, NULL,
                                       &old_pattern) != PDF_OK);
  fail_if(pdf_text_new_from_pdf_string(new_pattern_ascii,
                                       strlen((char *)new_pattern_ascii),
                                       NULL, NULL,
                                       &new_pattern) != PDF_OK);
  
  /* 1. The call to  pdf_text_replace should return PDF_OK. */
  fail_unless(pdf_text_replace(text, new_pattern, old_pattern) == PDF_OK);
  
  /* 2. The contents of the output text object remain unchanged. */
  fail_if(pdf_text_get_pdfdocenc(&output_string, text) != PDF_OK);
  fail_if(output_string == NULL);
  fail_if(strlen((char *)output_string) != strlen((char *)expected_string));
  fail_if(strcmp((char *)output_string, (char *)expected_string) != 0);
  
  pdf_text_destroy(text);
  pdf_text_destroy(new_pattern);
  pdf_text_destroy(old_pattern);
  pdf_dealloc(output_string);
}
END_TEST


/*
 * Test: pdf_text_replace_008
 * Description:
 *   Replace an old non-empty pattern with a new empty pattern (remove old
 *   pattern from text). No replacement is done.
 * Success conditions:
 *   1. The call to  pdf_text_replace should return PDF_OK.
 *   2. The contents of the output text object remain unchanged.
 */
START_TEST(pdf_text_replace_008)
{
  pdf_text_t text;
  pdf_text_t new_pattern;
  pdf_text_t old_pattern;
  const pdf_char_t *old_pattern_ascii = (pdf_char_t *)"GNU";
  const pdf_char_t *new_pattern_ascii = (pdf_char_t *)"";
  const pdf_char_t *input_string = (pdf_char_t *) \
                                   "Simple test string with no old pattern";
  const pdf_char_t *expected_string = (pdf_char_t *) \
                                      "Simple test string with no old pattern";
  pdf_char_t *output_string = NULL;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  
  fail_if(pdf_text_new_from_pdf_string(input_string,
                                       strlen((char *)input_string),
                                       NULL, NULL,
                                       &text) != PDF_OK);
  fail_if(pdf_text_new_from_pdf_string(old_pattern_ascii,
                                       strlen((char *)old_pattern_ascii),
                                       NULL, NULL,
                                       &old_pattern) != PDF_OK);
  fail_if(pdf_text_new_from_pdf_string(new_pattern_ascii,
                                       strlen((char *)new_pattern_ascii),
                                       NULL, NULL,
                                       &new_pattern) != PDF_OK);
  
  /* 1. The call to  pdf_text_replace should return PDF_OK. */
  fail_unless(pdf_text_replace(text, new_pattern, old_pattern) == PDF_OK);
  
  /* 2. The contents of the output text object remain unchanged. */
  fail_if(pdf_text_get_pdfdocenc(&output_string, text) != PDF_OK);
  fail_if(output_string == NULL);
  fail_if(strlen((char *)output_string) != strlen((char *)expected_string));
  fail_if(strcmp((char *)output_string, (char *)expected_string) != 0);
  
  pdf_text_destroy(text);
  pdf_text_destroy(new_pattern);
  pdf_text_destroy(old_pattern);
  pdf_dealloc(output_string);
}
END_TEST


/*
 * Test: pdf_text_replace_009
 * Description:
 *   Replace an old empty pattern with a new non-empty pattern (old pattern
 *   can't be empty!)
 * Success conditions:
 *   1. The call to  pdf_text_replace should NOT return PDF_OK.
 */
START_TEST(pdf_text_replace_009)
{
  pdf_text_t text;
  pdf_text_t new_pattern;
  pdf_text_t old_pattern;
  const pdf_char_t *old_pattern_ascii = (pdf_char_t *)"";
  const pdf_char_t *new_pattern_ascii = (pdf_char_t *)"GNU";
  const pdf_char_t *input_string = (pdf_char_t *) \
                                   "Simple test string with no old pattern";

  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  
  fail_if(pdf_text_new_from_pdf_string(input_string,
                                       strlen((char *)input_string),
                                       NULL, NULL,
                                       &text) != PDF_OK);
  fail_if(pdf_text_new_from_pdf_string(old_pattern_ascii,
                                       strlen((char *)old_pattern_ascii),
                                       NULL, NULL,
                                       &old_pattern) != PDF_OK);
  fail_if(pdf_text_new_from_pdf_string(new_pattern_ascii,
                                       strlen((char *)new_pattern_ascii),
                                       NULL, NULL,
                                       &new_pattern) != PDF_OK);
  
  /* 1. The call to  pdf_text_replace should NOT return PDF_OK. */
  fail_unless(pdf_text_replace(text, new_pattern, old_pattern) != PDF_OK);

  pdf_text_destroy(text);
  pdf_text_destroy(new_pattern);
  pdf_text_destroy(old_pattern);
}
END_TEST


/*
 * Test: pdf_text_replace_010
 * Description:
 *   Replace an old non-empty pattern with a new non-empty pattern in an
 *   empty text object.
 * Success conditions:
 *   1. The call to  pdf_text_replace should return PDF_OK.
 *   2. The contents of the output text object remain empty.
 */
START_TEST(pdf_text_replace_010)
{
  pdf_text_t text;
  pdf_text_t new_pattern;
  pdf_text_t old_pattern;
  const pdf_char_t *old_pattern_ascii = (pdf_char_t *)"Linux";
  const pdf_char_t *new_pattern_ascii = (pdf_char_t *)"GNU/Linux";
  const pdf_char_t *input_string = (pdf_char_t *) "";
  pdf_char_t *output_string = NULL;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  
  fail_if(pdf_text_new_from_pdf_string(input_string,
                                       strlen((char *)input_string),
                                       NULL, NULL,
                                       &text) != PDF_OK);
  fail_if(pdf_text_new_from_pdf_string(old_pattern_ascii,
                                       strlen((char *)old_pattern_ascii),
                                       NULL, NULL,
                                       &old_pattern) != PDF_OK);
  fail_if(pdf_text_new_from_pdf_string(new_pattern_ascii,
                                       strlen((char *)new_pattern_ascii),
                                       NULL, NULL,
                                       &new_pattern) != PDF_OK);
  
  /* 1. The call to  pdf_text_replace should return PDF_OK. */
  fail_unless(pdf_text_replace(text, new_pattern, old_pattern) == PDF_OK);
  
  /* 2. The contents of the output text object remain empty. */
  fail_if(pdf_text_get_pdfdocenc(&output_string, text) != PDF_OK);
  fail_if(output_string == NULL);
  fail_if(strlen((char *)output_string) != 0);
  
  pdf_text_destroy(text);
  pdf_text_destroy(new_pattern);
  pdf_text_destroy(old_pattern);
  pdf_dealloc(output_string);
}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_text_replace (void)
{
  TCase *tc = tcase_create("pdf_text_replace");
  tcase_add_test(tc, pdf_text_replace_001);
  tcase_add_test(tc, pdf_text_replace_002);
  tcase_add_test(tc, pdf_text_replace_003);
  tcase_add_test(tc, pdf_text_replace_004);
  tcase_add_test(tc, pdf_text_replace_005);
  tcase_add_test(tc, pdf_text_replace_006);
  tcase_add_test(tc, pdf_text_replace_007);
  tcase_add_test(tc, pdf_text_replace_008);
  tcase_add_test(tc, pdf_text_replace_009);
  tcase_add_test(tc, pdf_text_replace_010);
  return tc;
}

/* End of pdf-text-replace.c */

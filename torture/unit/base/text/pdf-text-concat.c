/* -*- mode: C -*- Time-stamp: ""
 *
 *       File:         pdf-text-concat.c
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_concat test case
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
 * Test: pdf_text_concat_001
 * Description:
 *   Concatenate two non-empty text objects with the same lang/country
 *   information. Don't override langinfo.
 * Success conditions:
 *   1. The call to  pdf_text_concat should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 *   3. The lang/country information in the output object must remain unchanged.
 */
START_TEST(pdf_text_concat_001)
{
  pdf_char_t *output_data = NULL;
  pdf_text_t text1 = NULL;
  pdf_text_t text2 = NULL;
  pdf_char_t *country = (pdf_char_t *)"GB";
  pdf_char_t *language = (pdf_char_t *)"en";
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  text1 = pdf_text_new();
  fail_if(text1 == NULL);
  text2 = pdf_text_new();
  fail_if(text2 == NULL);

  fail_if(pdf_text_set_pdfdocenc(text1, (pdf_char_t *)"GNU's not") != PDF_OK);
  fail_if(pdf_text_set_pdfdocenc(text2, (pdf_char_t *)" Unix") != PDF_OK);
  fail_if(pdf_text_set_language(text1, language) != PDF_OK);
  fail_if(pdf_text_set_language(text2, language) != PDF_OK);
  fail_if(pdf_text_set_country(text1, country) != PDF_OK);
  fail_if(pdf_text_set_country(text2, country) != PDF_OK);

  /* 1. The call to  pdf_text_concat should return PDF_OK. */
  fail_unless(pdf_text_concat(text1, text2, PDF_FALSE) == PDF_OK);

  /* 2. The contents of the output text object must be the expected ones. */
  fail_if(pdf_text_get_pdfdocenc(&output_data, text1) != PDF_OK);
  fail_unless(strlen((char *)output_data) == strlen("GNU's not Unix"));
  fail_unless(strcmp((char *)output_data, "GNU's not Unix") == 0);
  
  /* 3. The lang/country information in the output object must remain
   *    unchanged. */
  fail_unless(strcmp((char *)pdf_text_get_language(text1),
                     (char *)language) == 0);
  fail_unless(strcmp((char *)pdf_text_get_country(text1),
                     (char *)country) == 0);
  
  pdf_text_destroy(text1);
  pdf_text_destroy(text2);
  pdf_dealloc(output_data);
}
END_TEST


/*
 * Test: pdf_text_concat_002
 * Description:
 *   Concatenate two non-empty text objects without lang/country
 *   information. Don't override langinfo.
 * Success conditions:
 *   1. The call to  pdf_text_concat should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 *   3. The lang/country information in the output object must be empty.
 */
START_TEST(pdf_text_concat_002)
{
  pdf_char_t *output_data = NULL;
  pdf_text_t text1 = NULL;
  pdf_text_t text2 = NULL;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  text1 = pdf_text_new();
  fail_if(text1 == NULL);
  text2 = pdf_text_new();
  fail_if(text2 == NULL);
  
  fail_if(pdf_text_set_pdfdocenc(text1, (pdf_char_t *)"GNU's not") != PDF_OK);
  fail_if(pdf_text_set_pdfdocenc(text2, (pdf_char_t *)" Unix") != PDF_OK);
  
  /* 1. The call to  pdf_text_concat should return PDF_OK. */
  fail_unless(pdf_text_concat(text1, text2, PDF_FALSE) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_if(pdf_text_get_pdfdocenc(&output_data, text1) != PDF_OK);
  fail_unless(strlen((char *)output_data) == strlen("GNU's not Unix"));
  fail_unless(strcmp((char *)output_data, "GNU's not Unix") == 0);
  
  /* 3. The lang/country information in the output object must be empty. */
  fail_unless(strlen((char *)pdf_text_get_language(text1)) == 0);
  fail_unless(strlen((char *)pdf_text_get_country(text1)) == 0);
  
  pdf_text_destroy(text1);
  pdf_text_destroy(text2);
  pdf_dealloc(output_data);
}
END_TEST


/*
 * Test: pdf_text_concat_003
 * Description:
 *   Concatenate two non-empty text objects with different lang/country
 *   information. Don't override langinfo.
 * Success conditions:
 *   1. The call to  pdf_text_concat should NOT return PDF_OK.
 *   2. The contents of the output text object must remain unchanged.
 */
START_TEST(pdf_text_concat_003)
{
  pdf_char_t *output_data = NULL;
  pdf_text_t text1 = NULL;
  pdf_text_t text2 = NULL;
  pdf_char_t *country1 = (pdf_char_t *)"GB";
  pdf_char_t *language1 = (pdf_char_t *)"en";
  pdf_char_t *country2 = (pdf_char_t *)"FR";
  pdf_char_t *language2 = (pdf_char_t *)"fr";

  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  text1 = pdf_text_new();
  fail_if(text1 == NULL);
  text2 = pdf_text_new();
  fail_if(text2 == NULL);
  
  fail_if(pdf_text_set_pdfdocenc(text1, (pdf_char_t *)"GNU's not") != PDF_OK);
  fail_if(pdf_text_set_pdfdocenc(text2, (pdf_char_t *)" Unix") != PDF_OK);
  fail_if(pdf_text_set_language(text1, language1) != PDF_OK);
  fail_if(pdf_text_set_language(text2, language2) != PDF_OK);
  fail_if(pdf_text_set_country(text1, country1) != PDF_OK);
  fail_if(pdf_text_set_country(text2, country2) != PDF_OK);
  
  /* 1. The call to  pdf_text_concat should NOT return PDF_OK. */
  fail_unless(pdf_text_concat(text1, text2, PDF_FALSE) != PDF_OK);
  
  /* 2. The contents of the output text object must remain unchanged. */
  fail_if(pdf_text_get_pdfdocenc(&output_data, text1) != PDF_OK);
  fail_unless(strlen((char *)output_data) == strlen("GNU's not"));
  fail_unless(strcmp((char *)output_data, "GNU's not") == 0);
  fail_unless(strcmp((char *)pdf_text_get_language(text1),
                     (char *)language1) == 0);
  fail_unless(strcmp((char *)pdf_text_get_country(text1),
                     (char *)country1) == 0);
  
  pdf_text_destroy(text1);
  pdf_text_destroy(text2);
  pdf_dealloc(output_data);
}
END_TEST

/*
 * Test: pdf_text_concat_004
 * Description:
 *   Concatenate two empty text objects with the same lang/country
 *   information. Don't override langinfo.
 * Success conditions:
 *   1. The call to  pdf_text_concat should return PDF_OK.
 *   2. The contents of the output text object should be empty.
 *   3. The lang/country information in the output object must remain unchanged.
 */
START_TEST(pdf_text_concat_004)
{
  pdf_char_t *output_data = NULL;
  pdf_text_t text1 = NULL;
  pdf_text_t text2 = NULL;
  pdf_char_t *country = (pdf_char_t *)"GB";
  pdf_char_t *language = (pdf_char_t *)"en";
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  text1 = pdf_text_new();
  fail_if(text1 == NULL);
  text2 = pdf_text_new();
  fail_if(text2 == NULL);
  
  fail_if(pdf_text_set_pdfdocenc(text1, (pdf_char_t *)"") != PDF_OK);
  fail_if(pdf_text_set_pdfdocenc(text2, (pdf_char_t *)"") != PDF_OK);
  fail_if(pdf_text_set_language(text1, language) != PDF_OK);
  fail_if(pdf_text_set_language(text2, language) != PDF_OK);
  fail_if(pdf_text_set_country(text1, country) != PDF_OK);
  fail_if(pdf_text_set_country(text2, country) != PDF_OK);
  
  /* 1. The call to  pdf_text_concat should return PDF_OK. */
  fail_unless(pdf_text_concat(text1, text2, PDF_FALSE) == PDF_OK);
  
  /* 2. The contents of the output text object should be empty. */
  fail_if(pdf_text_get_pdfdocenc(&output_data, text1) != PDF_OK);
  fail_unless(strlen((char *)output_data) == 0);
  
  /* 3. The lang/country information in the output object must remain
   *    unchanged. */
  fail_unless(strcmp((char *)pdf_text_get_language(text1),
                     (char *)language) == 0);
  fail_unless(strcmp((char *)pdf_text_get_country(text1),
                     (char *)country) == 0);
  
  pdf_text_destroy(text1);
  pdf_text_destroy(text2);
  pdf_dealloc(output_data);
}
END_TEST


/*
 * Test: pdf_text_concat_005
 * Description:
 *   Concatenate two empty text objects without lang/country
 *   information. Don't override langinfo.
 * Success conditions:
 *   1. The call to  pdf_text_concat should return PDF_OK.
 *   2. The contents of the output text object should be empty.
 *   3. The lang/country information in the output object must be empty.
 */
START_TEST(pdf_text_concat_005)
{
  pdf_char_t *output_data = NULL;
  pdf_text_t text1 = NULL;
  pdf_text_t text2 = NULL;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  text1 = pdf_text_new();
  fail_if(text1 == NULL);
  text2 = pdf_text_new();
  fail_if(text2 == NULL);
  
  fail_if(pdf_text_set_pdfdocenc(text1, (pdf_char_t *)"") != PDF_OK);
  fail_if(pdf_text_set_pdfdocenc(text2, (pdf_char_t *)"") != PDF_OK);
  
  /* 1. The call to  pdf_text_concat should return PDF_OK. */
  fail_unless(pdf_text_concat(text1, text2, PDF_FALSE) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_if(pdf_text_get_pdfdocenc(&output_data, text1) != PDF_OK);
  fail_unless(strlen((char *)output_data) == 0);
  
  /* 3. The lang/country information in the output object must be empty. */
  fail_unless(strlen((char *)pdf_text_get_language(text1)) == 0);
  fail_unless(strlen((char *)pdf_text_get_country(text1)) == 0);
  
  pdf_text_destroy(text1);
  pdf_text_destroy(text2);
  pdf_dealloc(output_data);
}
END_TEST


/*
 * Test: pdf_text_concat_006
 * Description:
 *   Concatenate two empty text objects with different lang/country
 *   information. Don't override langinfo.
 * Success conditions:
 *   1. The call to  pdf_text_concat should NOT return PDF_OK.
 *   2. The contents of the output text object must remain unchanged.
 */
START_TEST(pdf_text_concat_006)
{
  pdf_char_t *output_data = NULL;
  pdf_text_t text1 = NULL;
  pdf_text_t text2 = NULL;
  pdf_char_t *country1 = (pdf_char_t *)"GB";
  pdf_char_t *language1 = (pdf_char_t *)"en";
  pdf_char_t *country2 = (pdf_char_t *)"FR";
  pdf_char_t *language2 = (pdf_char_t *)"fr";
  
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  text1 = pdf_text_new();
  fail_if(text1 == NULL);
  text2 = pdf_text_new();
  fail_if(text2 == NULL);
  
  fail_if(pdf_text_set_pdfdocenc(text1, (pdf_char_t *)"") != PDF_OK);
  fail_if(pdf_text_set_pdfdocenc(text2, (pdf_char_t *)"") != PDF_OK);
  fail_if(pdf_text_set_language(text1, language1) != PDF_OK);
  fail_if(pdf_text_set_language(text2, language2) != PDF_OK);
  fail_if(pdf_text_set_country(text1, country1) != PDF_OK);
  fail_if(pdf_text_set_country(text2, country2) != PDF_OK);
  
  /* 1. The call to  pdf_text_concat should NOT return PDF_OK. */
  fail_unless(pdf_text_concat(text1, text2, PDF_FALSE) != PDF_OK);
  
  /* 2. The contents of the output text object must remain unchanged. */
  fail_if(pdf_text_get_pdfdocenc(&output_data, text1) != PDF_OK);
  fail_unless(strlen((char *)output_data) == 0);
  fail_unless(strcmp((char *)pdf_text_get_language(text1),
                     (char *)language1) == 0);
  fail_unless(strcmp((char *)pdf_text_get_country(text1),
                     (char *)country1) == 0);
  
  pdf_text_destroy(text1);
  pdf_text_destroy(text2);
  pdf_dealloc(output_data);
}
END_TEST


/*
 * Test: pdf_text_concat_007
 * Description:
 *   Concatenate two non-empty text objects with the same lang/country
 *   information. Override langinfo.
 * Success conditions:
 *   1. The call to  pdf_text_concat should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 *   3. The lang/country information in the output object must remain unchanged.
 */
START_TEST(pdf_text_concat_007)
{
  pdf_char_t *output_data = NULL;
  pdf_text_t text1 = NULL;
  pdf_text_t text2 = NULL;
  pdf_char_t *country = (pdf_char_t *)"GB";
  pdf_char_t *language = (pdf_char_t *)"en";
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  text1 = pdf_text_new();
  fail_if(text1 == NULL);
  text2 = pdf_text_new();
  fail_if(text2 == NULL);
  
  fail_if(pdf_text_set_pdfdocenc(text1, (pdf_char_t *)"GNU's not") != PDF_OK);
  fail_if(pdf_text_set_pdfdocenc(text2, (pdf_char_t *)" Unix") != PDF_OK);
  fail_if(pdf_text_set_language(text1, language) != PDF_OK);
  fail_if(pdf_text_set_language(text2, language) != PDF_OK);
  fail_if(pdf_text_set_country(text1, country) != PDF_OK);
  fail_if(pdf_text_set_country(text2, country) != PDF_OK);
  
  /* 1. The call to  pdf_text_concat should return PDF_OK. */
  fail_unless(pdf_text_concat(text1, text2, PDF_TRUE) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_if(pdf_text_get_pdfdocenc(&output_data, text1) != PDF_OK);
  fail_unless(strlen((char *)output_data) == strlen("GNU's not Unix"));
  fail_unless(strcmp((char *)output_data, "GNU's not Unix") == 0);
  
  /* 3. The lang/country information in the output object must remain
   *    unchanged. */
  fail_unless(strcmp((char *)pdf_text_get_language(text1),
                     (char *)language) == 0);
  fail_unless(strcmp((char *)pdf_text_get_country(text1),
                     (char *)country) == 0);
  
  pdf_text_destroy(text1);
  pdf_text_destroy(text2);
  pdf_dealloc(output_data);
}
END_TEST


/*
 * Test: pdf_text_concat_008
 * Description:
 *   Concatenate two non-empty text objects without lang/country
 *   information. Override langinfo.
 * Success conditions:
 *   1. The call to  pdf_text_concat should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 *   3. The lang/country information in the output object must be empty.
 */
START_TEST(pdf_text_concat_008)
{
  pdf_char_t *output_data = NULL;
  pdf_text_t text1 = NULL;
  pdf_text_t text2 = NULL;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  text1 = pdf_text_new();
  fail_if(text1 == NULL);
  text2 = pdf_text_new();
  fail_if(text2 == NULL);
  
  fail_if(pdf_text_set_pdfdocenc(text1, (pdf_char_t *)"GNU's not") != PDF_OK);
  fail_if(pdf_text_set_pdfdocenc(text2, (pdf_char_t *)" Unix") != PDF_OK);
  
  /* 1. The call to  pdf_text_concat should return PDF_OK. */
  fail_unless(pdf_text_concat(text1, text2, PDF_TRUE) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_if(pdf_text_get_pdfdocenc(&output_data, text1) != PDF_OK);
  fail_unless(strlen((char *)output_data) == strlen("GNU's not Unix"));
  fail_unless(strcmp((char *)output_data, "GNU's not Unix") == 0);
  
  /* 3. The lang/country information in the output object must be empty. */
  fail_unless(strlen((char *)pdf_text_get_language(text1)) == 0);
  fail_unless(strlen((char *)pdf_text_get_country(text1)) == 0);
  
  pdf_text_destroy(text1);
  pdf_text_destroy(text2);
  pdf_dealloc(output_data);
}
END_TEST


/*
 * Test: pdf_text_concat_009
 * Description:
 *   Concatenate two non-empty text objects with different lang/country
 *   information. Override langinfo.
 * Success conditions:
 *   1. The call to  pdf_text_concat should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 *   3. The lang/country information in the output object must remain unchanged.
 */
START_TEST(pdf_text_concat_009)
{
  pdf_char_t *output_data = NULL;
  pdf_text_t text1 = NULL;
  pdf_text_t text2 = NULL;
  pdf_char_t *country1 = (pdf_char_t *)"GB";
  pdf_char_t *language1 = (pdf_char_t *)"en";
  pdf_char_t *country2 = (pdf_char_t *)"FR";
  pdf_char_t *language2 = (pdf_char_t *)"fr";
  
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  text1 = pdf_text_new();
  fail_if(text1 == NULL);
  text2 = pdf_text_new();
  fail_if(text2 == NULL);
  
  fail_if(pdf_text_set_pdfdocenc(text1, (pdf_char_t *)"GNU's not") != PDF_OK);
  fail_if(pdf_text_set_pdfdocenc(text2, (pdf_char_t *)" Unix") != PDF_OK);
  fail_if(pdf_text_set_language(text1, language1) != PDF_OK);
  fail_if(pdf_text_set_language(text2, language2) != PDF_OK);
  fail_if(pdf_text_set_country(text1, country1) != PDF_OK);
  fail_if(pdf_text_set_country(text2, country2) != PDF_OK);
  
  /* 1. The call to  pdf_text_concat should return PDF_OK. */
  fail_unless(pdf_text_concat(text1, text2, PDF_TRUE) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_if(pdf_text_get_pdfdocenc(&output_data, text1) != PDF_OK);
  fail_unless(strlen((char *)output_data) == strlen("GNU's not Unix"));
  fail_unless(strcmp((char *)output_data, "GNU's not Unix") == 0);
  
  /* 3. The lang/country information in the output object must remain
   *    unchanged. */
  fail_unless(strcmp((char *)pdf_text_get_language(text1),
                     (char *)language1) == 0);
  fail_unless(strcmp((char *)pdf_text_get_country(text1),
                     (char *)country1) == 0);
  
  pdf_text_destroy(text1);
  pdf_text_destroy(text2);
  pdf_dealloc(output_data);
}
END_TEST

/*
 * Test: pdf_text_concat_010
 * Description:
 *   Concatenate two empty text objects with the same lang/country
 *   information. Override langinfo.
 * Success conditions:
 *   1. The call to  pdf_text_concat should return PDF_OK.
 *   2. The contents of the output text object should be empty.
 *   3. The lang/country information in the output object must remain unchanged.
 */
START_TEST(pdf_text_concat_010)
{
  pdf_char_t *output_data = NULL;
  pdf_text_t text1 = NULL;
  pdf_text_t text2 = NULL;
  pdf_char_t *country = (pdf_char_t *)"GB";
  pdf_char_t *language = (pdf_char_t *)"en";
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  text1 = pdf_text_new();
  fail_if(text1 == NULL);
  text2 = pdf_text_new();
  fail_if(text2 == NULL);
  
  fail_if(pdf_text_set_pdfdocenc(text1, (pdf_char_t *)"") != PDF_OK);
  fail_if(pdf_text_set_pdfdocenc(text2, (pdf_char_t *)"") != PDF_OK);
  fail_if(pdf_text_set_language(text1, language) != PDF_OK);
  fail_if(pdf_text_set_language(text2, language) != PDF_OK);
  fail_if(pdf_text_set_country(text1, country) != PDF_OK);
  fail_if(pdf_text_set_country(text2, country) != PDF_OK);
  
  /* 1. The call to  pdf_text_concat should return PDF_OK. */
  fail_unless(pdf_text_concat(text1, text2, PDF_TRUE) == PDF_OK);
  
  /* 2. The contents of the output text object should be empty. */
  fail_if(pdf_text_get_pdfdocenc(&output_data, text1) != PDF_OK);
  fail_unless(strlen((char *)output_data) == 0);
  
  /* 3. The lang/country information in the output object must remain
   *    unchanged. */
  fail_unless(strcmp((char *)pdf_text_get_language(text1),
                     (char *)language) == 0);
  fail_unless(strcmp((char *)pdf_text_get_country(text1),
                     (char *)country) == 0);
  
  pdf_text_destroy(text1);
  pdf_text_destroy(text2);
  pdf_dealloc(output_data);
}
END_TEST


/*
 * Test: pdf_text_concat_011
 * Description:
 *   Concatenate two empty text objects without lang/country
 *   information. Override langinfo.
 * Success conditions:
 *   1. The call to  pdf_text_concat should return PDF_OK.
 *   2. The contents of the output text object should be empty.
 *   3. The lang/country information in the output object must be empty.
 */
START_TEST(pdf_text_concat_011)
{
  pdf_char_t *output_data = NULL;
  pdf_text_t text1 = NULL;
  pdf_text_t text2 = NULL;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  text1 = pdf_text_new();
  fail_if(text1 == NULL);
  text2 = pdf_text_new();
  fail_if(text2 == NULL);
  
  fail_if(pdf_text_set_pdfdocenc(text1, (pdf_char_t *)"") != PDF_OK);
  fail_if(pdf_text_set_pdfdocenc(text2, (pdf_char_t *)"") != PDF_OK);
  
  /* 1. The call to  pdf_text_concat should return PDF_OK. */
  fail_unless(pdf_text_concat(text1, text2, PDF_TRUE) == PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_if(pdf_text_get_pdfdocenc(&output_data, text1) != PDF_OK);
  fail_unless(strlen((char *)output_data) == 0);
  
  /* 3. The lang/country information in the output object must be empty. */
  fail_unless(strlen((char *)pdf_text_get_language(text1)) == 0);
  fail_unless(strlen((char *)pdf_text_get_country(text1)) == 0);
  
  pdf_text_destroy(text1);
  pdf_text_destroy(text2);
  pdf_dealloc(output_data);
}
END_TEST


/*
 * Test: pdf_text_concat_012
 * Description:
 *   Concatenate two empty text objects with different lang/country
 *   information. Override langinfo.
 * Success conditions:
 *   1. The call to  pdf_text_concat should NOT return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 *   3. The lang/country information in the output object must remain
 *      unchanged.
 */
START_TEST(pdf_text_concat_012)
{
  pdf_char_t *output_data = NULL;
  pdf_text_t text1 = NULL;
  pdf_text_t text2 = NULL;
  pdf_char_t *country1 = (pdf_char_t *)"GB";
  pdf_char_t *language1 = (pdf_char_t *)"en";
  pdf_char_t *country2 = (pdf_char_t *)"FR";
  pdf_char_t *language2 = (pdf_char_t *)"fr";
  
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  text1 = pdf_text_new();
  fail_if(text1 == NULL);
  text2 = pdf_text_new();
  fail_if(text2 == NULL);
  
  fail_if(pdf_text_set_pdfdocenc(text1, (pdf_char_t *)"") != PDF_OK);
  fail_if(pdf_text_set_pdfdocenc(text2, (pdf_char_t *)"") != PDF_OK);
  fail_if(pdf_text_set_language(text1, language1) != PDF_OK);
  fail_if(pdf_text_set_language(text2, language2) != PDF_OK);
  fail_if(pdf_text_set_country(text1, country1) != PDF_OK);
  fail_if(pdf_text_set_country(text2, country2) != PDF_OK);
  
  /* 1. The call to  pdf_text_concat should return PDF_OK. */
  fail_unless(pdf_text_concat(text1, text2, PDF_FALSE) != PDF_OK);
  
  /* 2. The contents of the output text object must be the expected ones. */
  fail_if(pdf_text_get_pdfdocenc(&output_data, text1) != PDF_OK);
  fail_unless(strlen((char *)output_data) == 0);
  
  /* 3. The lang/country information in the output object must remain
   *    unchanged. */
  fail_unless(strcmp((char *)pdf_text_get_language(text1),
                     (char *)language1) == 0);
  fail_unless(strcmp((char *)pdf_text_get_country(text1),
                     (char *)country1) == 0);
  
  pdf_text_destroy(text1);
  pdf_text_destroy(text2);
  pdf_dealloc(output_data);
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_text_concat (void)
{
  TCase *tc = tcase_create("pdf_text_concat");
  tcase_add_test(tc, pdf_text_concat_001);
  tcase_add_test(tc, pdf_text_concat_002);
  tcase_add_test(tc, pdf_text_concat_003);
  tcase_add_test(tc, pdf_text_concat_004);
  tcase_add_test(tc, pdf_text_concat_005);
  tcase_add_test(tc, pdf_text_concat_006);
  tcase_add_test(tc, pdf_text_concat_007);
  tcase_add_test(tc, pdf_text_concat_008);
  tcase_add_test(tc, pdf_text_concat_009);
  tcase_add_test(tc, pdf_text_concat_010);
  tcase_add_test(tc, pdf_text_concat_011);
  tcase_add_test(tc, pdf_text_concat_012);
  return tc;
}

/* End of pdf-text-concat.c */

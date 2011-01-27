/* -*- mode: C -*-
 *
 *       File:         pdf-text-new-from-pdf-string.c
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_new_from_pdf_string test case
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


#define INTERACTIVE_DEBUG   0

/*
 * Test: pdf_text_new_from_pdf_string_001
 * Description:
 *   Create a text object with an input valid PDF-Doc-Encoded string
 * Success conditions:
 *   1. The call to pdf_text_new_from_pdf_string should return PDF_OK.
 *   2. The function should return a valid pointer to the new text object
 *   3. The contents of the text object must be the expected ones.
 *   4. The language code within the text object must be empty.
 *   5. The country code within the text object must be empty.
 *   6. The call should return a NULL remaining_str pointer and a zero
 *      remaining_length
 */
START_TEST(pdf_text_new_from_pdf_string_001)
{
  


  extern const test_string_t pdf_doc_encoding_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(pdf_doc_encoding_strings[i].data != NULL)
    {
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *remaining_str = NULL;
      pdf_size_t remaining_length = 0;
      pdf_char_t *expected_data = NULL;
      const pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_size_t expected_size = 0;
      pdf_text_t text = NULL;

      /* Set input data */
      input_data = (pdf_char_t *)pdf_doc_encoding_strings[i].data;
      input_size = (pdf_size_t)pdf_doc_encoding_strings[i].size;
      
      /* Set expected data */
      expected_data = (pdf_char_t *)pdf_doc_encoding_strings[i].utf32be_data;
      expected_size = (pdf_size_t)pdf_doc_encoding_strings[i].utf32be_size;
      if(!pdf_text_test_big_endian_system())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness(expected_data,
                                                                expected_size);
          /* Just in case... */
          fail_if(expected_data == NULL);
        }
      
      /* 1. The call to pdf_text_new_from_pdf_string should return PDF_OK. */
      fail_if(pdf_text_new_from_pdf_string(input_data,
                                           input_size,
                                           &remaining_str,
                                           &remaining_length,
                                           &text) != PDF_OK);
      
      /* 2. The function should return a valid pointer to the new text object */
      fail_if(text == NULL);
      
      /* 3. The contents of the text object must be the expected ones. */
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size);
      fail_unless(memcmp(actual_data, expected_data, expected_size)==0);

      /* 4. The language code of the output object should be empty */
      fail_unless(strlen(pdf_text_get_language(text)) == 0);
      
      /* 5. The country code of the output object should be empty */
      fail_unless(strlen(pdf_text_get_country(text)) == 0);
      
      /* 6. The call should return a NULL remaining_str pointer and a zero
       *  remaining_length */
      fail_unless(remaining_str == NULL);
      fail_unless(remaining_length == 0);
      
      fail_if(pdf_text_destroy(text) != PDF_OK);
      
      if(expected_free)
        {
          pdf_dealloc(expected_data);
        }
      ++i;
    }

}
END_TEST

/*
 * Test: pdf_text_new_from_pdf_string_002
 * Description:
 *   Create a text object with an input invalid PDF-Doc-Encoded string.
 * Success conditions:
 *   1. The call to @code{pdf_text_new_from_pdf_string} should NOT return PDF_OK
 *   2. Pointer to the text object must remain unchanged.
 */
START_TEST(pdf_text_new_from_pdf_string_002)
{
  pdf_text_t text = NULL;
  pdf_char_t *remaining_str = NULL;
  pdf_size_t remaining_length = 0;
  
  /* 0x9F is Undefined in PDF Doc Encoding */
  const pdf_char_t *invalid_pdfdocenc = (pdf_char_t *)"\x9D\x9E\x9F";
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  /* 1. The call to  pdf_text_new_from_host should NOT return PDF_OK. */
  fail_unless(pdf_text_new_from_pdf_string(invalid_pdfdocenc,
                                           strlen((char*)invalid_pdfdocenc),
                                           &remaining_str,
                                           &remaining_length,
                                           &text) != PDF_OK);
  
  /* 2. Pointer to the text object must remain unchanged. */
  fail_if(text != NULL);
}
END_TEST


/*
 * Test: pdf_text_new_from_pdf_string_003
 * Description:
 *   Create a text object with an input valid UTF-16BE encoded string with BOM
 *    (containing both 16-bit and 32-bit UTF-16 code points!) and without
 *    lang/country information.
 * Success conditions:
 *   1. The call to pdf_text_new_from_pdf_string should return PDF_OK.
 *   2. The function should return a valid pointer to the new text object
 *   3. The contents of the text object must be the expected ones.
 *   4. The language code within the text object must be empty.
 *   5. The country code within the text object must be empty.
 *   6. The call should return a NULL remaining_str pointer and a zero
 *      remaining_length 
 */
START_TEST(pdf_text_new_from_pdf_string_003)
{
  


  extern const test_string_t utf16be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf16be_strings[i].data != NULL)
    {
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *remaining_str = NULL;
      pdf_size_t remaining_length = 0;
      const pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_char_t *expected_data = NULL;
      pdf_size_t expected_size = 0;
      pdf_text_t text = NULL;
      
      /* Set input data */
      input_data = (pdf_char_t *)utf16be_strings[i].data;
      input_size = (pdf_size_t)utf16be_strings[i].size;
      
      /* Set expected data */
      expected_data = (pdf_char_t *)utf16be_strings[i].utf32be_data;
      expected_size = utf16be_strings[i].utf32be_size;
      if(!pdf_text_test_big_endian_system())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness(expected_data,
                                                                expected_size);
          /* Just in case... */
          fail_if(expected_data == NULL);
        }
      
      /* 1. The call to pdf_text_new_from_pdf_string should return PDF_OK. */
      fail_if(pdf_text_new_from_pdf_string(input_data,
                                           input_size,
                                           &remaining_str,
                                           &remaining_length,
                                           &text) != PDF_OK);
      
      /* 2. The function should return a valid pointer to the new text object */
      fail_if(text == NULL);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          pdf_char_t *expected_hex = NULL;
          
          pdf_size_t actual_size;
          pdf_char_t *actual_data;
          fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                           PDF_TEXT_UTF32_HE,0) == PDF_OK);
          internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');

          expected_hex = pdf_text_test_get_hex(expected_data,expected_size,':');
          fail_if((internal_hex == NULL) || \
                  (expected_hex == NULL));
          printf("pdf_text_new_from_pdf_string_003:%d:Internal> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_new_from_pdf_string_003:%d:Expected> '%s'\n",
                 i, expected_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
        }
      
      /* 3. The contents of the text object must be the expected ones. */
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size);
      fail_unless(memcmp(actual_data, expected_data, expected_size)==0);
      
      /* 4. The language code of the output object should be empty */
      fail_unless(strlen(pdf_text_get_language(text)) == 0);
      
      /* 5. The country code of the output object should be empty */
      fail_unless(strlen(pdf_text_get_country(text)) == 0);
      
      /* 6. The call should return a NULL remaining_str pointer and a zero
       *  remaining_length */
      fail_unless(remaining_str == NULL);
      fail_unless(remaining_length == 0);
      
      fail_if(pdf_text_destroy(text) != PDF_OK);
      
      if(expected_free)
        {
          pdf_dealloc(expected_data);
        }
      ++i;
    }

}
END_TEST


/*
 * Test: pdf_text_new_from_pdf_string_004
 * Description:
 *   Create a text object with an input valid UTF-16BE encoded string without
 *   BOM
 * Success conditions:
 *   1. The call to pdf_text_new_from_pdf_string should NOT return PDF_OK.
 *   2. Pointer to the text object must remain unchanged.
 */
START_TEST(pdf_text_new_from_pdf_string_004)
{
  extern const test_string_t utf16be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf16be_strings[i].data != NULL)
    {
      pdf_char_t *remaining_str = NULL;
      pdf_size_t remaining_length = 0;
      const pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_text_t text = NULL;
      
      /* Set input data, SKIPPING 2 bytes of the BOM */
      input_data = (pdf_char_t *)&(utf16be_strings[i].data[2]);
      input_size = (pdf_size_t)(utf16be_strings[i].size - 2);
      
      /* 1. The call to pdf_text_new_from_pdf_string should NOT return
       *  PDF_OK. */
      fail_unless(pdf_text_new_from_pdf_string(input_data,
                                               input_size,
                                               &remaining_str,
                                               &remaining_length,
                                               &text) != PDF_OK);
      
      /* 2. Pointer to the text object must remain unchanged. */
      fail_if(text != NULL);
      ++i;
    }
}
END_TEST


/*
 * Test: pdf_text_new_from_pdf_string_005
 * Description:
 *   Create a text object with an input invalid UTF-16BE encoded string with BOM
 * Success conditions:
 *   1. The call to pdf_text_new_from_pdf_string should NOT return PDF_OK.
 *   2. Pointer to the text object must remain unchanged.
 */
START_TEST(pdf_text_new_from_pdf_string_005)
{
  extern const test_string_t utf16be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf16be_strings[i].data != NULL)
    {
      pdf_char_t aux;
      pdf_char_t *remaining_str = NULL;
      pdf_size_t remaining_length = 0;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_text_t text = NULL;
      
      /* Set input UTF-16LE data WITH BOM */
      input_size = (pdf_size_t)utf16be_strings[i].size;
      input_data=pdf_text_test_change_utf16_endianness((pdf_char_t *) \
                                                       utf16be_strings[i].data,
                                                       input_size);
      fail_if(input_data == NULL);

      /* Now, swap first two bytes of the BOM, but leave the remaining string
       *  untouched. We will create a UTF-16LE encoded string with UTF-16BE
       *  BOM */
      aux = input_data[0];
      input_data[0] = input_data[1];
      input_data[1] = input_data[0];
            
      /* 1. The call to pdf_text_new_from_pdf_string should NOT return
       *  PDF_OK. */
      fail_unless(pdf_text_new_from_pdf_string(input_data,
                                               input_size,
                                               &remaining_str,
                                               &remaining_length,
                                               &text) != PDF_OK);
      
      /* 2. Pointer to the text object must remain unchanged. */
      fail_if(text != NULL);

      pdf_dealloc(input_data);
      
      ++i;
    }
}
END_TEST



/*
 * Test: pdf_text_new_from_pdf_string_006
 * Description:
 *   Create a text object with an input valid UTF-16BE encoded string with BOM
 *    (containing both 16-bit and 32-bit UTF-16 code points!) which also 
 *    contains a valid language code embedded.
 * Success conditions:
 *   1. The call to pdf_text_new_from_pdf_string should return PDF_OK.
 *   2. The function should return a valid pointer to the new text object
 *   3. The contents of the text object must be the expected ones.
 *   4. The languange code within the text object must be the expected one.
 *   5. The country code within the text object must be empty.
 *   6. The call should return a NULL remaining_str pointer and a zero
 *      remaining_length
 */
START_TEST(pdf_text_new_from_pdf_string_006)
{
  


  extern const test_string_t utf16be_strings[];
  int i;
  const pdf_char_t *language_code = (pdf_char_t *)"en";
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf16be_strings[i].data != NULL)
    {
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *remaining_str = NULL;
      pdf_size_t remaining_length = 0;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_char_t *expected_data = NULL;
      pdf_size_t expected_size = 0;
      pdf_text_t text = NULL;
      
      /* Set input data with LANGUAGE code and BOM */
      input_size = (pdf_size_t)utf16be_strings[i].size + 6;
      input_data = (pdf_char_t *)pdf_alloc(input_size);
      fail_if(input_data == NULL);

      /* Store BOM */
      memcpy(input_data, utf16be_strings[i].data, 2);
      /* Store language code */
      memcpy(&input_data[2], "\x00\x1B", 2);
      memcpy(&input_data[4], language_code, 2);
      memcpy(&input_data[6], "\x00\x1B", 2);
      /* Store remaining string contents */
      memcpy(&input_data[8], 
             &utf16be_strings[i].data[2],
             (pdf_size_t)utf16be_strings[i].size -2);
      
      /* Set expected data */
      expected_data = (pdf_char_t *)utf16be_strings[i].utf32be_data;
      expected_size = utf16be_strings[i].utf32be_size;
      if(!pdf_text_test_big_endian_system())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness(expected_data,
                                                                expected_size);
          /* Just in case... */
          fail_if(expected_data == NULL);
        }
      
      /* 1. The call to pdf_text_new_from_pdf_string should return PDF_OK. */
      fail_if(pdf_text_new_from_pdf_string(input_data,
                                           input_size,
                                           &remaining_str,
                                           &remaining_length,
                                           &text) != PDF_OK);
      
      /* 2. The function should return a valid pointer to the new text object */
      fail_if(text == NULL);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          pdf_char_t *expected_hex = NULL;

          pdf_size_t actual_size;
          pdf_char_t *actual_data;
          fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                           PDF_TEXT_UTF32_HE,0) == PDF_OK);
          internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
          expected_hex = pdf_text_test_get_hex(expected_data,expected_size,':');
          fail_if((internal_hex == NULL) || \
                  (expected_hex == NULL));
          printf("pdf_text_new_from_pdf_string_006:%d:Internal> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_new_from_pdf_string_006:%d:Expected> '%s'\n",
                 i, expected_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
          printf("pdf_text_new_from_pdf_string_006:%d:Lang> '%s'\n",
                 i, pdf_text_get_language(text));
          printf("pdf_text_new_from_pdf_string_006:%d:Country> '%s'\n",
                 i, pdf_text_get_country(text));
        }
      
      /* 3. The contents of the text object must be the expected ones. */
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size);
      fail_unless(memcmp(actual_data, expected_data, expected_size)==0);

      
      /* 4. The languange code within the text object must be the expected
       *    one */
      fail_unless(strlen(pdf_text_get_language(text)) == 2);
      fail_unless(strcmp(pdf_text_get_language(text), \
                         language_code) == 0);
      
      /* 5. The country code of the output object should be empty */
      fail_unless(strlen(pdf_text_get_country(text)) == 0);
      
      /* 6. The call should return a NULL remaining_str pointer and a zero
       *  remaining_length */
      fail_unless(remaining_str == NULL);
      fail_unless(remaining_length == 0);
      
      fail_if(pdf_text_destroy(text) != PDF_OK);
      
      if(expected_free)
        {
          pdf_dealloc(expected_data);
        }
      pdf_dealloc(input_data);
      ++i;
    }

}
END_TEST


/*
 * Test: pdf_text_new_from_pdf_string_007
 * Description:
 *   Create a text object with an input valid UTF-16BE encoded string with BOM
 *    (containing both 16-bit and 32-bit UTF-16 code points!) which also 
 *    contains a valid language code and a valid country code embedded.
 * Success conditions:
 *   1. The call to pdf_text_new_from_pdf_string should return PDF_OK.
 *   2. The function should return a valid pointer to the new text object
 *   3. The contents of the text object must be the expected ones.
 *   4. The languange code within the text object must be the expected one.
 *   5. The country code within the text object must be the expected one.
 *   6. The call should return a NULL remaining_str pointer and a zero
 *      remaining_length
 */
START_TEST(pdf_text_new_from_pdf_string_007)
{
  


  extern const test_string_t utf16be_strings[];
  int i;
  const pdf_char_t *language_code = (pdf_char_t *)"en";
  const pdf_char_t *country_code = (pdf_char_t *)"US";
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf16be_strings[i].data != NULL)
    {
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *remaining_str = NULL;
      pdf_size_t remaining_length = 0;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_char_t *expected_data = NULL;
      pdf_size_t expected_size = 0;
      pdf_text_t text = NULL;
      
      /* Set input data with LANGUAGE code and BOM */
      input_size = (pdf_size_t)utf16be_strings[i].size + 8;
      input_data = (pdf_char_t *)pdf_alloc(input_size);
      fail_if(input_data == NULL);
      
      /* Store BOM */
      memcpy(input_data, utf16be_strings[i].data, 2);
      /* Store language code */
      memcpy(&input_data[2], "\x00\x1B", 2);
      memcpy(&input_data[4], language_code, 2);
      memcpy(&input_data[6], country_code, 2);
      memcpy(&input_data[8], "\x00\x1B", 2);
      /* Store remaining string contents */
      memcpy(&input_data[10], 
             &utf16be_strings[i].data[2],
             (pdf_size_t)utf16be_strings[i].size -2);

      /* Set expected data */
      expected_data = (pdf_char_t *)utf16be_strings[i].utf32be_data;
      expected_size = utf16be_strings[i].utf32be_size;
      if(!pdf_text_test_big_endian_system())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness(expected_data,
                                                                expected_size);
          /* Just in case... */
          fail_if(expected_data == NULL);
        }
      
      /* 1. The call to pdf_text_new_from_pdf_string should return PDF_OK. */
      fail_if(pdf_text_new_from_pdf_string(input_data,
                                           input_size,
                                           &remaining_str,
                                           &remaining_length,
                                           &text) != PDF_OK);
      
      /* 2. The function should return a valid pointer to the new text object */
      fail_if(text == NULL);
      
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          pdf_char_t *expected_hex = NULL;

          pdf_size_t actual_size;
          pdf_char_t *actual_data;
          fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                           PDF_TEXT_UTF32_HE,0) == PDF_OK);
          internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
          expected_hex = pdf_text_test_get_hex(expected_data,expected_size,':');
          fail_if((internal_hex == NULL) || \
                  (expected_hex == NULL));
          printf("pdf_text_new_from_pdf_string_007:%d:Internal> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_new_from_pdf_string_007:%d:Expected> '%s'\n",
                 i, expected_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
          printf("pdf_text_new_from_pdf_string_007:%d:Lang> '%s'\n",
                 i, pdf_text_get_language(text));
          printf("pdf_text_new_from_pdf_string_007:%d:Country> '%s'\n",
                 i, pdf_text_get_country(text));
        }
      
      /* 3. The contents of the text object must be the expected ones. */
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size);
      fail_unless(memcmp(actual_data, expected_data, expected_size)==0);
      
      /* 4. The languange code within the text object must be the expected
       *    one */
      fail_unless(strlen(pdf_text_get_language(text)) == 2);
      fail_unless(strcmp(pdf_text_get_language(text), \
                         language_code) == 0);
      
      /* 5. The country code within the text object must be the expected one */
      fail_unless(strlen(pdf_text_get_country(text)) == 2);
      fail_unless(strcmp(pdf_text_get_country(text), \
                         country_code) == 0);
      
      /* 6. The call should return a NULL remaining_str pointer and a zero
       *  remaining_length */
      fail_unless(remaining_str == NULL);
      fail_unless(remaining_length == 0);
      
      fail_if(pdf_text_destroy(text) != PDF_OK);
      
      if(expected_free)
        {
          pdf_dealloc(expected_data);
        }
      pdf_dealloc(input_data);
      ++i;
    }

}
END_TEST

/*
 * Test: pdf_text_new_from_pdf_string_008
 * Description:
 *   Create a text object with an input valid UTF-16BE encoded string with BOM
 *    (containing both 16-bit and 32-bit UTF-16 code points!) which also
 *    contains more than one valid language code embedded (the string contains
 *    more than one string in more than one different language).
 * Success conditions:
 *   1. The first call to pdf_text_new_from_pdf_string should return PDF_OK.
 *   2. The first call should return a valid pointer to the new text object
 *   3. The contents of the first text object must be the expected ones.
 *   4. The languange code within the first text object must be the expected one
 *   5. The country code within the first text object must be empty.
 *   6. The first call should return a valid remaining_str pointer and a
 *      non-zero remaining_length
 *   7. The second call to @code{pdf_text_new_from_pdf_string} should return
 *      PDF_OK.
 *   8. The first call should return a valid pointer to the new text object
 *   9. The contents of the second text object must be the expected ones.
 *   10. The languange code within the second text object must be the expected
 *      one.
 *   11. The country code within the second text object must be empty.
 *   12. The second call should return a valid @code{remaining_str} pointer and
 *       a non-zero @code{remaining_length}
 *   13. The third call to @code{pdf_text_new_from_pdf_string} should return
 *       PDF_OK.
 *   14. The third call should return a valid pointer to the new text object
 *   15. The contents of the third text object must be the expected ones.
 *   16. The languange code within the third text object must be the expected
 *       one.
 *   17. The country code within the third text object must be empty.
 *   18. The third call should return a NULL @code{remaining_str} pointer and
 *       a zero @code{remaining_length}
 */
START_TEST(pdf_text_new_from_pdf_string_008)
{
  


  extern const test_string_t utf16be_strings[];
  int i;
  const pdf_char_t *language_code1 = (pdf_char_t *)"en";
  const pdf_char_t *language_code2 = (pdf_char_t *)"sp";
  const pdf_char_t *language_code3 = (pdf_char_t *)"fr";
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  /* 3 different strings will be read into a single utf-16be string */
  i = 0;
  while((utf16be_strings[i].data != NULL) && \
        (utf16be_strings[i+1].data != NULL) && \
        (utf16be_strings[i+2].data != NULL))
    {
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *remaining_str = NULL;
      pdf_size_t remaining_length = 0;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_char_t *expected_data1 = NULL;
      pdf_size_t expected_size1 = 0;
      pdf_char_t *expected_data2 = NULL;
      pdf_size_t expected_size2 = 0;
      pdf_char_t *expected_data3 = NULL;
      pdf_size_t expected_size3 = 0;
      pdf_text_t text1 = NULL;
      pdf_text_t text2 = NULL;
      pdf_text_t text3 = NULL;
      pdf_char_t *str_walker;
      
      /* Set input data with LANGUAGE code and BOM */
      input_size = (pdf_size_t)utf16be_strings[i].size -2 + \
                   (pdf_size_t)utf16be_strings[i+1].size -2 + \
                   (pdf_size_t)utf16be_strings[i+2].size -2 + 20;
      input_data = (pdf_char_t *)pdf_alloc(input_size);
      fail_if(input_data == NULL);
      
      /* Store BOM (2 bytes) */
      memcpy(input_data, utf16be_strings[i].data, 2);
      str_walker = &input_data[2];

      /* Store FIRST language code (6 bytes) */
      memcpy(str_walker, "\x00\x1B", 2);
      memcpy(&str_walker[2], language_code1, 2);
      memcpy(&str_walker[4], "\x00\x1B", 2);

      /* Store FIRST string */
      memcpy(&str_walker[6], 
             &utf16be_strings[i].data[2], /* Skipping bom */
             (pdf_size_t)utf16be_strings[i].size -2);
      str_walker = &str_walker[6 + utf16be_strings[i].size - 2];

      /* Store SECOND language code (6 bytes) */
      memcpy(str_walker, "\x00\x1B", 2);
      memcpy(&str_walker[2], language_code2, 2);
      memcpy(&str_walker[4], "\x00\x1B", 2);

      /* Store SECOND string */
      memcpy(&str_walker[6], 
             &utf16be_strings[i+1].data[2], /* Skipping bom */
             (pdf_size_t)utf16be_strings[i+1].size -2);
      str_walker = &str_walker[6 + utf16be_strings[i+1].size - 2];

      /* Store THIRD language code (6 bytes) */
      memcpy(str_walker, "\x00\x1B", 2);
      memcpy(&str_walker[2], language_code3, 2);
      memcpy(&str_walker[4], "\x00\x1B", 2);

      /* Store THIRD string */
      memcpy(&str_walker[6], 
             &utf16be_strings[i+2].data[2], /* Skipping bom */
             (pdf_size_t)utf16be_strings[i+2].size -2);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *input_hex = NULL;
          
          input_hex = pdf_text_test_get_hex(input_data,input_size,':');
          fail_if(input_hex == NULL);
          printf("pdf_text_new_from_pdf_string_008:%d:Input> '%s'\n",
                 i, input_hex);
          pdf_dealloc(input_hex);
        }
      
      /* Set expected data */
      expected_data1 = (pdf_char_t *)utf16be_strings[i].utf32be_data;
      expected_size1 = utf16be_strings[i].utf32be_size;
      expected_data2 = (pdf_char_t *)utf16be_strings[i+1].utf32be_data;
      expected_size2 = utf16be_strings[i+1].utf32be_size;
      expected_data3 = (pdf_char_t *)utf16be_strings[i+2].utf32be_data;
      expected_size3 = utf16be_strings[i+2].utf32be_size;
      if(!pdf_text_test_big_endian_system())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data1 =pdf_text_test_change_utf32_endianness(expected_data1,
                                                                expected_size1);
          expected_data2 =pdf_text_test_change_utf32_endianness(expected_data2,
                                                                expected_size2);
          expected_data3 =pdf_text_test_change_utf32_endianness(expected_data3,
                                                                expected_size3);
          /* Just in case... */
          fail_if(expected_data1 == NULL);
          fail_if(expected_data2 == NULL);
          fail_if(expected_data3 == NULL);
        }
      
      /* 1. The call to pdf_text_new_from_pdf_string should return PDF_OK.
       */
      fail_if(pdf_text_new_from_pdf_string(input_data,
                                           input_size,
                                           &remaining_str,
                                           &remaining_length,
                                           &text1) != PDF_OK);
      
      /* 2. The function should return a valid pointer to the new text object */
      fail_if(text1 == NULL);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          pdf_char_t *expected_hex = NULL;

          pdf_size_t actual_size;
          pdf_char_t *actual_data;
          fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text1,
                                           PDF_TEXT_UTF32_HE,0) == PDF_OK);
          internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
          expected_hex = pdf_text_test_get_hex(expected_data1,
                                               expected_size1,':');
          fail_if((internal_hex == NULL) || \
                  (expected_hex == NULL));
          printf("pdf_text_new_from_pdf_string_008:%d:Internal_1> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_new_from_pdf_string_008:%d:Expected_1> '%s'\n",
                 i, expected_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
          printf("pdf_text_new_from_pdf_string_008:%d:Lang_1> '%s'\n",
                 i, pdf_text_get_language(text1));
          printf("pdf_text_new_from_pdf_string_008:%d:Country_1> '%s'\n",
                 i, pdf_text_get_country(text1));
          printf("pdf_text_new_from_pdf_string_008:%d:remaining_length_1> "
                 "'%ld'\n", i, (long)remaining_length);
          printf("pdf_text_new_from_pdf_string_008:%d:remaining_length_1> "
                 "'%s'\n", i, ((remaining_str == NULL)?"NULL":"not NULL"));
        }
      
      /* 3. The contents of the first text object must be the expected ones. */
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text1,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size1);
      fail_unless(memcmp(actual_data, expected_data1, expected_size1)==0);
      
      /* 4. The languange code within the first text object must be the expected
       *    one */
      fail_unless(strlen(pdf_text_get_language(text1)) == 2);
      fail_unless(strcmp(pdf_text_get_language(text1), \
                         language_code1) == 0);
      
      /* 5. The country code of the output object should be empty */
      fail_unless(strlen(pdf_text_get_country(text1)) == 0);
      
      /* 6. The first call should return a valid remaining_str pointer and a
       *      non-zero remaining_length */
      fail_unless(remaining_str != NULL);
      fail_unless(remaining_length != 0);
      

      /* 7. The second call to @code{pdf_text_new_from_pdf_string} should return
       *      PDF_OK. */
      fail_if(pdf_text_new_from_pdf_string(remaining_str,
                                           remaining_length,
                                           &remaining_str,
                                           &remaining_length,
                                           &text2) != PDF_OK);
      
      /* 8. The function should return a valid pointer to the new text object */
      fail_if(text2 == NULL);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          pdf_char_t *expected_hex = NULL;

          pdf_size_t actual_size;
          pdf_char_t *actual_data;
          fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text2,
                                           PDF_TEXT_UTF32_HE,0) == PDF_OK);
          internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
          expected_hex = pdf_text_test_get_hex(expected_data2,
                                               expected_size2,':');
          fail_if((internal_hex == NULL) || \
                  (expected_hex == NULL));
          printf("pdf_text_new_from_pdf_string_008:%d:Internal_2> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_new_from_pdf_string_008:%d:Expected_2> '%s'\n",
                 i, expected_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
          printf("pdf_text_new_from_pdf_string_008:%d:Lang_2> '%s'\n",
                 i, pdf_text_get_language(text2));
          printf("pdf_text_new_from_pdf_string_008:%d:Country_2> '%s'\n",
                 i, pdf_text_get_country(text2));
          printf("pdf_text_new_from_pdf_string_008:%d:remaining_length_2> "
                 "'%ld'\n", i, (long)remaining_length);
          printf("pdf_text_new_from_pdf_string_008:%d:remaining_length_2> "
                 "'%s'\n", i, ((remaining_str == NULL)?"NULL":"not NULL"));
        }
      
      /* 9. The contents of the second text object must be the expected ones. */
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text2,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size2);
      fail_unless(memcmp(actual_data, expected_data2, expected_size2)==0);
      
      /* 10. The languange code within the second text object must be the 
       *    expected one */
      fail_unless(strlen(pdf_text_get_language(text2)) == 2);
      fail_unless(strcmp(pdf_text_get_language(text2), \
                         language_code2) == 0);
      
      /* 11. The country code of the second output object should be empty */
      fail_unless(strlen(pdf_text_get_country(text2)) == 0);
      
      /* 12. The second call should return a valid remaining_str pointer and a
       *      non-zero remaining_length */
      fail_unless(remaining_str != NULL);
      fail_unless(remaining_length != 0);
      
      
      /* 13. The third call to @code{pdf_text_new_from_pdf_string} should 
       *      return PDF_OK. */
      fail_if(pdf_text_new_from_pdf_string(remaining_str,
                                           remaining_length,
                                           &remaining_str,
                                           &remaining_length,
                                           &text3) != PDF_OK);
      
      /* 14. The function should return a valid pointer to the new text object */
      fail_if(text3 == NULL);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          pdf_char_t *expected_hex = NULL;

          pdf_size_t actual_size;
          pdf_char_t *actual_data;
          fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text3,
                                           PDF_TEXT_UTF32_HE,0) == PDF_OK);
          internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
          expected_hex = pdf_text_test_get_hex(expected_data3,
                                               expected_size3,':');
          fail_if((internal_hex == NULL) || \
                  (expected_hex == NULL));
          printf("pdf_text_new_from_pdf_string_008:%d:Internal_3> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_new_from_pdf_string_008:%d:Expected_3> '%s'\n",
                 i, expected_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
          printf("pdf_text_new_from_pdf_string_008:%d:Lang_3> '%s'\n",
                 i, pdf_text_get_language(text3));
          printf("pdf_text_new_from_pdf_string_008:%d:Country_3> '%s'\n",
                 i, pdf_text_get_country(text3));
          printf("pdf_text_new_from_pdf_string_008:%d:remaining_length_3> "
                 "'%ld'\n", i, (long)remaining_length);
          printf("pdf_text_new_from_pdf_string_008:%d:remaining_length_3> "
                 "'%s'\n", i, ((remaining_str == NULL)?"NULL":"not NULL"));
        }
      
      /* 15. The contents of the third text object must be the expected ones. */
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text3,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size3);
      fail_unless(memcmp(actual_data, expected_data3, expected_size3)==0);
      
      /* 16. The languange code within the third text object must be the 
       *    expected one */
      fail_unless(strlen(pdf_text_get_language(text3)) == 2);
      fail_unless(strcmp(pdf_text_get_language(text3), \
                         language_code3) == 0);
      
      /* 17. The country code of the third output object should be empty */
      fail_unless(strlen(pdf_text_get_country(text3)) == 0);
      
      /* 18. The third call should return a NULL @code{remaining_str} pointer
       *      and a zero @code{remaining_length} */
      fail_unless(remaining_str == NULL);
      fail_unless(remaining_length == 0);
      
      
      fail_if(pdf_text_destroy(text1) != PDF_OK);
      fail_if(pdf_text_destroy(text2) != PDF_OK);
      fail_if(pdf_text_destroy(text3) != PDF_OK);
      
      if(expected_free)
        {
          pdf_dealloc(expected_data1);
          pdf_dealloc(expected_data2);
          pdf_dealloc(expected_data3);
        }
      pdf_dealloc(input_data);
      ++i;
    }


}
END_TEST


/*
 * Test: pdf_text_new_from_pdf_string_009
 * Description:
 *   Create a text object with an input valid UTF-16BE encoded string with BOM
 *    (containing both 16-bit and 32-bit UTF-16 code points!) which also
 *    contains more than one valid language code and a valid country code
 *    embedded (the string contains more than one string in more than one
 *    different language/country).
 * Success conditions:
 *   1. The first call to pdf_text_new_from_pdf_string should return PDF_OK.
 *   2. The first call should return a valid pointer to the new text object
 *   3. The contents of the first text object must be the expected ones.
 *   4. The languange code within the first text object must be the expected one
 *   5. The country code within the first text object must be the expected one.
 *   6. The first call should return a valid remaining_str pointer and a
 *      non-zero remaining_length
 *   7. The second call to @code{pdf_text_new_from_pdf_string} should return
 *      PDF_OK.
 *   8. The second call should return a valid pointer to the new text object
 *   9. The contents of the second text object must be the expected ones.
 *   10. The languange code within the second text object must be the expected
 *      one.
 *   11. The country code within the second text object must be the expected one.
 *   12. The second call should return a valid @code{remaining_str} pointer and
 *       a non-zero @code{remaining_length}
 *   13. The third call to @code{pdf_text_new_from_pdf_string} should return
 *       PDF_OK.
 *   14. The third call should return a valid pointer to the new text object
 *   15. The contents of the third text object must be the expected ones.
 *   16. The languange code within the third text object must be the expected
 *       one.
 *   17. The country code within the third text object must be the expected one.
 *   18. The third call should return a NULL @code{remaining_str} pointer and
 *       a zero @code{remaining_length}
 */
START_TEST(pdf_text_new_from_pdf_string_009)
{
  


  extern const test_string_t utf16be_strings[];
  int i;
  const pdf_char_t *language_code1 = (pdf_char_t *)"en";
  const pdf_char_t *language_code2 = (pdf_char_t *)"sp";
  const pdf_char_t *language_code3 = (pdf_char_t *)"fr";
  const pdf_char_t *country_code1 = (pdf_char_t *)"GB";
  const pdf_char_t *country_code2 = (pdf_char_t *)"SP";
  const pdf_char_t *country_code3 = (pdf_char_t *)"FR";
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  /* 3 different strings will be read into a single utf-16be string */
  i = 0;
  while((utf16be_strings[i].data != NULL) && \
        (utf16be_strings[i+1].data != NULL) && \
        (utf16be_strings[i+2].data != NULL))
    {
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *remaining_str = NULL;
      pdf_size_t remaining_length = 0;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_char_t *expected_data1 = NULL;
      pdf_size_t expected_size1 = 0;
      pdf_char_t *expected_data2 = NULL;
      pdf_size_t expected_size2 = 0;
      pdf_char_t *expected_data3 = NULL;
      pdf_size_t expected_size3 = 0;
      pdf_text_t text1 = NULL;
      pdf_text_t text2 = NULL;
      pdf_text_t text3 = NULL;
      pdf_char_t *str_walker;
      
      /* Set input data with LANGUAGE code, COUNTRY code and BOM */
      input_size = (pdf_size_t)utf16be_strings[i].size -2 + \
      (pdf_size_t)utf16be_strings[i+1].size -2 + \
      (pdf_size_t)utf16be_strings[i+2].size -2 + 26;
      input_data = (pdf_char_t *)pdf_alloc(input_size);
      fail_if(input_data == NULL);
      
      /* Store BOM (2 bytes) */
      memcpy(input_data, utf16be_strings[i].data, 2);
      str_walker = &input_data[2];
      
      /* Store FIRST language and country codes (8 bytes) */
      memcpy(str_walker, "\x00\x1B", 2);
      memcpy(&str_walker[2], language_code1, 2);
      memcpy(&str_walker[4], country_code1, 2);
      memcpy(&str_walker[6], "\x00\x1B", 2);
      
      /* Store FIRST string */
      memcpy(&str_walker[8], 
             &utf16be_strings[i].data[2], /* Skipping bom */
             (pdf_size_t)utf16be_strings[i].size -2);
      str_walker = &str_walker[8 + utf16be_strings[i].size - 2];
      
      /* Store SECOND language and country codes (8 bytes) */
      memcpy(str_walker, "\x00\x1B", 2);
      memcpy(&str_walker[2], language_code2, 2);
      memcpy(&str_walker[4], country_code2, 2);
      memcpy(&str_walker[6], "\x00\x1B", 2);
      
      /* Store SECOND string */
      memcpy(&str_walker[8], 
             &utf16be_strings[i+1].data[2], /* Skipping bom */
             (pdf_size_t)utf16be_strings[i+1].size -2);
      str_walker = &str_walker[8 + utf16be_strings[i+1].size - 2];

      /* Store THIRD language and country codes (8 bytes) */
      memcpy(str_walker, "\x00\x1B", 2);
      memcpy(&str_walker[2], language_code3, 2);
      memcpy(&str_walker[4], country_code3, 2);
      memcpy(&str_walker[6], "\x00\x1B", 2);
      
      /* Store THIRD string */
      memcpy(&str_walker[8], 
             &utf16be_strings[i+2].data[2], /* Skipping bom */
             (pdf_size_t)utf16be_strings[i+2].size -2);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *input_hex = NULL;
          
          input_hex = pdf_text_test_get_hex(input_data,input_size,':');
          fail_if(input_hex == NULL);
          printf("pdf_text_new_from_pdf_string_008:%d:Input> '%s'\n",
                 i, input_hex);
          pdf_dealloc(input_hex);
        }
      
      /* Set expected data */
      expected_data1 = (pdf_char_t *)utf16be_strings[i].utf32be_data;
      expected_size1 = utf16be_strings[i].utf32be_size;
      expected_data2 = (pdf_char_t *)utf16be_strings[i+1].utf32be_data;
      expected_size2 = utf16be_strings[i+1].utf32be_size;
      expected_data3 = (pdf_char_t *)utf16be_strings[i+2].utf32be_data;
      expected_size3 = utf16be_strings[i+2].utf32be_size;
      if(!pdf_text_test_big_endian_system())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data1 =pdf_text_test_change_utf32_endianness(expected_data1,
                                                                expected_size1);
          expected_data2 =pdf_text_test_change_utf32_endianness(expected_data2,
                                                                expected_size2);
          expected_data3 =pdf_text_test_change_utf32_endianness(expected_data3,
                                                                expected_size3);
          /* Just in case... */
          fail_if(expected_data1 == NULL);
          fail_if(expected_data2 == NULL);
          fail_if(expected_data3 == NULL);
        }
      
      /* 1. The call to pdf_text_new_from_pdf_string should return PDF_OK.
       */
      fail_if(pdf_text_new_from_pdf_string(input_data,
                                           input_size,
                                           &remaining_str,
                                           &remaining_length,
                                           &text1) != PDF_OK);
      
      /* 2. The function should return a valid pointer to the new text object */
      fail_if(text1 == NULL);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          pdf_char_t *expected_hex = NULL;

          pdf_size_t actual_size;
          pdf_char_t *actual_data;
          fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text1,
                                           PDF_TEXT_UTF32_HE,0) == PDF_OK);
          internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
          expected_hex = pdf_text_test_get_hex(expected_data1,
                                               expected_size1,':');
          fail_if((internal_hex == NULL) || \
                  (expected_hex == NULL));
          printf("pdf_text_new_from_pdf_string_008:%d:Internal_1> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_new_from_pdf_string_008:%d:Expected_1> '%s'\n",
                 i, expected_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
          printf("pdf_text_new_from_pdf_string_008:%d:Lang_1> '%s'\n",
                 i, pdf_text_get_language(text1));
          printf("pdf_text_new_from_pdf_string_008:%d:Country_1> '%s'\n",
                 i, pdf_text_get_country(text1));
          printf("pdf_text_new_from_pdf_string_008:%d:remaining_length_1> "
                 "'%ld'\n", i, (long)remaining_length);
          printf("pdf_text_new_from_pdf_string_008:%d:remaining_length_1> "
                 "'%s'\n", i, ((remaining_str == NULL)?"NULL":"not NULL"));
        }
      
      /* 3. The contents of the first text object must be the expected ones. */
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text1,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size1);
      fail_unless(memcmp(actual_data, expected_data1, expected_size1)==0);
      
      /* 4. The languange code within the first text object must be the expected
       *    one */
      fail_unless(strlen(pdf_text_get_language(text1)) == 2);
      fail_unless(strcmp(pdf_text_get_language(text1), \
                         language_code1) == 0);
      
      /* 5. The country code of the output object should be empty */
      fail_unless(strlen(pdf_text_get_country(text1)) == 2);
      fail_unless(strcmp(pdf_text_get_country(text1), \
                         country_code1) == 0);
      
      /* 6. The first call should return a valid remaining_str pointer and a
       *      non-zero remaining_length */
      fail_unless(remaining_str != NULL);
      fail_unless(remaining_length != 0);
      
      
      /* 7. The second call to @code{pdf_text_new_from_pdf_string} should return
       *      PDF_OK. */
      fail_if(pdf_text_new_from_pdf_string(remaining_str,
                                           remaining_length,
                                           &remaining_str,
                                           &remaining_length,
                                           &text2) != PDF_OK);
      
      /* 8. The function should return a valid pointer to the new text object */
      fail_if(text2 == NULL);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          pdf_char_t *expected_hex = NULL;

          pdf_size_t actual_size;
          pdf_char_t *actual_data;
          fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text2,
                                           PDF_TEXT_UTF32_HE,0) == PDF_OK);
          internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
          expected_hex = pdf_text_test_get_hex(expected_data2,
                                               expected_size2,':');
          fail_if((internal_hex == NULL) || \
                  (expected_hex == NULL));
          printf("pdf_text_new_from_pdf_string_008:%d:Internal_2> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_new_from_pdf_string_008:%d:Expected_2> '%s'\n",
                 i, expected_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
          printf("pdf_text_new_from_pdf_string_008:%d:Lang_2> '%s'\n",
                 i, pdf_text_get_language(text2));
          printf("pdf_text_new_from_pdf_string_008:%d:Country_2> '%s'\n",
                 i, pdf_text_get_country(text2));
          printf("pdf_text_new_from_pdf_string_008:%d:remaining_length_2> "
                 "'%ld'\n", i, (long)remaining_length);
          printf("pdf_text_new_from_pdf_string_008:%d:remaining_length_2> "
                 "'%s'\n", i, ((remaining_str == NULL)?"NULL":"not NULL"));
        }
      
      /* 9. The contents of the second text object must be the expected ones. */
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text2,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size2);
      fail_unless(memcmp(actual_data, expected_data2, expected_size2)==0);
      
      /* 10. The languange code within the second text object must be the 
       *    expected one */
      fail_unless(strlen(pdf_text_get_language(text2)) == 2);
      fail_unless(strcmp(pdf_text_get_language(text2), \
                         language_code2) == 0);
      
      /* 11. The country code of the second output object should be empty */
      fail_unless(strlen(pdf_text_get_country(text2)) == 2);
      fail_unless(strcmp(pdf_text_get_country(text2), \
                         country_code2) == 0);

      /* 12. The second call should return a valid remaining_str pointer and a
       *      non-zero remaining_length */
      fail_unless(remaining_str != NULL);
      fail_unless(remaining_length != 0);
      
      
      /* 13. The third call to @code{pdf_text_new_from_pdf_string} should 
       *      return PDF_OK. */
      fail_if(pdf_text_new_from_pdf_string(remaining_str,
                                           remaining_length,
                                           &remaining_str,
                                           &remaining_length,
                                           &text3) != PDF_OK);
      
      /* 14. The function should return a valid pointer to the new text object */
      fail_if(text3 == NULL);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          pdf_char_t *expected_hex = NULL;

          pdf_size_t actual_size;
          pdf_char_t *actual_data;
          fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text3,
                                           PDF_TEXT_UTF32_HE,0) == PDF_OK);
          internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
          expected_hex = pdf_text_test_get_hex(expected_data3,
                                               expected_size3,':');
          fail_if((internal_hex == NULL) || \
                  (expected_hex == NULL));
          printf("pdf_text_new_from_pdf_string_008:%d:Internal_3> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_new_from_pdf_string_008:%d:Expected_3> '%s'\n",
                 i, expected_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
          printf("pdf_text_new_from_pdf_string_008:%d:Lang_3> '%s'\n",
                 i, pdf_text_get_language(text3));
          printf("pdf_text_new_from_pdf_string_008:%d:Country_3> '%s'\n",
                 i, pdf_text_get_country(text3));
          printf("pdf_text_new_from_pdf_string_008:%d:remaining_length_3> "
                 "'%ld'\n", i, (long)remaining_length);
          printf("pdf_text_new_from_pdf_string_008:%d:remaining_length_3> "
                 "'%s'\n", i, ((remaining_str == NULL)?"NULL":"not NULL"));
        }
      
      /* 15. The contents of the third text object must be the expected ones. */
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text3,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size3);
      fail_unless(memcmp(actual_data, expected_data3, expected_size3)==0);
      
      /* 16. The languange code within the third text object must be the 
       *    expected one */
      fail_unless(strlen(pdf_text_get_language(text3)) == 2);
      fail_unless(strcmp(pdf_text_get_language(text3), \
                         language_code3) == 0);
      
      /* 17. The country code of the third output object should be empty */
      fail_unless(strlen(pdf_text_get_country(text3)) == 2);
      fail_unless(strcmp(pdf_text_get_country(text3), \
                         country_code3) == 0);
      
      /* 18. The third call should return a NULL @code{remaining_str} pointer
       *      and a zero @code{remaining_length} */
      fail_unless(remaining_str == NULL);
      fail_unless(remaining_length == 0);
      
      
      fail_if(pdf_text_destroy(text1) != PDF_OK);
      fail_if(pdf_text_destroy(text2) != PDF_OK);
      fail_if(pdf_text_destroy(text3) != PDF_OK);
      
      if(expected_free)
        {
          pdf_dealloc(expected_data1);
          pdf_dealloc(expected_data2);
          pdf_dealloc(expected_data3);
        }
      pdf_dealloc(input_data);
      ++i;
    }

}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_text_new_from_pdf_string (void)
{
  TCase *tc = tcase_create("pdf_text_new_from_pdf_string");
  tcase_add_test(tc, pdf_text_new_from_pdf_string_001);
  tcase_add_test(tc, pdf_text_new_from_pdf_string_002);
  tcase_add_test(tc, pdf_text_new_from_pdf_string_003);
  tcase_add_test(tc, pdf_text_new_from_pdf_string_004);
  tcase_add_test(tc, pdf_text_new_from_pdf_string_005);
  tcase_add_test(tc, pdf_text_new_from_pdf_string_006);
  tcase_add_test(tc, pdf_text_new_from_pdf_string_007);
  tcase_add_test(tc, pdf_text_new_from_pdf_string_008);
  tcase_add_test(tc, pdf_text_new_from_pdf_string_009);
  return tc;
}

/* End of pdf-text-new-from-pdf-string.c */

/* -*- mode: C -*-
 *
 *       File:         pdf-text-get-unicode.c
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_get_unicode test case
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

#define INTERACTIVE_DEBUG   0



/*
 * Test: pdf_text_get_unicode_001
 * Description:
 *   Get the contents of a non-empty text object in UTF-8 without BOM. The
 *   contents of the text object include characters that are encoded in UTF-8
 *   using 8-bit, 16-bit, 24-bit and 32-bit. 
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must be the expected one, not NUL terminated.
 *    3. The returned length must be the expected one.
 */
START_TEST(pdf_text_get_unicode_001)
{
  


  extern const test_string_t utf8_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf8_strings[i].data != NULL)
    {
      pdf_text_t text;
      pdf_char_t *data = NULL;
      pdf_size_t size = 0;
      const pdf_char_t *expected_data;
      pdf_size_t expected_size;
      
      /* Set expected data and size (without BOM) */
      expected_data = (pdf_char_t *)&(utf8_strings[i].data[3]);
      expected_size = utf8_strings[i].size -3;
      
      fail_if(pdf_text_new_from_unicode((pdf_char_t *) \
                                        utf8_strings[i].utf32be_data,
                                        (pdf_size_t) \
                                        utf8_strings[i].utf32be_size,
                                        PDF_TEXT_UTF32_BE,
                                        &text) != PDF_OK);
      
      
      /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
      fail_unless(pdf_text_get_unicode(&data,
                                       &size,
                                       text,
                                       PDF_TEXT_UTF8,
                                       PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
      
      
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
          fail_if(expected_hex == NULL);
          fail_if(internal_hex == NULL);
          printf("pdf_text_get_unicode_001:%d:Internal> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_get_unicode_001:%d:Expected> '%s'\n",
                 i, expected_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
        }
      
      /* 2. The returned string must be the expected one, not NUL terminated */
      fail_if(data == NULL);
      fail_unless(memcmp(expected_data, data, size) == 0);

      /* 3. The returned length must be the expected one. */
      fail_unless(size == expected_size);

      pdf_text_destroy(text);
      pdf_dealloc(data);
      
      ++i;
    }
  
}
END_TEST


/*
 * Test: pdf_text_get_unicode_002
 * Description:
 *   Get the contents of a non-empty text object in UTF-8 with BOM. The
 *   contents of the text object include characters that are encoded in UTF-8
 *   using 8-bit, 16-bit, 24-bit and 32-bit. 
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must be the expected one, not NUL terminated.
 *    3. The returned length must be the expected one, including the length of
 *       the BOM in UTF-8.
 */
START_TEST(pdf_text_get_unicode_002)
{
  


  extern const test_string_t utf8_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf8_strings[i].data != NULL)
    {
      pdf_text_t text;
      pdf_char_t *data = NULL;
      pdf_size_t size = 0;
      const pdf_char_t *expected_data;
      pdf_size_t expected_size;
      
      /* Set expected data and size (with BOM) */
      expected_data = (pdf_char_t *)&(utf8_strings[i].data[0]);
      expected_size = utf8_strings[i].size;
      
      fail_if(pdf_text_new_from_unicode((pdf_char_t *) \
                                        utf8_strings[i].utf32be_data,
                                        (pdf_size_t) \
                                        utf8_strings[i].utf32be_size,
                                        PDF_TEXT_UTF32_BE,
                                        &text) != PDF_OK);
      
      
      /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
      fail_unless(pdf_text_get_unicode(&data,
                                       &size,
                                       text,
                                       PDF_TEXT_UTF8,
                                       PDF_TEXT_UNICODE_WITH_BOM) == PDF_OK);
      
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          pdf_char_t *expected_hex = NULL;
          pdf_char_t *output_hex = NULL;
          pdf_size_t actual_size;
          pdf_char_t *actual_data;
          fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                           PDF_TEXT_UTF32_HE,0) == PDF_OK);
          internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
          expected_hex = pdf_text_test_get_hex(expected_data,expected_size,':');
          output_hex = pdf_text_test_get_hex(data,size,':');
          fail_if(expected_hex == NULL);
          fail_if(internal_hex == NULL);
          fail_if(output_hex == NULL);
          printf("pdf_text_get_unicode_002:%d:Internal> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_get_unicode_002:%d:Expected> '%s'\n",
                 i, expected_hex);
          printf("pdf_text_get_unicode_002:%d:Output> '%s'\n",
                 i, output_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
          pdf_dealloc(output_hex);
        }
      
      /* 2. The returned string must be the expected one, not NUL terminated */
      fail_if(data == NULL);
      fail_unless(memcmp(expected_data, data, size) == 0);

      /* 3. The returned length must be the expected one, including the length
       *       of the BOM in UTF-8. */
      fail_unless(size == expected_size);
      
      pdf_text_destroy(text);
      pdf_dealloc(data);
      
      ++i;
    }
  
}
END_TEST


/*
 * Test: pdf_text_get_unicode_003
 * Description:
 *   Get the contents of an empty text object in UTF-8 without BOM. 
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must be NULL.
 *    3. The returned length must be zero.
 */
START_TEST(pdf_text_get_unicode_003)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF8,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The returned string must be NULL */
  fail_unless(data == NULL);
  /* 3. The returned length must be zero. */
  fail_unless(size == 0);
  
  pdf_text_destroy(text);  
}
END_TEST


/*
 * Test: pdf_text_get_unicode_004
 * Description:
 *   Get the contents of an empty text object in UTF-8 with BOM. 
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must only contain the BOM in UTF-8, not NUL
 *       terminated.
 *    3. The returned length must be equal to the length of the BOM in UTF-8.
 */
START_TEST(pdf_text_get_unicode_004)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF8,
                                   PDF_TEXT_UNICODE_WITH_BOM) == PDF_OK);
  
  /* 2. The returned string must only contain the BOM in UTF-8, not NUL
   *       terminated. */
  fail_unless(data != NULL);
  fail_unless(memcmp(&data[0], "\xEF\xBB\xBF", 3) == 0);

  /* 3. The returned length must be equal to the length of the BOM in UTF-8. */
  fail_unless(size == 3);
  
  pdf_text_destroy(text);
  pdf_dealloc(data);
}
END_TEST


/*
 * Test: pdf_text_get_unicode_005
 * Description:
 *   Get the contents of a non-empty text object with lang/country info, in
 *   UTF-8 without BOM and with lang/country information embedded (which should
 *   not be supported in UTF-8).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_005)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  pdf_char_t *utf8data = (pdf_char_t *)"GNU's not Unix";
  pdf_size_t utf8size = strlen((char *)utf8data);
  const pdf_char_t *language = (pdf_char_t *)"en";
  const pdf_char_t *country = (pdf_char_t *)"GB";
  
  fail_if(pdf_text_new_from_unicode(utf8data, utf8size,
                                    PDF_TEXT_UTF8,
                                    &text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  fail_if(pdf_text_set_country(text, country) != PDF_OK);
  

  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF8,
                                   PDF_TEXT_UTF16BE_WITH_LANGCODE) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST


/*
 * Test: pdf_text_get_unicode_006
 * Description:
 *   Get the contents of a non-empty text object with lang/country info, in
 *   UTF-8 with BOM and with lang/country information embedded (which should
 *   not be supported in UTF-8).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_006)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  pdf_char_t *utf8data = (pdf_char_t *)"GNU's not Unix";
  pdf_size_t utf8size = strlen((char *)utf8data);
  const pdf_char_t *language = (pdf_char_t *)"en";
  const pdf_char_t *country = (pdf_char_t *)"GB";
  
  fail_if(pdf_text_new_from_unicode(utf8data, utf8size,
                                    PDF_TEXT_UTF8,
                                    &text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  fail_if(pdf_text_set_country(text, country) != PDF_OK);

  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF8,
                                   (PDF_TEXT_UNICODE_WITH_BOM | \
                                    PDF_TEXT_UTF16BE_WITH_LANGCODE)) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST


/*
 * Test: pdf_text_get_unicode_007
 * Description:
 *   Get the contents of a non-empty text object in UTF-16BE without BOM. The
 *   contents of the text object include characters that are encoded in UTF-16
 *   using 16-bit and 32-bit.
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must be the expected one, not NUL terminated.
 *    3. The returned length must be the expected one.
 */
START_TEST(pdf_text_get_unicode_007)
{
  


  extern const test_string_t utf16be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf16be_strings[i].data != NULL)
    {
      pdf_text_t text;
      pdf_char_t *data = NULL;
      pdf_size_t size = 0;
      const pdf_char_t *expected_data;
      pdf_size_t expected_size;
      
      /* Set expected data and size (without BOM) */
      expected_data = (pdf_char_t *)&(utf16be_strings[i].data[2]);
      expected_size = utf16be_strings[i].size -2;
      
      fail_if(pdf_text_new_from_unicode((pdf_char_t *) \
                                        utf16be_strings[i].utf32be_data,
                                        (pdf_size_t) \
                                        utf16be_strings[i].utf32be_size,
                                        PDF_TEXT_UTF32_BE,
                                        &text) != PDF_OK);
      
      
      /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
      fail_unless(pdf_text_get_unicode(&data,
                                       &size,
                                       text,
                                       PDF_TEXT_UTF16_BE,
                                       PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
      
      
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
          fail_if(expected_hex == NULL);
          fail_if(internal_hex == NULL);
          printf("pdf_text_get_unicode_007:%d:Internal> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_get_unicode_007:%d:Expected> '%s'\n",
                 i, expected_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
        }
      
      /* 2. The returned string must be the expected one, not NUL terminated */
      fail_if(data == NULL);
      fail_unless(memcmp(expected_data, data, size) == 0);

      /* 3. The returned length must be the expected one. */
      fail_unless(size == expected_size);
      
      pdf_text_destroy(text);
      pdf_dealloc(data);
      
      ++i;
    }
  
}
END_TEST


/*
 * Test: pdf_text_get_unicode_008
 * Description:
 *   Get the contents of a non-empty text object in UTF-16BE with BOM. The
 *   contents of the text object include characters that are encoded in UTF-16
 *   using 16-bit and 32-bit.
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must be the expected one, not NUL terminated.
 *    3. The returned length must be the expected one.
 */
START_TEST(pdf_text_get_unicode_008)
{
  


  extern const test_string_t utf16be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf16be_strings[i].data != NULL)
    {
      pdf_text_t text;
      pdf_char_t *data = NULL;
      pdf_size_t size = 0;
      const pdf_char_t *expected_data;
      pdf_size_t expected_size;
      
      /* Set expected data and size (without BOM) */
      expected_data = (pdf_char_t *)&(utf16be_strings[i].data[0]);
      expected_size = utf16be_strings[i].size;
      
      fail_if(pdf_text_new_from_unicode((pdf_char_t *) \
                                        utf16be_strings[i].utf32be_data,
                                        (pdf_size_t) \
                                        utf16be_strings[i].utf32be_size,
                                        PDF_TEXT_UTF32_BE,
                                        &text) != PDF_OK);
      
      
      /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
      fail_unless(pdf_text_get_unicode(&data,
                                       &size,
                                       text,
                                       PDF_TEXT_UTF16_BE,
                                       PDF_TEXT_UNICODE_WITH_BOM) == PDF_OK);
      
      
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
          fail_if(expected_hex == NULL);
          fail_if(internal_hex == NULL);
          printf("pdf_text_get_unicode_008:%d:Internal> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_get_unicode_008:%d:Expected> '%s'\n",
                 i, expected_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
        }
      
      /* 2. The returned string must be the expected one, not NUL terminated */
      fail_if(data == NULL);
      fail_unless(memcmp(expected_data, data, size) == 0);

      /* 3. The returned length must be the expected one. */
      fail_unless(size == expected_size);
      
      pdf_text_destroy(text);
      pdf_dealloc(data);
      
      ++i;
    }
  
}
END_TEST


/*
 * Test: pdf_text_get_unicode_009
 * Description:
 *   Get the contents of an empty text object in UTF-16BE without BOM. 
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must be NULL.
 *    3. The returned length must be zero.
 */
START_TEST(pdf_text_get_unicode_009)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF16_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The returned string must be NULL */
  fail_unless(data == NULL);
  /* 3. The returned length must be zero. */
  fail_unless(size == 0);
  
  pdf_text_destroy(text);  
}
END_TEST


/*
 * Test: pdf_text_get_unicode_010
 * Description:
 *   Get the contents of an empty text object in UTF-16BE with BOM. 
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must only contain the BOM in UTF-16BE, not NUL
 *       terminated.
 *    3. The returned length must be equal to the length of the BOM in UTF-16BE.
 */
START_TEST(pdf_text_get_unicode_010)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF16_BE,
                                   PDF_TEXT_UNICODE_WITH_BOM) == PDF_OK);
  
  /* 2. The returned string must only contain the BOM in UTF-16BE, not NUL
   *       terminated. */
  fail_unless(data != NULL);
  fail_unless(memcmp(&data[0], "\xFE\xFF", 2) == 0);

  /* 3. The returned length must be equal to the length of the BOM in UTF-16BE.
   */
  fail_unless(size == 2);

  
  pdf_text_destroy(text); 
  pdf_dealloc(data);
}
END_TEST


/*
 * Test: pdf_text_get_unicode_011
 * Description:
 *   Get the contents of a non-empty text object with lang/country info, in
 *   UTF-16BE without BOM and with lang/country information embedded (which IS
 *   supported in UTF-16BE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must be the expected one, including the
 *       lang/country information embedded, not NUL terminated.
 *    3. The returned length must be the expected one, including the length of
 *       the lang/country info.
 */
START_TEST(pdf_text_get_unicode_011)
{
  


  extern const test_string_t utf16be_strings[];
  int i;

  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);

  i = 0;
  while(utf16be_strings[i].data != NULL)
    {
      pdf_text_t text;
      pdf_char_t *data = NULL;
      pdf_size_t size = 0;
      const pdf_char_t *language = (pdf_char_t *)"en";
      const pdf_char_t *country = (pdf_char_t *)"GB";
      const pdf_char_t *expected_langcode = (pdf_char_t *) \
                                            "\x00\x1B" "enGB" "\x00\x1B";
      pdf_size_t expected_langcode_size = 8;
      const pdf_char_t *expected_data = (pdf_char_t *) \
                                        &utf16be_strings[i].data[2];
      pdf_size_t expected_size = utf16be_strings[i].size -2;
      
      fail_if(pdf_text_new_from_unicode((pdf_char_t *) \
                                        utf16be_strings[i].utf32be_data,
                                        utf16be_strings[i].utf32be_size,
                                        PDF_TEXT_UTF32_BE,
                                        &text) != PDF_OK);
      fail_if(pdf_text_set_language(text, language) != PDF_OK);
      fail_if(pdf_text_set_country(text, country) != PDF_OK);
      
      /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
      fail_unless(pdf_text_get_unicode(&data,
                                       &size,
                                       text,
                                       PDF_TEXT_UTF16_BE,
                                       PDF_TEXT_UTF16BE_WITH_LANGCODE)==PDF_OK);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          pdf_char_t *expected_hex = NULL;
          pdf_char_t *output_hex = NULL;
          pdf_size_t actual_size;
          pdf_char_t *actual_data;
          fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                           PDF_TEXT_UTF32_HE,0) == PDF_OK);
          internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
          expected_hex = pdf_text_test_get_hex(expected_data,expected_size,':');
          output_hex = pdf_text_test_get_hex(data,size,':');          
          fail_if(expected_hex == NULL);
          fail_if(internal_hex == NULL);
          fail_if(output_hex == NULL);
          printf("pdf_text_get_unicode_011:%d:Internal> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_get_unicode_011:%d:Expected> '%s'\n",
                 i, expected_hex);
          printf("pdf_text_get_unicode_011:%d:Output> '%s'\n",
                 i, output_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
          pdf_dealloc(output_hex);
        }
      
      /* 2. The returned string must be the expected one, including the
       *       lang/country information embedded, not NUL terminated. */
      fail_unless(memcmp(&data[0],
                         expected_langcode,
                         expected_langcode_size)==0);
      fail_unless(memcmp(&data[expected_langcode_size],
                         expected_data,
                         expected_size)==0);
      
      /* 3. The returned length must be the expected one, including the length
       *       of the lang/country info. */
      fail_unless(size == (expected_size + expected_langcode_size));

      pdf_text_destroy(text);
      pdf_dealloc(data);
      ++i;
    }

}
END_TEST


/*
 * Test: pdf_text_get_unicode_012
 * Description:
 *   Get the contents of a non-empty text object with language info (no country
 *   info), in UTF-16BE without BOM and with lang/country information embedded
 *   (which IS supported in UTF-16BE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must be the expected one, including the language
 *       information embedded, not NUL terminated.
 *    3. The returned length must be the expected one, including the length of
 *       the language info.
 */
START_TEST(pdf_text_get_unicode_012)
{  
  


  extern const test_string_t utf16be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf16be_strings[i].data != NULL)
    {
      pdf_text_t text;
      pdf_char_t *data = NULL;
      pdf_size_t size = 0;
      const pdf_char_t *language = (pdf_char_t *)"en";
      const pdf_char_t *expected_langcode = (pdf_char_t *) \
                                            "\x00\x1B" "en" "\x00\x1B";
      pdf_size_t expected_langcode_size = 6;
      const pdf_char_t *expected_data = (pdf_char_t *) \
                                        &utf16be_strings[i].data[2];
      pdf_size_t expected_size = utf16be_strings[i].size -2;
      
      fail_if(pdf_text_new_from_unicode((pdf_char_t *) \
                                        utf16be_strings[i].utf32be_data,
                                        utf16be_strings[i].utf32be_size,
                                        PDF_TEXT_UTF32_BE,
                                        &text) != PDF_OK);
      fail_if(pdf_text_set_language(text, language) != PDF_OK);      
      
      
      /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
      fail_unless(pdf_text_get_unicode(&data,
                                       &size,
                                       text,
                                       PDF_TEXT_UTF16_BE,
                                       PDF_TEXT_UTF16BE_WITH_LANGCODE)==PDF_OK);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          pdf_char_t *expected_hex = NULL;
          pdf_char_t *output_hex = NULL;
          pdf_size_t actual_size;
          pdf_char_t *actual_data;
          fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                           PDF_TEXT_UTF32_HE,0) == PDF_OK);
          internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
          expected_hex = pdf_text_test_get_hex(expected_data,expected_size,':');
          output_hex = pdf_text_test_get_hex(data,size,':');          
          fail_if(expected_hex == NULL);
          fail_if(internal_hex == NULL);
          fail_if(output_hex == NULL);
          printf("pdf_text_get_unicode_012:%d:Internal> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_get_unicode_012:%d:Expected> '%s'\n",
                 i, expected_hex);
          printf("pdf_text_get_unicode_012:%d:Output> '%s'\n",
                 i, output_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
          pdf_dealloc(output_hex);
        }
      
      /* 2. The returned string must be the expected one, including the
       *       language information embedded, not NUL terminated. */
      fail_unless(memcmp(&data[0],
                         expected_langcode,
                         expected_langcode_size)==0);
      fail_unless(memcmp(&data[expected_langcode_size],
                         expected_data,
                         expected_size)==0);
      
      /* 3. The returned length must be the expected one, including the length
       *       of the language info. */
      fail_unless(size == (expected_size + expected_langcode_size));
      
      pdf_text_destroy(text);
      pdf_dealloc(data);
      ++i;
    }


}
END_TEST


/*
 * Test: pdf_text_get_unicode_013
 * Description:
 *   Get the contents of an empty text object with lang/country info, in
 *   UTF-16BE without BOM and with lang/country information embedded (which IS
 *   supported in UTF-16BE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must only include the lang/country information
 *       embedded, not NUL terminated.
 *    3. The returned length must be equal to the length of the lang/country
 *       info.
 */
START_TEST(pdf_text_get_unicode_013)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  const pdf_char_t *language = (pdf_char_t *)"en";
  const pdf_char_t *country = (pdf_char_t *)"GB";
  const pdf_char_t *expected_langcode = (pdf_char_t *) \
                                        "\x00\x1B" "enGB" "\x00\x1B";
  pdf_size_t expected_langcode_size = 8;
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  fail_if(pdf_text_set_country(text, country) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF16_BE,
                                   PDF_TEXT_UTF16BE_WITH_LANGCODE) == PDF_OK);

  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *output_hex = NULL;
      output_hex = pdf_text_test_get_hex(data,size,':');          
      fail_if(output_hex == NULL);
      printf("pdf_text_get_unicode_013:Output> '%s'\n",
             output_hex);
      pdf_dealloc(output_hex);
    }

  /* 2. The returned string must only include the lang/country information
   *       embedded, not NUL terminated. */
  fail_unless(data != NULL);
  fail_unless(memcmp(&data[0], expected_langcode, expected_langcode_size) == 0);

  /* 3. The returned length must be equal to the length of the lang/country
   *       info. */
  fail_unless(size == expected_langcode_size);
  
  pdf_text_destroy(text);
  pdf_dealloc(data);
}
END_TEST


/*
 * Test: pdf_text_get_unicode_014
 * Description:
 *   Get the contents of an empty text object with language info (no country
 *   info), in UTF-16BE without BOM and with lang/country information embedded
 *   (which IS supported in UTF-16BE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must only contain the language information
 *       embedded, not NUL terminated.
 *    3. The returned length must be equal to the length of the language info.
 */
START_TEST(pdf_text_get_unicode_014)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  const pdf_char_t *language = (pdf_char_t *)"en";
  const pdf_char_t *expected_langcode = (pdf_char_t *) \
                                        "\x00\x1B" "en" "\x00\x1B";
  pdf_size_t expected_langcode_size = 6;
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF16_BE,
                                   PDF_TEXT_UTF16BE_WITH_LANGCODE) == PDF_OK);
  
  /* 2. The returned string must only contain the language information
   *       embedded, not NUL terminated. */
  fail_unless(data != NULL);
  fail_unless(memcmp(&data[0], expected_langcode, expected_langcode_size) == 0);

  /* 3. The returned length must be equal to the length of the language info. */
  fail_unless(size == expected_langcode_size);
  
  pdf_text_destroy(text);
  pdf_dealloc(data);
}
END_TEST


/*
 * Test: pdf_text_get_unicode_015
 * Description:
 *   Get the contents of a non-empty text object with lang/country info, in
 *   UTF-16BE with BOM and with lang/country information embedded (which IS
 *   supported in UTF-16BE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must be the expected one, including the BOM and the
 *       lang/country information embedded, not NUL terminated.
 *    3. The returned length must be the expected one, including the length of
 *       the lang/country info and the BOM.
 */
START_TEST(pdf_text_get_unicode_015)
{
  extern const test_string_t utf16be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf16be_strings[i].data != NULL)
    {
      pdf_text_t text;
      pdf_char_t *data = NULL;
      pdf_size_t size = 0;
      const pdf_char_t *language = (pdf_char_t *)"en";
      const pdf_char_t *country = (pdf_char_t *)"GB";
      const pdf_char_t *expected_langcode = (pdf_char_t *) \
                                            "\x00\x1B" "enGB" "\x00\x1B";
      pdf_size_t expected_langcode_size = 8;
      const pdf_char_t *expected_data = (pdf_char_t *) \
                                        &utf16be_strings[i].data[2];
      pdf_size_t expected_size = utf16be_strings[i].size-2;
      
      fail_if(pdf_text_new_from_unicode((pdf_char_t *) \
                                        utf16be_strings[i].utf32be_data,
                                        utf16be_strings[i].utf32be_size,
                                        PDF_TEXT_UTF32_BE,
                                        &text) != PDF_OK);
      fail_if(pdf_text_set_language(text, language) != PDF_OK);
      fail_if(pdf_text_set_country(text, country) != PDF_OK);
      
      /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
      fail_unless(pdf_text_get_unicode(&data,
                                       &size,
                                       text,
                                       PDF_TEXT_UTF16_BE,
                                       (PDF_TEXT_UTF16BE_WITH_LANGCODE | \
                                        PDF_TEXT_UNICODE_WITH_BOM)) == PDF_OK);
      
      /* 2. The returned string must be the expected one, including the BOM and
       *       the lang/country information embedded, not NUL terminated. */
      fail_unless(memcmp(&data[0], "\xFE\xFF", 2)==0);
      fail_unless(memcmp(&data[2],
                         expected_langcode,
                         expected_langcode_size)==0);
      fail_unless(memcmp(&data[2+expected_langcode_size],
                         expected_data,
                         expected_size)==0);
      
      /* 3. The returned length must be the expected one, including the length
       *       of the lang/country info and the BOM. */
      fail_unless(size == (2 + expected_size + expected_langcode_size));
      
      pdf_text_destroy(text);
      pdf_dealloc(data);
      ++i;
    }
}
END_TEST


/*
 * Test: pdf_text_get_unicode_016
 * Description:
 *   Get the contents of a non-empty text object with language info (no country
 *   info), in UTF-16BE with BOM and with lang/country information embedded
 *   (which IS supported in UTF-16BE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must be the expected one, including the language
 *       information embedded, not NUL terminated.
 *    3. The returned length must be the expected one, including the length of
 *       the language info.
 */
START_TEST(pdf_text_get_unicode_016)
{  
  extern const test_string_t utf16be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf16be_strings[i].data != NULL)
    {
      pdf_text_t text;
      pdf_char_t *data = NULL;
      pdf_size_t size = 0;
      const pdf_char_t *language = (pdf_char_t *)"en";
      const pdf_char_t *expected_langcode = (pdf_char_t *) \
                                            "\x00\x1B" "en" "\x00\x1B";
      pdf_size_t expected_langcode_size = 6;
      const pdf_char_t *expected_data = (pdf_char_t *) \
                                        &utf16be_strings[i].data[2];
      pdf_size_t expected_size = utf16be_strings[i].size -2;
      
      fail_if(pdf_text_new_from_unicode((pdf_char_t *) \
                                        utf16be_strings[i].utf32be_data,
                                        utf16be_strings[i].utf32be_size,
                                        PDF_TEXT_UTF32_BE,
                                        &text) != PDF_OK);
      fail_if(pdf_text_set_language(text, language) != PDF_OK);      
      
      
      /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
      fail_unless(pdf_text_get_unicode(&data,
                                       &size,
                                       text,
                                       PDF_TEXT_UTF16_BE,
                                       (PDF_TEXT_UTF16BE_WITH_LANGCODE | \
                                        PDF_TEXT_UNICODE_WITH_BOM)) == PDF_OK);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *output_hex = NULL;
          output_hex = pdf_text_test_get_hex(data,size,':');          
          fail_if(output_hex == NULL);
          printf("pdf_text_get_unicode_016:Output> '%s'\n",
                 output_hex);
          pdf_dealloc(output_hex);
        }
      
      /* 2. The returned string must be the expected one, including the
       *       lang/country information embedded, not NUL terminated. */
      fail_unless(memcmp(&data[0], "\xFE\xFF", 2)==0);
      fail_unless(memcmp(&data[2],
                         expected_langcode,
                         expected_langcode_size)==0);
      fail_unless(memcmp(&data[2+expected_langcode_size],
                         expected_data,
                         expected_size)==0);
      
      /* 3. The returned length must be the expected one, including the length
       *       of the lang/country info. */
      fail_unless(size == (2 + expected_size + expected_langcode_size));
      
      pdf_text_destroy(text);
      pdf_dealloc(data);
      ++i;
    }
}
END_TEST


/*
 * Test: pdf_text_get_unicode_017
 * Description:
 *   Get the contents of an empty text object with lang/country info, in
 *   UTF-16BE with BOM and with lang/country information embedded (which IS
 *   supported in UTF-16BE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must only include the BOM and lang/country
 *       information embedded, not NUL terminated.
 *    3. The returned length must be equal to the length of the lang/country
 *       info plus the length of the BOM.
 */
START_TEST(pdf_text_get_unicode_017)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  const pdf_char_t *language = (pdf_char_t *)"en";
  const pdf_char_t *country = (pdf_char_t *)"GB";
  const pdf_char_t *expected_langcode = (pdf_char_t *) \
                                        "\x00\x1B" "enGB" "\x00\x1B";
  pdf_size_t expected_langcode_size = 8;
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  fail_if(pdf_text_set_country(text, country) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF16_BE,
                                   (PDF_TEXT_UTF16BE_WITH_LANGCODE | \
                                    PDF_TEXT_UNICODE_WITH_BOM)) == PDF_OK);
  
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *output_hex = NULL;
      output_hex = pdf_text_test_get_hex(data,size,':');          
      fail_if(output_hex == NULL);
      printf("pdf_text_get_unicode_017:Output> '%s'\n",
             output_hex);
      pdf_dealloc(output_hex);
    }
  
  /* 2. The returned string must only include the BOM and lang/country
   *       information embedded, not NUL terminated. */
  fail_unless(data != NULL);
  fail_unless(memcmp(&data[0], "\xFE\xFF", 2) == 0);
  fail_unless(memcmp(&data[2], expected_langcode, expected_langcode_size) == 0);
  
  /* 3. The returned length must be equal to the length of the lang/country
   *       info plus the length of the BOM. */
  fail_unless(size == (2 + expected_langcode_size));
  
  pdf_text_destroy(text);
  pdf_dealloc(data);
}
END_TEST


/*
 * Test: pdf_text_get_unicode_018
 * Description:
 *   Get the contents of an empty text object with language info (no country
 *   info), in UTF-16BE with BOM and with lang/country information embedded
 *   (which IS supported in UTF-16BE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must only contain the BOM and the language
 *       information embedded, not NUL terminated.
 *    3. The returned length must be equal to the length of the language info
 *       plus the length of the BOM.
 */
START_TEST(pdf_text_get_unicode_018)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  const pdf_char_t *language = (pdf_char_t *)"en";
  const pdf_char_t *expected_langcode = (pdf_char_t *) \
                                        "\x00\x1B" "en" "\x00\x1B";
  pdf_size_t expected_langcode_size = 6;
  
  fail_if(pdf_text_new(&text) != PDF_OK);
  
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF16_BE,
                                   (PDF_TEXT_UTF16BE_WITH_LANGCODE | \
                                    PDF_TEXT_UNICODE_WITH_BOM)) == PDF_OK);
  
  /* 2. The returned string must only contain the BOM and the language
   *       information embedded, not NUL terminated. */
  fail_unless(data != NULL);
  fail_unless(memcmp(&data[0], "\xFE\xFF", 2) == 0);
  fail_unless(memcmp(&data[2], expected_langcode, expected_langcode_size) == 0);
  
  /* 3. The returned length must be equal to the length of the language info
   *       plus the length of the BOM. */
  fail_unless(size == (2 + expected_langcode_size));
  
  pdf_text_destroy(text);
  pdf_dealloc(data);
}
END_TEST



/*
 * Test: pdf_text_get_unicode_019
 * Description:
 *   Get the contents of a non-empty text object in UTF-16LE without BOM. The
 *   contents of the text object include characters that are encoded in UTF-16
 *   using 16-bit and 32-bit.
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must be the expected one, not NUL terminated.
 *    3. The returned length must be the expected one.
 */
START_TEST(pdf_text_get_unicode_019)
{
  extern const test_string_t utf16be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf16be_strings[i].data != NULL)
    {
      pdf_text_t text;
      pdf_char_t *data = NULL;
      pdf_size_t size = 0;
      pdf_char_t *expected_data;
      pdf_size_t expected_size;
      
      /* Set expected data and size (without BOM) */
      expected_data = (pdf_char_t *)&(utf16be_strings[i].data[2]);
      expected_size = utf16be_strings[i].size -2;
      expected_data = pdf_text_test_change_utf16_endianness(expected_data,
                                                            expected_size);
      /* Just in case... */
      fail_if(expected_data == NULL);
      
      fail_if(pdf_text_new_from_unicode((pdf_char_t *) \
                                        utf16be_strings[i].utf32be_data,
                                        (pdf_size_t) \
                                        utf16be_strings[i].utf32be_size,
                                        PDF_TEXT_UTF32_BE,
                                        &text) != PDF_OK);
      
      
      /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
      fail_unless(pdf_text_get_unicode(&data,
                                       &size,
                                       text,
                                       PDF_TEXT_UTF16_LE,
                                       PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
      
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *output_hex = NULL;
          pdf_char_t *expected_hex = NULL;
          output_hex = pdf_text_test_get_hex(data,size,':');
          expected_hex = pdf_text_test_get_hex(expected_data,expected_size,':');
          fail_if(expected_hex == NULL);
          fail_if(output_hex == NULL);
          printf("pdf_text_get_unicode_019:%d:Output> '%s'\n",
                 i, output_hex);
          printf("pdf_text_get_unicode_019:%d:Expected> '%s'\n",
                 i, expected_hex);
          pdf_dealloc(output_hex);
          pdf_dealloc(expected_hex);
        }
      
      /* 2. The returned string must be the expected one, not NUL terminated */
      fail_if(data == NULL);
      fail_unless(memcmp(expected_data, data, size) == 0);

      /* 3. The returned length must be the expected one. */
      fail_unless(size == expected_size);
      
      pdf_text_destroy(text);
      pdf_dealloc(data);
      pdf_dealloc(expected_data);
      
      ++i;
    }
  
}
END_TEST


/*
 * Test: pdf_text_get_unicode_020
 * Description:
 *   Get the contents of a non-empty text object in UTF-16LE with BOM. The
 *   contents of the text object include characters that are encoded in UTF-16
 *   using 16-bit and 32-bit.
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must be the expected one, not NUL terminated.
 *    3. The returned length must be the expected one.
 */
START_TEST(pdf_text_get_unicode_020)
{
  


  extern const test_string_t utf16be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf16be_strings[i].data != NULL)
    {
      pdf_text_t text;
      pdf_char_t *data = NULL;
      pdf_size_t size = 0;
      pdf_char_t *expected_data;
      pdf_size_t expected_size;
      
      /* Set expected data and size (without BOM) */
      expected_data = (pdf_char_t *)&(utf16be_strings[i].data[0]);
      expected_size = utf16be_strings[i].size;
      expected_data = pdf_text_test_change_utf16_endianness(expected_data,
                                                            expected_size);
      /* Just in case... */
      fail_if(expected_data == NULL);
      
      fail_if(pdf_text_new_from_unicode((pdf_char_t *) \
                                        utf16be_strings[i].utf32be_data,
                                        (pdf_size_t) \
                                        utf16be_strings[i].utf32be_size,
                                        PDF_TEXT_UTF32_BE,
                                        &text) != PDF_OK);
      
      
      /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
      fail_unless(pdf_text_get_unicode(&data,
                                       &size,
                                       text,
                                       PDF_TEXT_UTF16_LE,
                                       PDF_TEXT_UNICODE_WITH_BOM) == PDF_OK);
      
      
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
          fail_if(expected_hex == NULL);
          fail_if(internal_hex == NULL);
          printf("pdf_text_get_unicode_020:%d:Internal> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_get_unicode_020:%d:Expected> '%s'\n",
                 i, expected_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
        }
      
      /* 2. The returned string must be the expected one, not NUL terminated */
      fail_if(data == NULL);
      fail_unless(memcmp(expected_data, data, size) == 0);

      /* 3. The returned length must be the expected one. */
      fail_unless(size == expected_size);

      
      pdf_text_destroy(text);
      pdf_dealloc(data);
      pdf_dealloc(expected_data);
      
      ++i;
    }
  
}
END_TEST


/*
 * Test: pdf_text_get_unicode_021
 * Description:
 *   Get the contents of an empty text object in UTF-16LE without BOM. 
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must be NULL.
 *    3. The returned length must be zero.
 */
START_TEST(pdf_text_get_unicode_021)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF16_LE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The returned string must be NULL */
  fail_unless(data == NULL);
  /* 3. The returned length must be zero. */
  fail_unless(size == 0);
  
  pdf_text_destroy(text);  
}
END_TEST


/*
 * Test: pdf_text_get_unicode_022
 * Description:
 *   Get the contents of an empty text object in UTF-16LE with BOM. 
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must only contain the BOM in UTF-16LE, not NUL
 *       terminated.
 *    3. The returned length must be equal to the length of the BOM in UTF-16LE.
 */
START_TEST(pdf_text_get_unicode_022)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF16_LE,
                                   PDF_TEXT_UNICODE_WITH_BOM) == PDF_OK);
  
  /* 2. The returned string must only contain the BOM in UTF-16LE, not NUL
   *       terminated. */
  fail_unless(data != NULL);
  fail_unless(memcmp(&data[0], "\xFF\xFE", 2) == 0);

  /* 3. The returned length must be equal to the length of the BOM in
   *       UTF-16LE. */
  fail_unless(size == 2);
  
  pdf_text_destroy(text); 
  pdf_dealloc(data);
}
END_TEST


/*
 * Test: pdf_text_get_unicode_023
 * Description:
 *   Get the contents of a non-empty text object with lang/country info, in
 *   UTF-16LE without BOM and with lang/country information embedded (which is
 *   NOT supported in UTF-16LE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_023)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  pdf_char_t *utf8data = (pdf_char_t *)"GNU's not Unix";
  pdf_size_t utf8size = strlen((char *)utf8data);
  const pdf_char_t *language = (pdf_char_t *)"en";
  const pdf_char_t *country = (pdf_char_t *)"GB";
  
  fail_if(pdf_text_new_from_unicode(utf8data,
                                    utf8size,
                                    PDF_TEXT_UTF8,
                                    &text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  fail_if(pdf_text_set_country(text, country) != PDF_OK);
  
  
  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF16_LE,
                                   PDF_TEXT_UTF16BE_WITH_LANGCODE) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST

/*
 * Test: pdf_text_get_unicode_024
 * Description:
 *   Get the contents of a non-empty text object with language info (no country
 *   info), in UTF-16LE without BOM and with lang/country information embedded
 *   (which is NOT supported in UTF-16LE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_024)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  pdf_char_t *utf8data = (pdf_char_t *)"GNU's not Unix";
  pdf_size_t utf8size = strlen((char *)utf8data);
  const pdf_char_t *language = (pdf_char_t *)"en";
  
  fail_if(pdf_text_new_from_unicode(utf8data,
                                    utf8size,
                                    PDF_TEXT_UTF8,
                                    &text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  
  
  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF16_LE,
                                   PDF_TEXT_UTF16BE_WITH_LANGCODE) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST


/*
 * Test: pdf_text_get_unicode_025
 * Description:
 *   Get the contents of an empty text object with lang/country info, in
 *   UTF-16LE without BOM and with lang/country information embedded (which is
 *   NOT supported in UTF-16LE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_025)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  const pdf_char_t *language = (pdf_char_t *)"en";
  const pdf_char_t *country = (pdf_char_t *)"GB";
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  fail_if(pdf_text_set_country(text, country) != PDF_OK);
  
  
  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF16_LE,
                                   PDF_TEXT_UTF16BE_WITH_LANGCODE) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST

/*
 * Test: pdf_text_get_unicode_026
 * Description:
 *   Get the contents of an empty text object with language info (no country
 *   info), in UTF-16LE without BOM and with lang/country information embedded
 *   (which is NOT supported in UTF-16LE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_026)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  const pdf_char_t *language = (pdf_char_t *)"en";

  fail_if(pdf_text_new (&text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF16_LE,
                                   PDF_TEXT_UTF16BE_WITH_LANGCODE) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST

/*
 * Test: pdf_text_get_unicode_027
 * Description:
 *   Get the contents of a non-empty text object with lang/country info, in
 *   UTF-16LE with BOM and with lang/country information embedded (which is
 *   NOT supported in UTF-16LE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_027)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  pdf_char_t *utf8data = (pdf_char_t *)"GNU's not Unix";
  pdf_size_t utf8size = strlen((char *)utf8data);
  const pdf_char_t *language = (pdf_char_t *)"en";
  const pdf_char_t *country = (pdf_char_t *)"GB";
  
  fail_if(pdf_text_new_from_unicode(utf8data,
                                    utf8size,
                                    PDF_TEXT_UTF8,
                                    &text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  fail_if(pdf_text_set_country(text, country) != PDF_OK);
  
  
  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF16_LE,
                                   (PDF_TEXT_UTF16BE_WITH_LANGCODE | \
                                    PDF_TEXT_UNICODE_WITH_BOM)) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST

/*
 * Test: pdf_text_get_unicode_028
 * Description:
 *   Get the contents of a non-empty text object with language info (no country
 *   info), in UTF-16LE with BOM and with lang/country information embedded
 *   (which is NOT supported in UTF-16LE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_028)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  pdf_char_t *utf8data = (pdf_char_t *)"GNU's not Unix";
  pdf_size_t utf8size = strlen((char *)utf8data);
  const pdf_char_t *language = (pdf_char_t *)"en";
  
  fail_if(pdf_text_new_from_unicode(utf8data,
                                    utf8size,
                                    PDF_TEXT_UTF8,
                                    &text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  
  
  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF16_LE,
                                   (PDF_TEXT_UTF16BE_WITH_LANGCODE | \
                                    PDF_TEXT_UNICODE_WITH_BOM)) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST


/*
 * Test: pdf_text_get_unicode_029
 * Description:
 *   Get the contents of an empty text object with lang/country info, in
 *   UTF-16LE with BOM and with lang/country information embedded (which is
 *   NOT supported in UTF-16LE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_029)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  const pdf_char_t *language = (pdf_char_t *)"en";
  const pdf_char_t *country = (pdf_char_t *)"GB";
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  fail_if(pdf_text_set_country(text, country) != PDF_OK);
  
  
  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF16_LE,
                                   (PDF_TEXT_UTF16BE_WITH_LANGCODE | \
                                    PDF_TEXT_UNICODE_WITH_BOM)) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST

/*
 * Test: pdf_text_get_unicode_030
 * Description:
 *   Get the contents of an empty text object with language info (no country
 *   info), in UTF-16LE with BOM and with lang/country information embedded
 *   (which is NOT supported in UTF-16LE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_030)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  const pdf_char_t *language = (pdf_char_t *)"en";
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF16_LE,
                                   (PDF_TEXT_UTF16BE_WITH_LANGCODE | \
                                    PDF_TEXT_UNICODE_WITH_BOM)) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST



/*
 * Test: pdf_text_get_unicode_031
 * Description:
 *   Get the contents of a non-empty text object in UTF-32BE without BOM.
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must be the expected one, not NUL terminated.
 *    3. The returned length must be the expected one.
 */
START_TEST(pdf_text_get_unicode_031)
{
  


  extern const test_string_t utf32be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf32be_strings[i].data != NULL)
    {
      pdf_text_t text;
      pdf_char_t *data = NULL;
      pdf_size_t size = 0;
      const pdf_char_t *expected_data;
      pdf_size_t expected_size;
      
      /* Set expected data and size (without BOM) */
      expected_data = (pdf_char_t *)&(utf32be_strings[i].data[4]);
      expected_size = utf32be_strings[i].size -4;
      
      fail_if(pdf_text_new_from_unicode((pdf_char_t *) \
                                        utf32be_strings[i].utf32be_data,
                                        (pdf_size_t) \
                                        utf32be_strings[i].utf32be_size,
                                        PDF_TEXT_UTF32_BE,
                                         &text) != PDF_OK);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          pdf_size_t actual_size;
          pdf_char_t *actual_data;
          fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                           PDF_TEXT_UTF32_HE,0) == PDF_OK);
          internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
          fail_if(internal_hex == NULL);
          printf("pdf_text_get_unicode_031:%d:Internal> '%s'\n",
                 i, internal_hex);
          pdf_dealloc(internal_hex);
        }
      
      
      /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
      fail_unless(pdf_text_get_unicode(&data,
                                       &size,
                                       text,
                                       PDF_TEXT_UTF32_BE,
                                       PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
      
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *output_hex = NULL;
          pdf_char_t *expected_hex = NULL;
          output_hex = pdf_text_test_get_hex(data,size,':');
          expected_hex = pdf_text_test_get_hex(expected_data,expected_size,':');
          fail_if(expected_hex == NULL);
          fail_if(output_hex == NULL);
          printf("pdf_text_get_unicode_031:%d:Output> '%s'\n",
                 i, output_hex);
          printf("pdf_text_get_unicode_031:%d:Expected> '%s'\n",
                 i, expected_hex);
          pdf_dealloc(output_hex);
          pdf_dealloc(expected_hex);
        }
      
      /* 2. The returned string must be the expected one, not NUL terminated */
      fail_if(data == NULL);
      fail_unless(memcmp(expected_data, data, size) == 0);

      /* 3. The returned length must be the expected one. */
      fail_unless(size == expected_size);
      
      pdf_text_destroy(text);
      pdf_dealloc(data);      
      ++i;
    }
  
}
END_TEST


/*
 * Test: pdf_text_get_unicode_032
 * Description:
 *   Get the contents of a non-empty text object in UTF-32BE with BOM.
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must be the expected one, not NUL terminated.
 *    3. The returned length must be the expected one, including the length of
 *       the BOM in UTF-32.
 */
START_TEST(pdf_text_get_unicode_032)
{
  


  extern const test_string_t utf32be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf32be_strings[i].data != NULL)
    {
      pdf_text_t text;
      pdf_char_t *data = NULL;
      pdf_size_t size = 0;
      const pdf_char_t *expected_data;
      pdf_size_t expected_size;
      
      /* Set expected data and size (with BOM) */
      expected_data = (pdf_char_t *)&(utf32be_strings[i].data[0]);
      expected_size = utf32be_strings[i].size;
      
      fail_if(pdf_text_new_from_unicode((pdf_char_t *) \
                                        utf32be_strings[i].utf32be_data,
                                        (pdf_size_t) \
                                        utf32be_strings[i].utf32be_size,
                                        PDF_TEXT_UTF32_BE,
                                        &text) != PDF_OK);
      
      
      /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
      fail_unless(pdf_text_get_unicode(&data,
                                       &size,
                                       text,
                                       PDF_TEXT_UTF32_BE,
                                       PDF_TEXT_UNICODE_WITH_BOM) == PDF_OK);
      
      
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
          fail_if(expected_hex == NULL);
          fail_if(internal_hex == NULL);
          printf("pdf_text_get_unicode_032:%d:Internal> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_get_unicode_032:%d:Expected> '%s'\n",
                 i, expected_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
        }
      
      /* 2. The returned string must be the expected one, not NUL terminated */
      fail_if(data == NULL);
      fail_unless(memcmp(expected_data, data, size) == 0);

      /* 3. The returned length must be the expected one, including the length
       *       of the BOM in UTF-32. */
      fail_unless(size == expected_size);
      
      pdf_text_destroy(text);
      pdf_dealloc(data);
      
      ++i;
    }
  
}
END_TEST


/*
 * Test: pdf_text_get_unicode_033
 * Description:
 *   Get the contents of an empty text object in UTF-32BE without BOM.
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must be NULL.
 *    3. The returned length must be zero.
 */
START_TEST(pdf_text_get_unicode_033)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The returned string must be NULL */
  fail_unless(data == NULL);
  /* 3. The returned length must be zero. */
  fail_unless(size == 0);
  
  pdf_text_destroy(text);  
}
END_TEST


/*
 * Test: pdf_text_get_unicode_034
 * Description:
 *   Get the contents of an empty text object in UTF-32BE with BOM. 
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must only contain the BOM in UTF-32BE, not NUL
 *       terminated.
 *    3. The returned length must be equal to the length of the BOM in UTF-32BE.
 */
START_TEST(pdf_text_get_unicode_034)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_WITH_BOM) == PDF_OK);
  
  /* 2. The returned string must only contain the BOM in UTF-32BE, not NUL
   *       terminated. */
  fail_unless(data != NULL);
  fail_unless(memcmp(&data[0], "\x00\x00\xFE\xFF", 4) == 0);

  /* 3. The returned length must be equal to the length of the BOM in UTF-32BE.
   */
  fail_unless(size == 4);
  
  pdf_text_destroy(text); 
  pdf_dealloc(data);
}
END_TEST


/*
 * Test: pdf_text_get_unicode_035
 * Description:
 *   Get the contents of a non-empty text object with lang/country info, in
 *   UTF-32BE without BOM and with lang/country information embedded (which is
 *   NOT supported in UTF-32BE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_035)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  pdf_char_t *utf8data = (pdf_char_t *)"GNU's not Unix";
  pdf_size_t utf8size = strlen((char *)utf8data);
  const pdf_char_t *language = (pdf_char_t *)"en";
  const pdf_char_t *country = (pdf_char_t *)"GB";
  
  fail_if(pdf_text_new_from_unicode(utf8data,
                                    utf8size,
                                    PDF_TEXT_UTF8,
                                    &text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  fail_if(pdf_text_set_country(text, country) != PDF_OK);
  
  
  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UTF16BE_WITH_LANGCODE) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST

/*
 * Test: pdf_text_get_unicode_036
 * Description:
 *   Get the contents of a non-empty text object with language info (no country
 *   info), in UTF-32BE without BOM and with lang/country information embedded
 *   (which is NOT supported in UTF-32BE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_036)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  pdf_char_t *utf8data = (pdf_char_t *)"GNU's not Unix";
  pdf_size_t utf8size = strlen((char *)utf8data);
  const pdf_char_t *language = (pdf_char_t *)"en";
  
  fail_if(pdf_text_new_from_unicode(utf8data,
                                    utf8size,
                                    PDF_TEXT_UTF8,
                                    &text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  
  
  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UTF16BE_WITH_LANGCODE) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST


/*
 * Test: pdf_text_get_unicode_037
 * Description:
 *   Get the contents of an empty text object with lang/country info, in
 *   UTF-32BE without BOM and with lang/country information embedded (which is
 *   NOT supported in UTF-32BE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_037)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  const pdf_char_t *language = (pdf_char_t *)"en";
  const pdf_char_t *country = (pdf_char_t *)"GB";
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  fail_if(pdf_text_set_country(text, country) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UTF16BE_WITH_LANGCODE) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST

/*
 * Test: pdf_text_get_unicode_038
 * Description:
 *   Get the contents of an empty text object with language info (no country
 *   info), in UTF-32BE without BOM and with lang/country information embedded
 *   (which is NOT supported in UTF-32BE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_038)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  const pdf_char_t *language = (pdf_char_t *)"en";
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UTF16BE_WITH_LANGCODE) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST

/*
 * Test: pdf_text_get_unicode_039
 * Description:
 *   Get the contents of a non-empty text object with lang/country info, in
 *   UTF-32BE with BOM and with lang/country information embedded (which is
 *   NOT supported in UTF-32BE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_039)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  pdf_char_t *utf8data = (pdf_char_t *)"GNU's not Unix";
  pdf_size_t utf8size = strlen((char *)utf8data);
  const pdf_char_t *language = (pdf_char_t *)"en";
  const pdf_char_t *country = (pdf_char_t *)"GB";
  
  fail_if(pdf_text_new_from_unicode(utf8data,
                                    utf8size,
                                    PDF_TEXT_UTF8,
                                    &text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  fail_if(pdf_text_set_country(text, country) != PDF_OK);
  
  
  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   (PDF_TEXT_UTF16BE_WITH_LANGCODE | \
                                    PDF_TEXT_UNICODE_WITH_BOM)) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST

/*
 * Test: pdf_text_get_unicode_040
 * Description:
 *   Get the contents of a non-empty text object with language info (no country
 *   info), in UTF-32BE with BOM and with lang/country information embedded
 *   (which is NOT supported in UTF-32BE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_040)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  pdf_char_t *utf8data = (pdf_char_t *)"GNU's not Unix";
  pdf_size_t utf8size = strlen((char *)utf8data);
  const pdf_char_t *language = (pdf_char_t *)"en";
  
  fail_if(pdf_text_new_from_unicode(utf8data,
                                    utf8size,
                                    PDF_TEXT_UTF8,
                                    &text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);

  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   (PDF_TEXT_UTF16BE_WITH_LANGCODE | \
                                    PDF_TEXT_UNICODE_WITH_BOM)) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST


/*
 * Test: pdf_text_get_unicode_041
 * Description:
 *   Get the contents of an empty text object with lang/country info, in
 *   UTF-32BE with BOM and with lang/country information embedded (which is
 *   NOT supported in UTF-32BE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_041)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  const pdf_char_t *language = (pdf_char_t *)"en";
  const pdf_char_t *country = (pdf_char_t *)"GB";
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  fail_if(pdf_text_set_country(text, country) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   (PDF_TEXT_UTF16BE_WITH_LANGCODE | \
                                    PDF_TEXT_UNICODE_WITH_BOM)) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST

/*
 * Test: pdf_text_get_unicode_042
 * Description:
 *   Get the contents of an empty text object with language info (no country
 *   info), in UTF-32BE with BOM and with lang/country information embedded
 *   (which is NOT supported in UTF-32BE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_042)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  const pdf_char_t *language = (pdf_char_t *)"en";
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   (PDF_TEXT_UTF16BE_WITH_LANGCODE | \
                                    PDF_TEXT_UNICODE_WITH_BOM)) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST


/*
 * Test: pdf_text_get_unicode_043
 * Description:
 *   Get the contents of a non-empty text object in UTF-32LE without BOM.
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must be the expected one, not NUL terminated.
 *    3. The returned length must be the expected one.
 */
START_TEST(pdf_text_get_unicode_043)
{
  extern const test_string_t utf32be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf32be_strings[i].data != NULL)
    {
      pdf_text_t text;
      pdf_char_t *data = NULL;
      pdf_size_t size = 0;
      pdf_char_t *expected_data;
      pdf_size_t expected_size;
      
      /* Set expected data and size (without BOM) */
      expected_data = (pdf_char_t *)&(utf32be_strings[i].data[4]);
      expected_size = utf32be_strings[i].size -4;
      expected_data = pdf_text_test_change_utf32_endianness(expected_data,
                                                            expected_size);
      /* Just in case... */
      fail_if(expected_data == NULL);
      
      fail_if(pdf_text_new_from_unicode((pdf_char_t *) \
                                        utf32be_strings[i].utf32be_data,
                                        (pdf_size_t) \
                                        utf32be_strings[i].utf32be_size,
                                        PDF_TEXT_UTF32_BE,
                                        &text) != PDF_OK);
      
      
      /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
      fail_unless(pdf_text_get_unicode(&data,
                                       &size,
                                       text,
                                       PDF_TEXT_UTF32_LE,
                                       PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
      
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *output_hex = NULL;
          pdf_char_t *expected_hex = NULL;
          output_hex = pdf_text_test_get_hex(data,size,':');
          expected_hex = pdf_text_test_get_hex(expected_data,expected_size,':');
          fail_if(expected_hex == NULL);
          fail_if(output_hex == NULL);
          printf("pdf_text_get_unicode_043:%d:Output> '%s'\n",
                 i, output_hex);
          printf("pdf_text_get_unicode_043:%d:Expected> '%s'\n",
                 i, expected_hex);
          pdf_dealloc(output_hex);
          pdf_dealloc(expected_hex);
        }
      
      /* 2. The returned string must be the expected one, not NUL terminated */
      fail_if(data == NULL);
      fail_unless(memcmp(expected_data, data, size) == 0);

      /* 3. The returned length must be the expected one. */
      fail_unless(size == expected_size);
      
      pdf_text_destroy(text);
      pdf_dealloc(data);
      pdf_dealloc(expected_data);
      ++i;
    }
  
}
END_TEST


/*
 * Test: pdf_text_get_unicode_044
 * Description:
 *   Get the contents of a non-empty text object in UTF-32LE with BOM.
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must be the expected one, not NUL terminated.
 *    3. The returned length must be the expected one, including the length of
 *       the BOM in UTF-32.
 */
START_TEST(pdf_text_get_unicode_044)
{
  


  extern const test_string_t utf32be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf32be_strings[i].data != NULL)
    {
      pdf_text_t text;
      pdf_char_t *data = NULL;
      pdf_size_t size = 0;
      pdf_char_t *expected_data;
      pdf_size_t expected_size;
      
      /* Set expected data and size (with BOM) */
      expected_data = (pdf_char_t *)&(utf32be_strings[i].data[0]);
      expected_size = utf32be_strings[i].size;
      expected_data = pdf_text_test_change_utf32_endianness(expected_data,
                                                            expected_size);
      /* Just in case... */
      fail_if(expected_data == NULL);
      
      fail_if(pdf_text_new_from_unicode((pdf_char_t *) \
                                        utf32be_strings[i].utf32be_data,
                                        (pdf_size_t) \
                                        utf32be_strings[i].utf32be_size,
                                        PDF_TEXT_UTF32_BE,
                                        &text) != PDF_OK);
      
      
      /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
      fail_unless(pdf_text_get_unicode(&data,
                                       &size,
                                       text,
                                       PDF_TEXT_UTF32_LE,
                                       PDF_TEXT_UNICODE_WITH_BOM) == PDF_OK);
      
      
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
          fail_if(expected_hex == NULL);
          fail_if(internal_hex == NULL);
          printf("pdf_text_get_unicode_044:%d:Internal> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_get_unicode_044:%d:Expected> '%s'\n",
                 i, expected_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
        }
      
      /* 2. The returned string must be the expected one, not NUL terminated */
      fail_if(data == NULL);
      fail_unless(memcmp(expected_data, data, size) == 0);

      /* 3. The returned length must be the expected one, including the length of
       *       the BOM in UTF-32. */
      fail_unless(size == expected_size);
      
      pdf_text_destroy(text);
      pdf_dealloc(data);
      pdf_dealloc(expected_data);
      
      ++i;
    }
  
}
END_TEST


/*
 * Test: pdf_text_get_unicode_045
 * Description:
 *   Get the contents of an empty text object in UTF-32LE without BOM.
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must be NULL.
 *    3. The returned length must be zero.
 */
START_TEST(pdf_text_get_unicode_045)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF32_LE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);
  
  /* 2. The returned string must be NULL */
  fail_unless(data == NULL);
  /* 3. The returned length must be zero. */
  fail_unless(size == 0);
  
  pdf_text_destroy(text);  
}
END_TEST


/*
 * Test: pdf_text_get_unicode_046
 * Description:
 *   Get the contents of an empty text object in UTF-32LE with BOM. 
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must only contain the BOM in UTF-32LE, not NUL
 *       terminated.
 *    3. The returned length must be equal to the length of the BOM in UTF-32LE.
 */
START_TEST(pdf_text_get_unicode_046)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF32_LE,
                                   PDF_TEXT_UNICODE_WITH_BOM) == PDF_OK);
  
  /* 2. The returned string must only contain the BOM in UTF-32LE, not NUL
   *       terminated. */
  fail_unless(data != NULL);
  fail_unless(memcmp(&data[0], "\xFF\xFE\x00\x00", 4) == 0);
  
  /* 3. The returned length must be equal to the length of the BOM in UTF-32LE.
   */
  fail_unless(size == 4);
  
  pdf_text_destroy(text); 
  pdf_dealloc(data);
}
END_TEST


/*
 * Test: pdf_text_get_unicode_047
 * Description:
 *   Get the contents of a non-empty text object with lang/country info, in
 *   UTF-32LE without BOM and with lang/country information embedded (which is
 *   NOT supported in UTF-32LE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_047)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  pdf_char_t *utf8data = (pdf_char_t *)"GNU's not Unix";
  pdf_size_t utf8size = strlen((char *)utf8data);
  const pdf_char_t *language = (pdf_char_t *)"en";
  const pdf_char_t *country = (pdf_char_t *)"GB";
  
  fail_if(pdf_text_new_from_unicode(utf8data,
                                    utf8size,
                                    PDF_TEXT_UTF8,
                                    &text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  fail_if(pdf_text_set_country(text, country) != PDF_OK);
  
  
  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF32_LE,
                                   PDF_TEXT_UTF16BE_WITH_LANGCODE) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST

/*
 * Test: pdf_text_get_unicode_048
 * Description:
 *   Get the contents of a non-empty text object with language info (no country
 *   info), in UTF-32LE without BOM and with lang/country information embedded
 *   (which is NOT supported in UTF-32LE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_048)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  pdf_char_t *utf8data = (pdf_char_t *)"GNU's not Unix";
  pdf_size_t utf8size = strlen((char *)utf8data);
  const pdf_char_t *language = (pdf_char_t *)"en";
  
  fail_if(pdf_text_new_from_unicode(utf8data,
                                    utf8size,
                                    PDF_TEXT_UTF8,
                                    &text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  
  
  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF32_LE,
                                   PDF_TEXT_UTF16BE_WITH_LANGCODE) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST


/*
 * Test: pdf_text_get_unicode_049
 * Description:
 *   Get the contents of an empty text object with lang/country info, in
 *   UTF-32LE without BOM and with lang/country information embedded (which is
 *   NOT supported in UTF-32LE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_049)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  const pdf_char_t *language = (pdf_char_t *)"en";
  const pdf_char_t *country = (pdf_char_t *)"GB";
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  fail_if(pdf_text_set_country(text, country) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF32_LE,
                                   PDF_TEXT_UTF16BE_WITH_LANGCODE) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST

/*
 * Test: pdf_text_get_unicode_050
 * Description:
 *   Get the contents of an empty text object with language info (no country
 *   info), in UTF-32LE without BOM and with lang/country information embedded
 *   (which is NOT supported in UTF-32LE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_050)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  const pdf_char_t *language = (pdf_char_t *)"en";
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF32_LE,
                                   PDF_TEXT_UTF16BE_WITH_LANGCODE) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST

/*
 * Test: pdf_text_get_unicode_051
 * Description:
 *   Get the contents of a non-empty text object with lang/country info, in
 *   UTF-32LE with BOM and with lang/country information embedded (which is
 *   NOT supported in UTF-32LE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_051)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  pdf_char_t *utf8data = (pdf_char_t *)"GNU's not Unix";
  pdf_size_t utf8size = strlen((char *)utf8data);
  const pdf_char_t *language = (pdf_char_t *)"en";
  const pdf_char_t *country = (pdf_char_t *)"GB";
  
  fail_if(pdf_text_new_from_unicode(utf8data,
                                    utf8size,
                                    PDF_TEXT_UTF8,
                                    &text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  fail_if(pdf_text_set_country(text, country) != PDF_OK);
  
  
  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF32_LE,
                                   (PDF_TEXT_UTF16BE_WITH_LANGCODE | \
                                    PDF_TEXT_UNICODE_WITH_BOM)) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST

/*
 * Test: pdf_text_get_unicode_052
 * Description:
 *   Get the contents of a non-empty text object with language info (no country
 *   info), in UTF-32LE with BOM and with lang/country information embedded
 *   (which is NOT supported in UTF-32LE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_052)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  pdf_char_t *utf8data = (pdf_char_t *)"GNU's not Unix";
  pdf_size_t utf8size = strlen((char *)utf8data);
  const pdf_char_t *language = (pdf_char_t *)"en";
  
  fail_if(pdf_text_new_from_unicode(utf8data,
                                    utf8size,
                                    PDF_TEXT_UTF8,
                                    &text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF32_LE,
                                   (PDF_TEXT_UTF16BE_WITH_LANGCODE | \
                                    PDF_TEXT_UNICODE_WITH_BOM)) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST


/*
 * Test: pdf_text_get_unicode_053
 * Description:
 *   Get the contents of an empty text object with lang/country info, in
 *   UTF-32LE with BOM and with lang/country information embedded (which is
 *   NOT supported in UTF-32LE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_053)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  const pdf_char_t *language = (pdf_char_t *)"en";
  const pdf_char_t *country = (pdf_char_t *)"GB";
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  fail_if(pdf_text_set_country(text, country) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF32_LE,
                                   (PDF_TEXT_UTF16BE_WITH_LANGCODE | \
                                    PDF_TEXT_UNICODE_WITH_BOM)) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST

/*
 * Test: pdf_text_get_unicode_054
 * Description:
 *   Get the contents of an empty text object with language info (no country
 *   info), in UTF-32LE with BOM and with lang/country information embedded
 *   (which is NOT supported in UTF-32LE).
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_054)
{  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  const pdf_char_t *language = (pdf_char_t *)"en";
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF32_LE,
                                   (PDF_TEXT_UTF16BE_WITH_LANGCODE | \
                                    PDF_TEXT_UNICODE_WITH_BOM)) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST




/*
 * Test: pdf_text_get_unicode_055
 * Description:
 *   Get the contents of a non-empty text object in UTF-8 without BOM and with
 *   NUL suffix. The contents of the text object include characters that are
 *   encoded in UTF-8 using 8-bit, 16-bit, 24-bit and 32-bit. 
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must be the expected one, NUL terminated.
 *    3. The returned length must be the expected one, including the length of
 *        the last NUL.
 */
START_TEST(pdf_text_get_unicode_055)
{
  


  extern const test_string_t utf8_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf8_strings[i].data != NULL)
    {
      pdf_text_t text;
      pdf_char_t *data = NULL;
      pdf_size_t size = 0;
      pdf_char_t *expected_data;
      pdf_size_t expected_size;
      
      /* Set expected data and size (without BOM, with last NUL) */
      expected_size = 1 + utf8_strings[i].size -3;
      expected_data = pdf_alloc(expected_size);
      fail_if(expected_data == NULL);
      memcpy(expected_data, &(utf8_strings[i].data[3]), utf8_strings[i].size - 3);
      expected_data[expected_size-1] = '\0';      
      
      fail_if(pdf_text_new_from_unicode((pdf_char_t *) \
                                        utf8_strings[i].utf32be_data,
                                        (pdf_size_t) \
                                        utf8_strings[i].utf32be_size,
                                        PDF_TEXT_UTF32_BE,
                                        &text) != PDF_OK);
      
      
      /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
      fail_unless(pdf_text_get_unicode(&data,
                                       &size,
                                       text,
                                       PDF_TEXT_UTF8,
                                       PDF_TEXT_UNICODE_WITH_NUL_SUFFIX) == PDF_OK);
      
      
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
          fail_if(expected_hex == NULL);
          fail_if(internal_hex == NULL);
          printf("pdf_text_get_unicode_055:%d:Internal> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_get_unicode_055:%d:Expected> '%s'\n",
                 i, expected_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
        }
      
      /* 2. The returned string must be the expected one, NUL terminated */
      fail_if(data == NULL);
      fail_unless(memcmp(expected_data, data, size) == 0);
      fail_unless(data[size-1] == '\0');

      /* 3. The returned length must be the expected one. */
      fail_unless(size == expected_size);

      pdf_text_destroy(text);
      pdf_dealloc(data);
      pdf_dealloc(expected_data);
      
      ++i;
    }
  
}
END_TEST







/*
 * Test: pdf_text_get_unicode_056
 * Description:
 *   Get the contents of a non-empty text object in UTF-8 with BOM and with NUL
 *   suffix. The contents of the text object include characters that are encoded
 *   in UTF-8 using 8-bit, 16-bit, 24-bit and 32-bit.
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must be the expected one, NUL terminated.
 *    3. The returned length must be the expected one, including the length of
 *       the BOM in UTF-8 and the length of the NUL suffix.
 */
START_TEST(pdf_text_get_unicode_056)
{
  


  extern const test_string_t utf8_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf8_strings[i].data != NULL)
    {
      pdf_text_t text;
      pdf_char_t *data = NULL;
      pdf_size_t size = 0;
      pdf_char_t *expected_data;
      pdf_size_t expected_size;
      
      /* Set expected data and size (with BOM) */
      expected_size = 1 + utf8_strings[i].size;
      expected_data = pdf_alloc(expected_size);
      fail_if(expected_data == NULL);
      memcpy(expected_data, &(utf8_strings[i].data[0]), utf8_strings[i].size);
      expected_data[expected_size-1] = '\0';

      
      fail_if(pdf_text_new_from_unicode((pdf_char_t *) \
                                        utf8_strings[i].utf32be_data,
                                        (pdf_size_t) \
                                        utf8_strings[i].utf32be_size,
                                        PDF_TEXT_UTF32_BE,
                                        &text) != PDF_OK);
      
      
      /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
      fail_unless(pdf_text_get_unicode(&data,
                                       &size,
                                       text,
                                       PDF_TEXT_UTF8,
                                       (PDF_TEXT_UNICODE_WITH_BOM | \
                                        PDF_TEXT_UNICODE_WITH_NUL_SUFFIX)) == PDF_OK);
      
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
          pdf_char_t *expected_hex = NULL;
          pdf_char_t *output_hex = NULL;
          pdf_size_t actual_size;
          pdf_char_t *actual_data;
          fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                           PDF_TEXT_UTF32_HE,0) == PDF_OK);
          internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
          expected_hex = pdf_text_test_get_hex(expected_data,expected_size,':');
          output_hex = pdf_text_test_get_hex(data,size,':');
          fail_if(expected_hex == NULL);
          fail_if(internal_hex == NULL);
          fail_if(output_hex == NULL);
          printf("pdf_text_get_unicode_056:%d:Internal> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_get_unicode_056:%d:Expected> '%s'\n",
                 i, expected_hex);
          printf("pdf_text_get_unicode_056:%d:Output> '%s'\n",
                 i, output_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
          pdf_dealloc(output_hex);
        }
      
      /* 2. The returned string must be the expected one, not NUL terminated */
      fail_if(data == NULL);
      fail_unless(memcmp(expected_data, data, size) == 0);

      /* 3. The returned length must be the expected one, including the length
       *       of the BOM in UTF-8. */
      fail_unless(size == expected_size);
      
      pdf_text_destroy(text);
      pdf_dealloc(data);
      pdf_dealloc(expected_data);
      
      ++i;
    }
  
}
END_TEST


/*
 * Test: pdf_text_get_unicode_057
 * Description:
 *   Get the contents of an empty text object in UTF-8 without BOM and with NUL
 *    suffix.
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must not be NULL.
 *    3. The returned length must be equal to the length of the last NUL.
 */
START_TEST(pdf_text_get_unicode_057)
{
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF8,
                                   PDF_TEXT_UNICODE_WITH_NUL_SUFFIX) == PDF_OK);
  
  /* 2. The returned string must not be NULL */
  fail_unless(data != NULL);
  fail_unless(memcmp(data, "\x00", 1) == 0);
  /* 3. The returned length must be equal to the last NUL. */
  fail_unless(size == 1);
  
  pdf_text_destroy(text);
}
END_TEST


/*
 * Test: pdf_text_get_unicode_058
 * Description:
 *   Get the contents of an empty text object in UTF-8 with BOM and with NUL
 *    suffix.
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must only contain the BOM in UTF-8, NUL
 *       terminated.
 *    3. The returned length must be equal to the length of the BOM in UTF-8
 *        plus the length of the last NUL.
 */
START_TEST(pdf_text_get_unicode_058)
{
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  
  fail_if(pdf_text_new (&text) != PDF_OK);
  
  /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF8,
                                   (PDF_TEXT_UNICODE_WITH_BOM | \
                                    PDF_TEXT_UNICODE_WITH_NUL_SUFFIX)) == PDF_OK);
  
  /* 2. The returned string must only contain the BOM in UTF-8, NUL
   *       terminated. */
  fail_unless(data != NULL);
  fail_unless(memcmp(&data[0], "\xEF\xBB\xBF\x00", 4) == 0);

  /* 3. The returned length must be equal to the length of the BOM in UTF-8 plus
   *      the length of the last NUL. */
  fail_unless(size == 4);
  
  pdf_text_destroy(text);
  pdf_dealloc(data);
}
END_TEST


/*
 * Test: pdf_text_get_unicode_059
 * Description:
 *   Get the contents of a non-empty text object with lang/country info, in
 *   UTF-8 without BOM, with lang/country information embedded (which should
 *   not be supported in UTF-8) and with NUL suffix.
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_059)
{
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  pdf_char_t *utf8data = (pdf_char_t *)"GNU's not Unix";
  pdf_size_t utf8size = strlen((char *)utf8data);
  const pdf_char_t *language = (pdf_char_t *)"en";
  const pdf_char_t *country = (pdf_char_t *)"GB";
  
  fail_if(pdf_text_new_from_unicode(utf8data, utf8size,
                                    PDF_TEXT_UTF8,
                                    &text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  fail_if(pdf_text_set_country(text, country) != PDF_OK);
  

  
  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF8,
                                   (PDF_TEXT_UTF16BE_WITH_LANGCODE | \
                                    PDF_TEXT_UNICODE_WITH_NUL_SUFFIX)) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST


/*
 * Test: pdf_text_get_unicode_060
 * Description:
 *   Get the contents of a non-empty text object with lang/country info, in
 *   UTF-8 with BOM, with lang/country information embedded (which should
 *   not be supported in UTF-8) and with NUL suffix.
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should NOT return PDF_OK.
 */
START_TEST(pdf_text_get_unicode_060)
{
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  pdf_text_t text;
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;
  pdf_char_t *utf8data = (pdf_char_t *)"GNU's not Unix";
  pdf_size_t utf8size = strlen((char *)utf8data);
  const pdf_char_t *language = (pdf_char_t *)"en";
  const pdf_char_t *country = (pdf_char_t *)"GB";
  
  fail_if(pdf_text_new_from_unicode(utf8data, utf8size,
                                    PDF_TEXT_UTF8,
                                    &text) != PDF_OK);
  fail_if(pdf_text_set_language(text, language) != PDF_OK);
  fail_if(pdf_text_set_country(text, country) != PDF_OK);

  /* 1. The call to  pdf_text_get_unicode should NOT return PDF_OK. */
  fail_unless(pdf_text_get_unicode(&data,
                                   &size,
                                   text,
                                   PDF_TEXT_UTF8,
                                   (PDF_TEXT_UNICODE_WITH_BOM | \
                                    PDF_TEXT_UTF16BE_WITH_LANGCODE | \
                                    PDF_TEXT_UNICODE_WITH_NUL_SUFFIX)) != PDF_OK);
  pdf_text_destroy(text);
}
END_TEST


/*
 * Test: pdf_text_get_unicode_061
 * Description:
 *   Get the contents of a non-empty text object in UTF-16BE without BOM and
 *   with NUL suffix. The contents of the text object include characters that
 *   are encoded in UTF-16 using 16-bit and 32-bit.
 * Success conditions:
 *    1. The call to  pdf_text_get_unicode should return PDF_OK.
 *    2. The returned string must be the expected one, NUL terminated.
 *    3. The returned length must be the expected one, including the length of
 *        the last NUL.
 */
START_TEST(pdf_text_get_unicode_061)
{
  


  extern const test_string_t utf16be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf16be_strings[i].data != NULL)
    {
      pdf_text_t text;
      pdf_char_t *data = NULL;
      pdf_size_t size = 0;
      pdf_char_t *expected_data;
      pdf_size_t expected_size;
      
      /* Set expected data and size (without BOM) and with last NUL */
      expected_size = utf16be_strings[i].size -2 + 2;
      expected_data = pdf_alloc(expected_size);
      fail_if(expected_data == NULL);
      memcpy(expected_data, &(utf16be_strings[i].data[2]), utf16be_strings[i].size -2);
      memset(&expected_data[expected_size-2], 0, 2);

      
      fail_if(pdf_text_new_from_unicode((pdf_char_t *) \
                                        utf16be_strings[i].utf32be_data,
                                        (pdf_size_t) \
                                        utf16be_strings[i].utf32be_size,
                                        PDF_TEXT_UTF32_BE,
                                        &text) != PDF_OK);
      
      
      /* 1. The call to  pdf_text_get_unicode should return PDF_OK. */
      fail_unless(pdf_text_get_unicode(&data,
                                       &size,
                                       text,
                                       PDF_TEXT_UTF16_BE,
                                       PDF_TEXT_UNICODE_WITH_NUL_SUFFIX) == PDF_OK);
      
      
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
          fail_if(expected_hex == NULL);
          fail_if(internal_hex == NULL);
          printf("pdf_text_get_unicode_061:%d:Internal> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_get_unicode_061:%d:Expected> '%s'\n",
                 i, expected_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
        }
      
      /* 2. The returned string must be the expected one, not NUL terminated */
      fail_if(data == NULL);
      fail_unless(memcmp(expected_data, data, size) == 0);

      /* 3. The returned length must be the expected one. */
      fail_unless(size == expected_size);
      
      pdf_text_destroy(text);
      pdf_dealloc(data);
      pdf_dealloc(expected_data);
      
      ++i;
    }
  
}
END_TEST


/*

CONTINUE HERE WITH TESTS FROM #62 to #108.

Check:
http://gnupdf.org/Lib:Test_Specification_Document#pdf_text_get_unicode

And the corresponding Flyspray task.

Tests from #55 to #108 are EQUIVALENT to tests from #1 to #54, with the only
difference that the new PDF_TEXT_UNICODE_WITH_NUL_SUFFIX option is used. 

Check the pass/fail criteria from the TSD for each test.

The next test to be done is the #62, which is equivalent to #8.

Also, take into account that last NUL has different sizes depending the
encoding:
  - 1 NUL byte for UTF-8
  - 2 NUL bytes for all the UTF-16 encodings 
  - 4 NUL bytes for all the UTF-32 encodings 

*/




/*
 * Test case creation function
 */
TCase *
test_pdf_text_get_unicode (void)
{
  TCase *tc = tcase_create("pdf_text_get_unicode");
  
  tcase_add_test(tc, pdf_text_get_unicode_001);
  tcase_add_test(tc, pdf_text_get_unicode_002);
  tcase_add_test(tc, pdf_text_get_unicode_003);
  tcase_add_test(tc, pdf_text_get_unicode_004);
  tcase_add_test(tc, pdf_text_get_unicode_005);
  tcase_add_test(tc, pdf_text_get_unicode_006);
  tcase_add_test(tc, pdf_text_get_unicode_007);
  tcase_add_test(tc, pdf_text_get_unicode_008);
  tcase_add_test(tc, pdf_text_get_unicode_009);
  tcase_add_test(tc, pdf_text_get_unicode_010);
  tcase_add_test(tc, pdf_text_get_unicode_011);
  tcase_add_test(tc, pdf_text_get_unicode_012);
  tcase_add_test(tc, pdf_text_get_unicode_013);
  tcase_add_test(tc, pdf_text_get_unicode_014);
  tcase_add_test(tc, pdf_text_get_unicode_015);
  tcase_add_test(tc, pdf_text_get_unicode_016);
  tcase_add_test(tc, pdf_text_get_unicode_017);
  tcase_add_test(tc, pdf_text_get_unicode_018);
  tcase_add_test(tc, pdf_text_get_unicode_019);
  tcase_add_test(tc, pdf_text_get_unicode_020);
  tcase_add_test(tc, pdf_text_get_unicode_021);
  tcase_add_test(tc, pdf_text_get_unicode_022);
  tcase_add_test(tc, pdf_text_get_unicode_023);
  tcase_add_test(tc, pdf_text_get_unicode_024);
  tcase_add_test(tc, pdf_text_get_unicode_025);
  tcase_add_test(tc, pdf_text_get_unicode_026);
  tcase_add_test(tc, pdf_text_get_unicode_027);
  tcase_add_test(tc, pdf_text_get_unicode_028);
  tcase_add_test(tc, pdf_text_get_unicode_029);
  tcase_add_test(tc, pdf_text_get_unicode_030);
  tcase_add_test(tc, pdf_text_get_unicode_031);
  tcase_add_test(tc, pdf_text_get_unicode_032);
  tcase_add_test(tc, pdf_text_get_unicode_033);
  tcase_add_test(tc, pdf_text_get_unicode_034);
  tcase_add_test(tc, pdf_text_get_unicode_035);
  tcase_add_test(tc, pdf_text_get_unicode_036);
  tcase_add_test(tc, pdf_text_get_unicode_037);
  tcase_add_test(tc, pdf_text_get_unicode_038);
  tcase_add_test(tc, pdf_text_get_unicode_039);
  tcase_add_test(tc, pdf_text_get_unicode_040);
  tcase_add_test(tc, pdf_text_get_unicode_041);
  tcase_add_test(tc, pdf_text_get_unicode_042);
  tcase_add_test(tc, pdf_text_get_unicode_043);
  tcase_add_test(tc, pdf_text_get_unicode_044);
  tcase_add_test(tc, pdf_text_get_unicode_045);
  tcase_add_test(tc, pdf_text_get_unicode_046);
  tcase_add_test(tc, pdf_text_get_unicode_047);
  tcase_add_test(tc, pdf_text_get_unicode_048);
  tcase_add_test(tc, pdf_text_get_unicode_049);
  tcase_add_test(tc, pdf_text_get_unicode_050);
  tcase_add_test(tc, pdf_text_get_unicode_051);
  tcase_add_test(tc, pdf_text_get_unicode_052);
  tcase_add_test(tc, pdf_text_get_unicode_053);
  tcase_add_test(tc, pdf_text_get_unicode_054);

  tcase_add_test(tc, pdf_text_get_unicode_055);
  tcase_add_test(tc, pdf_text_get_unicode_056);
  tcase_add_test(tc, pdf_text_get_unicode_057);
  tcase_add_test(tc, pdf_text_get_unicode_058);
  tcase_add_test(tc, pdf_text_get_unicode_059);
  tcase_add_test(tc, pdf_text_get_unicode_060);
  tcase_add_test(tc, pdf_text_get_unicode_061);

  
  
  return tc;
}

/* End of pdf-text-get-unicode.c */

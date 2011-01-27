/* -*- mode: C -*-
 *
 *       File:         pdf-text-set-unicode.c
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_set_unicode test case
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
 * Test: pdf_text_set_unicode_001
 * Description:
 *   Set the contents of a text object with an input valid UTF-8 encoded string
 *   (containing 8-bit, 16-bit, 24-bit and 32-bit code points!) without BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 */
START_TEST(pdf_text_set_unicode_001)
{
  


  extern const test_string_t utf8_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf8_strings[i].data != NULL)
    {
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data = NULL;
      const pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_size_t expected_size = 0;
      pdf_text_t text = NULL;
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set input data */
      input_data = (pdf_char_t *)&(utf8_strings[i].data[3]);
      input_size = (pdf_size_t)utf8_strings[i].size -3;
      
      /* Set expected data */
      expected_data = (pdf_char_t *)utf8_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf8_strings[i].utf32be_size;
      if(!pdf_text_test_big_endian_system())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness(expected_data,
                                                                expected_size);
          /* Just in case... */
          fail_if(expected_data == NULL);
        }
      
      /* 1. The call to pdf_text_set_unicode should return PDF_OK. */
      fail_if(pdf_text_set_unicode(text,
                                   input_data,
                                   input_size,
                                   PDF_TEXT_UTF8) != PDF_OK);
      
      /* 2. The contents of the text object must be the expected ones
       *    (without BOM). */

      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size);
      fail_unless(memcmp(actual_data, expected_data, expected_size)==0);
      
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
 * Test: pdf_text_set_unicode_002
 * Description:
 *   Set the contents of a text object with an input valid UTF-16BE encoded 
 *    string (16-bit and 32-bit code points!) without BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 */
START_TEST(pdf_text_set_unicode_002)
{
  


  extern const test_string_t utf16be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf16be_strings[i].data != NULL)
    {
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data = NULL;
      const pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_size_t expected_size = 0;
      pdf_text_t text = NULL;
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set input data */
      input_data = (pdf_char_t *)&(utf16be_strings[i].data[2]);
      input_size = (pdf_size_t)utf16be_strings[i].size -2;
      
      /* Set expected data */
      expected_data = (pdf_char_t *)utf16be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf16be_strings[i].utf32be_size;
      if(!pdf_text_test_big_endian_system())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness(expected_data,
                                                                expected_size);
          /* Just in case... */
          fail_if(expected_data == NULL);
        }
      
      /* 1. The call to pdf_text_new_from_unicode should return PDF_OK. */
      fail_if(pdf_text_set_unicode(text,
                                   input_data,
                                   input_size,
                                   PDF_TEXT_UTF16_BE) != PDF_OK);
      
      /* 2. The contents of the text object must be the expected ones. */

      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size);
      fail_unless(memcmp(actual_data, expected_data, expected_size)==0);
      
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
 * Test: pdf_text_set_unicode_003
 * Description:
 *   Set the contents of a text object with an input valid UTF-16LE encoded 
 *    string (16-bit and 32-bit code points!) without BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 */
START_TEST(pdf_text_set_unicode_003)
{
  


  extern const test_string_t utf16be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf16be_strings[i].data != NULL)
    {
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_size_t expected_size = 0;
      pdf_text_t text = NULL;
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set input data (change endiannes to input UTF-16BE string) */
      input_size = (pdf_size_t)utf16be_strings[i].size -2;
      input_data=pdf_text_test_change_utf16_endianness((pdf_char_t *) \
                                                       &utf16be_strings[i].data[2],
                                                       input_size);
      
      /* Set expected data */
      expected_data = (pdf_char_t *)utf16be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf16be_strings[i].utf32be_size;
      if(!pdf_text_test_big_endian_system())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness(expected_data,
                                                                expected_size);
          /* Just in case... */
          fail_if(expected_data == NULL);
        }
      
      /* 1. The call to pdf_text_set_unicode should return PDF_OK. */
      fail_if(pdf_text_set_unicode(text,
                                   input_data,
                                   input_size,
                                   PDF_TEXT_UTF16_LE) != PDF_OK);
            
      /* 2. The contents of the text object must be the expected ones. */

      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size);
      fail_unless(memcmp(actual_data, expected_data, expected_size)==0);
      
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
 * Test: pdf_text_set_unicode_004
 * Description:
 *   Set the contents of a text object with an input valid UTF-16HE encoded 
 *    string (16-bit and 32-bit code points!) without BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 */
START_TEST(pdf_text_set_unicode_004)
{
  


  extern const test_string_t utf16be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf16be_strings[i].data != NULL)
    {
      pdf_bool_t input_free = PDF_FALSE;
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_size_t expected_size = 0;
      pdf_text_t text = NULL;
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set input data (change endiannes to input UTF-16BE string if required)
       */
      input_size = (pdf_size_t)utf16be_strings[i].size -2;
      if(!pdf_text_test_big_endian_system())
        {
          input_free = PDF_TRUE;
          input_data = pdf_text_test_change_utf16_endianness((pdf_char_t *) \
                                                             &utf16be_strings[i].data[2],
                                                             input_size);
          /* Just in case... */
          fail_if(input_data == NULL);
        }
      else
        {
          input_data = (pdf_char_t *)&utf16be_strings[i].data[2];
        }
      
      /* Set expected data */
      expected_data = (pdf_char_t *)utf16be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf16be_strings[i].utf32be_size;
      if(!pdf_text_test_big_endian_system())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness(expected_data,
                                                                expected_size);
          /* Just in case... */
          fail_if(expected_data == NULL);
        }
      
      /* 1. The call to pdf_text_set_unicode should return PDF_OK. */
      fail_if(pdf_text_set_unicode(text,
                                   input_data,
                                   input_size,
                                   PDF_TEXT_UTF16_HE) != PDF_OK);
      
      /* 2. The contents of the text object must be the expected ones. */

      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size);
      fail_unless(memcmp(actual_data, expected_data, expected_size)==0);
      
      fail_if(pdf_text_destroy(text) != PDF_OK);
      
      if(expected_free)
        {
          pdf_dealloc(expected_data);
        }
      if(input_free)
        {
          pdf_dealloc(input_data);
        }
      ++i;
    }  

}
END_TEST


/*
 * Test: pdf_text_set_unicode_005
 * Description:
 *   Set the contents of a text object with an input valid UTF-32BE encoded 
 *    string without BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 */
START_TEST(pdf_text_set_unicode_005)
{
  


  extern const test_string_t utf32be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf32be_strings[i].data != NULL)
    {
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data = NULL;
      const pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_size_t expected_size = 0;
      pdf_text_t text = NULL;
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set input data */
      input_size = (pdf_size_t)utf32be_strings[i].size -4;
      input_data = (pdf_char_t *)&utf32be_strings[i].data[4];
      
      /* Set expected data */
      expected_data = (pdf_char_t *)utf32be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf32be_strings[i].utf32be_size;
      if(!pdf_text_test_big_endian_system())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness(expected_data,
                                                                expected_size);
          /* Just in case... */
          fail_if(expected_data == NULL);
        }
      
      /* 1. The call to pdf_text_set_unicode should return PDF_OK. */
      fail_if(pdf_text_set_unicode(text,
                                   input_data,
                                   input_size,
                                   PDF_TEXT_UTF32_BE) != PDF_OK);
      
      /* 2. The contents of the text object must be the expected ones. */

      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size);
      fail_unless(memcmp(actual_data, expected_data, expected_size)==0);
      
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
 * Test: pdf_text_set_unicode_006
 * Description:
 *   Set the contents of a text object with an input valid UTF-32LE encoded 
 *    string without BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 */
START_TEST(pdf_text_set_unicode_006)
{
  


  extern const test_string_t utf32be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf32be_strings[i].data != NULL)
    {
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_size_t expected_size = 0;
      pdf_text_t text = NULL;
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set input data */
      input_size = (pdf_size_t)utf32be_strings[i].size -4;
      input_data = pdf_text_test_change_utf32_endianness((pdf_char_t *) \
                                                         &utf32be_strings[i].data[4],
                                                         input_size);
      
      /* Set expected data */
      expected_data = (pdf_char_t *)utf32be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf32be_strings[i].utf32be_size;
      if(!pdf_text_test_big_endian_system())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness(expected_data,
                                                                expected_size);
          /* Just in case... */
          fail_if(expected_data == NULL);
        }
      
      /* 1. The call to pdf_text_set_unicode should return PDF_OK. */
      fail_if(pdf_text_set_unicode(text,
                                   input_data,
                                   input_size,
                                   PDF_TEXT_UTF32_LE) != PDF_OK);
      
      /* 2. The contents of the text object must be the expected ones. */

      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size);
      fail_unless(memcmp(actual_data, expected_data, expected_size)==0);

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
 * Test: pdf_text_set_unicode_007
 * Description:
 *   Set the contents of a text object with an input valid UTF-32HE encoded 
 *    string without BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 */
START_TEST(pdf_text_set_unicode_007)
{
  


  extern const test_string_t utf32be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf32be_strings[i].data != NULL)
    {
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_bool_t input_free = PDF_FALSE;
      pdf_char_t *expected_data = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_size_t expected_size = 0;
      pdf_text_t text = NULL;
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set input data (UTF-32HE) */
      input_size = (pdf_size_t)utf32be_strings[i].size -4;
      if(!pdf_text_test_big_endian_system())
        {
          input_free = PDF_TRUE;
          input_data = pdf_text_test_change_utf32_endianness((pdf_char_t *) \
                                                             &utf32be_strings[i].data[4],
                                                             input_size);
          /* Just in case... */
          fail_if(input_data == NULL);
        }
      else
        {
          input_data = (pdf_char_t *)&utf32be_strings[i].data[4];
        }
      
      /* Set expected data */
      expected_data = (pdf_char_t *)utf32be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf32be_strings[i].utf32be_size;
      if(!pdf_text_test_big_endian_system())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness(expected_data,
                                                                expected_size);
          /* Just in case... */
          fail_if(expected_data == NULL);
        }
      
      /* 1. The call to pdf_text_set_unicode should return PDF_OK. */
      fail_if(pdf_text_set_unicode(text,
                                   input_data,
                                   input_size,
                                   PDF_TEXT_UTF32_HE) != PDF_OK);
      
      /* 2. The contents of the text object must be the expected ones. */

      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size);
      fail_unless(memcmp(actual_data, expected_data, expected_size)==0);

      fail_if(pdf_text_destroy(text) != PDF_OK);
      
      if(expected_free)
        {
          pdf_dealloc(expected_data);
        }
      if(input_free)
        {
          pdf_dealloc(input_data);
        }
      ++i;
    }  

}
END_TEST

/*
 * Test: pdf_text_set_unicode_008
 * Description:
 *   Set the contents of a text object with an input valid UTF-8 encoded string
 *   (containing 8-bit, 16-bit, 24-bit and 32-bit code points!) with BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 */
START_TEST(pdf_text_set_unicode_008)
{
  


  extern const test_string_t utf8_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf8_strings[i].data != NULL)
    {
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data = NULL;
      const pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_size_t expected_size = 0;
      pdf_text_t text = NULL;
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set input data */
      input_data = (pdf_char_t *)&(utf8_strings[i].data[0]);
      input_size = (pdf_size_t)utf8_strings[i].size;
      
      /* Set expected data */
      expected_data = (pdf_char_t *)utf8_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf8_strings[i].utf32be_size;
      if(!pdf_text_test_big_endian_system())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness(expected_data,
                                                                expected_size);
          /* Just in case... */
          fail_if(expected_data == NULL);
        }
      
      /* 1. The call to pdf_text_set_unicode should return PDF_OK. */
      fail_if(pdf_text_set_unicode(text,
                                   input_data,
                                   input_size,
                                   PDF_TEXT_UTF8) != PDF_OK);
      
      /* 2. The contents of the text object must be the expected ones. */

      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size);
      fail_unless(memcmp(actual_data, expected_data, expected_size)==0);
      
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
 * Test: pdf_text_set_unicode_009
 * Description:
 *   Set the contents of a text object with an input valid UTF-16BE encoded 
 *    string (16-bit and 32-bit code points!) with BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 */
START_TEST(pdf_text_set_unicode_009)
{
  


  extern const test_string_t utf16be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf16be_strings[i].data != NULL)
    {
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data = NULL;
      const pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_size_t expected_size = 0;
      pdf_text_t text = NULL;
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set input data */
      input_data = (pdf_char_t *)&(utf16be_strings[i].data[0]);
      input_size = (pdf_size_t)utf16be_strings[i].size;
      
      /* Set expected data */
      expected_data = (pdf_char_t *)utf16be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf16be_strings[i].utf32be_size;
      if(!pdf_text_test_big_endian_system())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness(expected_data,
                                                                expected_size);
          /* Just in case... */
          fail_if(expected_data == NULL);
        }
      
      /* 1. The call to pdf_text_new_from_unicode should return PDF_OK. */
      fail_if(pdf_text_set_unicode(text,
                                   input_data,
                                   input_size,
                                   PDF_TEXT_UTF16_BE) != PDF_OK);
      
      /* 2. The contents of the text object must be the expected ones. */

      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size);
      fail_unless(memcmp(actual_data, expected_data, expected_size)==0);
      
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
 * Test: pdf_text_set_unicode_010
 * Description:
 *   Set the contents of a text object with an input valid UTF-16LE encoded 
 *    string (16-bit and 32-bit code points!) with BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 */
START_TEST(pdf_text_set_unicode_010)
{
  


  extern const test_string_t utf16be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf16be_strings[i].data != NULL)
    {
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_size_t expected_size = 0;
      pdf_text_t text = NULL;
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set input data (change endiannes to input UTF-16BE string) */
      input_size = (pdf_size_t)utf16be_strings[i].size;
      input_data=pdf_text_test_change_utf16_endianness((pdf_char_t *) \
                                                       &utf16be_strings[i].data[0],
                                                       input_size);
      
      /* Set expected data */
      expected_data = (pdf_char_t *)utf16be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf16be_strings[i].utf32be_size;
      if(!pdf_text_test_big_endian_system())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness(expected_data,
                                                                expected_size);
          /* Just in case... */
          fail_if(expected_data == NULL);
        }
      
      /* 1. The call to pdf_text_new_from_unicode should return PDF_OK. */
      fail_if(pdf_text_set_unicode(text,
                                   input_data,
                                   input_size,
                                   PDF_TEXT_UTF16_LE) != PDF_OK);
      
      /* 2. The contents of the text object must be the expected ones. */

      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size);
      fail_unless(memcmp(actual_data, expected_data, expected_size)==0);
      
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
 * Test: pdf_text_set_unicode_011
 * Description:
 *   Set the contents of a text object with an input valid UTF-16HE encoded 
 *    string (16-bit and 32-bit code points!) with BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 */
START_TEST(pdf_text_set_unicode_011)
{
  


  extern const test_string_t utf16be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf16be_strings[i].data != NULL)
    {
      pdf_bool_t input_free = PDF_FALSE;
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_size_t expected_size = 0;
      pdf_text_t text = NULL;
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set input data (change endiannes to input UTF-16BE string if required)
       */
      input_size = (pdf_size_t)utf16be_strings[i].size;
      if(!pdf_text_test_big_endian_system())
        {
          input_free = PDF_TRUE;
          input_data = pdf_text_test_change_utf16_endianness((pdf_char_t *) \
                                                             &utf16be_strings[i].data[0],
                                                             input_size);
          /* Just in case... */
          fail_if(input_data == NULL);
        }
      else
        {
          input_data = (pdf_char_t *)&utf16be_strings[i].data[0];
        }
      
      /* Set expected data */
      expected_data = (pdf_char_t *)utf16be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf16be_strings[i].utf32be_size;
      if(!pdf_text_test_big_endian_system())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness(expected_data,
                                                                expected_size);
          /* Just in case... */
          fail_if(expected_data == NULL);
        }
      
      /* 1. The call to pdf_text_new_from_unicode should return PDF_OK. */
      fail_if(pdf_text_set_unicode(text,
                                   input_data,
                                   input_size,
                                   PDF_TEXT_UTF16_HE) != PDF_OK);
      
      /* 2. The contents of the text object must be the expected ones. */

      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size);
      fail_unless(memcmp(actual_data, expected_data, expected_size)==0);
      
      fail_if(pdf_text_destroy(text) != PDF_OK);
      
      if(expected_free)
        {
          pdf_dealloc(expected_data);
        }
      if(input_free)
        {
          pdf_dealloc(input_data);
        }
      ++i;
    }

}
END_TEST


/*
 * Test: pdf_text_set_unicode_012
 * Description:
 *   Set the contents of a text object with an input valid UTF-32BE encoded 
 *    string with BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 */
START_TEST(pdf_text_set_unicode_012)
{
  


  extern const test_string_t utf32be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf32be_strings[i].data != NULL)
    {
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data = NULL;
      const pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_size_t expected_size = 0;
      pdf_text_t text = NULL;
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set input data */
      input_size = (pdf_size_t)utf32be_strings[i].size;
      input_data = (pdf_char_t *)&utf32be_strings[i].data[0];
      
      /* Set expected data */
      expected_data = (pdf_char_t *)utf32be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf32be_strings[i].utf32be_size;
      if(!pdf_text_test_big_endian_system())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness(expected_data,
                                                                expected_size);
          /* Just in case... */
          fail_if(expected_data == NULL);
        }
      
      /* 1. The call to pdf_text_set_unicode should return PDF_OK. */
      fail_if(pdf_text_set_unicode(text,
                                   input_data,
                                   input_size,
                                   PDF_TEXT_UTF32_BE) != PDF_OK);
      
      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *input_hex = NULL;
          pdf_char_t *internal_hex = NULL;
          pdf_char_t *expected_hex = NULL;
          pdf_size_t actual_size;
          pdf_char_t *actual_data;
          fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                           PDF_TEXT_UTF32_HE,0) == PDF_OK);
          internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
          input_hex = pdf_text_test_get_hex(input_data,input_size,':');
          expected_hex = pdf_text_test_get_hex(expected_data,expected_size,':');
          fail_if(input_hex == NULL);
          fail_if(expected_hex == NULL);
          fail_if(internal_hex == NULL);
          printf("pdf_text_new_from_unicode_012:%d:Input> '%s'\n",
                 i, input_hex);
          printf("pdf_text_new_from_unicode_012:%d:Internal> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_new_from_unicode_012:%d:Expected> '%s'\n",
                 i, expected_hex);
          pdf_dealloc(input_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
        }
      
      /* 2. The contents of the text object must be the expected ones. */

      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size);
      fail_unless(memcmp(actual_data, expected_data, expected_size)==0);

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
 * Test: pdf_text_set_unicode_013
 * Description:
 *   Set the contents of a text object with an input valid UTF-32LE encoded 
 *    string with BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 */
START_TEST(pdf_text_set_unicode_013)
{
  


  extern const test_string_t utf32be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf32be_strings[i].data != NULL)
    {
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_char_t *expected_data = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_size_t expected_size = 0;
      pdf_text_t text = NULL;
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set input data */
      input_size = (pdf_size_t)utf32be_strings[i].size;
      input_data = pdf_text_test_change_utf32_endianness((pdf_char_t *) \
                                                         &utf32be_strings[i].data[0],
                                                         input_size);
      
      /* Set expected data */
      expected_data = (pdf_char_t *)utf32be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf32be_strings[i].utf32be_size;
      if(!pdf_text_test_big_endian_system())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness(expected_data,
                                                                expected_size);
          /* Just in case... */
          fail_if(expected_data == NULL);
        }
      
      /* 1. The call to pdf_text_set_unicode should return PDF_OK. */
      fail_if(pdf_text_set_unicode(text,
                                   input_data,
                                   input_size,
                                   PDF_TEXT_UTF32_LE) != PDF_OK);
            
      /* 2. The contents of the text object must be the expected ones. */

      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size);
      fail_unless(memcmp(actual_data, expected_data, expected_size)==0);
      
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
 * Test: pdf_text_set_unicode_014
 * Description:
 *   Set the contents of a text object with an input valid UTF-32HE encoded 
 *    string with BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones (without BOM).
 */
START_TEST(pdf_text_set_unicode_014)
{
  


  extern const test_string_t utf32be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  i = 0;
  while(utf32be_strings[i].data != NULL)
    {
      pdf_bool_t expected_free = PDF_FALSE;
      pdf_bool_t input_free = PDF_FALSE;
      pdf_char_t *expected_data = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_size_t expected_size = 0;
      pdf_text_t text = NULL;
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set input data (UTF-32HE) */
      input_size = (pdf_size_t)utf32be_strings[i].size;
      if(!pdf_text_test_big_endian_system())
        {
          input_free = PDF_TRUE;
          input_data = pdf_text_test_change_utf32_endianness((pdf_char_t *) \
                                                             &utf32be_strings[i].data[0],
                                                             input_size);
          /* Just in case... */
          fail_if(input_data == NULL);
        }
      else
        {
          input_data = (pdf_char_t *)&utf32be_strings[i].data[0];
        }
      
      /* Set expected data */
      expected_data = (pdf_char_t *)utf32be_strings[i].utf32be_data;
      expected_size = (pdf_size_t)utf32be_strings[i].utf32be_size;
      if(!pdf_text_test_big_endian_system())
        {
          /* Must change endianness of expected_data */
          expected_free = PDF_TRUE;
          expected_data = pdf_text_test_change_utf32_endianness(expected_data,
                                                                expected_size);
          /* Just in case... */
          fail_if(expected_data == NULL);
        }
      
      /* 1. The call to pdf_text_new_from_unicode should return PDF_OK. */
      fail_if(pdf_text_set_unicode(text,
                                   input_data,
                                   input_size,
                                   PDF_TEXT_UTF32_HE) != PDF_OK);
      
      /* 2. The contents of the text object must be the expected ones. */

      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size);
      fail_unless(memcmp(actual_data, expected_data, expected_size)==0);
      
      fail_if(pdf_text_destroy(text) != PDF_OK);
      
      if(expected_free)
        {
          pdf_dealloc(expected_data);
        }
      if(input_free)
        {
          pdf_dealloc(input_data);
        }
      ++i;
    }  

}
END_TEST


/*
 * Test: pdf_text_set_unicode_015
 * Description:
 *   Set the contents of a text object with an input invalid UTF-8 encoded
 *   string without BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should NOT return PDF_OK.
 *   2. The contents of the text object must remain unchanged.
 */
START_TEST(pdf_text_set_unicode_015)
{
  extern const test_string_t utf16be_strings[];
  extern const test_string_t utf32be_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  
  /* Try UTF-16BE strings */
  i = 0;
  while(utf16be_strings[i].data != NULL)
    {
      pdf_text_t text = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_char_t *output_data;
      const pdf_char_t *previous_string = (pdf_char_t *)"a";
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set previous string */
      fail_if(pdf_text_set_pdfdocenc(text, previous_string) != PDF_OK);
      
      input_data = (pdf_char_t *)utf16be_strings[i].data;
      input_size = (pdf_size_t)utf16be_strings[i].size;
      
      /* 1. The call to  pdf_text_set_unicode should NOT return PDF_OK. */
      fail_unless(pdf_text_set_unicode(text,
                                       input_data,
                                       input_size,
                                       PDF_TEXT_UTF8) != PDF_OK);
      
      /* 2. The contents of the text object must remain unchanged. */
      fail_if(pdf_text_get_pdfdocenc(&output_data, text) != PDF_OK);
      fail_if(output_data == NULL);
      fail_unless(strcmp((char *)output_data, (char *)previous_string) == 0);
      
      pdf_text_destroy(text);
      pdf_dealloc(output_data);
      ++i;
    }
  /* Try UTF-32BE strings */
  i = 0;
  while(utf32be_strings[i].data != NULL)
    {
      pdf_text_t text = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_char_t *output_data;
      const pdf_char_t *previous_string = (pdf_char_t *)"a";
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set previous string */
      fail_if(pdf_text_set_pdfdocenc(text, previous_string) != PDF_OK);
      
      input_data = (pdf_char_t *)utf32be_strings[i].data;
      input_size = (pdf_size_t)utf32be_strings[i].size;
      
      /* 1. The call to  pdf_text_set_unicode should NOT return PDF_OK. */
      fail_unless(pdf_text_set_unicode(text,
                                       input_data,
                                       input_size,
                                       PDF_TEXT_UTF8) != PDF_OK);
      
      /* 2. The contents of the text object must remain unchanged. */
      fail_if(pdf_text_get_pdfdocenc(&output_data, text) != PDF_OK);
      fail_if(output_data == NULL);
      fail_unless(strcmp((char *)output_data, (char *)previous_string) == 0);
      
      pdf_text_destroy(text);
      pdf_dealloc(output_data);
      
      ++i;
    }
}
END_TEST


/*
 * Test: pdf_text_set_unicode_016
 * Description:
 *   Set the contents of a text object with an input invalid UTF-16BE encoded
 *   string without BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should NOT return PDF_OK.
 *   2. The contents of the text object must remain unchanged.
 */
START_TEST(pdf_text_set_unicode_016)
{
  extern const test_string_t utf8_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  
  /* Try UTF-8 strings */
  i = 0;
  while(utf8_strings[i].data != NULL)
    {
      pdf_text_t text = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_char_t *output_data;
      const pdf_char_t *previous_string = (pdf_char_t *)"a";
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set previous string */
      fail_if(pdf_text_set_pdfdocenc(text, previous_string) != PDF_OK);
      
      input_data = (pdf_char_t *)utf8_strings[i].data;
      input_size = (pdf_size_t)utf8_strings[i].size;
      
      /* 1. The call to  pdf_text_set_unicode should NOT return PDF_OK. */
      fail_unless(pdf_text_set_unicode(text,
                                       input_data,
                                       input_size,
                                       PDF_TEXT_UTF16_BE) != PDF_OK);
      
      /* 2. The contents of the text object must remain unchanged. */
      fail_if(pdf_text_get_pdfdocenc(&output_data, text) != PDF_OK);
      fail_if(output_data == NULL);
      fail_unless(strcmp((char *)output_data, (char *)previous_string) == 0);
      
      pdf_text_destroy(text);
      pdf_dealloc(output_data);
      
      ++i;
    }
}
END_TEST


/*
 * Test: pdf_text_set_unicode_017
 * Description:
 *   Set the contents of a text object with an input invalid UTF-16LE encoded
 *   string without BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should NOT return PDF_OK.
 *   2. The contents of the text object must remain unchanged.
 */
START_TEST(pdf_text_set_unicode_017)
{
  extern const test_string_t utf8_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  
  /* Try UTF-8 strings */
  i = 0;
  while(utf8_strings[i].data != NULL)
    {
      pdf_text_t text = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_char_t *output_data;
      const pdf_char_t *previous_string = (pdf_char_t *)"a";
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set previous string */
      fail_if(pdf_text_set_pdfdocenc(text, previous_string) != PDF_OK);
      
      input_data = (pdf_char_t *)utf8_strings[i].data;
      input_size = (pdf_size_t)utf8_strings[i].size;
      
      /* 1. The call to  pdf_text_set_unicode should NOT return PDF_OK. */
      fail_unless(pdf_text_set_unicode(text,
                                       input_data,
                                       input_size,
                                       PDF_TEXT_UTF16_LE) != PDF_OK);
      
      /* 2. The contents of the text object must remain unchanged. */
      fail_if(pdf_text_get_pdfdocenc(&output_data, text) != PDF_OK);
      fail_if(output_data == NULL);
      fail_unless(strcmp((char *)output_data, (char *)previous_string) == 0);
      
      pdf_text_destroy(text);
      pdf_dealloc(output_data);
      
      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_set_unicode_018
 * Description:
 *   Set the contents of a text object with an input invalid UTF-16HE encoded
 *   string without BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should NOT return PDF_OK.
 *   2. The contents of the text object must remain unchanged.
 */
START_TEST(pdf_text_set_unicode_018)
{
  extern const test_string_t utf8_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  
  /* Try UTF-8 strings */
  i = 0;
  while(utf8_strings[i].data != NULL)
    {
      pdf_text_t text = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_char_t *output_data;
      const pdf_char_t *previous_string = (pdf_char_t *)"a";
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set previous string */
      fail_if(pdf_text_set_pdfdocenc(text, previous_string) != PDF_OK);
      
      input_data = (pdf_char_t *)utf8_strings[i].data;
      input_size = (pdf_size_t)utf8_strings[i].size;
      
      /* 1. The call to  pdf_text_set_unicode should NOT return PDF_OK. */
      fail_unless(pdf_text_set_unicode(text,
                                       input_data,
                                       input_size,
                                       PDF_TEXT_UTF16_HE) != PDF_OK);
      
      /* 2. The contents of the text object must remain unchanged. */
      fail_if(pdf_text_get_pdfdocenc(&output_data, text) != PDF_OK);
      fail_if(output_data == NULL);
      fail_unless(strcmp((char *)output_data, (char *)previous_string) == 0);
      
      pdf_text_destroy(text);
      pdf_dealloc(output_data);
      
      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_set_unicode_019
 * Description:
 *   Set the contents of a text object with an input invalid UTF-32BE encoded
 *   string without BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should NOT return PDF_OK.
 *   2. The contents of the text object must remain unchanged.
 */
START_TEST(pdf_text_set_unicode_019)
{
  extern const test_string_t utf8_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  
  /* Try UTF-8 strings */
  i = 0;
  while(utf8_strings[i].data != NULL)
    {
      pdf_text_t text = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_char_t *output_data;
      const pdf_char_t *previous_string = (pdf_char_t *)"a";
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set previous string */
      fail_if(pdf_text_set_pdfdocenc(text, previous_string) != PDF_OK);
      
      input_data = (pdf_char_t *)utf8_strings[i].data;
      input_size = (pdf_size_t)utf8_strings[i].size;
      
      /* 1. The call to  pdf_text_set_unicode should NOT return PDF_OK. */
      fail_unless(pdf_text_set_unicode(text,
                                       input_data,
                                       input_size,
                                       PDF_TEXT_UTF32_BE) != PDF_OK);
      
      /* 2. The contents of the text object must remain unchanged. */
      fail_if(pdf_text_get_pdfdocenc(&output_data, text) != PDF_OK);
      fail_if(output_data == NULL);
      fail_unless(strcmp((char *)output_data, (char *)previous_string) == 0);
      
      pdf_text_destroy(text);
      pdf_dealloc(output_data);
      
      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_set_unicode_020
 * Description:
 *   Set the contents of a text object with an input invalid UTF-32LE encoded
 *   string without BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should NOT return PDF_OK.
 *   2. The contents of the text object must remain unchanged.
 */
START_TEST(pdf_text_set_unicode_020)
{
  extern const test_string_t utf8_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  
  /* Try UTF-8 strings */
  i = 0;
  while(utf8_strings[i].data != NULL)
    {
      pdf_text_t text = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_char_t *output_data;
      const pdf_char_t *previous_string = (pdf_char_t *)"a";
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set previous string */
      fail_if(pdf_text_set_pdfdocenc(text, previous_string) != PDF_OK);
      
      input_data = (pdf_char_t *)utf8_strings[i].data;
      input_size = (pdf_size_t)utf8_strings[i].size;
      
      /* 1. The call to  pdf_text_set_unicode should NOT return PDF_OK. */
      fail_unless(pdf_text_set_unicode(text,
                                       input_data,
                                       input_size,
                                       PDF_TEXT_UTF32_LE) != PDF_OK);
      
      /* 2. The contents of the text object must remain unchanged. */
      fail_if(pdf_text_get_pdfdocenc(&output_data, text) != PDF_OK);
      fail_if(output_data == NULL);
      fail_unless(strcmp((char *)output_data, (char *)previous_string) == 0);
      
      pdf_text_destroy(text);
      pdf_dealloc(output_data);
      
      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_set_unicode_021
 * Description:
 *   Set the contents of a text object with an input invalid UTF-32HE encoded
 *   string without BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should NOT return PDF_OK.
 *   2. The contents of the text object must remain unchanged.
 */
START_TEST(pdf_text_set_unicode_021)
{
  extern const test_string_t utf8_strings[];
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  
  /* Try UTF-8 strings */
  i = 0;
  while(utf8_strings[i].data != NULL)
    {
      pdf_text_t text = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_char_t *output_data;
      const pdf_char_t *previous_string = (pdf_char_t *)"a";
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set previous string */
      fail_if(pdf_text_set_pdfdocenc(text, previous_string) != PDF_OK);
      
      input_data = (pdf_char_t *)utf8_strings[i].data;
      input_size = (pdf_size_t)utf8_strings[i].size;
      
      /* 1. The call to  pdf_text_set_unicode should NOT return PDF_OK. */
      fail_unless(pdf_text_set_unicode(text,
                                       input_data,
                                       input_size,
                                       PDF_TEXT_UTF32_HE) != PDF_OK);
      
      /* 2. The contents of the text object must remain unchanged. */
      fail_if(pdf_text_get_pdfdocenc(&output_data, text) != PDF_OK);
      fail_if(output_data == NULL);
      fail_unless(strcmp((char *)output_data, (char *)previous_string) == 0);
      
      pdf_text_destroy(text);
      pdf_dealloc(output_data);
      
      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_set_unicode_022
 * Description:
 *   Set the contents of a text object with an input invalid UTF-8 encoded
 *   string with BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should NOT return PDF_OK.
 *   2. The contents of the text object must remain unchanged.
 */
START_TEST(pdf_text_set_unicode_022)
{
  extern const test_string_t utf16be_strings[];
  extern const test_string_t utf32be_strings[];
  int i;
  const pdf_char_t *bom_data = (pdf_char_t *)"\xEF\xBB\xBF";
  pdf_size_t bom_size = 3;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  
  /* Try UTF-16BE strings */
  i = 0;
  while(utf16be_strings[i].data != NULL)
    {
      pdf_text_t text = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_char_t *output_data;
      const pdf_char_t *previous_string = (pdf_char_t *)"a";
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set previous string */
      fail_if(pdf_text_set_pdfdocenc(text, previous_string) != PDF_OK);
      
      input_size = (pdf_size_t)utf16be_strings[i].size + bom_size;
      input_data = (pdf_char_t *)pdf_alloc(input_size);
      memcpy(&input_data[0], bom_data, bom_size);
      memcpy(&input_data[bom_size -1],
             utf16be_strings[i].data,
             utf16be_strings[i].size);
      
      /* 1. The call to  pdf_text_set_unicode should NOT return PDF_OK. */
      fail_unless(pdf_text_set_unicode(text,
                                       input_data,
                                       input_size,
                                       PDF_TEXT_UTF8) != PDF_OK);
      
      /* 2. The contents of the text object must remain unchanged. */
      fail_if(pdf_text_get_pdfdocenc(&output_data, text) != PDF_OK);
      fail_if(output_data == NULL);
      fail_unless(strcmp((char *)output_data, (char *)previous_string) == 0);
      
      pdf_text_destroy(text);
      pdf_dealloc(output_data);
      pdf_dealloc(input_data);
      ++i;
    }
  /* Try UTF-32BE strings */
  i = 0;
  while(utf32be_strings[i].data != NULL)
    {
      pdf_text_t text = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_char_t *output_data;
      const pdf_char_t *previous_string = "a";
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set previous string */
      fail_if(pdf_text_set_pdfdocenc(text, previous_string) != PDF_OK);
      
      input_size = (pdf_size_t)utf32be_strings[i].size + bom_size;
      input_data = (pdf_char_t *)pdf_alloc(input_size);
      memcpy(&input_data[0], bom_data, bom_size);
      memcpy(&input_data[bom_size -1],
             utf32be_strings[i].data,
             utf32be_strings[i].size);
      
      /* 1. The call to  pdf_text_set_unicode should NOT return PDF_OK. */
      fail_unless(pdf_text_set_unicode(text,
                                       input_data,
                                       input_size,
                                       PDF_TEXT_UTF8) != PDF_OK);
      
      /* 2. The contents of the text object must remain unchanged. */
      fail_if(pdf_text_get_pdfdocenc(&output_data, text) != PDF_OK);
      fail_if(output_data == NULL);
      fail_unless(strcmp((char *)output_data, (char *)previous_string) == 0);
      
      pdf_text_destroy(text);
      pdf_dealloc(output_data);
      pdf_dealloc(input_data);
      ++i;
    }
}
END_TEST


/*
 * Test: pdf_text_set_unicode_023
 * Description:
 *   Set the contents of a text object with an input invalid UTF-16BE encoded
 *   string with BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should NOT return PDF_OK.
 *   2. The contents of the text object must remain unchanged.
 */
START_TEST(pdf_text_set_unicode_023)
{
  extern const test_string_t utf8_strings[];
  int i;
  const pdf_char_t *bom_data = (pdf_char_t *)"\xFE\xFF";
  pdf_size_t bom_size = 2;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  
  /* Try UTF-8 strings */
  i = 0;
  while(utf8_strings[i].data != NULL)
    {
      pdf_text_t text = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_char_t *output_data;
      const pdf_char_t *previous_string = (pdf_char_t *)"a";
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set previous string */
      fail_if(pdf_text_set_pdfdocenc(text, previous_string) != PDF_OK);
      
      input_size = (pdf_size_t)utf8_strings[i].size + bom_size;
      input_data = (pdf_char_t *)pdf_alloc(input_size);
      memcpy(&input_data[0], bom_data, bom_size);
      memcpy(&input_data[bom_size -1],
             utf8_strings[i].data,
             utf8_strings[i].size);
      
      /* 1. The call to  pdf_text_set_unicode should NOT return PDF_OK. */
      fail_unless(pdf_text_set_unicode(text,
                                       input_data,
                                       input_size,
                                       PDF_TEXT_UTF16_BE) != PDF_OK);
      
      /* 2. The contents of the text object must remain unchanged. */
      fail_if(pdf_text_get_pdfdocenc(&output_data, text) != PDF_OK);
      fail_if(output_data == NULL);
      fail_unless(strcmp((char *)output_data, (char *)previous_string) == 0);
      
      pdf_text_destroy(text);
      pdf_dealloc(output_data);
      pdf_dealloc(input_data);
      ++i;
    }
}
END_TEST


/*
 * Test: pdf_text_set_unicode_024
 * Description:
 *   Set the contents of a text object with an input invalid UTF-16LE encoded
 *   string with BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should NOT return PDF_OK.
 *   2. The contents of the text object must remain unchanged.
 */
START_TEST(pdf_text_set_unicode_024)
{
  extern const test_string_t utf8_strings[];
  int i;
  const pdf_char_t *bom_data = (pdf_char_t *)"\xFF\xFE";
  pdf_size_t bom_size = 2;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  
  /* Try UTF-8 strings */
  i = 0;
  while(utf8_strings[i].data != NULL)
    {
      pdf_text_t text = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_char_t *output_data;
      const pdf_char_t *previous_string = (pdf_char_t *)"a";
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set empty string */
      fail_if(pdf_text_set_pdfdocenc(text, previous_string) != PDF_OK);
      
      input_size = (pdf_size_t)utf8_strings[i].size + bom_size;
      input_data = (pdf_char_t *)pdf_alloc(input_size);
      memcpy(&input_data[0], bom_data, bom_size);
      memcpy(&input_data[bom_size -1],
             utf8_strings[i].data,
             utf8_strings[i].size);
      
      /* 1. The call to  pdf_text_set_unicode should NOT return PDF_OK. */
      fail_unless(pdf_text_set_unicode(text,
                                       input_data,
                                       input_size,
                                       PDF_TEXT_UTF16_LE) != PDF_OK);
      
      /* 2. The contents of the text object must remain unchanged. */
      fail_if(pdf_text_get_pdfdocenc(&output_data, text) != PDF_OK);
      fail_if(output_data == NULL);
      fail_unless(strcmp((char *)output_data, (char *)previous_string) == 0);
      
      pdf_text_destroy(text);
      pdf_dealloc(output_data);
      pdf_dealloc(input_data);
      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_set_unicode_025
 * Description:
 *   Set the contents of a text object with an input invalid UTF-16HE encoded
 *   string with BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should NOT return PDF_OK.
 *   2. The contents of the text object must remain unchanged.
 */
START_TEST(pdf_text_set_unicode_025)
{
  extern const test_string_t utf8_strings[];
  int i;
  const pdf_char_t *bom_data;
  pdf_size_t bom_size = 2;
  
  if(pdf_text_test_big_endian_system())
    {
      bom_data = (pdf_char_t *)"\xFE\xFF";
    }
  else
    {
      bom_data = (pdf_char_t *)"\xFF\xFE";
    }
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  
  /* Try UTF-8 strings */
  i = 0;
  while(utf8_strings[i].data != NULL)
    {
      pdf_text_t text = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_char_t *output_data;
      const pdf_char_t *previous_string = (pdf_char_t *)"a";
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set empty string */
      fail_if(pdf_text_set_pdfdocenc(text, previous_string) != PDF_OK);
      
      input_size = (pdf_size_t)utf8_strings[i].size + bom_size;
      input_data = (pdf_char_t *)pdf_alloc(input_size);
      memcpy(&input_data[0], bom_data, bom_size);
      memcpy(&input_data[bom_size -1],
             utf8_strings[i].data,
             utf8_strings[i].size);
      
      /* 1. The call to  pdf_text_set_unicode should NOT return PDF_OK. */
      fail_unless(pdf_text_set_unicode(text,
                                       input_data,
                                       input_size,
                                       PDF_TEXT_UTF16_HE) != PDF_OK);
      
      /* 2. The contents of the text object must remain unchanged. */
      fail_if(pdf_text_get_pdfdocenc(&output_data, text) != PDF_OK);
      fail_if(output_data == NULL);
      fail_unless(strcmp((char *)output_data, (char *)previous_string) == 0);
      
      pdf_text_destroy(text);
      pdf_dealloc(output_data);
      pdf_dealloc(input_data);
      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_set_unicode_026
 * Description:
 *   Set the contents of a text object with an input invalid UTF-32BE encoded
 *   string with BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should NOT return PDF_OK.
 *   2. The contents of the text object must remain unchanged.
 */
START_TEST(pdf_text_set_unicode_026)
{
  extern const test_string_t utf8_strings[];
  int i;
  const pdf_char_t *bom_data = (pdf_char_t *)"\x00\x00\xFE\xFF";
  pdf_size_t bom_size = 4;

  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  
  /* Try UTF-8 strings */
  i = 0;
  while(utf8_strings[i].data != NULL)
    {
      pdf_text_t text = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_char_t *output_data;
      const pdf_char_t *previous_string = (pdf_char_t *)"a";
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set empty string */
      fail_if(pdf_text_set_pdfdocenc(text, previous_string) != PDF_OK);
      
      input_size = (pdf_size_t)utf8_strings[i].size + bom_size;
      input_data = (pdf_char_t *)pdf_alloc(input_size);
      memcpy(&input_data[0], bom_data, bom_size);
      memcpy(&input_data[bom_size -1],
             utf8_strings[i].data,
             utf8_strings[i].size);
      
      /* 1. The call to  pdf_text_set_unicode should NOT return PDF_OK. */
      fail_unless(pdf_text_set_unicode(text,
                                       input_data,
                                       input_size,
                                       PDF_TEXT_UTF32_BE) != PDF_OK);
      
      /* 2. The contents of the text object must remain unchanged. */
      fail_if(pdf_text_get_pdfdocenc(&output_data, text) != PDF_OK);
      fail_if(output_data == NULL);
      fail_unless(strcmp((char *)output_data, (char *)previous_string) == 0);
      
      pdf_text_destroy(text);
      pdf_dealloc(output_data);
      pdf_dealloc(input_data);
      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_set_unicode_027
 * Description:
 *   Set the contents of a text object with an input invalid UTF-32LE encoded
 *   string with BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should NOT return PDF_OK.
 *   2. The contents of the text object must remain unchanged.
 */
START_TEST(pdf_text_set_unicode_027)
{
  extern const test_string_t utf8_strings[];
  int i;
  const pdf_char_t *bom_data = (pdf_char_t *)"\xFF\xFE\x00\x00";
  pdf_size_t bom_size = 4;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  
  /* Try UTF-8 strings */
  i = 0;
  while(utf8_strings[i].data != NULL)
    {
      pdf_text_t text = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_char_t *output_data;
      const pdf_char_t *previous_string = (pdf_char_t *)"a";
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set empty string */
      fail_if(pdf_text_set_pdfdocenc(text, previous_string) != PDF_OK);
      
      input_size = (pdf_size_t)utf8_strings[i].size + bom_size;
      input_data = (pdf_char_t *)pdf_alloc(input_size);
      memcpy(&input_data[0], bom_data, bom_size);
      memcpy(&input_data[bom_size -1],
             utf8_strings[i].data,
             utf8_strings[i].size);
      
      /* 1. The call to  pdf_text_set_unicode should NOT return PDF_OK. */
      fail_unless(pdf_text_set_unicode(text,
                                       input_data,
                                       input_size,
                                       PDF_TEXT_UTF32_LE) != PDF_OK);
      
      /* 2. The contents of the text object must remain unchanged. */
      fail_if(pdf_text_get_pdfdocenc(&output_data, text) != PDF_OK);
      fail_if(output_data == NULL);
      fail_unless(strcmp((char *)output_data, (char *)previous_string) == 0);
      
      pdf_text_destroy(text);
      pdf_dealloc(output_data);
      pdf_dealloc(input_data);
      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_set_unicode_028
 * Description:
 *   Set the contents of a text object with an input invalid UTF-32HE encoded
 *   string with BOM
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should NOT return PDF_OK.
 *   2. The contents of the text object must remain unchanged.
 */
START_TEST(pdf_text_set_unicode_028)
{
  extern const test_string_t utf8_strings[];
  int i;
  const pdf_char_t *bom_data;
  pdf_size_t bom_size = 4;
  
  if(pdf_text_test_big_endian_system())
    {
      bom_data = (pdf_char_t *)"\x00\x00\xFE\xFF";
    }
  else
    {
      bom_data = (pdf_char_t *)"\xFF\xFE\x00\x00";
    }
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);
  
  
  /* Try UTF-8 strings */
  i = 0;
  while(utf8_strings[i].data != NULL)
    {
      pdf_text_t text = NULL;
      pdf_char_t *input_data = NULL;
      pdf_size_t input_size = 0;
      pdf_char_t *output_data;
      const pdf_char_t *previous_string = (pdf_char_t *)"a";
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set previous string */
      fail_if(pdf_text_set_pdfdocenc(text, previous_string) != PDF_OK);
      
      input_size = (pdf_size_t)utf8_strings[i].size + bom_size;
      input_data = (pdf_char_t *)pdf_alloc(input_size);
      memcpy(&input_data[0], bom_data, bom_size);
      memcpy(&input_data[bom_size -1],
             utf8_strings[i].data,
             utf8_strings[i].size);

      /* 1. The call to  pdf_text_set_unicode should NOT return PDF_OK. */
      fail_unless(pdf_text_set_unicode(text,
                                       input_data,
                                       input_size,
                                       PDF_TEXT_UTF32_HE) != PDF_OK);
      
      /* 2. The contents of the text object must remain unchanged. */
      fail_if(pdf_text_get_pdfdocenc(&output_data, text) != PDF_OK);
      fail_if(output_data == NULL);
      fail_unless(strcmp((char *)output_data, (char *)previous_string) == 0);
      
      pdf_text_destroy(text);
      pdf_dealloc(output_data);
      pdf_dealloc(input_data);
      ++i;
    }
}
END_TEST

/*
 * Test: pdf_text_set_unicode_029
 * Description:
 *   Set the contents of a text object with an input valid UTF-16BE encoded
 *   string with BOM and embedded language/country information (this function
 *   does not allow this kind of input!)
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones.
 *   3. The language code must be empty.
 *   4. The country code must be empty.
 */
START_TEST(pdf_text_set_unicode_029)
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
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
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
      expected_size = utf16be_strings[i].utf32be_size + 16;
      expected_data = (pdf_char_t *)pdf_alloc(expected_size);
      fail_if(expected_data == NULL);
      memcpy(&expected_data[0], "\x00\x00\x00\x1B", 4);
      memcpy(&expected_data[4], "\x00\x00", 2);
      memcpy(&expected_data[6], language_code, 2);
      memcpy(&expected_data[8], "\x00\x00", 2);
      memcpy(&expected_data[10], country_code, 2);
      memcpy(&expected_data[12], "\x00\x00\x00\x1B", 4);
      memcpy(&expected_data[16],
             utf16be_strings[i].utf32be_data,
             utf16be_strings[i].utf32be_size);

      if(!pdf_text_test_big_endian_system())
        {
          pdf_char_t *temp;
          /* Must change endianness of expected_data */
          
          temp = pdf_text_test_change_utf32_endianness(expected_data,
                                                       expected_size);
          pdf_dealloc(expected_data);
          expected_data = temp;
          /* Just in case... */
          fail_if(expected_data == NULL);
        }
      
      /* 1. The call to pdf_text_set_unicode should return PDF_OK. */
      fail_if(pdf_text_set_unicode(text,
                                   input_data,
                                   input_size,
                                   PDF_TEXT_UTF16_BE) != PDF_OK);
      
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
          fail_if(internal_hex == NULL);
          fail_if(expected_hex == NULL);          
          printf("pdf_text_get_unicode_029:%d:Internal> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_get_unicode_029:%d:Expected> '%s'\n",
                 i, expected_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
        }
      
      /* 2. The contents of the text object must be the expected ones */

      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size);
      fail_unless(memcmp(actual_data, expected_data, expected_size)==0);
      
      /* 3. The language code must be empty. */
      fail_if(strlen((char *)pdf_text_get_language(text)) != 0);
      
      /* 4. The country code must be empty. */
      fail_if(strlen((char *)pdf_text_get_country(text)) != 0);      
      
      fail_if(pdf_text_destroy(text) != PDF_OK);
      pdf_dealloc(expected_data);
      pdf_dealloc(input_data);

      ++i;
    }

}
END_TEST


/*
 * Test: pdf_text_set_unicode_030
 * Description:
 *   Set the contents of a text object without an input valid UTF-16BE encoded
 *   string with BOM and embedded language/country information (this function
 *   does not allow this kind of input!)
 * Success conditions:
 *   1. The call to pdf_text_set_unicode should return PDF_OK.
 *   2. The contents of the text object must be the expected ones.
 *   3. The language code must be empty.
 *   4. The country code must be empty.
 */
START_TEST(pdf_text_set_unicode_030)
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
      
      fail_if(pdf_text_new (&text) != PDF_OK);
      
      /* Set input data with LANGUAGE code WITHOUT BOM */
      input_size = (pdf_size_t)utf16be_strings[i].size + 8 - 2;
      input_data = (pdf_char_t *)pdf_alloc(input_size);
      fail_if(input_data == NULL);
      
      /* Store language code */
      memcpy(&input_data[0], "\x00\x1B", 2);
      memcpy(&input_data[2], language_code, 2);
      memcpy(&input_data[4], country_code, 2);
      memcpy(&input_data[6], "\x00\x1B", 2);
      /* Store remaining string contents */
      memcpy(&input_data[8], 
             &utf16be_strings[i].data[2],
             (pdf_size_t)utf16be_strings[i].size -2);
      
      /* Set expected data */
      expected_size = utf16be_strings[i].utf32be_size + 16;
      expected_data = (pdf_char_t *)pdf_alloc(expected_size);
      fail_if(expected_data == NULL);
      memcpy(&expected_data[0], "\x00\x00\x00\x1B", 4);
      memcpy(&expected_data[4], "\x00\x00", 2);
      memcpy(&expected_data[6], language_code, 2);
      memcpy(&expected_data[8], "\x00\x00", 2);
      memcpy(&expected_data[10], country_code, 2);
      memcpy(&expected_data[12], "\x00\x00\x00\x1B", 4);
      memcpy(&expected_data[16],
             utf16be_strings[i].utf32be_data,
             utf16be_strings[i].utf32be_size);
      
      if(!pdf_text_test_big_endian_system())
        {
          pdf_char_t *temp;
          /* Must change endianness of expected_data */
          
          temp = pdf_text_test_change_utf32_endianness(expected_data,
                                                       expected_size);
          pdf_dealloc(expected_data);
          expected_data = temp;
          /* Just in case... */
          fail_if(expected_data == NULL);
        }
      
      /* 1. The call to pdf_text_set_unicode should return PDF_OK. */
      fail_if(pdf_text_set_unicode(text,
                                   input_data,
                                   input_size,
                                   PDF_TEXT_UTF16_BE) != PDF_OK);
      
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
          fail_if(internal_hex == NULL);
          fail_if(expected_hex == NULL);          
          printf("pdf_text_get_unicode_030:%d:Internal> '%s'\n",
                 i, internal_hex);
          printf("pdf_text_get_unicode_030:%d:Expected> '%s'\n",
                 i, expected_hex);
          pdf_dealloc(internal_hex);
          pdf_dealloc(expected_hex);
        }
      
      /* 2. The contents of the text object must be the expected ones */

      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      fail_unless(actual_size == expected_size);
      fail_unless(memcmp(actual_data, expected_data, expected_size)==0);
      
      /* 3. The language code must be empty */
      fail_if(strlen((char *)pdf_text_get_language(text)) != 0);
      
      /* 4. The country code must be empty */
      fail_if(strlen((char *)pdf_text_get_country(text)) != 0);      
      
      fail_if(pdf_text_destroy(text) != PDF_OK);
      pdf_dealloc(expected_data);
      pdf_dealloc(input_data);
      
      ++i;
    }

}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_text_set_unicode (void)
{
  TCase *tc = tcase_create("pdf_text_set_unicode");
  tcase_add_test(tc, pdf_text_set_unicode_001);
  tcase_add_test(tc, pdf_text_set_unicode_002);
  tcase_add_test(tc, pdf_text_set_unicode_003);
  tcase_add_test(tc, pdf_text_set_unicode_004);
  tcase_add_test(tc, pdf_text_set_unicode_005);
  tcase_add_test(tc, pdf_text_set_unicode_006);
  tcase_add_test(tc, pdf_text_set_unicode_007);
  tcase_add_test(tc, pdf_text_set_unicode_008);
  tcase_add_test(tc, pdf_text_set_unicode_009);
  tcase_add_test(tc, pdf_text_set_unicode_010);
  tcase_add_test(tc, pdf_text_set_unicode_011);
  tcase_add_test(tc, pdf_text_set_unicode_012);
  tcase_add_test(tc, pdf_text_set_unicode_013);
  tcase_add_test(tc, pdf_text_set_unicode_014);
  tcase_add_test(tc, pdf_text_set_unicode_015);
  tcase_add_test(tc, pdf_text_set_unicode_016);
  tcase_add_test(tc, pdf_text_set_unicode_017);
  tcase_add_test(tc, pdf_text_set_unicode_018);
  tcase_add_test(tc, pdf_text_set_unicode_019);
  tcase_add_test(tc, pdf_text_set_unicode_020);
  tcase_add_test(tc, pdf_text_set_unicode_021);
  tcase_add_test(tc, pdf_text_set_unicode_022);
  tcase_add_test(tc, pdf_text_set_unicode_023);
  tcase_add_test(tc, pdf_text_set_unicode_024);
  tcase_add_test(tc, pdf_text_set_unicode_025);
  tcase_add_test(tc, pdf_text_set_unicode_026);
  tcase_add_test(tc, pdf_text_set_unicode_027);
  tcase_add_test(tc, pdf_text_set_unicode_028);
  tcase_add_test(tc, pdf_text_set_unicode_029);
  tcase_add_test(tc, pdf_text_set_unicode_030);

  return tc;
}

/* End of pdf-text-set-unicode.c */

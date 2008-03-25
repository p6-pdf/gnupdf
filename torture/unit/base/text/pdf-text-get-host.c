/* -*- mode: C -*- Time-stamp: ""
 *
 *       File:         pdf-text-get-host.c
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_get_host test case
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

/*
 * Test: pdf_text_get_host_001
 * Description:
 *   Get the contents of a text object in a valid host encoding 
 * Success conditions:
 *    1. The call to  pdf_text_get_host should return PDF_OK.
 *    2. The returned string must be the expected one. 
 */
START_TEST(pdf_text_get_host_001)
{
  extern const test_string_t ascii_strings[];
  pdf_text_host_encoding_t host_enc;
  int i;

  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create, without using the API, a valid pdf_text_host_encoding_t */
  strcpy((char *)(&(host_enc.name[0])), "us-ascii");
  
  i = 0;
  while(ascii_strings[i].data != NULL)
    {
      pdf_text_t text;
      pdf_char_t *data = NULL;
      pdf_size_t size = 0;
      const pdf_char_t *expected_data;
      pdf_size_t expected_size;
      
      /* Set expected data and size */
      expected_data = (pdf_char_t *)ascii_strings[i].data;
      expected_size = ascii_strings[i].size;

      fail_if(pdf_text_new_from_unicode(&text,
                                        (pdf_char_t *) \
                                        ascii_strings[i].utf32be_data,
                                        (pdf_size_t) \
                                        ascii_strings[i].utf32be_size,
                                        PDF_TEXT_UTF32_BE) != PDF_OK);
      
      
      /* 1. The call to  pdf_text_get_host should return PDF_OK. */
      fail_unless(pdf_text_get_host(&data, &size,
                                    text,
                                    host_enc) == PDF_OK);
      
      fail_if(data == NULL);
      /* 2. The returned string must be the expected one. */
      fail_unless(size == expected_size);
      fail_unless(memcmp(expected_data, data, size) == 0);
      
      pdf_text_destroy(text);
      pdf_dealloc(data);

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
START_TEST(pdf_text_get_host_002)
{
  extern const test_string_t ascii_strings[];
  pdf_text_host_encoding_t host_enc;
  int i;
  
  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_text_init() != PDF_OK);
  
  /* Create, without using the API, an invalid pdf_text_host_encoding_t */
  strcpy((char *)(&(host_enc.name[0])), "invalid_host_encoding");
  
  i = 0;
  while(ascii_strings[i].data != NULL)
    {
      pdf_text_t text;
      pdf_char_t *data = NULL;
      pdf_size_t size = 0;
      const pdf_char_t *expected_data;
      pdf_size_t expected_size;
      
      /* Set expected data and size */
      expected_data = (pdf_char_t *)ascii_strings[i].data;
      expected_size = ascii_strings[i].size;
      
      fail_if(pdf_text_new_from_unicode(&text,
                                        (pdf_char_t *) \
                                        ascii_strings[i].utf32be_data,
                                        (pdf_size_t) \
                                        ascii_strings[i].utf32be_size,
                                        PDF_TEXT_UTF32_BE) != PDF_OK);
      
      
      /* 1. The call to  pdf_text_get_host should NOT return PDF_OK. */
      fail_unless(pdf_text_get_host(&data, &size,
                                    text,
                                    host_enc) != PDF_OK);
      
      fail_if(data != NULL);
      fail_if(size != 0);

      pdf_text_destroy(text);

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
  TCase *tc = tcase_create("pdf_text_get_host");
  tcase_add_test(tc, pdf_text_get_host_001);
  tcase_add_test(tc, pdf_text_get_host_002);  
  
  return tc;
}


/* End of pdf-text-get-host.c */

/* -*- mode: C -*-
 *
 *       File:         pdf-types-uuid-string.c
 *       Date:         Thu Apr 21 08:50:40 2011
 *
 *       GNU PDF Library - Unit tests for pdf_uuid_string
 *
 */

/* Copyright (C) 2011 Free Software Foundation, Inc. */

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

#include <check.h>
#include <string.h>
#include <pdf.h>
#include <pdf-test-common.h>
/*
 * Test: pdf_uuid_string_001
 * Description:
 *   Generate an UUID ascii representation in heap.
 * Success condition:
 *   Generated UUID ascii should be ok.
 */
START_TEST (pdf_uuid_string_001)
{

  pdf_uuid_t uuid;
  pdf_char_t *buf;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;

  /* Crate a dynamically allocated buffer */
  buf_size = 46;
  
  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);

  /* Generate a new UUID */
  uuid = pdf_uuid_generate (PDF_UUID_RANDOM);

  /* Get ascii representation of uuid */
  ret_buf = pdf_uuid_string (uuid, buf, buf_size);
  fail_if(ret_buf == NULL);

  /* Free resources */
  pdf_dealloc (buf);

}
END_TEST

/*
 * Test: pdf_uuid_string_002
 * Description:
 *   Generate an UUID ascii representation in stack.
 * Success condition:
 *   Generated UUID ascii should be ok.
 */
START_TEST (pdf_uuid_string_002)
{

  pdf_uuid_t uuid;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;

  /* Crate a statically allocated buffer */
  buf_size = 46;

  pdf_char_t buf[buf_size];

  /* Generate a new UUID */
  uuid = pdf_uuid_generate (PDF_UUID_RANDOM);

  /* Get ascii representation of uuid */
  ret_buf = pdf_uuid_string (uuid, buf, buf_size);
  fail_if(ret_buf == NULL);

}
END_TEST

/*
 * Test: pdf_uuid_string_003
 * Description:
 *   Generate an UUID ascii representation, buffer size more than
 *   required.
 * Success conditions: The buffer to store the UUID string is long
 *   enough. The conversion should pass.
 */
START_TEST (pdf_uuid_string_003)
{

  pdf_uuid_t uuid;
  pdf_char_t *buf;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;

  /* Crate the buffer */
  buf_size = 1001;

  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);

  /* Generate a new UUID */
  uuid = pdf_uuid_generate (PDF_UUID_RANDOM);

  /* Get ascii representation of uuid */
  ret_buf = pdf_uuid_string (uuid, buf, buf_size);
  fail_if(ret_buf == NULL);

  /* Free resources */
  pdf_dealloc (buf);

}
END_TEST

/*
 * Test: pdf_uuid_string_004
 * Description:
 *   Cheks for appropriate structure of UUID ascii.
 * Success conditions: The ascii structure is 
 *   00000000-0000-0000-0000-000000000000, where 0s match
 *   hexadecimal digits.
 */
START_TEST (pdf_uuid_string_004)
{

  pdf_uuid_t uuid;
  pdf_char_t *buf;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_char_t *hex_digits = "\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x41\x42\x43\x44\x45\x46\x61\x62\x63\x64\x65\x66";
  pdf_char_t *oc;
  pdf_size_t chunk_size;

  /* Crate the buffer */
  buf_size = 46;

  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);

  /* Generate a new UUID */
  uuid = pdf_uuid_generate (PDF_UUID_RANDOM);

  /* Get ascii representation of uuid */
  ret_buf = pdf_uuid_string (uuid, buf, buf_size);
  fail_if(ret_buf == NULL);

  /* Check if characters are valid hex digits */
  /* Check if hex chunks have correct size */
  /* Check if scores are in correct positions */
  
  chunk_size = strspn(buf, hex_digits);
  fail_if(chunk_size != 8);

  oc = strchr(buf, '-');
  fail_if(oc == NULL);
  fail_if(oc-buf != 8);

  chunk_size = strspn(oc+1, hex_digits);
  fail_if(chunk_size != 4);

  oc = strchr(oc+1, '-');
  fail_if(oc == NULL);
  fail_if(oc-buf != 13);

  chunk_size = strspn(oc+1, hex_digits);
  fail_if(chunk_size != 4);

  oc = strchr(oc+1, '-');
  fail_if(oc == NULL);
  fail_if(oc-buf != 18);

  chunk_size = strspn(oc+1, hex_digits);
  fail_if(chunk_size != 4);

  oc = strchr(oc+1, '-');
  fail_if(oc == NULL);
  fail_if(oc-buf != 23);

  chunk_size = strspn(oc+1, hex_digits);
  fail_if(chunk_size != 12);

  /* Free resources */
  pdf_dealloc (buf);

}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_uuid_string (void)
{
  TCase *tc = tcase_create ("pdf_uuid_string");

  tcase_add_test(tc, pdf_uuid_string_001);
  tcase_add_test(tc, pdf_uuid_string_002);
  tcase_add_test(tc, pdf_uuid_string_003);
  tcase_add_test(tc, pdf_uuid_string_004);

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);

  return tc;
}

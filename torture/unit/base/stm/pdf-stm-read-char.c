/* -*- mode: C -*- Time-stamp: "08/09/20 18:15:53 jemarch"
 *
 *       File:         pdf-stm-read-char.c
 *       Date:         Sat Sep 20 16:59:27 2008
 *
 *       GNU PDF Library - Unit tests for pdf_stm_read_char
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

#include <string.h>
#include <check.h>
#include <pdf.h>

/*
 * Test: pdf_stm_read_char_001
 * Description:
 *   Read a character from a memory stream.
 * Success condition:
 *   The read character should be ok.
 */
START_TEST (pdf_stm_read_char_001)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_u32_t char_value;
  pdf_char_t ret_char;
  pdf_size_t buf_size;

  /* Create a buffer with some contents */
  buf_size = 11;
  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);
  strcpy ((char *) buf, "0123456789");

  /* Create the stream */
  ret = pdf_stm_mem_new (buf,
                         buf_size,
                         0, /* Use the default cache size */
                         PDF_STM_READ,
                         &stm);
  fail_if(ret != PDF_OK);

  /* Read a character from the stream */
  char_value = pdf_stm_read_char (stm);

  /* Check the result */
  fail_if(char_value == PDF_EOF);

  ret_char = (pdf_char_t) char_value;
  fail_if(ret_char != '0');
  
  /* Destroy data */
  pdf_dealloc (buf);
  pdf_stm_destroy (stm);
}
END_TEST

/*
 * Test: pdf_stm_read_char_002
 * Description:
 *   Read a character from an empty stream.
 * Success condition:
 *   The read character should be PDF_EOF.
 */
START_TEST (pdf_stm_read_char_002)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_u32_t char_value;
  pdf_char_t ret_char;
  pdf_size_t buf_size;

  /* Create a buffer with some contents */
  buf_size = 2;
  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);
  buf[0] = '0';
  buf[1] = '1';

  /* Create the stream */
  ret = pdf_stm_mem_new (buf,
                         buf_size,
                         0, /* Use the default cache size */
                         PDF_STM_READ,
                         &stm);
  fail_if(ret != PDF_OK);

  /* Read a character from the stream */
  char_value = pdf_stm_read_char (stm);

  /* Check the result */
  fail_if(char_value == PDF_EOF);

  ret_char = (pdf_char_t) char_value;
  fail_if(ret_char != '0');

  /* Read a character from the stream */
  char_value = pdf_stm_read_char (stm);

  /* Check the result */
  fail_if(char_value == PDF_EOF);

  ret_char = (pdf_char_t) char_value;
  fail_if(ret_char != '1');

  /* Try to read a character from the stream */
  char_value = pdf_stm_read_char (stm);

  /* Check the result */
  fail_if(char_value != PDF_EOF);
  
  /* Destroy data */
  pdf_dealloc (buf);
  pdf_stm_destroy (stm);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_stm_read_char (void)
{
  TCase *tc = tcase_create ("pdf_stm_read_char");

  tcase_add_test(tc, pdf_stm_read_char_001);
  tcase_add_test(tc, pdf_stm_read_char_002);

  return tc;
}

/* End of pdf-stm-read-char.c */

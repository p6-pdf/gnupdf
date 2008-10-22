/* -*- mode: C -*- Time-stamp: "08/09/20 19:12:47 jemarch"
 *
 *       File:         pdf-stm-seek.c
 *       Date:         Sat Sep 20 18:50:33 2008
 *
 *       GNU PDF Library - Unit tests for pdf_stm_seek
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
 * Test: pdf_stm_seek_001
 * Description:
 *   Seek into a memory read stream.
 * Success condition:
 *   The seek operation should success.
 */
START_TEST (pdf_stm_seek_001)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_u32_t ret_char_value;
  pdf_char_t ret_char;
  pdf_size_t buf_size;
  pdf_off_t pos;

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

  /* Seek into the stream */
  pos = pdf_stm_seek (stm, 3);
  fail_if(pos != 3);

  /* Read a character */
  ret_char_value = pdf_stm_read_char (stm);
  fail_if(ret_char_value == PDF_EOF);
  ret_char = (pdf_char_t) ret_char_value;
  fail_if(ret_char != '3');

  /* Seek into the stream */
  pos = pdf_stm_seek (stm, 8);
  fail_if(pos != 8);

  /* Read a character */
  ret_char_value = pdf_stm_read_char (stm);
  fail_if(ret_char_value == PDF_EOF);
  ret_char = (pdf_char_t) ret_char_value;
  fail_if(ret_char != '8');
  
  /* Destroy data */
  pdf_dealloc (buf);
  pdf_stm_destroy (stm);
}
END_TEST

/*
 * Test: pdf_stm_seek_002
 * Description:
 *   Seek to a position beyond the size of the stream 
 *   into a memory read stream.
 * Success condition:
 *   The seek operation should success and the seek
 *   position should be to the last valid position
 *   in the stream.
 */
START_TEST (pdf_stm_seek_002)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_u32_t ret_char_value;
  pdf_char_t ret_char;
  pdf_size_t buf_size;
  pdf_off_t pos;

  /* Create a buffer with some contents */
  buf_size = 10;
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

  /* Seek into the stream */
  pos = pdf_stm_seek (stm, 300);
  fail_if(pos != 9);

  /* Read a character */
  ret_char_value = pdf_stm_read_char (stm);
  fail_if(ret_char_value == PDF_EOF);
  ret_char = (pdf_char_t) ret_char_value;
  fail_if(ret_char != '9');
  
  /* Destroy data */
  pdf_dealloc (buf);
  pdf_stm_destroy (stm);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_stm_seek (void)
{
  TCase *tc = tcase_create ("pdf_stm_seek");

  tcase_add_test(tc, pdf_stm_seek_001);
  tcase_add_test(tc, pdf_stm_seek_002);

  return tc;
}

/* End of pdf-stm-seek.c */

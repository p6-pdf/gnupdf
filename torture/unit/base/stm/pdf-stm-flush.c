/* -*- mode: C -*-
 *
 *       File:         pdf-stm-flush.c
 *       Date:         Mon Feb 02 09:53:26 2009
 *
 *       GNU PDF Library - Unit tests for pdf_stm_flush
 *
 */

/* Copyright (C) 2008, 2009-2011 Free Software Foundation, Inc. */

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
#include <pdf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pdf-test-common.h>
/*
 * Test: pdf_stm_flush_001
 * Description:
 * Test mem stream with option PDF_STM_READ.
 * Success condition:
 * 1. Function pdf_stm_flush schould return PDF_OK
 * 2. Numer of flushed bytes schould be 0.
 */
START_TEST (pdf_stm_flush_001)
{

  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_size_t buf_size;
  pdf_size_t flushed_bytes;

  /* Create a memory buffer */
  buf_size = 10;
  pdf_init();

  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);
  strcpy((char*)buf, "hello world");

  /* Create the stream */
  ret = pdf_stm_mem_new (buf,
                         buf_size,
                         0, /* Use the default cache size */
                         PDF_STM_READ,
                         &stm);
  fail_if(ret != PDF_OK);
  flushed_bytes = 0;
  ret =  pdf_stm_flush(stm, PDF_TRUE, &flushed_bytes);
  fail_unless(ret == PDF_OK);

  /* There schould be 0 flushed bytes */
  fail_if(flushed_bytes > 0);


  /* Destroy the stream */
  pdf_stm_destroy (stm);
  pdf_dealloc (buf);
}
END_TEST

/*
 * Test: pdf_stm_flush_002
 * Description:
 * Test mem stream with option PDF_STM_WRITE. Write less bytes
 * than stream cache size.
 * Success condition:
 * 1. Function pdf_stm_flush schould return PDF_OK
 * 2. Numer of flushed bytes schould be equal to written bytes.
 * 3. In output buffer schould be expected string.
 *
 */
START_TEST (pdf_stm_flush_002)
{

  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_size_t buf_size;
  pdf_size_t flushed_bytes;
  pdf_size_t written_bytes;

  /* Create a memory buffer */
  buf_size = 40;
  pdf_init();

  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);

  /* Create the stream */
  ret = pdf_stm_mem_new (buf,
                         buf_size,
                         5,
                         PDF_STM_WRITE,
                         &stm);
  fail_if(ret != PDF_OK);

  ret = pdf_stm_write(stm, "abc",3,&written_bytes);

  fail_if(ret != PDF_OK);
  fail_if(written_bytes !=3);


  ret =  pdf_stm_flush(stm, PDF_FALSE, &flushed_bytes);
  fail_unless(ret == PDF_OK);

  /* There schould be 3 bytes flushed */
  fail_if(flushed_bytes != 3);

  /* In buffer schould be expected string */
  fail_unless(memcmp(buf,"abc",3) == 0);


  /* Destroy the stream */
  pdf_stm_destroy (stm);
  pdf_dealloc (buf);
}
END_TEST

/*
 * Test: pdf_stm_flush_003
 * Description:
 * Test mem stream with option PDF_STM_WRITE.
 * Write more bytes then stream cache size, but less then
 * buffer size.
 * Success condition:
 *
 * 1. Function pdf_stm_flush schould return PDF_OK
 * 2. Numer of flushed bytes schould be equal to
 *    written bytes minus cache size.
 * 3. In output buffer schould be expected string.
 *
 */
START_TEST (pdf_stm_flush_003)
{

  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_size_t buf_size;
  pdf_size_t flushed_bytes;
  pdf_size_t written_bytes;
 // pdf_stm_filter_t tail_filter;

  /* Create a memory buffer */
  buf_size = 40;
  pdf_init();

  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);

  /* Create the stream */
  ret = pdf_stm_mem_new (buf,
                         buf_size,
                         5,
                         PDF_STM_WRITE,
                         &stm);
  fail_if(ret != PDF_OK);

  ret = pdf_stm_write(stm, "abcdefghi",9,&written_bytes);

  fail_if(ret != PDF_OK);
  fail_if(written_bytes !=9);

  /* In buffer schould be expected string */
  fail_unless(memcmp(buf,"abcde",5) == 0);

  ret =  pdf_stm_flush(stm, PDF_FALSE, &flushed_bytes);

  fail_unless(ret == PDF_OK);

  /* There schould be 4 bytes flushed */
  fail_if(flushed_bytes != 4);

  /* In buffer schould be expected string */
  fail_unless(memcmp(buf,"abcdefghi",9) == 0);


  /* Destroy the stream */
  pdf_stm_destroy (stm);
  pdf_dealloc (buf);
}
END_TEST


/*
 * Test: pdf_stm_flush_004
 * Description:
 * Test mem stream with option PDF_STM_WRITE.
 * Write to stream as much bytes as output buffer size plus
 * stream cache size, and half of null-filter input buffer.
 * Success condition:
 * 1. All Bytes schould be written.
 * 2. Function pdf_stm_flush schould return PDF_EEOF.
 * 3. Flushed bytes is 0.
 * 4. In output buffer is expected string.
 * 5.
 * 2. In stm
 * 2. Numer of flushed bytes schould be m - k.
 */
START_TEST (pdf_stm_flush_004)
{

  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_size_t buf_size;
  pdf_size_t flushed_bytes;
  pdf_size_t written_bytes;

  /* Create a memory buffer */
  buf_size = 12;

  pdf_init();

  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);

  /* Create the stream */
  ret = pdf_stm_mem_new (buf,
                         buf_size,
                         4,
                         PDF_STM_WRITE,
                         &stm);
  fail_if(ret != PDF_OK);
  ret = pdf_stm_write(stm, "111122223333444455",18,&written_bytes);
  fail_if(ret != PDF_OK);
  fail_if(written_bytes !=18);

  /* In buffer schould be expected string */
  fail_unless(memcmp(buf,"111122223333",12) == 0);

  ret =  pdf_stm_flush(stm, PDF_FALSE, &flushed_bytes);

  /* Output buffer is full and streem can't be flushed */
  fail_unless(ret == PDF_EEOF);
  fail_if(flushed_bytes>0);

  /* Erase data in buffer */
  pdf_stm_bseek(stm,0);

  ret =  pdf_stm_flush(stm, PDF_FALSE, &flushed_bytes);
  fail_unless(ret == PDF_OK);

  /* There schould be 2 bytes flushed */
  fail_if(flushed_bytes != 0);

  /* In buffer schould be expected string */
  fail_unless(memcmp(buf,"4444",4) == 0);


  /* Destroy the stream */
  pdf_stm_destroy (stm);
  pdf_dealloc (buf);
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_stm_flush (void)
{
  TCase *tc = tcase_create ("pdf_stm_flush");

  tcase_add_test(tc, pdf_stm_flush_001);
  tcase_add_test(tc, pdf_stm_flush_002);
  tcase_add_test(tc, pdf_stm_flush_003);
  tcase_add_test(tc, pdf_stm_flush_004);




  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-stm-flush.c */

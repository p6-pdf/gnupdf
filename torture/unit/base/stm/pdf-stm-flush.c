/* -*- mode: C -*-
 *
 *       File:         pdf-stm-flush.c
 *       Date:         Mon Feb 02 09:53:26 2009
 *
 *       GNU PDF Library - Unit tests for pdf_stm_flush
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

#include <check.h>
#include <pdf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pdf-test-common.h>

/*
 * Test: pdf_stm_flush_001
 * Description:
 *   Test mem stream with option PDF_STM_READ.
 * Success condition:
 *   1. Function pdf_stm_flush should return an error
 */
START_TEST (pdf_stm_flush_001)
{
  pdf_error_t *error = NULL;
  pdf_stm_t *stm;
  pdf_char_t *buf;
  pdf_size_t buf_size = 11;
  pdf_size_t flushed_bytes;
  int i;

  /* Create a buffer with some contents */
  buf = pdf_alloc (buf_size);
  fail_unless (buf != NULL);
  strcpy (buf, "0123456789");

  /* Create the stream */
  stm = pdf_stm_mem_new (buf,
                         buf_size,
                         0, /* Use the default cache size */
                         PDF_STM_READ,
                         &error);
  fail_unless (stm != NULL);
  fail_if (error != NULL);

  flushed_bytes = 0;
  fail_if (pdf_stm_flush (stm,
                          PDF_TRUE,
                          &flushed_bytes,
                          &error) == PDF_TRUE);
  fail_unless (error != NULL);

  /* Destroy the stream */
  pdf_stm_destroy (stm);
  pdf_dealloc (buf);
}
END_TEST

/*
 * Test: pdf_stm_flush_002
 * Description:
 *   Test mem stream with option PDF_STM_WRITE. Write less bytes
 *   than stream cache size.
 * Success condition:
 *   1. Function pdf_stm_flush should return PDF_OK
 *   2. Numer of flushed bytes should be equal to written bytes.
 *   3. In output buffer schould be expected string.
 *
 */
START_TEST (pdf_stm_flush_002)
{
  const pdf_char_t *text_sample = "GNU";
  pdf_error_t *error = NULL;
  pdf_stm_t *stm;
  pdf_char_t *buf;
  pdf_size_t buf_size;
  pdf_size_t flushed_bytes;
  pdf_size_t written_bytes;

  /* Create a memory buffer */
  buf_size = 40;

  buf = pdf_alloc (buf_size);
  fail_unless (buf != NULL);
  memset (buf, 0, buf_size);

  /* Create the stream */
  stm = pdf_stm_mem_new (buf,
                         buf_size,
                         5,
                         PDF_STM_WRITE,
                         &error);
  fail_unless (stm != NULL);
  fail_if (error != NULL);

  fail_unless (pdf_stm_write (stm,
                              text_sample,
                              strlen (text_sample),
                              &written_bytes,
                              &error) == PDF_TRUE);
  fail_unless (written_bytes == strlen (text_sample));

  fail_unless (pdf_stm_flush (stm,
                              PDF_FALSE,
                              &flushed_bytes,
                              &error) == PDF_TRUE);
  fail_if (error != NULL);

  /* There should be 3 bytes flushed */
  fail_unless (flushed_bytes == strlen (text_sample));

  /* In buffer should be expected string */
  fail_unless (strncmp (buf, text_sample, 3) == 0);

  /* Destroy the stream */
  pdf_stm_destroy (stm);
  pdf_dealloc (buf);
}
END_TEST

/*
 * Test: pdf_stm_flush_003
 * Description:
 *   Test mem stream with option PDF_STM_WRITE.
 *   Write more bytes then stream cache size, but less then
 *   buffer size.
 * Success condition:
 *   1. Function pdf_stm_flush should return PDF_OK
 *   2. Number of flushed bytes should be equal to
 *      written bytes minus cache size.
 *   3. In output buffer should be expected string.
 *
 */
START_TEST (pdf_stm_flush_003)
{
  const pdf_char_t *text_sample = "abcdefghi";
  pdf_error_t *error = NULL;
  pdf_stm_t *stm;
  pdf_char_t *buf;
  pdf_size_t buf_size;
  pdf_size_t flushed_bytes;
  pdf_size_t written_bytes;

  /* Create a memory buffer */
  buf_size = 40;

  buf = pdf_alloc (buf_size);
  fail_unless (buf != NULL);
  memset (buf, 0, buf_size);

  /* Create the stream */
  stm = pdf_stm_mem_new (buf,
                         buf_size,
                         5,
                         PDF_STM_WRITE,
                         &error);
  fail_unless (stm != NULL);
  fail_if (error != NULL);

  fail_unless (pdf_stm_write (stm,
                              text_sample,
                              strlen (text_sample),
                              &written_bytes,
                              &error) == PDF_TRUE);
  fail_unless (written_bytes == strlen (text_sample));

  fail_unless (pdf_stm_flush (stm,
                              PDF_FALSE,
                              &flushed_bytes,
                              &error) == PDF_TRUE);
  fail_if (error != NULL);

  /* In buffer we should have the whole string */
  fail_unless (strncmp (buf,
                        text_sample,
                        strlen (text_sample)) == 0);

  /* Destroy the stream */
  pdf_stm_destroy (stm);
  pdf_dealloc (buf);
}
END_TEST


/*
 * Test: pdf_stm_flush_004
 * Description:
 *   Test mem stream with option PDF_STM_WRITE.
 *   Write to stream as much bytes as output buffer size plus
 *   stream cache size, and half of null-filter input buffer.
 * Success condition:
 *   1. All Bytes should be written.
 *   2. Function pdf_stm_flush schould return PDF_EEOF.
 *   3. Flushed bytes is 0.
 *   4. In output buffer is expected string.
 */
START_TEST (pdf_stm_flush_004)
{
  const pdf_char_t *text_sample = "123456789012345678";
  pdf_error_t *error = NULL;
  pdf_stm_t *stm;
  pdf_char_t *buf;
  pdf_size_t buf_size = 12;
  pdf_size_t cache_size = 4;
  pdf_size_t flushed_bytes;
  pdf_size_t written_bytes;
  pdf_size_t written_bytes_2nd;

  /* Create a memory buffer */
  buf = pdf_alloc (buf_size);
  fail_unless (buf != NULL);
  memset (buf, 0, buf_size);

  /* Create the stream, internal cache size of 4 */
  stm = pdf_stm_mem_new (buf,
                         buf_size,
                         cache_size,
                         PDF_STM_WRITE,
                         &error);
  fail_unless (stm != NULL);
  fail_if (error != NULL);

  /* We try to write 18 bytes, only 12+4 can be written */
  fail_unless (pdf_stm_write (stm,
                              text_sample,
                              strlen (text_sample),
                              &written_bytes,
                              &error) != PDF_TRUE);
  fail_unless (written_bytes == (cache_size + buf_size));

  /* There should be 12 bytes in the output buffer (4 written remaining are in cache) */
  fail_unless (strncmp (buf, text_sample, 12) == 0);
  fail_unless (strncmp (buf, text_sample, 13) != 0);

  /* Output buffer is full and stream can't be flushed */
  flushed_bytes = 0;
  fail_if (pdf_stm_flush (stm,
                          PDF_FALSE,
                          &flushed_bytes,
                          &error) == PDF_TRUE);
  fail_if (flushed_bytes > 0);

  /* Erase data in buffer */
  pdf_stm_bseek (stm, 0);
  memset (buf, 0, buf_size);

  /* And flush again, passing the 4 bytes from the cache to the output buffer */
  fail_unless (pdf_stm_flush (stm,
                              PDF_FALSE,
                              &flushed_bytes,
                              &error) == PDF_TRUE);

  /* In output buffer we should have the remaining 4 bytes written */
  fail_unless (strncmp (buf, &text_sample[12], 4) == 0);
  fail_unless (strncmp (buf, &text_sample[12], 5) != 0);

  /* Write the remaining bytes (18-16=2)... */
  written_bytes_2nd = 0;
  fail_unless (pdf_stm_write (stm,
                              &text_sample[written_bytes],
                              strlen (text_sample) - written_bytes,
                              &written_bytes_2nd,
                              &error) == PDF_TRUE);
  fail_unless (written_bytes_2nd == (strlen (text_sample) - written_bytes));

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

  tcase_add_test (tc, pdf_stm_flush_001);
  tcase_add_test (tc, pdf_stm_flush_002);
  tcase_add_test (tc, pdf_stm_flush_003);
  tcase_add_test (tc, pdf_stm_flush_004);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-stm-flush.c */

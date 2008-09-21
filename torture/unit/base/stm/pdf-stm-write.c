/* -*- mode: C -*- Time-stamp: "08/09/21 19:19:12 jemarch"
 *
 *       File:         pdf-stm-write.c
 *       Date:         Sun Sep 21 16:37:27 2008
 *
 *       GNU PDF Library - Unit tests for pdf_stm_write
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
 * Test: pdf_stm_write_001
 * Description:
 *   Write some bytes to a write memory stream.
 * Success condition:
 *   The written data should be consistent.
 */
START_TEST (pdf_stm_write_001)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_size_t buf_size;
  pdf_size_t written_bytes;

  /* Create a buffer */
  buf_size = 10;
  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);

  /* Create the stream */
  ret = pdf_stm_mem_new (buf,
                         buf_size,
                         0, /* Use the default cache size */
                         PDF_STM_WRITE,
                         &stm);
  fail_if(ret != PDF_OK);

  /* Write some data into the stream */
  written_bytes = pdf_stm_write (stm,
                                 "GNU",
                                 3);
  fail_if(written_bytes != 3);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if(strncmp (buf, "GNU", 3) != 0);

  /* Destroy data */
  pdf_dealloc (buf);
}
END_TEST

/*
 * Test: pdf_stm_write_002
 * Description:
 *   Write some bytes to a write memory stream with
 *   two null filter installed.
 * Success condition:
 *   The written data should be consistent.
 */
START_TEST (pdf_stm_write_002)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_size_t buf_size;
  pdf_size_t written_bytes;
  pdf_hash_t null_filter_params;

  /* Create a buffer */
  buf_size = 10;
  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);

  /* Create the stream */
  ret = pdf_stm_mem_new (buf,
                         buf_size,
                         0, /* Use the default cache size */
                         PDF_STM_WRITE,
                         &stm);
  fail_if(ret != PDF_OK);

  /* Install a new filter in the chain */
  pdf_hash_new (NULL, &null_filter_params);
  fail_if(pdf_stm_install_filter (stm,
                                  PDF_STM_FILTER_NULL,
                                  null_filter_params) != PDF_OK);

  /* Write some data into the stream */
  written_bytes = pdf_stm_write (stm,
                                 "GNU",
                                 3);
  fail_if(written_bytes != 3);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if(strncmp (buf, "GNU", 3) != 0);

  /* Destroy data */
  pdf_dealloc (buf);
  pdf_hash_destroy (null_filter_params);
}
END_TEST

/*
 * Test: pdf_stm_write_003
 * Description:
 *   Write some bytes to a write memory stream
 *   producing a cache flush.
 * Success condition:
 *   The written data should be consistent.
 */
START_TEST (pdf_stm_write_003)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_size_t buf_size;
  pdf_size_t written_bytes;

  /* Create a buffer */
  buf_size = 10;
  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);

  /* Create the stream */
  ret = pdf_stm_mem_new (buf,
                         buf_size,
                         2,
                         PDF_STM_WRITE,
                         &stm);
  fail_if(ret != PDF_OK);

  /* Write some data into the stream */
  written_bytes = pdf_stm_write (stm,
                                 "GNU",
                                 3);
  fail_if(written_bytes != 3);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if(strncmp (buf, "GNU", 3) != 0);
  printf("XXX buf[0] = %c\n", buf[0]);
  printf("XXX buf[1] = %c\n", buf[1]);
  printf("XXX buf[2] = %c\n", buf[2]);

  /* Destroy data */
  pdf_dealloc (buf);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_stm_write (void)
{
  TCase *tc = tcase_create ("pdf_stm_write");

  tcase_add_test(tc, pdf_stm_write_001);
  tcase_add_test(tc, pdf_stm_write_002);
  tcase_add_test(tc, pdf_stm_write_003);

  return tc;
}

/* End of pdf-stm-write.c */

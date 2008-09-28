/* -*- mode: C -*- Time-stamp: "08/09/28 13:53:56 jemarch"
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

  /* Destroy data */
  pdf_dealloc (buf);
}
END_TEST

/*
 * Test: pdf_stm_write_004
 * Description:
 *   Write some bytes to a write memory stream with an
 *   ASCII Hex encoder.
 * Success condition:
 *   The written data should be consistent.
 */
START_TEST (pdf_stm_write_004)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_size_t buf_size;
  pdf_size_t written_bytes;
  pdf_hash_t ahexenc_filter_parameters;

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

  /* Install an ASCII Hex encoder in the stream */
  ret = pdf_hash_new (NULL, &ahexenc_filter_parameters);
  fail_if(ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
                                PDF_STM_FILTER_AHEX_ENC,
                                ahexenc_filter_parameters);
  fail_if(ret != PDF_OK);

  /* Write some data into the stream */
  written_bytes = pdf_stm_write (stm,
                                 "abc",
                                 3);
  fail_if(written_bytes != 3);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if(strncmp (buf, "616263>", 7) != 0);


  /* Destroy data */
  pdf_hash_destroy (ahexenc_filter_parameters);
  pdf_dealloc (buf);
}
END_TEST


/*
 * Test: pdf_stm_write_005
 * Description:
 *   Create a memory-based writing stream and attach a RunLength filter
 *   encoder to it.
 * Success condition:
 *   The encoded data should be correct.
 */
START_TEST (pdf_stm_write_005)
{
  pdf_status_t ret;
  pdf_hash_t params;
  pdf_stm_t stm;
  pdf_char_t *buf, *decoded="122333444455555666666777777788888888999999999";
  pdf_size_t buf_size, total=46, written;
  pdf_char_t *dataux, *encoded =
    "\x00" "1" "\xff" "2" "\xfe" "3" "\xfd" "4" "\xfc" "5" "\xfb" "6" \
    "\xfa" "7" "\xf9" "8" "\xf8" "9" "\x00" "\x00" "\x80";

  /* Writing stream */
  /* Create a memory buffer */
  buf_size = 100;
  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);
  /* Create the stream */
  ret = pdf_stm_mem_new (buf,
                         buf_size,
                         5, /* Minimum with EOD, to restore filter's state */
                         PDF_STM_WRITE,
                         &stm);
  fail_if(ret != PDF_OK);
  /* Create the filter */
  fail_if (pdf_stm_install_filter (stm, PDF_STM_FILTER_RL_ENC, params) !=
           PDF_OK);

  written=0;
  dataux = decoded;
  while (total > 0)
    {
      written = pdf_stm_write (stm, dataux, total);
      dataux = dataux + written;
      total -= written;
    }
  pdf_stm_flush(stm);
  pdf_stm_finish(stm);

  fail_if (memcmp (buf, encoded, 21) != 0);
  /* Destroy the stream */
  pdf_stm_destroy (stm);
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
  tcase_add_test(tc, pdf_stm_write_004);
  tcase_add_test(tc, pdf_stm_write_005);
  
  return tc;
}

/* End of pdf-stm-write.c */

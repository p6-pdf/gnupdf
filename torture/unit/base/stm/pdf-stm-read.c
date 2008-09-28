/* -*- mode: C -*- Time-stamp: "2008-09-28 20:17:40 gerel"
 *
 *       File:         pdf-stm-read.c
 *       Date:         Sat Sep 20 15:20:17 2008
 *
 *       GNU PDF Library - Unit tests for pdf_stm_read
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
 * Test: pdf_stm_read_001
 * Description:
 *   Read some bytes from a reading memory buffer.
 * Success condition:
 *   The readed data should be ok.
 */
START_TEST (pdf_stm_read_001)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;

  /* Create a buffer with some contents */
  buf_size = 11;
  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);
  strcpy ((char *) buf, "0123456789");

  /* Create the return buffer */
  ret_buf = pdf_alloc (buf_size);
  fail_if(ret_buf == NULL);

  /* Create the stream */
  ret = pdf_stm_mem_new (buf,
                         buf_size,
                         0, /* Use the default cache size */
                         PDF_STM_READ,
                         &stm);
  fail_if(ret != PDF_OK);

  /* Read some data from the stream */
  read_bytes = pdf_stm_read (stm,
                             ret_buf,
                             buf_size);
  fail_if(read_bytes == 0);
  fail_if(read_bytes != buf_size);

  /* Check for the result */
  fail_if(strcmp("0123456789", (char *) ret_buf) != 0);
  
  /* Destroy data */
  pdf_dealloc (buf);
  pdf_dealloc (ret_buf);
  pdf_stm_destroy (stm);
}
END_TEST

/*
 * Test: pdf_stm_read_002
 * Description:
 *   Read bytes from a reading memory stream getting an
 *   EOF condition in the returned value.
 * Success condition:
 *   The readed data should be ok and we should get an
 *   EOF condition in the returned value.
 */
START_TEST (pdf_stm_read_002)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;

  /* Create a buffer with some contents */
  buf_size = 11;
  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);
  strcpy ((char *) buf, "0123456789");

  /* Create the return buffer */
  ret_buf = pdf_alloc (buf_size * 2);
  fail_if(ret_buf == NULL);

  /* Create the stream */
  ret = pdf_stm_mem_new (buf,
                         buf_size,
                         0, /* Use the default cache size */
                         PDF_STM_READ,
                         &stm);
  fail_if(ret != PDF_OK);

  /* Read some data from the stream */
  read_bytes = pdf_stm_read (stm,
                             ret_buf,
                             (buf_size * 2));
  fail_if(read_bytes == 0);
  fail_if(read_bytes != buf_size);

  /* Check for the result */
  fail_if(strcmp("0123456789", (char *) ret_buf) != 0);
  
  /* Destroy data */
  pdf_dealloc (buf);
  pdf_dealloc (ret_buf);
  pdf_stm_destroy (stm);
}
END_TEST

/*
 * Test: pdf_stm_read_003
 * Description:
 *   Read bytes from a reading memory stream in
 *   several steps.
 * Success condition:
 *   The readed data should be ok;
 */
START_TEST (pdf_stm_read_003)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;

  /* Create a buffer with some contents */
  buf_size = 11;
  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);
  strcpy ((char *) buf, "0123456789");

  /* Create the return buffer */
  ret_buf = pdf_alloc (2);
  fail_if(ret_buf == NULL);

  /* Create the stream */
  ret = pdf_stm_mem_new (buf,
                         buf_size,
                         0, /* Use the default cache size */
                         PDF_STM_READ,
                         &stm);
  fail_if(ret != PDF_OK);

  /* Read 2 octects from the stream */
  read_bytes = pdf_stm_read (stm,
                             ret_buf,
                             2);
  fail_if(read_bytes != 2);

  /* Check for the result */
  fail_if(ret_buf[0] != '0' && ret_buf[1] != '1');

  /* Read 2 octects from the stream */
  read_bytes = pdf_stm_read (stm,
                             ret_buf,
                             2);
  fail_if(read_bytes != 2);

  /* Check for the result */
  fail_if(ret_buf[0] != '2' && ret_buf[1] != '3');

  /* Read 2 octects from the stream */
  read_bytes = pdf_stm_read (stm,
                             ret_buf,
                             2);
  fail_if(read_bytes != 2);

  /* Check for the result */
  fail_if(ret_buf[0] != '4' && ret_buf[1] != '5');

  /* Read 2 octects from the stream */
  read_bytes = pdf_stm_read (stm,
                             ret_buf,
                             2);
  fail_if(read_bytes != 2);

  /* Check for the result */
  fail_if(ret_buf[0] != '6' && ret_buf[1] != '7');

  /* Read 2 octects from the stream */
  read_bytes = pdf_stm_read (stm,
                             ret_buf,
                             2);
  fail_if(read_bytes != 2);

  /* Check for the result */
  fail_if(ret_buf[0] != '8' && ret_buf[1] != '9');

  /* Read 1 octect from the stream */
  read_bytes = pdf_stm_read (stm,
                             ret_buf,
                             1);
  fail_if(read_bytes != 1);

  /* Check for the result */
  fail_if(ret_buf[0] != 0);
  
  /* Destroy data */
  pdf_dealloc (buf);
  pdf_dealloc (ret_buf);
  pdf_stm_destroy (stm);
}
END_TEST

/*
 * Test: pdf_stm_read_004
 * Description:
 *   Read some bytes from a reading memory cache with two
 *   null filters installed
 * Success condition:
 *   The readed data should be ok.
 */
START_TEST (pdf_stm_read_004)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t null_filter_params;

  /* Create a buffer with some contents */
  buf_size = 11;
  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);
  strcpy ((char *) buf, "0123456789");

  /* Create the return buffer */
  ret_buf = pdf_alloc (buf_size);
  fail_if(ret_buf == NULL);

  /* Create the stream */
  ret = pdf_stm_mem_new (buf,
                         buf_size,
                         0, /* Use the default cache size */
                         PDF_STM_READ,
                         &stm);
  fail_if(ret != PDF_OK);

  /* Install a new filter in the chain */
  pdf_hash_new (NULL, &null_filter_params);
  fail_if(pdf_stm_install_filter (stm,
                                  PDF_STM_FILTER_NULL,
                                  null_filter_params) != PDF_OK);

  /* Read some data from the stream */
  read_bytes = pdf_stm_read (stm,
                             ret_buf,
                             buf_size);
  fail_if(read_bytes == 0);
  fail_if(read_bytes != buf_size);

  /* Check for the result */
  fail_if(strcmp("0123456789", (char *) ret_buf) != 0);
  
  /* Destroy data */
  pdf_dealloc (buf);
  pdf_dealloc (ret_buf);
  pdf_hash_destroy (null_filter_params);
  pdf_stm_destroy (stm);
}
END_TEST

/*
 * Test: pdf_stm_read_005
 * Description:
 *   Create a memory-based reading stream and attach a RunLength filter
 *   decoder to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_read_005)
{
  pdf_status_t ret;
  pdf_hash_t params;
  pdf_stm_t stm;
  pdf_char_t *buf, *decoded="122333444455555666666777777788888888999999999";
  pdf_size_t buf_size, total=46,read;
  pdf_char_t *dataux, *encoded =
    "\x00" "1" "\xff" "2" "\xfe" "3" "\xfd" "4" "\xfc" "5" "\xfb" "6" \
    "\xfa" "7" "\xf9" "8" "\xf8" "9" "\x80" "\x00" "\x00" "\x80";

  /* Writing stream */
  /* Create a memory buffer */
  buf_size = 100;
  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);
  /* Create the stream */
  ret = pdf_stm_mem_new (encoded,
                         22,
                         1, /* Minimum, to restore filter's state */
                         PDF_STM_READ,
                         &stm);
  fail_if(ret != PDF_OK);
  /* Create the filter */
  fail_if (pdf_stm_install_filter (stm, PDF_STM_FILTER_RL_DEC, params) !=
           PDF_OK);

  read = 0;
  total = 46;
  dataux = buf;
  while (total > 0)
    {
      read = pdf_stm_read (stm, dataux, total);
      dataux = dataux + read;
      total -= read;
    }

  fail_if (memcmp (buf, decoded, 46) != 0);
  /* Destroy the stream */
  pdf_stm_destroy (stm);
  pdf_dealloc (buf);


}
END_TEST

/*
 * Test: pdf_stm_read_006
 * Description:
 *   Read some bytes from a read memory stream with an
 *   ASCII Hex decoder.
 * Success condition:
 *   The read data should be consistent.
 */
START_TEST (pdf_stm_read_006)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t ahexdec_filter_parameters;

  /* Create the buffers */
  buf_size = 10;
  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);
  strcpy (buf, "616263>");

  ret_buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);

  /* Create the stream */
  ret = pdf_stm_mem_new (buf,
                         buf_size,
                         0, /* Use the default cache size */
                         PDF_STM_READ,
                         &stm);
  fail_if(ret != PDF_OK);

  /* Install an ASCII Hex decoder in the stream */
  ret = pdf_hash_new (NULL, &ahexdec_filter_parameters);
  fail_if(ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
                                PDF_STM_FILTER_AHEX_DEC,
                                ahexdec_filter_parameters);
  fail_if(ret != PDF_OK);

  /* Read some data from the stream */
  read_bytes = pdf_stm_read (stm,
                             ret_buf,
                             3);
  fail_if(read_bytes != 3);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if(strncmp (ret_buf, "abc", 3) != 0);


  /* Destroy data */
  pdf_hash_destroy (ahexdec_filter_parameters);
  pdf_dealloc (buf);
  pdf_dealloc (ret_buf);
}
END_TEST

/*
 * Test: pdf_stm_read_007
 * Description:
 *   Read some bytes from a read memory stream with an
 *   ASCII Hex decoder having white characters in the data.
 * Success condition:
 *   The read data should be consistent.
 */
START_TEST (pdf_stm_read_007)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t ahexdec_filter_parameters;

  /* Create the buffers */
  buf_size = 20;
  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);
  strcpy (buf, "61 6\n2 63\n>");

  ret_buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);

  /* Create the stream */
  ret = pdf_stm_mem_new (buf,
                         buf_size,
                         0, /* Use the default cache size */
                         PDF_STM_READ,
                         &stm);
  fail_if(ret != PDF_OK);

  /* Install an ASCII Hex decoder in the stream */
  ret = pdf_hash_new (NULL, &ahexdec_filter_parameters);
  fail_if(ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
                                PDF_STM_FILTER_AHEX_DEC,
                                ahexdec_filter_parameters);
  fail_if(ret != PDF_OK);

  /* Read some data from the stream */
  read_bytes = pdf_stm_read (stm,
                             ret_buf,
                             3);
  fail_if(read_bytes != 3);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if(strncmp (ret_buf, "abc", 3) != 0);


  /* Destroy data */
  pdf_hash_destroy (ahexdec_filter_parameters);
  pdf_dealloc (buf);
  pdf_dealloc (ret_buf);
}
END_TEST

/*
 * Test: pdf_stm_read_008
 * Description:
 *   Read some bytes from a read memory stream with an
 *   ASCII Hex decoder having an even number of hex digits
 *   in the input.
 * Success condition:
 *   The read data should be consistent.
 */
START_TEST (pdf_stm_read_008)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t ahexdec_filter_parameters;

  /* Create the buffers */
  buf_size = 20;
  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);
  strcpy (buf, "61626>");

  ret_buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);

  /* Create the stream */
  ret = pdf_stm_mem_new (buf,
                         buf_size,
                         0, /* Use the default cache size */
                         PDF_STM_READ,
                         &stm);
  fail_if(ret != PDF_OK);

  /* Install an ASCII Hex decoder in the stream */
  ret = pdf_hash_new (NULL, &ahexdec_filter_parameters);
  fail_if(ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
                                PDF_STM_FILTER_AHEX_DEC,
                                ahexdec_filter_parameters);
  fail_if(ret != PDF_OK);

  /* Read some data from the stream */
  read_bytes = pdf_stm_read (stm,
                             ret_buf,
                             3);
  fail_if(read_bytes != 3);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if(strncmp (ret_buf, "ab`", 3) != 0);


  /* Destroy data */
  pdf_hash_destroy (ahexdec_filter_parameters);
  pdf_dealloc (buf);
  pdf_dealloc (ret_buf);
}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_stm_read (void)
{
  TCase *tc = tcase_create ("pdf_stm_read");

  tcase_add_test(tc, pdf_stm_read_001);
  tcase_add_test(tc, pdf_stm_read_002);
  tcase_add_test(tc, pdf_stm_read_003);
  tcase_add_test(tc, pdf_stm_read_004);
  tcase_add_test(tc, pdf_stm_read_005);
  tcase_add_test(tc, pdf_stm_read_006);
  tcase_add_test(tc, pdf_stm_read_007);
  tcase_add_test(tc, pdf_stm_read_008);

  return tc;
}

/* End of pdf-stm-read.c */

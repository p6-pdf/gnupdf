/* -*- mode: C -*-
 *
 *       File:         pdf-stm-read.c
 *       Date:         Sat Sep 20 15:20:17 2008
 *
 *       GNU PDF Library - Unit tests for pdf_stm_read
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

#include <string.h>
#include <check.h>
#include <pdf.h>
#include <pdf-test-common.h>
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
  strcpy (buf, "0123456789");

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
  ret = pdf_stm_read (stm,
                      ret_buf,
                      buf_size,
                      &read_bytes);
  fail_if(ret != PDF_OK);
  fail_if(read_bytes != buf_size);

  /* Check for the result */
  fail_if(strcmp("0123456789", ret_buf) != 0);

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
  strcpy (buf, "0123456789");

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
  ret = pdf_stm_read (stm,
                      ret_buf,
                      (buf_size * 2),
                      &read_bytes);
  fail_if(ret != PDF_EEOF);
  fail_if(read_bytes == 0);
  fail_if(read_bytes != buf_size);

  /* Check for the result */
  fail_if(strcmp("0123456789", ret_buf) != 0);

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
  strcpy (buf, "0123456789");

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
  ret = pdf_stm_read (stm,
                      ret_buf,
                      2,
                      &read_bytes);
  fail_if(ret != PDF_OK);
  fail_if(read_bytes != 2);

  /* Check for the result */
  fail_if(ret_buf[0] != '0' && ret_buf[1] != '1');

  /* Read 2 octects from the stream */
  ret = pdf_stm_read (stm,
                      ret_buf,
                      2,
                      &read_bytes);
  fail_if(ret != PDF_OK);
  fail_if(read_bytes != 2);

  /* Check for the result */
  fail_if(ret_buf[0] != '2' && ret_buf[1] != '3');

  /* Read 2 octects from the stream */
  ret = pdf_stm_read (stm,
                      ret_buf,
                      2,
                      &read_bytes);
  fail_if(ret != PDF_OK);
  fail_if(read_bytes != 2);

  /* Check for the result */
  fail_if(ret_buf[0] != '4' && ret_buf[1] != '5');

  /* Read 2 octects from the stream */
  ret = pdf_stm_read (stm,
                      ret_buf,
                      2,
                      &read_bytes);
  fail_if(ret != PDF_OK);
  fail_if(read_bytes != 2);

  /* Check for the result */
  fail_if(ret_buf[0] != '6' && ret_buf[1] != '7');

  /* Read 2 octects from the stream */
  ret = pdf_stm_read (stm,
                      ret_buf,
                      2,
                      &read_bytes);
  fail_if(ret != PDF_OK);
  fail_if(read_bytes != 2);

  /* Check for the result */
  fail_if(ret_buf[0] != '8' && ret_buf[1] != '9');

  /* Read 1 octect from the stream */
  ret = pdf_stm_read (stm,
                      ret_buf,
                      1,
                      &read_bytes);
  fail_if(ret != PDF_OK);
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
  pdf_hash_t *null_filter_params;

  /* Create a buffer with some contents */
  buf_size = 11;

  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);
  strcpy (buf, "0123456789");

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
  null_filter_params = pdf_hash_new (NULL);
  fail_if(pdf_stm_install_filter (stm,
                                  PDF_STM_FILTER_NULL,
                                  null_filter_params) != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm,
                      ret_buf,
                      buf_size,
                      &read_bytes);
  fail_if(ret != PDF_OK);
  fail_if(read_bytes == 0);
  fail_if(read_bytes != buf_size);

  /* Check for the result */
  fail_if(strcmp("0123456789", ret_buf) != 0);

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
  pdf_hash_t *params;
  pdf_stm_t stm;
  pdf_char_t *buf, *decoded="122333444455555666666777777788888888999999999";
  pdf_size_t buf_size, total=46,read;
  pdf_char_t *dataux, *encoded =
    "\x00" "1" "\xff" "2" "\xfe" "3" "\xfd" "4" "\xfc" "5" "\xfb" "6" \
    "\xfa" "7" "\xf9" "8" "\xf8" "9" "\x00" "\x00" "\x80";

  /* Writing stream */
  /* Create a memory buffer */
  buf_size = 100;

  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);
  /* Create the stream */
  ret = pdf_stm_mem_new (encoded,
                         21,
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
      ret = pdf_stm_read (stm, dataux, total, &read);
      fail_if(ret == PDF_ERROR);
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
  pdf_hash_t *ahexdec_filter_parameters;

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
  ahexdec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
                                PDF_STM_FILTER_AHEX_DEC,
                                ahexdec_filter_parameters);
  fail_if(ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm,
                      ret_buf,
                      3,
                      &read_bytes);
  fail_if(ret != PDF_OK);
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
  pdf_hash_t *ahexdec_filter_parameters;

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
  ahexdec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
                                PDF_STM_FILTER_AHEX_DEC,
                                ahexdec_filter_parameters);
  fail_if(ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm,
                      ret_buf,
                      3,
                      &read_bytes);
  fail_if(ret != PDF_OK);
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
  pdf_hash_t *ahexdec_filter_parameters;

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
  ahexdec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
                                PDF_STM_FILTER_AHEX_DEC,
                                ahexdec_filter_parameters);
  fail_if(ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm,
                      ret_buf,
                      3,
                      &read_bytes);
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


/* Note: PDF_STM_FILTER_FLATE_DEC is only available if LIBZ is used */
#if defined HAVE_LIBZ

/*
 * Test: pdf_stm_read_009
 * Description:
 *   Create a memory-based reading stream and attach a flate filter
 *   decoder to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_read_009)
{
  pdf_status_t ret;
  pdf_hash_t *params;
  pdf_stm_t stm;
  pdf_size_t buf_size, total=1059,read;
  pdf_char_t *buf, *decoded=
    "1223334444555556666667777777888888889999999990" \
    "1223334444555556666667777777888888889999999990" \
    "1223334444555556666667777777888888889999999990" \
    "1223334444555556666667777777888888889999999990" \
    "1223334444555556666667777777888888889999999990" \
    "1223334444555556666667777777888888889999999990" \
    "1223334444555556666667777777888888889999999990" \
    "1223334444555556666667777777888888889999999990" \
    "1223334444555556666667777777888888889999999990" \
    "1223334444555556666667777777888888889999999990" \
    "1223334444555556666667777777888888889999999990" \
    "1223334444555556666667777777888888889999999990" \
    "1223334444555556666667777777888888889999999990" \
    "1223334444555556666667777777888888889999999990" \
    "1223334444555556666667777777888888889999999990" \
    "1223334444555556666667777777888888889999999990" \
    "1223334444555556666667777777888888889999999990" \
    "1223334444555556666667777777888888889999999990" \
    "1223334444555556666667777777888888889999999990" \
    "1223334444555556666667777777888888889999999990" \
    "1223334444555556666667777777888888889999999990" \
    "1223334444555556666667777777888888889999999990" \
    "1223334444555556666667777777888888889999999990";
  pdf_char_t *dataux, *encoded =
    "\x78\x9c\x33\x34\x32\x32\x36\x36\x36\x01\x02\x53\x10\x30\x03\x03"
    "\x73\x08\xb0\x80\x02\x4b\x18\x30\x30\x1c\x55\x3d\xaa\x7a\x54\xf5"
    "\x88\x52\xcd\x00\x00\xe1\x0b\xdf\xfc";

  /* Writing stream */
  /* Create a memory buffer */
  buf_size = 2000;

  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);
  /* Create the stream */
  ret = pdf_stm_mem_new (encoded,
                         41,
                         1,
                         PDF_STM_READ,
                         &stm);
  fail_if(ret != PDF_OK);
  /* Create the filter */
  fail_if (pdf_stm_install_filter (stm, PDF_STM_FILTER_FLATE_DEC, params) !=
           PDF_OK);

  read = 0;
  total = 1059;
  dataux = buf;
  while (total > 0)
    {
      ret = pdf_stm_read (stm, dataux, total, &read);
      fail_if(ret == PDF_ERROR);
      dataux = dataux + read;
      total -= read;
    }

  fail_if (memcmp (buf, decoded, 1059) != 0);
  /* Destroy the stream */
  pdf_stm_destroy (stm);
  pdf_dealloc (buf);
}
END_TEST

#endif

/*
 * Test: pdf_stm_read_011
 * Description:
 *   Create a memory-based writing stream and attach an V2 cipher filter
 *   decode to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_read_011)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_hash_t *params;

  pdf_char_t out[14];

  pdf_size_t out_size = sizeof(out);
  pdf_char_t in[] =
    {
      0x45, 0xA0, 0x1F, 0x64, 0x5F, 0xC3, 0x5B,
      0x38, 0x35, 0x52, 0x54, 0x4B, 0x9B, 0xF5
    };
  pdf_size_t in_size = sizeof(in);
  pdf_char_t key[6] = "Secret"; /* not trailing '\0' */
  pdf_size_t keysize = sizeof(key);
  pdf_char_t plaintext[] = "Attack at dawn";
  pdf_size_t read;


  fail_if ( pdf_stm_mem_new (in, in_size, 0, PDF_STM_READ, &stm) != PDF_OK);

  params = pdf_hash_new (NULL);
  pdf_hash_add (params, "Key", key, NULL, NULL);
  pdf_hash_add_size (params, "KeySize", keysize, NULL);

  fail_if ( pdf_stm_install_filter (stm, PDF_STM_FILTER_V2_DEC, params) != PDF_OK);

  ret = pdf_stm_read (stm, out, out_size, &read);
  fail_if (ret == PDF_ERROR);
  fail_if (read != out_size);

  fail_if (memcmp (out, plaintext, read) != 0);

  pdf_hash_destroy (params);
  pdf_stm_destroy (stm);
}
END_TEST



/*
 * Test: pdf_stm_read_012
 * Description:
 *   Create a memory-based reading stream and attach an AESV2 filter
 *   decoder to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_read_012)
{
  pdf_stm_t stm;
  pdf_hash_t *params;
  pdf_char_t out[48];
  pdf_size_t out_size;
  pdf_status_t ret;

  pdf_char_t key[] =
    {
      0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
      0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
    };

  pdf_size_t keysize = sizeof(key);

  pdf_char_t plain[] =
    {
      0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, /* iv */
      0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, /* content */
      0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
      0x0f,
      /* padding */
      /* 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
         0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f */
    };

  pdf_char_t ciphered[] =
    {
      0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, /* iv vector */
      0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
      0xbb, 0x03, 0x13, 0xee, 0x2f, 0x65, 0x43, 0xa9, /* content */
      0x04, 0xf2, 0x8e, 0xff, 0x68, 0x59, 0x05, 0x35,
      0x81, 0x7e, 0x24, 0x33, 0x5f, 0x18, 0x72, 0x12, /* padding */
      0x8c, 0xb8, 0x4c, 0x23, 0x59, 0xd0, 0xe0, 0x36,
    };


  params = pdf_hash_new (NULL);
  pdf_hash_add (params, "Key", key, NULL, NULL);
  pdf_hash_add_size (params, "KeySize", keysize, NULL);

  fail_if ( pdf_stm_mem_new (ciphered, sizeof(ciphered), 0, PDF_STM_READ, &stm) != PDF_OK);
  fail_if ( pdf_stm_install_filter (stm, PDF_STM_FILTER_AESV2_DEC, params) != PDF_OK);

  ret = pdf_stm_read (stm, out , sizeof(out), &out_size);
  fail_if(ret == PDF_ERROR);

  fail_if (out_size != sizeof(plain));
  fail_if (memcmp (out, plain, out_size) != 0);

  pdf_hash_destroy (params);
  pdf_stm_destroy (stm);
}
END_TEST



/*
 * Test: pdf_stm_read_013
 * Description:
 *   Read some bytes from a read file stream.
 * Success condition:
 *   The read data should be consistent.
 */
START_TEST (pdf_stm_read_013)
{
  pdf_error_t *error = NULL;
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_size_t written,read_bytes;

  pdf_fsys_file_t file;
  char data[4];
  pdf_text_t *path;
  pdf_char_t * remain;
  pdf_size_t remain_length;

  /* Create the file path */
  path = pdf_text_new_from_pdf_string ("tmp.test", 8, &remain, &remain_length, &error);
  fail_unless (path != NULL);
  fail_if (error != NULL);

  /* Open new file */
  ret = pdf_fsys_file_open (NULL, path, PDF_FSYS_OPEN_MODE_WRITE, &file);
  fail_if (ret != PDF_OK);

  ret = pdf_fsys_file_write (file, "GNU", 3, &written);
  fail_if (ret != PDF_OK);
  fail_if (written != 3);
  pdf_fsys_file_close (file);

  ret = pdf_fsys_file_open (NULL, path, PDF_FSYS_OPEN_MODE_READ, &file);
  fail_if (ret != PDF_OK);
  /* Create the stream */
  ret = pdf_stm_file_new (file,
                          0,
                          0, /* Use the default cache size */
                          PDF_STM_READ,
                          &stm);
  fail_if(ret != PDF_OK);

  ret = pdf_stm_read (stm,
                      data,
                      3,
                      &read_bytes);
  fail_if(ret != PDF_OK);
  fail_if(read_bytes != 3);
  data[3] = '\0';

  fail_if (strcmp (data, "GNU") != 0);

  /* Free resources */
  pdf_stm_destroy (stm);
  pdf_fsys_file_close (file);
  pdf_text_destroy (path);
}
END_TEST


/*
 * Test: pdf_stm_read_014
 * Description:
 *   Read some bytes from a read file stream with null filter.
 * Success condition:
 *   The read data should be consistent.
 */
START_TEST (pdf_stm_read_014)
{
  pdf_error_t *error = NULL;
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_size_t written,read_bytes;

  pdf_fsys_file_t file;
  char data[4];
  pdf_text_t *path;
  pdf_char_t * remain;
  pdf_size_t remain_length;
  pdf_hash_t *null_filter_params;

  /* Create the file path */
  path = pdf_text_new_from_pdf_string ("tmp.test", 8, &remain, &remain_length, &error);
  fail_unless (path != NULL);
  fail_if (error != NULL);

  /* Open new file */
  ret = pdf_fsys_file_open (NULL, path, PDF_FSYS_OPEN_MODE_WRITE, &file);
  fail_if (ret != PDF_OK);

  ret = pdf_fsys_file_write (file, "GNU", 3, &written);
  fail_if (ret != PDF_OK);
  fail_if (written != 3);
  pdf_fsys_file_close (file);

  ret = pdf_fsys_file_open (NULL, path, PDF_FSYS_OPEN_MODE_READ, &file);
  fail_if (ret != PDF_OK);
  /* Create the stream */
  ret = pdf_stm_file_new (file,
                          0,
                          0, /* Use the default cache size */
                          PDF_STM_READ,
                          &stm);
  fail_if(ret != PDF_OK);


  /* Install a new filter in the chain */
  null_filter_params = pdf_hash_new (NULL);
  fail_if(pdf_stm_install_filter (stm,
                                  PDF_STM_FILTER_NULL,
                                  null_filter_params) != PDF_OK);

  ret = pdf_stm_read (stm,
                      data,
                      3,
                      &read_bytes);
  fail_if(ret != PDF_OK);
  fail_if(read_bytes != 3);
  data[3] = '\0';

  fail_if (strcmp (data, "GNU") != 0);

  /* Free resources */
  pdf_stm_destroy (stm);
  pdf_fsys_file_close (file);
  pdf_text_destroy (path);
}
END_TEST

/*
 * Test: pdf_stm_read_015
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_read_015)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47~>";


  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded,
                         18,
                         0, /* Use the default cache size */
                         PDF_STM_READ,
                         &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
                                PDF_STM_FILTER_A85_DEC,
                                a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm,
                      ret_buf,
                      16,
                      &read_bytes);
  fail_if (read_bytes != 16);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 16) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_016
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_read_016)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded = "\x00";
  pdf_char_t *encoded =   "!!~>";


  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded,
                         4,
                         0, /* Use the default cache size */
                         PDF_STM_READ,
                         &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
                                PDF_STM_FILTER_A85_DEC,
                                a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm,
                      ret_buf,
                      1,
                      &read_bytes);
  fail_if (read_bytes != 1);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 1) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST

/*
 * Test: pdf_stm_read_017
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_read_017)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded = "\x00\x00";
  pdf_char_t *encoded = "!!!~>";


  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded,
                         5,
                         0, /* Use the default cache size */
                         PDF_STM_READ,
                         &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
                                PDF_STM_FILTER_A85_DEC,
                                a85dec_filter_parameters);
  fail_if(ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm,
                      ret_buf,
                      2,
                      &read_bytes);
  fail_if (read_bytes != 2);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 2) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST

/*
 * The following is the spec from the gnupdf site for the ASCII85 filter
 * I have placed within this text requirement identifiers in the form **(#n)**
 * to identify requirements coverage in the test cases.
 *
 * From gnupdf.org:
 *
 * The ASCII85Decode filter decodes data that has been encoded in ASCII
 * base-85 encoding and produces binary data. The following paragraphs
 * describe the process for encoding binary data in ASCII base-85; the
 * ASCII85Decode filter reverses this process.
 *
 * The ASCII base-85 encoding uses the characters ! through u and the
 * character z, with the 2-character sequence ~> as its EOD marker. The
 * ASCII85Decode filter ignores all white-space characters **(#1)**. Any other
 * characters **(#2)**, and any character sequences that represent impossible
 * combinations in the ASCII base-85 encoding **(#3)**, cause an error.
 *
 * Specifically, ASCII base-85 encoding produces 5 ASCII characters for
 * every 4 bytes of binary data. Each group of 4binary input bytes,
 * `(b1 b2 b3 b4)`, is converted to a group of 5 output bytes,
 * `(c1 c2 c3 c4 c5)`, using the relation:
 *
 * (b1 * 256^3) + (b2 * 256^2) + (b3 * 256^1) + b4 =
 *
 * (c1 * 85^4) + (c2 * 85^3) + (c3 * 85^2) + (c4 * 85^1) + c5
 *
 * In other words, 4 bytes of binary data are interpreted as a base-256
 * number and then converted to a base-85 number. The five bytes of the
 * base-85 number are then converted to ASCII characters by adding 33
 * (the ASCII code for the character !) to each. The resulting encoded
 * data contains only printable ASCII characters with codes in the
 * range 33 (!) to 117 (u). As a special case, if all five bytes are 0,
 * they are represented by the character with code 122 (z) **(#4)** instead of
 * by five exclamation points (!!!!!).
 *
 * If the length of the binary data to be encoded is not a multiple of
 * 4 bytes, the last, partial group of 4 is used to produce a last,
 * partial group of 5 output  characters. Given n (1, 2 or 3) bytes of
 * binary data, the encoder first appends 4 - n zero bytes to make a
 * complete group of 4. It then encodes this group in the usual way,
 * but without applying the special z case **(#5)**. Finally, it writes only the
 * first n + 1 characters of the resulting group of 5 **(#6)**. These characters
 * are immediately followed by the ~> EOD marker.
 *
 * The following conditions (which never occur in a correctly encoded
 * byte sequence) cause errors during decoding:
 *
 *  * The value represented by a group of 5 characters is >= 2^32 **(#7)**.
 *  * A z character occurs in the middle of a group. **(#8)**
 *  * A final partial group contains only one character. (**(#9)**
 *
 */

/*
 * Test: pdf_stm_read_018
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_read_018)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded = "\x00\x00\x00";
  pdf_char_t *encoded = "!!!!~>";


  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded,
                         6,
                         0, /* Use the default cache size */
                         PDF_STM_READ,
                         &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
                                PDF_STM_FILTER_A85_DEC,
                                a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm,
                      ret_buf,
                      3,
                      &read_bytes);
  fail_if (read_bytes != 3);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 3) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST

/*
 * Test: pdf_stm_read_019
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_read_019)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded = "\x00\x00\x00\x00";
  pdf_char_t *encoded = "z~>";


  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded,
                         3,
                         0, /* Use the default cache size */
                         PDF_STM_READ,
                         &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
                                PDF_STM_FILTER_A85_DEC,
                                a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm,
                      ret_buf,
                      4,
                      &read_bytes);
  fail_if (read_bytes != 4);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 4) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST

/*
 * Test: pdf_stm_read_020
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition: Requirement **(#4)**
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_read_020)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded = "\x00\x00\x00\x00";
  pdf_char_t *encoded = "z~>";


  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 3, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 4, &read_bytes);
  fail_if (read_bytes != 4);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 4) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_021
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it - test requirement **(#1)** - ignore whitespace.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_read_021)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded = "\x00\x00\x00\x00";
  pdf_char_t *encoded = " z\t~\n>       ";


  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 13, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 4, &read_bytes);
  fail_if (read_bytes != 4);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 4) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_022
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition: Requirement **(#6)**
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_read_022)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded = "\x00";
  pdf_char_t *encoded = "!!~>";


  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 4, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 1, &read_bytes);
  fail_if (read_bytes != 1);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 1) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_023
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_read_023)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded = "\x00\x00";
  pdf_char_t *encoded = "!!!~>";


  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 5, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 2, &read_bytes);
  fail_if (read_bytes != 2);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 2) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_024
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it. Requirement **(#5)** **(#6)**
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_read_024)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded = "\x00\x00\x00";
  pdf_char_t *encoded = "!!!!~>";


  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 6, 0,	/* Use the default cache size */
                         PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 3, &read_bytes);
  fail_if (read_bytes != 3);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 3) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_025
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_read_025)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47~>";


  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);
  fail_if (read_bytes != 16);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 16) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_026
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_read_026)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47\x21\x2D~>";


  /* Create the buffers */
  buf_size = 30;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 20, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 17, &read_bytes);
  fail_if (read_bytes != 17);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 17) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_027
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_read_027)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "\t!!\x2A\x2D\x27z\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47~>\t\n\n\n";


  /* Create the buffers */
  buf_size = 30;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 23, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);
  fail_if (read_bytes != 16);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 16) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_028
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_read_028)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x22\x39\n\n\n\x65\x75\x37\x23\x52\x4C\x68\x47\n~\n>";


  /* Create the buffers */
  buf_size = 30;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 23, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);
  fail_if (read_bytes != 16);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 16) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_029
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_read_029)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47\x21\n\x2D\n~\n>";


  /* Create the buffers */
  buf_size = 30;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 23, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 17, &read_bytes);
  fail_if (read_bytes != 17);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 17) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_030
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_read_030)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47\x21\x45\x3A~>";


  /* Create the buffers */
  buf_size = 30;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 21, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 18, &read_bytes);
  fail_if (read_bytes != 18);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 18) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_031
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   Requirement **(#8)**
 *   The decoding should fail.
 */
START_TEST (pdf_stm_read_031)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2Dz\x27\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47~>";
  /* Note that ^ this z is in an invalid position */

  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_032
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoding should fail.
 */
START_TEST (pdf_stm_read_032)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2Az\x2D\x27\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47~>";
  /* Note  ^ this z is in an invalid position */

  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_033
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoding should fail.
 */
START_TEST (pdf_stm_read_033)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!z\x2A\x2D\x27\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47~>";
  /*   ^ Note: preceeding z is in an invalid position */

  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_034
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoding should fail.
 */
START_TEST (pdf_stm_read_034)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!z!\x2A\x2D\x27\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47~>";
  /*  ^ Note that preceeding z is in an invalid position */

  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_035
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it. Requirement **(#9)**
 * Success condition:
 *   The decoding should fail.
 */
START_TEST (pdf_stm_read_035)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47\x48~>";
  /* Note that terminating group is only one byte */

  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_036
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it. Requirement **(#7)**
 * Success condition:
 *   The decoding should fail.
 */
START_TEST (pdf_stm_read_036)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x73\x75\x75\x75\x75\x23\x52\x4C\x68\x47~>";
  /*  Note that Third group is above 2^32 limit */

  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_037
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoding should fail.
 */
START_TEST (pdf_stm_read_037)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x74\x21\x21\x21\x21\x23\x52\x4C\x68\x47~>";
  /*  Note that Thirnd group is above 2^32 limit */

  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_038
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it. Requirement **(#7)**
 * Success condition:
 *   The decoding should fail.
 */
START_TEST (pdf_stm_read_038)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x73\x38\x57\x2D\x22\x23\x52\x4C\x68\x47~>";
  /*  Note that Third group equals 2^32 limit */

  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_039
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoding should fail.
 */
START_TEST (pdf_stm_read_039)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x73\x38\x57\x2D\x23\x23\x52\x4C\x68\x47~>";
  /*  Note that Third group is above 2^32 limit */

  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_040
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoding should fail.
 */
START_TEST (pdf_stm_read_040)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x73\x38\x57\x2E\x21\x23\x52\x4C\x68\x47~>";
  /*  Note that Third group is above 2^32 limit */

  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_041
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoding should fail.
 */
START_TEST (pdf_stm_read_041)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x73\x38\x58\x2D\x21\x23\x52\x4C\x68\x47~>";
  /*  Note that Third group is above 2^32 limit */

  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_042
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoding should fail.
 */
START_TEST (pdf_stm_read_042)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x73\x39\x57\x2D\x21\x23\x52\x4C\x68\x47~>";
  /*  Note that Third group is above 2^32 limit */

  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_043
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   Requirement **(#2)** - invalid character
 *   The decoding should fail.
 */
START_TEST (pdf_stm_read_043)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x63\x39\x57\x2D\x21\x23\x52\x4C\x68\x76~>";
  /*  Note that Last character is out of allowed range */

  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_read_044
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   Requirement **(#3)** - invalid character
 *   The decoding should fail.
 */
START_TEST (pdf_stm_read_044)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t *a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x63\x39\x57\x2D\x21\x23\x52\x4C\x68\x00~>";
  /*  Note that Last character is out of allowed range */

  /* Create the buffers */
  buf_size = 20;

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  a85dec_filter_parameters = pdf_hash_new (NULL);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
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
/* Note: PDF_STM_FILTER_FLATE_DEC is only available if LIBZ is used */
#if defined HAVE_LIBZ
  tcase_add_test(tc, pdf_stm_read_009);
#endif
  tcase_add_test(tc, pdf_stm_read_011);
  tcase_add_test(tc, pdf_stm_read_012);
  tcase_add_test(tc, pdf_stm_read_013);
  tcase_add_test(tc, pdf_stm_read_014);
  tcase_add_test(tc, pdf_stm_read_015);
  tcase_add_test(tc, pdf_stm_read_016);
  tcase_add_test(tc, pdf_stm_read_017);
  tcase_add_test(tc, pdf_stm_read_018);
  tcase_add_test(tc, pdf_stm_read_019);
  tcase_add_test(tc, pdf_stm_read_020);
  tcase_add_test(tc, pdf_stm_read_021);
  tcase_add_test(tc, pdf_stm_read_022);
  tcase_add_test(tc, pdf_stm_read_023);
  tcase_add_test(tc, pdf_stm_read_024);
  tcase_add_test(tc, pdf_stm_read_025);
  tcase_add_test(tc, pdf_stm_read_026);
  tcase_add_test(tc, pdf_stm_read_027);
  tcase_add_test(tc, pdf_stm_read_028);
  tcase_add_test(tc, pdf_stm_read_029);
  tcase_add_test(tc, pdf_stm_read_030);
  tcase_add_test(tc, pdf_stm_read_031);
  tcase_add_test(tc, pdf_stm_read_032);
  tcase_add_test(tc, pdf_stm_read_033);
  tcase_add_test(tc, pdf_stm_read_034);
  tcase_add_test(tc, pdf_stm_read_035);
  tcase_add_test(tc, pdf_stm_read_036);
  tcase_add_test(tc, pdf_stm_read_037);
  tcase_add_test(tc, pdf_stm_read_038);
  tcase_add_test(tc, pdf_stm_read_039);
  tcase_add_test(tc, pdf_stm_read_040);
  tcase_add_test(tc, pdf_stm_read_041);
  tcase_add_test(tc, pdf_stm_read_042);
  tcase_add_test(tc, pdf_stm_read_043);
  tcase_add_test(tc, pdf_stm_read_044);

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-stm-read.c */

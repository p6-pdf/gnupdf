/* -*- mode: C -*- Time-stamp: "2009-05-15 13:53:39 gerel"
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
  ret = pdf_stm_write (stm,
                       "GNU",
                       3,
                       &written_bytes);
  fail_if(ret != PDF_OK);
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
  ret = pdf_stm_write (stm,
                       "GNU",
                       3,
                       &written_bytes);
  fail_if(ret != PDF_OK);
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
  ret = pdf_stm_write (stm,
                       "GNU",
                       3,
                       &written_bytes);
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
  ret = pdf_stm_write (stm,
                       "abc",
                       3,
                       &written_bytes);
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
  pdf_size_t flushed_bytes;
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
                         1, /* Minimum with EOD, to restore filter's state */
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
      ret = pdf_stm_write (stm, dataux, total, &written);
      fail_if(ret == PDF_ERROR);
      dataux = dataux + written;
      total -= written;
    }
  fail_if(pdf_stm_flush(stm, PDF_TRUE, &flushed_bytes) == PDF_ERROR);

  fail_if (memcmp (buf, encoded, 21) != 0);
  /* Destroy the stream */
  pdf_stm_destroy (stm);
  pdf_dealloc (buf);

}
END_TEST

/*
 * Test: pdf_stm_write_006
 * Description:
 *   Write some bytes to a write memory stream with an
 *   ASCII Hex encoder, generating new lines.
 * Success condition:
 *   The encoded data should be consistent.
 */
START_TEST (pdf_stm_write_006)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_size_t buf_size;
  pdf_size_t written_bytes;
  pdf_hash_t ahexenc_filter_parameters;

  /* Create a buffer */
  buf_size = 100;
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
  ret = pdf_stm_write (stm,
                       "aaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaa",
                       45,
                       &written_bytes);
  fail_if(ret != PDF_OK);
  fail_if(written_bytes != 45);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if(strncmp (buf,
                   "616161616162626262626161616161626262626261616161616262626262\n616161616162626262626161616161>",
                   91) != 0);


  /* Destroy data */
  pdf_hash_destroy (ahexenc_filter_parameters);
  pdf_dealloc (buf);
}
END_TEST


/* Note: PDF_STM_FILTER_FLATE_DEC is only available if LIBZ is used */
#if defined(HAVE_LIBZ)

/*
 * Test: pdf_stm_write_007
 * Description:
 *   Create a memory-based writing stream and attach a flate filter
 *   encoder to it.
 * Success condition:
 *   The encoded data should be correct.
 */
START_TEST (pdf_stm_write_007)
{
  pdf_status_t ret;
  pdf_hash_t params;
  pdf_stm_t stm;
  pdf_size_t buf_size, total,written;
  pdf_size_t flushed_bytes;
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
  buf_size = 100;
  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);
  /* Create the stream */
  ret = pdf_stm_mem_new (buf,
                         buf_size,
                         1,
                         PDF_STM_WRITE,
                         &stm);
  fail_if(ret != PDF_OK);
  /* Create the filter */
  fail_if (pdf_stm_install_filter (stm, PDF_STM_FILTER_FLATE_ENC, params) !=
           PDF_OK);

  written = 0;
  total = 1059;
  dataux = decoded;
  while (total > 0)
    {
      ret = pdf_stm_write (stm, dataux, total, &written);
      fail_if(ret == PDF_ERROR);
      dataux = dataux + written;
      total -= written;
    }
  fail_if(pdf_stm_flush (stm, PDF_TRUE, &flushed_bytes) == PDF_ERROR);

  fail_if (memcmp (buf, encoded, 41) != 0);
  /* Destroy the stream */
  pdf_stm_destroy (stm);
  pdf_dealloc (buf);

}
END_TEST

#endif



/*
 * Test: pdf_stm_write_008
 * Description:
 *   Create a memory-based writing stream and attach an V2 filter without parameters.
 * Success condition:
 *   The installation of the filter should fail.
 */
START_TEST (pdf_stm_write_008)
{
  pdf_stm_t stm;
  pdf_hash_t params;
  pdf_char_t buffer[16];
  pdf_status_t ret;
  
  pdf_crypt_init();

  ret = pdf_stm_mem_new (buffer, sizeof(buffer), 0, PDF_STM_WRITE, &stm);
  fail_if (ret != PDF_OK);

  pdf_hash_new (NULL, &params);
  
  ret = pdf_stm_install_filter (stm, PDF_STM_FILTER_V2_ENC, params);
  fail_if (ret == PDF_OK);

  pdf_hash_destroy (params);
  pdf_stm_destroy (stm);
}
END_TEST



/*
 * Test: pdf_stm_write_009
 * Description:
 *   Create a memory-based writing stream and attach an V2 filter
 *   encoder to it.
 * Success condition:
 *   The encoded data should be correct.
 */
START_TEST (pdf_stm_write_009)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_hash_t params;

  pdf_size_t flushed_bytes;
  pdf_char_t out[14];
  pdf_size_t out_size = sizeof(out);
  pdf_char_t in[14] = "Attack at dawn"; /* not trailing '\0' */
  pdf_size_t in_size = sizeof(in);
  pdf_char_t key[6] = "Secret"; /* not trailing '\0' */
  pdf_size_t keysize = sizeof(key);
  
  pdf_char_t ciphered[] = {
    0x45, 0xA0, 0x1F, 0x64, 0x5F, 0xC3, 0x5B,
    0x38, 0x35, 0x52, 0x54, 0x4B, 0x9B, 0xF5
  };

  pdf_size_t written;
  
  pdf_crypt_init();

  fail_if ( pdf_stm_mem_new (out, out_size, 0, PDF_STM_WRITE, &stm) != PDF_OK);

  pdf_hash_new (NULL, &params);
  pdf_hash_add (params, "Key", key, NULL);
  pdf_hash_add (params, "KeySize", &keysize, NULL);

  fail_if ( pdf_stm_install_filter (stm, PDF_STM_FILTER_V2_ENC, params) != PDF_OK);

  ret = pdf_stm_write (stm, in, in_size, &written);
  fail_if (written != out_size);

  fail_if(pdf_stm_flush (stm, PDF_TRUE, &flushed_bytes) == PDF_ERROR);

  fail_if (memcmp (out, ciphered, written) != 0);

  pdf_hash_destroy (params);
  pdf_stm_destroy (stm);
}
END_TEST



/*
 * Test: pdf_stm_write_010
 * Description:
 *   Create a memory-based writing stream and attach an AESV2 filter
 *   encoder to it.
 * Success condition:
 *   The encoded data should be correct.
 */
START_TEST (pdf_stm_write_010)
{
  pdf_status_t ret;
  pdf_size_t written;
  pdf_stm_t stm;
  pdf_hash_t params;
  pdf_size_t flushed_bytes;
  pdf_char_t out[96];
  pdf_char_t key[16] =
    {
      0x56, 0xe4, 0x7a, 0x38,
      0xc5, 0x59, 0x89, 0x74,
      0xbc, 0x46, 0x90, 0x3d,
      0xba, 0x29, 0x03, 0x49
    };
  pdf_size_t keysize = sizeof(key);
  pdf_char_t plain[] =
    {
      0x8c, 0xe8, 0x2e, 0xef, 0xbe, 0xa0, 0xda, 0x3c, /* iv vector */
      0x44, 0x69, 0x9e, 0xd7, 0xdb, 0x51, 0xb7, 0xd9,

      0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
      0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
      0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
      0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
      0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
      0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
      0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
      0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf
    };

  pdf_char_t ciphered[] =
    {
      0x8c, 0xe8, 0x2e, 0xef, 0xbe, 0xa0, 0xda, 0x3c, /* iv vector */
      0x44, 0x69, 0x9e, 0xd7, 0xdb, 0x51, 0xb7, 0xd9,
      
      0xc3, 0x0e, 0x32, 0xff, 0xed, 0xc0, 0x77, 0x4e,
      0x6a, 0xff, 0x6a, 0xf0, 0x86, 0x9f, 0x71, 0xaa,
      0x0f, 0x3a, 0xf0, 0x7a, 0x9a, 0x31, 0xa9, 0xc6,
      0x84, 0xdb, 0x20, 0x7e, 0xb0, 0xef, 0x8e, 0x4e, 
      0x35, 0x90, 0x7a, 0xa6, 0x32, 0xc3, 0xff, 0xdf,
      0x86, 0x8b, 0xb7, 0xb2, 0x9d, 0x3d, 0x46, 0xad, 
      0x83, 0xce, 0x9f, 0x9a, 0x10, 0x2e, 0xe9, 0x9d,
      0x49, 0xa5, 0x3e, 0x87, 0xf4, 0xc3, 0xda, 0x55,
    };

  pdf_crypt_init();

  pdf_hash_new (NULL, &params);
  pdf_hash_add (params, "Key", key, NULL);
  pdf_hash_add (params, "KeySize", &keysize, NULL);

  fail_if ( pdf_stm_mem_new (out, sizeof(out), 0, PDF_STM_WRITE, &stm) != PDF_OK);
  fail_if ( pdf_stm_install_filter (stm, PDF_STM_FILTER_AESV2_ENC, params) != PDF_OK);

  ret = pdf_stm_write (stm, plain + 00 , 16, &written);
  fail_if(ret == PDF_ERROR);
  ret = pdf_stm_write (stm, plain + 16 , 16, &written);
  fail_if(ret == PDF_ERROR);
  ret = pdf_stm_write (stm, plain + 32 , 16, &written);
  fail_if(ret == PDF_ERROR);
  ret = pdf_stm_write (stm, plain + 48 , 16, &written);
  fail_if(ret == PDF_ERROR);
  ret = pdf_stm_write (stm, plain + 64 , 16, &written);
  fail_if(ret == PDF_ERROR);

  fail_if(pdf_stm_flush (stm, PDF_TRUE, &flushed_bytes) == PDF_ERROR);

  fail_if (memcmp (out, ciphered, sizeof(ciphered)) != 0);

  pdf_hash_destroy (params);
  pdf_stm_destroy (stm);
}
END_TEST



/*
 * Test: pdf_stm_write_011
 * Description:
 *   Create a memory-based writing stream and attach an MD5
 *   encoder to it.
 * Success condition:
 *   The output data should be correct.
 */
START_TEST (pdf_stm_write_011)
{
  pdf_status_t ret;
  pdf_size_t written;
  pdf_stm_t stm;
  pdf_hash_t params;
  pdf_size_t flushed_bytes;
  pdf_char_t in[26] = "abcdefghijklmnopqrstuvwxyz";
  pdf_char_t out[16];
  pdf_crypt_md_t md;
  pdf_char_t real_out[] = {
    0xC3, 0xFC, 0xD3, 0xD7,
    0x61, 0x92, 0xE4, 0x00,
    0x7D, 0xFB, 0x49, 0x6C,
    0xCA, 0x67, 0xE1, 0x3B
  };

  pdf_crypt_init();

  pdf_hash_new (NULL, &params);

  fail_if ( pdf_stm_mem_new (out, sizeof(out), 0, PDF_STM_WRITE, &stm) != PDF_OK);
  fail_if ( pdf_stm_install_filter (stm, PDF_STM_FILTER_MD5_ENC, params) != PDF_OK);

  ret = pdf_stm_write (stm, in, sizeof(in), &written);
  fail_if(ret == PDF_ERROR);

  fail_if(pdf_stm_flush (stm, PDF_TRUE, &flushed_bytes) == PDF_ERROR);

  fail_if (memcmp (out, real_out, sizeof(out)) != 0);

  pdf_hash_destroy (params);
  pdf_stm_destroy (stm);
}
END_TEST



/*
 * Test: pdf_stm_write_012
 * Description:
 *   Create a memory-based writing stream and attach an AESV2 filter
 *   decoder to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_write_012)
{
  pdf_status_t ret;
  pdf_size_t written;
  pdf_stm_t stm;
  pdf_hash_t params;
  pdf_size_t flushed_bytes;
  pdf_char_t out[48];
  
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

  pdf_crypt_init();

  pdf_hash_new (NULL, &params);
  pdf_hash_add (params, "Key", key, NULL);
  pdf_hash_add (params, "KeySize", &keysize, NULL);

  fail_if ( pdf_stm_mem_new (out, sizeof(out), 0, PDF_STM_WRITE, &stm) != PDF_OK);
  fail_if ( pdf_stm_install_filter (stm, PDF_STM_FILTER_AESV2_ENC, params) != PDF_OK);

  ret = pdf_stm_write (stm, plain , sizeof(plain), &written);
  fail_if(ret == PDF_ERROR);
  fail_if(pdf_stm_flush (stm, PDF_TRUE, &flushed_bytes) == PDF_ERROR);

  fail_if (memcmp (out, ciphered, sizeof(out)) != 0);

  pdf_hash_destroy (params);
  pdf_stm_destroy (stm);
}
END_TEST



/*
 * Test: pdf_stm_write_013
 * Description:
 *   Write some bytes to a write file stream.
 * Success condition:
 *   The written data should be consistent.
 */
START_TEST (pdf_stm_write_013)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_size_t read,written_bytes;

  pdf_fsys_file_t file;
  char data[4];
  pdf_text_t path;
  pdf_char_t * remain;
  pdf_size_t remain_length;

  /* Create the file path */
  pdf_text_init ();
  ret = pdf_text_new_from_pdf_string ("tmp.test", 8, &remain, &remain_length, &path);
  fail_if (ret != PDF_OK);

  /* Open new file */
  ret = pdf_fsys_file_open (NULL, path, PDF_FSYS_OPEN_MODE_WRITE, &file); 
  fail_if (ret != PDF_OK);

  /* Create the stream */
  ret = pdf_stm_file_new (file,
                          0,
                          0, /* Use the default cache size */
                          PDF_STM_WRITE,
                          &stm);
  fail_if(ret != PDF_OK);

  /* Write some data into the stream */
  ret = pdf_stm_write (stm,
                       "GNU",
                       3,
                       &written_bytes);
  fail_if(ret != PDF_OK);
  fail_if(written_bytes != 3);

  /* Destroy data */
  pdf_stm_destroy (stm);
  pdf_fsys_file_close (file);

  /* Read file data and compare */
  ret = pdf_fsys_file_open (NULL, path, PDF_FSYS_OPEN_MODE_READ, &file); 
  fail_if (ret != PDF_OK);

  ret = pdf_fsys_file_read (file, data, 3, &read);
  fail_if (ret != PDF_OK);
  data[3] = '\0';

  fail_if(read != 3);
  fail_if (strcmp (data, "GNU") != 0);

  /* Free resources */
  pdf_fsys_file_close (file);
  pdf_text_destroy (path);
}
END_TEST



/*
 * Test: pdf_stm_write_014
 * Description:
 *   Write some bytes to a write file stream with
 *   a null filter installed.
 * Success condition:
 *   The written data should be consistent.
 */
START_TEST (pdf_stm_write_014)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_size_t read,written_bytes;
  pdf_hash_t null_filter_params;

  pdf_fsys_file_t file;
  char data[4];
  pdf_text_t path;
  pdf_char_t * remain;
  pdf_size_t remain_length;

  /* Create the file path */
  pdf_text_init ();
  ret = pdf_text_new_from_pdf_string ("tmp.test", 8, &remain, &remain_length, &path);
  fail_if (ret != PDF_OK);

  /* Open new file */
  ret = pdf_fsys_file_open (NULL, path, PDF_FSYS_OPEN_MODE_WRITE, &file); 
  fail_if (ret != PDF_OK);

  /* Create the stream */
  ret = pdf_stm_file_new (file,
                          0,
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
  ret = pdf_stm_write (stm,
                       "GNU",
                       3,
                       &written_bytes);
  fail_if(ret != PDF_OK);
  fail_if(written_bytes != 3);

  /* Destroy data */
  pdf_hash_destroy (null_filter_params);
  pdf_stm_destroy (stm);
  pdf_fsys_file_close (file);

  /* Read file data and compare */
  ret = pdf_fsys_file_open (NULL, path, PDF_FSYS_OPEN_MODE_READ, &file); 
  fail_if (ret != PDF_OK);

  ret = pdf_fsys_file_read (file, data, 3, &read);
  fail_if (ret != PDF_OK);
  data[3] = '\0';

  fail_if(read != 3);
  fail_if (strcmp (data, "GNU") != 0);

  /* Free resources */
  pdf_fsys_file_close (file);
  pdf_text_destroy (path);
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
  tcase_add_test(tc, pdf_stm_write_006);
/* Note: PDF_STM_FILTER_FLATE_DEC is only available if LIBZ is used */
#if defined(HAVE_LIBZ)
  tcase_add_test(tc, pdf_stm_write_007);
#endif
  tcase_add_test(tc, pdf_stm_write_008);
  tcase_add_test(tc, pdf_stm_write_009);
  tcase_add_test(tc, pdf_stm_write_010);
  tcase_add_test(tc, pdf_stm_write_011);
  tcase_add_test(tc, pdf_stm_write_012);
  tcase_add_test(tc, pdf_stm_write_013);
  tcase_add_test(tc, pdf_stm_write_014);

  return tc;
}

/* End of pdf-stm-write.c */

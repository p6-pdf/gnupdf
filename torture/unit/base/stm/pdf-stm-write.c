/* -*- mode: C -*- Time-stamp: "2008-12-23 17:33:28 davazp"
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
      written = pdf_stm_write (stm, dataux, total);
      dataux = dataux + written;
      total -= written;
    }
  pdf_stm_flush(stm, PDF_TRUE);

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
  written_bytes = pdf_stm_write (stm,
                                 "aaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaa",
                                 45);
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
      written = pdf_stm_write (stm, dataux, total);
      dataux = dataux + written;
      total -= written;
    }
  pdf_stm_flush (stm, PDF_TRUE);

  fail_if (memcmp (buf, encoded, 41) != 0);
  /* Destroy the stream */
  pdf_stm_destroy (stm);
  pdf_dealloc (buf);

}
END_TEST





/* 
 * /\*
 *  * Test: pdf_stm_write_008
 *  * Description:
 *  *   Create a memory-based writing stream and attach an V2 filter without parameters.
 *  * Success condition:
 *  *   The installation of the filter should fail.
 *  *\/
 * START_TEST (pdf_stm_write_008)
 * {
 *   pdf_stm_t stm;
 *   pdf_hash_t params;
 *   pdf_char_t buffer[16];
 *   pdf_status_t ret;
 *   
 *   pdf_crypt_init();
 * 
 *   ret = pdf_stm_mem_new (buffer, sizeof(buffer), 0, PDF_STM_WRITE, &stm);
 *   fail_if (ret != PDF_OK);
 * 
 *   pdf_hash_new (NULL, &params);
 *   
 *   ret = pdf_stm_install_filter (stm, PDF_STM_FILTER_V2_ENC, params);
 *   fail_if (ret == PDF_OK);
 * 
 *   pdf_hash_destroy (params);
 *   pdf_stm_destroy (stm);
 * }
 * END_TEST
 * 
 * 
 * 
 * /\*
 *  * Test: pdf_stm_write_009
 *  * Description:
 *  *   Create a memory-based writing stream and attach an V2 filter
 *  *   encoder to it.
 *  * Success condition:
 *  *   The encoded data should be correct.
 *  *\/
 * START_TEST (pdf_stm_write_009)
 * {
 *   pdf_stm_t stm;
 *   pdf_hash_t params;
 * 
 *   pdf_char_t out[14];
 *   pdf_size_t out_size = sizeof(out);
 *   pdf_char_t in[14] = "Attack at dawn"; /\* not trailing '\0' *\/
 *   pdf_size_t in_size = sizeof(in);
 *   pdf_char_t key[6] = "Secret"; /\* not trailing '\0' *\/
 *   pdf_size_t keysize = sizeof(key);
 *   
 *   pdf_char_t ciphered[] = {
 *     0x45, 0xA0, 0x1F, 0x64, 0x5F, 0xC3, 0x5B,
 *     0x38, 0x35, 0x52, 0x54, 0x4B, 0x9B, 0xF5
 *   };
 * 
 *   pdf_char_t written;
 *   
 *   pdf_crypt_init();
 * 
 *   fail_if ( pdf_stm_mem_new (out, out_size, 0, PDF_STM_WRITE, &stm) != PDF_OK);
 * 
 *   pdf_hash_new (NULL, &params);
 *   pdf_hash_add (params, "Key", key, NULL);
 *   pdf_hash_add (params, "KeySize", &keysize, NULL);
 * 
 *   fail_if ( pdf_stm_install_filter (stm, PDF_STM_FILTER_V2_ENC, params) != PDF_OK);
 * 
 *   written = pdf_stm_write (stm, in, in_size);
 *   fail_if (written != out_size);
 * 
 *   pdf_stm_flush (stm, PDF_TRUE);
 * 
 *   fail_if (memcmp (out, ciphered, written) != 0);
 * 
 *   pdf_hash_destroy (params);
 *   pdf_stm_destroy (stm);
 * }
 * END_TEST
 * 
 * 
 * 
 * /\*
 *  * Test: pdf_stm_write_010
 *  * Description:
 *  *   Create a memory-based writing stream and attach an AESV2 filter
 *  *   encoder to it.
 *  * Success condition:
 *  *   The encoded data should be correct.
 *  *\/
 * START_TEST (pdf_stm_write_010)
 * {
 * }
 * END_TEST
 * 
 * 
 * 
 * /\*
 *  * Test: pdf_stm_write_011
 *  * Description:
 *  *   Create a memory-based writing stream and attach an MD5 filter
 *  *   encoder to it.
 *  * Success condition:
 *  *   The output data should be correct.
 *  *\/
 * START_TEST (pdf_stm_write_011)
 * {
 *   pdf_stm_t stm;
 *   pdf_hash_t params;
 *   pdf_char_t in[26] = "abcdefghijklmnopqrstuvwxyz";
 *   pdf_char_t out[16];
 *   pdf_crypt_md_t md;
 *   pdf_char_t real_out[] = {
 *     0xC3, 0xFC, 0xD3, 0xD7,
 *     0x61, 0x92, 0xE4, 0x00,
 *     0x7D, 0xFB, 0x49, 0x6C,
 *     0xCA, 0x67, 0xE1, 0x3B
 *   };
 * 
 *   pdf_crypt_init();
 * 
 *   pdf_hash_new (NULL, &params);
 * 
 *   fail_if ( pdf_stm_mem_new (out, sizeof(out), 0, PDF_STM_WRITE, &stm) != PDF_OK);
 *   fail_if ( pdf_stm_install_filter (stm, PDF_STM_FILTER_MD5_ENC, params) != PDF_OK);
 * 
 *   pdf_stm_write (stm, in, sizeof(in));
 *   pdf_stm_flush (stm, PDF_TRUE);
 * 
 *   fail_if (memcmp (out, real_out, sizeof(out)) != 0);
 * 
 *   pdf_hash_destroy (params);
 *   pdf_stm_destroy (stm);
 * }
 * END_TEST
 */



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
  tcase_add_test(tc, pdf_stm_write_007);
  /* 
   * tcase_add_test(tc, pdf_stm_write_008);
   * tcase_add_test(tc, pdf_stm_write_009);
   * tcase_add_test(tc, pdf_stm_write_010);
   * tcase_add_test(tc, pdf_stm_write_011);
   */

  return tc;
}

/* End of pdf-stm-write.c */

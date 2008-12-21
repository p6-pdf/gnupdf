/* -*- mode: C -*- Time-stamp: "2008-12-21 14:50:55 davazp"
 *
 *       File:         pdf-crypt-md-hash.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_crypt_md_hash
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
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pdf.h>
#include <check.h>


/*
 * Test: pdf_crypt_md_hash_001
 * Description:
 *   Compute the md5 of an empty buffer.
 * Success condition:
 *   Returns PDF_OK and output buffer matches.
 */
START_TEST (pdf_crypt_md_hash_001)
{
  pdf_char_t *in;
  pdf_char_t out[16];
  pdf_crypt_md_t md;
  pdf_char_t real_out[] = {
    0xd4, 0x1d, 0x8c, 0xd9,
    0x8f, 0x00, 0xb2, 0x04,
    0xe9, 0x80, 0x09, 0x98,
    0xec, 0xf8, 0x42, 0x7e
  };

  pdf_crypt_init();

  pdf_crypt_md_new (PDF_CRYPT_MD_MD5, &md);
  fail_if (pdf_crypt_md_hash (md, out, sizeof(out), in, 0) != PDF_OK);
  fail_if (memcmp (real_out, out, sizeof(out)));
  
  pdf_crypt_md_destroy (md);
}
END_TEST



/*
 * Test: pdf_crypt_md_hash_002
 * Description:
 *   Compute the md5 checksum of a string
 * Success condition:
 *   The output data should be correct.
 */
START_TEST (pdf_crypt_md_hash_002)
{
  pdf_char_t in[1] = "a";
  pdf_char_t out[16];
  pdf_crypt_md_t md;
  pdf_char_t real_out[] = {
    0x0C, 0xC1, 0x75, 0xB9,
    0xC0, 0xF1, 0xB6, 0xA8,
    0x31, 0xC3, 0x99, 0xE2,
    0x69, 0x77, 0x26, 0x61
  };

  pdf_crypt_init();

  pdf_crypt_md_new (PDF_CRYPT_MD_MD5, &md);
  fail_if (pdf_crypt_md_hash (md, out, sizeof(out), in, sizeof(in)) != PDF_OK);
  fail_if (memcmp (real_out, out, sizeof(out)));
  
  pdf_crypt_md_destroy (md);
}
END_TEST



/*
 * Test: pdf_crypt_md_hash_003
 * Description:
 *   Compute the md5 checksum of a string
 * Success condition:
 *   The output data should be correct.
 */
START_TEST (pdf_crypt_md_hash_003)
{
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

  pdf_crypt_md_new (PDF_CRYPT_MD_MD5, &md);
  fail_if (pdf_crypt_md_hash (md, out, sizeof(out), in, sizeof(in)) != PDF_OK);
  fail_if (memcmp (real_out, out, sizeof(out)));
  
  pdf_crypt_md_destroy (md);
}
END_TEST




/*
 * Test case creation function
 */
TCase *
test_pdf_crypt_md_hash (void)
{
  TCase *tc = tcase_create("pdf_crypt_md_hash");
  tcase_add_test(tc, pdf_crypt_md_hash_001);
  tcase_add_test(tc, pdf_crypt_md_hash_002);
  tcase_add_test(tc, pdf_crypt_md_hash_003);
  return tc;
}


/* End of pdf-crypt-md-hash.c */


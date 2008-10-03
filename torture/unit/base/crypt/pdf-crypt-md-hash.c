/* -*- mode: C -*- Time-stamp: "2008-09-09 04:18:07 david"
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

  pdf_crypt_md_new (&md, PDF_CRYPT_MD_MD5);
  fail_if (pdf_crypt_md_hash (md, out, sizeof(out), in, 0) != PDF_OK);
  fail_if (!memcmp (real_out, out, sizeof(out)));
  
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
  return tc;
}


/* End of pdf-crypt-md-hash.c */


/* -*- mode: C -*-
 *
 *       File:         pdf-crypt-md-read.c
 *       Date:         Tue Dec 23 17:30:19 2008
 *
 *       GNU PDF Library - Unit tests for pdf_crypt_md_read
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
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pdf.h>
#include <check.h>
#include <pdf-test-common.h>

/*
 * Test: pdf_crypt_md_read_001
 * Description:
 *   Compute the md5 of an empty buffer.
 * Success condition:
 *   Returns PDF_OK and output buffer matches.
 */
START_TEST (pdf_crypt_md_read_001)
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

  pdf_init();

  pdf_crypt_md_new (PDF_CRYPT_MD_MD5, &md);

  pdf_crypt_md_write (md, in, 0);

  fail_if (pdf_crypt_md_read (md, out, sizeof(out)) != PDF_OK);
  fail_if (memcmp (real_out, out, sizeof(out)));

  pdf_crypt_md_destroy (md);
}
END_TEST



/*
 * Test: pdf_crypt_md_read_002
 * Description:
 *   Compute the md5 checksum of a string
 * Success condition:
 *   The output data should be correct.
 */
START_TEST (pdf_crypt_md_read_002)
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

  pdf_init();

  pdf_crypt_md_new (PDF_CRYPT_MD_MD5, &md);

  pdf_crypt_md_write (md, in, sizeof(in));

  fail_if (pdf_crypt_md_read (md, out, sizeof(out)) != PDF_OK);
  fail_if (memcmp (real_out, out, sizeof(out)));

  pdf_crypt_md_destroy (md);
}
END_TEST



/*
 * Test: pdf_crypt_md_read_003
 * Description:
 *   Compute the md5 checksum of a string
 * Success condition:
 *   The output data should be correct.
 */
START_TEST (pdf_crypt_md_read_003)
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

  pdf_init();

  pdf_crypt_md_new (PDF_CRYPT_MD_MD5, &md);

  pdf_crypt_md_write (md, in, sizeof(in));

  fail_if (pdf_crypt_md_read (md, out, sizeof(out)) != PDF_OK);
  fail_if (memcmp (real_out, out, sizeof(out)));

  pdf_crypt_md_destroy (md);
}
END_TEST


/*
 * Test: pdf_crypt_md_read_004
 * Description:
 *   Confirm that we handle the "output buffer is too small" case
 * Success condition:
 *   Returns PDF_EBADDATA.
 */
START_TEST (pdf_crypt_md_read_004)
{
  pdf_char_t *in;
  pdf_char_t out[6];
  pdf_crypt_md_t md;
  pdf_char_t real_out[] = {
    0xd4, 0x1d, 0x8c, 0xd9,
    0x8f, 0x00, 0xb2, 0x04,
    0xe9, 0x80, 0x09, 0x98,
    0xec, 0xf8, 0x42, 0x7e
  };

  pdf_init();

  pdf_crypt_md_new (PDF_CRYPT_MD_MD5, &md);

  pdf_crypt_md_write (md, in, 0);

  fail_unless (pdf_crypt_md_read (md, out, sizeof(out)) == PDF_EBADDATA);

  pdf_crypt_md_destroy (md);
}
END_TEST

/*
 * Test: pdf_crypt_md_read_005
 * Description:
 *   Confirm that we handle passing an unsupported message digest
 *   algorithm to pdf_crypt_md_new().
 * Success condition:
 *   Returns PDF_EBADDATA.
 */
START_TEST (pdf_crypt_md_read_005)
{
  pdf_char_t *in;
  pdf_char_t out[20];
  pdf_crypt_md_t md;

  pdf_init();

  fail_unless (pdf_crypt_md_new (PDF_CRYPT_MD_MD5 + 1, &md) == PDF_EBADDATA);
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_crypt_md_read (void)
{
  TCase *tc = tcase_create("pdf_crypt_md_read");
  tcase_add_test(tc, pdf_crypt_md_read_001);
  tcase_add_test(tc, pdf_crypt_md_read_002);
  tcase_add_test(tc, pdf_crypt_md_read_003);
  tcase_add_test(tc, pdf_crypt_md_read_004);
  tcase_add_test(tc, pdf_crypt_md_read_005);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}


/* End of pdf-crypt-md-read.c */

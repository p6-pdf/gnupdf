/* -*- mode: C -*-
 *
 *       File:         pdf-crypt-md-hash.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_crypt_md_write
 *
 */

/* Copyright (C) 2008, 2009-2011 Free Software Foundation, Inc. */

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *#include <pdf-test-common.h>
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
 * Test: pdf_crypt_md_write_001
 * Description:
 *   Pass an empty message
 * Success condition:
 *   Returns PDF_OK.
 */
START_TEST (pdf_crypt_md_write_001)
{
  pdf_char_t * in;
  pdf_crypt_md_t md;
    pdf_crypt_md_new (PDF_CRYPT_MD_MD5, &md);
  fail_if (pdf_crypt_md_write (md, in, 0) != PDF_OK);
  pdf_crypt_md_destroy (md);
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_crypt_md_write (void)
{
  TCase *tc = tcase_create("pdf_crypt_md_write");
  tcase_add_test(tc, pdf_crypt_md_write_001);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}


/* End of pdf-crypt-md-write.c */

/* -*- mode: C -*- Time-stamp: ""
 *
 *       File:         pdf-text-init.c
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_init test case
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
#include <pdf.h>
#include <check.h>

/*
 * Test: pdf_text_init_001
 * Description:
 *   Initialize text module
 * Success conditions:
 *   1. The call should not produce an error.
 *   2. A valid (non-empty) host encoding should be detected.
 *   3. A valid host endianness should be detected (either LE or BE).
 *   4. A valid (non-empty) host EOL marker should be detected.
 *   5. A valid (non-empty) language ID should be detected.
 */
START_TEST(pdf_text_init_001)
{
  /* 1. The call should not produce an error */
  fail_if(pdf_text_init() != PDF_OK);

  /*
   * The other conditions are already checked within pdf_text_init. If any of
   *  them is not fullfilled then the function won't return PDF_OK 
   */
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_text_init (void)
{
  TCase *tc = tcase_create("pdf_text_init");
  tcase_add_test(tc, pdf_text_init_001);
  return tc;
}

/* End of pdf-text-init.c */

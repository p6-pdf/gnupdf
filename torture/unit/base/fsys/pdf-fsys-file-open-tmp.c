/* -*- mode: C -*-
 *
 *       File:         pdf-fsys-file-open-tmp.c
 *       Date:         Wed Feb  3 21:09:22 2010
 *
 *       GNU PDF Library - Unit tests for pdf_fsys_file_open_tmp
 *
 */

/* Copyright (C) 2010 Free Software Foundation, Inc.  */

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
#include <tortutils.h>
#include <check.h>

#include <pdf.h>

/*
 * Test: pdf_fsys_file_open_tmp_001
 * Description:
 *   Open a temporary file for writing
 * Success condition:
 *   The call to pdf_fsys_file_open_tmp should return PDF_OK.
 */

START_TEST(pdf_fsys_file_open_tmp_001)
{
  pdf_fsys_file_t file;

  fail_if( pdf_text_init() != PDF_OK);

  fail_if( pdf_fsys_file_open_tmp (NULL, &file) != PDF_OK );

  pdf_fsys_file_close(file);
}
END_TEST

/*
 * Test case creation function.
 */

TCase *
test_pdf_fsys_file_open_tmp (void)
{
  TCase *tc = tcase_create ("pdf_fsys_file_open_tmp");
  
  tcase_add_test (tc, pdf_fsys_file_open_tmp_001);
  return tc;
}

/* End of pdf-fsys-file-open-tmp.c */

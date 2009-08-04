/* -*- mode: C -*- Time-stamp: "2009-05-03 21:32:19 derhans"
 *
 *       File:         pdf-fsys-get-temp-path-name.c
 *       Date:         Sat May  3 21:02:12 2009
 *
 *       GNU PDF Library - Unit tests for pdf_fsys_get_temp_path_name
 *
 */

/* Copyright (C) 2009 Free Software Foundation, Inc. */

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
#include <check.h>
#include <pdf.h>

/*
 * Test: pdf_fsys_get_temp_path_name_001
 * Description:
 *   Get the path to a new temporary file
 * Success condition:
 *   The call to pdf_fsys_get_temp_path_name should not return NULL
 */

START_TEST(pdf_fsys_get_temp_path_name_001)
{
  fail_if( pdf_text_init() != PDF_OK );
  fail_if( pdf_fsys_get_temp_path_name(NULL) == NULL );
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_fsys_get_temp_path_name (void)
{
  TCase *tc = tcase_create("pdf_fsys_get_temp_path_name");
  tcase_add_test(tc, pdf_fsys_get_temp_path_name_001);
  return tc;
}

/* End of pdf-fsys-get-temp-path-name.c */

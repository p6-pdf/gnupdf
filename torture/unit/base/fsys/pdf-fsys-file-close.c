/* -*- mode: C -*-
 *
 *       File:         pdf-fsys-file-close.c
 *       Date:         Sat May  3 21:02:12 2009
 *
 *       GNU PDF Library - Unit tests for pdf_fsys_file_close
 *
 */

/* Copyright (C) 2009-2011 Free Software Foundation, Inc. */

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
#include <string.h>

#include <tortutils.h>
#include <pdf-test-common.h>
/*
 * Test: pdf_fsys_file_close_001
 * Description:
 *   Open an existing and readable file and close it afterwards
 * Success condition:
 *   The call to pdf_fsys_file_close should return PDF_OK
 */

START_TEST (pdf_fsys_file_close_001)
{
  pdf_fsys_file_t file;
  pdf_text_t path;
  pdf_char_t filename[] = "TD00005", *pathname;

  pathname = tortu_get_data_file_path(filename);
  file = (pdf_fsys_file_t)pdf_alloc(sizeof(struct pdf_fsys_file_s));

  fail_if( pdf_text_new_from_unicode(pathname,
                                     strlen((char*) pathname),
                                     PDF_TEXT_UTF8,
                                     &path) != PDF_OK );

  fail_if( pdf_fsys_file_open(NULL,
                              path,
                              PDF_FSYS_OPEN_MODE_READ,
                              &file) != PDF_OK );

  fail_if( pdf_fsys_file_close(file) != PDF_OK );
  pdf_text_destroy(path);
}
END_TEST

/*
 * Test: pdf_fsys_file_close_002
 * Description:
 *   Try to close a NULL file
 * Success condition:
 *   The call to pdf_fsys_file_close should return PDF_EBADDATA
 */

START_TEST (pdf_fsys_file_close_002)
{
  fail_if( pdf_fsys_file_close( NULL ) != PDF_EBADDATA );
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_fsys_file_close (void)
{
  TCase *tc = tcase_create("pdf_fsys_file_close");
  tcase_add_test(tc, pdf_fsys_file_close_001);
  tcase_add_test(tc, pdf_fsys_file_close_002);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-fsys-file-close.c */

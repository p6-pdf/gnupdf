/* -*- mode: C -*-
 *
 *       File:         pdf-fsys-build-path.c
 *       Date:         Mon Apr  16 00:02:09 2010
 *
 *       GNU PDF Library - Unit tests for pdf_fsys_build_path
 *
 */

/* Copyright (C) 2010 Free Software Foundation, Inc. */

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
 * Test: pdf_fsys_build_path_001
 * Description:
 *   Build a path name from two text objects
 * Success condition:
 *   The call to pdf_fsys_build_path should return PDF_OK
 */

START_TEST(pdf_fsys_build_path_001)
{
  pdf_text_t text1,text2,text3,result;
  pdf_char_t *first="want",*dir="some",*dirr="beer?", *output_data=NULL;

  /* Always INIT! Check runs each test in a different process */
  fail_if(pdf_init() != PDF_OK);

  fail_if(pdf_text_new_from_unicode(first, 4, PDF_TEXT_UTF8, &text1) != PDF_OK);
  fail_if(pdf_text_new_from_unicode(dir, 4, PDF_TEXT_UTF8, &text2) != PDF_OK);
  fail_if(pdf_text_new_from_unicode(dirr, 5, PDF_TEXT_UTF8, &text3) != PDF_OK);

  fail_if(pdf_fsys_build_path (NULL, &result, text1, text2, text3, NULL)
          != PDF_OK);

  fail_if(pdf_text_get_pdfdocenc(&output_data, result) != PDF_OK);

#if FILE_SYSTEM_BACKSLASH_IS_FILE_NAME_SEPARATOR
  fail_unless(strcmp(output_data, "want\\some\\beer?" ) == 0);
  fail_unless(strlen(output_data) == strlen("want\\some\\beer?"));
#else
  fail_unless(strcmp(output_data, "want/some/beer?" ) == 0);
  fail_unless(strlen(output_data) == strlen("want/some/beer?"));
#endif /*  FILE_SYSTEM_BACKSLASH_IS_FILE_NAME_SEPARATOR */


  pdf_text_destroy (text1);
  pdf_text_destroy (text2);
  pdf_text_destroy (text3);
  pdf_dealloc (output_data);
}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_fsys_build_path (void)
{
  TCase *tc = tcase_create("pdf_fsys_build_path");
  tcase_add_test(tc, pdf_fsys_build_path_001);
  return tc;
}


/* End of pdf-fsys-build-path.c */

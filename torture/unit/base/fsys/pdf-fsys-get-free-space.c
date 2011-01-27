/* -*- mode: C -*-
 *
 *       File:         pdf-fsys-get-free-space.c
 *       Date:         Fri May  1 20:20:11 2009
 *
 *       GNU PDF Library - Unit tests for pdf_fsys_get_free_space
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

#include <string.h>
#include <check.h>
#include <pdf.h>

/*
 * Test: pdf_fsys_get_free_space_001
 * Description:
 *   Get the free space left on the filesystem for an existing path.
 * Success condition:
 *   The call to pdf_fsys_get_free_space should not return an error
 */

START_TEST(pdf_fsys_get_free_space_001)
{
  pdf_i64_t free_space, error;
  pdf_text_t path;
  pdf_char_t pathname[] = ".";

  fail_if( pdf_text_init() != PDF_OK );

  fail_if( pdf_text_new_from_unicode(pathname,
                                     strlen((char*) pathname),
                                     PDF_TEXT_UTF8,
                                     &path) != PDF_OK );

  error = -1;

  free_space = pdf_fsys_get_free_space(NULL, path);
  fail_if(free_space == error);

  pdf_text_destroy(path);
}
END_TEST

/*
 * Test: pdf_fsys_get_free_space_002
 * Description:
 *   Get the free space left on the filesystem for an non existing path.
 * Success condition:
 *   The call to pdf_fsys_get_free_space should return -1, which indicates an
 *   error.
 */

START_TEST(pdf_fsys_get_free_space_002)
{
  pdf_i64_t free_space, error;
  pdf_text_t path;
  pdf_char_t pathname[] = "DOESNTEXIST";

  fail_if( pdf_text_init() != PDF_OK );

  fail_if( pdf_text_new_from_unicode(pathname,
                                     strlen((char*) pathname),
                                     PDF_TEXT_UTF8,
                                     &path) != PDF_OK );

  error = -1;

  free_space = pdf_fsys_get_free_space(NULL, path);
  fail_if (free_space != error);

  pdf_text_destroy(path);
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_fsys_get_free_space (void)
{
  TCase *tc = tcase_create("pdf_fsys_get_free_space");
  tcase_add_test(tc, pdf_fsys_get_free_space_001);
  tcase_add_test(tc, pdf_fsys_get_free_space_002);
  return tc;
}


/* End of pdf-fsys-get-free-space.c */

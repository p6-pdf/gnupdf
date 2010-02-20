/* -*- mode: C -*-
 *
 *       File:         pdf-stm-file-new.c
 *       Date:         Sat Sep 20 14:37:26 2008
 *
 *       GNU PDF Library - Unit tests for pdf_stm_file_new
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

#include <check.h>
#include <pdf.h>

/*
 * Test: pdf_stm_file_new_001
 * Description:
 *   Create a new file-based reading stream.
 * Success condition:
 *   The call to pdf_stm_file_new should return success.
 */
START_TEST (pdf_stm_file_new_001)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_fsys_file_t file;
  pdf_text_t path;
  pdf_char_t * remain;
  pdf_size_t remain_length;

  /* Create the file path */
  pdf_init();

  ret = pdf_text_new_from_pdf_string ("tmp.test", 8, &remain, &remain_length, &path);
  fail_if (ret != PDF_OK);

  /* Open new file */
  ret = pdf_fsys_file_open (NULL, path, PDF_FSYS_OPEN_MODE_WRITE, &file); 
  fail_if (ret != PDF_OK);

  /* Create the stream */
  ret = pdf_stm_file_new (file,
                          0,
                          0, /* Use the default cache size */
                          PDF_STM_WRITE,
                          &stm);
  fail_if(ret != PDF_OK);

  /* Free all resources */
  pdf_stm_destroy (stm);
  pdf_fsys_file_close (file);
  pdf_text_destroy (path);

}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_stm_file_new (void)
{
  TCase *tc = tcase_create ("pdf_stm_file_new");

  tcase_add_test(tc, pdf_stm_file_new_001);

  return tc;
}

/* End of pdf-stm-file-new.c */

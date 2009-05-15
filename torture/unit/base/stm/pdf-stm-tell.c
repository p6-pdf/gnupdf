/* -*- mode: C -*- Time-stamp: "2009-05-15 13:50:03 gerel"
 *
 *       File:         pdf-stm-tell.c
 *       Date:         Sat Sep 20 19:16:41 2008
 *
 *       GNU PDF Library - Unit tests for pdf_stm_tell
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

#include <string.h>
#include <check.h>
#include <pdf.h>

/*
 * Test: pdf_stm_tell_001
 * Description:
 *   Tell an initialized reading memory stream.
 * Success condition:
 *   The tell operation should report the first position
 *   in the stream (0).
 */
START_TEST (pdf_stm_tell_001)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_size_t buf_size;
  pdf_off_t pos;

  /* Create a buffer with some contents */
  buf_size = 11;
  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);
  strcpy ((char *) buf, "0123456789");

  /* Create the stream */
  ret = pdf_stm_mem_new (buf,
                         buf_size,
                         0, /* Use the default cache size */
                         PDF_STM_READ,
                         &stm);
  fail_if(ret != PDF_OK);

  /* Tell the stream */
  pos = pdf_stm_tell (stm);
  fail_if(pos != 0);

  /* Destroy data */
  pdf_dealloc (buf);
  pdf_stm_destroy (stm);
}
END_TEST


/*
 * Test: pdf_stm_tell_002
 * Description:
 *   Tell an initialized reading file stream.
 * Success condition:
 *   The tell operation should report the first position
 *   in the stream (0).
 */
START_TEST (pdf_stm_tell_002)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t ret_char;
  pdf_off_t pos;

  pdf_fsys_file_t file;
  pdf_text_t path;
  pdf_char_t * remain;
  pdf_size_t remain_length,written;

  /* Create the file path */
  pdf_text_init ();
  ret = pdf_text_new_from_pdf_string ("tmp.test", 8, &remain, &remain_length,
                                      &path);
  fail_if (ret != PDF_OK);

  /* Open new file */
  ret = pdf_fsys_file_open (NULL, path, PDF_FSYS_OPEN_MODE_WRITE, &file); 
  fail_if (ret != PDF_OK);

  ret = pdf_fsys_file_write (file, "0123456789", 10, &written);
  fail_if (ret != PDF_OK);
  fail_if (written != 10);
  pdf_fsys_file_close (file);

  ret = pdf_fsys_file_open (NULL, path, PDF_FSYS_OPEN_MODE_READ, &file); 
  fail_if (ret != PDF_OK);
  /* Create the stream */
  ret = pdf_stm_file_new (file,
                          0,
                          0, /* Use the default cache size */
                          PDF_STM_READ,
                          &stm);
  fail_if(ret != PDF_OK);

  /* Tell the stream */
  pos = pdf_stm_tell (stm);
  fail_if(pos != 0);

  /* Destroy data */
  pdf_stm_destroy (stm);
  pdf_fsys_file_close (file);
  pdf_text_destroy (path);
}
END_TEST




/*
 * Test case creation function
 */
TCase *
test_pdf_stm_tell (void)
{
  TCase *tc = tcase_create ("pdf_stm_tell");

  tcase_add_test(tc, pdf_stm_tell_001);
  tcase_add_test(tc, pdf_stm_tell_002);

  return tc;
}

/* End of pdf-stm-tell.c */

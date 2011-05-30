/* -*- mode: C -*-
 *
 *       File:         pdf-stm-peek-char.c
 *       Date:         Sat Sep 20 18:07:45 2008
 *
 *       GNU PDF Library - Unit tests for pdf_stm_peek_char
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

#include <string.h>
#include <check.h>
#include <pdf.h>
#include <pdf-test-common.h>

/*
 * Test: pdf_stm_peek_char_001
 * Description:
 *   Peek a character from a memory stream.
 * Success condition:
 *   The peek character should be ok.
 */
START_TEST (pdf_stm_peek_char_001)
{
  pdf_error_t *error = NULL;
  pdf_stm_t *stm;
  pdf_char_t *buf;
  pdf_char_t ret_char = '\0';
  pdf_size_t buf_size = 11;
  int i;

  /* Create a buffer with some contents */
  buf = pdf_alloc (buf_size);
  fail_unless (buf != NULL);
  strcpy (buf, "0123456789");

  /* Create the stream */
  stm = pdf_stm_mem_new (buf,
                         buf_size,
                         0, /* Use the default cache size */
                         PDF_STM_READ,
                         &error);
  fail_unless (stm != NULL);
  fail_if (error != NULL);

  /* Read all characters from the stream, one by one */
  for (i = 0; i < buf_size; i++)
    {
      /* Peek */
      fail_unless (pdf_stm_peek_char (stm, &ret_char, &error) == PDF_TRUE);
      fail_if (error != NULL);
      fail_unless (ret_char == buf[i]);

      /* And consume it */
      fail_unless (pdf_stm_read_char (stm, &ret_char, &error) == PDF_TRUE);
      fail_if (error != NULL);
      fail_unless (ret_char == buf[i]);
    }

  /* Try to read another character from the stream. Should return PDF_FALSE
   * and no error, indicating EOF. */
  fail_unless (pdf_stm_peek_char (stm, &ret_char, &error) == PDF_FALSE);
  fail_if (error != NULL);

  /* Destroy data */
  pdf_dealloc (buf);
  pdf_stm_destroy (stm);
}
END_TEST

/*
 * Test: pdf_stm_peek_char_002
 * Description:
 *   Peek a character from a file stream.
 * Success condition:
 *   The peek character should be ok.
 */
START_TEST (pdf_stm_peek_char_002)
{
  const pdf_char_t *filename = "tmp.test";
  const pdf_char_t *file_contents = "GNU";
  pdf_error_t *error = NULL;
  pdf_stm_t *stm;
  pdf_char_t ret_char = '\0';
  pdf_size_t written_bytes;
  pdf_fsys_file_t *file;
  pdf_text_t *path;
  int i;

  /* Create the file path */
  path = pdf_text_new_from_unicode (filename, strlen (filename),
                                    PDF_TEXT_UTF8,
                                    &error);
  fail_unless (path != NULL);
  fail_if (error != NULL);

  /* Open new file */
  file = pdf_fsys_file_open (PDF_FSYS_DISK,
                             path,
                             PDF_FSYS_OPEN_MODE_WRITE,
                             &error);
  fail_unless (file != NULL);
  fail_if (error != NULL);

  written_bytes = 0;
  fail_unless (pdf_fsys_file_write (file,
                                    file_contents,
                                    strlen (file_contents),
                                    &written_bytes,
                                    &error) == PDF_TRUE);
  fail_if (written_bytes != strlen (file_contents));
  pdf_fsys_file_close (file, NULL);

  file = pdf_fsys_file_open (PDF_FSYS_DISK,
                             path,
                             PDF_FSYS_OPEN_MODE_READ,
                             &error);
  fail_unless (file != NULL);
  fail_if (error != NULL);

  for (i = 0; i < strlen (file_contents); i++)
    {
      /* Peek */
      fail_unless (pdf_stm_peek_char (stm, &ret_char, &error) == PDF_TRUE);
      fail_if (error != NULL);
      fail_unless (ret_char == file_contents[i]);

      /* And consume it */
      fail_unless (pdf_stm_read_char (stm, &ret_char, &error) == PDF_TRUE);
      fail_if (error != NULL);
      fail_unless (ret_char == file_contents[i]);
    }

  /* Try to read another character from the stream. Should return PDF_FALSE
   * and no error, indicating EOF. */
  fail_unless (pdf_stm_peek_char (stm, &ret_char, &error) == PDF_FALSE);
  fail_if (error != NULL);

  /* Destroy data */
  pdf_stm_destroy (stm);
  pdf_fsys_file_close (file, NULL);
  pdf_text_destroy (path);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_stm_peek_char (void)
{
  TCase *tc = tcase_create ("pdf_stm_peek_char");

  tcase_add_test(tc, pdf_stm_peek_char_001);
  tcase_add_test(tc, pdf_stm_peek_char_002);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-stm-peek-char.c */

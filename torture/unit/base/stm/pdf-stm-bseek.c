/* -*- mode: C -*-
 *
 *       File:         pdf-stm-bseek.c
 *       Date:         Sat Sep 20 18:50:33 2008
 *
 *       GNU PDF Library - Unit tests for pdf_stm_bseek
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
 * Test: pdf_stm_bseek_001
 * Description:
 *   Bseek into a memory read stream.
 * Success condition:
 *   The bseek operation should success.
 */
START_TEST (pdf_stm_bseek_001)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_char_t ret_char;
  pdf_size_t buf_size;
  pdf_off_t pos;

  /* Create a buffer with some contents */
  buf_size = 11;
  pdf_init();

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

  /* Bseek into the stream */
  pos = pdf_stm_bseek (stm, 3);
  fail_if(pos != 3);

  /* Read a character */
  ret = pdf_stm_read_char (stm, &ret_char);
  fail_if(ret != PDF_OK);
  fail_if(ret_char != '3');

  /* Bseek into the stream */
  pos = pdf_stm_bseek (stm, 8);
  fail_if(pos != 8);

  /* Read a character */
  ret = pdf_stm_read_char (stm, &ret_char);
  fail_if(ret != PDF_OK);
  fail_if(ret_char != '8');
  
  /* Destroy data */
  pdf_dealloc (buf);
  pdf_stm_destroy (stm);
}
END_TEST

/*
 * Test: pdf_stm_bseek_002
 * Description:
 *   Bseek to a position beyond the size of the stream 
 *   into a memory read stream.
 * Success condition:
 *   The bseek operation should success and the bseek
 *   position should be to the last valid position
 *   in the stream.
 */
START_TEST (pdf_stm_bseek_002)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_char_t ret_char;
  pdf_size_t buf_size;
  pdf_off_t pos;

  /* Create a buffer with some contents */
  buf_size = 10;
  pdf_init();

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

  /* Bseek into the stream */
  pos = pdf_stm_bseek (stm, 300);
  fail_if(pos != 9);

  /* Read a character */
  ret = pdf_stm_read_char (stm, &ret_char);
  fail_if(ret != PDF_OK);
  fail_if(ret_char != '9');
  
  /* Destroy data */
  pdf_dealloc (buf);
  pdf_stm_destroy (stm);
}
END_TEST


/*
 * Test: pdf_stm_bseek_003
 * Description:
 *   Bseek into a file read stream.
 * Success condition:
 *   The bseek operation should success.
 */
START_TEST (pdf_stm_bseek_003)
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
  pdf_init();

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

  /* Bseek into the stream */
  pos = pdf_stm_bseek (stm, 3);
  fail_if(pos != 3);

  /* Read a character */
  ret = pdf_stm_read_char (stm, &ret_char);
  fail_if(ret != PDF_OK);
  fail_if(ret_char != '3');

  /* Bseek into the stream */
  pos = pdf_stm_bseek (stm, 8);
  fail_if(pos != 8);

  /* Read a character */
  ret = pdf_stm_read_char (stm, &ret_char);
  fail_if(ret != PDF_OK);
  fail_if(ret_char != '8');
  
  /* Destroy data */
  pdf_stm_destroy (stm);
  pdf_fsys_file_close (file);
  pdf_text_destroy (path);
}
END_TEST


/*
 * Test: pdf_stm_bseek_004
 * Description:
 *   Bseek to a position beyond the size of the stream 
 *   into a file read stream.
 * Success condition:
 *   The bseek operation should success and the bseek
 *   position should be to the last valid position
 *   in the stream.
 */
START_TEST (pdf_stm_bseek_004)
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
  pdf_init();

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

  /* Bseek into the stream */
  pos = pdf_stm_bseek (stm, 300);
  fail_if(pos != 9);

  /* Read a character */
  ret = pdf_stm_read_char (stm, &ret_char);
  fail_if(ret != PDF_OK);
  fail_if(ret_char != '9');

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
test_pdf_stm_bseek (void)
{
  TCase *tc = tcase_create ("pdf_stm_bseek");

  tcase_add_test(tc, pdf_stm_bseek_001);
  tcase_add_test(tc, pdf_stm_bseek_002);
  tcase_add_test(tc, pdf_stm_bseek_003);
  tcase_add_test(tc, pdf_stm_bseek_004);

  return tc;
}

/* End of pdf-stm-bseek.c */

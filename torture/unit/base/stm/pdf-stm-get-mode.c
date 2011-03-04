/* -*- mode: C -*-
 *
 *       File:         pdf-stm-get-mode.c
 *       Date:         Wed Oct 28 21:34:34 2009
 *
 *       GNU PDF Library - Unit tests for pdf_stm_get_mode
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
#include <pdf-test-common.h>
/*
 * Test: pdf_stm_get_mode_001
 * Description:
 *   Get the mode of a reading memory stream.
 * Success condition:
 *   The call to pdf_stm_get_mode should return
 *   PDF_STM_READ.
 */
START_TEST (pdf_stm_get_mode_001)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_size_t buf_size;

  /* Create a memory buffer */
  buf_size = 100;

  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);

  /* Create the stream */
  ret = pdf_stm_mem_new (buf,
                         buf_size,
                         0, /* Use the default cache size */
                         PDF_STM_READ,
                         &stm);
  fail_if(ret != PDF_OK);

  /* Get the mode of the stream */
  fail_if (pdf_stm_get_mode (stm) != PDF_STM_READ);

  /* Destroy the stream */
  pdf_stm_destroy (stm);
  pdf_dealloc (buf);
}
END_TEST

/*
 * Test: pdf_stm_get_mode_002
 * Description:
 *   Get the mode of a writing memory stream.
 * Success condition:
 *   The call to pdf_stm_get_mode should return
 *   PDF_STM_WRITE.
 */

START_TEST (pdf_stm_get_mode_002)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_size_t buf_size;

  /* Create a memory buffer */
  buf_size = 100;

  buf = pdf_alloc (buf_size);
  fail_if(buf == NULL);

  /* Create the stream */
  ret = pdf_stm_mem_new (buf,
                         buf_size,
                         0, /* Use the default cache size */
                         PDF_STM_WRITE,
                         &stm);
  fail_if(ret != PDF_OK);

  /* Get the mode of the stream */
  fail_if (pdf_stm_get_mode (stm) != PDF_STM_WRITE);

  /* Destroy the stream */
  pdf_stm_destroy (stm);
  pdf_dealloc (buf);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_stm_get_mode (void)
{
  TCase *tc = tcase_create ("pdf_stm_get_mode");

  tcase_add_test (tc, pdf_stm_get_mode_001);
  tcase_add_test (tc, pdf_stm_get_mode_002);

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-stm-get-mode.c */

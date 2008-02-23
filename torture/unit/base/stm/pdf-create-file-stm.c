/* -*- mode: C -*- Time-stamp: "08/02/22 20:08:19 jemarch"
 *
 *       File:         pdf-create-file-stm.c
 *       Date:         Thu Dec  6 19:21:01 2007
 *
 *       GNU PDF Library - Unit tests for `pdf_create_file_stm'
 *
 */

/* Copyright (C) 2007, 2008 Free Software Foundation, Inc. */

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
#include <stdio.h>
#include <pdf.h>
#include <check.h>

/*
 * Test: pdf_create_file_stm_001
 * Description:
 *   Try to open a file stream to a non-existing file.
 * Success condition:
 *   `pdf_create_file_stm' should return a NULL pointer.
 */
START_TEST(pdf_create_file_stm_001)
{
  pdf_stm_t stm;
  
  /* Test the file backend */
  stm = pdf_create_file_stm ("non-existing-file", 
                             PDF_STM_OPEN_MODE_READ);
  fail_if(stm != NULL);
}
END_TEST

/*
 * Test: pdf_create_file_stm_002
 * Description:
 *   Open a file stream to an existing file in
 *   read-only mode.
 * Success condition:
 *   `pdf_create_file_stm' should return a correct
 *   stream object.
 */
START_TEST(pdf_create_file_stm_002)
{
  pdf_stm_t stm;
  
  stm = pdf_create_file_stm ("torture-file-1", 
                             PDF_STM_OPEN_MODE_READ);
  
  fail_if (stm == NULL);
  pdf_stm_close (stm);
}
END_TEST
 
/*
 * Test: pdf_create_file_stm_003
 * Description:
 *   Open a file stream to a non existing file in
 *   write-only mode.
 * Success condition:
 *   `pdf_create_file_stm' should return a correct
 *   stream object.
 */
START_TEST(pdf_create_file_stm_003)
{
  pdf_stm_t stm;
  
  stm = pdf_create_file_stm ("temporal-test-file", 
                             PDF_STM_OPEN_MODE_WRITE);
  
  fail_if (stm == NULL);
  fail_if (!pdf_stm_close (stm));
}
END_TEST

/*
 * Test: pdf_create_file_stm_004
 * Description:
 *   Open a file stream to an existing file in
 *   write-only mode.
 * Success condition:
 *   `pdf_create_file_stm' should return a correct
 *   stream object.
 */
START_TEST(pdf_create_file_stm_004)
{
  pdf_stm_t stm;
  
  stm = pdf_create_file_stm ("temporal-test-file", 
                             PDF_STM_OPEN_MODE_WRITE);
  
  fail_if (stm == NULL);
  fail_if (!pdf_stm_close (stm));
}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_create_file_stm (void)
{
  TCase *tc = tcase_create("pdf_create_file_stm");
  tcase_add_test(tc, pdf_create_file_stm_001);
  tcase_add_test(tc, pdf_create_file_stm_002);
  tcase_add_test(tc, pdf_create_file_stm_003);
  tcase_add_test(tc, pdf_create_file_stm_004);

  return tc;
}


/* End of test_stm_module.c */

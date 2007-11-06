/* -*- mode: C -*- Time-stamp: "07/11/06 22:44:04 jemarch"
 *
 *       File:         test-stm_openclose.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Mon Jul  9 22:51:16 2007
 *
 *       GNU PDF Library - Testsuite, opening and closing streams
 *
 */

/* Copyright (C) 2007 Free Software Foundation, Inc. */

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
#include <pdf_stm.h>
#include <check.h>
  

START_TEST(noexist)
{
  pdf_stm_t stm;
  
  /* Test the file backend */
  stm = pdf_create_file_stm ("non-existing-file", 
                             PDF_STM_OPEN_MODE_READ);
  fail_if(stm != NULL);
}
END_TEST
 
 
START_TEST(readmode)
{
  pdf_stm_t stm;
  
  stm = pdf_create_file_stm ("torture-file-1", 
                             PDF_STM_OPEN_MODE_READ);
  
  fail_if (stm == NULL);
  pdf_stm_close (stm);
}
END_TEST
 
 
START_TEST(writemode)
{
  pdf_stm_t stm;
  
  stm = pdf_create_file_stm ("temporal-test-file", 
                             PDF_STM_OPEN_MODE_WRITE);
  
  fail_if (stm == NULL);
  fail_if (!pdf_stm_close (stm));
}
END_TEST

START_TEST(writemode_again)
{
  pdf_stm_t stm;
  
  stm = pdf_create_file_stm ("temporal-test-file", 
                             PDF_STM_OPEN_MODE_WRITE);
  
  fail_if (stm == NULL);
  fail_if (!pdf_stm_close (stm));
}
END_TEST
 
TCase *
test_stm_openclose_tests (void)
{
  TCase *tc = tcase_create("test-stm_openclose");
  tcase_add_test(tc, noexist);
  tcase_add_test(tc, readmode);
  tcase_add_test(tc, writemode);
  tcase_add_test(tc, writemode_again);

  return tc;
}
  

/* End of test-stm_openclose.c */

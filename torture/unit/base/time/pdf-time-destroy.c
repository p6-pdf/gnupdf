/* -*- mode: C -*- Time-stamp: "09/04/17 00:05:24 jemarch"
 *
 *       File:         pdf-time-destroy.c
 *       Date:         Fri Feb 27 17:35:31 2008
 *
 *       GNU PDF Library - Unit tests for pdf_time_destroy
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
#include <check.h>
#include <pdf.h>
#include <stdlib.h>

/*
 * Test: pdf_time_destroy_001
 * Description:
 *   Create new pdf_time_t and destroy it.
 *   
 *Success condition:
 * 1. Function pdf_time_new schould return PDF_OK
 * 4. Fuction pdf_time_destroy schould return PDF_OK
 */
START_TEST (pdf_time_destroy_001)
{
  pdf_status_t status;

  pdf_time_t time1;
  pdf_char_t *time_str1;

  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  fail_if(time1==NULL);


  status = pdf_time_destroy(time1);
  fail_if(status != PDF_OK);

}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_time_destroy (void)
{
  TCase *tc = tcase_create ("pdf_time_destroy");

  tcase_add_test(tc, pdf_time_destroy_001);

  return tc;
}

/* End of pdf-time-destroy.c */

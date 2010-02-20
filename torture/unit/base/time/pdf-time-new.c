/* -*- mode: C -*-
 *
 *       File:         pdf-time-new.c
 *       Date:         Fri Feb 27 17:35:31 2008
 *
 *       GNU PDF Library - Unit tests for pdf_time_new
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
#include <string.h>

/*
 * Test: pdf_time_new_001
 * Description:
 *   Create new pdf_time_t.
 *   
 *Success condition:
 * 1. Function pdf_time_new schould return PDF_OK
 * 2. New pdf_time_t object is not NUL
 * 3. pdf_time_t is set to 1970-01-01 00:00:00
 * 4. Fuction pdf_time_destroy schould return PDF_OK
 */
START_TEST (pdf_time_new_001)
{
  pdf_status_t status;

  pdf_time_t time1;
  pdf_char_t *time_str1;
  pdf_char_t *expected_time = "1970-01-01T00:00:00Z";

  pdf_init();

  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  fail_if(time1==NULL);

  
  time_str1 = pdf_time_to_string(time1, PDF_TIME_FORMAT_ISO_8601, PDF_FALSE);

  fail_unless(memcmp(time_str1, expected_time, strlen(expected_time)) == 0);

  status = pdf_time_destroy(time1);
  fail_if(status != PDF_OK);

}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_time_new (void)
{
  TCase *tc = tcase_create ("pdf_time_new");

  tcase_add_test(tc, pdf_time_new_001);

  return tc;
}

/* End of pdf-time-new.c */

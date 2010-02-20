/* -*- mode: C -*-
 *
 *       File:         pdf-time-copy.c
 *       Date:         Fri Feb 27 17:35:31 2008
 *
 *       GNU PDF Library - Unit tests for pdf_time_copy
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
 * Test: pdf_time_copy_001
 * Description:
 *   Copy value of pdf_time_t object.
 *Success condition:
 * 1. Function pdf_time_new schould return PDF_OK.
 * 2. Function pdf_time_copy schould return PDF_OK.
 * 3. Structures pdf_time_s schould have the same data.
 */
START_TEST (pdf_time_copy_001)
{
  pdf_status_t status;
  pdf_time_t time1;
  pdf_time_t time2;
  pdf_i64_t seconds;

  pdf_init();

  status = pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  fail_if(time1 == NULL);

  status = pdf_time_new(&time2);
  fail_if(status != PDF_OK);
  fail_if(time2 == NULL);

  seconds = pdf_i64_new(0x12345678,0x9ABCDEF0);

  status = pdf_time_set_from_i64(time1, seconds);
  fail_if(status != PDF_OK);

  status = pdf_time_copy(time1, time2);
  fail_if (status != PDF_OK);
  fail_unless(pdf_time_cmp(time1, time2) == 0);

  status = pdf_time_destroy(time1);
  fail_if(status !=PDF_OK);

  status = pdf_time_destroy(time2);
  fail_if(status !=PDF_OK);

}
END_TEST

/*
 * Test: pdf_time_copy_002
 * Description:
 *   Try to copy pdf_time_t object to object
 *   which os initialized to NUL.
 *Success condition:
 * 1. Function pdf_time_copy schould return NULL
 */
START_TEST (pdf_time_copy_002)
{
  pdf_status_t status;
  pdf_time_t time1;
  pdf_time_t time2;
  time2 = NULL;

  pdf_init();
  
  status =  pdf_time_new(&time1);
  fail_if(status != PDF_OK);
  status  = pdf_time_copy(time1, time2);
  fail_if(status != PDF_ERROR);
}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_time_copy (void)
{
  TCase *tc = tcase_create ("pdf_time_copy");

  tcase_add_test(tc, pdf_time_copy_001);
//  tcase_add_test(tc, pdf_time_copy_002);

  return tc;
}

/* End of pdf-time-copy.c */

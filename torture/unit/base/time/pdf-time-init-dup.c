/* -*- mode: C -*-
 *
 *       File:         pdf-time-init_dup.c
 *       Date:         Thu Mar 24 21:48:07 2011
 *
 *       GNU PDF Library - Unit tests for pdf_time_init_dup
 *
 */

/* Copyright (C) 2011 Free Software Foundation, Inc. */

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
#include <pdf-test-common.h>

/*
 * Test: pdf_time_init_dup_001
 * Description:
 *   Duplicate initialized pdf_time_t object
 *Success condition:
 * 1. Function pdf_time_init should return PDF_OK
 * 2. Function pdf_time_init_dup should return pdf_time_t object.
 * 3. Structures pdf_time_s should have the same data.
 */
START_TEST (pdf_time_init_dup_001)
{
  pdf_time_t time1;
  pdf_time_t time2;

  pdf_time_init (&time1);
  pdf_time_set_utc (&time1, 1234567890);
  pdf_time_init_dup (&time2, &time1);

  fail_unless (pdf_time_cmp (&time1, &time2) == 0);

  pdf_time_deinit (&time1);
  pdf_time_deinit (&time2);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_time_init_dup (void)
{
  TCase *tc = tcase_create ("pdf_time_init_dup");

  tcase_add_test (tc, pdf_time_init_dup_001);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-time-init_dup.c */

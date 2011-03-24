/* -*- mode: C -*-
 *
 *       File:         pdf-time-clear.c
 *       Date:         Fri Feb 27 17:35:31 2008
 *
 *       GNU PDF Library - Unit tests for pdf_time_clear
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
#include <stdlib.h>
#include <pdf-test-common.h>

/*
 * Test: pdf_time_clear_001
 * Description:
 * Create new pdf_time_t object and initialize it with
 * value diffrent to zero.
 * Then clear that object and compare it with
 * newly created empty pdf_time_t.
 *Success condition:
 * 1. Function pdf_time_new schould return PDF_OK
 * 2. Function pdf_time_clear schould return PDF_OK.
 * 3. Cleared object schould be the same as newly created one.
 */
START_TEST (pdf_time_clear_001)
{
  pdf_error_t *error = NULL;
  pdf_time_t time_var;
  pdf_time_t zero_time;

  pdf_time_init (&time_var);
  pdf_time_init (&zero_time);

  pdf_time_set_to_current_local_time (&time_var);

  pdf_time_clear (&time_var);

  fail_unless (pdf_time_cmp (&time_var, &zero_time) == 0);

  pdf_time_deinit (&time_var);
  pdf_time_deinit (&zero_time);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_time_clear (void)
{
  TCase *tc = tcase_create ("pdf_time_clear");

  tcase_add_test (tc, pdf_time_clear_001);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-time-clear.c */

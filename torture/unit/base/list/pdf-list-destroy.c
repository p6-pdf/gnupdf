/* -*- mode: C -*-
 *
 *       File:         pdf-list-destroy.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_destroy
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

#include <stdio.h>
#include <pdf.h>
#include <check.h>
#include <pdf-test-common.h>
#include "pdf-list-test-common.h"

/*
 * Test: pdf_list_destroy_001
 * Description:
 *   Try to destroy an empty list.
 * Success condition:
 *   Doesn't crash.
 */
START_TEST (pdf_list_destroy_001)
{

  pdf_list_destroy (pdf_list_new (NULL, NULL, PDF_FALSE, NULL));
}
END_TEST

/*
 * Test: pdf_list_destroy_002
 * Description:
 *   Try to destroy a NULL list
 * Success condition:
 *   Doesn't crash
 */
START_TEST (pdf_list_destroy_002)
{

  pdf_list_destroy (NULL);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_list_destroy (void)
{
  TCase *tc = tcase_create ("pdf_list_destroy");

  tcase_add_test (tc, pdf_list_destroy_001);
  tcase_add_test (tc, pdf_list_destroy_002);

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-list-destroy.c */

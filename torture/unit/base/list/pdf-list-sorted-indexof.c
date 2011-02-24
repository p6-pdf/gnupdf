/* -*- mode: C -*-
 *
 *       File:         pdf-list-sorted-indexof.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_sorted_indexof
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
 * Test: pdf_list_sorted_indexof_001
 * Description:
 *   Try to get the index of an existent element.
 * Success condition:
 *   Returns the correct index.
 */
START_TEST (pdf_list_sorted_indexof_001)
{
  pdf_list_t *list;
  int elem;
  pdf_size_t pos;

  elem = 2232;

  pdf_init ();

  list = pdf_list_new (l_comp, NULL, PDF_FALSE, NULL);

  pdf_list_sorted_add (list, l_comp_asc, &elem, NULL);

  pos = pdf_list_sorted_indexof (list, l_comp_asc, &elem);
  fail_if (pos == (pdf_size_t)-1);

  pdf_list_destroy (list);
}
END_TEST

/*
 * Test: pdf_list_sorted_indexof_002
 * Description:
 *   Try to get the index of a non-existent element.
 * Success condition:
 *   Returns (pdf_size_t)-1
 */
START_TEST (pdf_list_sorted_indexof_002)
{
  pdf_list_t *list;
  int elem, elem2;
  pdf_size_t pos;

  elem = 2232;
  elem2 = 1234;

  pdf_init ();

  list = pdf_list_new (l_comp, NULL, PDF_FALSE, NULL);

  pdf_list_sorted_add (list, l_comp_asc, &elem, NULL);

  pos = pdf_list_sorted_indexof (list, l_comp_asc, &elem2);
  fail_if (pos != (pdf_size_t)-1);

  pdf_list_destroy (list);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_list_sorted_indexof (void)
{
  TCase *tc = tcase_create ("pdf_list_sorted_indexof");

  tcase_add_test (tc, pdf_list_sorted_indexof_001);
  tcase_add_test (tc, pdf_list_sorted_indexof_002);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-list-sorted-indexof.c */

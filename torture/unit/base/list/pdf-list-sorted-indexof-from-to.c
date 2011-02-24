/* -*- mode: C -*-
 *
 *       File:         pdf-list-sorted-indexof-from-to.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_sorted_indexof_from_to
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
 * Test: pdf_list_sorted_indexof_from_to_001
 * Description:
 *   Try to get the index of a existent element from position '0'.
 * Success condition:
 *   Returns the correct index.
 */
START_TEST (pdf_list_sorted_indexof_from_to_001)
{
  pdf_list_t *list;
  int elem;
  pdf_error_t *error = NULL;
  pdf_size_t pos;

  elem = 2232;

  pdf_init ();

  list = pdf_list_new (l_comp, NULL, PDF_FALSE, NULL);

  pdf_list_sorted_add (list, l_comp_asc, &elem, NULL);

  pos = pdf_list_sorted_indexof_from_to (list, l_comp_asc, 0, 1, &elem, &error);
  fail_if (pos == (pdf_size_t)-1);
  fail_if (error != NULL);

  pdf_list_destroy (list);
}
END_TEST

/*
 * Test: pdf_list_sorted_indexof_from_to_002
 * Description:
 *   Try to get the index of a existent element from invalid position.
 * Success condition:
 *   Returns PDF_EINVRANGE
 */
START_TEST (pdf_list_sorted_indexof_from_to_002)
{
  pdf_list_t *list;
  int elem;
  pdf_error_t *error = NULL;
  pdf_size_t pos;

  elem = 2232;

  pdf_init ();

  list = pdf_list_new (l_comp, NULL, PDF_FALSE, NULL);

  pdf_list_sorted_add (list, l_comp_asc, &elem, NULL);

  pos = pdf_list_sorted_indexof_from_to (list, l_comp_asc, 0, 2, &elem, &error);
  fail_if (pos != (pdf_size_t)-1);
  fail_if (error == NULL);
  fail_if (pdf_error_get_status (error) != PDF_EINVRANGE);

  pdf_clear_error (&error);

  pos = pdf_list_sorted_indexof_from_to (list, l_comp_asc, 2, 0, &elem, &error);
  fail_if (pos != (pdf_size_t)-1);
  fail_if (error == NULL);
  fail_if (pdf_error_get_status (error) != PDF_EINVRANGE);

  pdf_error_destroy (error);
  pdf_list_destroy (list);
}
END_TEST

/*
 * Test: pdf_list_sorted_indexof_from_to_003
 * Description:
 *   Try to get the index of a non-existent element.
 * Success condition:
 *   Returns (pdf_size_t)-1
 */
START_TEST (pdf_list_sorted_indexof_from_to_003)
{
  pdf_list_t *list;
  int elem, elem2;
  pdf_error_t *error = NULL;
  pdf_size_t pos;

  elem = 2232;
  elem2 = 1234;

  pdf_init ();

  list = pdf_list_new (l_comp, NULL, PDF_FALSE, NULL);

  pdf_list_sorted_add (list, l_comp_asc, &elem, NULL);

  pos = pdf_list_sorted_indexof_from_to (list, l_comp_asc, 0, 1, &elem2, &error);
  fail_if (pos != (pdf_size_t)-1);
  fail_if (error != NULL);

  pdf_list_destroy (list);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_list_sorted_indexof_from_to (void)
{
  TCase *tc = tcase_create ("pdf_list_sorted_indexof_from_to");

  tcase_add_test (tc, pdf_list_sorted_indexof_from_to_001);
  tcase_add_test (tc, pdf_list_sorted_indexof_from_to_002);
  tcase_add_test (tc, pdf_list_sorted_indexof_from_to_003);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-list-sorted-indexof-from-to.c */

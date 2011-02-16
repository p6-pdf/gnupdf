/* -*- mode: C -*-
 *
 *       File:         pdf-list-search-from-to.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_search_from_to
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

#include "pdf-list-test-common.h"

/*
 * Test: pdf_list_search_from_to_001
 * Description:
 *   Try to search an existent element in the correct range.
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_list_search_from_to_001)
{
  pdf_list_t *list;
  int elem;
  pdf_list_node_t *node;
  pdf_error_t *error = NULL;

  elem = 2232;

  pdf_init ();

  list = pdf_list_new (l_comp, NULL, 0, NULL);

  pdf_list_add_last (list, &elem, NULL);

  node = pdf_list_search_from_to (list, 0, 1, &elem, &error);
  fail_if (node == NULL);
  fail_if (error != NULL);

  pdf_list_destroy (list);
}
END_TEST

/*
 * Test: pdf_list_search_from_to_002
 * Description:
 *   Try to seach an element in an invalid range.
 * Success condition:
 *   Returns PDF_EINVRANGE.
 */
START_TEST (pdf_list_search_from_to_002)
{
  pdf_list_t *list;
  int elem;
  pdf_list_node_t *node;
  pdf_error_t *error = NULL;

  elem = 2232;

  pdf_init ();

  list = pdf_list_new (l_comp, NULL, 0, NULL);

  pdf_list_add_last (list, &elem, NULL);

  node = pdf_list_search_from_to (list, 0, 3, &elem, &error);
  fail_if (node != NULL);
  fail_if (error == NULL);
  fail_if (pdf_error_get_status (error) != PDF_EINVRANGE);

  pdf_list_destroy (list);
}
END_TEST

/*
 * Test: pdf_list_search_from_to_003
 * Description:
 *   Try to search a non-existent element in a list.
 * Success condition:
 *   Returns PDF_ENONODE.
 */
START_TEST (pdf_list_search_from_to_003)
{
  pdf_list_t *list;
  int elem, elem2;
  pdf_list_node_t *node;
  pdf_error_t *error = NULL;

  elem = 2232;
  elem2 = 1223;

  pdf_init ();

  list = pdf_list_new (l_comp, NULL, 0, NULL);

  pdf_list_add_last (list, &elem, NULL);

  node = pdf_list_search_from_to (list, 0, 1, &elem2, &error);
  fail_if (node != NULL);
  fail_if (error != NULL);

  pdf_list_destroy (list);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_list_search_from_to (void)
{
  TCase *tc = tcase_create ("pdf_list_search_from_to");

  tcase_add_test (tc, pdf_list_search_from_to_001);
  tcase_add_test (tc, pdf_list_search_from_to_002);
  tcase_add_test (tc, pdf_list_search_from_to_003);

  return tc;
}

/* End of pdf-list-search-from-to.c */

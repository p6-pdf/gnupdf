/* -*- mode: C -*-
 *
 *       File:         pdf-list-set-at.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_set_at
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
 * Test: pdf_list_set_at_001
 * Description:
 *   Try to replace a node value in a list.
 * Success condition:
 *   Returns the newly created node.
 */
START_TEST (pdf_list_set_at_001)
{
  pdf_list_t *list;
  int elem, elem2;
  pdf_list_node_t *node;
  pdf_error_t *error = NULL;

  elem = 1123;
  elem2 = 3323;


  list = pdf_list_new (NULL, NULL, PDF_FALSE, NULL);

  pdf_list_add_last (list, &elem, NULL);

  node = pdf_list_set_at (list, 0, &elem2, &error);
  fail_if (node == NULL);
  fail_if (error != NULL);

  pdf_list_destroy (list);
}
END_TEST

/*
 * Test: pdf_list_set_at_002
 * Description:
 *   Try to replace a node value at an invalid position.
 * Success condition:
 *   Returns PDF_EINVRANGE
 */
START_TEST (pdf_list_set_at_002)
{
  pdf_list_t *list;
  int elem, elem2;
  pdf_list_node_t *node;
  pdf_error_t *error = NULL;

  elem = 1123;
  elem2 = 3323;


  list = pdf_list_new (NULL, NULL, PDF_FALSE, NULL);

  pdf_list_add_last (list, &elem, NULL);

  node = pdf_list_set_at (list, 6, &elem2, &error);
  fail_if (node != NULL);
  fail_if (error == NULL);
  fail_if (pdf_error_get_status (error) != PDF_EINVRANGE);

  pdf_error_destroy (error);
  pdf_list_destroy (list);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_list_set_at (void)
{
  TCase *tc = tcase_create ("pdf_list_set_at");

  tcase_add_test (tc, pdf_list_set_at_001);
  tcase_add_test (tc, pdf_list_set_at_002);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-list-set-at.c */

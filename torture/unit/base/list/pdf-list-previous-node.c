/* -*- mode: C -*-
 *
 *       File:         pdf-list-previous-node.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_previous_node
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
 * Test: pdf_list_previous_node_001
 * Description:
 *   Try to get the previous node given another node.
 * Success condition:
 *   Returns the previous node.
 */
START_TEST (pdf_list_previous_node_001)
{
  pdf_list_t *list;
  pdf_list_node_t *node, *prev;
  int elem, elem2;

  elem = 222;
  elem2 = 333;


  list = pdf_list_new (NULL, NULL, PDF_FALSE, NULL);

  pdf_list_add_last (list, &elem, NULL);
  node = pdf_list_add_last (list, &elem2, NULL);

  prev = pdf_list_previous_node (list, node);
  fail_if (prev == NULL);

  fail_if (*((int*) pdf_list_node_value (list, prev)) != 222);

  pdf_list_destroy (list);
}
END_TEST

/*
 * Test: pdf_list_previous_node_002
 * Description:
 *   Try to get the previous node given the first node.
 * Success condition:
 *   Returns NULL
 */
START_TEST (pdf_list_previous_node_002)
{
  pdf_list_t *list;
  pdf_list_node_t *node, *prev;
  int elem;

  elem = 222;


  list = pdf_list_new (NULL, NULL, PDF_FALSE, NULL);

  node = pdf_list_add_last (list, &elem, NULL);

  prev = pdf_list_previous_node (list, node);
  fail_if (prev != NULL);

  pdf_list_destroy (list);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_list_previous_node (void)
{
  TCase *tc = tcase_create ("pdf_list_previous_node");

  tcase_add_test (tc, pdf_list_previous_node_001);
  tcase_add_test (tc, pdf_list_previous_node_002);

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-list-previous-node.c */

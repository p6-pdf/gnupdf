/* -*- mode: C -*-
 *
 *       File:         pdf-list-add-last.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_add_last
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
 * Test: pdf_list_add_last_001
 * Description:
 *   Try to add an element at the end of the list.
 * Success condition:
 *   We get the right pdf_list_size().
 */
START_TEST (pdf_list_add_last_001)
{
  pdf_list_node_t *node;
  pdf_list_t *list;
  int elem, elem2;
  pdf_error_t *error = NULL;

  elem = 5123;
  elem2 = 5431;

  pdf_init ();

  list = pdf_list_new (l_comp, NULL, PDF_FALSE, NULL);

  node = pdf_list_add_last (list, &elem, &error);
  fail_if (node == NULL);
  fail_if (error != NULL);
  fail_if (pdf_list_size(list) != 1);

  /* Should fail as duplicates not allowed */
  node = pdf_list_add_last (list, &elem, &error);
  fail_if (node != NULL);
  fail_if (error == NULL);
  fail_if (pdf_error_get_status (error) != PDF_EEXIST);
  fail_if (pdf_list_size(list) != 1);

  node = pdf_list_add_last (list, &elem2, &error);
  fail_if (node == NULL);
  fail_if (error != NULL);
  fail_if (pdf_list_size(list) != 2);

  pdf_list_destroy (list);
}
END_TEST

/*
 * Test: pdf_list_add_last_002
 * Description:
 *   Try to add an element at the end of the list allowing duplicates.
 * Success condition:
 *   We get the right pdf_list_size().
 */
START_TEST (pdf_list_add_last_002)
{
  pdf_list_node_t *node;
  pdf_list_t *list;
  int elem;
  pdf_error_t *error = NULL;

  elem = 5123;

  pdf_init ();

  list = pdf_list_new (l_comp, NULL, PDF_FALSE, NULL);

  node = pdf_list_add_last (list, &elem, &error);
  fail_if (node == NULL);
  fail_if (error != NULL);
  fail_if (pdf_list_size(list) != 1);

  node = pdf_list_add_last (list, &elem, &error);
  fail_if (node == NULL);
  fail_if (error != NULL);
  fail_if (pdf_list_size(list) != 2);

  pdf_list_destroy (list);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_list_add_last (void)
{
  TCase *tc = tcase_create ("pdf_list_add_last");

  tcase_add_test (tc, pdf_list_add_last_001);
  tcase_add_test (tc, pdf_list_add_last_002);

  return tc;
}

/* End of pdf-list-add-last.c */

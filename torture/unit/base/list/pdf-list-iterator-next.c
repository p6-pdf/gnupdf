/* -*- mode: C -*-
 *
 *       File:         pdf-list-iterator-next.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_iterator_next
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
 * Test: pdf_list_iterator_next_001
 * Description:
 *   Try to get the next element using an iterator.
 * Success condition:
 *   Returns PDF_TRUE
 */
START_TEST (pdf_list_iterator_next_001)
{
  pdf_list_t *list;
  pdf_list_iterator_t itr;
  int elem, *next;
  pdf_list_node_t *next_node;

  elem = 222;


  list = pdf_list_new (NULL, NULL, PDF_FALSE, NULL);
  pdf_list_add_last (list, &elem, NULL);

  pdf_list_iterator_init (&itr, list);

  fail_if (pdf_list_iterator_next (&itr,
                                   (const void **) &next,
                                   &next_node) != PDF_TRUE);
  fail_if (next == NULL);
  fail_if (next_node == NULL);

  pdf_list_iterator_deinit (&itr);
  pdf_list_destroy (list);
}
END_TEST

/*
 * Test: pdf_list_iterator_next_002
 * Description:
 *   Try to get the next element using an iterator from an empty list.
 * Success condition:
 *   Returns PDF_FALSE and no next element
 */
START_TEST (pdf_list_iterator_next_002)
{
  pdf_list_t *list;
  pdf_list_iterator_t itr;
  int *next;
  pdf_list_node_t *next_node;


  list = pdf_list_new (NULL, NULL, PDF_FALSE, NULL);

  pdf_list_iterator_init (&itr, list);

  fail_if (pdf_list_iterator_next (&itr,
                                   (const void **) &next,
                                   &next_node) == PDF_TRUE);
  fail_if (next != NULL);
  fail_if (next_node != NULL);

  pdf_list_iterator_deinit (&itr);
  pdf_list_destroy (list);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_list_iterator_next (void)
{
  TCase *tc = tcase_create ("pdf_list_iterator_next");

  tcase_add_test (tc, pdf_list_iterator_next_001);
  tcase_add_test (tc, pdf_list_iterator_next_002);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-list-iterator-next.c */

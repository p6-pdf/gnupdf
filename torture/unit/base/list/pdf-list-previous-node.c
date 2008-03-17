/* -*- mode: C -*- Time-stamp: "08/03/17 14:24:33 jemarch"
 *
 *       File:         pdf-list-previous-node.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_previous_node
 *
 */

/* Copyright (C) 2008 Free Software Foundation, Inc. */

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

/*
 * Test: pdf_list_previous_node_001
 * Description:
 *   Try to get the previous node given another node.
 * Success condition:
 *   Returns PDF_OK.
 */
START_TEST (pdf_list_previous_node_001)
{
  pdf_list_t list;
  pdf_list_node_t node,prev;
  int elem, elem2;
  pdf_status_t st;
  
  elem = 222;
  elem2 = 333;

  pdf_list_create (NULL, NULL, 0, &list);

  pdf_list_add_last (list, &elem);
  node = pdf_list_add_last (list, &elem2);

  st = pdf_list_previous_node (list, node, &prev);

  fail_if (st != PDF_OK);
  fail_if (*((int*) pdf_list_node_value (list, prev)) != 222);

  pdf_list_destroy (list);
}
END_TEST


/*
 * Test: pdf_list_previous_node_002
 * Description:
 *   Try to get the previous node given the first node.
 * Success condition:
 *   Returns PDF_ENONODE.
 */
START_TEST (pdf_list_previous_node_002)
{
  pdf_list_t list;
  pdf_list_node_t node,prev;
  int elem;
  pdf_status_t st;
  
  elem = 222;

  pdf_list_create (NULL, NULL, 0, &list);

  node = pdf_list_add_last (list, &elem);

  st = pdf_list_previous_node (list, node, &prev);

  fail_if (st != PDF_ENONODE);

  pdf_list_destroy (list);
}
END_TEST


/*
 * Test: pdf_list_previous_node_003
 * Description:
 *   Try to get the previous node given a NULL prev pointer.
 * Success condition:
 *   Returns PDF_EBADDATA.
 */
START_TEST (pdf_list_previous_node_003)
{
  pdf_list_t list;
  pdf_list_node_t node;
  int elem;
  pdf_status_t st;
  
  elem = 222;

  pdf_list_create (NULL, NULL, 0, &list);

  node = pdf_list_add_last (list, &elem);

  st = pdf_list_previous_node (list, node, NULL);

  fail_if (st != PDF_EBADDATA);

  pdf_list_destroy (list);
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_list_previous_node (void)
{
  TCase *tc = tcase_create("pdf_list_previous_node");
  tcase_add_test(tc, pdf_list_previous_node_001);
  tcase_add_test(tc, pdf_list_previous_node_002);
  tcase_add_test(tc, pdf_list_previous_node_003);

  return tc;
}

/* End of pdf-list-previous-node.c */

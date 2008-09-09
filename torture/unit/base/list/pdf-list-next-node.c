/* -*- mode: C -*- Time-stamp: "08/09/08 22:52:45 jemarch"
 *
 *       File:         pdf-list-next-node.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_next_node
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
 * Test: pdf_list_next_node_001
 * Description:
 *   Try to get the next node given another node.
 * Success condition:
 *   Returns PDF_OK.
 */
START_TEST (pdf_list_next_node_001)
{
  pdf_list_t list;
  pdf_list_node_t node,next;
  int elem, elem2;
  pdf_status_t st;
  
  elem = 222;
  elem2 = 333;

  pdf_list_new (NULL, NULL, 0, &list);

  pdf_list_add_last (list, &elem, &node);
  pdf_list_add_last (list, &elem2, NULL);

  st = pdf_list_next_node (list, node, &next);

  fail_if (st != PDF_OK);
  fail_if (*((int*) pdf_list_node_value (list, next)) != 333);

  pdf_list_destroy (list);
}
END_TEST


/*
 * Test: pdf_list_next_node_002
 * Description:
 *   Try to get the next node given the last node in the list.
 * Success condition:
 *   Returns PDF_ENONODE
 */
START_TEST (pdf_list_next_node_002)
{
  pdf_list_t list;
  pdf_list_node_t node, next;
  int elem;
  pdf_status_t st;
  
  elem = 222;

  pdf_list_new (NULL, NULL, 0, &list);

  pdf_list_add_first (list, &elem, &node);

  st = pdf_list_next_node (list, node, &next);
  fail_if (st != PDF_ENONODE);

  pdf_list_destroy (list);
}
END_TEST



/*
 * Test: pdf_list_next_node_003
 * Description:
 *   Try to get the next node given a NULL next pointer.
 * Success condition:
 *   Returns PDF_EBADDATA.
 */
START_TEST (pdf_list_next_node_003)
{
  pdf_list_t list;
  pdf_list_node_t node;
  int elem;
  pdf_status_t st;
  
  elem = 222;

  pdf_list_new (NULL, NULL, 0, &list);

  pdf_list_add_last (list, &elem, &node);


  st = pdf_list_next_node (list, node, NULL);

  fail_if (st != PDF_EBADDATA);

  pdf_list_destroy (list);
}
END_TEST




/*
 * Test case creation function
 */
TCase *
test_pdf_list_next_node (void)
{
  TCase *tc = tcase_create("pdf_list_next_node");
  tcase_add_test(tc, pdf_list_next_node_001);
  tcase_add_test(tc, pdf_list_next_node_002);
  tcase_add_test(tc, pdf_list_next_node_003);

  return tc;
}

/* End of pdf-list-next-node.c */

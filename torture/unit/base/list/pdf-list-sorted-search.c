/* -*- mode: C -*- Time-stamp: "08/09/08 22:56:56 jemarch"
 *
 *       File:         pdf-list-sorted-search.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_sorted_search
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

#include <stdbool.h>
#include <stdio.h>
#include <pdf.h>
#include <check.h>

extern bool l_comp (const void * elemb, const void * elema);
extern int l_comp_desc (const void *elema, const void * elemb);
extern int l_comp_asc (const void *elema, const void * elemb);

/*
 * Test: pdf_list_sorted_search_001
 * Description:
 *   Try to search for an existing element in a list.
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_list_sorted_search_001)
{
  pdf_list_t list;
  int elem;
  pdf_status_t st;
  pdf_list_node_t node;

  elem = 2232;
  
  pdf_list_new (l_comp, NULL, 0, &list);
  pdf_list_sorted_add (list, l_comp_asc, &elem, NULL);

  st = pdf_list_sorted_search (list, l_comp_asc, &elem, &node);

  fail_if (st != PDF_OK);

  pdf_list_destroy (list);
}
END_TEST



/*
 * Test: pdf_list_sorted_search_002
 * Description:
 *   Try to search an existent element given a NULL node pointer.
 * Success condition:
 *   Returns PDF_EBADDATA
 */
START_TEST (pdf_list_sorted_search_002)
{
  pdf_list_t list;
  int elem;
  pdf_status_t st;

  elem = 2232;
  
  pdf_list_new (l_comp, NULL, 0, &list);
  pdf_list_sorted_add (list, l_comp_asc, &elem, NULL);

  st = pdf_list_sorted_search (list, l_comp_asc, &elem, NULL);

  fail_if (st != PDF_EBADDATA);

  pdf_list_destroy (list);
}
END_TEST


/*
 * Test: pdf_list_sorted_search_003
 * Description:
 *   Try to search for a non-existent element in a list.
 * Success condition:
 *   Returns PDF_ENONODE
 */
START_TEST (pdf_list_sorted_search_003)
{
  pdf_list_t list;
  int elem, elem2;
  pdf_list_node_t node;
  pdf_status_t st;

  elem = 2232;
  elem2 = 1223;
  
  pdf_list_new (l_comp, NULL, 0, &list);
  pdf_list_sorted_add (list, l_comp_asc, &elem, NULL);

  st = pdf_list_sorted_search (list, l_comp_asc, &elem2, &node);

  fail_if (st != PDF_ENONODE);

  pdf_list_destroy (list);
}
END_TEST



/*
 * Test: pdf_list_sorted_search_004
 * Description:
 *   Try to search an existent element given a NULL compar_fn.
 * Success condition:
 *   Returns PDF_EBADDATA
 */
START_TEST (pdf_list_sorted_search_004)
{
  pdf_list_t list;
  int elem;
  pdf_status_t st;
  pdf_list_node_t node;

  elem = 2232;
  
  pdf_list_new (l_comp, NULL, 0, &list);
  pdf_list_sorted_add (list, l_comp_asc, &elem, NULL);

  st = pdf_list_sorted_search (list, NULL, &elem, &node);

  fail_if (st != PDF_EBADDATA);

  pdf_list_destroy (list);
}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_list_sorted_search (void)
{
  TCase *tc = tcase_create("pdf_list_sorted_search");
  tcase_add_test(tc, pdf_list_sorted_search_001);
  tcase_add_test(tc, pdf_list_sorted_search_002);
  tcase_add_test(tc, pdf_list_sorted_search_003);
  tcase_add_test(tc, pdf_list_sorted_search_004);

  return tc;
}

/* End of pdf-list-sorted-search.c */

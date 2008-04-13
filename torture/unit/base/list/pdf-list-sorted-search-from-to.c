/* -*- mode: C -*- Time-stamp: "2008-04-13 15:29:11 gerel"
 *
 *       File:         pdf-list-sorted-search-from-to.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_sorted_search_from_to
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
 * Test: pdf_list_sorted_search_from_to_001
 * Description:
 *   Try to search an existent element in the correct range.
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_list_sorted_search_from_to_001)
{
  pdf_list_t list;
  int elem;
  pdf_status_t st;
  pdf_list_node_t node;

  elem = 2232;
    
  pdf_list_create (l_comp, NULL, 0, &list);
  pdf_list_sorted_add (list, l_comp_asc, &elem, NULL);

  st = pdf_list_sorted_search_from_to (list, l_comp_asc, 0, 1, &elem, &node);

  fail_if (st != PDF_OK);

  pdf_list_destroy (list);
}
END_TEST

/*
 * Test: pdf_list_sorted_search_from_to_002
 * Description:
 *   Try to seach an element in an invalid range.
 * Success condition:
 *   Returns PDF_EINVRANGE.
 */
START_TEST (pdf_list_sorted_search_from_to_002)
{
  pdf_list_t list;
  int elem;
  pdf_status_t st;
  pdf_list_node_t node;

  elem = 2232;
  
  pdf_list_create (l_comp, NULL, 0, &list);
  pdf_list_sorted_add (list, l_comp_asc, &elem, NULL);

  st = pdf_list_sorted_search_from_to (list, l_comp_asc, 0, 5, &elem, &node);

  fail_if (st != PDF_EINVRANGE);

  pdf_list_destroy (list);
}
END_TEST

/*
 * Test: pdf_list_sorted_search_from_to_003
 * Description:
 *   Try to search a non-existent element in a list.
 * Success condition:
 *   Returns PDF_ENONODE.
 */
START_TEST (pdf_list_sorted_search_from_to_003)
{
  pdf_list_t list;
  int elem, elem2;
  pdf_status_t st;
  pdf_list_node_t node;

  elem = 2232;
  elem2 = 232323;

  pdf_list_create (l_comp, NULL, 0, &list);
  pdf_list_sorted_add (list, l_comp_asc, &elem, NULL);

  st = pdf_list_sorted_search_from_to (list, l_comp_asc, 0, 1, &elem2, &node);

  fail_if (st != PDF_ENONODE);

  pdf_list_destroy (list);
}
END_TEST

/*
 * Test: pdf_list_sorted_search_from_to_004
 * Description:
 *   Try search an element given a NULL node pointer.
 * Success condition:
 *   Returns PDF_EBADDATA
 */
START_TEST (pdf_list_sorted_search_from_to_004)
{
  pdf_list_t list;
  int elem;
  pdf_status_t st;

  elem = 2232;
  
  pdf_list_create (l_comp, NULL, 0, &list);
  pdf_list_sorted_add (list, l_comp_asc, &elem, NULL);

  st = pdf_list_sorted_search_from_to (list, l_comp_asc, 0, 1, &elem, NULL);

  fail_if (st != PDF_EBADDATA);

  pdf_list_destroy (list);
}
END_TEST

/*
 * Test: pdf_list_sorted_search_from_to_005
 * Description:
 *   Try search an element given a NULL compar_fn.
 * Success condition:
 *   Returns PDF_EBADDATA
 */
START_TEST (pdf_list_sorted_search_from_to_005)
{
  pdf_list_t list;
  int elem;
  pdf_status_t st;
  pdf_list_node_t node;
  
  elem = 2232;
  
  pdf_list_create (l_comp, NULL, 0, &list);
  pdf_list_sorted_add (list, l_comp_asc, &elem, NULL);

  st = pdf_list_sorted_search_from_to (list, NULL, 0, 1, &elem, &node);

  fail_if (st != PDF_EBADDATA);

  pdf_list_destroy (list);
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_list_sorted_search_from_to (void)
{
  TCase *tc = tcase_create("pdf_list_sorted_search_from_to");
  tcase_add_test(tc, pdf_list_sorted_search_from_to_001);
  tcase_add_test(tc, pdf_list_sorted_search_from_to_002);
  tcase_add_test(tc, pdf_list_sorted_search_from_to_003);
  tcase_add_test(tc, pdf_list_sorted_search_from_to_004);
  tcase_add_test(tc, pdf_list_sorted_search_from_to_005);

  return tc;
}

/* End of pdf-list-sorted-search-from-to.c */

/* -*- mode: C -*-
 *
 *       File:         pdf-list-sorted-add.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_sorted_add
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
 * Test: pdf_list_sorted_add_001
 * Description:
 *   Try to add an element at the beginning of the list.
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_list_sorted_add_001)
{
  pdf_list_t list;
  int elem;
  pdf_status_t st;
  
  elem = 5123;

  pdf_init();

  pdf_list_new (l_comp, NULL, 0, &list);
  st = pdf_list_sorted_add (list, l_comp_asc, &elem, NULL);
  fail_if (st != PDF_OK);

  pdf_list_destroy (list);
}
END_TEST


/*
 * Test: pdf_list_sorted_add_002
 * Description:
 *   Try to add an element at the beginning of the list with NULL compar_fn.
 * Success condition:
 *   Returns PDF_EBADDATA
 */
START_TEST (pdf_list_sorted_add_002)
{
  pdf_list_t list;
  int elem;
  pdf_status_t st;
  
  elem = 5123;

  pdf_init();

  pdf_list_new (l_comp, NULL, 0, &list);
  st = pdf_list_sorted_add (list, NULL, &elem, NULL);
  fail_if (st != PDF_EBADDATA);

  pdf_list_destroy (list);
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_list_sorted_add (void)
{
  TCase *tc = tcase_create("pdf_list_sorted_add");
  tcase_add_test(tc, pdf_list_sorted_add_001);
  tcase_add_test(tc, pdf_list_sorted_add_002);

  return tc;
}

/* End of pdf-list-sorted-add.c */

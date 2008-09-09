/* -*- mode: C -*- Time-stamp: "08/09/08 22:54:10 jemarch"
 *
 *       File:         pdf-list-remove.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_remove
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

/*
 * Test: pdf_list_remove_001
 * Description:
 *   Try to remove an existing element in a list.
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_list_remove_001)
{
  pdf_list_t list;
  int elem;
  pdf_status_t st;

  elem = 1212;

  pdf_list_new (NULL, NULL, 0, &list);
  pdf_list_add_first (list, &elem, NULL);
  st = pdf_list_remove (list, &elem);

  fail_if (st != PDF_OK);

  pdf_list_destroy (list);
}
END_TEST


/*
 * Test: pdf_list_remove_002
 * Description:
 *   Try to remove an non-existent element.
 * Success condition:
 *   Returns PDF_ENONODE
 */
START_TEST (pdf_list_remove_002)
{
  pdf_list_t list;
  int elem,elem2;
  pdf_status_t st;

  elem = 1212;
  elem2 = 3333;
  
  pdf_list_new (l_comp, NULL, 0, &list);
  pdf_list_add_first (list, &elem, NULL);
  st = pdf_list_remove (list, &elem2);

  fail_if (st != PDF_ENONODE);

  pdf_list_destroy (list);


}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_list_remove (void)
{
  TCase *tc = tcase_create("pdf_list_remove");
  tcase_add_test(tc, pdf_list_remove_001);
  tcase_add_test(tc, pdf_list_remove_002);

  return tc;
}

/* End of pdf-list-remove.c */

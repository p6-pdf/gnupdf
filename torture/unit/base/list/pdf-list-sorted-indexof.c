/* -*- mode: C -*-
 *
 *       File:         pdf-list-sorted-indexof.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_sorted_indexof
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
 * Test: pdf_list_sorted_indexof_001
 * Description:
 *   Try to get the index of an existent element.
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_list_sorted_indexof_001)
{
  pdf_list_t list;
  int elem;
  pdf_size_t pos;
  pdf_status_t st;

  elem = 2121;

  pdf_init();

  pdf_list_new (NULL, NULL, 0, &list);

  pdf_list_sorted_add (list, l_comp_asc, &elem, NULL);

  st = pdf_list_sorted_indexof (list, l_comp_asc, &elem, &pos);

  fail_if (st != PDF_OK);

  pdf_list_destroy (list);
}
END_TEST


/*
 * Test: pdf_list_sorted_indexof_002
 * Description:
 *   Try to get the index of a non-existent element.
 * Success condition:
 *   Returns PDF_ENONODE
 */
START_TEST (pdf_list_sorted_indexof_002)
{
  pdf_list_t list;
  int elem;
  pdf_size_t pos;
  pdf_status_t st;

  elem = 2121;

  pdf_init();

  pdf_list_new (NULL, NULL, 0, &list);

  st = pdf_list_sorted_indexof (list, l_comp_asc, &elem, &pos);

  fail_if (st != PDF_ENONODE);

  pdf_list_destroy (list);
}
END_TEST


/*
 * Test: pdf_list_sorted_indexof_003
 * Description:
 *   Try to get the index of an element given a NULL position pointer.
 * Success condition:
 *   Returns PDF_EBADDATA
 */
START_TEST (pdf_list_sorted_indexof_003)
{
  pdf_list_t list;
  int elem;
  pdf_status_t st;

  elem = 2121;

  pdf_init();

  pdf_list_new (NULL, NULL, 0, &list);

  st = pdf_list_sorted_indexof (list, l_comp_asc, &elem, NULL);

  fail_if (st != PDF_EBADDATA);

  pdf_list_destroy (list);
}
END_TEST


/*
 * Test: pdf_list_sorted_indexof_004
 * Description:
 *   Try to get the index of an element given a NULL compar_fn.
 * Success condition:
 *   Returns PDF_EBADDATA
 */
START_TEST (pdf_list_sorted_indexof_004)
{
  pdf_list_t list;
  int elem;
  pdf_size_t pos;
  pdf_status_t st;

  elem = 2121;

  pdf_init();

  pdf_list_new (NULL, NULL, 0, &list);

  st = pdf_list_sorted_indexof (list, NULL, &elem, &pos);

  fail_if (st != PDF_EBADDATA);

  pdf_list_destroy (list);
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_list_sorted_indexof (void)
{
  TCase *tc = tcase_create("pdf_list_sorted_indexof");
  tcase_add_test(tc, pdf_list_sorted_indexof_001);
  tcase_add_test(tc, pdf_list_sorted_indexof_002);
  tcase_add_test(tc, pdf_list_sorted_indexof_003);
  tcase_add_test(tc, pdf_list_sorted_indexof_004);

  return tc;
}

/* End of pdf-list-sorted-indexof.c */

/* -*- mode: C -*- Time-stamp: "08/03/17 14:22:24 jemarch"
 *
 *       File:         pdf-list-indexof-from-to.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_indexof_from_to
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
 * Test: pdf_list_indexof_from_to_001
 * Description:
 *   Try to get the index of a existent element from position '0'.
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_list_indexof_from_to_001)
{
  pdf_list_t list;
  int elem;
  pdf_size_t pos;
  pdf_status_t st;
  
  elem = 2121;

  pdf_list_create (NULL, NULL, 0, &list);
  
  pdf_list_add_first (list, &elem);

  st = pdf_list_indexof_from_to (list, 0, 1, &elem, &pos);

  fail_if (st != PDF_OK);

  pdf_list_destroy (list);
}
END_TEST



/*
 * Test: pdf_list_indexof_from_to_002
 * Description:
 *   Try to get the index of a existent element from invalid position.
 * Success condition:
 *   Returns PDF_EINVRANGE
 */
START_TEST (pdf_list_indexof_from_to_002)
{
  pdf_list_t list;
  int elem;
  pdf_size_t pos;
  pdf_status_t st;
  
  elem = 2121;

  pdf_list_create (NULL, NULL, 0, &list);
  
  pdf_list_add_first (list, &elem);

  st = pdf_list_indexof_from_to (list, 0, 2, &elem, &pos);
  fail_if (st != PDF_EINVRANGE);

  st = pdf_list_indexof_from_to (list, -2, 1, &elem, &pos);
  fail_if (st != PDF_EINVRANGE);

  st = pdf_list_indexof_from_to (list, 1, -1, &elem, &pos);
  fail_if (st != PDF_EINVRANGE);

  pdf_list_destroy (list);
}
END_TEST


/*
 * Test: pdf_list_indexof_from_to_003
 * Description:
 *   Try to get the index of a existent element given a NULL position pointer.
 * Success condition:
 *   Returns PDF_EBADDATA
 */
START_TEST (pdf_list_indexof_from_to_003)
{
  pdf_list_t list;
  int elem;
  pdf_status_t st;
  
  elem = 2121;

  pdf_list_create (NULL, NULL, 0, &list);
  
  pdf_list_add_first (list, &elem);

  st = pdf_list_indexof_from_to (list, 0, 1, &elem, NULL);
  fail_if (st != PDF_EBADDATA);

  pdf_list_destroy (list);
}
END_TEST


/*
 * Test: pdf_list_indexof_from_to_004
 * Description:
 *   Try to get the index of a non-existent element.
 * Success condition:
 *   Returns PDF_ENONODE
 */
START_TEST (pdf_list_indexof_from_to_004)
{
  pdf_list_t list;
  int elem, elem2;
  pdf_size_t pos;
  pdf_status_t st;
  
  elem = 2121;
  elem2 = 2222;
  
  pdf_list_create (NULL, NULL, 0, &list);
  
  pdf_list_add_last (list, &elem);

  st = pdf_list_indexof_from_to (list, 0, 1, &elem2, &pos);
  fail_if (st != PDF_ENONODE);

  pdf_list_destroy (list);
}
END_TEST




/*
 * Test case creation function
 */
TCase *
test_pdf_list_indexof_from_to (void)
{
  TCase *tc = tcase_create("pdf_list_indexof_from_to");
  tcase_add_test(tc, pdf_list_indexof_from_to_001);
  tcase_add_test(tc, pdf_list_indexof_from_to_002);
  tcase_add_test(tc, pdf_list_indexof_from_to_003);
  tcase_add_test(tc, pdf_list_indexof_from_to_004);

  return tc;
}

/* End of pdf-list-indexof-from-to.c */

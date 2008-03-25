/* -*- mode: C -*- Time-stamp: "2008-03-18 15:19:40 gerel"
 *
 *       File:         pdf-list-iterator-from-to.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_iterator_from_to
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
 * Test: pdf_list_iterator_from_to_001
 * Description:
 *   Try to get an iterator in a given range.
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_list_iterator_from_to_001)
{
  pdf_list_t list;
  int elem, elem2;
  pdf_list_iterator_t itr;
  pdf_status_t st;

  elem = 123;
  elem2 = 321;

  pdf_list_create (NULL, NULL, 0, &list);
  pdf_list_add_last (list, &elem);
  pdf_list_add_last (list, &elem2);
  
  st = pdf_list_iterator_from_to (list, 0, 1, &itr);
  fail_if (st != PDF_OK);

  pdf_list_destroy (list);
}
END_TEST



/*
 * Test: pdf_list_iterator_from_to_002
 * Description:
 *   Try to get an iterator in an invalid range.
 * Success condition:
 *   Returns PDF_EINVRANGE
 */
START_TEST (pdf_list_iterator_from_to_002)
{
  pdf_list_t list;
  int elem, elem2;
  pdf_status_t st;
  pdf_list_iterator_t itr;

  elem = 123;
  elem2 = 321;

  pdf_list_create (NULL, NULL, 0, &list);
  pdf_list_add_last (list, &elem);
  pdf_list_add_last (list, &elem2);
  
  st = pdf_list_iterator_from_to (list, 0, 3, &itr);
  fail_if (st != PDF_EINVRANGE);

  st = pdf_list_iterator_from_to (list, 0, -2, &itr);
  fail_if (st != PDF_EINVRANGE);

  st = pdf_list_iterator_from_to (list, -1, 2, &itr);
  fail_if (st != PDF_EINVRANGE);

  st = pdf_list_iterator_from_to (list, -1, -2, &itr);
  fail_if (st != PDF_EINVRANGE);


  pdf_list_destroy (list);
}
END_TEST



/*
 * Test: pdf_list_iterator_from_to_003
 * Description:
 *   Try to get an iterator given a NULL iterator pointer.
 * Success condition:
 *   Returns PDF_EBADDATA
 */
START_TEST (pdf_list_iterator_from_to_003)
{
  pdf_list_t list;
  pdf_status_t st;
  pdf_list_iterator_t itr;
  int elem;

  elem = 123;
  
  pdf_list_create (NULL, NULL, 0, &list);
  pdf_list_add_last (list, &elem);
  
  st = pdf_list_iterator_from_to (list, 0, 0, NULL);
  fail_if (st != PDF_EBADDATA);

  pdf_list_destroy (list);
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_list_iterator_from_to (void)
{
  TCase *tc = tcase_create("pdf_list_iterator_from_to");
  tcase_add_test(tc, pdf_list_iterator_from_to_001);
  tcase_add_test(tc, pdf_list_iterator_from_to_002);
  tcase_add_test(tc, pdf_list_iterator_from_to_003);

  return tc;
}

/* End of pdf-list-iterator-from-to.c */

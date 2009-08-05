/* -*- mode: C -*- Time-stamp: "2009-08-05 21:52:09 davazp"
 *
 *       File:         pdf-list-iterator-next.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_iterator_next
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
 * Test: pdf_list_iterator_next_001
 * Description:
 *   Try to get the next element using an iterator.
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_list_iterator_next_001)
{
  pdf_list_t list;
  pdf_list_iterator_t itr;
  int elem, *next;
  pdf_status_t st;
  
  elem = 222;

  
  pdf_init();

  pdf_list_new (NULL, NULL, 0, &list);
  pdf_list_add_last (list, &elem, NULL);
  pdf_list_iterator (list, &itr);

  st = pdf_list_iterator_next (&itr, (const void **) &next, NULL);

  fail_if (st != PDF_OK);

  pdf_list_destroy (list);
  pdf_list_iterator_free(&itr);
}
END_TEST


/*
 * Test: pdf_list_iterator_next_002
 * Description:
 *   Try to get the next element using an iterator from an empty list.
 * Success condition:
 *   Returns PDF_ENONODE
 */
START_TEST (pdf_list_iterator_next_002)
{
  pdf_list_t list;
  pdf_list_iterator_t itr;
  pdf_status_t st;
  int *next;

  pdf_init();

  pdf_list_new (NULL, NULL, 0, &list);
  pdf_list_iterator (list, &itr);

  st = pdf_list_iterator_next (&itr, (const void **) &next, NULL);

  fail_if (st != PDF_ENONODE);

  pdf_list_destroy (list);
  pdf_list_iterator_free(&itr);
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_list_iterator_next (void)
{
  TCase *tc = tcase_create("pdf_list_iterator_next");
  tcase_add_test(tc, pdf_list_iterator_next_001);
  tcase_add_test(tc, pdf_list_iterator_next_002);

  return tc;
}

/* End of pdf-list-iterator-next.c */

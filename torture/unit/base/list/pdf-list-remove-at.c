/* -*- mode: C -*- Time-stamp: "08/04/21 19:43:43 jemarch"
 *
 *       File:         pdf-list-remove-at.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_remove_at
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
 * Test: pdf_list_remove_at_001
 * Description:
 *   Try to remove a node at some position
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_list_remove_at_001)
{
  pdf_list_t list;
  int elem;
  pdf_status_t st;
  
  elem = 2212;

  pdf_list_create (NULL, NULL, 0, &list);

  pdf_list_add_last (list, &elem, NULL);
  st = pdf_list_remove_at (list, 0);

  fail_if (st != PDF_OK);

  pdf_list_destroy (list);
}
END_TEST

/*
 * Test: pdf_list_remove_at_002
 * Description:
 *   Try to remove a node at an invalid position
 * Success condition:
 *   Returns PDF_EINVRANGE
 */
START_TEST (pdf_list_remove_at_002)
{
  pdf_list_t list;
  int elem;
  pdf_status_t st;
  
  elem = 2212;

  pdf_list_create (NULL, NULL, 0, &list);

  pdf_list_add_last (list, &elem, NULL);
  st = pdf_list_remove_at (list, 3);

  fail_if (st != PDF_EINVRANGE);

  pdf_list_destroy (list);
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_list_remove_at (void)
{
  TCase *tc = tcase_create("pdf_list_remove_at");
  tcase_add_test(tc, pdf_list_remove_at_001);
  tcase_add_test(tc, pdf_list_remove_at_002);
  
  return tc;
}

/* End of pdf-list-remove-at.c */

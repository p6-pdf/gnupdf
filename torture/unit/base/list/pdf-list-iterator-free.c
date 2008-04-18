/* -*- mode: C -*- Time-stamp: "2008-04-16 20:41:29 gerel"
 *
 *       File:         pdf-list-iterator-free.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_iterator_free
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
 * Test: pdf_list_iterator_free_001
 * Description:
 *   Try to destroy an iterator.
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_list_iterator_free_001)
{
  pdf_list_t list;
  pdf_list_iterator_t itr;

  pdf_list_create (NULL, NULL, 0, &list);

  pdf_list_iterator (list, &itr);

  fail_if (pdf_list_iterator_free (&itr) != PDF_OK);

  pdf_list_destroy (list);
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_list_iterator_free (void)
{
  TCase *tc = tcase_create("pdf_list_iterator_free");
  tcase_add_test(tc, pdf_list_iterator_free_001);

  return tc;
}

/* End of pdf-list-iterator-free.c */

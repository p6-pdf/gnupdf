/* -*- mode: C -*-
 *
 *       File:         pdf-list-indexof-from.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_indexof_from
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
 * Test: pdf_list_indexof_from_001
 * Description:
 *   Try to get the index of a existent element from position '0'.
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_list_indexof_from_001)
{
  pdf_list_t *list;
  int elem;
  pdf_size_t pos;
  pdf_error_t *error = NULL;

  elem = 2121;

  pdf_init ();

  list = pdf_list_new (NULL, NULL, PDF_FALSE, NULL);

  pdf_list_add_last (list, &elem, &error);
  fail_if (error != NULL);

  pos = pdf_list_indexof_from (list, 0, &elem, &error);
  fail_if (pos == (pdf_size_t)-1);
  fail_if (error != NULL);

  pdf_list_destroy (list);
}
END_TEST



/*
 * Test: pdf_list_indexof_from_002
 * Description:
 *   Try to get the index of a existent element from invalid position.
 * Success condition:
 *   Returns PDF_EINVRANGE
 */
START_TEST (pdf_list_indexof_from_002)
{
  pdf_list_t *list;
  int elem;
  pdf_size_t pos;
  pdf_error_t *error = NULL;

  elem = 2121;

  pdf_init ();

  list = pdf_list_new (NULL, NULL, PDF_FALSE, NULL);

  pdf_list_add_last (list, &elem, &error);
  fail_if (error != NULL);

  pos = pdf_list_indexof_from (list, 2, &elem, &error);
  fail_if (pos != (pdf_size_t)-1);
  fail_if (error == NULL);
  fail_if (pdf_error_get_status (error) != PDF_EINVRANGE);

  pdf_list_destroy (list);
}
END_TEST

/*
 * Test: pdf_list_indexof_from_003
 * Description:
 *   Try to get the index of a non-existent element.
 * Success condition:
 *   Returns (pdf_size_t)-1
 */
START_TEST (pdf_list_indexof_from_003)
{
  pdf_list_t *list;
  int elem1, elem2, elem3;
  pdf_size_t pos;
  pdf_error_t *error = NULL;

  elem1 = 2121;
  elem2 = 1234;
  elem3 = 9876;

  pdf_init ();

  list = pdf_list_new (NULL, NULL, PDF_FALSE, NULL);

  pdf_list_add_last (list, &elem2, &error);
  fail_if (error != NULL);
  pdf_list_add_last (list, &elem3, &error);
  fail_if (error != NULL);

  pos = pdf_list_indexof_from (list, 0, &elem1, &error);
  fail_if (pos == (pdf_size_t)-1);
  fail_if (error != NULL);

  pdf_list_destroy (list);
}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_list_indexof_from (void)
{
  TCase *tc = tcase_create("pdf_list_indexof_from");
  tcase_add_test(tc, pdf_list_indexof_from_001);
  tcase_add_test(tc, pdf_list_indexof_from_002);
  tcase_add_test(tc, pdf_list_indexof_from_003);

  return tc;
}

/* End of pdf-list-indexof-from.c */

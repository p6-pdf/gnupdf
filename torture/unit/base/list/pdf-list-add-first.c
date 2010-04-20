/* -*- mode: C -*-
 *
 *       File:         pdf-list-add-first.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_add_first
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

extern pdf_bool_t l_comp (const void * elemb, const void * elema);

/*
 * Test: pdf_list_add_first_001
 * Description:
 *   Try to add an element at the beginning of the list.
 * Success condition:
 *   We get the right pdf_list_size().
 */
START_TEST (pdf_list_add_first_001)
{
  pdf_list_t list;
  int elem, elem2;

  elem = 5123;
  elem2 = 5431;

  pdf_init();

  pdf_list_new (l_comp, NULL, 0, &list);

  pdf_list_add_first (list, &elem, NULL);
  pdf_list_add_first (list, &elem, NULL);
  fail_if (pdf_list_size(list) != 1);

  pdf_list_add_first (list, &elem2, NULL);
  fail_if (pdf_list_size(list) != 2);
  
  pdf_list_destroy (list);
}
END_TEST

/*
 * Test: pdf_list_add_first_002
 * Description:
 *   Try to add an element at the beginning of the list allowing duplicates.
 * Success condition:
 *   We get the right pdf_list_size().
 */
START_TEST (pdf_list_add_first_002)
{
  pdf_list_t list;
  int elem;

  elem = 5123;

  pdf_init();

  pdf_list_new (l_comp, NULL, 1, &list);
  pdf_list_add_first (list, &elem, NULL);
  pdf_list_add_first (list, &elem, NULL);
  fail_if (pdf_list_size(list) != 2);

  pdf_list_destroy (list);
}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_list_add_first (void)
{
  TCase *tc = tcase_create("pdf_list_add_first");
  tcase_add_test(tc, pdf_list_add_first_001);
  tcase_add_test(tc, pdf_list_add_first_002);

  return tc;
}

/* End of pdf-list-add-first.c */

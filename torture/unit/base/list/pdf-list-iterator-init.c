/* -*- mode: C -*-
 *
 *       File:         pdf-list-iterator-init.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_iterator
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
#include <gl_list.h>
#include <pdf-test-common.h>
#include "pdf-list-test-common.h"

/*
 * Test: pdf_list_iterator_init_001
 * Description:
 *   Try to create an iterator from list.
 * Success condition:
 *   Doesn't crash.
 */
START_TEST (pdf_list_iterator_init_001)
{
  pdf_list_t *list;
  pdf_list_iterator_t itr;


  list = pdf_list_new (NULL, NULL, PDF_FALSE, NULL);

  pdf_list_iterator_init (&itr, list);;

  pdf_list_iterator_deinit (&itr);
  pdf_list_destroy (list);
}
END_TEST

/*
 * Test: pdf_list_iterator_init_002
 * Description:
 *   Iterator size check.
 * Success condition:
 *   The pdf list iterator size is greater or equal than the gl list one.
 */
START_TEST (pdf_list_iterator_init_002)
{

  fail_if (sizeof (pdf_list_iterator_t) < sizeof (gl_list_iterator_t));
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_list_iterator_init (void)
{
  TCase *tc = tcase_create ("pdf_list_iterator_init");

  tcase_add_test (tc, pdf_list_iterator_init_001);
  tcase_add_test (tc, pdf_list_iterator_init_002);

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-list-iterator-init.c */

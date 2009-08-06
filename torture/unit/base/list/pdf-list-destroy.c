/* -*- mode: C -*- Time-stamp: "2009-08-05 21:50:58 davazp"
 *
 *       File:         pdf-list-destroy.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_destroy
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

/*
 * Test: pdf_list_destroy_001
 * Description:
 *   Try to destroy an empty list.
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_list_destroy_001)
{
  pdf_list_t list;
  
  pdf_init();

  pdf_list_new (NULL, NULL, 0, &list);

  fail_if (pdf_list_destroy (list) != PDF_OK);

}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_list_destroy (void)
{
  TCase *tc = tcase_create("pdf_list_destroy");
  tcase_add_test(tc, pdf_list_destroy_001);
  return tc;
}

/* End of pdf-list-destroy.c */

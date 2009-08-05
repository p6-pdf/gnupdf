/* -*- mode: C -*- Time-stamp: "2009-08-05 21:53:42 davazp"
 *
 *       File:         pdf-list-size.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_size
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
 * Test: pdf_list_size_001
 * Description:
 *   Try to get an empty list size.
 * Success condition:
 *   should return '0'
 */
START_TEST (pdf_list_size_001)
{
  pdf_list_t list;
  pdf_size_t size;
  
  pdf_init();

  pdf_list_new (NULL, NULL, 0, &list);
  size = pdf_list_size (list);

  fail_if (size != 0);

  pdf_list_destroy (list);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_list_size (void)
{
  TCase *tc = tcase_create("pdf_list_size");
  tcase_add_test(tc, pdf_list_size_001);

  return tc;
}

/* End of pdf-list-size.c */

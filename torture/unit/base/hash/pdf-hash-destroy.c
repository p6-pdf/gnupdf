/* -*- mode: C -*- Time-stamp: "2008-04-16 20:25:16 gerel"
 *
 *       File:         pdf-hash-destroy.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_hash_destroy
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
 * Test: pdf_hash_destroy_001
 * Description:
 *   Try to destroy an empty hash.
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_hash_destroy_001)
{
  pdf_hash_t table;

  pdf_hash_create (NULL, NULL, &table);

  fail_if (pdf_hash_destroy (&table) != PDF_OK);
}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_hash_destroy (void)
{
  TCase *tc = tcase_create("pdf_hash_destroy");
  tcase_add_test(tc, pdf_hash_destroy_001);
  return tc;
}

/* End of pdf-hash-destroy.c */

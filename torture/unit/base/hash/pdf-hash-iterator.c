/* -*- mode: C -*- Time-stamp: "2008-04-16 20:30:29 gerel"
 *
 *       File:         pdf-hash-iterator.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_hash_iterator
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
 * Test: pdf_hash_iterator_001
 * Description:
 *   Try to create an iterator.
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_hash_iterator_001)
{
  pdf_hash_t table;
  pdf_hash_iterator_t itr;
  
  pdf_hash_create (NULL, NULL, &table);

  fail_if (pdf_hash_iterator (table, &itr) != PDF_OK);

  pdf_hash_destroy (&table);

}
END_TEST


/*
 * Test: pdf_hash_iterator_002
 * Description:
 *   Try to create an iterator given a NULL iterator pointer.
 * Success condition:
 *   Returns PDF_EBADDATA
 */
START_TEST (pdf_hash_iterator_002)
{
  pdf_hash_t table;
  
  pdf_hash_create (NULL, NULL, &table);

  fail_if (pdf_hash_iterator (table, NULL) != PDF_EBADDATA);

  pdf_hash_destroy (&table);

}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_hash_iterator (void)
{
  TCase *tc = tcase_create("pdf_hash_iterator");
  tcase_add_test(tc, pdf_hash_iterator_001);
  tcase_add_test(tc, pdf_hash_iterator_002);
  return tc;
}

/* End of pdf-hash-iterator.c */

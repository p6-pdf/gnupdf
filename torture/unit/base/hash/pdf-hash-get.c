/* -*- mode: C -*- Time-stamp: "2009-08-05 21:46:47 davazp"
 *
 *       File:         pdf-hash-get.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_hash_get
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
 * Test: pdf_hash_get_001
 * Description:
 *   Try to search an existent element.
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_hash_get_001)
{
  pdf_hash_t table;
  char *s;

  pdf_init();

  pdf_hash_new (NULL, &table);
  pdf_hash_add (table, "key", "val",NULL);
  pdf_hash_add (table, "ky", "val",NULL);
  pdf_hash_add (table, "ky23", "val",NULL);
  pdf_hash_add (table, "2ky23", "val",NULL);
  pdf_hash_add (table, "032ky23", "val",NULL);
  pdf_hash_add (table, "230", "val",NULL);
  pdf_hash_add (table, "0389", "val",NULL);
  pdf_hash_add (table, "123", "val",NULL);
  pdf_hash_add (table, "1", "val",NULL);
  pdf_hash_add (table, "aky23", "val",NULL);
  pdf_hash_add (table, "AB23", "val",NULL);
  pdf_hash_add (table, "0", "val",NULL);
  pdf_hash_add (table, "AB", "val",NULL);
  
  fail_if (pdf_hash_get (table, "key", (void *) &s) != PDF_OK);

  pdf_hash_destroy (table);

}
END_TEST

/*
 * Test: pdf_hash_get_002
 * Description:
 *   Try to search a non-existent element.
 * Success condition:
 *   Returns PDF_ERROR
 */
START_TEST (pdf_hash_get_002)
{
  pdf_hash_t table;
  char *s;

  pdf_hash_new (NULL, &table);
  pdf_hash_add (table, "key", "val",NULL);
  
  fail_if (pdf_hash_get (table, "ke", (void *) &s) != PDF_ERROR);

  pdf_hash_destroy (table);

}
END_TEST

/*
 * Test: pdf_hash_get_003
 * Description:
 *   Try to search an element given a NULL elem_pointer.
 * Success condition:
 *   Returns PDF_EBADDATA
 */
START_TEST (pdf_hash_get_003)
{
  pdf_hash_t table;

  pdf_hash_new (NULL, &table);
  pdf_hash_add (table, "key", "val",NULL);
  
  fail_if (pdf_hash_get (table, "key", NULL) != PDF_EBADDATA);

  pdf_hash_destroy (table);

}
END_TEST


/*
 * Test: pdf_hash_get_004
 * Description:
 *   Try to search an element given a NULL key.
 * Success condition:
 *   Returns PDF_EBADDATA
 */
START_TEST (pdf_hash_get_004)
{
  pdf_hash_t table;
  char *s;

  pdf_hash_new (NULL, &table);
  pdf_hash_add (table, "key", "val",NULL);
  
  fail_if (pdf_hash_get (table, NULL, (void *) &s) != PDF_EBADDATA);

  pdf_hash_destroy (table);

}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_hash_get (void)
{
  TCase *tc = tcase_create("pdf_hash_get");
  tcase_add_test(tc, pdf_hash_get_001);
  tcase_add_test(tc, pdf_hash_get_002);
  tcase_add_test(tc, pdf_hash_get_003);
  tcase_add_test(tc, pdf_hash_get_004);
  return tc;
}

/* End of pdf-hash-search.c */

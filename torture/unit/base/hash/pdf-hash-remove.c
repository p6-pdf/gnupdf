/* -*- mode: C -*-
 *
 *       File:         pdf-hash-remove.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_hash_remove
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

#include <stdbool.h>
#include <stdio.h>
#include <pdf.h>
#include <check.h>

/*
 * Test: pdf_hash_remove_001
 * Description:
 *   Try to remove an existent element.
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_hash_remove_001)
{
  pdf_hash_t table;

  pdf_init();

  pdf_hash_new (NULL, &table);
  pdf_hash_add (table, "key", "val",NULL);

  fail_if (pdf_hash_remove (table, "key") != PDF_OK);

  pdf_hash_destroy (table);

}
END_TEST


/*
 * Test: pdf_hash_remove_002
 * Description:
 *   Try to remove a non-existent element.
 * Success condition:
 *   Returns PDF_ERROR
 */
START_TEST (pdf_hash_remove_002)
{
  pdf_hash_t table;

  pdf_init();

  pdf_hash_new (NULL, &table);
  pdf_hash_add (table, "key", "val",NULL);

  fail_if (pdf_hash_remove (table, "ke") != PDF_ERROR);

  pdf_hash_destroy (table);

}
END_TEST

/*
 * Test: pdf_hash_remove_003
 * Description:
 *   Try to remove a element given a NULL key.
 * Success condition:
 *   Returns PDF_EBADDATA
 */
START_TEST (pdf_hash_remove_003)
{
  pdf_hash_t table;

  pdf_init();

  pdf_hash_new (NULL, &table);
  pdf_hash_add (table, "key", "val",NULL);

  fail_if (pdf_hash_remove (table, NULL) != PDF_EBADDATA);

  pdf_hash_destroy (table);

}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_hash_remove (void)
{
  TCase *tc = tcase_create("pdf_hash_remove");
  tcase_add_test(tc, pdf_hash_remove_001);
  tcase_add_test(tc, pdf_hash_remove_002);
  tcase_add_test(tc, pdf_hash_remove_003);
  return tc;
}

/* End of pdf-hash-remove.c */

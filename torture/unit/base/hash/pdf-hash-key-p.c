/* -*- mode: C -*- Time-stamp: "2009-08-05 21:47:16 davazp"
 *
 *       File:         pdf-hash-key-p.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_hash_key_p
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
 * Test: pdf_hash_key_p_001
 * Description:
 *   Try to prove that a given key exist.
 * Success condition:
 *   Returns PDF_TRUE
 */
START_TEST (pdf_hash_key_p_001)
{
  pdf_hash_t table;

  pdf_init();

  pdf_hash_new (NULL, &table);
  pdf_hash_add (table, "key", "val", NULL);
 
  fail_if (pdf_hash_key_p (table, "key") != PDF_TRUE);

  pdf_hash_destroy (table);
}
END_TEST


/*
 * Test: pdf_hash_key_p_002
 * Description:
 *   Try to prove that a given key doesn't exist.
 * Success condition:
 *   Returns PDF_FALSE
 */
START_TEST (pdf_hash_key_p_002)
{
  pdf_hash_t table;

  pdf_hash_new (NULL, &table);
  pdf_hash_add (table, "key", "val", NULL);
 
  fail_if (pdf_hash_key_p (table, "ke") != PDF_FALSE);

  pdf_hash_destroy (table);
}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_hash_key_p (void)
{
  TCase *tc = tcase_create("pdf_hash_key_p");
  tcase_add_test(tc, pdf_hash_key_p_001);
  tcase_add_test(tc, pdf_hash_key_p_002);
  return tc;
}

/* End of pdf-hash-key-p.c */

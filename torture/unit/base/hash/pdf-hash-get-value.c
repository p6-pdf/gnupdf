/* -*- mode: C -*-
 *
 *       File:         pdf-hash-get-value.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_hash_get
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

static const pdf_char_t *val1 = "val1";
static const pdf_char_t *val2 = "val2";
static const pdf_char_t *val3 = "val3";

/*
 * Test: pdf_hash_get_value_001
 * Description:
 *   Try to search an existent element.
 * Success condition:
 *   Returns element value
 */
START_TEST (pdf_hash_get_value_001)
{
  pdf_hash_t *table;
  const pdf_char_t *val;

  pdf_init ();

  table = pdf_hash_new (NULL);

  pdf_hash_add (table, "key1", val1, NULL, NULL);
  pdf_hash_add (table, "key2", val2, NULL, NULL);
  pdf_hash_add (table, "key3", val3, NULL, NULL);

  val = pdf_hash_get_value (table, "key1");
  fail_if (val != val1);


  val = pdf_hash_get_value (table, "key2");
  fail_if (val != val2);

  val = pdf_hash_get_value (table, "key3");
  fail_if (val != val3);

  pdf_hash_destroy (table);

}
END_TEST

/*
 * Test: pdf_hash_get_value_002
 * Description:
 *   Try to search a non-existent element.
 * Success condition:
 *   Returns NULL value
 */
START_TEST (pdf_hash_get_value_002)
{
  pdf_hash_t *table;
  const pdf_char_t *val;

  pdf_init ();

  table = pdf_hash_new (NULL);

  pdf_hash_add (table, "key1", val1, NULL, NULL);
  pdf_hash_add (table, "key2", val2, NULL, NULL);
  pdf_hash_add (table, "key3", val3, NULL, NULL);

  val = pdf_hash_get_value (table, "ke");
  fail_if (val != NULL);

  pdf_hash_destroy (table);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_hash_get_value (void)
{
  TCase *tc = tcase_create ("pdf_hash_get_value");

  tcase_add_test (tc, pdf_hash_get_value_001);
  tcase_add_test (tc, pdf_hash_get_value_002);
  return tc;
}

/* End of pdf-hash-get.c */

/* -*- mode: C -*-
 *
 *       File:         pdf-hash-replace.c
 *       Date:         Mon Feb 21 19:41:33 2011
 *
 *       GNU PDF Library - Unit tests for pdf_hash_replace
 *
 */

/* Copyright (C) 2011 Free Software Foundation, Inc. */

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
#include <pdf-test-common.h>
/*
 * Test: pdf_hash_replace_001
 * Description:
 *   Try to add key/value pair to an empty table.
 * Success condition:
 *   Returns PDF_TRUE
 */
START_TEST (pdf_hash_replace_001)
{
  pdf_hash_t *table;
  pdf_error_t *error = NULL;
  int n1 = 1;
  const int *n;

  pdf_init ();

  table = pdf_hash_new (NULL);

  fail_if (pdf_hash_replace (table,
                             "key",
                             &n1, NULL,
                             &error) != PDF_TRUE);
  fail_if (error != NULL);

  n = pdf_hash_get_value (table, "key");
  fail_if (n == NULL);
  fail_if (*n != n1);

  pdf_hash_destroy (table);
}
END_TEST

/*
 * Test: pdf_hash_replace_002
 * Description:
 *   Try to add key/NULL pair to an empty table.
 * Success condition:
 *   Returns PDF_TRUE
 */
START_TEST (pdf_hash_replace_002)
{
  pdf_hash_t *table;
  pdf_error_t *error = NULL;

  pdf_init ();

  table = pdf_hash_new (NULL);

  fail_if (pdf_hash_replace (table,
                             "key",
                             NULL, NULL,
                             &error) != PDF_TRUE);
  fail_if (error != NULL);

  pdf_hash_destroy (table);
}
END_TEST

/*
 * Test: pdf_hash_replace_003
 * Description:
 *   Try to replace an existing key with another value.
 * Success condition:
 *   Returns PDF_TRUE
 */
START_TEST (pdf_hash_replace_003)
{
  pdf_hash_t *table;
  pdf_error_t *error = NULL;
  int n1 = 1;
  int n2 = 2;
  const int *n;

  pdf_init ();

  table = pdf_hash_new (NULL);

  fail_if (pdf_hash_replace (table,
                             "key",
                             &n1, NULL,
                             &error) != PDF_TRUE);
  fail_if (error != NULL);

  fail_if (pdf_hash_replace (table,
                             "key",
                             &n2, NULL,
                             &error) != PDF_TRUE);
  fail_if (error != NULL);

  n = pdf_hash_get_value (table, "key");
  fail_if (n == NULL);
  fail_if (*n != n2);

  pdf_hash_destroy (table);
}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_hash_replace (void)
{
  TCase *tc = tcase_create ("pdf_hash_replace");

  tcase_add_test (tc, pdf_hash_replace_001);
  tcase_add_test (tc, pdf_hash_replace_002);
  tcase_add_test (tc, pdf_hash_replace_003);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-hash-replace.c */

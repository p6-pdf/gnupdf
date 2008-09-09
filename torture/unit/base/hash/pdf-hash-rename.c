/* -*- mode: C -*- Time-stamp: "08/09/09 00:03:10 jemarch"
 *
 *       File:         pdf-hash-rename.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_hash_rename
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
 * Test: pdf_hash_rename_001
 * Description:
 *   Try to rename an existent key and prove it was renamed.
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_hash_rename_001)
{
  pdf_hash_t table;

  pdf_hash_new (NULL, &table);
  pdf_hash_add (table, "key", "val",NULL);

  fail_if (pdf_hash_rename (table, "key", "ke") != PDF_OK);
  fail_if (pdf_hash_key_p (table, "ke") != PDF_TRUE);

  pdf_hash_destroy (&table);
}
END_TEST

/*
 * Test: pdf_hash_rename_002
 * Description:
 *   Try to rename a non-existent key.
 * Success condition:
 *   Returns PDF_ERROR
 */
START_TEST (pdf_hash_rename_002)
{
  pdf_hash_t table;

  pdf_hash_new (NULL, &table);
  pdf_hash_add (table, "key", "val",NULL);

  fail_if (pdf_hash_rename (table, "ke", "key") != PDF_ERROR);

  pdf_hash_destroy (&table);
}
END_TEST

/*
 * Test: pdf_hash_rename_003
 * Description:
 *   Try to rename a key given a NULL key.
 * Success condition:
 *   Returns PDF_EBADDATA
 */
START_TEST (pdf_hash_rename_003)
{
  pdf_hash_t table;

  pdf_hash_new (NULL, &table);

  fail_if (pdf_hash_rename (table, NULL, "ke") != PDF_EBADDATA);

  pdf_hash_destroy (&table);
}
END_TEST

/*
 * Test: pdf_hash_rename_004
 * Description:
 *   Try to rename a key given a NULL new_key.
 * Success condition:
 *   Returns PDF_EBADDATA
 */
START_TEST (pdf_hash_rename_004)
{
  pdf_hash_t table;

  pdf_hash_new (NULL, &table);

  fail_if (pdf_hash_rename (table, "key", NULL) != PDF_EBADDATA);

  pdf_hash_destroy (&table);
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_hash_rename (void)
{
  TCase *tc = tcase_create("pdf_hash_rename");
  tcase_add_test(tc, pdf_hash_rename_001);
  tcase_add_test(tc, pdf_hash_rename_002);
  tcase_add_test(tc, pdf_hash_rename_003);
  tcase_add_test(tc, pdf_hash_rename_004);
  return tc;
}

/* End of pdf-hash-rename.c */

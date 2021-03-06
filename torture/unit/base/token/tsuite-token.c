/* -*- mode: C -*-
 *
 *       File:         tsuite-token.c
 *       Date:         Wed Jan 14 05:43:09 2009
 *
 *       GNU PDF Library - Testcase definition for the reader module
 *
 */

/* Copyright (C) 2009-2011 Free Software Foundation, Inc. */

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

#include <check.h>
#include <pdf-test-common.h>

extern TCase *test_pdf_token_reader (void);
extern TCase *test_pdf_token_writer (void);

Suite *
tsuite_token ()
{
  Suite *s;

  s = suite_create ("token");

  suite_add_tcase (s, test_pdf_token_reader ());
  suite_add_tcase (s, test_pdf_token_writer ());

  return s;
}


/* End of tsuite-token.c */

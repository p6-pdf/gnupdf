/* -*- mode: C -*- Time-stamp: "2009-05-20 04:35:27 mgold"
 *
 *       File:         tsuite-token.c
 *       Date:         Wed Jan 14 05:43:09 2009
 *
 *       GNU PDF Library - Testcase definition for the reader module
 *
 */

/* Copyright (C) 2009 Free Software Foundation, Inc. */

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

#include <check.h>

extern TCase *test_pdf_token_read (void);

Suite *
tsuite_token ()
{
  Suite *s;

  s = suite_create("token");

  suite_add_tcase (s, test_pdf_token_read ());

  return s;
}


/* End of tsuite-token.c */

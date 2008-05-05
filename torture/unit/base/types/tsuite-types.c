/* -*- mode: C -*- Time-stamp: ""
 *
 *       File:         tsuite-types.c
 *       Date:         Mon May 05 10:30:01 2008
 *
 *       GNU PDF Library - Testcase definition for the types module
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

#include <check.h>

extern TCase *test_pdf_i64_new (void);
extern TCase *test_pdf_i64_assign (void);
extern TCase *test_pdf_i64_assign_quick (void);
extern TCase *test_pdf_i64_copy (void);
extern TCase *test_pdf_i64_add (void);

Suite *
tsuite_types ()
{
  Suite *s;
  
  s = suite_create("types");

  suite_add_tcase (s, test_pdf_i64_new());
  suite_add_tcase (s, test_pdf_i64_assign());
  suite_add_tcase (s, test_pdf_i64_assign_quick());
  suite_add_tcase (s, test_pdf_i64_copy());
  suite_add_tcase (s, test_pdf_i64_add());
  
  return s;
}


/* End of tsuite-types.c */

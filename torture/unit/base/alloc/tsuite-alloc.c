/* -*- mode: C -*-
 *
 *       File:         tsuite-alloc.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Sat Feb 23 23:09:12 2008
 *
 *       GNU PDF Library - Testcase definition for the alloc module
 *
 */

/* Copyright (C) 2007-2011 Free Software Foundation, Inc. */

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
extern TCase *test_pdf_alloc (void);
extern TCase *test_pdf_realloc (void);

Suite *
tsuite_alloc ()
{
  Suite *s;

  s = suite_create("alloc");
  suite_add_tcase (s, test_pdf_alloc());
  suite_add_tcase (s, test_pdf_realloc());
  return s;
}

/* End of tsuite-alloc.c */

/* -*- mode: C -*- Time-stamp: "2008-08-25 15:56:40 davazp"
 *
 *       File:         tsuite-alloc.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Sat Feb 23 23:09:12 2008
 *
 *       GNU PDF Library - Testcase definition for the crypt module
 *
 */

/* Copyright (C) 2007, 2008 Free Software Foundation, Inc. */

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

extern TCase *test_pdf_crypt_init (void);


Suite *
tsuite_crypt ()
{
  Suite *s;

  s = suite_create("crypt");

  suite_add_tcase (s, test_pdf_crypt_init ());

  return s;
}

/* End of tsuite-crypt.c */

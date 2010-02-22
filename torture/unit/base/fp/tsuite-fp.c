/* -*- mode: C -*-
 *
 *       File:         tsuite-fp.c
 *       Date:         Tue Dec  2 20:08:22 2008
 *
 *       GNU PDF Library - Testcase definition for the FP module
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

#include <check.h>

extern TCase *test_pdf_fp_func_4_new (void);
extern TCase *test_pdf_fp_func_eval (void);


Suite *
tsuite_fp ()
{
  Suite *s;

  s = suite_create("fp");
  
  suite_add_tcase (s, test_pdf_fp_func_4_new ());
  suite_add_tcase (s, test_pdf_fp_func_eval ());

  return s;
}


/* End of tsuite-fp.c */

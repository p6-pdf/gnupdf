/* -*- mode: C -*-
 *
 *       File:         tsuite-error.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Testcase definition for the Error module
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

#include <check.h>
#include <pdf-test-common.h>
extern TCase *test_pdf_error (void);
extern TCase *test_pdf_perror (void);
extern TCase *test_pdf_error_new (void);
extern TCase *test_pdf_set_error (void);

Suite *
tsuite_error ()
{
  Suite *s;

  s = suite_create("error");

  suite_add_tcase (s, test_pdf_perror ());
  suite_add_tcase (s, test_pdf_error ());
  suite_add_tcase (s, test_pdf_error_new ());
  suite_add_tcase (s, test_pdf_set_error ());

  return s;
}


/* End of tsuite-error.c */

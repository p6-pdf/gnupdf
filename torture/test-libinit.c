/* -*- mode: C -*- Time-stamp: "07/10/23 12:58:16 jemarch"
 *
 *       File:         test-libinit.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Fri Sep 14 23:08:35 2007
 *
 *       GNU PDF Library - Library initialization and finalization
 *
 */

/* Copyright (C) 2007 Free Software Foundation, Inc. */

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
#include <string.h>
#include <pdf.h>
#include <check.h>


PDF_GLOBALS



START_TEST(names)
{
  pdf_obj_t name;

  pdf_init ();

  /* Test preallocated names */
  name = pdf_create_name ((unsigned char*) "PageLayout", 10);
  fail_if (!pdf_obj_equal_p (name, PDF_N_PAGE_LAYOUT));
  pdf_finish ();
}
END_TEST


TCase* test_libinit_tests(void)
{
  TCase *tc = tcase_create("test-libinit");
  tcase_add_test(tc, names);
  return tc;
}


/* End of test-libinit.c */

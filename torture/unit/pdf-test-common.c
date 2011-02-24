/* -*- mode: C -*-
 *
 *       File:         pdf-text-common.c
 *       Date:         Thu Feb 24 22:53:57 2011
 *
 *       GNU PDF Library - Common utilities for the unit tests
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
#include <locale.h>
#include <pdf-test-common.h>

void
pdf_test_setup (void)
{
#if defined HAVE_SETLOCALE
  /* Initialize locale in the program.
   * We really need to fix a given locale, so that the unit tests
   * don't depend on where they are run */
  setlocale (LC_ALL, "en_US.utf8");
#endif /* HAVE_SETLOCALE */

  /* Initialize the PDF library */
  pdf_init ();
}

void
pdf_test_teardown (void)
{
  pdf_finish ();
}

/* End of pdf-test-common.c */

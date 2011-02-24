/* -*- mode: C -*-
 *
 *       File:         tsuite-types.c
 *       Date:         Mon May 05 10:30:01 2008
 *
 *       GNU PDF Library - Testcase definition for the types module
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
Suite *
tsuite_types ()
{
  Suite *s;

  s = suite_create("types");

  return s;
}


/* End of tsuite-types.c */

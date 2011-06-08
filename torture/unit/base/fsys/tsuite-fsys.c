/* -*- mode: C -*-
 *
 *       File:         tsuite-fsys.c
 *       Date:         Fri May  1 18:08:12 2009
 *
 *       GNU PDF Library - Testcase definition for the Fsys module
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

extern TCase *test_pdf_fsys_disk_get_free_space (void);
extern TCase *test_pdf_fsys_disk_file_open (void);
extern TCase *test_pdf_fsys_disk_file_open_tmp (void);
extern TCase *test_pdf_fsys_disk_build_path (void);
extern TCase *test_pdf_fsys_disk_get_parent (void);

Suite *
tsuite_fsys ()
{
  Suite *s;

  s = suite_create ("fsys");

  /* Disk filesystem tests */
  suite_add_tcase (s, test_pdf_fsys_disk_get_free_space ());
  suite_add_tcase (s, test_pdf_fsys_disk_file_open ());
  suite_add_tcase (s, test_pdf_fsys_disk_file_open_tmp ());
  suite_add_tcase (s, test_pdf_fsys_disk_build_path ());
  suite_add_tcase (s, test_pdf_fsys_disk_get_parent ());

  return s;
}


/* End of tsuite-fsys.c */

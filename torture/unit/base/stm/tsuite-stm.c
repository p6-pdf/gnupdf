/* -*- mode: C -*- Time-stamp: "09/10/28 21:42:34 jemarch"
 *
 *       File:         tsuit-stm.c
 *       Date:         Sat Feb 23 21:35:11 2008
 *
 *       GNU PDF Library - Testcase definition for the stm module
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

#include <config.h>

#include <check.h>

extern TCase *test_pdf_stm_mem_new (void);
extern TCase *test_pdf_stm_file_new (void);
extern TCase *test_pdf_stm_read (void);
extern TCase *test_pdf_stm_read_char (void);
extern TCase *test_pdf_stm_peek_char (void);
extern TCase *test_pdf_stm_bseek (void);
extern TCase *test_pdf_stm_btell (void);
extern TCase *test_pdf_stm_tell (void);
extern TCase *test_pdf_stm_write (void);
extern TCase *test_pdf_stm_flush (void);
extern TCase *test_pdf_stm_get_mode (void);

Suite *
tsuite_stm ()
{
  Suite *s;

  s = suite_create("stm");
  
  suite_add_tcase (s, test_pdf_stm_mem_new ());
  suite_add_tcase (s, test_pdf_stm_file_new ());
  suite_add_tcase (s, test_pdf_stm_read ());
  suite_add_tcase (s, test_pdf_stm_read_char ());
  suite_add_tcase (s, test_pdf_stm_peek_char ());
  suite_add_tcase (s, test_pdf_stm_bseek ());
  suite_add_tcase (s, test_pdf_stm_btell ());
  suite_add_tcase (s, test_pdf_stm_tell ());
  suite_add_tcase (s, test_pdf_stm_write ());
  suite_add_tcase (s, test_pdf_stm_flush ());
  suite_add_tcase (s, test_pdf_stm_get_mode ());


  return s;
}


/* End of tsuit-stm.c */

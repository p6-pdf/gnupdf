/* -*- mode: C -*- Time-stamp: "09/09/05 16:10:34 jemarch"
 *
 *       File:         tsuite-hash.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Testcase definition for the Hash module
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

extern TCase *test_pdf_hash_new (void);
extern TCase *test_pdf_hash_destroy (void);
extern TCase *test_pdf_hash_key_p (void);
extern TCase *test_pdf_hash_rename (void);
extern TCase *test_pdf_hash_size (void);
extern TCase *test_pdf_hash_add (void);
extern TCase *test_pdf_hash_add_time (void);
extern TCase *test_pdf_hash_add_hash (void);
extern TCase *test_pdf_hash_add_text (void);
extern TCase *test_pdf_hash_add_list (void);
extern TCase *test_pdf_hash_add_stm (void);
extern TCase *test_pdf_hash_remove (void);
extern TCase *test_pdf_hash_get (void);
extern TCase *test_pdf_hash_iterator_new (void);
extern TCase *test_pdf_hash_iterator_next (void);
extern TCase *test_pdf_hash_iterator_destroy (void);

Suite *
tsuite_hash ()
{
  Suite *s;

  s = suite_create("hash");
  
  suite_add_tcase (s, test_pdf_hash_new ());
  suite_add_tcase (s, test_pdf_hash_destroy ());
  suite_add_tcase (s, test_pdf_hash_size ());
  suite_add_tcase (s, test_pdf_hash_add ());
  suite_add_tcase (s, test_pdf_hash_remove ());
  suite_add_tcase (s, test_pdf_hash_get ());
  suite_add_tcase (s, test_pdf_hash_key_p ());
  suite_add_tcase (s, test_pdf_hash_rename ());
  suite_add_tcase (s, test_pdf_hash_iterator_new ());
  suite_add_tcase (s, test_pdf_hash_iterator_next ());
  suite_add_tcase (s, test_pdf_hash_iterator_destroy ());
  suite_add_tcase (s, test_pdf_hash_add_time ());
  suite_add_tcase (s, test_pdf_hash_add_hash ());
  suite_add_tcase (s, test_pdf_hash_add_text ());
  suite_add_tcase (s, test_pdf_hash_add_list ());
  suite_add_tcase (s, test_pdf_hash_add_stm ());


  return s;
}


/* End of tsuite-hash.c */

/* -*- mode: C -*-
 *
 *       File:         tsuite-text.c
 *       Date:         Sat Feb 27 20:42:34 2008
 *
 *       GNU PDF Library - Testcase definition for the text module
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

extern TCase *test_pdf_text_new_destroy (void);
extern TCase *test_pdf_text_dup (void);
extern TCase *test_pdf_text_new_from_host (void);
extern TCase *test_pdf_text_new_from_pdf_string (void);
extern TCase *test_pdf_text_new_from_unicode (void);
extern TCase *test_pdf_text_new_from_u32 (void);
extern TCase *test_pdf_text_get_country (void);
extern TCase *test_pdf_text_get_language (void);
extern TCase *test_pdf_text_set_country (void);
extern TCase *test_pdf_text_set_language (void);
extern TCase *test_pdf_text_empty_p (void);
extern TCase *test_pdf_text_get_host_encoding (void);
extern TCase *test_pdf_text_check_host_encoding (void);
extern TCase *test_pdf_text_get_best_encoding (void);
extern TCase *test_pdf_text_get_host (void);
extern TCase *test_pdf_text_get_pdfdocenc (void);
extern TCase *test_pdf_text_get_unicode (void);
extern TCase *test_pdf_text_get_hex(void);
extern TCase *test_pdf_text_set_host(void);
extern TCase *test_pdf_text_set_pdfdocenc (void);
extern TCase *test_pdf_text_set_unicode (void);
extern TCase *test_pdf_text_concat (void);
extern TCase *test_pdf_text_replace (void);
extern TCase *test_pdf_text_replace_ascii (void);
extern TCase *test_pdf_text_filter (void);
extern TCase *test_pdf_text_cmp (void);

Suite *
tsuite_text ()
{
  Suite *s;
  
  s = suite_create("text");

  suite_add_tcase (s, test_pdf_text_new_destroy());
  suite_add_tcase (s, test_pdf_text_dup());
  suite_add_tcase (s, test_pdf_text_new_from_host());
  suite_add_tcase (s, test_pdf_text_new_from_pdf_string ());
  suite_add_tcase (s, test_pdf_text_new_from_unicode());
  suite_add_tcase (s, test_pdf_text_new_from_u32());
  suite_add_tcase (s, test_pdf_text_get_country());
  suite_add_tcase (s, test_pdf_text_get_language());
  suite_add_tcase (s, test_pdf_text_set_country());
  suite_add_tcase (s, test_pdf_text_set_language());
  suite_add_tcase (s, test_pdf_text_empty_p());
  suite_add_tcase (s, test_pdf_text_get_host_encoding());
  suite_add_tcase (s, test_pdf_text_check_host_encoding());
  suite_add_tcase (s, test_pdf_text_get_best_encoding());
  suite_add_tcase (s, test_pdf_text_get_host());
  suite_add_tcase (s, test_pdf_text_get_pdfdocenc());
  suite_add_tcase (s, test_pdf_text_get_unicode());
  suite_add_tcase (s, test_pdf_text_get_hex());
  suite_add_tcase (s, test_pdf_text_set_host());
  suite_add_tcase (s, test_pdf_text_set_pdfdocenc());
  suite_add_tcase (s, test_pdf_text_set_unicode());
  suite_add_tcase (s, test_pdf_text_concat());
  suite_add_tcase (s, test_pdf_text_replace());
  suite_add_tcase (s, test_pdf_text_replace_ascii());
  suite_add_tcase (s, test_pdf_text_filter());
  suite_add_tcase (s, test_pdf_text_cmp());

  return s;
}


/* End of tsuite-text.c */

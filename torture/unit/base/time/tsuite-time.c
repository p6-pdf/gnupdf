/* -*- mode: C -*-
 *
 *       File:         tsuit-time.c
 *       Date:         Sat Feb 23 21:35:11 2008
 *
 *       GNU PDF Library - Testcase definition for the time module
 *
 */

/* Copyright (C) 2009-2011 Free Software Foundation, Inc. */

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

extern TCase *test_pdf_time_new (void);
extern TCase *test_pdf_time_init (void);
extern TCase *test_pdf_time_dup (void);
extern TCase *test_pdf_time_destroy (void);
extern TCase *test_pdf_time_copy (void);
extern TCase *test_pdf_time_clear (void);
extern TCase *test_pdf_time_set_from_u32 (void);
extern TCase *test_pdf_time_set_from_i64 (void);
extern TCase *test_pdf_time_add_cal_span (void);
extern TCase *test_pdf_time_sub_cal_span (void);
extern TCase *test_pdf_time_add_span (void);
extern TCase *test_pdf_time_sub_span (void);
extern TCase *test_pdf_time_get_local_cal (void);
extern TCase *test_pdf_time_get_utc_cal (void);
extern TCase *test_pdf_time_set_from_cal (void);
extern TCase *test_pdf_time_set_local_offset (void);
extern TCase *test_pdf_time_from_string (void);
extern TCase *test_pdf_time_diff_cal (void);
extern TCase *test_pdf_time_set_to_current_local_time (void);
extern TCase *test_pdf_time_set_to_current_utc_time (void);
extern TCase *test_pdf_time_diff (void);
extern TCase *test_pdf_time_to_string (void);
extern TCase *test_pdf_time_cal_span_cmp (void);
extern TCase *test_pdf_time_cal_span_diff (void);
extern TCase *test_pdf_time_add_cal_span_with_base (void);

#ifdef PDF_HOST_WIN32
extern TCase *test_pdf_time_w32_set_from_filetime (void);
#endif

Suite *
tsuite_time ()
{
  Suite *s;

  s = suite_create("time");

	suite_add_tcase (s, test_pdf_time_new ());
	suite_add_tcase (s, test_pdf_time_dup ());
	suite_add_tcase (s, test_pdf_time_destroy ());
	suite_add_tcase (s, test_pdf_time_copy ());
	suite_add_tcase (s, test_pdf_time_clear ());
	suite_add_tcase (s, test_pdf_time_set_from_u32 ());
	suite_add_tcase (s, test_pdf_time_set_from_i64 ());
	suite_add_tcase (s, test_pdf_time_add_cal_span ());
	suite_add_tcase (s, test_pdf_time_sub_cal_span ());
	suite_add_tcase (s, test_pdf_time_add_span ());
	suite_add_tcase (s, test_pdf_time_sub_span ());
        suite_add_tcase (s, test_pdf_time_get_local_cal ());
        suite_add_tcase (s, test_pdf_time_get_utc_cal ());
	suite_add_tcase (s, test_pdf_time_from_cal ());
        suite_add_tcase (s, test_pdf_time_set_local_offset ());
	suite_add_tcase (s, test_pdf_time_from_string ());
	suite_add_tcase (s, test_pdf_time_span_new ());
	suite_add_tcase (s, test_pdf_time_span_dup ());
	suite_add_tcase (s, test_pdf_time_span_destroy ());
	suite_add_tcase (s, test_pdf_time_span_set ());
	suite_add_tcase (s, test_pdf_time_span_negate ());
	suite_add_tcase (s, test_pdf_time_span_add ());
	suite_add_tcase (s, test_pdf_time_span_cmp ());
	suite_add_tcase (s, test_pdf_time_span_copy ());
	suite_add_tcase (s, test_pdf_time_span_diff ());
	suite_add_tcase (s, test_pdf_time_span_to_secs ());
	suite_add_tcase (s, test_pdf_time_diff_cal ());
	suite_add_tcase (s, test_pdf_time_set_to_current_local_time ());
    suite_add_tcase (s, test_pdf_time_set_to_current_utc_time ());
	suite_add_tcase (s, test_pdf_time_diff ());
	suite_add_tcase (s, test_pdf_time_to_string ());
	suite_add_tcase (s, test_pdf_time_cal_span_cmp ());
	suite_add_tcase (s, test_pdf_time_cal_span_diff ());
	suite_add_tcase (s, test_pdf_time_add_cal_span_with_base ());
#ifdef PDF_HOST_WIN32
	suite_add_tcase (s, test_pdf_time_w32_set_from_filetime ());
#endif





  return s;
}


/* End of tsuit-time.c */

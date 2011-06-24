/* -*- mode: C -*-
 *
 *       File:         pdf-types-uuid-equal-p.c
 *       Date:         Tue May 17 11:49:23 2011
 *
 *       GNU PDF Library - Unit tests for pdf_uuid_equal_p
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

#include <check.h>
#include <pdf.h>
#include <pdf-test-common.h>
/*
 * Test: pdf_uuid_equal_p_001
 * Description: 
 *   Checks if two time-based UUIDs are equal.
 * Success condition:
 *   Both time-based UUIDs are different.
 */
START_TEST (pdf_uuid_equal_p_001)
{

  pdf_uuid_t uuid1;
  pdf_uuid_t uuid2;

  /* Generate two time-based UUIDs */
  uuid1 = pdf_uuid_generate (PDF_UUID_TIME);
  uuid2 = pdf_uuid_generate (PDF_UUID_TIME);

  fail_if(pdf_uuid_equal_p (uuid1, uuid2) == PDF_TRUE);

}
END_TEST

/*
 * Test: pdf_uuid_equal_p_002
 * Description: 
 *   Checks if two random-based UUIDs are equal.
 * Success condition:
 *   Both random-based UUIDs are different.
 */
START_TEST (pdf_uuid_equal_p_002)
{

  pdf_uuid_t uuid1;
  pdf_uuid_t uuid2;

  /* Generate two time-based UUIDs */
  uuid1 = pdf_uuid_generate (PDF_UUID_RANDOM);
  uuid2 = pdf_uuid_generate (PDF_UUID_RANDOM);

  fail_if(pdf_uuid_equal_p (uuid1, uuid2) == PDF_TRUE);
  
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_uuid_equal_p (void)
{
  TCase *tc = tcase_create ("pdf_uuid_equal_p");

  tcase_add_test(tc, pdf_uuid_equal_p_001);
  tcase_add_test(tc, pdf_uuid_equal_p_002);

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);

  return tc;
}

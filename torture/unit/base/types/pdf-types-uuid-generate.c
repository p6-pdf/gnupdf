/* -*- mode: C -*-
 *
 *       File:         pdf-types-uuid-generate.c
 *       Date:         Wed Apr 20 10:31:51 2011
 *
 *       GNU PDF Library - Unit tests for pdf_uuid_generate
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
 * Test: pdf_uuid_generate_001
 * Description:
 *   Generate some UUIDs of supported types.
 * Success condition:
 *   Generated UUIDs should be ok.
 */
START_TEST (pdf_uuid_generate_001)
{

  pdf_uuid_t uuid_time;
  pdf_uuid_t uuid_random;

  /* Create a time-based UUID */
  uuid_time = pdf_uuid_generate (PDF_UUID_TIME);

  /* Create a random-based UUID */
  uuid_random = pdf_uuid_generate (PDF_UUID_RANDOM);

}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_uuid_generate (void)
{
  TCase *tc = tcase_create ("pdf_uuid_generate");

  tcase_add_test(tc, pdf_uuid_generate_001);

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);

  return tc;
}

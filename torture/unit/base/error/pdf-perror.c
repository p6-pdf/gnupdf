/* -*- mode: C -*-
 *
 *       File:         pdf-perror.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_perror
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

#include <stdio.h>
#include <pdf.h>
#include <check.h>
#include <pdf-test-common.h>

/*
 * Test: pdf_perror_001
 * Description:
 *   Try to trigger an error code with NULL string.
 * Success condition:
 *   The program doesn't crash.
 */
START_TEST (pdf_perror_001)
{
    pdf_perror (PDF_OK, NULL);
}
END_TEST


/*
 * Test: pdf_perror_002
 * Description:
 *   Try to trigger an error code with valid string.
 * Success condition:
 *   The program doesn't crash.
 */
START_TEST (pdf_perror_002)
{
    pdf_perror (PDF_OK, "test");
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_perror (void)
{
  TCase *tc = tcase_create("pdf_perror");
  tcase_add_test(tc, pdf_perror_001);
  tcase_add_test(tc, pdf_perror_002);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-perror.c */

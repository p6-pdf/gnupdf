/* -*- mode: C -*- Time-stamp: "2008-06-16 23:13:29 gerel"
 *
 *       File:         pdf-error.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_error
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

#include <stdio.h>
#include <pdf.h>
#include <check.h>


/*
 * Test: pdf_error_001
 * Description:
 *   Try to trigger an error code with NULL fd.
 * Success condition:
 *   The program doesn't crash.
 */
START_TEST (pdf_error_001)
{

  pdf_error (PDF_OK, NULL, "test");
  
}
END_TEST


/*
 * Test: pdf_error_002
 * Description:
 *   Try to trigger an error code with NULL format.
 * Success condition:
 *   The program doesn't crash.
 */
START_TEST (pdf_error_002)
{

  pdf_error (PDF_OK, stdout, NULL);
  
}
END_TEST


/*
 * Test: pdf_error_003
 * Description:
 *   Try to trigger an error code with valid format and fd.
 * Success condition:
 *   The program doesn't crash.
 */
START_TEST (pdf_error_003)
{

  pdf_error (PDF_OK, stdout, "test");
  
}
END_TEST


/*
 * Test: pdf_error_004
 * Description:
 *   Try to trigger an error code with valid format and fd.
 * Success condition:
 *   The program doesn't crash.
 */
START_TEST (pdf_error_004)
{

  pdf_error (PDF_ERROR, stdout, "test");
  
}
END_TEST


/*
 * Test: pdf_error_005
 * Description:
 *   Try to trigger an error code with valid format and fd.
 * Success condition:
 *   The program doesn't crash.
 */
START_TEST (pdf_error_005)
{

  pdf_error (PDF_EBADDATA, stdout, "test");
  
}
END_TEST


/*
 * Test: pdf_error_006
 * Description:
 *   Try to trigger an error code with valid format and fd.
 * Success condition:
 *   The program doesn't crash.
 */
START_TEST (pdf_error_006)
{

  pdf_error (PDF_ENOMEM, stdout, "test");
  
}
END_TEST


/*
 * Test: pdf_error_007
 * Description:
 *   Try to trigger an error code with valid format and fd.
 * Success condition:
 *   The program doesn't crash.
 */
START_TEST (pdf_error_007)
{

  pdf_error (PDF_EEOF, stdout, "test");
  
}
END_TEST


/*
 * Test: pdf_error_008
 * Description:
 *   Try to trigger an error code with valid format and fd.
 * Success condition:
 *   The program doesn't crash.
 */
START_TEST (pdf_error_008)
{

  pdf_error (PDF_EDIVBYZERO, stdout, "test");
  
}
END_TEST


/*
 * Test: pdf_error_009
 * Description:
 *   Try to trigger an error code with valid format and fd.
 * Success condition:
 *   The program doesn't crash.
 */
START_TEST (pdf_error_009)
{

  pdf_error (PDF_ENONODE, stdout, "test");
  
}
END_TEST


/*
 * Test: pdf_error_010
 * Description:
 *   Try to trigger an error code with valid format and fd.
 * Success condition:
 *   The program doesn't crash.
 */
START_TEST (pdf_error_010)
{

  pdf_error (PDF_EINVRANGE, stdout, "test");
  
}
END_TEST


/*
 * Test: pdf_error_011
 * Description:
 *   Try to trigger an error code with valid format and fd.
 * Success condition:
 *   The program doesn't crash.
 */
START_TEST (pdf_error_011)
{

  pdf_error (PDF_ETEXTENC, stdout, "test");
  
}
END_TEST


/*
 * Test: pdf_error_012
 * Description:
 *   Try to trigger an error code with valid format and fd.
 * Success condition:
 *   The program doesn't crash.
 */
START_TEST (pdf_error_012)
{

  pdf_error (PDF_ENOMATCH, stdout, "test");
  
}
END_TEST


/*
 * Test: pdf_error_013
 * Description:
 *   Try to trigger an error code with valid format and fd.
 * Success condition:
 *   The program doesn't crash.
 */
START_TEST (pdf_error_013)
{

  pdf_error (PDF_EBADPERMS, stdout, "test");
  
}
END_TEST


/*
 * Test: pdf_error_014
 * Description:
 *   Try to trigger an error code with valid format and fd.
 * Success condition:
 *   The program doesn't crash.
 */
START_TEST (pdf_error_014)
{

  pdf_error (PDF_EBADNAME, stdout, "test");
  
}
END_TEST


/*
 * Test: pdf_error_015
 * Description:
 *   Try to trigger an error code with valid format and fd.
 * Success condition:
 *   The program doesn't crash.
 */
START_TEST (pdf_error_015)
{

  pdf_error (PDF_EAGAIN, stdout, "test");
  
}
END_TEST


/*
 * Test: pdf_error_016
 * Description:
 *   Try to trigger an error code with valid format and fd.
 * Success condition:
 *   The program doesn't crash.
 */
START_TEST (pdf_error_016)
{

  pdf_error (PDF_ENOSPC, stdout, "test");
  
}
END_TEST


/*
 * Test: pdf_error_017
 * Description:
 *   Try to trigger an error code with valid format and fd.
 * Success condition:
 *   The program doesn't crash.
 */
START_TEST (pdf_error_017)
{

  pdf_error (PDF_ENOTEMPTY, stdout, "test");
  
}
END_TEST



/*
 * Test: pdf_error_018
 * Description:
 *   Try to trigger an error code with valid format and fd.
 * Success condition:
 *   The program doesn't crash.
 */
START_TEST (pdf_error_018)
{

  pdf_error (PDF_STATUS_ITEMS, stdout, "test");
  
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_error (void)
{
  TCase *tc = tcase_create("pdf_error");
  tcase_add_test(tc, pdf_error_001);
  tcase_add_test(tc, pdf_error_002);
  tcase_add_test(tc, pdf_error_003);
  tcase_add_test(tc, pdf_error_004);
  tcase_add_test(tc, pdf_error_005);
  tcase_add_test(tc, pdf_error_006);
  tcase_add_test(tc, pdf_error_007);
  tcase_add_test(tc, pdf_error_008);
  tcase_add_test(tc, pdf_error_009);
  tcase_add_test(tc, pdf_error_010);
  tcase_add_test(tc, pdf_error_011);
  tcase_add_test(tc, pdf_error_012);
  tcase_add_test(tc, pdf_error_013);
  tcase_add_test(tc, pdf_error_014);
  tcase_add_test(tc, pdf_error_015);
  tcase_add_test(tc, pdf_error_016);
  tcase_add_test(tc, pdf_error_017);
  tcase_add_test(tc, pdf_error_018);

  return tc;
}

/* End of pdf-error.c */

/* -*- mode: C -*- Time-stamp: "2009-08-05 21:36:52 davazp"
 *
 *       File:         pdf-dealloc.c
 *       Date:         Wed Sep 10 21:28:49 2008
 *
 *       GNU PDF Library - Allocation Module - pdf_dealloc test case
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
#include <string.h>
#include <pdf.h>
#include <check.h>

/*
 * Test: pdf_dealloc_001
 * Description:
 *   Deallocate some memory.
 * Success conditions:
 *   1. The call should not produce an error.
 */
START_TEST(pdf_dealloc_001)
{
  char *data;

  pdf_init();

  /* Allocate some memory */
  data = pdf_alloc (100);
  fail_if (data == NULL);

  /* Deallocate it */
  pdf_dealloc (data);
}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_dealloc (void)
{
  TCase *tc = tcase_create("pdf_dealloc");
  tcase_add_test (tc, pdf_dealloc_001);
  return tc;
}

/* End of pdf-dealloc.c */

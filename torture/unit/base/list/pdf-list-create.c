/* -*- mode: C -*- Time-stamp: "08/03/17 14:34:28 jemarch"
 *
 *       File:         pdf-list-create.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_list_create
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

#include <stdbool.h>
#include <stdio.h>
#include <pdf.h>
#include <check.h>


bool l_comp (const void * elemb, const void * elema)
{
  int *elem, *elem2;
  elem = (int*) elemb;
  elem2 = (int*) elema;
        
  if (*elem == *elem2)
    {
      return 1;
    }
  return 0;
}

void l_disp (const void * elema)
{
}


/*
 * Test: pdf_list_create_001
 * Description:
 *   Try to create an empty list.
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_list_create_001)
{
  pdf_list_t list;
  
  fail_if (pdf_list_create (l_comp, l_disp, 0, &list) != PDF_OK);

  pdf_list_destroy (list);
}
END_TEST


/*
 * Test: pdf_list_create_002
 * Description:
 *   Try to create an empty list given a NULL list pointer.
 * Success condition:
 *   Returns PDF_EBADDATA
 */
START_TEST (pdf_list_create_002)
{
  fail_if (pdf_list_create (l_comp, l_disp, 0, NULL) != PDF_EBADDATA);
}
END_TEST


/*
 * Test: pdf_list_create_003
 * Description:
 *   Try to create an empty list allowing duplicates.
 * Success condition:
 *   Returns PDF_OK
 */
START_TEST (pdf_list_create_003)
{
  pdf_list_t list;
  
  fail_if (pdf_list_create (l_comp, l_disp, 1, &list) != PDF_OK);

  pdf_list_destroy (list);
}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_list_create (void)
{
  TCase *tc = tcase_create("pdf_list_create");
  tcase_add_test(tc, pdf_list_create_001);
  tcase_add_test(tc, pdf_list_create_002);
  tcase_add_test(tc, pdf_list_create_003);

  return tc;
}

/* End of pdf-list-create.c */

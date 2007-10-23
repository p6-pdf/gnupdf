/* -*- mode: C -*- Time-stamp: "07/10/23 12:58:51 jemarch"
 *
 *       File:         test-obj_dupequality.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Sat Jul  7 20:25:39 2007
 *
 *       GNU PDF Library - Test suite for the PDF object dupping and equality
 *
 */

/* Copyright (C) 2007 Free Software Foundation, Inc. */

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
#include <pdf_obj.h>

#define TEST_DUP_EQUAL(type, crargs)                                \
  do                                                                \
    {                                                               \
      obj1 = pdf_create_##type crargs;                              \
      obj2 = pdf_obj_dup (obj1);                                    \
      if (!pdf_obj_equal_p (obj1, obj2))                            \
        {                                                           \
          printf ("Error comparing two `" #type"' objects.\n");     \
          success = PDF_FALSE;                                      \
        }                                                           \
      pdf_destroy_obj (obj1);                                       \
      pdf_destroy_obj (obj2);                                       \
    }                                                               \
  while (0)


int
main ()
{
  int success;
  pdf_obj_t obj1;
  pdf_obj_t obj2;

  success = PDF_TRUE;

  TEST_DUP_EQUAL(null,());
  TEST_DUP_EQUAL(boolean, (PDF_TRUE));
  TEST_DUP_EQUAL(integer, (666));
  TEST_DUP_EQUAL(real, (12.2));
  TEST_DUP_EQUAL(string, ("foobar",6));
  TEST_DUP_EQUAL(name, ("foobar",6));
  TEST_DUP_EQUAL(array,());
  TEST_DUP_EQUAL(dict,());
  TEST_DUP_EQUAL(indirect, (0,0));

  if (success)
    {
      return 0;
    }
  else
    {
      return 1;
    }
}

/* End of test-obj_dupequality.c */

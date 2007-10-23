/* -*- mode: C -*- Time-stamp: "07/10/23 12:58:34 jemarch"
 *
 *       File:         test-obj_createdestroy.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Sat Jul  7 16:05:07 2007
 *
 *       GNU PDF Library - Test suite for PDF object creation and destruction
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

int 
main ()
{
  pdf_obj_t obj;

  /* This testsuite consist of the creation and destruction of all the
     object types defined in the pdf_obj library module.
     
     After that, it return SUCCESS. */

  obj = pdf_create_null ();
  pdf_destroy_obj (obj);

  obj = pdf_create_boolean (PDF_TRUE);
  pdf_destroy_obj (obj);

  obj = pdf_create_boolean (PDF_FALSE);
  pdf_destroy_obj (obj);

  obj = pdf_create_integer (666);
  pdf_destroy_obj (obj);

  obj = pdf_create_real (666.6);
  pdf_destroy_obj (obj);

  /* Size cover all the string */
  obj = pdf_create_string ("foo", 3);
  pdf_destroy_obj (obj);

  /* Size doesnt cover all the string */
  obj = pdf_create_string ("foo", 2);
  pdf_destroy_obj (obj);

  /* Empty string */
  obj = pdf_create_string ("", 0);
  pdf_destroy_obj (obj);

  /* Zero size */
  obj = pdf_create_string ("foo", 0);
  pdf_destroy_obj (obj);

  obj = pdf_create_array ();
  pdf_destroy_obj (obj);

  obj = pdf_create_dict ();
  pdf_destroy_obj (obj);

  pdf_create_indirect (0, 0);
  pdf_destroy_obj (obj);

  return 0;
}


/* End of test-obj_createdestroy.c */

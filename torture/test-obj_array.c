/* -*- mode: C -*- Time-stamp: "07/10/23 12:58:25 jemarch"
 *
 *       File:         test-obj_array.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Sat Jul  7 21:40:36 2007
 *
 *       GNU PDF Library - Test suite for the PDF array object
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
#include <string.h>
#include <xalloc.h>
#include <stdio.h>
#include <pdf_obj.h>


int
main ()
{
  int success;
  pdf_obj_t obj;
  pdf_obj_t obj2;
  pdf_obj_t elt1;
  pdf_obj_t elt2;
  pdf_obj_t elt3;
  pdf_obj_t elt4;
  pdf_obj_t elt5;
  pdf_obj_t elt6;
  char *string;

  success = PDF_TRUE;

  obj = pdf_create_array ();
  if (pdf_get_array_size (obj) != 0)
    {
      printf ("Error getting the size of an empty array; %d\n",
              pdf_get_array_size (obj));
      success = PDF_FALSE;
    }

  elt1 = pdf_create_string ("one", 3);
  elt2 = pdf_create_array ();

  pdf_add_array_elt (obj,
                     pdf_get_array_size (obj),
                     elt1);
  pdf_add_array_elt (obj,
                     pdf_get_array_size (obj),
                     elt2);
  pdf_add_array_elt (obj,
                     -1,
                     elt2);

  if (pdf_get_array_size (obj) != 2)
    {
      printf ("Error getting the size of a 2 sized array: %d\n",
              pdf_get_array_size (obj));
      success = PDF_FALSE;
    }

  string = (char *) xmalloc (pdf_get_string_size (pdf_get_array_elt (obj, 0)));
  memcpy (string,
          pdf_get_string_data (pdf_get_array_elt (obj, 0)),
          pdf_get_string_size (pdf_get_array_elt (obj, 0)));
  
  if (memcmp (string, "one", 3))
    {
      printf ("Error getting the string value of an element: %s\n", string);
      success = PDF_FALSE;
    }

  obj2 = pdf_obj_dup (obj);

  if (!pdf_obj_equal_p (obj, obj2))
    {
      printf ("Error comparing two equal arrays.\n");
      success = PDF_FALSE;
    }

  elt3 = pdf_create_array ();
  pdf_append_array_elt (obj2, elt3);

  if (pdf_obj_equal_p (obj, obj2))
    {
      printf ("Error comparing two distinct arrays.\n");
      success = PDF_FALSE;
    }

  if (!pdf_remove_array_elt (obj, 1))
    {
      printf("Error removing an element of an array.\n");
      success = PDF_FALSE;
    }

  if (pdf_get_array_size (obj) != 1)
    {
      printf("Error getting the size of an array with previous element removal.");
      success = PDF_FALSE;
    }


  if (success)
    {
      return 0;
    }
  else
    {
      return 1;
    }
}

/* End of test-obj_array.c */

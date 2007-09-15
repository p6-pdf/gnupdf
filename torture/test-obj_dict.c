/* -*- mode: C -*- Time-stamp: "07/09/15 13:05:17 jemarch"
 *
 *       File:         test-obj_dict.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Sat Sep 15 12:41:37 2007
 *
 *       GNU PDF Library - Test suite for the PDF dict object
 *
 */

/* Copyright (C) 2007 Jose E. Marchesi */

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
  pdf_obj_t dict;
  pdf_obj_t key1;
  pdf_obj_t value1;
  pdf_obj_t key2;
  pdf_obj_t value2;
  pdf_obj_t entry1;
  pdf_obj_t entry2;

  success = PDF_TRUE;

  key1 = pdf_create_name ("key1", 4);
  value1 = pdf_create_string ("value1", 6);
  key2 = pdf_create_name ("key2", 4);
  value2 = pdf_create_string ("value2", 6);

  dict = pdf_create_dict ();
  
  if (pdf_create_dict_entry (dict, key1, value1) == 
      PDF_ERROR)
    {
      success = PDF_FALSE;
      printf ("Error creating dictionary entry for key1 and value1\n");
    }

  if (pdf_create_dict_entry (dict, key2, value2) ==
      PDF_ERROR)
    {
      success = PDF_FALSE;
      printf ("Error creating dictionary entry for key2 and value2\n");
    }
  
  entry1 = pdf_get_dict_entry (dict, key1);
  if (entry1 == NULL)
    {
      success = PDF_FALSE;
      printf ("Error getting dictionary entry\n");
    }
  else
    {
      if (strncmp (pdf_get_string_data (entry1), 
                   "value1", 
                   pdf_get_string_size (entry1)) != 0)
        {
          success = PDF_FALSE;
          printf ("Invalid value in dictionary entry for key1");
        }
    }

  entry2 = pdf_get_dict_entry (dict, key2);
  if (entry2 == NULL)
    {
      success = PDF_FALSE;
      printf ("Error getting dictionary entry\n");
    }
  else
    {
      if (strncmp (pdf_get_string_data (entry2), 
                   "value2", 
                   pdf_get_string_size (entry2)) != 0)
        {
          success = PDF_FALSE;
          printf ("Invalid value in dictionary entry for key2");
        }
    }

  pdf_destroy_obj (key1);
  pdf_destroy_obj (value1);
  pdf_destroy_obj (key2);
  pdf_destroy_obj (value2);
  pdf_destroy_obj (dict);

  if (success)
    {
      return 0;
    }
  else
    {
      return 1;
    }
}

/* End of test-obj_dict.c */

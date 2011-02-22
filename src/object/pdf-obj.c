/* -*- mode: C -*-
 *
 *       File:         pdf-obj.c
 *       Date:         Sat Jul  7 03:04:30 2007
 *
 *       GNU PDF Library - PDF objects implementation
 *
 */

/* Copyright (C) 2007, 2008, 2011 Free Software Foundation, Inc. */

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
#include <stdlib.h>
#include <xalloc.h>

#include <pdf_obj.h>

/* ------------------------ Data types ------------------------ */

/* The contents of the pdf_obj_s structure (defined in pdf-obj.h) are
   internally interpreted as follows:

   Some flags are stored in the 'f' field:

   - Bit 31:     0 => Direct object.  1 => Indirect object.
   - Bits 15..0: Type of the object:
     + 0 => Null.
     + 1 => Boolean.
     + 2 => Integer.
     + 3 => Real.
     + 4 => Name.
     + 5 => String.
     + 6 => Array.
     + 7 => Dictionary.
     + 8 => Stream.

   The null object contains {0, 0, NULL}.

   The values of direct scalar objects are stored in the structure like:

   - Boolean => v = PDF_FALSE|PDF_TRUE
   - Integer => v = pdf_i32_t
   - Real    => v = pdf_real_t

   Direct non-scalar objects (names, strings, arrays, dictionaries and
   streams) are stored in pdf_obj_*_s structures, pointed by 'p'. */

/* A PDF name object is an atomic symbol uniquely defined by a
   sequence of regular characters. It has no internal structure. */

struct pdf_name_s
{
  pdf_char_t *data;
  pdf_size_t  size;
};

/* A PDF string is a sequence of bytes, in the range of 0-255.  In
   particular it may contain NULL characters (code 0 in the ASCII
   CCS).  */

struct pdf_string_s
{
  pdf_char_t *data;
  pdf_size_t  size;
};

/* A PDF array is a one-dimensional collection of objects arranged
   sequentially. */

struct pdf_array_s
{
  pdf_list_t obj_list;
};

/* A PDF dictionary object is an associative table containing pairs of
   objects.  The first element of the pair is the `key' and the second
   element is the `value'.

   Keys should be names.  The values may be of any type (including the
   null object). */

struct pdf_dict_entry_s
{
  pdf_obj_t key;
  pdf_obj_t value;
};

struct pdf_dict_s
{
  pdf_list_t obj_list;
}

/* A PDF stream object is composed by a dictionary describing the
   stream, a reference to a stm structure, and a pointer (bytes
   offset) that point to the beginning of the stream data in the stm
   backend.

   Note that the actual data of the stream is stored in a temporary
   file.  */

struct pdf_stream_s
{
  pdf_obj_t *dict;
  pdf_stm_t  stm;
  pdf_off_t  offset;
};

/* Private functions prototypes */

/* Public functions */

/* Private functions */

/* End of pdf_obj.c */

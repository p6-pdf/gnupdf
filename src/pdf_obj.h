/* -*- mode: C -*- Time-stamp: "07/07/07 03:29:15 jemarch"
 *
 *       File:         pdf_obj.h
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Sat Jul  7 01:10:11 2007
 *
 *       GNU PDF Library - PDF objects header file
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

/* This library module implement the basic objects that compound a PDF
   document:

      - Null object (PDF_NULL_OBJECT)
      - Boolean value (pdf_boolean_t)
      - Integer number (pdf_integer_t)
      - Real number (pdf_real_t)
      - String (pdf_string_t)
      - Name (pdf_name_t)
      - Array (pdf_array_t)
      - Dictionary (pdf_dict_t)
      - Indirect object (pdf_indirect_t)

   A generic data type capable to contain any type of PDF object is
   also included (pdf_obj_t). */

#ifndef PDF_OBJ_H
#define PDF_OBJ_H

#include <gl_list.h>


/* The PDF NULL object has a type and a value that are unequal to
   those of any other object. There is only one possible value for
   this object type: null.

   There is not a `pdf_null_t' type. In order to reflect a PDF null
   object we use `pdf_obj_t'. */


/* PDF_TRUE and PDF_FALSE (from `pdf_base.h') should be used as the values of
   boolean variables. 

   Can be used as: 
   
   - Values of array elements
   - Values of dict entries
   - Values in PostScript calculator functions
*/

typedef int pdf_boolean_t;

typedef int pdf_integer_t;
typedef float pdf_real_t;

/* According to the PDF reference, a PDF name object is an atomic
   symbol uniquely defined by a sequence of regular characters. It has
   no internal structure.

   In the practice, PDF uses name objects in order to store
   information (such as font names). In that situation, it is
   recommended to code such information in UTF-8. Due to this
   stupidity we should store the entire byte sequence that conform the
   name. */

typedef char* pdf_name_t;

/* A PDF string is a sequence of bytes, in the range of 0-255. In
   particular it may contain NULL characters (code 0 in the ASCII
   CCS). */
 
struct pdf_string_s
{
  unsigned char *data;
  int size;
};

typedef struct pdf_string_s *pdf_string_t;

/* A PDF array object is a one-dimensional collection of objects
   arranged sequentially. 
   
   Note that the list of objects is heterogeneous, so we must use
   `pdf_obj_s'.
*/

struct pdf_obj_s; /* Forward reference */

struct pdf_array_s
{
  gl_list_t *objs;
};

typedef struct pdf_array_s *pdf_array_t;

/* A PDF dictionary object is an associative table containing pairs of
   objects. The first of the pair is the `key' and the second element
   is the `value'.

   Any `key' object should be a name pdf object. On the other hand,
   the values may have any pdf object type. */

struct pdf_dict_s
{


};

typedef struct pdf_dict_s *pdf_dict_t;

/* A PDF indirect object is composed by:

   - Object number (positive integer)
   - Generation number (non-negative integer) */

struct pdf_indirect_s
{
  /* Object number (cannot be 0) */
  unsigned int on;

  /* Generation number */
  unsigned int gn;
};

typedef struct pdf_indirect_s *pdf_indirect_t;
   

enum pdf_obj_type_t
{
  PDF_NULL_OBJ = 0,
  PDF_BOOLEAN_OBJ,
  PDF_INT_OBJ,
  PDF_REAL_OBJ,
  PDF_STRING_OBJ,
  PDF_NAME_OBJ,
  PDF_ARRAY_OBJ,
  PDF_DICT_OBJ,
  PDF_INDIRECT_OBJ
};

/* A `pdf_obj_s' structure stores a PDf object. The object may be of
   any type (including NULL). */

struct pdf_obj_s
{
  enum pdf_obj_type_t type;
  
  union
  {

    pdf_boolean_t boolean;
    pdf_integer_t integer;
    pdf_real_t real;
    pdf_string_t string;
    pdf_name_t name;
    pdf_array_t array;
    pdf_dict_t dict;
    pdf_indirect_t indirect;

  } value;
};

typedef struct pdf_obj_s *pdf_obj_t;


#endif /* pdf_obj.h */

/* End of pdf_obj.h */

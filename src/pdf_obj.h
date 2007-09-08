/* -*- mode: C -*- Time-stamp: "07/09/08 13:51:08 jemarch"
 *
 *       File:         pdf_obj.h
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Sat Jul  7 01:10:11 2007
 *
 *       GNU PDF Library - PDF objects
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

   A generic data type capable to contain any kind of PDF object is
   also included (pdf_obj_t). */

#ifndef PDF_OBJ_H
#define PDF_OBJ_H

#include <config.h>
#include <gl_array_list.h>
#include <pdf_base.h>

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

/* PDF integers and reals are implemented using basic C types. */

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

struct pdf_name_s
{
  unsigned char *data;
  int size;
}; 

typedef struct pdf_name_s *pdf_name_t;

/* A PDF string is a sequence of bytes, in the range of 0-255. In
   particular it may contain NULL characters (code 0 in the ASCII
   CCS). 

   Corollary: NEVER NEVER NEVER EVER use a PDF string as an input
   expecting null-terminated strings. You have been warned. */
 
struct pdf_string_s
{
  unsigned char *data;
  int size;
};

typedef struct pdf_string_s *pdf_string_t;

/* A PDF array object is a one-dimensional collection of objects
   arranged sequentially. 
   
   Note that the list of objects is heterogeneous, so we must use
   `pdf_obj_s'. */

struct pdf_array_s
{
  gl_list_t objs;
};

typedef struct pdf_array_s *pdf_array_t;

/* A PDF dictionary object is an associative table containing pairs of
   objects. The first element of the pair is the `key' and the second
   element is the `value'.

   Any `key' object should be a name pdf object. On the other hand,
   the values may have any pdf object type. */

struct pdf_obj_s; /* Forward reference */

struct pdf_dict_entry_s
{
  struct pdf_obj_s *key;
  struct pdf_obj_s *value;
};

typedef struct pdf_dict_entry_s *pdf_dict_entry_t;

struct pdf_dict_s
{
  gl_list_t entries;
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

/* A `pdf_obj_s' structure stores a PDF object. The object may be of
   any type (including NULL). */

struct pdf_obj_s
{
  enum pdf_obj_type_t type;
  
  union
  {

    pdf_boolean_t boolean;
    pdf_integer_t integer;
    pdf_real_t real;
    struct pdf_string_s string;
    struct pdf_name_s name;
    struct pdf_array_s array;
    struct pdf_dict_s dict;
    struct pdf_indirect_s indirect;

  } value;
};

typedef struct pdf_obj_s *pdf_obj_t;

/* Accessing macros */

#define IS_NULL(obj) \
  (obj->type == PDF_NULL_OBJ)
#define IS_BOOL(obj) \
  (obj->type == PDF_BOOLEAN_OBJ)
#define IS_INT(obj) \
  (obj->type == PDF_INT_OBJ)
#define IS_REAL(obj) \
  (obj->type == PDF_REAL_OBJ)
#define IS_STRING(obj) \
  (obj->type == PDF_STRING_OBJ)
#define IS_NAME(obj) \
  (obj->type == PDF_NAME_OBJ)
#define IS_ARRAY(obj) \
  ((obj)->type == PDF_ARRAY_OBJ)
#define IS_DICT(obj) \
  ((obj)->type == PDF_DICT_OBJ)
#define IS_INDIRECT(obj) \
  ((obj)->type == PDF_INDIRECT_OBJ)

/* The following macros references values */
#define GET_BOOL(obj) \
  ((obj)->value.boolean)
#define GET_INT(obj) \
  ((obj)->value.integer)
#define GET_REAL(obj) \
  ((obj)->value.real)
#define GET_STRING(obj) \
  ((obj)->value.string)
#define GET_NAME(obj) \
  ((obj)->value.name)
#define GET_ARRAY(obj) \
  ((obj)->value.array)
#define GET_DICT(obj) \
  ((obj)->value.dict)
#define GET_INDIRECT(obj) \
  ((obj)->value.indirect)

/* 
 * Forward declarations
 */

/* Object creation */
pdf_obj_t pdf_create_null (void);
pdf_obj_t pdf_create_boolean (int value);
pdf_obj_t pdf_create_integer (int value);
pdf_obj_t pdf_create_real (float value);
pdf_obj_t pdf_create_string (unsigned char *value, int size);
pdf_obj_t pdf_create_name (unsigned char *value, int size);
pdf_obj_t pdf_create_array (void);
pdf_obj_t pdf_create_dict (void);
pdf_obj_t pdf_create_indirect (unsigned int on, unsigned int gn);
pdf_obj_t pdf_obj_dup (pdf_obj_t obj);

/* Object destruction */
int pdf_destroy_obj (pdf_obj_t obj);

/* Testing for equality */
int pdf_obj_equal_p (pdf_obj_t obj1, pdf_obj_t obj2);

/* Managing objects of basic types */
int pdf_get_bool (pdf_obj_t obj);
void pdf_set_bool (pdf_obj_t obj, int value);
int pdf_get_int (pdf_obj_t obj);
void pdf_set_int (pdf_obj_t obj, int value);
float pdf_get_real (pdf_obj_t obj);
void pdf_set_real (pdf_obj_t obj, float value);

/* Managing strings */
int pdf_get_string_size (pdf_obj_t obj);
char *pdf_get_string_data (pdf_obj_t obj);

/* Managing names */
int pdf_get_name_size (pdf_obj_t obj);
char *pdf_get_name_data (pdf_obj_t obj);

/* Managing arrays */
int pdf_get_array_size (pdf_obj_t obj);
int pdf_remove_array_elt (pdf_obj_t obj, int index);
pdf_obj_t pdf_get_array_elt (pdf_obj_t obj, int index);
int pdf_set_array_elt (pdf_obj_t obj, int index, pdf_obj_t elt);
int pdf_add_array_elt (pdf_obj_t obj, int index, pdf_obj_t elt);
int pdf_append_array_elt (pdf_obj_t obj, pdf_obj_t elt);

/* Managing dictionaries */
int pdf_get_dict_size (pdf_obj_t obj);
int pdf_dict_key_p (pdf_obj_t obj, pdf_obj_t key);
pdf_obj_t pdf_get_dict_entry (pdf_obj_t obj, pdf_obj_t key);
int pdf_remove_dict_entry (pdf_obj_t obj, pdf_obj_t key);
int pdf_create_dict_entry (pdf_obj_t obj, pdf_obj_t key, pdf_obj_t value);

#endif /* pdf_obj.h */

/* End of pdf_obj.h */

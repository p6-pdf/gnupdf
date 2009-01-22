/* -*- mode: C -*- Time-stamp: "2008-03-06 20:09:02 gerel"
 *
 *       File:         pdf-obj.h
 *       Date:         Sat Jul  7 01:10:11 2007
 *
 *       GNU PDF Library - PDF objects
 *
 */

/* Copyright (C) 2007, 2008 Free Software Foundation, Inc. */

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

      - Null object (PDF_NULL_OBJ)
      - Boolean value (pdf_bool_t)
      - Integer number (int)
      - Real number (pdf_real_t)
      - String
      - Name
      - Array
      - Dictionary
      - Indirect object
      - Stream object

   And some object types used by the tokeniser:
      - Comment
      - Keyword
      - Valueless tokens:
        - PDF_DICT_START_TOK
        - PDF_DICT_END_TOK
        - PDF_ARRAY_START_TOK
        - PDF_ARRAY_END_TOK
        - PDF_PROC_START_TOK
        - PDF_PROC_END_TOK

   A generic data type capable to contain any kind of PDF object is
   also included (pdf_obj_t). */

#ifndef PDF_OBJ_H
#define PDF_OBJ_H

#include "config.h"
#include "pdf-types.h"
#include "pdf-list.h"
#include "pdf-base.h"
#include "pdf-stm.h"

/* BEGIN PUBLIC */
/* pdf-obj.h */

enum pdf_obj_type_e
{
  PDF_NULL_OBJ = 0,
  PDF_BOOLEAN_OBJ = 1,
  PDF_INT_OBJ = 2,
  PDF_REAL_OBJ = 3,
  PDF_STRING_OBJ = 4,
  PDF_NAME_OBJ = 5,
  PDF_ARRAY_OBJ = 6,
  PDF_DICT_OBJ = 7,
  PDF_INDIRECT_OBJ = 8,
  PDF_STREAM_OBJ = 9,

  /* the following types are only used by the tokeniser */
  PDF_COMMENT_TOK = 10,
  PDF_KEYWORD_TOK = 11,
  PDF_DICT_START_TOK = 12,
  PDF_DICT_END_TOK = 13,
  PDF_ARRAY_START_TOK = 14,
  PDF_ARRAY_END_TOK = 15,
  PDF_PROC_START_TOK = 16,
  PDF_PROC_END_TOK = 17,
};
typedef enum pdf_obj_type_e pdf_obj_type_t;

struct pdf_obj_s;  /* opaque type */
typedef struct pdf_obj_s *pdf_obj_t;

/* Object creation */
pdf_status_t pdf_obj_null_new (pdf_obj_t *obj);
pdf_status_t pdf_obj_boolean_new (pdf_bool_t value, pdf_obj_t *obj);
pdf_status_t pdf_obj_integer_new (int value, pdf_obj_t *obj);
pdf_status_t pdf_obj_real_new (pdf_real_t value, pdf_obj_t *obj);
pdf_status_t pdf_obj_string_new (const pdf_char_t *value, pdf_size_t size,
                                 pdf_obj_t *obj);
pdf_status_t pdf_obj_name_new (const pdf_char_t *value, pdf_size_t size,
                               pdf_obj_t *obj);
pdf_status_t pdf_obj_array_new (pdf_obj_t *obj);
pdf_status_t pdf_obj_dict_new (pdf_obj_t *obj);
pdf_status_t pdf_obj_indirect_new (unsigned int on, unsigned int gn,
                                   pdf_obj_t *obj);
pdf_status_t pdf_obj_stream_new (pdf_obj_t dict, pdf_stm_t stm,
                                 pdf_off_t data, pdf_obj_t *obj);
pdf_status_t pdf_tok_valueless_new (pdf_obj_type_t type, pdf_obj_t *obj);
pdf_status_t pdf_tok_comment_new (const pdf_char_t *value, pdf_size_t size,
                                  pdf_bool_t continuation, pdf_obj_t *obj);
pdf_status_t pdf_tok_keyword_new (const pdf_char_t *value, pdf_size_t size,
                                  pdf_obj_t *obj);
pdf_status_t pdf_obj_dup (const pdf_obj_t obj, pdf_obj_t *new);

/* Object destruction */
pdf_status_t pdf_obj_destroy (pdf_obj_t obj);

/* Common functions */
pdf_obj_type_t pdf_obj_type (pdf_obj_t obj);
pdf_bool_t pdf_obj_equal_p (pdf_obj_t obj1, pdf_obj_t obj2);

/* Managing objects of basic types */
pdf_bool_t pdf_obj_bool_value (pdf_obj_t bool_obj);
int pdf_obj_int_value (pdf_obj_t int_obj);
pdf_real_t pdf_obj_real_value (pdf_obj_t real_obj);

/* Managing strings */
pdf_size_t pdf_obj_string_size (pdf_obj_t str_obj);
const pdf_char_t *pdf_obj_string_data (pdf_obj_t str_obj);

/* Managing names */
pdf_size_t pdf_obj_name_size (const pdf_obj_t name);
const pdf_char_t *pdf_obj_name_data (const pdf_obj_t name);
pdf_bool_t pdf_obj_name_equal_p (const pdf_obj_t obj, const pdf_char_t *data);

/* Managing keywords */
pdf_size_t pdf_tok_keyword_size (const pdf_obj_t keyword);
const pdf_char_t *pdf_tok_keyword_data (const pdf_obj_t keyword);
pdf_bool_t pdf_tok_keyword_equal_p (const pdf_obj_t obj,
                                    const pdf_char_t *data);

/* Managing comments */
pdf_size_t pdf_tok_comment_size (const pdf_obj_t comment);
const pdf_char_t *pdf_tok_comment_data (const pdf_obj_t comment);

/* Managing arrays */
pdf_size_t pdf_obj_array_size (const pdf_obj_t array);
pdf_status_t pdf_obj_array_get (const pdf_obj_t array, pdf_size_t index,
                                pdf_obj_t *obj);
pdf_status_t pdf_obj_array_set (pdf_obj_t array, pdf_size_t index,
                                const pdf_obj_t new_obj,
                                pdf_obj_t *old_obj);
pdf_status_t pdf_obj_array_insert (pdf_obj_t array, pdf_size_t index,
                                   const pdf_obj_t obj);
pdf_status_t pdf_obj_array_append (pdf_obj_t array, const pdf_obj_t obj);
pdf_status_t pdf_obj_array_remove (pdf_obj_t array, pdf_size_t index,
                                   pdf_obj_t *obj);
pdf_status_t pdf_obj_array_clear (pdf_obj_t array);
pdf_status_t pdf_obj_array_clear_nodestroy (pdf_obj_t array);

/* Managing dictionaries */
pdf_size_t pdf_obj_dict_size (const pdf_obj_t dict);
pdf_bool_t pdf_obj_dict_key_p (const pdf_obj_t dict, const pdf_obj_t key);
pdf_bool_t pdf_obj_dict_keyc_p (const pdf_obj_t dict, const pdf_char_t *key);
pdf_status_t pdf_obj_dict_get (const pdf_obj_t dict, const pdf_obj_t key,
                               pdf_obj_t *value);
pdf_status_t pdf_obj_dict_getc (const pdf_obj_t dict, const pdf_char_t *key,
                                pdf_obj_t *value);
pdf_status_t pdf_obj_dict_add (pdf_obj_t dict, const pdf_obj_t key,
                               pdf_obj_t value);
pdf_status_t pdf_obj_dict_remove (pdf_obj_t dict, const pdf_obj_t key);
pdf_status_t pdf_obj_dict_clear (pdf_obj_t dict);
pdf_status_t pdf_obj_dict_clear_nodestroy (pdf_obj_t dict);

/* Managing streams */
pdf_obj_t pdf_obj_stream_dict (pdf_obj_t stream);
pdf_stm_t pdf_obj_stream_stm (pdf_obj_t stream);
pdf_off_t pdf_obj_stream_data (pdf_obj_t stream);

/* Managing indirect objects */
unsigned int pdf_obj_indirect_on (pdf_obj_t indir);
unsigned int pdf_obj_indirect_gn (pdf_obj_t indir);

/* END PUBLIC */



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

/* NOTE: PDF integers and reals are implemented in pdf_base.h */

/* According to the PDF reference, a PDF name object is an atomic
   symbol uniquely defined by a sequence of non-null characters. It has
   no internal structure.

   In the practice, PDF uses name objects in order to store
   information (such as font names). In that situation, it is
   recommended to code such information in UTF-8. Due to this
   stupidity we should store the entire byte sequence that conform the
   name.

   A pdf_obj_buffer_s structure is used to store a name's value. */


/* A PDF string is a sequence of bytes, in the range of 0-255. In
   particular it may contain NULL characters (code 0 in the ASCII
   CCS). 

   Corollary: NEVER NEVER NEVER EVER use a PDF string as an input
   expecting null-terminated strings. You have been warned.

   A pdf_obj_buffer_s structure is used to store a string's value. */


/* A PDF array object is a one-dimensional collection of objects
   arranged sequentially. 
   
   Note that the list of objects is heterogeneous, so we must use
   `pdf_obj_s'. */


/* A PDF dictionary object is stored using pdf_hash_t.
   Any `key' object should be a name pdf object. On the other hand,
   the values may have any pdf object type. */


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

/* A PDF stream object is composed by a dictionary describing the
   stream, a reference to a stm structure, and a pointer (octects
   offset) that point to the beginning of the stream data in the stm
   backend.

   Note that the actual data of the stream may not be stored in
   memory. It depends on the backend type of the stm. */

struct pdf_stream_s
{
  struct pdf_obj_s *dict;
  pdf_stm_t stm;
  pdf_off_t data;
};

/* pdf_obj_buffer_s is an internal structure used for tokens with an
 * associated byte array. 'data' may or may not be null-terminated,
 * but size never includes the trailing null. */
struct pdf_obj_buffer_s
{
  pdf_char_t *data;
  pdf_size_t size;
};

struct pdf_comment_s
{
  pdf_char_t *data;
  pdf_size_t size;
  /* This structure shares a common initial sequence with pdf_obj_buffer_s,
   * so the above fields can be accessed via obj->value.buffer. */

  pdf_bool_t continuation;  /* is data continued from a previous token? */
};


/* A `pdf_obj_s' structure stores a PDF object. The object may be of
   any type (including NULL). */

struct pdf_obj_s
{
  pdf_obj_type_t type;

  union
  {

    struct pdf_obj_buffer_s buffer;
    pdf_bool_t boolean;
    int integer;
    pdf_real_t real;
    pdf_list_t array;
    pdf_hash_t dict;
    struct pdf_indirect_s indirect;
    struct pdf_stream_s stream;
    struct pdf_comment_s comment;

  } value;
};

/* Accessing macros */

#define IS_NULL(obj) \
  ((obj)->type == PDF_NULL_OBJ)
#define IS_BOOL(obj) \
  ((obj)->type == PDF_BOOLEAN_OBJ)
#define IS_INT(obj) \
  ((obj)->type == PDF_INT_OBJ)
#define IS_REAL(obj) \
  ((obj)->type == PDF_REAL_OBJ)
#define IS_NUMBER(obj) \
  (IS_REAL((obj)) || IS_INT((obj)))
#define IS_STRING(obj) \
  ((obj)->type == PDF_STRING_OBJ)
#define IS_NAME(obj) \
  ((obj)->type == PDF_NAME_OBJ)
#define IS_ARRAY(obj) \
  ((obj)->type == PDF_ARRAY_OBJ)
#define IS_DICT(obj) \
  ((obj)->type == PDF_DICT_OBJ)
#define IS_INDIRECT(obj) \
  ((obj)->type == PDF_INDIRECT_OBJ)
#define IS_STREAM(obj) \
  ((obj)->type == PDF_STREAM_OBJ)

/* The following macros references values */
#define GET_BOOL(obj) \
  ((obj)->value.boolean)
#define GET_INT(obj) \
  ((obj)->value.integer)
#define GET_REAL(obj) \
  ((obj)->value.real)

struct pdf_obj_child_s
{
  pdf_obj_t key;
  pdf_obj_t value;
  int owned;
};
typedef struct pdf_obj_child_s *pdf_obj_child_t;

#endif /* pdf_obj.h */

/* End of pdf_obj.h */

/* -*- mode: C -*- Time-stamp: "2008-03-06 20:09:02 gerel"
 *
 *       File:         pdf-token-obj.h
 *       Date:         Sat Jul  7 01:10:11 2007
 *
 *       GNU PDF Library - PDF token objects
 *
 */

/* Copyright (C) 2007, 2008, 2009 Free Software Foundation, Inc. */

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

/* pdf-token-obj.{c,h} implement the token ADT, used to represent
   all types of tokens used by the token reader and writer:
      - Integer number
      - Real number
      - String
      - Name
      - Keyword
      - Comment
      - Valueless tokens:
        - PDF_TOKEN_DICT_START
        - PDF_TOKEN_DICT_END
        - PDF_TOKEN_ARRAY_START
        - PDF_TOKEN_ARRAY_END
        - PDF_TOKEN_PROC_START
        - PDF_TOKEN_PROC_END
*/

#ifndef PDF_TOKEN_OBJ_H
#define PDF_TOKEN_OBJ_H

#include "config.h"
#include "pdf-types.h"
#include "pdf-base.h"

/* BEGIN PUBLIC */
/* pdf-token-obj.h */

enum pdf_token_type_e
{
  PDF_TOKEN_INTEGER = 1,
  PDF_TOKEN_REAL = 2,
  PDF_TOKEN_STRING = 3,
  PDF_TOKEN_NAME = 4,
  PDF_TOKEN_KEYWORD = 5,
  PDF_TOKEN_COMMENT = 6,
  PDF_TOKEN_DICT_START = 7,
  PDF_TOKEN_DICT_END = 8,
  PDF_TOKEN_ARRAY_START = 9,
  PDF_TOKEN_ARRAY_END = 10,
  PDF_TOKEN_PROC_START = 11,
  PDF_TOKEN_PROC_END = 12,
};

struct pdf_token_s;  /* opaque type */
typedef struct pdf_token_s *pdf_token_t;

/* Token creation */
pdf_status_t pdf_token_integer_new (pdf_i32_t value, pdf_token_t *token);
pdf_status_t pdf_token_real_new (pdf_real_t value, pdf_token_t *token);
pdf_status_t pdf_token_string_new (const pdf_char_t *value, pdf_size_t size,
                                   pdf_token_t *token);
pdf_status_t pdf_token_name_new (const pdf_char_t *value, pdf_size_t size,
                                 pdf_token_t *token);
pdf_status_t pdf_token_keyword_new (const pdf_char_t *value, pdf_size_t size,
                                    pdf_token_t *token);
pdf_status_t pdf_token_comment_new (const pdf_char_t *value, pdf_size_t size,
                                    pdf_bool_t continued, pdf_token_t *token);
pdf_status_t pdf_token_valueless_new (enum pdf_token_type_e type,
                                      pdf_token_t *token);
pdf_status_t pdf_token_dup (const pdf_token_t token, pdf_token_t *new);

/* Token destruction */
pdf_status_t pdf_token_destroy (pdf_token_t token);

/* Common functions */
enum pdf_token_type_e pdf_token_type (pdf_token_t token);

/* Managing tokens of numeric types */
pdf_i32_t pdf_token_get_integer_value (pdf_token_t int_obj);
pdf_real_t pdf_token_get_real_value (pdf_token_t real_obj);

/* Managing strings */
pdf_size_t pdf_token_get_string_size (pdf_token_t str_obj);
const pdf_char_t *pdf_token_get_string_data (pdf_token_t str_obj);

/* Managing names */
pdf_size_t pdf_token_get_name_size (const pdf_token_t name);
const pdf_char_t *pdf_token_get_name_data (const pdf_token_t name);

/* Managing keywords */
pdf_size_t pdf_token_get_keyword_size (const pdf_token_t keyword);
const pdf_char_t *pdf_token_get_keyword_data (const pdf_token_t keyword);

/* Managing comments */
pdf_size_t pdf_token_get_comment_size (const pdf_token_t comment);
const pdf_char_t *pdf_token_get_comment_data (const pdf_token_t comment);
pdf_bool_t pdf_token_get_comment_continued (const pdf_token_t comment);

/* END PUBLIC */



/* According to the PDF reference, a PDF name object is an atomic
   symbol uniquely defined by a sequence of non-null characters. It has
   no internal structure.

   In the practice, PDF uses name objects in order to store
   information (such as font names). In that situation, it is
   recommended to code such information in UTF-8. Due to this
   stupidity we should store the entire byte sequence that conform the
   name.

   A pdf_token_buffer_s structure is used to store a name's value. */


/* A PDF string is a sequence of bytes, in the range of 0-255. In
   particular it may contain NULL characters (code 0 in the ASCII
   CCS).

   Corollary: NEVER NEVER NEVER EVER use a PDF string as an input
   expecting null-terminated strings. You have been warned.

   A pdf_token_buffer_s structure is used to store a string's value. */


/* pdf_token_buffer_s is an internal structure used for tokens with an
 * associated byte array. 'data' may or may not be null-terminated,
 * but size never includes the trailing null. */
struct pdf_token_buffer_s
{
  pdf_char_t *data;
  pdf_size_t size;
};

struct pdf_comment_s
{
  pdf_char_t *data;
  pdf_size_t size;
  /* This structure shares a common initial sequence with pdf_token_buffer_s,
   * so the above fields can be accessed via token->value.buffer. */

  pdf_bool_t continued;  /* is data continued from a previous token? */
};


/* A `pdf_token_s' structure stores a PDF object. The object may be of
   any type (including NULL). */

struct pdf_token_s
{
  enum pdf_token_type_e type;

  union
  {

    struct pdf_token_buffer_s buffer;
    pdf_i32_t integer;
    pdf_real_t real;
    struct pdf_comment_s comment;

  } value;
};

#endif /* PDF_TOKEN_OBJ_H */

/* End of pdf-token-token.h */

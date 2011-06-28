/* -*- mode: C -*-
 *
 *       File:         pdf-token.h
 *       Date:         Sat Jul  7 01:10:11 2007
 *
 *       GNU PDF Library - PDF token objects
 *
 */

/* Copyright (C) 2007-2011 Free Software Foundation, Inc. */

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

/* pdf-token.{c,h} implement the token ADT, used to represent
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

#include <config.h>

#include <pdf-error.h>
#include <pdf-fp.h>
#include <pdf-types.h>

/* BEGIN PUBLIC */

/* --------------------- Common Tokeniser module ------------------------- */

enum pdf_token_rw_flags_e
  {
    /* these flags are used for token_read and token_write */
    PDF_TOKEN_NO_NAME_ESCAPES  = 0x01, /* read/write */
    PDF_TOKEN_RET_COMMENTS     = 0x02, /* read */
    PDF_TOKEN_END_AT_STREAM    = 0x04, /* read */
    PDF_TOKEN_HEX_STRINGS      = 0x08, /* write */
    PDF_TOKEN_READABLE_STRINGS = 0x10, /* write */
  };

/* --------------------- Token Object ------------------------- */

enum pdf_token_type_e
{
  PDF_TOKEN_UNKNOWN = 0,
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

/* opaque type */
typedef struct pdf_token_s pdf_token_t;

/* Token creation */
pdf_token_t *pdf_token_integer_new (pdf_i32_t     value,
                                    pdf_error_t **error);
pdf_token_t *pdf_token_real_new (pdf_real_t    value,
                                 pdf_error_t **error);
pdf_token_t *pdf_token_string_new (const pdf_char_t  *value,
                                   pdf_size_t         size,
                                   pdf_error_t      **error);
pdf_token_t *pdf_token_name_new (const pdf_char_t  *value,
                                 pdf_size_t        size,
                                 pdf_error_t     **error);
pdf_token_t *pdf_token_keyword_new (const pdf_char_t  *value,
                                    pdf_size_t         size,
                                    pdf_error_t      **error);
pdf_token_t *pdf_token_comment_new (const pdf_char_t  *value,
                                    pdf_size_t         size,
                                    pdf_error_t      **error);
pdf_token_t *pdf_token_valueless_new (enum pdf_token_type_e   type,
                                      pdf_error_t           **error);
pdf_token_t *pdf_token_dup (const pdf_token_t  *token,
                            pdf_error_t       **error);

/* Token destruction */
void pdf_token_destroy (pdf_token_t *token);

/* Common functions */
enum pdf_token_type_e pdf_token_get_type (const pdf_token_t *token);
pdf_bool_t pdf_token_equal_p (const pdf_token_t *token1,
                              const pdf_token_t *token2);

/* Managing tokens of numeric types */
pdf_i32_t pdf_token_get_integer_value (const pdf_token_t *token);
pdf_real_t pdf_token_get_real_value (const pdf_token_t *token);

/* Managing strings */
pdf_size_t pdf_token_get_string_size (const pdf_token_t *token);
const pdf_char_t *pdf_token_get_string_data (const pdf_token_t *token);

/* Managing names */
pdf_size_t pdf_token_get_name_size (const pdf_token_t *token);
const pdf_char_t *pdf_token_get_name_data (const pdf_token_t *token);

/* Managing keywords */
pdf_size_t pdf_token_get_keyword_size (const pdf_token_t *token);
const pdf_char_t *pdf_token_get_keyword_data (const pdf_token_t *token);

/* Managing comments */
pdf_size_t pdf_token_get_comment_size (const pdf_token_t *token);
const pdf_char_t *pdf_token_get_comment_data (const pdf_token_t *token);

/* END PUBLIC */

/* ASCII codes for NUL, HT, LF, FF, CR, SP */
#define pdf_is_wspace_char(ch)                  \
  (ch == 0 || ch == 9 || ch == 10 ||            \
   ch == 12 || ch == 13 || ch == 32)

/* ASCII codes for '%', '(', ')', '/'; '<', '>', '[', ']'; '{', '}' */
#define pdf_is_delim_char(ch)                   \
  (ch == 37 || ch == 40 || ch == 41 ||          \
   ch == 47 || ch == 60 || ch == 62 ||          \
   ch == 91 || ch == 93 || ch == 123 ||         \
   ch == 125)

#define pdf_is_eol_char(ch)                     \
  (ch == 10 || ch == 13)

#define pdf_is_regular_char(ch)                 \
  (!pdf_is_wspace_char (ch) &&                  \
   !pdf_is_delim_char (ch))

#endif /* PDF_TOKEN_OBJ_H */

/* End of pdf-token.h */

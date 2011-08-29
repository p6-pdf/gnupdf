/* -*- mode: C -*-
 *
 *       File:         pdf-token.c
 *       Date:         Sat Jul  7 03:04:30 2007
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

#include <config.h>

#include <string.h>
#include <math.h>

#include <pdf-token.h>
#include <pdf-alloc.h>

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

  } value;
};

/* Private functions */

static pdf_token_t *
token_new (enum pdf_token_type_e   type,
           pdf_error_t           **error)
{
  pdf_token_t *token;

  token = (pdf_token_t *) pdf_alloc (sizeof (struct pdf_token_s));
  if (!token)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TOKENISER,
                     PDF_ENOMEM,
                     "cannot create new token: "
                     "couldn't allocate '%lu' bytes",
                     (unsigned long)sizeof (struct pdf_token_s));
      return NULL;
    }

  token->type = type;
  return token;
}

void
pdf_token_destroy (pdf_token_t *token)
{
  PDF_ASSERT_POINTER_RETURN (token);
  PDF_ASSERT_RETURN (token->type >= PDF_TOKEN_INTEGER &&
                     token->type <= PDF_TOKEN_PROC_END);

  switch (token->type)
    {
    case PDF_TOKEN_STRING:   /* fall through */
    case PDF_TOKEN_NAME:     /* fall through */
    case PDF_TOKEN_KEYWORD:  /* fall through */
    case PDF_TOKEN_COMMENT:
      {
        if (token->value.buffer.data)
          pdf_dealloc (token->value.buffer.data);
        break;
      }
    default:
      {
        /* NOP */
        break;
      }
    }

  pdf_dealloc (token);
}

static pdf_token_t *
token_buffer_new (enum pdf_token_type_e   type,
                  const pdf_char_t       *value,
                  pdf_size_t              size,
                  pdf_bool_t              nullterm,
                  pdf_error_t           **error)
{
  pdf_token_t *token;

  token = token_new (type, error);
  if (!token)
    return NULL;

  token->value.buffer.data = pdf_alloc (size + 1);
  if (!token->value.buffer.data)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TOKENISER,
                     PDF_ENOMEM,
                     "cannot create token buffer: "
                     "couldn't allocate '%lu' bytes",
                     (unsigned long)sizeof (struct pdf_token_s));
      pdf_token_destroy (token);
      return NULL;
    }

  token->value.buffer.size = size;
  memcpy (token->value.buffer.data, value, size);

  /* If the value isn't null terminated, append a non-null character
   * to catch bugs. */
  token->value.buffer.data[size] = nullterm ? '\0' : 'X';

  return token;
}


/* General functions */

enum pdf_token_type_e
pdf_token_get_type (const pdf_token_t *token)
{
  PDF_ASSERT_POINTER_RETURN_VAL (token, PDF_TOKEN_UNKNOWN);
  PDF_ASSERT_RETURN_VAL ((token->type >= PDF_TOKEN_INTEGER &&
                          token->type <= PDF_TOKEN_PROC_END),
                         PDF_TOKEN_UNKNOWN);

  return token->type;
}

pdf_bool_t
pdf_token_equal_p (const pdf_token_t *token1,
                   const pdf_token_t *token2)
{
  PDF_ASSERT_POINTER_RETURN_VAL (token1, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (token2, PDF_FALSE);
  PDF_ASSERT_RETURN_VAL ((token1->type >= PDF_TOKEN_INTEGER &&
                          token1->type <= PDF_TOKEN_PROC_END),
                         PDF_FALSE);
  PDF_ASSERT_RETURN_VAL ((token2->type >= PDF_TOKEN_INTEGER &&
                          token2->type <= PDF_TOKEN_PROC_END),
                         PDF_FALSE);

  if (token1->type != token2->type)
    return PDF_FALSE;

  switch (token1->type)
    {
    case PDF_TOKEN_DICT_START:   /* fall through */
    case PDF_TOKEN_DICT_END:     /* fall through */
    case PDF_TOKEN_ARRAY_START:  /* fall through */
    case PDF_TOKEN_ARRAY_END:    /* fall through */
    case PDF_TOKEN_PROC_START:   /* fall through */
    case PDF_TOKEN_PROC_END:
      return PDF_TRUE;

    case PDF_TOKEN_INTEGER:
      return (token1->value.integer == token2->value.integer ?
              PDF_TRUE : PDF_FALSE);

    case PDF_TOKEN_REAL:
      return (token1->value.real == token2->value.real ?
              PDF_TRUE : PDF_FALSE);

    case PDF_TOKEN_COMMENT:  /* fall through */
    case PDF_TOKEN_STRING:   /* fall through */
    case PDF_TOKEN_NAME:     /* fall through */
    case PDF_TOKEN_KEYWORD:
      {
        return ((token1->value.buffer.size == token2->value.buffer.size &&
                 memcmp (token1->value.buffer.data,
                         token2->value.buffer.data,
                         token1->value.buffer.size) == 0) ?
                PDF_TRUE : PDF_FALSE);
      }

    default:
      /* shouldn't happen */
      PDF_ASSERT_TRACE_NOT_REACHED ();
      return PDF_FALSE;
    }
}

pdf_token_t *
pdf_token_dup (const pdf_token_t  *token,
               pdf_error_t       **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (token, NULL);
  PDF_ASSERT_RETURN_VAL ((token->type >= PDF_TOKEN_INTEGER &&
                          token->type <= PDF_TOKEN_PROC_END),
                         NULL);

  switch (token->type)
    {
    case PDF_TOKEN_DICT_START:   /* fall through */
    case PDF_TOKEN_DICT_END:     /* fall through */
    case PDF_TOKEN_ARRAY_START:  /* fall through */
    case PDF_TOKEN_ARRAY_END:    /* fall through */
    case PDF_TOKEN_PROC_START:   /* fall through */
    case PDF_TOKEN_PROC_END:
      return pdf_token_valueless_new (token->type, error);

    case PDF_TOKEN_INTEGER:
      return pdf_token_integer_new (token->value.integer, error);

    case PDF_TOKEN_REAL:
      return pdf_token_real_new (token->value.real, error);

    case PDF_TOKEN_STRING:
      return pdf_token_string_new (token->value.buffer.data,
                                   token->value.buffer.size,
                                   error);
    case PDF_TOKEN_NAME:
      return pdf_token_name_new (token->value.buffer.data,
                                 token->value.buffer.size,
                                 error);
    case PDF_TOKEN_KEYWORD:
      return pdf_token_keyword_new (token->value.buffer.data,
                                    token->value.buffer.size,
                                    error);
    case PDF_TOKEN_COMMENT:
      return pdf_token_comment_new (token->value.buffer.data,
                                    token->value.buffer.size,
                                    error);
    default:
      /* shouldn't happen */
      PDF_ASSERT_TRACE_NOT_REACHED ();
      return NULL;
    }
}

pdf_token_t *
pdf_token_valueless_new (enum pdf_token_type_e   type,
                         pdf_error_t           **error)
{
  PDF_ASSERT_RETURN_VAL ((type >= PDF_TOKEN_INTEGER &&
                          type <= PDF_TOKEN_PROC_END),
                         NULL);

  switch (type)
    {
    case PDF_TOKEN_DICT_START:   /* fall through */
    case PDF_TOKEN_DICT_END:     /* fall through */
    case PDF_TOKEN_ARRAY_START:  /* fall through */
    case PDF_TOKEN_ARRAY_END:    /* fall through */
    case PDF_TOKEN_PROC_START:   /* fall through */
    case PDF_TOKEN_PROC_END:
      return token_new (type, error);
    default:
      return NULL;
    }
}

/** integers *****/

pdf_token_t *
pdf_token_integer_new (pdf_i32_t     value,
                       pdf_error_t **error)
{
  pdf_token_t *token;

  token = token_new (PDF_TOKEN_INTEGER, error);
  if (token)
    token->value.integer = value;

  return token;
}

pdf_i32_t
pdf_token_get_integer_value (const pdf_token_t *token)
{
  PDF_ASSERT_POINTER_RETURN_VAL (token, 0);
  PDF_ASSERT_RETURN_VAL (token->type == PDF_TOKEN_INTEGER, 0);

  return token->value.integer;
}

/** reals *****/

pdf_token_t *
pdf_token_real_new (pdf_real_t    value,
                    pdf_error_t **error)
{
  pdf_token_t *token;

  if (isnan (value))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TOKENISER,
                     PDF_EBADDATA,
                     "cannot create real token from NaN value");
      return NULL;
    }

  if (isinf (value))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TOKENISER,
                     PDF_EBADDATA,
                     "cannot create real token from infinite value");
      return NULL;
    }

  token = token_new (PDF_TOKEN_REAL, error);
  if (token)
    token->value.real = value;

  return token;
}

pdf_real_t
pdf_token_get_real_value (const pdf_token_t *token)
{
  PDF_ASSERT_POINTER_RETURN_VAL (token, 0.0);
  PDF_ASSERT_RETURN_VAL (token->type == PDF_TOKEN_REAL, 0.0);

  return token->value.real;
}

/** names *****/

pdf_token_t *
pdf_token_name_new (const pdf_char_t  *value,
                    pdf_size_t         size,
                    pdf_error_t      **error)
{
  pdf_size_t i;

  PDF_ASSERT_POINTER_RETURN_VAL (value, NULL);

  for (i = 0; i < size; ++i)
    {
      /* names can't include NUL bytes */
      if (value[i] == '\0')
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_TOKENISER,
                         PDF_EBADDATA,
                         "cannot create name token: "
                         "names cannot include NUL bytes");
          return NULL;
        }
    }

  return token_buffer_new (PDF_TOKEN_NAME,
                           value,
                           size,
                           PDF_TRUE,
                           error);
}

pdf_size_t
pdf_token_get_name_size (const pdf_token_t *token)
{
  PDF_ASSERT_POINTER_RETURN_VAL (token, 0);
  PDF_ASSERT_RETURN_VAL (token->type == PDF_TOKEN_NAME, 0);

  return token->value.buffer.size;
}

const pdf_char_t *
pdf_token_get_name_data (const pdf_token_t *token)
{
  PDF_ASSERT_POINTER_RETURN_VAL (token, NULL);
  PDF_ASSERT_RETURN_VAL (token->type == PDF_TOKEN_NAME, NULL);

  return token->value.buffer.data;
}

/** strings *****/

pdf_token_t *
pdf_token_string_new (const pdf_char_t  *value,
                      pdf_size_t         size,
                      pdf_error_t      **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (value, NULL);

  return token_buffer_new (PDF_TOKEN_STRING,
                           value,
                           size,
                           PDF_FALSE,
                           error);
}

pdf_size_t
pdf_token_get_string_size (const pdf_token_t *token)
{
  PDF_ASSERT_POINTER_RETURN_VAL (token, 0);
  PDF_ASSERT_RETURN_VAL (token->type == PDF_TOKEN_STRING, 0);

  return token->value.buffer.size;
}

const pdf_char_t *
pdf_token_get_string_data (const pdf_token_t *token)
{
  PDF_ASSERT_POINTER_RETURN_VAL (token, NULL);
  PDF_ASSERT_RETURN_VAL (token->type == PDF_TOKEN_STRING, NULL);

  return token->value.buffer.data;
}

/** comments *****/

pdf_token_t *
pdf_token_comment_new (const pdf_char_t  *value,
                       pdf_size_t         size,
                       pdf_error_t      **error)
{
  pdf_size_t i;

  PDF_ASSERT_POINTER_RETURN_VAL (value, NULL);

  for (i = 0; i < size; ++i)
    {
      /* comments can't span multiple lines */
      if (pdf_is_eol_char (value[i]))
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_TOKENISER,
                         PDF_EBADDATA,
                         "cannot create comment token: "
                         "comments cannot span multiple lines");
          return NULL;
        }
    }

  return token_buffer_new (PDF_TOKEN_COMMENT,
                           value,
                           size,
                           PDF_FALSE,
                           error);
}

pdf_size_t
pdf_token_get_comment_size (const pdf_token_t *token)
{
  PDF_ASSERT_POINTER_RETURN_VAL (token, 0);
  PDF_ASSERT_RETURN_VAL (token->type == PDF_TOKEN_COMMENT, 0);

  return token->value.buffer.size;
}

const pdf_char_t *
pdf_token_get_comment_data (const pdf_token_t *token)
{
  PDF_ASSERT_POINTER_RETURN_VAL (token, NULL);
  PDF_ASSERT_RETURN_VAL (token->type == PDF_TOKEN_COMMENT, NULL);

  return token->value.buffer.data;
}

/** keywords *****/

pdf_token_t *
pdf_token_keyword_new (const pdf_char_t  *value,
                       pdf_size_t         size,
                       pdf_error_t      **error)
{
  pdf_size_t i;

  PDF_ASSERT_POINTER_RETURN_VAL (value, NULL);

  for (i = 0; i < size; ++i)
    {
      /* keywords can only include regular characters */
      if (!pdf_is_regular_char (value[i]))
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_TOKENISER,
                         PDF_EBADDATA,
                         "cannot create keyword token: "
                         "keywords can only include regular characters");
          return NULL;
        }
    }

  return token_buffer_new (PDF_TOKEN_KEYWORD,
                           value,
                           size,
                           PDF_TRUE,
                           error);
}

pdf_size_t
pdf_token_get_keyword_size (const pdf_token_t *token)
{
  PDF_ASSERT_POINTER_RETURN_VAL (token, 0);
  PDF_ASSERT_RETURN_VAL (token->type == PDF_TOKEN_KEYWORD, 0);

  return token->value.buffer.size;
}

const pdf_char_t *
pdf_token_get_keyword_data (const pdf_token_t *token)
{
  PDF_ASSERT_POINTER_RETURN_VAL (token, NULL);
  PDF_ASSERT_RETURN_VAL (token->type == PDF_TOKEN_KEYWORD, NULL);

  return token->value.buffer.data;
}

/* End of pdf-token.c */

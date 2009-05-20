/* -*- mode: C -*- Time-stamp: "08/09/08 22:44:16 jemarch"
 *
 *       File:         pdf-token-obj.c
 *       Date:         Sat Jul  7 03:04:30 2007
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

#include <string.h>
#include <assert.h>
#include <math.h>

#include "config.h"
#include "pdf-token-obj.h"
#include "pdf-alloc.h"

/* Private functions */

static INLINE pdf_status_t
pdf_token_new (enum pdf_token_type_e type, pdf_token_t *token)
{
  pdf_token_t new;
  assert (token);

  new = (pdf_token_t) pdf_alloc (sizeof (struct pdf_token_s));
  if (!new)
    return PDF_ENOMEM;

  new->type = type;
  *token = new;
  return PDF_OK;
}

pdf_status_t
pdf_token_destroy (pdf_token_t token)
{
  assert (token);
  switch (token->type)
    {
    case PDF_TOKEN_STRING:   /* fall through */
    case PDF_TOKEN_NAME:     /* fall through */
    case PDF_TOKEN_KEYWORD:  /* fall through */
    case PDF_TOKEN_COMMENT:
      {
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
  return PDF_OK;
}

static pdf_status_t
pdf_token_buffer_new (enum pdf_token_type_e type,
                      const pdf_char_t *value,
                      pdf_size_t size,
                      pdf_bool_t nullterm,
                      pdf_token_t *token)
{
  pdf_token_t new_obj = NULL;
  pdf_status_t rv = pdf_token_new (type, &new_obj);
  if (rv != PDF_OK)
    goto fail;

  rv = PDF_ENOMEM;
  new_obj->value.buffer.data = pdf_alloc (nullterm ? size + 1 : size);
  if (!new_obj->value.buffer.data)
    goto fail;

  new_obj->value.buffer.size = size;
  memcpy (new_obj->value.buffer.data, value, size);
  if (nullterm)
    new_obj->value.buffer.data[size] = 0;

  *token = new_obj;
  return PDF_OK;

fail:
  if (new_obj)
    pdf_dealloc (new_obj);
  return rv;
}


/* General functions */

enum pdf_token_type_e
pdf_token_get_type (const pdf_token_t token)
{
  assert (token);
  return token->type;
}

pdf_bool_t
pdf_token_equal_p (const pdf_token_t token1, const pdf_token_t token2)
{
  assert (token1 && token2);
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
      return token1->value.integer == token2->value.integer;

    case PDF_TOKEN_REAL:
      return token1->value.real == token2->value.real;

    case PDF_TOKEN_COMMENT:
      if (token1->value.comment.continued != token2->value.comment.continued)
        return PDF_FALSE;

      /* fall through */
    case PDF_TOKEN_STRING:   /* fall through */
    case PDF_TOKEN_NAME:     /* fall through */
    case PDF_TOKEN_KEYWORD:
      {
        struct pdf_token_buffer_s *buf1 = &token1->value.buffer;
        struct pdf_token_buffer_s *buf2 = &token2->value.buffer;
        return (buf1->size == buf2->size
                 && ( buf1->data == buf2->data
                      || !memcmp (buf1->data, buf2->data, buf1->size) ));
      }

    default:
      assert (0);  /* shouldn't happen */
      return 0;
    }
}

pdf_status_t
pdf_token_dup (const pdf_token_t token, pdf_token_t *new)
{
  assert (token);
  switch (token->type)
    {
    case PDF_TOKEN_DICT_START:   /* fall through */
    case PDF_TOKEN_DICT_END:     /* fall through */
    case PDF_TOKEN_ARRAY_START:  /* fall through */
    case PDF_TOKEN_ARRAY_END:    /* fall through */
    case PDF_TOKEN_PROC_START:   /* fall through */
    case PDF_TOKEN_PROC_END:
      return pdf_token_valueless_new (token->type, new);

    case PDF_TOKEN_INTEGER:
      return pdf_token_integer_new (token->value.integer, new);

    case PDF_TOKEN_REAL:
      return pdf_token_real_new (token->value.real, new);

    case PDF_TOKEN_STRING:
      return pdf_token_string_new (token->value.buffer.data,
                                   token->value.buffer.size,
                                   new);
    case PDF_TOKEN_NAME:
      return pdf_token_name_new (token->value.buffer.data,
                                 token->value.buffer.size,
                                 new);
    case PDF_TOKEN_KEYWORD:
      return pdf_token_keyword_new (token->value.buffer.data,
                                    token->value.buffer.size,
                                    new);
    case PDF_TOKEN_COMMENT:
      return pdf_token_comment_new (token->value.comment.data,
                                    token->value.comment.size,
                                    token->value.comment.continued,
                                    new);
    default:
      /* Should not be reached: make the compiler happy */
      return PDF_EBADDATA;
    }
}

pdf_status_t
pdf_token_valueless_new (enum pdf_token_type_e type,
                         pdf_token_t *token)
{
  switch (type)
  {
    case PDF_TOKEN_DICT_START:   /* fall through */
    case PDF_TOKEN_DICT_END:     /* fall through */
    case PDF_TOKEN_ARRAY_START:  /* fall through */
    case PDF_TOKEN_ARRAY_END:    /* fall through */
    case PDF_TOKEN_PROC_START:   /* fall through */
    case PDF_TOKEN_PROC_END:
      return pdf_token_new (type, token);
    default:
      return PDF_EBADDATA;
  }
}


/** integers *****/

pdf_status_t
pdf_token_integer_new (pdf_i32_t value, pdf_token_t *token)
{
  pdf_status_t rv = pdf_token_new (PDF_TOKEN_INTEGER, token);
  if (rv == PDF_OK)
    (*token)->value.integer = value;

  return rv;
}

pdf_i32_t
pdf_token_get_integer_value (const pdf_token_t token)
{
  assert (token && token->type == PDF_TOKEN_INTEGER);
  return token->value.integer;
}


/** reals *****/

pdf_status_t
pdf_token_real_new (pdf_real_t value, pdf_token_t *token)
{
  pdf_status_t rv;

  if (isnan(value) || isinf(value))
    return PDF_EBADDATA;

  rv = pdf_token_new (PDF_TOKEN_REAL, token);
  if (rv == PDF_OK)
    (*token)->value.real = value;

  return rv;
}

pdf_real_t
pdf_token_get_real_value (const pdf_token_t token)
{
  assert (token && token->type == PDF_TOKEN_REAL);
  return token->value.real;
}


/** names *****/

pdf_status_t
pdf_token_name_new (const pdf_char_t *value,
                  pdf_size_t size,
                  pdf_token_t *token)
{
  return pdf_token_buffer_new (PDF_TOKEN_NAME, value, size, 1, token);
}

pdf_size_t
pdf_token_get_name_size (const pdf_token_t name)
{
  assert (name && name->type == PDF_TOKEN_NAME);
  return name->value.buffer.size;
}

const pdf_char_t *
pdf_token_get_name_data (const pdf_token_t name)
{
  assert (name && name->type == PDF_TOKEN_NAME);
  return name->value.buffer.data;
}


/** strings *****/

pdf_status_t
pdf_token_string_new (const pdf_char_t *value,
                    pdf_size_t size,
                    pdf_token_t *token)
{
  return pdf_token_buffer_new (PDF_TOKEN_STRING, value, size, 0, token);
}

pdf_size_t
pdf_token_get_string_size (const pdf_token_t token)
{
  assert (token && token->type == PDF_TOKEN_STRING);
  return token->value.buffer.size;
}

const pdf_char_t *
pdf_token_get_string_data (const pdf_token_t token)
{
  assert (token && token->type == PDF_TOKEN_STRING);
  return token->value.buffer.data;
}


/** comments *****/

pdf_status_t
pdf_token_comment_new (const pdf_char_t *value,
                       pdf_size_t size,
                       pdf_bool_t continued,
                       pdf_token_t *token)
{
  pdf_status_t rv;
  rv = pdf_token_buffer_new (PDF_TOKEN_COMMENT, value, size, 0, token);
  if (rv == PDF_OK)
    (*token)->value.comment.continued = !!continued;

  return rv;
}

pdf_size_t
pdf_token_get_comment_size (const pdf_token_t comment)
{
  assert (comment && comment->type == PDF_TOKEN_COMMENT);
  return comment->value.buffer.size;
}

const pdf_char_t *
pdf_token_get_comment_data (const pdf_token_t comment)
{
  assert (comment && comment->type == PDF_TOKEN_COMMENT);
  return comment->value.buffer.data;
}

pdf_bool_t
pdf_token_get_comment_continued (const pdf_token_t comment)
{
  assert (comment && comment->type == PDF_TOKEN_COMMENT);
  return comment->value.comment.continued;
}


/** keywords *****/

pdf_status_t
pdf_token_keyword_new (const pdf_char_t *value,
                       pdf_size_t size,
                       pdf_token_t *token)
{
  return pdf_token_buffer_new (PDF_TOKEN_KEYWORD, value, size, 1, token);
}

pdf_size_t
pdf_token_get_keyword_size (const pdf_token_t keyword)
{
  assert (keyword && keyword->type == PDF_TOKEN_KEYWORD);
  return keyword->value.buffer.size;
}

const pdf_char_t *
pdf_token_get_keyword_data (const pdf_token_t keyword)
{
  assert (keyword && keyword->type == PDF_TOKEN_KEYWORD);
  return keyword->value.buffer.data;
}


/* End of pdf-token-obj.c */

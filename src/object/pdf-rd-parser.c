/* -*- mode: C -*- Time-stamp: "2009-01-14 21:21:44 mgold"
 *
 *       File:         pdf-rd-tokeniser.c
 *       Date:         Wed Jan 14 21:21:40 2009
 *
 *       GNU PDF Library - Stream parser
 *
 */

/* Copyright (C) 2009 Michael Gold */

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

#include <assert.h>
#include "pdf-rd-parser.h"
#include "pdf-obj.h"

static const pdf_char_t PDF_KW_null[] = {0x6e, 0x75, 0x6c, 0x6c};
static const pdf_char_t PDF_KW_true[] = {0x74, 0x72, 0x75, 0x65};
static const pdf_char_t PDF_KW_false[] = {0x66, 0x61, 0x6c, 0x73, 0x65};

static pdf_status_t open_stack (pdf_parser_t parser, pdf_bool_t is_dict);
static pdf_status_t close_stack (pdf_parser_t parser, pdf_bool_t is_dict);

pdf_status_t
pdf_parser_new (pdf_stm_t stm, pdf_parser_t *parser)
{
  pdf_status_t rv;
  pdf_parser_t new_parser;

  rv = PDF_ENOMEM;
  new_parser = pdf_alloc (sizeof (*new_parser));
  if (!new_parser)
    goto fail;

  new_parser->tokr = NULL;
  rv = pdf_tokeniser_new (stm, &new_parser->tokr);
  if (rv != PDF_OK)
    goto fail;

  new_parser->token = NULL;

  /* open the top-level stack (parent==NULL) */
  new_parser->stack = NULL;
  rv = open_stack (new_parser, 0);
  if (rv != PDF_OK)
    goto fail;

  *parser = new_parser;
  return PDF_OK;

fail:
  if (new_parser)
    {
      if (new_parser->tokr)
        pdf_tokeniser_destroy (new_parser->tokr);
      pdf_dealloc (new_parser);
    }
  return rv;
}

pdf_status_t
pdf_parser_destroy (pdf_parser_t parser)
{
  if (!parser) return PDF_EBADDATA;

  assert (parser->tokr);
  if (parser->tokr)
    pdf_tokeniser_destroy (parser->tokr);
  if (parser->token)
    pdf_obj_destroy (parser->token);

  while (parser->stack)
    {
      pdf_parser_stack_t parent = parser->stack->parent;
      pdf_obj_destroy (parser->stack->items);
      pdf_dealloc (parser->stack);
      parser->stack = parent;
    }

  pdf_dealloc (parser);
  return PDF_OK;
}

static pdf_status_t
handle_keyword (pdf_parser_t parser)
{
  pdf_status_t rv;
  pdf_obj_t new_token = NULL;
  const pdf_char_t *keyword = pdf_tok_keyword_data (parser->token);

  switch (pdf_tok_keyword_size (parser->token))
    {
    case 1:
      if (keyword[0] == 82)  /* 'R' (indirect object) */
        {
          pdf_obj_t on, gn;
          pdf_obj_t items = parser->stack->items;
          pdf_size_t size = pdf_obj_array_size (items);
          if (size < 2)
            return PDF_EBADFILE;

          rv = pdf_obj_array_get (items, size-2, &on);
          if (rv != PDF_OK)
            return rv;

          rv = pdf_obj_array_get (items, size-1, &gn);
          if (rv != PDF_OK)
            return rv;

          if (pdf_obj_type (on) != PDF_INT_OBJ
               || pdf_obj_type (gn) != PDF_INT_OBJ)
            return PDF_EBADFILE;

          rv = pdf_obj_indirect_new (pdf_obj_int_value (on),
                                     pdf_obj_int_value (gn),
                                     &new_token);
          if (rv != PDF_OK)
            return rv;

          pdf_obj_array_remove (items, size-1, NULL);
          pdf_obj_array_remove (items, size-2, NULL);
        }
      break;
    case 4:
      if (memcmp (keyword, PDF_KW_null, 4) == 0)
        {
          rv = pdf_obj_null_new (&new_token);
          if (rv != PDF_OK)
            return rv;
        }
      else if (memcmp (keyword, PDF_KW_true, 4) == 0)
        {
          rv = pdf_obj_boolean_new (PDF_TRUE, &new_token);
          if (rv != PDF_OK)
            return rv;
        }
      break;
    case 5:
      if (memcmp (keyword, PDF_KW_false, 5) == 0)
        {
          rv = pdf_obj_boolean_new (PDF_FALSE, &new_token);
          if (rv != PDF_OK)
            return rv;
        }
      break;
    }

  if (new_token)
    {
      pdf_obj_destroy (parser->token);
      parser->token = new_token;
    }
  else if (parser->stack->parent)
    {
      /* The token looks like a command, but isn't at the top level. */
      return PDF_EBADFILE;
    }

  return PDF_OK;
}

static pdf_status_t
handle_token (pdf_parser_t parser)
{
  pdf_status_t rv;
  assert (parser->token);

  switch (pdf_obj_type (parser->token))
    {
    case PDF_ARRAY_START_TOK:
      rv = open_stack (parser, 0);
      if (rv != PDF_OK)
        return rv;

      pdf_obj_destroy (parser->token);
      parser->token = NULL;
      return PDF_OK;

    case PDF_DICT_START_TOK:
      rv = open_stack (parser, 1);
      if (rv != PDF_OK)
        return rv;

      pdf_obj_destroy (parser->token);
      parser->token = NULL;
      return PDF_OK;

    case PDF_ARRAY_END_TOK:
      rv = close_stack (parser, 0);
      if (rv != PDF_OK)
        return rv;

      assert (pdf_obj_type (parser->token) != PDF_ARRAY_END_TOK);
      break;

    case PDF_DICT_END_TOK:
      rv = close_stack (parser, 1);
      if (rv != PDF_OK)
        return rv;

      assert (pdf_obj_type (parser->token) != PDF_DICT_END_TOK);
      break;

    case PDF_PROC_START_TOK:  /* fall through */
    case PDF_PROC_END_TOK:
      return PDF_EBADFILE;

    case PDF_NULL_OBJ:      /* fall through */
    case PDF_BOOLEAN_OBJ:   /* fall through */
    case PDF_INDIRECT_OBJ:  /* fall through */
      /* Note: the above types are never produced by the tokeniser,
       *       but may appear in parser->token if we didn't finish
       *       processing a keyword last time. */
    case PDF_INT_OBJ:       /* fall through */
    case PDF_REAL_OBJ:      /* fall through */
    case PDF_STRING_OBJ:    /* fall through */
    case PDF_NAME_OBJ:      /* fall through */
    case PDF_COMMENT_TOK:
      break;

    case PDF_KEYWORD_TOK:
      rv = handle_keyword (parser);
      if (rv != PDF_OK)
        return rv;
      else if (!parser->token)
        return PDF_OK;

      break;

    default:
      assert (0);
      return PDF_ERROR;
    }

  /* store the token */  /*TODO: limit the length and depth of the stack */
  assert (parser->token);
  rv = pdf_obj_array_append (parser->stack->items, parser->token);
  if (rv != PDF_OK)
    return rv;

  parser->token = NULL;
  return PDF_OK;
}

static pdf_status_t
open_stack (pdf_parser_t parser, pdf_bool_t is_dict)
{
  pdf_status_t rv;
  pdf_parser_stack_t new_stack;

  new_stack = pdf_alloc (sizeof (*new_stack));
  if (!new_stack)
    return PDF_ENOMEM;

  rv = pdf_obj_array_new (&new_stack->items);
  if (rv != PDF_OK)
    goto fail;

  new_stack->is_dict = is_dict;
  new_stack->parent = parser->stack;
  parser->stack = new_stack;
  return PDF_OK;

fail:
  if (new_stack)
    pdf_dealloc (new_stack);
  return rv;
}

static pdf_status_t
close_stack (pdf_parser_t parser, pdf_bool_t is_dict)
{
  pdf_status_t rv;
  pdf_obj_t new_dict = NULL;
  pdf_parser_stack_t stack = parser->stack;

  assert (parser->token);
  if (!stack || !stack->parent || (stack->is_dict != is_dict))
    return PDF_EBADFILE;

  if (is_dict)
    {
      size_t i, size;
      pdf_obj_t items = stack->items;
      size = pdf_obj_array_size (items);

      rv = pdf_obj_dict_new (&new_dict);
      if (rv != PDF_OK)
        goto fail;

      /* turn the object stack into a dictionary */
      for (i = 0; i < size; i += 2)
        {
          pdf_obj_t key, value;

          rv = PDF_EBADFILE;
          if (i+1 == size)  /* key with no value */
            goto fail;

          rv = pdf_obj_array_get (items, i, &key);
          if (rv != PDF_OK)
            goto fail;

          rv = PDF_EBADFILE;
          if (pdf_obj_type (key) != PDF_NAME_OBJ)
            goto fail;

          rv = pdf_obj_array_get (items, i+1, &value);
          if (rv != PDF_OK)
            goto fail;

          rv = pdf_obj_dict_add (new_dict, key, value);
          if (rv != PDF_OK)
            goto fail;
        }

      /* now delete the original stack without deleting its objects,
       * which were transferred to the dictionary */
      pdf_obj_array_clear_nodestroy (stack->items);
      pdf_obj_destroy (stack->items);

      /* replace the token with a PDF_DICT_OBJ */
      pdf_obj_destroy (parser->token);
      parser->token = new_dict;
    }
  else
    {
      /* replace the token with a PDF_LIST_OBJ */
      pdf_obj_destroy (parser->token);
      parser->token = stack->items;
    }

  /* close this stack level */
  parser->stack = stack->parent;
  pdf_dealloc (stack);
  return PDF_OK;

fail:
  if (new_dict)
    {
      pdf_obj_dict_clear_nodestroy (new_dict);
      pdf_obj_destroy (new_dict);
    }
  return rv;
}

pdf_status_t
pdf_parser_read_to_command (pdf_parser_t parser, pdf_obj_t *stack_ref)
{
  pdf_status_t rv;
  assert (parser->stack);
  for (;;)
    {
      if (!parser->token)
        {
          rv = pdf_tokeniser_read (parser->tokr, &parser->token);
          if (rv == PDF_EEOF)
            break;
          else if (rv != PDF_OK)
            return rv;
        }

      assert (parser->token);
      rv = handle_token (parser);
      if (rv != PDF_OK)
        return rv;

      assert (parser->stack);
      if (!parser->stack->parent)  /* at the top level */
        {
          pdf_obj_t last_obj;
          pdf_size_t size = pdf_obj_array_size (parser->stack->items);
          if (size)
            {
              rv = pdf_obj_array_get (parser->stack->items,
                                      size - 1,
                                      &last_obj);
              assert (rv == PDF_OK);
              if (rv != PDF_OK)
                return PDF_ERROR;  /* shouldn't happen */

              assert (last_obj);
              if (pdf_obj_type (last_obj) == PDF_KEYWORD_TOK)
                {
                  /* this token is a command */
                  *stack_ref = parser->stack->items;
                  return PDF_OK;
                }
            }
        }
    }

  /* end of file */
  if (parser->stack->parent)
    {
      /* got EOF before all arrays/dicts were closed */
      return PDF_EBADFILE;
    }

  return PDF_EEOF;
}

/* End of pdf-rd-parser.c */

/* -*- mode: C -*- Time-stamp: "2009-01-14 20:47:21 mgold"
 *
 *       File:         pdf-rd-tokeniser.h
 *       Date:         Wed Jan 14 20:47:23 2009
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

#ifndef PDF_RD_PARSER
#define PDF_RD_PARSER
#include "pdf-types.h"
#include "pdf-obj.h"
#include "pdf-rd-tokeniser.h"

/* BEGIN PUBLIC */
/* pdf-rd-parser.h */
typedef struct pdf_parser_s *pdf_parser_t;
pdf_status_t pdf_parser_new(pdf_stm_t stm, pdf_parser_t *parser);
pdf_status_t pdf_parser_destroy(pdf_parser_t parser);
pdf_status_t pdf_parser_read(pdf_parser_t parser, pdf_obj_t *obj);
/* END PUBLIC */

typedef struct pdf_parser_stack_s *pdf_parser_stack_t;
struct pdf_parser_stack_s
{
  pdf_parser_stack_t parent;
  pdf_bool_t is_dict;
  pdf_obj_t items;
};

struct pdf_parser_s
{
  pdf_parser_stack_t stack;
  pdf_tokeniser_t tokr;
  pdf_obj_t token;  /* the next token to process, or NULL */
  pdf_bool_t have_cmd;
};

#endif

/* End of pdf-rd-parser.h */

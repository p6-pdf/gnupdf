/* -*- mode: C -*-
 *
 *       File:         pdf-token-writer.h
 *       Date:         Wed Sep 23 04:30:26 2009
 *
 *       GNU PDF Library - Stream token writer
 *
 */

/* Copyright (C) 2009-2011 Free Software Foundation, Inc. */

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

#ifndef PDF_TOKEN_WRITER_H
#define PDF_TOKEN_WRITER_H

#include <config.h>

#include <pdf-types.h>
#include <pdf-stm.h>
#include <pdf-token.h>

/* BEGIN PUBLIC */

/* --------------------- Token Writer ------------------------- */

/* opaque type */
typedef struct pdf_token_writer_s pdf_token_writer_t;

pdf_token_writer_t *pdf_token_writer_new (pdf_stm_t    *stm,
                                          pdf_error_t **error);
void pdf_token_writer_destroy (pdf_token_writer_t *writer);
pdf_bool_t pdf_token_writer_reset (pdf_token_writer_t  *writer,
                                   pdf_error_t        **error);
pdf_bool_t pdf_token_writer_write (pdf_token_writer_t  *writer,
                                   pdf_u32_t            flags,
                                   const pdf_token_t   *token,
                                   pdf_error_t        **error);

/* END PUBLIC */

#endif

/* End of pdf-token-writer.h */

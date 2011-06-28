/* -*- mode: C -*-
 *
 *       File:         pdf-token-reader.h
 *       Date:         Mon Dec 29 00:45:09 2008
 *
 *       GNU PDF Library - Stream tokeniser
 *
 */

/* Copyright (C) 2008-2011 Free Software Foundation, Inc. */

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

#ifndef PDF_TOKEN_READER_H
#define PDF_TOKEN_READER_H

#include <config.h>

#include <pdf-types.h>
#include <pdf-stm.h>
#include <pdf-token.h>

/* BEGIN PUBLIC */

/* --------------------- Token Reader ------------------------- */

/* opaque type */
typedef struct pdf_token_reader_s pdf_token_reader_t;

pdf_token_reader_t *pdf_token_reader_new (pdf_stm_t    *stm,
                                          pdf_error_t **error);
void pdf_token_reader_destroy (pdf_token_reader_t *reader);
pdf_bool_t pdf_token_reader_reset (pdf_token_reader_t  *reader,
                                   pdf_error_t        **error);
pdf_size_t pdf_token_reader_begin_pos (pdf_token_reader_t *reader);
pdf_token_t *pdf_token_read (pdf_token_reader_t  *reader,
                             pdf_u32_t            flags,
                             pdf_error_t        **error);

/* END PUBLIC */

#endif

/* End of pdf-token-reader.h */

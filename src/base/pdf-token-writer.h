/* -*- mode: C -*- Time-stamp: "2009-10-25 18:06:21 mgold"
 *
 *       File:         pdf-token-writer.h
 *       Date:         Wed Sep 23 04:30:26 2009
 *
 *       GNU PDF Library - Stream token writer
 *
 */

/* Copyright (C) 2009 Free Software Foundation, Inc. */

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
#include <pdf-token-reader.h>

/* BEGIN PUBLIC */
/* pdf-token-writer.h */

struct pdf_token_writer_s;  /* opaque type */
typedef struct pdf_token_writer_s *pdf_token_writer_t;

pdf_status_t pdf_token_writer_new (pdf_stm_t stm, pdf_token_writer_t *writer);
pdf_status_t pdf_token_writer_destroy (pdf_token_writer_t writer);
pdf_status_t pdf_token_writer_reset (pdf_token_writer_t writer);
pdf_status_t pdf_token_write (pdf_token_writer_t writer, pdf_u32_t flags,
                              pdf_token_t token);

/* END PUBLIC */

/* Internal state */
struct pdf_token_writer_s {
  pdf_stm_t stream;  /* stream to read bytes from */
  char *decimal_point;

  pdf_bool_t in_keyword;
  pdf_size_t line_length, buffered_line_length, max_line_length;

  int stage;
  pdf_size_t pos;
  pdf_size_t paren_quoting_start, paren_quoting_end;
  pdf_bool_t utf8;
  pdf_buffer_t buffer;
};

/* PDF32000 7.5.1: "lines that are not part of stream object data
 * are limited to no more than 255 characters"... */
#define PDF_TOKW_MAX_LINE_LENGTH 255

/* The buffer size is mostly arbitrary, except that reserve_buffer_space can't
 * be called with a length larger that the buffer size.  We never reserve more
 * than 4 bytes at a time, so any number >= 4 is fine. */
#define PDF_TOKW_BUFFER_SIZE 32768

#endif

/* End of pdf-token-writer.h */

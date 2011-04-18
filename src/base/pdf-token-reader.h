/* -*- mode: C -*-
 *
 *       File:         pdf-token-reader.h
 *       Date:         Mon Dec 29 00:45:09 2008
 *
 *       GNU PDF Library - Stream tokeniser
 *
 */

/* Copyright (C) 2008, 2009 Free Software Foundation, Inc. */

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
/* pdf-token-reader.h */

struct pdf_token_reader_s;  /* opaque type */
typedef struct pdf_token_reader_s *pdf_token_reader_t;

pdf_status_t pdf_token_reader_new (pdf_stm_t stm, pdf_token_reader_t *reader);
pdf_status_t pdf_token_reader_destroy (pdf_token_reader_t reader);
pdf_status_t pdf_token_reader_reset (pdf_token_reader_t reader);
pdf_size_t pdf_token_reader_begin_pos (pdf_token_reader_t reader);
pdf_status_t pdf_token_read (pdf_token_reader_t reader, pdf_u32_t flags,
                             pdf_token_t *token);

enum pdf_token_rw_flags_e {
  /* these flags are used for token_read and token_write */
  PDF_TOKEN_NO_NAME_ESCAPES = 1,    /* read/write */
  PDF_TOKEN_RET_COMMENTS = 2,       /* read */
  PDF_TOKEN_END_AT_STREAM = 4,      /* read */
  PDF_TOKEN_HEX_STRINGS = 8,        /* write */
  PDF_TOKEN_READABLE_STRINGS = 16,  /* write */
};

/* END PUBLIC */

enum pdf_token_reader_state_e
{
  /* PDF_TOKR_ is used as an internal prefix for the token reader. */
  PDF_TOKR_STATE_NONE = 0,
  PDF_TOKR_STATE_COMMENT,
  PDF_TOKR_STATE_KEYWORD,
  PDF_TOKR_STATE_NAME,
  PDF_TOKR_STATE_STRING,
  PDF_TOKR_STATE_HEXSTRING,
  PDF_TOKR_STATE_DICTEND,
  PDF_TOKR_STATE_PENDING,
  PDF_TOKR_STATE_EOF
};

/* Token reader states (from pdf_token_reader_state_e):
 * NONE - Initial state; not reading a token.
 * COMMENT - Reading a comment.  buffer collects the comment bytes.
 *   Substates:
 *     0 - normal state
 *     1 - don't produce a token
 * KEYWORD - Reading some regular characters into buffer; this could result
 *           in a symbol like "null", or a number.
 * NAME - Reading a name (which starts with '/').
 *   Substates:
 *     0 - normal state
 *     1 - just read a '#' (escape prefix)
 *     2 - read the first hex digit after '#'; the value is in charparam
 *   buffer collects the name, excluding the initial '/'.
 * STRING - Reading a literal string (enclosed in '(', ')').
 *   Substates:
 *     0 - normal state
 *     1 - ignore the next byte if its value is 10 (ASCII LF;
 *         this is used to treat CRLF as a single line ending)
 *     2 - just saw a backslash (escape prefix)
 *     3 - read 1 octal digit; the value is in charparam
 *     4 - read 2 octal digits; the value is in charparam
 *   intparam is the bracket nesting level; ')' at level 0 ends the string.
 *   buffer collects the string.
 * HEXSTRING - Reading a hex string.
 *   Substates:
 *     0 - initial state: we just saw the opening '<', and if the next byte is
 *         also '<' this is the start of a dictionary rather than a string
 *     1 - normal state (the next hex digit will be the first in a pair)
 *     2 - read the first hex digit; its value is in charparam
 *     3 - end state; saw the closing '>'
 *   buffer collects the string.
 * DICTEND - Just got a '>'; expecting another.
 *   Substates:
 *     0 - starting state
 *     1 - saw the second '>'
 * PENDING - Need to emit a token (determined by charparam) ASAP.
 * EOF - Can't continue tokenising (reached EOF, or beginning of stream)
 */

/* Internal state */
struct pdf_token_reader_s {
  pdf_stm_t stream;  /* stream to read bytes from */
  char *decimal_point;

  pdf_size_t state_pos;
  pdf_size_t beg_pos; /* Beginning position of the last read token in
                         the input stream */

  /* variables used by the state machine (described above) */
  int state;
  int substate;
  pdf_char_t charparam;
  int intparam;
  pdf_buffer_t *buffer;
  /***/
  pdf_size_t buffer_size_min;
};

#endif

/* End of pdf-token-reader.h */

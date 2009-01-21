/* -*- mode: C -*- Time-stamp: "2009-01-05 08:53:02 mgold"
 *
 *       File:         pdf-rd-tokeniser.h
 *       Date:         Mon Dec 29 00:45:09 2008
 *
 *       GNU PDF Library - Stream tokeniser
 *
 */

/* Copyright (C) 2008 Michael Gold */

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

#ifndef PDF_RD_TOKENISER
#define PDF_RD_TOKENISER
#include "pdf-types.h"
#include "pdf-stm.h"
#include "pdf-obj.h"

/* BEGIN PUBLIC */
/* pdf-rd-tokeniser.h */

struct pdf_tokeniser_s;  /* opaque type */
typedef struct pdf_tokeniser_s *pdf_tokeniser_t;

pdf_status_t pdf_tokeniser_new(pdf_stm_t stm, pdf_tokeniser_t *tokr);
pdf_status_t pdf_tokeniser_destroy(pdf_tokeniser_t tokr);
pdf_status_t pdf_tokeniser_read(pdf_tokeniser_t tokr, pdf_obj_t *token);
pdf_status_t pdf_tokeniser_end_at_stream(pdf_tokeniser_t tokr);
pdf_status_t pdf_tokeniser_reset_state(pdf_tokeniser_t tokr);
/* END PUBLIC */

enum pdf_tokeniser_state_e {
  PDF_TOKENISER_STATE_NONE = 0,
  PDF_TOKENISER_STATE_COMMENT,
  PDF_TOKENISER_STATE_KEYWORD,
  PDF_TOKENISER_STATE_NAME,
  PDF_TOKENISER_STATE_STRING,
  PDF_TOKENISER_STATE_HEXSTRING,
  PDF_TOKENISER_STATE_DICTEND,
  PDF_TOKENISER_STATE_STREAMSTART,
  PDF_TOKENISER_STATE_PENDING,
  PDF_TOKENISER_STATE_EOF
};

/* Tokeniser states (from pdf_tokeniser_state_e):
 * NONE - Initial state; not reading a token.
 * COMMENT - Reading a comment.  buffer collects the comment bytes, excluding
 *   intparam is 1 if this token is continued from a previous token,
 *   and 0 otherwise.
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

enum pdf_tokeniser_flag_e {
  PDF_TOKENISER_FLAG_RET_COMMENTS = 1,  /* return comments as tokens */
  PDF_TOKENISER_FLAG_PDF11 = 2,         /* disallow '#' escapes in names */
};

/* Internal state */
struct pdf_tokeniser_s {
  int flags;  /* miscellaneous settings (from pdf_tokeniser_flag_e) */
  pdf_stm_t stream;  /* stream to read bytes from */
  char *decimal_point;

  /* variables used by the state machine (described above) */
  int state;
  int substate;
  pdf_char_t charparam;
  int intparam;
  pdf_buffer_t buffer;
  pdf_bool_t findstream;
  /***/
};

#endif

/* End of pdf-rd-tokeniser.h */

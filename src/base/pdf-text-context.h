/* -*- mode: C -*- Time-stamp: "08/09/09 00:54:52 jemarch"
 *
 *       File:         pdf-text-context.h
 *       Date:         Fri Jan 11 21:09:23 2008
 *
 *       GNU PDF Library - Encoded Text Context
 *
 */

/* Copyright (C) 2008 Free Software Foundation, Inc. */

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

#ifndef _PDF_TEXT_CONTEXT_H
#define _PDF_TEXT_CONTEXT_H

#include <pdf-text.h>

/* Enumeration defining the types of system endianness */ 
enum pdf_endianness_e {
  PDF_TEXT_BIG_ENDIAN      =  0,
  PDF_TEXT_LITTLE_ENDIAN   =  1
};

/* Defines specifying some UTF-32 values for typical characters */
#define PDF_TEXT_DEF_CR    0x000D /* Carriage Return */
#define PDF_TEXT_DEF_LF    0x000A /* Line Feed */
#define PDF_TEXT_DEF_NEL   0x0085 /* New Line (In UTF-8: C2 85) */

/* Specific type for EOL sequences */
#define PDF_TEXT_EOLMAXL  3
struct pdf_text_eol_s {
  pdf_char_t sequence [PDF_TEXT_EOLMAXL]; /* In UTF-8 */
};
typedef struct pdf_text_eol_s * pdf_text_eol_t;

/* Platform-dependent EOL types. Note that Mac OS X is considered as Unix */
enum pdf_text_eol_types {
  PDF_TEXT_EOL_HOST     = -1,
  PDF_TEXT_EOL_WINDOWS  =  0,
  PDF_TEXT_EOL_UNIX     =  1,
  PDF_TEXT_EOL_EBCDIC   =  2,
  PDF_TEXT_EOL_MACOS    =  3,
  PDF_TEXT_EOLMAX
};


/* Initialize text context. Must be done only once at program startup!.
 *  Not thread-safe! */
pdf_status_t
pdf_text_context_init(void);

/* Is the text context initialized? */
pdf_bool_t pdf_text_context_initialized (void);

/* Returns the host endianess */
enum pdf_endianness_e
pdf_text_context_get_host_endianness(void);


/* Returns the host encoding */
pdf_text_host_encoding_t
pdf_text_context_get_host_encoding(void);


/* Returns the host language */
const pdf_char_t *
pdf_text_context_get_host_language(void);


/* Returns the host country */
const pdf_char_t *
pdf_text_context_get_host_country(void);


/* Returns the host EOL */
pdf_text_eol_t
pdf_text_context_get_host_eol(enum pdf_text_eol_types eol_type);


#endif /* _PDF_TEXT_CONTEXT_H */

/* End of pdf-text-context.h */

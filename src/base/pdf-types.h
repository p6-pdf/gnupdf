/* -*- mode: C -*- Time-stamp: "08/03/05 12:18:52 jemarch"
 *
 *       File:         pdf-types.h
 *       Date:         Sun Feb 10 21:30:00 2008
 *
 *       GNU PDF Library - Basic Types Module
 *
 */

/* Copyright (C) 2008 Free Software Foundation, Inc */

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

#ifndef PDF_TYPES_H
#define PDF_TYPES_H

#include <config.h>
/* BEGIN PUBLIC */
#include <sys/types.h> /* for off_t */
/* END PUBLIC */

#ifdef HAVE_INLINE
#define INLINE inline
#else
#define INLINE
#endif /* HAVE_INLINE */

/* BEGIN PUBLIC */

#define PDF_TRUE 1
#define PDF_FALSE 0

/* A variable of type `pdf_stm_pos_t' contain a byte-offset relative to
   the beginning of a stream object. 

   Please be careful manipulating `pdf_stm_pos_t' values. Its value is
   assured to be a signed scalable one, but its size may be wider than
   a long. */

#define NO_POS -1
typedef off_t pdf_stm_pos_t;

/* Memory indexes (size_t) */
typedef size_t pdf_size_t;

/* PDF manages bytes with values in the range 0-255 */

typedef unsigned char pdf_char_t;

/* PDF_EOF to store an EOF marker in integers */
#define PDF_EOF -1

/* END PUBLIC */

#endif /* !PDF_TYPES_H */

/* End of pdf-types.h */

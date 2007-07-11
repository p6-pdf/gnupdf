/* -*- mode: C -*- Time-stamp: "07/07/11 17:14:06 jemarch"
 *
 *       File:         pdf_base.h
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Fri Jul  6 20:22:05 2007
 *
 *       GNU PDF Library - Common resources for the library modules
 *
 */

/* Copyright (C) 2007 Jose E. Marchesi */

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

#ifndef _PDF_BASE_H
#define _PDF_BASE_H

#include <sys/types.h> /* for off_t */
#include <config.h>

#ifdef HAVE_INLINE
#define INLINE inline
#else
#define INLINE
#endif /* HAVE_INLINE */

#define PDF_TRUE 1
#define PDF_FALSE 0

#define PDF_OK PDF_TRUE
#define PDF_ERROR PDF_FALSE

/* A variable of type `pdf_stm_pos_t' contain a byte-offset relative to
   the beginning of a stream object. 

   Please be careful manipulating `pdf_stm_pos_t' values. Its value is
   assured to be a signed scalable one, but its size may be wider than
   a long. 

   Note that this is a ugly hack: we define `pdf_stm_pos_t' in
   `pdf_base.h' just because the stream backends need to use it. Its
   proper place would be `pdf_stm.h'. */

typedef off_t pdf_stm_pos_t;

/* PDF manages bytes with values in the range 0-255 */

typedef unsigned char pdf_char;

#endif /* pdf_base.h */

/* End of pdf_base.h */

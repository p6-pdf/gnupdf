/* -*- mode: C -*- Time-stamp: "07/07/06 17:19:17 jemarch"
 *
 *       File:         pdf.h
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Thu Jul  5 19:52:48 2007
 *
 *       GNU PDF Library - Main header file
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

#ifndef _PDF_H
#define _PDF_H

/* This document opaque type contain all the information extracted
   from the input stream used in the `pdf_doc_open' or
   `pdf_doc_open_socket' functions. */

typedef struct _pdf_doc_t
{
  pdf_io_t src; /* Source of the pdf data */
  pdf_io_t trg; /* Target of the pdf data */
  

} *pdf_doc_t;;
  

#endif /* pdf.h */

/* End of pdf.h */

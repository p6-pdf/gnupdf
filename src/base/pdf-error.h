/* -*- mode: C -*- Time-stamp: "2008-02-27 16:28:57 gerel"
 *
 *       File:         pdf-error.h
 *       Date:         Sun Feb  24 20:22:05 2008
 *
 *       GNU PDF Library - Header file for the Error module
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

#ifndef PDF_ERROR_H
#define PDF_ERROR_H

#include <stdio.h>

#ifdef HAVE_DEBUG_BASE
#define PDF_DEBUG_BASE(message, ...) \
  pdf_error (0, stderr, "***DEBUG BASE***:%s:%d: " message, \
  __FILE__, __LINE__, __VA_ARGS__)
#else
#define PDF_DEBUG_BASE(...)
#endif /* HAVE_DEBUG_BASE */

#ifdef HAVE_DEBUG_OBJECT
#define PDF_DEBUG_OBJECT(message, ...) \
  pdf_error (0, stderr, "***DEBUG OBJECT***:%s:%d: " message, \
  __FILE__, __LINE__, __VA_ARGS__)
#else
#define PDF_DEBUG_OBJECT(...)
#endif /* HAVE_DEBUG_OBJECT */

#ifdef HAVE_DEBUG_DOCUMENT
#define PDF_DEBUG_DOCUMENT(message, ...) \
  pdf_error (0, stderr, "***DEBUG DOCUMENT***:%s:%d: " message, \
  __FILE__, __LINE__, __VA_ARGS__)
#else
#define PDF_DEBUG_DOCUMENT(...)
#endif /* HAVE_DEBUG_DOCUMENT */

#ifdef HAVE_DEBUG_PAGE
#define PDF_DEBUG_PAGE(message, ...) \
  pdf_error (0, stderr, "***DEBUG PAGE***:%s:%d: " message, \
  __FILE__, __LINE__, __VA_ARGS__)
#else
#define PDF_DEBUG_PAGE(...)
#endif /* HAVE_DEBUG_PAGE */

/* BEGIN PUBLIC */

typedef enum 
{
        PDF_OK = 0,
        PDF_ERROR,
        PDF_EBADDATA,
        PDF_ENOMEM,
        PDF_EEOF,
        PDF_EDIVBYZERO,
        PDF_ENONODE,
        PDF_EINVRANGE,
        PDF_ETEXTENC,
        PDF_ENOMATCH,
        PDF_STATUS_ITEMS
} pdf_status_t;

extern const char * pdf_error_stlist [];

/* Print a message with `fprintf (fd, FORMAT, ...)';
   if status is nonzero, print the corresponding message. */
extern void pdf_error (pdf_status_t status, FILE * fd, const char *format, ...);

/* Print the message corresponding to 'status' to stderr
 * followed by 'str'.
 */
extern void pdf_perror (pdf_status_t status, const char *str);


/* END PUBLIC */

#endif /* PDF_ERROR_H */

/* End of pdf-error.h */

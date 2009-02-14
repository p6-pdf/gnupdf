/* -*- mode: C -*- Time-stamp: "2009-02-14 18:05:43 davazp"
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

#include <config.h>
#include <stdio.h>

#ifdef HAVE_DEBUG_BASE
#define PDF_DEBUG_BASE(message, ...) \
  pdf_error (0, stderr, "***DEBUG BASE***:%s:%d: " message, \
  __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define PDF_DEBUG_BASE(...)
#endif /* HAVE_DEBUG_BASE */

#ifdef HAVE_DEBUG_OBJECT
#define PDF_DEBUG_OBJECT(message, ...) \
  pdf_error (0, stderr, "***DEBUG OBJECT***:%s:%d: " message, \
  __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define PDF_DEBUG_OBJECT(...)
#endif /* HAVE_DEBUG_OBJECT */

#ifdef HAVE_DEBUG_DOCUMENT
#define PDF_DEBUG_DOCUMENT(message, ...) \
  pdf_error (0, stderr, "***DEBUG DOCUMENT***:%s:%d: " message, \
  __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define PDF_DEBUG_DOCUMENT(...)
#endif /* HAVE_DEBUG_DOCUMENT */

#ifdef HAVE_DEBUG_PAGE
#define PDF_DEBUG_PAGE(message, ...) \
  pdf_error (0, stderr, "***DEBUG PAGE***:%s:%d: " message, \
  __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define PDF_DEBUG_PAGE(...)
#endif /* HAVE_DEBUG_PAGE */

/* BEGIN PUBLIC */

#include <stdio.h>  /* Included again for the public header */

#define PDF_ERROR_LIST                                                  \
  ERROR_ENTRY (PDF_ERROR,       "error"),                               \
  ERROR_ENTRY (PDF_EBADDATA,    "invalid or bad arguments"),            \
  ERROR_ENTRY (PDF_ENOMEM,      "insufficient memory"),                 \
  ERROR_ENTRY (PDF_EEOF,        "end of file"),                         \
  ERROR_ENTRY (PDF_EDIVBYZERO,  "divison by zero"),                     \
  ERROR_ENTRY (PDF_ENONODE,     "no node found"),                       \
  ERROR_ENTRY (PDF_EINVRANGE,   "invalid range"),                       \
  ERROR_ENTRY (PDF_ETEXTENC,    "error in text encoding"),              \
  ERROR_ENTRY (PDF_ENOMATCH,    "no matching found"),                   \
  ERROR_ENTRY (PDF_EBADTEXT,    "invalid text format"),                 \
  ERROR_ENTRY (PDF_EBADPERMS,   "not enough permissions"),              \
  ERROR_ENTRY (PDF_EBADNAME,    "invalid path name"),                   \
  ERROR_ENTRY (PDF_EAGAIN,      "operation can't be performed now, maybe later"), \
  ERROR_ENTRY (PDF_ENOSPC,      "no space left on device"),             \
  ERROR_ENTRY (PDF_EBADCONTEXT, "text module global context not initialized"),    \
  ERROR_ENTRY (PDF_ENOTEMPTY,   "operation requires some entity be empty"), \
  ERROR_ENTRY (PDF_ENINPUT,     "filter needs more input"),             \
  ERROR_ENTRY (PDF_ENOUTPUT,    "filter needs more output room"),       \
  ERROR_ENTRY (PDF_EEXIST,      "already exists"),                      \
  ERROR_ENTRY (PDF_EOPENED,     "file is opened"),                      \
  ERROR_ENTRY (PDF_ECLOSED,     "file is closed"),                      \
  ERROR_ENTRY (PDF_EOVERFLOW,   "Input is too large"),                  \
  ERROR_ENTRY (PDF_EUNDERFLOW,  "Input is too small"),                  \
  ERROR_ENTRY (PDF_EMTOOBIG,    "/FunctionType 0: input dim too high (<= 12)"), \
  ERROR_ENTRY (PDF_EBADSAMPLES, "/FunctionType 0: error while reading sample table"), \
  ERROR_ENTRY (PDF_EBADAESKEY,  "the size of an AES  key should be a multiple of 16"), \
  ERROR_ENTRY (PDF_EBADV2KEY,   "a V2 key should be at least 40 bits long"), \
  ERROR_ENTRY (PDF_EINVOP,      "invalid operation"),                        \
  ERROR_ENTRY (PDF_EBADOP,      "/FunctionType 4: Unknown operator"),                 \
  ERROR_ENTRY (PDF_EMISSBODY,   "/FunctionType 4: Missing body conditional"),         \
  ERROR_ENTRY (PDF_ENOWRAP,     "/FunctionType 4: Code should be wrapped by braces"), \
  ERROR_ENTRY (PDF_ETOODEPTH,   "/FunctionType 4: Exceeded the max depth level"),     \
  ERROR_ENTRY (PDF_EMISSIF,     "/FunctionType 4: Missing if/ifelse operator"),       \
  ERROR_ENTRY (PDF_EBADTYPE,    "/FunctionType 4: Bad type"), \
  ERROR_ENTRY (PDF_ETYPE0,      "/FunctionType 0: Error"),            \
  ERROR_ENTRY (PDF_ETYPE2,      "/FunctionType 2: Error"),            \
  ERROR_ENTRY (PDF_ETYPE3,      "/FunctionType 3: Error"),            \
  ERROR_ENTRY (PDF_ETYPE4,      "/FunctionType 4: Error")

#define ERROR_ENTRY(id,string) id
enum pdf_status_e
{
  PDF_OK = 0,
  PDF_ERROR_LIST,
  PDF_STATUS_ITEMS
};
#undef ERROR_ENTRY

typedef enum pdf_status_e pdf_status_t;

extern const char * pdf_error_stlist [];

/* Print a message with `fprintf (fd, FORMAT, ...)';
   if status is nonzero, print the corresponding message. */
extern void pdf_error (const pdf_status_t status, FILE * fd, const char *format, ...);

/* Print the message corresponding to 'status' to stderr
 * followed by 'str'.
 */
extern void pdf_perror (const pdf_status_t status, const char *str);


/* END PUBLIC */

#endif /* PDF_ERROR_H */

/* End of pdf-error.h */

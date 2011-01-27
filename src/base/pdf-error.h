/* -*- mode: C -*-
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

#include <pdf-types.h>

/* BEGIN PUBLIC */

#include <stdio.h>  /* Included again for the public header */


/* --------------------- Error Data Types ------------------------- */


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
  ERROR_ENTRY (PDF_EBADCONTEXT, "global context not initialized"),      \
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
  ERROR_ENTRY (PDF_EBADTYPE,    "/FunctionType 4: Bad type"),         \
  ERROR_ENTRY (PDF_EMATH,       "/FunctionType 4: Math error"),       \
  ERROR_ENTRY (PDF_ETYPE0,      "/FunctionType 0: Error"),            \
  ERROR_ENTRY (PDF_ETYPE2,      "/FunctionType 2: Error"),            \
  ERROR_ENTRY (PDF_ETYPE3,      "/FunctionType 3: Error"),            \
  ERROR_ENTRY (PDF_ETYPE4,      "/FunctionType 4: Error"),            \
  ERROR_ENTRY (PDF_EBADFILE,    "file violates PDF specifications"),  \
  ERROR_ENTRY (PDF_EIMPLLIMIT,  "implementation limit exceeded")

typedef enum pdf_status_e pdf_status_t;
#define ERROR_ENTRY(id,string) id
enum pdf_status_e
{
  PDF_OK = 0,
  PDF_ERROR_LIST,
  PDF_STATUS_ITEMS
};
#undef ERROR_ENTRY
extern const pdf_char_t * pdf_error_stlist [];


#define PDF_ERROR_DOMAIN_LIST                                           \
   ERROR_ENTRY (PDF_EDOMAIN_UNDEFINED,       "[Undefined]"),            \
   ERROR_ENTRY (PDF_EDOMAIN_BASE_BASIC,      "[Base] Basic Types"),     \
   ERROR_ENTRY (PDF_EDOMAIN_BASE_HT,         "[Base] Hash Tables"),     \
   ERROR_ENTRY (PDF_EDOMAIN_BASE_LIST,       "[Base] Lists"),           \
   ERROR_ENTRY (PDF_EDOMAIN_BASE_STREAM,     "[Base] Streams"),         \
   ERROR_ENTRY (PDF_EDOMAIN_BASE_FP,         "[Base] Floating Points"), \
   ERROR_ENTRY (PDF_EDOMAIN_BASE_TEXT,       "[Base] Text"),            \
   ERROR_ENTRY (PDF_EDOMAIN_BASE_TIME,       "[Base] Time"),            \
   ERROR_ENTRY (PDF_EDOMAIN_BASE_FS,         "[Base] Filesystem"),      \
   ERROR_ENTRY (PDF_EDOMAIN_BASE_TOKENISER,  "[Base] Tokeniser"),       \
   ERROR_ENTRY (PDF_EDOMAIN_BASE_ENCRYPTION, "[Base] Encryption")

typedef enum pdf_error_domain_e pdf_error_domain_t;
#define ERROR_ENTRY(id,string) id
enum pdf_error_domain_e
{
  PDF_ERROR_DOMAIN_LIST,
  PDF_ERROR_DOMAIN_ITEMS
};
#undef ERROR_ENTRY
extern const pdf_char_t * pdf_error_domain_stlist [];

/* The PDF error object */
typedef struct pdf_error_s pdf_error_t;

/* --------------------- Error Reporting procedures ------------------------- */

/* Basic new()/destroy() and getters */
pdf_error_t *pdf_error_new (pdf_error_domain_t  domain,
                            pdf_status_t        status,
                            const pdf_char_t   *format,
                            ...);

pdf_status_t pdf_error_get_status (const pdf_error_t *error);

pdf_error_domain_t pdf_error_get_domain  (const pdf_error_t *error);

const pdf_char_t *pdf_error_get_message (const pdf_error_t *error);

void pdf_error_destroy (pdf_error_t *error);

/* Does nothing if err is NULL; if err is non-NULL, then *err must be
 * NULL. A new pdf_error_t is created and assigned to *err. */
void pdf_set_error (pdf_error_t        **err,
                    pdf_error_domain_t   domain,
                    pdf_status_t         status,
                    const pdf_char_t    *format,
                    ...);

/* Does nothing if err is NULL; if err is non-NULL, then destroys the
 * pdf_error_t in *err. */
void pdf_clear_error (pdf_error_t **err);

/* If dest is NULL, free src; otherwise moves src into *dest. */
void pdf_propagate_error (pdf_error_t **dest,
                          pdf_error_t  *src);


/* --------------------- Error Printing procedures ------------------------- */


/* Print a message with `fprintf (fd, FORMAT, ...)';
   if status is nonzero, print the corresponding message. */
void pdf_error (const pdf_status_t  status,
                FILE               *fd,
                const pdf_char_t   *format,
                ...);

/* Print the message corresponding to 'status' to stderr
 * followed by 'str'.
 */
void pdf_perror (const pdf_status_t  status,
                 const pdf_char_t   *str);


/* --------------------- Debugging procedures ------------------------- */


#ifdef PDF_HAVE_DEBUG_BASE
#define PDF_DEBUG_BASE(message, ...) \
  pdf_error (0, stderr, "***DEBUG BASE***:%s:%d: " message, \
  __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define PDF_DEBUG_BASE(...)
#endif /* PDF_HAVE_DEBUG_BASE */

#ifdef PDF_HAVE_DEBUG_OBJECT
#define PDF_DEBUG_OBJECT(message, ...) \
  pdf_error (0, stderr, "***DEBUG OBJECT***:%s:%d: " message, \
  __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define PDF_DEBUG_OBJECT(...)
#endif /* PDF_HAVE_DEBUG_OBJECT */

#ifdef PDF_HAVE_DEBUG_DOCUMENT
#define PDF_DEBUG_DOCUMENT(message, ...) \
  pdf_error (0, stderr, "***DEBUG DOCUMENT***:%s:%d: " message, \
  __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define PDF_DEBUG_DOCUMENT(...)
#endif /* PDF_HAVE_DEBUG_DOCUMENT */

#ifdef PDF_HAVE_DEBUG_PAGE
#define PDF_DEBUG_PAGE(message, ...) \
  pdf_error (0, stderr, "***DEBUG PAGE***:%s:%d: " message, \
  __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define PDF_DEBUG_PAGE(...)
#endif /* PDF_HAVE_DEBUG_PAGE */


#define PDF_ASSERT_BASE(condition)                                      \
  do                                                                    \
    {                                                                   \
      if (!(condition))                                                 \
        {                                                               \
          PDF_DEBUG_BASE("the assert `%s' failed", #condition);         \
        }                                                               \
    } while(0)

#define PDF_ASSERT_OBJECT(condition)                                    \
  do                                                                    \
    {                                                                   \
      if (!(condition))                                                 \
        {                                                               \
          PDF_DEBUG_OBJECT("the assert `%s' failed", #condition);       \
        }                                                               \
    } while(0)

#define PDF_ASSERT_DOCUMENT(condition)                                  \
  do                                                                    \
    {                                                                   \
      if (!(condition))                                                 \
        {                                                               \
          PDF_DEBUG_DOCUMENT("the assert `%s' failed", #condition);     \
        }                                                               \
    } while(0)

#define PDF_ASSERT_PAGE(condition)                                      \
  do                                                                    \
    {                                                                   \
      if (!(condition))                                                 \
        {                                                               \
          PDF_DEBUG_PAGE("the assert `%s' failed", #condition);         \
        }                                                               \
    } while(0)




/* END PUBLIC */

#endif /* PDF_ERROR_H */

/* End of pdf-error.h */

/* -*- mode: C -*-
 *
 *       File:         pdf-error.c
 *       Date:         Sun Feb  24 20:22:05 2008
 *
 *       GNU PDF Library - Implementation for the Error module
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

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <pdf-alloc.h>
#include <pdf-global.h>
#include <pdf-error.h>

extern pdf_char_t *pdf_library_name;

/* Update this list according to pdf_status_t. */
#define ERROR_ENTRY(id, string) string
const pdf_char_t * pdf_error_stlist [] =
  {
    PDF_ERROR_LIST
  };
#undef ERROR_ENTRY

/* Update this list according to pdf_error_domain_t. */
#define ERROR_ENTRY(id, string) string
const pdf_char_t * pdf_error_domain_stlist [] =
  {
    PDF_ERROR_DOMAIN_LIST
  };
#undef ERROR_ENTRY

/* Structure of the PDF error object */
struct pdf_error_s {
  pdf_error_domain_t domain;
  pdf_status_t status;
  pdf_char_t *message;
};

void
pdf_perror (const pdf_status_t  status,
            const pdf_char_t   *str)
{
  pdf_error ((int) status, stderr, str);
}

void
pdf_error (const pdf_status_t  status,
           FILE               *fd,
           const pdf_char_t   *format,
           ...)
{
  va_list args;
  int errnum;

  errnum = (int) status;

  if (fd == NULL)
    {
      fd = stderr;
    }

  fprintf (fd, "%s", pdf_library_name);

  if (format != NULL)
    {
      fprintf (fd, ": ");
      va_start (args, format);
      vfprintf (fd, format, args);
      va_end (args);
    }

  if (errnum >  0 && errnum < PDF_STATUS_ITEMS)
    fprintf (fd, ": %s", pdf_error_stlist[errnum-1]);

  fprintf (fd, ".\n");
  fflush (fd);
}

static pdf_error_t *
error_new_valist (pdf_error_domain_t  domain,
                  pdf_status_t        status,
                  const pdf_char_t   *format,
                  va_list             args)
{
  pdf_error_t *error;

  error = pdf_alloc (sizeof (struct pdf_error_s));
  if (error != NULL)
    {
      if (vasprintf (&(error->message), format, args) < 0)
        {
          pdf_dealloc (error);
          return NULL;
        }
      error->status = status;
      error->domain = domain;
    }
  return error;
}

pdf_error_t *
pdf_error_new (pdf_error_domain_t  domain,
               pdf_status_t        status,
               const pdf_char_t   *format,
               ...)
{
  va_list args;
  pdf_error_t *error;

  va_start (args, format);
  error = error_new_valist (domain,
                            status,
                            format,
                            args);
  va_end (args);
  return error;
}

pdf_status_t
pdf_error_get_status (const pdf_error_t *error)
{
  return error->status;
}

pdf_error_domain_t
pdf_error_get_domain (const pdf_error_t *error)
{
  return error->domain;
}

const pdf_char_t *
pdf_error_get_message (const pdf_error_t *error)
{
  return error->message;
}

void
pdf_error_destroy (pdf_error_t *error)
{
  if (error != NULL)
    {
      pdf_dealloc (error->message);
      pdf_dealloc (error);
    }
}

void
pdf_set_error (pdf_error_t        **err,
               pdf_error_domain_t   domain,
               pdf_status_t         status,
               const pdf_char_t    *format,
               ...)
{
  if (err != NULL)
    {
      va_list args;

      PDF_ASSERT_POINTER_RETURN (*err);

      va_start (args, format);
      *err = error_new_valist (domain,
                               status,
                               format,
                               args);
      va_end (args);
    }
}

void
pdf_clear_error (pdf_error_t **err)
{
  if ((err == NULL) ||
      (*err == NULL))
    return;

  pdf_error_destroy (*err);
  *err = NULL;
}

void
pdf_propagate_error (pdf_error_t **dest,
                     pdf_error_t  *src)
{
  if (dest == NULL)
    pdf_error_destroy (src);
  else
    *dest = src;
}

/* End of pdf-error.c */

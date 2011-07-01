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
#include <string.h>

/* From gnulib */
#include "vasnprintf.h"

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

/* This static constant structure is used as backup when setting a new error and
 * memory allocation of the new error fails. */
static const pdf_error_t static_enomem_error = {
  .domain = PDF_EDOMAIN_UNDEFINED,
  .status = PDF_ENOMEM,
  .message = "memory allocation failed"
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
    fd = stderr;

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
  if ((error != NULL) &&
      (vasprintf (&(error->message), format, args) >= 0) &&
      (error->message != NULL))
    {
      error->status = status;
      error->domain = domain;
    }
  else
    {
      if (error)
        pdf_dealloc (error);
      /* Set backup enomem error */
      error = (pdf_error_t *)&static_enomem_error;
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

pdf_error_t *
pdf_error_dup (const pdf_error_t *error)
{
  PDF_ASSERT_RETURN_VAL (error != NULL, NULL);

  return pdf_error_new (error->domain,
                        error->status,
                        error->message);
}

pdf_status_t
pdf_error_get_status (const pdf_error_t *error)
{
  PDF_ASSERT_RETURN_VAL (error != NULL, PDF_EBADDATA);

  return error->status;
}

pdf_error_domain_t
pdf_error_get_domain (const pdf_error_t *error)
{
  PDF_ASSERT_RETURN_VAL (error != NULL, PDF_EDOMAIN_UNDEFINED);

  return error->domain;
}

const pdf_char_t *
pdf_error_get_message (const pdf_error_t *error)
{
  PDF_ASSERT_RETURN_VAL (error != NULL, NULL);

  return error->message;
}

void
pdf_error_destroy (pdf_error_t *error)
{
  if (error != NULL &&
      error != &static_enomem_error)
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

      PDF_ASSERT (*err == NULL);

#ifdef PDF_HAVE_DEBUG_BASE
      if (*err != NULL)
        PDF_DEBUG_BASE ("  Previous error contents: %s",
                        (*err)->message);
#endif /* PDF_HAVE_DEBUG_BASE */

      va_start (args, format);
      *err = error_new_valist (domain,
                               status,
                               format,
                               args);
      va_end (args);
    }
}

void
pdf_prefix_error (pdf_error_t        **err,
                  const pdf_char_t    *format,
                  ...)
{
  if ((err != NULL) &&
      (*err != NULL) &&
      (*err != &static_enomem_error))
    {
      pdf_char_t *new_message = NULL;
      va_list args;
      pdf_bool_t enomem = PDF_FALSE;

      va_start (args, format);
      if ((vasprintf (&new_message, format, args) >= 0) &&
          (new_message != NULL))
        {
          pdf_char_t *prefixed;
          pdf_size_t new_message_len;

          new_message_len = (strlen (new_message) + strlen ((*err)->message) + 1);
          prefixed = pdf_realloc (new_message, new_message_len);
          if (!prefixed)
            {
              pdf_dealloc (new_message);
              enomem = PDF_TRUE;
            }
          else
            {
              strcat (prefixed, (*err)->message);
              prefixed[new_message_len - 1] = '\0';
              pdf_dealloc ((*err)->message);
              (*err)->message = prefixed;
            }
        }
      else
        enomem = PDF_TRUE;

      if (enomem)
        {
          /* Oops, ENOMEM */
          if (new_message)
            pdf_dealloc (new_message);
          pdf_error_destroy (*err);
          /* Set backup enomem error */
          *err = (pdf_error_t *)&static_enomem_error;
        }
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
  PDF_ASSERT_RETURN (src != NULL);

  if (dest == NULL)
    pdf_error_destroy (src);
  else
    *dest = src;
}

void
pdf_propagate_error_dup (pdf_error_t       **dest,
                         const pdf_error_t  *src)
{
  PDF_ASSERT_RETURN (src != NULL);

  if (dest)
    *dest = pdf_error_dup (src);
}

/* End of pdf-error.c */

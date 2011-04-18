/* -*- mode: C -*-
 *
 *       File:         pdf-types-buffer.c
 *       Date:         Wed Jan 26 17:35:27 2011
 *
 *       GNU PDF Library - Basic Types, Simple memory buffers
 *
 */

/* Copyright (C) 2008-2011 Free Software Foundation, Inc */

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

#include <pdf-types-buffer.h>
#include <pdf-alloc.h>

pdf_buffer_t *
pdf_buffer_new (pdf_size_t    size,
                pdf_error_t **error)
{
  pdf_buffer_t *new_buf;

  /* Don't allow creating zero-sized buffers */
  PDF_ASSERT_RETURN_VAL (size > 0, NULL);

  /* We cannot allocate in a single run both the buffer control struct and the
   * internal buffer, as we allow resizing the internal buffer */
  new_buf = pdf_alloc (sizeof (struct pdf_buffer_s));
  if (!new_buf)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_BASIC,
                     PDF_ENOMEM,
                     "not enough memory to create the buffer control structure: "
                     "couldn't allocate %lu bytes",
                     (unsigned long) sizeof (struct pdf_buffer_s));
      return PDF_FALSE;
    }

  /* Allocate internal buffer */
  new_buf->data = pdf_alloc (size);
  if (!new_buf->data)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_BASIC,
                     PDF_ENOMEM,
                     "not enough memory to create the internal buffer: "
                     "couldn't allocate %lu bytes",
                     (unsigned long) size);
      pdf_dealloc (new_buf);
      return PDF_FALSE;
    }

  new_buf->size = size;
  pdf_buffer_rewind (new_buf);

  return new_buf;
}

void
pdf_buffer_destroy (pdf_buffer_t *buffer)
{
  if (!buffer)
    return;

  pdf_dealloc (buffer->data);
  pdf_dealloc (buffer);
}

pdf_bool_t
pdf_buffer_full_p (pdf_buffer_t *buffer)
{
  PDF_ASSERT_POINTER_RETURN_VAL (buffer, PDF_FALSE);

  return (buffer->wp == buffer->size ?
          PDF_TRUE : PDF_FALSE);
}

pdf_bool_t
pdf_buffer_eob_p (pdf_buffer_t *buffer)
{
  PDF_ASSERT_POINTER_RETURN_VAL (buffer, PDF_FALSE);

  return ((buffer->wp - buffer->rp) == 0 ?
          PDF_TRUE : PDF_FALSE);
}

pdf_bool_t
pdf_buffer_resize (pdf_buffer_t  *buffer,
                   pdf_size_t     newsize,
                   pdf_error_t  **error)
{
  pdf_char_t *newdata;

  PDF_ASSERT_POINTER_RETURN_VAL (buffer, PDF_FALSE);
  PDF_ASSERT_RETURN_VAL (newsize > 0, PDF_FALSE);

  if (newsize == buffer->size)
    return PDF_TRUE;

  /* Allocate new buffer size */
  newdata = pdf_realloc (buffer->data, newsize);
  if (!newdata)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_BASIC,
                     PDF_ENOMEM,
                     "not enough memory to reallocate the internal buffer: "
                     "couldn't allocate %lu bytes",
                     (unsigned long) newsize);
      return PDF_FALSE;
    }

  /* If properly reallocated, set new buffer contents */
  buffer->data = newdata;
  buffer->size = newsize;
  buffer->rp = PDF_MIN (buffer->rp, newsize);
  buffer->wp = PDF_MIN (buffer->wp, newsize);
  return PDF_TRUE;
}

void
pdf_buffer_rewind (pdf_buffer_t *buffer)
{
  PDF_ASSERT_RETURN (buffer);

  buffer->rp = 0;
  buffer->wp = 0;
}

/* End of pdf-types-buffer.c */

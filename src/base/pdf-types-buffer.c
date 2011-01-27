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

pdf_buffer_t
pdf_buffer_new (pdf_size_t size)
{
  pdf_buffer_t new_buf;

  new_buf = pdf_alloc (sizeof(struct pdf_buffer_s));

  if (new_buf != NULL)
    {
      new_buf->data = pdf_alloc (sizeof(pdf_char_t) * size);
      new_buf->size = size;
      pdf_buffer_rewind (new_buf);
    }

  return new_buf;
}

pdf_status_t
pdf_buffer_destroy (pdf_buffer_t buffer)
{
  pdf_dealloc (buffer->data);
  pdf_dealloc (buffer);

  return PDF_OK;
}

pdf_bool_t
pdf_buffer_full_p (pdf_buffer_t buffer)
{
  return (buffer->wp == buffer->size);
}

pdf_bool_t
pdf_buffer_eob_p (pdf_buffer_t buffer)
{
  return ((buffer->wp - buffer->rp) == 0);
}


pdf_status_t
pdf_buffer_resize (pdf_buffer_t buffer,
                   pdf_size_t   newsize)
{
  pdf_char_t *newdata = pdf_realloc (buffer->data, newsize);

  if (!newdata)
    return PDF_ENOMEM;

  buffer->data = newdata;
  buffer->size = newsize;
  buffer->rp = PDF_MIN (buffer->rp, newsize);
  buffer->wp = PDF_MIN (buffer->wp, newsize);
  return PDF_OK;
}

pdf_status_t
pdf_buffer_rewind (pdf_buffer_t buffer)
{
  buffer->rp = 0;
  buffer->wp = 0;

  return PDF_OK;
}

/* End of pdf-buffer.c */

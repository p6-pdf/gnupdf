/* -*- mode: C -*- Time-stamp: "08/09/12 05:47:44 jemarch"
 *
 *       File:         pdf-stm-buffer.c
 *       Date:         Wed Jul 23 23:28:59 2008
 *
 *       GNU PDF Library - Stream memory buffer
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

#include <pdf-alloc.h>
#include <pdf-stm-buffer.h>

pdf_stm_buffer_t
pdf_stm_buffer_new (pdf_size_t size)
{
  pdf_stm_buffer_t new_buf;

  new_buf = pdf_alloc (sizeof(struct pdf_stm_buffer_s));

  if (new_buf != NULL)
    {
      new_buf->data = pdf_alloc (sizeof(pdf_char_t) * size);
      new_buf->size = size;
      pdf_stm_buffer_rewind (new_buf);
    }

  return new_buf;
}

pdf_status_t
pdf_stm_buffer_destroy (pdf_stm_buffer_t buffer)
{
  pdf_dealloc (buffer->data);
  pdf_dealloc (buffer);

  return PDF_OK;
}

pdf_bool_t
pdf_stm_buffer_full_p (pdf_stm_buffer_t buffer)
{
  return (buffer->wp == buffer->size);
}

pdf_bool_t
pdf_stm_buffer_eob_p (pdf_stm_buffer_t buffer)
{
  return ((buffer->wp - buffer->rp) == 0);
}

pdf_status_t
pdf_stm_buffer_rewind (pdf_stm_buffer_t buffer)
{
  buffer->rp = 0;
  buffer->wp = 0;
}

/* End of pdf-stm-buffer.c */

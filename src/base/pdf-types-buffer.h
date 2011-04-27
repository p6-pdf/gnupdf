/* -*- mode: C -*-
 *
 *       File:         pdf-types-buffer.h
 *       Date:         Wed Jan 26 17:34:21 2011
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

#ifndef PDF_TYPES_BUFFER_H
#define PDF_TYPES_BUFFER_H

#include <config.h>

#include <pdf-types.h>
#include <pdf-error.h>

/* BEGIN PUBLIC */

/* --------------------- Buffers ------------------------- */

struct pdf_buffer_s
{
  pdf_uchar_t *data;   /* Buffer contents */
  pdf_size_t size;     /* Size of the buffer in octects */
  pdf_size_t rp;       /* Read pointer */
  pdf_size_t wp;       /* Write pointer */
};

typedef struct pdf_buffer_s pdf_buffer_t;

/* Create new buffer */
pdf_buffer_t *pdf_buffer_new (pdf_size_t    size,
                              pdf_error_t **error);

/* Destroy a buffer */
void pdf_buffer_destroy (pdf_buffer_t *buffer);

/* Check if buffer is full */
pdf_bool_t pdf_buffer_full_p (pdf_buffer_t *buffer);

/* Check if end-of-buffer reached */
pdf_bool_t pdf_buffer_eob_p (pdf_buffer_t *buffer);

/* Resize the given buffer */
pdf_bool_t pdf_buffer_resize (pdf_buffer_t  *buffer,
                              pdf_size_t     newsize,
                              pdf_error_t  **error);

/* Rewind a buffer */
void pdf_buffer_rewind (pdf_buffer_t *buffer);

/* END PUBLIC */

#endif /* !PDF_TYPES_BUFFER_H */

/* End of pdf-types-buffer.h */

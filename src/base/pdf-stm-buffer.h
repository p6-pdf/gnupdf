/* -*- mode: C -*- Time-stamp: "08/07/28 00:35:51 jemarch"
 *
 *       File:         pdf-stm-buffer.h
 *       Date:         Wed Jun 18 20:34:23 2008
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

#ifndef PDF_STM_BUFFER_H
#define PDF_STM_BUFFER_H

#include <pdf-types.h>

/* BEGIN PUBLIC */

/* Memory buffer data type */
struct pdf_stm_buffer_s
{
  pdf_char_t *data;    /* Buffer contents */
  pdf_size_t size;     /* Size of the buffer in octects */
};

typedef struct pdf_stm_buffer_s *pdf_stm_buffer_t;

/* END PUBLIC */

/*
 * Public functions
 */

pdf_stm_buffer_t pdf_stm_buffer_new (pdf_size_t size);
pdf_status_t pdf_stm_buffer_destroy (pdf_stm_buffer_t buffer);

#endif /* !PDF_STM_BUFFER_H */

/* End of pdf-stm-buffer.h */

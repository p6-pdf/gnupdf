/* -*- mode: C -*- Time-stamp: "08/11/29 15:14:38 jemarch"
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

/* Memory buffer data type */
struct pdf_stm_buffer_s
{
  pdf_char_t *data;    /* Buffer contents */
  pdf_size_t size;     /* Size of the buffer in octects */
  pdf_size_t rp;       /* Read pointer */
  pdf_size_t wp;       /* Write pointer */
};

typedef struct pdf_stm_buffer_s *pdf_stm_buffer_t;

/*
 * Public functions
 */

pdf_stm_buffer_t pdf_stm_buffer_new (pdf_size_t size);
pdf_status_t pdf_stm_buffer_destroy (pdf_stm_buffer_t buffer);

pdf_bool_t pdf_stm_buffer_full_p (pdf_stm_buffer_t buffer);
pdf_bool_t pdf_stm_buffer_eob_p (pdf_stm_buffer_t buffer);

pdf_status_t pdf_stm_buffer_rewind (pdf_stm_buffer_t buffer);

#endif /* !PDF_STM_BUFFER_H */

/* End of pdf-stm-buffer.h */

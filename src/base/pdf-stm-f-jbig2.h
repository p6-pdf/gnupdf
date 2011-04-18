/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-jbig2.h
 *       Date:         Fri Nov 21 01:07:54 2008
 *
 *       GNU PDF Library - JBIG2 encoder/decoder
 *
 */

/* Copyright (C) 2008, 2009 Free Software Foundation, Inc. */

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

#ifndef PDF_STM_F_JBIG2_H
#define PDF_STM_F_JBIG2_H

#include <config.h>

#include <jbig2.h>

#include <pdf-types.h>
#include <pdf-types-buffer.h>
#include <pdf-hash.h>

/* Internal state */
struct pdf_stm_f_jbig2dec_s
{
  Jbig2Allocator *jbig2_allocator;
  Jbig2Ctx *jbig2_context;
  Jbig2GlobalCtx *jbig2_global_context;
  Jbig2ErrorCallback jbig2_error_cb_fn;
  Jbig2Image *jbig2_page;
  pdf_size_t index;
  pdf_bool_t error_p;
  pdf_size_t data_size;
};

typedef struct pdf_stm_f_jbig2dec_s *pdf_stm_f_jbig2dec_t;

/* Filters implementation API */

pdf_status_t pdf_stm_f_jbig2dec_init (pdf_hash_t  *params,
                                      void       **state);
pdf_status_t pdf_stm_f_jbig2dec_apply (pdf_hash_t   *params,
                                       void         *state,
                                       pdf_buffer_t *in,
                                       pdf_buffer_t *out,
                                       pdf_bool_t    finish_p);
pdf_status_t pdf_stm_f_jbig2dec_dealloc_state (void *state);

#endif /* pdf-stm-f-jbig2.h */

/* End of pdf-stm-f-jbig2.h */

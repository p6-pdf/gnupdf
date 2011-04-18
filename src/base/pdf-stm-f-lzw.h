/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-lzw.h
 *       Date:         Wed Aug 15 14:13:20 2007
 *
 *       GNU PDF Library - LZW encoder/decoder
 *
 */

/* Copyright (C) 2007, 2008, 2009 Free Software Foundation, Inc. */

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

#ifndef PDF_STM_F_LZW_H
#define PDF_STM_F_LZW_H

#include <config.h>

#include <pdf-types.h>
#include <pdf-types-buffer.h>
#include <pdf-hash.h>

/* Filter API implementation */

pdf_status_t pdf_stm_f_lzwenc_init (pdf_hash_t  *params,
                                    void       **state);
pdf_status_t pdf_stm_f_lzwenc_apply (pdf_hash_t   *params,
                                     void         *state,
                                     pdf_buffer_t *in,
                                     pdf_buffer_t *out,
                                     pdf_bool_t    finish_p);

pdf_status_t pdf_stm_f_lzwdec_init (pdf_hash_t  *params,
                                    void       **state);
pdf_status_t pdf_stm_f_lzwdec_apply (pdf_hash_t   *params,
                                     void         *state,
                                     pdf_buffer_t *in,
                                     pdf_buffer_t *out,
                                     pdf_bool_t    finish_p);

pdf_status_t pdf_stm_f_lzwdec_dealloc_state (void *state);

pdf_status_t pdf_stm_f_lzwenc_dealloc_state (void *state);

#endif /* pdf_stm_f_lzw.h */

/* End of pdf_stm_f_lzw.h */

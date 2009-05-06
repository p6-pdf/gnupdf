/* -*- mode: C -*- Time-stamp: "09/05/06 19:43:10 jemarch"
 *
 *       File:         pdf-stm-f-dct.h
 *       Date:         Mon Dec 13 21:21:35 2008
 *
 *       GNU PDF Library - DCT encoder/decoder
 *
 */

/* Copyright (C) 2009 Free Software Foundation, Inc. */

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

#ifndef PDF_STM_F_DCT_H
#define PDF_STM_F_DCT_H
#include <config.h>
#include <pdf-types.h>
#include <pdf-hash.h>

/* Filters implementation API */

pdf_status_t pdf_stm_f_dctdec_init (pdf_hash_t params,
                                     void **state);
pdf_status_t pdf_stm_f_dctdec_apply (pdf_hash_t params,
                                      void *state,
                                      pdf_buffer_t in,
                                      pdf_buffer_t out,
                                      pdf_bool_t finish_p);
pdf_status_t pdf_stm_f_dctdec_dealloc_state (void *state);

pdf_status_t pdf_stm_f_dctenc_init (pdf_hash_t params,
                                     void **state);
pdf_status_t pdf_stm_f_dctenc_apply (pdf_hash_t params,
                                      void *state,
                                      pdf_buffer_t in,
                                      pdf_buffer_t out,
                                      pdf_bool_t finish_p);
pdf_status_t pdf_stm_f_dctenc_dealloc_state (void *state);

#endif /* pdf_stm_f_dct.h */

/* End of pdf_stm_f_dct.h */

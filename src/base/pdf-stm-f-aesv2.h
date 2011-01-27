/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-aesv2.h
 *       Date:         Fri Dec  5 16:58:49 2008
 *
 *       GNU PDF Library - AESv2 stream filter
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

#ifndef PDF_STM_F_AESV2_H
#define PDF_STM_F_AESV2_H

#include <config.h>

#include <stdio.h>

#include <pdf-types.h>
#include <pdf-types-buffer.h>
#include <pdf-hash.h>
#include <pdf-alloc.h>
#include <pdf-crypt.h>

pdf_status_t pdf_stm_f_aesv2enc_init (pdf_hash_t params,
                                      void **state);

pdf_status_t pdf_stm_f_aesv2enc_apply (pdf_hash_t params,
                                       void *state,
                                       pdf_buffer_t in,
                                       pdf_buffer_t out,
                                       pdf_bool_t finish_p);

pdf_status_t pdf_stm_f_aesv2enc_dealloc_state (void *state);


pdf_status_t pdf_stm_f_aesv2dec_init (pdf_hash_t params,
                                      void **state);

pdf_status_t pdf_stm_f_aesv2dec_apply (pdf_hash_t params,
                                       void *state,
                                       pdf_buffer_t in,
                                       pdf_buffer_t out,
                                       pdf_bool_t finish_p);

pdf_status_t pdf_stm_f_aesv2dec_dealloc_state (void *state);



#endif /* PDF_STM_F_AESV2_H */

/* End of pdf_stm_f_aesv2.h */

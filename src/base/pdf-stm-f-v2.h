/* -*- mode: C -*- Time-stamp: "2008-12-14 20:16:02 davazp"
 *
 *       File:         pdf-stm-f-v2.h
 *       Date:         Fri Dec  5 16:58:49 2008
 *
 *       GNU PDF Library - V2 stream filter
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

#ifndef PDF_STM_F_V2_H
#define PDF_STM_F_V2_H

#include <config.h>
#include <pdf-types.h>
#include <pdf-hash.h>
#include <pdf-alloc.h>
#include <pdf-stm-buffer.h>
#include <pdf-crypt.h>
#include <stdio.h>


/* Filter API implementation */

pdf_status_t pdf_stm_f_v2enc_init (pdf_hash_t params,
                                   void **state);

pdf_status_t pdf_stm_f_v2enc_apply (pdf_hash_t params,
                                    void *state,
                                    pdf_stm_buffer_t in,
                                    pdf_stm_buffer_t out,
                                    pdf_bool_t finish_p);

pdf_status_t pdf_stm_f_v2enc_dealloc_state (void *state);


pdf_status_t pdf_stm_f_v2dec_init (pdf_hash_t params,
                                   void **state);

pdf_status_t pdf_stm_f_v2dec_apply (pdf_hash_t params,
                                    void *state,
                                    pdf_stm_buffer_t in,
                                    pdf_stm_buffer_t out,
                                    pdf_bool_t finish_p);

pdf_status_t pdf_stm_f_v2dec_dealloc_state (void *state);



#endif /* PDF_STM_F_V2_H */

/* End of pdf_stm_f_v2.h */

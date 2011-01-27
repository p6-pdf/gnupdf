/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-rl.h
 *       Date:         Sun Jul 15 21:57:35 2007
 *
 *       GNU PDF Library - RunLength encoder/decoder
 *
 */

/* Copyright (C) 2007-2011 Free Software Foundation, Inc. */

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

#ifndef PDF_STM_F_RL_H
#define PDF_STM_F_RL_H

#include <config.h>

#include <pdf-types.h>
#include <pdf-types-buffer.h>
#include <pdf-hash.h>

typedef enum
{
        PDF_STM_F_RL_WRL=0,
        PDF_STM_F_RL_NONE
} pdf_stm_f_rl_enc_e;

struct pdf_stm_f_rl_s
{
  pdf_uchar_t curchar;
  pdf_uchar_t rlchar;
  pdf_uchar_t dec_count;
  pdf_i32_t rl;
  pdf_bool_t run_p, dec_p;
  pdf_stm_f_rl_enc_e enc_p;
};

typedef struct pdf_stm_f_rl_s * pdf_stm_f_rl_t;


/* Filter API implementation */

pdf_status_t pdf_stm_f_rldec_init (pdf_hash_t params,
                                   void **state);

pdf_status_t pdf_stm_f_rldec_apply (pdf_hash_t params,
                                    void *state,
                                    pdf_buffer_t in,
                                    pdf_buffer_t out,
                                    pdf_bool_t finish_p);
pdf_status_t pdf_stm_f_rldec_dealloc_state (void *state);

pdf_status_t pdf_stm_f_rlenc_init (pdf_hash_t params,
                                   void **state);

pdf_status_t pdf_stm_f_rlenc_apply (pdf_hash_t params,
                                    void *state,
                                    pdf_buffer_t in,
                                    pdf_buffer_t out,
                                    pdf_bool_t finish_p);
pdf_status_t pdf_stm_f_rlenc_dealloc_state (void *state);

#endif /* pdf_stm_f_rl.h */

/* End of pdf_stm_f_rl.h */

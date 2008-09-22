/* -*- mode: C -*- Time-stamp: "08/09/22 22:05:16 jemarch"
 *
 *       File:         pdf-stm-f-ahex.h
 *       Date:         Fri Jul 13 17:04:29 2007
 *
 *       GNU PDF Library - ASCIIHex encoder/decoder
 *
 */

/* Copyright (C) 2007, 2008 Free Software Foundation, Inc. */

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

#ifndef PDF_STM_F_AHEX_H
#define PDF_STM_F_AHEX_H

#include <config.h>
#include <pdf-types.h>
#include <pdf-hash.h>
#include <pdf-stm-buffer.h>

/* Internal state */
struct pdf_stm_f_ahexenc_s
{
  pdf_i32_t last_nibble;
};

typedef struct pdf_stm_f_ahexenc_s *pdf_stm_f_ahexenc_t;

/* Filters implementation API */

pdf_status_t pdf_stm_f_ahexdec_init (pdf_hash_t params,
                                     void **state);
pdf_status_t pdf_stm_f_ahexdec_apply (pdf_hash_t params,
                                      void *state,
                                      pdf_stm_buffer_t in,
                                      pdf_stm_buffer_t out,
                                      pdf_bool_t finish_p);
pdf_status_t pdf_stm_f_ahexenc_init (pdf_hash_t params,
                                     void **state);
pdf_status_t pdf_stm_f_ahexenc_apply (pdf_hash_t params,
                                      void *state,
                                      pdf_stm_buffer_t in,
                                      pdf_stm_buffer_t out,
                                      pdf_bool_t finish_p);

#endif /* pdf_stm_f_ahex.h */

/* End of pdf_stm_f_ahex.h */

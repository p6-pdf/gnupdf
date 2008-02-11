/* -*- mode: C -*- Time-stamp: "08/02/11 01:04:56 jemarch"
 *
 *       File:         pdf-stm-f-rl.h
 *       Date:         Sun Jul 15 21:57:35 2007
 *
 *       GNU PDF Library - RunLength encoder/decoder
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

#ifndef PDF_STM_F_RL_H
#define PDF_STM_F_RL_H

#include <config.h>
#include <pdf-base.h>

/* BEGIN PUBLIC */

enum pdf_stm_f_rl_mode_t
{
  PDF_STM_F_RL_MODE_ENCODE,
  PDF_STM_F_RL_MODE_DECODE
};

/* END PUBLIC */

/* Configuration data */

struct pdf_stm_f_rl_conf_s
{
  int mode;
};

typedef struct pdf_stm_f_rl_conf_s *pdf_stm_f_rl_conf_t;

/* Private data */

struct pdf_stm_f_rl_data_s
{
  int mode;
};

typedef struct pdf_stm_f_rl_data_s *pdf_stm_f_rl_data_t;

/* Filter API implementation */

int pdf_stm_f_rl_init (void **filter_data, void *conf_data);
int pdf_stm_f_rl_apply (void *filter_data,
                        pdf_char_t *in, pdf_stm_pos_t in_size,
                        pdf_char_t **out, pdf_stm_pos_t *out_size);
int pdf_stm_f_rl_dealloc (void **filter_data);

#endif /* pdf_stm_f_rl.h */

/* End of pdf_stm_f_rl.h */

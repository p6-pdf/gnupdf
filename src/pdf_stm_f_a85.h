/* -*- mode: C -*- Time-stamp: "07/10/23 12:54:00 jemarch"
 *
 *       File:         pdf_stm_f_a85.h
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Sun Jul 15 05:57:38 2007
 *
 *       GNU PDF Library - ASCII85 encoder/decoder
 *
 */

/* Copyright (C) 2007 Free Software Foundation, Inc. */

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


#ifndef PDF_STM_F_A85_H
#define PDF_STM_F_A85_H

#include <config.h>
#include <pdf_base.h>

/* Configuration data */

enum pdf_stm_f_a85_mode_t
{
  PDF_STM_F_A85_MODE_ENCODE,
  PDF_STM_F_A85_MODE_DECODE
};

struct pdf_stm_f_a85_conf_s
{
  int mode;
};

typedef struct pdf_stm_f_a85_conf_s *pdf_stm_f_a85_conf_t;

/* Private data */

struct pdf_stm_f_a85_data_s
{
  int mode;
};

typedef struct pdf_stm_f_a85_data_s *pdf_stm_f_a85_data_t;

/* Filter API implementation */

int pdf_stm_f_a85_init (void **filter_data, void *conf_data);
int pdf_stm_f_a85_apply (void *filter_data,
                         pdf_char_t *in, pdf_stm_pos_t in_size,
                         pdf_char_t **out, pdf_stm_pos_t *out_size);
int pdf_stm_f_a85_dealloc (void **filter_data);

#endif /* pdf_stm_f_a85.h */

/* End of pdf_stm_f_a85.h */

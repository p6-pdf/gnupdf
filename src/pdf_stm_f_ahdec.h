/* -*- mode: C -*- Time-stamp: "07/07/13 17:16:15 jemarch"
 *
 *       File:         pdf_stm_f_ahdec.h
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Fri Jul 13 17:04:29 2007
 *
 *       GNU PDF Library - ASCIIHex decoder filter
 *
 */

/* Copyright (C) 2007 Jose E. Marchesi */

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

#ifndef PDF_STM_F_AHDEC_H
#define PDF_STM_F_AHDEC_H

#include <config.h>
#include <pdf_base.h>

/* This filter do not have a configuration structure */

/* Private data */

struct pdf_stm_f_ahdec_data_s
{
  int dummy;
};

typedef struct pdf_stm_f_ahdec_data_s *pdf_stm_f_ahdec_data_t;

/* Filter API implementation */

int pdf_stm_f_ahdec_init (void **filter_data, void *conf_data);
int pdf_stm_f_ahdec_apply (void *filter_data,
                           pdf_char_t *in, pdf_stm_pos_t in_size,
                           pdf_char_t **out, pdf_stm_pos_t *out_size);
int pdf_stm_f_ahdec_dealloc (void **filter_data);

#endif /* pdf_stm_f_ahdec.h */

/* End of pdf_stm_f_ahdec.h */

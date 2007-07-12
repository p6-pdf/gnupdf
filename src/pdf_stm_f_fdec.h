/* -*- mode: C -*- Time-stamp: "07/07/12 01:21:10 jemarch"
 *
 *       File:         pdf_stm_f_fdec.h
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Tue Jul 10 23:39:27 2007
 *
 *       GNU PDF Library - FlateDecode stream filter
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

#ifndef PDF_STM_F_FDEC_H
#define PDF_STM_F_FDEC_H

#include <config.h>
#include <pdf_base.h>

/* This filter doesnt have a configuration structure */

/* Private data */

struct pdf_stm_f_fdec_data_s
{
  int dummy;
};

typedef struct pdf_stm_f_fdec_data_s *pdf_stm_f_fdec_data_t;

/* Filter API implementation */

int pdf_stm_f_fdec_init (void **filter_data, void *conf_data);
int pdf_stm_f_fdec_apply (void *filter_data,
                          pdf_char *in, pdf_stm_pos_t in_size,
                          pdf_char **out, pdf_stm_pos_t *out_size);
int pdf_stm_f_fdec_dealloc (void **filter_data);


#endif /* pdf_stm_f_fdec.h */

/* End of pdf_stm_f_fdec.h */

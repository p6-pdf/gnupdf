/* -*- mode: C -*- Time-stamp: "07/07/09 22:24:27 jemarch"
 *
 *       File:         pdf_stm_f_null.h
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Mon Jul  9 21:59:50 2007
 *
 *       GNU PDF Library - NULL stream filter
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

#ifndef PDF_STM_F_NULL_H
#define PDF_STM_F_NULL_H

#include <config.h>
#include <pdf_base.h>

/* This filter do not have a configuration structure */

/* Private data */

struct pdf_stm_f_null_data_s
{
  int dummy;
};

typedef struct pdf_stm_f_null_data_s *pdf_stm_f_null_data_t;

/* Filter API implementation */

int pdf_stm_f_null_init (void **filter_data, void *conf_data);
int pdf_stm_f_null_apply (void *filter_data,
                          char *in, pdf_stm_pos_t in_size,
                          char **out, pdf_stm_pos_t *out_size);
int pdf_stm_f_null_dealloc (void **filter_data);


#endif /* pdf_stm_f_null.h */

/* End of pdf_stm_f_null.h */

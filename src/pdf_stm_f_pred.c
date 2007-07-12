/* -*- mode: C -*- Time-stamp: "07/07/12 01:37:54 jemarch"
 *
 *       File:         pdf_stm_f_pred.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Thu Jul 12 01:21:33 2007
 *
 *       GNU PDF Library - Predictor stream filter
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

#include <config.h>

#include <malloc.h>
#include <xalloc.h>
#include <string.h>
#include <pdf_base.h>
#include <pdf_stm_f_pred.h>

int
pdf_stm_f_pred_init (void **filter_data,
                     void *conf_data)
{
  pdf_stm_f_pred_data_t *data;
  pdf_stm_f_pred_conf_t conf;

  data = (pdf_stm_f_pred_data_t *) filter_data;
  conf = (pdf_stm_f_pred_conf_t) conf_data;

  /* Create the private data storage */
  *data =
    (pdf_stm_f_pred_data_t) xmalloc (sizeof(struct pdf_stm_f_pred_data_s));
  
  (*data)->predictor = conf->predictor;
  (*data)->colors = conf->colors;
  (*data)->bits_per_component = conf->bits_per_component;
  (*data)->columns = conf->columns;

  return PDF_OK;
}

int
pdf_stm_f_pred_apply (void *filter_data,
                      pdf_char *in, pdf_stm_pos_t in_size,
                      pdf_char **out, pdf_stm_pos_t *out_size)
{
  pdf_stm_f_pred_data_t data;

  data = (pdf_stm_f_pred_data_t) filter_data;

  *out = (pdf_char *) xmalloc (in_size);
  memcpy (*out,
          in,
          in_size);
  *out_size = in_size;

  return PDF_OK;
}

int
pdf_stm_f_pred_dealloc (void **filter_data)
{
  pdf_stm_f_pred_data_t *data;

  data = (pdf_stm_f_pred_data_t *) filter_data;
  free (*data);
  
  return PDF_OK;
}

/* End of pdf_stm_f_pred.c */

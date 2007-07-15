/* -*- mode: C -*- Time-stamp: "07/07/15 06:09:14 jemarch"
 *
 *       File:         pdf_stm_f_a85.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Sun Jul 15 06:01:42 2007
 *
 *       GNU PDF Library - ASCII85 encoder/decoder
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
#include <stdio.h>
#include <pdf_stm_f_a85.h>


int
pdf_stm_f_a85_init (void **filter_data,
                    void *conf_data)
{
  pdf_stm_f_a85_data_t *data;
  pdf_stm_f_a85_conf_t conf;

  data = (pdf_stm_f_a85_data_t *) filter_data;
  conf = (pdf_stm_f_a85_conf_t) conf_data;

  /* Create the private data storage */
  *data =
    (pdf_stm_f_a85_data_t) xmalloc (sizeof(struct pdf_stm_f_a85_data_s));
  (*data)->mode = conf->mode;

  return PDF_OK;
}

int
pdf_stm_f_a85_apply (void *filter_data,
                     pdf_char_t *in, pdf_stm_pos_t in_size,
                     pdf_char_t **out, pdf_stm_pos_t *out_size)
{
  return PDF_ERROR;
}

int
pdf_stm_f_a85_dealloc (void **filter_data)
{
  pdf_stm_f_a85_data_t *data;

  data = (pdf_stm_f_a85_data_t *) filter_data;
  free (*data);

  return PDF_OK;
}

/* End of pdf_stm_f_a85.c */

/* -*- mode: C -*- Time-stamp: "07/07/15 02:13:32 jemarch"
 *
 *       File:         pdf_stm_f_flate.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Tue Jul 10 23:44:00 2007
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

#include <config.h>

#include <stdio.h>
#include <malloc.h>
#include <xalloc.h>
#include <string.h>
#include <zlib.h>
#include <pdf_base.h>
#include <pdf_stm_f_flate.h>


static int pdf_stm_f_flate_encode (pdf_char_t *in, pdf_stm_pos_t in_size,
                                   pdf_char_t **out, pdf_stm_pos_t *out_size);
static int pdf_stm_f_flate_decode (pdf_char_t *in, pdf_stm_pos_t in_size,
                                   pdf_char_t **out, pdf_stm_pos_t *out_size);

int
pdf_stm_f_flate_init (void **filter_data,
                     void *conf_data)
{
  pdf_stm_f_flate_data_t *data;
  pdf_stm_f_flate_conf_t conf;

  data = (pdf_stm_f_flate_data_t *) filter_data;
  conf = (pdf_stm_f_flate_conf_t) conf_data;

  /* Create the private data storage */
  *data =
    (pdf_stm_f_flate_data_t) xmalloc (sizeof(struct pdf_stm_f_flate_data_s));
  (*data)->mode = conf->mode;

  return PDF_OK;
}

int
pdf_stm_f_flate_apply (void *filter_data,
                      pdf_char_t *in, pdf_stm_pos_t in_size,
                      pdf_char_t **out, pdf_stm_pos_t *out_size)
{
  pdf_stm_f_flate_data_t data;

  data = (pdf_stm_f_flate_data_t) filter_data;
  switch (data->mode)
    {
    case PDF_STM_F_FLATE_MODE_ENCODE:
      {
        return pdf_stm_f_flate_encode (in, in_size, out, out_size);
      }
    case PDF_STM_F_FLATE_MODE_DECODE:
      {
        return pdf_stm_f_flate_decode (in, in_size, out, out_size);
      }
    default:
      {
        return PDF_ERROR;
      }
    }
  
  /* Not reached */
}

int
pdf_stm_f_flate_dealloc (void **filter_data)
{
  pdf_stm_f_flate_data_t *data;

  data = (pdf_stm_f_flate_data_t *) filter_data;
  free (*data);

  return PDF_OK;
}

/* Private functions */

static int
pdf_stm_f_flate_encode (pdf_char_t *in, 
                        pdf_stm_pos_t in_size,
                        pdf_char_t **out,
                        pdf_stm_pos_t *out_size)
{
  *out = (pdf_char_t *) xmalloc (1);

  return PDF_FALSE;
}

static int
pdf_stm_f_flate_decode (pdf_char_t *in,
                        pdf_stm_pos_t in_size,
                        pdf_char_t **out,
                        pdf_stm_pos_t *out_size)
{
  *out = (pdf_char_t *) xmalloc (1);

  return PDF_FALSE;
}


/* End of pdf_stm_f_flate.c */

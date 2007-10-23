/* -*- mode: C -*- Time-stamp: "07/10/23 12:54:30 jemarch"
 *
 *       File:         pdf_stm_f_fax.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Sun Aug  5 19:50:33 2007
 *
 *       GNU PDF Library - CCITT Fax encoder/decoder filter
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

#include <config.h>
#include <malloc.h>
#include <xalloc.h>
#include <string.h>
#include <pdf_stm_f_fax.h>

static int pdf_stm_f_fax_encode (pdf_stm_f_fax_data_t data,
                                 pdf_char_t *in, pdf_stm_pos_t in_size,
                                 pdf_char_t **out, pdf_stm_pos_t *out_size);
static int pdf_stm_f_fax_decode (pdf_stm_f_fax_data_t data,
                                 pdf_char_t *in, pdf_stm_pos_t in_size,
                                 pdf_char_t **out, pdf_stm_pos_t *out_size);

int
pdf_stm_f_fax_init (void **filter_data,
                    void *conf_data)
{
  pdf_stm_f_fax_data_t *data;
  pdf_stm_f_fax_conf_t conf;

  data = (pdf_stm_f_fax_data_t *) filter_data;
  conf = (pdf_stm_f_fax_conf_t) conf_data;

  /* Create the private data storage */
  *data =
    (pdf_stm_f_fax_data_t) xmalloc (sizeof(struct pdf_stm_f_fax_data_s));
  (*data)->mode = conf->mode;
  (*data)->k = conf->k;
  (*data)->end_of_line_p = conf->end_of_line_p;
  (*data)->encoded_byte_align_p = conf->encoded_byte_align_p;
  (*data)->columns = conf->columns;
  (*data)->rows = conf->rows;
  (*data)->end_of_block_p = conf->end_of_block_p;
  (*data)->blackls1_p = conf->blackls1_p;
  (*data)->damaged_rows_before_error = conf->damaged_rows_before_error;

  return PDF_OK;
}

int
pdf_stm_f_fax_apply (void *filter_data,
                     pdf_char_t *in, pdf_stm_pos_t in_size,
                     pdf_char_t **out, pdf_stm_pos_t *out_size)
{
  pdf_stm_f_fax_data_t data;

  data = (pdf_stm_f_fax_data_t) filter_data;
  switch (data->mode)
    {
    case PDF_STM_F_FAX_MODE_ENCODE:
      {
        return pdf_stm_f_fax_encode (data, in, in_size, out, out_size);
      }
    case PDF_STM_F_FAX_MODE_DECODE:
      {
        return pdf_stm_f_fax_decode (data, in, in_size, out, out_size);
      }
    default:
      {
        return PDF_ERROR;
      }
    }

  /* Not reached */
}

int
pdf_stm_f_fax_dealloc (void **filter_data)
{
  pdf_stm_f_fax_data_t *data;

  data = (pdf_stm_f_fax_data_t *) filter_data;
  free (*data);

  return PDF_OK;
}

/* Private functions */

static int
pdf_stm_f_fax_encode (pdf_stm_f_fax_data_t data,
                      pdf_char_t *in,
                      pdf_stm_pos_t in_size,
                      pdf_char_t **out,
                      pdf_stm_pos_t *out_size)
{
  *out = (pdf_char_t *) xmalloc (in_size);
  memcpy (*out,
          in,
          in_size);
  *out_size = in_size;

  return PDF_OK;
}

static int
pdf_stm_f_fax_decode (pdf_stm_f_fax_data_t data,
                      pdf_char_t *in,
                      pdf_stm_pos_t in_size,
                      pdf_char_t **out,
                      pdf_stm_pos_t *out_size)
{
  *out = (pdf_char_t *) xmalloc (in_size);
  memcpy (*out,
          in,
          in_size);
  *out_size = in_size;

  return PDF_OK;
}

/* End of pdf_stm_f_fax.c */

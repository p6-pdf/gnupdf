/* -*- mode: C -*- Time-stamp: "07/07/11 00:21:06 jemarch"
 *
 *       File:         pdf_stm_f_fdec.c
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

#include <malloc.h>
#include <xalloc.h>
#include <string.h>
#include <zlib.h>
#include <pdf_base.h>
#include <pdf_stm_f_fdec.h>

int
pdf_stm_f_fdec_init (void **filter_data,
                     void *conf_data)
{
  pdf_stm_f_fdec_data_t *data;
  data = (pdf_stm_f_fdec_data_t *) filter_data;

  /* Create the private data storage */
  *data =
    (pdf_stm_f_fdec_data_t) xmalloc (sizeof(struct pdf_stm_f_fdec_data_s));
  (*data)->dummy = 0;

  return PDF_OK;
}

int
pdf_stm_f_fdec_apply (void *filter_data,
                      char *in, pdf_stm_pos_t in_size,
                      char **out, pdf_stm_pos_t *out_size)
{
  pdf_stm_f_fdec_data_t data;
  z_stream zstm;
  int ret;

  data = (pdf_stm_f_fdec_data_t) filter_data;

  /*
  zstm.zalloc = Z_NULL;
  zstm.zfree = Z_NULL;
  zstm.opaque = Z_NULL;
  zstm.avail_in = 0;
  zstm.next_in = Z_NULL;
  ret = inflateInit (&zstm);
  if (ret != Z_OK)
    {
      goto error;
    }

  *out = (char *) xmalloc (in_size);
  
  zstm.avail_in = in_size;
  zstm.avail_out = in_size;
  zstm.next_out = out;
  inflate (&zstm, Z_NO_FLUSH);
  */
  
 error:
  /* Clone input buffer */
  *out = (char *) xmalloc (in_size);
  memcpy (*out,
          in,
          in_size);
  *out_size = in_size;
  
 success:

  return PDF_OK;
}

int
pdf_stm_f_fdec_dealloc (void **filter_data)
{
  pdf_stm_f_fdec_data_t *data;

  data = (pdf_stm_f_fdec_data_t *) filter_data;
  free (*data);

  return PDF_OK;
}


/* End of pdf_stm_f_fdec.c */

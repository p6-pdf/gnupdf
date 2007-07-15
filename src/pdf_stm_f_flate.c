/* -*- mode: C -*- Time-stamp: "07/07/15 05:52:18 jemarch"
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
  unsigned long compressed_bound;
  int ret;

  /* Allocate memory for destination buffer */
  compressed_bound = compressBound (in_size);
  *out_size = compressed_bound;
  *out = (pdf_char_t *) xmalloc (*out_size);

  /* Compress input */
  ret = compress (*out, 
                  (unsigned long *) out_size,
                  in,
                  in_size);

  if (ret == Z_OK)
    {
      /* Adjust memory to really used  and return */
      *out = (pdf_char_t *) xrealloc (*out,
                                      *out_size);
      return PDF_OK;
    }
  else
    {
      /* Z_MEM_ERROR or Z_BUF_ERROR happened.  In any case, return
         reporting that the filter application failed. */
      return PDF_FALSE;
    }

  /* Not reached */
}

#define CHUNK 16384

static int
pdf_stm_f_flate_decode (pdf_char_t *in,
                        pdf_stm_pos_t in_size,
                        pdf_char_t **out,
                        pdf_stm_pos_t *out_size)
{
  z_stream zstm;
  int ret;
  pdf_char_t out_aux[16384];
  pdf_stm_pos_t nchunks;

  zstm.zalloc = Z_NULL;
  zstm.zfree = Z_NULL;
  zstm.opaque = Z_NULL;
  zstm.avail_in = in_size;
  zstm.next_in = in;

  inflateInit (&zstm);

  *out_size = 0;
  *out = NULL;
  nchunks = 0;
  do
    {
      zstm.avail_out = CHUNK;
      zstm.next_out = out_aux;
      
      ret = inflate (&zstm, Z_NO_FLUSH);

      switch (ret)
        {
        case Z_NEED_DICT:
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
          {
            goto error;
          }
        }
          
      *out_size =  *out_size + (CHUNK - zstm.avail_out);
      *out = (pdf_char_t *) xrealloc (*out,
                                      *out_size);

      memcpy (*out + (nchunks * CHUNK),
              out_aux,
              CHUNK - zstm.avail_out);

      nchunks++;

    } while (ret != Z_STREAM_END);

  ret = inflateEnd (&zstm);
  
  return PDF_OK;

 error:
  (void) inflateEnd (&zstm);
  return PDF_ERROR;
}


/* End of pdf_stm_f_flate.c */

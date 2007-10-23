/* -*- mode: C -*- Time-stamp: "07/10/23 12:56:24 jemarch"
 *
 *       File:         pdf_stm_f_rl.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Sun Jul 15 22:01:18 2007
 *
 *       GNU PDF Library - RunLength encoder/decoder
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
#include <stdio.h>
#include <pdf_stm_f_rl.h>

static int pdf_stm_f_rl_encode (pdf_char_t *in, pdf_stm_pos_t in_size,
                                pdf_char_t **out, pdf_stm_pos_t *out_size);
static int pdf_stm_f_rl_decode (pdf_char_t *in, pdf_stm_pos_t in_size,
                                pdf_char_t **out, pdf_stm_pos_t *out_size);

int
pdf_stm_f_rl_init (void **filter_data,
                   void *conf_data)
{
  pdf_stm_f_rl_data_t *data;
  pdf_stm_f_rl_conf_t conf;

  data = (pdf_stm_f_rl_data_t *) filter_data;
  conf = (pdf_stm_f_rl_conf_t) conf_data;

  /* Create the private data storage */
  *data =
    (pdf_stm_f_rl_data_t) xmalloc (sizeof(struct pdf_stm_f_rl_data_s));
  (*data)->mode = conf->mode;

  return PDF_OK;
}

int
pdf_stm_f_rl_apply (void *filter_data,
                    pdf_char_t *in, pdf_stm_pos_t in_size,
                    pdf_char_t **out, pdf_stm_pos_t *out_size)
{
  pdf_stm_f_rl_data_t data;

  data = (pdf_stm_f_rl_data_t) filter_data;
  switch (data->mode)
    {
    case PDF_STM_F_RL_MODE_ENCODE:
      {
        return pdf_stm_f_rl_encode (in, in_size, out, out_size);
        
      }
    case PDF_STM_F_RL_MODE_DECODE:
      {
        return pdf_stm_f_rl_decode (in, in_size, out, out_size);
      }
    default:
      {
        return PDF_ERROR;
      }
    }

  /* Not reached */
}

int
pdf_stm_f_rl_dealloc (void **filter_data)
{
  pdf_stm_f_rl_data_t *data;

  data = (pdf_stm_f_rl_data_t *) filter_data;
  free (*data);

  return PDF_OK;
}

/* Private functions */

static int
pdf_stm_f_rl_encode (pdf_char_t *in,
                     pdf_stm_pos_t in_size,
                     pdf_char_t **out,
                     pdf_stm_pos_t *out_size)
{
  pdf_stm_pos_t in_pos;
  pdf_stm_pos_t out_pos;
  pdf_stm_pos_t run_length;
  pdf_char_t run_char;
  int run_p;
  pdf_char_t c;

  /* The compression achieved by run-length encoding depends on the
     input data. The worst case (the hexadecimal sequence `00'
     alternating with `FF') results in an expansion of 127:128 

     But take care about the EOD marker (one octect). */
  *out_size = (in_size * 2) + (in_size / 127) + 1;
  *out = (pdf_char_t *) xmalloc (*out_size);

  out_pos = 0;
  run_length = 0;
  run_char = 0;
  run_p = PDF_FALSE;
  in_pos = 0;
  while (in_pos < in_size)
    {
      c = in[in_pos];

      if (!run_p)
        {
          run_char = c;
          run_p = PDF_TRUE;
        }
     
      if ((c == run_char) &&
          (run_length < 128))
        {
          run_length++;
          in_pos++;
        }
      else
        {
          (*out)[out_pos++] = run_length;
          (*out)[out_pos++] = run_char;
          run_length = 0;
          run_p = PDF_FALSE;
        }
    }

  /* Insert EOD marker */
  (*out)[out_pos++] = 128;

  *out_size = out_pos;
  *out = (pdf_char_t *) xrealloc (*out,
                                  *out_size);

  return PDF_OK;
}

static int
pdf_stm_f_rl_decode (pdf_char_t *in,
                     pdf_stm_pos_t in_size,
                     pdf_char_t **out,
                     pdf_stm_pos_t *out_size)
{
  pdf_stm_pos_t in_pos;
  pdf_stm_pos_t out_pos;
  pdf_stm_pos_t run_length;
  pdf_stm_pos_t i;
  pdf_char_t c;

  /* In the best case (all zeros), a compression of approximately 64:1
     is achieved for long files. */
  *out_size = in_size * 64;
  *out = (pdf_char_t *) xmalloc (*out_size);

  out_pos = 0;
  in_pos = 0;
  while (in_pos < in_size)
    {
      c = in[in_pos];

      if (c == 128)
        {
          /* EOD marker */
          break;
        }
      if (c <= 127)
        {
          run_length = c;
          if ((in_pos + run_length) >= in_size)
            {
              /* Not enough input */
              goto error;
            }

          for (i = 0;
               i < c;
               i++)
            {
              (*out)[out_pos++] = in[in_pos + run_length];
            }
          in_pos = in_pos + run_length;
        }
      if (c >= 129)
        {
          run_length = c;
          if ((in_pos + 1) >= in_size)
            {
              goto error;
            }

          for (i = 0;
               i < (287 - run_length);
               i++)
            {
              (*out)[out_pos++] = in[in_pos + 1];
            }
          in_pos = in_pos + 1;
        }
    }

  /* Adjust output buffer */
  *out_size = out_pos;
  *out = (pdf_char_t *) xrealloc (*out, *out_size);

  return PDF_OK;

 error:
  free (*out);
  return PDF_ERROR;
}

/* End of pdf_stm_f_rl.c */

/* -*- mode: C -*- Time-stamp: "2008-09-28 20:18:38 gerel"
 *
 *       File:         pdf-stm-f-rl.c
 *       Date:         Sun Jul 15 22:01:18 2007
 *
 *       GNU PDF Library - RunLength encoder/decoder
 *
 */

/* Copyright (C) 2007, 2008 Free Software Foundation, Inc. */

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


#include <string.h>
#include <stdio.h>
#include <pdf-alloc.h>
#include <pdf-stm-f-rl.h>


static int encode_rl_char (pdf_stm_f_rl_t st, pdf_stm_buffer_t out);
static int decode_rl_char (pdf_stm_f_rl_t st, pdf_stm_buffer_t out);
static int copy_next_bytes (pdf_stm_f_rl_t st, pdf_stm_buffer_t in,
                            pdf_stm_buffer_t out);


pdf_status_t
pdf_stm_f_rlenc_init (pdf_hash_t params, void **state)
{
  pdf_status_t ret;
  pdf_stm_f_rl_t filter_state;

  filter_state = pdf_alloc (sizeof (struct pdf_stm_f_rl_s));

  if (state == NULL)
    {
      ret = PDF_EBADDATA;
    }
  else if (filter_state == NULL)
    {
      ret = PDF_ENOMEM;
    }
  else
    {
      /* Initialize fields */
      filter_state->curchar = 0;
      filter_state->rlchar = 0;
      filter_state->run_p = PDF_FALSE;
      filter_state->rl = -1;
      filter_state->dec_p = PDF_FALSE;;
      filter_state->dec_count = 0;
      filter_state->enc_p = PDF_STM_F_RL_NONE;

      *state = (void *) filter_state;
      ret = PDF_OK;
    }

  return ret;
}


pdf_status_t
pdf_stm_f_rldec_init (pdf_hash_t params, void **state)
{
  return pdf_stm_f_rlenc_init (params,state);
}


pdf_status_t
pdf_stm_f_rlenc_apply (pdf_hash_t params, void * state, pdf_stm_buffer_t in,
                       pdf_stm_buffer_t out, pdf_bool_t finish_p)
{
  pdf_stm_f_rl_t st;

  st = (pdf_stm_f_rl_t) state;

  while (!pdf_stm_buffer_eob_p (in))
    {
      st->curchar = in->data[in->rp];

      /* we're not encoding any character yet */
      if (!st->run_p)
        {
          st->rlchar = st->curchar;
          st->run_p = PDF_TRUE;
          st->rl++; 
          in->rp++;
        }
      /* we're encoding some character now */
      else if (st->curchar == st->rlchar && st->rl < 127)
        {
          st->rl++;
          in->rp++;
        }
      /* 
       * the rl code is too long or the rl char is different,
       * so we write what we encoded so far.
       */
      else
        {
          if (encode_rl_char (st, out) < 0)
            {
              return PDF_OK;
            }
          st->rl=-1;
          st->run_p = PDF_FALSE;
        }
    }

  /* 
   * we may have finished with some history, we save it if needed,
   * then we add the EOD.
   */
  if (finish_p)
    {
      if (st->run_p)
        {
          if (encode_rl_char (st, out) < 0)
            {
              /* Should not be reached */
              return PDF_ERROR;
            }
          st->rl=-1;
          st->run_p = PDF_FALSE;
        }
      if (pdf_stm_buffer_full_p (out))
        {
          /* Should not be reached */
          return PDF_ERROR;
        }
      /* Insert EOD marker */
      out->data[out->wp++] = 128;
    }

  if (pdf_stm_buffer_eob_p (in))
    {
      return PDF_EEOF;
    }
  return PDF_OK;
}


pdf_status_t
pdf_stm_f_rlenc_dealloc_state (void *state)
{
  pdf_dealloc (state);
  return PDF_OK;
}

pdf_status_t
pdf_stm_f_rldec_apply (pdf_hash_t params, void *state, pdf_stm_buffer_t in,
                       pdf_stm_buffer_t out, pdf_bool_t finish_p)
{
  pdf_stm_f_rl_t st;
  pdf_status_t copied;

  st = (pdf_stm_f_rl_t) state;

  while (!pdf_stm_buffer_eob_p (in))
    {
      st->curchar = in->data[in->rp];

      /* we're not decoding any character yet */
      if (!st->run_p)
        {
          st->rlchar = st->curchar;
          st->run_p = PDF_TRUE;
          in->rp++;
        }
      /* copy the following 1 to 128 bytes literally */
      else if (st->rlchar < 128)
        {
          copied = copy_next_bytes (st, in, out);
          if (copied < 0)
            {
              return PDF_OK;
            }
          else if (copied > 0)
            {
              return PDF_EEOF;
            }
          st->run_p = PDF_FALSE;
        }
      /* copy the next char 257 - length (2 to 128) times */
      else if (st->rlchar > 128)
        {
          if (decode_rl_char (st, out) < 0)
            {
              return PDF_OK;
            }
          st->run_p = PDF_FALSE;
          in->rp++;
        }
      /* EOD mark */
      else
        {
          st->run_p = PDF_FALSE;
        }
    }

  if (pdf_stm_buffer_eob_p (in))
    {
      return PDF_EEOF;
    }

  return PDF_OK;
}

pdf_status_t
pdf_stm_f_rldec_dealloc_state (void *state)
{
  pdf_dealloc (state);
  return PDF_OK;
}

/* Private functions */

static int
encode_rl_char (pdf_stm_f_rl_t st, pdf_stm_buffer_t out)
{
  if (st->enc_p == PDF_STM_F_RL_NONE)
    {
      if (pdf_stm_buffer_full_p (out))
        {
          return -1;
        }
      out->data[out->wp++] = (st->rl == 0) ? 0 : 256 - st->rl;    
      st->enc_p = PDF_STM_F_RL_WRL;
    }

  if (st->enc_p == PDF_STM_F_RL_WRL)
    {
      if (pdf_stm_buffer_full_p (out))
        {
          return -1;
        }
      out->data[out->wp++] = st->rlchar;
      st->enc_p = PDF_STM_F_RL_NONE;
    }

  return 0;
}


static int
decode_rl_char (pdf_stm_f_rl_t st, pdf_stm_buffer_t out)
{
  if (!st->dec_p)
    {
      st->dec_count = 257 - st->rlchar;
      st->dec_p = PDF_TRUE;
    }

  while (st->dec_count > 0)
    {
      if (pdf_stm_buffer_full_p (out))
        {
          return -1;
        }
      out->data[out->wp++] = st->curchar;
      st->dec_count--;
    }

  st->dec_p = PDF_FALSE;
  return 0;
}


static int
copy_next_bytes (pdf_stm_f_rl_t st, pdf_stm_buffer_t in, pdf_stm_buffer_t out)
{
  if (!st->dec_p)
    {
      st->dec_count = st->rlchar + 1;
      st->dec_p = PDF_TRUE;
      out->data[out->wp++] = st->curchar;
      in->rp++;
      st->dec_count--;
    }

  while (st->dec_count > 0)
    {
      if (pdf_stm_buffer_eob_p (in))
        {
          return 1;
        }
      else if (pdf_stm_buffer_full_p (out))
        {
          return -1;
        }
      out->data[out->wp] = in->data[in->rp];
      out->wp++;
      in->rp++;
      st->dec_count--;
    }

  st->dec_p = PDF_FALSE;
  return 0;
}


/* End of pdf_stm_f_rl.c */

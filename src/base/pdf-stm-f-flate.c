/* -*- mode: C -*- Time-stamp: "09/01/11 22:02:58 jemarch"
 *
 *       File:         pdf-stm-f-flate.c
 *       Date:         Tue Jul 10 23:44:00 2007
 *
 *       GNU PDF Library - FlateDecode stream filter
 *
 */

/* Copyright (C) 2007, 2008, 2009 Free Software Foundation, Inc. */

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


#include <stdio.h>
#include <string.h>
#include <pdf-alloc.h>
#include <pdf-hash.h>
#include <pdf-stm-f-flate.h>


static pdf_status_t read_and_inflate (pdf_stm_f_flate_t st, pdf_buffer_t in,
                                      pdf_buffer_t out);
static pdf_status_t read_and_deflate (pdf_stm_f_flate_t st, pdf_buffer_t in,
                                      pdf_buffer_t out,
                                      pdf_bool_t finish_p);
static int deflate_inbuf (pdf_stm_f_flate_t st, pdf_buffer_t out,
                          int flush);
static pdf_status_t deflate_inbuf_return (pdf_stm_f_flate_t st,
                                          pdf_buffer_t out,
                                          pdf_bool_t finish_p);

pdf_status_t
pdf_stm_f_flateenc_init (pdf_hash_t params, void **state)
{
  pdf_status_t ret;
  pdf_stm_f_flate_t filter_state;

  filter_state = pdf_alloc (sizeof (struct pdf_stm_f_flate_s));

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
      filter_state->stream.zalloc = Z_NULL;
      filter_state->stream.zfree = Z_NULL;
      filter_state->stream.opaque = Z_NULL;
      filter_state->writing_p = PDF_FALSE;
      filter_state->to_write = 0;
      filter_state->incnt = 0;
      filter_state->outcnt = 0;
      filter_state->zret = Z_OK;

      if (deflateInit (&(filter_state->stream), Z_DEFAULT_COMPRESSION) != Z_OK)
        {
          ret = PDF_ERROR;
        }
      else
        {
          *state = (void *) filter_state;
          ret = PDF_OK;
        }
    }

  return ret;
}


pdf_status_t
pdf_stm_f_flatedec_init (pdf_hash_t params, void **state)
{
  pdf_status_t ret;
  pdf_stm_f_flate_t filter_state;

  filter_state = pdf_alloc (sizeof (struct pdf_stm_f_flate_s));

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
      filter_state->stream.zalloc = Z_NULL;
      filter_state->stream.zfree = Z_NULL;
      filter_state->stream.opaque = Z_NULL;
      filter_state->stream.avail_in = 0;
      filter_state->stream.next_in = Z_NULL;
      filter_state->writing_p = PDF_FALSE;
      filter_state->to_write = 0;
      filter_state->incnt = 0;
      filter_state->outcnt = 0;
      filter_state->zret = Z_OK;

      if (inflateInit (&(filter_state->stream)) != Z_OK)
        {
          ret = PDF_ERROR;
        }
      else
        {
          *state = (void *) filter_state;
          ret = PDF_OK;
        }
    }

  return ret;

}


pdf_status_t
pdf_stm_f_flateenc_apply (pdf_hash_t params, void *state, pdf_buffer_t in,
                          pdf_buffer_t out, pdf_bool_t finish_p)
{
  pdf_stm_f_flate_t st;
  pdf_status_t ret;
  st = (pdf_stm_f_flate_t) state;

  ret = read_and_deflate (st, in, out, finish_p);
  while (ret == PDF_OK)
    {
      ret = read_and_deflate (st, in, out, finish_p);
    }
  return ret;
}


pdf_status_t
pdf_stm_f_flatedec_apply (pdf_hash_t params, void *state, pdf_buffer_t in,
                          pdf_buffer_t out, pdf_bool_t finish_p)
{
  pdf_stm_f_flate_t st;
  pdf_status_t ret;
  st = (pdf_stm_f_flate_t) state;
  
  ret = read_and_inflate (st, in, out);
  while (ret == PDF_OK)
    {
      ret = read_and_inflate (st, in, out);
    }
  return ret;
}



pdf_status_t
pdf_stm_f_flatedec_dealloc_state (void *state)
{
  pdf_stm_f_flate_t st = state;
  inflateEnd(&(st->stream));
  pdf_dealloc (state);
  return PDF_OK;
}

pdf_status_t
pdf_stm_f_flateenc_dealloc_state (void *state)
{
  pdf_dealloc (state);
  return PDF_OK;
}


/* Private functions */


static pdf_status_t read_and_deflate (pdf_stm_f_flate_t st, pdf_buffer_t in,
                                      pdf_buffer_t out, pdf_bool_t finish_p)
{

  /* Fill the input CHUNK  */
  if (!st->writing_p)
    {
      while (st->incnt < PDF_STM_F_FLATE_CHUNK && !pdf_buffer_eob_p(in))
        {
          st->inbuf[st->incnt] = in->data[in->rp];
          st->incnt++;
          in->rp++;
        }
      /* If more data may come and the input CHUNK has space, ask for it. */
      if (!finish_p && st->incnt < PDF_STM_F_FLATE_CHUNK)
        {
          return PDF_ENINPUT;
        }
    }
    
  /* 
   * Now we have the input CHUNK full or finish_p is set,
   * we deflate and write to out.
   */ 
  return (deflate_inbuf_return (st, out, finish_p));
}


static pdf_status_t read_and_inflate (pdf_stm_f_flate_t st, pdf_buffer_t in,
                                      pdf_buffer_t out)
{
  /* Fill the input CHUNK */
  if (!st->writing_p)
    {
      while (st->incnt < PDF_STM_F_FLATE_CHUNK && !pdf_buffer_eob_p(in))
        {
          st->inbuf[st->incnt] = in->data[in->rp];
          st->incnt++;
          in->rp++;
        }
    }
  else
    {
      /*
       * Not nice, but keeps the writing process code clear.
       * Notice that the labeled code is inside a while loop,
       * so I feel that avoiding this goto won't bring us better code.
       */
      goto writing;
    }

  if (st->incnt == 0)
    {
      return PDF_ENINPUT;
    }

  /* we inflate and write to out */
  st->stream.avail_in = st->incnt;
  st->stream.next_in = st->inbuf;
  do {
    st->stream.avail_out = PDF_STM_F_FLATE_CHUNK;
    st->stream.next_out = st->outbuf;
    st->outcnt = 0;

    st->zret = inflate(&(st->stream), Z_NO_FLUSH);
    if (st->zret == Z_STREAM_ERROR || st->zret == Z_NEED_DICT ||
        st->zret == Z_DATA_ERROR || st->zret == Z_MEM_ERROR)
      {
        /* should not be reached */
        inflateEnd(&(st->stream));
        return PDF_ERROR;
      }

    st->to_write = PDF_STM_F_FLATE_CHUNK - st->stream.avail_out;

  writing:
    while (st->outcnt < st->to_write && !pdf_buffer_full_p(out))
      {
        out->data[out->wp] = st->outbuf[st->outcnt];
        out->wp++;
        st->outcnt++;
      }
    if (pdf_buffer_full_p(out))
      {
        st->writing_p = PDF_TRUE;
        return PDF_ENOUTPUT;
      }
  } while (st->stream.avail_out == 0);

  if (st->zret == Z_STREAM_END)
    {
      return PDF_EEOF;
    }
  /* the input CHUNK now is empty, if needed, ask for input */
  st->writing_p = PDF_FALSE;
  st->incnt = 0;
  if (pdf_buffer_eob_p(in))
    {
      return PDF_ENINPUT;
    }

  /* ask for the input we couldn't read */
  return PDF_OK;
}


static int
deflate_inbuf (pdf_stm_f_flate_t st, pdf_buffer_t out, int flush)
{
  if (st->writing_p)
    {
      /* 
       * Not nice, but keeps the writing process code clear.
       * Notice that the labeled code is inside a while loop,
       * so I feel that avoiding this goto won't bring us better code.
       */
      goto writing;
    }

  st->stream.avail_in = st->incnt;
  st->stream.next_in = st->inbuf;
  do {
    st->stream.avail_out = PDF_STM_F_FLATE_CHUNK;
    st->stream.next_out = st->outbuf;
    st->outcnt = 0;

    st->zret = deflate(&(st->stream), flush);
    if (st->zret == Z_STREAM_ERROR)
      {
        /* should not be reached */
        deflateEnd (&(st->stream));
        return -1;
      }

    st->to_write = PDF_STM_F_FLATE_CHUNK - st->stream.avail_out;

  writing:

    while (st->outcnt < st->to_write && !pdf_buffer_full_p(out))
      {
        out->data[out->wp++] = st->outbuf[st->outcnt];
        st->outcnt++;
      }
    if (pdf_buffer_full_p(out))
      {
        st->writing_p = PDF_TRUE;
        return 1;
      }
  } while (st->stream.avail_out == 0);

  st->writing_p = PDF_FALSE;
  return 0;
}


static pdf_status_t
deflate_inbuf_return (pdf_stm_f_flate_t st, pdf_buffer_t out,
                      pdf_bool_t finish_p)
{
  int ret;

  if (finish_p)
    {
      ret = deflate_inbuf(st, out, Z_FINISH);
    }
  else
    {
      ret = deflate_inbuf(st, out, Z_NO_FLUSH);
    }

  if (ret < 0)
    {
      return PDF_ERROR;
    }
  else if (ret > 0)
    {
      return PDF_ENOUTPUT;
    }
  else if (finish_p)
    {
      deflateEnd (&(st->stream));
      return PDF_EEOF;
    }
  else
    {
      /* the input CHUNK now is empty */
      st->incnt = 0;
      /* ask for the input we couldn't read */
      return PDF_OK;
    }
}


/* End of pdf_stm_f_flate.c */

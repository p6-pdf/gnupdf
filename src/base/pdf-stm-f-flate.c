/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-flate.c
 *       Date:         Tue Jul 10 23:44:00 2007
 *
 *       GNU PDF Library - FlateDecode stream filter
 *
 */

/* Copyright (C) 2007-2011 Free Software Foundation, Inc. */

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
#include <string.h>

#include <zlib.h>

#include <pdf-alloc.h>
#include <pdf-hash.h>
#include <pdf-stm-f-flate.h>
#include <pdf-types.h>
#include <pdf-types-buffer.h>
#include <pdf-hash.h>

/* Define FLATE encoder */
PDF_STM_FILTER_DEFINE (pdf_stm_f_flateenc_get,
                       stm_f_flateenc_init,
                       stm_f_flateenc_apply,
                       stm_f_flateenc_deinit);

/* Define FLATE decoder */
PDF_STM_FILTER_DEFINE (pdf_stm_f_flatedec_get,
                       stm_f_flatedec_init,
                       stm_f_flatedec_apply,
                       stm_f_flatedec_deinit);

#define PDF_STM_F_FLATE_CHUNK 16384

struct pdf_stm_f_flate_s
{
  z_stream stream;
  int zret;
  pdf_size_t incnt;
  pdf_size_t outcnt;
  pdf_size_t to_write;
  pdf_bool_t writing;
  pdf_char_t inbuf[PDF_STM_F_FLATE_CHUNK];
  pdf_char_t outbuf[PDF_STM_F_FLATE_CHUNK];
};

/* Common implementation */

static pdf_bool_t
stm_f_flate_init (void        **state,
                  pdf_error_t **error)
{
  struct pdf_stm_f_flate_s *filter_state;

  /* Allocate the internal state structure */
  filter_state = pdf_alloc (sizeof (struct pdf_stm_f_flate_s));
  if (!filter_state)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "cannot create FLATE encoder/decoder internal state: "
                     "couldn't allocate %lu bytes",
                     (unsigned long)sizeof (struct pdf_stm_f_flate_s));
      return PDF_FALSE;
    }

  /* Initialize common fields */
  filter_state->stream.zalloc = Z_NULL;
  filter_state->stream.zfree = Z_NULL;
  filter_state->stream.opaque = Z_NULL;
  filter_state->stream.next_in = Z_NULL;
  filter_state->stream.avail_in = 0;
  filter_state->writing = PDF_FALSE;
  filter_state->to_write = 0;
  filter_state->incnt = 0;
  filter_state->outcnt = 0;
  filter_state->zret = Z_OK;

  *state = filter_state;

  return PDF_TRUE;
}

static void
stm_f_flate_deinit (void *state)
{
  pdf_dealloc (state);
}

static void
set_error_from_zlib_ret (pdf_error_t **error,
                         int           zlib_ret)
{
  const pdf_char_t *errmsg;

  switch (zlib_ret)
    {
    case Z_STREAM_ERROR:
      errmsg = "zlib error detected: invalid compression level";
      break;
    case Z_NEED_DICT:
      errmsg = "zlib error detected: need dictionary";
      break;
    case Z_DATA_ERROR:
      errmsg = "zlib error detected: invalid or incomplete deflate data";
      break;
    case Z_MEM_ERROR:
      errmsg = "zlib error detected: out of memory";
      break;
    default:
      errmsg = "zlib error detected";
      break;
    }

  pdf_set_error (error,
                 PDF_EDOMAIN_BASE_STM,
                 PDF_ERROR,
                 errmsg);
}

/* Encoder implementation */

static pdf_bool_t
stm_f_flateenc_init (const pdf_hash_t  *params,
                     void             **state,
                     pdf_error_t      **error)
{
  struct pdf_stm_f_flate_s *filter_state;

  /* Initialize common stuff */
  if (!stm_f_flate_init (state, error))
    return PDF_FALSE;

  filter_state = *state;
  if (deflateInit (&(filter_state->stream),
                   Z_DEFAULT_COMPRESSION) != Z_OK)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ERROR,
                     "cannot create FLATE encoder internal state: "
                     "couldn't initialize deflate");
      stm_f_flate_deinit (filter_state);
      return PDF_FALSE;
    }

  return PDF_TRUE;
}

static void
stm_f_flateenc_deinit (void *state)
{
  struct pdf_stm_f_flate_s *filter_state = state;

  deflateEnd (&(filter_state->stream));
  stm_f_flate_deinit (state);
}

static enum pdf_stm_filter_apply_status_e
deflate_inbuf (struct pdf_stm_f_flate_s  *st,
               pdf_buffer_t              *out,
               pdf_bool_t                 finish,
               pdf_error_t              **error)
{
  if (st->writing)
    {
      /*
       * Not nice, but keeps the writing process code clear.
       * Notice that the labeled code is inside a while loop,
       * so I feel that avoiding this goto won't bring us better code.
       */
      goto writing;
    }

  st->stream.avail_in = st->incnt;
  st->stream.next_in = (Bytef *) st->inbuf;
  do {
    st->stream.avail_out = PDF_STM_F_FLATE_CHUNK;
    st->stream.next_out = (Bytef *) st->outbuf;
    st->outcnt = 0;

    st->zret = deflate (&(st->stream),
                        (finish ? Z_FINISH : Z_NO_FLUSH));
    if (st->zret == Z_STREAM_ERROR)
      {
        set_error_from_zlib_ret (error, st->zret);
        return PDF_STM_FILTER_APPLY_STATUS_ERROR;
      }

    st->to_write = PDF_STM_F_FLATE_CHUNK - st->stream.avail_out;

  writing:

    while (st->outcnt < st->to_write &&
           !pdf_buffer_full_p (out))
      {
        out->data[out->wp++] = st->outbuf[st->outcnt];
        st->outcnt++;
      }

    if (pdf_buffer_full_p (out))
      {
        st->writing = PDF_TRUE;
        return PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT;
      }
  } while (st->stream.avail_out == 0);

  st->writing = PDF_FALSE;

  if (finish)
    {
      return PDF_STM_FILTER_APPLY_STATUS_EOF;
    }
  else
    {
      /* the input CHUNK now is empty */
      st->incnt = 0;
      /* ask for the input we couldn't read */
      return PDF_STM_FILTER_APPLY_STATUS_OK;
    }
}

static enum pdf_stm_filter_apply_status_e
read_and_deflate (struct pdf_stm_f_flate_s  *st,
                  pdf_buffer_t              *in,
                  pdf_buffer_t              *out,
                  pdf_bool_t                 finish,
                  pdf_error_t              **error)
{
  /* Fill the input CHUNK  */
  if (!st->writing)
    {
      while (st->incnt < PDF_STM_F_FLATE_CHUNK &&
             !pdf_buffer_eob_p (in))
        {
          st->inbuf[st->incnt] = in->data[in->rp];
          st->incnt++;
          in->rp++;
        }

      /* If more data may come and the input CHUNK has space, ask for it. */
      if (!finish &&
          st->incnt < PDF_STM_F_FLATE_CHUNK)
        {
          return PDF_STM_FILTER_APPLY_STATUS_NO_INPUT;
        }
    }

  /*
   * Now we have the input CHUNK full or finish_p is set,
   * we deflate and write to out.
   */
  return deflate_inbuf (st, out, finish, error);
}

static enum pdf_stm_filter_apply_status_e
stm_f_flateenc_apply (void          *state,
                      pdf_buffer_t  *in,
                      pdf_buffer_t  *out,
                      pdf_bool_t     finish,
                      pdf_error_t  **error)
{
  struct pdf_stm_f_flate_s *st = state;
  enum pdf_stm_filter_apply_status_e ret;

  do
    {
      ret = read_and_deflate (st, in, out, finish, error);
    }
  while (ret == PDF_STM_FILTER_APPLY_STATUS_OK);

  return ret;
}

/* Decoder implementation */

static pdf_bool_t
stm_f_flatedec_init (const pdf_hash_t  *params,
                     void             **state,
                     pdf_error_t      **error)
{
  struct pdf_stm_f_flate_s *filter_state;

  /* Initialize common stuff */
  if (!stm_f_flate_init (state, error))
    return PDF_FALSE;

  filter_state = *state;

  if (inflateInit (&(filter_state->stream)) != Z_OK)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ERROR,
                     "cannot create FLATE decoder internal state: "
                     "couldn't initialize inflate");
      stm_f_flate_deinit (filter_state);
      return PDF_FALSE;
    }

  return PDF_TRUE;
}

static void
stm_f_flatedec_deinit (void *state)
{
  struct pdf_stm_f_flate_s *filter_state = state;

  inflateEnd (&filter_state->stream);
  stm_f_flate_deinit (state);
}

static enum pdf_stm_filter_apply_status_e
read_and_inflate (struct pdf_stm_f_flate_s  *st,
                  pdf_buffer_t              *in,
                  pdf_buffer_t              *out,
                  pdf_error_t              **error)
{
  /* Fill the input CHUNK */
  if (!st->writing)
    {
      while (st->incnt < PDF_STM_F_FLATE_CHUNK &&
             !pdf_buffer_eob_p (in))
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
    return PDF_STM_FILTER_APPLY_STATUS_NO_INPUT;

  /* we inflate and write to out */
  st->stream.avail_in = st->incnt;
  st->stream.next_in = (Bytef *) st->inbuf;
  do {
    st->stream.avail_out = PDF_STM_F_FLATE_CHUNK;
    st->stream.next_out = (Bytef *) st->outbuf;
    st->outcnt = 0;

    st->zret = inflate (&(st->stream),
                        Z_NO_FLUSH);
    if (st->zret == Z_STREAM_ERROR ||
        st->zret == Z_NEED_DICT ||
        st->zret == Z_DATA_ERROR ||
        st->zret == Z_MEM_ERROR)
      {
        set_error_from_zlib_ret (error, st->zret);
        return PDF_STM_FILTER_APPLY_STATUS_ERROR;
      }

    st->to_write = PDF_STM_F_FLATE_CHUNK - st->stream.avail_out;

  writing:

    while (st->outcnt < st->to_write &&
           !pdf_buffer_full_p (out))
      {
        out->data[out->wp] = st->outbuf[st->outcnt];
        out->wp++;
        st->outcnt++;
      }

    if (pdf_buffer_full_p (out))
      {
        st->writing = PDF_TRUE;
        return PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT;
      }
  } while (st->stream.avail_out == 0);

  if (st->zret == Z_STREAM_END)
    return PDF_STM_FILTER_APPLY_STATUS_EOF;

  /* the input CHUNK now is empty, if needed, ask for input */
  st->writing = PDF_FALSE;
  st->incnt = 0;
  if (pdf_buffer_eob_p (in))
    return PDF_STM_FILTER_APPLY_STATUS_NO_INPUT;

  /* ask for the input we couldn't read */
  return PDF_STM_FILTER_APPLY_STATUS_OK;
}

static enum pdf_stm_filter_apply_status_e
stm_f_flatedec_apply (void          *state,
                      pdf_buffer_t  *in,
                      pdf_buffer_t  *out,
                      pdf_bool_t     finish,
                      pdf_error_t  **error)
{
  struct pdf_stm_f_flate_s *st = state;
  enum pdf_stm_filter_apply_status_e ret;

  do
    {
      ret = read_and_inflate (st, in, out, error);
    }
  while (ret == PDF_STM_FILTER_APPLY_STATUS_OK);

  return ret;
}

/* End of pdf_stm_f_flate.c */

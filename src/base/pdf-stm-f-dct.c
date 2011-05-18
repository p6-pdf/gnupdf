/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-dct.c
 *       Date:         Mon Dec 22 22:00:21 2008
 *
 *       GNU PDF Library - DCT encoder/decoder filter
 *
 */

/* Copyright (C) 2009-2011 Free Software Foundation, Inc. */

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

#include <stdio.h>  /* For jpeglib.h */
#include <memory.h> /* For jpeglib.h */
#include <jerror.h> /* For jpeglib.h */
#include <jpeglib.h>

#include <pdf-types.h>
#include <pdf-types-buffer.h>
#include <pdf-hash.h>
#include <pdf-stm-f-dct.h>

#define PPM_MAXVAL 255
#define PDF_DJPEG_CACHE_SIZE (1024)

enum pdf_stm_f_dctdec_state_t
  {
    DCTDEC_STATE_INIT,
    DCTDEC_STATE_CACHE_IN,
    DCTDEC_STATE_READHDR,
    DCTDEC_STATE_STARTDJP,
    DCTDEC_STATE_WRITEHDR,
    DCTDEC_STATE_SCANLINE,
    DCTDEC_STATE_OUTPUTLINE,
    DCTDEC_STATE_FINISHDJP,
    DCTDEC_STATE_ERROR
  };

struct pdf_stm_f_dctdec_s
{
  struct jpeg_decompress_struct *cinfo;
  struct jpeg_error_mgr *jerr;

  enum pdf_stm_f_dctdec_state_t state;
  enum pdf_stm_f_dctdec_state_t backup_state;
  /* filter dictionary */
  pdf_i32_t param_color_transform;

  /* image cache for input data */
  pdf_buffer_t *djpeg_in;

  /* cache for output data */
  pdf_size_t row_stride;
  pdf_uchar_t **row_buf;
  pdf_size_t row_valid_size;
  pdf_size_t row_copy_index;
  pdf_u32_t num_scanlines;
};

static pdf_bool_t stm_f_dctdec_init (const pdf_hash_t  *params,
                                     void             **state,
                                     pdf_error_t      **error);

static void stm_f_dctdec_deinit (void *state);

static enum pdf_stm_filter_apply_status_e stm_f_dctdec_apply (void          *state,
                                                              pdf_buffer_t  *in,
                                                              pdf_buffer_t  *out,
                                                              pdf_bool_t     finish,
                                                              pdf_error_t  **error);

/* Filter implementations */

static const pdf_stm_filter_impl_t dec_impl = {
  .init_fn   = stm_f_dctdec_init,
  .apply_fn  = stm_f_dctdec_apply,
  .deinit_fn = stm_f_dctdec_deinit,
};

const pdf_stm_filter_impl_t *
pdf_stm_f_dctdec_get (void)
{
  return &dec_impl;
}

static const pdf_char_t *DCTDecode_param_name = "ColorTransform";

static pdf_bool_t
stm_f_dctdec_init (const pdf_hash_t  *params,
                   void             **state,
                   pdf_error_t      **error)
{
  struct pdf_stm_f_dctdec_s *filter_state;

  /* Allocate the internal state structure */
  filter_state = pdf_alloc (sizeof (struct pdf_stm_f_dctdec_s));
  if (!filter_state)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "cannot create DCT decoder internal state: "
                     "couldn't allocate %lu bytes",
                     (unsigned long)sizeof (struct pdf_stm_f_dctdec_s));
      return PDF_FALSE;
    }

  memset (filter_state, 0, sizeof (struct pdf_stm_f_dctdec_s));

  filter_state->cinfo = pdf_alloc (sizeof (struct jpeg_decompress_struct));
  if (!filter_state->cinfo)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "cannot create DCT decoder internal state: "
                     "couldn't allocate %lu bytes",
                     (unsigned long)sizeof (struct jpeg_decompress_struct));
      pdf_dealloc (filter_state);
      return PDF_FALSE;
    }

  filter_state->jerr = pdf_alloc (sizeof (struct jpeg_error_mgr));
  if (!filter_state->jerr)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "cannot create DCT decoder internal state: "
                     "couldn't allocate %lu bytes",
                     (unsigned long)sizeof (struct jpeg_error_mgr));
      pdf_dealloc (filter_state->cinfo);
      pdf_dealloc (filter_state);
      return PDF_FALSE;
    }

  filter_state->cinfo->err = jpeg_std_error (filter_state->jerr);
  jpeg_create_decompress (filter_state->cinfo);

  filter_state->param_color_transform = -1;
  if (params &&
      pdf_hash_key_p (params, DCTDecode_param_name))
    {
      pdf_i32_t *ptr;

      ptr = pdf_hash_get_value (params, DCTDecode_param_name);
      filter_state->param_color_transform = *ptr;
    }

  filter_state->state = DCTDEC_STATE_INIT;
  *state = filter_state;

  return PDF_TRUE;
}

static void
stm_f_dctdec_deinit (void *state)
{
  struct pdf_stm_f_dctdec_s *filter_state = state;

  if (filter_state->cinfo)
    {
      jpeg_finish_decompress (filter_state->cinfo);
      filter_state->cinfo->mem->free_pool ((j_common_ptr) filter_state->cinfo,
                                           JPOOL_IMAGE);
      jpeg_destroy_decompress (filter_state->cinfo);

      pdf_buffer_destroy (filter_state->djpeg_in);
      pdf_dealloc (filter_state->cinfo);
      pdf_dealloc (filter_state->jerr);
    }

  pdf_dealloc (filter_state);
}

static void
pdf_stm_f_dctdec_set_djpeg_param (j_decompress_ptr           cinfo,
                                  struct pdf_stm_f_dctdec_s *filter_state)
{
  /* set color transfor according to DCTDecode dictionary. */
  if (cinfo->saw_Adobe_marker)
    {
      /* if adobe marker is present in encoded data, dictionary should
         be ignored.*/
      return;
    }

  switch (cinfo->num_components)
    {
    case 3:
      {
        if ((filter_state->param_color_transform == -1) ||
            (filter_state->param_color_transform == 1))
          {
            /* dictionary not present or value is 1, transform should
               be done.*/
            cinfo->jpeg_color_space = JCS_YCbCr; /* YCbCr */
          }
        else
          {
            /* no transform, so jpeg color space should be rgb.*/
            cinfo->jpeg_color_space = JCS_RGB;
          }
        break;
      }
    case 4:
      {
        if (filter_state->param_color_transform == 1)
          {
            /* do transform only if the dictionary value is one.*/
            cinfo->jpeg_color_space = JCS_YCCK;
          }
        else
          {
            /* no transform.*/
            cinfo->jpeg_color_space = JCS_CMYK;
          }
        break;
      }
    default:
      {
        break;
      }
    }
}

/* source manager for decompress */
struct pdf_stm_f_dct_cache_source_mgr_s
{
  struct jpeg_source_mgr pub; /* public fields */

  pdf_buffer_t *cache;
  pdf_size_t size_to_skip;
};

static void
init_source (j_decompress_ptr cinfo)
{
  /* This callback is intended to do nothing */
}

static boolean
fill_input_buffer (j_decompress_ptr cinfo)
{
  /* suspending mode, just return false.*/
  return FALSE;
}

static void
skip_input_data (j_decompress_ptr cinfo,
                 long             num_bytes)
{
  if (num_bytes > 0)
    {
      struct pdf_stm_f_dct_cache_source_mgr_s *src =
        (struct pdf_stm_f_dct_cache_source_mgr_s *)cinfo->src;
      long sz_skiped = src->cache->wp - src->cache->rp;

      if (sz_skiped > num_bytes)
        {
          sz_skiped = num_bytes;
          src->size_to_skip = 0;
        }

      src->cache->rp += (pdf_size_t) sz_skiped;
      src->size_to_skip = num_bytes - sz_skiped;

      src->pub.next_input_byte = (pdf_uchar_t *) (src->cache->data + src->cache->rp);
      src->pub.bytes_in_buffer = src->cache->wp - src->cache->rp;
    }
}

static void
term_source (j_decompress_ptr cinfo)
{
  /* This callback is intended to do nothing */
}

static enum pdf_stm_filter_apply_status_e
src_fill (j_decompress_ptr  cinfo,
          pdf_buffer_t     *in)
{
  struct pdf_stm_f_dct_cache_source_mgr_s *src =
    (struct pdf_stm_f_dct_cache_source_mgr_s *)cinfo->src;
  pdf_size_t bytes_to_copy;

  if (src->size_to_skip > in->wp - in->rp)
    {
      src->size_to_skip -= in->wp - in->rp;
      in->rp = in->wp;
      return PDF_STM_FILTER_APPLY_STATUS_NO_INPUT;
    }

  in->rp += src->size_to_skip;
  src->size_to_skip = 0;

  /* clear the src cache */
  src->cache->rp = 0;
  src->cache->wp = 0;
  bytes_to_copy = src->pub.bytes_in_buffer;

  if (bytes_to_copy > 0)
    {
      /* shift the backtrack data when suspension happens. */
      memmove (src->cache->data, src->pub.next_input_byte, bytes_to_copy);
      src->cache->wp += bytes_to_copy;
    }

  bytes_to_copy = PDF_MIN (in->wp - in->rp, src->cache->size - src->cache->wp);
  if (bytes_to_copy > 0)
    {
      memcpy (src->cache->data + src->cache->wp, in->data + in->rp, bytes_to_copy);
      in->rp += bytes_to_copy;
      src->cache->wp += bytes_to_copy;
    }

  src->pub.next_input_byte = (pdf_uchar_t *) (src->cache->data + src->cache->rp);
  src->pub.bytes_in_buffer = src->cache->wp - src->cache->rp;

  return PDF_STM_FILTER_APPLY_STATUS_OK;
}

static void
jpeg_cache_src (j_decompress_ptr  cinfo,
                pdf_buffer_t     *cache)
{
  struct pdf_stm_f_dct_cache_source_mgr_s *src =
    (struct pdf_stm_f_dct_cache_source_mgr_s *)cinfo->src;

  if (cinfo->src == NULL)
    {
      /* first time for this JPEG object? */
      cinfo->src = (struct jpeg_source_mgr *)
        (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo,
                                    JPOOL_PERMANENT,
                                    sizeof (struct pdf_stm_f_dct_cache_source_mgr_s));
      src = (struct pdf_stm_f_dct_cache_source_mgr_s *) cinfo->src;
    }

  src->cache = cache;

  src->pub.init_source = init_source;
  src->pub.fill_input_buffer = fill_input_buffer;
  src->pub.skip_input_data = skip_input_data;
  src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
  src->pub.term_source = term_source;
  src->pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
  src->pub.next_input_byte = NULL; /* until buffer loaded */
}

static enum pdf_stm_filter_apply_status_e
write_ppm_header (j_decompress_ptr   cinfo,
                  pdf_buffer_t      *out,
                  pdf_error_t      **error)
{
  pdf_char_t header[64];
  pdf_i32_t hlen;

  PDF_ASSERT (out->size >= out->wp);
  if (out->size - out->wp < 64)
    return PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT;

  /* Emit file header */
  switch (cinfo->out_color_space)
    {
    case JCS_GRAYSCALE:
      {
        /* emit header for raw PGM format */
        sprintf (header, "P5\n%ld %ld\n%d\n",
                 (long) cinfo->output_width, (long) cinfo->output_height,
                 PPM_MAXVAL);
        break;
      }
  case JCS_RGB:
    {
      /* emit header for raw PPM format */
      sprintf (header, "P6\n%ld %ld\n%d\n",
               (long) cinfo->output_width, (long) cinfo->output_height,
               PPM_MAXVAL);
      break;
    }
  default:
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ERROR,
                     "unhandled out color space");
      return PDF_STM_FILTER_APPLY_STATUS_ERROR;
    }
  }

  hlen = strlen (header);
  if (hlen > out->size-out->wp)
    return PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT;

  memcpy (out->data + out->wp, header, hlen);
  out->wp += hlen;
  return PDF_STM_FILTER_APPLY_STATUS_OK;
}

static enum pdf_stm_filter_apply_status_e
stm_f_dctdec_apply (void          *state,
                    pdf_buffer_t  *in,
                    pdf_buffer_t  *out,
                    pdf_bool_t     finish,
                    pdf_error_t  **error)
{
  struct pdf_stm_f_dctdec_s *filter_state = state;
  struct jpeg_decompress_struct *pcinfo = filter_state->cinfo;
  enum pdf_stm_filter_apply_status_e ret;
  pdf_i32_t iret;

  if (finish &&
      ((in->wp - in->rp) < 1 ) &&
      (pcinfo->output_scanline == pcinfo->output_height) &&
      (0 == filter_state->row_valid_size - filter_state->row_copy_index))
    {
      return PDF_STM_FILTER_APPLY_STATUS_EOF;
    }

  ret = PDF_STM_FILTER_APPLY_STATUS_OK;
  while (ret == PDF_STM_FILTER_APPLY_STATUS_OK)
    {
      if (filter_state->state == DCTDEC_STATE_INIT)
        {
          if (!filter_state->djpeg_in)
            {
              filter_state->djpeg_in = pdf_buffer_new (PDF_DJPEG_CACHE_SIZE, error);
              if (!filter_state->djpeg_in)
                {
                  ret = PDF_STM_FILTER_APPLY_STATUS_ERROR;
                  break;
                }
            }

          jpeg_cache_src (pcinfo, filter_state->djpeg_in);
          filter_state->backup_state = DCTDEC_STATE_READHDR;
          filter_state->state = DCTDEC_STATE_CACHE_IN;
        }

      if (filter_state->state == DCTDEC_STATE_CACHE_IN)
        {
          if (pdf_buffer_eob_p (in))
            {
              ret = PDF_STM_FILTER_APPLY_STATUS_NO_INPUT;
              break;
            }

          ret = src_fill (pcinfo, in);
          if (ret != PDF_STM_FILTER_APPLY_STATUS_OK)
            break;

          filter_state->state = filter_state->backup_state;
        }

      if (filter_state->state == DCTDEC_STATE_READHDR)
        {
          iret = jpeg_read_header (pcinfo, TRUE);
          if (iret == JPEG_SUSPENDED)
            {
              /* continue the loop, go into the "cache state" */
              ret = PDF_STM_FILTER_APPLY_STATUS_OK;
              filter_state->backup_state = filter_state->state;
              filter_state->state = DCTDEC_STATE_CACHE_IN;
              continue;
            }

          if (iret != JPEG_HEADER_OK)
            {
              pdf_set_error (error,
                             PDF_EDOMAIN_BASE_STM,
                             PDF_ERROR,
                             "error reading JPEG header");
              ret = PDF_STM_FILTER_APPLY_STATUS_ERROR;
              break;
            }

          pdf_stm_f_dctdec_set_djpeg_param (pcinfo, filter_state);
          filter_state->state = DCTDEC_STATE_STARTDJP;
        }

      if (filter_state->state == DCTDEC_STATE_STARTDJP)
        {
          iret = jpeg_start_decompress (pcinfo);
          if (iret == FALSE)
            {
              /* continue the loop, go into the "cache state" */
              ret = PDF_STM_FILTER_APPLY_STATUS_OK;
              filter_state->backup_state = filter_state->state;
              filter_state->state = DCTDEC_STATE_CACHE_IN;
              continue;
            }

          /* here we know the output size, so allocate memory for them. */
          filter_state->row_stride = pcinfo->output_width * pcinfo->output_components;

          /* Make a one-row-high sample array that will go away when done with image */
          filter_state->row_buf = (*pcinfo->mem->alloc_sarray)
            ((j_common_ptr) pcinfo, JPOOL_IMAGE, filter_state->row_stride, 1);
          filter_state->row_valid_size = 0;
          filter_state->row_copy_index = 0;
          filter_state->state = DCTDEC_STATE_WRITEHDR;
        }

      if (filter_state->state == DCTDEC_STATE_WRITEHDR)
        {
          ret = write_ppm_header (pcinfo, out, error);
          if (ret != PDF_STM_FILTER_APPLY_STATUS_OK)
            break;

          filter_state->state = DCTDEC_STATE_SCANLINE;
        }

      if (filter_state->state == DCTDEC_STATE_OUTPUTLINE)
        {
          if ((filter_state->row_valid_size > 0) &&
              (filter_state->row_valid_size > filter_state->row_copy_index))
            {
              if (pdf_buffer_full_p (out))
                {
                  ret = PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT;
                  break;
                }
              else
                {
                  pdf_size_t bytes_to_copy =
                    PDF_MIN (filter_state->row_valid_size - filter_state->row_copy_index,
                             out->size - out->wp);

                  if (bytes_to_copy > 0)
                    {
                      memcpy (out->data+out->wp,
                              filter_state->row_buf[0]+filter_state->row_copy_index,
                              bytes_to_copy);
                      out->wp += bytes_to_copy;
                    }

                  filter_state->row_copy_index += bytes_to_copy;
                  if (filter_state->row_copy_index != filter_state->row_valid_size)
                    {
                      ret = PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT;
                      break;
                    }
                }

              if ((ret == PDF_STM_FILTER_APPLY_STATUS_OK) &&
                  (pcinfo->output_scanline == pcinfo->output_height))
                {
                  ret = PDF_STM_FILTER_APPLY_STATUS_EOF;
                  break;
                }
            }

          filter_state->state = DCTDEC_STATE_SCANLINE;
        }

      if (filter_state->state == DCTDEC_STATE_SCANLINE)
        {
          ret = PDF_STM_FILTER_APPLY_STATUS_OK;
          if (pcinfo->output_scanline < pcinfo->output_height)
            {
              pdf_i32_t scannum;

              scannum = jpeg_read_scanlines (pcinfo,
                                             filter_state->row_buf,
                                             1);
              if (scannum == 0)
                {
                  filter_state->backup_state = filter_state->state;
                  filter_state->state = DCTDEC_STATE_CACHE_IN;
                  break;
                }

              if (scannum != 1)
                {
                  ret = PDF_STM_FILTER_APPLY_STATUS_ERROR;
                  break;
                }

              filter_state->num_scanlines += scannum;
              filter_state->row_valid_size = scannum * filter_state->row_stride;
              filter_state->row_copy_index = 0;
              filter_state->state = DCTDEC_STATE_OUTPUTLINE;
            }
        }
    }

  if (ret == PDF_STM_FILTER_APPLY_STATUS_NO_INPUT &&
      finish)
    {
      ret = PDF_STM_FILTER_APPLY_STATUS_EOF;
    }

  return ret;
}

/* End of pdf-stm-f-dct.c */

/* -*- mode: C -*- Time-stamp: "08/11/24 15:50:21 yangchanghua"
 imagetojpeg_dst_term*
 *       File:         pdf-stm-f-dct.c
 *       Date:         Fri Jul 13 17:08:41 2007
 *
 *       GNU PDF Library - DCT decoder filter
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

#include <config.h>
#include <pdf-stm-f-dct.h>
#include <jerror.h>
#include <memory.h>

#define PPM_MAXVAL 255

static void         pdf_stm_f_dctdec_jpeg_cache_src (j_decompress_ptr cinfo, pdf_stm_buffer_t cache);
static pdf_status_t pdf_stm_f_dctdec_src_fill(j_decompress_ptr cinfo, pdf_stm_buffer_t in);
static pdf_status_t pdf_stm_f_dctdec_write_ppm_header(j_decompress_ptr cinfo, pdf_stm_buffer_t out);


pdf_status_t
pdf_stm_f_dctenc_init (pdf_hash_t params,
                        void **state)
{
  return PDF_OK;
}

pdf_status_t
pdf_stm_f_dctenc_apply (pdf_hash_t params,
                         void *state,
                         pdf_stm_buffer_t in,
                         pdf_stm_buffer_t out,
                         pdf_bool_t finish_p)
{
  pdf_status_t ret = PDF_OK;
  return ret;
}

pdf_status_t
pdf_stm_f_dctenc_dealloc_state (void *state)
{
  return PDF_OK;
}

pdf_status_t
pdf_stm_f_dctdec_init (pdf_hash_t params,
                        void **state)
{
  pdf_stm_f_dctdec_t dctdec_state;
  pdf_status_t ret;

  /* Allocate the internal state structure */
  dctdec_state = pdf_alloc (sizeof(struct pdf_stm_f_dctdec_s));
  memset(dctdec_state, 0, sizeof(struct pdf_stm_f_dctdec_s));
  if (dctdec_state != NULL)
    {
      dctdec_state->cinfo.err = jpeg_std_error(&dctdec_state->jerr);
      jpeg_create_decompress(&dctdec_state->cinfo);

      dctdec_state->state = DCTDEC_STATE_INIT;
      *state = dctdec_state;
      ret = PDF_OK;
    }
  else
    {
      ret = PDF_ERROR;
    }

  return ret;
}

#define PDF_DJPEG_CACHE_SIZE (1024)

pdf_status_t
pdf_stm_f_dctdec_apply (pdf_hash_t params,
                         void *state,
                         pdf_stm_buffer_t in,
                         pdf_stm_buffer_t out,
                         pdf_bool_t finish_p)
{
  pdf_i32_t    loop_end = 0;
  pdf_status_t ret = PDF_OK;
  pdf_i32_t iret;
  pdf_stm_f_dctdec_t filter_state = (pdf_stm_f_dctdec_t)state;
  struct jpeg_decompress_struct *pcinfo = &filter_state->cinfo;

  if(finish_p 
      && in->wp - in->rp < 1 
      && pcinfo->output_scanline == pcinfo->output_height 
      && 0 == filter_state->row_valid_size - filter_state->row_copy_index )
  {
    return PDF_EEOF;
  }

  while(!loop_end && ret == PDF_OK)
  {
    if(DCTDEC_STATE_INIT == filter_state->state)
    {
      if(!filter_state->djpeg_in)
      {
        filter_state->djpeg_in = pdf_stm_buffer_new(PDF_DJPEG_CACHE_SIZE);
        if(!filter_state->djpeg_in)
        {
          ret = PDF_ERROR;
          break;
        }
      }
      pdf_stm_f_dctdec_jpeg_cache_src(pcinfo, filter_state->djpeg_in);
      filter_state->backup_state = DCTDEC_STATE_READHDR;
      filter_state->state = DCTDEC_STATE_CACHE;
    }
    if(DCTDEC_STATE_CACHE == filter_state->state)
    {
      if(pdf_stm_buffer_eob_p(in))
      {
        ret = PDF_ENINPUT;
        break;
      }
      ret = pdf_stm_f_dctdec_src_fill(pcinfo, in);
      if(PDF_OK != ret)
      {
        break;
      }
      filter_state->state = filter_state->backup_state;
    }
    if( DCTDEC_STATE_READHDR == filter_state->state)
    {
      iret = jpeg_read_header(&filter_state->cinfo, TRUE);
      if(iret == JPEG_SUSPENDED)
      {
        /* continue the loop, go into the "cache state" */
        ret = PDF_OK;
        filter_state->backup_state = filter_state->state;
        filter_state->state = DCTDEC_STATE_CACHE;
        continue;
      }

      if(iret != JPEG_HEADER_OK)
      {
        ret = PDF_ERROR;
        break;
      }
      filter_state->state = DCTDEC_STATE_STARTDJP;
    }
    if(DCTDEC_STATE_WRITEHDR == filter_state->state)
    {
      ret = pdf_stm_f_dctdec_write_ppm_header( pcinfo, out);
      if(PDF_OK != ret) 
      {
        break;
      }
      filter_state->state = DCTDEC_STATE_SCANLINE;
    }
    
    if(DCTDEC_STATE_STARTDJP == filter_state->state)
    {
      iret = jpeg_start_decompress(&filter_state->cinfo);
      if(iret == FALSE)
      {
        /* continue the loop, go into the "cache state" */
        ret = PDF_OK;
        filter_state->backup_state = filter_state->state;
        filter_state->state = DCTDEC_STATE_CACHE;
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
    if(DCTDEC_STATE_SCANLINE == filter_state->state)
    {
      if(!filter_state->djpeg_out)
      {
        filter_state->djpeg_out = pdf_stm_buffer_new(pcinfo->output_width * pcinfo->output_height * 3);
        if(!filter_state->djpeg_out)
        {
          return PDF_ERROR;
        }
      }
      while(pcinfo->output_scanline < pcinfo->output_height || filter_state->row_valid_size > filter_state->row_copy_index)
      {
        ret = PDF_OK;
        if(filter_state->row_valid_size <= filter_state->row_copy_index)
        {
          pdf_i32_t scannum = 0;
          scannum = jpeg_read_scanlines(pcinfo, filter_state->row_buf, 1);  //pcinfo->output_height);
          if(scannum == 0)
          {
            ret = PDF_OK;
            filter_state->backup_state = filter_state->state;
            filter_state->state = DCTDEC_STATE_CACHE;
            break;
          }
          if(scannum != 1)
          {
            ret = PDF_ERROR;
            break;
            loop_end = 1;
          }
          filter_state->num_scanlines += scannum;
          filter_state->row_valid_size = scannum * filter_state->row_stride;
          filter_state->row_copy_index = 0;
        }
    
        if(pdf_stm_buffer_full_p(out))
        {
          ret = PDF_ENOUTPUT;
          break;
          loop_end = 1;
        }
        else
        {
          pdf_size_t bytes_to_copy = PDF_MIN(filter_state->row_valid_size - filter_state->row_copy_index, out->size - out->wp);
          if(bytes_to_copy > 0)
          {
            memcpy(out->data+out->wp, filter_state->row_buf[0]+filter_state->row_copy_index, bytes_to_copy);
            out->wp += bytes_to_copy;
          }
          filter_state->row_copy_index += bytes_to_copy;
          if(filter_state->row_copy_index != filter_state->row_valid_size)
          {
            ret = PDF_ENOUTPUT;
            break;
            loop_end = 1;
          }
        }
        if(PDF_OK == ret && pcinfo->output_scanline == pcinfo->output_height)
        {
          ret = PDF_EEOF;
          break;
          loop_end = 1;
        }
      }
    }
  }
  if(PDF_OK == ret)
  {
    ret = PDF_EEOF;
  }
  return ret;
}

pdf_status_t
pdf_stm_f_dctdec_dealloc_state (void *state)
{
  pdf_stm_f_dctdec_t dctdec_state = (pdf_stm_f_dctdec_t) state;

  jpeg_finish_decompress(&dctdec_state->cinfo);
  jpeg_destroy_decompress(&dctdec_state->cinfo);

  pdf_dealloc (dctdec_state);

  return PDF_OK;
}

/* Private functions */

/* source manager for decompress */
struct pdf_stm_f_dct_cache_source_mgr_s{
  struct jpeg_source_mgr pub; /* public fields */

  pdf_stm_buffer_t cache;
  pdf_size_t size_to_skip;
};

typedef struct pdf_stm_f_dct_cache_source_mgr_s *pdf_stm_f_dct_cache_source_mgr_t;

static void
pdf_stm_f_dctdec_init_source (j_decompress_ptr cinfo)
{
}

static boolean
pdf_stm_f_dctdec_fill_input_buffer (j_decompress_ptr cinfo)
{
  /* suspending mode, just return false.*/
  return FALSE;
}

static void
pdf_stm_f_dctdec_skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
  if (num_bytes > 0) 
  {
    pdf_stm_f_dct_cache_source_mgr_t src = (pdf_stm_f_dct_cache_source_mgr_t ) cinfo->src;
    long sz_skiped = src->cache->wp - src->cache->rp;
    if(sz_skiped > num_bytes)
    {
      sz_skiped = num_bytes;
      src->size_to_skip = 0;
    }
    src->cache->rp += (pdf_size_t)sz_skiped;
    src->size_to_skip = num_bytes - sz_skiped;
    
    src->pub.next_input_byte = src->cache->data + src->cache->rp;
    src->pub.bytes_in_buffer = src->cache->wp - src->cache->rp;
  }
}

static void
pdf_stm_f_dctdec_term_source (j_decompress_ptr cinfo)
{
}

pdf_status_t
pdf_stm_f_dctdec_src_fill(j_decompress_ptr cinfo, pdf_stm_buffer_t in)
{
  pdf_stm_f_dct_cache_source_mgr_t src = (pdf_stm_f_dct_cache_source_mgr_t ) cinfo->src;

  pdf_size_t bytes_to_copy; 

  if(src->size_to_skip > in->wp - in->rp)
  {
    src->size_to_skip -= in->wp - in->rp;
    in->rp = in->wp;
    return PDF_ENINPUT;
  }
  else
  {
    in->rp += src->size_to_skip;
    src->size_to_skip = 0;
  }

  bytes_to_copy = src->pub.bytes_in_buffer;
  if(bytes_to_copy > 0)
  {
    /* shift the backtrack data when suspension happens. */
    memmove(src->cache->data, src->pub.next_input_byte, bytes_to_copy);
    src->cache->rp = 0;
    src->cache->wp = bytes_to_copy;
  }

  bytes_to_copy = PDF_MIN(in->wp - in->rp, src->cache->size - src->cache->wp);
  if(bytes_to_copy > 0)
  {
    memcpy(src->cache->data + src->cache->wp, in->data + in->rp, bytes_to_copy);
    in->rp += bytes_to_copy;
    src->cache->wp += bytes_to_copy;
  }

  src->pub.bytes_in_buffer = src->cache->wp - src->cache->rp;
  src->pub.next_input_byte = src->cache->data + src->cache->rp;
  return PDF_OK;
}

void 
pdf_stm_f_dctdec_src_fill_eoi(j_decompress_ptr cinfo)
{
  pdf_stm_f_dct_cache_source_mgr_t src = (pdf_stm_f_dct_cache_source_mgr_t ) cinfo->src;
  if (pdf_stm_buffer_eob_p(src->cache) && src->cache->size > 1) 
  {
    /* Insert a fake EOI marker */
    src->cache->rp = 0;
    src->cache->wp = 2;
    src->cache->data[0] = (JOCTET) 0xFF;
    src->cache->data[1] = (JOCTET) JPEG_EOI;
  }
  src->pub.bytes_in_buffer = src->cache->wp - src->cache->rp;
  src->pub.next_input_byte = src->cache->data + src->cache->rp;
}

static void
pdf_stm_f_dctdec_jpeg_cache_src (j_decompress_ptr cinfo, pdf_stm_buffer_t cache)
{
  pdf_stm_f_dct_cache_source_mgr_t src = (pdf_stm_f_dct_cache_source_mgr_t ) cinfo->src;

  if (cinfo->src == NULL) 
  {
    /* first time for this JPEG object? */
    cinfo->src = (struct jpeg_source_mgr *)
        (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(struct pdf_stm_f_dct_cache_source_mgr_s));
    src = (pdf_stm_f_dct_cache_source_mgr_t) cinfo->src;
  }

  src->cache = cache;

  src->pub.init_source = pdf_stm_f_dctdec_init_source;
  src->pub.fill_input_buffer = pdf_stm_f_dctdec_fill_input_buffer;
  src->pub.skip_input_data = pdf_stm_f_dctdec_skip_input_data;
  src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
  src->pub.term_source = pdf_stm_f_dctdec_term_source;
  src->pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
  src->pub.next_input_byte = NULL; /* until buffer loaded */
}

static pdf_status_t 
pdf_stm_f_dctdec_write_ppm_header(j_decompress_ptr cinfo, pdf_stm_buffer_t out)
{
  pdf_char_t header[64];
  pdf_i32_t hlen;

  if(out->size - out->wp < 64)
  {
    return PDF_ENOUTPUT;
  }
    /* Emit file header */
  switch (cinfo->out_color_space) {
    case JCS_GRAYSCALE:
      /* emit header for raw PGM format */

      sprintf(header, "P5\n%ld %ld\n%d\n",
          (long) cinfo->output_width, (long) cinfo->output_height,
          PPM_MAXVAL);
      break;
    case JCS_RGB:
      /* emit header for raw PPM format */
      sprintf(header, "P6\n%ld %ld\n%d\n",
             (long) cinfo->output_width, (long) cinfo->output_height,
             PPM_MAXVAL);
     break;
    default:
     return PDF_ERROR;
  }
  hlen = strlen(header);
  if(hlen > out->size-out->wp)
  {
    return PDF_ENOUTPUT;
  }
#if 0
  fwrite(header, hlen, 1, stdout);
#else
  memcpy(out->data + out->wp, header, hlen); 
  out->wp += hlen; 
#endif
  return PDF_OK;
}

/* End of pdf_stm_f_ahex.c */

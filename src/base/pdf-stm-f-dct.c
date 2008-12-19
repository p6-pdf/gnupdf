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

#if 1
#define debugmsg(msg) 
#else
#define debugmsg(msg) printf msg
#endif
#if 1
#define debugmsg1(msg) 
#else
#define debugmsg1(msg) printf msg
#endif

#define PPM_MAXVAL 255

pdf_status_t pdf_stm_f_dct_src_fill(j_decompress_ptr cinfo, pdf_stm_buffer_t in);
static void pdf_stm_dct_dec_jpeg_cache_src (j_decompress_ptr cinfo, pdf_stm_buffer_t cache);
static void pdf_stm_dct_dec_jpeg_cache_src_special (j_decompress_ptr cinfo, pdf_stm_buffer_t cache);
METHODDEF(boolean)  print_text_marker (j_decompress_ptr cinfo);
static pdf_status_t write_bmp_header (j_decompress_ptr cinfo, pdf_stm_buffer_t out);
static pdf_status_t pdf_stm_dctdec_write_ppm_header(j_decompress_ptr cinfo, pdf_stm_buffer_t out);


pdf_status_t
pdf_stm_f_dctenc_init (pdf_hash_t params,
                        void **state)
{
#if 0
  pdf_stm_f_ahexenc_t filter_state;

  /* This filter uses no parameters */
  /* Allocate the internal state structure */
  filter_state = pdf_alloc (sizeof (struct pdf_stm_f_ahexenc_s));
  if (filter_state == NULL)
    {
      return PDF_ERROR;
    }
  
  /* Initialize fields */
  filter_state->last_nibble = -1;
  filter_state->written_bytes = 0;

  *state = (void *) filter_state;
#endif

  return PDF_OK;
}

#define PDF_STM_F_AHEX_LINE_WIDTH 60

pdf_status_t
pdf_stm_f_dctenc_apply (pdf_hash_t params,
                         void *state,
                         pdf_stm_buffer_t in,
                         pdf_stm_buffer_t out,
                         pdf_bool_t finish_p)
{
  pdf_status_t ret = PDF_OK;
#if 0
  pdf_stm_f_ahexenc_t filter_state;
  pdf_char_t first_nibble;
  pdf_char_t second_nibble;
  pdf_char_t in_char;
  pdf_bool_t wrote_newline;

  if (finish_p)
    {
      /* Assume that there is room in the output buffer to hold the
         EOD marker */
      out->data[out->wp++] = '>';
      return PDF_EEOF;
    }

  filter_state = (pdf_stm_f_ahexenc_t) state;
  
  wrote_newline = PDF_FALSE;
  ret = PDF_OK;
  while (!pdf_stm_buffer_full_p (out))
    {
      if ((!wrote_newline) &&
          (filter_state->written_bytes != 0) &&
          (filter_state->written_bytes % PDF_STM_F_AHEX_LINE_WIDTH) == 0)
        {
          /* Write down a newline character */
          out->data[out->wp] = '\n';
          out->wp++;
          wrote_newline = PDF_TRUE;
          filter_state->written_bytes = 0;
          continue;
        }

      /* Write down any pending nibble, if needed, without consuming
         any input byte */
      if (filter_state->last_nibble != -1) 
        {
          out->data[out->wp] = filter_state->last_nibble;
          out->wp++;
          filter_state->written_bytes++;
          filter_state->last_nibble = -1;
          continue;
        }

      /* Try to consume an input byte */
      if (!pdf_stm_buffer_eob_p (in))
        {
          /* For each byte in the input we should generate two bytes in the
             output. */
          in_char = in->data[in->rp];
          
          /* Determine the hex digits to write for this input character */
          if (filter_state->last_nibble != -1)
            {
              first_nibble = (pdf_char_t) filter_state->last_nibble;
              second_nibble = pdf_stm_f_ahex_int2hex (in_char >> 4);
            }
          else
            {
              first_nibble = pdf_stm_f_ahex_int2hex (in_char >> 4);
              second_nibble = pdf_stm_f_ahex_int2hex (in_char);
            }
          in->rp++;

          /* Write the hex digits into the output buffer, if
             possible */
          
          /* First nibble */
          out->data[out->wp] = first_nibble;
          out->wp++;
          filter_state->written_bytes++;
          
          /* Maybe write the second nibble */
          if (pdf_stm_buffer_full_p (out))
            {
              filter_state->last_nibble = second_nibble;
            }
          else
            {
              out->data[out->wp] = second_nibble;
              out->wp++;
              filter_state->written_bytes++;
            }
        }
      else
        {
          /* We need more input */
          break;
        }
    }

  if (pdf_stm_buffer_full_p (out))
    {
      ret = PDF_ENOUTPUT;
    }
  else
    {
      ret = PDF_ENINPUT;
    }
#endif
  return ret;
}

pdf_status_t
pdf_stm_f_dctenc_dealloc_state (void *state)
{
#if 0
  pdf_stm_f_ahexenc_t ahexenc_state;

  ahexenc_state = (pdf_stm_f_ahexenc_t) state;
  pdf_dealloc (ahexenc_state);
#endif
  return PDF_OK;
}


static void 
pdf_stm_f_dctdec_dst_init(pdf_stm_f_dctdec_t state)
{
  struct jpeg_decompress_struct *cinfo = &state->cinfo;

  return;
}
#if 0
static boolean 
pdf_stm_f_dctdec_dst_empty(j_compress_ptr cinfo)
{
  struct pdf_jpeg_dec_dst_s *dst = (void *)cinfo->dest;
  dst->sz *= 2;
  dst->used = dst->off - dst->buf;
  dst->buf = pdf_realloc(dst->buf, dst->sz * sizeof *dst->buf);
  dst->off = dst->buf + dst->used;
  dst->jdst.next_output_byte = dst->off;
  dst->jdst.free_in_buffer = dst->sz - dst->used;

  return TRUE;
}

static void 
pdf_stm_f_dctdec_dst_term(j_compress_ptr cinfo)
{
  struct pdf_jpeg_dec_dst_s *dst = (void *)cinfo->dest;

  dst->used += dst->sz - dst->jdst.free_in_buffer;
  dst->off = dst->buf + dst->used;

  return;
} 

static void 
pdf_stm_f_dctdec_dst_set(j_compress_ptr cinfo, 
                          struct pdf_jpeg_dec_dst_s *dst)
{
  dst->jdst.init_destination =  pdf_stm_f_dctdec_dst_init;
  dst->jdst.empty_output_buffer = pdf_stm_f_dctdec_dst_empty;
  dst->jdst.term_destination = pdf_stm_f_dctdec_dst_term;
  cinfo->dest = (void *)dst;

  return;
}
#endif

pdf_status_t
pdf_stm_f_dctdec_init (pdf_hash_t params,
                        void **state)
{
  pdf_stm_f_dctdec_t dctdec_state;
  pdf_status_t ret;
  const char *ifile_key = "dct_input_file";


  /* This filter uses no parameters */
  /* Allocate the internal state structure */
  dctdec_state = pdf_alloc (sizeof(struct pdf_stm_f_dctdec_s));
  memset(dctdec_state, 0, sizeof(struct pdf_stm_f_dctdec_s));
  if (dctdec_state != NULL)
    {
      dctdec_state->cinfo.err = jpeg_std_error(&dctdec_state->jerr);
      jpeg_create_decompress(&dctdec_state->cinfo);
#if 0 
      if(pdf_hash_key_p(params, ifile_key))
      {
        char *ifname = NULL;
        pdf_hash_search(params, ifile_key, &ifname); 
        if(ifname == NULL)
        {
          return PDF_ERROR;
        }
        dctdec_state->input_file = fopen(ifname, "rb");
        if(dctdec_state->input_file == NULL)
        {
          return PDF_ERROR;
        }
        jpeg_stdio_src(&dctdec_state->cinfo, dctdec_state->input_file);
        
      }
#endif

      jpeg_set_marker_processor(&dctdec_state->cinfo, JPEG_COM, print_text_marker);
      jpeg_set_marker_processor(&dctdec_state->cinfo, JPEG_APP0+12, print_text_marker);

      //jpeg_stdio_src(&dctdec_state->cinfo, dctdec_state->input_file);
      dctdec_state->state = DCTDEC_STATE_INIT;
      *state = dctdec_state;
      ret = PDF_OK;
      {
        int i=0;
        while(i--)
          sleep(1);
      }
    }
  else
    {
      ret = PDF_ERROR;
    }

  return ret;
}

#define PDF_DJPEG_CACHE_SIZE (10240)

pdf_status_t
pdf_stm_f_dctdec_apply (pdf_hash_t params,
                         void *state,
                         pdf_stm_buffer_t in,
                         pdf_stm_buffer_t out,
                         pdf_bool_t finish_p)
{
  pdf_i32_t    loop_end = 0;
  pdf_status_t ret;
  pdf_i32_t iret;
  pdf_u32_t unusedint;
  pdf_char_t ic;
  pdf_stm_f_dctdec_t filter_state = (pdf_stm_f_dctdec_t)state;
  struct jpeg_decompress_struct *pcinfo = &filter_state->cinfo;
  pdf_stm_buffer_t cache_in, cache_out;


  ret = PDF_OK;
  filter_state = (pdf_stm_f_dctdec_t) state;

  debugmsg1(("\n\n#############################################\n"));
  debugmsg1(("#     enter dct_dec_apply....\n"));
  debugmsg1(("#     apply status ======= %d\nbuffer in:%x, %d, %d\n", filter_state->state, in->data, in->rp, in->wp));
  debugmsg1(("#     scanline: %d of %d\n", pcinfo->output_scanline, pcinfo->output_height));

  if(finish_p && in->wp - in->rp < 1 && pcinfo->output_scanline == pcinfo->output_height)
  {
    debugmsg1(("\n\nfinished.\n"));
    return PDF_EEOF;
  }

  while(!loop_end && ret == PDF_OK)
  {
  debugmsg1(("\n=========================\n"));
  debugmsg1(("= ret: %d,  status %d \n", ret, filter_state->state));
  debugmsg1(("=========================\n"));
 
  if(DCTDEC_STATE_INIT == filter_state->state)
  {
      cache_in = filter_state->djpeg_in;
      if(!filter_state->djpeg_in)
      {
        filter_state->djpeg_in = pdf_stm_buffer_new(PDF_DJPEG_CACHE_SIZE);
        if(!filter_state->djpeg_in)
        {
          ret = PDF_ERROR;
          break;
        }
        cache_in = filter_state->djpeg_in;
      }
      pdf_stm_dct_dec_jpeg_cache_src(pcinfo, filter_state->djpeg_in);
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
    ret = pdf_stm_f_dct_src_fill(pcinfo, in);
    {
      int i = 0;
      while(i--)
        sleep(1);
    }
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
      debugmsg(("read_header error!!!!!...\n"));
      ret = PDF_ERROR;
      break;
    }
    filter_state->state = DCTDEC_STATE_STARTDJP;
  }

  if(DCTDEC_STATE_WRITEHDR == filter_state->state)
  {
    ret = pdf_stm_dctdec_write_ppm_header( pcinfo, out);
    if(PDF_ENINPUT == ret) 
    {
      ret = PDF_OK;
      filter_state->backup_state = filter_state->state;
      filter_state->state = DCTDEC_STATE_CACHE;
      continue;
    }
    else if(PDF_OK != ret)
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
        return PDF_ERROR;
    }
    while(pcinfo->output_scanline < pcinfo->output_height || filter_state->row_valid_size > filter_state->row_copy_index)
    {
          ret = PDF_OK;
      if(filter_state->row_valid_size <= filter_state->row_copy_index)
      {
        pdf_i32_t scannum = 0;
        scannum = jpeg_read_scanlines(pcinfo, filter_state->row_buf, 1);  //pcinfo->output_height);
  debugmsg1(("\n-------------\n"));
  debugmsg1(("- scan: %d,  result: %d \n", pcinfo->output_scanline, scannum));
  debugmsg1(("-------------\n"));
        if(scannum == 0)
        {
          ret = PDF_OK;
          filter_state->backup_state = filter_state->state;
          filter_state->state = DCTDEC_STATE_CACHE;
          break;
        }
        if(scannum != 1)
        {
          debugmsg1(("!!!error scannum: %d\n", scannum));
          ret = PDF_ERROR;
          break;
          loop_end = 1;
        }
        filter_state->num_scanlines += scannum;
        filter_state->row_valid_size = scannum * filter_state->row_stride;
        filter_state->row_copy_index = 0;
        debugmsg(("scan sucessfully! now %d \n", filter_state->num_scanlines));
      }
    
      if(pdf_stm_buffer_full_p(out))
      {
        ret = PDF_ENOUTPUT;
        break;
        loop_end = 1;
      }
      else
      {
#if 0
        static int copy_index = 0;
        pdf_size_t bytes_to_copy = filter_state->row_valid_size - filter_state->row_copy_index;
        if(bytes_to_copy > 0)
        {
          fwrite(filter_state->row_buf[0], 1, bytes_to_copy, stdout);
          filter_state->row_copy_index += bytes_to_copy;
          copy_index += bytes_to_copy;
        }
#else
        pdf_size_t bytes_to_copy = PDF_MIN(filter_state->row_valid_size - filter_state->row_copy_index, out->size - out->wp);
        if(bytes_to_copy > 0)
        {
          memcpy(out->data+out->wp, filter_state->row_buf[0]+filter_state->row_copy_index, bytes_to_copy);
          out->wp += bytes_to_copy;
        }
        filter_state->row_copy_index += bytes_to_copy;
#endif
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
  debugmsg1(("leave dct_dec_apply....%d\n", ret));
  debugmsg1(("#############################################\n"));
  return ret;
}

pdf_status_t
pdf_stm_f_dctdec_dealloc_state (void *state)
{
  pdf_stm_f_dctdec_t dctdec_state;
  dctdec_state = (pdf_stm_f_dctdec_t) state;
  debugmsg1(("\nentering dealloc, %d\n", __LINE__));
  debugmsg1(("scanlines : %d\n", dctdec_state->num_scanlines));
#if 1
  //(*dctdec_state->dest_mgr->start_output) (&dctdec_state->cinfo, dctdec_state->dest_mgr);
  /* Process data */
 /* 
  while (dctdec_state->cinfo.output_scanline < dctdec_state->cinfo.output_height) 
     {
      dctdec_state->num_scanlines = jpeg_read_scanlines(&dctdec_state->cinfo, dctdec_state->dest_mgr->buffer,
          dctdec_state->dest_mgr->buffer_height);
       (*dctdec_state->dest_mgr->put_pixel_rows) (&dctdec_state->cinfo, dctdec_state->dest_mgr, dctdec_state->num_scanlines);
     }
*/
   //(*dctdec_state->dest_mgr->finish_output) (&dctdec_state->cinfo, dctdec_state->dest_mgr);
   (void) jpeg_finish_decompress(&dctdec_state->cinfo);
   jpeg_destroy_decompress(&dctdec_state->cinfo);
 
  /* Close files, if we opened them */

  //pdf_dealloc (dctdec_state);
#endif
  return PDF_OK;
}

/* Private functions */
#if 1
/* source manager for decompress */
struct pdf_stm_f_dct_cache_source_mgr_s{
  struct jpeg_source_mgr pub; /* public fields */

  pdf_stm_buffer_t cache;
  pdf_size_t size_to_skip;
} ;

typedef struct pdf_stm_f_dct_cache_source_mgr_s *pdf_stm_f_dct_cache_source_mgr_t;

#define INPUT_BUF_SIZE  4096  /* choose an efficiently fread'able size */

static void
pdf_stm_dct_dec_init_source (j_decompress_ptr cinfo)
{
  pdf_stm_f_dct_cache_source_mgr_t src = (pdf_stm_f_dct_cache_source_mgr_t ) cinfo->src;
  src->size_to_skip = 0;

  /* We reset the empty-input-file flag for each image,
  ** but we don't clear the input buffer.
  ** This is correct behavior for reading a series of images from one source.
  **/
}

static boolean
pdf_stm_dct_dec_fill_input_buffer (j_decompress_ptr cinfo)
{
  debugmsg(("fill_input_buffer called.\n"));
  return FALSE;
}

static void
pdf_stm_dct_dec_skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
  debugmsg(("skip_input_data called.\n"));
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
    
    /* note we assume that fill_input_buffer will never return FALSE,
    ** so suspension need not be handled.
    **/
    src->pub.next_input_byte = src->cache->data + src->cache->rp;
    src->pub.bytes_in_buffer = src->cache->wp - src->cache->rp;
  }
}

static void
pdf_stm_dct_dec_term_source (j_decompress_ptr cinfo)
{
/* no work necessary here */
}

pdf_status_t
pdf_stm_f_dct_src_fill(j_decompress_ptr cinfo, pdf_stm_buffer_t in)
{
  pdf_stm_f_dct_cache_source_mgr_t src = (pdf_stm_f_dct_cache_source_mgr_t ) cinfo->src;

  pdf_size_t bytes_to_copy; 
  debugmsg1(("---->entering src_fill called.\n"));
  debugmsg(("src->pub->fill: %x\n", src->pub.fill_input_buffer));

  if(src->size_to_skip > in->wp - in->rp)
  {
    debugmsg(("skip size:%d\n", in->wp - in->rp));
    src->size_to_skip -= in->wp - in->rp;
    in->rp = in->wp;
    return PDF_ENINPUT;
  }
  else
  {
    debugmsg(("skip size:%d\n", src->size_to_skip));
    in->rp += src->size_to_skip;
    src->size_to_skip = 0;
  }

  bytes_to_copy = src->pub.bytes_in_buffer;
  if(bytes_to_copy > 0)
  {
    /* shift the backtrack data when suspension happens. */
    debugmsg(("move data in jpeglib....%d\n", bytes_to_copy));
    memmove(src->cache->data, src->pub.next_input_byte, bytes_to_copy);
    src->cache->rp = 0;
    src->cache->wp = bytes_to_copy;
  }

  bytes_to_copy = PDF_MIN(in->wp - in->rp, src->cache->size - src->cache->wp);
  if(bytes_to_copy > 0)
  {
    memcpy(src->cache->data + src->cache->wp, in->data + in->rp, bytes_to_copy);
    {
      FILE *htmp = fopen("original.jpg", "ab+");
      fwrite(in->data + in->rp, bytes_to_copy, 1, htmp);
      fclose(htmp);
    }
    in->rp += bytes_to_copy;
    src->cache->wp += bytes_to_copy;
    debugmsg1(("copy data into cache....%d, left %d\n", bytes_to_copy, in->wp - in->rp));
  }

  debugmsg1(("<---leave src_fill..source buffer: %d, %d, %d\n", src->cache->size, src->cache->rp, src->cache->wp));
  src->pub.bytes_in_buffer = src->cache->wp - src->cache->rp;
  src->pub.next_input_byte = src->cache->data + src->cache->rp;
  return PDF_OK;
}

void 
pdf_stm_f_dct_src_fill_eoi(j_decompress_ptr cinfo)
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
pdf_stm_dct_dec_jpeg_cache_src_special (j_decompress_ptr cinfo, pdf_stm_buffer_t cache)
{
  pdf_stm_f_dct_cache_source_mgr_t src = (pdf_stm_f_dct_cache_source_mgr_t ) cinfo->src;

  src->pub.init_source = pdf_stm_dct_dec_init_source;
  src->pub.fill_input_buffer = pdf_stm_dct_dec_fill_input_buffer;
  src->pub.skip_input_data = pdf_stm_dct_dec_skip_input_data;
  src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
  src->pub.term_source = pdf_stm_dct_dec_term_source;
}

static void
pdf_stm_dct_dec_jpeg_cache_src (j_decompress_ptr cinfo, pdf_stm_buffer_t cache)
{
  pdf_stm_f_dct_cache_source_mgr_t src = (pdf_stm_f_dct_cache_source_mgr_t ) cinfo->src;

  if (cinfo->src == NULL) 
  { /* first time for this JPEG object? */
    cinfo->src = (struct jpeg_source_mgr *)
        (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(struct pdf_stm_f_dct_cache_source_mgr_s));
    src = (pdf_stm_f_dct_cache_source_mgr_t) cinfo->src;
  }

  src->cache = cache;

  src->pub.init_source = pdf_stm_dct_dec_init_source;
  src->pub.fill_input_buffer = pdf_stm_dct_dec_fill_input_buffer;
  src->pub.skip_input_data = pdf_stm_dct_dec_skip_input_data;
  src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
  src->pub.term_source = pdf_stm_dct_dec_term_source;
  src->pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
  src->pub.next_input_byte = NULL; /* until buffer loaded */
}
#endif


LOCAL(unsigned int)
  jpeg_getc (j_decompress_ptr cinfo)
  /* Read next byte */
{
    struct jpeg_source_mgr * datasrc = cinfo->src;

      if (datasrc->bytes_in_buffer == 0) {
            if (! (*datasrc->fill_input_buffer) (cinfo))
                    ERREXIT(cinfo, JERR_CANT_SUSPEND);
              }
        datasrc->bytes_in_buffer--;
          return GETJOCTET(*datasrc->next_input_byte++);
}



METHODDEF(boolean)
  print_text_marker (j_decompress_ptr cinfo)
{
    boolean traceit = (cinfo->err->trace_level >= 1);
    INT32 length;
    unsigned int ch;
    unsigned int lastch = 0;

   length = jpeg_getc(cinfo) << 8;
   length += jpeg_getc(cinfo);
   length -= 2;      /* discount the length word itself */

   if (traceit) {
      if (cinfo->unread_marker == JPEG_COM)
         fprintf(stderr, "Comment, length %ld:\n", (long) length);
      else      /* assume it is an APPn otherwise */
         fprintf(stderr, "APP%d, length %ld:\n",
         cinfo->unread_marker - JPEG_APP0, (long) length);
  }

   while (--length >= 0) {
     ch = jpeg_getc(cinfo);
     if (traceit) {
       /* Emit the character in a readable form.
       *        * Nonprintables are converted to \nnn form,
       *               * while \ is converted to \\.
       *                      * Newlines in CR, CR/LF, or LF form will be printed as one newline.
       *                             */
       if (ch == '\r') {
          fprintf(stderr, "\n");
       } else if (ch == '\n') {
          if (lastch != '\r')
            fprintf(stderr, "\n");
       } else if (ch == '\\') {
         fprintf(stderr, "\\\\");
       } else if (isprint(ch)) {
         putc(ch, stderr);
       } else {
         fprintf(stderr, "\\%03o", ch);
       }
       lastch = ch;
     }
   }

   if (traceit)
   fprintf(stderr, "\n");

   return TRUE;
}

static pdf_status_t
write_bmp_header (j_decompress_ptr cinfo, pdf_stm_buffer_t out)
  /* Write a Windows-style BMP file header, including colormap if needed */
{
  char bmpfileheader[14];
  char bmpinfoheader[40];
#define PUT_2B(array,offset,value)  \
  (array[offset] = (char) ((value) & 0xFF), \
  array[offset+1] = (char) (((value) >> 8) & 0xFF))
#define PUT_4B(array,offset,value)  \
  (array[offset] = (char) ((value) & 0xFF), \
  array[offset+1] = (char) (((value) >> 8) & 0xFF), \
  array[offset+2] = (char) (((value) >> 16) & 0xFF), \
  array[offset+3] = (char) (((value) >> 24) & 0xFF))
  INT32 headersize, bfSize;
  int bits_per_pixel, cmap_entries;

  /* check out buffer writable len */
  if(out->size - out->wp < 64)
  {
    return PDF_ENOUTPUT;
  }

  /* Compute colormap size and total file size */
  if (cinfo->out_color_space == JCS_RGB) {
    if (cinfo->quantize_colors) {
      /* Colormapped RGB */
      bits_per_pixel = 8;
      cmap_entries = 256;
    } else {
      /* Unquantized, full color RGB */
      bits_per_pixel = 24;
      cmap_entries = 0;
    }
  } else {
    /* Grayscale output.  We need to fake a 256-entry colormap. */
    bits_per_pixel = 8;
    cmap_entries = 256;
  }
  /* File size */
  headersize = 14 + 40 + cmap_entries * 4; /* Header and colormap */
  bfSize = headersize + (INT32) cinfo->output_width * (INT32) cinfo->output_height;

  /* Set unused fields of header to 0 */
  memset(bmpfileheader, 0, sizeof(bmpfileheader));
  memset(bmpinfoheader, 0, sizeof(bmpinfoheader));

  /* Fill the file header */
  bmpfileheader[0] = 0x42;  /* first 2 bytes are ASCII 'B', 'M' */
  bmpfileheader[1] = 0x4D;
  PUT_4B(bmpfileheader, 2, bfSize); /* bfSize */
  /* we leave bfReserved1 & bfReserved2 = 0 */
  PUT_4B(bmpfileheader, 10, headersize); /* bfOffBits */

  /* Fill the info header (Microsoft calls this a BITMAPINFOHEADER) */
  PUT_2B(bmpinfoheader, 0, 40); /* biSize */
  PUT_4B(bmpinfoheader, 4, cinfo->output_width); /* biWidth */
  PUT_4B(bmpinfoheader, 8, cinfo->output_height); /* biHeight */

  PUT_2B(bmpinfoheader, 12, 1); /* biPlanes - must be 1 */
  PUT_2B(bmpinfoheader, 14, bits_per_pixel); /* biBitCount */
  /* we leave biCompression = 0, for none */
  /* we leave biSizeImage = 0; this is correct for uncompressed data */
  if (cinfo->density_unit == 2) { /* if have density in dots/cm, then */
    PUT_4B(bmpinfoheader, 24, (INT32) (cinfo->X_density*100)); /* XPels/M */
    PUT_4B(bmpinfoheader, 28, (INT32) (cinfo->Y_density*100)); /* XPels/M */
  }
  PUT_2B(bmpinfoheader, 32, cmap_entries); /* biClrUsed */
  /* we leave biClrImportant = 0 */

  memcpy(out->data + out->wp, bmpfileheader, 14);
  out->wp += 14;
  memcpy(out->data + out->wp, bmpinfoheader, 40);
  out->wp += 40;
  return PDF_OK;
}
static pdf_status_t 
pdf_stm_dctdec_write_ppm_header(j_decompress_ptr cinfo, pdf_stm_buffer_t out)
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

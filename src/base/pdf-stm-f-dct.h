/* -*- mode: C -*- Time-stamp: "08/12/22 21:21:35 yangchanghua"
 *
 *       File:         pdf-stm-f-dct.h
 *       Date:         Mon Dec 13 21:21:35 2008
 *
 *       GNU PDF Library - DCT encoder/decoder
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

#ifndef PDF_STM_F_DCT_H
#define PDF_STM_F_DCT_H
#include <config.h>
#include <pdf-types.h>
#include <pdf-hash.h>
#include <pdf-stm-buffer.h>
#if defined(HAVE_JPEGLIB)
#include <jpeglib.h>

enum pdf_stm_f_dctstate_t
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
/* Internal state */
struct pdf_stm_f_dctenc_s
{
};

typedef struct pdf_stm_f_dctenc_s *pdf_stm_f_dctenc_t;

struct pdf_stm_f_dctdec_s
{
  struct jpeg_decompress_struct *cinfo;
  struct jpeg_error_mgr *jerr;

  enum pdf_stm_f_dctstate_t  state;
  enum pdf_stm_f_dctstate_t  backup_state;

  /* image cache for input data */
  pdf_stm_buffer_t djpeg_in;

  /* cache for output data */
  pdf_size_t row_stride;
  pdf_char_t **row_buf;
  pdf_size_t row_valid_size;
  pdf_size_t row_copy_index;
  pdf_u32_t num_scanlines;
};

typedef struct pdf_stm_f_dctdec_s *pdf_stm_f_dctdec_t;

/* Filters implementation API */

pdf_status_t pdf_stm_f_dctdec_init (pdf_hash_t params,
                                     void **state);
pdf_status_t pdf_stm_f_dctdec_apply (pdf_hash_t params,
                                      void *state,
                                      pdf_stm_buffer_t in,
                                      pdf_stm_buffer_t out,
                                      pdf_bool_t finish_p);
pdf_status_t pdf_stm_f_dctdec_dealloc_state (void *state);

pdf_status_t pdf_stm_f_dctenc_init (pdf_hash_t params,
                                     void **state);
pdf_status_t pdf_stm_f_dctenc_apply (pdf_hash_t params,
                                      void *state,
                                      pdf_stm_buffer_t in,
                                      pdf_stm_buffer_t out,
                                      pdf_bool_t finish_p);
pdf_status_t pdf_stm_f_dctenc_dealloc_state (void *state);
#endif /* HAVE_JPEGLIB */

#endif /* pdf_stm_f_dct.h */

/* End of pdf_stm_f_dct.h */

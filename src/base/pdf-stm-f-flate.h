/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-flate.h
 *       Date:         Tue Jul 10 23:39:27 2007
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

#ifndef PDF_STM_F_FLATE_H
#define PDF_STM_F_FLATE_H

#include <config.h>

#include <zlib.h>

#include <pdf-types.h>
#include <pdf-types-buffer.h>
#include <pdf-hash.h>

#define PDF_STM_F_FLATE_CHUNK 16384

struct pdf_stm_f_flate_s
{
  z_stream stream;
  int zret;
  pdf_size_t incnt, outcnt, to_write;
  pdf_bool_t writing_p;
  pdf_char_t inbuf[PDF_STM_F_FLATE_CHUNK], outbuf[PDF_STM_F_FLATE_CHUNK];
};

typedef struct pdf_stm_f_flate_s * pdf_stm_f_flate_t;


/* Filter API implementation */

pdf_status_t pdf_stm_f_flatedec_init (pdf_hash_t  *params,
                                      void       **state);

pdf_status_t pdf_stm_f_flatedec_apply (pdf_hash_t   *params,
                                       void         *state,
                                       pdf_buffer_t *in,
                                       pdf_buffer_t *out,
                                       pdf_bool_t    finish_p);

pdf_status_t pdf_stm_f_flateenc_init (pdf_hash_t  *params,
                                      void       **state);

pdf_status_t pdf_stm_f_flateenc_apply (pdf_hash_t   *params,
                                       void         *state,
                                       pdf_buffer_t *in,
                                       pdf_buffer_t *out,
                                       pdf_bool_t    finish_p);

pdf_status_t pdf_stm_f_flatedec_dealloc_state (void *state);
pdf_status_t pdf_stm_f_flateenc_dealloc_state (void *state);


#endif /* pdf_stm_f_flate.h */

/* End of pdf_stm_f_flate.h */

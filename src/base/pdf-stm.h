/* -*- mode: C -*- Time-stamp: "08/08/16 13:47:14 jemarch"
 *
 *       File:         pdf-stm.h
 *       Date:         Fri Jul  6 18:37:57 2007
 *
 *       GNU PDF Library - Streams
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

/* This library module implement stream objects capable to front-end
   to several backends: 

   - Open files implemented by the filesystem module (see pdf-fsys.h).
   - A memory buffer.

   Filters (such as the PDF standard ones) are supported for both
   reading and writing. Many filters can be used in a single stream
   (are applied in order when writing or reading).

 
                               +----------+
                               |          |<-----> pdf_fsys_file
      <--read----(filtering)---|          |
                               |  stream  |
       --write---(filtering)-->|          |
                               |          |<-----> memory buffer
                               +----------+
*/

#ifndef _PDF_STM_H
#define _PDF_STM_H

#include <config.h>

#include <stdio.h>

#include <pdf-types.h>
#include <pdf-hash.h>
#include <pdf-fsys.h>
#include <pdf-stm-buffer.h>
#include <pdf-stm-filter.h>
#include <pdf-stm-be.h>

/* BEGIN PUBLIC */

/* Default size for the stream caches */
#define PDF_STM_DEFAULT_CACHE_SIZE 4096

/* Mode to use when opening a stream */
enum pdf_stm_mode_e
{
  /* Reading stream */
  PDF_STM_READ,
  /* Writing stream */
  PDF_STM_WRITE
};

/* Types of streams */
enum pdf_stm_type_e
{
  PDF_STM_FILE,
  PDF_STM_MEM
};

/* Stream data type */
struct pdf_stm_s
{
  enum pdf_stm_type_e type;
  enum pdf_stm_mode_e mode;

  pdf_stm_be_t backend;     /* Stream backend */
  pdf_stm_filter_t filter;  /* Filter chain */
  pdf_stm_buffer_t cache;   /* Stream cache */
  pdf_size_t cache_used;    /* Number of octects used in the cache */
};

typedef struct pdf_stm_s *pdf_stm_t;

/* 
 * Stream API
 */

/* Creation and destruction */
pdf_status_t pdf_stm_file_new (pdf_fsys_file_t file,
                               pdf_off_t offset,
                               pdf_size_t cache_size,
                               enum pdf_stm_mode_e mode,
                               pdf_stm_t *stm);
pdf_status_t pdf_stm_mem_new (pdf_char_t *buffer,
                              pdf_size_t size,
                              pdf_size_t cache_size,
                              enum pdf_stm_mode_e mode,
                              pdf_stm_t *stm);
pdf_status_t pdf_stm_destroy (pdf_stm_t stm);

/* Reading and writing data */
pdf_size_t pdf_stm_read (pdf_stm_t stm,
                         pdf_char_t *buf,
                         pdf_size_t bytes);
pdf_size_t pdf_stm_write (pdf_stm_t stm,
                          const pdf_size_t elem_size,
                          const pdf_size_t elem_count,
                          pdf_char_t *buf);
pdf_u32_t pdf_stm_read_char (pdf_stm_t stm);
pdf_u32_t pdf_stm_peek_char (pdf_stm_t stm);
pdf_size_t pdf_stm_flush (pdf_stm_t stm);

/* Positioning */

pdf_off_t pdf_stm_seek (pdf_stm_t stm,
                        pdf_off_t pos);
pdf_off_t pdf_stm_tell (pdf_stm_t stm);


/* Management of the filter chain */

pdf_status_t pdf_stm_uninstall_filters (pdf_stm_t stm);
pdf_status_t pdf_stm_install_filter (pdf_stm_t stm,
                                     enum pdf_stm_filter_type_e filter_type,
                                     pdf_hash_t filter_params);
/* END PUBLIC */

#endif /* pdf_stm.h */

/* End of pdf_stm.h */

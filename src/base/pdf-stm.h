/* -*- mode: C -*-
 *
 *       File:         pdf-stm.h
 *       Date:         Fri Jul  6 18:37:57 2007
 *
 *       GNU PDF Library - Streams
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
#include <pdf-types-buffer.h>
#include <pdf-hash.h>
#include <pdf-fsys.h>
#include <pdf-stm-filter.h>
#include <pdf-stm-be.h>

/* BEGIN PUBLIC */

/* --------------------- Stream Module Initialization ------------------------- */

/* Default size for the stream caches */
#define PDF_STM_DEFAULT_CACHE_SIZE 4096

/* Mode to use when opening a stream */
enum pdf_stm_mode_e
{
  PDF_STM_UNKNOWN = -1,
  /* Reading stream */
  PDF_STM_READ = 0,
  /* Writing stream */
  PDF_STM_WRITE
};

/* Types of streams */
enum pdf_stm_type_e
{
  /* File stream */
  PDF_STM_FILE,
  /* Memory stream */
  PDF_STM_MEM
};

/* The opaque stream object */
typedef struct pdf_stm_s pdf_stm_t;

/* --------------------- Stream Creation and Destruction -------------------- */

/* Create a new file stream from a C file handler */
pdf_stm_t *pdf_stm_cfile_new (FILE                 *file,
                              pdf_off_t             offset,
                              pdf_size_t            cache_size,
                              enum pdf_stm_mode_e   mode,
                              pdf_error_t         **error);

/* Create a new file stream from a file object */
pdf_stm_t *pdf_stm_file_new (pdf_fsys_file_t       file,
                             pdf_off_t             offset,
                             pdf_size_t            cache_size,
                             enum pdf_stm_mode_e   mode,
                             pdf_error_t         **error);

/* Create a new memory stream */
pdf_stm_t *pdf_stm_mem_new (pdf_uchar_t          *buffer,
                            pdf_size_t            size,
                            pdf_size_t            cache_size,
                            enum pdf_stm_mode_e   mode,
                            pdf_error_t         **error);

/* Destroy the stream */
void pdf_stm_destroy (pdf_stm_t *stm);

/* ------------------- Getting and Setting Stream properties ---------------- */

enum pdf_stm_mode_e pdf_stm_get_mode (pdf_stm_t *stm);

/* ------------------- Reading and Writing data in the Streams -------------- */

pdf_bool_t pdf_stm_read (pdf_stm_t     *stm,
                         pdf_uchar_t   *buf,
                         pdf_size_t     bytes,
                         pdf_size_t    *read_bytes,
                         pdf_error_t  **error);

pdf_bool_t pdf_stm_write (pdf_stm_t          *stm,
                          const pdf_uchar_t  *buf,
                          pdf_size_t          bytes,
                          pdf_size_t         *written_bytes,
                          pdf_error_t       **error);

pdf_bool_t pdf_stm_flush (pdf_stm_t    *stm,
                          pdf_bool_t    finish,
                          pdf_size_t   *flushed_bytes,
                          pdf_error_t **error);

pdf_bool_t pdf_stm_read_char (pdf_stm_t    *stm,
                              pdf_uchar_t  *read_char,
                              pdf_error_t **error);

pdf_bool_t pdf_stm_peek_char (pdf_stm_t    *stm,
                              pdf_uchar_t  *read_char,
                              pdf_error_t **error);

/* ------------------- Stream positionining -------------------------------- */

pdf_off_t pdf_stm_bseek (pdf_stm_t *stm,
                         pdf_off_t  pos);

pdf_off_t pdf_stm_btell (pdf_stm_t *stm);

pdf_off_t pdf_stm_tell (pdf_stm_t *stm);


/* ------------------- Management of the Stream filter chain --------------- */

pdf_bool_t pdf_stm_install_filter     (pdf_stm_t                   *stm,
                                       enum pdf_stm_filter_type_e   filter_type,
                                       const pdf_hash_t            *filter_params,
                                       pdf_error_t                **error);

pdf_bool_t pdf_stm_supported_filter_p (enum pdf_stm_filter_type_e   filter_type);
/* END PUBLIC */

/* Stream data type, opaque object */
struct pdf_stm_s
{
  enum pdf_stm_type_e type;
  enum pdf_stm_mode_e mode;

  pdf_stm_be_t     *backend;     /* Stream backend */
  pdf_stm_filter_t *filter;      /* Filter chain */
  pdf_buffer_t     *cache;       /* Stream cache */
  pdf_off_t         seq_counter; /* Number of octects read/written in the
                                  * current sequential
                                  * operation. i.e. since the last bseek or
                                  * the creation of the stream */
};

#endif /* pdf_stm.h */

/* End of pdf_stm.h */

/* -*- mode: C -*- Time-stamp: "10/02/20 15:33:32 jemarch"
 *
 *       File:         pdf-fsys-http.h
 *       Date:         Thu Dec 17 18:22:59 2009
 *
 *       GNU PDF Library - HTTP Filesystem Implementation
 *
 */

/* Copyright (C) 2009, 2010 Free Software Foundation, Inc. */

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

#ifndef PDF_FSYS_HTTP_H
#define PDF_FSYS_HTTP_H

/*
 * Read-Only HTTP filesystem implementation with Read In Advance
 */

#include <pdf-fsys.h>

/* BEGIN PUBLIC */

/* END PUBLIC */

/* Filesystem internal data associated with open files */
struct pdf_fsys_http_file_s
{
  pdf_char_t *    host_path;       /* Path name used to open the file, host
                                      encoded version */
  pdf_size_t      host_path_size;  /* Size of the host encoded path */
  void *          buffer;          /* Destination buffer for downloaded data */
  pdf_i64_t       write_idx;       /* write index for buffer */
  pdf_i64_t       read_idx;        /* read index for buffer */
  pdf_fsys_file_t temp_file;       /* temp file for downloaded data */
  pdf_bool_t      temp_file_sized; /* temp file size has been set */
  pdf_i64_t       temp_file_size;  /* size that temp file has been set to */
  pdf_bool_t      open;            /* This pdf_fsys_http_file_s is open */
};

typedef struct pdf_fsys_http_file_s *pdf_fsys_http_file_t;



/* --------------------- Filesystem interface ------------------------- */




/* --------------------- File interface ------------------------- */

pdf_status_t 
pdf_fsys_http_file_set_size (pdf_fsys_file_t file, pdf_size_t size);




#endif /* !PDF_FSYS_HTTP_H */

/* End of pdf-fsys-http.h */

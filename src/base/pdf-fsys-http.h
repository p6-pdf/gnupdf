/* -*- mode: C -*-
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

#include <curl/curl.h>

#include <pthread.h>

/* BEGIN PUBLIC */

extern const struct pdf_fsys_impl_s pdf_fsys_http_implementation;

/* END PUBLIC */

/* Filesystem internal data associated with open files */
struct pdf_fsys_http_file_s
{
  pdf_char_t     *host_path;       /* Path name used to open the file, host
                                      encoded version */
  pdf_size_t      host_path_size;  // Size of the host encoded path 
  pdf_size_t      write_idx;       // write index for temp_file
  pdf_size_t      read_idx;        // read index for temp_file
  pdf_fsys_file_t temp_file;       // temp file for downloaded data
  pdf_bool_t      temp_file_sized; // temp file size has been set
  pdf_size_t      temp_file_size;  // size that temp file has been set to
  pdf_bool_t      downloading;     // libcurl is actively downloading the file
  pdf_bool_t      open;            // This pdf_fsys_http_file_s is open
  CURL            *curl;           // The libcurl object associated with file
  pthread_mutex_t mutex;           // mutex to lock file object.  
};

typedef struct pdf_fsys_http_file_s *pdf_fsys_http_file_t;


/* Filesystem internal data - for the whole http filesystem */

struct pdf_fsys_http_data_s
{
  pdf_status_t           init_stat; 
  pthread_t              worker;
  CURLM                 *curlm;
  pthread_mutex_t        mutex;     // mutex to lock the file_list object.
  pdf_list_t             file_list;
  int                    running_handles;
  volatile pdf_bool_t    terminate; // volatile so never cached in a register
};

typedef struct pdf_fsys_http_data_s *pdf_fsys_http_data_t;


/* --------------------- Filesystem interface ------------------------- */

/* Filesystem initialization and cleanup */
pdf_status_t
pdf_fsys_http_init (void **data);

pdf_status_t
pdf_fsys_http_cleanup (void *data);

/* Filesystem implementation */
pdf_status_t
pdf_fsys_http_create_folder (void *data,
                             pdf_text_t path_name);

pdf_status_t
pdf_fsys_http_get_folder_contents (void *data,
                                   pdf_text_t path_name,
                                   pdf_list_t item_list);

pdf_status_t
pdf_fsys_http_get_parent (void *data,
                          pdf_text_t path_name,
                          pdf_text_t parent_path);

pdf_status_t
pdf_fsys_http_remove_folder (void *data,
                             pdf_text_t path_name);

pdf_status_t
pdf_fsys_http_get_item_props (void *data,
                              pdf_text_t path_name,
                              struct pdf_fsys_item_props_s *props);

pdf_i64_t
pdf_fsys_http_get_free_space (void *data,
                              pdf_text_t path_name);

pdf_bool_t
pdf_fsys_http_item_p (void *data,
                      pdf_text_t path_name);

pdf_bool_t
pdf_fsys_http_item_readable_p (void *data,
                               pdf_text_t path_name);

pdf_bool_t
pdf_fsys_http_item_writable_p (void *data,
                               pdf_text_t path_name);

/* --------------------- File interface ------------------------- */

pdf_status_t
pdf_fsys_http_file_read (pdf_fsys_file_t file, 
                         pdf_char_t *buf,
                         pdf_size_t bytes, 
                         pdf_size_t *read_bytes);

pdf_status_t
pdf_fsys_http_file_write (pdf_fsys_file_t file, 
                          pdf_char_t *buf,
                          pdf_size_t bytes, 
                          pdf_size_t *written_bytes);

pdf_status_t
pdf_fsys_http_file_flush (pdf_fsys_file_t file);

pdf_bool_t
pdf_fsys_http_file_can_set_size_p (pdf_fsys_file_t file,
                                   pdf_size_t pos);

pdf_status_t
pdf_fsys_http_file_open (void *data,
                         pdf_text_t path_name,
                         enum pdf_fsys_file_mode_e mode,
                         pdf_fsys_file_t *p_file);

pdf_status_t
pdf_fsys_http_file_open_tmp (void *data,
                             pdf_fsys_file_t *p_file);

pdf_size_t
pdf_fsys_http_file_get_size (pdf_fsys_file_t file);

pdf_status_t
pdf_fsys_http_file_set_size (pdf_fsys_file_t file,
                             pdf_size_t pos);

pdf_status_t
pdf_fsys_http_file_get_pos (pdf_fsys_file_t file,
                            pdf_size_t *pos);

pdf_status_t
pdf_fsys_http_file_set_pos (pdf_fsys_file_t file,
                            pdf_size_t pos);

enum pdf_fsys_file_mode_e
pdf_fsys_http_file_get_mode (pdf_fsys_file_t file);

pdf_status_t
pdf_fsys_http_file_set_mode (pdf_fsys_file_t file,
                             enum pdf_fsys_file_mode_e new_mode);

pdf_text_t
pdf_fsys_http_file_get_url (pdf_fsys_file_t file);

pdf_bool_t
pdf_fsys_http_file_same_p (pdf_fsys_file_t file,
                           pdf_text_t path_name);

pdf_status_t
pdf_fsys_http_file_request_ria (pdf_fsys_file_t file,
                                pdf_size_t offset,
                                pdf_size_t count);

pdf_bool_t
pdf_fsys_http_file_has_ria (pdf_fsys_file_t file);

pdf_status_t
pdf_fsys_http_file_cancel_ria (pdf_fsys_file_t file);

pdf_status_t
pdf_fsys_http_file_close (pdf_fsys_file_t file);

pdf_status_t
pdf_fsys_http_file_reopen (pdf_fsys_file_t file,
                           enum pdf_fsys_file_mode_e mode);


#endif /* !PDF_FSYS_HTTP_H */

/* End of pdf-fsys-http.h */

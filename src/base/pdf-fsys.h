/* -*- mode: C -*- Time-stamp: "10/02/17 20:22:09 jemarch"
 *
 *       File:         pdf-fsys.h
 *       Date:         Thu May 22 15:49:59 2008
 *
 *       GNU PDF Library - Filesystem Module
 *
 */

/* Copyright (C) 2008, 2009, 2010 Free Software Foundation, Inc. */

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

#ifndef PDF_FSYS_H
#define PDF_FSYS_H

#include <config.h>

#include <pdf-types.h>
#include <pdf-list.h>
#include <pdf-hash.h>
#include <pdf-text.h>
#include <pdf-time.h>

/*
 * Data Types
 */

/* BEGIN PUBLIC */

/* Filesystem item properties */
struct pdf_fsys_item_props_s
{
  pdf_bool_t is_hidden;
  pdf_bool_t is_readable;
  pdf_bool_t is_writable;
  pdf_time_t creation_date;
  pdf_time_t modification_date;
  pdf_u32_t file_size_high;
  pdf_u32_t file_size_low;
  pdf_u32_t folder_size;
};

/* Filesystem */
struct pdf_fsys_impl_s;
struct pdf_fsys_s
{
  struct pdf_fsys_impl_s *implementation; /* Implementation of the
                                           * filesystem */

  void *data; /* Private data used by the underlying filesystem
                 implementation.  */
};

typedef struct pdf_fsys_s *pdf_fsys_t;

/* Open files */
struct pdf_fsys_file_s
{
  pdf_fsys_t fs; /* Underlying filesystem */
  void *data;    /* Private data used by the underlying filesystem
                  * implementation */
};

typedef struct pdf_fsys_file_s *pdf_fsys_file_t;

/* File modes */
enum pdf_fsys_file_mode_e 
  {
    /* First valid element of the enum, useful for iterating */
    PDF_FSYS_OPEN_MODE_FIRST = 0,
    /* Invalid file mode identifier */
    PDF_FSYS_OPEN_MODE_INVALID = PDF_FSYS_OPEN_MODE_FIRST,
    /* The file is opened in read-only mode. The file should exist in
     * the filesystem */
    PDF_FSYS_OPEN_MODE_READ = 1,
    /* The file is opened in write-only mode. The file is truncated to
     * 0 size (any previous contents are lost) if the file exist and
     * it is created if it do not exist */
    PDF_FSYS_OPEN_MODE_WRITE = 2,
    /* The file is opened in read/write mode. The file should exist in
     * the filesystem */
    PDF_FSYS_OPEN_MODE_RW = 3,
    /* Last valid element of the enum, useful for iterating */
    PDF_FSYS_OPEN_MODE_LAST = PDF_FSYS_OPEN_MODE_RW,
    /* Helper to define array size based on this enum */
    PDF_FSYS_OPEN_MODE_MAX,
  };

/* Filesystem initialization and cleanup callbacks.  */
typedef pdf_status_t (*pdf_fsys_init_fn_t) (void **data);
typedef pdf_status_t (*pdf_fsys_cleanup_fn_t) (void *data);

/* Filesystem implementation callbacks */
typedef pdf_status_t (*pdf_fsys_create_folder_fn_t) (void *data,
                                                     pdf_text_t path_name);
typedef pdf_status_t (*pdf_fsys_get_folder_contents_fn_t) (void *data,
                                                           pdf_text_t path_name,
                                                           pdf_list_t item_list);
typedef pdf_status_t (*pdf_fsys_get_parent_fn_t) (void *data,
                                                  pdf_text_t path_name,
                                                  pdf_text_t parent_path);
typedef pdf_status_t (*pdf_fsys_remove_folder_fn_t) (void *data,
                                                     pdf_text_t path_name);
typedef pdf_status_t (*pdf_fsys_get_item_props_fn_t) (void *data,
                                                      pdf_text_t path_name,
                                                      struct pdf_fsys_item_props_s *props);
typedef pdf_i64_t (*pdf_fsys_get_free_space_fn_t) (void *data,
                                                   pdf_text_t path_name);
typedef pdf_bool_t (*pdf_fsys_item_p_fn_t) (void *data,
                                            pdf_text_t path_name);
typedef pdf_bool_t (*pdf_fsys_item_readable_p_fn_t) (void *data,
                                                     pdf_text_t path_name);
typedef pdf_bool_t (*pdf_fsys_item_writable_p_fn_t) (void *data,
                                                     pdf_text_t path_name);

typedef pdf_status_t
(*pdf_fsys_file_read_fn_t) (pdf_fsys_file_t file, pdf_char_t *buf,
                            pdf_size_t bytes, pdf_size_t *read_bytes);

typedef pdf_status_t
(*pdf_fsys_file_write_fn_t) (pdf_fsys_file_t file, pdf_char_t *buf,
                             pdf_size_t bytes, pdf_size_t *written_bytes);

typedef pdf_status_t (*pdf_fsys_file_flush_fn_t) (pdf_fsys_file_t file);
typedef pdf_bool_t (*pdf_fsys_file_can_set_size_p_fn_t) (pdf_fsys_file_t file,
                                                       pdf_size_t pos);
typedef pdf_status_t (*pdf_fsys_file_open_fn_t) (void *data,
                                                 pdf_text_t path_name,
                                                 enum pdf_fsys_file_mode_e mode,
                                                 pdf_fsys_file_t *p_file);
typedef pdf_status_t (*pdf_fsys_file_open_tmp_fn_t) (void *data,
                                                     pdf_fsys_file_t *p_file);
typedef pdf_size_t (*pdf_fsys_file_get_size_fn_t) (pdf_fsys_file_t file);
typedef pdf_status_t (*pdf_fsys_file_set_size_fn_t) (pdf_fsys_file_t file,
                                                     pdf_size_t pos);
typedef pdf_status_t (*pdf_fsys_file_get_pos_fn_t) (pdf_fsys_file_t file,
                                                    pdf_size_t *pos);
typedef pdf_status_t (*pdf_fsys_file_set_pos_fn_t) (pdf_fsys_file_t file,
                                                    pdf_size_t pos);
typedef enum pdf_fsys_file_mode_e (*pdf_fsys_file_get_mode_fn_t) (pdf_fsys_file_t file);
typedef pdf_status_t (*pdf_fsys_file_set_mode_fn_t) (pdf_fsys_file_t file,
                                                     enum pdf_fsys_file_mode_e new_mode);
typedef pdf_text_t (*pdf_fsys_file_get_url_fn_t) (pdf_fsys_file_t file);
typedef pdf_bool_t (*pdf_fsys_file_same_p_fn_t) (pdf_fsys_file_t file,
                                                 pdf_text_t path_name);
typedef pdf_status_t (*pdf_fsys_file_request_ria_fn_t) (pdf_fsys_file_t file,
                                                        pdf_size_t offset,
                                                        pdf_size_t count);
typedef pdf_bool_t (*pdf_fsys_file_has_ria_fn_t) (pdf_fsys_file_t file);
typedef pdf_status_t (*pdf_fsys_file_cancel_ria_fn_t) (pdf_fsys_file_t file);
typedef pdf_status_t (*pdf_fsys_file_close_fn_t) (pdf_fsys_file_t file);
typedef pdf_status_t (*pdf_fsys_file_reopen_fn_t) (pdf_fsys_file_t file,
                                                   enum pdf_fsys_file_mode_e mode);

/* Filesystem implementation */
struct pdf_fsys_impl_s
{
  /* Initialization and cleanup callbacks.  */
  pdf_fsys_init_fn_t init_fn;
  pdf_fsys_cleanup_fn_t cleanup_fn;

  /* Filesystem interface callbacks */
  pdf_fsys_create_folder_fn_t create_folder_fn;
  pdf_fsys_get_folder_contents_fn_t get_folder_contents_fn;
  pdf_fsys_get_parent_fn_t get_parent_fn;
  pdf_fsys_remove_folder_fn_t remove_folder_fn;
  pdf_fsys_get_item_props_fn_t get_item_props_fn;
  pdf_fsys_get_free_space_fn_t get_free_space_fn;
  pdf_fsys_item_p_fn_t item_p_fn;
  pdf_fsys_item_readable_p_fn_t item_readable_p_fn;
  pdf_fsys_item_writable_p_fn_t item_writable_p_fn;
  pdf_fsys_file_get_url_fn_t file_get_url_fn;

  /* File interface callbacks */
  pdf_fsys_file_open_fn_t file_open_fn;
  pdf_fsys_file_open_tmp_fn_t file_open_tmp_fn;
  pdf_fsys_file_read_fn_t file_read_fn;
  pdf_fsys_file_write_fn_t file_write_fn;
  pdf_fsys_file_flush_fn_t file_flush_fn;
  pdf_fsys_file_can_set_size_p_fn_t file_can_set_size_p_fn;
  pdf_fsys_file_get_size_fn_t file_get_size_fn;
  pdf_fsys_file_set_size_fn_t file_set_size_fn;
  pdf_fsys_file_get_pos_fn_t file_get_pos_fn;
  pdf_fsys_file_set_pos_fn_t file_set_pos_fn;
  pdf_fsys_file_get_mode_fn_t file_get_mode_fn;
  pdf_fsys_file_set_mode_fn_t file_set_mode_fn;
  pdf_fsys_file_same_p_fn_t file_same_p_fn;
  pdf_fsys_file_request_ria_fn_t file_request_ria_fn;
  pdf_fsys_file_has_ria_fn_t file_has_ria_fn;
  pdf_fsys_file_cancel_ria_fn_t file_cancel_ria_fn;
  pdf_fsys_file_close_fn_t file_close_fn;
  pdf_fsys_file_reopen_fn_t file_reopen_fn;
};

/*
 * Function Declarations
 */

/* Filesystem Interface */

pdf_i64_t pdf_fsys_get_free_space (pdf_fsys_t filesystem,
                                   pdf_text_t path_name);
pdf_status_t pdf_fsys_create_folder (const pdf_fsys_t filesystem,
                                     const pdf_text_t path_name);
pdf_status_t pdf_fsys_get_folder_contents (const pdf_fsys_t filesystem,
                                           const pdf_text_t path_name,
                                           pdf_list_t item_list);
pdf_status_t pdf_fsys_get_parent (const pdf_fsys_t filesystem,
                                  const pdf_text_t path_name,
                                  pdf_text_t parent_path);
pdf_status_t pdf_fsys_remove_folder (const pdf_fsys_t filesystem,
                                     const pdf_text_t path_name);
pdf_status_t pdf_fsys_get_item_props (pdf_fsys_t filesystem,
                                      pdf_text_t path_name,
                                      struct pdf_fsys_item_props_s *item_props);
pdf_status_t pdf_fsys_item_props_to_hash (const struct pdf_fsys_item_props_s item_props,
                                          pdf_hash_t props_hash);
pdf_bool_t pdf_fsys_item_p (pdf_fsys_t filesystem,
                            pdf_text_t path_name);
pdf_bool_t pdf_fsys_item_readable_p (pdf_fsys_t filesystem,
                                     pdf_text_t path_name);
pdf_bool_t pdf_fsys_item_writable_p (pdf_fsys_t filesystem,
                                     pdf_text_t path_name);

/* File Interface */
pdf_status_t pdf_fsys_file_open (const pdf_fsys_t filesystem,
                                 const pdf_text_t path_name,
                                 const enum pdf_fsys_file_mode_e mode,
                                 pdf_fsys_file_t *p_file);

pdf_status_t pdf_fsys_file_open_tmp (const pdf_fsys_t filesystem,
                                     pdf_fsys_file_t *p_file);

pdf_fsys_t pdf_fsys_file_get_filesystem (pdf_fsys_file_t file);
enum pdf_fsys_file_mode_e pdf_fsys_file_get_mode (pdf_fsys_file_t file);
pdf_text_t pdf_fsys_file_get_url (pdf_fsys_file_t file);
pdf_status_t pdf_fsys_file_set_mode (pdf_fsys_file_t file,
                                     enum pdf_fsys_file_mode_e new_mode);
pdf_bool_t pdf_fsys_file_same_p (pdf_fsys_file_t file,
                                 pdf_text_t path);
pdf_status_t pdf_fsys_file_get_pos (pdf_fsys_file_t file,
                                    pdf_size_t *pos);
pdf_status_t pdf_fsys_file_set_pos (pdf_fsys_file_t file,
                                    pdf_size_t new_pos);
pdf_bool_t pdf_fsys_file_can_set_size_p (pdf_fsys_file_t file,
                                         pdf_size_t size);
pdf_size_t pdf_fsys_file_get_size (pdf_fsys_file_t file);
pdf_status_t pdf_fsys_file_set_size (pdf_fsys_file_t file,
                                     pdf_size_t size);

pdf_status_t pdf_fsys_file_read (pdf_fsys_file_t file, pdf_char_t *buf,
                                 pdf_size_t bytes, pdf_size_t *read_bytes);

pdf_status_t pdf_fsys_file_write (pdf_fsys_file_t file, pdf_char_t *buf,
                                  pdf_size_t bytes, pdf_size_t *written_bytes);

pdf_status_t pdf_fsys_file_flush (pdf_fsys_file_t file);
pdf_status_t pdf_fsys_file_request_ria (pdf_fsys_file_t file,
                                        pdf_size_t offset,
                                        pdf_size_t count);
pdf_bool_t pdf_fsys_file_has_ria (pdf_fsys_file_t file);
pdf_status_t pdf_fsys_file_cancel_ria (pdf_fsys_file_t file);
pdf_status_t pdf_fsys_file_close (pdf_fsys_file_t file);
pdf_status_t pdf_fsys_file_reopen (pdf_fsys_file_t file,
                                   enum pdf_fsys_file_mode_e mode);

/* Filesystem Definition Interface */

pdf_fsys_t pdf_fsys_create (struct pdf_fsys_impl_s implementation);
pdf_status_t pdf_fsys_destroy (pdf_fsys_t filesystem);

/* END PUBLIC */

#endif /* ! PDF_FSYS_H */

/* End of pdf-fsys.h */

/* -*- mode: C -*-
 *
 *       File:         pdf-fsys.h
 *       Date:         Thu May 22 15:49:59 2008
 *
 *       GNU PDF Library - Filesystem Module
 *
 */

/* Copyright (C) 2008-2011 Free Software Foundation, Inc. */

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

#include <stdarg.h>
#include <pdf-types.h>
#include <pdf-list.h>
#include <pdf-hash.h>
#include <pdf-text.h>
#include <pdf-time.h>

/* ------------------ Filesystem Module initialization ---------------------- */

/* Initializes Filesystem module and all the built-in filesystem
 * implementations */
pdf_bool_t pdf_fsys_init (pdf_error_t **error);

/* Deinitializes Filesystem Module, in case a clean exit is wanted */
void pdf_fsys_deinit (void);

/* BEGIN PUBLIC */

/* -------------------------- Filesystem Data Types ------------------------- */

/* Filesystem item properties */
struct pdf_fsys_item_props_s
{
  pdf_bool_t is_hidden;
  pdf_bool_t is_readable;
  pdf_bool_t is_writable;
  pdf_time_t creation_date;
  pdf_time_t modification_date;
  pdf_off_t file_size;
};

/* Key names of filesystem item properties when available in a hash table */
#define PDF_FSYS_ITEM_IS_HIDDEN         "isHidden"
#define PDF_FSYS_ITEM_IS_READABLE       "isReadable"
#define PDF_FSYS_ITEM_IS_WRITABLE       "isWritable"
#define PDF_FSYS_ITEM_FILE_SIZE         "fileSize"
#define PDF_FSYS_ITEM_CREATION_DATE     "creationDate"
#define PDF_FSYS_ITEM_MODIFICATION_DATE "modificationDate"

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

/* ---------------------- Filesystem Implementation API --------------------- */

typedef struct pdf_fsys_s pdf_fsys_t;
typedef struct pdf_fsys_file_s pdf_fsys_file_t;

/* ------ (Filesystem) Filesystem Properties */

typedef pdf_i64_t (*pdf_fsys_get_free_space_fn_t) (const pdf_fsys_t  *fsys,
                                                   const pdf_text_t  *path,
                                                   pdf_error_t      **error);

/* ------ (Filesystem) Folder Management */

typedef pdf_bool_t (*pdf_fsys_create_folder_fn_t) (const pdf_fsys_t  *fsys,
                                                   const pdf_text_t  *path,
                                                   pdf_error_t      **error);
typedef pdf_list_t * (*pdf_fsys_get_folder_contents_fn_t) (const pdf_fsys_t  *fsys,
                                                           const pdf_text_t  *path,
                                                           pdf_error_t      **error);

typedef pdf_bool_t (*pdf_fsys_remove_folder_fn_t) (const pdf_fsys_t  *fsys,
                                                   const pdf_text_t  *path,
                                                   pdf_error_t      **error);

/* ------ (Filesystem) Paths and URLs */

typedef pdf_text_t * (*pdf_fsys_get_parent_fn_t) (const pdf_fsys_t  *fsys,
                                                  const pdf_text_t  *path,
                                                  pdf_error_t      **error);

typedef pdf_text_t * (*pdf_fsys_get_basename_fn_t) (const pdf_fsys_t  *fsys,
                                                    const pdf_text_t  *path,
                                                    pdf_error_t      **error);

typedef pdf_bool_t (*pdf_fsys_compare_path_p_fn_t) (const pdf_fsys_t  *fsys,
                                                    const pdf_text_t  *first,
                                                    const pdf_text_t  *second,
                                                    pdf_error_t      **error);

typedef pdf_text_t *(*pdf_fsys_build_path_fn_t) (const pdf_fsys_t  *fsys,
                                                 pdf_error_t      **error,
                                                 const pdf_text_t  *first_element,
                                                 ...);

typedef pdf_char_t *(*pdf_fsys_get_url_from_path_fn_t) (const pdf_fsys_t  *fsys,
                                                        const pdf_text_t  *path,
                                                        pdf_error_t      **error);

typedef pdf_text_t *(*pdf_fsys_get_path_from_url_fn_t) (const pdf_fsys_t  *fsys,
                                                        const pdf_char_t  *url,
                                                        pdf_error_t      **error);

/* ------ (Filesystem) Filesystem Item Properties */

typedef pdf_bool_t (*pdf_fsys_item_p_fn_t) (const pdf_fsys_t *fsys,
                                            const pdf_text_t *path);

typedef pdf_bool_t (*pdf_fsys_get_item_props_fn_t) (const pdf_fsys_t              *fsys,
                                                    const pdf_text_t              *path,
                                                    struct pdf_fsys_item_props_s  *props,
                                                    pdf_error_t                  **error);

typedef pdf_bool_t (*pdf_fsys_item_readable_p_fn_t) (const pdf_fsys_t *fsys,
                                                     const pdf_text_t *path);

typedef pdf_bool_t (*pdf_fsys_item_writable_p_fn_t) (const pdf_fsys_t *fsys,
                                                     const pdf_text_t *path);

/* ------ (File) Opening and Closing Files */

typedef pdf_fsys_file_t * (*pdf_fsys_file_open_fn_t) (const pdf_fsys_t           *fsys,
                                                      const pdf_text_t           *path,
                                                      enum pdf_fsys_file_mode_e   mode,
                                                      pdf_error_t               **error);

typedef pdf_fsys_file_t * (*pdf_fsys_file_open_tmp_fn_t) (const pdf_fsys_t  *fsys,
                                                          pdf_error_t      **error);

typedef pdf_bool_t (*pdf_fsys_file_reopen_fn_t) (pdf_fsys_file_t            *file,
                                                 enum pdf_fsys_file_mode_e   mode,
                                                 pdf_error_t               **error);

typedef pdf_bool_t (*pdf_fsys_file_close_fn_t) (pdf_fsys_file_t  *file,
                                                pdf_error_t     **error);

/* ------ (File) File Positions */

typedef pdf_off_t (*pdf_fsys_file_get_pos_fn_t) (const pdf_fsys_file_t  *file,
                                                 pdf_error_t           **error);

typedef pdf_bool_t (*pdf_fsys_file_set_pos_fn_t) (pdf_fsys_file_t  *file,
                                                  pdf_off_t         pos,
                                                  pdf_error_t     **error);

/* ------ (File) File Size */

typedef pdf_off_t (*pdf_fsys_file_get_size_fn_t) (const pdf_fsys_file_t  *file,
                                                  pdf_error_t           **error);

typedef pdf_bool_t (*pdf_fsys_file_can_set_size_p_fn_t) (const pdf_fsys_file_t *file,
                                                         pdf_off_t              size);

typedef pdf_bool_t (*pdf_fsys_file_set_size_fn_t) (pdf_fsys_file_t  *file,
                                                   pdf_off_t         size,
                                                   pdf_error_t     **error);

/* ------ (File) Synchronous Input/Output */

typedef pdf_bool_t (*pdf_fsys_file_read_fn_t) (pdf_fsys_file_t  *file,
                                               pdf_char_t       *buf,
                                               pdf_size_t        bytes,
                                               pdf_size_t       *read_bytes,
                                               pdf_error_t     **error);

typedef pdf_bool_t (*pdf_fsys_file_write_fn_t) (pdf_fsys_file_t  *file,
                                                const pdf_char_t *buf,
                                                pdf_size_t        bytes,
                                                pdf_size_t       *written_bytes,
                                                pdf_error_t     **error);

typedef pdf_bool_t (*pdf_fsys_file_flush_fn_t) (pdf_fsys_file_t  *file,
                                                pdf_error_t     **error);

/* ------ (File) Read In Advance */

typedef pdf_u32_t (*pdf_fsys_file_request_ria_fn_t) (pdf_fsys_file_t  *file,
                                                     pdf_off_t         offset,
                                                     pdf_size_t        count,
                                                     pdf_error_t     **error);

typedef pdf_bool_t (*pdf_fsys_file_has_ria_fn_t) (pdf_fsys_file_t *file);

typedef pdf_bool_t (*pdf_fsys_file_cancel_ria_fn_t) (pdf_fsys_file_t  *file,
                                                     pdf_u32_t         ria_id,
                                                     pdf_error_t     **error);

/* Filesystem implementation */
struct pdf_fsys_s
{
  /* --- Filesystem interface methods --- */
  pdf_fsys_get_free_space_fn_t get_free_space_fn;
  pdf_fsys_create_folder_fn_t create_folder_fn;
  pdf_fsys_get_folder_contents_fn_t get_folder_contents_fn;
  pdf_fsys_remove_folder_fn_t remove_folder_fn;
  pdf_fsys_get_parent_fn_t get_parent_fn;
  pdf_fsys_get_basename_fn_t get_basename_fn;
  pdf_fsys_compare_path_p_fn_t compare_path_p_fn;
  pdf_fsys_build_path_fn_t build_path_fn;
  pdf_fsys_get_url_from_path_fn_t get_url_from_path_fn;
  pdf_fsys_get_path_from_url_fn_t get_path_from_url_fn;
  pdf_fsys_item_p_fn_t item_p_fn;
  pdf_fsys_get_item_props_fn_t get_item_props_fn;
  pdf_fsys_item_readable_p_fn_t item_readable_p_fn;
  pdf_fsys_item_writable_p_fn_t item_writable_p_fn;
  /*    <Padding> */
  void *fsys_padding[32];


  /* --- File interface methods --- */
  pdf_fsys_file_open_fn_t file_open_fn;
  pdf_fsys_file_open_tmp_fn_t file_open_tmp_fn;
  pdf_fsys_file_reopen_fn_t file_reopen_fn;
  pdf_fsys_file_close_fn_t file_close_fn;
  pdf_fsys_file_get_pos_fn_t file_get_pos_fn;
  pdf_fsys_file_set_pos_fn_t file_set_pos_fn;
  pdf_fsys_file_get_size_fn_t file_get_size_fn;
  pdf_fsys_file_can_set_size_p_fn_t file_can_set_size_p_fn;
  pdf_fsys_file_set_size_fn_t file_set_size_fn;
  pdf_fsys_file_read_fn_t file_read_fn;
  pdf_fsys_file_write_fn_t file_write_fn;
  pdf_fsys_file_flush_fn_t file_flush_fn;
  pdf_fsys_file_request_ria_fn_t file_request_ria_fn;
  pdf_fsys_file_has_ria_fn_t file_has_ria_fn;
  pdf_fsys_file_cancel_ria_fn_t file_cancel_ria_fn;
  /*    <Padding> */
  void *fsys_file_padding[32];
};

/* File implementation */
struct pdf_fsys_file_s
{
  /* Filesystem used to open the file */
  const struct pdf_fsys_s *fsys;
  /* Mode used when opening */
  enum pdf_fsys_file_mode_e mode;
  /* Path used to open the file */
  pdf_text_t *unicode_path;
};

/* Helper methods to be used by filesystem implementations */
pdf_bool_t pdf_fsys_impl_helper_file_init (struct pdf_fsys_file_s     *common,
                                           const struct pdf_fsys_s    *fsys,
                                           const pdf_text_t           *path,
                                           enum pdf_fsys_file_mode_e   mode,
                                           pdf_error_t               **error);
void pdf_fsys_impl_helper_file_deinit (struct pdf_fsys_file_s *common);

/* ------------------------ Filesystem Management API ----------------------- */

typedef void (*pdf_fsys_deinit_fn_t) (struct pdf_fsys_s *implementation);
typedef void (*pdf_fsys_foreach_fn_t) (void                    *user_data,
                                       const pdf_char_t        *id,
                                       const struct pdf_fsys_s *implementation);

pdf_bool_t pdf_fsys_add (const pdf_char_t         *id,
                         const struct pdf_fsys_s  *fsys,
                         pdf_fsys_deinit_fn_t      fsys_deinit,
                         pdf_error_t             **error);

void pdf_fsys_remove (const pdf_char_t *id);

const pdf_fsys_t *pdf_fsys_get (const pdf_char_t *id);

void pdf_fsys_foreach (pdf_fsys_foreach_fn_t  foreach_fn,
                       void                  *user_data);

/* -------------------------- Filesystem interface -------------------------- */

#define pdf_fsys_get_free_space(fsys, path, error)  \
  fsys->get_free_space_fn (fsys, path, error)

#define pdf_fsys_create_folder(fsys, path, error)   \
  fsys->create_folder_fn (fsys, path, error)

#define pdf_fsys_get_folder_contents(fsys, path, error) \
  fsys->get_folder_contents_fn (fsys, path, error)

#define pdf_fsys_remove_folder(fsys, path, error)   \
  fsys->remove_folder_fn (fsys, path, error)

#define pdf_fsys_get_parent(fsys, path, error)  \
  fsys->get_parent_fn (fsys, path, error)

#define pdf_fsys_get_basename(fsys, path, error)  \
  fsys->get_basename_fn (fsys, path, error)

#define pdf_fsys_compare_path_p(fsys, first, second, error) \
  fsys->compare_path_p_fn (fsys, first, second, error)

#define pdf_fsys_build_path(fsys, error, first, ...)        \
  fsys->build_path_fn (fsys, error, first, ##__VA_ARGS__)

#define pdf_fsys_get_url_from_path(fsys,path,error) \
  fsys->get_url_from_path_fn (fsys, path, error)

#define pdf_fsys_get_path_from_url(fsys,url,error)  \
  fsys->get_path_from_url_fn (fsys, url, error)

#define pdf_fsys_item_p(fsys, path)             \
  fsys->item_p_fn (fsys, path)

#define pdf_fsys_get_item_props(fsys, path, props, error)   \
  fsys->get_item_props_fn (fsys, path, props, error)

pdf_hash_t *pdf_fsys_item_props_to_hash (const struct pdf_fsys_item_props_s  *item_props,
                                         pdf_error_t                        **error);

#define pdf_fsys_item_readable_p(fsys, path)    \
  fsys->item_readable_p_fn (fsys, path)

#define pdf_fsys_item_writable_p(fsys, path)    \
  fsys->item_writable_p_fn (fsys, path)

/* ----------------------------- File interface ----------------------------- */

#define pdf_fsys_file_open(fsys, path, mode, error) \
  fsys->file_open_fn (fsys, path, mode, error)

#define pdf_fsys_file_open_tmp(fsys, error)     \
  fsys->file_open_tmp_fn (fsys, error)

#define pdf_fsys_file_reopen(file, mode, error) \
  file->fsys->file_reopen_fn (file, mode, error)

#define pdf_fsys_file_close(file,error)         \
  file->fsys->file_close_fn (file,error)

#define pdf_fsys_file_get_pos(file,error)       \
  file->fsys->file_get_pos_fn (file, error)

#define pdf_fsys_file_set_pos(file,pos,error)   \
  file->fsys->file_set_pos_fn (file, pos, error)

#define pdf_fsys_file_can_set_size_p(file,size) \
  file->fsys->file_can_set_size_p_fn (file, size)

#define pdf_fsys_file_get_size(file,error)       \
  file->fsys->file_get_size_fn (file, error)

#define pdf_fsys_file_set_size(file,size,error)   \
  file->fsys->file_set_size_fn (file, size, error)

#define pdf_fsys_file_read(file, buf, bytes, read_bytes, error) \
  file->fsys->file_read_fn (file, buf, bytes, read_bytes, error)

#define pdf_fsys_file_write(file, buf, bytes, written_bytes, error) \
  file->fsys->file_write_fn (file, buf, bytes, written_bytes, error)

#define pdf_fsys_file_flush(file, error)        \
  file->fsys->file_flush_fn (file, error)

#define pdf_fsys_file_request_ria(file, offset, size, error)    \
  file->fsys->file_request_ria_fn (file, offset, size, error)

#define pdf_fsys_file_has_ria(file)             \
  file->fsys->file_has_ria_fn (file)

#define pdf_fsys_file_cancel_ria(file, ria_id, error)   \
  file->fsys->file_cancel_ria_fn (file, ria_id, error)

/* Common for all proper filesystem implementations... */

#define pdf_fsys_file_get_filesystem(file)      \
  (const pdf_fsys_t *)file->fsys

#define pdf_fsys_file_get_mode(file)            \
  file->mode

#define pdf_fsys_file_get_path(file)            \
  (const pdf_text_t *)file->path

/* END PUBLIC */

#endif /* ! PDF_FSYS_H */

/* End of pdf-fsys.h */

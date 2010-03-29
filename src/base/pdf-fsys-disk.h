/* -*- mode: C -*-
 *
 *       File:         pdf-fsys-disk.h
 *       Date:         Thu May 22 18:22:59 2008
 *
 *       GNU PDF Library - Disk Filesystem Implementation
 *
 */

/* Copyright (C) 2008 Free Software Foundation, Inc. */

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

#ifndef PDF_FSYS_DISK_H
#define PDF_FSYS_DISK_H

#include <pdf-fsys.h>

/* Filesystem internal data associated with open files */
struct pdf_fsys_disk_file_s
{
  pdf_text_t unicode_path;    /* Path name used to open the file,
                                 Unicode version */
  pdf_char_t *host_path;      /* Path name used to open the file, host
                                 encoded version */
  pdf_size_t host_path_size;  /* Size of the host encoded path */

  enum pdf_fsys_file_mode_e file_mode; /* file mode set when file
                                          was opened */
  FILE *file_descriptor;      /* The descriptor of the open file */
};

typedef struct pdf_fsys_disk_file_s *pdf_fsys_disk_file_t;



/* --------------------- Filesystem interface ------------------------- */

/* Initialization and cleanup.  */
pdf_status_t pdf_fsys_disk_init (void **data);
pdf_status_t pdf_fsys_disk_cleanup (void *data);

/* Get the free storage space in the volume containing path_name */
pdf_i64_t
pdf_fsys_disk_get_free_space (void *data,
                              pdf_text_t path_name);

/* Create folder in disk */
pdf_status_t
pdf_fsys_disk_create_folder (void *data,
                             const pdf_text_t path_name);

/* Get folder contents from disk (list files in disk) */
pdf_status_t
pdf_fsys_disk_get_folder_contents (void *data,
                                   const pdf_text_t path_name,
                                   pdf_list_t item_list);

/* Get path name of the father of a given item in the filesystem */
pdf_status_t
pdf_fsys_disk_get_parent (void *data,
                          const pdf_text_t path_name,
                          pdf_text_t parent_path);

/* Remove folder from disk */
pdf_status_t
pdf_fsys_disk_remove_folder (void *data,
                             const pdf_text_t path_name);

/* Get item properties */
pdf_status_t
pdf_fsys_disk_get_item_props (void *data,
                              pdf_text_t path_name,
                              struct pdf_fsys_item_props_s *item_props);

/* Check if file exists in disk */
pdf_bool_t
pdf_fsys_disk_item_p (void *data,
                      pdf_text_t path_name);

/* Check if file is readable in disk */
pdf_bool_t
pdf_fsys_disk_item_readable_p (void *data,
                               pdf_text_t path_name);

/* Check if file is writable in disk */
pdf_bool_t
pdf_fsys_disk_item_writable_p (void *data,
                               pdf_text_t path_name);


/* --------------------- File interface ------------------------- */

/* Open a given file */
pdf_status_t
pdf_fsys_disk_file_open (void *data,
                         const pdf_text_t path_name,
                         const enum pdf_fsys_file_mode_e mode,
                         pdf_fsys_file_t *p_file);

/* Open a temporary file.  */
pdf_status_t
pdf_fsys_disk_file_open_tmp (void *data,
                             pdf_fsys_file_t *p_file);

/* Get file mode info */
enum pdf_fsys_file_mode_e
pdf_fsys_disk_file_get_mode (pdf_fsys_file_t file);

/* Get URL from file */
pdf_text_t
pdf_fsys_disk_file_get_url (pdf_fsys_file_t file);

/* Set file mode info */
pdf_status_t
pdf_fsys_disk_file_set_mode (pdf_fsys_file_t file,
                             enum pdf_fsys_file_mode_e new_mode);

/* Check if paths are same/equivalent */ 
pdf_bool_t
pdf_fsys_disk_file_same_p (pdf_fsys_file_t file,
                           pdf_text_t path);

/* Get pos */
pdf_status_t pdf_fsys_disk_file_get_pos (pdf_fsys_file_t file,
                                         pdf_size_t *pos);

pdf_status_t pdf_fsys_disk_file_set_pos (pdf_fsys_file_t file,
                                         pdf_size_t new_pos);

pdf_bool_t pdf_fsys_disk_file_can_set_size_p (pdf_fsys_file_t file,
                                              pdf_size_t size);

pdf_size_t pdf_fsys_disk_file_get_size (pdf_fsys_file_t file);

pdf_status_t pdf_fsys_disk_file_set_size (pdf_fsys_file_t file,
                                          pdf_size_t size);

pdf_status_t
pdf_fsys_disk_file_write (pdf_fsys_file_t file, pdf_char_t *buf,
                          pdf_size_t bytes, pdf_size_t *written_bytes);

pdf_status_t
pdf_fsys_disk_file_read (pdf_fsys_file_t file, pdf_char_t *buf,
                         pdf_size_t bytes, pdf_size_t *read_bytes);

pdf_status_t pdf_fsys_disk_file_flush (pdf_fsys_file_t file);

pdf_status_t pdf_fsys_disk_file_request_ria (pdf_fsys_file_t file,
                                             pdf_size_t offset,
                                             pdf_size_t count);
pdf_bool_t pdf_fsys_disk_file_has_ria (pdf_fsys_file_t file);
pdf_status_t pdf_fsys_disk_file_cancel_ria (pdf_fsys_file_t file);
pdf_status_t pdf_fsys_disk_file_close (pdf_fsys_file_t file);
pdf_status_t pdf_fsys_disk_file_reopen (pdf_fsys_file_t file,
                                        enum pdf_fsys_file_mode_e mode);

#endif /* !PDF_FSYS_DISK_H */

/* End of pdf-fsys-disk.h */

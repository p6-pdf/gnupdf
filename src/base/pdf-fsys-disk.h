/* -*- mode: C -*- Time-stamp: "08/05/28 17:37:31 jemarch"
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

  FILE *file_descriptor;      /* The descriptor of the open file */
};

typedef struct pdf_fsys_disk_file_s *pdf_fsys_disk_file_t;

/* Filesystem Interface */
pdf_size_t pdf_fsys_disk_get_free_space (pdf_text_t path_name);
pdf_status_t pdf_fsys_disk_open (const pdf_text_t path_name,
                                 const enum pdf_fsys_file_mode_e mode,
                                 pdf_fsys_file_t file);
pdf_status_t pdf_fsys_disk_create_folder (const pdf_text_t path_name);
pdf_status_t pdf_fsys_disk_get_folder_contents (const pdf_text_t path_name,
                                                pdf_list_t item_list);
pdf_status_t pdf_fsys_disk_get_parent (const pdf_text_t path_name,
                                       pdf_text_t parent_path);
pdf_status_t pdf_fsys_disk_remove_folder (const pdf_text_t path_name);
pdf_status_t pdf_fsys_disk_get_item_props (pdf_text_t path_name,
                                           struct pdf_fsys_item_props_s *item_props);
pdf_bool_t pdf_fsys_disk_item_p (pdf_text_t path_name);
pdf_bool_t pdf_fsys_disk_item_readable_p (pdf_text_t path_name);
pdf_bool_t pdf_fsys_disk_item_writable_p (pdf_text_t path_name);
pdf_text_t pdf_fsys_disk_get_temp_path_name (void);

/* File Interface */

pdf_fsys_t pdf_fsys_disk_file_get_filesystem (pdf_fsys_file_t file);
enum pdf_fsys_file_mode_e pdf_fsys_disk_file_get_mode (pdf_fsys_file_t file);
pdf_text_t pdf_fsys_disk_file_get_url (pdf_fsys_file_t file);
enum pdf_fsys_file_mode_e pdf_fsys_disk_file_set_mode (pdf_fsys_file_t file,
                                                       enum pdf_fsys_file_mode_e new_mode);
pdf_bool_t pdf_fsys_disk_file_same_p (pdf_fsys_file_t file,
                                      pdf_text_t path);
pdf_status_t pdf_fsys_disk_file_get_pos (pdf_fsys_file_t file,
                                         pdf_size_t *pos);
pdf_status_t pdf_fsys_disk_file_set_pos (pdf_fsys_file_t file,
                                         pdf_size_t new_pos);
pdf_bool_t pdf_fsys_disk_file_can_set_size_p (pdf_fsys_file_t file,
                                              pdf_size_t size);
pdf_status_t pdf_fsys_disk_file_get_size (pdf_fsys_file_t file, 
                                          pdf_size_t *size);
pdf_status_t pdf_fsys_disk_file_set_size (pdf_fsys_file_t file,
                                          pdf_size_t size);
pdf_size_t pdf_fsys_disk_file_read (pdf_fsys_file_t file,
                                    const pdf_size_t elem_size,
                                    const pdf_size_t elem_count,
                                    void *data);
pdf_size_t pdf_fsys_disk_file_write (pdf_fsys_file_t file,
                                     const pdf_size_t elem_size,
                                     const pdf_size_t elem_count,
                                     void *data);
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

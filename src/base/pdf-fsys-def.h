/* -*- mode: C -*-
 *
 *       File:         pdf-fsys-def.h
 *       Date:         Thu May 22 17:50:20 2008
 *
 *       GNU PDF Library - Default Filesystem
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


#ifndef PDF_FSYS_DEF_H
#define PDF_FSYS_DEF_H

#include <pdf-fsys-disk.h>

#define pdf_fsys_def_init(DATA) \
  pdf_fsys_disk_init ((DATA))
#define pdf_fsys_def_cleanup(DATA) \
  pdf_fsys_disk_cleanup ((DATA))
#define pdf_fsys_def_get_free_space(path_name) \
  pdf_fsys_disk_get_free_space (NULL, (path_name))
#define pdf_fsys_def_create_folder(path_name) \
  pdf_fsys_disk_create_folder (NULL, (path_name))
#define pdf_fsys_def_get_folder_contents(path_name, item_list) \
  pdf_fsys_disk_get_folder_contents (NULL, (path_name), (item_list))
#define pdf_fsys_def_get_parent(path_name, parent_path) \
  pdf_fsys_disk_get_parent (NULL, (path_name), (parent_path))
#define pdf_fsys_def_remove_folder(path_name) \
  pdf_fsys_disk_remove_folder (NULL, (path_name))
#define pdf_fsys_def_get_item_props(path_name, item_props) \
  pdf_fsys_disk_get_item_props (NULL, (path_name), (item_props))
#define pdf_fsys_def_item_p(path_name) \
  pdf_fsys_disk_item_p (NULL, (path_name))
#define pdf_fsys_def_item_readable_p(path_name) \
  pdf_fsys_disk_item_readable_p (NULL, (path_name))
#define pdf_fsys_def_item_writable_p(path_name) \
  pdf_fsys_disk_item_writable_p (NULL, (path_name))
#define pdf_fsys_def_build_path(output, first_element, rest) \
  pdf_fsys_disk_build_path (NULL, (output), (first_element), (rest))
#define pdf_fsys_def_file_open(path_name, mode, p_file)     \
  pdf_fsys_disk_file_open (NULL, (path_name), (mode), (p_file))
#define pdf_fsys_def_file_open_tmp(p_file)   \
  pdf_fsys_disk_file_open_tmp (NULL, (p_file))
#define pdf_fsys_def_file_get_mode(file) \
  pdf_fsys_disk_file_get_mode ((file))
#define pdf_fsys_def_file_get_url(file) \
  pdf_fsys_disk_file_get_url ((file))
#define pdf_fsys_def_file_set_mode(file, new_mode) \
  pdf_fsys_disk_file_set_mode ((file), (new_mode))
#define pdf_fsys_def_file_same_p(file, path) \
  pdf_fsys_disk_file_same_p ((file), (path))
#define pdf_fsys_def_file_get_pos(file, pos) \
  pdf_fsys_disk_file_get_pos ((file), (pos))
#define pdf_fsys_def_file_set_pos(file, new_pos) \
  pdf_fsys_disk_file_set_pos ((file), (new_pos))
#define pdf_fsys_def_file_can_set_size_p(file, size) \
  pdf_fsys_disk_file_can_set_size_p ((file), (size))
#define pdf_fsys_def_file_get_size(file) \
  pdf_fsys_disk_file_get_size ((file))
#define pdf_fsys_def_file_set_size(file, size) \
  pdf_fsys_disk_file_set_size ((file), (size))
#define pdf_fsys_def_file_read(file, elem_size, elem_count, data) \
  pdf_fsys_disk_file_read ((file), (elem_size), (elem_count), (data))
#define pdf_fsys_def_file_write(file, elem_size, elem_count, data) \
  pdf_fsys_disk_file_write ((file), (elem_size), (elem_count), (data))
#define pdf_fsys_def_file_flush(file) \
  pdf_fsys_disk_file_flush ((file))
#define pdf_fsys_def_file_request_ria(file, offset, count) \
  pdf_fsys_disk_file_request_ria ((file), (offset), (count))
#define pdf_fsys_def_file_has_ria(file) \
  pdf_fsys_disk_file_has_ria ((file))
#define pdf_fsys_def_file_cancel_ria(file) \
  pdf_fsys_disk_file_cancel_ria ((file))
#define pdf_fsys_def_file_close(file) \
  pdf_fsys_disk_file_close ((file))
#define pdf_fsys_def_file_reopen(file,mode) \
  pdf_fsys_disk_file_reopen ((file), (mode))

#endif /* !PDF_FSYS_DEF_H */

/* End of pdf-fsys-def.h */

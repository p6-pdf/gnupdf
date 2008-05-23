/* -*- mode: C -*- Time-stamp: "08/05/22 21:12:45 jemarch"
 *
 *       File:         pdf-fsys-disk.c
 *       Date:         Thu May 22 18:27:35 2008
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

#include <pdf-fsys-disk.h>

/*
 * Filesystem Interface Implementation
 */

pdf_size_t
pdf_fsys_disk_get_free_space (pdf_text_t path_name)
{
  /* FIXME: Please implement me :D */
  return 0;
}


pdf_status_t
pdf_fsys_disk_open (const pdf_text_t path_name,
                    const enum pdf_fsys_file_mode_e mode,
                    pdf_fsys_file_t file)
{
  /* FIXME: Please implement me :D */
  return PDF_OK;
}

pdf_status_t
pdf_fsys_disk_create_folder (const pdf_text_t path_name)
{
  /* FIXME: Bored? Implement me! :D */
  return PDF_OK;
}

pdf_status_t
pdf_fsys_disk_get_folder_contents (const pdf_text_t path_name,
                                   pdf_list_t item_list)
{
  /* FIXME: Please implement me :D */
  return PDF_OK;
}


pdf_status_t
pdf_fsys_disk_get_parent (const pdf_text_t path_name,
                          pdf_text_t parent_path)
{
  /* FIXME: Please implement me :D */
  return PDF_OK;
}

pdf_status_t
pdf_fsys_disk_remove_folder (const pdf_text_t path_name)
{
  /* FIXME: Please implement me :D */
  return PDF_OK;
}

pdf_status_t
pdf_fsys_disk_get_item_props (pdf_text_t path_name,
                              struct pdf_fsys_item_props_s *item_props)
{
  /* FIXME: Please implement me :D */
  return PDF_OK;
}

pdf_status_t
pdf_fsys_disk_item_props_to_hash (const struct pdf_fsys_item_props_s item_props,
                                  pdf_hash_t props_hash)
{
  /* FIXME: Please implement me :D */
  return PDF_OK;
}

pdf_bool_t
pdf_fsys_disk_item_p (pdf_text_t path_name)
{
  /* FIXME: Please implement me :D */
  return PDF_TRUE;
}

pdf_bool_t 
pdf_fsys_disk_item_readable_p (pdf_text_t path_name)
{
  /* FIXME: Please implement me :D */
  return PDF_TRUE;
}

pdf_bool_t
pdf_fsys_disk_item_writable_p (pdf_text_t path_name)
{
  /* FIXME: Please implement me :D */
  return PDF_TRUE;
}

pdf_text_t
pdf_fsys_disk_get_temp_path_name (void)
{
  /* FIXME: Please implement me :D */
  return NULL;
}

/*
 * File Interface Implementation
 */

enum pdf_fsys_file_mode_e
pdf_fsys_disk_file_get_mode (pdf_fsys_file_t file)
{
  /* FIXME: Please implement me :o) */
  return 0;
}

pdf_text_t 
pdf_fsys_disk_file_get_url (pdf_fsys_file_t file)
{
  /* FIXME: Please implement me :D */
  return NULL;
}

enum pdf_fsys_file_mode_e
pdf_fsys_disk_file_set_mode (pdf_fsys_file_t file,
                             enum pdf_fsys_file_mode_e new_mode)
{
  /* FIXME: Please implement me :D */
  return 0;
}

pdf_bool_t
pdf_fsys_disk_file_same_p (pdf_fsys_file_t file,
                           pdf_text_t path)
{
  /* FIXME: Please implement me XD */
  return PDF_TRUE;
}

pdf_status_t
pdf_fsys_disk_file_get_pos (pdf_fsys_file_t file,
                            pdf_size_t *pos)
{
  /* FIXME: Wanna be my friend? :D then please implement me :'D */
  return 0;
}

pdf_status_t
pdf_fsys_disk_file_set_pos (pdf_fsys_file_t file,
                            pdf_size_t new_pos)
{
  /* FIXME: Please implement me and I will give you a kiss :* */
  return PDF_OK;
}

pdf_bool_t
pdf_fsys_disk_file_can_set_size_p (pdf_fsys_file_t file,
                                   pdf_size_t size)
{
  /* FIXME: I am quite simple! Prove it by implementing me! :D! */
  return PDF_TRUE;
}

pdf_status_t
pdf_fsys_disk_file_get_size (pdf_fsys_file_t file, 
                             pdf_size_t *size)
{
  /* FIXME: Please implement me :) */
  return PDF_OK;
}

pdf_status_t
pdf_fsys_disk_file_set_size (pdf_fsys_file_t file,
                             pdf_size_t size)
{
  /* FIXME: Please implement me :D */
  return PDF_OK;
}

pdf_size_t
pdf_fsys_disk_file_read (pdf_fsys_file_t file,
                         const pdf_size_t elem_size,
                         const pdf_size_t elem_count,
                         void *data)
{
  /* FIXME: Please implement me :DDD */
  return 0;
}

pdf_size_t
pdf_fsys_disk_file_write (pdf_fsys_file_t file,
                          const pdf_size_t elem_size,
                          const pdf_size_t elem_count,
                          void *data)
{
  /* FIXME: Wanna increase your penis? Implement me! ;) */
  return 0;
}

pdf_status_t
pdf_fsys_disk_file_flush (pdf_fsys_file_t file)
{
  /* FIXME: Implement me, but dont tell to Adobe :S */
  return PDF_OK;
}

pdf_status_t
pdf_fsys_disk_file_request_ria (pdf_fsys_file_t file,
                                pdf_size_t offset,
                                pdf_size_t count)
{
  /* This filesystem implementation do not provide Read-In-Advance
   * capabilities, so this function is a no-op */

  return PDF_OK;
}

pdf_bool_t
pdf_fsys_disk_file_has_ria (pdf_fsys_file_t file)
{
  /* This filesystem implementation do not provide Read-In-Advance
   * capabilities */

  return PDF_FALSE;
}

pdf_status_t
pdf_fsys_disk_file_cancel_ria (pdf_fsys_file_t file)
{
  /* This filesystem implementation do not provide Read-In-Advance
   * capabilities, so this function is a no-op */

  return PDF_OK;
}

pdf_status_t
pdf_fsys_disk_file_close (pdf_fsys_file_t file)
{
  /* FIXME: Please implement me :D */
  return PDF_OK;
}

pdf_status_t
pdf_fsys_disk_file_reopen (pdf_fsys_file_t file,
                           enum pdf_fsys_file_mode_e mode)
{
  /* FIXME: Impress your cat by implementing me! O_o */
  return PDF_OK;
}

/* End of pdf-fsys-disk.c */

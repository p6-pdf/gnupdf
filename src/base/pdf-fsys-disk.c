/* -*- mode: C -*- Time-stamp: "08/05/26 20:37:51 jemarch"
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

#include <config.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <pdf-types.h>
#include <pdf-error.h>
#include <pdf-fsys-disk.h>

/* Private function declarations */

static pdf_status_t
pdf_fsys_disk_build_mode_string (const enum pdf_fsys_file_mode_e mode,
                                 pdf_char_t *mode_str);
static pdf_bool_t
pdf_fsys_disk_win32_device_p (pdf_text_t path);

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
  pdf_fsys_disk_file_t file_data;
  pdf_char_t mode_str[4];
  pdf_status_t ret_status;

  /* Allocate private data storage for the file */
  file_data = 
    (pdf_fsys_disk_file_t) pdf_alloc (sizeof(struct pdf_fsys_disk_file_s));
  file->data = (void *) file_data;
  

  /* Make and store a copy of the unicode file path and get the host
     encoded path */
  file_data->unicode_path = pdf_text_dup (path_name);
  if (pdf_text_get_host (&file_data->host_path,
                         &file_data->host_path_size,
                         file_data->unicode_path,
                         pdf_text_get_host_encoding ()) != PDF_OK)
    {
      /* Cleanup and report error */
      pdf_text_destroy (file_data->unicode_path);
      pdf_dealloc (file_data);

      return PDF_ERROR;
    }

  /* Build the mode string for fopen */
  pdf_fsys_disk_build_mode_string (mode, mode_str);

  /* Open the file */
  file_data->file_descriptor =
    fopen ((char *) file_data->host_path,
           (char *) mode_str);

  if (file_data->file_descriptor == NULL)
    {
      /* Cleanup and report error */
      pdf_text_destroy (file_data->unicode_path);
      pdf_dealloc (file_data->host_path);
      pdf_dealloc (file_data);
      pdf_dealloc (mode_str);

      switch (errno)
        {
        case EACCES:
        case EPERM:
        case EROFS:
#ifndef PDF_HOST_WIN32
        case ETXTBSY:
#endif /* !PDF_HOST_WIN32 */
          {
            /* Not enough permissions */
            ret_status = PDF_EBADPERMS;
            break;
          }
        case EISDIR:
        case ENAMETOOLONG:
        case ENOTDIR:
          {
            /* Invalid path name */
            ret_status = PDF_EBADNAME;
            break;
          }
        case ENOMEM:
          {
            /* Not enough memory */
            ret_status = PDF_ENOMEM;
            break;
          }
        default:
          {
            /* Other error */
            ret_status = PDF_ERROR;
            break;
          }
        }

      return ret_status;
    }

  /* Set the filesystem for the file */
  file->fs = NULL; /* This is the default filesystem */

  /* All was ok */
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
  pdf_fsys_disk_file_t file_data;
  pdf_status_t result_status;

  file_data = (pdf_fsys_disk_file_t) file->data;

  if (fflush (file_data->file_descriptor) != 0)
    {
      switch (errno)
        {
#ifndef PDF_HOST_WIN32
          /* On Windows platforms (excluding Cygwin), this function
             does not set errno upon failure. */

        case EBADF:
        case EFAULT:
        case EFBIG:
        case EINVAL:
          {
            /* Bad function parameters to underlying write() */
            result_status = PDF_EBADDATA;
            break;
          }
        case EAGAIN:
          {
            /* non-blocking descriptor and blocking writing
               requested */
            result_status = PDF_EAGAIN;
            break;
          }
        case ENOSPC:
          {
            /* Not room in the disk for the data */
            result_status = PDF_ENOSPC;
            break;
          }
#endif /* !PDF_HOST_WIN32 */
        default:
          {
            /* Other error conditions */
            result_status = PDF_ERROR;
            break;
          }
        }
    }
  else
    {
      result_status = PDF_TRUE;
    }

  return result_status;
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

/*
 * Private functions
 */

static pdf_status_t
pdf_fsys_disk_build_mode_string (const enum pdf_fsys_file_mode_e mode,
                                 pdf_char_t *mode_str)
{
  int mode_str_size;

  switch (mode)
    {
    case PDF_FSYS_OPEN_MODE_READ:
      {
        mode_str[0] = 'r';
        mode_str[1] = 0;

        mode_str_size = 1;
        break;
      }
    case PDF_FSYS_OPEN_MODE_WRITE:
      {
        mode_str[0] = 'w';
        mode_str[1] = 0;

        mode_str_size = 1;
        break;
      }
    case PDF_FSYS_OPEN_MODE_RW:
      {
        mode_str[0] = 'r';
        mode_str[1] = '+';
        mode_str[2] = 0;

        mode_str_size = 2;
        break;
      }
    }

#if defined PDF_HOST_WIN32
  /* Windoze portability note:
   *
   * Files are opened in "text mode" (with crlf translation) by
   * default.
   *
   * Although the "b" fopen option is supported by POSIX some old Unix
   * systems may not implement it, so we should use that option to
   * open files only while running in Windows.
   */
  mode_str[mode_str_size] = 'b';
  mode_str[mode_str_size + 1] = 0;
#endif /* PDF_HOST_WIN32 */

  return PDF_OK;
}

static pdf_bool_t
pdf_fsys_disk_win32_device_p (pdf_text_t path)
{
  /* The following special "files", which access devices, exist in all
     directories, case-insensitively, and with all possible endings
     after a period or colon, excpets in pathnames that start with
     \\?\: */
  char *device_names[] = 
    {"NUL", "CON", "PRN", "AUX", "COM1", "COM2", "COM3", "COM4",
     "COM5", "COM6", "COM7", "COM8", "COM9", "LPT1", "LPT2", "LPT3",
     "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"};
  int num_device_names = 22;
  int i;
  pdf_bool_t device_p;
  pdf_text_t device_name;
  pdf_status_t ret_code;

  device_p = PDF_FALSE;
  for (i = 0; i < num_device_names; i++)
    {
      device_name = pdf_text_new ();
      pdf_text_new_from_unicode (&device_name,
                                 (pdf_char_t *) device_names[i],
                                 strlen (device_names[i]),
                                 PDF_TEXT_UTF8);
      
      if (pdf_text_cmp (path, device_name, PDF_FALSE, &ret_code) == 0)
        {
          device_p = PDF_TRUE;
          break;
        }

      pdf_text_destroy (device_name);
    }

  return device_p;
}

/* End of pdf-fsys-disk.c */

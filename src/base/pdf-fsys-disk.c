/* -*- mode: C -*-
 *
 *       File:         pdf-fsys-disk.c
 *       Date:         Thu May 22 18:27:35 2008
 *
 *       GNU PDF Library - Disk Filesystem Implementation
 *
 */

/* Copyright (C) 2008, 2010, 2011 Free Software Foundation, Inc. */

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
#include <stdio-safer.h>
#include <string.h>
#include <sys/stat.h>

#if defined PDF_HOST_WIN32 || defined PDF_HOST_BSD
#   include <sys/param.h>
#   if defined PDF_HOST_BSD
#      include <sys/mount.h>
#   endif /* PDF_HOST_BSD */
#else
#   include <sys/statfs.h>
#endif /* PDF_HOST_WIN32 | PDF_HOST_BSD */

#if defined PDF_HOST_WIN32
#   include <windows.h>
#   include <wchar.h>
#endif /* !PDF_HOST_WIN32 */

#include <dirent.h>
#include <unistd.h>

#include <pdf-types.h>
#include <pdf-error.h>
#include <pdf-fsys-disk.h>

const struct pdf_fsys_impl_s pdf_fsys_disk_implementation =
  {
    pdf_fsys_disk_init,
    pdf_fsys_disk_cleanup,
    pdf_fsys_disk_create_folder,
    pdf_fsys_disk_get_folder_contents,
    pdf_fsys_disk_get_parent,
    pdf_fsys_disk_remove_folder,
    pdf_fsys_disk_get_item_props,
    pdf_fsys_disk_get_free_space,
    pdf_fsys_disk_item_p,
    pdf_fsys_disk_item_readable_p,
    pdf_fsys_disk_item_writable_p,
    pdf_fsys_disk_file_get_url,
    pdf_fsys_disk_build_path,
    pdf_fsys_disk_file_open,
    pdf_fsys_disk_file_open_tmp,
    pdf_fsys_disk_file_read,
    pdf_fsys_disk_file_write,
    pdf_fsys_disk_file_flush,
    pdf_fsys_disk_file_can_set_size_p,
    pdf_fsys_disk_file_get_size,
    pdf_fsys_disk_file_set_size,
    pdf_fsys_disk_file_get_pos,
    pdf_fsys_disk_file_set_pos,
    pdf_fsys_disk_file_get_mode,
    pdf_fsys_disk_file_set_mode,
    pdf_fsys_disk_file_same_p,
    pdf_fsys_disk_file_request_ria,
    pdf_fsys_disk_file_has_ria,
    pdf_fsys_disk_file_cancel_ria,
    pdf_fsys_disk_file_close,
    pdf_fsys_disk_file_reopen
  };

/* Private function declarations */

static pdf_status_t __pdf_fsys_disk_get_status_from_errno (int _errno);

static pdf_char_t *__pdf_fsys_disk_get_host_path (const pdf_text_t  *path,
                                                  pdf_error_t      **error);

static pdf_text_t *__pdf_fsys_disk_set_host_path (const pdf_char_t *host_path,
                                                  const pdf_size_t  host_path_size,
                                                  pdf_error_t      **error);

static const pdf_char_t *__pdf_fsys_disk_get_mode_string (const enum pdf_fsys_file_mode_e mode);

#ifdef PDF_HOST_WIN32
static pdf_bool_t __pdf_fsys_disk_win32_device_p (const pdf_text_t  *path,
						  pdf_error_t      **error);
#endif

/*
 * Filesystem Interface Implementation
 */

pdf_status_t
pdf_fsys_disk_init (void **data)
{
  /* Do nothing.  */
  *data = NULL;
  return PDF_OK;
}

pdf_status_t
pdf_fsys_disk_cleanup (void *data)
{
  /* Do nothing.  */
  return PDF_OK;
}

#ifdef PDF_HOST_WIN32

pdf_i64_t
pdf_fsys_disk_get_free_space (void              *data,
                              const pdf_text_t  *path_name)
{
  pdf_char_t *utf16le_path;
  pdf_u32_t utf16le_path_size = 0;
  ULARGE_INTEGER free_bytes;
  pdf_error_t *inner_error = NULL;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, -1);

  /* Note that as we get the string as UTF-16LE with LAST NUL suffix,
   *  it's equivalent to a wchar_t in windows environments */
  utf16le_path = pdf_text_get_unicode (path_name,
                                       PDF_TEXT_UTF16_LE,
                                       PDF_TEXT_UNICODE_WITH_NUL_SUFFIX,
                                       &utf16le_path_size,
                                       &inner_error);
  if (!utf16le_path)
    {
      /* TODO: Propagate error */
      pdf_error_destroy (inner_error);
      return -1;
    }

  /* No need to get the drive letter of the specified path:
   * This parameter does not have to specify the root directory on a disk.
   *  The function accepts any directory on a disk.
   **/

  /* Get the information from the filesystem
   *
   * BOOL WINAPI GetDiskFreeSpaceEx(
   *  __in_opt   LPCTSTR lpDirectoryName,
   *  __out_opt  PULARGE_INTEGER lpFreeBytesAvailable,
   *  __out_opt  PULARGE_INTEGER lpTotalNumberOfBytes,
   *  __out_opt  PULARGE_INTEGER lpTotalNumberOfFreeBytes
   * );
   *
   **/
  if (!GetDiskFreeSpaceExW ((LPCWSTR) utf16le_path,
                            NULL,
                            NULL,
                            &free_bytes))
    {
      /* TODO: Propagate error */
      /* pdf_set_error (error, */
      /*                PDF_EDOMAIN_BASE_FS, */
      /*                __pdf_fsys_disk_get_status_from_errno (errno), */
      /*                "cannot get free disk space: got error code %u", */
      /*                (uint)GetLastError ()); */
      pdf_dealloc (host_path);
      return -1;
    }

  pdf_dealloc (utf16le_path);

  return ((pdf_i64_t)free_bytes << 32) + free_bytes.LowPart;
}

#else

pdf_i64_t
pdf_fsys_disk_get_free_space (void              *data,
                              const pdf_text_t  *path_name)
{
  struct statfs fs_stats;
  pdf_char_t *host_path;
  pdf_error_t *inner_error = NULL;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, -1);

  /* We get the string in HOST-encoding (with NUL-suffix) */
  host_path = __pdf_fsys_disk_get_host_path (path_name, &inner_error);
  if (!host_path)
    {
      /* TODO: Propagate error */
      pdf_error_destroy (inner_error);
      return -1;
    }

  PDF_DEBUG_BASE ("Getting free bytes of FS at path '%s'...", host_path);

  if (statfs (host_path, &fs_stats) < 0)
    {
      /* pdf_set_error (error, */
      /*                PDF_EDOMAIN_BASE_FS, */
      /*                __pdf_fsys_disk_get_status_from_errno (errno), */
      /*                "cannot get free disk space: %s", */
      /*                strerror (errno)); */
      pdf_dealloc (host_path);
      return -1;
    }

  pdf_dealloc (host_path);

  /* Return the free space in octects */
  return (pdf_i64_t)(fs_stats.f_bfree * fs_stats.f_bsize);
}

#endif /* !PDF_HOST_WIN32 */

/* Private function to de-initialize and de-allocate base file data */
static void
__pdf_fsys_deinit_base_file_data (pdf_fsys_disk_file_t *p_file_data)
{
  if (p_file_data && *p_file_data)
    {
      pdf_text_destroy ((*p_file_data)->unicode_path);
      pdf_dealloc ((*p_file_data)->host_path);
      pdf_dealloc (*p_file_data);
      *p_file_data = NULL;
    }
}

/* Private function to allocate and initialize base file data */
static pdf_fsys_disk_file_t
__pdf_fsys_init_base_file_data (const pdf_text_t  *path_name,
                                pdf_error_t      **error)
{
  pdf_fsys_disk_file_t file_data;

  /* Allocate private data storage for the file */
  file_data = (pdf_fsys_disk_file_t) pdf_alloc (sizeof (struct pdf_fsys_disk_file_s));
  if (!file_data)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FS,
                     PDF_ENOMEM,
                     "cannot init base file data: "
                     "couldn't allocate %lu bytes",
                     (unsigned long) sizeof (struct pdf_fsys_disk_file_s));
      return NULL;
    }

  memset (file_data, 0, sizeof (struct pdf_fsys_disk_file_s));

  /* Make and store a copy of the unicode file path */
  file_data->unicode_path = pdf_text_dup (path_name, error);
  if (!file_data->unicode_path)
    {
      pdf_dealloc (file_data);
      return NULL;
    }

  /* Get the host encoded path */
  file_data->host_path = __pdf_fsys_disk_get_host_path (file_data->unicode_path,
                                                        error);
  if (!file_data->host_path)
    {
      pdf_text_destroy (file_data->unicode_path);
      pdf_dealloc (file_data);
      return NULL;
    }

  return file_data;
}

/* Host-dependent fopen() */
#ifdef PDF_HOST_WIN32
#define PDF_FOPEN(f,m) _wfopen((wchar_t *)f,(wchar_t *)m)
#else
#define PDF_FOPEN(f,m) fopen_safer(f,m)
#endif

pdf_status_t
pdf_fsys_disk_file_open (void                            *data,
                         const pdf_text_t                *path_name,
                         const enum pdf_fsys_file_mode_e  mode,
                         pdf_fsys_file_t                 *p_file)
{
  pdf_fsys_file_t file = NULL;
  pdf_fsys_disk_file_t file_data = NULL;
  pdf_error_t *inner_error = NULL;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, PDF_EBADDATA);
  PDF_ASSERT_POINTER_RETURN_VAL (p_file, PDF_EBADDATA);
  PDF_ASSERT_RETURN_VAL (mode >= PDF_FSYS_OPEN_MODE_FIRST, PDF_EBADDATA);
  PDF_ASSERT_RETURN_VAL (mode <= PDF_FSYS_OPEN_MODE_LAST, PDF_EBADDATA);

  /* Allocate file struct */
  file = (pdf_fsys_file_t) pdf_alloc (sizeof (struct pdf_fsys_file_s));
  if (!file)
    {
      /* TODO: Propagate error */
      return PDF_ENOMEM;
    }

  /* Get base data */
  file_data = __pdf_fsys_init_base_file_data (path_name, &inner_error);
  if (!file_data)
    {
      pdf_dealloc (file);
      /* TODO: Propagate error */
      pdf_error_destroy (inner_error);
      return PDF_ENOMEM;
    };

  /* Open the file */
  file_data->file_descriptor = PDF_FOPEN (file_data->host_path,
                                          __pdf_fsys_disk_get_mode_string (mode));
  /* If got a good FD, set output data */
  if (file_data->file_descriptor)
    {
      /* Store the mode */
      file_data->file_mode = mode;
      /* Store reference to the file data in the output variable*/
      file->data = (void *) file_data;
      /* Set the filesystem for the file */
      file->fs = NULL; /* This is the default filesystem */
      /* Finally, if everything went ok, put output element */
      *p_file = file;
      /* All ok ! */
      return PDF_OK;
    }

  /* TODO: Propagate error */

  /* Deallocate and deinit base file data */
  __pdf_fsys_deinit_base_file_data (&file_data);
  pdf_dealloc (file);

  /* Get pdf_status_t code from errno */
  return __pdf_fsys_disk_get_status_from_errno (errno);
}

pdf_status_t
pdf_fsys_disk_file_open_tmp (void            *data,
                             pdf_fsys_file_t *p_file)
{
  pdf_text_t *path_name;
  pdf_fsys_file_t file = NULL;
  pdf_fsys_disk_file_t file_data = NULL;
  pdf_error_t *inner_error = NULL;

  PDF_ASSERT_POINTER_RETURN_VAL (p_file, PDF_EBADDATA);

  file = (pdf_fsys_file_t) pdf_alloc (sizeof (struct pdf_fsys_file_s));
  if (!file)
    {
      /* TODO: Propagate error */
      return PDF_ENOMEM;
    }

  /* Create a dummy path name, so we can manage the temporary file
     like an ordinary file.  */
  path_name = pdf_text_new_from_unicode ((pdf_char_t *) "tmp",
                                         3,
                                         PDF_TEXT_UTF8,
                                         &inner_error);
  if (!path_name)
    {
      pdf_dealloc (file);
      /* TODO: Propagate error */
      pdf_error_destroy (inner_error);
      return PDF_ENOMEM;
    }

  /* Get base data */
  file_data = __pdf_fsys_init_base_file_data (path_name, &inner_error);
  if (!file_data)
    {
      pdf_dealloc (file);
      pdf_text_destroy (path_name);
      /* TODO: Propagate error */
      pdf_error_destroy (inner_error);
      return PDF_ENOMEM;
    };

  file->fs = NULL; /* Default filesystem.  */
  file_data->file_mode = PDF_FSYS_OPEN_MODE_RW;

  /* Open a temporary file.  */
  file_data->file_descriptor = tmpfile_safer ();
  if (file_data->file_descriptor)
    {
      /* Success.  */
      file->data = (void *) file_data;
      *p_file = file;
      return PDF_OK;
    }

  /* TODO: Propagate error */

  /* Deallocate and deinit base file data */
  __pdf_fsys_deinit_base_file_data (&file_data);
  pdf_text_destroy (path_name);
  pdf_dealloc (file);

  /* Get pdf_status_t code from errno */
  return __pdf_fsys_disk_get_status_from_errno (errno);
}

pdf_status_t
pdf_fsys_disk_file_close (pdf_fsys_file_t file)
{
  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_EBADDATA);
  PDF_ASSERT_POINTER_RETURN_VAL (file->data, PDF_EBADDATA);

  /* Close the I/O stream only if still open */
  if ((((pdf_fsys_disk_file_t)(file->data))->file_descriptor) &&
      (fclose (((pdf_fsys_disk_file_t)(file->data))->file_descriptor) == EOF))
    {
      /* TODO: Propagate error */
      /* An error was detected closing the I/O stream */
      return PDF_ERROR;
    }

  /* Deallocate other internal contents */
  __pdf_fsys_deinit_base_file_data ((pdf_fsys_disk_file_t *)&(file->data));
  pdf_dealloc (file);

  return PDF_OK;
}

/* Host-dependent mkdir() */
#ifdef PDF_HOST_WIN32
#define PDF_MKDIR(f,m) _wmkdir((const wchar_t *)f)
#else
#define PDF_MKDIR(f,m) mkdir(f,m)
#endif

pdf_status_t
pdf_fsys_disk_create_folder (void             *data,
                             const pdf_text_t *path_name)
{
  pdf_error_t *inner_error = NULL;
  pdf_char_t *host_path;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, PDF_EBADDATA);

  /* Get a host-encoded version of the path name */
  host_path = __pdf_fsys_disk_get_host_path (path_name,
                                             &inner_error);
  if (!host_path)
    {
      /* TODO: Propagate error */
      pdf_error_destroy (inner_error);
      return PDF_ERROR;
    }

  /* Set the permissions of the new directory (posix-only):
   * rwxr_xr_x
   */

  /* Open the file */
  if (PDF_MKDIR (host_path,                             \
                 (S_IRUSR | S_IWUSR | S_IXUSR |         \
                  S_IRGRP | S_IXGRP |                   \
                  S_IROTH | S_IXOTH)) != 0)
    {
      /* TODO: Propagate error */
      pdf_dealloc (host_path);
      return __pdf_fsys_disk_get_status_from_errno (errno);
    }
  pdf_dealloc (host_path);

  return PDF_OK;
}

/* Host-dependent opendir(), closedir() and friends*/
#ifdef PDF_HOST_WIN32
#define PDF_DIR          _WDIR
#define pdf_dirent_s     _wdirent
#define PDF_OPENDIR(f)   _wopendir((const wchar_t *)f)
#define PDF_READDIR(ds)  _wreaddir((PDF_DIR *)ds)
#define PDF_CLOSEDIR(ds) _wclosedir((PDF_DIR *)ds)
    /* In mingw dir_entry->d_namlen is an array of FILENAME_MAX
       octects long. The dir_entry->d_namlen contain the length of
       the name stored in d_name */
#define PDF_NAMELEN(de)  (de->d_namlen)
#else
#define PDF_DIR          DIR
#define pdf_dirent_s     dirent
#define PDF_OPENDIR(f)   opendir((const char *)f)
#define PDF_READDIR(ds)  readdir((PDF_DIR *)ds)
#define PDF_CLOSEDIR(ds) closedir((PDF_DIR *)ds)
    /* In POSIX systems dir_entry->d_name is a NULL-terminated
       string */
#define PDF_NAMELEN(de)  (strlen(de->d_name))

#endif /* !PDF_HOST_WIN32 */

pdf_status_t
pdf_fsys_disk_get_folder_contents (void             *data,
                                   const pdf_text_t *path_name,
                                   pdf_list_t       *item_list)
{
  pdf_error_t *inner_error = NULL;
  PDF_DIR *dir_stream;
  pdf_char_t *host_path;
  pdf_text_t *entry_text;
  pdf_u32_t name_length;
  struct pdf_dirent_s *dir_entry;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, PDF_EBADDATA);

  /* Get the pathname in the host encoding */
  host_path = __pdf_fsys_disk_get_host_path (path_name,
                                             &inner_error);
  if (!host_path)
    {
      /* TODO: Propagate error */
      pdf_error_destroy (inner_error);
      return PDF_ERROR;
    }

  /* Open the directory stream */
  dir_stream = PDF_OPENDIR (host_path);
  if (!dir_stream)
    {
      pdf_dealloc (host_path);
      /* TODO: Propagate error */
      return __pdf_fsys_disk_get_status_from_errno (errno);
    }

  /* Scan directory contents */
  while ((dir_entry = PDF_READDIR (dir_stream)) != NULL)
    {
      /* Note that dir_entry is statically allocated and can be
         rewritten by a subsequent call. Also, there is not need to
         free that structure */

      /* Get the length of the entry name */
      name_length = PDF_NAMELEN (dir_entry);

      /* Create the text object containing the entry name */
      entry_text = __pdf_fsys_disk_set_host_path ((pdf_char_t *)(dir_entry->d_name),
                                                  name_length,
                                                  &inner_error);
      if (!entry_text)
        {
          PDF_CLOSEDIR (dir_stream);

          /* TODO: Propagate error */
          pdf_error_destroy (inner_error);
          return PDF_ERROR;
        }

      if (!pdf_list_add_last (item_list,
                              (void *) entry_text,
                              &inner_error))
        {
          PDF_CLOSEDIR (dir_stream);

          /* TODO: Propagate error */
          pdf_error_destroy (inner_error);
          return PDF_ERROR;
        }
    }

  PDF_CLOSEDIR (dir_stream);
  return PDF_OK;
}

pdf_status_t
pdf_fsys_disk_get_parent (void              *data,
                          const pdf_text_t  *path_name,
                          pdf_text_t       **parent_path)
{
  PDF_ASSERT_POINTER_RETURN_VAL (path_name, PDF_EBADDATA);
  PDF_ASSERT_POINTER_RETURN_VAL (parent_path, PDF_EBADDATA);

  /* TODO: This involves getting an absolute path from a relative path */

#warning Function 'pdf_fsys_disk_get_parent' not yet implemented

  return PDF_ERROR;
}

/* Host-dependent rmdir() */
#ifdef PDF_HOST_WIN32
#define PDF_RMDIR(f) _wrmdir((wchar_t *)f)
#else
#define PDF_RMDIR(f) rmdir(f)
#endif

pdf_status_t
pdf_fsys_disk_remove_folder (void             *data,
                             const pdf_text_t *path_name)
{
  pdf_error_t *inner_error = NULL;
  pdf_char_t *host_path;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, PDF_EBADDATA);

  /* Get the pathname in the host encoding */
  host_path = __pdf_fsys_disk_get_host_path (path_name,
                                             &inner_error);
  if (!host_path)
    {
      /* TODO: Propagate error */
      pdf_error_destroy (inner_error);
      return PDF_ERROR;
    }

  /* Try to remove the directory */
  if (PDF_RMDIR (host_path) < 0)
    {
      pdf_dealloc (host_path);

      /* TODO: Propagate error */
      return __pdf_fsys_disk_get_status_from_errno (errno);
    }

  pdf_dealloc (host_path);
  return PDF_OK;
}

/* Host-dependent access() */
#ifdef PDF_HOST_WIN32
#define PDF_ACCESS(f,m) _waccess((wchar_t *)f,m)
#else
#define PDF_ACCESS(f,m) access(f,m)
#endif

static pdf_bool_t
__pdf_fsys_disk_is_readable_from_host_path (const pdf_char_t  *host_path,
                                            pdf_error_t      **error)
{
  if (PDF_ACCESS (host_path, R_OK) < 0)
    {
      /* Now, either is not readable, or another error happened */
      if (errno != EACCES)
        pdf_set_error (error,
                       PDF_EDOMAIN_BASE_FS,
                       __pdf_fsys_disk_get_status_from_errno (errno),
                       "cannot check if file is readable: '%s'",
                       strerror (errno));
      return PDF_FALSE;
    }
  return PDF_TRUE;
}

static pdf_bool_t
__pdf_fsys_disk_is_writable_from_host_path (const pdf_char_t  *host_path,
                                            pdf_error_t      **error)
{
  if (PDF_ACCESS (host_path, W_OK) < 0)
    {
      /* Now, either is not readable, or another error happened */
      if (errno != EACCES)
        pdf_set_error (error,
                       PDF_EDOMAIN_BASE_FS,
                       __pdf_fsys_disk_get_status_from_errno (errno),
                       "cannot check if file is writable: '%s'",
                       strerror (errno));
      return PDF_FALSE;
    }
  return PDF_TRUE;
}

/* Host-dependent stat() */
#ifdef PDF_HOST_WIN32
#define PDF_STAT(f,s) _wstat((wchar_t *)f,s)
typedef struct _stat pdf_stat_s;
#else
#define PDF_STAT(f,s) stat(f,s)
typedef struct stat pdf_stat_s;
#endif

static pdf_off_t
__pdf_fsys_disk_file_get_size_from_host_path (const pdf_char_t *host_path,
                                              pdf_error_t      **error)
{
  pdf_stat_s file_info;

  /* TODO: If available, we should try to use the 64-bit versions of stat() */

  if (PDF_STAT (host_path, &file_info) < 0)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FS,
                     __pdf_fsys_disk_get_status_from_errno (errno),
                     "cannot get file size: '%s'",
                     strerror (errno));
      return 0;
    }
  return file_info.st_size;
}

pdf_status_t
pdf_fsys_disk_get_item_props (void                         *data,
                              const pdf_text_t             *path_name,
                              struct pdf_fsys_item_props_s *item_props)
{
  pdf_error_t *inner_error = NULL;
  pdf_char_t* host_path;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, PDF_EBADDATA);
  PDF_ASSERT_POINTER_RETURN_VAL (item_props, PDF_EBADDATA);

  /* Get the pathname in the host encoding */
  host_path = __pdf_fsys_disk_get_host_path (path_name,
                                             &inner_error);
  if (!host_path)
    {
      /* TODO: Propagate error */
      pdf_error_destroy (inner_error);
      return PDF_ERROR;
    }

  /* Is readable? */
  item_props->is_readable = __pdf_fsys_disk_is_readable_from_host_path (host_path,
                                                                        &inner_error);
  if (inner_error)
    {
      pdf_dealloc (host_path);
      /* TODO: Propagate error */
      pdf_error_destroy (inner_error);
      return PDF_ERROR;
    }

  /* Is writable? */
  item_props->is_writable = __pdf_fsys_disk_is_writable_from_host_path (host_path,
                                                                        &inner_error);
  if (inner_error)
    {
      pdf_dealloc (host_path);
      /* TODO: Propagate error */
      pdf_error_destroy (inner_error);
      return PDF_ERROR;
    }

  /* Get file size */
  item_props->file_size = __pdf_fsys_disk_file_get_size_from_host_path (host_path,
                                                                        &inner_error);
  if (inner_error)
    {
      pdf_dealloc (host_path);
      /* TODO: Propagate error */
      pdf_error_destroy (inner_error);
      return PDF_ERROR;
    }

  /* TODO: is hidden ? */
#warning missing check for hidden files
  /* TODO: Get creation date */
#warning missing check for creation date
  /* TODO: Get modification date */
#warning missing check for modification date
  /* TODO: Get folder size */
#warning missing check for folder size

  pdf_dealloc (host_path);

  return PDF_OK;
}

pdf_bool_t
pdf_fsys_disk_item_p (void             *data,
                      const pdf_text_t *path_name)
{
  struct pdf_fsys_item_props_s item_props;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, PDF_FALSE);

#ifdef PDF_HOST_WIN32
  if (__pdf_fsys_disk_win32_device_p (path_name))
    {
      return PDF_TRUE;
    }
#endif
  return ((pdf_fsys_disk_get_item_props (data,
                                         path_name,
                                         &item_props) == PDF_OK) ?
          PDF_TRUE :
          PDF_FALSE);
}

pdf_bool_t
pdf_fsys_disk_item_readable_p (void             *data,
                               const pdf_text_t *path_name)
{
  pdf_error_t *inner_error = NULL;
  pdf_char_t* host_path;
  pdf_bool_t readable;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, PDF_FALSE);

    /* We get the string in HOST-encoding (with NUL-suffix) */
  host_path = __pdf_fsys_disk_get_host_path (path_name, &inner_error);
  if (!host_path)
    {
      /* TODO: Propagate error */
      pdf_error_destroy (inner_error);
      return PDF_FALSE;
    }

  readable = __pdf_fsys_disk_is_readable_from_host_path (host_path,
                                                         &inner_error);
  if (inner_error)
    {
      pdf_dealloc (host_path);
      /* TODO: Propagate error */
      pdf_error_destroy (inner_error);
      return PDF_FALSE;
    }

  pdf_dealloc (host_path);
  return readable;
}

pdf_bool_t
pdf_fsys_disk_item_writable_p (void             *data,
                               const pdf_text_t *path_name)
{
  pdf_error_t *inner_error = NULL;
  pdf_char_t* host_path;
  pdf_bool_t writable;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, PDF_FALSE);

    /* We get the string in HOST-encoding (with NUL-suffix) */
  host_path = __pdf_fsys_disk_get_host_path (path_name, &inner_error);
  if (!host_path)
    {
      /* TODO: Propagate error */
      pdf_error_destroy (inner_error);
      return PDF_FALSE;
    }

  writable = __pdf_fsys_disk_is_writable_from_host_path (host_path,
                                                         &inner_error);
  if (inner_error)
    {
      pdf_dealloc (host_path);
      /* TODO: Propagate error */
      pdf_error_destroy (inner_error);
      return PDF_FALSE;
    }

  pdf_dealloc (host_path);
  return writable;
}

pdf_status_t
pdf_fsys_disk_build_path (void              *data,
                          pdf_text_t       **output,
                          const pdf_text_t  *first_element,
                          const pdf_list_t  *rest)
{
  pdf_error_t *inner_error = NULL;
  pdf_list_iterator_t itr;
  pdf_text_t *next;
  pdf_text_t *text_sep;

  PDF_ASSERT_POINTER_RETURN_VAL (output, PDF_EBADDATA);
  PDF_ASSERT_POINTER_RETURN_VAL (first_element, PDF_EBADDATA);

  pdf_list_iterator_init (&itr, rest);

  *output = pdf_text_dup (first_element, &inner_error);
  if (!*output)
    {
      /* TODO: Propagate error */
      pdf_list_iterator_deinit (&itr);
      return PDF_ENOMEM;
    }

#if FILE_SYSTEM_BACKSLASH_IS_FILE_NAME_SEPARATOR
  text_sep = pdf_text_new_from_unicode ((pdf_char_t*)"\\", 1, PDF_TEXT_UTF8, &inner_error);
#else
  text_sep = pdf_text_new_from_unicode ((pdf_char_t*)"/", 1, PDF_TEXT_UTF8, &inner_error);
#endif /* FILE_SYSTEM_BACKSLASH_IS_FILE_NAME_SEPARATOR */
  if (!text_sep)
    {
      pdf_list_iterator_deinit (&itr);
      pdf_text_destroy (*output);
      /* TODO: Propagate error */
      pdf_error_destroy (inner_error);
      return PDF_ERROR;
    }

  /* Concatenate separator and next text object */
  while (pdf_list_iterator_next (&itr, (const void **) &next, NULL))
    {
      if (!pdf_text_concat (*output, text_sep, PDF_TRUE, &inner_error))
        {
          pdf_list_iterator_deinit (&itr);
          pdf_text_destroy (*output);
          pdf_text_destroy (text_sep);
          /* TODO: Propagate error */
          pdf_error_destroy (inner_error);
          return PDF_ERROR;
        }

      if (!pdf_text_concat (*output, text_sep, PDF_TRUE, &inner_error))
        {
          pdf_list_iterator_deinit (&itr);
          pdf_text_destroy (*output);
          pdf_text_destroy (text_sep);
          /* TODO: Propagate error */
          pdf_error_destroy (inner_error);
          return PDF_ERROR;
        }
    }

  pdf_list_iterator_deinit (&itr);
  pdf_text_destroy (text_sep);
  return PDF_OK;
}

/*
 * File Interface Implementation
 */

enum pdf_fsys_file_mode_e
pdf_fsys_disk_file_get_mode (pdf_fsys_file_t file)
{
  return ((file && file->data != NULL) ?
          (((pdf_fsys_disk_file_t)file->data)->file_mode) :
          PDF_FSYS_OPEN_MODE_INVALID);
}

pdf_text_t *
pdf_fsys_disk_file_get_url (pdf_fsys_file_t file)
{
  PDF_ASSERT_POINTER_RETURN_VAL (file, NULL);
  PDF_ASSERT_POINTER_RETURN_VAL (file->data, NULL);

#warning Correctly implement this, see FS#126

  return NULL;
}

pdf_status_t
pdf_fsys_disk_file_set_mode (pdf_fsys_file_t           file,
                             enum pdf_fsys_file_mode_e new_mode)
{
  pdf_fsys_disk_file_t file_data;

  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_EBADDATA);
  PDF_ASSERT_POINTER_RETURN_VAL (file->data, PDF_ECLOSED);

  file_data = (pdf_fsys_disk_file_t)file->data;

  /* If modes are different, call reopen */
  return (file_data->file_mode != new_mode ?
          pdf_fsys_disk_file_reopen (file, new_mode) :
          PDF_OK);
}

pdf_bool_t
pdf_fsys_disk_file_same_p (pdf_fsys_file_t   file,
                           const pdf_text_t *path)
{
  pdf_error_t *inner_error = NULL;
  pdf_i32_t ret;
  pdf_bool_t case_sensitive;
  pdf_fsys_disk_file_t work_file = (pdf_fsys_disk_file_t)file->data;

  PDF_ASSERT_POINTER_RETURN_VAL (path, PDF_FALSE);

  /* TODO: Mac OS X should have a method in CoreFoundation libs to
   *  check if a the HFS+ filesystem is case-sensitive or not */

  /* Unix-like systems have case sensitive paths, Windows doesn't. */
#ifndef PDF_HOST_WIN32
  case_sensitive = PDF_TRUE;
#else
  case_sensitive = PDF_FALSE;
#endif

  /* TODO : We should be able to get the whole ABSOLUTE path of the
   *         files, before comparing the routes */
#warning need to get absolute paths

  /* Compare text strings */
  ret = pdf_text_cmp (work_file->unicode_path,
                      path,
                      case_sensitive,
                      &inner_error);
  if (inner_error)
    {
      /* TODO: Propagate error */
      pdf_error_destroy (inner_error);
      return PDF_FALSE;
    }
  return (ret == 0 ? PDF_TRUE : PDF_FALSE);
}

pdf_status_t
pdf_fsys_disk_file_get_pos (pdf_fsys_file_t  file,
                            pdf_off_t       *pos)
{
  long cpos;

  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_EBADDATA);
  PDF_ASSERT_POINTER_RETURN_VAL (pos, PDF_EBADDATA);

  cpos = ftello (((pdf_fsys_disk_file_t)file->data)->file_descriptor);
  if (cpos < 0)
    {
      /* TODO: Propagate error */
      return __pdf_fsys_disk_get_status_from_errno (errno);
    }

  *pos = cpos;
  return PDF_OK;
}

pdf_status_t
pdf_fsys_disk_file_set_pos (pdf_fsys_file_t file,
                            pdf_off_t       new_pos)
{
  int st;

  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_EBADDATA);

  st = fseeko (((pdf_fsys_disk_file_t)file->data)->file_descriptor,
               new_pos,
               SEEK_SET);
  if (st < 0)
    {
      /* TODO: Propagate error */
      return __pdf_fsys_disk_get_status_from_errno (errno);
    }
  return PDF_OK;
}

pdf_bool_t
pdf_fsys_disk_file_can_set_size_p (pdf_fsys_file_t file,
                                   pdf_off_t       size)
{
  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_FALSE);

  /* FIXME: Please implement me XD */
#warning method pdf_fsys_disk_file_can_set_size_p not implemented
  return PDF_TRUE;
}

pdf_off_t
pdf_fsys_disk_file_get_size (pdf_fsys_file_t file)
{
  pdf_error_t *inner_error = NULL;
  pdf_off_t size;

  PDF_ASSERT_POINTER_RETURN_VAL (file, 0);

  size = __pdf_fsys_disk_file_get_size_from_host_path (((pdf_fsys_disk_file_t)file->data)->host_path,
                                                       &inner_error);
  if (inner_error)
    {
      /* TODO: Propagate error */
      pdf_error_destroy (inner_error);
      return 0;
    }
  return size;
}

pdf_status_t
pdf_fsys_disk_file_set_size (pdf_fsys_file_t file,
                             pdf_off_t       size)
{
  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_EBADDATA);

  /* FIXME: Please implement me :D */
#warning method pdf_fsys_disk_file_set_size not implemented
  return PDF_OK;
}

pdf_status_t
pdf_fsys_disk_file_read (pdf_fsys_file_t  file,
                         pdf_char_t      *buf,
                         pdf_size_t       bytes,
                         pdf_size_t      *read_bytes)
{
  pdf_fsys_disk_file_t file_data;

  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_EBADDATA);
  PDF_ASSERT_POINTER_RETURN_VAL (buf, PDF_EBADDATA);
  PDF_ASSERT_POINTER_RETURN_VAL (read_bytes, PDF_EBADDATA);

  file_data = (pdf_fsys_disk_file_t)(file->data);
  /* Check if the file is open. */
  if (!file_data)
    {
      /* TODO: Propagate error */
      return PDF_ERROR;
    }

  *read_bytes = fread (buf,
                       1,
                       bytes,
                       file_data->file_descriptor);

  if (feof (file_data->file_descriptor))
    return PDF_EEOF;

  if (ferror (file_data->file_descriptor))
    return PDF_ERROR;

  return PDF_OK;
}

pdf_status_t
pdf_fsys_disk_file_write (pdf_fsys_file_t  file,
                          pdf_char_t      *buf,
                          pdf_size_t       bytes,
                          pdf_size_t      *written_bytes)
{
  pdf_fsys_disk_file_t file_data;

  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_EBADDATA);
  PDF_ASSERT_POINTER_RETURN_VAL (buf, PDF_EBADDATA);
  PDF_ASSERT_POINTER_RETURN_VAL (written_bytes, PDF_EBADDATA);

  file_data = (pdf_fsys_disk_file_t)(file->data);
  /* Check if the file is open. */
  if (!file_data)
    {
      /* TODO: Propagate error */
      return PDF_ERROR;
    }

  *written_bytes = fwrite (buf,
                           1,
                           bytes,
                           file_data->file_descriptor);

  if (ferror (file_data->file_descriptor))
    {
      if (errno == ENOSPC)
        return PDF_ENOMEM;
      return PDF_ERROR;
    }
  return PDF_OK;
}

pdf_status_t
pdf_fsys_disk_file_flush (pdf_fsys_file_t file)
{
  pdf_fsys_disk_file_t file_data;

  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_EBADDATA);

  file_data = (pdf_fsys_disk_file_t) file->data;

  if (fflush (file_data->file_descriptor) < 0)
    {
      /* TODO: Propagate error */

      /* On Windows platforms (excluding Cygwin), fflush does not
         set errno upon failure. */
#ifndef PDF_HOST_WIN32
      return __pdf_fsys_disk_get_status_from_errno (errno);
#else
      return PDF_ERROR;
#endif
    }
  return PDF_OK;
}

pdf_status_t
pdf_fsys_disk_file_request_ria (pdf_fsys_file_t file,
                                pdf_off_t       offset,
                                pdf_size_t      count)
{
  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_EBADDATA);

  /* This filesystem implementation do not provide Read-In-Advance
   * capabilities, so this function is a no-op */

  return PDF_OK;
}

pdf_bool_t
pdf_fsys_disk_file_has_ria (pdf_fsys_file_t file)
{
  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_FALSE);

  /* This filesystem implementation do not provide Read-In-Advance
   * capabilities */

  return PDF_FALSE;
}

pdf_status_t
pdf_fsys_disk_file_cancel_ria (pdf_fsys_file_t file)
{
  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_EBADDATA);

  /* This filesystem implementation do not provide Read-In-Advance
   * capabilities, so this function is a no-op */

  return PDF_OK;
}


/* Host-dependent freopen() */
#ifdef PDF_HOST_WIN32
#define PDF_FREOPEN(f,m,s) _wfreopen((wchar_t *)f,(wchar_t *)m,s)
#else
#define PDF_FREOPEN(f,m,s) freopen_safer (f,m,s)
#endif


pdf_status_t
pdf_fsys_disk_file_reopen (pdf_fsys_file_t           file,
                           enum pdf_fsys_file_mode_e mode)
{
  pdf_fsys_disk_file_t file_data;

  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_EBADDATA);

  file_data = (pdf_fsys_disk_file_t)(file->data);

  /* Check if the file is open. If it's closed, error must be returned
   *  as reopen doesn't have the file path input */
  /* Check if file is closed (no valid data in object */
  if (!file_data)
    {
      PDF_DEBUG_BASE("File is closed! Can't re-open");
      /* TODO: Propagate error */
      return PDF_ECLOSED;
    }

  if (!file_data->file_descriptor)
    {
      PDF_DEBUG_BASE("Invalid FD! Can't re-open");
      /* TODO: Propagate error */
      return PDF_ECLOSED;
    }

  if (!file_data->host_path)
    {
      PDF_DEBUG_BASE("Invalid file object. FD not NULL but empty path");
      /* TODO: Propagate error */
      return PDF_ERROR;
    }

  /* re-open the file */
  file_data->file_descriptor = PDF_FREOPEN (file_data->host_path,
                                            __pdf_fsys_disk_get_mode_string (mode),
                                            file_data->file_descriptor);
  /* reset mode */
  file_data->file_mode = mode;

  /* TODO: Propagate error */

  /* If got a good FD, set output data */
  return ((file_data->file_descriptor != NULL) ?
          PDF_OK :
          __pdf_fsys_disk_get_status_from_errno (errno));
}

/*
 * Private functions
 */

static pdf_char_t *
__pdf_fsys_disk_get_host_path (const pdf_text_t  *path,
                               pdf_error_t      **error)
{
#ifdef PDF_HOST_WIN32
  pdf_size_t length;

  /* For W32, we will always use widechar functions, so Windows' wchar_t
   * implementation should be used (UTF-16LE) */
  return pdf_text_get_unicode (path,
                               PDF_TEXT_UTF16_LE,
                               PDF_TEXT_UNICODE_WITH_NUL_SUFFIX,
                               &length,
                               error);
#else
  /* Call the pdf_text module to get a host-encoded version of the
   *  given path */
  pdf_char_t *str;
  pdf_char_t *nul_suffixed;
  pdf_size_t length = 0;

  str = pdf_text_get_host (path,
                           pdf_text_get_host_encoding (),
                           &length,
                           error);
  if (!str)
    return NULL;

  /* Realloc to append last NUL byte */
  length++;
  nul_suffixed = pdf_realloc (str, length);
  if (!nul_suffixed)
    {
      pdf_dealloc (str);
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FS,
                     PDF_ENOMEM,
                     "cannot get text contents host encoded: "
                     "couldn't reallocate %lu bytes",
                     (unsigned long)length);
      return NULL;
    }
  nul_suffixed[length - 1] = '\0';
  return nul_suffixed;
#endif
}

static pdf_text_t *
__pdf_fsys_disk_set_host_path (const pdf_char_t *host_path,
                               const pdf_size_t  host_path_size,
                               pdf_error_t      **error)
{
#ifdef PDF_HOST_WIN32
  /* For W32, we will always use widechar functions, so Windows' wchar_t
   * implementation should be used (UTF-16LE) */
  return pdf_text_new_from_unicode (host_path,
                                    host_path_size,
                                    PDF_TEXT_UTF16_LE,
                                    error);
#else
  /* Call the pdf_text module to get a host-encoded version of the
   *  given path */
  return pdf_text_new_from_host (host_path,
                                 host_path_size,
                                 pdf_text_get_host_encoding (),
                                 error);
#endif
}

#ifndef PDF_HOST_WIN32
/* Posix-based open mode */
static const pdf_char_t *__pdf_fsys_open_mode_strings[PDF_FSYS_OPEN_MODE_MAX] = {
  /* PDF_FSYS_OPEN_MODE_INVALID  */ (pdf_char_t *)"",
  /* PDF_FSYS_OPEN_MODE_READ     */ (pdf_char_t *)"r",
  /* PDF_FSYS_OPEN_MODE_WRITE    */ (pdf_char_t *)"w",
  /* PDF_FSYS_OPEN_MODE_RW      */ (pdf_char_t *)"r+",
};
#else
  /* Windows portability note:
   *
   * Files are opened in "text mode" (with crlf translation) by
   * default.
   *
   * Although the "b" fopen option is supported by POSIX some old Unix
   * systems may not implement it, so we should use that option to
   * open files only while running in Windows.
   *
   * Also, note that the open modes are in UTF-16 for w32
   */
static const pdf_char_t *__pdf_fsys_open_mode_strings[PDF_FSYS_OPEN_MODE_MAX] = {
  /* PDF_FSYS_OPEN_MODE_INVALID  */ (pdf_char_t *)"\x00\x00",
  /* PDF_FSYS_OPEN_MODE_READ     */ (pdf_char_t *)"\x72\x00\x62\x00\x00\x00",
  /* PDF_FSYS_OPEN_MODE_WRITE    */ (pdf_char_t *)"\x77\x00\x62\x00\x00\x00",
  /* PDF_FSYS_OPEN_MODE_RW       */ (pdf_char_t *)"\x72\x00\x62\x00\x2B\x00\x00\x00",
};
#endif

static const pdf_char_t *
__pdf_fsys_disk_get_mode_string (const enum pdf_fsys_file_mode_e mode)
{
  return __pdf_fsys_open_mode_strings[mode];
}

#ifdef PDF_HOST_WIN32

#define PDF_MAX_W32_DEVICE_NAMES  32
static const pdf_char_t *device_names[PDF_MAX_W32_DEVICE_NAMES] = {
  (pdf_char_t *)"NUL",
  (pdf_char_t *)"CON",
  (pdf_char_t *)"PRN",
  (pdf_char_t *)"AUX",
  (pdf_char_t *)"COM1",
  (pdf_char_t *)"COM2",
  (pdf_char_t *)"COM3",
  (pdf_char_t *)"COM4",
  (pdf_char_t *)"COM5",
  (pdf_char_t *)"COM6",
  (pdf_char_t *)"COM7",
  (pdf_char_t *)"COM8",
  (pdf_char_t *)"COM9",
  (pdf_char_t *)"LPT1",
  (pdf_char_t *)"LPT2",
  (pdf_char_t *)"LPT3",
  (pdf_char_t *)"LPT4",
  (pdf_char_t *)"LPT5",
  (pdf_char_t *)"LPT6",
  (pdf_char_t *)"LPT7",
  (pdf_char_t *)"LPT8",
  (pdf_char_t *)"LPT9"
};

static pdf_bool_t
__pdf_fsys_disk_win32_device_p (const pdf_text_t  *path,
                                pdf_error_t      **error)
{
  /* The following special "files", which access devices, exist in all
     directories, case-insensitively, and with all possible endings
     after a period or colon, excpets in pathnames that start with
     \\?\: */
  int i;
  pdf_text_t *device_name;

  device_p = PDF_FALSE;
  for (i = 0; i < PDF_MAX_W32_DEVICE_NAMES; ++i)
    {
      pdf_error_t *inner_error = NULL;

      device_name = pdf_text_new_from_unicode (device_names[i],
                                               strlen (device_names[i]),
                                               PDF_TEXT_UTF8,
                                               error);
      if (!device_name)
        return PDF_FALSE;

      if (pdf_text_cmp (path,
                        device_name,
                        PDF_FALSE,
                        &inner_error) == 0)
        {
          pdf_text_destroy (device_name);
          return PDF_TRUE;
        }

      pdf_text_destroy (device_name);

      if (inner_error)
        {
          pdf_propagate_error (error, inner_error);
          return PDF_FALSE;
        }
    }

  return PDF_FALSE;
}

#endif /* !PDF_HOST_WIN32 */

static pdf_status_t
__pdf_fsys_disk_get_status_from_errno(int _errno)
{
  switch (_errno)
    {
      case EINVAL:
      case ESPIPE:
      case EOVERFLOW:
      case EBADF:
      case EFAULT:
      case EFBIG:
        /* Bad data */
        return PDF_EBADDATA;
      case EACCES:
      case EPERM:
      case EROFS:
      case ETXTBSY:
        /* Not enough permissions */
        return PDF_EBADPERMS;
      case EISDIR:
      case ENAMETOOLONG:
      case ENOENT:
      case ENOTDIR:
#ifdef ELOOP
      case ELOOP:
#endif
        /* Invalid path name */
        return PDF_EBADNAME;
      case ENOMEM:
        /* No memory */
        return PDF_ENOMEM;
      case EEXIST:
        /* File Exists */
        return PDF_EEXIST;
      case ENOTEMPTY:
        /* Not empty */
        return PDF_ENOTEMPTY;
      case EAGAIN:
        /* non-blocking descriptor and blocking writing
           requested */
        return PDF_EAGAIN;
      case ENOSPC:
        /* Not enough room in disk */
        return PDF_ENOSPC;
      default:
        /* Other error */
        return PDF_ERROR;
    }
}

/* End of pdf-fsys-disk.c */

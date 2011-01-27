/* -*- mode: C -*-
 *
 *       File:         pdf-fsys-disk.c
 *       Date:         Thu May 22 18:27:35 2008
 *
 *       GNU PDF Library - Disk Filesystem Implementation
 *
 */

/* Copyright (C) 2008, 2010 Free Software Foundation, Inc. */

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

static pdf_status_t
__pdf_fsys_disk_get_status_from_errno(int _errno);
static pdf_status_t
__pdf_fsys_disk_get_host_path (const pdf_text_t path,
                               pdf_char_t **host_path,
                               pdf_size_t *host_path_size);
static pdf_status_t
__pdf_fsys_disk_set_host_path (const pdf_char_t *host_path,
                               const pdf_size_t  host_path_size,
                               pdf_text_t *p_path);
static pdf_status_t
__pdf_fsys_disk_is_writable_from_host_path(const pdf_char_t *host_path,
                                           pdf_bool_t *p_result);
static pdf_status_t
__pdf_fsys_disk_is_readable_from_host_path(const pdf_char_t *host_path,
                                           pdf_bool_t *p_result);
static pdf_status_t
__pdf_fsys_disk_file_get_size_from_host_path(const pdf_char_t *host_path,
                                             pdf_size_t *p_result);
static const pdf_char_t *
__pdf_fsys_disk_get_mode_string(const enum pdf_fsys_file_mode_e mode);

#ifdef PDF_HOST_WIN32
static pdf_bool_t
__pdf_fsys_disk_win32_device_p (pdf_text_t path);
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
pdf_fsys_disk_get_free_space (void *data,
                              pdf_text_t path_name)
{
  pdf_char_t *utf16le_path = NULL;
  pdf_u32_t utf16le_path_size = 0;
  ULARGE_INTEGER free_bytes;
  pdf_i64_t result = pdf_i64_new((32 << 1),1); /* (-1) */

  /* Note that as we get the string as UTF-16LE with LAST NUL suffix,
   *  it's equivalent to a wchar_t in windows environments */


  if (pdf_text_get_unicode (&utf16le_path,
                            &utf16le_path_size,
                            path_name,
                            PDF_TEXT_UTF16_LE,
                            (PDF_TEXT_UNICODE_WITH_NUL_SUFFIX |     \
                             PDF_TEXT_UNICODE_WITH_BOM)) != PDF_OK)
    {
      PDF_DEBUG_BASE("Couldn't get UTF-16LE path");
    }
  else
    {
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
      if (GetDiskFreeSpaceExW ((LPCWSTR) utf16le_path,
                               NULL, 
                               NULL,
                               &free_bytes))
        {
          pdf_status_t i64_ret_code;
          /* Assign the number of bytes in the pdf_i64_t */
          pdf_i64_assign(&result,
                         free_bytes.HighPart,
                         free_bytes.LowPart,
                         &i64_ret_code);
        }

      /* Cleanup */
      pdf_dealloc (utf16le_path);
    }

  /* Return the free space in octects */
  return result;
}

#else

pdf_i64_t
pdf_fsys_disk_get_free_space (void *data,
                              pdf_text_t path_name)
{
  pdf_i64_t result = pdf_i64_new((32 << 1),1); /* (-1) */

  /* Safety check that path_name is a valid pointer */
  if(path_name != NULL)
    {
      struct statfs fs_stats;
      pdf_char_t *host_path = NULL;
      pdf_size_t host_path_size = 0;


      /* We get the string in HOST-encoding (with NUL-suffix) */
      if (__pdf_fsys_disk_get_host_path (path_name,
                                       &host_path,
                                       &host_path_size) != PDF_OK)
        {
          PDF_DEBUG_BASE("Couldn't get host-encoded path");
        }
      else
        {
          PDF_DEBUG_BASE("Getting free bytes of FS at path '%s'...", host_path);
          if (statfs ((const char *) host_path, &fs_stats) == 0)
            {
              pdf_status_t i64_ret_code = PDF_OK;
              /* Compute the number of bytes in the pdf_i64_t */
              pdf_i64_assign_quick(&result,
                                   fs_stats.f_bfree,
                                   &i64_ret_code);  
              /* Only continue operation if assign is OK */
              if(i64_ret_code == PDF_OK)
                {
                  pdf_i64_mult_i32(&result,
                                   result,
                                   fs_stats.f_bsize,
                                   &i64_ret_code);
                }
              /* If any of the previous failed, reset return value */
              if(i64_ret_code != PDF_OK)
                {
                  PDF_DEBUG_BASE("Couldn't compute bfree*bsize");
                  pdf_i64_assign_quick(&result, -1, &i64_ret_code); /* (-1) */
                }
            }
          else
            {
              PDF_DEBUG_BASE("Statfs failed... at path: '%s'", host_path);
            }

          /* Cleanup */
          pdf_dealloc (host_path);
        }
    }

  /* Return the free space in octects */
  return result;
}

#endif /* !PDF_HOST_WIN32 */



/* Private function to de-initialize and de-allocate base file data */
static void
__pdf_fsys_deinit_base_file_data(pdf_fsys_disk_file_t *p_file_data)
{
  if((p_file_data != NULL) && \
     (*p_file_data != NULL))
    {
      if((*p_file_data)->unicode_path != NULL)
        {
          pdf_text_destroy ((*p_file_data)->unicode_path);
        }
      if((*p_file_data)->host_path != NULL)
        {
          pdf_dealloc((*p_file_data)->host_path);
        }
      pdf_dealloc (*p_file_data);
      *p_file_data = NULL;
    }
}


/* Private function to allocate and initialize base file data */
static pdf_fsys_disk_file_t
__pdf_fsys_init_base_file_data(const pdf_text_t path_name)
{
  pdf_fsys_disk_file_t file_data = NULL;
  if(path_name != NULL)
    {
      /* Allocate private data storage for the file */
      file_data = (pdf_fsys_disk_file_t) pdf_alloc (sizeof(struct pdf_fsys_disk_file_s));
      if(file_data != NULL)
        {
          memset(file_data, 0, sizeof(struct pdf_fsys_disk_file_s));

          /* Make and store a copy of the unicode file path and get the host
             encoded path */
          file_data->unicode_path = pdf_text_dup (path_name);
          if(file_data->unicode_path != NULL)
            {
              if (__pdf_fsys_disk_get_host_path(file_data->unicode_path,
                                                &(file_data->host_path),
                                                &(file_data->host_path_size)) != PDF_OK)
                {
                  pdf_text_destroy(file_data->unicode_path);
                  pdf_dealloc(file_data);
                  file_data = NULL;
                }
            }
          else
            {
              pdf_dealloc(file_data);
              file_data = NULL;
            }
        }
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
pdf_fsys_disk_file_open (void *data,
                         const pdf_text_t path_name,
                         const enum pdf_fsys_file_mode_e mode,
                         pdf_fsys_file_t *p_file)
{
  pdf_status_t ret_status = PDF_EBADDATA;

  if((path_name != NULL) && \
     (p_file != NULL))
    {
      pdf_fsys_file_t file = NULL;
      pdf_fsys_disk_file_t file_data = NULL;
      const pdf_char_t * mode_str;

      /* Allocate file struct */
      file = (pdf_fsys_file_t)pdf_alloc(sizeof(struct pdf_fsys_file_s));

      if(file != NULL)
        {
          /* Get base data */
          file_data = __pdf_fsys_init_base_file_data(path_name);

          /* Get the mode string for fopen */
          mode_str =__pdf_fsys_disk_get_mode_string(mode);

          /* Only continue if previous operations went ok */
          if((file_data != NULL) &&             \
             (mode_str != NULL))
            {
              /* Open the file */
              file_data->file_descriptor = PDF_FOPEN(file_data->host_path, mode_str);
              /* If got a good FD, set output data */
              if (file_data->file_descriptor != NULL)
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
                  ret_status = PDF_OK;
                }
              else
                {
                  PDF_DEBUG_BASE("fopen failed...");
                  /* Deallocate and deinit base file data */
                  __pdf_fsys_deinit_base_file_data(&file_data);
                  pdf_dealloc(file);
                  /* Get pdf_status_t code from errno */
                  ret_status = __pdf_fsys_disk_get_status_from_errno(errno);
                }
            }
          else
            {
              pdf_dealloc(file);
            }
        }
    }

  /* All was ok */
  return ret_status;
}

pdf_status_t
pdf_fsys_disk_file_open_tmp (void *data,
                             pdf_fsys_file_t *p_file)
{
  pdf_text_t path_name;
  pdf_status_t ret_status = PDF_ERROR;
  pdf_fsys_file_t file = NULL;
  pdf_fsys_disk_file_t file_data = NULL;
  

  file = (pdf_fsys_file_t) pdf_alloc (sizeof (struct pdf_fsys_file_s));
  file->fs = NULL; /* Default filesystem.  */
  
  if (file != NULL)
    {
      /* Create a dummy path name, so we can manage the temporary file
         like an ordinary file.  */
      if (pdf_text_new_from_unicode ((pdf_char_t *) "tmp",
                                     3,
                                     PDF_TEXT_UTF8,
                                     &path_name)
          != PDF_OK)
        {
          ret_status = PDF_ENOMEM;
        }
      else
        {
          /* Get base data.  */
          file_data = __pdf_fsys_init_base_file_data (path_name);
          file_data->file_mode = PDF_FSYS_OPEN_MODE_RW;

          /* Open a temporary file.  */
          file_data->file_descriptor = tmpfile_safer ();
          if (file_data->file_descriptor != NULL)
            {
              /* Success.  */
              file->data = (void *) file_data;
              *p_file = file;
              ret_status = PDF_OK;
            }
          else
            {
              pdf_text_destroy (path_name);
            }
        }
    }

  return ret_status;
}


pdf_status_t
pdf_fsys_disk_file_close (pdf_fsys_file_t file)
{
  pdf_status_t ret_code = PDF_EBADDATA;
  if((file != NULL) && \
     (file->data != NULL))
    {
      ret_code = PDF_OK;
      /* Close the I/O stream only if already open */
      if(((pdf_fsys_disk_file_t)(file->data))->file_descriptor != NULL)
        {
          if(fclose (((pdf_fsys_disk_file_t)(file->data))->file_descriptor) == EOF)
            {
              PDF_DEBUG_BASE("fclose returned an error");
              /* An error was detected closing the I/O stream */
              ret_code = PDF_ERROR;
            }
          PDF_DEBUG_BASE("Closed");
          /* Deallocate other internal contents */
          __pdf_fsys_deinit_base_file_data((pdf_fsys_disk_file_t *)&(file->data));
        }
      else
        {
          PDF_DEBUG_BASE("File is already closed");
        }

      if(file != NULL)
        {
          pdf_dealloc(file);
        }
    }
  return ret_code;
}



/* Host-dependent mkdir() */
#ifdef PDF_HOST_WIN32
#define PDF_MKDIR(f,m) _wmkdir((const wchar_t *)f)
#else
#define PDF_MKDIR(f,m) mkdir(f,m)
#endif

pdf_status_t
pdf_fsys_disk_create_folder (void *data,
                             const pdf_text_t path_name)
{
  pdf_status_t ret_code = PDF_EBADDATA;
  if(path_name != NULL)
    {
      pdf_char_t *host_path;
      pdf_size_t host_path_size;

      /* Default return code */
      ret_code = PDF_OK;

      /* Get a host-encoded version of the path name */
      if (__pdf_fsys_disk_get_host_path(path_name, 
                                        &host_path, 
                                        &host_path_size) != PDF_OK)
        {
          PDF_DEBUG_BASE("Couldn't get host path to create the folder");
          ret_code = PDF_ERROR;
        }
      else
        {
          /* Set the permissions of the new directory (posix-only):
             rwxr_xr_x
          */
          /* Open the file */
          if (PDF_MKDIR(host_path,                     \
                        (S_IRUSR | S_IWUSR | S_IXUSR | \
                         S_IRGRP | S_IXGRP |           \
                         S_IROTH | S_IXOTH)) != 0)
            {
              PDF_DEBUG_BASE("mkdir failed with errno '%d'", errno);
              ret_code = __pdf_fsys_disk_get_status_from_errno(errno);
            }
        }
    }

  return ret_code;
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
pdf_fsys_disk_get_folder_contents (void *data,
                                   const pdf_text_t path_name,
                                   pdf_list_t item_list)
{
  pdf_status_t ret_code = PDF_EBADDATA;
  if(path_name != NULL)
    {
      PDF_DIR *dir_stream = NULL;
      pdf_char_t *host_path = NULL;
      pdf_size_t host_path_size;
      pdf_text_t entry_text = NULL;
      pdf_u32_t name_length;
  
      /* Default here is ok... */
      ret_code = PDF_OK;

      /* Get the pathname in the host encoding */
      if (__pdf_fsys_disk_get_host_path(path_name,
                                        &host_path,
                                        &host_path_size) != PDF_OK)
        {
          PDF_DEBUG_BASE("Couldn't get host path to get folder contents");
          ret_code = PDF_ERROR;
        }
      else
        {
          /* Open the directory stream */
          dir_stream = PDF_OPENDIR(host_path);
          if (dir_stream == NULL)
            {
              PDF_DEBUG_BASE("opendir failed with errno: '%d'",errno);
              ret_code = __pdf_fsys_disk_get_status_from_errno(errno);
            }
          else
            {
              struct pdf_dirent_s *dir_entry = NULL;
              /* Scan directory contents */
              while ((ret_code == PDF_OK) &&                            \
                     (dir_stream != NULL) &&                            \
                     (dir_entry = PDF_READDIR (dir_stream)) != NULL)
                {
                  /* Note that dir_entry is statically allocated and can be
                     rewritten by a subsequent call. Also, there is not need to
                     free that structure */
              
                  /* Get the length of the entry name */
                  name_length = PDF_NAMELEN(dir_entry);
              
                  /* Create the text object containing the entry name */
                  if ((__pdf_fsys_disk_set_host_path((pdf_char_t *)(dir_entry->d_name),
                                                     name_length,
                                                     &entry_text) != PDF_OK) ||
                      (pdf_list_add_last (item_list, 
                                          (void *) entry_text, NULL) != PDF_OK))
                    {
                      /* Stop loop and report an error */
                      ret_code = PDF_ERROR;
                    }
                }
              /* Close dir... */
              PDF_CLOSEDIR(dir_stream);
            }
        }
    }
  return ret_code;
}


pdf_status_t
pdf_fsys_disk_get_parent (void *data,
                          const pdf_text_t path_name,
                          pdf_text_t parent_path)
{
  pdf_status_t ret_code = PDF_EBADDATA;
  if((path_name != NULL) &&                     \
     (parent_path != NULL))
    {
      /* TODO: This involves getting an absolute path from a relative path */
    }

  return ret_code;
}


/* Host-dependent rmdir() */
#ifdef PDF_HOST_WIN32
#define PDF_RMDIR(f) _wrmdir((wchar_t *)f)
#else
#define PDF_RMDIR(f) rmdir(f)
#endif

pdf_status_t
pdf_fsys_disk_remove_folder (void *data,
                             const pdf_text_t path_name)
{
  pdf_status_t ret_code = PDF_EBADDATA;
  if(path_name != NULL)
    {
      pdf_char_t *host_path;
      pdf_size_t host_path_size;

      /* Get the pathname in the host encoding */
      if (__pdf_fsys_disk_get_host_path (path_name,
                                         &host_path,
                                         &host_path_size) != PDF_OK)
        {
          ret_code = PDF_ERROR;
        }
      else
        {
          /* Try to remove the directory */
          if (PDF_RMDIR(host_path) != 0)
            {
              /* Cleanup */
              pdf_dealloc (host_path);
              
              ret_code = __pdf_fsys_disk_get_status_from_errno(errno);
            }
        }
      /* Cleanup */
      pdf_dealloc (host_path);
    }

  return ret_code;
}

/* Host-dependent access() */
#ifdef PDF_HOST_WIN32
#define PDF_ACCESS(f,m) _waccess((wchar_t *)f,m)
#else
#define PDF_ACCESS(f,m) access(f,m)
#endif


static pdf_status_t
__pdf_fsys_disk_is_readable_from_host_path(const pdf_char_t *host_path,
                                           pdf_bool_t *p_result)
{
  pdf_status_t ret_code = PDF_EBADDATA;
  /* Check if file can be read */
  if((host_path != NULL) && \
     (p_result != NULL))
    {
      if(PDF_ACCESS(host_path, R_OK) == -1)
        {
          /* Now, either is not readable, or another error happened */
          if(errno == EACCES)
            {
              *p_result = PDF_FALSE;
              ret_code = PDF_OK;
            }
          else
            {
              ret_code = __pdf_fsys_disk_get_status_from_errno(errno);
            }
        }
      else
        {
          *p_result = PDF_TRUE;
          ret_code = PDF_OK;
        }
    }
  return ret_code;
}

static pdf_status_t
__pdf_fsys_disk_is_writable_from_host_path(const pdf_char_t *host_path,
                                           pdf_bool_t *p_result)
{
  pdf_status_t ret_code = PDF_EBADDATA;
  /* Check if file can be written */
  if((host_path != NULL)  && \
     (p_result != NULL))
    {
      if(PDF_ACCESS(host_path, W_OK) == -1)
        {
          /* Now, either is not readable, or another error happened */
          if(errno == EACCES)
            {
              *p_result = PDF_FALSE;
              ret_code = PDF_OK;
            }
          else
            {
              ret_code = __pdf_fsys_disk_get_status_from_errno(errno);
            }
        }
      else
        {
          *p_result = PDF_TRUE;
          ret_code = PDF_OK;
        }
    }
  return ret_code;
}

/* Host-dependent stat() */
#ifdef PDF_HOST_WIN32
#define PDF_STAT(f,s) _wstat((wchar_t *)f,s)
typedef struct _stat pdf_stat_s;
#else
#define PDF_STAT(f,s) stat(f,s)
typedef struct stat pdf_stat_s;
#endif

static pdf_status_t
__pdf_fsys_disk_file_get_size_from_host_path(const pdf_char_t *host_path,
                                             pdf_size_t *p_result)
{
  pdf_status_t ret_code = PDF_EBADDATA;
  /* Check if file can be written */
  if((host_path != NULL)  && \
     (p_result != NULL))
    {

      /* TODO:
       * If available, we should try to use the 64-bit versions of stat() */

      pdf_stat_s file_info;
      if (PDF_STAT(host_path, &file_info) != 0)
        {
          ret_code = __pdf_fsys_disk_get_status_from_errno(errno);
        }
      else
        {
          *p_result = file_info.st_size;
          ret_code = PDF_OK;
        }
    }

  return ret_code;
}



pdf_status_t
pdf_fsys_disk_get_item_props (void *data,
                              pdf_text_t path_name,
                              struct pdf_fsys_item_props_s *item_props)
{
  pdf_char_t* host_path = NULL;
  pdf_size_t host_path_len = 0;
  pdf_status_t ret_code = PDF_EBADDATA;

  /* Get host path */
  if((path_name != NULL) && \
     (item_props != NULL))
    {
      ret_code = __pdf_fsys_disk_get_host_path (path_name,
                                                &host_path,
                                                &host_path_len);
    }

  /* Is readable ? */
  if(ret_code == PDF_OK)
    {
      ret_code = __pdf_fsys_disk_is_readable_from_host_path(host_path,                 \
                                                            &(item_props->is_readable));
    }

  /* Is writable ? */
  if(ret_code == PDF_OK)
    {
      ret_code = __pdf_fsys_disk_is_writable_from_host_path(host_path,                 \
                                                            &(item_props->is_writable));
    }

  /* TODO: is hidden ? */


  /* TODO: Get creation date */


  /* TODO: Get modification date */


  /* Get file size */
  if(ret_code == PDF_OK)
    {
      pdf_size_t size;

      ret_code = __pdf_fsys_disk_file_get_size_from_host_path(host_path, &size);
      if(ret_code == PDF_OK)
        {
          item_props->file_size_high = 0;
          item_props->file_size_low = size;
        }
    }


  /* TODO: Get folder size */


  /* Clean aux host path */
  if (host_path != NULL)
    {
      pdf_dealloc (host_path);
    }

  return ret_code;
}


pdf_bool_t
pdf_fsys_disk_item_p (void *data,
                      pdf_text_t path_name)
{
  struct pdf_fsys_item_props_s item_props;
#ifdef PDF_HOST_WIN32
  if (__pdf_fsys_disk_win32_device_p(path_name) == PDF_TRUE)
    {
      return PDF_TRUE;
    }
#endif
  return ((pdf_fsys_disk_get_item_props(data,
                                        path_name,
                                        &item_props) == PDF_OK) ?
          PDF_TRUE :
          PDF_FALSE);
}


pdf_bool_t 
pdf_fsys_disk_item_readable_p (void *data,
                               pdf_text_t path_name)
{
  pdf_bool_t result = PDF_FALSE;
  if(path_name != NULL)
    {
      pdf_char_t* host_path = NULL;
      pdf_size_t host_path_len = 0;

      if(__pdf_fsys_disk_get_host_path (path_name,
                                        &host_path,
                                        &host_path_len) == PDF_OK)
        {
          __pdf_fsys_disk_is_readable_from_host_path(host_path, &result);
          pdf_dealloc(host_path);
        }
    }
  return result;
}




pdf_bool_t
pdf_fsys_disk_item_writable_p (void *data,
                               pdf_text_t path_name)
{
  pdf_bool_t result = PDF_FALSE;
  if(path_name != NULL)
    {
      pdf_char_t* host_path = NULL;
      pdf_size_t host_path_len = 0;

      if(__pdf_fsys_disk_get_host_path (path_name,
                                        &host_path,
                                        &host_path_len) == PDF_OK)
        {
          __pdf_fsys_disk_is_writable_from_host_path(host_path, &result);
          pdf_dealloc(host_path);
        }
    }
  return result;
}

pdf_status_t
pdf_fsys_disk_build_path (void *data,
                          pdf_text_t * output,
                          pdf_text_t first_element,
                          pdf_list_t rest)
{
  pdf_list_iterator_t itr;
  pdf_text_t next, text_sep;
  pdf_status_t st, st2;

  st = pdf_list_iterator (rest, &itr);
  if (st != PDF_OK)
    {
      return st;
    }

  *output = pdf_text_dup (first_element);
  if (*output == NULL)
    {
      pdf_list_iterator_free (&itr);
      return PDF_ENOMEM;
    }

#if FILE_SYSTEM_BACKSLASH_IS_FILE_NAME_SEPARATOR
  st = pdf_text_new_from_unicode ((pdf_char_t*)"\\",1,PDF_TEXT_UTF8, &text_sep);
#else
  st = pdf_text_new_from_unicode ((pdf_char_t*)"/",1,PDF_TEXT_UTF8, &text_sep);
#endif /* FILE_SYSTEM_BACKSLASH_IS_FILE_NAME_SEPARATOR */

  if (st != PDF_OK)
    {
      pdf_list_iterator_free (&itr);
      pdf_text_destroy(*output);
      return st;
    }

  /* Concatenate separator and next text object */
  st = pdf_list_iterator_next (&itr, (const void **) &next, NULL);
  while (st == PDF_OK)
    {
      st2 = pdf_text_concat (*output, text_sep, PDF_TRUE);
      if (st2 != PDF_OK)
        {
          pdf_list_iterator_free (&itr);
          pdf_text_destroy(*output);
          pdf_text_destroy (text_sep);
          return st2;
        }
      
      st2 = pdf_text_concat (*output, next, PDF_TRUE);
      if (st2 != PDF_OK)
        {
          pdf_list_iterator_free (&itr);
          pdf_text_destroy(*output);
          pdf_text_destroy (text_sep);
          return st2;
        }
      st = pdf_list_iterator_next (&itr, (const void **) &next, NULL);
    }

  pdf_list_iterator_free (&itr);
  pdf_text_destroy (text_sep);
  return PDF_OK;
}


/*
 * File Interface Implementation
 */

enum pdf_fsys_file_mode_e
pdf_fsys_disk_file_get_mode (pdf_fsys_file_t file)
{
  return (((file != NULL) && (file->data != NULL)) ?        \
          (((pdf_fsys_disk_file_t)file->data)->file_mode) : \
          PDF_FSYS_OPEN_MODE_INVALID);
}

pdf_text_t 
pdf_fsys_disk_file_get_url (pdf_fsys_file_t file)
{
  if((file != NULL) && (file->data != NULL))
    {
#ifdef PDF_HOST_WIN32
      /* In W32, we just copy the path string */
      return pdf_text_dup((((pdf_fsys_disk_file_t)file->data)->unicode_path));
#else
      /* In POSIX, we return file:/ plus the path string */ 
      pdf_text_t url = NULL;
#define PDF_DISK_URL_PREFIX  (pdf_char_t *)"file:/"
      if(pdf_text_new_from_unicode(PDF_DISK_URL_PREFIX,
                                   strlen(PDF_DISK_URL_PREFIX),
                                   PDF_TEXT_UTF8,
                                   &url) == PDF_OK)
        {
          if(pdf_text_concat(url,
                             (((pdf_fsys_disk_file_t)file->data)->unicode_path),
                             PDF_TRUE) == PDF_OK)
            {
              return url;
            }
        }
#undef PDF_DISK_URL_PREFIX
      if(url != NULL)
        {
          pdf_text_destroy(url);
        }
      return NULL;
#endif
    }
  else
    return NULL;
}

pdf_status_t
pdf_fsys_disk_file_set_mode (pdf_fsys_file_t file,
                             enum pdf_fsys_file_mode_e new_mode)
{
  pdf_status_t ret_code = PDF_EBADDATA;
  if(file != NULL)
    {
      if(file->data == NULL)
        {
          ret_code = PDF_ECLOSED;
        }
      else
        {
          pdf_fsys_disk_file_t file_data = (pdf_fsys_disk_file_t)file->data;
          /* If modes are different, call reopen */
          if(file_data->file_mode != new_mode)
            {
              ret_code = pdf_fsys_disk_file_reopen(file,new_mode);
            }
          else
            {
              /* Same mode, so don't do anything */
              ret_code = PDF_OK;
            }
        }
    }
  return ret_code;
}

pdf_bool_t
pdf_fsys_disk_file_same_p (pdf_fsys_file_t file,
                           pdf_text_t path)
{
  pdf_status_t stat;
  pdf_i32_t ret;
  pdf_bool_t case_sensitive;
  pdf_fsys_disk_file_t work_file = (pdf_fsys_disk_file_t)file->data;



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



  /* Compare text strings */
  ret = pdf_text_cmp(work_file->unicode_path, path, case_sensitive, &stat);

  return (((ret == 0 ) && (stat == PDF_OK)) ? PDF_TRUE : PDF_FALSE);
}


pdf_status_t
pdf_fsys_disk_file_get_pos (pdf_fsys_file_t file,
                            pdf_size_t *pos)
{
  if((file != NULL) && \
     (pos != NULL))
    {
      long cpos;
      cpos = ftell (((pdf_fsys_disk_file_t)file->data)->file_descriptor);
      if (cpos<0)
        {
          return __pdf_fsys_disk_get_status_from_errno (errno);
        }
      else
        {
          *pos = cpos;
          return PDF_OK;
        }
    }
  else
    {
      return PDF_EBADDATA;
    }
}

pdf_status_t
pdf_fsys_disk_file_set_pos (pdf_fsys_file_t file,
                            pdf_size_t new_pos)
{
  if(file != NULL)
    {
      int st;
      st = fseek (((pdf_fsys_disk_file_t)file->data)->file_descriptor,
                 new_pos, SEEK_SET);
      if (st < 0)
        {
          return __pdf_fsys_disk_get_status_from_errno (errno);
        }
      else
        {
          return PDF_OK;
        }
    }
  else
    {
      return PDF_EBADDATA;
    }
}

pdf_bool_t
pdf_fsys_disk_file_can_set_size_p (pdf_fsys_file_t file,
                                   pdf_size_t size)
{
  /* FIXME: Please implement me XD */
  return PDF_TRUE;
}

pdf_size_t
pdf_fsys_disk_file_get_size (pdf_fsys_file_t file)
{
  pdf_size_t size;
  if(__pdf_fsys_disk_file_get_size_from_host_path(((pdf_fsys_disk_file_t)file->data)->host_path,
                                                  &size) == PDF_OK)
    {
      return size;
    }
  else
    {
      /* Should we change the API to return pdf_status_t ? */
      return 0;
    }
}

pdf_status_t
pdf_fsys_disk_file_set_size (pdf_fsys_file_t file,
                             pdf_size_t size)
{
  /* FIXME: Please implement me :D */
  return PDF_OK;
}


pdf_status_t
pdf_fsys_disk_file_read (pdf_fsys_file_t file, pdf_char_t *buf,
                         pdf_size_t bytes, pdf_size_t *read_bytes)
{
  pdf_status_t ret = PDF_EBADDATA;
  if((file != NULL) &&                          \
     (buf != NULL))
    {
      pdf_fsys_disk_file_t file_data = NULL;
      file_data = (pdf_fsys_disk_file_t)(file->data);

      /* Check if the file is open. */
      if(file_data == NULL)
        {
          PDF_DEBUG_BASE("File is closed! Can't read data");
        }
      else
        {
          *read_bytes = fread(buf,
                              1,
                              bytes,
                              file_data->file_descriptor);
          if (feof(file_data->file_descriptor))
            {
              ret = PDF_EEOF;
            }
          else if (ferror(file_data->file_descriptor))
            {
              ret = PDF_ERROR;
            }
          else
            {
              ret = PDF_OK;
            }
        }
    }
  else
    {
      PDF_DEBUG_BASE("Invalid inputs to file_read! (file:%p, buf:%p, size:%u)",
                     file,buf,bytes);
    }
  return ret;
}


pdf_status_t
pdf_fsys_disk_file_write (pdf_fsys_file_t file, pdf_char_t *buf,
                          pdf_size_t bytes, pdf_size_t *written_bytes)
{
  pdf_status_t ret = PDF_EBADDATA;
  if((file != NULL) && \
     (buf != NULL))
    {
      pdf_fsys_disk_file_t file_data = NULL;
      file_data = (pdf_fsys_disk_file_t)(file->data);

      /* Check if the file is open. */
      if(file_data == NULL)
        {
          PDF_DEBUG_BASE("File is closed! Can't write data");
        }
      else
        {
          *written_bytes = fwrite(buf,
                                  1,
                                  bytes,
                                  file_data->file_descriptor);
          if (ferror(file_data->file_descriptor))
            {
              if (errno == ENOSPC)
                {
                  ret = PDF_ENOMEM;
                }
              else
                {
                  ret = PDF_ERROR;
                }
            }
          else
            {
              ret = PDF_OK;
            }
        }
    }
  else
    {
      PDF_DEBUG_BASE("Invalid inputs to file_write! (file:%p, buf:%p, size:%u)",
                     file,buf,bytes);
    }
  return ret;

}


pdf_status_t
pdf_fsys_disk_file_flush (pdf_fsys_file_t file)
{
  pdf_fsys_disk_file_t file_data;
  pdf_status_t ret_code;

  file_data = (pdf_fsys_disk_file_t) file->data;

  if (fflush (file_data->file_descriptor) != 0)
    {
      /* On Windows platforms (excluding Cygwin), fflush does not
         set errno upon failure. */
#ifndef PDF_HOST_WIN32
      ret_code = __pdf_fsys_disk_get_status_from_errno(errno);
#else
      ret_code = PDF_ERROR;
#endif
    }
  else
    {
      ret_code = PDF_OK;
    }

  return ret_code;
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


/* Host-dependent freopen() */
#ifdef PDF_HOST_WIN32
#define PDF_FREOPEN(f,m,s) _wfreopen((wchar_t *)f,(wchar_t *)m,s)
#else
#define PDF_FREOPEN(f,m,s) freopen_safer (f,m,s)
#endif


pdf_status_t
pdf_fsys_disk_file_reopen (pdf_fsys_file_t file,
                           enum pdf_fsys_file_mode_e mode)
{
  pdf_status_t ret_code = PDF_EBADDATA;
  if(file != NULL)
    {
      pdf_fsys_disk_file_t file_data = NULL;

      file_data = (pdf_fsys_disk_file_t)(file->data);

      /* Check if the file is open. If it's closed, error must be returned
       *  as reopen doesn't have the file path input */
      /* Check if file is closed (no valid data in object */
      if(file_data == NULL)
        {
          PDF_DEBUG_BASE("File is closed! Can't re-open");
          ret_code = PDF_ECLOSED;
        }
      else if(file_data->file_descriptor == NULL)
        {
          PDF_DEBUG_BASE("Invalid FD! Can't re-open");
          ret_code = PDF_ECLOSED;
        }
      else if(file_data->host_path == NULL)
        {
          PDF_DEBUG_BASE("Invalid file object. FD not NULL but empty path");
          ret_code = PDF_ERROR;
        }
      else
        {
          const pdf_char_t* mode_str = NULL;
          ret_code = PDF_ERROR;
          /* Get the new mode string for freopen */
          mode_str =__pdf_fsys_disk_get_mode_string(mode);
          PDF_DEBUG_BASE("Re-opening file '%s' with mode '%s'...",
                         file_data->host_path,
                         mode_str);

          if(mode_str != NULL)
            {
              /* re-open the file */
              file_data->file_descriptor = PDF_FREOPEN(file_data->host_path, \
                                                       mode_str,        \
                                                       file_data->file_descriptor);

              /* reset mode */
              file_data->file_mode = mode;

              /* If got a good FD, set output data */
              ret_code = ((file_data->file_descriptor != NULL) ? \
                          PDF_OK :                               \
                          __pdf_fsys_disk_get_status_from_errno(errno));
            }
        }
    }
  return ret_code;
}

/*
 * Private functions
 */

static pdf_status_t
__pdf_fsys_disk_get_host_path (pdf_text_t path,
                               pdf_char_t **host_path,
                               pdf_size_t *host_path_size)
{
#ifdef PDF_HOST_WIN32
  /* For W32, we will always use widechar functions, so Windows' wchar_t
   * implementation should be used (UTF-16LE) */
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;

  if(pdf_text_get_unicode(&data,
                          &size,
                          path,
                          PDF_TEXT_UTF16_LE,
                          PDF_TEXT_UNICODE_WITH_NUL_SUFFIX) == PDF_OK)
    {
      *host_path = data;
      *host_path_size = size;
      return PDF_OK;
    }
  else
    return PDF_ERROR;
#else
  /* Call the pdf_text module to get a host-encoded version of the
   *  given path */
  pdf_char_t *padded = NULL;
  pdf_size_t padded_size = 0;

  if(pdf_text_get_host(&padded,
                       &padded_size,
                       path,
                       pdf_text_get_host_encoding ()) == PDF_OK)
    {
      *host_path = pdf_realloc(padded, padded_size+2);
      if(*host_path != NULL)
        {
          *host_path_size = padded_size + 1;
          (*host_path)[(*host_path_size)-1] = '\0'; 
          return PDF_OK;
        }
      else
        return PDF_ENOMEM;
    }
  else
    return PDF_ERROR;
#endif
}


static pdf_status_t
__pdf_fsys_disk_set_host_path (const pdf_char_t *host_path,
                               const pdf_size_t  host_path_size,
                               pdf_text_t *p_path)
{
  if((host_path == NULL) || \
     (host_path_size == 0) || \
     (p_path == NULL))
    {
      return PDF_EBADDATA;
    }

#ifdef PDF_HOST_WIN32
  /* For W32, we will always use widechar functions, so Windows' wchar_t
   * implementation should be used (UTF-16LE) */
  return pdf_text_new_from_unicode(host_path,
                                   host_path_size,
                                   PDF_TEXT_UTF16_LE,
                                   p_path);
#else
  /* Call the pdf_text module to get a host-encoded version of the
   *  given path */
  return pdf_text_new_from_host(host_path,
                                host_path_size,
                                pdf_text_get_host_encoding (),
                                p_path);
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
__pdf_fsys_disk_get_mode_string(const enum pdf_fsys_file_mode_e mode)
{
  return (((mode >= PDF_FSYS_OPEN_MODE_FIRST) &&                        \
           (mode <= PDF_FSYS_OPEN_MODE_LAST)) ?                         \
          __pdf_fsys_open_mode_strings[mode] :                          \
          NULL);
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
__pdf_fsys_disk_win32_device_p (pdf_text_t path)
{
  /* The following special "files", which access devices, exist in all
     directories, case-insensitively, and with all possible endings
     after a period or colon, excpets in pathnames that start with
     \\?\: */

  int i;
  pdf_bool_t device_p = PDF_FALSE;
  pdf_text_t device_name;
  pdf_status_t ret_code;

  device_p = PDF_FALSE;
  i = 0;
  while((! device_p) && \
        (i < PDF_MAX_W32_DEVICE_NAMES))
    {
      pdf_text_new_from_unicode (device_names[i],
                                 strlen (device_names[i]),
                                 PDF_TEXT_UTF8,
                                 &device_name);
      
      if (pdf_text_cmp (path, device_name, PDF_FALSE, &ret_code) == 0)
        {
          device_p = PDF_TRUE;
        }
      else
        {
          i++;
        }
      pdf_text_destroy (device_name);
    }

  return device_p;
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

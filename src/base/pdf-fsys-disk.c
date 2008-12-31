/* -*- mode: C -*- Time-stamp: "2008-12-31 19:26:43 aleksander"
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
#include <sys/stat.h>

#include <dirent.h>
#include <unistd.h>

#include <pdf-types.h>
#include <pdf-error.h>
#include <pdf-fsys-disk.h>

#ifndef PDF_HOST_WIN32
 #ifndef PDF_HOST_BSD
  #include <sys/statfs.h>
 #else
  #include <sys/param.h>
  #include <sys/mount.h>
 #endif
#else
#include <windows.h>
#endif /* !PDF_HOST_WIN32 */



/* Directory separator definition */
#ifdef PDF_HOST_WIN32
#define PDF_DIR_SEPARATOR '\\'
#else
#define PDF_DIR_SEPARATOR '/'
#endif



/* Maximum length of the Mode String */
#define PDF_MAX_MODE_STR_LENGTH 4

/* Private function declarations */

static pdf_status_t
__pdf_fsys_disk_get_status_from_errno(int _errno);
static pdf_status_t
__pdf_fsys_disk_get_host_path (const pdf_text_t path,
                               pdf_char_t **host_path,
                               pdf_u32_t *host_path_size);
static pdf_status_t
__pdf_fsys_disk_is_writable_from_host_path(const pdf_char_t *host_path,
                                           pdf_bool_t *p_result);
static pdf_status_t
__pdf_fsys_disk_is_readable_from_host_path(const pdf_char_t *host_path,
                                           pdf_bool_t *p_result);
static pdf_status_t
__pdf_fsys_disk_file_get_size_from_host_path(const pdf_char_t *host_path,
                                             pdf_size_t *p_result);

/* static pdf_status_t */
/* __pdf_fsys_disk_build_mode_string (const enum pdf_fsys_file_mode_e mode, */
/*                                    pdf_char_t mode_str[PDF_MAX_MODE_STR_LENGTH]); */
static pdf_bool_t
__pdf_fsys_disk_win32_device_p (pdf_text_t path);

static const pdf_char_t *
__pdf_fsys_disk_get_mode_string(const enum pdf_fsys_file_mode_e mode);

/*
 * Filesystem Interface Implementation
 */


#if defined PDF_HOST_WIN32

pdf_i64_t
pdf_fsys_disk_get_free_space (pdf_text_t path_name)
{
  pdf_char_t *host_path = NULL;
  pdf_u32_t host_path_size = 0;
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
pdf_fsys_disk_get_free_space (pdf_text_t path_name)
{
  pdf_i64_t result = pdf_i64_new((32 << 1),1); /* (-1) */

  /* Safety check that path_name is a valid pointer */
  if(path_name != NULL)
    {
      struct statfs fs_stats;
      pdf_char_t *host_path = NULL;
      pdf_u32_t host_path_size = 0;


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
                  pdf_i64_assign_quick(&result, -1, NULL); /* (-1) */
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
                                                &file_data->host_path,
                                                &file_data->host_path_size) != PDF_OK)
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
#define PDF_FOPEN(f,m) _wfopen((wchar_t *)f,(wchar_t *m)  
#else
#define PDF_FOPEN(f,m) fopen((char *)f,(char *)m)
#endif


pdf_status_t
pdf_fsys_disk_file_open(const pdf_text_t path_name,
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
          PDF_DEBUG_BASE("Opening file '%s' with mode '%s'... (file_data: %p)",
                         file_data->host_path,
                         (((mode_str != NULL) ? (char *)mode_str : "<null>")),
                         file_data);

          /* Only continue if previous operations went ok */
          if((file_data != NULL) &&             \
             (mode_str != NULL))
            {
              /* Open the file */
              file_data->file_descriptor = PDF_FOPEN (file_data->host_path,
                                                      mode_str);
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
                    PDF_DEBUG_BASE("fopen '%s' failed...", file_data->host_path);
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
#define PDF_MKDIR(f,m) _wmkdir((wchar_t *)f)
#else
#define PDF_MKDIR(f,m) mkdir((char *)f,m)
#endif

pdf_status_t
pdf_fsys_disk_create_folder (const pdf_text_t path_name)
{
  pdf_status_t ret_code = PDF_EBADDATA;
  if(path_name != NULL)
    {
      pdf_char_t *host_path;
      pdf_u32_t host_path_size;
      //      mode_t dir_mode = 0;

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
          ret_code = PDF_OK;
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



pdf_status_t
pdf_fsys_disk_get_folder_contents (const pdf_text_t path_name,
                                   pdf_list_t item_list)
{
  pdf_status_t ret_code = PDF_EBADDATA;
  if(path_name != NULL)
    {
      DIR *dir_stream = NULL;
      struct dirent *dir_entry = NULL;
      pdf_char_t *host_path = NULL;
      pdf_u32_t host_path_size;
      pdf_text_t entry_text = NULL;
      pdf_u32_t name_length;
  

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
          dir_stream = opendir ((const char*) host_path);
          if (dir_stream == NULL)
            {
              PDF_DEBUG_BASE("opendir failed with errno: '%d'",errno);
              switch (errno)
                {
                  /* File name syntax errors */
                case ENAMETOOLONG:
                case ENOENT:
                case ENOTDIR:
#ifndef PDF_HOST_WIN32
                case ELOOP:
#endif /* !PDF_HOST_WIN32 */
                  {
                    ret_code = PDF_EBADNAME;
                  }
                  /* Other specific errors */
                case EACCES:
                  {
                    ret_code = PDF_EBADPERMS;
                  }
                case EMFILE:
                case ENFILE:
                case ENOMEM:
                  /* Any other error */
                default:
                  {
                    ret_code = PDF_ERROR;
                  }
                }
            }

          /* Scan directory contents */
          while ((dir_stream != NULL) &&                        \
                 (dir_entry = readdir (dir_stream)) != NULL)
            {
              /* Note that dir_entry is statically allocated and can be
                 rewritten by a subsequent call. Also, there is not need to
                 free that structure */
              
              /* Get the length of the entry name */
#if defined PDF_HOST_WIN32
              /* In mingw dir_entry->d_namlen is an array of FILENAME_MAX
                 octects long. The dir_entry->d_namlen contain the length of
                 the name stored in d_name */
              name_length = dir_entry->d_namlen;
#else
              /* In POSIX systems dir_entry->d_name is a NULL-terminated
                 string */
              name_length = strlen (dir_entry->d_name);
#endif /* PDF_HOST_WIN32 */
              
              /* Create the text object containing the entry name */
              if ((pdf_text_new_from_host ((pdf_char_t *) dir_entry->d_name,
                                           name_length,
                                           pdf_text_get_host_encoding(),
                                           &entry_text) != PDF_OK) ||
                  (pdf_list_add_last (item_list, 
                                      (void *) entry_text, NULL) != PDF_OK))
                {
                  /* Cleanup and report an error */
                  closedir (dir_stream);
                  ret_code = PDF_ERROR;
                }
            }
        }
    }
  return ret_code;
}


pdf_status_t
pdf_fsys_disk_get_parent (const pdf_text_t path_name,
                          pdf_text_t parent_path)
{
  pdf_status_t ret_code = PDF_EBADDATA;
  if((path_name != NULL) &&                     \
     (parent_path != NULL))
    {

      /* FIXME: Implement me :D */

    }

  return ret_code;
}


/* Host-dependent rmdir() */
#ifdef PDF_HOST_WIN32
#define PDF_RMDIR(f) _wrmdir((wchar_t *)f)
#else
#define PDF_RMDIR(f) rmdir((char *)f)
#endif

pdf_status_t
pdf_fsys_disk_remove_folder (const pdf_text_t path_name)
{
  pdf_status_t ret_code = PDF_EBADDATA;
  if(path_name != NULL)
    {
      pdf_char_t *host_path;
      pdf_u32_t host_path_size;

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
#define PDF_ACCESS(f,m) access((char *)f,m)
#endif

/* Host-dependent stat() */
#ifdef PDF_HOST_WIN32
#define PDF_STAT(f,s) _wstat((wchar_t *)f,s)
#else
#define PDF_STAT(f,s) stat((char *)f,s)
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


static pdf_status_t
__pdf_fsys_disk_file_get_size_from_host_path(const pdf_char_t *host_path,
                                             pdf_size_t *p_result)
{
  pdf_status_t ret_code = PDF_EBADDATA;
  /* Check if file can be written */
  if((host_path != NULL)  && \
     (p_result != NULL))
    {
      struct stat file_info;
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
pdf_fsys_disk_get_item_props (pdf_text_t path_name,
                              struct pdf_fsys_item_props_s *item_props)
{
  pdf_char_t* host_path = NULL;
  pdf_u32_t host_path_len = 0;
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
pdf_fsys_disk_item_p (pdf_text_t path_name)
{
  struct pdf_fsys_item_props_s item_props;

  if (__pdf_fsys_disk_win32_device_p(path_name) == PDF_TRUE)
    {
      return PDF_TRUE;
    }

  return ((pdf_fsys_disk_get_item_props(path_name,                  \
                                        &item_props) == PDF_OK) ?   \
          PDF_TRUE :                                                \
          PDF_FALSE);
}


pdf_bool_t 
pdf_fsys_disk_item_readable_p (pdf_text_t path_name)
{
  pdf_bool_t result = PDF_FALSE;
  if(path_name != NULL)
    {
      pdf_char_t* host_path = NULL;
      pdf_u32_t host_path_len = 0;

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
pdf_fsys_disk_item_writable_p (pdf_text_t path_name)
{
  pdf_bool_t result = PDF_FALSE;
  if(path_name != NULL)
    {
      pdf_char_t* host_path = NULL;
      pdf_u32_t host_path_len = 0;

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
  return (((file != NULL) && (file->data != NULL)) ?        \
          (((pdf_fsys_disk_file_t)file->data)->file_mode) : \
          PDF_FSYS_OPEN_MODE_INVALID);
}

pdf_text_t 
pdf_fsys_disk_file_get_url (pdf_fsys_file_t file)
{
  /* FIXME: Please implement me :D */
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

  /* Unix-like systems have case sensitive paths, Windows doesn't. */

  /* TODO: Mac OS X should have a method in CoreFoundation libs to
   *  check if a the HFS+ filesystem is case-sensitive or not */

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
      return ((fgetpos(((pdf_fsys_disk_file_t)file->data)->file_descriptor, \
                       (fpos_t*)pos) == 0) ?                            \
              PDF_OK :                                                  \
              __pdf_fsys_disk_get_status_from_errno(errno));
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
      return ((fsetpos(((pdf_fsys_disk_file_t)file->data)->file_descriptor, \
                       (fpos_t*)&new_pos) == 0) ?                       \
              PDF_OK :                                                  \
              __pdf_fsys_disk_get_status_from_errno(errno));
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
  /* FIXME: Please implement me :) */
  return 0;
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
  pdf_size_t read_bytes = 0;
  if((file != NULL) &&                          \
     (data != NULL) &&                          \
     (elem_size > 0) &&                         \
     (elem_count > 0))
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
          read_bytes = fread(data,
                             (size_t)elem_size,
                             (size_t)elem_count,
                             file_data->file_descriptor);
        }
    }
  else
    {
      PDF_DEBUG_BASE("Invalid inputs to file_read! (file:%p, data:%p, size:%u, count: %u)",
                     file,data,elem_size,elem_count);
    }
  return read_bytes;
}

pdf_size_t
pdf_fsys_disk_file_write (pdf_fsys_file_t file,
                          const pdf_size_t elem_size,
                          const pdf_size_t elem_count,
                          void *data)
{
  pdf_size_t written_bytes = 0;
  if((file != NULL) && \
     (data != NULL) && \
     (elem_size > 0) && \
     (elem_count > 0))
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
          written_bytes = fwrite(data,
                                 (size_t)elem_size,
                                 (size_t)elem_count,
                                 file_data->file_descriptor);
        }
    }
  else
    {
      PDF_DEBUG_BASE("Invalid inputs to file_read! (file:%p, data:%p, size:%u, count: %u)",
                     file,data,elem_size,elem_count);
    }
  return written_bytes;
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
#define PDF_FREOPEN(f,m,s) _wfreopen((wchar_t *)f,(wchar_t *m),s)
#else
#define PDF_FREOPEN(f,m,s) freopen((char *)f,(char *)m,s)
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
                               pdf_u32_t *host_path_size)
{
  /* Call the pdf_text module to get a host-encoded version of the
   *  given path */
  pdf_char_t *padded = NULL;
  pdf_u32_t padded_size = 0;

  if(pdf_text_get_host(&padded,
                       &padded_size,
                       path,
                       pdf_text_get_host_encoding ()) == PDF_OK)
    {
      *host_path = pdf_realloc(padded, padded_size+2);
      if(*host_path != NULL)
        {
          *host_path_size = padded_size + 2;
          (*host_path)[(*host_path_size)-1] = '\0'; 
          (*host_path)[(*host_path_size)-2] = '\0'; 
          return PDF_OK;
        }
      else
        return PDF_ENOMEM;
    }
  else
    return PDF_ERROR;
}





#ifndef PDF_HOST_WIN32
/* Posix-based open mode */
static const pdf_char_t *__pdf_fsys_open_mode_strings[PDF_FSYS_OPEN_MODE_MAX] = {
  /* PDF_FSYS_OPEN_MODE_INVALID  */ (pdf_char_t *)"invalid",
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
   */
static const pdf_char_t *__pdf_fsys_open_mode_strings[PDF_FSYS_OPEN_MODE_MAX] = {
  /* PDF_FSYS_OPEN_MODE_INVALID  */ (pdf_char_t *)"invalid",
  /* PDF_FSYS_OPEN_MODE_READ     */ (pdf_char_t *)"rb",
  /* PDF_FSYS_OPEN_MODE_WRITE    */ (pdf_char_t *)"wb",
  /* PDF_FSYS_OPEN_MODE_RW       */ (pdf_char_t *)"rb+",
};
#endif


static const pdf_char_t *
__pdf_fsys_disk_get_mode_string(const enum pdf_fsys_file_mode_e mode)
{
  extern const pdf_char_t *__pdf_fsys_open_mode_strings[PDF_FSYS_OPEN_MODE_MAX];
  return (((mode >= PDF_FSYS_OPEN_MODE_FIRST) &&                        \
           (mode <= PDF_FSYS_OPEN_MODE_LAST)) ?                         \
          __pdf_fsys_open_mode_strings[mode] :                          \
          NULL);
}



static pdf_bool_t
__pdf_fsys_disk_win32_device_p (pdf_text_t path)
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
      pdf_text_new_from_unicode ((pdf_char_t *) device_names[i],
                                 strlen (device_names[i]),
                                 PDF_TEXT_UTF8,
                                 &device_name);
      
      if (pdf_text_cmp (path, device_name, PDF_FALSE, &ret_code) == 0)
        {
          device_p = PDF_TRUE;
          break;
        }

      pdf_text_destroy (device_name);
    }

  return device_p;
}



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

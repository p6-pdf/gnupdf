/* -*- mode: C -*-
 *
 *       File:         pdf-fsys-disk.c
 *       Date:         Thu May 22 18:27:35 2008
 *
 *       GNU PDF Library - Disk Filesystem Implementation
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

#if FILE_SYSTEM_BACKSLASH_IS_FILE_NAME_SEPARATOR
#   define DIR_SEPARATOR_C '\\'
#   define DIR_SEPARATOR_S "\\"
#   define IS_DIR_SEPARATOR(c) (((c) == '/') || ((c) == '\\'))
#else
#   define DIR_SEPARATOR_C '/'
#   define DIR_SEPARATOR_S "/"
#   define IS_DIR_SEPARATOR(c) ((c) == '/')
#endif /* FILE_SYSTEM_BACKSLASH_IS_FILE_NAME_SEPARATOR */

/* Filesystem-specific implementation */
struct pdf_fsys_disk_s
{
  /* The common parent struct */
  struct pdf_fsys_s common;

  /* Precomputed filename separator */
  pdf_text_t *filename_separator;
};

/* Filesystem-specific implementation of open files */
struct pdf_fsys_disk_file_s
{
  /* The common parent struct */
  struct pdf_fsys_file_s common;

  /* Host-encoded path */
  pdf_char_t *host_path;
  pdf_size_t host_path_length;

  /* The descriptor of the open file */
  FILE *file_descriptor;
};

/* Private function declarations */

static pdf_status_t get_status_from_errno (int _errno);

static pdf_char_t *get_host_path (const pdf_text_t  *path,
                                  pdf_size_t        *host_path_size,
                                  pdf_error_t      **error);

static pdf_text_t *set_host_path (const pdf_char_t  *host_path,
                                  const pdf_size_t   host_path_size,
                                  pdf_error_t      **error);

static const pdf_char_t *get_mode_string (const enum pdf_fsys_file_mode_e mode);

#ifdef PDF_HOST_WIN32
static pdf_bool_t win32_device_p (const pdf_text_t  *path,
                                  pdf_error_t      **error);
#endif

/*
 * Filesystem Interface Implementation
 */

/* Private function to de-initialize and de-allocate base file data */
static void
deinit_base_file_data (struct pdf_fsys_disk_file_s *file)
{
  pdf_fsys_file_deinit_common (&(file->common));

  if (file->host_path)
    pdf_dealloc (file->host_path);
}

/* Private function to allocate and initialize base file data */
static pdf_bool_t
init_base_file_data (struct pdf_fsys_disk_file_s  *file,
                     const struct pdf_fsys_s      *fsys,
                     const pdf_text_t             *path,
                     enum pdf_fsys_file_mode_e     mode,
                     pdf_error_t                 **error)
{
  memset (file, 0, sizeof (struct pdf_fsys_disk_file_s));

  /* Initialize common data */
  if (!pdf_fsys_file_init_common (&(file->common),
                                  fsys,
                                  path,
                                  mode,
                                  error))
    {
      deinit_base_file_data (file);
      return PDF_FALSE;
    }

  /* Get the host encoded path */
  if (file->common.unicode_path)
    {
      file->host_path = get_host_path (file->common.unicode_path,
                                       &(file->host_path_length),
                                       error);
      if (!file->host_path)
        {
          deinit_base_file_data (file);
          return PDF_FALSE;
        }
    }

  return PDF_TRUE;
}

static pdf_bool_t
is_absolute_path (const pdf_text_t  *path,
                  pdf_error_t      **error)
{
  pdf_char_t *utf8;
  pdf_size_t utf8_size;

  utf8 = pdf_text_get_unicode (path,
                               PDF_TEXT_UTF8,
                               PDF_TEXT_UNICODE_WITH_NUL_SUFFIX,
                               &utf8_size,
                               error);
  if (!utf8)
    return PDF_FALSE;

  if (utf8_size == 0)
    {
      pdf_dealloc (utf8);
      return PDF_FALSE;
    }

  if (IS_DIR_SEPARATOR (utf8[0]))
    {
      pdf_dealloc (utf8);
      return PDF_TRUE;
    }

#ifdef PDF_HOST_WIN32
  /* Recognize drive letter in native windows */
  if (utf8_size >= 3 &&
      ((utf8[0] >= 'a' && utf8[0] <= 'z') ||
       (utf8[0] >= 'A' && utf8[0] <= 'Z')) &&
      utf8[1] == ':' &&
      IS_DIR_SEPARATOR (utf8[2]))
    {
      pdf_dealloc (utf8);
      return PDF_TRUE;
    }
#endif /* PDF_HOST_WIN32 */

  pdf_dealloc (utf8);
  return PDF_FALSE;
}

static pdf_text_t *
get_current_path (pdf_error_t **error)
{
#ifdef PDF_HOST_WIN32
  wchar_t dummy[2];
  wchar_t *current;
  pdf_size_t current_len;
  pdf_text_t *current_text;

  /* Get number of wchar_t items that may get written when getting current
   * directory (includes last NIL wchar_t).
   * Note len here is given in wchar_ts not in bytes */
  current_len = GetCurrentDirectoryW (2, dummy);

  /* Allocate buffer to store the current dir */
  current = pdf_alloc (sizeof (wchar_t) * current_len);
  if (!current)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     PDF_ENOMEM,
                     "cannot get current path: "
                     "couldn't allocate %lu bytes",
                     sizeof (wchar_t) * current_len);
      return NULL;
    }

  /* Fill the buffer with the current directory */
  if (GetCurrentDirectoryW (current, current_len) != (current_len - 1))
    {
      pdf_dealloc (current);
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     PDF_ENOMEM,
                     "cannot get current path: "
                     "couldn't query current directory");
      return NULL;
    }

  current_text = set_host_path ((const pdf_char_t *)current,
                                (current_len - 1) * sizeof (wchar_t),
                                error);

  pdf_dealloc (current);
  return current_text;

#else
  pdf_char_t *current;
  pdf_text_t *current_text;

  /* Rely on gnulib's getcwd module for portability issues */
  current = getcwd (NULL, 0);
  if (!current)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     PDF_ENOMEM,
                     "cannot build absolute path: "
                     "couldn't get current directory");
      return NULL;
    }

  current_text = set_host_path (current,
                                strlen (current),
                                error);

  pdf_dealloc (current);
  return current_text;

#endif /* !PDF_HOST_WIN32 */
}

static pdf_char_t *
skip_root_utf8 (const pdf_char_t *path_utf8)
{
  pdf_char_t *p = (pdf_char_t *)path_utf8;

#ifdef PDF_HOST_WIN32
  /* On windows, skip leading X:\ */
  if (((p[0] >= 'a' && p[0] <= 'z') ||
       (p[0] >= 'A' && p[0] <= 'Z')) &&
      p[1] == ':' &&
      IS_DIR_SEPARATOR (p[2]))
    p = &p[3];
#else
  /* Skip initial slash */
  if (IS_DIR_SEPARATOR (p[0]))
    p++;
#endif /* PDF_HOST_WIN32 */

  return p;
}

/* Returns UTF-8 encoded path */
static pdf_char_t *
ensure_absolute_path (const pdf_fsys_t  *fsys,
                      const pdf_text_t  *path,
                      pdf_error_t      **error)
{
  pdf_error_t *inner_error = NULL;
  pdf_char_t *path_utf8;
  pdf_size_t path_utf8_size;

  if (!is_absolute_path (path, &inner_error))
    {
      pdf_text_t *current;
      pdf_text_t *absolute;

      if (inner_error)
        {
          pdf_propagate_error (error, inner_error);
          return NULL;
        }

      /* Get current path */
      current = get_current_path (error);
      if (!current)
        return NULL;

      /* Build path with current absolute plus relative */
      absolute = pdf_fsys_build_path (fsys,
                                      error,
                                      current,
                                      path,
                                      NULL);
      pdf_text_destroy (current);

      if (!absolute)
        return NULL;

      /* Get UTF-8 of the resulting absolute path */
      path_utf8 = pdf_text_get_unicode (absolute,
                                        PDF_TEXT_UTF8,
                                        PDF_TEXT_UNICODE_WITH_NUL_SUFFIX,
                                        &path_utf8_size,
                                        error);
      pdf_text_destroy (absolute);
    }
  else
    {
      /* Get UTF-8 of the original absolute path */
      path_utf8 = pdf_text_get_unicode (path,
                                        PDF_TEXT_UTF8,
                                        PDF_TEXT_UNICODE_WITH_NUL_SUFFIX,
                                        &path_utf8_size,
                                        error);
    }

  return path_utf8;
}

/* Based on GLib's canonicalize_filename(), in gio/glocalfile.c
 *  Copyright (C) 2006-2007 Red Hat, Inc.
 *  Author: Alexander Larsson <alexl@redhat.com>
 *
 * Returns either UTF-8 encoded path or pdf_text_t, or both
 */
static pdf_bool_t
canonicalize_path (const pdf_fsys_t  *fsys,
                   const pdf_text_t  *path,
                   pdf_text_t       **canonicalized_path,
                   pdf_char_t       **canonicalized_path_utf8,
                   pdf_error_t      **error)
{
  pdf_char_t *canon;
  pdf_char_t *start;
  pdf_char_t *p;
  pdf_char_t *q;
  int i;

  /* Get UTF-8 encoded path */
  canon = ensure_absolute_path (fsys, path, error);
  if (!canon)
    return PDF_FALSE;

  /* Skip root */
  start = skip_root_utf8 (canon);

  /* POSIX allows double slashes at the start to
   * mean something special (as does windows too).
   * So, "//" != "/", but more than two slashes
   * is treated as "/".
   */
  i = 0;
  for (p = start - 1;
       (p >= canon) &&
         IS_DIR_SEPARATOR (*p);
       p--)
    i++;
  if (i > 2)
    {
      i -= 1;
      start -= i;
      memmove (start, start + i, strlen (start + i) + 1);
    }

  p = start;
  while (*p != 0)
    {
      if (p[0] == '.' && (p[1] == 0 || IS_DIR_SEPARATOR (p[1])))
        {
          memmove (p, p + 1, strlen (p + 1) + 1);
        }
      else if (p[0] == '.' && p[1] == '.' && (p[2] == 0 || IS_DIR_SEPARATOR (p[2])))
        {
          q = p + 2;
          /* Skip previous separator */
          p = p - 2;
          if (p < start)
            p = start;
          while (p > start && !IS_DIR_SEPARATOR (*p))
            p--;
          if (IS_DIR_SEPARATOR (*p))
            *p++ = DIR_SEPARATOR_C;
          memmove (p, q, strlen (q)+1);
        }
      else
        {
          /* Skip until next separator */
          while (*p != 0 && !IS_DIR_SEPARATOR (*p))
            p++;

          if (*p != 0)
            {
              /* Canonicalize one separator */
              *p++ = DIR_SEPARATOR_C;
            }
        }

      /* Remove additional separators */
      q = p;
      while (*q && IS_DIR_SEPARATOR (*q))
        q++;

      if (p != q)
        memmove (p, q, strlen (q)+1);
    }

  /* Remove trailing slashes */
  if (p > start && IS_DIR_SEPARATOR (*(p-1)))
    *(p-1) = 0;


  /* If length of the canonicalized path is zero, report error */
  if (canon [0] == '\0')
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     PDF_ERROR,
                     "empty canonicalized path built");
      pdf_dealloc (canon);
      return PDF_FALSE;
    }

  /* Set output(s) */

  if (canonicalized_path)
    *canonicalized_path = pdf_text_new_from_unicode (canon,
                                                     strlen (canon),
                                                     PDF_TEXT_UTF8,
                                                     error);

  if (canonicalized_path_utf8)
    *canonicalized_path_utf8 = canon;
  else
    pdf_dealloc (canon);

  return PDF_TRUE;
}

/* Host-dependent fopen() */
#ifdef PDF_HOST_WIN32
#define PDF_FOPEN(f,m) _wfopen((wchar_t *)f,(wchar_t *)m)
#else
#define PDF_FOPEN(f,m) fopen_safer(f,m)
#endif

static pdf_fsys_file_t *
file_open (const pdf_fsys_t           *fsys,
           const pdf_text_t           *path_name,
           enum pdf_fsys_file_mode_e   mode,
           pdf_error_t               **error)
{
  struct pdf_fsys_disk_file_s *file;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, NULL);
  PDF_ASSERT_RETURN_VAL (mode >= PDF_FSYS_OPEN_MODE_FIRST, NULL);
  PDF_ASSERT_RETURN_VAL (mode <= PDF_FSYS_OPEN_MODE_LAST, NULL);

  /* Allocate private data storage for the file */
  file = (struct pdf_fsys_disk_file_s *) pdf_alloc (sizeof (struct pdf_fsys_disk_file_s));
  if (!file)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     PDF_ENOMEM,
                     "cannot create disk file object: "
                     "couldn't allocate %lu bytes",
                     (unsigned long) sizeof (struct pdf_fsys_disk_file_s));
      return NULL;
    }

  /* Init base data */
  if (!init_base_file_data (file,
                            fsys,
                            path_name,
                            mode,
                            error))
    {
      deinit_base_file_data (file);
      pdf_dealloc (file);
      return NULL;
    }

  /* Open the file */
  file->file_descriptor = PDF_FOPEN (file->host_path,
                                     get_mode_string (mode));
  if (!file->file_descriptor)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     get_status_from_errno (errno),
                     "cannot open file: '%s'",
                     strerror (errno));
      deinit_base_file_data (file);
      pdf_dealloc (file);
      return NULL;
    }

  return (struct pdf_fsys_file_s *)file;
}

static pdf_fsys_file_t *
file_open_tmp (const pdf_fsys_t  *fsys,
               pdf_error_t      **error)
{
  struct pdf_fsys_disk_file_s *file;

  /* Allocate private data storage for the file */
  file = (struct pdf_fsys_disk_file_s *) pdf_alloc (sizeof (struct pdf_fsys_disk_file_s));
  if (!file)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     PDF_ENOMEM,
                     "cannot create disk file object: "
                     "couldn't allocate %lu bytes",
                     (unsigned long) sizeof (struct pdf_fsys_disk_file_s));
      return NULL;
    }

  /* Init base data */
  if (!init_base_file_data (file,
                            fsys,
                            NULL,
                            PDF_FSYS_OPEN_MODE_RW,
                            error))
    {
      deinit_base_file_data (file);
      pdf_dealloc (file);
      return NULL;
    }

  /* Open a temporary file.  */
  file->file_descriptor = tmpfile_safer ();
  if (!file->file_descriptor)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     get_status_from_errno (errno),
                     "cannot open temporary file: '%s'",
                     strerror (errno));
      deinit_base_file_data (file);
      pdf_dealloc (file);
      return NULL;
    }

  return (struct pdf_fsys_file_s *)file;
}

static pdf_bool_t
file_close (pdf_fsys_file_t  *file,
            pdf_error_t     **error)
{
  struct pdf_fsys_disk_file_s *disk_file = (struct pdf_fsys_disk_file_s *)file;
  pdf_bool_t ret = PDF_TRUE;

  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_FALSE);
  PDF_ASSERT_RETURN_VAL (disk_file->file_descriptor > 0, PDF_FALSE);

  /* Host path is optional, not available in tmp files */

  /* Close the I/O stream only if still open */
  if (disk_file->file_descriptor &&
      fclose (disk_file->file_descriptor) == EOF)
    {
      /* Note that even if an error is set, the file is fully closed
       * and the object disposed */
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     get_status_from_errno (errno),
                     "cannot open temporary file: '%s'",
                     strerror (errno));
      ret = PDF_FALSE;
    }

  deinit_base_file_data (disk_file);
  pdf_dealloc (disk_file);
  return ret;
}

/* Host-dependent mkdir() */
#ifdef PDF_HOST_WIN32
#define PDF_MKDIR(f,m) _wmkdir((const wchar_t *)f)
#else
#define PDF_MKDIR(f,m) mkdir(f,m)
#endif

static pdf_bool_t
create_folder (const pdf_fsys_t  *fsys,
               const pdf_text_t  *path_name,
               pdf_error_t      **error)
{
  pdf_char_t *host_path;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, PDF_FALSE);

  /* Get a host-encoded version of the path name */
  host_path = get_host_path (path_name, NULL, error);
  if (!host_path)
    return PDF_FALSE;

  /* Set the permissions of the new directory (posix-only):
   * rwxr_xr_x
   */

  /* Create the directory */
  if (PDF_MKDIR (host_path,                             \
                 (S_IRUSR | S_IWUSR | S_IXUSR |         \
                  S_IRGRP | S_IXGRP |                   \
                  S_IROTH | S_IXOTH)) != 0)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     get_status_from_errno (errno),
                     "cannot create directory: '%s'",
                     strerror (errno));
      pdf_dealloc (host_path);
      return PDF_FALSE;
    }

  pdf_dealloc (host_path);
  return PDF_TRUE;
}

/* Host-dependent opendir(), closedir() and friends */
#ifdef PDF_HOST_WIN32
#define PDF_DIR          _WDIR
#define pdf_dirent_s     _wdirent
#define PDF_OPENDIR(f)   _wopendir ((const wchar_t *)f)
#define PDF_READDIR(ds)  _wreaddir ((PDF_DIR *)ds)
#define PDF_CLOSEDIR(ds) _wclosedir ((PDF_DIR *)ds)
/* In mingw dir_entry->d_namlen is an array of FILENAME_MAX
 * octects long. The dir_entry->d_namlen contain the length of
 * the name stored in d_name */
#define PDF_NAMELEN(de)  (de->d_namlen)
#else
#define PDF_DIR          DIR
#define pdf_dirent_s     dirent
#define PDF_OPENDIR(f)   opendir ((const char *)f)
#define PDF_READDIR(ds)  readdir ((PDF_DIR *)ds)
#define PDF_CLOSEDIR(ds) closedir ((PDF_DIR *)ds)
/* In POSIX systems dir_entry->d_name is a NULL-terminated
 * string */
#define PDF_NAMELEN(de)  (strlen (de->d_name))

#endif /* !PDF_HOST_WIN32 */

static pdf_list_t *
get_folder_contents (const pdf_fsys_t  *fsys,
                     const pdf_text_t  *path_name,
                     pdf_error_t      **error)
{
  PDF_DIR *dir_stream;
  pdf_char_t *host_path;
  struct pdf_dirent_s *dir_entry;
  pdf_list_t *list;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, NULL);

  /* Get a host-encoded version of the path name */
  host_path = get_host_path (path_name, NULL, error);
  if (!host_path)
    return NULL;

  /* Open the directory stream */
  dir_stream = PDF_OPENDIR (host_path);
  if (!dir_stream)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     get_status_from_errno (errno),
                     "cannot open directory: '%s'",
                     strerror (errno));
      pdf_dealloc (host_path);
      return NULL;
    }

  /* Create the list to be returned.
   * Note that we don't expect duplicates when listing files in
   * a single directory, so there's no point in comparing the
   * entry names and disallowing duplicates. */
  list = pdf_list_new (NULL,
                       (pdf_list_element_dispose_fn_t)pdf_text_destroy,
                       PDF_TRUE,
                       error);
  if (!list)
    {
      PDF_CLOSEDIR (dir_stream);
      pdf_dealloc (host_path);
      return NULL;
    }

  /* Scan directory contents */
  while ((dir_entry = PDF_READDIR (dir_stream)) != NULL)
    {
      pdf_text_t *entry_text;
      pdf_u32_t name_length;

      /* Note that dir_entry is statically allocated and can be
       * rewritten by a subsequent call. Also, there is not need to
       * free that structure */

      /* Get the length of the entry name */
      name_length = PDF_NAMELEN (dir_entry);

      /* Create the text object containing the entry name */
      entry_text = set_host_path (dir_entry->d_name,
                                  name_length,
                                  error);
      if (!entry_text)
        {
          PDF_CLOSEDIR (dir_stream);
          pdf_dealloc (host_path);
          if (list)
            pdf_list_destroy (list);
          return NULL;
        }

      if (!pdf_list_add_last (list,
                              entry_text,
                              error))
        {
          PDF_CLOSEDIR (dir_stream);
          PDF_CLOSEDIR (dir_stream);
          pdf_dealloc (host_path);
          return NULL;
        }

      /* Go on to next item */
    }

  PDF_CLOSEDIR (dir_stream);
  return list;
}

static pdf_bool_t
get_directory_and_filename (const pdf_fsys_t  *fsys,
                            const pdf_text_t  *path_name,
                            pdf_text_t       **directory,
                            pdf_text_t       **filename,
                            pdf_char_t       **filename_utf8,
                            pdf_error_t      **error)
{
  pdf_size_t utf8_size;
  pdf_char_t *utf8;
  pdf_char_t *p;
  pdf_size_t filename_size;

  /* Canonicalize path and get output in UTF-8 */
  if (!canonicalize_path (fsys, path_name, NULL, &utf8, error))
    return PDF_FALSE;

  /* Move pointer to last character in the path */
  utf8_size = strlen (utf8);
  p = &utf8[utf8_size - 1];

  /* Skip possible separator at the end of the path */
  if (IS_DIR_SEPARATOR (*p))
    p--;

  /* Find previous directory separator */
  while (p >= utf8 &&
         !IS_DIR_SEPARATOR (*p))
    p--;

  /* Directory separator found? */
  if (p < utf8)
    {
      /* No parent */
      pdf_dealloc (utf8);
      return NULL;
    }

  /* Filename starts always after the dir separator */
  filename_size = strlen (&p[1]);

  /* Get filename if requested */
  if (filename)
    {
      *filename = pdf_text_new_from_unicode (&p[1],
                                             filename_size,
                                             PDF_TEXT_UTF8,
                                             error);
      if (!*filename)
        {
          pdf_dealloc (utf8);
          return PDF_FALSE;
        }
    }

  /* Get filename in UTF-8 if requested */
  if (filename_utf8)
    {
      *filename_utf8 = pdf_alloc (filename_size + 1);
      if (!*filename_utf8)
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_FSYS,
                         PDF_ENOMEM,
                         "cannot get filename: "
                         "couldn't allocate %lu bytes",
                         (unsigned long)(filename_size + 1));
          if (filename)
            pdf_text_destroy (*filename);
          return PDF_FALSE;
        }
      memcpy (*filename_utf8, &p[1], filename_size);
      (*filename_utf8)[filename_size] = '\0';
    }

  if (directory)
    {
      /* Directory last (to be NUL-ed) depends on whether the directory is the root
       * directory (we shouldn't remove the dir separator from the root directory)
       */
      if (p == utf8)
        p[1] = '\0';
      else
        p[0] = '\0';

      *directory = pdf_text_new_from_unicode (utf8,
                                              strlen (utf8),
                                              PDF_TEXT_UTF8,
                                              error);
      if (!*directory)
        {
          if (filename)
            pdf_text_destroy (*filename);
          if (filename_utf8)
            pdf_dealloc (*filename_utf8);
          pdf_dealloc (utf8);
          return PDF_FALSE;
        }
    }

  pdf_dealloc (utf8);
  return PDF_TRUE;
}

static pdf_text_t *
get_basename (const pdf_fsys_t  *fsys,
              const pdf_text_t  *path_name,
              pdf_error_t      **error)
{
  pdf_text_t *basename;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, NULL);

  if (!get_directory_and_filename (fsys,
                                   path_name,
                                   NULL,
                                   &basename,
                                   NULL,
                                   error))
    {
      pdf_prefix_error (error, "couldn't get basename: ");
      return NULL;
    }

  return basename;
}

static pdf_text_t *
get_parent (const pdf_fsys_t  *fsys,
            const pdf_text_t  *path_name,
            pdf_error_t      **error)
{
  pdf_text_t *parent;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, NULL);

  if (!get_directory_and_filename (fsys,
                                   path_name,
                                   &parent,
                                   NULL,
                                   NULL,
                                   error))
    {
      pdf_prefix_error (error, "couldn't get parent: ");
      return NULL;
    }

  return parent;
}

/* Host-dependent rmdir() */
#ifdef PDF_HOST_WIN32
#define PDF_RMDIR(f) _wrmdir((wchar_t *)f)
#else
#define PDF_RMDIR(f) rmdir(f)
#endif

static pdf_bool_t
remove_folder (const pdf_fsys_t  *fsys,
               const pdf_text_t  *path_name,
               pdf_error_t      **error)
{
  pdf_char_t *host_path;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, PDF_FALSE);

  /* Get a host-encoded version of the path name */
  host_path = get_host_path (path_name, NULL, error);
  if (!host_path)
    return PDF_FALSE;

  /* Try to remove the directory */
  if (PDF_RMDIR (host_path) < 0)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     get_status_from_errno (errno),
                     "cannot create directory: '%s'",
                     strerror (errno));
      pdf_dealloc (host_path);
      return PDF_FALSE;
    }

  pdf_dealloc (host_path);
  return PDF_TRUE;
}

/* Host-dependent access() */
#ifdef PDF_HOST_WIN32
#define PDF_ACCESS(f,m) _waccess((wchar_t *)f,m)
#else
#define PDF_ACCESS(f,m) access(f,m)
#endif

static pdf_bool_t
is_readable_from_host_path (const pdf_char_t  *host_path,
                            pdf_error_t      **error)
{
  if (PDF_ACCESS (host_path, R_OK) < 0)
    {
      /* Now, either is not readable, or another error happened */
      if (errno != EACCES)
        pdf_set_error (error,
                       PDF_EDOMAIN_BASE_FSYS,
                       get_status_from_errno (errno),
                       "cannot check if file is readable: '%s'",
                       strerror (errno));
      return PDF_FALSE;
    }
  return PDF_TRUE;
}

static pdf_bool_t
is_writable_from_host_path (const pdf_char_t  *host_path,
                            pdf_error_t      **error)
{
  if (PDF_ACCESS (host_path, W_OK) < 0)
    {
      /* Now, either is not readable, or another error happened */
      if (errno != EACCES)
        pdf_set_error (error,
                       PDF_EDOMAIN_BASE_FSYS,
                       get_status_from_errno (errno),
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
#define PDF_STAT(f,s) stat64(f,s)
typedef struct stat64 pdf_stat_s;
#endif

static pdf_bool_t
get_stat_info_from_host_path (const pdf_char_t  *host_path,
                              pdf_off_t         *size,
                              pdf_time_t        *ctime,
                              pdf_time_t        *mtime,
                              pdf_error_t      **error)
{
  pdf_stat_s file_info;

  if (PDF_STAT (host_path, &file_info) < 0)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     get_status_from_errno (errno),
                     "cannot get stat info: '%s'",
                     strerror (errno));
      return PDF_FALSE;
    }

  if (size)
    {
      *size = (pdf_off_t)file_info.st_size;
    }

  if (ctime)
    {
      pdf_time_init (ctime);
      pdf_time_set_utc (ctime, file_info.st_ctime);
    }

  if (mtime)
    {
      pdf_time_init (mtime);
      pdf_time_set_utc (mtime, file_info.st_mtime);
    }

  return PDF_TRUE;
}

static pdf_bool_t
get_item_props (const pdf_fsys_t              *fsys,
                const pdf_text_t              *path_name,
                struct pdf_fsys_item_props_s  *props,
                pdf_error_t                  **error)
{
  pdf_char_t* host_path;
  pdf_error_t *inner_error = NULL;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (props, PDF_FALSE);

  /* Get a host-encoded version of the path name */
  host_path = get_host_path (path_name, NULL, error);
  if (!host_path)
    return PDF_FALSE;

  /* Initialize the output to all zeros */
  memset (props, 0, sizeof (struct pdf_fsys_item_props_s));

  /* Is readable? */
  props->is_readable = is_readable_from_host_path (host_path, &inner_error);
  if (inner_error)
    {
      pdf_dealloc (host_path);
      pdf_propagate_error (error, inner_error);
      return PDF_FALSE;
    }

  /* Is writable? */
  props->is_writable = is_writable_from_host_path (host_path, &inner_error);
  if (inner_error)
    {
      pdf_dealloc (host_path);
      pdf_propagate_error (error, inner_error);
      return PDF_FALSE;
    }

  /* Get file size, ctime and mtime */
  if (!get_stat_info_from_host_path (host_path,
                                     &props->file_size,
                                     &props->creation_date,
                                     &props->modification_date,
                                     error))
    {
      pdf_dealloc (host_path);
      return PDF_FALSE;
    }

#ifdef PDF_HOST_WIN32
  /* TODO: Check if file hidden in Windows */
  props->is_hidden = PDF_FALSE;
#else
  {
    pdf_char_t *filename_utf8;

    /* Get filename */
    filename_utf8 = NULL;
    if (!get_directory_and_filename (fsys,
                                     path_name,
                                     NULL,
                                     NULL,
                                     &filename_utf8,
                                     error))
      {
        pdf_dealloc (host_path);
        return PDF_FALSE;
      }

    /* Lets consider files starting with dot as hidden */
    props->is_hidden = (filename_utf8[0] == '.' ? PDF_TRUE : PDF_FALSE);
    pdf_dealloc (filename_utf8);
  }
#endif /* PDF_HOST_WIN32 */

  pdf_dealloc (host_path);

  return PDF_TRUE;
}

static pdf_bool_t
item_p (const pdf_fsys_t *fsys,
        const pdf_text_t *path_name)
{
  struct pdf_fsys_item_props_s item_props;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, PDF_FALSE);

#ifdef PDF_HOST_WIN32
  if (win32_device_p (path_name))
    return PDF_TRUE;
#endif

  /* TODO: Do we really need to get item properties just to know if it
   * exists? */
  return get_item_props (fsys,
                         path_name,
                         &item_props,
                         NULL);
}

static pdf_bool_t
item_readable_p (const pdf_fsys_t *fsys,
                 const pdf_text_t *path_name)
{
  pdf_error_t *inner_error = NULL;
  pdf_char_t* host_path;
  pdf_bool_t readable;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, PDF_FALSE);

  /* We get the string in HOST-encoding (with NUL-suffix) */
  host_path = get_host_path (path_name, NULL, NULL);
  if (!host_path)
    return PDF_FALSE;

  readable = is_readable_from_host_path (host_path, &inner_error);
  if (inner_error)
    {
      pdf_dealloc (host_path);
      pdf_error_destroy (inner_error);
      return PDF_FALSE;
    }

  pdf_dealloc (host_path);
  return readable;
}

static pdf_bool_t
item_writable_p (const pdf_fsys_t *fsys,
                 const pdf_text_t *path_name)
{
  pdf_error_t *inner_error = NULL;
  pdf_char_t* host_path;
  pdf_bool_t writable;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, PDF_FALSE);

  /* We get the string in HOST-encoding (with NUL-suffix) */
  host_path = get_host_path (path_name, NULL, NULL);
  if (!host_path)
    return PDF_FALSE;

  writable = is_writable_from_host_path (host_path, &inner_error);
  if (inner_error)
    {
      pdf_dealloc (host_path);
      pdf_error_destroy (inner_error);
      return PDF_FALSE;
    }

  pdf_dealloc (host_path);
  return writable;
}

#ifdef PDF_HOST_WIN32

static pdf_i64_t
get_free_space (const pdf_fsys_t  *fsys,
                const pdf_text_t  *path_name,
                pdf_error_t      **error)
{
  pdf_char_t *utf16le_path;
  pdf_u32_t utf16le_path_size = 0;
  ULARGE_INTEGER free_bytes;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, -1);

  /* Note that as we get the string as UTF-16LE with LAST NUL suffix,
   *  it's equivalent to a wchar_t in windows environments */
  utf16le_path = pdf_text_get_unicode (path_name,
                                       PDF_TEXT_UTF16_LE,
                                       PDF_TEXT_UNICODE_WITH_NUL_SUFFIX,
                                       &utf16le_path_size,
                                       error);
  if (!utf16le_path)
    return -1;

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
      /* Propagate error */
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     __pdf_fsys_disk_get_status_from_errno (errno),
                     "cannot get free disk space: got error code %u",
                     (uint)GetLastError ());
      pdf_dealloc (utf16le_path);
      return -1;
    }

  pdf_dealloc (utf16le_path);

  return ((pdf_i64_t)free_bytes << 32) + free_bytes.LowPart;
}

#else

static pdf_i64_t
get_free_space (const pdf_fsys_t  *fsys,
                const pdf_text_t  *path_name,
                pdf_error_t      **error)
{
  struct statfs fs_stats;
  pdf_char_t *host_path;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, -1);

  /* We get the string in HOST-encoding (with NUL-suffix) */
  host_path = get_host_path (path_name, NULL, error);
  if (!host_path)
    return -1;

  PDF_DEBUG_BASE ("Getting free bytes of FS at path '%s'...",
                  host_path);

  if (statfs (host_path, &fs_stats) < 0)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     get_status_from_errno (errno),
                     "cannot get free disk space: %s",
                     strerror (errno));
      pdf_dealloc (host_path);
      return -1;
    }

  pdf_dealloc (host_path);

  /* Return the free space in octects */
  return (pdf_i64_t)(fs_stats.f_bfree * fs_stats.f_bsize);
}

#endif /* !PDF_HOST_WIN32 */

static pdf_text_t *
build_path (const pdf_fsys_t  *fsys,
            pdf_error_t      **error,
            const pdf_text_t  *first_element,
            ...)
{
  const struct pdf_fsys_disk_s *disk_fsys = (const struct pdf_fsys_disk_s *)fsys;
  pdf_text_t *output;
  pdf_text_t *next;
  va_list args;

  PDF_ASSERT_POINTER_RETURN_VAL (first_element, NULL);

  output = pdf_text_dup (first_element, error);
  if (!output)
    return NULL;

  va_start (args, first_element);
  next = va_arg (args, pdf_text_t *);
  while (next != NULL)
    {
      if (!pdf_text_concat (output,
                            disk_fsys->filename_separator,
                            PDF_TRUE,
                            error) ||
          !pdf_text_concat (output,
                            next,
                            PDF_TRUE,
                            error))
        {
          pdf_text_destroy (output);
          va_end (args);
          return NULL;
        }
      next = va_arg (args, pdf_text_t *);
    }
  va_end (args);

  return output;
}

static pdf_char_t *
get_url_from_path (const pdf_fsys_t  *fsys,
                   const pdf_text_t  *path,
                   pdf_error_t      **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (path, NULL);

  /* TODO. See FS#126 */

  return NULL;
}

/*
 * File Interface Implementation
 */

static pdf_bool_t
file_same_p (const pdf_fsys_file_t  *file,
             const pdf_text_t       *path,
             pdf_error_t           **error)
{
  pdf_error_t *inner_error = NULL;
  pdf_i32_t ret;
  pdf_bool_t case_sensitive;
  pdf_text_t *canonicalized_file_path = NULL;
  pdf_text_t *canonicalized_path = NULL;

  PDF_ASSERT_POINTER_RETURN_VAL (path, PDF_FALSE);

  /* TODO: Mac OS X should have a method in CoreFoundation libs to
   *  check if a the HFS+ filesystem is case-sensitive or not */

  /* Unix-like systems have case sensitive paths, Windows doesn't. */
#ifndef PDF_HOST_WIN32
  case_sensitive = PDF_TRUE;
#else
  case_sensitive = PDF_FALSE;
#endif

  /* Canonicalize paths before comparing */
  if (!canonicalize_path (file->fsys,
                          file->unicode_path,
                          &canonicalized_file_path,
                          NULL,
                          error) ||
      !canonicalize_path (file->fsys,
                          path,
                          &canonicalized_path,
                          NULL,
                          error))
    {
      pdf_prefix_error (error, "couldn't check if same files: ");

      if (canonicalized_file_path)
        pdf_text_destroy (canonicalized_file_path);

      return PDF_FALSE;
    }

  /* Compare canonicalized paths */
  ret = pdf_text_cmp (canonicalized_file_path,
                      canonicalized_path,
                      case_sensitive,
                      &inner_error);

  pdf_text_destroy (canonicalized_path);
  pdf_text_destroy (canonicalized_file_path);

  if (inner_error)
    {
      pdf_propagate_error (error, inner_error);
      pdf_prefix_error (error, "couldn't check if same files: ");
      return PDF_FALSE;
    }

  return (ret == 0 ? PDF_TRUE : PDF_FALSE);
}

static pdf_off_t
file_get_pos (const pdf_fsys_file_t  *file,
              pdf_error_t           **error)
{
  struct pdf_fsys_disk_file_s *disk_file;
  pdf_off_t cpos;

  PDF_ASSERT_POINTER_RETURN_VAL (file, (pdf_off_t)-1);

  disk_file = (struct pdf_fsys_disk_file_s *)file;
  cpos = ftello (disk_file->file_descriptor);
  if (cpos == (pdf_off_t)-1)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     get_status_from_errno (errno),
                     "cannot get position from file: '%s'",
                     strerror (errno));
      return (pdf_off_t)-1;
    }

  return cpos;
}

static pdf_bool_t
file_set_pos (pdf_fsys_file_t  *file,
              pdf_off_t         new_pos,
              pdf_error_t     **error)
{
  struct pdf_fsys_disk_file_s *disk_file;

  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_FALSE);
  PDF_ASSERT_RETURN_VAL (new_pos >= 0, PDF_FALSE);

  disk_file = (struct pdf_fsys_disk_file_s *)file;
  if (fseeko (disk_file->file_descriptor,
              new_pos,
              SEEK_SET) == (pdf_off_t)-1)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     get_status_from_errno (errno),
                     "cannot set position in file: '%s'",
                     strerror (errno));
      return PDF_FALSE;
    }

  return PDF_TRUE;
}

static pdf_bool_t
file_can_set_size_p (const pdf_fsys_file_t *file,
                     pdf_off_t              size)
{
  struct pdf_fsys_disk_file_s *disk_file;

  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_FALSE);

  /* We'll assume that we can set any new size of the file if it is writable */
  disk_file = (struct pdf_fsys_disk_file_s *)file;
  return is_writable_from_host_path (disk_file->host_path, NULL);
}

static pdf_off_t
file_get_size (const pdf_fsys_file_t  *file,
               pdf_error_t           **error)
{
  struct pdf_fsys_disk_file_s *disk_file;
  pdf_off_t size;

  PDF_ASSERT_POINTER_RETURN_VAL (file, (pdf_off_t)-1);

  disk_file = (struct pdf_fsys_disk_file_s *)file;
  return (get_stat_info_from_host_path (disk_file->host_path,
                                        &size,
                                        NULL,
                                        NULL,
                                        error) ?
          size :
          (pdf_off_t)-1);
}

static pdf_bool_t
file_set_size (pdf_fsys_file_t  *file,
               pdf_off_t         size,
               pdf_error_t     **error)
{
  struct pdf_fsys_disk_file_s *disk_file = (struct pdf_fsys_disk_file_s *)file;

  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_FALSE);
  PDF_ASSERT_RETURN_VAL (disk_file->file_descriptor > 0, PDF_FALSE);

  /* Setting size of the file is just seeking to the given size offset and
   * writing an EOF... isn't it? */
  if (!file_set_pos (file, (size > 0 ? size - 1 : 0), error))
    {
      pdf_prefix_error (error, "cannot set size: ");
      return PDF_FALSE;
    }

  /* putc() returns exactly the same character written, or EOF on error,
   * so if we try to write EOF we should indeed get EOF */
  putc (EOF, disk_file->file_descriptor);

  return PDF_TRUE;
}

static pdf_bool_t
file_read (pdf_fsys_file_t  *file,
           pdf_char_t       *buf,
           pdf_size_t        bytes,
           pdf_size_t       *read_bytes,
           pdf_error_t     **error)
{
  struct pdf_fsys_disk_file_s *disk_file = (struct pdf_fsys_disk_file_s *)file;

  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (buf, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (read_bytes, PDF_FALSE);
  PDF_ASSERT_RETURN_VAL (disk_file->file_descriptor > 0, PDF_FALSE);

  *read_bytes = fread (buf,
                       1,
                       bytes,
                       disk_file->file_descriptor);

  /* On EOF, PDF_FALSE without error */
  if (feof (disk_file->file_descriptor))
    return PDF_FALSE;

  if (ferror (disk_file->file_descriptor))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     get_status_from_errno (errno),
                     "cannot read from file: '%s'",
                     strerror (errno));
      return PDF_FALSE;
    }

  return PDF_TRUE;
}

static pdf_bool_t
file_write (pdf_fsys_file_t  *file,
            const pdf_char_t *buf,
            pdf_size_t        bytes,
            pdf_size_t       *written_bytes,
            pdf_error_t     **error)
{
  struct pdf_fsys_disk_file_s *disk_file = (struct pdf_fsys_disk_file_s *)file;

  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (buf, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (written_bytes, PDF_FALSE);
  PDF_ASSERT_RETURN_VAL (disk_file->file_descriptor > 0, PDF_FALSE);

  *written_bytes = fwrite (buf,
                           1,
                           bytes,
                           disk_file->file_descriptor);

  if (ferror (disk_file->file_descriptor))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     get_status_from_errno (errno),
                     "cannot write to file: '%s'",
                     strerror (errno));
      return PDF_FALSE;
    }

  return PDF_TRUE;
}

static pdf_bool_t
file_flush (pdf_fsys_file_t  *file,
            pdf_error_t     **error)
{
  struct pdf_fsys_disk_file_s *disk_file = (struct pdf_fsys_disk_file_s *)file;

  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_FALSE);
  PDF_ASSERT_RETURN_VAL (disk_file->file_descriptor > 0, PDF_FALSE);

  if (fflush (disk_file->file_descriptor) < 0)
    {
      /* On Windows platforms (excluding Cygwin), fflush does not
       * set errno upon failure. */
#ifndef PDF_HOST_WIN32
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     get_status_from_errno (errno),
                     "cannot flush to file: '%s'",
                     strerror (errno));
#else
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     PDF_ERROR,
                     "cannot flush to file: 'unknown error'",
                     strerror (errno));
      PDF_ERROR,
#endif
        return PDF_FALSE;
    }

  return PDF_TRUE;
}

static pdf_bool_t
file_request_ria (pdf_fsys_file_t  *file,
                  pdf_off_t         offset,
                  pdf_size_t        count,
                  pdf_error_t     **error)
{
  pdf_set_error (error,
                 PDF_EDOMAIN_BASE_FSYS,
                 PDF_EBADDATA,
                 "can't request RIA: not provided by the disk filesystem");
  return PDF_FALSE;
}

static pdf_bool_t
file_has_ria (pdf_fsys_file_t  *file,
              pdf_error_t     **error)
{
  /* We can just safely return FALSE here, without error:
   * the file has definitely not an ongoing RIA operation */
  return PDF_FALSE;
}

static pdf_bool_t
file_cancel_ria (pdf_fsys_file_t  *file,
                 pdf_error_t     **error)
{
  /* We can just safely return FALSE here, without error:
   * the file has definitely not an ongoing RIA operation, so
   * canceling RIA always succeeds. */
  return PDF_TRUE;
}


/* Host-dependent freopen() */
#ifdef PDF_HOST_WIN32
#define PDF_FREOPEN(f,m,s) _wfreopen((wchar_t *)f,(wchar_t *)m,s)
#else
#define PDF_FREOPEN(f,m,s) freopen_safer (f,m,s)
#endif

static pdf_bool_t
file_reopen (pdf_fsys_file_t            *file,
             enum pdf_fsys_file_mode_e   mode,
             pdf_error_t               **error)
{
  struct pdf_fsys_disk_file_s *disk_file;
  FILE *reopened;

  disk_file = (struct pdf_fsys_disk_file_s *)file;

  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_FALSE);
  PDF_ASSERT_RETURN_VAL (disk_file->file_descriptor > 0, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (disk_file->host_path, PDF_FALSE);

  /* re-open the file */
  reopened = PDF_FREOPEN (disk_file->host_path,
                          get_mode_string (mode),
                          disk_file->file_descriptor);
  if (!reopened)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     get_status_from_errno (errno),
                     "cannot reopen file: '%s'",
                     strerror (errno));
      return PDF_FALSE;
    }

  disk_file->file_descriptor = reopened;
  file->mode = mode;

  return PDF_TRUE;
}

static pdf_bool_t
file_set_mode (pdf_fsys_file_t            *file,
               enum pdf_fsys_file_mode_e   new_mode,
               pdf_error_t               **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_FALSE);

  /* If modes are different, call reopen */
  return (file->mode != new_mode ?
          file_reopen (file, new_mode, error) :
          PDF_TRUE);
}

/*
 * Private functions
 */

static pdf_char_t *
get_host_path (const pdf_text_t  *path,
               pdf_size_t        *host_path_size,
               pdf_error_t      **error)
{
#ifdef PDF_HOST_WIN32
  /* For W32, we will always use widechar functions, so Windows' wchar_t
   * implementation should be used (UTF-16LE) */
  return pdf_text_get_unicode (path,
                               PDF_TEXT_UTF16_LE,
                               PDF_TEXT_UNICODE_WITH_NUL_SUFFIX,
                               host_path_size,
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
                     PDF_EDOMAIN_BASE_FSYS,
                     PDF_ENOMEM,
                     "cannot get text contents host encoded: "
                     "couldn't reallocate %lu bytes",
                     (unsigned long)length);
      return NULL;
    }
  nul_suffixed[length - 1] = '\0';

  if (host_path_size)
    *host_path_size = length;

  return nul_suffixed;
#endif
}

static pdf_text_t *
set_host_path (const pdf_char_t *host_path,
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
static const pdf_char_t *open_mode_strings[PDF_FSYS_OPEN_MODE_MAX] = {
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
static const pdf_char_t *open_mode_strings[PDF_FSYS_OPEN_MODE_MAX] = {
  /* PDF_FSYS_OPEN_MODE_INVALID  */ (pdf_char_t *)"\x00\x00",
  /* PDF_FSYS_OPEN_MODE_READ     */ (pdf_char_t *)"\x72\x00\x62\x00\x00\x00",
  /* PDF_FSYS_OPEN_MODE_WRITE    */ (pdf_char_t *)"\x77\x00\x62\x00\x00\x00",
  /* PDF_FSYS_OPEN_MODE_RW       */ (pdf_char_t *)"\x72\x00\x62\x00\x2B\x00\x00\x00",
};
#endif

static const pdf_char_t *
get_mode_string (const enum pdf_fsys_file_mode_e mode)
{
  return open_mode_strings[mode];
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
win32_device_p (const pdf_text_t  *path,
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
get_status_from_errno (int _errno)
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

/* Setup the disk filesystem implementation, using named initializers */
static struct pdf_fsys_disk_s pdf_fsys_disk_implementation =
  {
    .common.create_folder_fn       = create_folder,
    .common.get_folder_contents_fn = get_folder_contents,
    .common.get_parent_fn          = get_parent,
    .common.get_basename_fn        = get_basename,
    .common.remove_folder_fn       = remove_folder,
    .common.get_item_props_fn      = get_item_props,
    .common.get_free_space_fn      = get_free_space,
    .common.item_p_fn              = item_p,
    .common.item_readable_p_fn     = item_readable_p,
    .common.item_writable_p_fn     = item_writable_p,
    .common.build_path_fn          = build_path,
    .common.get_url_from_path_fn   = get_url_from_path,

    .common.file_open_fn           = file_open,
    .common.file_open_tmp_fn       = file_open_tmp,
    .common.file_reopen_fn         = file_reopen,
    .common.file_close_fn          = file_close,
    .common.file_read_fn           = file_read,
    .common.file_write_fn          = file_write,
    .common.file_flush_fn          = file_flush,
    .common.file_can_set_size_p_fn = file_can_set_size_p,
    .common.file_get_size_fn       = file_get_size,
    .common.file_set_size_fn       = file_set_size,
    .common.file_get_pos_fn        = file_get_pos,
    .common.file_set_pos_fn        = file_set_pos,
    .common.file_set_mode_fn       = file_set_mode,
    .common.file_same_p_fn         = file_same_p,
    .common.file_request_ria_fn    = file_request_ria,
    .common.file_has_ria_fn        = file_has_ria,
    .common.file_cancel_ria_fn     = file_cancel_ria,
  };

static void
pdf_fsys_disk_deinit_cb (struct pdf_fsys_s *impl)
{
  struct pdf_fsys_disk_s *disk_fsys = (struct pdf_fsys_disk_s *)impl;

  if (disk_fsys->filename_separator)
    {
      pdf_text_destroy (disk_fsys->filename_separator);
      disk_fsys->filename_separator = NULL;
    }
}

pdf_bool_t
pdf_fsys_disk_init (pdf_error_t **error)
{
  /* Disk filesystem implementation specific stuff */
  pdf_fsys_disk_implementation.filename_separator =
    pdf_text_new_from_unicode ((pdf_char_t*)DIR_SEPARATOR_S,
                               1,
                               PDF_TEXT_UTF8,
                               error);
  if (!pdf_fsys_disk_implementation.filename_separator)
    {
      pdf_prefix_error (error, "couldn't initialize disk filesystem: ");
      return PDF_FALSE;
    }

  return pdf_fsys_add (PDF_FSYS_DISK_ID,
                       (struct pdf_fsys_s *)&pdf_fsys_disk_implementation,
                       pdf_fsys_disk_deinit_cb,
                       error);
}

void
pdf_fsys_disk_deinit (void)
{
  pdf_fsys_remove (PDF_FSYS_DISK_ID);
}

/* End of pdf-fsys-disk.c */

/* -*- mode: C -*-
 *
 *       File:         pdf-fsys-http.c
 *       Date:         Thu Dec 17 18:27:35 2009
 *
 *       GNU PDF Library - HTTP Filesystem Implementation
 *
 */

/* Copyright (C) 2009-2011 Free Software Foundation, Inc. */

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

#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <curl/curl.h>

#include <pdf-error.h>
#include <pdf-fsys-http.h>

/* Filesystem-specific implementation */
struct pdf_fsys_http_s
{
  /* The common parent struct */
  struct pdf_fsys_s common;

  CURLM *curlm;
};

/* Filesystem-specific implementation of open files */
struct pdf_fsys_http_file_s
{
  /* The common parent struct */
  struct pdf_fsys_file_s common;

};

/*
 * Filesystem Interface Implementation
 */

/* Private function to de-initialize and de-allocate base file data */
static void
deinit_base_file_data (struct pdf_fsys_http_file_s *file)
{
  pdf_fsys_file_deinit_common (&(file->common));
}

/* Private function to allocate and initialize base file data */
static pdf_bool_t
init_base_file_data (struct pdf_fsys_http_file_s  *file,
                     const struct pdf_fsys_s      *fsys,
                     const pdf_text_t             *path,
                     enum pdf_fsys_file_mode_e     mode,
                     pdf_error_t                 **error)
{
  memset (file, 0, sizeof (struct pdf_fsys_http_file_s));

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

  return PDF_TRUE;
}

static pdf_fsys_file_t *
file_open (const pdf_fsys_t           *fsys,
           const pdf_text_t           *path_name,
           enum pdf_fsys_file_mode_e   mode,
           pdf_error_t               **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (path_name, NULL);

  /* TODO */

  return NULL;
}

static pdf_fsys_file_t *
file_open_tmp (const pdf_fsys_t  *fsys,
               pdf_error_t      **error)
{
  pdf_set_error (error,
                 PDF_EDOMAIN_BASE_FSYS,
                 PDF_EBADDATA,
                 "can't open tmp file: the http filesystem is read-only");
  return NULL;
}

static pdf_bool_t
file_close (pdf_fsys_file_t  *file,
            pdf_error_t     **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_FALSE);
  /* TODO */
  return PDF_FALSE;
}

static pdf_bool_t
create_folder (const pdf_fsys_t  *fsys,
               const pdf_text_t  *path_name,
               pdf_error_t      **error)
{
  pdf_set_error (error,
                 PDF_EDOMAIN_BASE_FSYS,
                 PDF_EBADDATA,
                 "can't create folder: the http filesystem is read-only");
  return PDF_FALSE;
}

static pdf_list_t *
get_folder_contents (const pdf_fsys_t  *fsys,
                     const pdf_text_t  *path_name,
                     pdf_error_t      **error)
{
  /* Sometimes it is possible to get the index of files available under a given
   * directory, when for example no index.html is available in a given remote
   * path. But this is just sometimes, not something supported by HTTP protocol
   * directly, so just return an error here */
  pdf_set_error (error,
                 PDF_EDOMAIN_BASE_FSYS,
                 PDF_EBADDATA,
                 "can't get folder contents: the http protocol doesn't support it");
  return PDF_FALSE;
}

static pdf_text_t *
get_parent (const pdf_fsys_t  *fsys,
            const pdf_text_t  *path_name,
            pdf_error_t      **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (path_name, PDF_FALSE);

  /* TODO */
  return NULL;
}

static pdf_bool_t
remove_folder (const pdf_fsys_t  *fsys,
               const pdf_text_t  *path_name,
               pdf_error_t      **error)
{
  pdf_set_error (error,
                 PDF_EDOMAIN_BASE_FSYS,
                 PDF_EBADDATA,
                 "can't remove folder: the http filesystem is read-only");
  return PDF_FALSE;
}

static pdf_bool_t
get_item_props (const pdf_fsys_t              *fsys,
                const pdf_text_t              *path_name,
                struct pdf_fsys_item_props_s  *props,
                pdf_error_t                  **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (path_name, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (props, PDF_FALSE);

  /* Initialize the output to all zeros */
  memset (props, 0, sizeof (struct pdf_fsys_item_props_s));

  /* TODO */
  return PDF_FALSE;
}

static pdf_bool_t
item_p (const pdf_fsys_t *fsys,
        const pdf_text_t *path_name)
{
  /* TODO */
  return PDF_FALSE;
}

static pdf_bool_t
item_readable_p (const pdf_fsys_t *fsys,
                 const pdf_text_t *path_name)
{
  /* HTTP filesystem is read-only, so if the item exists, it is readable */
  return item_p (fsys, path_name);
}

static pdf_bool_t
item_writable_p (const pdf_fsys_t *fsys,
                 const pdf_text_t *path_name)
{
  /* HTTP filesystem is read-only, so items NEVER writable */
  return PDF_FALSE;
}

static pdf_i64_t
get_free_space (const pdf_fsys_t  *fsys,
                const pdf_text_t  *path_name,
                pdf_error_t      **error)
{
  /* HTTP filesystem is read-only, so NO free space always */
  return 0;
}

static pdf_text_t *
build_path (const pdf_fsys_t  *fsys,
            pdf_error_t      **error,
            const pdf_text_t  *first_element,
            ...)
{
  PDF_ASSERT_POINTER_RETURN_VAL (first_element, NULL);

  /* TODO */
  return NULL;
}

static pdf_char_t *
get_url_from_path (const pdf_fsys_t  *fsys,
                   pdf_char_t        *path,
                   pdf_error_t      **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (path, NULL);

  /* TODO */

  return NULL;
}

static pdf_bool_t
file_same_p (const pdf_fsys_file_t  *file,
             const pdf_text_t       *path,
             pdf_error_t           **error)
{
  /* TODO */
  return PDF_FALSE;
}

static pdf_off_t
file_get_pos (const pdf_fsys_file_t  *file,
              pdf_error_t           **error)
{
  /* TODO */

  return -1;
}

static pdf_bool_t
file_set_pos (pdf_fsys_file_t  *file,
              pdf_off_t         new_pos,
              pdf_error_t     **error)
{
  /* TODO */

  return PDF_FALSE;
}

static pdf_bool_t
file_can_set_size_p (const pdf_fsys_file_t *file,
                     pdf_off_t              size)
{
  /* Just return FALSE, no error */
  return PDF_FALSE;
}

static pdf_off_t
file_get_size (const pdf_fsys_file_t  *file,
               pdf_error_t           **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_FALSE);

  /* TODO */

  return -1;
}

static pdf_bool_t
file_set_size (pdf_fsys_file_t  *file,
               pdf_off_t         size,
               pdf_error_t     **error)
{
  pdf_set_error (error,
                 PDF_EDOMAIN_BASE_FSYS,
                 PDF_EBADDATA,
                 "can't set size: the http filesystem is read-only");
  return PDF_FALSE;
}

static pdf_bool_t
file_read (pdf_fsys_file_t  *file,
           pdf_char_t       *buf,
           pdf_size_t        bytes,
           pdf_size_t       *read_bytes,
           pdf_error_t     **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_FALSE);

  /* TODO */
  return PDF_FALSE;
}

static pdf_bool_t
file_write (pdf_fsys_file_t  *file,
            const pdf_char_t *buf,
            pdf_size_t        bytes,
            pdf_size_t       *written_bytes,
            pdf_error_t     **error)
{
  pdf_set_error (error,
                 PDF_EDOMAIN_BASE_FSYS,
                 PDF_EBADDATA,
                 "can't write: the http filesystem is read-only");
  return PDF_FALSE;
}

static pdf_bool_t
file_flush (pdf_fsys_file_t  *file,
            pdf_error_t     **error)
{
  pdf_set_error (error,
                 PDF_EDOMAIN_BASE_FSYS,
                 PDF_EBADDATA,
                 "can't flush: the http filesystem is read-only");
  return PDF_FALSE;
}

static pdf_bool_t
file_request_ria (pdf_fsys_file_t  *file,
                  pdf_off_t         offset,
                  pdf_size_t        count,
                  pdf_error_t     **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_FALSE);

  /* TODO */
  return PDF_FALSE;
}

static pdf_bool_t
file_has_ria (pdf_fsys_file_t  *file,
              pdf_error_t     **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_FALSE);

  /* TODO */
  return PDF_FALSE;
}

static pdf_bool_t
file_cancel_ria (pdf_fsys_file_t  *file,
                 pdf_error_t     **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_FALSE);

  /* TODO */
  return PDF_FALSE;
}

static pdf_bool_t
file_reopen (pdf_fsys_file_t            *file,
             enum pdf_fsys_file_mode_e   mode,
             pdf_error_t               **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_FALSE);

  /* Allow only open for READ-ing */
  if (mode != PDF_FSYS_OPEN_MODE_READ)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     PDF_EBADDATA,
                     "can't reopen: the http filesystem is read-only");
      return PDF_FALSE;
    }

  /* And we just return TRUE, there is nothing else to be done here */
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

/* Setup the http filesystem implementation, using named initializers */
static struct pdf_fsys_http_s pdf_fsys_http_implementation =
  {
    .common.create_folder_fn       = create_folder,
    .common.get_folder_contents_fn = get_folder_contents,
    .common.get_parent_fn          = get_parent,
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
pdf_fsys_http_deinit_cb (struct pdf_fsys_s *impl)
{
  /* From curl_global_cleanup(3) (libcurl manpage):
   * This function is not thread safe. You must not call it when any other
   * thread in the program  (i.e.  a  thread  sharing  the same memory) is
   * running.  This doesn't just mean no other thread that is using
   * libcurl.  Because curl_global_cleanup(3) calls  functions  of
   * other  libraries  that  are  similarly  thread  unsafe, it could
   * conflict with any other thread that uses these other libraries.
   */
  curl_global_cleanup ();
}

pdf_bool_t
pdf_fsys_http_init (pdf_error_t **error)
{
  /* The documentation for libcurl indicates that this initialization
   * ( curl_global_init () ) is _not_ thread-safe, and that it should
   * therefore be performed during application startup while the
   * application as a whole is single-threaded. If this is not possible
   * it must be ensured that no other threads are initializing any of
   * the libraries that libcurl will be initializing.
   */

  /* From curl_global_init(3) (libcurl manpage):
   * This function is not thread safe. You must not call it when any
   * other thread in the program  (i.e.  a  thread  sharing  the same
   * memory) is running.  This doesn't just mean no other thread that
   * is using libcurl.  Because curl_global_init() calls functions of
   * other libraries that are similarly thread unsafe, it could
   * conflict with any other thread that uses these other libraries.
   */
  if (curl_global_init (CURL_GLOBAL_ALL) != 0)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     PDF_ERROR,
                     "couldn't initialize curl");
      return PDF_FALSE;
    }

  return pdf_fsys_add (PDF_FSYS_HTTP_ID,
                       (struct pdf_fsys_s *)&pdf_fsys_http_implementation,
                       pdf_fsys_http_deinit_cb,
                       error);
}

void
pdf_fsys_http_deinit (void)
{
  pdf_fsys_remove (PDF_FSYS_HTTP_ID);
}

/* End of pdf-fsys-http.c */

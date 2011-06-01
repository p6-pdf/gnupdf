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

  /* Common handle in the filesystem for multi-functions */
  CURLM *curlm;
};

/* Filesystem-specific implementation of open files */
struct pdf_fsys_http_file_s
{
  /* The common parent struct */
  struct pdf_fsys_file_s common;

  /* The URL to pass to curl */
  pdf_char_t *url;

  /* The reported Content-Length, if any
   * NOTE: (double)-1 if none reported */
  double content_length;

  /* Current position when reading */
  pdf_off_t pos;
};

/*
 * Filesystem Interface Implementation
 */

static size_t
request_http_head_header_parse (void   *ptr,
                                size_t  size,
                                size_t  nmemb,
                                void   *user_data)
{
  /* NOTE:
   * This callback gets called for ALL headers of all intermediate responses,
   * not just for the final one.
   *
   * ALSO:
   * Not to be assumed that the string we get is NUL-terminated.. :-/
   */
  pdf_size_t n_bytes = size * nmemb;
  pdf_char_t *str = (pdf_char_t *)ptr;
  pdf_bool_t *accept_ranges = (pdf_bool_t *)user_data;

#define ACCEPT_RANGES "Accept-Ranges: bytes"

  /* TODO: Use a regex to match the header we're looking for */
  if (*accept_ranges == PDF_FALSE &&
      n_bytes >= strlen (ACCEPT_RANGES) &&
      strncmp (str, ACCEPT_RANGES, strlen (ACCEPT_RANGES)) == 0)
    {
      *accept_ranges =  PDF_TRUE;
    }

#undef ACCEPT_RANGES

  return n_bytes;
}

static pdf_bool_t
request_http_head (const pdf_char_t  *url,
                   double            *content_length,
                   pdf_error_t      **error)
{
  CURL *curl;
  CURLcode res;
  long http_res;
  double inner_content_length;
  pdf_bool_t accept_ranges = PDF_FALSE;

  curl = curl_easy_init ();

  /* Setup request options */
  if ((res = curl_easy_setopt (curl,
                               CURLOPT_URL,
                               url)) != CURLE_OK ||
      /* Set that no-body is requested (HTTP-HEADER only) */
      (res = curl_easy_setopt (curl,
                               CURLOPT_NOBODY,
                               1L)) != CURLE_OK ||
      /* Try to follow location and therefore avoid 302 errors */
      (res = curl_easy_setopt (curl,
                               CURLOPT_FOLLOWLOCATION,
                               1L)) != CURLE_OK ||
      /* Setup a header parsing function */
      (res = curl_easy_setopt (curl,
                               CURLOPT_HEADERFUNCTION,
                               request_http_head_header_parse)) != CURLE_OK ||
      /* Setup context data for the header parsing function */
      (res = curl_easy_setopt (curl,
                               CURLOPT_HEADERDATA,
                               &accept_ranges)) != CURLE_OK)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     PDF_ERROR,
                     "couldn't set request options: '%s'",
                     curl_easy_strerror (res));
      curl_easy_cleanup (curl);
      return PDF_FALSE;
    }

  /* Perform request, BLOCKING */
  if ((res = curl_easy_perform (curl)) != CURLE_OK)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     PDF_ERROR,
                     "couldn't perform request: '%s'",
                     curl_easy_strerror (res));
      curl_easy_cleanup (curl);
      return PDF_FALSE;
    }

  /* Get response code */
  if ((res = curl_easy_getinfo (curl,
                                CURLINFO_RESPONSE_CODE,
                                &http_res)) != CURLE_OK)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     PDF_ERROR,
                     "couldn't get request response code: '%s'",
                     curl_easy_strerror (res));
      curl_easy_cleanup (curl);
      return PDF_FALSE;
    }

  /* If response code is NOT 200 OK, set an error here */
  if (http_res != 200)
    {
      /* TODO: if no response was received, http_res will be 0 */
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     PDF_ERROR,
                     "error in HTTP request: %ld",
                     http_res);
      curl_easy_cleanup (curl);
      return PDF_FALSE;
    }

  /* We do not support files reported as not allowing Ranges */
  if (!accept_ranges)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     PDF_EIMPLLIMIT,
                     "Accept-Ranges not supported by remote server");
      curl_easy_cleanup (curl);
      return PDF_FALSE;
    }

  /* Get Content-Length. It is ok if we don't get one */
  if (content_length)
    {
      if (curl_easy_getinfo (curl,
                             CURLINFO_CONTENT_LENGTH_DOWNLOAD,
                             &inner_content_length) == CURLE_OK &&
          inner_content_length >= 0)
        {
          *content_length = inner_content_length;
        }
      else
        {
          *content_length = (double)-1;
        }
    }

  curl_easy_cleanup (curl);
  return PDF_TRUE;
}

static pdf_fsys_file_t *
file_open (const pdf_fsys_t           *fsys,
           const pdf_text_t           *path_name,
           enum pdf_fsys_file_mode_e   mode,
           pdf_error_t               **error)
{
  struct pdf_fsys_http_file_s *file;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, NULL);

  /* Allow only open for READ-ing */
  if (mode != PDF_FSYS_OPEN_MODE_READ)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     PDF_EBADDATA,
                     "can't open: the http filesystem is read-only");
      return PDF_FALSE;
    }

  /* Allocate private data storage for the file */
  file = (struct pdf_fsys_http_file_s *) pdf_alloc (sizeof (struct pdf_fsys_http_file_s));
  if (!file)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     PDF_ENOMEM,
                     "cannot create http file object: "
                     "couldn't allocate %lu bytes",
                     (unsigned long) sizeof (struct pdf_fsys_http_file_s));
      return NULL;
    }

  memset (file, 0, sizeof (struct pdf_fsys_http_file_s));

  /* Initialize common data */
  if (!pdf_fsys_file_init_common (&(file->common),
                                  fsys,
                                  path_name,
                                  mode,
                                  error))
    {
      pdf_fsys_file_deinit_common (&(file->common));
      pdf_dealloc (file);
      return NULL;
    }

  /* Get URL */
  file->url = pdf_fsys_get_url_from_path (fsys,
                                          file->common.unicode_path,
                                          error);
  if (!file->url)
    {
      pdf_fsys_file_deinit_common (&(file->common));
      pdf_dealloc (file);
      return NULL;
    }

  /* Launch an HTTP-HEAD request to get basic info of the remote file */
  if (!request_http_head (file->url,
                          &(file->content_length),
                          error))
    {
      pdf_fsys_file_deinit_common (&(file->common));
      pdf_dealloc (file);
      return NULL;
    }

  return (pdf_fsys_file_t *)file;
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
  struct pdf_fsys_http_file_s *http_file = (struct pdf_fsys_http_file_s *)file;
  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_FALSE);

  if (http_file->url)
    pdf_dealloc (http_file->url);

  pdf_fsys_file_deinit_common (&(http_file->common));
  pdf_dealloc (file);

  return PDF_TRUE;
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
  pdf_char_t *url;
  double content_length;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (props, PDF_FALSE);

  /* Get URL */
  url = pdf_fsys_get_url_from_path (fsys,
                                    path_name,
                                    error);
  if (!url)
    return PDF_FALSE;

  /* Launch an HTTP-HEAD request to get basic info of the remote file */
  if (!request_http_head (url,
                          &content_length,
                          error))
    {
      pdf_dealloc (url);
      return PDF_FALSE;
    }

  props->is_hidden = PDF_FALSE;
  props->is_readable = PDF_TRUE;
  props->is_writable = PDF_FALSE;

  pdf_time_init (&props->creation_date);
  pdf_time_set_utc (&props->creation_date, 0);
  pdf_time_init (&props->modification_date);
  pdf_time_set_utc (&props->modification_date, 0);

  props->file_size = (pdf_off_t)content_length;
  props->folder_size = 0;

  pdf_dealloc (url);
  return PDF_TRUE;
}

static pdf_bool_t
item_p (const pdf_fsys_t *fsys,
        const pdf_text_t *path_name)
{
  pdf_char_t *url;

  PDF_ASSERT_POINTER_RETURN_VAL (path_name, PDF_FALSE);

  /* Get URL */
  url = pdf_fsys_get_url_from_path (fsys,
                                    path_name,
                                    NULL);
  if (!url)
    return PDF_FALSE;

  /* Launch an HTTP-HEAD request to get basic info of the remote file */
  if (!request_http_head (url, NULL, NULL))
    {
      pdf_dealloc (url);
      return PDF_FALSE;
    }

  /* It does exist! (we got 200 OK) */
  pdf_dealloc (url);
  return PDF_TRUE;
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
                   const pdf_text_t  *path,
                   pdf_error_t      **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (fsys, NULL);
  PDF_ASSERT_POINTER_RETURN_VAL (path, NULL);

  /* TODO: Properly do this. We must make sure the string we get is a real valid
   * URL, see FS#126 */
  return pdf_text_get_unicode (path,
                               PDF_TEXT_UTF8,
                               PDF_TEXT_UNICODE_WITH_NUL_SUFFIX,
                               NULL,
                               error);
}

static pdf_bool_t
file_same_p (const pdf_fsys_file_t  *file,
             const pdf_text_t       *path,
             pdf_error_t           **error)
{
  const struct pdf_fsys_http_file_s *http_file = (const struct pdf_fsys_http_file_s *)file;
  pdf_char_t *path_url;
  pdf_bool_t same;

  path_url = pdf_fsys_get_url_from_path (file->fsys,
                                         path,
                                         error);
  if (!path_url)
    return PDF_FALSE;

  same = (strcmp (path_url, http_file->url) == 0 ?
          PDF_TRUE : PDF_FALSE);

  pdf_dealloc (path_url);
  return same;
}

static pdf_off_t
file_get_pos (const pdf_fsys_file_t  *file,
              pdf_error_t           **error)
{
  const struct pdf_fsys_http_file_s *http_file = (const struct pdf_fsys_http_file_s *)file;

  PDF_ASSERT_POINTER_RETURN_VAL (file, (pdf_off_t)-1);

  return http_file->pos;
}

static pdf_bool_t
file_set_pos (pdf_fsys_file_t  *file,
              pdf_off_t         new_pos,
              pdf_error_t     **error)
{
  struct pdf_fsys_http_file_s *http_file = (struct pdf_fsys_http_file_s *)file;

  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_FALSE);
  PDF_ASSERT_RETURN_VAL (new_pos >= 0, PDF_FALSE);

  /* If we got reported a proper content-length, ensure we don't go off its limits */
  if (http_file->content_length >= 0 &&
      new_pos > http_file->content_length)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     PDF_EINVRANGE,
                     "cannot set new position (%lu) off limits (%lu)",
                     (unsigned long)new_pos,
                     (unsigned long)http_file->content_length);
      return PDF_FALSE;
    }

  /* If we don't have a proper content-length value, or it is within limits,
   * just accept the new pos */
  http_file->pos = new_pos;

  return PDF_TRUE;
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
  const struct pdf_fsys_http_file_s *http_file = (const struct pdf_fsys_http_file_s *)file;

  PDF_ASSERT_POINTER_RETURN_VAL (file, (pdf_off_t)-1);

  /* We got size during the HTTP-HEAD while opening the file */
  return (pdf_off_t)http_file->content_length;
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
  /* Cleanup the multi-function handle */
  if (pdf_fsys_http_implementation.curlm)
    {
      curl_multi_cleanup (pdf_fsys_http_implementation.curlm);
      pdf_fsys_http_implementation.curlm = NULL;
    }

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

  /* Initialize the multi-function handle */
  pdf_fsys_http_implementation.curlm = curl_multi_init ();
  if (!pdf_fsys_http_implementation.curlm)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     PDF_ERROR,
                     "couldn't initialize multi-function curl handle");
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

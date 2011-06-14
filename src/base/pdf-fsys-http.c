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

static const pdf_char_t *http_schemes[] = {
  "http://",
  "https://",
  NULL
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

struct partial_get_context_s {
  pdf_char_t *buffer;
  pdf_size_t  available_size;
  pdf_size_t  index;
};

static size_t
request_http_partial_get_body_parse (void   *ptr,
                                     size_t  size,
                                     size_t  nmemb,
                                     void   *user_data)
{
  pdf_size_t n_bytes = size * nmemb;
  struct partial_get_context_s *context = user_data;

  if (n_bytes == 0)
    return 0;

  if (n_bytes > context->available_size)
    {
      /* If we ask for 10 bytes, we should not get more than 10.
       * We return 0 here so that we get an error afterwards. */
      PDF_ASSERT_TRACE_NOT_REACHED ();
      return 0;
    }

  memcpy (&context->buffer[context->index], ptr, n_bytes);
  context->available_size -= n_bytes;
  context->index += n_bytes;

  return n_bytes;
}

static pdf_bool_t
request_http_partial_get (const pdf_char_t  *url,
                          pdf_char_t        *buffer,
                          pdf_size_t         offset,
                          pdf_size_t         bytes,
                          pdf_size_t        *read_bytes,
                          pdf_error_t      **error)
{
  CURL *curl;
  CURLcode res;
  long http_res;
  pdf_char_t range [128] = { 0 };
  struct partial_get_context_s context;

  /* Setup context */
  context.buffer = buffer;
  context.available_size = bytes;
  context.index = 0;

  curl = curl_easy_init ();

  /* Compute range string */
  sprintf (range,
           "%lu-%lu",
           (unsigned long)offset,
           (unsigned long) (offset + bytes - 1));

  /* Setup request options */
  if ((res = curl_easy_setopt (curl,
                               CURLOPT_URL,
                               url)) != CURLE_OK ||
      /* Try to follow location and therefore avoid 302 errors */
      (res = curl_easy_setopt (curl,
                               CURLOPT_FOLLOWLOCATION,
                               1L)) != CURLE_OK ||
      /* Set range of bytes to download */
      (res = curl_easy_setopt (curl,
                               CURLOPT_RANGE,
                               range)) != CURLE_OK ||
      /* Setup a content parsing function */
      (res = curl_easy_setopt (curl,
                               CURLOPT_WRITEFUNCTION,
                               request_http_partial_get_body_parse)) != CURLE_OK ||
      /* Setup context data for the context parsing function */
      (res = curl_easy_setopt (curl,
                               CURLOPT_WRITEDATA,
                               &context)) != CURLE_OK)
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

  /* Several responses should be treated as no-errors:
   *  - 200 OK
   *  - 206 Partial Content
   */
  if (http_res != 200 &&
      http_res != 206)
    {
      /* Treat 416 Requested Range Not Satisfiable in a special way */
      if (http_res == 416)
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_FSYS,
                         PDF_EINVRANGE,
                         "invalid range requested: %ld",
                         http_res);
        }
      else
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_FSYS,
                         PDF_ERROR,
                         "error in HTTP request: %ld",
                         http_res);
        }
      curl_easy_cleanup (curl);
      return PDF_FALSE;
    }

  /* Set number of read bytes */
  *read_bytes = context.index;

  curl_easy_cleanup (curl);
  return PDF_TRUE;
}

static pdf_fsys_file_t *
file_open (const pdf_fsys_t           *fsys,
           const pdf_text_t           *path,
           enum pdf_fsys_file_mode_e   mode,
           pdf_error_t               **error)
{
  struct pdf_fsys_http_file_s *file;

  PDF_ASSERT_POINTER_RETURN_VAL (path, NULL);

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
  if (!pdf_fsys_impl_helper_file_init (&(file->common),
                                       fsys,
                                       path,
                                       mode,
                                       error))
    {
      pdf_fsys_impl_helper_file_deinit (&(file->common));
      pdf_dealloc (file);
      return NULL;
    }

  /* Get URL */
  file->url = pdf_fsys_get_url_from_path (fsys,
                                          file->common.unicode_path,
                                          error);
  if (!file->url)
    {
      pdf_fsys_impl_helper_file_deinit (&(file->common));
      pdf_dealloc (file);
      return NULL;
    }

  /* Launch an HTTP-HEAD request to get basic info of the remote file */
  if (!request_http_head (file->url,
                          &(file->content_length),
                          error))
    {
      pdf_fsys_impl_helper_file_deinit (&(file->common));
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

  pdf_fsys_impl_helper_file_deinit (&(http_file->common));
  pdf_dealloc (file);

  return PDF_TRUE;
}

static pdf_bool_t
create_folder (const pdf_fsys_t  *fsys,
               const pdf_text_t  *path,
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
                     const pdf_text_t  *path,
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

static const pdf_char_t *
skip_scheme_from_url (const pdf_char_t *url)
{
  pdf_u32_t i;
  const pdf_char_t *start;

  /* Skip scheme from the URL */
  start = NULL;
  i = 0;
  while (!start && http_schemes[i])
    {
      if (strncmp (url,
                   http_schemes[i],
                   strlen (http_schemes[i])) == 0)
        start = &url[strlen (http_schemes[i])];
    }

  return start;
}

static pdf_bool_t
get_directory_and_filename (const pdf_fsys_t  *fsys,
                            const pdf_text_t  *path,
                            pdf_text_t       **directory,
                            pdf_text_t       **filename,
                            pdf_error_t      **error)
{
  pdf_size_t utf8_size;
  pdf_char_t *utf8;
  pdf_char_t *p;
  const pdf_char_t *start;
  pdf_size_t filename_size;

  /* Get in UTF-8 */
  utf8 = pdf_text_get_unicode (path,
                               PDF_TEXT_UTF8,
                               PDF_TEXT_UNICODE_WITH_NUL_SUFFIX,
                               &utf8_size,
                               error);
  if (!utf8)
    return PDF_FALSE;

  start = skip_scheme_from_url (utf8);
  if (!start)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     PDF_ERROR,
                     "invalid URL: '%s'",
                     utf8);
      pdf_dealloc (utf8);
      return PDF_FALSE;
    }

  /* Move pointer to last character in the path
   * We move 2 positions because utf8_size here counts last NUL byte */
  p = &utf8[utf8_size - 2];

  /* Skip possible separator at the end of the path */
  if (*p == '/')
    p--;

  /* Find previous directory separator */
  while (p >= start && *p != '/')
    p--;

  /* Directory separator found? */
  if (p < start)
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

  if (directory)
    {
      /* Directory last (to be NUL-ed) depends on whether the directory is the root
       * directory (we shouldn't remove the dir separator from the root directory)
       */
      if (p == start)
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
          pdf_dealloc (utf8);
          return PDF_FALSE;
        }
    }

  pdf_dealloc (utf8);
  return PDF_TRUE;
}

static pdf_text_t *
get_basename (const pdf_fsys_t  *fsys,
              const pdf_text_t  *path,
              pdf_error_t      **error)
{
  pdf_text_t *basename;

  PDF_ASSERT_POINTER_RETURN_VAL (path, NULL);

  if (!get_directory_and_filename (fsys,
                                   path,
                                   NULL,
                                   &basename,
                                   error))
    {
      pdf_prefix_error (error, "couldn't get basename: ");
      return NULL;
    }

  return basename;
}

static pdf_text_t *
get_parent (const pdf_fsys_t  *fsys,
            const pdf_text_t  *path,
            pdf_error_t      **error)
{
  pdf_text_t *parent;

  PDF_ASSERT_POINTER_RETURN_VAL (path, NULL);

  if (!get_directory_and_filename (fsys,
                                   path,
                                   &parent,
                                   NULL,
                                   error))
    {
      pdf_prefix_error (error, "couldn't get parent: ");
      return NULL;
    }

  return parent;
}

static pdf_bool_t
remove_folder (const pdf_fsys_t  *fsys,
               const pdf_text_t  *path,
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
                const pdf_text_t              *path,
                struct pdf_fsys_item_props_s  *props,
                pdf_error_t                  **error)
{
  pdf_char_t *url;
  double content_length;

  PDF_ASSERT_POINTER_RETURN_VAL (path, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (props, PDF_FALSE);

  /* Get URL */
  url = pdf_fsys_get_url_from_path (fsys,
                                    path,
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

  pdf_dealloc (url);
  return PDF_TRUE;
}

static pdf_bool_t
item_p (const pdf_fsys_t *fsys,
        const pdf_text_t *path)
{
  pdf_char_t *url;

  PDF_ASSERT_POINTER_RETURN_VAL (path, PDF_FALSE);

  /* Get URL */
  url = pdf_fsys_get_url_from_path (fsys,
                                    path,
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
                 const pdf_text_t *path)
{
  /* HTTP filesystem is read-only, so if the item exists, it is readable */
  return item_p (fsys, path);
}

static pdf_bool_t
item_writable_p (const pdf_fsys_t *fsys,
                 const pdf_text_t *path)
{
  /* HTTP filesystem is read-only, so items NEVER writable */
  return PDF_FALSE;
}

static pdf_i64_t
get_free_space (const pdf_fsys_t  *fsys,
                const pdf_text_t  *path,
                pdf_error_t      **error)
{
  /* HTTP filesystem is read-only, so NO free space always */
  return 0;
}

static pdf_bool_t
compare_path_p (const pdf_fsys_t  *fsys,
                const pdf_text_t  *first,
                const pdf_text_t  *second,
                pdf_error_t      **error)
{
  /* TODO */

  PDF_ASSERT_POINTER_RETURN_VAL (first, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (second, PDF_FALSE);

  return PDF_FALSE;
}

static pdf_text_t *
build_path (const pdf_fsys_t  *fsys,
            pdf_error_t      **error,
            const pdf_text_t  *first_element,
            ...)
{
  pdf_text_t *output;
  pdf_char_t *utf8;
  pdf_size_t utf8_size;
  pdf_text_t *next;
  va_list args;

  PDF_ASSERT_POINTER_RETURN_VAL (first_element, NULL);

  /* Get first in UTF-8 */
  utf8 = pdf_text_get_unicode (first_element,
                               PDF_TEXT_UTF8,
                               PDF_TEXT_UNICODE_WITH_NUL_SUFFIX,
                               &utf8_size,
                               error);
  if (!utf8)
    return NULL;

  /* Ensure first item comes with scheme */
  if (!skip_scheme_from_url (utf8))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     PDF_ERROR,
                     "cannot build path: "
                     "first element must have URL scheme");
      pdf_dealloc (utf8);
      return NULL;
    }

  /* Do not count last NUL as part of the size */
  utf8_size--;

  va_start (args, first_element);
  next = va_arg (args, pdf_text_t *);
  while (next != NULL)
    {
      pdf_char_t *next_utf8;
      pdf_size_t next_utf8_size;
      pdf_char_t *new_url;

      /* Get next in UTF-8 */
      next_utf8 = pdf_text_get_unicode (next,
                                        PDF_TEXT_UTF8,
                                        PDF_TEXT_UNICODE_WITH_NUL_SUFFIX,
                                        &next_utf8_size,
                                        error);
      if (!next_utf8)
        {
          pdf_dealloc (utf8);
          return NULL;
        }

      /* Do not count last NUL as part of the size */
      next_utf8_size--;

      /* Ensure next elements do NOT have scheme */
      if (skip_scheme_from_url (next_utf8))
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_FSYS,
                         PDF_ERROR,
                         "cannot build path: "
                         "next elements must not have URL scheme");
          pdf_dealloc (utf8);
          pdf_dealloc (next_utf8);
          return NULL;
        }

      /* Reallocate for the expanded URL.
       *  +1 for '/'
       *  +1 for last NUL
       */
      new_url = pdf_realloc (utf8, utf8_size + next_utf8_size + 2);
      if (!new_url)
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_FSYS,
                         PDF_ENOMEM,
                         "cannot build path: "
                         "couldn't reallocate %lu bytes",
                         utf8_size + next_utf8_size + 2);
          pdf_dealloc (utf8);
          pdf_dealloc (next_utf8);
          return NULL;
        }

      /* Concat next part */
      utf8 = new_url;
      utf8[utf8_size] = '/';
      memcpy (&utf8[utf8_size + 1], next_utf8, next_utf8_size);
      utf8_size  += (next_utf8_size + 1);
      utf8[utf8_size] = '\0';

      next = va_arg (args, pdf_text_t *);
    }
  va_end (args);

  /* Create pdf_text_t to return */
  output = pdf_text_new_from_unicode (utf8,
                                      utf8_size,
                                      PDF_TEXT_UTF8,
                                      error);
  pdf_dealloc (utf8);
  return output;
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

static pdf_text_t *
get_path_from_url (const pdf_fsys_t  *fsys,
                   const pdf_char_t  *url,
                   pdf_error_t      **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (url, NULL);

  /* TODO. See FS#126 */

  return NULL;
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
  struct pdf_fsys_http_file_s *http_file = (struct pdf_fsys_http_file_s *)file;
  pdf_size_t inner_read_bytes = 0;

  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_FALSE);

  if (request_http_partial_get (http_file->url,
                                buf,
                                (pdf_size_t)http_file->pos,
                                bytes,
                                &inner_read_bytes,
                                error))
    {
      /* Advance the reading offset ourselves here */
      http_file->pos += inner_read_bytes;
      if (read_bytes)
        *read_bytes = inner_read_bytes;

      /* Return FALSE (eof) if read bytes are less than requested ones */
      return (inner_read_bytes < bytes ?
              PDF_FALSE: PDF_TRUE);
    }
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

static pdf_u32_t
file_request_ria (pdf_fsys_file_t  *file,
                  pdf_off_t         offset,
                  pdf_size_t        count,
                  pdf_error_t     **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (file, 0);

  /* TODO */

  pdf_set_error (error,
                 PDF_EDOMAIN_BASE_FSYS,
                 PDF_EBADDATA,
                 "can't request RIA: not provided by the http filesystem");
  return 0;
}

static pdf_bool_t
file_has_ria (pdf_fsys_file_t *file)
{
  PDF_ASSERT_POINTER_RETURN_VAL (file, PDF_FALSE);

  /* TODO */

  return PDF_FALSE;
}

static pdf_bool_t
file_cancel_ria (pdf_fsys_file_t  *file,
                 pdf_u32_t         ria_id,
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

/* Setup the http filesystem implementation, using named initializers */
static struct pdf_fsys_http_s pdf_fsys_http_implementation =
  {
    .common.get_free_space_fn      = get_free_space,
    .common.create_folder_fn       = create_folder,
    .common.get_folder_contents_fn = get_folder_contents,
    .common.remove_folder_fn       = remove_folder,
    .common.get_parent_fn          = get_parent,
    .common.get_basename_fn        = get_basename,
    .common.compare_path_p_fn      = compare_path_p,
    .common.build_path_fn          = build_path,
    .common.get_url_from_path_fn   = get_url_from_path,
    .common.get_path_from_url_fn   = get_path_from_url,
    .common.item_p_fn              = item_p,
    .common.get_item_props_fn      = get_item_props,
    .common.item_readable_p_fn     = item_readable_p,
    .common.item_writable_p_fn     = item_writable_p,
    .common.file_open_fn           = file_open,
    .common.file_open_tmp_fn       = file_open_tmp,
    .common.file_reopen_fn         = file_reopen,
    .common.file_close_fn          = file_close,
    .common.file_get_size_fn       = file_get_size,
    .common.file_can_set_size_p_fn = file_can_set_size_p,
    .common.file_set_size_fn       = file_set_size,
    .common.file_get_pos_fn        = file_get_pos,
    .common.file_set_pos_fn        = file_set_pos,
    .common.file_read_fn           = file_read,
    .common.file_write_fn          = file_write,
    .common.file_flush_fn          = file_flush,
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

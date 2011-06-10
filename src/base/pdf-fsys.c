/* -*- mode: C -*-
 *
 *       File:         pdf-fsys.c
 *       Date:         Thu May 22 15:51:13 2008
 *
 *       GNU PDF Library - Filesystem Module
 *
 */

/* Copyright (C) 2008, 2011 Free Software Foundation, Inc. */

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

#include <pthread.h>

#include <pdf-alloc.h>
#include <pdf-hash-helper.h>
#include <pdf-fsys.h>
#include <pdf-fsys-disk.h>

#if PDF_HAVE_FSYS_HTTP
# include <pdf-fsys-http.h>
#endif /* PDF_HAVE_FSYS_HTTP */

/* Hash of filesystem implementations */
pdf_hash_t *fsys_implementations;
pthread_mutex_t fsys_implementations_mutex = PTHREAD_MUTEX_INITIALIZER;

static pdf_bool_t
validate_implementation (const pdf_char_t         *id,
                         const struct pdf_fsys_s  *implementation,
                         pdf_error_t             **error)
{
  /* Validate the implementation */
#define VALIDATE_METHOD(method) do {                                    \
    if (!implementation->method)                                        \
      {                                                                 \
        pdf_set_error (error,                                           \
                       PDF_EDOMAIN_BASE_FSYS,                           \
                       PDF_ERROR,                                       \
                       "Missing method '%s' in Filesystem '%s' implementation", \
                       #method,                                         \
                       id);                                             \
        return PDF_FALSE;                                               \
      }                                                                 \
  } while (0)

  VALIDATE_METHOD (create_folder_fn);
  VALIDATE_METHOD (get_folder_contents_fn);
  VALIDATE_METHOD (get_parent_fn);
  VALIDATE_METHOD (get_basename_fn);
  VALIDATE_METHOD (remove_folder_fn);
  VALIDATE_METHOD (get_item_props_fn);
  VALIDATE_METHOD (get_free_space_fn);
  VALIDATE_METHOD (item_p_fn);
  VALIDATE_METHOD (item_readable_p_fn);
  VALIDATE_METHOD (item_writable_p_fn);
  VALIDATE_METHOD (build_path_fn);
  VALIDATE_METHOD (get_url_from_path_fn);
  VALIDATE_METHOD (get_path_from_url_fn);

  VALIDATE_METHOD (file_open_fn);
  VALIDATE_METHOD (file_open_tmp_fn);
  VALIDATE_METHOD (file_reopen_fn);
  VALIDATE_METHOD (file_close_fn);
  VALIDATE_METHOD (file_read_fn);
  VALIDATE_METHOD (file_write_fn);
  VALIDATE_METHOD (file_flush_fn);
  VALIDATE_METHOD (file_can_set_size_p_fn);
  VALIDATE_METHOD (file_get_size_fn);
  VALIDATE_METHOD (file_set_size_fn);
  VALIDATE_METHOD (file_get_pos_fn);
  VALIDATE_METHOD (file_set_pos_fn);
  VALIDATE_METHOD (file_set_mode_fn);
  VALIDATE_METHOD (file_same_p_fn);
  VALIDATE_METHOD (file_request_ria_fn);
  VALIDATE_METHOD (file_has_ria_fn);
  VALIDATE_METHOD (file_cancel_ria_fn);

#undef VALIDATE_METHOD

  return PDF_TRUE;
}

pdf_bool_t
pdf_fsys_add (const pdf_char_t         *id,
              const struct pdf_fsys_s  *fsys,
              pdf_fsys_deinit_fn_t      fsys_deinit,
              pdf_error_t             **error)
{
  PDF_ASSERT_RETURN_VAL (fsys, PDF_FALSE);

  if (!validate_implementation (id, fsys, error))
    return PDF_FALSE;

  pthread_mutex_lock (&fsys_implementations_mutex);

  /* If first implementation, allocate new HT */
  if (!fsys_implementations)
    {
      fsys_implementations = pdf_hash_new (error);
      if (!fsys_implementations)
        {
          pthread_mutex_unlock (&fsys_implementations_mutex);
          return PDF_FALSE;
        }
    }

  /* Check if ID already exists */
  if (pdf_hash_key_p (fsys_implementations, id))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_FSYS,
                     PDF_EEXIST,
                     "Filesystem with ID '%s' already exists",
                     id);
      pthread_mutex_unlock (&fsys_implementations_mutex);
      return PDF_FALSE;
    }

  /* Add the new implementation */
  if (!pdf_hash_add (fsys_implementations,
                     id,
                     fsys,
                     (pdf_hash_value_dispose_fn_t)fsys_deinit,
                     error))
    {
      pthread_mutex_unlock (&fsys_implementations_mutex);
      return PDF_FALSE;
    }

  pthread_mutex_unlock (&fsys_implementations_mutex);
  return PDF_TRUE;
}

void
pdf_fsys_remove (const pdf_char_t *id)
{
  pthread_mutex_lock (&fsys_implementations_mutex);

  if (fsys_implementations)
    {
      if (!pdf_hash_remove (fsys_implementations, id))
        {
          /* If the implementation was not removed is because it was not registered,
           * and that is a programmer error */
          PDF_ASSERT_TRACE_NOT_REACHED ();
        }
      else if (pdf_hash_size (fsys_implementations) == 0)
        {
          /* Remove hash if no contents */
          pdf_hash_destroy (fsys_implementations);
          fsys_implementations = NULL;
        }
    }

  pthread_mutex_unlock (&fsys_implementations_mutex);
}

const pdf_fsys_t *
pdf_fsys_get (const pdf_char_t *id)
{
  const pdf_fsys_t *impl;

  pthread_mutex_lock (&fsys_implementations_mutex);

  impl = (fsys_implementations ?
          pdf_hash_get_value (fsys_implementations, id) :
          NULL);;

  pthread_mutex_unlock (&fsys_implementations_mutex);

  return impl;
}

void
pdf_fsys_foreach (pdf_fsys_foreach_fn_t  foreach_fn,
                  void                  *user_data)
{
  pdf_hash_iterator_t it;

  /* Note we're locking the whole implementations HT here, so please do not call
   * pdf_fsys_remove() or pdf_fsys_get() from within the foreach_fn cb. */
  pthread_mutex_lock (&fsys_implementations_mutex);

  if (fsys_implementations)
    {
      if (pdf_hash_iterator_init (&it, fsys_implementations))
        {
          const pdf_char_t *id;
          const void *impl;

          while (pdf_hash_iterator_next (&it, &id, &impl))
            foreach_fn (user_data, id, impl);

          pdf_hash_iterator_deinit (&it);
        }
    }

  pthread_mutex_unlock (&fsys_implementations_mutex);
}

pdf_bool_t
pdf_fsys_init (pdf_error_t **error)
{
  /* Init default DISK filesystem */
  if (!pdf_fsys_disk_init (error))
    return PDF_FALSE;

#if PDF_HAVE_FSYS_HTTP
  /* Init HTTP read-only filesystem */
  if (!pdf_fsys_http_init (error))
    return PDF_FALSE;
#endif /* PDF_HAVE_FSYS_HTTP */

  return PDF_TRUE;
}

void
pdf_fsys_deinit (void)
{
#if PDF_HAVE_FSYS_HTTP
  /* Deinit HTTP read-only filesystem */
  pdf_fsys_http_deinit ();
#endif /* PDF_HAVE_FSYS_HTTP */

  /* Deinit DISK filesystem */
  pdf_fsys_disk_deinit ();
}

pdf_hash_t *
pdf_fsys_item_props_to_hash (const struct pdf_fsys_item_props_s  *item_props,
                             pdf_error_t                        **error)
{
  pdf_char_t *creation_date_str;
  pdf_char_t *mod_date_str;
  pdf_hash_t *props_hash;

  /* Create new hash table */
  props_hash = pdf_hash_new (error);
  if (!props_hash)
    return NULL;

  /* Associate values with hash keys */
  if ((!pdf_hash_add_bool (props_hash,
                           PDF_FSYS_ITEM_IS_HIDDEN,
                           item_props->is_hidden,
                           error)) ||
      (!pdf_hash_add_bool (props_hash,
                           PDF_FSYS_ITEM_IS_READABLE,
                           item_props->is_readable,
                           error)) ||
      (!pdf_hash_add_bool (props_hash,
                           PDF_FSYS_ITEM_IS_WRITABLE,
                           item_props->is_writable,
                           error)) ||
      (!pdf_hash_add_size (props_hash,
                           PDF_FSYS_ITEM_FILE_SIZE,
                           (pdf_size_t) item_props->file_size,
                           error)))
    {
      pdf_hash_destroy (props_hash);
      return NULL;
    }

  /* Get creation time from the props structure */
  creation_date_str = pdf_time_to_string (&item_props->creation_date,
                                          PDF_TIME_STRING_FORMAT_PDF,
                                          PDF_TIME_STRING_TRAILING_APOSTROPHE,
                                          error);
  if (!creation_date_str)
    {
      pdf_hash_destroy (props_hash);
      return NULL;
    }

  if (!pdf_hash_add_string (props_hash,
                            PDF_FSYS_ITEM_CREATION_DATE,
                            creation_date_str,
                            error))
    {
      pdf_dealloc (creation_date_str);
      pdf_hash_destroy (props_hash);
      return NULL;
    }

  /* Get mtime from the props structure */
  mod_date_str = pdf_time_to_string (&item_props->modification_date,
                                     PDF_TIME_STRING_FORMAT_PDF,
                                     PDF_TIME_STRING_TRAILING_APOSTROPHE,
                                     error);
  if (!mod_date_str)
    {
      pdf_hash_destroy (props_hash);
      return NULL;
    }

  if (!pdf_hash_add_string (props_hash,
                            PDF_FSYS_ITEM_MODIFICATION_DATE,
                            mod_date_str,
                            error))
    {
      pdf_dealloc (mod_date_str);
      pdf_hash_destroy (props_hash);
      return NULL;
    }

  /* Done */
  return props_hash;
}

pdf_bool_t
pdf_fsys_file_init_common (struct pdf_fsys_file_s     *common,
                           const struct pdf_fsys_s    *fsys,
                           const pdf_text_t           *path,
                           enum pdf_fsys_file_mode_e   mode,
                           pdf_error_t               **error)
{
  PDF_ASSERT_RETURN_VAL (common != NULL, PDF_FALSE);

  if (path)
    {
      common->unicode_path = pdf_text_dup (path, error);
      if (!common->unicode_path)
        return PDF_FALSE;
    }
  else
    common->unicode_path = NULL;

  common->fsys = fsys;
  common->mode = mode;

  return PDF_TRUE;
}

void
pdf_fsys_file_deinit_common (struct pdf_fsys_file_s *common)
{
  PDF_ASSERT_RETURN (common != NULL);

  if (common->unicode_path)
    pdf_text_destroy (common->unicode_path);
}

/* End of pdf-fsys.c */

/* -*- mode: C -*-
 *
 *       File:         pdf-stm-be-file.c
 *       Date:         Mon Apr 18 15:25:05 2011
 *
 *       GNU PDF Library - File Stream backend
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

#include <pdf-alloc.h>
#include <pdf-stm-be.h>
#include <pdf-stm-be-file.h>

/* File backend implementation */
struct pdf_stm_be_file_s
{
  struct pdf_stm_be_s parent;

  /* File handler */
  pdf_fsys_file_t file;

  /* Current offset in the file. This offset is only valid within this stream
   * backend object, and may not be equal to the real file offset. */
  pdf_off_t pos;
};

typedef struct pdf_stm_be_file_s pdf_stm_be_file_t;

static pdf_ssize_t stm_be_file_read    (pdf_stm_be_t *be,
                                        pdf_uchar_t  *buffer,
                                        pdf_size_t    bytes,
                                        pdf_error_t **error);
static pdf_ssize_t stm_be_file_write   (pdf_stm_be_t  *be,
                                        pdf_uchar_t   *buffer,
                                        pdf_size_t     bytes,
                                        pdf_error_t  **error);
static pdf_off_t   stm_be_file_seek    (pdf_stm_be_t *be,
                                        pdf_off_t     pos);
static pdf_off_t   stm_be_file_tell    (pdf_stm_be_t *be);
static void        stm_be_file_destroy (pdf_stm_be_t *be);

/* Vtable for virtual method implementations */
static const pdf_stm_be_vtable_t stm_be_vtable = {
  .read    = stm_be_file_read,
  .write   = stm_be_file_write,
  .seek    = stm_be_file_seek,
  .tell    = stm_be_file_tell,
  .destroy = stm_be_file_destroy,
};

pdf_stm_be_t *
pdf_stm_be_new_file (pdf_fsys_file_t   file,
                     pdf_off_t         pos,
                     pdf_error_t     **error)
{
  pdf_stm_be_file_t *new;

  /* Allocate a new structure */
  new = (pdf_stm_be_file_t *) pdf_alloc (sizeof (struct pdf_stm_be_file_s));
  if (!new)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "cannot create new file stream object: "
                     "couldn't allocate %lu bytes",
                     (unsigned long) sizeof (struct pdf_stm_be_file_s));
      return NULL;
    }

  /* Initialization */
  ((pdf_stm_be_t *)new)->vtable = &stm_be_vtable;
  new->file = file;
  new->pos = pos;

  return (pdf_stm_be_t *)new;
}

static void
stm_be_file_destroy (pdf_stm_be_t *be)
{
#warning We should take ownership of the file and close it here
  pdf_dealloc (be);
}

static pdf_bool_t
stm_be_ensure_correct_offset (pdf_stm_be_file_t *file_be)
{
  pdf_off_t current_pos;

  /* Get current real file offset */
  if (pdf_fsys_file_get_pos (file_be->file,
                             &current_pos) != PDF_OK)
    {
      PDF_ASSERT_TRACE_NOT_REACHED ();
      return PDF_FALSE;
    }

  /* If needed, fix file offset */
  if (current_pos != file_be->pos)
    {
      if (pdf_fsys_file_set_pos (file_be->file,
                                 file_be->pos) != PDF_OK)
        {
          PDF_ASSERT_TRACE_NOT_REACHED ();
          return PDF_FALSE;
        }
    }
  return PDF_TRUE;
}

static pdf_ssize_t
stm_be_file_read (pdf_stm_be_t  *be,
                  pdf_uchar_t   *buffer,
                  pdf_size_t     bytes,
                  pdf_error_t  **error)
{
  pdf_stm_be_file_t *file_be = (pdf_stm_be_file_t *)be;
  pdf_size_t read_bytes = 0;
  pdf_status_t ret;

  /* Note: bytes is unsigned */
  if (bytes == 0)
    return (pdf_ssize_t)0;

  /* Ensure we read from the correct offset */
  if (!stm_be_ensure_correct_offset (file_be))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_EINVRANGE,
                     "cannot read from file backend: "
                     "unable to correct offset");
      return -1;
    }

  /* Read the requested number of bytes */
  ret = pdf_fsys_file_read (file_be->file,
                            (pdf_char_t *)buffer,
                            bytes,
                            &read_bytes);
  /* Make sure EEOF is not treated as an error */
  if (ret != PDF_OK && ret != PDF_EEOF)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ERROR,
                     "cannot read from file backend");
      return -2;
    }

  /* Update the position of the stream */
  file_be->pos += read_bytes;

  /* NOTE: Important note, we do not recover the original file offset
   * we found when entering this method. This is, using the file stream
   * backend WILL modify the real file offset. This is done so that we
   * don't waste time moving the file offset when not really needed.
   */

  return (pdf_ssize_t)read_bytes;
}

static pdf_ssize_t
stm_be_file_write (pdf_stm_be_t  *be,
                   pdf_uchar_t   *buffer,
                   pdf_size_t     bytes,
                   pdf_error_t  **error)
{
  pdf_stm_be_file_t *file_be = (pdf_stm_be_file_t *)be;
  pdf_size_t written_bytes = 0;

  /* Note: bytes is unsigned */
  if (bytes == 0)
    return (pdf_ssize_t)0;

  /* Ensure we write in the correct offset */
  if (!stm_be_ensure_correct_offset (file_be))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_EINVRANGE,
                     "cannot write to file backend: "
                     "unable to correct offset");
      return -1;
    }

  /* Write the requested number of bytes */
  if (pdf_fsys_file_write (file_be->file,
                           (pdf_char_t *)buffer,
                           bytes,
                           &written_bytes) != PDF_OK)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ERROR,
                     "cannot write to file backend");
      return -2;
    }

  /* Update the position of the stream */
  file_be->pos += written_bytes;

  /* NOTE: Important note, we do not recover the original file offset
   * we found when entering this method. This is, using the file stream
   * backend WILL modify the real file offset. This is done so that we
   * don't waste time moving the file offset when not really needed.
   */

  return (pdf_ssize_t)written_bytes;
}

static pdf_off_t
stm_be_file_seek (pdf_stm_be_t *be,
                  pdf_off_t     pos)
{
  pdf_stm_be_file_t *file_be = (pdf_stm_be_file_t *)be;
  pdf_off_t file_size;

  file_size = pdf_fsys_file_get_size (file_be->file);

  /* Ensure we don't go off limits */
  if (pos < 0)
    pos = 0;
  if (pos >= file_size)
    pos = (file_size > 0 ? file_size - 1 : 0);

  file_be->pos = pos;
  return pos;
}

static pdf_off_t
stm_be_file_tell (pdf_stm_be_t *be)
{
  pdf_stm_be_file_t *file_be = (pdf_stm_be_file_t *)be;

  return file_be->pos;
}

/* End of pdf-stm-be-file.c */

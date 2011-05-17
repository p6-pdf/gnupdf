/* -*- mode: C -*-
 *
 *       File:         pdf-stm-be-cfile.c
 *       Date:         Mon Apr 18 16:05:33 2011
 *
 *       GNU PDF Library - C-File Stream backend
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
#include <pdf-stm-be-cfile.h>

/* C-File backend implementation */
struct pdf_stm_be_cfile_s
{
  struct pdf_stm_be_s parent;

  /* C File handler */
  FILE *file;

  /* Current offset in the file. This offset is only valid within this stream
   * backend object, and may not be equal to the real file offset. */
  pdf_off_t pos;
};

typedef struct pdf_stm_be_cfile_s pdf_stm_be_cfile_t;

static pdf_ssize_t stm_be_cfile_read    (pdf_stm_be_t *be,
                                         pdf_uchar_t  *buffer,
                                         pdf_size_t    bytes,
                                         pdf_error_t **error);
static pdf_ssize_t stm_be_cfile_write   (pdf_stm_be_t  *be,
                                         pdf_uchar_t   *buffer,
                                         pdf_size_t     bytes,
                                         pdf_error_t  **error);
static pdf_off_t   stm_be_cfile_seek    (pdf_stm_be_t *be,
                                         pdf_off_t     pos);
static pdf_off_t   stm_be_cfile_tell    (pdf_stm_be_t *be);
static void        stm_be_cfile_destroy (pdf_stm_be_t *be);

/* Vtable for virtual method implementations */
static const pdf_stm_be_vtable_t stm_be_vtable = {
  .read    = stm_be_cfile_read,
  .write   = stm_be_cfile_write,
  .seek    = stm_be_cfile_seek,
  .tell    = stm_be_cfile_tell,
  .destroy = stm_be_cfile_destroy,
};

pdf_stm_be_t *
pdf_stm_be_new_cfile (FILE         *file,
                      pdf_off_t     pos,
                      pdf_error_t **error)
{
  pdf_stm_be_cfile_t *new;

  /* Allocate a new structure */
  new = (pdf_stm_be_cfile_t *) pdf_alloc (sizeof (struct pdf_stm_be_cfile_s));
  if (!new)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "cannot create new c-file stream object: "
                     "couldn't allocate %lu bytes",
                     (unsigned long) sizeof (struct pdf_stm_be_cfile_s));
      return NULL;
    }

  /* Initialization */
  ((pdf_stm_be_t *)new)->vtable = &stm_be_vtable;
  new->file = file;
  new->pos = pos;

  return (pdf_stm_be_t *)new;
}

static void
stm_be_cfile_destroy (pdf_stm_be_t *be)
{
#warning We should take ownership of the cfile and close it here
  pdf_dealloc (be);
}

static pdf_bool_t
stm_be_ensure_correct_offset (pdf_stm_be_cfile_t *cfile_be)
{
  pdf_off_t current_pos;

  /* Get current real file offset */
  current_pos = ftello (cfile_be->file);
  if (current_pos < 0)
    {
      PDF_ASSERT_TRACE_NOT_REACHED ();
      return PDF_FALSE;
    }

  /* If needed, fix file offset */
  if (current_pos != cfile_be->pos)
    {
      if (fseeko (cfile_be->file, cfile_be->pos, SEEK_SET) < 0)
        {
          PDF_ASSERT_TRACE_NOT_REACHED ();
          return PDF_FALSE;
        }
    }
  return PDF_TRUE;
}

static pdf_ssize_t
stm_be_cfile_read  (pdf_stm_be_t  *be,
                    pdf_uchar_t   *buffer,
                    pdf_size_t     bytes,
                    pdf_error_t  **error)
{
  pdf_stm_be_cfile_t *cfile_be = (pdf_stm_be_cfile_t *)be;
  pdf_size_t read_bytes;

  /* Note: bytes is unsigned */
  if (bytes == 0)
    return (pdf_ssize_t)0;

  /* Ensure we read from the correct offset */
  if (!stm_be_ensure_correct_offset (cfile_be))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_EINVRANGE,
                     "cannot read from cfile backend: "
                     "unable to correct offset");
      return -1;
    }

  /* Read the requested number of bytes */
  read_bytes = fread (buffer,
                      1, bytes,
                      cfile_be->file);
  if (read_bytes < bytes)
    {
      /* Do not set error in EOF */
      if (ferror (cfile_be->file))
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_STM,
                         PDF_ERROR,
                         "error reading from the c-file backend");
          return -2;
        }
    }

  /* Update the position of the stream */
  cfile_be->pos += read_bytes;

  /* NOTE: Important note, we do not recover the original file offset
   * we found when entering this method. This is, using the file stream
   * backend WILL modify the real file offset. This is done so that we
   * don't waste time moving the file offset when not really needed.
   */

  return (pdf_ssize_t)read_bytes;
}

static pdf_ssize_t
stm_be_cfile_write (pdf_stm_be_t  *be,
                    pdf_uchar_t   *buffer,
                    pdf_size_t     bytes,
                    pdf_error_t  **error)
{
  pdf_stm_be_cfile_t *cfile_be = (pdf_stm_be_cfile_t *)be;
  pdf_size_t written_bytes;

  /* Note: bytes is unsigned */
  if (bytes == 0)
    return (pdf_ssize_t)0;

  /* Ensure we read from the correct offset */
  if (!stm_be_ensure_correct_offset (cfile_be))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_EINVRANGE,
                     "cannot write to cfile backend: "
                     "unable to correct offset");
      return -1;
    }

  /* Write the requested number of bytes */
  written_bytes = fwrite (buffer,
                          1, bytes,
                          cfile_be->file);
  if (written_bytes < bytes)
    {
      /* Do not set error in EOF */
      if (ferror (cfile_be->file))
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_STM,
                         PDF_ERROR,
                         "error writing in the c-file backend");
          return -2;
        }
    }

  /* Update the position of the stream */
  cfile_be->pos += written_bytes;

  /* NOTE: Important note, we do not recover the original file offset
   * we found when entering this method. This is, using the file stream
   * backend WILL modify the real file offset. This is done so that we
   * don't waste time moving the file offset when not really needed.
   */

  return (pdf_ssize_t)written_bytes;
}

static pdf_off_t
stm_be_cfile_seek (pdf_stm_be_t *be,
                   pdf_off_t     pos)
{
  pdf_stm_be_cfile_t *cfile_be = (pdf_stm_be_cfile_t *)be;
  pdf_off_t max_pos;

  /* Get last position in the cfile */
  fseek (cfile_be->file, 0, SEEK_END);
  max_pos = ftello (cfile_be->file);

  /* Check the requested position */
  if (pos > max_pos)
    pos = max_pos;
  else if (pos < 0)
    pos = 0;

  /* This fseek is not really needed, as we'll ensure the offset when
   * reading or writing, but anyway, as we already moved the offset when
   * looking for last position, this doesn't seem that bad. */
  fseeko (cfile_be->file, pos, SEEK_SET);
  cfile_be->pos = pos;

  return pos;
}

static pdf_off_t
stm_be_cfile_tell (pdf_stm_be_t *be)
{
  pdf_stm_be_cfile_t *cfile_be = (pdf_stm_be_cfile_t *)be;

  return cfile_be->pos;
}

/* End of pdf-stm-be-cfile.c */

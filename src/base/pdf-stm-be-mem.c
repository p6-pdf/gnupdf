/* -*- mode: C -*-
 *
 *       File:         pdf-stm-be-mem.c
 *       Date:         Mon Apr 18 15:01:48 2011
 *
 *       GNU PDF Library - Memory Stream backend
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
#include <pdf-stm-be-mem.h>

/* Memory backend implementation */
struct pdf_stm_be_mem_s
{
  struct pdf_stm_be_s parent;

  pdf_uchar_t *buffer;  /* Buffer contents */
  pdf_size_t size;     /* Size of the buffer in octects */
  pdf_size_t pos;      /* Current position into the buffer */
};

typedef struct pdf_stm_be_mem_s pdf_stm_be_mem_t;

static pdf_ssize_t stm_be_mem_read    (pdf_stm_be_t *be,
                                       pdf_uchar_t  *buffer,
                                       pdf_size_t    bytes,
                                       pdf_error_t **error);
static pdf_ssize_t stm_be_mem_write   (pdf_stm_be_t  *be,
                                       pdf_uchar_t   *buffer,
                                       pdf_size_t     bytes,
                                       pdf_error_t  **error);
static pdf_off_t   stm_be_mem_seek    (pdf_stm_be_t *be,
                                       pdf_off_t     pos);
static pdf_off_t   stm_be_mem_tell    (pdf_stm_be_t *be);
static void        stm_be_mem_destroy (pdf_stm_be_t *be);

/* Vtable for virtual method implementations */
static const pdf_stm_be_vtable_t stm_be_vtable = {
  .read    = stm_be_mem_read,
  .write   = stm_be_mem_write,
  .seek    = stm_be_mem_seek,
  .tell    = stm_be_mem_tell,
  .destroy = stm_be_mem_destroy,
};

pdf_stm_be_t *
pdf_stm_be_new_mem (pdf_uchar_t  *buffer,
                    pdf_size_t    size,
                    pdf_size_t    pos,
                    pdf_error_t **error)
{
  pdf_stm_be_mem_t *new;

  /* Allocate a new structure */
  new = (pdf_stm_be_mem_t *) pdf_alloc (sizeof (struct pdf_stm_be_mem_s));
  if (!new)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "cannot create new memory stream object: "
                     "couldn't allocate %lu bytes",
                     (unsigned long) sizeof (struct pdf_stm_be_mem_s));
      return NULL;
    }

  /* Initialization */
  ((pdf_stm_be_t *)new)->vtable = &stm_be_vtable;
  new->buffer = buffer;
  new->size = size;
  new->pos = pos;

  return (pdf_stm_be_t *)new;
}

static void
stm_be_mem_destroy (pdf_stm_be_t *be)
{
#warning We should take ownership of the buffer and deallocate it here
  pdf_dealloc (be);
}

static pdf_ssize_t
stm_be_mem_read (pdf_stm_be_t  *be,
                 pdf_uchar_t   *buffer,
                 pdf_size_t     bytes,
                 pdf_error_t  **error)
{
  pdf_stm_be_mem_t *mem_be = (pdf_stm_be_mem_t *)be;
  pdf_size_t free_bytes;
  pdf_size_t read_bytes;

  /* Note: bytes is unsigned */
  if (bytes == 0)
    return (pdf_ssize_t)0;

  /* How many bytes can we read from the buffer? */
  free_bytes = mem_be->size - mem_be->pos;
  read_bytes = (bytes < free_bytes ?
                bytes :
                free_bytes);

  if (read_bytes != 0)
    {
      /* Copy the data */
      memcpy (buffer,
              mem_be->buffer + mem_be->pos,
              read_bytes);

      mem_be->pos += read_bytes;
    }

  /* Ok, bye bye */
  return (pdf_ssize_t)read_bytes;
}

static pdf_ssize_t
stm_be_mem_write (pdf_stm_be_t  *be,
                  pdf_uchar_t   *buffer,
                  pdf_size_t     bytes,
                  pdf_error_t  **error)
{
  pdf_stm_be_mem_t *mem_be = (pdf_stm_be_mem_t *)be;
  pdf_size_t free_bytes;
  pdf_size_t written_bytes;

  /* Note: bytes is unsigned */
  if (bytes == 0)
    return 0;

  /* How many bytes can we write into the buffer? */
  free_bytes = mem_be->size - mem_be->pos;
  written_bytes = (bytes < free_bytes ?
                   bytes :
                   free_bytes);
  if (written_bytes != 0)
    {
      /* Copy the data */
      memcpy (mem_be->buffer + mem_be->pos,
              buffer,
              written_bytes);

      mem_be->pos += written_bytes;
    }

  /* Ok, bye bye */
  return (pdf_ssize_t)written_bytes;
}

static pdf_off_t
stm_be_mem_seek (pdf_stm_be_t *be,
                 pdf_off_t     pos)
{
  pdf_stm_be_mem_t *mem_be = (pdf_stm_be_mem_t *)be;

  /* Ensure we don't go off limits */
  if (pos >= mem_be->size)
    pos = mem_be->size - 1;
  else if (pos < 0)
    pos = 0;

  /* Move there */
  mem_be->pos = pos;

  return pos;
}

static pdf_off_t
stm_be_mem_tell (pdf_stm_be_t *be)
{
  pdf_stm_be_mem_t *mem_be = (pdf_stm_be_mem_t *)be;

  return mem_be->pos;
}

/* End of pdf-stm-be-mem.c */

/* -*- mode: C -*-
 *
 *       File:         pdf-stm.c
 *       Date:         Fri Jul  6 18:43:15 2007
 *
 *       GNU PDF Library - Streams
 *
 */

/* Copyright (C) 2007-2011 Free Software Foundation, Inc. */

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

#include <unistd.h>
#include <string.h>

#include <pdf-alloc.h>
#include <pdf-stm.h>
#include <pdf-stm-be.h>
#include <pdf-stm-be-mem.h>
#include <pdf-stm-be-cfile.h>
#include <pdf-stm-be-file.h>

/* Forward declarations */

static pdf_bool_t pdf_stm_init (pdf_stm_t            *stm,
                                pdf_size_t            cache_size,
                                enum pdf_stm_mode_e   mode,
                                pdf_error_t         **error);

static pdf_bool_t pdf_stm_read_peek_char (pdf_stm_t     *stm,
                                          pdf_uchar_t   *read_char,
                                          pdf_bool_t     peek,
                                          pdf_error_t  **error);

/*
 * Public functions
 */

pdf_stm_t *
pdf_stm_cfile_new (FILE                 *file,
                   pdf_off_t             offset,
                   pdf_size_t            cache_size,
                   enum pdf_stm_mode_e   mode,
                   pdf_error_t         **error)
{
  pdf_stm_t *stm;

  PDF_ASSERT_POINTER_RETURN_VAL (file, NULL);
  /* Note: if cache_size == 0, we'll use the default one */

  /* Allocate memory for the new stream */
  stm = pdf_alloc (sizeof (struct pdf_stm_s));
  if (!stm)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "not enough memory to create a stream: "
                     "couldn't allocate %lu bytes",
                     (unsigned long) sizeof (struct pdf_stm_s));
      return NULL;
    }

  /* Initialize a file stream */
  stm->type = PDF_STM_FILE;
  stm->backend = pdf_stm_be_new_cfile (file, offset, error);
  if (!stm->backend)
    {
      pdf_stm_destroy (stm);
      return NULL;
    }

  /* Initialize the common parts */
  if (!pdf_stm_init (stm, cache_size, mode, error))
    {
      pdf_stm_destroy (stm);
      return NULL;
    }
  return stm;
}

pdf_stm_t *
pdf_stm_file_new (pdf_fsys_file_t       file,
                  pdf_off_t             offset,
                  pdf_size_t            cache_size,
                  enum pdf_stm_mode_e   mode,
                  pdf_error_t         **error)
{
  pdf_stm_t *stm;

  PDF_ASSERT_POINTER_RETURN_VAL (file, NULL);
  /* Note: if cache_size == 0, we'll use the default one */

  /* Allocate memory for the new stream */
  stm = pdf_alloc (sizeof (struct pdf_stm_s));
  if (!stm)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "not enough memory to create a stream: "
                     "couldn't allocate %lu bytes",
                     (unsigned long) sizeof (struct pdf_stm_s));
      return NULL;
    }

  /* Initialize a file stream */
  stm->type = PDF_STM_FILE;
  stm->backend = pdf_stm_be_new_file (file, offset, error);
  if (!stm->backend)
    {
      pdf_stm_destroy (stm);
      return NULL;
    }

  /* Initialize the common parts */
  if (!pdf_stm_init (stm, cache_size, mode, error))
    {
      pdf_stm_destroy (stm);
      return NULL;
    }
  return stm;
}

pdf_stm_t *
pdf_stm_mem_new (pdf_uchar_t          *buffer,
                 pdf_size_t            size,
                 pdf_size_t            cache_size,
                 enum pdf_stm_mode_e   mode,
                 pdf_error_t         **error)
{
  pdf_stm_t *stm;

  PDF_ASSERT_POINTER_RETURN_VAL (buffer, NULL);
  PDF_ASSERT_RETURN_VAL (size > 0, NULL);
  /* Note: if cache_size == 0, we'll use the default one */

  /* Allocate memory for the new stream */
  stm = pdf_alloc (sizeof (struct pdf_stm_s));
  if (!stm)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "not enough memory to create a stream: "
                     "couldn't allocate %lu bytes",
                     (unsigned long) sizeof (struct pdf_stm_s));
      return NULL;
    }

  /* Initialize a memory stream */
  stm->type = PDF_STM_MEM;
  stm->backend = pdf_stm_be_new_mem (buffer, size, 0, error);
  if (!stm->backend)
    {
      pdf_stm_destroy (stm);
      return NULL;
    }

  /* Initialize the common parts */
  if (!pdf_stm_init (stm, cache_size, mode, error))
    {
      pdf_stm_destroy (stm);
      return NULL;
    }
  return stm;
}

void
pdf_stm_destroy (pdf_stm_t *stm)
{
  pdf_stm_filter_t *filter;
  pdf_size_t flushed_bytes = 0;

  /* Flush the cache ignoring any possible error, finishing the filters */
  if (stm->mode == PDF_STM_WRITE)
    pdf_stm_flush (stm, PDF_TRUE, &flushed_bytes, NULL);

  /* Destroy the backend */
  pdf_stm_be_destroy (stm->backend);

  /* Destroy the cache */
  pdf_buffer_destroy (stm->cache);

  /* Destroy the filter chain */
  filter = stm->filter;
  while (filter != NULL)
    {
      pdf_stm_filter_t *filter_to_delete;

      filter_to_delete = filter;
      filter = pdf_stm_filter_get_next (filter);
      pdf_stm_filter_destroy (filter_to_delete);
    }

  /* Deallocate the stm structure */
  pdf_dealloc (stm);
}

enum pdf_stm_mode_e
pdf_stm_get_mode (pdf_stm_t *stm)
{
  PDF_ASSERT_POINTER_RETURN_VAL (stm, PDF_STM_UNKNOWN);

  return stm->mode;
}

pdf_bool_t
pdf_stm_read (pdf_stm_t    *stm,
              pdf_uchar_t  *buf,
              pdf_size_t    bytes,
              pdf_size_t   *read_bytes,
              pdf_error_t **error)
{
  pdf_bool_t eof;

  PDF_ASSERT_POINTER_RETURN_VAL (stm, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (buf, PDF_FALSE);
  PDF_ASSERT_RETURN_VAL (bytes > 0, PDF_FALSE);

  *read_bytes = 0;

  /* Is this a read stream? */
  if (stm->mode != PDF_STM_READ)
    {
      /* Invalid operation */
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_EINVOP,
                     "cannot read from a write stream");
      return PDF_FALSE;
    }

  eof = PDF_FALSE;
  while (*read_bytes < bytes && !eof)
    {
      pdf_size_t pending_bytes;
      pdf_size_t to_copy_bytes;
      pdf_size_t cache_size;

      /* If the cache is empty, refill it with filtered data */
      if (pdf_buffer_eob_p (stm->cache))
        {
          pdf_error_t *inner_error = NULL;

          pdf_buffer_rewind (stm->cache);

          /* Break if apply fails */
          if (!pdf_stm_filter_apply (stm->filter,
                                     PDF_FALSE,
                                     &eof,
                                     &inner_error))
            {
              /* Report error */
              pdf_propagate_error (error, inner_error);
              return PDF_FALSE;
            }
        }

      /* Read data from the cache */
      PDF_ASSERT (stm->cache->wp >= stm->cache->rp);
      pending_bytes = bytes - *read_bytes;
      cache_size = stm->cache->wp - stm->cache->rp;
      to_copy_bytes = PDF_MIN (pending_bytes, cache_size);

      memcpy ((buf + *read_bytes),
              stm->cache->data + stm->cache->rp,
              to_copy_bytes);

      *read_bytes += to_copy_bytes;
      stm->cache->rp += to_copy_bytes;
    }

  /* Update the sequential counter */
  stm->seq_counter += *read_bytes;

  /* On EOF, we return PDF_FALSE without error */
  return ((eof && *read_bytes < bytes) ? PDF_FALSE : PDF_TRUE);
}

pdf_bool_t
pdf_stm_write (pdf_stm_t         *stm,
               const pdf_uchar_t *buf,
               pdf_size_t         bytes,
               pdf_size_t        *written_bytes,
               pdf_error_t      **error)
{
  pdf_stm_filter_t *tail_filter;
  pdf_buffer_t *tail_buffer;
  pdf_bool_t eof = PDF_FALSE;

  PDF_ASSERT_POINTER_RETURN_VAL (stm, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (buf, PDF_FALSE);
  PDF_ASSERT_RETURN_VAL (bytes > 0, PDF_FALSE);

  *written_bytes = 0;

  /* Is this a write stream? */
  if (stm->mode != PDF_STM_WRITE)
    {
      /* Invalid operation */
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_EINVOP,
                     "cannot write in a read stream");
      return PDF_FALSE;
    }

  tail_filter = pdf_stm_filter_get_tail (stm->filter);
  tail_buffer = pdf_stm_filter_get_in (tail_filter);

  while (*written_bytes < bytes)
    {
      pdf_size_t tail_buffer_size;
      pdf_size_t to_write_bytes;
      pdf_size_t pending_bytes;

      if ((pdf_buffer_full_p (tail_buffer)) &&
          (!pdf_buffer_eob_p (tail_buffer)))
        {
          pdf_size_t flushed_bytes = 0;
          pdf_error_t *inner_error = NULL;

          /* Flush the cache */
          tail_buffer_size = tail_buffer->wp - tail_buffer->rp;
          if (!pdf_stm_flush (stm,
                              PDF_FALSE,
                              &flushed_bytes,
                              &inner_error))
            {
              if (inner_error)
                {
                  /* Error flushing */
                  pdf_propagate_error (error, inner_error);
                  return PDF_FALSE;
                }

              /* flush returned PDF_FALSE without error: EOF */
              eof = PDF_TRUE;
              break;
            }

          /* No flush needed, keep on */
        }

      /* Write the data into the tail buffer. Note that at this
       * point the tail buffer should be empty */
      tail_buffer_size = tail_buffer->size - tail_buffer->wp;
      pending_bytes = bytes - *written_bytes;

      to_write_bytes = PDF_MIN (pending_bytes, tail_buffer_size);

      if (to_write_bytes != 0)
        {
          memcpy (tail_buffer->data + tail_buffer->wp,
                  buf + *written_bytes,
                  to_write_bytes);

          *written_bytes += to_write_bytes;
          tail_buffer->wp += to_write_bytes;
        }
    }

  /* Update the sequential counter */
  stm->seq_counter += *written_bytes;

  return ((eof && *written_bytes < bytes) ? PDF_FALSE : PDF_TRUE);
}

pdf_bool_t
pdf_stm_flush (pdf_stm_t    *stm,
               pdf_bool_t    finish,
               pdf_size_t   *flushed_bytes,
               pdf_error_t **error)
{
  pdf_stm_filter_t *tail_filter;
  pdf_buffer_t *tail_buffer;
  pdf_error_t *inner_error = NULL;
  pdf_bool_t eof = PDF_FALSE;

  /* NOTE: NULL flushed_bytes is ALLOWED */

  PDF_ASSERT_POINTER_RETURN_VAL (stm, -1);

  /* Is this a write stream? */
  if (stm->mode != PDF_STM_WRITE)
    {
      /* Invalid operation */
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_EINVOP,
                     "cannot write in a read stream");
      return PDF_FALSE;
    }

  /* Apply the head filter until the filter chain gets empty */
  tail_filter = pdf_stm_filter_get_tail (stm->filter);
  tail_buffer = pdf_stm_filter_get_in (tail_filter);

  if (flushed_bytes)
    *flushed_bytes = 0;

  while (PDF_TRUE)
    {
      pdf_size_t cache_size;
      pdf_ssize_t written_bytes;
      pdf_size_t tail_size;

      tail_size = tail_buffer->wp - tail_buffer->rp;

      /* Break only on error */
      if (!pdf_stm_filter_apply (stm->filter, finish, &eof, &inner_error))
        break;

      /* Update the number of flushed bytes */
      if (flushed_bytes)
        *flushed_bytes += tail_size - (tail_buffer->wp - tail_buffer->rp);

      /* Avoid false EEOF error */
      if (eof && pdf_buffer_eob_p (stm->cache))
        {
          eof = PDF_FALSE;
          pdf_buffer_rewind (tail_buffer);
          break;
        }

      /* Write the data from the buffer cache into the backend */
      cache_size = stm->cache->wp - stm->cache->rp;
      written_bytes = pdf_stm_be_write (stm->backend,
                                        stm->cache->data + stm->cache->rp,
                                        cache_size,
                                        &inner_error);
      if (written_bytes < 0)
        break;

      if (written_bytes != cache_size)
        {
          /* EOF, could not write all the contents of the cache buffer into
           * the backend */
          stm->cache->rp += written_bytes;
          eof = PDF_TRUE;
          break;
        }

      /* Rewind the cache */
      pdf_buffer_rewind (stm->cache);
    }

  /* Propagate error */
  if (inner_error)
    {
      pdf_propagate_error (error, inner_error);
      return PDF_FALSE;
    }

  return (eof ? PDF_FALSE : PDF_TRUE);;
}

pdf_bool_t
pdf_stm_supported_filter_p (enum pdf_stm_filter_type_e filter_type)
{
  PDF_ASSERT_RETURN_VAL (filter_type >= PDF_STM_FILTER_NULL, PDF_FALSE);
  PDF_ASSERT_RETURN_VAL (filter_type < PDF_STM_FILTER_LAST, PDF_FALSE);

  return pdf_stm_filter_p (filter_type);
}

pdf_bool_t
pdf_stm_install_filter (pdf_stm_t                   *stm,
                        enum pdf_stm_filter_type_e   filter_type,
                        const pdf_hash_t            *filter_params,
                        pdf_error_t                **error)
{
  pdf_stm_filter_t *filter;
  enum pdf_stm_filter_mode_e filter_mode;

  PDF_ASSERT_POINTER_RETURN_VAL (stm, PDF_FALSE);

  filter_mode = (stm->mode == PDF_STM_READ ?
                 PDF_STM_FILTER_MODE_READ :
                 PDF_STM_FILTER_MODE_WRITE);

  /* Create the new filter. Note that filter_params is passed directly to the
   * filter implementation creator, and that it may well be NULL. */
  filter = pdf_stm_filter_new (filter_type,
                               filter_params,
                               stm->cache->size,
                               filter_mode,
                               error);
  if (!filter)
    return PDF_FALSE;

  /* Set the new filter as the new head of the filter chain */
  pdf_stm_filter_set_next (filter, stm->filter);
  pdf_stm_filter_set_out (filter, stm->cache);
  pdf_stm_filter_set_out (stm->filter, pdf_stm_filter_get_in (filter));
  stm->filter = filter;

  return PDF_TRUE;
}

pdf_bool_t
pdf_stm_read_char (pdf_stm_t    *stm,
                   pdf_uchar_t  *read_char,
                   pdf_error_t **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (stm, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (read_char, PDF_FALSE);

  return pdf_stm_read_peek_char (stm, read_char, PDF_FALSE, error);
}

pdf_bool_t
pdf_stm_peek_char (pdf_stm_t    *stm,
                   pdf_uchar_t  *read_char,
                   pdf_error_t **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (stm, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (read_char, PDF_FALSE);

  return pdf_stm_read_peek_char (stm, read_char, PDF_TRUE, error);
}

pdf_off_t
pdf_stm_bseek (pdf_stm_t *stm,
               pdf_off_t  pos)
{
  pdf_off_t cur_pos;
  pdf_off_t new_pos;

  PDF_ASSERT_POINTER_RETURN_VAL (stm, (pdf_off_t)-1);

  cur_pos = pdf_stm_tell (stm);

  if (stm->mode == PDF_STM_READ)
    {
      /* Discard the cache contents */
      pdf_buffer_rewind (stm->cache);

      /* Seek the backend */
      new_pos = pdf_stm_be_seek (stm->backend, pos);
    }
  else /* Writing stream */
    {
      pdf_stm_filter_t *tail_filter;
      pdf_buffer_t *tail_buffer;

      tail_filter = pdf_stm_filter_get_tail (stm->filter);
      tail_buffer = pdf_stm_filter_get_in (tail_filter);

      if (!pdf_buffer_eob_p (tail_buffer))
        {
          pdf_error_t *inner_error = NULL;
          pdf_size_t flushed_bytes = 0;

          /* Flush the stream */
          if (!pdf_stm_flush (stm, PDF_FALSE, &flushed_bytes, &inner_error) &&
              inner_error)
            {
              /* Error flushing the stream: return the current position */
              pdf_error_destroy (inner_error);
              return cur_pos;
            }
        }

      /* Note that if there is an EOF condition in the backend we are
       * going to loose data */
      pdf_buffer_rewind (tail_buffer);

      /* Seek the backend */
      new_pos = pdf_stm_be_seek (stm->backend, pos);
    }

  /* Reset the sequential counter */
  stm->seq_counter = 0;

  return new_pos;
}

pdf_off_t
pdf_stm_btell (pdf_stm_t *stm)
{
  pdf_off_t pos;
  pdf_size_t cache_size;

  PDF_ASSERT_POINTER_RETURN_VAL (stm, (pdf_off_t)-1);

  if (stm->mode == PDF_STM_READ)
    {
      cache_size = stm->cache->wp - stm->cache->rp;
      pos = pdf_stm_be_tell (stm->backend) - cache_size;
    }
  else /* Writing stream */
    {
      pdf_stm_filter_t *tail_filter;
      pdf_buffer_t *tail_buffer;

      tail_filter = pdf_stm_filter_get_tail (stm->filter);
      tail_buffer = pdf_stm_filter_get_in (tail_filter);

      cache_size = tail_buffer->wp - tail_buffer->rp;
      pos = pdf_stm_be_tell (stm->backend) - cache_size;
    }

  return pos;
}

pdf_off_t
pdf_stm_tell (pdf_stm_t *stm)
{
  PDF_ASSERT_POINTER_RETURN_VAL (stm, (pdf_off_t)-1);

  return stm->seq_counter;
}

/*
 * Private functions
 */

static pdf_bool_t
pdf_stm_init (pdf_stm_t            *stm,
              pdf_size_t            cache_size,
              enum pdf_stm_mode_e   mode,
              pdf_error_t         **error)
{
  /* Use the default cache size */
  if (cache_size == 0)
    cache_size = PDF_STM_DEFAULT_CACHE_SIZE;

  /* The sequential counter is initially 0 */
  stm->seq_counter = 0;

  stm->filter = pdf_stm_filter_new (PDF_STM_FILTER_NULL,
                                    NULL, /* No filter params needed */
                                    cache_size,
                                    (stm->mode == PDF_STM_READ ?
                                     PDF_STM_FILTER_MODE_READ :
                                     PDF_STM_FILTER_MODE_WRITE),
                                    error);
  if (!stm->filter)
    return PDF_FALSE;

  /* Initialize the filter cache */
  stm->cache = pdf_buffer_new (cache_size, error);
  if (!stm->cache)
    return PDF_FALSE;

  /* Configure the filter */
  stm->mode = mode;
  if (stm->mode == PDF_STM_READ)
    {
      /* Configuration for a reading stream
       *
       *  <cache> <--- <null-filter> <--- <backend>
       */
      pdf_stm_filter_set_out (stm->filter, stm->cache);
      pdf_stm_filter_set_be (stm->filter, stm->backend);
    }
  else
    {
      /* Configuration for a writing stream
       *
       * <null-filter> --> <cache> --> <backend>
       */
      pdf_stm_filter_set_out (stm->filter, stm->cache);
    }

  return PDF_TRUE;
}

static pdf_bool_t
pdf_stm_read_peek_char (pdf_stm_t    *stm,
                        pdf_uchar_t  *read_char,
                        pdf_bool_t    peek,
                        pdf_error_t **error)
{
  /* Is this a read stream? */
  if (stm->mode != PDF_STM_READ)
    {
      /* Invalid operation */
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_EINVOP,
                     "cannot read from a write stream");
      return PDF_FALSE;
    }

  /* Is the cache empty? */
  if (pdf_buffer_eob_p (stm->cache))
    {
      pdf_bool_t eof = PDF_FALSE;
      pdf_error_t *inner_error = NULL;

      pdf_buffer_rewind (stm->cache);
      if (!pdf_stm_filter_apply (stm->filter,
                                 PDF_FALSE,
                                 &eof,
                                 &inner_error))
        {
          pdf_propagate_error (error, inner_error);
          return PDF_FALSE;
        }
    }

  /* If still empty, EOF */
  if (pdf_buffer_eob_p (stm->cache))
    return PDF_FALSE;

  /* Read a character from the cache */
  *read_char = (pdf_u32_t) stm->cache->data[stm->cache->rp];

  if (!peek)
    {
      stm->cache->rp++;
      stm->seq_counter++;
    }

  return PDF_TRUE;
}

/* End of pdf_stm.c */

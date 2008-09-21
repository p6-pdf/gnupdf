/* -*- mode: C -*- Time-stamp: "08/09/21 22:05:58 jemarch"
 *
 *       File:         pdf-stm.c
 *       Date:         Fri Jul  6 18:43:15 2007
 *
 *       GNU PDF Library - Streams
 *
 */

/* Copyright (C) 2007, 2008 Free Software Foundation, Inc. */

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

/* Forward declarations */

static pdf_status_t pdf_stm_init (pdf_size_t buffer_size,
                                  enum pdf_stm_mode_e mode,
                                  pdf_stm_t stm);
static inline pdf_stm_t pdf_stm_alloc (void);
static inline void pdf_stm_dealloc (pdf_stm_t stm);
static pdf_u32_t pdf_stm_read_peek_char (pdf_stm_t stm, pdf_bool_t peek_p);

/*
 * Public functions
 */

pdf_status_t
pdf_stm_file_new (pdf_fsys_file_t file,
                  pdf_off_t offset,
                  pdf_size_t cache_size,
                  enum pdf_stm_mode_e mode,
                  pdf_stm_t *stm)
{
  /* Allocate memory for the new stream */
  *stm = pdf_stm_alloc ();

  /* Initialize a file stream */
  (*stm)->type = PDF_STM_FILE;
  (*stm)->backend = pdf_stm_be_new_file (file,
                                         offset);
  pdf_stm_filter_set_be ((*stm)->filter,
                         (*stm)->backend);

  /* Initialize the common parts */
  return pdf_stm_init (cache_size,
                       mode,
                       *stm);
}

pdf_status_t
pdf_stm_mem_new (pdf_char_t *buffer,
                 pdf_size_t size,
                 pdf_size_t cache_size,
                 enum pdf_stm_mode_e mode,
                 pdf_stm_t *stm)
{
  /* Allocate memory for the new stream */
  *stm = pdf_stm_alloc ();

  /* Initialize a memory stream */
  (*stm)->type = PDF_STM_MEM;
  (*stm)->backend = pdf_stm_be_new_mem (buffer,
                                        size,
                                        0);
  
  /* Initialize the common parts */
  return pdf_stm_init (cache_size,
                       mode,
                       *stm);
}

pdf_status_t
pdf_stm_destroy (pdf_stm_t stm)
{
  pdf_stm_filter_t filter;
  pdf_stm_filter_t filter_to_delete;

  if (stm->mode == PDF_STM_WRITE)
    {
      /* Flush the cache */
      pdf_stm_flush (stm);

      /* Finish the filters */
      pdf_stm_finish (stm);
    }

  /* Destroy the backend */
  pdf_stm_be_destroy (stm->backend);

  /* Destroy the cache */
  pdf_stm_buffer_destroy (stm->cache);

  /* Destroy the filter chain */
  filter = stm->filter;
  while (filter != NULL)
    {
      filter_to_delete = filter;
      filter = filter->next;
      pdf_stm_filter_destroy (filter_to_delete);
    }

  /* Deallocate the stm structure */
  pdf_stm_dealloc (stm);

  return PDF_OK;
}

pdf_size_t
pdf_stm_read (pdf_stm_t stm,
              pdf_char_t *buf,
              pdf_size_t bytes)
{
  pdf_size_t read_bytes;
  pdf_size_t pending_bytes;
  pdf_size_t cache_size;
  pdf_size_t to_copy_bytes;
  pdf_status_t ret;

  if (stm->mode != PDF_STM_READ)
    {
      /* Invalid operation */
      return 0;
    }

  ret = PDF_OK;
  read_bytes = 0;
  while ((read_bytes < bytes) &&
         (ret == PDF_OK))
    {
      pending_bytes = bytes - read_bytes;

      /* If the cache is empty, refill it with filtered data */
      if (pdf_stm_buffer_eob_p (stm->cache))
        {
          ret = pdf_stm_filter_apply (stm->filter, PDF_FALSE);
        }

      if (ret != PDF_ERROR)
        {
          /* Read data from the cache */
          pending_bytes = bytes - read_bytes;
          cache_size = stm->cache->wp - stm->cache->rp;
          to_copy_bytes = PDF_MIN(pending_bytes, cache_size);

          strncpy ((char *) (buf + read_bytes),
                   (char *) stm->cache->data + stm->cache->rp,
                   to_copy_bytes);
          
          read_bytes += to_copy_bytes;
          stm->cache->rp += to_copy_bytes;
        }
    }

  if (ret == PDF_ERROR)
    {
      read_bytes = 0;
    }

  return read_bytes;
}


pdf_size_t
pdf_stm_write (pdf_stm_t stm,
               pdf_char_t *buf,
               pdf_size_t bytes)
{
  pdf_status_t ret;
  pdf_size_t written_bytes;
  pdf_size_t pending_bytes;
  pdf_size_t to_write_bytes;
  pdf_stm_filter_t tail_filter;
  pdf_stm_buffer_t tail_buffer;
  pdf_size_t tail_buffer_size;

  if (stm->mode != PDF_STM_WRITE)
    {
      /* Invalid operation */
      return 0;
    }

  tail_filter = pdf_stm_filter_get_tail (stm->filter);
  tail_buffer = pdf_stm_filter_get_in (tail_filter);

  ret = PDF_OK;
  written_bytes = 0;
  while ((written_bytes < bytes) &&
         (ret == PDF_OK))
    {
      if (pdf_stm_buffer_full_p (tail_buffer))
        {
          /* Flush the cache */
          tail_buffer_size = tail_buffer->wp - tail_buffer->rp;
          if (pdf_stm_flush (stm) < tail_buffer_size)
            {
              ret = PDF_EEOF;
            }
        }

      if (ret == PDF_OK)
        {
          /* Write the data into the tail buffer. Note that at this
             point the tail buffer should be empty */
          tail_buffer_size = tail_buffer->size - tail_buffer->wp;
          pending_bytes = bytes - written_bytes;

          to_write_bytes = PDF_MIN(pending_bytes, tail_buffer_size);

          if (to_write_bytes != 0)
            {
              strncpy ((char *) tail_buffer->data + tail_buffer->wp,
                       (char *) buf + written_bytes,
                       to_write_bytes);

              written_bytes += to_write_bytes;
              tail_buffer->wp += to_write_bytes;
            }
        }
    }
  
  return written_bytes;
}

pdf_size_t
pdf_stm_flush (pdf_stm_t stm)
{
  pdf_stm_filter_t tail_filter;
  pdf_stm_buffer_t tail_buffer;
  pdf_status_t ret;
  pdf_size_t cache_size;
  pdf_size_t written_bytes;
  pdf_size_t tail_buffer_size;

  if (stm->mode != PDF_STM_WRITE)
    {
      /* Invalid operation */
      return 0;
    }

  tail_filter = pdf_stm_filter_get_tail (stm->filter);
  tail_buffer = pdf_stm_filter_get_in (tail_filter);
  tail_buffer_size = tail_buffer->wp - tail_buffer->rp;

  /* Apply the head filter until the input buffer of the
     tail filter gets empty */
  ret = PDF_OK;
  while ((!pdf_stm_buffer_eob_p (tail_buffer)) &&
         (ret == PDF_OK))
    {
      ret = pdf_stm_filter_apply (stm->filter, PDF_FALSE);
      
      if (ret != PDF_ERROR)
        {
          /* Write the data from the buffer cache into the backend */
          cache_size = stm->cache->wp - stm->cache->rp;
          written_bytes = pdf_stm_be_write (stm->backend,
                                            stm->cache->data + stm->cache->rp,  
                                            cache_size);

          if (written_bytes < cache_size)
            {
              /* There is no room in the backend */
              ret = PDF_EEOF;
            }
        }
    }

  if (pdf_stm_buffer_eob_p (tail_buffer))
    {
      pdf_stm_buffer_rewind (tail_buffer);
    }

  return (tail_buffer_size - 
          (tail_buffer->wp - tail_buffer->rp));
}

pdf_status_t
pdf_stm_install_filter (pdf_stm_t stm,
                        enum pdf_stm_filter_type_e filter_type,
                        pdf_hash_t filter_params)
{
  pdf_stm_filter_t filter;

  /* Create the new filter */
  filter = pdf_stm_filter_new (filter_type,
                               filter_params,
                               stm->cache->size);

  /* Set the new filter as the new head of the filter chain */
  pdf_stm_filter_set_next (filter, stm->filter);
  pdf_stm_filter_set_out (filter, stm->cache);
  pdf_stm_filter_set_out (stm->filter, pdf_stm_filter_get_in (filter));
  stm->filter = filter;

  return PDF_OK;
}

pdf_u32_t
pdf_stm_read_char (pdf_stm_t stm)
{
  return pdf_stm_read_peek_char (stm, PDF_FALSE);
}

pdf_u32_t
pdf_stm_peek_char (pdf_stm_t stm)
{
  return pdf_stm_read_peek_char (stm, PDF_TRUE);
}

pdf_off_t
pdf_stm_seek (pdf_stm_t stm,
              pdf_off_t pos)
{
  pdf_off_t new_pos;
  pdf_stm_filter_t tail_filter;
  pdf_stm_buffer_t tail_buffer;

  if (stm->mode == PDF_STM_READ)
    {
      /* Discard the cache contents */
      pdf_stm_buffer_rewind (stm->cache);

      /* Seek the backend */
      new_pos = pdf_stm_be_seek (stm->backend, pos);
    }
  else
    {
      /* Writing stream */
      /* Flush the stream */
      tail_filter = pdf_stm_filter_get_tail (stm->filter);
      tail_buffer = pdf_stm_filter_get_in (tail_filter);

      pdf_stm_flush (stm);

      /* Note that if there is an EOF condition in the backend we are
         going to lose data */
      pdf_stm_buffer_rewind (tail_buffer);

      /* Seek the backend */
      new_pos = pdf_stm_be_seek (stm->backend, pos);
    }

  return new_pos;
}

pdf_off_t
pdf_stm_tell (pdf_stm_t stm)
{
  pdf_off_t pos;
  pdf_size_t cache_size;
  pdf_stm_filter_t tail_filter;
  pdf_stm_buffer_t tail_buffer;

  if (stm->mode == PDF_STM_READ)
    {
      cache_size = stm->cache->wp - stm->cache->rp;
      pos = pdf_stm_be_tell (stm->backend) + cache_size;
    }
  else
    {
      /* Writing stream */

      tail_filter = pdf_stm_filter_get_tail (stm->filter);
      tail_buffer = pdf_stm_filter_get_in (tail_filter);

      cache_size = tail_buffer->wp - tail_buffer->rp;
      pos = pdf_stm_be_tell (stm->backend) + cache_size;
    }

  return pos;
}

pdf_size_t
pdf_stm_finish (pdf_stm_t stm)
{
  pdf_stm_filter_t tail_filter;
  pdf_stm_buffer_t tail_buffer;
  pdf_status_t ret;
  pdf_size_t written_bytes;
  pdf_size_t cache_size;

  if (stm->mode != PDF_STM_WRITE)
    {
      /* Invalid operation */
      return 0;
    }

  written_bytes = 0;
  tail_filter = pdf_stm_filter_get_tail (stm->filter);
  tail_buffer = pdf_stm_filter_get_in (tail_filter);

  /* Rewind the tail buffer */
  pdf_stm_buffer_rewind (tail_buffer);
  
  /* Finish the filters */
  ret = pdf_stm_filter_apply (stm->filter, PDF_TRUE);
      
  if (ret != PDF_ERROR)
    {
      /* Write the data from the buffer cache into the backend. 
         
         Note that we have a design flaw here: the data generated by
         the finalization of the filters should not exceed the size of
         the stream cache. */
      cache_size = stm->cache->wp - stm->cache->rp;
      written_bytes = pdf_stm_be_write (stm->backend,
                                        stm->cache->data + stm->cache->rp,  
                                        cache_size);
    }

  return written_bytes;
}

/*
 * Private functions
 */

static pdf_status_t
pdf_stm_init (pdf_size_t cache_size,
              enum pdf_stm_mode_e mode,
              pdf_stm_t stm)
{
  pdf_hash_t null_filter_params;
  pdf_hash_t null_filter_state;

  if (cache_size == 0)
    {
      /* Use the default cache size */
      cache_size = PDF_STM_DEFAULT_CACHE_SIZE;
    } 

  /* Initialize the null filter */
  pdf_hash_new (NULL, &null_filter_params);
  pdf_hash_new (NULL, &null_filter_state);
  stm->filter = pdf_stm_filter_new (PDF_STM_FILTER_NULL,
                                    null_filter_params,
                                    cache_size);

  /* Initialize the filter cache */
  stm->cache = pdf_stm_buffer_new (cache_size);

  /* Configure the filter */
  stm->mode = mode;
  if (stm->mode == PDF_STM_READ)
    {
      /* Configuration for a reading stream
       *
       *  <cache> <--- <null-filter> <--- <backend>
       */

      pdf_stm_filter_set_out (stm->filter, 
                              stm->cache);
      pdf_stm_filter_set_be (stm->filter,
                             stm->backend);
    } 
  else
    {
      /* Configuration for a writing stream
       *
       * <null-filter> --> <cache> --> <backend>
       */

      pdf_stm_filter_set_out (stm->filter,
                              stm->cache);
    }

  return PDF_OK;
}


static pdf_u32_t
pdf_stm_read_peek_char (pdf_stm_t stm,
                        pdf_bool_t peek_p)
{
  pdf_status_t ret;
  pdf_u32_t ret_char;

  /* Is the cache empty? */
  ret = PDF_OK;
  if (pdf_stm_buffer_eob_p (stm->cache))
    {
      ret = pdf_stm_filter_apply (stm->filter, PDF_FALSE);
    }

  if (pdf_stm_buffer_eob_p (stm->cache))
    {
      ret_char = PDF_EOF;
    }
  else
    {
      /* Read a character from the cache */
      ret_char = 
        (pdf_u32_t) stm->cache->data[stm->cache->rp];

      if (!peek_p)
        {
          stm->cache->rp++;
        }
    }
  
  return ret_char;
}

static inline pdf_stm_t
pdf_stm_alloc (void)
{
  pdf_stm_t new;

  new =  pdf_alloc (sizeof(struct pdf_stm_s));
  return new;
}

static inline void
pdf_stm_dealloc (pdf_stm_t stm)
{
  pdf_dealloc (stm);
}

/* End of pdf_stm.c */

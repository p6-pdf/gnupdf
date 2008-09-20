/* -*- mode: C -*- Time-stamp: "08/09/20 18:06:30 jemarch"
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

  ret = PDF_OK;
  read_bytes = 0;
  while ((read_bytes < bytes) &&
         (ret == PDF_OK))
    {
      pending_bytes = bytes - read_bytes;

      /* If the cache is empty, refill it with filtered data */
      if (pdf_stm_buffer_eob_p (stm->cache))
        {
          ret = pdf_stm_filter_apply (stm->filter);
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
  pdf_bool_t eof_p;
  pdf_size_t written_bytes;
  pdf_size_t pending_bytes;
  pdf_stm_buffer_t tail_in;
  pdf_stm_filter_t tail;

  /* Get the input buffer of the tail filter */
  tail = pdf_stm_filter_get_tail (stm->filter);
  tail_in = pdf_stm_filter_get_in (tail);

  eof_p = PDF_FALSE;
  written_bytes = 0;
  while ((written_bytes < bytes) 
         && (! eof_p))
    {
      pending_bytes = bytes - written_bytes;

      /* If the input buffer of the tail filter is full, apply the
         head */
      if (pdf_stm_buffer_full_p (tail_in))
        {
          while (!pdf_stm_buffer_eob_p (tail_in))
            {
              if (pdf_stm_filter_apply (stm->filter) == 0)
                {
                  /* EOF condition */
                  eof_p = PDF_TRUE;
                }
              else
                {
                  /* Write the cache contents into the backend */
                  if (pdf_stm_be_write (stm->backend,
                                        stm->cache->data + stm->cache->rp,
                                        stm->cache->wp - stm->cache->rp)
                      < stm->cache->wp - stm->cache->rp)
                    {
                      /* EOF condition */
                      eof_p = PDF_TRUE;
                    }
                }
            }
        }

      /* In this point the input buffer of the tail filter should be
         empty. Copy data into that buffer */
      strncpy ((char *) tail_in->data,
               (char *) buf + written_bytes,
               PDF_MIN(pending_bytes, stm->cache->size));
               
      written_bytes += PDF_MIN(pending_bytes, stm->cache->size);
    }

  return written_bytes;
}

pdf_size_t
pdf_stm_flush (pdf_stm_t stm)
{
  /* FIXME: TODO */

  return 0;
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
      ret = pdf_stm_filter_apply (stm->filter);
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

/* -*- mode: C -*- Time-stamp: "08/07/31 00:09:42 jemarch"
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

#include <unistd.h>

#include <pdf-alloc.h>
#include <pdf-stm.h>

/* Forward declarations */

static pdf_status_t pdf_stm_init (pdf_size_t buffer_size,
                                  enum pdf_stm_mode_e mode,
                                  pdf_stm_t stm);
static inline pdf_stm_t pdf_stm_alloc (void);

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
  pdf_hash_create (NULL, NULL, &null_filter_params);
  pdf_hash_create (NULL, NULL, &null_filter_state);
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

static inline pdf_stm_t
pdf_stm_alloc (void)
{
  pdf_stm_t new;

  new =  pdf_alloc (sizeof(struct pdf_stm_s));
  return new;
}

/* End of pdf_stm.c */

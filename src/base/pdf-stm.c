/* -*- mode: C -*- Time-stamp: "08/07/29 01:07:05 jemarch"
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

/*
 * Public functions
 */

pdf_status_t
pdf_stm_file_new (pdf_fsys_file_t file,
                  pdf_off_t offset,
                  pdf_size_t buffer_size,
                  enum pdf_stm_mode_e mode,
                  pdf_stm_t *stm)
{
  pdf_stm_t new;
  pdf_hash_t null_filter_params;
  pdf_hash_t null_filter_state;

  /* Allocate memory for the new stream */
  new = pdf_alloc (sizeof(struct pdf_stm_s));
  
  /* Initialize the stream attributes */
  new->type = PDF_STM_FILE;
  new->mode = mode;

  /* Initialize the null filter */
  pdf_hash_create (NULL, NULL, &null_filter_params);
  pdf_hash_create (NULL, NULL, &null_filter_state);
  new->filter = pdf_stm_filter_new (PDF_STM_FILTER_NULL,
                                    null_filter_params,
                                    buffer_size);

  /* Initialize the filter backend */
  new->backend = pdf_stm_be_new_file (file,
                                      offset);
  pdf_stm_filter_set_be (new->filter,
                         new->backend);

  /* Initialize the filter cache */
  new->cache = pdf_stm_buffer_new (buffer_size);

  /* Set the result */
  *stm = new;

  return PDF_OK;
}




/* End of pdf_stm.c */

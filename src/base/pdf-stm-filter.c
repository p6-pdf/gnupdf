/* -*- mode: C -*- Time-stamp: "08/07/28 23:10:16 jemarch"
 *
 *       File:         pdf-stm-filter.c
 *       Date:         Thu Jun 12 22:13:31 2008
 *
 *       GNU PDF Library - Stream Filter
 *
 */

/* Copyright (C) 2008 Free Software Foundation, Inc. */

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
#include <pdf-stm-filter.h>

/* Forward references */
static pdf_size_t pdf_stm_filter_get_input (pdf_stm_filter_t filter);

/*
 * Public functions
 */

pdf_stm_filter_t
pdf_stm_filter_new (enum pdf_stm_filter_type_e type,
                    pdf_hash_t params,
                    pdf_size_t buffer_size)
{
  pdf_stm_filter_t new;

  /* Allocate the filter structure */
  new = (pdf_stm_filter_t) 
    pdf_alloc (sizeof(struct pdf_stm_filter_s));

  /* Initialisation */
  new->type = type;

  /* Data sources */
  new->next = NULL;
  new->backend = NULL;

  /* Input buffer */
  new->in = pdf_stm_buffer_new (buffer_size);
  new->in_begin = 0;
  new->in_end = 0;

  /* Output buffer */
  new->out = NULL;

  /* Install the appropriate implementation */
  switch (new->type)
    {
    case PDF_STM_FILTER_NULL:
      {
        new->impl.init_fn = pdf_stm_f_null_init;
        new->impl.apply_fn = pdf_stm_f_null_apply;
        break;
      }
    default:
      {
        /* Shall not be reached, but makes the compiler happy */
        break;
      }
    }

  /* Initialization of the implementation */
  new->params = params;
  pdf_hash_create (NULL, NULL,
                   &(new->state));
  new->impl.init_fn (new->params,
                     new->state);

  return new;
}

pdf_status_t
pdf_stm_filter_destroy (pdf_stm_filter_t filter)
{
  pdf_stm_buffer_destroy (filter->in);
  pdf_hash_destroy (&filter->state);
  pdf_dealloc (filter);

  /* Note that the memory used by the output buffer and by the params
     hash is NOT managed by the filter */

  return PDF_OK;
}

inline pdf_status_t
pdf_stm_filter_set_next (pdf_stm_filter_t filter,
                         pdf_stm_filter_t next_filter)
{
  filter->next = next_filter;
  return PDF_OK;
}

inline pdf_status_t
pdf_stm_filter_set_be (pdf_stm_filter_t filter,
                       pdf_stm_be_t be)
{
  filter->backend = be;
  return PDF_OK;
}

inline pdf_status_t
pdf_stm_filter_set_out (pdf_stm_filter_t filter,
                        pdf_stm_buffer_t buffer)
{
  filter->out = buffer;
  return PDF_OK;
}

pdf_size_t
pdf_stm_filter_apply (pdf_stm_filter_t filter)
{
  pdf_status_t ret;
  pdf_size_t bytes_read;
  pdf_size_t bytes_produced;
  pdf_size_t impl_bytes_consumed;
  pdf_size_t impl_bytes_produced;
  struct pdf_stm_buffer_s buffer_in;
  struct pdf_stm_buffer_s buffer_out;

  /* Note that at this point the output buffer should be empty => any
     previous contents of the buffer can be safely overwritten */

  bytes_read = 0;
  bytes_produced = 0;

  while (bytes_produced < filter->out->size)
    {
      if (filter->in_begin == filter->in_end)
        {
          /* Fill the input buffer */
          bytes_read = pdf_stm_filter_get_input (filter);
          if (bytes_read == 0)
            {
              /* Error. The filter is in an invalid state now */
              return 0;
            }

          filter->in_begin = 0;
          filter->in_end = bytes_read;
        }

      /* Set buffers for the filter implementation */
      buffer_in.data = filter->in->data + filter->in_begin;
      buffer_in.size = filter->in_end - filter->in_begin;
      buffer_out.data = filter->out->data;
      buffer_out.size = filter->out->size;

      /* Generate output */
      ret = filter->impl.apply_fn (filter->params,
                                   filter->state,
                                   &buffer_in,
                                   &buffer_out,
                                   &impl_bytes_consumed,
                                   &impl_bytes_produced);
      if (ret != PDF_OK)
        {
          /* Error. The filter is in an invalid state now */
          return 0;
        }
      else
        {
          bytes_produced = bytes_produced + impl_bytes_produced;
          filter->in_begin = filter->in_begin + impl_bytes_consumed;
        }
    }

  return bytes_produced;
}

pdf_status_t
pdf_stm_filter_reset (pdf_stm_filter_t filter)
{
  return filter->impl.init_fn (filter->params,
                               filter->state);
}

/*
 * Private functions
 */

static pdf_size_t
pdf_stm_filter_get_input (pdf_stm_filter_t filter)
{
  pdf_size_t bytes_read;

  bytes_read = 0;
  if (filter->next != NULL)
    {
      bytes_read = pdf_stm_filter_apply (filter->next);
    }
  else if (filter->backend != NULL)
    {
      bytes_read = pdf_stm_be_read (filter->backend,
                                    filter->in->data,
                                    filter->in->size);
    }
  
  return bytes_read;
}

/* End of pdf-stm-filter.c */

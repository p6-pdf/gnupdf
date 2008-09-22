/* -*- mode: C -*- Time-stamp: "08/09/22 20:17:16 jemarch"
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
static pdf_size_t pdf_stm_filter_get_input (pdf_stm_filter_t filter,
                                            pdf_bool_t finish_p);


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
  new->state = NULL;
  new->impl.init_fn (new->params,
                     &(new->state));

  return new;
}

pdf_status_t
pdf_stm_filter_destroy (pdf_stm_filter_t filter)
{
  pdf_stm_buffer_destroy (filter->in);
  pdf_dealloc (filter->state);
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

inline pdf_stm_buffer_t
pdf_stm_filter_get_in (pdf_stm_filter_t filter)
{
  return filter->in;
}

pdf_status_t
pdf_stm_filter_apply (pdf_stm_filter_t filter,
                      pdf_bool_t finish_p)
{
  pdf_status_t ret;

  pdf_stm_buffer_rewind (filter->out);
  ret = PDF_OK;

  while ((!pdf_stm_buffer_full_p (filter->out)) 
         && (ret == PDF_OK))
    {
      /* If the input buffer is empty, refill it */
      if (pdf_stm_buffer_eob_p (filter->in))
        {
          ret = pdf_stm_filter_get_input (filter, finish_p);
        }

      if (ret != PDF_ERROR)
        {
          /* Generate output */
          ret = filter->impl.apply_fn (filter->params,
                                       filter->state,
                                       filter->in,
                                       filter->out,
                                       finish_p);
        }
    }

  return ret;
}

pdf_status_t
pdf_stm_filter_reset (pdf_stm_filter_t filter)
{
  return filter->impl.init_fn (filter->params,
                               &(filter->state));
}

pdf_stm_filter_t
pdf_stm_filter_get_tail (pdf_stm_filter_t filter)
{
  if (filter->next == NULL)
    {
      return filter;
    }
  else
    {
      return pdf_stm_filter_get_tail (filter->next);
    }
}

/*
 * Private functions
 */

static pdf_size_t
pdf_stm_filter_get_input (pdf_stm_filter_t filter,
                          pdf_bool_t finish_p)
{
  pdf_status_t ret;
  pdf_size_t read_bytes;

  pdf_stm_buffer_rewind (filter->in);

  if (filter->next != NULL)
    {
      ret = pdf_stm_filter_apply (filter->next, finish_p);
    }
  else if (filter->backend != NULL)
    {
      read_bytes = pdf_stm_be_read (filter->backend,
                                    filter->in->data,
                                    filter->in->size);
      filter->in->wp = read_bytes;
      if (read_bytes == 0)
        {
          ret = PDF_EEOF;
        }
      else
        {
          ret = PDF_OK;
        }
    }
  else
    {
      /* No backend */
      ret = PDF_EEOF;
    }

  return ret;
}

/* End of pdf-stm-filter.c */

/* -*- mode: C -*-
 *
 *       File:         pdf-stm-filter.c
 *       Date:         Thu Jun 12 22:13:31 2008
 *
 *       GNU PDF Library - Stream Filter
 *
 */

/* Copyright (C) 2008-2011 Free Software Foundation, Inc. */

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
#include <pdf-stm-f-null.h>
#include <pdf-stm-f-ahex.h>
#include <pdf-stm-f-rl.h>
#include <pdf-stm-f-v2.h>
#include <pdf-stm-f-aesv2.h>
#include <pdf-stm-f-md5.h>
#include <pdf-stm-f-lzw.h>
#include <pdf-stm-f-a85.h>

/* Build-dependent filters */

#if defined PDF_HAVE_LIBZ
# include <pdf-stm-f-flate.h>
#else
# define pdf_stm_f_flateenc_get NULL
# define pdf_stm_f_flatedec_get NULL
#endif /* PDF_HAVE_LIBZ */

#if defined PDF_HAVE_LIBJBIG2DEC
# include <pdf-stm-f-jbig2.h>
#else
# define pdf_stm_f_jbig2dec_get NULL
#endif /* PDF_HAVE_LIBJBIG2DEC */

#if defined PDF_HAVE_LIBJPEG
# include <pdf-stm-f-dct.h>
#else
# define pdf_stm_f_dctdec_get NULL
#endif /* PDF_HAVE_LIBJPEG */

static pdf_bool_t pdf_stm_filter_get_input (pdf_stm_filter_t  *filter,
                                            pdf_bool_t         finish,
                                            pdf_error_t      **error);

/* Filter definition */
struct stm_filter_s {
  /* Filter name */
  const pdf_char_t *name;
  /* Filter implementation getter */
  const pdf_stm_filter_impl_t *(* get_impl) (void);
};

/* Same order as pdf_stm_filter_type_e */
static const struct stm_filter_s filters[PDF_STM_FILTER_LAST] = {
  { "NULL",              pdf_stm_f_null_get     },
  { "RunLength encoder", pdf_stm_f_rlenc_get    },
  { "RunLength decoder", pdf_stm_f_rldec_get    },
  { "FLATE encoder",     pdf_stm_f_flateenc_get },
  { "FLATE decoder",     pdf_stm_f_flatedec_get },
  { "JBI2 decoder",      pdf_stm_f_jbig2dec_get },
  { "DCT decoder",       pdf_stm_f_dctdec_get   },
  { "ASCII-HEX encoder", pdf_stm_f_ahexenc_get  },
  { "ASCII-HEX decoder", pdf_stm_f_ahexdec_get  },
  { "AESv2 encoder",     pdf_stm_f_aesv2enc_get },
  { "AESv2 decoder",     pdf_stm_f_aesv2dec_get },
  { "V2 encoder",        pdf_stm_f_v2enc_get    },
  { "V2 decoder",        pdf_stm_f_v2enc_get    },
  { "MD5 encoder",       pdf_stm_f_md5enc_get   },
  { "LZW encoder",       pdf_stm_f_lzwenc_get   },
  { "LZW decoder",       pdf_stm_f_lzwdec_get   },
  { "ASCII-85 encoder",  pdf_stm_f_a85enc_get   },
  { "ASCII-85 decoder",  pdf_stm_f_a85dec_get   }
};

/* Filter data type */
struct pdf_stm_filter_s
{
  /* Filter implementation */
  enum pdf_stm_filter_type_e type;
  const pdf_stm_filter_impl_t *impl;

  pdf_stm_filter_t *next; /* Next filter in the chain, or NULL */
  pdf_stm_be_t *backend;   /* Backend, or NULL */

  /* Input and output buffers */
  pdf_buffer_t *in;
  pdf_buffer_t *out;

  /* Filter-specific information */
  pdf_hash_t *params;
  void *state;

  /* Filter status */
  pdf_error_t *error;
  pdf_bool_t really_finish;

  /* Operation mode */
  enum pdf_stm_filter_mode_e mode;
};

/*
 * Public functions
 */

pdf_bool_t
pdf_stm_filter_p (enum pdf_stm_filter_type_e type)
{
  return (filters[type].get_impl != NULL ? PDF_TRUE : PDF_FALSE);
}

pdf_stm_filter_t *
pdf_stm_filter_new (enum pdf_stm_filter_type_e   type,
                    pdf_hash_t                  *params,
                    pdf_size_t                   buffer_size,
                    enum pdf_stm_filter_mode_e   mode,
                    pdf_error_t                **error)
{
  pdf_stm_filter_t *filter;

  /* Allocate the filter structure */
  filter = pdf_alloc (sizeof (struct pdf_stm_filter_s));
  if (!filter)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "not enough memory to create a filter: "
                     "couldn't allocate %lu bytes",
                     (unsigned long) sizeof (struct pdf_stm_filter_s));
      return NULL;
    }

  /* Initialisation */
  PDF_ASSERT (type < PDF_STM_FILTER_LAST);
  filter->type = type;

  /* Ensure filter is supported in current build */
  filter->impl = (filters[filter->type].get_impl != NULL ?
                  filters[filter->type].get_impl () :
                  NULL);
  if (!filter->impl)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_EBADDATA,
                     "filter type `%s' not supported",
                     filters[filter->type].name);
      pdf_stm_filter_destroy (filter);
      return NULL;
    }

  /* Data sources */
  filter->next = NULL;
  filter->backend = NULL;

  /* Operation mode */
  filter->mode = mode;

  /* Input buffer */
  filter->in = pdf_buffer_new (buffer_size, error);
  if (!filter->in)
    {
      /* Not enough memory. Retreat. */
      pdf_stm_filter_destroy (filter);
      return NULL;
    }

  /* Output buffer */
  filter->out = NULL;

  /* Initialization of the implementation */
  filter->params = params;
  filter->state = NULL;
  filter->error = NULL;
  filter->really_finish = PDF_FALSE;

  /* Error initializing the filter implementation? */
  if (filter->impl->init_fn &&
      !filter->impl->init_fn (filter->params, &(filter->state), error))
    {
      /* Reset implementation before destroying, so that deinit() is not
       * called */
      filter->impl = NULL;
      pdf_stm_filter_destroy (filter);
      return NULL;
    }

  return filter;
}

void
pdf_stm_filter_destroy (pdf_stm_filter_t *filter)
{
  if (!filter)
    return;

  if (filter->in)
    pdf_buffer_destroy (filter->in);
  if (filter->impl && filter->impl->deinit_fn)
    filter->impl->deinit_fn (filter->state);
  pdf_dealloc (filter);

  /* Note that the memory used by the params hash is NOT managed by the filter */
}

void
pdf_stm_filter_set_next (pdf_stm_filter_t *filter,
                         pdf_stm_filter_t *next_filter)
{
  filter->next = next_filter;
}

void
pdf_stm_filter_set_be (pdf_stm_filter_t *filter,
                       pdf_stm_be_t     *be)
{
  filter->backend = be;
}

void
pdf_stm_filter_set_out (pdf_stm_filter_t *filter,
                        pdf_buffer_t     *buffer)
{
  filter->out = buffer;
}

pdf_buffer_t *
pdf_stm_filter_get_in (pdf_stm_filter_t *filter)
{
  return filter->in;
}

pdf_stm_filter_t *
pdf_stm_filter_get_next (pdf_stm_filter_t *filter)
{
  return filter->next;
}

pdf_stm_filter_t *
pdf_stm_filter_get_tail (pdf_stm_filter_t *filter)
{
  if (!filter->next)
    return filter;

  return pdf_stm_filter_get_tail (filter->next);
}

pdf_bool_t
pdf_stm_filter_apply (pdf_stm_filter_t  *filter,
                      pdf_bool_t         finish,
                      pdf_error_t      **error)
{
  /* If the filter is in an error state or it is in an eof state, just
     communicate it to the caller */
  if (filter->error)
    {
      /* The error is copied, the original is left untouched */
      pdf_propagate_error_dup (error, filter->error);
      return PDF_FALSE;
    }

  while (!pdf_buffer_full_p (filter->out))
    {
      pdf_error_t *inner_error = NULL;
      enum pdf_stm_filter_apply_status_e filter_status;

      /* Generate output */
      filter_status = filter->impl->apply_fn (filter->params,
                                              filter->state,
                                              filter->in,
                                              filter->out,
                                              filter->really_finish,
                                              &(filter->error));

      if (filter_status == PDF_STM_FILTER_APPLY_STATUS_ERROR)
        {
          /* The error is copied, the original is left untouched */
          pdf_propagate_error_dup (error, filter->error);
          return PDF_FALSE;
        }

      /* TODO: Do not treat EOF as an error */
      if (filter_status == PDF_STM_FILTER_APPLY_STATUS_EOF)
        {
          /* The filter is now in an EOF condition. We should not call
             this filter anymore without a previous reset */
          filter->error = pdf_error_new (PDF_EDOMAIN_BASE_STM,
                                         PDF_EEOF,
                                         "EOF while applying filter");
          pdf_propagate_error_dup (error, filter->error);
          return PDF_FALSE;
        }

      if (filter_status == PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT)
        {
          /* The filter needs to generate output and the output buffer
           * is full */
          break;
        }

      /* The filter needs more input, try to get the input buffer
       * filled with some data */
      if (!pdf_stm_filter_get_input (filter, finish, &inner_error))
        {
          /* Error getting input */
          if (pdf_error_get_status (inner_error) == PDF_EEOF)
            {
              if (!pdf_buffer_eob_p (filter->in))
                {
                  /* We got EOF, but data is still available in the input,
                   * so go on */
                  continue;
                }

              if ((!filter->really_finish) &&
                  ((filter->mode == PDF_STM_FILTER_MODE_WRITE &&
                    finish) ||
                   (filter->mode == PDF_STM_FILTER_MODE_READ)))
                {
                  filter->really_finish = PDF_TRUE;
                  pdf_clear_error (&inner_error);
                  continue;
                }
              else
                {
                  /* Propagate EOF error without storing it */
                  pdf_propagate_error (error, inner_error);
                  return PDF_FALSE;
                }
            }

          /* Otherwise, store error and exit */
          filter->error = inner_error;
          pdf_propagate_error_dup (error, filter->error);
          return PDF_FALSE;
        }

      /* Filter got more input, continue */
    }

  /* all done */
  return PDF_TRUE;
}

pdf_bool_t
pdf_stm_filter_reset (pdf_stm_filter_t  *filter,
                      pdf_error_t      **error)
{
  pdf_clear_error (&(filter->error));
  filter->really_finish = PDF_FALSE;

  /* Deinit before re-init */
  if (filter->impl->deinit_fn)
    filter->impl->deinit_fn (filter->state);

  /* Re-init */
  return (filter->impl->init_fn ?
          filter->impl->init_fn (filter->params,
                                 &(filter->state),
                                 error) :
          PDF_TRUE);
}

/*
 * Private functions
 */

static pdf_bool_t
pdf_stm_filter_get_input (pdf_stm_filter_t  *filter,
                          pdf_bool_t         finish,
                          pdf_error_t      **error)
{
  /* The input buffer should be empty at this point */
  pdf_buffer_rewind (filter->in);

  if (filter->next)
    return pdf_stm_filter_apply (filter->next, finish, error);

  if (filter->backend)
    {
      pdf_ssize_t read_bytes;

      read_bytes = pdf_stm_be_read (filter->backend,
                                    filter->in->data,
                                    filter->in->size,
                                    error);
      if (read_bytes < 0)
        return PDF_FALSE;

      filter->in->wp = (pdf_size_t)read_bytes;
      if (read_bytes == filter->in->size)
        return PDF_TRUE;
      /* else, no more input */
    }
  /* else, no backend */

  pdf_set_error (error,
                 PDF_EDOMAIN_BASE_STM,
                 PDF_EEOF,
                 "no more input");
  return PDF_FALSE;
}

/* End of pdf-stm-filter.c */

/* -*- mode: C -*- Time-stamp: "09/09/05 22:52:08 jemarch"
 *
 *       File:         pdf-stm-filter.c
 *       Date:         Thu Jun 12 22:13:31 2008
 *
 *       GNU PDF Library - Stream Filter
 *
 */

/* Copyright (C) 2008, 2009 Free Software Foundation, Inc. */

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

pdf_status_t
pdf_stm_filter_new (enum pdf_stm_filter_type_e type,
                    pdf_hash_t params,
                    pdf_size_t buffer_size,
                    enum pdf_stm_filter_mode_e mode,
                    pdf_stm_filter_t *filter)
{
  pdf_status_t init_ret = PDF_ERROR;
  pdf_stm_filter_t new;

  /* Allocate the filter structure */
  new = (pdf_stm_filter_t) 
    pdf_alloc (sizeof(struct pdf_stm_filter_s));

  if (new)
    {
      /* Initialisation */
      new->type = type;
      
      /* Data sources */
      new->next = NULL;
      new->backend = NULL;
      
      /* Operation mode */
      new->mode = mode;
      
      /* Input buffer */
      new->in = pdf_buffer_new (buffer_size);
      if (!new->in)
        {
          /* Not enough memory. Retreat. */
          pdf_dealloc (new);
          goto exit;
        }
      
      /* Output buffer */
      new->out = NULL;
      
      /* Install the appropriate implementation */
      switch (new->type)
        {
        case PDF_STM_FILTER_NULL:
          {
            new->impl.init_fn = pdf_stm_f_null_init;
            new->impl.apply_fn = pdf_stm_f_null_apply;
            new->impl.dealloc_state_fn = pdf_stm_f_null_dealloc_state;
            break;
          }
        case PDF_STM_FILTER_AHEX_ENC:
          {
            new->impl.init_fn = pdf_stm_f_ahexenc_init;
            new->impl.apply_fn = pdf_stm_f_ahexenc_apply;
            new->impl.dealloc_state_fn = pdf_stm_f_ahexenc_dealloc_state;
            break;
          }
        case PDF_STM_FILTER_AHEX_DEC:
          {
            new->impl.init_fn = pdf_stm_f_ahexdec_init;
            new->impl.apply_fn = pdf_stm_f_ahexdec_apply;
            new->impl.dealloc_state_fn = pdf_stm_f_ahexdec_dealloc_state;
            break;
          }
        case PDF_STM_FILTER_RL_ENC:
          {
            new->impl.init_fn = pdf_stm_f_rlenc_init;
            new->impl.apply_fn = pdf_stm_f_rlenc_apply;
            new->impl.dealloc_state_fn = pdf_stm_f_rlenc_dealloc_state;
            break;
          }
        case PDF_STM_FILTER_RL_DEC:
          {
            new->impl.init_fn = pdf_stm_f_rldec_init;
            new->impl.apply_fn = pdf_stm_f_rldec_apply;
            new->impl.dealloc_state_fn = pdf_stm_f_rldec_dealloc_state;
            break;
          }
#if defined(HAVE_LIBZ)
        case PDF_STM_FILTER_FLATE_ENC:
          {
            new->impl.init_fn = pdf_stm_f_flateenc_init;
            new->impl.apply_fn = pdf_stm_f_flateenc_apply;
            new->impl.dealloc_state_fn = pdf_stm_f_flateenc_dealloc_state;
            break;
          }
        case PDF_STM_FILTER_FLATE_DEC:
          {
            new->impl.init_fn = pdf_stm_f_flatedec_init;
            new->impl.apply_fn = pdf_stm_f_flatedec_apply;
            new->impl.dealloc_state_fn = pdf_stm_f_flatedec_dealloc_state;
            break;
          }
#endif /* HAVE_LIBZ */
        case PDF_STM_FILTER_V2_ENC:
          {
            new->impl.init_fn = pdf_stm_f_v2enc_init;
            new->impl.apply_fn = pdf_stm_f_v2enc_apply;
            new->impl.dealloc_state_fn = pdf_stm_f_v2enc_dealloc_state;
            break;
          }
        case PDF_STM_FILTER_V2_DEC:
          {
            new->impl.init_fn = pdf_stm_f_v2dec_init;
            new->impl.apply_fn = pdf_stm_f_v2dec_apply;
            new->impl.dealloc_state_fn = pdf_stm_f_v2dec_dealloc_state;
            break;
          }
        case PDF_STM_FILTER_AESV2_ENC:
          {
            new->impl.init_fn = pdf_stm_f_aesv2enc_init;
            new->impl.apply_fn = pdf_stm_f_aesv2enc_apply;
            new->impl.dealloc_state_fn = pdf_stm_f_aesv2enc_dealloc_state;
            break;
          }
        case PDF_STM_FILTER_AESV2_DEC:
          {
            new->impl.init_fn = pdf_stm_f_aesv2dec_init;
            new->impl.apply_fn = pdf_stm_f_aesv2dec_apply;
            new->impl.dealloc_state_fn = pdf_stm_f_aesv2dec_dealloc_state;
            break;
          }
        case PDF_STM_FILTER_MD5_ENC:
          {
            new->impl.init_fn = pdf_stm_f_md5enc_init;
            new->impl.apply_fn = pdf_stm_f_md5enc_apply;
            new->impl.dealloc_state_fn = pdf_stm_f_md5enc_dealloc_state;
            break;
          }
	case PDF_STM_FILTER_LZW_ENC:
          {
            new->impl.init_fn = pdf_stm_f_lzwenc_init;
            new->impl.apply_fn = pdf_stm_f_lzwenc_apply;
            new->impl.dealloc_state_fn = pdf_stm_f_lzwenc_dealloc_state;
            break;
          }
	case PDF_STM_FILTER_LZW_DEC:
          {
            new->impl.init_fn = pdf_stm_f_lzwdec_init;
            new->impl.apply_fn = pdf_stm_f_lzwdec_apply;
            new->impl.dealloc_state_fn = pdf_stm_f_lzwdec_dealloc_state;
            break;
          }
#if defined(HAVE_LIBJBIG2DEC)
        case PDF_STM_FILTER_JBIG2_DEC:
          {
            new->impl.init_fn = pdf_stm_f_jbig2dec_init;
            new->impl.apply_fn = pdf_stm_f_jbig2dec_apply;
            new->impl.dealloc_state_fn = pdf_stm_f_jbig2dec_dealloc_state;
            break;
          }
#endif /* HAVE_LIBJBIG2DEC */
#if defined(HAVE_LIBJPEG)
        case PDF_STM_FILTER_DCT_DEC:
          {
            new->impl.init_fn = pdf_stm_f_dctdec_init;
            new->impl.apply_fn = pdf_stm_f_dctdec_apply;
            new->impl.dealloc_state_fn = pdf_stm_f_dctdec_dealloc_state;
            break;
          }
#endif /* HAVE_LIBJPEG */
        default:
          {
            /* Shall not be reached, but makes the compiler happy */
            break;
          }
        }

      /* Initialization of the implementation */
      new->params = params;
      new->state = NULL;
      new->status = PDF_OK;
      new->really_finish_p = PDF_FALSE;

      init_ret = new->impl.init_fn (new->params,
                                    &(new->state));
      if (init_ret != PDF_OK)
        {
          /* Error initializing the filter implementation */
          pdf_buffer_destroy (new->in);
          pdf_dealloc (new);
          new = NULL;
        }

      *filter = new;
    }

 exit:

  return init_ret;
}

pdf_status_t
pdf_stm_filter_destroy (pdf_stm_filter_t filter)
{
  pdf_buffer_destroy (filter->in);
  filter->impl.dealloc_state_fn (filter->state);
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
                        pdf_buffer_t buffer)
{
  filter->out = buffer;
  return PDF_OK;
}

inline pdf_buffer_t
pdf_stm_filter_get_in (pdf_stm_filter_t filter)
{
  return filter->in;
}

pdf_status_t
pdf_stm_filter_apply (pdf_stm_filter_t filter,
                      pdf_bool_t finish_p)
{
  pdf_status_t ret;
  pdf_status_t apply_ret;
  pdf_status_t ret_in;

  /* If the filter is in an error state or it is in an eof state, just
     communicate it to the caller */
  if (filter->status != PDF_OK)
    {
      return filter->status;
    }

  ret = PDF_OK;
  while (!pdf_buffer_full_p (filter->out))
    {
      /* Generate output */
      apply_ret = filter->impl.apply_fn (filter->params,
                                         filter->state,
                                         filter->in,
                                         filter->out,
                                         filter->really_finish_p);

      if (apply_ret == PDF_ERROR)
        {
          /* The filter is now in an error condition. We should not
             call this filter anymore without a previous reset */
          filter->status = PDF_ERROR;
          ret = filter->status;
          break;
        }
      if (apply_ret == PDF_EEOF)
        {
          /* The filter is now in an EOF condition. We should not call
             this filter anymore without a previous reset */
          filter->status = PDF_EEOF;
          ret = filter->status;
          break;
        }
      if (apply_ret == PDF_ENINPUT)
        {
          /* The filter needs more input, try to get the input buffer
             filled with some data */
          ret_in = pdf_stm_filter_get_input (filter, finish_p);
          if (ret_in == PDF_ERROR)
            {
              filter->status = PDF_ERROR;
              ret = filter->status;
              break;
            }
          else if ((ret_in == PDF_EEOF) 
                   && (pdf_buffer_eob_p (filter->in)))
            {
              if (((filter->mode == PDF_STM_FILTER_MODE_WRITE) 
                   && ((finish_p) && (!filter->really_finish_p))) ||
                  ((filter->mode == PDF_STM_FILTER_MODE_READ)
                   && (!filter->really_finish_p)))
                {
                  filter->really_finish_p = PDF_TRUE;
                }
              else
                {
                  ret = PDF_EEOF;
                  break;
                }
            }
        }
      if (apply_ret == PDF_ENOUTPUT)
        {
          /* The filter needs to generate output and the output buffer
             is full */
          filter->status = PDF_OK;
          break;
        }
    }

  return ret;
}

pdf_status_t
pdf_stm_filter_reset (pdf_stm_filter_t filter)
{
  filter->status = PDF_OK;
  filter->really_finish_p = PDF_FALSE;
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

  /* The input buffer should be empty at this point */
  pdf_buffer_rewind (filter->in);

  ret = PDF_OK;
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
      if (read_bytes < filter->in->size)
        {
          ret = PDF_EEOF;
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

/* -*- mode: C -*- Time-stamp: "2008-12-21 14:10:04 davazp"
 *
 *       File:         pdf-stm-f-md5.c
 *       Date:         Fri Dec  5 16:40:50 2008
 *
 *       GNU PDF Library - Message-digest stream filter
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

#include <pdf-stm-f-md5.h>
#include <string.h>

#define MD5_OUTPUT_SIZE 16
#define MD5_CACHE_SIZE 1024


/* Internal state */
struct pdf_stm_f_md5_s
{
  pdf_crypt_md_t md;
  pdf_stm_buffer_t cache;
};
typedef struct pdf_stm_f_md5_s * pdf_stm_f_md5_t;



pdf_status_t
pdf_stm_f_md5enc_init (pdf_hash_t params, void **state)
{
  pdf_status_t ret;
  pdf_stm_f_md5_t filter_state;
  
  filter_state = pdf_alloc (sizeof (struct pdf_stm_f_md5_s));
  
  if (filter_state == NULL)
    {
      ret = PDF_ENOMEM;
    }
  else if (state == NULL)
    {
      pdf_dealloc (filter_state);
      ret = PDF_EBADDATA;
    }
  else
    {
      pdf_crypt_md_t md;
      
      if (pdf_crypt_md_new (PDF_CRYPT_MD_MD5, &md) == PDF_OK)
        {
          filter_state->md = md;

          /* Initialize the buffer cache */
          filter_state->cache = pdf_stm_buffer_new (MD5_CACHE_SIZE);
          if (filter_state->cache != NULL)
            {
              ret = PDF_OK;
              *state = filter_state;
            }
          else
            {
              ret = PDF_ENOMEM;
            }
        }
      else
        {
          pdf_dealloc (filter_state);
          ret = PDF_EBADDATA;
        }
    }
  return ret;
}
  


pdf_status_t
pdf_stm_f_md5enc_apply (pdf_hash_t params, void *state, pdf_stm_buffer_t in,
                        pdf_stm_buffer_t out, pdf_bool_t finish_p)
{
  pdf_stm_f_md5_t filter_state = state;
  pdf_stm_buffer_t cache = filter_state->cache;
  pdf_size_t in_size;
  
  in_size = in->wp - in->rp;
  
  /* We need operate on whole buffer in a single function
     call. So, we must to cache all input before. */
  if (in_size != 0)
    {
      if ((cache->size - cache->wp) < in_size)
        {
          if (pdf_stm_buffer_resize (cache, cache->size + in_size) != PDF_OK)
            return PDF_ERROR;
        }
      
      memcpy (cache->data + cache->wp, in->data, in_size);

      cache->wp += in_size;
      in->rp    += in_size;
    }

  if (finish_p == PDF_TRUE)
    {
      /* Once we have collected all input, then we can go on. */
      pdf_size_t cache_size;
      pdf_size_t out_size;

      cache_size  = cache->wp - cache->rp;

      if (cache_size == 0)
        return PDF_ENINPUT;

      out_size = out->size - out->wp;

      if (MD5_OUTPUT_SIZE > out_size)
        {
          /* We should can return PDF_ENOUTPUT. However the
             output buffer is assumed to be full if we return
             PDF_ENOUTPUT, but we can not write it partially.  */
          return PDF_ERROR;
        }
      else
        {
          pdf_crypt_md_hash (filter_state->md,
                             out->data,
                             out_size,
                             cache->data,
                             cache_size);
        }

      /* Update output buffer */
      cache->rp += cache_size;
      out->wp += MD5_OUTPUT_SIZE;
    }


  return PDF_ENINPUT;
}



pdf_status_t
pdf_stm_f_md5enc_dealloc_state (void *state)
{
  pdf_stm_f_md5_t filter_state = state;
  pdf_crypt_md_destroy (filter_state->md);
  pdf_stm_buffer_destroy (filter_state->cache);
  pdf_dealloc (state);
  return PDF_OK;
}



/* End of pdf_stm_f_md5.c */

/* -*- mode: C -*- Time-stamp: "09/01/11 22:04:42 jemarch"
 *
 *       File:         pdf-stm-f-md5.c
 *       Date:         Fri Dec  5 16:40:50 2008
 *
 *       GNU PDF Library - Message-digest stream filter
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

#include <pdf-stm-f-md5.h>
#include <string.h>

#define MD5_OUTPUT_SIZE 16



/* Internal state */
struct pdf_stm_f_md5_s
{
  pdf_crypt_md_t md;
  pdf_buffer_t cache;
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
          filter_state->cache = pdf_buffer_new (MD5_OUTPUT_SIZE);
          
          if (filter_state->cache == NULL)
            {
              pdf_dealloc (filter_state);
              ret = PDF_ERROR;
            }
          else
            {
              *state = filter_state;
              ret = PDF_OK;
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
pdf_stm_f_md5enc_apply (pdf_hash_t params, void *state, pdf_buffer_t in,
                        pdf_buffer_t out, pdf_bool_t finish_p)
{
  pdf_stm_f_md5_t filter_state = state;
  pdf_buffer_t cache = filter_state->cache;
  pdf_size_t in_size;
  pdf_status_t ret;
  
  in_size = in->wp - in->rp;
  
  pdf_crypt_md_write (filter_state->md, in->data, in_size);
  in->rp += in_size;
  ret = PDF_ENINPUT;

  if (finish_p == PDF_TRUE)
    {
      pdf_size_t bytes_to_write;
      pdf_size_t cache_size;
      pdf_size_t out_size;

      if (pdf_buffer_eob_p (cache))
        {
          /* If we have reached the end, read the hash value in cache */
          pdf_crypt_md_read (filter_state->md, cache->data, cache->size);
          cache->wp = cache->size;
        }

      out_size = out->size - out->wp;
      cache_size = cache->wp - cache->rp;
      bytes_to_write = PDF_MIN (out_size, cache_size);

      memcpy (out->data, cache->data + cache->rp, bytes_to_write);

      cache->rp += bytes_to_write;
      out->wp   += bytes_to_write;

      if (out_size < cache_size)
        ret = PDF_ENOUTPUT;
      else
        ret = PDF_EEOF;
    }

  return ret;
}



pdf_status_t
pdf_stm_f_md5enc_dealloc_state (void *state)
{
  pdf_stm_f_md5_t filter_state = state;
  pdf_crypt_md_destroy (filter_state->md);
  pdf_buffer_destroy (filter_state->cache);
  pdf_dealloc (state);
  return PDF_OK;
}



/* End of pdf_stm_f_md5.c */

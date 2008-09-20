/* -*- mode: C -*- Time-stamp: "08/09/20 16:35:45 jemarch"
 *
 *       File:         pdf-stm-f-null.c
 *       Date:         Mon Jul  9 22:01:41 2007
 *
 *       GNU PDF Library - NULL stream filter
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
#include <string.h>

#include <pdf-stm-buffer.h>
#include <pdf-stm-f-null.h>

/*
 * Public functions
 */

pdf_status_t
pdf_stm_f_null_init (pdf_hash_t params,
                     pdf_hash_t state)
{
  /* This filter doe not use any parameters and does not hold any
     internal state */

  return PDF_OK;
}

pdf_status_t
pdf_stm_f_null_apply (pdf_hash_t params,
                      pdf_hash_t state,
                      pdf_stm_buffer_t in,
                      pdf_stm_buffer_t out)
{
  pdf_status_t ret;
  pdf_size_t in_size;
  pdf_size_t out_size;
  pdf_size_t bytes_to_copy;

  /* Fill the output buffer with the contents of the input buffer, but
     note that the second may be bigger than the former */
  in_size = in->wp - in->rp;
  out_size = out->size - out->wp;

  bytes_to_copy = PDF_MIN(out_size, in_size);
  strncpy ((char *) out->data,
           (char *) in->data,
           bytes_to_copy);

  in->rp = in->rp + bytes_to_copy;
  out->wp = out->wp + bytes_to_copy;

  if (bytes_to_copy < out_size)
    {
      ret = PDF_EEOF;
    }
  else
    {
      ret = PDF_OK;
    }

  return ret;
}

/* End of pdf_stm_f_null.c */

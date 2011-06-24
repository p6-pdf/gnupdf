/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-null.c
 *       Date:         Mon Jul  9 22:01:41 2007
 *
 *       GNU PDF Library - NULL stream filter
 *
 */

/* Copyright (C) 2007-2011 Free Software Foundation, Inc. */

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

#include <pdf-types.h>
#include <pdf-types-buffer.h>
#include <pdf-stm-f-null.h>
#include <pdf-hash.h>

/* Define NULL filter */
PDF_STM_FILTER_DEFINE_STATELESS (pdf_stm_f_null_get,
                                 stm_f_null_apply);

static enum pdf_stm_filter_apply_status_e
stm_f_null_apply (void          *state,
                  pdf_buffer_t  *in,
                  pdf_buffer_t  *out,
                  pdf_bool_t     finish,
                  pdf_error_t  **error)
{
  pdf_size_t in_size;
  pdf_size_t out_size;
  pdf_size_t bytes_to_copy;

  /* Fill the output buffer with the contents of the input buffer, but
   * note that the second may be bigger than the former */
  PDF_ASSERT (in->wp >= in->rp);
  PDF_ASSERT (out->size >= out->wp);
  in_size = in->wp - in->rp;
  out_size = out->size - out->wp;

  bytes_to_copy = PDF_MIN (out_size, in_size);
  if (bytes_to_copy != 0)
    {
      memcpy (out->data,
              in->data,
              bytes_to_copy);

      in->rp += bytes_to_copy;
      out->wp += bytes_to_copy;
    }

  return (in_size > out_size ?
          PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT :
          PDF_STM_FILTER_APPLY_STATUS_NO_INPUT);
}

/* End of pdf_stm_f_null.c */

/* -*- mode: C -*-
 *
 *       File:         pdf-stm-filter.h
 *       Date:         Thu Jun 12 22:05:06 2008
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

#ifndef PDF_STM_FILTER_H
#define PDF_STM_FILTER_H

#include <config.h>

#include <pdf-types.h>
#include <pdf-types-buffer.h>
#include <pdf-hash.h>
#include <pdf-stm-be.h>

/* BEGIN PUBLIC */

/* --------------------- Stream Filters ------------------------- */

/* Types of filters.
 *
 * Note that some filters are only available if some external libraries are
 * present. Do NOT #ifdef the enumerations of those optional filters, as then
 * API will change depending on the compilation options.
*/
enum pdf_stm_filter_type_e
{
  PDF_STM_FILTER_UNKNOWN = -1,
  PDF_STM_FILTER_NULL = 0,
  PDF_STM_FILTER_RL_ENC,
  PDF_STM_FILTER_RL_DEC,
  PDF_STM_FILTER_FLATE_ENC, /* Only if libz available */
  PDF_STM_FILTER_FLATE_DEC, /* Only if libz available */
  PDF_STM_FILTER_JBIG2_DEC, /* Only if libjbig2dec available */
  PDF_STM_FILTER_DCT_DEC,   /* Only if libjpeg available */
  PDF_STM_FILTER_AHEX_ENC,
  PDF_STM_FILTER_AHEX_DEC,
  PDF_STM_FILTER_AESV2_ENC,
  PDF_STM_FILTER_AESV2_DEC,
  PDF_STM_FILTER_V2_ENC,
  PDF_STM_FILTER_V2_DEC,
  PDF_STM_FILTER_MD5_ENC,
  PDF_STM_FILTER_LZW_ENC,
  PDF_STM_FILTER_LZW_DEC,
  PDF_STM_FILTER_A85_ENC,
  PDF_STM_FILTER_A85_DEC,
  PDF_STM_FILTER_LAST
};

/* END PUBLIC */

enum pdf_stm_filter_mode_e
{
  PDF_STM_FILTER_MODE_READ,
  PDF_STM_FILTER_MODE_WRITE
};

enum pdf_stm_filter_apply_status_e
{
  PDF_STM_FILTER_APPLY_STATUS_OK = 0,   /* No error, internal use only! */
  PDF_STM_FILTER_APPLY_STATUS_ERROR,    /* An error happened */
  PDF_STM_FILTER_APPLY_STATUS_EOF,      /* Nothing else to do */
  PDF_STM_FILTER_APPLY_STATUS_NO_INPUT, /* More input data wanted */
  PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT /* More output space wanted */
};

/* Filter implementation */
typedef struct {
  /* Initialize filter. */
  pdf_bool_t (* init_fn) (pdf_hash_t   *params,
                          void        **state,
                          pdf_error_t **error);

  /* Apply filter. */
  enum pdf_stm_filter_apply_status_e (* apply_fn) (pdf_hash_t    *params,
                                                   void          *state,
                                                   pdf_buffer_t  *in,
                                                   pdf_buffer_t  *out,
                                                   pdf_bool_t     finish,
                                                   pdf_error_t  **error);

  /* Deinitialize filter */
  void (* deinit_fn) (void *state);
} pdf_stm_filter_impl_t;

typedef struct pdf_stm_filter_s pdf_stm_filter_t;

pdf_bool_t pdf_stm_filter_p (enum pdf_stm_filter_type_e type);

pdf_stm_filter_t *pdf_stm_filter_new (enum pdf_stm_filter_type_e   type,
                                      pdf_hash_t                  *params,
                                      pdf_size_t                   buffer_size,
                                      enum pdf_stm_filter_mode_e   mode,
                                      pdf_error_t                **error);

void pdf_stm_filter_destroy (pdf_stm_filter_t *filter);

void pdf_stm_filter_set_next (pdf_stm_filter_t *filter,
                              pdf_stm_filter_t *next_filter);

void pdf_stm_filter_set_be (pdf_stm_filter_t *filter,
                            pdf_stm_be_t     *be);

void pdf_stm_filter_set_out (pdf_stm_filter_t *filter,
                             pdf_buffer_t     *buffer);

pdf_buffer_t *pdf_stm_filter_get_in (pdf_stm_filter_t *filter);

pdf_stm_filter_t *pdf_stm_filter_get_next (pdf_stm_filter_t *filter);

pdf_stm_filter_t *pdf_stm_filter_get_tail (pdf_stm_filter_t *filter);

pdf_bool_t pdf_stm_filter_apply (pdf_stm_filter_t  *filter,
                                 pdf_bool_t         finish,
                                 pdf_error_t      **error);

pdf_bool_t pdf_stm_filter_reset (pdf_stm_filter_t  *filter,
                                 pdf_error_t      **error);

#endif /* ! PDF_STM_FILTER_H */

/* End of pdf-stm-filter.h */

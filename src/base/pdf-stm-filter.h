/* -*- mode: C -*- Time-stamp: "08/09/22 22:18:18 jemarch"
 *
 *       File:         pdf-stm-filter.h
 *       Date:         Thu Jun 12 22:05:06 2008
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

#ifndef PDF_STM_FILTER_H
#define PDF_STM_FILTER_H

#include <config.h>
#include <pdf-types.h>
#include <pdf-hash.h>
#include <pdf-stm-buffer.h>
#include <pdf-stm-be.h>

#include <pdf-stm-f-null.h>
#include <pdf-stm-f-ahex.h>

/* BEGIN PUBLIC */

/* Types of filters */
enum pdf_stm_filter_type_e 
{
  PDF_STM_FILTER_NULL = 0,
  PDF_STM_FILTER_AHEX_ENC,
  PDF_STM_FILTER_AHEX_DEC
};

/* Filter implementation */
struct pdf_stm_filter_impl_s
{
  pdf_status_t (*init_fn) (pdf_hash_t params,
                           void **state);

  pdf_status_t (*apply_fn) (pdf_hash_t params,
                            void *state,
                            pdf_stm_buffer_t in,
                            pdf_stm_buffer_t out,
                            pdf_bool_t finish_p);
};

/* Filter data type */
struct pdf_stm_filter_s
{
  enum pdf_stm_filter_type_e type;

  struct pdf_stm_filter_s *next; /* Next filter in the chain, or
                                    NULL */
  pdf_stm_be_t backend;          /* Backend, or NULL */

  /* Input and output buffers */
  pdf_stm_buffer_t in;
  pdf_stm_buffer_t out;

  /* Filter-specific information */
  pdf_hash_t params;
  void *state;

  /* Filter implementation */
  struct pdf_stm_filter_impl_s impl;
};

typedef struct pdf_stm_filter_s *pdf_stm_filter_t;

/* END PUBLIC */

/*
 * Public API
 */

pdf_stm_filter_t pdf_stm_filter_new (enum pdf_stm_filter_type_e type,
                                     pdf_hash_t params,
                                     pdf_size_t buffer_size);
pdf_status_t pdf_stm_filter_destroy (pdf_stm_filter_t filter);
inline pdf_status_t pdf_stm_filter_set_next (pdf_stm_filter_t filter,
                                             pdf_stm_filter_t next_filter);
inline pdf_status_t pdf_stm_filter_set_be (pdf_stm_filter_t filter,
                                           pdf_stm_be_t be);
inline pdf_status_t pdf_stm_filter_set_out (pdf_stm_filter_t filter,
                                            pdf_stm_buffer_t buffer);
pdf_stm_filter_t pdf_stm_filter_get_tail (pdf_stm_filter_t filter);
inline pdf_stm_buffer_t pdf_stm_filter_get_in (pdf_stm_filter_t filter);

pdf_status_t pdf_stm_filter_apply (pdf_stm_filter_t filter, pdf_bool_t finish_p);
pdf_status_t pdf_stm_filter_reset (pdf_stm_filter_t filter);

#endif /* ! PDF_STM_FILTER_H */

/* End of pdf-stm-filter.h */

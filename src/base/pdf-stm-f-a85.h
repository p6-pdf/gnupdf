/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-a85.h
 *       Date:         Mon Jul  9 21:59:50 2007
 *
 *       GNU PDF Library - ASCII85 stream filter
 *
 */

/* Copyright (C) 2009 Free Software Foundation, Inc. */

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


#ifndef PDF_STM_F_A85_H
#define PDF_STM_F_A85_H

#include <config.h>
#include <pdf-types.h>
#include <pdf-hash.h>

#define A85_SPARE_BYTES_LEN 4
#define A85_OUTPUT_BUFF_LEN 8

/* defined as 79 so that the newline character completes the 80 char line */
/* (of course that only happens when newline is a single character...)    */
#define A85_ENC_LINE_LENGTH 79



/* Internal state */
struct pdf_stm_f_a85_s
{
  /* spare_bytes will store leftover bytes if finish_p is not set */
  pdf_size_t line_length;
  pdf_size_t spare_count; /* Number of input bytes in spare_bytes */
  pdf_size_t output_count; /* Number of output bytes left in output_buff */
  int        terminated;
  pdf_char_t spare_bytes[A85_SPARE_BYTES_LEN];
  pdf_char_t output_buff[A85_OUTPUT_BUFF_LEN];
};

typedef struct pdf_stm_f_a85_s *pdf_stm_f_a85_t;

/* Filter implementation API */

pdf_status_t
pdf_stm_f_a85enc_init (pdf_hash_t params, void **state);

pdf_status_t
pdf_stm_f_a85enc_apply (pdf_hash_t params, void *state, pdf_buffer_t in,
                        pdf_buffer_t out, pdf_bool_t finish_p);

pdf_status_t
pdf_stm_f_a85enc_dealloc_state (void *state);

pdf_status_t
pdf_stm_f_a85dec_init (pdf_hash_t params, void **state);

pdf_status_t
pdf_stm_f_a85dec_apply (pdf_hash_t params, void *state, pdf_buffer_t in,
                        pdf_buffer_t out, pdf_bool_t finish_p);

pdf_status_t
pdf_stm_f_a85dec_dealloc_state (void *state);


#endif /* !PDF_STM_F_A85_H */

/* End of pdf_stm_f_a85.h */

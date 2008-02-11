/* -*- mode: C -*- Time-stamp: "08/02/10 23:24:20 jemarch"
 *
 *       File:         pdf_stm_f_lzw.h
 *       Date:         Wed Aug 15 14:13:20 2007
 *
 *       GNU PDF Library - LZW encoder/decoder
 *
 */

/* Copyright (C) 2007 Free Software Foundation, Inc. */

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

#ifndef PDF_STM_F_LZW_H
#define PDF_STM_F_LZW_H

#include <config.h>
#include <pdf-base.h>

/* Configuration data */

/* BEGIN PUBLIC */

enum pdf_stm_f_lzw_mode_t
{
  PDF_STM_F_LZW_MODE_ENCODE,
  PDF_STM_F_LZW_MODE_DECODE
};

/* END PUBLIC */

struct pdf_stm_f_lzw_conf_s
{
  int mode;
  int early_change;   /* An indication of when to increase the code
                         length. If the value of this entry is 0, code
                         length increases are postponed as long as
                         possible. If the value is 1, code length
                         increases occur one code early. This
                         parameter is included because LZW sample code
                         distributed by some vendors increases the
                         code length one code earlier than necessary.
                         
                         Default value: 1 */
};

typedef struct pdf_stm_f_lzw_conf_s *pdf_stm_f_lzw_conf_t;

/* Private data */

struct pdf_stm_f_lzw_data_s
{
  int mode;
  int early_change;
};

typedef struct pdf_stm_f_lzw_data_s *pdf_stm_f_lzw_data_t;

/* Filter API implementation */

int pdf_stm_f_lzw_init (void **filter_data, void *conf_data);
int pdf_stm_f_lzw_apply (void *filter_data,
                         pdf_char_t *in, pdf_stm_pos_t in_size,
                         pdf_char_t **out, pdf_stm_pos_t *out_size);
int pdf_stm_f_lzw_dealloc (void **filter_data);

#endif /* pdf_stm_f_lzw.h */

/* End of pdf_stm_f_lzw.h */

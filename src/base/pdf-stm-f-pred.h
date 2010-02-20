/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-pred.h
 *       Date:         Thu Jul 12 01:18:14 2007
 *
 *       GNU PDF Library - Predictor stream filter
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

#ifndef PDF_STM_F_PRED_H
#define PDF_STM_F_PRED_H

/* BEGIN PUBLIC */

typedef enum
{
  PDF_STM_F_PREDENC_NO_PREDICTION = 1,
  PDF_STM_F_PREDENC_TIFF_PREDICTOR_2 = 2,
  PDF_STM_F_PREDENC_PNG_NONE_ALL_ROWS = 10,
  PDF_STM_F_PREDENC_PNG_SUB_ALL_ROWS = 11,
  PDF_STM_F_PREDENC_PNG_UP_ALL_ROWS = 12,
  PDF_STM_F_PREDENC_PNG_AVERAGE_ALL_ROWS = 13,
  PDF_STM_F_PREDENC_PNG_PAETH_ALL_ROWS = 14,
  PDF_STM_F_PREDENC_PNG_OPTIMUM = 15
} pdf_stm_f_predenc_method_t;

typedef enum
{
  PDF_STM_F_PREDDEC_NO_PREDICTION = 1,
  PDF_STM_F_PREDDEC_TIFF_PREDICTOR_2 = 2,
  PDF_STM_F_PREDDEC_PNG = 3
} pdf_stm_f_preddec_type_t;

typedef enum
{
  PDF_STM_F_PREDDEC_PNG_NONE = 10,
  PDF_STM_F_PREDDEC_PNG_SUB = 11,
  PDF_STM_F_PREDDEC_PNG_UP = 12,
  PDF_STM_F_PREDDEC_PNG_AVERAGE = 13,
  PDF_STM_F_PREDDEC_PNG_PAETH = 14
} pdf_stm_f_predec_png_type_t;

typedef enum {
  PDF_STM_F_PRED_MODE_ENCODE,
  PDF_STM_F_PRED_MODE_DECODE
} pdf_stm_f_pred_mode_t;

/* END PUBLIC */

/* Configuration structure */

struct pdf_stm_f_pred_conf_s
{
  int mode;      /* It might be encode or decode. */
  int predictor; /* A code that selects the predictor algorithm. If
                    the value of this entry is 1, the filter assumes
                    that the normal algorithm was used to encode the
                    data, without prediction. If the value is greater
                    than 1, the filter assumes that the data was
                    differenced before being encoded, and `predictor'
                    selects the predictor algorithm */


  /* The following parameters are only useful when `predictor' > 1 */
  int colors;              /* Number of interleaved color components
                              per sample. Valid values are 1 or
                              greater. Default value: 1 */
  int bits_per_component;  /* The number of bits used to represent
                              each color component in a sample. Valid
                              values are 1, 2, 4, 8 and 16. Default
                              value: 1 */
  int columns;             /* The number of samples in each
                              row. Default value: 1 */
};

typedef struct pdf_stm_f_pred_conf_s *pdf_stm_f_pred_conf_t;

/* Private data */

struct pdf_stm_f_pred_data_s
{
  int mode;
  int predictor;
  int colors;
  int bits_per_component;
  int columns;
  int scanline_len;
};

typedef struct pdf_stm_f_pred_data_s *pdf_stm_f_pred_data_t;

/* Filter API implementation */

int pdf_stm_f_pred_init (void **filter_data, void *conf_data);
int pdf_stm_f_pred_apply (void *filter_data,
                          pdf_char_t *in, pdf_stm_pos_t in_size,
                          pdf_char_t **out, pdf_stm_pos_t *out_size);
int pdf_stm_f_pred_dealloc (void **filter_data);

#endif /* pdf_stm_f_pred.h */

/* End of pdf_stm_f_pred.h */

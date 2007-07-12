/* -*- mode: C -*- Time-stamp: "07/07/12 01:37:07 jemarch"
 *
 *       File:         pdf_stm_f_pred.h
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Thu Jul 12 01:18:14 2007
 *
 *       GNU PDF Library - Predictor stream filter
 *
 */

/* Copyright (C) 2007 Jose E. Marchesi */

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

enum pdf_stm_f_pred_type_t
{
  NO_PREDICTION = 1,
  TIFF_PREDICTOR_2 = 2,
  PNG_NONE_ALL_ROWS = 10,
  PNG_SUB_ALL_ROWS = 11,
  PNG_UP_ALL_ROWS = 12,
  PNG_AVERAGE_ALL_ROWS = 13,
  PNG_PAETH_ALL_ROWS = 14,
  PNG_OPTIMUM = 15
};


/* Configuration structure */

struct pdf_stm_f_pred_conf_s
{
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
  int predictor;
  int colors;
  int bits_per_component;
  int columns;
};

typedef struct pdf_stm_f_pred_data_s *pdf_stm_f_pred_data_t;

/* Filter API implementation */

int pdf_stm_f_pred_init (void **filter_data, void *conf_data);
int pdf_stm_f_pred_apply (void *filter_data,
                          pdf_char *in, pdf_stm_pos_t in_size,
                          pdf_char **out, pdf_stm_pos_t *out_size);
int pdf_stm_f_pred_dealloc (void **filter_data);

#endif /* pdf_stm_f_pred.h */

/* End of pdf_stm_f_pred.h */

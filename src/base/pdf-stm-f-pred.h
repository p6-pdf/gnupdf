/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-pred.h
 *       Date:         Thu Jul 12 01:18:14 2007
 *
 *       GNU PDF Library - Predictor stream filter
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

#ifndef PDF_STM_F_PRED_H
#define PDF_STM_F_PRED_H


#include <config.h>
#include <pdf-stm-filter.h>

const pdf_stm_filter_impl_t *pdf_stm_f_preddec_get (void);

const pdf_stm_filter_impl_t *pdf_stm_f_predenc_get (void);


#endif /* pdf_stm_f_pred.h */

/* End of pdf_stm_f_pred.h */

/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-v2.h
 *       Date:         Fri Dec  5 16:58:49 2008
 *
 *       GNU PDF Library - V2 stream filter
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

#ifndef PDF_STM_F_V2_H
#define PDF_STM_F_V2_H

#include <config.h>

#include <pdf-stm-filter.h>

const pdf_stm_filter_impl_t *pdf_stm_f_v2dec_get (void);

const pdf_stm_filter_impl_t *pdf_stm_f_v2enc_get (void);

#endif /* PDF_STM_F_V2_H */

/* End of pdf_stm_f_v2.h */

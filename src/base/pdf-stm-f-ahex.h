/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-ahex.h
 *       Date:         Fri Jul 13 17:04:29 2007
 *
 *       GNU PDF Library - ASCIIHex encoder/decoder
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

#ifndef PDF_STM_F_AHEX_H
#define PDF_STM_F_AHEX_H

#include <config.h>

#include <pdf-stm-filter.h>

const pdf_stm_filter_impl_t *pdf_stm_f_ahexdec_get (void);

const pdf_stm_filter_impl_t *pdf_stm_f_ahexenc_get (void);

#endif /* !PDF_STM_F_AHEX_H */

/* End of pdf_stm_f_ahex.h */

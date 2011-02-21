/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-null.h
 *       Date:         Mon Jul  9 21:59:50 2007
 *
 *       GNU PDF Library - NULL stream filter
 *
 */

/* Copyright (C) 2007, 2008, 2009 Free Software Foundation, Inc. */

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

#ifndef PDF_STM_F_NULL_H
#define PDF_STM_F_NULL_H

#include <config.h>

#include <pdf-types.h>
#include <pdf-types-buffer.h>
#include <pdf-hash.h>

/* Filter implementation API */

pdf_status_t pdf_stm_f_null_init (pdf_hash_t  *params,
                                  void       **state);

pdf_status_t pdf_stm_f_null_apply (pdf_hash_t   *params,
                                   void         *state,
                                   pdf_buffer_t  in,
                                   pdf_buffer_t  out,
                                   pdf_bool_t    finish_p);
pdf_status_t pdf_stm_f_null_dealloc_state (void *state);

#endif /* !PDF_STM_F_NULL_H */

/* End of pdf_stm_f_null.h */

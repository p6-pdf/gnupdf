/* -*- mode: C -*-
 *
 *       File:         pdf-stm-be-cfile.h
 *       Date:         Mon Apr 18 16:03:54 2011
 *
 *       GNU PDF Library - C-File Stream backend
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

#ifndef PDF_STM_BE_CFILE_H
#define PDF_STM_BE_CFILE_H

#include <config.h>

#include <stdio.h>

#include <pdf-stm-be.h>

pdf_stm_be_t *pdf_stm_be_new_cfile (FILE         *file,
                                    pdf_off_t     pos,
                                    pdf_error_t **error);

#endif /* !PDF_STM_BE_CFILE_H */

/* End of pdf-stm-be-cfile.h */

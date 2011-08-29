/* -*- mode: C -*-
 *
 *       File:         pdf-global.h
 *       Date:         Thu Jul  5 19:52:48 2007
 *
 *       GNU PDF Library - Global code
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

#ifndef _PDF_H
#define _PDF_H

#include <config.h>
#include <pdf-types.h>
#include <pdf-error.h>

/* BEGIN PUBLIC */

/* -------------------- General Library Initialization ---------------------- */
/* API of the library */

/* Initialize the library.
 * Must be called before any other method.
 */
pdf_bool_t pdf_init (pdf_error_t **error);

/* Clean exit handling.
 * Only to be called when you really want to spend some time cleaning up the
 * context allocated by the library, or when you want to get a clean memory
 * leak report. Otherwise, it's safe to skip this and just exit the program.
 */
void pdf_finish (void);

/* END PUBLIC */

#endif /* pdf.h */

/* End of pdf.h */

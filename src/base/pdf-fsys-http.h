/* -*- mode: C -*-
 *
 *       File:         pdf-fsys-http.h
 *       Date:         Thu Dec 17 18:22:59 2009
 *
 *       GNU PDF Library - HTTP Filesystem Implementation
 *
 */

/* Copyright (C) 2009-2011 Free Software Foundation, Inc. */

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

#ifndef PDF_FSYS_HTTP_H
#define PDF_FSYS_HTTP_H

#include <pdf-fsys.h>

/* BEGIN PUBLIC */

#define PDF_FSYS_HTTP_ID "http"

#define PDF_FSYS_HTTP pdf_fsys_get (PDF_FSYS_HTTP_ID)

/* END PUBLIC */

pdf_bool_t pdf_fsys_http_init (pdf_error_t **error);
void pdf_fsys_http_deinit (void);

#endif /* !PDF_FSYS_HTTP_H */

/* End of pdf-fsys-http.h */

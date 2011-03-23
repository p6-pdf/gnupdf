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

/* BEGIN PUBLIC */

/* API of the library */

int  pdf_init   (void);
void pdf_finish (void);

/* END PUBLIC */

#endif /* pdf.h */

/* End of pdf.h */

/* -*- mode: C -*- Time-stamp: "08/11/27 20:59:39 jemarch"
 *
 *       File:         pdf-fp.h
 *       Date:         Sun Feb 10 21:36:37 2008
 *
 *       GNU PDF Library - Floating-point Arithmetic Module
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

#ifndef PDF_FP_H
#define PDF_FP_H

#include <pdf-types.h>

/* BEGIN PUBLIC */

typedef float pdf_real_t;

/* Geometry */

typedef pdf_real_t *pdf_point_t;

#define P_X(point) (point)[0]
#define P_Y(point) (point)[1]

pdf_point_t pdf_create_point (void);
void pdf_destroy_point (pdf_point_t point);
pdf_point_t pdf_point_dup (pdf_point_t point);


/* END PUBLIC */

#endif /* !PDF_FP_H */

/* End of pdf-fp.h */

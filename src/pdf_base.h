/* -*- mode: C -*- Time-stamp: "07/09/11 20:20:10 jemarch"
 *
 *       File:         pdf_base.h
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Fri Jul  6 20:22:05 2007
 *
 *       GNU PDF Library - Common resources for the library modules
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

#ifndef _PDF_BASE_H
#define _PDF_BASE_H

#include <config.h>
#include <sys/types.h> /* for off_t */

#ifdef HAVE_INLINE
#define INLINE inline
#else
#define INLINE
#endif /* HAVE_INLINE */

#define PDF_TRUE 1
#define PDF_FALSE 0

#define PDF_OK PDF_TRUE
#define PDF_ERROR PDF_FALSE

/* A variable of type `pdf_stm_pos_t' contain a byte-offset relative to
   the beginning of a stream object. 

   Please be careful manipulating `pdf_stm_pos_t' values. Its value is
   assured to be a signed scalable one, but its size may be wider than
   a long. 

   Note that this is a ugly hack: we define `pdf_stm_pos_t' in
   `pdf_base.h' just because the stream backends need to use it. Its
   proper place would be `pdf_stm.h'. */

#define NO_POS -1
typedef off_t pdf_stm_pos_t;

/* PDF manages bytes with values in the range 0-255 */

typedef unsigned char pdf_char_t;


/* PDF integers and reals are implemented using basic C types. */

typedef int pdf_integer_t;
typedef float pdf_real_t;

/* String utility functions */

int pdf_str2int (char *string, 
                 int *num);
int pdf_str2long (char *string, 
                  long *num);
int pdf_str2double (char *string, 
                    double *num);

/* Geometry */

typedef pdf_real_t *pdf_point_t;

#define P_X(point) (point)[0]
#define P_Y(point) (point)[1]

pdf_point_t pdf_create_point (void);
void pdf_destroy_point (pdf_point_t point);
pdf_point_t pdf_point_dup (pdf_point_t point);

/* Interpolation functions */

double pdf_interp_lineal (double x1, double y1,
                          double x2, double y2,
                          double x);

double pdf_interp_exp_coef_m (double x1, double x2,
                              double y1, double y2);
double pdf_interp_exp_coef_k (double x1, double y1,
                              double m);
double pdf_interp_exp (double m, double k,
                       double x);

#endif /* pdf_base.h */

/* End of pdf_base.h */

/* -*- mode: C -*- Time-stamp: "09/09/05 22:49:41 jemarch"
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

#include <config.h> 

#include <math.h>

#include <pdf-types.h>

/* BEGIN PUBLIC */

/* Constants */

#define PDF_PI M_PI

/* Data types */

typedef float pdf_real_t;

struct pdf_point_s
{
  pdf_real_t h;
  pdf_real_t v;
};

typedef struct pdf_point_s *pdf_point_t;

struct pdf_matrix_s
{
  pdf_real_t a;
  pdf_real_t b;
  pdf_real_t c;
  pdf_real_t d;
  pdf_real_t h;
  pdf_real_t v;
};

typedef struct pdf_matrix_s *pdf_matrix_t;

struct pdf_quad_s
{
  pdf_real_t t1;
  pdf_real_t tr;
  pdf_real_t b1;
  pdf_real_t br;
};

typedef struct pdf_quad_s *pdf_quad_t;

struct pdf_rect_s
{
  pdf_real_t left;
  pdf_real_t top;
  pdf_real_t right;
  pdf_real_t bottom;
};

typedef struct pdf_rect_s *pdf_rect_t;

/* Function prototypes */

pdf_real_t pdf_fp_sqrt (pdf_real_t op);
pdf_real_t pdf_fp_sin (pdf_real_t op);
pdf_real_t pdf_fp_cos (pdf_real_t op);
pdf_real_t pdf_fp_atan2 (pdf_real_t op1, pdf_real_t op2);
pdf_real_t pdf_fp_exp (pdf_real_t op);
pdf_real_t pdf_fp_abs (pdf_real_t op);
pdf_real_t pdf_fp_log10 (pdf_real_t op);
pdf_real_t pdf_fp_log (pdf_real_t op);
pdf_real_t pdf_fp_floor (pdf_real_t op);
pdf_real_t pdf_fp_ceil (pdf_real_t op);

/* END PUBLIC */

#endif /* !PDF_FP_H */

/* End of pdf-fp.h */

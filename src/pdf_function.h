/* -*- mode: C -*- Time-stamp: "08/01/20 20:03:05 jemarch"
 *
 *       File:         pdf_function.h
 *       Date:         Sun Sep  9 21:02:24 2007
 *
 *       GNU PDF Library - Function common data type
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

#ifndef PDF_FUNCTION_H
#define PDF_FUNCTION_H

#include <config.h>
#include <pdf_base.h>
#include <pdf_obj.h>

/* Type 0: sampled functions */

struct pdf_func_0_s
{
  pdf_obj_t size;   /* An array of `m' positive integers specifying the
                       number of samples in each input dimension of the
                       sample table. */
  int bps;          /* Bits per sample. The number of bits used to represent
                       each sample. (If the function has multiple output
                       values, each one occupies `bps' bits). Valid values are
                       1, 2, 4, 8, 12, 16, 24 and 32. */
  int order;        /* The order of interpolation between samples. Valid
                       values are 1 and 3, specifying linear and cubic spline
                       interpolation, respectively. Default value: 1 */
  pdf_obj_t encode; /* An array of `2*m' numbers specifying the linear
                       mapping of input values into the domain of the
                       function's sample table. Default value: [0
                       (Size_0 - 1) 0 (Size_1 - 1) ...] */
  pdf_obj_t decode; /* An array of `2*n' numbers specifying the linear
                       mapping of sample values into the range
                       appropriate for the function's output
                       values. Default value: same as the value of
                       `range' (see below) */
};

/* Type 2: exponential interpolation functions */

struct pdf_func_2_s
{


};

/* Type 3: stitching functions */

struct pdf_func_3_s
{


};

/* Type 4: postscript calculator functions */

struct pdf_func_4_s
{


};

/* Function data structure */

struct pdf_func_s
{
  int type;         /* The function type:
                 
                       0 Sampled function
                       2 Exponential interpolation function
                       3 Stitching function
                       4 PostScript calculator function */
  pdf_obj_t domain; /* An array of `2*m' numbers, where `m' is the
                       number of input values. For each `i' from `0'
                       to `m - 1', `Domain_2i' must be less than or
                       equal to `Domain_2i+1', and the `ith' input
                       value, `x_i' must lie in the interval
                       `Domain_2i <= x_i <= Domain_2i+1'. Input values
                       outside the declared domains are clipped to the
                       nearest boundary value. */
  pdf_obj_t range;  /* An array of `2*n' numbers, where `n' is the
                       number of output values. For each 'j' from '0'
                       to `n-1', `Range_2j' must be less than or equal
                       to `Range_2j+1', and the `jth' output value,
                       `y_j', must lie in the interval `Range_2j <= y_j
                       <= Range_2j+1'. Output values outside the
                       declared range are clipped to the nearest
                       boundary value. If this entry is absent no
                       clipping is done. Only used with type 0 and 4
                       functions. */

  /* Type-specific data */
  union 
  {
    struct pdf_func_0_s t0;
    struct pdf_func_2_s t2;
    struct pdf_func_3_s t3;
    struct pdf_func_4_s t4;
  } details;

};

typedef struct pdf_func_s *pdf_func_t;


pdf_func_t pdf_create_func_0 (pdf_obj_t stream);
pdf_func_t pdf_create_func_2 (pdf_obj_t dict);
pdf_func_t pdf_create_func_3 (pdf_obj_t dict);
pdf_func_t pdf_create_func_4 (pdf_obj_t stream);


#endif /* pdf_function.h */

/* End of pdf_function.h */

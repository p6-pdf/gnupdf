/* -*- mode: C -*- Time-stamp: "2009-02-20 19:58:37 davazp"
 *
 *       File:         pdf-fp-func.h
 *       Date:         Sun Nov 30 18:44:41 2008
 *
 *       GNU PDF Library - Functions
 *
 */

/* Copyright (C) 2008 Free Software Foundation, Inc. */

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

#ifndef PDF_FP_FUNC_H
#define PDF_FP_FUNC_H

#include <config.h>

#include <pdf-types.h>
#include <pdf-fp.h>

/* BEGIN PUBLIC */

typedef struct pdf_fp_func_s *pdf_fp_func_t;

/* Interal size of the type 4 functions stack */
#define PDF_FP_FUNC_TYPE4_STACK_SIZE 100

typedef union
{
  struct {
    pdf_status_t status;
    pdf_i32_t op;
    double stack[PDF_FP_FUNC_TYPE4_STACK_SIZE];
  } type4;
} pdf_fp_func_debug_t;

/* END PUBLIC */


struct pdf_fp_func_0_s
{
  pdf_u32_t m;
  pdf_u32_t n;
  double *encode;
  double *decode;
  pdf_u32_t *size;
  // pdf_u32_t *stride; /* stride[i] := prod(j=0,i-1,size[i]) */
  pdf_u32_t nsamples; /* product of all size[] entries, number of samples per output dimension */
  
  pdf_u32_t *k; /* base index, never out of range */
  double *wm;
  double *w0;
  double *w1;
  double *w2; /* precalculated weights, do not index into y[] if zero */
  
  pdf_real_t *y; /* sample table */
  
  /* not used for recursive variants */
  pdf_real_t *r; /* reduction buffer */
  pdf_real_t *y1; /* spline derivatives */
  pdf_real_t *d; /* tmp buffer for diagonal of spline eq */
  pdf_real_t *b; /* tmp buffer for right hand side of spline eq */
};

struct pdf_fp_func_2_s
{
  pdf_real_t *c0;
  pdf_real_t *c1;
  pdf_real_t N;
};

struct pdf_fp_func_3_s
{
  pdf_u32_t k;
  /* number of functions 
   * m = 1 for all component functions 
   * fixed n for all component functions 
   */
  pdf_fp_func_t *functions;
  pdf_real_t *bounds; /* bounds[0] == domain[0], bounds[k] == domain[1] */
  double *encode; /* mapping of domains */
};

struct pdf_fp_func_4_s
{
  pdf_char_t *opcodes;
  pdf_u32_t n_opcodes;

  /* debug info */
  /* translate opcodes offset to code offset */
  pdf_u32_t (*debug_off)[2]; 
  pdf_size_t debug_size;
};

struct pdf_fp_func_s
{
  pdf_i32_t type;
  pdf_u32_t m;
  pdf_u32_t n;
  pdf_real_t *domain;
  pdf_real_t *range;
  pdf_i32_t init;
  pdf_status_t (*eval) (pdf_fp_func_t fun,
                        const pdf_real_t in[],
                        pdf_real_t out[],
                        pdf_fp_func_debug_t * debug);
  union 
  {
    struct pdf_fp_func_0_s t0;
    struct pdf_fp_func_2_s t2;
    struct pdf_fp_func_3_s t3;
    struct pdf_fp_func_4_s t4;
  } u;
};

/* BEGIN PUBLIC */

pdf_status_t pdf_fp_func_0_new (pdf_u32_t m,
                                pdf_u32_t n,
                                const pdf_real_t domain[],
                                const pdf_real_t range[],
                                pdf_u32_t size[],
                                pdf_u32_t bps,
                                pdf_u32_t order,
                                const pdf_real_t encode[],
                                const pdf_real_t decode[],
                                pdf_char_t *samples,
                                pdf_size_t samples_size,
                                pdf_fp_func_t *function);

pdf_status_t pdf_fp_func_2_new (pdf_u32_t m,
                                pdf_u32_t n,
                                const pdf_real_t domain[],
                                const pdf_real_t range[],
                                pdf_real_t N,
                                pdf_real_t c0[],
                                pdf_real_t c1[],
                                pdf_fp_func_t *function);

pdf_status_t pdf_fp_func_3_new (pdf_u32_t m,
                                pdf_u32_t n,
                                const pdf_real_t domain[],
                                const pdf_real_t range[],
                                pdf_u32_t k,
                                pdf_fp_func_t *functions,
                                pdf_real_t bounds[],
                                const pdf_u32_t encode[],
                                pdf_fp_func_t *function);

pdf_status_t pdf_fp_func_4_new (pdf_u32_t m,
                                pdf_u32_t n,
                                pdf_real_t domain[],
                                pdf_real_t range[],
                                pdf_char_t *code,
                                pdf_size_t code_size,
                                pdf_size_t *error_at,
                                pdf_fp_func_t *function);

pdf_status_t pdf_fp_func_eval (pdf_fp_func_t function,
                               const pdf_real_t in[],
                               pdf_real_t out[],
                               pdf_fp_func_debug_t * debug);

pdf_status_t pdf_fp_func_get_bounds (const pdf_fp_func_t function,
                                     pdf_i32_t *in_dimensions,
                                     pdf_i32_t *out_dimensions);

pdf_status_t pdf_fp_func_destroy (pdf_fp_func_t function);

/* END PUBLIC */

#endif /* pdf-fp-func.h */

/* End of pdf-fp-func.h */

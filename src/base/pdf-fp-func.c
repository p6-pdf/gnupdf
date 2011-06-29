/* -*- mode: C -*-
 *
 *       File:         pdf-fp-func.c
 *       Date:         Sun Nov 30 18:46:06 2008
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

#include <config.h>

#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>

#include <pdf-error.h>
#include <pdf-alloc.h>
#include <pdf-stm.h>
#include <pdf-token-reader.h>
#include <pdf-fp-func.h>

static pdf_real_t *
read_type0_sample_table (pdf_char_t *buf, pdf_size_t buf_size,
                         pdf_u32_t bps, pdf_u32_t nsamples , pdf_u32_t n);

static pdf_i32_t get_token (pdf_token_reader_t *reader,
                            double             *literal,
                            pdf_size_t         *token_begin);
static inline void setmap (double map[2],
                           const pdf_real_t to[2],
                           const pdf_real_t from[2]);

static pdf_status_t pdf_eval_spline (pdf_fp_func_t fun,
                                     const pdf_real_t t[],
                                     pdf_real_t out[],
                                     pdf_fp_func_debug_t * debug);

static pdf_status_t pdf_eval_linear (pdf_fp_func_t fun,
                                     const pdf_real_t t[],
                                     pdf_real_t out[],
                                     pdf_fp_func_debug_t * debug);

static pdf_status_t pdf_eval_exponential (pdf_fp_func_t t,
                                          const pdf_real_t in[],
                                          pdf_real_t out[],
                                          pdf_fp_func_debug_t * debug);

static pdf_status_t pdf_eval_stitch (pdf_fp_func_t t,
                                     const pdf_real_t in[],
                                     pdf_real_t out[],
                                     pdf_fp_func_debug_t * debug);

static pdf_status_t pdf_eval_type4(pdf_fp_func_t t,
                                   const pdf_real_t in[],
                                   pdf_real_t out[],
                                   pdf_fp_func_debug_t * debug);

typedef enum
  {
    OPC_ret, /* end of function, return result */
    OPC_jnt, /* jump if not true , offset follows  */
    OPC_jmp, /* jump always, offset follows   */
    OPC_lit_i, /* integer literal follows. */
    OPC_lit_r, /* real literal follows. */
    OPC_begin, /* token opening brace */
    OPC_end,   /* token closing brace */
    OPC_if,    /* token */
    OPC_ifelse,/* token */
    OPC_bad,   /* bad token */

    OPC_floor,
    OPC_ceiling,
    OPC_truncate,
    OPC_round,

    OPC_abs,
    OPC_sqrt,
    OPC_atan,
    OPC_sin,
    OPC_cos,
    OPC_exp,
    OPC_log,
    OPC_ln,
    OPC_neg,
    OPC_cvi,
    OPC_cvr,

    OPC_mod,
    OPC_div,
    OPC_add,
    OPC_idiv,
    OPC_bitshift,
    OPC_mul,
    OPC_sub,

    OPC_true,
    OPC_false,
    OPC_not,
    OPC_and,
    OPC_or,
    OPC_xor,

    OPC_eq,
    OPC_le,
    OPC_ge,
    OPC_lt,
    OPC_gt,
    OPC_ne,


    OPC_copy,
    OPC_exch,
    OPC_pop,
    OPC_dup,
    OPC_index,
    OPC_roll
  } PDF_TYPE4_OPC;


/*
 * Public functions
 */

pdf_status_t
pdf_fp_func_0_new (pdf_u32_t m,
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
                   pdf_fp_func_t *function)
{
  pdf_status_t ret;
  pdf_fp_func_t f;
  pdf_u32_t i, j, k;
  pdf_u32_t nsamples;
  pdf_u32_t nsamples_limit;

  ret = PDF_OK;

  /* Common data */
  f = pdf_alloc (sizeof(struct pdf_fp_func_s));
  f->m = m;
  f->n = n;
  f->type = 0;

  /* Specific data */
  f->u.t0.size = size;

  /* Get the number of samples */
  nsamples = 1;
  nsamples_limit = 1U<<28/f->n; /* insane values */
  for (i = 0; i < m; i++)
    {
      if (f->u.t0.size[i] > nsamples_limit/nsamples) /* avoid overflow */
        {
          return PDF_EOVERFLOW;
        }
      nsamples *= f->u.t0.size[i];
    }
  f->u.t0.nsamples = nsamples;

  /* Manage the bits per sample value */
  switch (bps)
    {
    case 1:
    case 2:
    case 4:
    case 8:
    case 12:
    case 16:
    case 24:
    case 32: /* possible loss of precision in conversion to pdf_real_t */
      {
        break;
      }
    default:
      {
        /* ths implementation allows any value between 1 and 32 */
        /* the spec does not */
        return PDF_EBADDATA;
      }
    }

  f->u.t0.m = f->m;
  f->u.t0.n = f->n;

  if (f->u.t0.m > 12 ) /* sanity check */
    {
      return PDF_EMTOOBIG;
    }

  f->u.t0.y = read_type0_sample_table (samples,
                                       samples_size,
                                       bps,
                                       nsamples,
                                       f->n);
  if (!f->u.t0.y)
    {
      return PDF_EBADSAMPLES;
    }

  if (encode != NULL)
    {
      f->u.t0.encode = pdf_alloc (2 * f->m * sizeof(f->u.t0.encode[0]));
      for (k = 0; k < f->m; k++)
        {
          setmap(f->u.t0.encode + (2 * k),
                 encode + (2 * k),
                 f->domain + (2 * k));
        }
    }
  else
    {
      /* Use the default encoding array */
      pdf_real_t enc[2];

      enc[0] = 0;
      for (k = 0; k < f->m; k++)
        {
          enc[1] = f->u.t0.size[k] - 1;
          setmap(f->u.t0.encode + 2*k, enc, f->domain+2*k);
        }
    }

  if (decode != NULL)
    {
      f->u.t0.decode = pdf_alloc (2 * f->n * sizeof(f->u.t0.decode[0]));
      for (j = 0; j < f->n; j++)
        {
          setmap (f->u.t0.decode + (2 * j),
                  f->range + (2 * j),
                  decode + (2 * j));
        }
    }
  else
    {
      /* Use the default decoding array */
      pdf_real_t dec[2];

      dec[0] = 0;
      dec[1] = (bps < 32) ? ((1U<<bps)-1U) : 0xFFFFFFFFU;
      for (j = 0; j < f->n; j++)
        {
          setmap (f->u.t0.decode + (2 * j),
                  f->range + (2 * j),
                  dec);
        }
    }

  f->u.t0.k  = pdf_alloc (m * sizeof(f->u.t0.k[0]));
  f->u.t0.w0  = pdf_alloc (m * sizeof(f->u.t0.w0[0]));
  f->u.t0.w1  = pdf_alloc (m * sizeof(f->u.t0.w1[0]));


  if (order == 3)
    {
      f->u.t0.wm  = pdf_alloc (m * sizeof(f->u.t0.wm[0]));
      f->u.t0.w2  = pdf_alloc (m * sizeof(f->u.t0.w2[0]));
      f->eval = pdf_eval_spline;
    }
  else
    {
      f->eval = pdf_eval_linear;
    }

  *function = f;
  return ret;
}

pdf_status_t
pdf_fp_func_2_new (pdf_u32_t m,
                   pdf_u32_t n,
                   const pdf_real_t domain[],
                   const pdf_real_t range[],
                   pdf_real_t N,
                   pdf_real_t c0[],
                   pdf_real_t c1[],
                   pdf_fp_func_t *function)
{
  pdf_status_t ret;
  pdf_fp_func_t f;

  ret = PDF_OK;

  /* Common data */
  f = pdf_alloc (sizeof(struct pdf_fp_func_s));
  f->m = m;
  f->n = n;
  f->type = 2;

  /* Specific data */
  f->u.t2.N = N;
  f->u.t2.c0 = c0;
  f->u.t2.c1 = c1;

  f->eval = pdf_eval_exponential;

  *function = f;
  return ret;
}

pdf_status_t
pdf_fp_func_3_new (pdf_u32_t m,
                   pdf_u32_t n,
                   const pdf_real_t domain[],
                   const pdf_real_t range[],
                   pdf_u32_t k,
                   pdf_fp_func_t *functions,
                   pdf_real_t bounds[],
                   const pdf_u32_t encode[],
                   pdf_fp_func_t *function)
{
  pdf_status_t ret;
  pdf_fp_func_t f;
  pdf_u32_t i;

  ret = PDF_OK;

  /* Common data */
  f = pdf_alloc (sizeof(struct pdf_fp_func_s));
  f->m = m;
  f->n = n;
  f->type = 3;

  /* Specific data */
  f->u.t3.k = k;
  f->u.t3.functions = functions;
  f->u.t3.bounds = bounds;

  f->u.t3.bounds = pdf_alloc ((1+k) * sizeof(f->u.t3.bounds[0]));
  f->u.t3.bounds[0] = f->domain[0];
  for (i = 1; i < k; i++)
    {
      f->u.t3.bounds[i] = bounds[i - 1];
    }
  f->u.t3.bounds[i] = f->domain[1];

  f->u.t3.encode = pdf_alloc (2 * k * sizeof(f->u.t3.encode[0]));
  for (i = 0; i < k; i++)
    {
      pdf_real_t enc[2];

      enc[0] = encode[2 * i];
      enc[1] = encode[2 * i + 1];
      setmap(f->u.t3.encode + 2 * i,
             enc,
             f->u.t3.bounds + i);
    }

  f->eval = pdf_eval_stitch;

  *function = f;
  return ret;
}

#define BRACES_MAX_LEVEL 32

pdf_status_t
pdf_fp_func_4_new (pdf_u32_t m,
                   pdf_u32_t n,
                   pdf_real_t domain[],
                   pdf_real_t range[],
                   pdf_char_t *code,
                   pdf_size_t code_size,
                   pdf_size_t *error_at,
                   pdf_fp_func_t *function)
{
  pdf_fp_func_t f;
  pdf_u32_t off[BRACES_MAX_LEVEL];
  pdf_u32_t braces_pos[BRACES_MAX_LEVEL];
  pdf_char_t *op;
  pdf_i32_t at;
  pdf_i32_t to;
  pdf_i32_t alloc;
  pdf_i32_t bsp;
  pdf_i32_t blevel;             /* braces level depth */
  double lit;
  pdf_i32_t opc;
  pdf_size_t beg_pos;               /* beginning of current token */
  pdf_status_t ret;
  pdf_u32_t (*debug_off)[2];
  pdf_size_t debug_size;
  pdf_size_t debug_alloc;
  size_t s_domain = sizeof(pdf_real_t)*m*2;
  size_t s_range = sizeof(pdf_real_t)*n*2;

  pdf_error_t *inner_error = NULL;
  pdf_stm_t *reader_stm;
  pdf_token_reader_t *reader;

  /* Common data */
  f = pdf_alloc (sizeof(struct pdf_fp_func_s));
  f->m = m;
  f->n = n;
  f->type = 4;
  f->domain = pdf_alloc (s_domain);
  f->range = pdf_alloc (s_range);
  memcpy (f->domain, domain, s_domain);
  memcpy (f->range, range, s_range);

  /* Specific data */
  beg_pos = 0;
  alloc = 64;
  op = pdf_alloc (alloc);
  debug_alloc = 64;
  debug_size = 0;
  debug_off = pdf_alloc (sizeof(*debug_off) * debug_alloc);

  /* Initialize the token reader */
  reader_stm = pdf_stm_mem_new ((pdf_uchar_t *)code,
                                code_size,
                                code_size, /* Use the default cache size */
                                PDF_STM_READ,
                                &inner_error);
  if (!reader_stm)
    {
      /* TODO: Propagate error */
      pdf_error_destroy (inner_error);
      ret = PDF_ERROR;
      goto fail;
    }

  reader = pdf_token_reader_new (reader_stm, &inner_error);
  if (!reader)
    {
      /* TODO: Propagate error */
      pdf_error_destroy (inner_error);
      ret = PDF_ERROR;
      goto fail;
    }

  opc = get_token (reader, &lit, &beg_pos);
  if (opc != OPC_begin)
    {
      ret = PDF_ENOWRAP;
      goto fail;
    }

  blevel = 1;
  bsp = 0;
  at  = 0;
  for(;;)
    {
      if (bsp >= BRACES_MAX_LEVEL)
        {
          ret = PDF_ETOODEPTH;
          goto fail;
        }

      if (at+1+sizeof(lit) >= alloc)
        {
          alloc *= 2;
          op = pdf_realloc (op,alloc);
        }

      if (debug_size >= debug_alloc)
        {
          debug_alloc *= 2;
          debug_off = pdf_realloc (debug_off, sizeof(*debug_off) * debug_alloc);
        }

      opc = get_token (reader, &lit, &beg_pos);
      if (opc < 0)
        {
          ret = PDF_EEOF;
          goto fail;
        }
      switch ((PDF_TYPE4_OPC)opc)
        {
        case OPC_lit_i:
        case OPC_lit_r:
          {
            debug_off[debug_size][0] = at;
            debug_off[debug_size][1] = beg_pos;
            debug_size++;

            op[at++] = opc;
            memcpy (op+at, &lit, sizeof(lit));
            at += sizeof (lit);
            break;
          }
        case OPC_begin:
          {
            blevel++;
            off[bsp] = at; /* backpatched by if/ifelse */
            braces_pos[bsp] = beg_pos;
            bsp++;

            debug_off[debug_size][0] = at;
            debug_off[debug_size][1] = beg_pos;
            debug_size++;

            op[at] = OPC_begin; /* OPC_error */
            at += 1+sizeof(at);

            break;
          }
        case OPC_end:
          {
            blevel--;

            if (blevel)
              {
                off[bsp] = at;
                braces_pos[bsp] = beg_pos;
                bsp++;
              }
            else
              {
                void *r;

                debug_off[debug_size][0] = at;
                debug_off[debug_size][1] = beg_pos;
                debug_size++;

                op[at++] = OPC_ret;

                if (get_token (reader, &lit, &beg_pos) >= 0)
                  {
                    ret = PDF_ENOWRAP;
                    goto fail;
                  }

                if (bsp)
                  {
                    beg_pos = braces_pos[--bsp]; /* reporting error offset */
                    ret = PDF_EMISSIF;
                    goto fail;
                  }

                /* memory is transferred to f, not freed here */
                debug_off = pdf_realloc (debug_off, sizeof(*debug_off) * debug_alloc);

                r = pdf_realloc (op,at);
                if (r)
                  {
                    op = r;
                  }

                f->u.t4.opcodes = op;
                f->u.t4.n_opcodes = at;
                f->u.t4.debug_off = debug_off;
                f->u.t4.debug_size = debug_size;

                f->eval = pdf_eval_type4;

                goto success;
              }
            break;
          }
        case OPC_if:
          {
            if (bsp < 2 || at != off[--bsp])
              {
                ret = PDF_EMISSBODY;
                goto fail;
              }

            debug_off[debug_size][0] = --bsp;
            debug_off[debug_size][1] = beg_pos;
            debug_size++;

            op[off[bsp]] = OPC_jnt;
            to = at - 1;
            memcpy(op+1+off[bsp],&to,sizeof(to));
            break;
          }
        case OPC_ifelse:
          {
            if (bsp < 4 || at != off[--bsp])
              {
                ret = PDF_EMISSBODY;
                goto fail;
              }

            debug_off[debug_size][0] = --bsp;
            debug_off[debug_size][1] = beg_pos;
            debug_size++;

            op[off[bsp]] = OPC_jmp;
            to = at-1;
            memcpy(op+off[bsp]+1,&to,sizeof(to));

            to = off[bsp--];
            if (to != off[bsp--])
              {
                ret = PDF_EMISSBODY;
                goto fail;
              }
            to += sizeof(to);
            op[off[bsp]] = OPC_jnt;
            memcpy(op+off[bsp]+1,&to,sizeof(to));
            break;
          }
        case OPC_bad:
          {
            ret = PDF_EBADOP;
            goto fail;
            break;
          }
        default:
          {
            debug_off[debug_size][0] = at;
            debug_off[debug_size][1] = beg_pos;
            debug_size++;

            op[at++] = opc;
            break;
          }
        }
    }
  /* not reached */

 fail:
  pdf_dealloc (op);
  pdf_dealloc (debug_off);

  if (error_at != NULL)
    {
      *error_at = beg_pos;
    }

  *function = NULL;
  return ret;

 success:
  *function = f;
  return PDF_OK;
}

pdf_status_t
pdf_fp_func_eval (pdf_fp_func_t function,
                  const pdf_real_t in[],
                  pdf_real_t out[],
                  pdf_fp_func_debug_t * result)
{
  return function->eval(function, in, out, result);
}

pdf_status_t
pdf_fp_func_get_bounds (const pdf_fp_func_t function,
                        pdf_i32_t *in_dimensions,
                        pdf_i32_t *out_dimensions)
{
  *in_dimensions = function->m;
  *out_dimensions = function->n;

  return PDF_OK;
}

pdf_status_t
pdf_fp_func_destroy (pdf_fp_func_t t)
{
  pdf_u32_t i;

  if (!t)
    {
      /* pdf_destroy_func(0) is valid. */
      /* this simplifies cleanup after a 'goto fail' */
      return PDF_OK;
    }

  pdf_dealloc (t->domain);
  pdf_dealloc (t->range);

  switch(t->type)
    {
    case 0:
      pdf_dealloc (t->u.t0.size);
      pdf_dealloc (t->u.t0.y);
      pdf_dealloc (t->u.t0.y1);
      pdf_dealloc (t->u.t0.r);

      pdf_dealloc (t->u.t0.wm);
      pdf_dealloc (t->u.t0.w0);
      pdf_dealloc (t->u.t0.w1);
      pdf_dealloc (t->u.t0.w2);

      pdf_dealloc (t->u.t0.b);
      pdf_dealloc (t->u.t0.d);
      pdf_dealloc (t->u.t0.encode);
      pdf_dealloc (t->u.t0.decode);
      break;

    case 2:
      pdf_dealloc (t->u.t2.c0);
      pdf_dealloc (t->u.t2.c1);
      break;

    case 3:
      for (i = 0; i < t->u.t3.k; i++)
        {
          pdf_fp_func_destroy (t->u.t3.functions[i]);
        }
      pdf_dealloc (t->u.t3.functions);
      pdf_dealloc (t->u.t3.bounds);
      pdf_dealloc (t->u.t3.encode);
      break;
    case 4:
      pdf_dealloc (t->u.t4.opcodes);
      pdf_dealloc (t->u.t4.debug_off);
      break;
    }

  pdf_dealloc (t);

  return PDF_OK;
}

/*
 * Private functions
 */

/* On type 0 functions.
 * Linear interpolation works the obvious way in 1D,
 * and there is a simple,recursive generalization to multilinear interpolation.
 * any performance penalty would show up only for large m (# of inputs).
 *
 * True splines have C2 continuity, at least in the one-dimensional case.
 * Solving the linear equations is easy (see below), and the results may be cached.
 * There is, however, no straightforward generalization to m > 1.
 * An iterative solution would require to solve many systems of linear equations.
 * Only the outermost results would be cacheable.
 * It does not feel right.
 *
 * If we use Catmull-Rom splines, which are common in computer graphics,
 * these difficulties vanish.
 * They depend on four surrounding samples instead of two,
 * but the algorithm is almost the same for order 1 and order 3.
 *
 */

static inline double
clip(double x, const pdf_real_t r[2])
/* HF: clip() will pass NaNs */
{
  if (x < r[0])
    x = r[0];
  else if (x > r[1])
    x = r[1];
  return x;
}

static inline void
setmap(double map[2], const pdf_real_t to[2] , const pdf_real_t from[2])
{
  map[0] = (to[1] - to[0]) / (from[1] - from[0]);
  map[1] = to[0] - map[0] * from[0];
}

#if 0
/* True Splines, see comments above */
/* build two and expect to throw away one of them */
/* the eqn sover is not used anymore */


static pdf_real_t
eval_spline(pdf_real_t x,
            pdf_u32_t nsamples,
            const pdf_real_t y[],
            const pdf_real_t y1[])
{
  pdf_real_t t;
  pdf_real_t c,d;
  pdf_u32_t i;

  if (x <= 0)
    return y[0];
  else if (x >= nsamples-1)
    return y[nsamples-1];

  t = pdf_fp_floor (x);
  i = (pdf_u32_t)t;
  t = x-t;

  PDF_ASSERT (i < nsamples);

  if (nsamples < 4)
    {
      return y[i] + y1[i]*t;
    }

  /* From the spec:
   * The /Size for an input dimension can be 1, in which case all input values in
   * that dimension will be mapped to the single allowed value. If Size is less than 4,
   * cubic spline interpolation is not possible and Order 3 will be ignored if specified.
   * This seems to exclude the use of quadratic interpolation if nsamples = 3
   * what happens in mixed cases ?
   * */

  c =  3*(y[i+1]-y[i]) - (2*y1[i] + y1[i+1]);
  d = -2*(y[i+1]-y[i]) + (  y1[i] + y1[i+1]);

  return ((d*t + c) *t + y1[i]) *t + y[i];
}

static void
solve_spline(
             pdf_u32_t nsamples,
             const pdf_real_t y[],
             pdf_real_t y1[],
             pdf_real_t d[],
             pdf_real_t b[])
/* calculate the vector y1[] of derivatives for a not-a-knot spline
 * by solving a tridiagonal system.
 * d[] and b[] are buffers for diagonal and right side, respectively
 */
{
  pdf_u32_t i;
  if (nsamples < 4)
    {
      for (i = 0; i < nsamples-1; i++)
        y1[i] = y[i+1] - y[i];
      /* y1[nsamples-1] is not used in this case */
    }
  else
    {
      b[0] = (-5*y[0] + 4*y[1] + y[2])/4;
      d[0] = 0.5;
      for (i = 1; i < nsamples-1; i++)
        {
          b[i] = 3*(y[i+1] - y[i-1]);
          d[i] = 4;
        }
      b[i] = (5*y[i] - 4*y[i-1] - y[i-2])/4;
      d[i] = 0.5;

      /* forward */
      for (i = 0; i < nsamples-1; i++)
        {
          b[i+1] -= b[i]/d[i];
          d[i+1] -= 1/d[i];
        }

      /* backward */
      for (i = nsamples-1; i > 0; --i)
        {
          y1[i] = b[i]/d[i];
          b[i-1] -= y1[i];
        }
      y1[0] = b[0]/d[0];
    }
}


static void pdf_spline_init(struct pdf_func0_s *p)
/* precalculate spline coefficients, final initialization steps */
{
  pdf_u32_t i,j,k,l;
  pdf_u32_t max;

  max = 0;
  for (k = 0; k < p->m; k++)
    {
      if (max < p->size[k])
        max = p->size[k];
    }

  p->b = pdf_alloc (max * sizeof(p->b[0]));
  p->d = pdf_alloc (max * sizeof(p->d[0]));

  p->y1 = pdf_alloc (sizeof(p->y1[0]) * p->n * p->nsamples);

  l = p->nsamples/p->size[0];

  p->r  = pdf_alloc (2*sizeof(p->r[0]) * l);

  for (j = 0; j < p->n; j++)
    {
      for (i = 0; i < l; i++)
        {
          solve_spline(p->size[0],
                       p->y  + j*p->nsamples + i*p->size[0],
                       p->y1 + j*p->nsamples + i*p->size[0], p->b , p->d);
        }
    }
}

static pdf_status_t
pdf_eval_spline (pdf_fp_func_t fun,
                 const pdf_real_t t[],
                 pdf_real_t out[])
{
  struct pdf_func0_s *p;
  pdf_u32_t i,j,l,k;
  pdf_real_t *r,*r1;
  pdf_real_t x,y;

  p = &fun->u.t0;


  l = p->nsamples/p->size[0];

  r = p->r;
  r1 = p->r + l;

  for (j = 0; j < p->n; j++)
    {
      k = 0;
      x = p->encode[0] * t[0] + p->encode[1];
      for (i = 0; i < l; i++)
        {
          r[i] = eval_spline(x , p->size[k],
                             p->y + (j * p->nsamples + i*p->size[k]) ,
                             p->y1+ (j * p->nsamples + i*p->size[k]) );
        }

      for (k = 1; k < p->m; k++)
        {
          l /= p->size[k];

          for (i = 0; i < l; i++)
            solve_spline(p->size[k], r+i*p->size[k],r1+i*p->size[k],p->b,p->d);

          x = p->encode[2*k] * t[k] + p->encode[2*k+1];
          for (i = 0; i < l; i++)
            {
              r[i] = eval_spline(x , p->size[k], r+i*p->size[k],r1+i*p->size[k]);
            }
          /* this reuse of r works! */
        }


      y = r[0];
      y = fun->u.t0.decode[2*j] * y + fun->u.t0.decode[2*j+1];

      if (fun->range)
        y = clip(y,fun->range+2*j);
      out[j] = y;
    }

  return PDF_OK;
}



static void
pdf_linear_init(struct pdf_func0_s *p)
{
  p->r  = pdf_alloc (sizeof(p->r[0]) * (1U << p->m));
}


static pdf_status_t
pdf_eval_linear (pdf_fp_func_t fun,
                 const pdf_real_t t[],
                 pdf_real_t out[])
/* the original code (postscript) was almost certainly more stack oriented */
{
  pdf_u32_t xcp;
  double x,y;
  struct pdf_func0_s *p;
  pdf_u32_t m;
  pdf_u32_t i,j,s,k,i0;
  pdf_u32_t cc;
  double t0[TYPE0_MAX_DIM];
  double t1[TYPE0_MAX_DIM];

  p = &fun->u.t0;
  m = fun->m;

  PDF_ASSERT (m <= TYPE0_MAX_DIM);

  xcp = 0;
  i0 = 0;
  cc = 1;
  for (k = 0; k < m; k++)
    {
      x = t[k] * p->encode[2*k] + p->encode[2*k+1];

      if (!(x < p->size[k]-1)) /* catch NaN, and size [k] == 1, too */
        {
          i = p->size[k]-1;
          xcp |= (1U<<k); /* exceptional cases */
          t1[k] = 0;
          t0[k] = 1;
        }
      else if (x <= 0) /* else !!! */
        {
          i  = 0;
          t1[k] = 0;
          t0[k] = 1;
        }
      else
        {
          i = (pdf_u32_t) pdf_fp_floor (x);
          t1[k] = x - pdf_fp_floor (x);
          t0[k] = (1 - t1[k]);
        }
      i0 += cc * i;
      cc *= p->size[k];
    }
  j = 0;

  for (j = 0; j < p->n; j++)
    {
      for (i = 0; i < (1U<<m); i++)
        /* smells like recursion */
        {
          s = i0;
          cc = 1;
          for (k = 0; k < m; k++)
            {
              if (i & (~xcp) & (1U<<k))
                s += cc;
              cc *= p->size[k];
            }
          p->r[i] = p->y[s];
          printf("\n\tsample[%u] = [%u]",i,s);
        }
      printf("\n");

      for (k = 0; k < m; k++) /* reduction steps */
        {
          /* i is dominant loop index. */
          for (i = 0; i < 1U<<(m-k-1); i++)
            {
              p->r[i] = t0[k]*p->r[2*i] + t1[k] * p->r[2*i+1];
            }
        }

      y = p->r[0];
      y = fun->u.t0.decode[2*j] * y + fun->u.t0.decode[2*j+1];

      if (fun->range)
        y = clip(y,fun->range+2*j);

      out[j] = y;
    }

  return PDF_OK;
}

#endif
/* TRUE_C2_NOT_A_KNOT_SPLINES */




static double
linear_interpolation(pdf_u32_t i,
                     const double w0[], const double w1[],
                     const pdf_u32_t k[],
                     const pdf_real_t y[],
                     pdf_u32_t stride,
                     const pdf_u32_t size[])
/* w0[i] and w1[i] are weights for samples surrounding samples k[i] and k[i+1] in the ith dimension */
/* cubic interpolation will need four weights */
/* Samples with weight 0 may have an index out of range */
{
  pdf_u32_t j;
  j = k[i];
  stride /= size[i];

  if (w1[i]) /* zero indicates a possibly invalid array index */
    {
      PDF_ASSERT (j+1 < size[i]);
      if (i == 0)
        return w0[i] * y[j] + w1[i] * y[j+1];
      else
        return w0[i] * linear_interpolation(i-1,w0,w1,k,y+j*stride,stride,size)
          + w1[i] * linear_interpolation(i-1,w0,w1,k,y+(j+1)*stride,stride,size);
    }
  else
    {
      if (i == 0)
        return y[j];
      else
        return linear_interpolation(i-1,w0,w1,k,y+j*stride,stride,size);
    }
}

static pdf_status_t
pdf_eval_linear (pdf_fp_func_t fun,
                 const pdf_real_t in[],
                 pdf_real_t out[],
                 pdf_fp_func_debug_t * debug)
{
  pdf_u32_t i,j;
  double t;
  double y;
  for (i = 0; i < fun->m; i++)
    {
      t = in[i] * fun->u.t0.encode[2*i] + fun->u.t0.encode[2*i+1];
      if (isnan(t))
        {
          return -1;
        }
      if (t >= fun->u.t0.size[i]-1)
        {
          fun->u.t0.k[i] = fun->u.t0.size[i]-1;
          fun->u.t0.w0[i] = 1;
          fun->u.t0.w1[i] = 0;
        }
      else if (t <= 0)
        {
          fun->u.t0.k[i] =  0;
          fun->u.t0.w0[i] = 1;
          fun->u.t0.w1[i] = 0;
        }
      else
        {
          fun->u.t0.k[i] =  (pdf_u32_t) pdf_fp_floor (t);
          t = t - pdf_fp_floor (t);
          fun->u.t0.w0[i] = 1-t;
          fun->u.t0.w1[i] = t;
        }
    }


  for (j = 0; j < fun->n; j++)
    {
      y = linear_interpolation(fun->m-1,fun->u.t0.w0,fun->u.t0.w1,fun->u.t0.k,
                               &fun->u.t0.y[j*fun->u.t0.nsamples], fun->u.t0.nsamples, fun->u.t0.size);

      y = fun->u.t0.decode[2*j] * y + fun->u.t0.decode[2*j+1];
      if (fun->range)
        y = clip(y,fun->range+2*j);
      if (isnan(y))
        {
          return -1;
        }
      out[j] = y;
    }

  return PDF_OK;
}



static double
spline_interpolation(pdf_u32_t i,
                     const double wm[], const double w0[], const double w1[], const double w2[],
                     const pdf_u32_t k[], const pdf_real_t y[], pdf_u32_t stride, const pdf_u32_t size[])
/* Catmull-Rom splines, cf wikipedia or almost any texbook on CG */
{
  pdf_u32_t j;
  double sum;
  stride /= size[i];

  j = k[i];
  sum = 0;
  if (wm[i])
    sum += wm[i] * ((i) ? spline_interpolation(i-1,wm,w0,w1,w2,k,y+(j-1)*stride,stride,size) : y[j-1]);
  if (w0[i])
    sum += w0[i] * ((i) ? spline_interpolation(i-1,wm,w0,w1,w2,k,y+(j)*stride,stride,size) : y[j]);
  if (w1[i])
    sum += w1[i] * ((i) ? spline_interpolation(i-1,wm,w0,w1,w2,k,y+(j+1)*stride,stride,size) : y[j+1]);
  if (w2[i])
    sum += w2[i] * ((i) ? spline_interpolation(i-1,wm,w0,w1,w2,k,y+(j+2)*stride,stride,size) : y[j+2]);
  /* the caller ensures that w[*] is zero if y[j+*] does not exist */
  /* if this seems ugly, try doing it with ... if (j >= 1 && j < size[i]-2 && ... */
  return sum;
}


static pdf_status_t
pdf_eval_spline (pdf_fp_func_t fun,
                 const pdf_real_t in[],
                 pdf_real_t out[],
                 pdf_fp_func_debug_t * debug)
{
  pdf_u32_t i,j;
  double t,v;

  for (i = 0; i < fun->m; i++)
    {
      t = in[i] * fun->u.t0.encode[2*i] + fun->u.t0.encode[2*i+1];
      if (isnan(t))
        {
          return PDF_ERROR;
        }

      v = t - pdf_fp_floor (t);
      fun->u.t0.k[i] = (pdf_u32_t) pdf_fp_floor (t);
      if (t >= fun->u.t0.size[i]-1)
        {
          fun->u.t0.k[i] = fun->u.t0.size[i]-1;
          fun->u.t0.wm[i] = 0;
          fun->u.t0.w0[i] = 1;
          fun->u.t0.w1[i] = 0;
          fun->u.t0.w2[i] = 0;
        }
      else if (t+1 >= fun->u.t0.size[i]-1)
        {
          if (t-1 >= 0)
            {
              fun->u.t0.wm[i] = -0.5*v*(1-v);
              fun->u.t0.w0[i] =  1-v*v;
              fun->u.t0.w1[i] =  0.5*v*(1+v);
            }
          else /* linear */
            {
              fun->u.t0.wm[i] = 0;
              fun->u.t0.w0[i] = 1-v;
              fun->u.t0.w1[i] = v;
            }
          fun->u.t0.w2[i] = 0;
        }
      else
        {
          if (t >= 1)
            {
              fun->u.t0.wm[i] = ((-0.5 * v + 1)*v-0.5)*v;
              fun->u.t0.w0[i] = (1.5* v -2.5)*v*v+1;
              fun->u.t0.w1[i] = ((-1.5*v+2)*v+0.5)*v;
              fun->u.t0.w2[i] = (0.5*v-0.5)*v*v;
            }
          else if (t > 0)
            {
              PDF_ASSERT (fun->u.t0.k[i] == 0);
              fun->u.t0.wm[i] = 0;
              fun->u.t0.w0[i] = (0.5*v-1.5)*v +1;
              fun->u.t0.w1[i] = (2-v)*v;
              fun->u.t0.w2[i] = 0.5*(v-1)*v;
            }
          else /* no samples */
            {
              fun->u.t0.k[i] = 0;
              fun->u.t0.wm[i] = 0;
              fun->u.t0.w0[i] = 1;
              fun->u.t0.w1[i] = 0;
              fun->u.t0.w2[i] = 0;
            }
        }
    }


  for (j = 0; j < fun->n; j++)
    {
      double y;
      y = spline_interpolation(fun->m-1,
                               fun->u.t0.wm,fun->u.t0.w0,fun->u.t0.w1,fun->u.t0.w2,
                               fun->u.t0.k,&fun->u.t0.y[j*fun->u.t0.nsamples], fun->u.t0.nsamples, fun->u.t0.size);

      y = y * fun->u.t0.decode[2*j] + fun->u.t0.decode[2*j+1];
      if (fun->range)
        y = clip(y,fun->range+2*j);
      if (isnan(y))
        {
          return PDF_ERROR;
        }
      out[j] = y;
    }

  return PDF_OK;
}


static pdf_status_t
pdf_eval_exponential (pdf_fp_func_t t,
                      const pdf_real_t in[],
                      pdf_real_t out[],
                      pdf_fp_func_debug_t * debug)
{
  pdf_u32_t j;
  pdf_real_t x,y;
  pdf_real_t c1,c0;

  x = clip(in[0],t->domain);
  for (j = 0; j < t->n; j++)
    {
      c0 = (t->u.t2.c0) ? t->u.t2.c0[j] : 0.0;
      c1 = (t->u.t2.c1) ? t->u.t2.c1[j] : 1.0;
      y = c0 + pow(x,t->u.t2.N)*(c1-c0);
      if (t->range)
        y = clip(y, t->range+2*j);
      out[j] = y;
      if (isnan(y))
        {
          return PDF_ERROR;
        }
    }

  return PDF_OK;
}

static pdf_status_t
pdf_eval_stitch (pdf_fp_func_t t,
                 const pdf_real_t in[],
                 pdf_real_t out[],
                 pdf_fp_func_debug_t * debug)
{
  pdf_fp_func_t f;
  pdf_real_t x;
  pdf_u32_t lo,hi;
  pdf_u32_t i;

  PDF_ASSERT (t->domain[0] == t->u.t3.bounds[0]);
  PDF_ASSERT (t->domain[1] == t->u.t3.bounds[t->u.t3.k]);

  x = in[0];

  if (isnan(x))
    {
      return PDF_ERROR;
    }

  if (! (x >= t->domain[0])) /* NaN */
    {
      i = 0;
    }
  else if (x >= t->domain[1])
    {
      i = t->u.t3.k - 1;
      x = t->domain[1];
    }
  else
    {
      lo = 0;
      hi = t->u.t3.k - 1;
      /* k is at least 1 , one constituent function */
      while (lo <= hi)
        {
          i = lo + (hi-lo)/2;
          if (x < t->u.t3.bounds[i])
            hi = i-1;
          else if (x >= t->u.t3.bounds[i+1])
            lo = i+1;
          else
            {
              break;
            }
        }
      if (lo > hi)
        {
          return PDF_ERROR;
        }
    }

  x = x * t->u.t3.encode[2*i] + t->u.t3.encode[2*i+1];

  f = t->u.t3.functions[i];

  return f->eval(f,&x,out, debug);
}

/* ------- decoder utilities ------ */

static pdf_real_t *
read_type0_sample_table (pdf_char_t *buf,
                         pdf_size_t buf_size,
                         pdf_u32_t bps,
                         pdf_u32_t nsamples,
                         pdf_u32_t n)
{
  pdf_real_t *y;
  pdf_u32_t data,mask;
  pdf_u32_t avail;
  pdf_u32_t i;
  pdf_i32_t c;
  pdf_size_t buf_index;

  if (!n || nsamples > UINT_MAX/n)
    {
      return NULL;
    }

  buf_index = 0;
  n = n * nsamples;

  /* single precision pdf_float_t loses precision with bps 32 */
  /* this should never be visible with pdf_float_t in[] and out[] */
  y = pdf_alloc (nsamples * sizeof(*y));

  avail = 0;
  data  = 0;
  mask  = (bps >= 32) ? 0xFFFFFFFFU : ((1U<<bps)-1U);
  for (i = 0; i < nsamples; i++)
    {
      while (avail < bps)
        {
          if (buf_index >= buf_size)
            {
              goto fail;
            }

          c = buf[buf_index++];

          if (c < 0)
            {
              goto fail;
            }

          data = (data << 8) | (pdf_char_t)c;
          avail += 8;
        }
      y[i] = (data >> (avail - bps)) & mask;
      avail -= bps;
    }
  return  y;

 fail:
  pdf_dealloc (y);
  return NULL;
}

/* --- ------------------------------- --- */
/* --- type 4 (postscript) functions --- */
/* --- ------------------------------- --- */


#define NSTACK          PDF_FP_FUNC_TYPE4_STACK_SIZE
#define REP_TRUE	PDF_FP_FUNC_TYPE4_TRUE
#define REP_FALSE	PDF_FP_FUNC_TYPE4_FALSE
#define TRUNC(x)	((x) < 0 ? pdf_fp_ceil(x) : pdf_fp_floor(x))
#define INT(x)	        ((int) TRUNC(x))
#define BOOL(x)	        ((x) \
                         ? PDF_FP_FUNC_TYPE4_TRUE \
                         : PDF_FP_FUNC_TYPE4_FALSE)
#define P_INT(x)        ((x).t == INT)
#define P_BOOL(x)       ((x).t == BOOL)
#define P_NUM(x)        ((x).t != BOOL)
#define P_TRUE(x)       ((x).v == REP_TRUE)
#define P_FALSE(x)      ((x).v == REP_FALSE)
#define NUM_TYPE(x, y)  ((P_INT(x) && P_INT(y)) ? INT : REAL)


static pdf_status_t
pdf_eval_type4 (pdf_fp_func_t t,
                const pdf_real_t in[],
                pdf_real_t out[],
                pdf_fp_func_debug_t * debug)
{
  struct t_stack {
    double v;
    enum { BOOL, INT, REAL } t;
  } stack[NSTACK+2];

  pdf_char_t *op;
  pdf_u32_t n;
  double tmp;
  struct t_stack tmp_s;
  pdf_i32_t sp;
  pdf_i32_t pc;
  pdf_fp_func_debug_t debug_info;
  pdf_i32_t aux;

  op  = t->u.t4.opcodes;
  n   = t->u.t4.n_opcodes;

  for (sp = 0; sp < t->m; sp++)
    {
      stack[sp].v = clip(in[sp], t->domain + 2*sp);
      stack[sp].t = REAL;
    }
  sp--;
  for (pc = 0; pc < n; pc++)
    {
      switch ((PDF_TYPE4_OPC) op[pc])
        {
        case OPC_ret:
          if (sp+1 < t->n) goto stack_underflow;
          if (sp+1 > t->n) goto stack_error;
          for (sp = 0; sp < t->n; sp++)
            {
              if (!P_NUM(stack[sp])) goto type_error;
              out[sp] = clip(stack[sp].v, t->range + 2*sp);
            }
          return PDF_OK;
          break;
        case OPC_lit_i:
          if (sp >= NSTACK) goto stack_overflow;
          sp++;
          memcpy(&stack[sp].v, op+pc+1, sizeof(stack[0].v));
          stack[sp].t = INT;
          pc += sizeof(stack[0].v);
          break;
        case OPC_lit_r:
          if (sp >= NSTACK) goto stack_overflow;
          sp++;
          memcpy(&stack[sp].v, op+pc+1, sizeof(stack[0].v));
          stack[sp].t = REAL;
          pc += sizeof(stack[0].v);
          break;
        case OPC_jmp:
          memcpy(&pc,op+pc+1,sizeof(pc));
          break;
        case OPC_jnt:
          if (sp < 0) goto stack_underflow;
          if (!P_BOOL(stack[sp])) goto type_error;
          if (P_TRUE(stack[sp]))
            pc += sizeof(pc);
          else if (P_FALSE(stack[sp]))
            memcpy(&pc,op+pc+1,sizeof(pc));
          else goto type_error;
          sp--;
          break;
        case OPC_dup:
          if (sp < 0) goto stack_underflow;
          if (sp >= NSTACK) goto stack_overflow;
          stack[sp+1] = stack[sp];
          sp++;
          break;
        case OPC_pop:
          if (sp < 0) goto stack_underflow;
          sp--;
          break;
        case OPC_exch:
          if (sp < 1) goto stack_underflow;
          tmp_s = stack[sp-1];
          stack[sp-1] = stack[sp];
          stack[sp] = tmp_s;
          break;
        case OPC_sin:
          if (sp < 0) goto stack_underflow;
          if (!P_NUM(stack[sp])) goto type_error;
          stack[sp].v = pdf_fp_sin ((180/PDF_PI)*stack[sp].v);
          stack[sp].t = REAL;
          break;
        case OPC_cos:
          if (sp < 0) goto stack_underflow;
          if (!P_NUM(stack[sp])) goto type_error;
          stack[sp].v = pdf_fp_cos ((180/PDF_PI)*stack[sp].v);
          stack[sp].t = REAL;
          break;
        case OPC_neg:
          if (sp < 0) goto stack_underflow;
          if (!P_NUM(stack[sp])) goto type_error;
          stack[sp].v = - stack[sp].v;
          break;
        case OPC_abs:
          if (sp < 0) goto stack_underflow;
          if (!P_NUM(stack[sp])) goto type_error;
          stack[sp].v = pdf_fp_abs (stack[sp].v);
          break;
        case OPC_exp:
          if (sp < 1) goto stack_underflow;
          if (!P_NUM(stack[sp-1]) || !P_NUM(stack[sp])) goto type_error;
          tmp = pdf_fp_pow (stack[sp-1].v, stack[sp].v);
          if (isinf(tmp) || tmp == HUGE_VAL) goto math_error;
          stack[sp-1].v = tmp;
          sp--;
          break;
        case OPC_log:
          if (sp < 0) goto stack_underflow;
          if (!P_NUM(stack[sp])) goto type_error;
          if (stack[sp].v <= 0) goto range_error;
          stack[sp].v = pdf_fp_log10 (stack[sp].v);
          stack[sp].t = REAL;
          break;
        case OPC_ln:
          if (sp < 0) goto stack_underflow;
          if (!P_NUM(stack[sp])) goto type_error;
          if (stack[sp].v <= 0) goto range_error;
          stack[sp].v = pdf_fp_log (stack[sp].v);
          stack[sp].t = REAL;
          break;
        case OPC_sqrt:
          if (sp < 0) goto stack_underflow;
          if (!P_NUM(stack[sp])) goto type_error;
          if (stack[sp].v < 0) goto range_error;
          stack[sp].v = pdf_fp_sqrt (stack[sp].v);
          stack[sp].t = REAL;
          break;
        case OPC_floor:
          if (sp < 0) goto stack_underflow;
          if (!P_NUM(stack[sp])) goto type_error;
          tmp = pdf_fp_floor (stack[sp].v);
          if (isnan(tmp)) goto limit_error;
          stack[sp].v = tmp;
          break;
        case OPC_ceiling:
          if (sp < 0) goto stack_underflow;
          if (!P_NUM(stack[sp])) goto type_error;
          tmp = pdf_fp_ceil (stack[sp].v);
          if (isnan(tmp)) goto limit_error;
          stack[sp].v = tmp;
          break;
        case OPC_truncate:
          if (sp < 0) goto stack_underflow;
          if (!P_NUM(stack[sp])) goto type_error;
          /* C99 stack[sp] = trunc(stack[sp]); */
          stack[sp].v = TRUNC(stack[sp].v);
          break;
        case OPC_round:
          if (sp < 0) goto stack_underflow;
          if (!P_NUM(stack[sp])) goto type_error;
          stack[sp].v = pdf_fp_floor (0.5 + stack[sp].v);
          break;
        case OPC_cvi:
          if (sp < 0) goto stack_underflow;
          if (!P_NUM(stack[sp])) goto type_error;
          stack[sp].v = INT(stack[sp].v);
          stack[sp].t = INT;
          break;
        case OPC_index:
          if (sp < 0) goto stack_underflow;
          if (!P_INT(stack[sp])) goto type_error;
          if (stack[sp].v < 0 || stack[sp].v > sp-1) goto range_error;
          /* copy complete stack element with type info */
          stack[sp] = stack[sp-1-INT(stack[sp].v)];
          break;
        case OPC_copy:
          {
            pdf_i32_t n,i;
            if (sp < 0) goto stack_underflow;
            if (!P_INT(stack[sp])) goto type_error;
            n = INT(stack[sp].v);
            if (n < 0 || n > sp) goto range_error;
            if (sp + n >= NSTACK) goto stack_overflow;
            sp = sp - n;
            for (i = 0; i < n; i++, sp++)
              {
                /* copy complete stack element with type info */
                stack[sp+n] = stack[sp];
              }
            if (n > 0)
              {
                sp++;
              }
            break;
          }
        case OPC_roll:
          {
            pdf_i32_t n,s,j;
            if (sp < 2) goto stack_underflow;
            if (!P_INT(stack[sp]) || !P_INT(stack[sp-1])) goto type_error;
            n = INT(stack[sp-1].v);
            j = INT(stack[sp].v);
            sp -= 2;
            if (n < 0 || n > sp) goto range_error;
            if (n >= 2)
              {
                struct t_stack pp[NSTACK];
                int pos;
                s = (j-1) % n;
                for (pos = 0; pos < n; pos++)
                  {
                    if (s < 0)
                       s = n - 1;
                    pp[pos] = stack[sp-s];
                    s--;
                  }
                memcpy(&stack[sp-n+1],pp,n*sizeof(stack[0]));
              }
            break;
          }
        case OPC_cvr:
          if (sp < 0) goto stack_underflow;
          if (!P_NUM(stack[sp])) goto type_error;
          stack[sp].t = REAL;
          break;
        case OPC_add:
          if (sp < 1) goto stack_underflow;
          if (!P_NUM(stack[sp-1]) || !P_NUM(stack[sp])) goto type_error;
          if (isinf(tmp = pdf_fp_add(stack[sp-1].v, stack[sp].v)))
            goto limit_error;
          stack[sp-1].v = tmp;
          stack[sp-1].t = NUM_TYPE(stack[sp-1], stack[sp]);
          sp--;
          break;
        case OPC_sub:
          if (sp < 1) goto stack_underflow;
          if (!P_NUM(stack[sp-1]) || !P_NUM(stack[sp])) goto type_error;
          if (isinf(tmp = pdf_fp_sub(stack[sp-1].v, stack[sp].v)))
            goto limit_error;
          stack[sp-1].v = tmp;
          stack[sp-1].t = NUM_TYPE(stack[sp-1], stack[sp]);
          sp--;
          break;
        case OPC_mul:
          if (sp < 1) goto stack_underflow;
          if (!P_NUM(stack[sp-1]) || !P_NUM(stack[sp])) goto type_error;
          if (isinf(tmp = pdf_fp_mul(stack[sp-1].v, stack[sp].v)))
            goto limit_error;
          stack[sp-1].v = tmp;
          stack[sp-1].t = NUM_TYPE(stack[sp-1], stack[sp]);
          sp--;
          break;
        case OPC_div:
          if (sp < 1) goto stack_underflow;
          if (!P_NUM(stack[sp-1]) || !P_NUM(stack[sp])) goto type_error;
          if (stack[sp].v == 0) goto math_error;
          if (isinf(tmp = pdf_fp_div(stack[sp-1].v, stack[sp].v)))
            goto limit_error;
          stack[sp-1].v = tmp;
          stack[sp-1].t = NUM_TYPE(stack[sp-1], stack[sp]);
          sp--;
          break;
        case OPC_atan:
          if (sp < 1) goto stack_underflow;
          if (!P_NUM(stack[sp-1]) || !P_NUM(stack[sp])) goto type_error;
          stack[sp-1].v = (180/PDF_PI) *
                          pdf_fp_atan2 (stack[sp-1].v, stack[sp].v);
          stack[sp-1].t = REAL;
          /* check against Ghostscript */
          sp--;
          break;
        case OPC_idiv:
          if (sp < 1) goto stack_underflow;
          if (!P_INT(stack[sp-1]) || !P_INT(stack[sp])) goto type_error;
          if (stack[sp].v == 0) goto math_error;
          stack[sp-1].v = INT(pdf_fp_div(INT(stack[sp-1].v), INT(stack[sp].v)));
          sp--;
          break;
        case OPC_mod:
          if (sp < 1) goto stack_underflow;
          if (!P_INT(stack[sp-1]) || !P_INT(stack[sp])) goto type_error;
          if (stack[sp].v == 0) goto math_error;
          stack[sp-1].v = pdf_fp_mod(stack[sp-1].v, stack[sp].v);
          sp--;
          break;
        case OPC_and:
          if (sp < 1) goto stack_underflow;
          if (P_INT(stack[sp-1]) && P_INT(stack[sp]))
            stack[sp-1].v = INT(stack[sp-1].v) & INT(stack[sp].v);
          else if (P_BOOL(stack[sp-1]) && P_BOOL(stack[sp]))
            stack[sp-1].v = BOOL(P_TRUE(stack[sp-1]) && P_TRUE(stack[sp]));
          else
            goto type_error;
          sp--;
          break;
        case OPC_bitshift:
          if (sp < 1) goto stack_underflow;
          if (!P_INT(stack[sp-1]) || !P_INT(stack[sp]))
            goto type_error;
          {
            pdf_i32_t x;
            pdf_i32_t i = INT(stack[sp].v);
            if ((stack[sp].v > PDF_I32_MAX) ||
                (stack[sp].v < PDF_I32_MIN))
              goto limit_error;
            x = INT(stack[sp-1].v);
            if (i >= 0)
              x = (i < 32) ? (x<<i) : 0;
            else if (i < 0)
              x = (i > -32) ? (x>>-i) : 0;
            stack[sp-1].v = x;
          }
          sp--;
          break;
        case OPC_or:
          if (sp < 1) goto stack_underflow;
          if (P_INT(stack[sp-1]) && P_INT(stack[sp]))
            stack[sp-1].v = INT(stack[sp-1].v) | INT(stack[sp].v);
          else if (P_BOOL(stack[sp-1]) && P_BOOL(stack[sp]))
            stack[sp-1].v = BOOL(P_TRUE(stack[sp-1]) || P_TRUE(stack[sp]));
          else
            goto type_error;
          sp--;
          break;
        case OPC_xor:
          if (sp < 1) goto stack_underflow;
          if (P_INT(stack[sp-1]) && P_INT(stack[sp]))
            stack[sp-1].v = INT(stack[sp-1].v) ^ INT(stack[sp].v);
          else if (P_BOOL(stack[sp-1]) && P_BOOL(stack[sp]))
            stack[sp-1].v = BOOL((P_TRUE(stack[sp-1]) || P_TRUE(stack[sp])) &&
                                 (stack[sp-1].v != stack[sp].v));
          else
            goto type_error;
          sp--;
          break;
        case OPC_not:
          if (sp < 0) goto stack_underflow;
          if (P_INT(stack[sp]))
            stack[sp].v = ~INT(stack[sp].v);
          else if (P_BOOL(stack[sp]))
            stack[sp].v = BOOL(!P_TRUE(stack[sp]));
          else
            goto type_error;
          break;
        case OPC_eq:
          if (sp < 1) goto stack_underflow;
          if (isnan(stack[sp-1].v) || isnan(stack[sp].v))
            goto math_error;
          /* any comparison involving NaN is false. */
          /* this should not go unnoticed */
          if ((P_NUM(stack[sp-1]) && P_NUM(stack[sp])) ||
              (P_BOOL(stack[sp-1]) && P_BOOL(stack[sp])))
            stack[sp-1].v = BOOL(stack[sp-1].v == stack[sp].v);
          else
            goto type_error;
          stack[sp-1].t = BOOL;
          sp--;
          break;
        case OPC_ne:
          if (sp < 1) goto stack_underflow;
          if (isnan(stack[sp-1].v) || isnan(stack[sp].v))
            goto math_error;
          if ((P_NUM(stack[sp-1]) && P_NUM(stack[sp])) ||
              (P_BOOL(stack[sp-1]) && P_BOOL(stack[sp])))
            stack[sp-1].v = BOOL(stack[sp-1].v != stack[sp].v);
          else
            goto type_error;
          stack[sp-1].t = BOOL;
          sp--;
          break;
        case OPC_lt:
          if (sp < 1) goto stack_underflow;
          if (isnan(stack[sp-1].v) || isnan(stack[sp].v))
            goto math_error;
          if ((P_NUM(stack[sp-1]) && P_NUM(stack[sp])) ||
              (P_BOOL(stack[sp-1]) && P_BOOL(stack[sp])))
            stack[sp-1].v = BOOL(stack[sp-1].v < stack[sp].v);
          else
            goto type_error;
          stack[sp-1].t = BOOL;
          sp--;
          break;
        case OPC_le:
          if (sp < 1) goto stack_underflow;
          if (isnan(stack[sp-1].v) || isnan(stack[sp].v))
            goto math_error;
          if ((P_NUM(stack[sp-1]) && P_NUM(stack[sp])) ||
              (P_BOOL(stack[sp-1]) && P_BOOL(stack[sp])))
            stack[sp-1].v = BOOL(stack[sp-1].v <= stack[sp].v);
          else
            goto type_error;
          stack[sp-1].t = BOOL;
          sp--;
          break;
        case OPC_ge:
          if (sp < 1) goto stack_underflow;
          if (isnan(stack[sp-1].v) || isnan(stack[sp].v))
            goto math_error;
          if ((P_NUM(stack[sp-1]) && P_NUM(stack[sp])) ||
              (P_BOOL(stack[sp-1]) && P_BOOL(stack[sp])))
            stack[sp-1].v = BOOL(stack[sp-1].v >= stack[sp].v);
          else
            goto type_error;
          stack[sp-1].t = BOOL;
          sp--;
          break;
        case OPC_gt:
          if (sp < 1) goto stack_underflow;
          if (isnan(stack[sp-1].v) || isnan(stack[sp].v))
            goto math_error;
          if ((P_NUM(stack[sp-1]) && P_NUM(stack[sp])) ||
              (P_BOOL(stack[sp-1]) && P_BOOL(stack[sp])))
            stack[sp-1].v = BOOL(stack[sp-1].v > stack[sp].v);
          else
            goto type_error;
          stack[sp-1].t = BOOL;
          sp--;
          break;
        case OPC_true:
          if (sp >= NSTACK) goto stack_overflow;
          sp++;
          stack[sp].v = REP_TRUE;
          stack[sp].t = BOOL;
          break;
        case OPC_false:
          if (sp >= NSTACK) goto stack_overflow;
          sp++;
          stack[sp].v = REP_FALSE;
          stack[sp].t = BOOL;
          break;
        default:
          goto block_error;
        }
    }


 block_error:     debug_info.type4.status = PDF_ERROR;       goto end;
 stack_error:     debug_info.type4.status = PDF_EINVRANGE;   goto end;
 stack_underflow: debug_info.type4.status = PDF_EUNDERFLOW;  goto end;
 stack_overflow:  debug_info.type4.status = PDF_EOVERFLOW;   goto end;
 range_error:     debug_info.type4.status = PDF_EINVRANGE;   goto end;
 type_error:      debug_info.type4.status = PDF_EBADTYPE;    goto end;
 math_error:      debug_info.type4.status = PDF_EMATH;       goto end;
 limit_error:     debug_info.type4.status = PDF_EIMPLLIMIT;  goto end;
 end:

  /* Found that code was compiled to the opcode for debugging.  */
  debug_info.type4.op = -1;
  for (aux = 0; aux < t->u.t4.debug_size; aux++)
    {
      if (t->u.t4.debug_off[aux][0] == pc)
        {
          debug_info.type4.op = t->u.t4.debug_off[aux][1];
          break;
        }
    }

  /* Copy some elements from the stack for debugging */
  for (aux = 0; aux <= sp; aux++)
    {
      debug_info.type4.stack[aux] = stack[sp - aux].v;
    }
  debug_info.type4.stack_size = sp + 1;

  if (debug != NULL)
    *debug = debug_info;

  return PDF_ETYPE4;
}
/* ANSI-C code produced by gperf version 3.0.1 */
/* Command-line: gperf -t -m 100 pdf_function_type4.gperf  */
/* Computed positions: -k'1-3' */
/* Caution : 7 bit only */

struct  toklut {  const char *name; pdf_i32_t ret; };

#define TOTAL_KEYWORDS 44
#define MIN_WORD_LENGTH 1
#define MAX_WORD_LENGTH 8
#define MIN_HASH_VALUE 2
#define MAX_HASH_VALUE 50
/* maximum key range = 49, duplicates = 0 */
static inline pdf_i32_t
hash (register const char *str, register pdf_i32_t len)
{
  static pdf_char_t asso_values[] =
    {
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51,  3, 30, 10,
      4,  4, 12,  2, 51,  5, 51, 51,  1, 27,
      3, 16, 13, 33,  7,  4,  1,  4, 18, 51,
      11, 51, 51,  2, 51,  1, 51, 51
    };
  register pdf_i32_t hval = len;

  switch (hval)
    {
    default:
      hval += asso_values[(pdf_uchar_t) str[2]];
      /*FALLTHROUGH*/
    case 2:
      hval += asso_values[(pdf_uchar_t) str[1]];
      /*FALLTHROUGH*/
    case 1:
      hval += asso_values[(pdf_uchar_t) str[0]];
      break;
    }
  return hval;
}

static inline struct  toklut *
in_word_set (register const char *str, register pdf_i32_t len)
{
  static struct  toklut wordlist[] =
    {
      {""}, {""},
      {"}",		OPC_end},
      {"{",		OPC_begin},
      {"lt",		OPC_lt},
      {"gt",		OPC_gt},
      {"ln",		OPC_ln},
      {"le",		OPC_le},
      {"ge",		OPC_ge},
      {"ne",		OPC_ne},
      {""},
      {"atan",	OPC_atan},
      {"neg",	OPC_neg},
      {"and",	OPC_and},
      {"add",	OPC_add},
      {"sin",	OPC_sin},
      {"true",	OPC_true},
      {"index",	OPC_index},
      {"idiv",	OPC_idiv},
      {"if",		OPC_if},
      {"truncate",	OPC_truncate},
      {"false",	OPC_false},
      {"log",	OPC_log},
      {"not",	OPC_not},
      {"dup",	OPC_dup},
      {"or",		OPC_or},
      {"ceiling",	 OPC_ceiling},
      {"ifelse",	OPC_ifelse},
      {"roll",	OPC_roll},
      {"exch",	OPC_exch},
      {"div",	OPC_div},
      {"exp",	OPC_exp},
      {"round",	OPC_round},
      {"cos",	OPC_cos},
      {"floor",	OPC_floor},
      {"mul",	OPC_mul},
      {"cvi",	OPC_cvi},
      {"xor",	OPC_xor},
      {"cvr",	OPC_cvr},
      {"eq",		OPC_eq},
      {"abs",	OPC_abs},
      {"sub",	OPC_sub},
      {""},
      {"copy",	OPC_copy},
      {"bitshift",	 OPC_bitshift},
      {"pop",	OPC_pop},
      {""}, {""},
      {"sqrt",	OPC_sqrt},
      {""},
      {"mod",	OPC_mod}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register pdf_i32_t key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}

static pdf_i32_t
get_token (pdf_token_reader_t *reader,
           double             *literal,
           pdf_size_t         *token_begin)
{
  pdf_token_t *token;
  pdf_i32_t ret = OPC_bad;
  pdf_error_t *inner_error = NULL;

  /* Invoke the tokeniser */
  token = pdf_token_reader_read (reader,
                                 PDF_TOKEN_NO_NAME_ESCAPES,
                                 &inner_error);

  *token_begin = pdf_token_reader_begin_pos (reader);

  if (token)
    {
      /* The token should be:
       * - A real literal or
       * - An identifier or
       * - An open brace { (OPC_begin) or
       * - A close brace } (OPC_end)
       */

      switch (pdf_token_get_type (token))
        {
        case PDF_TOKEN_INTEGER:
          {
            *literal = pdf_token_get_integer_value (token);
            ret = OPC_lit_i;
            break;
          }
        case PDF_TOKEN_REAL:
          {
            *literal = pdf_token_get_real_value (token);
            ret = OPC_lit_r;
            break;
          }
        case PDF_TOKEN_KEYWORD:
          {
            struct toklut *tk;

            tk = in_word_set (pdf_token_get_keyword_data (token),
                              pdf_token_get_keyword_size (token));
            ret = (tk) ? tk->ret : OPC_bad;
            break;
          }
        case PDF_TOKEN_PROC_START:
          {
            ret = OPC_begin;
            break;
          }
        case PDF_TOKEN_PROC_END:
          {
            ret = OPC_end;
            break;
          }
        default:
          {
            /* Wrong token type */
            ret = OPC_bad;
            break;
          }
        }

      pdf_token_destroy (token);
      return ret;
    }


  if (inner_error)
    {
      /* TODO: Propagate error */
      pdf_error_destroy (inner_error);
      return OPC_bad;
    }

  /* In EOF, returns -1 */
  return -1;
}

/* End of pdf-fp-func.c */

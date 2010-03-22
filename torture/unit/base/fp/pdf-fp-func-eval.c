/* -*- mode: C -*- Time-stamp: "2009-08-05 21:45:07 davazp"
 *
 *       File:         pdf-fp-func-eval.c
 *       Date:         Tue Dec  2 20:11:38 2008
 *
 *       GNU PDF Library - Unit tests for pdf_fp_func_eval
 *
 */

/* Copyright (C) 2008, 2009 Free Software Foundation, Inc. */

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
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <check.h>
#include <pdf.h>
#include <float.h>

#define ABS_ERROR (0.00001)
#define DEG2RAD(N) ((N) * (180.0/PDF_PI))
#define TRUNC(x) ((x) < 0 ? pdf_fp_ceil(x) : pdf_fp_floor(x))
#define INT(x) ((int)TRUNC(x))

#define TXT_ERR_EVAL     "Evaluation error with operator '%s'"
#define TXT_ERR_RES      "Incorrect result from operator '%s'"
#define TXT_ERR_EVALSUCC "Invalid evaluation success with operator '%s'"
#define TXT_ERR_ESTATUS  "Incorrect error status for operator '%s'"



/*
 * The creation of type 4 functions is encapsulated in this call.
 * The type 4 function string can be created from a standard
 * printf format string and varargs.
 */
static pdf_status_t 
pdf_fp_func_4_tmake (pdf_u32_t m,
                     pdf_u32_t n,
                     pdf_real_t domain[],                                 
                     pdf_real_t range[],                                 
                     pdf_fp_func_t *function,
                     const pdf_char_t *code_fmt,
                     ...)
{
  va_list ap;
  int size, s = 32; 
  pdf_char_t *np, *code;
  pdf_size_t prog_size;
  pdf_status_t ret;

  if ((code = malloc(sizeof(pdf_char_t) * s)) == NULL)
    return PDF_ERROR;

  while (1)
    {
      va_start(ap, code_fmt);
      size = vsnprintf(code, s, code_fmt, ap);
      va_end(ap);

      if (size > -1 && size < s)
          break;
      if (size > -1)
        s = size+1;
      else
        s *= 2;
      if ((np = realloc(code, s)) == NULL)
        {
          free(code);
          return PDF_ERROR;
        }
      else
        code = np;
    }

  pdf_init();

  prog_size = size;
  ret = pdf_fp_func_4_new (m, n,
                           domain,
                           range,
                           code,
                           prog_size,
                           NULL,
                           function);
  free(code);

  return ret;
}
                                        
/*
 * Test: pdf_fp_func_eval_001
 * Description:
 *   Evaluate a type 4 function implementing the predefined function
 *   SimpleDot and compare the results with a C implementation.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
simple_dot (pdf_real_t x, pdf_real_t y)
{
  return (1 - ((x * x) + (y * y)));
}

START_TEST(pdf_fp_func_eval_001)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t *prog =
    "{ "
    "dup mul exch dup mul add 1 exch sub"
    " }";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];

  pdf_init();

  prog_size = strlen (prog);
  
  /* Create the function */
  fail_if(pdf_fp_func_4_new (2, 1,
                             domain,
                             range,
                             prog,
                             prog_size,
                             NULL,
                             &func) != PDF_OK);

  /*
   * Eval for some values
   */

  /* x = 0, y = 0 */
  in[0] = 0;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - simple_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - simple_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - simple_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - simple_dot (in[0], in[1])) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_002
 * Description:
 *   Evaluate a type 4 function implementing the predefined function
 *   InvertedSimpleDot and compare the results with a C implementation.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
inverted_simple_dot (pdf_real_t x, pdf_real_t y)
{
  return (((x * x) + (y * y)) - 1);
}

START_TEST(pdf_fp_func_eval_002)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t *prog =
    "{ "
    "dup mul exch dup mul add 1 sub"
    " }";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];

  prog_size = strlen (prog);

  pdf_init();

  /* Create the function */
  fail_if(pdf_fp_func_4_new (2, 1,
                             domain,
                             range,
                             prog,
                             prog_size,
                             NULL,
                             &func) != PDF_OK);

  /*
   * Eval for some values
   */

  /* x = 0, y = 0 */
  in[0] = 0;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_simple_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_simple_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_simple_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_simple_dot (in[0], in[1])) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_003
 * Description:
 *   Evaluate a type 4 function implementing the predefined function
 *   DoubleDot and compare the results with a C implementation.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
double_dot (pdf_real_t x, pdf_real_t y)
{
  return ((pdf_fp_sin (DEG2RAD(360 * x)) / 2) +
          ((pdf_fp_sin (DEG2RAD(360 * y)) / 2)));
}

START_TEST(pdf_fp_func_eval_003)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t *prog =
    "{ "
    "360 mul sin "
    "2 div "
    "exch 360 mul sin "
    "2 div "
    "add "
    "}";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];

  prog_size = strlen (prog);

  pdf_init();

  /* Create the function */
  fail_if(pdf_fp_func_4_new (2, 1,
                             domain,
                             range,
                             prog,
                             prog_size,
                             NULL,
                             &func) != PDF_OK);

  /*
   * Eval for some values
   */

  /* x = 0, y = 0 */
  in[0] = 0;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - double_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - double_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - double_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - double_dot (in[0], in[1])) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_004
 * Description:
 *   Evaluate a type 4 function implementing the predefined function
 *   InvertedDoubleDot and compare the results with a C implementation.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
inverted_double_dot (pdf_real_t x, pdf_real_t y)
{
  return (-1 * ((sinf (DEG2RAD(360 * x)) / 2) +
                ((sinf (DEG2RAD(360 * y)) / 2))));
}

START_TEST(pdf_fp_func_eval_004)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t *prog =
    "{ "
    "360 mul sin "
    "2 div "
    "exch 360 mul sin "
    "2 div "
    "add "
    "neg "
    " }";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];

  prog_size = strlen (prog);

  pdf_init();

  /* Create the function */
  fail_if(pdf_fp_func_4_new (2, 1,
                             domain,
                             range,
                             prog,
                             prog_size,
                             NULL,
                             &func) != PDF_OK);

  /*
   * Eval for some values
   */

  /* x = 0, y = 0 */
  in[0] = 0;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_double_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_double_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_double_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_double_dot (in[0], in[1])) > ABS_ERROR);
}
END_TEST


/*
 * Test: pdf_fp_func_eval_005
 * Description:
 *   Evaluate a type 4 function implementing the predefined function
 *   CosineDot and compare the results with a C implementation.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
cosine_dot (pdf_real_t x, pdf_real_t y)
{
  return (((pdf_fp_cos (DEG2RAD(180 * x)) / 2) +
           ((pdf_fp_cos (DEG2RAD(180 * y)) / 2))));
}

START_TEST(pdf_fp_func_eval_005)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t *prog =
    "{ "
    "180 mul cos "
    "exch "
    "180 mul cos "
    "add "
    "2 div"
    " }";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];

  prog_size = strlen (prog);

  pdf_init();

  /* Create the function */
  fail_if(pdf_fp_func_4_new (2, 1,
                             domain,
                             range,
                             prog,
                             prog_size,
                             NULL,
                             &func) != PDF_OK);

  /*
   * Eval for some values
   */

  /* x = 0, y = 0 */
  in[0] = 0;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - cosine_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - cosine_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - cosine_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - cosine_dot (in[0], in[1])) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_006
 * Description:
 *   Evaluate a type 4 function implementing the predefined function
 *   Double and compare the results with a C implementation.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
double_f (pdf_real_t x, pdf_real_t y)
{
  return ((sinf (DEG2RAD(360 * (x / 2))) / 2) +
          (sinf (DEG2RAD(360 * y)) / 2));
}

START_TEST(pdf_fp_func_eval_006)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t *prog =
    "{ "
    "360 mul sin "
    "2 div "
    "exch "
    "2 div "
    "360 mul sin "
    "2 div "
    "add"
    " }";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];

  prog_size = strlen (prog);

  pdf_init();

  /* Create the function */
  fail_if(pdf_fp_func_4_new (2, 1,
                             domain,
                             range,
                             prog,
                             prog_size,
                             NULL,
                             &func) != PDF_OK);

  /*
   * Eval for some values
   */

  /* x = 0, y = 0 */
  in[0] = 0;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - double_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - double_f (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - double_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - double_f (in[0], in[1])) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_007
 * Description:
 *   Evaluate a type 4 function implementing the predefined function
 *   InvertedDouble and compare the results with a C implementation.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
inverted_double (pdf_real_t x, pdf_real_t y)
{
  return (-1 * ((sinf (DEG2RAD(360 * (x / 2))) / 2) +
                (sinf (DEG2RAD(360 * y)) / 2)));
}

START_TEST(pdf_fp_func_eval_007)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t *prog =
    "{ "
    "360 mul sin "
    "2 div "
    "exch "
    "2 div "
    "360 mul sin "
    "2 div "
    "add "
    "neg"
    " }";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];

  prog_size = strlen (prog);

  pdf_init();

  /* Create the function */
  fail_if(pdf_fp_func_4_new (2, 1,
                             domain,
                             range,
                             prog,
                             prog_size,
                             NULL,
                             &func) != PDF_OK);

  /*
   * Eval for some values
   */

  /* x = 0, y = 0 */
  in[0] = 0;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_double (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_double (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_double (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_double (in[0], in[1])) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_008
 * Description:
 *   Evaluate a type 4 function implementing the predefined function
 *   line and compare the results with a C implementation.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
line (pdf_real_t x, pdf_real_t y)
{
  return (- 1 * pdf_fp_abs(y));
}

START_TEST(pdf_fp_func_eval_008)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t *prog =
    "{ "
    "exch pop abs neg"
    " }";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];

  prog_size = strlen (prog);

  pdf_init();
  
  /* Create the function */
  fail_if(pdf_fp_func_4_new (2, 1,
                             domain,
                             range,
                             prog,
                             prog_size,
                             NULL,
                             &func) != PDF_OK);

  /*
   * Eval for some values
   */

  /* x = 0, y = 0 */
  in[0] = 0;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - line (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - line (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - line (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - line (in[0], in[1])) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_009
 * Description:
 *   Evaluate a type 4 function implementing the predefined function
 *   lineX and compare the results with a C implementation.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
line_x (pdf_real_t x, pdf_real_t y)
{
  return (x);
}

START_TEST(pdf_fp_func_eval_009)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t *prog =
    "{ "
    "pop"
    " }";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];

  prog_size = strlen (prog);

  pdf_init();

  /* Create the function */
  fail_if(pdf_fp_func_4_new (2, 1,
                             domain,
                             range,
                             prog,
                             prog_size,
                             NULL,
                             &func) != PDF_OK);

  /*
   * Eval for some values
   */

  /* x = 0, y = 0 */
  in[0] = 0;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - line_x (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - line_x (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - line_x (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - line_x (in[0], in[1])) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_010
 * Description:
 *   Evaluate a type 4 function implementing the predefined function
 *   lineY and compare the results with a C implementation.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
line_y (pdf_real_t x, pdf_real_t y)
{
  return (y);
}

START_TEST(pdf_fp_func_eval_010)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t *prog =
    "{ "
    "exch pop"
    " }";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];

  prog_size = strlen (prog);

  pdf_init();
  
  /* Create the function */
  fail_if(pdf_fp_func_4_new (2, 1,
                             domain,
                             range,
                             prog,
                             prog_size,
                             NULL,
                             &func) != PDF_OK);

  /*
   * Eval for some values
   */

  /* x = 0, y = 0 */
  in[0] = 0;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - line_y (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - line_y (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - line_y (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - line_y (in[0], in[1])) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_011
 * Description:
 *   Evaluate a type 4 function implementing the predefined function
 *   Round and compare the results with a C implementation.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
round_f (pdf_real_t x, pdf_real_t y)
{
  pdf_real_t res;
  pdf_real_t temp1;
  pdf_real_t temp2;

  if ((pdf_fp_abs(x) + pdf_fp_abs(y)) <= 1)
    {
      res = (1 - ((x * x) + (y * y)));
    }
  else
    {
      temp1 = (pdf_fp_abs(x) - 1);
      temp2 = (pdf_fp_abs(y) - 1);
      res = ((temp1 * temp1) + (temp2 * temp2) - 1);
    }

  return res;
}

START_TEST(pdf_fp_func_eval_011)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t *prog =
    "{ "
    "abs exch abs "
    "2 copy add 1 le "
    "        { dup mul exch dup mul add 1 exch sub } "
    "        { 1 sub dup mul exch 1 sub dup mul add 1 sub } "
    "  ifelse"
    " }";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];

  prog_size = strlen (prog);

  pdf_init();

  /* Create the function */
  fail_if(pdf_fp_func_4_new (2, 1,
                             domain,
                             range,
                             prog,
                             prog_size,
                             NULL,
                             &func) != PDF_OK);

  /*
   * Eval for some values
   */

  /* x = 0, y = 0 */
  in[0] = 0;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - round_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - round_f (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - round_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - round_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.93, y = 0.82 */
  in[0] = 0.93;
  in[1] = 0.82;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - round_f (in[0], in[1])) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_012
 * Type: 
 * Description:
 *   Evaluate a type 4 function implementing the predefined function
 *   Ellipse and compare the results with a C implementation.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
ellipse_f (pdf_real_t x, pdf_real_t y)
{
  pdf_real_t ret;
  pdf_real_t w;
  pdf_real_t temp;
  pdf_real_t temp2;

  w = (3 * pdf_fp_abs(x)) + (4 * pdf_fp_abs(y)) - 3;

  if (w < 0)
    {
      temp = (pdf_fp_abs(y) / 0.75);
      ret = (1 - (((x * x) + (temp * temp)) / 4));
    }
  else if (w > 1)
    {
      temp = ((1 - pdf_fp_abs(y)) / 0.75);
      temp2 = (1 - pdf_fp_abs(x));
      ret = ((((temp2 * temp2) + (temp * temp)) / 4) - 1);
    }
  else
    {
      ret = (0.5 - w);
    }

  return ret;
}

START_TEST(pdf_fp_func_eval_012)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t *prog =
    "{ "
    "abs exch abs 2 copy 3 mul exch 4 mul add 3 sub dup 0 lt "
    "    { pop dup mul exch 0.75 div dup mul add 4 div 1 exch sub } "
    "    { dup 1 gt "
    "        { pop 1 exch sub dup mul exch 1 exch sub 0.75 div dup mul add 4 div 1 sub } "
    "        { 0.5 exch sub exch pop exch pop } "
    "      ifelse } "
    "  ifelse"
    " }";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];

  prog_size = strlen (prog);

  pdf_init();

  /* Create the function */
  fail_if(pdf_fp_func_4_new (2, 1,
                             domain,
                             range,
                             prog,
                             prog_size,
                             NULL,
                             &func) != PDF_OK);

  /*
   * Eval for some values
   */

  /* x = 0, y = 0 */
  in[0] = 0;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_f (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.93, y = 0.82 */
  in[0] = 0.93;
  in[1] = 0.82;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_f (in[0], in[1])) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_013
 * Description:
 *   Evaluate a type 4 function implementing the predefined function
 *   EllipseA and compare the results with a C implementation.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
ellipse_a (pdf_real_t x, pdf_real_t y)
{
  return (1 - ((x * x) + 0.9 * (y * y)));
}

START_TEST(pdf_fp_func_eval_013)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t *prog =
    "{ "
    "dup mul 0.9 mul exch dup mul add 1 exch sub"
    " }";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];

  prog_size = strlen (prog);

  pdf_init();

  /* Create the function */
  fail_if(pdf_fp_func_4_new (2, 1,
                             domain,
                             range,
                             prog,
                             prog_size,
                             NULL,
                             &func) != PDF_OK);

  /*
   * Eval for some values
   */

  /* x = 0, y = 0 */
  in[0] = 0;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_a (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_a (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_a (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_a (in[0], in[1])) > ABS_ERROR);

  /* x = 0.93, y = 0.82 */
  in[0] = 0.93;
  in[1] = 0.82;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_a (in[0], in[1])) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_014
 * Description:
 *   Evaluate a type 4 function implementing the predefined function
 *   InvertedEllipseA and compare the results with a C implementation.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
inverted_ellipse_a (pdf_real_t x, pdf_real_t y)
{
  return ((x * x) + 0.9 * (y * y) - 1);
}

START_TEST(pdf_fp_func_eval_014)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t *prog =
    "{ "
    " dup mul 0.9 mul exch dup mul add 1 sub"
    " }";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];

  prog_size = strlen (prog);

  pdf_init();

  /* Create the function */
  fail_if(pdf_fp_func_4_new (2, 1,
                             domain,
                             range,
                             prog,
                             prog_size,
                             NULL,
                             &func) != PDF_OK);

  /*
   * Eval for some values
   */

  /* x = 0, y = 0 */
  in[0] = 0;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_ellipse_a (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_ellipse_a (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_ellipse_a (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_ellipse_a (in[0], in[1])) > ABS_ERROR);

  /* x = 0.93, y = 0.82 */
  in[0] = 0.93;
  in[1] = 0.82;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_ellipse_a (in[0], in[1])) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_015
 * Description:
 *   Evaluate a type 4 function implementing the predefined function
 *   EllipseB and compare the results with a C implementation.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
ellipse_b (pdf_real_t x, pdf_real_t y)
{
  return (1 - pdf_fp_sqrt ((x * x) + (5.0/8.0) * (y * y)));
}

START_TEST(pdf_fp_func_eval_015)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t *prog =
    "{ "
    "dup 5 mul 8 div mul exch dup mul exch add sqrt 1 exch sub"
    " }";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];

  prog_size = strlen (prog);

  pdf_init();

  /* Create the function */
  fail_if(pdf_fp_func_4_new (2, 1,
                             domain,
                             range,
                             prog,
                             prog_size,
                             NULL,
                             &func) != PDF_OK);

  /*
   * Eval for some values
   */

  /* x = 0, y = 0 */
  in[0] = 0;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_b (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_b (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_b (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_b (in[0], in[1])) > ABS_ERROR);

  /* x = 0.93, y = 0.82 */
  in[0] = 0.93;
  in[1] = 0.82;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_b (in[0], in[1])) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_016
 * Description:
 *   Evaluate a type 4 function implementing the predefined function
 *   EllipseC and compare the results with a C implementation.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
ellipse_c (pdf_real_t x, pdf_real_t y)
{
  return (1 - (0.9 * (x * x) + (y * y)));
}

START_TEST(pdf_fp_func_eval_016)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t *prog =
    "{ "
    "dup mul exch dup mul 0.9 mul add 1 exch sub"
    " }";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];

  prog_size = strlen (prog);

  pdf_init();

  /* Create the function */
  fail_if(pdf_fp_func_4_new (2, 1,
                             domain,
                             range,
                             prog,
                             prog_size,
                             NULL,
                             &func) != PDF_OK);

  /*
   * Eval for some values
   */

  /* x = 0, y = 0 */
  in[0] = 0;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_c (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_c (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_c (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_c (in[0], in[1])) > ABS_ERROR);

  /* x = 0.93, y = 0.82 */
  in[0] = 0.93;
  in[1] = 0.82;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_c (in[0], in[1])) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_017
 * Description:
 *   Evaluate a type 4 function implementing the predefined function
 *   InvertedEllipseC and compare the results with a C implementation.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
inverted_ellipse_c (pdf_real_t x, pdf_real_t y)
{
  return (0.9 * (x * x) + (y * y) - 1);
}

START_TEST(pdf_fp_func_eval_017)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t *prog =
    "{ "
    "dup mul exch dup mul 0.9 mul add 1 sub"
    " }";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];

  prog_size = strlen (prog);

  pdf_init();

  /* Create the function */
  fail_if(pdf_fp_func_4_new (2, 1,
                             domain,
                             range,
                             prog,
                             prog_size,
                             NULL,
                             &func) != PDF_OK);

  /*
   * Eval for some values
   */

  /* x = 0, y = 0 */
  in[0] = 0;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_ellipse_c (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_ellipse_c (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_ellipse_c (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_ellipse_c (in[0], in[1])) > ABS_ERROR);

  /* x = 0.93, y = 0.82 */
  in[0] = 0.93;
  in[1] = 0.82;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_ellipse_c (in[0], in[1])) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_018
 * Description:
 *   Evaluate a type 4 function implementing the predefined function
 *   Square and compare the results with a C implementation.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
square_f (pdf_real_t x, pdf_real_t y)
{
  return (-1 * PDF_MAX(pdf_fp_abs(x),pdf_fp_abs(y)));
}

START_TEST(pdf_fp_func_eval_018)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t *prog =
    "{ "
    "abs exch abs 2 copy lt "
    "    { exch } "
    "if "
    "pop neg"
    " }";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];

  prog_size = strlen (prog);

  pdf_init();

  /* Create the function */
  fail_if(pdf_fp_func_4_new (2, 1,
                             domain,
                             range,
                             prog,
                             prog_size,
                             NULL,
                             &func) != PDF_OK);

  /*
   * Eval for some values
   */

  /* x = 0, y = 0 */
  in[0] = 0;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - square_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - square_f (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - square_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - square_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.93, y = 0.82 */
  in[0] = 0.93;
  in[1] = 0.82;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - square_f (in[0], in[1])) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_019
 * Description:
 *   Evaluate a type 4 function implementing the predefined function
 *   Cross and compare the results with a C implementation.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
cross_f (pdf_real_t x, pdf_real_t y)
{
  return (-1 * PDF_MIN(pdf_fp_abs(x),pdf_fp_abs(y)));
}

START_TEST(pdf_fp_func_eval_019)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t *prog =
    "{ "
    "abs exch abs 2 copy gt "
    "    { exch } "
    "if "
    "pop neg"
    " }";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];

  prog_size = strlen (prog);

  pdf_init();

  /* Create the function */
  fail_if(pdf_fp_func_4_new (2, 1,
                             domain,
                             range,
                             prog,
                             prog_size,
                             NULL,
                             &func) != PDF_OK);

  /*
   * Eval for some values
   */

  /* x = 0, y = 0 */
  in[0] = 0;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - cross_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - cross_f (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - cross_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - cross_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.93, y = 0.82 */
  in[0] = 0.93;
  in[1] = 0.82;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - cross_f (in[0], in[1])) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_020
 * Description:
 *   Evaluate a type 4 function implementing the predefined function
 *   Rhomboid and compare the results with a C implementation.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
rhomboid_f (pdf_real_t x, pdf_real_t y)
{
  return ((0.9 * pdf_fp_abs(x) + pdf_fp_abs(y)) / 2);
}

START_TEST(pdf_fp_func_eval_020)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t *prog =
    "{ "
    "abs exch abs 0.9 mul add 2 div"
    " }";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];

  prog_size = strlen (prog);

  pdf_init();

  /* Create the function */
  fail_if(pdf_fp_func_4_new (2, 1,
                             domain,
                             range,
                             prog,
                             prog_size,
                             NULL,
                             &func) != PDF_OK);

  /*
   * Eval for some values
   */

  /* x = 0, y = 0 */
  in[0] = 0;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - rhomboid_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - rhomboid_f (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - rhomboid_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - rhomboid_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.93, y = 0.82 */
  in[0] = 0.93;
  in[1] = 0.82;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - rhomboid_f (in[0], in[1])) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_021
 * Description:
 *   Evaluate a type 4 function implementing the predefined function
 *   Diamond and compare the results with a C implementation.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
diamond_f (pdf_real_t x, pdf_real_t y)
{
  pdf_real_t ret;
  pdf_real_t temp1;
  pdf_real_t temp2;

  if ((pdf_fp_abs(x) + pdf_fp_abs(y)) <= 0.75)
    {
      ret = (1 - ((x * x) + (y * y)));
    }
  else if ((pdf_fp_abs(x) + pdf_fp_abs(y)) <= 1.23)
    {
      ret = (1 - (0.85 * pdf_fp_abs(x) + pdf_fp_abs(y)));
    }
  else
    {
      temp1 = (pdf_fp_abs(x) - 1);
      temp2 = (pdf_fp_abs(y) - 1);
      ret = ((temp1 * temp1) + (temp2 * temp2) - 1);
    }

  return ret;
}

START_TEST(pdf_fp_func_eval_021)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t *prog =
    "{ "
    "abs exch abs 2 copy add 0.75 le "
    "   { dup mul exch dup mul add 1 exch sub } "
    "   { 2 copy add 1.23 le "
    "        { 0.85 mul add 1 exch sub } "
    "        { 1 sub dup mul exch 1 sub dup mul add 1 sub } "
    "     ifelse } "
    " ifelse"
    " }";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];

  prog_size = strlen (prog);

  pdf_init();

  /* Create the function */
  fail_if(pdf_fp_func_4_new (2, 1,
                             domain,
                             range,
                             prog,
                             prog_size,
                             NULL,
                             &func) != PDF_OK);

  /*
   * Eval for some values
   */

  /* x = 0, y = 0 */
  in[0] = 0;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - diamond_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - diamond_f (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - diamond_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - diamond_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.93, y = 0.82 */
  in[0] = 0.93;
  in[1] = 0.82;
  fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - diamond_f (in[0], in[1])) > ABS_ERROR);
}
END_TEST


/*
 * **1 - Assure that functions do not accept arguments of the wrong
 * type
 */

/*
 * Test: pdf_fp_func_eval_101
 * Description:
 *   Evaluate a type 4 function and pass a boolean argument to
 *   binary operators that do not accept boolean operators.
 * Success condition:
 *   Return PDF_ETYPE4, set PDF_EBADTYPE in the debug information
 *   and the offset of the error is correct.
 */
START_TEST(pdf_fp_func_eval_101)
{
  char *ops[] = { "add", "sub", "mul", "div", "atan", "exp", "gt",
                  "ge", "lt", "le", "idiv", "mod", "bitshift" };
  char *progstrs[] = { "{ true %s }", "{ true exch %s }" };
  int o, p;;

  for (p = 0; p < sizeof(progstrs)/sizeof(char*); p++)
    {
      for (o = 0; o < sizeof(ops)/sizeof(char*); o++)
        {
          pdf_fp_func_t func;
          pdf_fp_func_debug_t debug;
    
          pdf_real_t domain[2] = {-20.0, 20.0};
          pdf_real_t range[2] = {-1.0, 1.0};
          pdf_real_t in[1];
          pdf_real_t out[1];
        
          /* Create the function */
          fail_if(pdf_fp_func_4_tmake (1, 1,
                                       domain,
                                       range,
                                       &func,
                                       progstrs[p], ops[o]) != PDF_OK);
        
          /*
           * Eval for some values
           */
        
          /* x = -1 
           * boolean identifiers must not be accepted as arguments */
          in[0] = -1;
          fail_if (pdf_fp_func_eval (func, in, out, &debug) != PDF_ETYPE4,
                   TXT_ERR_EVALSUCC, ops[o]);
          fail_if (debug.type4.status != PDF_EBADTYPE);
        }
    }
}
END_TEST

/*
 * Test: pdf_fp_func_eval_102
 * Description:
 *   Evaluate a type 4 function and pass a boolean argument to
 *   unary operators that do not accept boolean operators.
 * Success condition:
 *   Return PDF_ETYPE4, set PDF_EBADTYPE in the debug information
 *   and the offset of the error is correct.
 */
START_TEST(pdf_fp_func_eval_102)
{
  char *ops[] = { "neg", "abs", "ceiling", "floor", "round", "truncate",
                  "sqrt", "sin", "cos", "ln", "log", "cvi", "cvr" };
  int o;

  for (o = 0; o < sizeof(ops)/sizeof(char*); o++)
    {
      pdf_fp_func_t func;
      pdf_fp_func_debug_t debug;

      pdf_real_t domain[0] = {};
      pdf_real_t range[2] = {-1.0, 1.0};
      pdf_real_t in[0];
      pdf_real_t out[1];
    
      /* Create the function */
      fail_if(pdf_fp_func_4_tmake (0, 1,
                                   domain,
                                   range,
                                   &func,
                                   "{ true %s}", ops[o]) != PDF_OK);
    
      /*
       * Eval for some values
       */
    
      /* boolean identifiers must not be accepted as arguments */
      fail_if (pdf_fp_func_eval (func, in, out, &debug) != PDF_ETYPE4);
      fail_if (debug.type4.status != PDF_EBADTYPE);
      fail_if (debug.type4.op != 7);
      fail_if (debug.type4.stack[0] != PDF_FP_FUNC_TYPE4_TRUE);
    }
}
END_TEST

/*
 * Test: pdf_fp_func_eval_103
 * Description:
 *   Evaluate a type 4 function and pass a real argument to
 *   unary operators that do not accept real arguments.
 * Success condition:
 *   Return PDF_ETYPE4, set PDF_EBADTYPE in the debug information
 *   and the offset of the error is correct.
 */
START_TEST(pdf_fp_func_eval_103)
{
  pdf_fp_func_t func;
  pdf_fp_func_debug_t debug;

  pdf_real_t domain[2] = {0.0, 10.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[1];
  pdf_real_t out[1];
   
  /* Create the function */
  fail_if(pdf_fp_func_4_tmake (1, 1,
                               domain,
                               range,
                               &func,
                               "{ not }") != PDF_OK);
  /*
   * Eval for some values
   */

  /* x = 3.14 */
  in[0] = 3.14;
  fail_if (pdf_fp_func_eval (func, in, out, &debug) != PDF_ETYPE4);
  fail_if (debug.type4.status != PDF_EBADTYPE);
  fail_if (debug.type4.op != 2);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_104
 * Description:
 *   Evaluate a type 4 function and pass a real argument to
 *   binary operators that do not accept real arguments.
 * Success condition:
 *   Return PDF_ETYPE4, set PDF_EBADTYPE in the debug information
 *   and the offset of the error is correct.
 */
START_TEST(pdf_fp_func_eval_104)
{
  char *ops[] = { "and", "or", "xor", "bitshift", "idiv", "mod" };
  char *progstrs[] = { "{ 1 %s }", "{ 1 exch %s }" };
  int o, p;

  for (p = 0; p < sizeof(progstrs)/sizeof(char*); p++)
    {
      for (o = 0; o < sizeof(ops)/sizeof(char*); o++)
        {
          pdf_fp_func_t func;
          pdf_fp_func_debug_t debug;
    
          pdf_real_t domain[2] = {-20.0, 20.0};
          pdf_real_t range[2] = {-1.0, 1.0};
          pdf_real_t in[1];
          pdf_real_t out[1];
        
          /* Create the function */
          fail_if(pdf_fp_func_4_tmake(1, 1,
                                      domain,
                                      range,
                                      &func,
                                      progstrs[p], ops[o]) != PDF_OK);
        
          /*
           * Eval for some values
           */
        
          /* x = -1.345 */
          in[0] = -1.345;
          fail_if (pdf_fp_func_eval (func, in, out, &debug) != PDF_ETYPE4,
                   TXT_ERR_EVALSUCC, ops[o]);
          fail_if (debug.type4.status != PDF_EBADTYPE,
                  TXT_ERR_ESTATUS, ops[o]);

          /* x = 1.345 */
          in[0] = 1.345;
          fail_if (pdf_fp_func_eval (func, in, out, &debug) != PDF_ETYPE4,
                  TXT_ERR_EVALSUCC, ops[o]);
          fail_if (debug.type4.status != PDF_EBADTYPE,
                  TXT_ERR_ESTATUS, ops[o]);
        }
    }
}
END_TEST


/*
 * **2 - Assure that the return value is correctly typed.
 */

/*
 * Test: pdf_fp_func_eval_201
 * Description:
 *   Evaluate a type 4 function and pass the result of a binary
 *   function that must return an int to a function that requires
 *   an integer argument.
 * Success condition:
 *   The evaluation does not return an error.
 */
START_TEST(pdf_fp_func_eval_201)
{
  char *ops[] = { "add", "sub", "mul", "idiv", "mod", "bitshift",
                  "and", "or", "xor" };
  int o;

  for (o = 0; o < sizeof(ops)/sizeof(char*); o++)
    {
      pdf_fp_func_t func;
      pdf_fp_func_debug_t debug;

      pdf_real_t domain[0] = {};
      pdf_real_t range[2] = {-1.0, 1.0};
      pdf_real_t in[0];
      pdf_real_t out[1];
    
      /* Create the function */
      fail_if(pdf_fp_func_4_tmake(0, 1,
                                  domain,
                                  range,
                                  &func,
                                  "{ 1 1 %s 1 mod }",
                                  ops[o]) != PDF_OK);
    
      /*
       * Eval for some values
       */
    
      fail_if (pdf_fp_func_eval (func, in, out, &debug) != PDF_OK,
               TXT_ERR_EVAL, ops[o]);
    }
}
END_TEST

/*
 * Test: pdf_fp_func_eval_202
 * Description:
 *   Evaluate a type 4 function and pass the result of a binary
 *   function that must return a real to a function that requires
 *   a boolean argument.
 * Success condition:
 *   Return PDF_ETYPE4, set PDF_EBADTYPE in the debug information,
 */
START_TEST(pdf_fp_func_eval_202)
{
  char *ops[] = { "idiv", "atan", "exp" };
  int o;

  for (o = 0; o < sizeof(ops)/sizeof(char*); o++)
    {
      pdf_fp_func_t func;
      pdf_fp_func_debug_t debug;

      pdf_real_t domain[0] = {};
      pdf_real_t range[2] = {-1.0, 1.0};
      pdf_real_t in[0];
      pdf_real_t out[1];
    
      /* Create the function */
      fail_if(pdf_fp_func_4_tmake(0, 1,
                                  domain,
                                  range,
                                  &func,
                                  "{ 4 3 %s true and }",
                                  ops[o]) != PDF_OK);
    
      /*
       * Eval for some values
       */
    
      fail_if (pdf_fp_func_eval (func, in, out, &debug) != PDF_ETYPE4,
               TXT_ERR_EVALSUCC, ops[o]);
      fail_if (debug.type4.status != PDF_EBADTYPE,
              TXT_ERR_ESTATUS, ops[o]);
    }
}
END_TEST

/*
 * Test: pdf_fp_func_eval_203
 * Description:
 *   Evaluate a type 4 function and pass the result of a binary
 *   function that must return a boolean to a function that requires
 *   a boolean argument.
 * Success condition:
 *   The evaluation does not return an error.
 */
START_TEST(pdf_fp_func_eval_203)
{
  char *ops[] = { "eq", "ne", "gt", "ge", "lt", "le" };
  int o;

  for (o = 0; o < sizeof(ops)/sizeof(char*); o++)
    {
      pdf_fp_func_t func;
      pdf_fp_func_debug_t debug;

      pdf_real_t domain[4] = {-10, 10, -10, 10};
      pdf_real_t range[2] = {-1.0, 1.0};
      pdf_real_t in[2];
      pdf_real_t out[1];
    
      /* Create the function */
      fail_if(pdf_fp_func_4_tmake(2, 1,
                                  domain,
                                  range,
                                  &func,
                                  "{ %s true and pop 1 }",
                                  ops[o]) != PDF_OK);
    
      /*
       * Eval for some values
       */
      in[0] = 0;
      in[1] = 1; 
      fail_if (pdf_fp_func_eval (func, in, out, &debug) != PDF_OK,
               TXT_ERR_EVAL, ops[o]);

      in[0] = -1;
      in[1] = 1; 
      fail_if (pdf_fp_func_eval (func, in, out, &debug) != PDF_OK,
               TXT_ERR_EVAL, ops[o]);

      in[0] = 5;
      in[1] = 5; 
      fail_if (pdf_fp_func_eval (func, in, out, &debug) != PDF_OK,
               TXT_ERR_EVAL, ops[o]);
    }
}
END_TEST

/*
 * Test: pdf_fp_func_eval_204
 * Description:
 *   Evaluate a type 4 function and pass the result of a binary
 *   function that must return a boolean to a function than 
 *   requires a boolean argument.
 * Success condition:
 *   The evaluation does not return an error.
 */
START_TEST(pdf_fp_func_eval_204)
{
  char *ops[] = { "and", "or", "xor" };
  int o;

  for (o = 0; o < sizeof(ops)/sizeof(char*); o++)
    {
      pdf_fp_func_t func;
      pdf_fp_func_debug_t debug;

      pdf_real_t domain[0] = {};
      pdf_real_t range[2] = {-1.0, 1.0};
      pdf_real_t in[0];
      pdf_real_t out[1];
    
      /* Create the function */
      fail_if(pdf_fp_func_4_tmake(0, 1,
                                  domain,
                                  range,
                                  &func,
                                  "{ false true %s true and pop 1 }",
                                  ops[o]) != PDF_OK);
    
      /*
       * Eval for some values
       */
      fail_if (pdf_fp_func_eval (func, in, out, &debug) != PDF_OK,
               TXT_ERR_EVAL, ops[o]);
    }
}
END_TEST

/*
 * Test: pdf_fp_func_eval_205
 * Description:
 *   Evaluate a type 4 function and pass the result of a binary
 *   function that must return a boolean to a function that requires
 *   a boolean argument.
 * Success condition:
 *   The evaluation does not return an error.
 */
START_TEST(pdf_fp_func_eval_205)
{
  char *ops[] = { "not" };
  int o;

  for (o = 0; o < sizeof(ops)/sizeof(char*); o++)
    {
      pdf_fp_func_t func;
      pdf_fp_func_debug_t debug;

      pdf_real_t domain[0] = {};
      pdf_real_t range[2] = {-1.0, 1.0};
      pdf_real_t in[0];
      pdf_real_t out[1];
    
      /* Create the function */
      fail_if(pdf_fp_func_4_tmake(0, 1,
                                  domain,
                                  range,
                                  &func,
                                  "{ true %s true and pop 1 }",
                                  ops[o]) != PDF_OK);
    
      /*
       * Eval for some values
       */
      fail_if (pdf_fp_func_eval (func, in, out, &debug) != PDF_OK,
               TXT_ERR_EVAL, ops[o]);
    }
}
END_TEST

/*
 * Test: pdf_fp_func_eval_206
 * Description:
 *   Evaluate a type 4 function and pass the result of an unary
 *   function that must return a real to a function that requires
 *   a boolean argument.
 * Success condition:
 *   Return PDF_ETYPE4, set PDF_EBADTYPE in the debug information,
 */
START_TEST(pdf_fp_func_eval_206)
{
  char *ops[] = { "add", "mul", "sub", "div", "atan", "exp" };
  int o;

  for (o = 0; o < sizeof(ops)/sizeof(char*); o++)
    {
      pdf_fp_func_t func;
      pdf_fp_func_debug_t debug;

      pdf_real_t domain[4] = {-10, 10, -10, 10};
      pdf_real_t range[2] = {-1.0, 1.0};
      pdf_real_t in[2];
      pdf_real_t out[1];
    
      /* Create the function */
      fail_if(pdf_fp_func_4_tmake(2, 1,
                                  domain,
                                  range,
                                  &func,
                                  "{ %s true and }",
                                  ops[o]) != PDF_OK);
    
      /*
       * Eval for some values
       */
      in[0] = -1.43;
      in[1] = 1.43; 
      fail_if (pdf_fp_func_eval (func, in, out, &debug) != PDF_ETYPE4,
               TXT_ERR_EVALSUCC, ops[o]);
      fail_if (debug.type4.status != PDF_EBADTYPE,
              TXT_ERR_ESTATUS, ops[o]);
    }
}
END_TEST

/*
 * Test: pdf_fp_func_eval_207
 * Description:
 *   Evaluate a type 4 function and pass the result of an unary
 *   function that must return a real to a function that requires
 *   a boolean argument.
 * Success condition:
 *   The evaluation does not return an error.
 */
START_TEST(pdf_fp_func_eval_207)
{
  char *ops[] = { "neg", "abs", "ceiling", "floor", "round", "truncate",
                  "cvi", "not" };
  int o;

  for (o = 0; o < sizeof(ops)/sizeof(char*); o++)
    {
      pdf_fp_func_t func;
      pdf_fp_func_debug_t debug;

      pdf_real_t domain[0] = {};
      pdf_real_t range[2] = {-1.0, 1.0};
      pdf_real_t in[0];
      pdf_real_t out[1];
    
      /* Create the function */
      fail_if(pdf_fp_func_4_tmake(0, 1,
                                  domain,
                                  range,
                                  &func,
                                  "{ 4 %s 1 mod }",
                                  ops[o]) != PDF_OK);
    
      /*
       * Eval for some values
       */
      fail_if (pdf_fp_func_eval (func, in, out, &debug) != PDF_OK,
               TXT_ERR_EVAL, ops[o]);
    }
}
END_TEST

/*
 * Test: pdf_fp_func_eval_208
 * Description:
 *   Evaluate a type 4 function and pass the result of a unary
 *   function that must return an integer to a function that requires
 *   an boolean argument.
 * Success condition:
 *   Return PDF_ETYPE4, set PDF_EBADTYPE in the debug information,
 */
START_TEST(pdf_fp_func_eval_208)
{
  char *ops[] = { "neg", "abs", "ceiling", "floor", "round", "truncate",
                  "sqrt", "sin", "cos", "ln", "log", "cvr" };
  int o;

  for (o = 0; o < sizeof(ops)/sizeof(char*); o++)
    {
      pdf_fp_func_t func;
      pdf_fp_func_debug_t debug;

      pdf_real_t domain[2] = {-10.0, 10.0};
      pdf_real_t range[2] = {-1.0, 1.0};
      pdf_real_t in[1];
      pdf_real_t out[1];
    
      /* Create the function */
      fail_if(pdf_fp_func_4_tmake(1, 1,
                                  domain,
                                  range,
                                  &func,
                                  "{ %s true and }",
                                  ops[o]) != PDF_OK);
    
      /*
       * Eval for some values
       */
      in[0] = 1.43; 
      fail_if (pdf_fp_func_eval (func, in, out, &debug) != PDF_ETYPE4,
               TXT_ERR_EVALSUCC, ops[o]);
      fail_if (debug.type4.status != PDF_EBADTYPE,
              TXT_ERR_ESTATUS, ops[o]);
    }
}
END_TEST

/*
 * **3 - Assure that evaluations are correct.
 */

/*
 * Test: pdf_fp_func_eval_301
 * Description:
 *   Evaluate unary type 4 functions that take a real or an
 *   integer argument.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
truncate_f(pdf_real_t x)
{
    return TRUNC(x);
}
static pdf_real_t
sin_f(pdf_real_t x)
{
    return pdf_fp_sin(DEG2RAD(x));
}
static pdf_real_t
cos_f(pdf_real_t x)
{
    return pdf_fp_cos(DEG2RAD(x));
}
static pdf_real_t
neg_f(pdf_real_t x)
{
    return -x;
}
static pdf_real_t
abs_f(pdf_real_t x)
{
    return pdf_fp_abs(x);
}
static pdf_real_t
ceiling_f(pdf_real_t x)
{
    return pdf_fp_ceil(x);
}
static pdf_real_t
floor_f(pdf_real_t x)
{
    return pdf_fp_floor(x);
}
static pdf_real_t
round2_f(pdf_real_t x)
{
    return pdf_fp_floor(0.5 +x);
}
START_TEST(pdf_fp_func_eval_301)
{
    struct t_sample_elem
    {
        char *op;
        pdf_real_t (*fun)(pdf_real_t);
    } t_sample[] =
    { { "neg",      neg_f },
      { "abs",      abs_f },
      { "ceiling",  ceiling_f },
      { "floor",    floor_f },
      { "round",    round2_f },
      { "truncate", truncate_f },
      { "sin",      sin_f },
      { "cos",      cos_f } };
    int t;

    for (t = 0; t < sizeof(t_sample)/sizeof(struct t_sample_elem); t++)
    {
        pdf_fp_func_t func;
        pdf_fp_func_debug_t debug;

        pdf_real_t domain[2] = {-200.0, 200.0};
        pdf_real_t range[2] = {-100.0, 100.0};
        pdf_real_t in[1];
        pdf_real_t out[1];

        /* Create the function */
        fail_if(pdf_fp_func_4_tmake(1, 1,
                    domain,
                    range,
                    &func,
                    "{ %s }", t_sample[t].op) != PDF_OK);

        /*
         * Eval for some values
         */

        /* x = 0 */
        in[0] = 0;
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK, 
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0])) > ABS_ERROR,
                TXT_ERR_RES, t_sample[t].op);

        /* x = 5.23 */
        in[0] = 5.23;
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK,
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0])) > ABS_ERROR,
                TXT_ERR_RES, t_sample[t].op);

        /* x = -10.5 */
        in[0] = -10.5; 
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK,
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0])) > ABS_ERROR,
                TXT_ERR_RES, t_sample[t].op);

        /* x = 10.5 */
        in[0] = 10.5; 
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK,
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0])) > ABS_ERROR,
                TXT_ERR_RES, t_sample[t].op);

        /* x = 3 */
        in[0] = 3; 
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK,
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0])) > ABS_ERROR,
                TXT_ERR_RES, t_sample[t].op);
    }
}
END_TEST

/*
 * Test: pdf_fp_func_eval_302
 * Description:
 *   Evaluate unary type 4 functions that take a positive 
 *   real or an positive integer argument.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
sqrt_f(pdf_real_t x)
{
    return pdf_fp_sqrt(x);
}
static pdf_real_t
ln_f(pdf_real_t x)
{
    return pdf_fp_log(x);
}
static pdf_real_t
log_f(pdf_real_t x)
{
    return pdf_fp_log10(x);
}
START_TEST(pdf_fp_func_eval_302)
{
    struct t_sample_elem
    {
        char *op;
        pdf_real_t (*fun)(pdf_real_t);
    } t_sample[] =
    { { "sqrt",     sqrt_f },
      { "ln",       ln_f },
      { "log",      log_f } };
    int t;

    for (t = 0; t < sizeof(t_sample)/sizeof(struct t_sample_elem); t++)
    {
        pdf_fp_func_t func;
        pdf_fp_func_debug_t debug;

        pdf_real_t domain[2] = {-200.0, 200.0};
        pdf_real_t range[2] = {-100.0, 100.0};
        pdf_real_t in[1];
        pdf_real_t out[1];

        /* Create the function */
        fail_if(pdf_fp_func_4_tmake(1, 1,
                    domain,
                    range,
                    &func,
                    "{ %s }", t_sample[t].op) != PDF_OK);

        /*
         * Eval for some values
         */

        /* x = 5.23 */
        in[0] = 5.23;
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK,
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0])) > ABS_ERROR,
                TXT_ERR_RES, t_sample[t].op);

        /* x = 10.5 */
        in[0] = 10.5; 
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK,
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0])) > ABS_ERROR,
                TXT_ERR_RES, t_sample[t].op);

        /* x = 3 */
        in[0] = 3; 
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK,
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0])) > ABS_ERROR,
                TXT_ERR_RES, t_sample[t].op);
    }
}
END_TEST

/*
 * Test: pdf_fp_func_eval_303
 * Description:
 *   Evaluate binary type 4 functions that take real or
 *   integer arguments.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t 
add_f(pdf_real_t x, pdf_real_t y)
{
    return pdf_fp_add(x, y);
}
static pdf_real_t 
sub_f(pdf_real_t x, pdf_real_t y)
{
    return pdf_fp_sub(x, y);
}
static pdf_real_t 
mul_f(pdf_real_t x, pdf_real_t y)
{
    return pdf_fp_mul(x, y);
}
static pdf_real_t 
div_f(pdf_real_t x, pdf_real_t y)
{
    return pdf_fp_div(x, y);
}
static pdf_real_t
exp_f(pdf_real_t x, pdf_real_t y)
{
    return pdf_fp_pow(x, y);
}
static pdf_real_t
atan_f(pdf_real_t x, pdf_real_t y)
{
    return DEG2RAD(pdf_fp_atan2(x, y));
}
START_TEST(pdf_fp_func_eval_303)
{
    struct t_sample_elem
    {
        char *op;
        pdf_real_t (*fun)(pdf_real_t, pdf_real_t);
    } t_sample[] =
    { { "add",  add_f },
      { "sub",  sub_f },
      { "mul",  mul_f },
      { "div",  div_f }, 
      { "exp",  exp_f },
      { "atan", atan_f } };
    int t;

    for (t = 0; t < sizeof(t_sample)/sizeof(struct t_sample_elem); t++)
    {
        pdf_fp_func_t func;
        pdf_fp_func_debug_t debug;

        pdf_real_t domain[4] = {-200.0, 200.0, -200.0, 200};
        pdf_real_t range[2] = {-200.0, 200.0};
        pdf_real_t in[2];
        pdf_real_t out[1];

        /* Create the function */
        fail_if(pdf_fp_func_4_tmake(2, 1,
                    domain,
                    range,
                    &func,
                    "{ %s }", t_sample[t].op) != PDF_OK);

        /*
         * Eval for some values
         */

        /* x = 0, y = 2.345 */
        in[0] = 0;
        in[1] = 2.345;
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK,
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0], in[1])) 
                > ABS_ERROR, TXT_ERR_RES, t_sample[t].op);

        /* x = -15.345, y = 2.345 */
        in[0] = -15.345;
        in[1] = 2.345;
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK,
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0], in[1])) 
                > ABS_ERROR, TXT_ERR_RES, t_sample[t].op);
        /* x = 3.333, y =  -0.988*/
        in[0] = 3.333;
        in[1] = -0.988;
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK,
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0], in[1])) 
                > ABS_ERROR, TXT_ERR_RES, t_sample[t].op);
        /* x = -19.6, y = -2.22 */
        in[0] = -19.6;
        in[1] = -2.22;
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK,
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0], in[1])) 
                > ABS_ERROR, TXT_ERR_RES, t_sample[t].op);
        /* x = 4, y = -3 */
        in[0] = 4;
        in[1] = -3;
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK,
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0], in[1])) 
                > ABS_ERROR, TXT_ERR_RES, t_sample[t].op);
    }
}
END_TEST

/*
 * Test: pdf_fp_func_eval_304
 * Description:
 *   Evaluate binary type 4 functions that take integer
 *   arguments.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
idiv_f (pdf_real_t x, pdf_real_t y)
{
    return INT(pdf_fp_div(INT(x), INT(y)));
}
static pdf_real_t
mod_f (pdf_real_t x, pdf_real_t y)
{
    return pdf_fp_mod(INT(x), INT(y));
}
static pdf_real_t
bitshift_f (pdf_real_t x, pdf_real_t y)
{
  pdf_i32_t i = INT(y);

  if (i >= 0) 
    return (i < 32) ? INT(x) << i : 0;
  else
    return (i > -32) ? INT(x) >> -i : 0;
}
static pdf_real_t
and_i_f (pdf_real_t x, pdf_real_t y)
{
    return INT(x) & INT(y);
}
static pdf_real_t
or_i_f (pdf_real_t x, pdf_real_t y)
{
    return INT(x) | INT(y);
}
static pdf_real_t
xor_i_f (pdf_real_t x, pdf_real_t y)
{
    return INT(x) ^ INT(y);
}
START_TEST(pdf_fp_func_eval_304)
{
    struct t_sample_elem
    {
        char *op;
        pdf_real_t (*fun)(pdf_real_t, pdf_real_t);
    } t_sample[] =
    { { "idiv",     idiv_f },
      { "mod",      mod_f },
      { "bitshift", bitshift_f },
      { "and",      and_i_f },
      { "or",       or_i_f },
      { "xor",      xor_i_f } };
    int t;

    for (t = 0; t < sizeof(t_sample)/sizeof(struct t_sample_elem); t++)
    {
        pdf_fp_func_t func;
        pdf_fp_func_debug_t debug;

        pdf_real_t domain[4] = {-200.0, 200.0, -200.0, 200};
        pdf_real_t range[2] = {-100.0, 100.0};
        pdf_real_t in[2];
        pdf_real_t out[1];

        /* Create the function */
        fail_if(pdf_fp_func_4_tmake(2, 1,
                    domain,
                    range,
                    &func,
                    "{ cvi exch cvi exch %s }", 
                    t_sample[t].op) != PDF_OK);

        /*
         * Eval for some values
         */

        /* x = 0, y = 2 */
        in[0] = 0;
        in[1] = 2;
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK,
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0], in[1])) 
                > ABS_ERROR, TXT_ERR_RES, t_sample[t].op);

        /* x = -15, y = 2 */
        in[0] = -15;
        in[1] = 2;
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK,
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0], in[1])) 
                > ABS_ERROR, TXT_ERR_RES, t_sample[t].op);
        /* x = 33, y =  -3 */
        in[0] = 33;
        in[1] = -3;
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK,
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0], in[1])) 
                > ABS_ERROR, TXT_ERR_RES, t_sample[t].op);
        /* x = -19, y = -2 */
        in[0] = -19;
        in[1] = -2;
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK,
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0], in[1])) 
                > ABS_ERROR, TXT_ERR_RES, t_sample[t].op);
    }
}
END_TEST

/*
 * Test: pdf_fp_func_eval_305
 * Description:
 *   Evaluate binary type 4 functions that takes integers
 *   or reals as arguments and returns a bool..
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
eq_f(pdf_real_t x, pdf_real_t y)
{
    return (x == y) ? 1 : 0;
}
static pdf_real_t
ne_f(pdf_real_t x, pdf_real_t y)
{
    return (x != y) ? 1 : 0;
}
static pdf_real_t
gt_f(pdf_real_t x, pdf_real_t y)
{
    return (x > y) ? 1 : 0;
}
static pdf_real_t
ge_f(pdf_real_t x, pdf_real_t y)
{
    return (x >= y) ? 1 : 0;
}
static pdf_real_t
lt_f(pdf_real_t x, pdf_real_t y)
{
    return (x < y) ? 1 : 0;
}
static pdf_real_t
le_f(pdf_real_t x, pdf_real_t y)
{
    return (x <= y) ? 1 : 0;
}
START_TEST(pdf_fp_func_eval_305)
{
    struct t_sample_elem
    {
        char *op;
        pdf_real_t (*fun)(pdf_real_t, pdf_real_t);
    } t_sample[] =
    { { "eq",      eq_f },
        { "ne",      ne_f }, 
        { "gt",      gt_f }, 
        { "ge",      ge_f }, 
        { "lt",      lt_f }, 
        { "le",      le_f } };
    int t;

    for (t = 0; t < sizeof(t_sample)/sizeof(struct t_sample_elem); t++)
    {
        pdf_fp_func_t func;
        pdf_fp_func_debug_t debug;

        pdf_real_t domain[4] = {-10.0, 10.0, -10.0, 10.0};
        pdf_real_t range[2] = {0, 1};
        pdf_real_t in[2];
        pdf_real_t out[1];

        /* Create the function */
        fail_if(pdf_fp_func_4_tmake(2, 1,
                    domain,
                    range,
                    &func,
                    "{ %s { 1 } { 0 } ifelse }", 
                    t_sample[t].op) != PDF_OK);

        /*
         * Eval for some values
         */

        /* x = 0.34, y = -1.345 */
        in[0] = 0.34;
        in[1] = -1.345;
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK, 
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0], in[1])) > ABS_ERROR,
                TXT_ERR_RES, t_sample[t].op);

        /* x = 5.98, y = 5.98 */
        in[0] = 5.98;
        in[1] = 5.98;
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK, 
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0], in[1])) > ABS_ERROR,
                TXT_ERR_RES, t_sample[t].op);

        /* x = 5.23, y = 0.343 */
        in[0] = 5.23;
        in[1] = 0.343;
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK, 
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0], in[1])) > ABS_ERROR,
                TXT_ERR_RES, t_sample[t].op);

        /* x = -1.943, y = 0.134 */
        in[0] = -1.943;
        in[1] = 0.134;
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK, 
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0], in[1])) > ABS_ERROR,
                TXT_ERR_RES, t_sample[t].op);

        /* x = -2.23. y = -1.93 */
        in[0] = -2.23;
        in[1] = -1.93;
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK, 
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0], in[1])) > ABS_ERROR,
                TXT_ERR_RES, t_sample[t].op);
    }
}
END_TEST

/*
 * Test: pdf_fp_func_eval_306
 * Description:
 *   Evaluate binary type 4 functions that takes booleans
 *   as arguments.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
and_b_f(pdf_real_t x, pdf_real_t y)
{
    return (x == 1) && (y == 1);
}
static pdf_real_t
or_b_f(pdf_real_t x, pdf_real_t y)
{
    return (x == 1) || (y == 1);
}
static pdf_real_t
xor_b_f(pdf_real_t x, pdf_real_t y)
{
    return ((x == 1) && (y != 1)) ||
        ((x != 1) && (y == 1));
}
START_TEST(pdf_fp_func_eval_306)
{
    struct t_sample_elem
    {
        char *op;
        pdf_real_t (*fun)(pdf_real_t, pdf_real_t);
    } t_sample[] =
    { { "and",      and_b_f },
      { "or",       or_b_f }, 
      { "xor",      xor_b_f } };
    int t;

    for (t = 0; t < sizeof(t_sample)/sizeof(struct t_sample_elem); t++)
    {
        pdf_fp_func_t func;
        pdf_fp_func_debug_t debug;

        pdf_real_t domain[4] = {-10.0, 10.0, -10.0, 10.0};
        pdf_real_t range[2] = {0, 1};
        pdf_real_t in[2];
        pdf_real_t out[1];

        /* Create the function */
        fail_if(pdf_fp_func_4_tmake(2, 1,
                    domain,
                    range,
                    &func,
                    "{ 1 eq exch 1 eq exch %s "
                    "  { 1 } { 0 } ifelse }",
                    t_sample[t].op) != PDF_OK);

        /*
         * Eval for some values
         */

        /* x = 0, y = 0 */
        in[0] = 0;
        in[1] = 0;
        pdf_fp_func_eval (func, in, out, &debug);
        fail_if(pdf_fp_func_eval (func, in, out, &debug) != PDF_OK, 
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0], in[1])) > ABS_ERROR,
                TXT_ERR_RES, t_sample[t].op);

        /* x = 1, y = 1 */
        in[0] = 1;
        in[1] = 1;
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK, 
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0], in[1])) > ABS_ERROR,
                TXT_ERR_RES, t_sample[t].op);

        /* x = 0, y = 1 */
        in[0] = 0;
        in[1] = 1;
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK, 
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0], in[1])) > ABS_ERROR,
                TXT_ERR_RES, t_sample[t].op);

        /* x = 1, y = 0 */
        in[0] = 1;
        in[1] = 0;
        fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK, 
                TXT_ERR_EVAL, t_sample[t].op);
        fail_if(pdf_fp_abs(out[0] - t_sample[t].fun (in[0], in[1])) > ABS_ERROR,
                TXT_ERR_RES, t_sample[t].op);
    }
}
END_TEST

/*
 * Test: pdf_fp_func_eval_307
 * Description:
 *   Evaluate unary type 4 functions that takes booleans
 *   as arguments.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
not_b_f(pdf_real_t x)
{
    return !x;
}
START_TEST(pdf_fp_func_eval_307)
{
    pdf_fp_func_t func;
    pdf_fp_func_debug_t debug;

    pdf_real_t domain[2] = {-10.0, 10.0};
    pdf_real_t range[2] = {0, 1};
    pdf_real_t in[1];
    pdf_real_t out[1];

    /* Create the function */
    fail_if(pdf_fp_func_4_tmake(1, 1,
                domain,
                range,
                &func,
                "{ 1 eq not { 1 } { 0 } ifelse }") != PDF_OK);

    /*
     * Eval for some values
     */

    /* x = 0 */
    in[0] = 0;
    fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
    fail_if(pdf_fp_abs(out[0] - not_b_f(in[0])) > ABS_ERROR);

    /* x = 1 */
    in[0] = 1;
    fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
    fail_if(pdf_fp_abs(out[0] - not_b_f(in[0])) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_308
 * Description:
 *   Evaluate unary type 4 functions that takes integers
 *   as arguments.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
static pdf_real_t
not_i_f(pdf_real_t x)
{
    return ~INT(x);
}
START_TEST(pdf_fp_func_eval_308)
{
    pdf_fp_func_t func;
    pdf_fp_func_debug_t debug;

    pdf_real_t domain[2] = {-100.0, 100.0};
    pdf_real_t range[2] = {-100, 100};
    pdf_real_t in[1];
    pdf_real_t out[1];

    /* Create the function */
    fail_if(pdf_fp_func_4_tmake(1, 1,
                domain,
                range,
                &func,
                "{ cvi not }") != PDF_OK);

    /*
     * Eval for some values
     */

    /* x = 0 */
    in[0] = 0;
    fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
    fail_if(pdf_fp_abs(out[0] - not_i_f(in[0])) > ABS_ERROR);

    /* x = 17 */
    in[0] = 17;
    fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
    fail_if(pdf_fp_abs(out[0] - not_i_f(in[0])) > ABS_ERROR);

    /* x = -43 */
    in[0] = -43;
    fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
    fail_if(pdf_fp_abs(out[0] - not_i_f(in[0])) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_309
 * Description:
 *   Evaluate unary type 4 functions that that uses the stack
 *   manipulator pop.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
START_TEST(pdf_fp_func_eval_309)
{
    pdf_fp_func_t func;
    pdf_fp_func_debug_t debug;

    pdf_real_t domain[4] = {-10.0, 10.0, -10.0, 10};
    pdf_real_t range[2] = {0, 1};
    pdf_real_t in[2];
    pdf_real_t out[1];

    /* Create the function */
    fail_if(pdf_fp_func_4_tmake(2, 1,
                domain,
                range,
                &func,
                "{ pop }") != PDF_OK);

    /*
     * Eval for some values
     */

    /* x = 0, y = 1 */
    in[0] = 0;
    in[1] = 1;
    fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
    fail_if(pdf_fp_abs(out[0] - in[0]) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_310
 * Description:
 *   Evaluate unary type 4 functions that that uses the stack
 *   manipulator exch.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
START_TEST(pdf_fp_func_eval_310)
{
    pdf_fp_func_t func;
    pdf_fp_func_debug_t debug;

    pdf_real_t domain[4] = {-10.0, 10.0, -10.0, 10};
    pdf_real_t range[4] = {-10.0, 10.0, -10.0, 10};
    pdf_real_t in[2];
    pdf_real_t out[2];

    /* Create the function */
    fail_if(pdf_fp_func_4_tmake(2, 2,
                domain,
                range,
                &func,
                "{ exch }") != PDF_OK);

    /*
     * Eval for some values
     */

    /* x = 0, y = 1 */
    in[0] = 0;
    in[1] = 1;
    fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
    fail_if(pdf_fp_abs(out[0] - in[1]) > ABS_ERROR);
    fail_if(pdf_fp_abs(out[1] - in[0]) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_311
 * Description:
 *   Evaluate unary type 4 functions that that uses the stack
 *   manipulator dup.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
START_TEST(pdf_fp_func_eval_311)
{
    pdf_fp_func_t func;
    pdf_fp_func_debug_t debug;

    pdf_real_t domain[2] = {-10.0, 10.0};
    pdf_real_t range[4] = {-10.0, 10.0, -10.0, 10};
    pdf_real_t in[1];
    pdf_real_t out[2];

    /* Create the function */
    fail_if(pdf_fp_func_4_tmake(1, 2,
                domain,
                range,
                &func,
                "{ dup }") != PDF_OK);

    /*
     * Eval for some values
     */

    /* x = 0, y = 1 */
    in[0] = 3;
    fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
    fail_if(pdf_fp_abs(out[0] - in[0]) > ABS_ERROR);
    fail_if(pdf_fp_abs(out[1] - in[0]) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_312
 * Description:
 *   Evaluate unary type 4 functions that that uses the stack
 *   manipulator copy.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
START_TEST(pdf_fp_func_eval_312)
{
    pdf_fp_func_t func;
    pdf_fp_func_debug_t debug;

    pdf_real_t domain[6] = {-10.0, 10.0, -10.0, 10.0, -10.0, 10.0};
    pdf_real_t range[10] = {-10.0, 10.0, -10.0, 10.0, -10.0, 10.0,
                            -10.0, 10.0, -10.0, 10};
    pdf_real_t in[3];
    pdf_real_t out[5];

    /* Create the function */
    fail_if(pdf_fp_func_4_tmake(3, 5,
                domain,
                range,
                &func,
                "{ 2 copy }") != PDF_OK);

    /*
     * Eval for some values
     */

    /* x = 0, y = 1 */
    in[0] = 3;
    in[1] = 2;
    in[2] = 1;
    fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
    fail_if(pdf_fp_abs(out[0] - in[0]) > ABS_ERROR);
    fail_if(pdf_fp_abs(out[1] - in[1]) > ABS_ERROR);
    fail_if(pdf_fp_abs(out[2] - in[2]) > ABS_ERROR);
    fail_if(pdf_fp_abs(out[3] - in[1]) > ABS_ERROR);
    fail_if(pdf_fp_abs(out[4] - in[2]) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_313
 * Description:
 *   Evaluate unary type 4 functions that that uses the stack
 *   manipulator index.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
START_TEST(pdf_fp_func_eval_313)
{
    pdf_fp_func_t func;
    pdf_fp_func_debug_t debug;

    pdf_real_t domain[6] = {-10.0, 10.0, -10.0, 10.0, -10.0, 10.0};
    pdf_real_t range[8] = {-10.0, 10.0, -10.0, 10.0, -10.0, 10.0,
                           -10.0, 10.0};
    pdf_real_t in[3];
    pdf_real_t out[4];

    /* Create the function */
    fail_if(pdf_fp_func_4_tmake(3, 4,
                domain,
                range,
                &func,
                "{ 2 index }") != PDF_OK);

    /*
     * Eval for some values
     */

    /* x = 0, y = 1 */
    in[0] = 3;
    in[1] = 2;
    in[2] = 1;
    fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
    fail_if(pdf_fp_abs(out[0] - in[0]) > ABS_ERROR);
    fail_if(pdf_fp_abs(out[1] - in[1]) > ABS_ERROR);
    fail_if(pdf_fp_abs(out[2] - in[2]) > ABS_ERROR);
    fail_if(pdf_fp_abs(out[3] - in[0]) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_314
 * Description:
 *   Evaluate unary type 4 functions that that uses the stack
 *   manipulator roll.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
START_TEST(pdf_fp_func_eval_314)
{
    pdf_fp_func_t func;
    pdf_fp_func_debug_t debug;

    pdf_real_t domain[14] = {-10.0, 10.0, -10.0, 10.0, -10.0, 10.0,
                             -10.0, 10.0, -10.0, 10.0, -10.0, 10.0,
                             -10.0, 10.0};
    pdf_real_t range[10] = {-10.0, 10.0, -10.0, 10.0, -10.0, 10.0,
                           -10.0, 10.0, -10.0, 10.0};
    pdf_real_t in[7];
    pdf_real_t out[5];

    /* Create the function */
    fail_if(pdf_fp_func_4_tmake(7, 5,
                domain,
                range,
                &func,
                "{ cvi exch cvi exch roll }") != PDF_OK);

    /*
     * Eval for some values
     */

    /* 1 - 2 - 3 - 4 - 5 */
    in[0] = 1;
    in[1] = 2;
    in[2] = 3;
    in[3] = 4;
    in[4] = 5;
    in[5] = 4;
    in[6] = 3;
    fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
    fail_if(pdf_fp_abs(out[0] - in[0]) > ABS_ERROR);
    fail_if(pdf_fp_abs(out[1] - in[2]) > ABS_ERROR);
    fail_if(pdf_fp_abs(out[2] - in[3]) > ABS_ERROR);
    fail_if(pdf_fp_abs(out[3] - in[4]) > ABS_ERROR);
    fail_if(pdf_fp_abs(out[4] - in[1]) > ABS_ERROR);

    /* 1 - 2 - 3 - 4 - 5 */
    in[0] = 1;
    in[1] = 3;
    in[2] = 4;
    in[3] = 5;
    in[4] = 2;
    in[5] = 4;
    in[6] = -3;
    fail_if(pdf_fp_func_eval (func, in, out, NULL) != PDF_OK);
    fail_if(pdf_fp_abs(out[0] - in[0]) > ABS_ERROR);
    fail_if(pdf_fp_abs(out[1] - in[4]) > ABS_ERROR);
    fail_if(pdf_fp_abs(out[2] - in[1]) > ABS_ERROR);
    fail_if(pdf_fp_abs(out[3] - in[2]) > ABS_ERROR);
    fail_if(pdf_fp_abs(out[4] - in[3]) > ABS_ERROR);
}
END_TEST



/*
 * **4 - Check some special cases
 */

/*
 * Test: pdf_fp_func_eval_401
 * Description:
 *   Evaluate a type 4 function that uses the div, idiv or mod 
 *   operators and supplies invalid arguments (division by zero).
 * Success condition:
 *   Return PDF_ETYPE4, set PDF_EMATH in the debug information,
 */
START_TEST(pdf_fp_func_eval_401)
{
  char *ops[] = { "div", "idiv", "mod" };

  int o;

  for (o = 0; o < sizeof(ops)/sizeof(char*); o++)
    {
      pdf_fp_func_t func;
      pdf_fp_func_debug_t debug;
  
      pdf_real_t domain[4] = {-10, 10, -10.0, 10};
      pdf_real_t range[2] = {-10, 10};
      pdf_real_t in[2];
      pdf_real_t out[1];
  
      /* Create the function */
      fail_if(pdf_fp_func_4_tmake (2, 1,
                  domain,
                  range,
                  &func,
                  "{ cvi exch cvi exch %s }", ops[o]) != PDF_OK);
  
      /*
       * Eval for some values
       */
  
      /* x = 1, y = 0 */
      in[0] = 1;
      in[1] = 0;
      fail_if(pdf_fp_func_eval (func, in, out, &debug) != PDF_ETYPE4);
      fail_if (debug.type4.status != PDF_EMATH);
    }
}
END_TEST

/*
 * Test: pdf_fp_func_eval_402
 * Description:
 *   Evaluate a type 4 function that passes an negative value to
 *   the sqrt operator.
 * Success condition:
 *   Return PDF_ETYPE4, set PDF_EINVRANGE in the debug information,
 */

START_TEST(pdf_fp_func_eval_402)
{
    pdf_fp_func_t func;
    pdf_fp_func_debug_t debug;

    pdf_real_t domain[2] = {-1.0, 1.0};
    pdf_real_t range[2] = {-1.0, 1.0};
    pdf_real_t in[1];
    pdf_real_t out[1];

    /* Create the function */
    fail_if(pdf_fp_func_4_tmake (1, 1,
                                 domain,
                                 range,
                                 &func,
                                 "{ sqrt }") != PDF_OK);

    /* x = 0, y = 0 */
    in[0] = -1;
    fail_if (pdf_fp_func_eval (func, in, out, &debug) != PDF_ETYPE4);
    fail_if (debug.type4.status != PDF_EINVRANGE);
    fail_if (debug.type4.op != 2);
    fail_if (debug.type4.stack[0] != -1);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_403
 * Description:
 *   Evaluate a  type 4 function that passes invalid values to 
 *   the log and ln operators.
 * Success condition:
 *   Return PDF_ETYPE4, set PDF_EINVRANGE in the debug information,
 */
START_TEST(pdf_fp_func_eval_403)
{
  char *ops[] = { "log", "ln" };

  int o;

  for (o = 0; o < sizeof(ops)/sizeof(char*); o++)
    {
      pdf_fp_func_t func;
      pdf_fp_func_debug_t debug;
  
      pdf_real_t domain[2] = {-1.0, 1.0};
      pdf_real_t range[2] = {-1.0, 1.0};
      pdf_real_t in[1];
      pdf_real_t out[1];
  
      /* Create the function */
      fail_if(pdf_fp_func_4_tmake (1, 1,
                                   domain,
                                   range,
                                   &func,
                                   "{ %s }", ops[o]) != PDF_OK);
  
      /* x = 0, */
      in[0] = 0;
      fail_if (pdf_fp_func_eval (func, in, out, &debug) != PDF_ETYPE4);
      fail_if (debug.type4.status != PDF_EINVRANGE);
      fail_if (debug.type4.op != 2);
      fail_if (debug.type4.stack[0] != 0);
  
      /* x = -1, */
      in[0] = -1;
      fail_if (pdf_fp_func_eval (func, in, out, &debug) != PDF_ETYPE4);
      fail_if (debug.type4.status != PDF_EINVRANGE);
      fail_if (debug.type4.op != 2);
      fail_if (debug.type4.stack [0] != -1);
    }
}
END_TEST


/*
 * **5 - General tests
 */

/*
 * Test: pdf_fp_func_eval_501
 * Description:
 *   Evaluate a type 4 function that returns a boolean value on
 *   the stack.
 * Success condition:
 *   Return PDF_ETYPE4, set PDF_EBADTYPE in the debug information.
 */
START_TEST(pdf_fp_func_eval_501)
{
  pdf_fp_func_t func;
  pdf_fp_func_debug_t debug;
  
  pdf_real_t domain[0] = {};
  pdf_real_t range[2] = {-10, 10};
  pdf_real_t in[0];
  pdf_real_t out[1];
  
  /* Create the function */
  fail_if(pdf_fp_func_4_tmake (0, 1,
                               domain,
                               range,
                               &func,
                               "{ true }") != PDF_OK);
  
  /*
   * Eval for some values
   */
  
  fail_if(pdf_fp_func_eval (func, in, out, &debug) != PDF_ETYPE4);
  fail_if (debug.type4.status != PDF_EBADTYPE);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_502
 * Description:
 *   Evaluate a type 4 function that accepts and returns 100 
 *   arguments.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
START_TEST(pdf_fp_func_eval_502)
{
  pdf_fp_func_t func;
  pdf_fp_func_debug_t debug;
  
  pdf_real_t domain[200];
  pdf_real_t range[200];
  pdf_real_t in[100];
  pdf_real_t out[100];

  int i;
  for (i = 0; i <= 100; i++)
    {
      in[i] = i;
      domain[i*2] = range[i*2] = -1;
      domain[i*2+1] = range[i*2+1] = 200;
    }
  
  /* Create the function */
  fail_if(pdf_fp_func_4_tmake (100, 100,
                               domain,
                               range,
                               &func,
                               "{ exch }") != PDF_OK);
  
  /*
   * Eval for some values
   */
  
  fail_if(pdf_fp_func_eval (func, in, out, &debug) != PDF_OK);
  fail_if(memcmp(in, out, sizeof(pdf_real_t) * 98) != 0);
  fail_if(in[98] != out[99]);
  fail_if(in[99] != out[98]);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_503
 * Description:
 *   Evaluate a type 4 function and pass arguments out of the
 *   domain.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
START_TEST(pdf_fp_func_eval_503)
{
  pdf_fp_func_t func;
  pdf_fp_func_debug_t debug;
  
  pdf_real_t domain[2] = { 0, 10 };
  pdf_real_t range[2] = {-10, 100 };
  pdf_real_t in[1];
  pdf_real_t out[1];
  
  /* Create the function */
  fail_if(pdf_fp_func_4_tmake (1, 1,
                               domain,
                               range,
                               &func,
                               "{ true pop }") != PDF_OK);
  
  /*
   * Eval for some values
   */

  /* x = -3 */ 
  in[0] = -3; 
  fail_if(pdf_fp_func_eval (func, in, out, &debug) != PDF_OK);
  fail_if (out[0] != domain[0]);

  /* x = 20 */
  in[0] = 20; 
  fail_if(pdf_fp_func_eval (func, in, out, &debug) != PDF_OK);
  fail_if (out[0] != domain[1]);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_504
 * Description:
 *   Evaluate a type 4 function that must clip its return value
 *   to the range.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
START_TEST(pdf_fp_func_eval_504)
{
  pdf_fp_func_t func;
  pdf_fp_func_debug_t debug;
  
  pdf_real_t domain[2] = { -10, 100 };
  pdf_real_t range[2] = { 0, 10 };
  pdf_real_t in[1];
  pdf_real_t out[1];
  
  /* Create the function */
  fail_if(pdf_fp_func_4_tmake (1, 1,
                               domain,
                               range,
                               &func,
                               "{ true pop }") != PDF_OK);
  
  /*
   * Eval for some values
   */

  /* x = -3 */ 
  in[0] = -3; 
  fail_if(pdf_fp_func_eval (func, in, out, &debug) != PDF_OK);
  fail_if (out[0] != range[0]);

  /* x = 20 */
  in[0] = 20; 
  fail_if(pdf_fp_func_eval (func, in, out, &debug) != PDF_OK);
  fail_if (out[0] != range[1]);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_505
 * Description:
 *   Evaluate a type 4 function that causes a stack overflow.
 * Success condition:
 *   Return PDF_ETYPE4, set PDF_EINVRANGE in the debug information.
 */
START_TEST(pdf_fp_func_eval_505)
{
  pdf_fp_func_t func;
  pdf_fp_func_debug_t debug;
  
  pdf_real_t domain[4] = { -10, 100, -10, 100 };
  pdf_real_t range[2] = { 0, 10 };
  pdf_real_t in[2];
  pdf_real_t out[1];
  
  /* Create the function */
  fail_if(pdf_fp_func_4_tmake (2, 1,
                               domain,
                               range,
                               &func,
                               "{ 1 add }") != PDF_OK);
  
  /*
   * Eval for some values
   */

  /* x = -3, y = 3 */ 
  in[0] = -3; 
  in[1] = 3;
  fail_if(pdf_fp_func_eval (func, in, out, &debug) != PDF_ETYPE4);
  fail_if (debug.type4.status != PDF_EINVRANGE);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_506
 * Description:
 *   Evaluate a type 4 function that causes a stack underflow.
 * Success condition:
 *   Return PDF_ETYPE4, set PDF_EUNDERFLOW in the debug information.
 */
START_TEST(pdf_fp_func_eval_506)
{
  pdf_fp_func_t func;
  pdf_fp_func_debug_t debug;
  
  pdf_real_t domain[2] = { 0, 10 };
  pdf_real_t range[4] = { -10, 100, -10, 100 };
  pdf_real_t in[1];
  pdf_real_t out[2];
  
  /* Create the function */
  fail_if(pdf_fp_func_4_tmake (1, 2,
                               domain,
                               range,
                               &func,
                               "{ 1 add }") != PDF_OK);
  
  /*
   * Eval for some values
   */

  /* x = -3, y = 3 */ 
  in[0] = -3; 
  fail_if(pdf_fp_func_eval (func, in, out, &debug) != PDF_ETYPE4);
  fail_if (debug.type4.status != PDF_EUNDERFLOW);
}
END_TEST


/*
 * Test case creation function
 */
TCase *
test_pdf_fp_func_eval (void)
{
  TCase *tc = tcase_create("pdf_fp_func_eval");
  tcase_add_test(tc, pdf_fp_func_eval_001);
  tcase_add_test(tc, pdf_fp_func_eval_002);
  tcase_add_test(tc, pdf_fp_func_eval_003);
  tcase_add_test(tc, pdf_fp_func_eval_004);
  tcase_add_test(tc, pdf_fp_func_eval_005);
  tcase_add_test(tc, pdf_fp_func_eval_006);
  tcase_add_test(tc, pdf_fp_func_eval_007);
  tcase_add_test(tc, pdf_fp_func_eval_008);
  tcase_add_test(tc, pdf_fp_func_eval_009);
  tcase_add_test(tc, pdf_fp_func_eval_010);
  tcase_add_test(tc, pdf_fp_func_eval_011);
  tcase_add_test(tc, pdf_fp_func_eval_012);
  tcase_add_test(tc, pdf_fp_func_eval_013);
  tcase_add_test(tc, pdf_fp_func_eval_014);
  tcase_add_test(tc, pdf_fp_func_eval_015);
  tcase_add_test(tc, pdf_fp_func_eval_016);
  tcase_add_test(tc, pdf_fp_func_eval_017);
  tcase_add_test(tc, pdf_fp_func_eval_018);
  tcase_add_test(tc, pdf_fp_func_eval_019);
  tcase_add_test(tc, pdf_fp_func_eval_020);
  tcase_add_test(tc, pdf_fp_func_eval_021);
  tcase_add_test(tc, pdf_fp_func_eval_101);
  tcase_add_test(tc, pdf_fp_func_eval_102);
  tcase_add_test(tc, pdf_fp_func_eval_103);
  tcase_add_test(tc, pdf_fp_func_eval_104);
  tcase_add_test(tc, pdf_fp_func_eval_201);
  tcase_add_test(tc, pdf_fp_func_eval_202);
  tcase_add_test(tc, pdf_fp_func_eval_203);
  tcase_add_test(tc, pdf_fp_func_eval_204);
  tcase_add_test(tc, pdf_fp_func_eval_205);
  tcase_add_test(tc, pdf_fp_func_eval_206);
  tcase_add_test(tc, pdf_fp_func_eval_207);
  tcase_add_test(tc, pdf_fp_func_eval_208);
  tcase_add_test(tc, pdf_fp_func_eval_301);
  tcase_add_test(tc, pdf_fp_func_eval_302);
  tcase_add_test(tc, pdf_fp_func_eval_303);
  tcase_add_test(tc, pdf_fp_func_eval_304);
  tcase_add_test(tc, pdf_fp_func_eval_305);
  tcase_add_test(tc, pdf_fp_func_eval_306);
  tcase_add_test(tc, pdf_fp_func_eval_307);
  tcase_add_test(tc, pdf_fp_func_eval_308);
  tcase_add_test(tc, pdf_fp_func_eval_309);
  tcase_add_test(tc, pdf_fp_func_eval_310);
  tcase_add_test(tc, pdf_fp_func_eval_311);
  tcase_add_test(tc, pdf_fp_func_eval_312);
  tcase_add_test(tc, pdf_fp_func_eval_313);
  tcase_add_test(tc, pdf_fp_func_eval_314);
  tcase_add_test(tc, pdf_fp_func_eval_401);
  tcase_add_test(tc, pdf_fp_func_eval_402);
  tcase_add_test(tc, pdf_fp_func_eval_403);
  tcase_add_test(tc, pdf_fp_func_eval_501);
  tcase_add_test(tc, pdf_fp_func_eval_502);
  tcase_add_test(tc, pdf_fp_func_eval_503);
  tcase_add_test(tc, pdf_fp_func_eval_504);
  tcase_add_test(tc, pdf_fp_func_eval_505);
  tcase_add_test(tc, pdf_fp_func_eval_506);
  return tc;
}

/* End of pdf-fp-func-eval.c */

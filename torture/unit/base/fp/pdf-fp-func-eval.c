/* -*- mode: C -*- Time-stamp: "2009-02-08 17:59:18 davazp"
 *
 *       File:         pdf-fp-func-eval.c
 *       Date:         Tue Dec  2 20:11:38 2008
 *
 *       GNU PDF Library - Unit tests for pdf_fp_func_eval
 *
 */

#include <config.h>
#include <string.h>
#include <math.h>
#include <check.h>
#include <pdf.h>

#define ABS_ERROR (0.00001)
#define DEG2RAD(N) ((N) * (180.0/M_PI))


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
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - simple_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - simple_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - simple_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
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
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_simple_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_simple_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_simple_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
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
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - double_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - double_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - double_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
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
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_double_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_double_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_double_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
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
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - cosine_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - cosine_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - cosine_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
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
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - double_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - double_f (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - double_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
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
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_double (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_double (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_double (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
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
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - line (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - line (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - line (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
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
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - line_x (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - line_x (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - line_x (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
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
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - line_y (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - line_y (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - line_y (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
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
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - round_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - round_f (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - round_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - round_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.93, y = 0.82 */
  in[0] = 0.93;
  in[1] = 0.82;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - round_f (in[0], in[1])) > ABS_ERROR);
}
END_TEST

/*
 * Test: pdf_fp_func_eval_012
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
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_f (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.93, y = 0.82 */
  in[0] = 0.93;
  in[1] = 0.82;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
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
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_a (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_a (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_a (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_a (in[0], in[1])) > ABS_ERROR);

  /* x = 0.93, y = 0.82 */
  in[0] = 0.93;
  in[1] = 0.82;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
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
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_ellipse_a (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_ellipse_a (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_ellipse_a (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_ellipse_a (in[0], in[1])) > ABS_ERROR);

  /* x = 0.93, y = 0.82 */
  in[0] = 0.93;
  in[1] = 0.82;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
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
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_b (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_b (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_b (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_b (in[0], in[1])) > ABS_ERROR);

  /* x = 0.93, y = 0.82 */
  in[0] = 0.93;
  in[1] = 0.82;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
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
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_c (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_c (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_c (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - ellipse_c (in[0], in[1])) > ABS_ERROR);

  /* x = 0.93, y = 0.82 */
  in[0] = 0.93;
  in[1] = 0.82;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
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
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_ellipse_c (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_ellipse_c (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_ellipse_c (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - inverted_ellipse_c (in[0], in[1])) > ABS_ERROR);

  /* x = 0.93, y = 0.82 */
  in[0] = 0.93;
  in[1] = 0.82;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
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
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - square_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - square_f (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - square_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - square_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.93, y = 0.82 */
  in[0] = 0.93;
  in[1] = 0.82;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
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
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - cross_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - cross_f (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - cross_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - cross_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.93, y = 0.82 */
  in[0] = 0.93;
  in[1] = 0.82;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
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
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - rhomboid_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - rhomboid_f (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - rhomboid_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - rhomboid_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.93, y = 0.82 */
  in[0] = 0.93;
  in[1] = 0.82;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
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
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - diamond_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0, y = 1 */
  in[0] = 0;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - diamond_f (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 0 */
  in[0] = 1;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - diamond_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.322, y = 0.233 */
  in[0] = 0.322;
  in[1] = 0.233;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - diamond_f (in[0], in[1])) > ABS_ERROR);

  /* x = 0.93, y = 0.82 */
  in[0] = 0.93;
  in[1] = 0.82;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(pdf_fp_abs(out[0] - diamond_f (in[0], in[1])) > ABS_ERROR);
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

  return tc;
}

/* End of pdf-fp-func-eval.c */

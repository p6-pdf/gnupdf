/* -*- mode: C -*- Time-stamp: "08/12/02 21:28:01 jemarch"
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


/*
 * Test: pdf_fp_func_eval_001
 * Description:
 *   Evaluate a type 4 function implementing the predefined function
 *   DoubleDot and compare the results with a C implementation.
 * Success condition:
 *   The result of the evaluations should be correct.
 */
#define ABS_ERROR 0.00001

static pdf_real_t 
double_dot (pdf_real_t x, pdf_real_t y)
{
  return (((sin (360 * x)) / 2) + ((sin (360 * y)) / 2));
}

START_TEST(pdf_fp_func_eval_001)
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
                             &func) != PDF_OK);

  /*
   * Eval for some values
   */

  /* x = 0, y = 0 */
  in[0] = 0;
  in[1] = 0;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(fabs(out[0] - double_dot (in[0], in[1])) > ABS_ERROR);

  /* x = 1, y = 1 */
  in[0] = 1;
  in[1] = 1;
  fail_if(pdf_fp_func_eval (func, in, out) != PDF_OK);
  fail_if(fabs(out[0] - double_dot (in[0], in[1])) > ABS_ERROR);
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

  return tc;
}

/* End of pdf-fp-func-eval.c */

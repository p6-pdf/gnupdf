/* -*- mode: C -*-
 *
 *       File:         pdf-fp-func-eval.c
 *       Date:         Sat Feb  7 18:55:51 2009
 *
 *       GNU PDF Library - Unit tests for pdf_fp_func_4_new
 *
 */

/* Copyright (C) 2009 Free Software Foundation, Inc. */

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
#include <check.h>
#include <pdf.h>

/*
 * Test: pdf_fp_func_4_new_001
 * Description:
 *   Create a syntactically wrong function.
 * Success condition:
 *   Return PDF_ENOWRAP and the offset of the error is correct.
 */

START_TEST(pdf_fp_func_4_new_001)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t prog[] =
    "{"
    " 0 eq"
    " {"
    "   1 1 eq } dup mul exch dup mul add 1 exch sub } if"
    " } if "
    "}";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];
  pdf_size_t error_at;
  pdf_status_t ret;
  
  pdf_init();

  prog_size = sizeof(prog);

  /* Create the function */
  ret = pdf_fp_func_4_new (2, 1,
                           domain,
                           range,
                           prog,
                           prog_size,
                           &error_at,
                           &func);
  
  fail_if (ret != PDF_ENOWRAP);
  fail_if (error_at != 58);

  pdf_fp_func_destroy (func);
}
END_TEST



/*
 * Test: pdf_fp_func_4_new_002
 * Description:
 *   Create a syntactically wrong function.
 * Success condition:
 *   Return PDF_ENOWRAP and the offset of the error is correct.
 */

START_TEST(pdf_fp_func_4_new_002)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t prog[] =
    "{ dup } 2";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];
  pdf_size_t error_at;
  pdf_status_t ret;
  
  pdf_init();

  prog_size = sizeof(prog);

  /* Create the function */
  ret = pdf_fp_func_4_new (2, 1,
                           domain,
                           range,
                           prog,
                           prog_size,
                           &error_at,
                           &func);
  
  fail_if (ret != PDF_ENOWRAP);
  fail_if (error_at != 8);

  pdf_fp_func_destroy (func);
}
END_TEST


/*
 * Test: pdf_fp_func_4_new_003
 * Description:
 *   Create a syntactically wrong function.
 * Success condition:
 *   Return PDF_ENOWRAP and the offset of the error is correct.
 */

START_TEST(pdf_fp_func_4_new_003)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t prog[] =
    "2 { dup }";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];
  pdf_size_t error_at;
  pdf_status_t ret;
  
  pdf_init();

  prog_size = sizeof(prog);

  /* Create the function */
  ret = pdf_fp_func_4_new (2, 1,
                           domain,
                           range,
                           prog,
                           prog_size,
                           &error_at,
                           &func);
  
  fail_if (ret != PDF_ENOWRAP);
  fail_if (error_at != 0);

  pdf_fp_func_destroy (func);
}
END_TEST


/*
 * Test: pdf_fp_func_4_new_004
 * Description:
 *   Create a syntactically wrong function.
 * Success condition:
 *   Return PDF_EMISSIF and the offset of the error is correct.
 */

START_TEST(pdf_fp_func_4_new_004)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t prog[] =
    "{ { dup } }";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];
  pdf_size_t error_at;
  pdf_status_t ret;
  
  pdf_init();
  
  prog_size = sizeof(prog);

  /* Create the function */
  ret = pdf_fp_func_4_new (2, 1,
                           domain,
                           range,
                           prog,
                           prog_size,
                           &error_at,
                           &func);
  
  fail_if (ret != PDF_EMISSIF);
  fail_if (error_at != 8);

  pdf_fp_func_destroy (func);
}
END_TEST


/*
 * Test: pdf_fp_func_4_new_005
 * Description:
 *   Create a syntactically wrong function.
 * Success condition:
 *   Return PDF_EMISSBODY and the offset of the error is correct.
 */

START_TEST(pdf_fp_func_4_new_005)
{
  pdf_fp_func_t func;
  pdf_size_t prog_size;
  pdf_char_t prog[] =
    "{ 2 if }";

  pdf_real_t domain[4] = {-1.0, 1.0, -1.0, 1.0};
  pdf_real_t range[2] = {-1.0, 1.0};
  pdf_real_t in[2];
  pdf_real_t out[1];
  pdf_size_t error_at;
  pdf_status_t ret;
  
  pdf_init();

  prog_size = sizeof(prog);

  /* Create the function */
  ret = pdf_fp_func_4_new (2, 1,
                           domain,
                           range,
                           prog,
                           prog_size,
                           &error_at,
                           &func);
  
  fail_if (ret != PDF_EMISSBODY);
  fail_if (error_at != 4);

  pdf_fp_func_destroy (func);
}
END_TEST




/*
 * Test case creation function
 */
TCase *
test_pdf_fp_func_4_new (void)
{
  TCase *tc = tcase_create("pdf_fp_func_4_new");
  tcase_add_test(tc, pdf_fp_func_4_new_001);
  tcase_add_test(tc, pdf_fp_func_4_new_002);
  tcase_add_test(tc, pdf_fp_func_4_new_003);
  tcase_add_test(tc, pdf_fp_func_4_new_004);
  tcase_add_test(tc, pdf_fp_func_4_new_005);
  return tc;
}



/* End of pdf-fp-func-4-new.c */

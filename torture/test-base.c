/* -*- mode: C -*- Time-stamp: "07/10/31 19:27:30 jemarch"
 *
 *       File:         test-base.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Sat Sep  8 22:32:13 2007
 *
 *       GNU PDF Library - Test suite for the base module
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

#include <config.h>

#ifdef HAVE_MALLOC_H
 #include <malloc.h>
#else
 #include <stdlib.h>
#endif /* HAVE_MALLOC_H */

#include <xalloc.h>
#include <string.h>
#include <stdio.h>
#include <pdf_base.h>

#include <check.h>



START_TEST(str2int)
{
  char *str;
  int res_int;

  /* string to integer */
  str = (char *) xmalloc (4);
  strcpy (str, "100");
  fail_if(pdf_str2int (str, &res_int) == PDF_ERROR);
  fail_unless(res_int == 100);
  free (str);
}
END_TEST


START_TEST(str2int_badpost)
{
  char *str;
  int res_int;

  str = (char *) xmalloc (4);
  strcpy (str, "20a");
  fail_if(pdf_str2int (str, &res_int) == PDF_OK);
  free (str);
}
END_TEST


START_TEST(str2int_badpre)
{
  char *str;
  int res_int;

  str = (char *) xmalloc (4);
  strcpy (str, "a20");
  fail_if(pdf_str2int (str, &res_int) == PDF_OK);
  free (str);
}
END_TEST


START_TEST(str2int_empty)
{
  char *str;
  int res_int;

  str = (char *) xmalloc (4);
  strcpy (str, "   ");
  fail_if (pdf_str2int (str, &res_int) == PDF_OK);
  free (str);
}
END_TEST


START_TEST(str2long) 
{
  char *str;
  long res_long;

  str = (char *) xmalloc (4);
  strcpy (str, "100");
  fail_if (pdf_str2long (str, &res_long) == PDF_ERROR);
  fail_unless (res_long == 100);
  free (str);
}
END_TEST


START_TEST(str2long_badpost)
{
  char *str;
  long res_long;

  str = (char *) xmalloc (4);
  strcpy (str, "20a");
  fail_if (pdf_str2long (str, &res_long) == PDF_OK);
  free (str);
}
END_TEST


START_TEST(str2long_badpre)
{
  char *str;
  long res_long;

  str = (char *) xmalloc (4);
  strcpy (str, "a20");
  fail_if (pdf_str2long (str, &res_long) == PDF_OK);
  free (str);
}
END_TEST


START_TEST(str2long_empty)
{
  char *str;
  long res_long;

  str = (char *) xmalloc (4);
  strcpy (str, "   ");
  fail_if (pdf_str2long (str, &res_long) == PDF_OK);
  free (str);
}
END_TEST


START_TEST(str2double)
{
  char *str;
  double res_double;

  str = (char *) xmalloc (4);
  strcpy (str, "1.2");
  fail_if (pdf_str2double (str, &res_double) == PDF_ERROR);
  free (str);
}
END_TEST


START_TEST(str2double_badpost)
{
  char *str;
  double res_double;

  str = (char *) xmalloc (4);
  strcpy (str, "2..");
  fail_if (pdf_str2double (str, &res_double) == PDF_OK);
  free (str);
}
END_TEST


START_TEST(str2double_badpre)
{
  char *str;
  double res_double;

  str = (char *) xmalloc (4);
  strcpy (str, "..2");
  fail_if (pdf_str2double (str, &res_double) == PDF_OK);
  free (str);
}
END_TEST


START_TEST(str2double_empty)
{
  char *str;
  double res_double;

  str = (char *) xmalloc (4);
  strcpy (str, "   ");
  fail_if (pdf_str2double (str, &res_double) == PDF_OK);
  free (str);
}
END_TEST


TCase* test_base_tests(void)
{
  TCase *tc = tcase_create("test-base");
  tcase_add_test(tc, str2int);
  tcase_add_test(tc, str2int_badpost);
  tcase_add_test(tc, str2int_badpre);
  tcase_add_test(tc, str2int_empty);
  tcase_add_test(tc, str2long);
  tcase_add_test(tc, str2long_badpost);
  tcase_add_test(tc, str2long_badpre);
  tcase_add_test(tc, str2long_empty);
  tcase_add_test(tc, str2double);
  tcase_add_test(tc, str2double_badpost);
  tcase_add_test(tc, str2double_badpre);
  tcase_add_test(tc, str2double_empty);
  return tc;
}

/* End of test-base.c */

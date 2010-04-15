/* -*- mode: C -*-
 *
 *       File:         check.h
 *       Date:         Fri Apr 18 20:29:04 2008
 *
 *       GNU PDF Library - Simple way to run unit tests without Check...
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


#ifndef _CHECK_H
#define _CHECK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <config.h>
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif /* HAVE_MALLOC_H */


/* Type for a test function */
typedef void (*TFun) (int);


/* Definition of a test case */
typedef struct _nocheck_TCase {
  char  *name;
  TFun  *elements;
  long  n;
  long  size;
} TCase;
/* Create test case */
TCase *tcase_create (const char *name);
/* Add test function to test case */
void tcase_add_test (TCase *tc, TFun tf);


/* Definition of suite */
typedef struct _nocheck_Suite {
  char  *name;
  TCase **elements;
  long   n;
  long   size;
} Suite;
/* Create suite */
Suite *suite_create (const char *name);
/* Add test case to suite */
void suite_add_tcase (Suite *s, TCase *tc);


/* Definition of 'Suite Runner' */
typedef struct _nocheck_SRunner {
  Suite  **elements;
  long    n;
  long    size;
} SRunner;
/* Create suite runner */
SRunner *srunner_create (Suite *s);
/* Add suite to suite runner */
void srunner_add_suite (SRunner *sr, Suite *s);
/* Free suite runner and its contents */
void srunner_free (SRunner *sr);

enum print_output {
  CK_SILENT,
  CK_MINIMAL,
  CK_NORMAL,
  CK_VERBOSE,
  CK_ENV,
  CK_LAST,
  CK_FORK,
  CK_NOFORK
};

/* Run all tests */
void srunner_run_all (SRunner *sr, enum print_output print_mode);


/* Other needed functions */
int srunner_ntests_failed (SRunner *sr);
void tcase_fn_start (const char *fname, const char *file, int line);
void srunner_set_log (SRunner *sr, const char *fname);
void srunner_set_fork_status (SRunner *sr, enum print_output print_mode);



/* Test start and end points definition */
#define START_TEST(__testname)\
static void __testname (int __attribute__((unused)) _i)\
{\
   const char *test_name = ""# __testname; \
   tcase_fn_start (""# __testname, __FILE__, __LINE__);

#define END_TEST \
  _fail_unless(1, test_name, __FILE__, __LINE__,"Passed"); \
}



/* Support for fail_if and fail_unless */
void _fail_unless (int result,
                   const char *test,
                   const char *file,
                   int line,
                   const char *expr,
                   ...);

#define fail_unless(expr, ...) do { \
  if(!(expr)) { \
    _fail_unless(expr, test_name, __FILE__, __LINE__,"Assertion '"#expr"' failed" , ## __VA_ARGS__, NULL); \
    return; \
  } \
} while(0)

#define fail_if(expr, ...) do { \
  if((expr)) { \
    _fail_unless(!(expr), test_name, __FILE__, __LINE__,"Failure '"#expr"' occured" , ## __VA_ARGS__, NULL); \
    return; \
  } \
} while(0)

#endif /* !_CHECK_H */

/* End of check.h */

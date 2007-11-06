/* -*- mode: C -*- Time-stamp: "07/11/06 22:51:30 jemarch"
 *
 *       File:         testall.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Tue Nov  6 22:50:53 2007
 *
 *       GNU PDF Library - Test driver for the libgnupdf suite
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

/*  Run all the unit tests.

    Each unit test file contains a TCase* test_XXX_tests() function.
    Add that as an extern here.  Also you must add the test case to
    the suite inside the main() function.


    XXX This file could be generated by a script. 
*/

#include <check.h>

extern TCase* test_base_tests(void);
extern TCase* test_date_tests(void);
extern TCase* test_libinit_tests(void);
extern TCase* test_obj_array_tests(void);
extern TCase* test_obj_createdestroy_tests(void);
extern TCase* test_obj_dict_tests(void);
extern TCase* test_obj_dupequality_tests(void);
extern TCase* test_stm_openclose_tests(void);
extern TCase* test_rectangle_tests(void);


int 
main (int argc, char** argv)
{
  int failures;
  SRunner *sr;
  Suite *s;

  s = suite_create("libgnupdf");
  suite_add_tcase (s, test_base_tests());
  suite_add_tcase (s, test_date_tests());
  suite_add_tcase (s, test_libinit_tests());
  suite_add_tcase (s, test_obj_array_tests());
  suite_add_tcase (s, test_obj_createdestroy_tests());
  suite_add_tcase (s, test_obj_dict_tests());
  suite_add_tcase (s, test_obj_dupequality_tests());
  suite_add_tcase (s, test_stm_openclose_tests());
  suite_add_tcase (s, test_rectangle_tests());

  sr = srunner_create (s);
  srunner_run_all (sr, CK_NORMAL);
  failures = srunner_ntests_failed (sr);
  srunner_free (sr);

  return (failures == 0) ? 0 : 1;
}

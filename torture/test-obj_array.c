/* -*- mode: C -*- Time-stamp: "07/10/31 19:28:08 jemarch"
 *
 *       File:         test-obj_array.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Sat Jul  7 21:40:36 2007
 *
 *       GNU PDF Library - Test suite for the PDF array object
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

#include <string.h>

#ifdef HAVE_MALLOC_H
 #include <malloc.h>
#else
 #include <stdlib.h>
#endif /* HAVE_MALLOC_H */

#include <xalloc.h>
#include <stdio.h>
#include <pdf_obj.h>
#include <check.h>









START_TEST(create_empty)
{
  pdf_obj_t obj;

  obj = pdf_create_array ();
  fail_unless(pdf_get_array_size (obj) == 0);
}
END_TEST



START_TEST(manip)
{
  pdf_obj_t obj;
  pdf_obj_t obj2;
  pdf_obj_t elt1;
  pdf_obj_t elt2;
  pdf_obj_t elt3;
  char *string;

  obj = pdf_create_array ();

  elt1 = pdf_create_string ("one", 3);
  elt2 = pdf_create_array ();

  pdf_add_array_elt (obj,
                     pdf_get_array_size (obj),
                     elt1);
  pdf_add_array_elt (obj,
                     pdf_get_array_size (obj),
                     elt2);
  pdf_add_array_elt (obj,
                     -1,
                     elt2);

  fail_if (pdf_get_array_size (obj) != 2);

  string = (char *) xmalloc (pdf_get_string_size (pdf_get_array_elt (obj, 0)));
  memcpy (string,
          pdf_get_string_data (pdf_get_array_elt (obj, 0)),
          pdf_get_string_size (pdf_get_array_elt (obj, 0)));

  /* Check string value of element. */
  fail_unless(memcmp (string, "one", 3) == 0);

  obj2 = pdf_obj_dup (obj);

  /* Should compare equal. */
  fail_if(!pdf_obj_equal_p (obj, obj2));

  elt3 = pdf_create_array ();
  pdf_append_array_elt (obj2, elt3);

  /* These should be different. */
  fail_if (pdf_obj_equal_p (obj, obj2));

  /* Remove an element. */
  fail_if (!pdf_remove_array_elt (obj, 1));

  /* Should be one element left. */
  fail_if (pdf_get_array_size (obj) != 1);
}
END_TEST


TCase* test_obj_array_tests(void)
{
  TCase *tc = tcase_create("test-obj_array");
  tcase_add_test(tc, create_empty);
  tcase_add_test(tc, manip);
  return tc;
}



/* End of test-obj_array.c */

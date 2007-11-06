/* -*- mode: C -*- Time-stamp: "07/11/06 22:46:32 jemarch"
 *
 *       File:         test-rectangle.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Sun Sep  9 03:13:02 2007
 *
 *       GNU PDF Library - Test suite for the rectangle common data type
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
#include <pdf_obj.h>
#include <pdf_rectangle.h>
#include <check.h>


static const int RECT_VALUE_ERR = 1;


START_TEST(array_create)
{
  pdf_rectangle_t rect;
  pdf_obj_t array;
  pdf_obj_t elt1;
  pdf_obj_t elt2;
  pdf_obj_t elt3;
  pdf_obj_t elt4;

  /* Note the mixing of integer and real objects in the tests: the
     rectangle common data type should support numbers of any type */

  elt1 = pdf_create_real (-1.0);
  elt2 = pdf_create_integer (-1);
  elt3 = pdf_create_real (1.0);
  elt4 = pdf_create_integer (1);

  array = pdf_create_array();
  pdf_append_array_elt (array, elt4);
  pdf_append_array_elt (array, elt3);
  pdf_append_array_elt (array, elt2);
  pdf_append_array_elt (array, elt1);

  rect = pdf_create_rectangle (array);
  fail_if (rect == NULL);
  pdf_destroy_rectangle (rect);
  pdf_destroy_obj (array);
}
END_TEST



START_TEST(pts)
{

  pdf_rectangle_t rect;
  pdf_obj_t array;
  pdf_obj_t elt1;
  pdf_obj_t elt2;
  pdf_obj_t elt3;
  pdf_obj_t elt4;
  pdf_point_t ll;
  pdf_point_t lr;
  pdf_point_t ur;
  pdf_point_t ul;

  elt1 = pdf_create_real (0.0);
  elt2 = pdf_create_integer (0);
  elt3 = pdf_create_real (-1.0);
  elt4 = pdf_create_integer (1);

  array = pdf_create_array();
  pdf_append_array_elt (array, elt4);
  pdf_append_array_elt (array, elt3);
  pdf_append_array_elt (array, elt2);
  pdf_append_array_elt (array, elt1);

  rect = pdf_create_rectangle (array);
  fail_if (rect == NULL);
  
  ll = pdf_rectangle_ll (rect);
  lr = pdf_rectangle_lr (rect);
  ur = pdf_rectangle_ur (rect);
  ul = pdf_rectangle_ur (rect);
 
  if ((P_X(ll) != -1.0) || (P_Y(ll) != 0.0) ||
      (P_X(lr) != 0.0) || (P_Y(lr) != 0.0) ||
      (P_X(ur) != 0.0) || (P_Y(ur) != -1.0) ||
      (P_X(ul) != -1.0) || (P_Y(ul) != -1.0))
    {
      printf("Error in values of rectangle: (%f,%f) - (%f,%f)\n",
             P_X(ll), P_Y(ll), P_X(ur), P_Y(ur));
      fail_if(RECT_VALUE_ERR); /* Bail! */
    }
  
  pdf_destroy_point (ll);
  pdf_destroy_point (lr);
  pdf_destroy_point (ur);
  pdf_destroy_point (ul);
  pdf_destroy_rectangle (rect);
  pdf_destroy_obj (array);
}
END_TEST
  
  
TCase * 
test_rectangle_tests(void)
{
  TCase *tc = tcase_create("test-rectangle");
  tcase_add_test(tc, array_create);
  /*  tcase_add_test(tc, pts); */

  return tc;
}

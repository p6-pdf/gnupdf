/* -*- mode: C -*- Time-stamp: "08/01/20 20:03:18 jemarch"
 *
 *       File:         pdf_rectangle.c
 *       Date:         Sun Sep  9 01:33:26 2007
 *
 *       GNU PDF Library - Rectangle common data type
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
#include <pdf_rectangle.h>

static pdf_rectangle_t pdf_rectangle_alloc (void);
static void pdf_rectangle_dealloc (pdf_rectangle_t rect);

pdf_rectangle_t
pdf_create_rectangle (pdf_obj_t array)
{
  pdf_rectangle_t new_rect;
  pdf_obj_t elt1;
  pdf_obj_t elt2;
  pdf_obj_t elt3;
  pdf_obj_t elt4;
  pdf_point_t p1;
  pdf_point_t p2;
  pdf_point_t ll;
  pdf_point_t ur;

  if ((!IS_ARRAY(array)) ||
      (pdf_get_array_size (array) != 4))
    {
      return NULL;
    }

  elt1 = pdf_get_array_elt (array, 0);
  elt2 = pdf_get_array_elt (array, 1);
  elt3 = pdf_get_array_elt (array, 2);
  elt4 = pdf_get_array_elt (array, 3);

  /* Elements can be both reals or integers */
  if ((!IS_NUMBER(elt1)) || (!IS_NUMBER(elt2)) ||
      (!IS_NUMBER(elt3)) || (!IS_NUMBER(elt4)))
    {
      return NULL;
    }

  /* 
   * Create the new rectangle
   */
  new_rect = pdf_rectangle_alloc ();
  p1 = pdf_create_point ();
  p2 = pdf_create_point ();
  
  P_X(p1) = IS_INT(elt1) ? pdf_get_int (elt1) : pdf_get_real (elt1);
  P_Y(p1) = IS_INT(elt2) ? pdf_get_int (elt2) : pdf_get_real (elt2);
  P_X(p2) = IS_INT(elt3) ? pdf_get_int (elt3) : pdf_get_real (elt3);
  P_Y(p2) = IS_INT(elt4) ? pdf_get_int (elt4) : pdf_get_real (elt4);

  /* Normalize the points to (ll_x, ll_y) - (ur_x, ur_y) */
  ll = pdf_create_point ();
  ur = pdf_create_point ();

  if (P_X(p1) >= P_X(p2))
    {
      if (P_Y(p1) >= P_Y(p2))
        {
          P_X(ll) = P_X(p1);
          P_Y(ll) = P_Y(p1);
          P_X(ur) = P_X(p2);
          P_Y(ur) = P_Y(p2);
        }
      else
        {
          P_X(ll) = P_X(p1);
          P_Y(ll) = P_Y(p2);
          P_X(ur) = P_X(p2);
          P_Y(ur) = P_Y(p1);
        }
    }
  else
    {
      if (P_Y(p1) >= P_Y(p2))
        {
          P_X(ll) = P_X(p2);
          P_Y(ll) = P_Y(p1);
          P_X(ur) = P_X(p1);
          P_Y(ur) = P_Y(p2);
        }
      else
        {
          P_X(ll) = P_X(p2);
          P_Y(ll) = P_Y(p2);
          P_X(ur) = P_X(p1);
          P_Y(ur) = P_Y(p1);
        }
    }

  new_rect->ll = ll;
  new_rect->ur = ur;

  pdf_destroy_point (p1);
  pdf_destroy_point (p2);

  return new_rect;
}

inline void
pdf_destroy_rectangle (pdf_rectangle_t rect)
{
  pdf_destroy_point (rect->ll);
  pdf_destroy_point (rect->ur);
  pdf_rectangle_dealloc (rect);
}

inline pdf_point_t
pdf_rectangle_ll (pdf_rectangle_t rect)
{
  pdf_point_t ll;
  
  ll = pdf_create_point ();
  P_X(ll) = P_X(rect->ll);
  P_Y(ll) = P_Y(rect->ll);
  
  return ll;
}

inline pdf_point_t
pdf_rectangle_ur (pdf_rectangle_t rect)
{
  pdf_point_t ur;

  ur = pdf_create_point ();
  P_X(ur) = P_X(rect->ur);
  P_Y(ur) = P_Y(rect->ur);

  return ur;
}

inline void
pdf_rectangle_set_ll (pdf_rectangle_t rect,
                      pdf_point_t point)
{
  pdf_destroy_point (rect->ll);
  rect->ll = pdf_point_dup (point);
}

inline void
pdf_rectangle_set_ur (pdf_rectangle_t rect,
                      pdf_point_t point)
{
  pdf_destroy_point (rect->ur);
  rect->ur = pdf_point_dup (point);
}

inline pdf_point_t
pdf_rectangle_lr (pdf_rectangle_t rect)
{
  pdf_point_t lr;

  lr = pdf_create_point ();
  P_X(lr) = P_X(rect->ur);
  P_Y(lr) = P_Y(rect->ll);

  return lr;
}

inline pdf_point_t
pdf_rectangle_ul (pdf_rectangle_t rect)
{
  pdf_point_t ul;

  ul = pdf_create_point ();
  P_X(ul) = P_X(rect->ll);
  P_Y(ul) = P_Y(rect->ur);

  return ul;
}

/* Private functions */

static pdf_rectangle_t
pdf_rectangle_alloc (void)
{
  pdf_rectangle_t new_rect;

  new_rect = (pdf_rectangle_t) xmalloc (sizeof(struct pdf_rectangle_s));
  return new_rect;
}

static void
pdf_rectangle_dealloc (pdf_rectangle_t rect)
{
  free (rect);
}


/* End of pdf_rectangle.h */

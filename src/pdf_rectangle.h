/* -*- mode: C -*- Time-stamp: "07/09/09 03:04:21 jemarch"
 *
 *       File:         pdf_rectangle.h
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Sun Sep  9 01:30:37 2007
 *
 *       GNU PDF Library - Rectangle common data type
 *
 */

/* Copyright (C) 2007 Jose E. Marchesi */

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

/* Rectangles are used to describe locations and bounding boxes.

   A rectangle is specified using an array composed by four numbers:
   the coordinates of of two diagonally opposited corners of the
   rectangle.

   For example:

               [ll_x ll_y ur_x ur_y]

   Note that it is permitted to specify any two diagonally opposite
   corners. That implies we should normalize it.

   The GNU PDF library normalizes rectangles into the lower left and
   upper right corners. */



#ifndef PDF_RECTANGLE_H
#define PDF_RECTANGLE_H

#include <config.h>
#include <pdf_base.h>
#include <pdf_obj.h>

/* Rectangle internal structure */

struct pdf_rectangle_s
{
  pdf_point_t ll;  /* Lower left corner (ll_x, ll_y) */
  pdf_point_t ur;  /* Upper right corner (ur_x, ur_y) */
};

typedef struct pdf_rectangle_s *pdf_rectangle_t;

pdf_rectangle_t pdf_create_rectangle (pdf_obj_t array);
inline void pdf_destroy_rectangle (pdf_rectangle_t rect);

inline pdf_point_t pdf_rectangle_ll (pdf_rectangle_t rect);
inline pdf_point_t pdf_rectangle_lr (pdf_rectangle_t rect);
inline pdf_point_t pdf_rectangle_ur (pdf_rectangle_t rect);
inline pdf_point_t pdf_rectangle_ul (pdf_rectangle_t rect);
inline void pdf_rectangle_set_ur (pdf_rectangle_t rect, pdf_point_t point);
inline void pdf_rectangle_set_ll (pdf_rectangle_t rect, pdf_point_t point);

#endif /* pdf_rectangle.h */

/* End of pdf_rectangle.h */

/* -*- mode: C -*- Time-stamp: "08/11/27 21:00:00 jemarch"
 *
 *       File:         pdf-fp.c
 *       Date:         Sun Feb 10 21:38:34 2008
 *
 *       GNU PDF Library - Floating-point Arithmetic Module
 *
 */

/* Copyright (C) 2007, 2008 Free Software Foundation, Inc. */

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

#include <pdf-alloc.h>
#include <pdf-fp.h>

pdf_point_t
pdf_create_point (void)
{
  pdf_point_t new_point;

  new_point = (pdf_real_t *) pdf_alloc (sizeof(pdf_real_t) * 2);
  P_X(new_point) = 0.0;
  P_Y(new_point) = 0.0;
  return new_point;
}

void
pdf_destroy_point (pdf_point_t point)
{
  pdf_dealloc (point);
}

pdf_point_t
pdf_point_dup (pdf_point_t point)
{
  pdf_point_t new_point;

  new_point = pdf_create_point ();
  P_X(new_point) = P_X(point);
  P_Y(new_point) = P_Y(point);

  return new_point;
}


/* End of pdf-fp.c */

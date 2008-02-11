/* -*- mode: C -*- Time-stamp: "08/02/11 00:59:49 jemarch"
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

#include <pdf-fp.h>

pdf_point_t
pdf_create_point (void)
{
  pdf_point_t new_point;

  new_point = (pdf_real_t *) xmalloc (sizeof(pdf_real_t) * 2);
  P_X(new_point) = 0.0;
  P_Y(new_point) = 0.0;
  return new_point;
}

void
pdf_destroy_point (pdf_point_t point)
{
  free (point);
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

inline double
pdf_interp_lineal (double x1,
                   double y1,
                   double x2,
                   double y2,
                   double x)
{
  return (y2 + ((y2 - y1) / (x2 - x1)) * (x - x1));
}

inline double
pdf_interp_exp_coef_m (double x1, 
                       double y1,
                       double x2,
                       double y2)
{
  return ((log (y2 / y1)) / (x2 - x1));
}

inline double
pdf_interp_exp_coef_k (double x1,
                       double y1,
                       double m)
{
  return (y1 * exp (-1 * m * x1));
}

inline double
pdf_interp_exp (double m,
                double k,
                double x)
{
  return (k * exp (m * x));
}

/* End of pdf-fp.c */

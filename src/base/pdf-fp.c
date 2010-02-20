/* -*- mode: C -*-
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

#include <config.h>

#include <pdf-fp.h>

pdf_real_t
pdf_fp_sqrt (pdf_real_t op)
{
  return sqrtf (op);
}

pdf_real_t
pdf_fp_sin (pdf_real_t op)
{
  return sinf (op);
}

pdf_real_t
pdf_fp_cos (pdf_real_t op)
{
  return cosf (op);
}

pdf_real_t
pdf_fp_atan2 (pdf_real_t op1,
              pdf_real_t op2)
{
  return atan2f (op1, op2);
}

pdf_real_t
pdf_fp_exp (pdf_real_t op)
{
  return expf (op);
}

pdf_real_t
pdf_fp_abs (pdf_real_t op)
{
  return fabs (op);
}

pdf_real_t
pdf_fp_log10 (pdf_real_t op)
{
  return log10f (op);
}

pdf_real_t
pdf_fp_log (pdf_real_t op)
{
  return logf (op);
}

pdf_real_t
pdf_fp_floor (pdf_real_t op)
{
  return floorf (op);
}

pdf_real_t
pdf_fp_ceil (pdf_real_t op)
{
  return ceilf (op);
}


/* End of pdf-fp.c */

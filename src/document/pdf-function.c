/* -*- mode: C -*- Time-stamp: "08/02/11 01:05:43 jemarch"
 *
 *       File:         pdf-function.c
 *       Date:         Sun Sep  9 21:04:31 2007
 *
 *       GNU PDF Library - Function common data type
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

#ifdef HAVE_MALLOC_H
 #include <malloc.h>
#else
 #include <stdlib.h>
#endif /* HAVE_MALLOC_H */

#include <xalloc.h>
#include <pdf_function.h>

pdf_func_t 
pdf_create_func_0 (pdf_obj_t stream)
{
  if (!IS_STREAM(stream))
    {
      return NULL;
    }

  return NULL;
}

pdf_func_t
pdf_create_func_2 (pdf_obj_t dict)
{
  if (!IS_DICT(dict))
    {
      return NULL;
    }

  return NULL;
}

pdf_func_t
pdf_create_func_3 (pdf_obj_t dict)
{
  if (!IS_DICT(dict))
    {
      return NULL;
    }

  return NULL;
}

pdf_func_t
pdf_create_func_4 (pdf_obj_t stream)
{
  if (!IS_STREAM(stream))
    {
      return NULL;
    }

  return NULL;
}

/* Private functions */


/* End of pdf_function.c */

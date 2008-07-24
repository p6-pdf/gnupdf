/* -*- mode: C -*- Time-stamp: "2008-07-24 12:01:26 gerel"
 *
 *       File:         pdf-alloc.c
 *       Date:         Fri Feb 22 21:05:05 2008
 *
 *       GNU PDF Library - Memory De/Allocation Module
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

#include <pdf-alloc.h>

inline void *
pdf_alloc (const pdf_size_t size)
{
  void *pointer;

  pointer = malloc (size);
  if (!pointer && size != 0)
    {
      pointer = NULL;
    }
  return pointer;
}

inline void
pdf_dealloc (const void *pointer)
{
  free ((void*)pointer);
}

inline void *
pdf_realloc (void *pointer, const pdf_size_t size)
{
  pointer = realloc (pointer, size);
  if (!pointer && size != 0)
    {
      pointer = NULL;
    }
  return pointer;
}

/* End of pdf-alloc.c */

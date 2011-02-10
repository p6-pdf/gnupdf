/* -*- mode: C -*-
 *
 *       File:         pdf-list-test-common.c
 *       Date:         Thu Feb 10 21:17:46 2011
 *
 *       GNU PDF Library - Common utilities to test the List Module
 *
 */

/* Copyright (C) 2011 Free Software Foundation, Inc. */

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
#include <stdio.h>
#include <pdf.h>

#include "pdf-list-test-common.h"

pdf_bool_t
l_comp (const void * elemb,
        const void * elema)
{
  int *elem1, *elem2;
  elem1 = (int *) elema;
  elem2 = (int *) elemb;

  if (*elem1 == *elem2)
    return 1;
  return 0;
}

void
l_disp (const void *elema)
{
}

int
l_comp_desc (const void *elema,
             const void *elemb)
{
  int *elem1, *elem2;
  elem1 = (int *) elema;
  elem2 = (int *) elemb;

  if (*elem1 > *elem2)
    return -1;
  if (*elem1 < *elem2)
    return 1;
  return 0;
}

int
l_comp_asc (const void *elema,
            const void *elemb)
{
  int *elem1, *elem2;
  elem1 = (int *) elema;
  elem2 = (int *) elemb;

  if (*elem1 > *elem2)
    return 1;
  if (*elem1 < *elem2)
    return -1;
  return 0;
}

/* End of pdf-list-test-common.c */

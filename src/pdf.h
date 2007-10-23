/* -*- mode: C -*- Time-stamp: "07/10/23 12:52:55 jemarch"
 *
 *       File:         pdf.h
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Thu Jul  5 19:52:48 2007
 *
 *       GNU PDF Library - Main header file
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

#ifndef _PDF_H
#define _PDF_H

#include <config.h>

#include <pdf_base.h>
#include <pdf_obj.h>

/* This structure contains global data used by the library. A variable
   of this type, called `pdf_globals', is defined in pdf.c. The
   contents of this structure are initialized in the `pdf_init'
   function. */



struct pdf_globals_s
{
  pdf_obj_t names[PDF_NAMES]; /* Array of PDF names used in the
                                 standard dictionaries. Look in
                                 pdf_base.h for a list of those
                                 names */
};


/* API of the library */

int pdf_init (void);
int pdf_finish (void);

#endif /* pdf.h */

/* End of pdf.h */

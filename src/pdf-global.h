/* -*- mode: C -*- Time-stamp: "08/10/12 18:31:43 jemarch"
 *
 *       File:         pdf-global.h
 *       Date:         Thu Jul  5 19:52:48 2007
 *
 *       GNU PDF Library - Global code
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
#include <pdf-types.h>

/* BEGIN PUBLIC */

/* Macro to access the global state of the library pdf_globals */
#define PDF_GLOBALS extern struct pdf_globals_s pdf_globals;

/* This structure contains global data used by the library. A variable
   of this type, called `pdf_globals', is defined in pdf.c. The
   contents of this structure are initialized in the `pdf_init'
   function. */

struct pdf_globals_s
{

};


/* API of the library */

int pdf_init (void);
int pdf_finish (void);

/* END PUBLIC */

#endif /* pdf.h */

/* End of pdf.h */

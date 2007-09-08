/* -*- mode: C -*- Time-stamp: "07/09/08 20:13:58 jemarch"
 *
 *       File:         pdf_base.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Sat Sep  8 15:11:30 2007
 *
 *       GNU PDF Library - Common resources for the library modules
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

#include <config.h>

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <xalloc.h>

#include <pdf_base.h>

int
pdf_str2int (char *string,
             int *num)
{
  long num_long;

  if ((pdf_str2long (string, &num_long) == PDF_OK) &&
      (num_long <= INT_MAX))
    {
      *num = (int) num_long;
      return PDF_OK;
    }
  else
    {
      return PDF_ERROR;
    }

  /* Not reached */
}

int 
pdf_str2long (char *string,
              long *num)
{
  char *tail;
  *num = strtol (string,
                 &tail,
                 10);

  if ((tail == string) || (tail != (string + strlen(string))))
    {
      return PDF_ERROR;
    }
  else
    {
      return PDF_OK;
    }
}

int 
pdf_str2double (char *string,
                double *num)
{
  char *tail;
  *num = strtod (string, &tail);

  if ((tail == string) || (tail != (string + strlen(string))))
    {
      return PDF_ERROR;
    }
  else
    {
      return PDF_OK;
    }
}

/* End of pdf_base.c */

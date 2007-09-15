/* -*- mode: C -*- Time-stamp: "07/09/14 23:20:56 jemarch"
 *
 *       File:         test-libinit.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Fri Sep 14 23:08:35 2007
 *
 *       GNU PDF Library - Library initialization and finalization
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
#include <string.h>
#include <pdf.h>

PDF_GLOBALS

int
main ()
{
  int success;
  pdf_obj_t name;

  success = PDF_TRUE;

  pdf_init ();

  /* Test preallocated names */
  name = pdf_create_name ((unsigned char*) "PageLayout", 10);
  if (!pdf_obj_equal_p (name, PDF_N_PAGE_LAYOUT))
    {
      success = PDF_FALSE;
      printf("Invalid value in a preallocated name\n");
    }

  pdf_finish ();

  if (success)
    {
      return 0;
    }
  else
    {
      return 1;
    }
}

/* End of test-libinit.c */

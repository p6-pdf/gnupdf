/* -*- mode: C -*- Time-stamp: "07/07/06 22:28:38 jemarch"
 *
 *       File:         test-io_openclose.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Fri Jul  6 21:15:56 2007
 *
 *       GNU PDF Library - Test suite for IO open/close features
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


#include <stdio.h>
#include <pdf_io.h>

int 
main ()
{
  pdf_io_t io;

  io = pdf_io_open_file ("non-existing-file\n", PDF_IO_FILE_READ);
  if (io != NULL)
    {
      printf ("Error trying to open a non-existing file.\n");
      return 1;
    }

  io = pdf_io_open_file ("torture-file-1", PDF_IO_FILE_READ);
  if (io == NULL)
    {
      printf ("Error trying to open an existing file.\n");
      return 1;
    }

  if (!pdf_io_close (io))
    {
      printf ("Error trying to close a file.\n");
      return 1;
    }
  
  return 0;
}

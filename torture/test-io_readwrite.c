/* -*- mode: C -*- Time-stamp: "07/07/06 22:29:02 jemarch"
 *
 *       File:         test-io_readwrite.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Fri Jul  6 21:51:12 2007
 *
 *       GNU PDF Library - Test suite for IO read/write features
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
#include <string.h>
#include <pdf_io.h>

int 
main ()
{
  pdf_io_t io;
  char buf[100];

  /* Reading features */
  io = pdf_io_open_file ("torture-file-1", PDF_IO_FILE_READ);
  if (io == NULL)
    {
      printf ("Error opening an existing file.\n");
      return 1;
    }

  if ((pdf_io_read (io, buf, 2) != 2) ||
      (strncmp (buf, "GN", 2) != 0))
    {
      printf ("Error reading from file.\n");
      return 1;
    }
  
  if (!pdf_io_close (io))
    {
      printf ("Error trying to close a file.\n");
      return 1;
    }

  return 0;
}

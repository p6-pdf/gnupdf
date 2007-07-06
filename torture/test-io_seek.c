/* -*- mode: C -*- Time-stamp: "07/07/06 21:52:35 jemarch"
 *
 *       File:         test-io_seek.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Fri Jul  6 21:29:22 2007
 *
 *       GNU PDF Library - Test suite for IO seek features
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

  io = pdf_io_open_file ("torture-file-1");
  if (io == NULL)
    {
      printf ("Error opening an existing file.\n");
      return 1;
    }

  if (pdf_io_tell (io) != 0L)
    {
      printf ("Error telling just after opening the file.\n");
      return 1;
    }

  if (!pdf_io_seek_beg (io, 2L) ||
      (pdf_io_tell (io) != 2L))
    {
      printf ("Error seeking relative to the beginning of the file.\n");
      return 1;
    }

  if (!pdf_io_seek_cur (io, 2L) ||
      (pdf_io_tell (io) != 4L))
    {
      printf ("Error seeking relative to the current position.\n");
      return 1;
    }

  if (!pdf_io_seek_end (io, -2L) ||
      (pdf_io_tell (io) != 12))
    {
      printf ("%d\n", pdf_io_tell (io));
      printf ("Error seeking relative to the end of the file.\n");
      return 1;
    }

  if (!pdf_io_close (io))
    {
      printf ("Error trying to close a file.\n");
      return 1;
    }

  return 0;
}

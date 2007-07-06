/* -*- mode: C -*- Time-stamp: "07/07/06 22:11:48 jemarch"
 *
 *       File:         pdf_io.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Fri Jul  6 18:43:15 2007
 *
 *       GNU PDF Library - Input/Output routines
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


#include <unistd.h>
#include <string.h>
#include <xalloc.h>
#include <pdf_io.h>

pdf_io_t 
pdf_io_open_file (char *filename)
{
  pdf_io_t new_io;

  new_io = (pdf_io_t) xmalloc (sizeof(struct pdf_io_s));
  new_io->type = PDF_IO_FILE;
  new_io->data.file.writable_p = PDF_FALSE;
  new_io->data.file.filename = strdup (filename);
  new_io->data.file.stream = 
    fopen (filename, "rb");

  if (new_io->data.file.stream == NULL)
    {
      /* Open the file in read/write mode, if possible */
      new_io->data.file.stream = 
        fopen (filename, "r+b");
      
      if (new_io->data.file.stream == NULL)
        {
          return NULL;
        }
    }

  return new_io;
}

int
pdf_io_close (pdf_io_t io)
{
  if (fclose (io->data.file.stream))
    {
      return PDF_ERROR;
    }

  free (io);
  return PDF_OK;
}

static int
pdf_io_seek (pdf_io_t io,
             pdf_io_pos_t pos,
             int seek_mode)
{
  if (!fseek (io->data.file.stream,
              pos,
              seek_mode))
    {
      return PDF_ERROR;
    }
  
  return PDF_OK;
}

inline int
pdf_io_seek_beg (pdf_io_t io,
                 pdf_io_pos_t pos)
{
  if (pdf_io_seek (io, pos, SEEK_SET) == -1)
    {
      return PDF_ERROR;
    }
  
  return PDF_OK;
}

inline int
pdf_io_seek_cur (pdf_io_t io,
                 pdf_io_pos_t pos)
{
  if (pdf_io_seek (io, pos, SEEK_CUR) == -1)
    {
      return PDF_ERROR;
    }

  return PDF_OK;
}

inline int
pdf_io_seek_end (pdf_io_t io,
                 pdf_io_pos_t pos)
{
  if (pdf_io_seek (io, pos, SEEK_END) == -1)
    {
      return PDF_ERROR;
    }

  return PDF_OK;
}

inline pdf_io_pos_t
pdf_io_tell (pdf_io_t io)
{
  return ftell (io->data.file.stream);
}

inline size_t
pdf_io_read (pdf_io_t io,
             char *buf,
             size_t bytes)
{
  return fread (buf, 1, bytes, io->data.file.stream);
}

inline size_t
pdf_io_write (pdf_io_t io,
              char *buf,
              size_t bytes)
{
  if (!io->data.file.writable_p)
    {
      /* Reopen the file in read/write mode */
      io->data.file.stream =
        freopen (io->data.file.filename,
                 "r+b",
                 io->data.file.stream);
    }
  
  if (io->data.file.stream == NULL)
    {
      return -1;
    }

  return fwrite (buf, 1, bytes, io->data.file.stream);
}

/* End of pdf_io.h */

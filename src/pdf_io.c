/* -*- mode: C -*- Time-stamp: "07/07/07 03:56:00 jemarch"
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
#include <sys/stat.h>
#include <xalloc.h>
#include <pdf_io.h>

#ifdef HAVE_FSEEK
   #define fseek fseeko
#endif /* HAVE_FSEEK */

pdf_io_t 
pdf_io_open_file (char *filename, 
                  enum pdf_io_file_mode mode)
{
  pdf_io_t new_io;

  new_io = (pdf_io_t) xmalloc (sizeof(struct pdf_io_s));
  new_io->type = PDF_IO_FILE;
  new_io->data.file.mode = mode;
  new_io->data.file.filename = strdup (filename);
  new_io->data.file.stream = NULL;

  switch (mode)
    {
    case PDF_IO_FILE_READ:
      {
        new_io->data.file.stream = 
          fopen (filename, "rb");
        break;
      }
    case PDF_IO_FILE_WRITE:
      {
        new_io->data.file.stream =
          fopen (filename, "wb");
        break;
      }
    case PDF_IO_FILE_APPEND:
      {
        new_io->data.file.stream =
          fopen (filename, "ab");
        break;
      }
    }

  if (new_io->data.file.stream == NULL)
    {
      free (new_io);
      return NULL;
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

INLINE int
pdf_io_seek_beg (pdf_io_t io,
                 pdf_io_pos_t pos)
{
  if (pdf_io_seek (io, pos, SEEK_SET) == -1)
    {
      return PDF_ERROR;
    }
  
  return PDF_OK;
}

INLINE int
pdf_io_seek_cur (pdf_io_t io,
                 pdf_io_pos_t pos)
{
  if (pdf_io_seek (io, pos, SEEK_CUR) == -1)
    {
      return PDF_ERROR;
    }

  return PDF_OK;
}

INLINE int
pdf_io_seek_end (pdf_io_t io,
                 pdf_io_pos_t pos)
{
  if (pdf_io_seek (io, pos, SEEK_END) == -1)
    {
      return PDF_ERROR;
    }

  return PDF_OK;
}

INLINE pdf_io_pos_t
pdf_io_tell (pdf_io_t io)
{
  return ftell (io->data.file.stream);
}

INLINE size_t
pdf_io_read (pdf_io_t io,
             char *buf,
             size_t bytes)
{
  if (io->data.file.mode != PDF_IO_FILE_READ)
    {
      return -1;
    }

  return fread (buf, 1, bytes, io->data.file.stream);
}

INLINE size_t
pdf_io_write (pdf_io_t io,
              char *buf,
              size_t bytes)
{
  if (io->data.file.mode == PDF_IO_FILE_READ)
    {
      return -1;
    }

  return fwrite (buf, 1, bytes, io->data.file.stream);
}

INLINE pdf_io_pos_t
pdf_io_size (pdf_io_t io)
{
  struct stat file_stats;

  if (fstat (fileno(io->data.file.stream), &file_stats) != 0)
    {
      return -1;
    }

  return file_stats.st_size;
}

/* End of pdf_io.h */

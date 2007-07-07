/* -*- mode: C -*- Time-stamp: "07/07/06 23:14:11 jemarch"
 *
 *       File:         pdf_io.h
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Fri Jul  6 18:37:57 2007
 *
 *       GNU PDF Library - Input/Output header file
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

/* This library module implement the abstract input/output routines
   used by the pdf lexical scanner */

#ifndef _PDF_IO_H
#define _PDF_IO_H

#include <sys/types.h>
#include <stdio.h>
#include <config.h>
#include <pdf_base.h>


/* A variable of type `pdf_io_pos_t' contain a byte-offset relative to
   the beginning of a IO object. 

   Please be careful manipulating `pdf_io_pos_t' values. Its value is
   assured to be a signed scalable one, but its size may be wider than
   a long. */

typedef off_t pdf_io_pos_t;

enum pdf_io_type_t
{
  /* A file stored in a filesystem */
  PDF_IO_FILE,

  /* A network socket */
  PDF_IO_SOCKET

};

enum pdf_io_file_mode
{
  PDF_IO_FILE_READ,
  PDF_IO_FILE_WRITE,
  PDF_IO_FILE_APPEND
};

struct pdf_io_file_s
{
  FILE *stream;
  char *filename;
  enum pdf_io_file_mode mode;
};

struct pdf_io_socket_s
{
  /* FIXME: TODO */
};

struct pdf_io_s
{
  enum pdf_io_type_t type;

  union 
  {

    struct pdf_io_file_s file;
    struct pdf_io_socket_s socket;

  } data;

};

typedef struct pdf_io_s *pdf_io_t;


/* Forward declarations */

pdf_io_t pdf_io_open_file (char *filename, enum pdf_io_file_mode mode);
int pdf_io_close (pdf_io_t io);
pdf_io_pos_t pdf_io_size (pdf_io_t io);
int pdf_io_seek_beg (pdf_io_t io, pdf_io_pos_t pos);
int pdf_io_seek_cur (pdf_io_t io, pdf_io_pos_t pos);
int pdf_io_seek_end (pdf_io_t io, pdf_io_pos_t pos);
pdf_io_pos_t pdf_io_tell (pdf_io_t io);
size_t pdf_io_read (pdf_io_t io, char *buf, size_t bytes);
size_t pdf_io_write (pdf_io_t io, char *buf, size_t bytes);

#endif /* pdf_io.h */

/* End of pdf_io.h */

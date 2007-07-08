/* -*- mode: C -*- Time-stamp: "07/07/08 18:09:50 jemarch"
 *
 *       File:         pdf_stm.h
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Fri Jul  6 18:37:57 2007
 *
 *       GNU PDF Library - streams
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

#ifndef _PDF_STM_H
#define _PDF_STM_H

#include <sys/types.h>
#include <stdio.h>
#include <config.h>
#include <pdf_base.h>


/* A variable of type `pdf_stm_pos_t' contain a byte-offset relative to
   the beginning of a STREAM object. 

   Please be careful manipulating `pdf_stm_pos_t' values. Its value is
   assured to be a signed scalable one, but its size may be wider than
   a long. */

typedef off_t pdf_stm_pos_t;

enum pdf_stm_type_t
{
  /* A file stored in a filesystem */
  PDF_STM_FILE,
  /* A buffer in memory */
  PDF_STM_BUFFER

};

enum pdf_stm_file_mode_t
{
  PDF_STM_FILE_READ,
  PDF_STM_FILE_WRITE,
  PDF_STM_FILE_APPEND
};

enum pdf_stm_seek_mode_t
  {
    PDF_STM_SEEK_BEG,
    PDF_STM_SEEK_CUR,
    PDF_STM_SEEK_END
  };

struct pdf_stm_file_s
{
  FILE *stm;
  char *filename;
  enum pdf_stm_file_mode_t mode;
};

typedef struct pdf_stm_file_s *pdf_stm_file_t;

struct pdf_stm_buffer_s
{
  char *data;
};

typedef struct pdf_stm_buffer_s *pdf_stm_buffer_s;

struct pdf_stm_s
{
  enum pdf_stm_type_t type;

  union
  {

    struct pdf_stm_file_s file;
    struct pdf_stm_buffer_s buffer;

  } data;

};

typedef struct pdf_stm_s *pdf_stm_t;


/* 
 * Forward declarations
 */

/* Creation and destruction */
pdf_stm_t pdf_create_file_stm (char *filename, enum pdf_stm_file_mode_t mode);
pdf_stm_t pdf_create_buffer_stm (pdf_stm_pos_t size);
int pdf_stm_close (pdf_stm_t stm);

/* Getting information about stream objects */
pdf_stm_pos_t pdf_stm_size (pdf_stm_t stm);

/* Positioning */
int pdf_seek (pdf_stm_t stm, pdf_stm_pos_t pos, enum pdf_stm_seek_mode mode);
pdf_stm_pos_t pdf_tell (pdf_stm_t stm);
size_t pdf_stm_read (pdf_stm_t stm, char *buf, size_t bytes);
size_t pdf_stm_write (pdf_stm_t stm, char *buf, size_t bytes);

#endif /* pdf_stm.h */

/* End of pdf_stm.h */

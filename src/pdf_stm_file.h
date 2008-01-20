/* -*- mode: C -*- Time-stamp: "08/01/20 20:03:54 jemarch"
 *
 *       File:         pdf_stm_file.h
 *       Date:         Sun Jul  8 19:56:45 2007
 *
 *       GNU PDF Library - File stream backend
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



#ifndef PDF_STM_FILE_H
#define PDF_STM_FILE_H

#include <config.h>
#include <unistd.h>
#include <stdio.h>
#include <pdf_base.h>


enum pdf_stm_file_open_mode_t
{
  PDF_STM_FILE_OPEN_MODE_READ,
  PDF_STM_FILE_OPEN_MODE_WRITE,
  PDF_STM_FILE_OPEN_MODE_RW
};

/* Configuration structure */

struct pdf_stm_file_conf_s
{
  char *filename;
  enum pdf_stm_file_open_mode_t mode;
};

typedef struct pdf_stm_file_conf_s *pdf_stm_file_conf_t;

/* Private data */

#define PDF_STM_FILE_BUFSIZ BUFSIZ

struct pdf_stm_file_data_s
{
  char *filename;
  enum pdf_stm_file_open_mode_t mode;
  int filedes;
  pdf_stm_pos_t in_buffer_size;
  pdf_stm_pos_t in_buffer_pointer;
  pdf_char_t in_buffer[PDF_STM_FILE_BUFSIZ];
  pdf_stm_pos_t out_buffer_size;
  pdf_stm_pos_t out_buffer_pointer;
  pdf_char_t out_buffer[PDF_STM_FILE_BUFSIZ];
};

typedef struct pdf_stm_file_data_s *pdf_stm_file_data_t;

/* Backend API implementation */

int pdf_stm_file_init (void **be_data, void *conf_data);

int pdf_stm_file_write_p (void *be_data);
int pdf_stm_file_read_p (void *be_data);
int pdf_stm_file_seek_p (void *be_data);
int pdf_stm_file_size_p (void *be_data);
int pdf_stm_file_peek_p (void *be_data);
int pdf_stm_file_tell_p (void *be_data);

int pdf_stm_file_close (void **be_data);

pdf_stm_pos_t pdf_stm_file_size (void *be_data);
int pdf_stm_file_seek (void *be_data, pdf_stm_pos_t pos);
pdf_stm_pos_t pdf_stm_file_tell (void *be_data);

size_t pdf_stm_file_read (void *be_data, pdf_char_t **buf, size_t bytes);
size_t pdf_stm_file_write (void *be_data, pdf_char_t *buf, size_t bytes);
size_t pdf_stm_file_flush (void *be_data);

int pdf_stm_file_read_char (void *be_data);
int pdf_stm_file_peek_char (void *be_data);

#endif /* pdf_stm_file.h */

/* End of pdf_stm_file.h */

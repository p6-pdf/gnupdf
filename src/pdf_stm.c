/* -*- mode: C -*- Time-stamp: "07/07/08 21:14:14 jemarch"
 *
 *       File:         pdf_stm.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Fri Jul  6 18:43:15 2007
 *
 *       GNU PDF Library - Streams
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
#include <xalloc.h>
#include <pdf_stm_file.h>
/* #include <pdf_stm_buffer.h> */
#include <pdf_stm.h>


/*
 * Backend-specific initialization functions
 */

pdf_stm_t
pdf_create_file_stm (char *filename,
                     int mode)
{
  pdf_stm_t new_stm;
  struct pdf_stm_file_conf_s conf;

  new_stm = (pdf_stm_t) xmalloc (sizeof (struct pdf_stm_s));
  
  /* Install backend functions */
  new_stm->backend.funcs.init = pdf_stm_file_init;
  new_stm->backend.funcs.write_p = pdf_stm_file_write_p;
  new_stm->backend.funcs.read_p = pdf_stm_file_read_p;
  new_stm->backend.funcs.seek_p = pdf_stm_file_seek_p;
  new_stm->backend.funcs.size_p = pdf_stm_file_size_p;
  new_stm->backend.funcs.peek_p = pdf_stm_file_peek_p;
  new_stm->backend.funcs.size = pdf_stm_file_size;
  new_stm->backend.funcs.seek = pdf_stm_file_seek;
  new_stm->backend.funcs.tell = pdf_stm_file_tell;
  new_stm->backend.funcs.read = pdf_stm_file_read;
  new_stm->backend.funcs.write = pdf_stm_file_write;
  new_stm->backend.funcs.peek = pdf_stm_file_peek;

  /* Configure the backend */
  conf.filename = filename;
  
  switch (mode)
    {
    case PDF_STM_OPEN_MODE_READ:
      {
        conf.mode = PDF_STM_FILE_OPEN_MODE_READ;
        break;
      }
    case PDF_STM_OPEN_MODE_WRITE:
      {
        conf.mode = PDF_STM_FILE_OPEN_MODE_WRITE;
        break;
      }
    case PDF_STM_OPEN_MODE_APPEND:
      {
        conf.mode = PDF_STM_FILE_OPEN_MODE_APPEND;
        break;
      }
    default:
      {
        /* Make the compiler happy */
        break;
      }
    }

  /* Initialize the backend */
  (new_stm->backend.funcs.init) (&new_stm->backend.data, &conf);
                                
  return new_stm;
}

/**********************************************/
/* pdf_stm_t                                  */
/* pdf_create_buffer_stm (pdf_stm_pos_t size, */
/*                        char init_p,        */
/*                        char init)          */
/* {                                          */
/*   struct pdf_stm_file_conf_s conf;         */
/*                                            */
/*                                            */
/* }                                          */
/**********************************************/

/* 
 * Generic functions
 */

int
pdf_stm_close (pdf_stm_t stm)
{
  int status;

  status = stm->backend.funcs.close (BE_DATA(stm));
  free (stm);

  return status;
}

pdf_stm_pos_t
pdf_stm_size (pdf_stm_t stm)
{
  pdf_stm_pos_t size;

  if (stm->backend.funcs.size_p (BE_DATA(stm)))
    {
      size = stm->backend.funcs.size (BE_DATA(stm));
    }
  else
    {
      size = 0;
    }
  
  return size;
}

int
pdf_stm_seek (pdf_stm_t stm,
              pdf_stm_pos_t pos)
{
  int status;

  if (stm->backend.funcs.seek_p (BE_DATA(stm)))
    {
      status = stm->backend.funcs.seek (BE_DATA(stm), pos);
    }
  else
    {
      status = PDF_ERROR;
    }

  return status;
}

pdf_stm_pos_t
pdf_stm_tell (pdf_stm_t stm)
{
  pdf_stm_pos_t result;

  if (stm->backend.funcs.tell_p (BE_DATA(stm)))
    {
      result = stm->backend.funcs.tell (BE_DATA(stm));
    }
  else
    {
      result = 0;
    }

  return result;
}

size_t
pdf_stm_read (pdf_stm_t stm,
              char *buf,
              size_t bytes)
{
  size_t result;

  if (stm->backend.funcs.read_p (BE_DATA(stm)))
    {
      result = stm->backend.funcs.read (BE_DATA(stm),
                                        buf,
                                        bytes);
    }
  else
    {
      result = 0;
    }

  return result;
}

size_t
pdf_stm_write (pdf_stm_t stm,
               char *buf,
               size_t bytes)
{
  size_t result;

  if (stm->backend.funcs.write_p (BE_DATA(stm)))
    {
      result = stm->backend.funcs.write (BE_DATA(stm),
                                         buf,
                                         bytes);
    }
  else
    {
      result = 0;
    }

  return result;
}

size_t
pdf_stm_peek (pdf_stm_t stm,
              char *buf,
              size_t bytes)
{
  size_t result;

  if (stm->backend.funcs.peek_p (BE_DATA(stm)))
    {
      result = stm->backend.funcs.peek (BE_DATA(stm),
                                        buf,
                                        bytes);
    }
  else
    {
      result = 0;
    }

  return result;
}

/* End of pdf_stm.c */

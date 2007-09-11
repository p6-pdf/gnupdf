/* -*- mode: C -*- Time-stamp: "07/09/11 20:34:59 jemarch"
 *
 *       File:         pdf_stm.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Fri Jul  6 18:43:15 2007
 *
 *       GNU PDF Library - File stream backend
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
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <malloc.h>
#include <xalloc.h>
#include <pdf_base.h>
#include <pdf_stm_file.h>

#ifdef HAVE_FSEEK
#define fseek fseeko
#endif /* HAVE_FSEEK */

int
pdf_stm_file_init (void **be_data, 
                   void *conf_data)
{
  pdf_stm_file_data_t *data;
  pdf_stm_file_conf_t conf;

  data = (pdf_stm_file_data_t *) be_data;
  conf = (pdf_stm_file_conf_t) conf_data;

  /* Create the private data storage */
  *data = (pdf_stm_file_data_t) xmalloc (sizeof(struct pdf_stm_file_data_s));
  
  /* Fill it with configuration values */
  (*data)->filename = strdup (conf->filename);
  (*data)->mode = conf->mode;
  (*data)->peek_size = 0;
  (*data)->file_stm = NULL;

  switch ((*data)->mode)
    {
    case PDF_STM_FILE_OPEN_MODE_READ:
      {
        (*data)->file_stm = fopen ((*data)->filename, "rb");
        break;
      }
    case PDF_STM_FILE_OPEN_MODE_WRITE:
      {
        (*data)->file_stm = fopen ((*data)->filename, "wb");
        break;
      }
    case PDF_STM_FILE_OPEN_MODE_APPEND:
      {
        (*data)->file_stm = fopen ((*data)->filename, "ab");
        break;
      }
    default:
      {
        /* Make stupid compilers happy */
        break;
      }
    }

  if ((*data)->file_stm == NULL)
    {
      return PDF_ERROR;
    }
  else
    {
      return PDF_OK;
    }
}


int
pdf_stm_file_write_p (void *be_data)
{
  pdf_stm_file_data_t data;
  data = (pdf_stm_file_data_t) be_data;

  return ((data->mode == PDF_STM_FILE_OPEN_MODE_WRITE) ||
          (data->mode == PDF_STM_FILE_OPEN_MODE_APPEND));
}

int
pdf_stm_file_read_p (void *be_data)
{
  pdf_stm_file_data_t data;
  data = (pdf_stm_file_data_t) be_data;

  return (data->mode == PDF_STM_FILE_OPEN_MODE_READ);
}

inline int
pdf_stm_file_seek_p (void *be_data)
{
  return PDF_TRUE;
}

inline int
pdf_stm_file_size_p (void *be_data)
{
  return PDF_TRUE;
}

inline int
pdf_stm_file_peek_p (void *be_data)
{
  return PDF_TRUE;
}

inline int
pdf_stm_file_tell_p (void *be_data)
{
  pdf_stm_file_data_t data;

  data = (pdf_stm_file_data_t) be_data;
  return (data->mode != PDF_STM_FILE_OPEN_MODE_APPEND);
}

int
pdf_stm_file_close (void **be_data)
{
  pdf_stm_file_data_t *data;
  int status;

  data = (pdf_stm_file_data_t *) be_data;

  if (fclose ((*data)->file_stm))
    {
      status = PDF_ERROR;
    }
  else
    {
      status = PDF_OK;
    }

  free ((*data)->filename);
  free (*data);

  return status;
}

pdf_stm_pos_t
pdf_stm_file_size (void *be_data)
{
  pdf_stm_file_data_t data;
  struct stat file_stats;

  data = (pdf_stm_file_data_t) be_data;

  if (fstat (fileno (data->file_stm), &file_stats) != 0)
    {
      return NO_POS;
    }

  return file_stats.st_size;
}

int
pdf_stm_file_seek (void *be_data,
                   pdf_stm_pos_t pos)
{
  pdf_stm_file_data_t data;

  data = (pdf_stm_file_data_t) be_data;

  if (!fseek (data->file_stm,
              pos,
              SEEK_SET))
    {
      return PDF_ERROR;
    }

  return PDF_OK;
}

pdf_stm_pos_t
pdf_stm_file_tell (void *be_data)
{
  pdf_stm_file_data_t data;
  
  data = (pdf_stm_file_data_t) be_data;

  return ftell (data->file_stm);
}

size_t
pdf_stm_file_read (void *be_data,
                   pdf_char_t **buf,
                   size_t bytes)
{
  pdf_stm_file_data_t data;
  size_t read_bytes;
  size_t peek_bytes;
  size_t readed;

  data = (pdf_stm_file_data_t) be_data;
  read_bytes = bytes;
  peek_bytes = 0;
  readed = 0;

  *buf = (pdf_char_t *) xmalloc (bytes);

  if (data->peek_size > 0)
    {
      if (bytes > data->peek_size)
        {
          peek_bytes = data->peek_size;
          read_bytes = read_bytes - peek_bytes;
        }
      else
        {
          peek_bytes = bytes;
          read_bytes = 0;
        }

      memcpy (*buf, 
              data->peek_buffer,
              peek_bytes);

      data->peek_size = data->peek_size - peek_bytes;
      if (data->peek_size == 0)
        {
          free (data->peek_buffer);
        }
    }

  readed = readed + peek_bytes;

  if (read_bytes > 0)
    {
      readed = readed + 
        fread (*buf + peek_bytes,
               1, 
               read_bytes,
               data->file_stm);
    }

  return readed;
}

size_t
pdf_stm_file_write (void *be_data,
                    pdf_char_t *buf,
                    size_t bytes)
{
  pdf_stm_file_data_t data;

  data = (pdf_stm_file_data_t) be_data;

  return fwrite (buf, 1, bytes, data->file_stm);
}


size_t
pdf_stm_file_peek (void *be_data,
                   pdf_char_t **buf,
                   size_t bytes)
{
  pdf_stm_file_data_t data;
  size_t readed;

  data = (pdf_stm_file_data_t) be_data;

  *buf = (pdf_char_t *) xmalloc (bytes);

  readed = fread (*buf,
                  1,
                  bytes,
                  data->file_stm);

  if (data->peek_size > 0)
    {
      data->peek_buffer = (pdf_char_t *) xrealloc (data->peek_buffer,
                                             data->peek_size + readed);
      data->peek_size = readed;
    }
  else
    {
      data->peek_buffer = (pdf_char_t *) xmalloc (readed);
      memcpy (data->peek_buffer,
              *buf,
              readed);
      data->peek_size = readed;
    }

  return readed;
}

/* End of pdf_stm.h */

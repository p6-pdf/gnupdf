/* -*- mode: C -*- Time-stamp: "08/01/20 20:03:51 jemarch"
 *
 *       File:         pdf_stm.c
 *       Date:         Fri Jul  6 18:43:15 2007
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

#include <config.h>

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#ifdef HAVE_MALLOC_H
 #include <malloc.h>
#else
 #include <stdlib.h>
#endif /* HAVE_MALLOC_H */

#include <xalloc.h>
#include <pdf_base.h>
#include <pdf_stm_file.h>

#ifdef HAVE_FSEEK
#define fseek fseeko
#endif /* HAVE_FSEEK */

static int pdf_stm_file_readpeek_char (void *be_data, int peek_p);

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
  (*data)->in_buffer_size = 0;
  (*data)->in_buffer_pointer = 0;
  (*data)->out_buffer_size = PDF_STM_FILE_BUFSIZ;
  (*data)->out_buffer_pointer = 0;

  switch ((*data)->mode)
    {
    case PDF_STM_FILE_OPEN_MODE_READ:
      {
        (*data)->filedes = open ((*data)->filename, O_RDONLY);

        break;
      }
    case PDF_STM_FILE_OPEN_MODE_WRITE:
      {
        (*data)->filedes = open ((*data)->filename, 
                                 O_WRONLY | O_CREAT | O_TRUNC,
                                 S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        break;
      }
    case PDF_STM_FILE_OPEN_MODE_RW:
      {
        (*data)->filedes = open ((*data)->filename, 
                                 O_RDWR | O_CREAT,
                                 S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        break;
      }
    default:
      {
        /* Make stupid compilers happy */
        break;
      }
    }

  if ((*data)->filedes == -1)
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
          (data->mode == PDF_STM_FILE_OPEN_MODE_RW));
}

int
pdf_stm_file_read_p (void *be_data)
{
  pdf_stm_file_data_t data;
  data = (pdf_stm_file_data_t) be_data;

  return ((data->mode == PDF_STM_FILE_OPEN_MODE_READ) ||
          (data->mode == PDF_STM_FILE_OPEN_MODE_RW));
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
  return PDF_TRUE;
}

int
pdf_stm_file_close (void **be_data)
{
  pdf_stm_file_data_t *data;
  int status;
  size_t res;

  data = (pdf_stm_file_data_t *) be_data;

  /* Flush the output cache */
  if ((*data)->out_buffer_pointer > 0)
    {
      res = write ((*data)->filedes,
                   (*data)->out_buffer,
                   (*data)->out_buffer_pointer);
      if (res != (*data)->out_buffer_pointer)
        {
          /* Unable to write all the cached data */
          status = PDF_ERROR;
        }
    }

  if (close ((*data)->filedes) != 0)
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

  /* Flush any pending output */
  pdf_stm_file_flush (be_data);

  if (fstat (data->filedes, &file_stats) != 0)
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

  /* Flush any pending output */
  pdf_stm_file_flush (be_data);

  if (lseek (data->filedes,
             pos,
             SEEK_SET) == -1)
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

  return lseek (data->filedes,
                0,
                SEEK_CUR);
}

size_t
pdf_stm_file_read (void *be_data,
                   pdf_char_t **buf,
                   size_t bytes)
{
  pdf_stm_file_data_t data;
  size_t readed;
  size_t to_read;
  size_t available;
  int eof;

  data = (pdf_stm_file_data_t) be_data;

  *buf = (pdf_char_t *) xmalloc (bytes);

  eof = PDF_FALSE;
  readed = 0;
  while ((readed < bytes) && (!eof))
    {
      available = data->in_buffer_size - data->in_buffer_pointer;
      if (available == 0)
        {
          /* The buffer cache is empty: fill it with data from the file */
          data->in_buffer_size = read (data->filedes, 
                                       data->in_buffer,
                                       PDF_STM_FILE_BUFSIZ);
          data->in_buffer_pointer = 0;
          available = data->in_buffer_size;

          if (available < PDF_STM_FILE_BUFSIZ)
            {
              eof = PDF_TRUE;
            }
        }

      /* Read data from the cache */
      to_read = ((bytes - readed) <= available) ? (bytes - readed) : available;
      memcpy (*buf + readed,
              data->in_buffer + data->in_buffer_pointer,
              to_read);
      readed = readed + to_read;
      data->in_buffer_pointer = data->in_buffer_pointer + to_read;
    }

  /* Adjust output buffer */
  *buf = (pdf_char_t *) xrealloc (*buf, readed);

  return readed;
}

size_t
pdf_stm_file_write (void *be_data,
                    pdf_char_t *buf,
                    size_t bytes)
{
  pdf_stm_file_data_t data;
  size_t written;
  size_t res;
  size_t to_write;
  size_t available;
  int disk_full;

  data = (pdf_stm_file_data_t) be_data;

  disk_full = PDF_FALSE;
  written = 0;
  while ((written < bytes) && (!disk_full))
    {
      available = data->out_buffer_size - data->out_buffer_pointer;
      if (available == 0)
        {
          /* The buffer cache is full: write its contents to disk */
          res = write (data->filedes,
                       data->out_buffer,
                       PDF_STM_FILE_BUFSIZ);

          data->out_buffer_pointer = 0;
          available = PDF_STM_FILE_BUFSIZ;
          
          if (res < PDF_STM_FILE_BUFSIZ)
            {
              disk_full = PDF_TRUE;
            }
        }
      if (!disk_full)
        {
          /* Write the data into the output cache */
          to_write = ((bytes - written) <= available) ? (bytes - written) : available;
          memcpy (data->out_buffer + data->out_buffer_pointer,
                  buf + written,
                  to_write);
          written = written + to_write;
          data->out_buffer_pointer = data->out_buffer_pointer + to_write;
        }
    }
  
  return written;
}

int
pdf_stm_file_read_char (void *be_data)
{
  return pdf_stm_file_readpeek_char (be_data, PDF_FALSE);
}

int
pdf_stm_file_peek_char (void *be_data)
{
  return pdf_stm_file_readpeek_char (be_data, PDF_TRUE);
}

size_t
pdf_stm_file_flush (void *be_data)
{
  size_t res;
  pdf_stm_file_data_t data;

  data = (pdf_stm_file_data_t) be_data;

  /* Flush the output cache */
  res = 0;
  if (data->out_buffer_pointer > 0)
    {
      res = write (data->filedes,
                   data->out_buffer,
                   data->out_buffer_pointer);

      data->out_buffer_pointer = 0;
    }

  return res;
}

/* Private functions */

int
pdf_stm_file_readpeek_char (void *be_data,
                            int peek_p)
{
  int c;
  pdf_stm_file_data_t data;

  data = (pdf_stm_file_data_t) be_data;

  if (data->in_buffer_size == data->in_buffer_pointer)
    {
      /* The buffer cache is empty: fill it with data from the file */
      data->in_buffer_size = read (data->filedes,
                                   data->in_buffer,
                                   PDF_STM_FILE_BUFSIZ);
      data->in_buffer_pointer = 0;
    }

  if ((data->in_buffer_size > 0) &&
      (data->in_buffer_pointer < data->in_buffer_size))
    {
      c = data->in_buffer[data->in_buffer_pointer];
      if (!peek_p)
        {
          data->in_buffer_pointer++;
        }
      return c;
    }
  else
    {
      return PDF_EOF;
    }
}


/* End of pdf_stm.h */

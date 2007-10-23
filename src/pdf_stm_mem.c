/* -*- mode: C -*- Time-stamp: "07/10/23 21:38:59 jemarch"
 *
 *       File:         pdf_stm_mem.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Mon Jul  9 01:13:41 2007
 *
 *       GNU PDF Library - Memory stream backend
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
#include <string.h>
#include <malloc.h>
#include <xalloc.h>
#include <pdf_base.h>
#include <pdf_stm_mem.h>

static int pdf_stm_mem_readpeek_char (void *be_data, int peek_p);

int
pdf_stm_mem_init (void **be_data,
                  void *conf_data)
{
  pdf_stm_mem_data_t *data;
  pdf_stm_mem_conf_t conf;

  data = (pdf_stm_mem_data_t *) be_data;
  conf = (pdf_stm_mem_conf_t) conf_data;

  /* Create the private data storage */
  *data = (pdf_stm_mem_data_t) xmalloc (sizeof(struct pdf_stm_mem_data_s));

  /* Initialize private data */
  (*data)->size = conf->size;
  (*data)->resize_p = conf->resize_p;
  (*data)->data = (pdf_char_t *) xmalloc (conf->size);


  if  (conf->init_p)
    {
      (*data)->data = memset  ((*data)->data, 
                               conf->init_char,
                               conf->size);
    }

  (*data)->current = 0;

  return PDF_OK;
}

inline int
pdf_stm_mem_write_p (void *be_data)
{
  return PDF_TRUE;
}

inline int
pdf_stm_mem_read_p (void *be_data)
{
  return PDF_TRUE;
}

inline int
pdf_stm_mem_seek_p (void *be_data)
{
  return PDF_TRUE;
}

inline int
pdf_stm_mem_size_p (void *be_data)
{
  return PDF_TRUE;
}

inline int
pdf_stm_mem_peek_p (void *be_data)
{
  return PDF_TRUE;
}

int
pdf_stm_mem_close (void **be_data)
{
  pdf_stm_mem_data_t *data;

  data = (pdf_stm_mem_data_t *) be_data;
  free ((*data)->data);
  free (*data);

  return PDF_OK;
}

pdf_stm_pos_t
pdf_stm_mem_size (void *be_data)
{
  pdf_stm_mem_data_t data;

  data = (pdf_stm_mem_data_t) be_data;
  return data->size;
}

int
pdf_stm_mem_seek (void *be_data,
                  pdf_stm_pos_t pos)
{
  pdf_stm_mem_data_t data;

  data = (pdf_stm_mem_data_t) be_data;

  if ((pos < 0) || (pos >= data->size))
    {
      return PDF_ERROR;
    }

  data->current = pos;
  return PDF_OK;
}

pdf_stm_pos_t
pdf_stm_mem_tell (void *be_data)
{
  pdf_stm_mem_data_t data;

  data = (pdf_stm_mem_data_t) be_data;
  return data->current;
}

size_t
pdf_stm_mem_read (void *be_data,
                  pdf_char_t **buf,
                  size_t bytes)
{
  pdf_stm_mem_data_t data;
  size_t readed;

  data = (pdf_stm_mem_data_t) be_data;

  *buf = (pdf_char_t *) xmalloc (bytes);

  if ((data->current + bytes) >= data->size)
    {
      readed = data->size - data->current;
    }
  else
    {
      readed = bytes;
    }

  if (readed > 0)
    {
      memcpy (*buf,
              data->data + data->current,
              readed);
      data->current = data->current + readed;
    }

  return readed;
}

size_t
pdf_stm_mem_write (void *be_data,
                   pdf_char_t *buf,
                   size_t bytes)
{
  pdf_stm_mem_data_t data;
  size_t written;
  size_t free_bytes;

  data = (pdf_stm_mem_data_t) be_data;
  free_bytes = data->size - data->current;

  if (data->resize_p &&
      (bytes > free_bytes))
    {
      /* Allocate space */
      data->data = 
        (pdf_char_t *) xrealloc (data->data,
                           data->size + (bytes - free_bytes));
      data->size = data->size + (bytes - free_bytes);
      free_bytes = data->size - data->current;
    }
  
  if (bytes > free_bytes)
    {
      written = free_bytes;
    }
  else
    {
      written = bytes;
    }

  if (written > 0)
    {
      memcpy (data->data + data->current,
              buf,
              written);
      data->current = data->current + written;
    }
     
  return written;
}

int
pdf_stm_mem_read_char (void *be_data)
{
  return pdf_stm_mem_readpeek_char (be_data, PDF_FALSE);
}

int
pdf_stm_mem_peek_char (void *be_data)
{
  return pdf_stm_mem_readpeek_char (be_data, PDF_TRUE);
}

size_t
pdf_stm_mem_flush (void *be_data)
{
  /* Do nothing */
  return 0;
}

/* Private functions */

int
pdf_stm_mem_readpeek_char (void *be_data, 
                           int peek_p)
{
  pdf_stm_mem_data_t data;
  int c;

  data = (pdf_stm_mem_data_t) be_data;

  if (data->current < data->size)
    {
      c = data->data[data->current];
      if (!peek_p)
        {
          data->current++;
        }
      return c;
    }
  else
    {
      return PDF_EOF;      
    }
}


/* End of pdf_stm_mem.h */

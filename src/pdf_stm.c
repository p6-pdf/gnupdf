/* -*- mode: C -*- Time-stamp: "07/07/13 18:15:59 jemarch"
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


#include <config.h>
#include <unistd.h>
#include <xalloc.h>
#include <pdf_stm_file.h>
#include <pdf_stm_mem.h>
#include <pdf_stm_f_null.h>
#include <pdf_stm_f_pred.h>

#ifdef HAVE_LIBZ
 #include <pdf_stm_f_fdec.h>
#endif /* HAVE_LIBZ */

#include <pdf_stm.h>

static pdf_stm_t pdf_stm_alloc (void);
static void pdf_stm_dealloc (pdf_stm_t stm);
static int pdf_stm_install_filter (pdf_stm_t stm,
                                   int direction,
                                   pdf_stm_init_filter_fn_t init_fn,
                                   pdf_stm_apply_filter_fn_t process_fn,
                                   pdf_stm_dealloc_filter_fn_t dealloc_fn,
                                   void *conf);
static void pdf_stm_filter_dealloc_list (const void *elt);
static int pdf_stm_apply_filters (gl_list_t filter_list, pdf_char_t **buf, size_t *buf_size);
                                   

/*
 * Backend-specific initialization functions
 */

pdf_stm_t
pdf_create_file_stm (char *filename,
                     int mode)
{
  pdf_stm_t new_stm;
  struct pdf_stm_file_conf_s conf;

  new_stm = pdf_stm_alloc ();
  
  /* Install backend functions */
  new_stm->backend.funcs.init = pdf_stm_file_init;
  new_stm->backend.funcs.write_p = pdf_stm_file_write_p;
  new_stm->backend.funcs.read_p = pdf_stm_file_read_p;
  new_stm->backend.funcs.seek_p = pdf_stm_file_seek_p;
  new_stm->backend.funcs.size_p = pdf_stm_file_size_p;
  new_stm->backend.funcs.peek_p = pdf_stm_file_peek_p;
  new_stm->backend.funcs.tell_p = pdf_stm_file_tell_p;
  new_stm->backend.funcs.size = pdf_stm_file_size;
  new_stm->backend.funcs.seek = pdf_stm_file_seek;
  new_stm->backend.funcs.tell = pdf_stm_file_tell;
  new_stm->backend.funcs.read = pdf_stm_file_read;
  new_stm->backend.funcs.write = pdf_stm_file_write;
  new_stm->backend.funcs.peek = pdf_stm_file_peek;
  new_stm->backend.funcs.close = pdf_stm_file_close;

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
  if (!((new_stm->backend.funcs.init) (&new_stm->backend.data, &conf)))
    {
      pdf_stm_dealloc (new_stm);
      return NULL;
    }
  else
    {
      return new_stm;
    }
}

pdf_stm_t                                  
pdf_create_mem_stm (pdf_stm_pos_t size,
                    int init_p,      
                    unsigned char init,
                    int resize_p)
{
  pdf_stm_t new_stm;
  struct pdf_stm_mem_conf_s conf;          

  new_stm = pdf_stm_alloc ();

  /* Install backend functions */
  new_stm->backend.funcs.init = pdf_stm_mem_init;
  new_stm->backend.funcs.write_p = pdf_stm_mem_write_p;
  new_stm->backend.funcs.read_p = pdf_stm_mem_read_p;
  new_stm->backend.funcs.seek_p = pdf_stm_mem_seek_p;
  new_stm->backend.funcs.size_p = pdf_stm_mem_size_p;
  new_stm->backend.funcs.peek_p = pdf_stm_mem_peek_p;
  new_stm->backend.funcs.size = pdf_stm_mem_size;
  new_stm->backend.funcs.seek = pdf_stm_mem_seek;
  new_stm->backend.funcs.tell = pdf_stm_mem_tell;
  new_stm->backend.funcs.read = pdf_stm_mem_read;
  new_stm->backend.funcs.write = pdf_stm_mem_write;
  new_stm->backend.funcs.peek = pdf_stm_mem_peek;
  new_stm->backend.funcs.close = pdf_stm_mem_close;

  /* Configure the backend */
  conf.size = size;
  conf.init_p = init_p;
  conf.init_char = init;
  conf.resize_p = resize_p;

  /* Initialize the backend */
  if (!(new_stm->backend.funcs.init) (&new_stm->backend.data, &conf))
    {
      pdf_stm_dealloc (new_stm);
      return NULL;
    }
  else
    {
      return new_stm;
    }
}

/*
 * Filter-specific installation functions
 */

int
pdf_stm_install_null_filter (pdf_stm_t stm, 
                             int direction)
{
  return pdf_stm_install_filter (stm,
                                 direction,
                                 pdf_stm_f_null_init,
                                 pdf_stm_f_null_apply,
                                 pdf_stm_f_null_dealloc,
                                 NULL);
}

#ifdef HAVE_LIBZ
int
pdf_stm_install_fdec_filter (pdf_stm_t stm,
                             int direction)
{
  return pdf_stm_install_filter (stm,
                                 direction,
                                 pdf_stm_f_fdec_init,
                                 pdf_stm_f_fdec_apply,
                                 pdf_stm_f_fdec_dealloc,
                                 NULL);
}
#endif /* HAVE_LIBZ */

int
pdf_stm_install_pred_filter (pdf_stm_t stm,
                             int direction,
                             int predictor,
                             int colors,
                             int bits_per_component,
                             int columns)
{
  struct pdf_stm_f_pred_conf_s config;

  config.predictor = predictor;
  config.colors = colors;
  config.bits_per_component = bits_per_component;
  config.columns = columns;

  return pdf_stm_install_filter (stm,
                                 direction,
                                 pdf_stm_f_pred_init,
                                 pdf_stm_f_pred_apply,
                                 pdf_stm_f_pred_dealloc,
                                 &config);
}

int
pdf_stm_install_ahexdec_filter (pdf_stm_t stm, 
                                int direction)
{
  struct pdf_stm_f_ahex_conf_s conf;

  conf.mode = PDF_STM_F_AHEX_MODE_DECODE;

  return pdf_stm_install_filter (stm,
                                 direction,
                                 pdf_stm_f_ahdec_init,
                                 pdf_stm_f_ahdec_apply,
                                 pdf_stm_f_ahdec_dealloc,
                                 &conf);
}

int
pdf_stm_install_ahexenc_filter (pdf_stm_t stm, 
                                int direction)
{
  struct pdf_stm_f_ahex_conf_s conf;

  conf.mode = PDF_STM_F_AHEX_MODE_ENCODE;

  return pdf_stm_install_filter (stm,
                                 direction,
                                 pdf_stm_f_ahdec_init,
                                 pdf_stm_f_ahdec_apply,
                                 pdf_stm_f_ahdec_dealloc,
                                 &conf);
}

/* 
 * Generic functions
 */

int
pdf_stm_close (pdf_stm_t stm)
{
  int status;

  status = 
    stm->backend.funcs.close (&stm->backend.data);
  pdf_stm_dealloc (stm);

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
              unsigned char **buf,
              size_t bytes)
{
  size_t readed_bytes;

  if (stm->backend.funcs.read_p (BE_DATA(stm)))
    {
      readed_bytes = stm->backend.funcs.read (BE_DATA(stm),
                                              buf,
                                              bytes);
    }
  else
    {
      readed_bytes = 0;
    }

  if (readed_bytes != 0)
    {
      pdf_stm_apply_filters (stm->read_filter_list,
                             buf, 
                             &readed_bytes);
    }

  return readed_bytes;
}

size_t
pdf_stm_write (pdf_stm_t stm,
               unsigned char *buf,
               size_t bytes)
{
  size_t written_bytes;

  pdf_stm_apply_filters (stm->write_filter_list,
                         &buf,
                         &bytes);

  if (stm->backend.funcs.write_p (BE_DATA(stm)))
    {
      written_bytes = stm->backend.funcs.write (BE_DATA(stm),
                                                buf,
                                                bytes);
    }
  else
    {
      written_bytes = 0;
    }

  return written_bytes;
}

size_t
pdf_stm_peek (pdf_stm_t stm,
              unsigned char **buf,
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

int
pdf_stm_uninstall_filters (pdf_stm_t stm)
{
  int count;

  for (count = 0; 
       count < gl_list_size (stm->read_filter_list); 
       count++)
    {
      gl_list_remove_at (stm->read_filter_list, 0);
    }

  for (count = 0; 
       count < gl_list_size (stm->write_filter_list); 
       count++)
    {
      gl_list_remove_at (stm->write_filter_list, 0);
    }

  return PDF_OK;
}


/* Private functions */

static pdf_stm_t 
pdf_stm_alloc (void)
{
  pdf_stm_t stm;

  stm = (pdf_stm_t) xmalloc (sizeof (struct pdf_stm_s));
  stm->read_filter_list =
    gl_list_create_empty (GL_ARRAY_LIST,
                          NULL,      /* compare_fn */
                          NULL,      /* hashcode_fn */
                          pdf_stm_filter_dealloc_list,
                          PDF_TRUE); /* allow duplicates */
  stm->write_filter_list =
    gl_list_create_empty (GL_ARRAY_LIST,
                          NULL,      /* compare_fn */
                          NULL,      /* hashcode_fn */
                          pdf_stm_filter_dealloc_list,
                          PDF_TRUE); /* allow duplicates */

  return stm;
}

static void
pdf_stm_dealloc (pdf_stm_t stm)
{
  gl_list_free (stm->read_filter_list);
  gl_list_free (stm->write_filter_list);
  free (stm);
}

static int 
pdf_stm_install_filter (pdf_stm_t stm,
                        int direction,
                        pdf_stm_init_filter_fn_t init_fn,
                        pdf_stm_apply_filter_fn_t apply_fn,
                        pdf_stm_dealloc_filter_fn_t dealloc_fn,
                        void *conf)
{
  pdf_stm_filter_t filter;
  
  /* Create the new filter */
  filter = (pdf_stm_filter_t) xmalloc (sizeof(struct pdf_stm_filter_s));
  filter->funcs.init = init_fn;
  filter->funcs.apply = apply_fn;
  filter->funcs.dealloc = dealloc_fn;

  /* Initialize it */
  if (conf != NULL)
    {
      (filter->funcs.init) (&filter->data, &conf);
    }
  else
    {
      (filter->funcs.init) (&filter->data, NULL);
    }

  /* Queue it */
  switch (direction)
    {
    case PDF_STM_FILTER_READ:
      {
        gl_list_add_last (stm->read_filter_list, 
                          (const void *) filter);
        break;
      }
    case PDF_STM_FILTER_WRITE:
      {
        gl_list_add_last (stm->write_filter_list, 
                          (const void *) filter);
        break;
      }
    default:
      {
        /* Make compiler happy */
        break;
      }
    }

  return PDF_OK;
}

static void
pdf_stm_filter_dealloc_list (const void *elt)
{
  pdf_stm_filter_t filter;

  filter = (pdf_stm_filter_t) elt;

  (filter->funcs.dealloc) (&filter->data);
  free (filter);
}

static int
pdf_stm_apply_filters (gl_list_t filter_list,
                       pdf_char_t **buf,
                       size_t *buf_size)
{
  pdf_stm_filter_t filter;
  gl_list_iterator_t iter;
  gl_list_node_t node;
  pdf_char_t *filtered_data;
  pdf_stm_pos_t filtered_size;

  if (gl_list_size (filter_list) == 0)
    {
      return PDF_TRUE;
    }
  
  iter = gl_list_iterator (filter_list);
  while (gl_list_iterator_next (&iter, (const void **) &filter, &node))
    {
      if (filter->funcs.apply (filter->data, 
                               *buf, *buf_size,
                               &filtered_data, &filtered_size))
        {
          /* This filter was applied successfully. Replace data with
             the result */
          free (*buf);
          *buf = filtered_data;
          *buf_size = filtered_size;
        }
      else
        {
          /* Filter application failed. Free any residual data */
          free (filtered_data);
        }
    }
  gl_list_iterator_free (&iter);

  return PDF_OK;
}

/* End of pdf_stm.c */

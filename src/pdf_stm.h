/* -*- mode: C -*- Time-stamp: "07/07/15 02:04:09 jemarch"
 *
 *       File:         pdf_stm.h
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Fri Jul  6 18:37:57 2007
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

/* This library module implement stream objects capable to front-end
   to several backends: filesystem files, memory buffers, sockets,
   etc.

   Filters (such as the PDF standard ones) are supported for both
   reading and writing. Many filters should be used in a single stream
   (are applied in the an order when writing and in the inverse order
   when reading).

 
                               +----------+
                               |          |<-----> file
      <--read----(filtering)---|          |<-----  input socket
                               |  stream  |------> output socket
       --write---(filtering)-->|          |
                               |          |<-----> memory buffer
                               +----------+
*/

#ifndef _PDF_STM_H
#define _PDF_STM_H

#include <stdio.h>
#include <config.h>
#include <gl_array_list.h>
#include <pdf_base.h>
#include <pdf_stm_f_null.h>
#include <pdf_stm_f_flate.h>
#include <pdf_stm_f_pred.h>
#include <pdf_stm_f_ahex.h>

/* Please note that `pdf_stm_pos_t' is defined in `pdf_base.h' */

/* Mode to use when opening.

   Note that the mode may not have any sense for some backends. For
   others, only some of the modes may have sense */

enum pdf_stm_open_mode_t
{
  PDF_STM_OPEN_MODE_READ,
  PDF_STM_OPEN_MODE_WRITE,
  PDF_STM_OPEN_MODE_APPEND
};

/* Backends

   Each backend should provide the virtual functions depicted in the
   `pdf_stm_be_s' structure:

   `init'

       This call should initialize the internal state of the backend.

   `write_p'
   `read_p'
   `seek_p'
   `size_p'
   `peek_p'
   `close_p'
   `tell_p'
   
       Those boolean functions tell the stream frontend if the backend
       accepts reading, writing, seeking, peeking, closing or getting
       its size.

   `size'

       Return the size (in octects) of the storage managed by the
       backend. This function should only be called when `size_p'
       returns true.

   `seek'

       Seek in the backend storage. This function should only be
       called when `size_p' returns true.

   `tell'

       Return the current position of the read/write pointer in the
       backend storage. This function should only be called when
       `tell_p' returns true.
 
   `read'

       Read data from the backend storage. This function should only
       be called when `read_p' returns true.

   `write'

       Write data to the backend storage. This function should only be
       called when `write_p' returns true.

   `peek'

       Peek some data from the backend storage. This function should
       only be called when `peek_p' returns true.

   `close'

       Closes the backend storage. This function should only be called
       when `close_p' returns true.

*/

struct pdf_stm_be_s
{
  struct
  {

    /* Initialization */
    int (*init) (void **be_data, void *conf_data);

    /* Getting backend capabilities */
    int (*write_p) (void *data);
    int (*read_p) (void *data);
    int (*seek_p) (void *data);
    int (*size_p) (void *data);
    int (*peek_p) (void *data);
    int (*tell_p) (void *data);
    
    /* Positioning and getting size */
    pdf_stm_pos_t (*size) (void *data);
    int (*seek) (void *data, pdf_stm_pos_t pos);
    pdf_stm_pos_t (*tell) (void *data);

    /* Reading and writing data */
    size_t (*read) (void *data, pdf_char_t **buf, size_t bytes);
    size_t (*write) (void *data, pdf_char_t *buf, size_t bytes);
    size_t (*peek) (void *data, pdf_char_t **buf, size_t bytes);

    /* Closing */
    int (*close) (void **data);

  } funcs;

  void *data;
};

#define BE_DATA(STM) ((STM)->backend.data)

/* Filters

   This stream implementation support the concept of read/write
   filters.

   Each filter should provide the virtual functions depicted in the
   `pdf_stm_filter_s' structure:

   `init'

       This call should initialize the internal state of the filter.

   `apply'

       This function apply an input buffer and writes the result of
       the application of the filter into an output buffer.

       If the contents of the input buffer doesnt conform valid data
       for the given buffer, this call should return PDF_FALSE.

   `dealloc'
  
       This function should dealloc all memory resources managed by the
       filter, and perform any needed finalization tasks.

*/

typedef int (*pdf_stm_init_filter_fn_t) (void **filter_data, void *conf_data);
typedef int (*pdf_stm_apply_filter_fn_t) (void *filter_data,
                                          pdf_char_t *in, pdf_stm_pos_t in_size, 
                                          pdf_char_t **out, pdf_stm_pos_t *out_size);
typedef int (*pdf_stm_dealloc_filter_fn_t) (void **filter_data);

struct pdf_stm_filter_s
{
  struct
  {

    pdf_stm_init_filter_fn_t init;
    pdf_stm_apply_filter_fn_t apply;
    pdf_stm_dealloc_filter_fn_t dealloc;

  } funcs;

  void *data;
};

typedef struct pdf_stm_filter_s *pdf_stm_filter_t;

enum pdf_stm_fdir_t
{
  PDF_STM_FILTER_READ,
  PDF_STM_FILTER_WRITE
};

/* 
 * Streams
 */

struct pdf_stm_s
{
  struct pdf_stm_be_s backend;
  
  gl_list_t read_filter_list;
  gl_list_t write_filter_list;
};

typedef struct pdf_stm_s *pdf_stm_t;


/* 
 * Stream API
 */

/* Creation and destruction */
pdf_stm_t pdf_create_file_stm (char *filename, int mode);
pdf_stm_t pdf_create_mem_stm (pdf_stm_pos_t size, int init_p, unsigned char init, int resize_p);
int pdf_stm_close (pdf_stm_t stm);

/* Getting information about stream objects */
pdf_stm_pos_t pdf_stm_size (pdf_stm_t stm);

/* Positioning */
int pdf_stm_seek (pdf_stm_t stm, pdf_stm_pos_t pos);
pdf_stm_pos_t pdf_stm_tell (pdf_stm_t stm);
size_t pdf_stm_read (pdf_stm_t stm, unsigned char **buf, size_t bytes);
size_t pdf_stm_write (pdf_stm_t stm, unsigned char *buf, size_t bytes);
size_t pdf_stm_peek (pdf_stm_t stm, unsigned char **buf, size_t bytes);

/* Managing filters */

int pdf_stm_install_null_filter (pdf_stm_t stm, int direction);
int pdf_stm_install_flatedec_filter (pdf_stm_t stm, int direction);
int pdf_stm_install_flateenc_filter (pdf_stm_t stm, int direction);
int pdf_stm_install_pred_filter (pdf_stm_t stm,
                                 int direction,
                                 int predictor,
                                 int colors,
                                 int bits_per_component,
                                 int columns);
int pdf_stm_install_ahexdec_filter (pdf_stm_t stm, int direction);
int pdf_stm_install_ahexenc_filter (pdf_stm_t stm, int direction);
/*int pdf_stm_install_asciihexenc_filter (pdf_stm_t stm, int direction);
int pdf_stm_install_ascii85dec_filter (pdf_stm_t stm, int direction);
int pdf_stm_install_ascii85enc_filter (pdf_stm_t stm, int direction);*/
int pdf_stm_uninstall_filters (pdf_stm_t stm); 

#endif /* pdf_stm.h */

/* End of pdf_stm.h */

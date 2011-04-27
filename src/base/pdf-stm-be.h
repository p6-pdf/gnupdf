/* -*- mode: C -*-
 *
 *       File:         pdf-stm-be.h
 *       Date:         Wed Jun 18 20:53:27 2008
 *
 *       GNU PDF Library - Stream backend
 *
 */

/* Copyright (C) 2008-2011 Free Software Foundation, Inc. */

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

#ifndef PDF_STM_BE_H
#define PDF_STM_BE_H

#include <config.h>

#include <pdf-types.h>
#include <pdf-error.h>

typedef struct pdf_stm_be_s pdf_stm_be_t;

typedef struct {
  /* Read operation in the backend */
  pdf_ssize_t (* read)    (pdf_stm_be_t  *be,
                           pdf_uchar_t   *buffer,
                           pdf_size_t     bytes,
                           pdf_error_t  **error);
  /* Write operation in the backend */
  pdf_ssize_t (* write)   (pdf_stm_be_t  *be,
                           pdf_uchar_t   *buffer,
                           pdf_size_t     bytes,
                           pdf_error_t  **error);
  /* Seek operation in the backend */
  pdf_off_t   (* seek)    (pdf_stm_be_t  *be,
                           pdf_off_t      pos);
  /* Tell operation in the backend */
  pdf_off_t   (* tell)    (pdf_stm_be_t  *be);
  /* Destroy the backend */
  void        (* destroy) (pdf_stm_be_t  *be);
} pdf_stm_be_vtable_t;

/* Generic struct for the backend, implementations will subclass this */
struct pdf_stm_be_s
{
  const pdf_stm_be_vtable_t *vtable;
};

#define pdf_stm_be_read(be,buffer,bytes,error)   \
  be->vtable->read (be, buffer, bytes, error)
#define pdf_stm_be_write(be,buffer,bytes,error) \
  be->vtable->write (be, buffer, bytes, error)
#define pdf_stm_be_seek(be,pos)                 \
  be->vtable->seek (be, pos)
#define pdf_stm_be_tell(be)                     \
  be->vtable->tell (be)
#define pdf_stm_be_destroy(be)                  \
  be->vtable->destroy (be)

#endif /* !PDF_STM_BE_H */

/* End of pdf-stm-be.h */

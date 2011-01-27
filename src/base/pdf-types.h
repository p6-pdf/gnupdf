/* -*- mode: C -*-
 *
 *       File:         pdf-types.h
 *       Date:         Sun Feb 10 21:30:00 2008
 *
 *       GNU PDF Library - Basic Types Module
 *
 */

/* Copyright (C) 2008, 2009, 2010 Free Software Foundation, Inc */

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

#ifndef PDF_TYPES_H
#define PDF_TYPES_H

#include <config.h>

#include <pdf-error.h>

/* BEGIN PUBLIC */
#include <sys/types.h> /* for off_t.  */
#include <stdint.h>    /* for uint32_t and others, from gnulib.  */
#include <stdbool.h>   /* for bool.  */

#include <sys/param.h> /* for determining system types (mostly BSD).  */

#ifdef PDF_HAVE_INLINE
#define INLINE inline
#else
#define INLINE
#endif /* PDF_HAVE_INLINE */

#define PDF_TRUE 1
#define PDF_FALSE 0

#define PDF_MIN(i1, i2) \
  (((i1) < (i2)) ? (i1) : (i2))

#define PDF_MAX(i1, i2) \
  (((i1) > (i2)) ? (i1) : (i2))

/* Definitions used in the operations of the pdf_i64_t type. */
#define PDF_I32_MAX INT32_MAX
#define PDF_I32_MIN (-PDF_I32_MAX)
#define PDF_I32_DIV (PDF_I32_MAX + 1)
#define PDF_U32_MAX UINT32_MAX
#define PDF_U16_MAX UINT16_MAX
#define PDF_U16_DIV (PDF_U16_MAX + 1)
#define PDF_U8_MAX UINT8_MAX
#define PDF_U8_DIV (UINT8_MAX + 1)


/* A variable of type `pdf_off_t' contain a byte-offset relative to
   the beginning of a stream object.

   Please be careful manipulating `pdf_off_t' values. Its value is
   assured to be a signed scalable one, but its size may be wider than
   a long. */
#define NO_POS -1
typedef off_t pdf_off_t;

/* Memory indexes (size_t) */
typedef size_t pdf_size_t;

/* Characters.
 *
 * We provide two types here: a pdf_char_t that follow the signess of
 * the system 'char' type, and a pdf_uchar_t that is like an unsigned
 * char.
 */
typedef char pdf_char_t;
typedef unsigned char pdf_uchar_t;

/* 32-bit unsigned integer */
typedef uint32_t pdf_u32_t;

/* 32-bit signed integer */
typedef int32_t pdf_i32_t;

/* 16-bit unsigned integer */
typedef uint16_t pdf_u16_t;

/* 16-bit signed integer */
typedef int16_t pdf_i16_t;

/* 8-bit unsigned integer */
typedef unsigned char pdf_u8_t;

/* 8-bit signed integer */
typedef char pdf_i8_t;

/* Boolean type */
typedef bool pdf_bool_t;

/********************** Simple memory buffers **************************/

struct pdf_buffer_s
{
  pdf_char_t *data;    /* Buffer contents */
  pdf_size_t size;     /* Size of the buffer in octects */
  pdf_size_t rp;       /* Read pointer */
  pdf_size_t wp;       /* Write pointer */
};

typedef struct pdf_buffer_s *pdf_buffer_t;

pdf_buffer_t pdf_buffer_new (pdf_size_t size);
pdf_status_t pdf_buffer_destroy (pdf_buffer_t buffer);

pdf_bool_t pdf_buffer_full_p (pdf_buffer_t buffer);
pdf_bool_t pdf_buffer_eob_p (pdf_buffer_t buffer);

pdf_status_t pdf_buffer_resize (pdf_buffer_t buffer,
                                pdf_size_t newsize);
pdf_status_t pdf_buffer_rewind (pdf_buffer_t buffer);

/********************** progress monitors **************************/

typedef pdf_status_t (*pdf_pm_begin_operation_fn_t) (void *client_data);
typedef pdf_status_t (*pdf_pm_end_operation_fn_t) (void *client_data);
typedef pdf_status_t (*pdf_pm_get_duration_fn_t) (void *client_data, void *duration);
typedef pdf_status_t (*pdf_pm_set_duration_fn_t) (void *client_data, void *duration);
typedef pdf_status_t (*pdf_pm_get_current_value_fn_t) (void *client_data, void *duration);
typedef pdf_status_t (*pdf_pm_set_current_value_fn_t) (void *client_data, void *duration);
typedef pdf_status_t (*pdf_pm_set_text_fn_t) (void *client_data, void *text);

struct pdf_pm_s
{
  pdf_pm_begin_operation_fn_t begin_operation_fn;
  pdf_pm_end_operation_fn_t end_operation_fn;
  pdf_pm_get_duration_fn_t get_duration_fn;
  pdf_pm_set_duration_fn_t set_duration_fn;
  pdf_pm_get_current_value_fn_t get_current_value_fn_t;
  pdf_pm_set_current_value_fn_t set_current_value_fn_t;
  pdf_pm_set_text_fn_t set_text_fn;
};

/* END PUBLIC */

#endif /* !PDF_TYPES_H */

/* End of pdf-types.h */

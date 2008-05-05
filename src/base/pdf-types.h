/* -*- mode: C -*- Time-stamp: "2008-03-06 00:07:40 gerel"
 *
 *       File:         pdf-types.h
 *       Date:         Sun Feb 10 21:30:00 2008
 *
 *       GNU PDF Library - Basic Types Module
 *
 */

/* Copyright (C) 2008 Free Software Foundation, Inc */

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
#include <sys/types.h> /* for off_t */
#include <stdint.h> /* for uint32_t and others, from gnulib */


#ifdef HAVE_INLINE
#define INLINE inline
#else
#define INLINE
#endif /* HAVE_INLINE */

#define PDF_TRUE 1
#define PDF_FALSE 0

/* Definitions used in the operations of the  pdf_i64_t type*/
#define PDF_I32_MAX 2147483647;
#define PDF_I32_DIV 2147483648;
#define PDF_U32_MAX 4294967295;
#define PDF_U16_MAX 65535;
#define PDF_U16_DIV 65536;
#define PDF_U8_MAX 255;
#define PDF_U8_DIV 256;


/* A variable of type `pdf_stm_pos_t' contain a byte-offset relative to
   the beginning of a stream object. 

   Please be careful manipulating `pdf_stm_pos_t' values. Its value is
   assured to be a signed scalable one, but its size may be wider than
   a long. */
#define NO_POS -1
typedef off_t pdf_stm_pos_t;

/* Memory indexes (size_t) */
typedef size_t pdf_size_t;

/* PDF manages bytes with values in the range 0-255 */
typedef unsigned char pdf_char_t;

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
typedef unsigned char pdf_bool_t;


/*Definition of internal structure of the pdf_i64_t type*/
struct pdf_i64_s
{
  signed   high;
  unsigned low;
};

/*Definition of the pdf_i64_t type*/
typedef struct pdf_i64_s pdf_i64_t;

/*Create and initialise a new pdf_i64_t variable*/
pdf_i64_t pdf_i64_new(signed high, unsigned low);

/*Assign values to a pdf_i64_t pointer*/
pdf_status_t 
pdf_i64_assign (pdf_i64_t *bignum, signed high, unsigned low);

/*Quick version of assignment in which only the lowest siginificant
  part is taken into account*/
pdf_status_t 
pdf_i64_assign_quick(pdf_i64_t *bignum, signed value);

/*Copy one pdf_i64_t variable into another*/
pdf_status_t 
pdf_i64_copy(pdf_i64_t orig, pdf_i64_t *copy);

/*Add two pdf_i64_t variables*/
pdf_status_t 
pdf_i64_add(pdf_i64_t *dest, pdf_i64_t addend1, pdf_i64_t addend2);

/*Compare two pdf_i64_t variables*/
int pdf_i64_cmp(pdf_i64_t number_1, pdf_i64_t number_2);

/*Calculate the absolute value of a pdf_i64_t variable*/
pdf_status_t 
pdf_i64_abs(pdf_i64_t *dest, pdf_i64_t number);

/*Negate a pdf_i64_t type variable*/
pdf_status_t 
pdf_i64_neg(pdf_i64_t *dest, pdf_i64_t number);

/*Subtract two pdf_i64_t variables*/
pdf_status_t 
pdf_i64_subtraction(pdf_i64_t *dest, pdf_i64_t minuend, pdf_i64_t subtrahend);

/*Multiply two pdf_i64_t variables*/
pdf_status_t 
pdf_i64_mult(pdf_i64_t *dest, pdf_i64_t factor_1, pdf_i64_t factor_2);

/*Division between two pdf_i64_t type variables*/
pdf_status_t 
pdf_i64_div(pdf_i64_t *dest, pdf_i64_t dividend, pdf_i64_t divisor);

/*Modulus division between two pdf_i64_t variables*/
pdf_status_t 
pdf_i64_mod(pdf_i64_t *dest, pdf_i64_t dividend, pdf_i64_t divisor);





/* PDF_EOF to store an EOF marker in integers */
#define PDF_EOF -1


/* Host dependent definitions to be used */

#if defined _WIN32 || defined __WIN32__
 #define PDF_HOST_WIN32
#endif



/* END PUBLIC */

#endif /* !PDF_TYPES_H */

/* End of pdf-types.h */

/* -*- mode: C -*-
 *
 *       File:         pdf-types-64.h
 *       Date:         Wed Jan 26 17:17:54 2011
 *
 *       GNU PDF Library - Basic Types Module, Bignum support
 *
 */

/* Copyright (C) 2008-2011 Free Software Foundation, Inc */

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

#ifndef PDF_TYPES_64_H
#define PDF_TYPES_64_H

#include <config.h>

#include <pdf-types.h>
#include <pdf-error.h>

/* BEGIN PUBLIC */

/********************** 64-bit type built-in support **************************/

/* Note that int64_t may be defined by the gnulib stdint.h in
   platforms missing that header file.  To force use of BIGNUM, define
   PDF_FORCE_BIGNUMS in config.h. */
#if defined PDF_HAVE_INT64_T || defined int64_t && !defined PDF_FORCE_BIGNUMS
#  define PDF_USE_BUILTIN_64BIT_SUPPORT
#endif

#ifndef PDF_USE_BUILTIN_64BIT_SUPPORT

/* Definition of internal structure of the pdf_i64_t type */
struct pdf_i64_s
{
  pdf_i32_t high;
  pdf_u32_t low;
};
typedef struct pdf_i64_s pdf_i64_t;

#else

/* 64-bit signed integer supported by system. */
typedef int64_t pdf_i64_t;

#endif


#ifndef PDF_USE_BUILTIN_64BIT_SUPPORT

/* Create and initialise a new pdf_i64_t variable */
pdf_i64_t pdf_i64_new (const pdf_i32_t high,
                       const pdf_u32_t low);

/* Assign values to a pdf_i64_t pointer */
void pdf_i64_assign (pdf_i64_t       *bignum,
                     const pdf_i32_t  high,
                     const pdf_u32_t  low,
                     pdf_status_t    *p_status);

/* Quick version of assignment in which only the lowest siginificant
   part is taken into account */
void pdf_i64_assign_quick (pdf_i64_t       *bignum,
                           const pdf_i32_t  value,
                           pdf_status_t    *p_status);

/* Copy one pdf_i64_t variable into another */
void pdf_i64_copy (const pdf_i64_t  orig,
                   pdf_i64_t       *copy,
                   pdf_status_t    *p_status);

/* Add two pdf_i64_t variables */
void pdf_i64_add (pdf_i64_t       *dest,
                  const pdf_i64_t  addend1,
                  const pdf_i64_t  addend2,
                  pdf_status_t    *p_status);

/* Compare two pdf_i64_t variables */
int pdf_i64_cmp (const pdf_i64_t number_1,
                 const pdf_i64_t number_2);

/* Calculate the absolute value of a pdf_i64_t variable */
void pdf_i64_abs (pdf_i64_t       *dest,
                  const pdf_i64_t  number,
                  pdf_status_t    *p_status);

/* Negate a pdf_i64_t type variable */
void pdf_i64_neg (pdf_i64_t       *dest,
                  const pdf_i64_t  number,
                  pdf_status_t    *p_status);

/* Subtract two pdf_i64_t variables */
void pdf_i64_subtraction (pdf_i64_t       *dest,
                          const pdf_i64_t  minuend,
                          const pdf_i64_t  subtrahend,
                          pdf_status_t    *p_status);

/* Multiply two pdf_i64_t variables */
void pdf_i64_mult (pdf_i64_t       *dest,
                   const pdf_i64_t  factor_1,
                   const pdf_i64_t  factor_2,
                   pdf_status_t    *p_status);


/* Division between two pdf_i64_t type variables */
void pdf_i64_div (pdf_i64_t       *dest,
                  const pdf_i64_t  dividend,
                  const pdf_i64_t  divisor,
                  pdf_status_t    *p_status);

/* Modulus division between two pdf_i64_t variables */
void pdf_i64_mod (pdf_i64_t       *dest,
                  const pdf_i64_t  dividend,
                  const pdf_i64_t  divisor,
                  pdf_status_t    *p_status);

/* Add a pdf_i64_t and a pdf_i32_t */
void pdf_i64_add_i32 (pdf_i64_t       *dest,
                      const pdf_i64_t  addend1,
                      const pdf_i32_t  addend2,
                      pdf_status_t    *p_status);

/* Compare a pdf_i64_t and a pdf_i32_t */
int pdf_i64_cmp_i32 (const pdf_i64_t number_1,
                     const pdf_i32_t number_2);

/* Subtract a pdf_i64_t and a pdf_i32_t variable */
void pdf_i64_subtraction_i32_min (pdf_i64_t       *dest,
                                  const pdf_i32_t  minuend,
                                  const pdf_i64_t  subtrahend,
                                  pdf_status_t    *p_status);

void pdf_i64_subtraction_i32_sub (pdf_i64_t  *dest,
                             const pdf_i64_t  minuend,
                             const pdf_i32_t  subtrahend,
                             pdf_status_t     *p_status);

/* Multiply a pdf_i64_t and a pdf_i32_t */
void pdf_i64_mult_i32 (pdf_i64_t       *dest,
                       const pdf_i64_t  factor_1,
                       const pdf_i32_t  factor_2,
                       pdf_status_t    *p_status);

/* Divide a pdf_i64_t and a pdf_i32_t */
void pdf_i64_div_i32_dividend (pdf_i64_t       *dest,
                               const pdf_i32_t  dividend,
                               const pdf_i64_t  divisor,
                               pdf_status_t    *p_status);

void pdf_i64_div_i32_divisor (pdf_i64_t       *dest,
                              const pdf_i64_t  dividend,
                              const pdf_i32_t  divisor,
                              pdf_status_t    *p_status);

/* Modulus between a pdf_i64_t and a pdf_i32_t */
void pdf_i64_mod_i32_dividend (pdf_i64_t       *dest,
                               const pdf_i32_t  dividend,
                               const pdf_i64_t  divisor,
                               pdf_status_t    *p_status);

void pdf_i64_mod_i32_divisor (pdf_i64_t       *dest,
                              const pdf_i64_t  dividend,
                              const pdf_i32_t  divisor,
                              pdf_status_t    *p_status);

/* Get number as pdf_i32_t. Note that if the pdf_i64_t variable is holding a
 * number which can't be represented in 32bits, the result is undefined... so
 * use it with caution. */
pdf_i32_t pdf_i64_to_i32 (const pdf_i64_t bignum);

#else /* else of the PDF_USE_BUILTIN_64BIT_SUPPORT */

/*Create and initialise a new pdf_i64_t variable*/
#define pdf_i64_new(high, low) ((((pdf_i64_t)high)<<32) + low)

#define pdf_i64_assign(bignum, high, low, p_status)	\
  do                                                    \
    {                                                   \
      if (bignum == NULL)                               \
        {                                               \
          *p_status =  PDF_EBADDATA;                    \
        }                                               \
      else                                              \
        {                                               \
          *p_status =  PDF_OK;                          \
          *bignum = 0;                                  \
          *bignum = (((pdf_i64_t)high)<<32);            \
          *bignum = *bignum + low;                      \
        }                                               \
    } while(0)

/*Quick version of assignment in which only the lowest siginificant
  part is taken into account*/
#define pdf_i64_assign_quick(bignum, value, p_status)   \
  do                                                    \
    {                                                   \
      if (bignum == NULL)                               \
        {                                               \
          *p_status =  PDF_EBADDATA;                    \
        }                                               \
      else                                              \
        {                                               \
          *p_status =  PDF_OK;                          \
          *bignum = value;                              \
        }                                               \
    } while(0)

/*Copy one pdf_i64_t variable into another*/
#define pdf_i64_copy(orig, copy, p_status)      \
  do                                            \
    {                                           \
      if (copy == NULL)				\
        {                                       \
          *p_status =  PDF_EBADDATA;            \
        }                                       \
      else					\
        {                                       \
          *p_status =  PDF_OK;			\
          *copy = orig;				\
        }                                       \
    } while(0)

/*Add two pdf_i64_t variables*/
#define pdf_i64_add(dest, addend1, addend2, p_status)   \
  do                                                    \
    {                                                   \
      if (dest == NULL)                                 \
        {                                               \
          *p_status =  PDF_EBADDATA;                    \
        }                                               \
      else                                              \
        {                                               \
          *p_status =  PDF_OK;                          \
          *dest = addend1 + addend2;                    \
        }                                               \
    } while(0)

/*Compare two pdf_i64_t variables*/
#define pdf_i64_cmp(number_1, number_2) \
  ((number_1 > number_2) ? 1 : ((number_1 < number_2) ? -1 : 0))

/*Calculate the absolute value of a pdf_i64_t variable*/
#define pdf_i64_abs(dest, number, p_status) \
  do                                        \
    {                                       \
      if (dest == NULL)                     \
        {                                   \
          *p_status =  PDF_EBADDATA;        \
        }                                   \
      else                                  \
        {                                   \
          *p_status =  PDF_OK;              \
          *dest = abs(number);              \
        }                                   \
    } while(0)

/*Negate a pdf_i64_t type variable*/
#define pdf_i64_neg(dest, number, p_status) \
  do                                        \
    {                                       \
      if (dest == NULL)                     \
        {                                   \
          *p_status =  PDF_EBADDATA;        \
        }                                   \
      else                                  \
        {                                   \
          *p_status =  PDF_OK;              \
          *dest = -number;                  \
        }                                   \
    } while(0)

/*Subtract two pdf_i64_t variables*/
#define pdf_i64_subtraction(dest, minuend, subtrahend, p_status) \
  do                                                             \
    {                                                            \
      if (dest == NULL)                                          \
        {                                                        \
          *p_status =  PDF_EBADDATA;                             \
        }                                                        \
      else                                                       \
        {                                                        \
          *p_status =  PDF_OK;                                   \
          *dest = minuend - subtrahend;                          \
        }                                                        \
    } while(0)

/*Multiply two pdf_i64_t variables*/
#define pdf_i64_mult(dest, factor_1, factor_2, p_status) \
  do                                                     \
    {                                                    \
      if (dest == NULL)                                  \
        {                                                \
          *p_status =  PDF_EBADDATA;                     \
        }                                                \
      else                                               \
        {                                                \
          *p_status =  PDF_OK;                           \
          *dest = factor_1 * factor_2;                   \
        }                                                \
    } while(0)

/*Division between two pdf_i64_t type variables*/
#define pdf_i64_div(dest, dividend, divisor, p_status) \
  do                                                   \
    {                                                  \
      if (dest == NULL)                                \
        {                                              \
          *p_status =  PDF_EBADDATA;                   \
        }                                              \
      else                                             \
        {                                              \
          *p_status =  PDF_OK;                         \
          *dest = dividend/divisor;                    \
        }                                              \
    } while(0)

/*Modulus division between two pdf_i64_t variables*/
#define pdf_i64_mod(dest, dividend, divisor, p_status) \
  do                                                   \
    {                                                  \
      if (dest == NULL)                                \
        {                                              \
          *p_status =  PDF_EBADDATA;                   \
        }                                              \
      else                                             \
        {                                              \
          *p_status =  PDF_OK;                         \
          *dest = dividend%divisor;                    \
        }                                              \
    } while(0)

/* Add a pdf_i64_t and a pdf_i32_t */
#define pdf_i64_add_i32 pdf_i64_add

/* Compare a pdf_i64_t and a pdf_i32_t */
#define pdf_i64_cmp_i32 pdf_i64_cmp

/* Subtract a pdf_i64_t and a pdf_i32_t variable */
#define pdf_i64_subtraction_i32_min pdf_i64_subtraction
#define pdf_i64_subtraction_i32_sub pdf_i64_subtraction

/* Multiply a pdf_i64_t and a pdf_i32_t */
#define pdf_i64_mult_i32 pdf_i64_mult

/* Divide a pdf_i64_t and a pdf_i32_t */
#define pdf_i64_div_i32_dividend pdf_i64_div
#define pdf_i64_div_i32_divisor pdf_i64_div

/* Modulus between a pdf_i64_t and a pdf_i32_t */
#define pdf_i64_mod_i32_dividend pdf_i64_mod
#define pdf_i64_mod_i32_divisor pdf_i64_mod

/* Get number as pdf_i32_t. Note that if the pdf_i64_t variable is holding a
 * number which can't be represented in 32bits, the result is undefined... so
 * use it with caution. */
#define pdf_i64_to_i32(bignum) bignum

#endif

/* END PUBLIC */

#endif /* !PDF_TYPES_64H */

/* End of pdf-types-64.h */
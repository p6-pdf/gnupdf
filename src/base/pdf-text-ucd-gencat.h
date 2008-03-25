/* -*- mode: C -*- Time-stamp: ""
 *
 *       File:         pdf-text-ucd-gencat.h
 *       Date:         Sun Mar 09 12:59:01 2008
 *
 *       GNU PDF Library - Unicode Character Database - General Category Info
 *
 */

/* Copyright (C) 2008 Free Software Foundation, Inc. */

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

#ifndef _PDF_TEXT_UCD_GENCAT_H
#define _PDF_TEXT_UCD_GENCAT_H


#include <config.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif /* HAVE_MALLOC_H */


#include <pdf-text.h>


/* Enumeration of general categories of each Unicode point */
enum unicode_gencat_info_enum {
  UNICODE_GENCAT_Unknown = -1,
  UNICODE_GENCAT_Lu = 0, /* Letter, Uppercase */
  UNICODE_GENCAT_Ll, /* Letter, Lowercase */
  UNICODE_GENCAT_Lt, /* Letter, Titlecase */
  UNICODE_GENCAT_Lm, /* Letter, Modifier */
  UNICODE_GENCAT_Lo, /* Letter, Other */
  UNICODE_GENCAT_Mn, /* Mark, Nonspacing */
  UNICODE_GENCAT_Mc, /* Mark, Spacing Combining */
  UNICODE_GENCAT_Me, /* Mark, Enclosing */
  UNICODE_GENCAT_Nd, /* Number, Decimal Digit */
  UNICODE_GENCAT_Nl, /* Number, Letter */
  UNICODE_GENCAT_No, /* Number, Other */
  UNICODE_GENCAT_Pc, /* Punctuation, Connector */
  UNICODE_GENCAT_Pd, /* Punctuation, Dash */
  UNICODE_GENCAT_Ps, /* Punctuation, Open */
  UNICODE_GENCAT_Pe, /* Punctuation, Close */
  UNICODE_GENCAT_Pi, /* Punctuation, Initial quote */
  UNICODE_GENCAT_Pf, /* Punctuation, Final quote */
  UNICODE_GENCAT_Po, /* Punctuation, Other */
  UNICODE_GENCAT_Sm, /* Symbol, Math */
  UNICODE_GENCAT_Sc, /* Symbol, Currency */
  UNICODE_GENCAT_Sk, /* Symbol, Modifier */
  UNICODE_GENCAT_So, /* Symbol, Other */
  UNICODE_GENCAT_Zs, /* Separator, Space */
  UNICODE_GENCAT_Zl, /* Separator, Line */
  UNICODE_GENCAT_Zp, /* Separator, Paragraph */
  UNICODE_GENCAT_Cc, /* Other, Control */
  UNICODE_GENCAT_Cf, /* Other, Format */
  UNICODE_GENCAT_Cs, /* Other, Surrogate */
  UNICODE_GENCAT_Co, /* Other, Private use */
  UNICODE_GENCAT_Cn  /* Other, Not assigned */
};


/* Get General Category of a given UTF-32HE unicode point */
enum unicode_gencat_info_enum
pdf_text_ucd_get_general_category(const pdf_u32_t character);


#endif

/* End of pdf-text-ucd-gencat.h */

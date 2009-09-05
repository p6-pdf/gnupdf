/* -*- mode: C -*- Time-stamp: "09/09/05 22:55:13 jemarch"
 *
 *       File:         pdf-text-ucd-proplist.h
 *       Date:         Mon Mar 31 19:58:17 2008
 *
 *       GNU PDF Library - Unicode Character Database - Propertly List
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

#ifndef _PDF_TEXT_UCD_PROPLIST_H
#define _PDF_TEXT_UCD_PROPLIST_H

#include <config.h>

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif /* HAVE_MALLOC_H */

#include <pdf-text.h>

/*************** START OF SELF-GENERATED DATA *********************************/

enum pdf_text_ucd_proplist_e {
  PDF_TEXT_UCD_PL_None,
  PDF_TEXT_UCD_PL_White_Space,
  PDF_TEXT_UCD_PL_Bidi_Control,
  PDF_TEXT_UCD_PL_Join_Control,
  PDF_TEXT_UCD_PL_Dash,
  PDF_TEXT_UCD_PL_Hyphen,
  PDF_TEXT_UCD_PL_Quotation_Mark,
  PDF_TEXT_UCD_PL_Terminal_Punctuation,
  PDF_TEXT_UCD_PL_Other_Math,
  PDF_TEXT_UCD_PL_Hex_Digit,
  PDF_TEXT_UCD_PL_ASCII_Hex_Digit,
  PDF_TEXT_UCD_PL_Other_Alphabetic,
  PDF_TEXT_UCD_PL_Ideographic,
  PDF_TEXT_UCD_PL_Diacritic,
  PDF_TEXT_UCD_PL_Extender,
  PDF_TEXT_UCD_PL_Other_Lowercase,
  PDF_TEXT_UCD_PL_Other_Uppercase,
  PDF_TEXT_UCD_PL_Noncharacter_Code_Point,
  PDF_TEXT_UCD_PL_Other_Grapheme_Extend,
  PDF_TEXT_UCD_PL_IDS_Binary_Operator,
  PDF_TEXT_UCD_PL_IDS_Trinary_Operator,
  PDF_TEXT_UCD_PL_Radical,
  PDF_TEXT_UCD_PL_Unified_Ideograph,
  PDF_TEXT_UCD_PL_Other_Default_Ignorable_Code_Point,
  PDF_TEXT_UCD_PL_Deprecated,
  PDF_TEXT_UCD_PL_Soft_Dotted,
  PDF_TEXT_UCD_PL_Logical_Order_Exception,
  PDF_TEXT_UCD_PL_Other_ID_Start,
  PDF_TEXT_UCD_PL_Other_ID_Continue,
  PDF_TEXT_UCD_PL_STerm,
  PDF_TEXT_UCD_PL_Variation_Selector,
  PDF_TEXT_UCD_PL_Pattern_White_Space,
  PDF_TEXT_UCD_PL_Pattern_Syntax
};

/***************** END OF SELF-GENERATED DATA *********************************/



/* Returns true if the given UTF-32HE unicode point has the Soft_Dotted
 *  property */
pdf_bool_t
pdf_text_ucd_pl_is_Soft_Dotted(pdf_u32_t character);

/* NOTE: If needed, other properties can also be questioned like in 
 *  `pdf_text_ucd_pl_is_Soft_Dotted'. By now, this is not necessary. */

#endif

/* End of pdf-text-ucd-proplist.h */

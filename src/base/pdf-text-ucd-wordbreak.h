/* -*- mode: C -*- Time-stamp: ""
 *
 *       File:         pdf-text-ucd-wordbreak.h
 *       Date:         Sun Mar 09 12:59:01 2008
 *
 *       GNU PDF Library - Unicode Character Database - WordBreak Property
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

#ifndef _PDF_TEXT_UCD_WORDBREAK_H
#define _PDF_TEXT_UCD_WORDBREAK_H

#include <config.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif /* HAVE_MALLOC_H */


#include <pdf-text.h>


enum pdf_text_ucd_wb_property_e {
  PDF_TEXT_UCD_WBP_None,
  PDF_TEXT_UCD_WBP_Format,
  PDF_TEXT_UCD_WBP_Katakana,
  PDF_TEXT_UCD_WBP_ALetter,
  PDF_TEXT_UCD_WBP_MidLetter,
  PDF_TEXT_UCD_WBP_MidNum,
  PDF_TEXT_UCD_WBP_Numeric,
  PDF_TEXT_UCD_WBP_ExtendNumLet
};

/* Return the WordBreak Property of the given UTF-32HE unicode point */
enum pdf_text_ucd_wb_property_e
pdf_text_ucd_wb_get_property(pdf_u32_t character);


/* Returns true if the given UTF-32HE unicode point has the Format value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_format(pdf_u32_t character);


/* Returns true if the given UTF-32HE unicode point has the Katakana value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_katakana(pdf_u32_t character);


/* Returns true if the given UTF-32HE unicode point has the ALetter value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_aletter(pdf_u32_t character);


/* Returns true if the given UTF-32HE unicode point has the MidLetter value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_midletter(pdf_u32_t character);


/* Returns true if the given UTF-32HE unicode point has the Numeric value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_numeric(pdf_u32_t character);


/* Returns true if the given UTF-32HE unicode point has the MidNum value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_midnum(pdf_u32_t character);


/* Returns true if the given UTF-32HE unicode point has the ExtendNumLet value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_extendnumlet(pdf_u32_t character);


/* Detect next Word Break in UTF-32HE string */
pdf_status_t
pdf_text_ucd_wb_detect_next(const pdf_char_t *current,
                            const pdf_size_t n_bytes_left_in,
                            pdf_char_t **next,
                            pdf_size_t *n_bytes_left_out);


#endif

/* End of pdf-text-ucd-wordbreak.h */


/* -*- mode: C -*- Time-stamp: ""
 *
 *       File:         pdf-text-ucd-case.h
 *       Date:         Sun Mar 09 12:59:01 2008
 *
 *       GNU PDF Library - Unicode Character Database - Case Algorithms
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

#ifndef _PDF_TEXT_UCD_CASE_H
#define _PDF_TEXT_UCD_CASE_H


#include <config.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif /* HAVE_MALLOC_H */


#include <pdf-text.h>


/* List of case conversion types */
enum unicode_case_type {
  UNICODE_CASE_INFO_UPPER_CASE,
  UNICODE_CASE_INFO_LOWER_CASE,
  UNICODE_CASE_INFO_TITLE_CASE
};



/* Define maximum number of code points after applying a case conversion to a
 *  single code point */
#define UCD_SC_MAX_EXPAND 3


/* Convert a given UTF-32HE word to specified case.
 *  Warning!!! The destination word MUST contain enough space for the WORST
 *  conversion case, this is, for each unicode point, UCD_SC_MAX_EXPAND unicode 
 *  points are generated after the conversion!!! */
pdf_status_t
pdf_text_ucd_word_change_case(pdf_char_t *destination_word,
                              pdf_size_t *p_destination_length,
                              enum unicode_case_type destination_case,
                              const pdf_char_t *origin_word,
                              const pdf_size_t origin_length,
                              const pdf_char_t *origin_lang);


/* Check if a given UTF-32HE unicode point is case ignorable */
pdf_bool_t
pdf_text_ucd_is_case_ignorable(const pdf_u32_t character);


/* Check if a given UTF-32HE unicode point is cased */
pdf_bool_t
pdf_text_ucd_is_cased(const pdf_u32_t character);


#endif

/* End of pdf-text-ucd-case.h */

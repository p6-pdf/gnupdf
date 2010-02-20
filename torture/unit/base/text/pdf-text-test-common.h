/* -*- mode: C -*-
 *
 *       File:         pdf-text-test-common.h
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Commont utilities to test the Text Module
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

#ifndef _PDF_TEXT_TEST_COMMON_H
#define _PDF_TEXT_TEST_COMMON_H

#include <config.h>
#include <stdio.h>
#include <pdf.h>

/* Basic structure to store the test strings */
typedef struct test_string_s {
  char *data;
  int  size;
  char *utf32be_data;
  int  utf32be_size;
} test_string_t;


/* Test if the system is Big Endian */
short
pdf_text_test_big_endian_system(void);

/* Change UTF-32BE string to UTF-32LE and viceversa */
pdf_char_t *
pdf_text_test_change_utf32_endianness(const pdf_char_t *str_in,
                                      pdf_size_t size);

/* Change UTF-16BE string to UTF-16LE and viceversa */
pdf_char_t *
pdf_text_test_change_utf16_endianness(const pdf_char_t *str_in,
                                      pdf_size_t size);

/* Function quite similar to `pdf_text_get_hex', but using an array of bytes
 *  as input. */
pdf_char_t *
pdf_text_test_get_hex (const pdf_char_t *data,
                       const pdf_size_t size,
                       const pdf_char_t delimiter);

#endif /* _PDF_TEXT_TEST_COMMON_H */

/* End of pdf-text-test-common.h */

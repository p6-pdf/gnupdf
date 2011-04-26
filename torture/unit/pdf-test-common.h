/* -*- mode: C -*-
 *
 *       File:         pdf-text-common.h
 *       Date:         Thu Feb 24 22:53:03 2011
 *
 *       GNU PDF Library - Common utilities in the Unit Tests
 *
 */

/* Copyright (C) 2011 Free Software Foundation, Inc. */

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

#ifndef _PDF_TEST_COMMON_H
#define _PDF_TEST_COMMON_H

#include <stdio.h>
#include <pdf.h>

void pdf_test_setup (void);

void pdf_test_teardown (void);

/* Function quite similar to `pdf_text_get_hex', but using an array of bytes
 *  as input. */
pdf_char_t *pdf_test_get_hex (const pdf_uchar_t *data,
                              const pdf_size_t   size,
                              const pdf_char_t   delimiter);

#endif /* _PDF_TEST_COMMON_H */

/* End of pdf-test-common.h */

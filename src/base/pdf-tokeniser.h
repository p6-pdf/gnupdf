/* -*- mode: C -*-
 *
 *       File:         pdf-tokeniser.h
 *       Date:         Tue Jun 21 22:13:55 2011
 *
 *       GNU PDF Library - PDF tokeninser module
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

#ifndef PDF_TOKENISER_H
#define PDF_TOKENISER_H

#include <config.h>

#include <pdf-error.h>
#include <pdf-types.h>

/* Initialize Tokeniser module. Warning! Not thread-safe, must be used only once
 *  when the program starts. */
pdf_bool_t pdf_tokeniser_init (pdf_error_t **error);

/* Deinitialize Tokeniser module */
void pdf_tokeniser_deinit (void);

/* Get guessed decimal point */
const pdf_char_t *pdf_tokeniser_get_decimal_point (void);

#endif /* PDF_TOKENISER_H */

/* End of pdf-tokeniser.h */

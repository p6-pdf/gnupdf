/* -*- mode: C -*-
 *
 *       File:         pdf-filter-text.h
 *       Date:         Fri Jan 11 21:09:23 2008
 *
 *       GNU PDF Library - Encoded Text Filters
 *
 */

/* Copyright (C) 2008-2011 Free Software Foundation, Inc. */

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

#ifndef _PDF_TEXT_FILTER_H
#define _PDF_TEXT_FILTER_H

#include <pdf-types.h>
#include <pdf-error.h>
#include <pdf-text.h>

/* Make all text Upper Case */
pdf_bool_t pdf_text_filter_upper_case (pdf_text_t    text,
                                       pdf_error_t **error);

/* Make all text Lower Case */
pdf_bool_t pdf_text_filter_lower_case (pdf_text_t    text,
                                       pdf_error_t **error);

/* Make all text Title Case */
pdf_bool_t pdf_text_filter_title_case (pdf_text_t    text,
                                       pdf_error_t **error);

/* Remove all single ampersands, and turn '&&' into '&' */
pdf_bool_t pdf_text_filter_remove_amp (pdf_text_t    text,
                                       pdf_error_t **error);

/* Convert all ASCII code points to their Full-Width variants */
pdf_bool_t pdf_text_filter_normalize_full_width_ascii (pdf_text_t    text,
                                                       pdf_error_t **error);

/* Normalize all EOL sequences to the default host EOL */
pdf_bool_t pdf_text_filter_normalize_line_endings (pdf_text_t    text,
                                                   pdf_error_t **error);

/* Replace EOL sequences with white spaces */
pdf_bool_t pdf_text_filter_remove_line_endings (pdf_text_t    text,
                                                pdf_error_t **error);

#endif /* _PDF_TEXT_FILTER_H */

/* End of pdf-text-filter.h */

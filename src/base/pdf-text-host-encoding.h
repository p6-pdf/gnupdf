/* -*- mode: C -*-
 *
 *       File:         pdf-text-host-encoding.h
 *       Date:         Fri Jan 11 21:09:23 2008
 *
 *       GNU PDF Library - Encoded Text handling utilities - Host Encodings
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

#ifndef _PDF_TEXT_HOST_ENCODING_H
#define _PDF_TEXT_HOST_ENCODING_H

#include <pdf-text.h>

/* Function to convert from Host Encoding to UTF-32HE */
pdf_status_t
pdf_text_host_to_utf32he(const pdf_char_t      *input_data,
                         const pdf_size_t      input_length,
                         const pdf_text_host_encoding_t enc,
                         pdf_char_t            **p_output_data,
                         pdf_size_t            *p_output_length);


/* Function to convert from UTF-32HE to Host Encoding */
pdf_status_t
pdf_text_utf32he_to_host(const pdf_char_t      *input_data,
                         const pdf_size_t      input_length,
                         const pdf_text_host_encoding_t enc,
                         pdf_char_t            **p_output_data,
                         pdf_size_t            *p_output_length);


/* Check if a given Host Encoding is available in the system */
pdf_status_t
pdf_text_host_encoding_is_available(const pdf_char_t *encoding_name);



#endif /* _PDF_TEXT_HOST_ENCODING_H */

/* End of pdf-text-host-encoding.h */

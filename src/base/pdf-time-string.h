/* -*- mode: C -*-
 *
 *       File:         pdf-time-string.h
 *       Date:         Mon Jun 02 19:42:25 2008
 *
 *       GNU PDF Library - Header file for the Time module String utilities
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

#ifndef PDF_TIME_STRING_H
#define PDF_TIME_STRING_H

#include <config.h>

#include <stdio.h>

#include <pdf-time.h>


/* Set time object contents based on Date in PDF format */
pdf_status_t
pdf_time_from_string_pdf (pdf_time_t time_var,
                          const pdf_char_t *time_str,
                          pdf_bool_t allow_trailing_apostrophe);


/* Set time object contents based on Date in ASN1 format */
pdf_status_t
pdf_time_from_string_utc_asn1(pdf_time_t time_var,
                              const pdf_char_t *time_str);

/* Set time object contents based on Date in Generalized ASN1 format */
pdf_status_t
pdf_time_from_string_generalized_asn1(pdf_time_t time_var,
                                     const pdf_char_t *time_str);

/* Set time object contents based on Date in ISO-8601 format */
pdf_status_t
pdf_time_from_string_iso8601(pdf_time_t time_var,
                             const pdf_char_t *time_str);



/* Get Date as a string in PDF format */
pdf_char_t *
pdf_time_to_string_pdf (const pdf_time_t time_var,
                        pdf_bool_t include_trailing_apostrophe);


/* Get Date as a string in UTC-ASN1 format */
pdf_char_t *
pdf_time_to_string_utc_asn1(const pdf_time_t time_var);

/* Get Date as a string in Generalized ASN1 format */
pdf_char_t *
pdf_time_to_string_generalized_asn1(const pdf_time_t time_var);

/* Get Date as a string in ISO8601 format */
pdf_char_t *
pdf_time_to_string_iso8601(const pdf_time_t time_var);


#endif /* PDF_TIME_STRING_H */

/* End of pdf-time-string.h */

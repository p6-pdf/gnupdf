/* -*- mode: C -*-
 *
 *       File:         pdf-text-encoding.h
 *       Date:         Fri Jan 11 21:09:23 2008
 *
 *       GNU PDF Library - Encoded Text handling utilities - Encodings
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

#ifndef _PDF_TEXT_ENCODING_H
#define _PDF_TEXT_ENCODING_H

#include <pdf-types.h>
#include <pdf-error.h>
#include <pdf-text.h>

/* Specific type for UTF-32 character */
typedef union {
  pdf_u32_t i;
  pdf_uchar_t c[4];
} pdf_text_utf32_char_t;

/* Specific type for UTF-16 character (one or two characters of this type are
 *   needed for every UNICODE point) */
typedef union {
  pdf_u16_t i;
  pdf_uchar_t c[2];
} pdf_text_utf16_char_t;

/* Specific type to hold UNICODE Byte Order Marker (BOM) information */
typedef struct {
  const pdf_uchar_t bom_data[4];
  pdf_size_t bom_bytes;
} pdf_text_bom_t;

/* Define Language/Country code Initializer for UTF16BE PDF strings (U+001B).
 * Warning!!! This is a valid UTF-16BE code point!!! So it must only be
 * considered as language/country code initializer if coming from a PDF
 * String! */
#define PDF_TEXT_LCI_0  0x00
#define PDF_TEXT_LCI_1  0x1B

/* Static function to convert from PDF Doc Encoding to UTF-32HE, lossless */
pdf_bool_t pdf_text_pdfdocenc_to_utf32he (const pdf_char_t  *input_data,
                                          const pdf_size_t   input_length,
                                          pdf_char_t       **p_output_data,
                                          pdf_size_t        *p_output_length,
                                          pdf_error_t      **error);

/* Static function to convert from UTF-32HE to PDF Doc Encoding, with loss of
 information */
pdf_bool_t pdf_text_utf32he_to_pdfdocenc (const pdf_char_t  *input_data,
                                          const pdf_size_t   input_length,
                                          pdf_char_t       **p_output_data,
                                          pdf_size_t        *p_output_length,
                                          pdf_error_t      **error);

/* Function to convert from UTF32-HE to UTF32-HE, lossless */
pdf_bool_t pdf_text_utf32he_to_utf32he (const pdf_char_t  *input_data,
                                        const pdf_size_t   input_length,
                                        const pdf_bool_t   swap,
                                        const pdf_bool_t   check_input_he,
                                        const pdf_bool_t   check_output_he,
                                        pdf_char_t       **p_output_data,
                                        pdf_size_t        *p_output_length,
                                        pdf_error_t      **error);

/* Function to convert from UTF-32HE to UTF-32BE, lossless */
#define pdf_text_utf32he_to_utf32be(ind, inl, outd, outl, error)    \
  pdf_text_utf32he_to_utf32he (ind,                                 \
                               inl,                                 \
                               !PDF_IS_BIG_ENDIAN,                  \
                               PDF_TRUE,                            \
                               PDF_FALSE,                           \
                               outd,                                \
                               outl,                                \
                               error)


/* Function to convert from UTF-32BE to UTF-32HE, lossless */
#define pdf_text_utf32be_to_utf32he(ind, inl, outd, outl, error)    \
  pdf_text_utf32he_to_utf32he (ind,                                 \
                               inl,                                 \
                               !PDF_IS_BIG_ENDIAN,                  \
                               PDF_FALSE,                           \
                               PDF_TRUE,                            \
                               outd,                                \
                               outl,                                \
                               error)

/* Function to convert from UTF-32HE to UTF-32LE, lossless */
#define pdf_text_utf32he_to_utf32le(ind, inl, outd, outl, error)    \
  pdf_text_utf32he_to_utf32he (ind,                                 \
                               inl,                                 \
                               PDF_IS_BIG_ENDIAN,                   \
                               PDF_TRUE,                            \
                               PDF_FALSE,                           \
                               outd,                                \
                               outl,                                \
                               error)


/* Function to convert from UTF-32LE to UTF-32HE, lossless */
#define pdf_text_utf32le_to_utf32he(ind, inl, outd, outl, error)    \
  pdf_text_utf32he_to_utf32he (ind,                                 \
                               inl,                                 \
                               PDF_IS_BIG_ENDIAN,                   \
                               PDF_FALSE,                           \
                               PDF_TRUE,                            \
                               outd,                                \
                               outl,                                \
                               error)

/* Static function to convert from UTF16-HE to UTF32-HE, lossless */
pdf_bool_t pdf_text_utf16he_to_utf32he (const pdf_char_t  *input_data,
                                        const pdf_size_t   input_length,
                                        const pdf_bool_t   swap,
                                        pdf_char_t       **p_output_data,
                                        pdf_size_t        *p_output_length,
                                        pdf_char_t       **p_remaining_data,
                                        pdf_size_t        *p_remaining_length,
                                        pdf_error_t      **error);

/* Function to convert from UTF-16BE to UTF-32HE, lossless */
#define pdf_text_utf16be_to_utf32he(ind, inl, outd, outl, remd, reml, error) \
  pdf_text_utf16he_to_utf32he (ind,                                     \
                               inl,                                     \
                               !PDF_IS_BIG_ENDIAN,                      \
                               outd,                                    \
                               outl,                                    \
                               remd,                                    \
                               reml,                                    \
                               error)

/* Function to convert from UTF-16LE to UTF-32HE, lossless */
#define pdf_text_utf16le_to_utf32he(ind, inl, outd, outl, error)    \
  pdf_text_utf16he_to_utf32he (ind,                                 \
                               inl,                                 \
                               PDF_IS_BIG_ENDIAN,                   \
                               outd,                                \
                               outl,                                \
                               NULL,                                \
                               NULL,                                \
                               error)

/* Function to convert from UTF-32HE to UTF-16HE, lossless */
pdf_bool_t pdf_text_utf32he_to_utf16he (const pdf_char_t  *input_data,
                                        const pdf_size_t   input_length,
                                        pdf_char_t       **p_output_data,
                                        pdf_size_t        *p_output_length,
                                        pdf_bool_t         swap,
                                        pdf_error_t      **error);


/* Function to convert from UTF-32HE to UTF-16BE, lossless */
#define pdf_text_utf32he_to_utf16be(ind, inl, outd, outl, error)    \
  pdf_text_utf32he_to_utf16he (ind,                                 \
                               inl,                                 \
                               outd,                                \
                               outl,                                \
                               !PDF_IS_BIG_ENDIAN,                  \
                               error)

/* Function to convert from UTF-32HE to UTF-16LE, lossless */
#define pdf_text_utf32he_to_utf16le(ind, inl, outd, outl, error)    \
  pdf_text_utf32he_to_utf16he (ind,                                 \
                               inl,                                 \
                               outd,                                \
                               outl,                                \
                               PDF_IS_BIG_ENDIAN,                   \
                               error)


/* Function to convert from UTF-8 to UTF-32HE, lossless */
pdf_bool_t pdf_text_utf8_to_utf32he (const pdf_char_t  *input_data,
                                     const pdf_size_t   input_length,
                                     pdf_char_t       **p_output_data,
                                     pdf_size_t        *p_output_length,
                                     pdf_error_t      **error);


/* Function to convert from  UTF-32HE to UTF-8, lossless */
pdf_bool_t pdf_text_utf32he_to_utf8 (const pdf_char_t  *input_data,
                                     const pdf_size_t   input_length,
                                     pdf_char_t       **p_output_data,
                                     pdf_size_t        *p_output_length,
                                     pdf_error_t      **error);

/* Function to get the BOM of a given Unicode encoding */
pdf_text_bom_t pdf_text_get_unicode_bom (enum pdf_text_unicode_encoding_e unicode_encoding);

/* Function to check the presence of Unicode BOM in a given data array */
pdf_bool_t pdf_text_check_unicode_bom (const pdf_char_t *data,
                                       const pdf_size_t  size,
                                       enum pdf_text_unicode_encoding_e enc,
                                       int               swap);

#endif /* _PDF_TEXT_ENCODING_H */

/* End of pdf-text-encoding.h */

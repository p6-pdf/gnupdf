/* -*- mode: C -*-
 *
 *       File:         pdf-text.h
 *       Date:         Fri Jan 11 21:09:23 2008
 *
 *       GNU PDF Library - Encoded Text handling utilities
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

#ifndef _PDF_TEXT_H
#define _PDF_TEXT_H

#include <stdint.h>
#include <stdio.h>

#include <pdf-alloc.h>
#include <pdf-error.h>
#include <pdf-list.h>


/* --------------------- Text Module initialization ------------------------- */

/* Initialize Text Module. Warning! Not thread-safe, must be used only once when
 *  the program starts. It will be in charge of detecting currently configured
 *  locale, host encoding... in GNU/Linux and UNIX-like systems
 *  (Mac OS X included) */
pdf_status_t
pdf_text_init(void);



/* BEGIN PUBLIC */


/* -------------------------- Text Data Types ------------------------------- */


/* Enumeration of supported UNICODE encodings */
enum pdf_text_unicode_encoding_e {
  PDF_TEXT_HE_TO_BE = -2, /* Delta to convert HE to BE */
  PDF_TEXT_HE_TO_LE = -1, /* Delta to convert HE to LE */
  PDF_TEXT_UTF8     =  0, /* UTF-8 encoding */
  PDF_TEXT_UTF16_BE,      /* Big Endian UTF-16 encoding */
  PDF_TEXT_UTF16_LE,      /* Little Endian UTF-16 encoding */
  PDF_TEXT_UTF16_HE,      /* Host Endian UTF-16 encoding (LE or BE) */
  PDF_TEXT_UTF32_BE,      /* Big Endian UTF-32 encoding */
  PDF_TEXT_UTF32_LE,      /* Little Endian UTF-32 encoding */
  PDF_TEXT_UTF32_HE,      /* Host Endian UTF-32 encoding (LE or BE) */
  PDF_TEXT_MAX_UNICODE_ENC
};


/* Enumeration of possible options when getting data in UNICODE format.
 *  Note that some options are only applicable to some UNICODE formats.
 *  Each of these enumerations is a Mask of Bits, so that multiple options
 *  can be set at the same time */
enum pdf_text_unicode_options_e {
  PDF_TEXT_UNICODE_NO_OPTION       = 0x00,
  PDF_TEXT_UNICODE_WITH_BOM        = 0x01, /* UTF-8, UTF-16(L/B)E, UTF-32(L/B)E */
  PDF_TEXT_UTF16BE_WITH_LANGCODE   = 0x02, /* UTF16BE */
  PDF_TEXT_UNICODE_WITH_NUL_SUFFIX = 0x04  /* UTF-8, UTF-16(L/B)E, UTF-32(L/B)E */
};


/* Enumeration of supported filters in encoded text objects.
 *  Each of these enumerations is a Mask of Bits, so that multiple options
 *  can be set at the same time */
enum pdf_text_filter_type_e {
  PDF_TEXT_FILTER_IDENTITY              = 0x00, /* No-op */
  PDF_TEXT_FILTER_LINE_ENDINGS          = 0x01, /* Normalize line endings */
  PDF_TEXT_FILTER_UPPER_CASE            = 0x02, /* Make all text uppercase */
  PDF_TEXT_FILTER_LOWER_CASE            = 0x04, /* Make all text lowercase */
  PDF_TEXT_FILTER_TITLE_CASE            = 0x08, /* Make all text titlecase */
  PDF_TEXT_FILTER_REMOVE_AMP            = 0x10, /* Remove all single & */
  PDF_TEXT_FILTER_NORM_WITH_FULL_WIDTH  = 0x20, /* Normalize with full width */
  PDF_TEXT_FILTER_REMOVE_LINE_ENDINGS   = 0x40  /* Replace line endings */
};


/* And reference to the structure (basic type to be used) */
typedef struct pdf_text_s *pdf_text_t;


/* Host Encoding Name Maximum Length */
#define PDF_TEXT_HENMAXL   32
/* Define structure to hold host encoding information. Probably a structure is
 *  the best option for this type, so that it's easy to update it in the future
 *  if needed */
typedef struct pdf_text_host_encoding_s {
  pdf_char_t name[PDF_TEXT_HENMAXL];
} pdf_text_host_encoding_t;


/* --------------------- Text Creation and Destruction ---------------------- */

/* Create new empty text object. Returned type must be checked against NULL. */
pdf_status_t
pdf_text_new (pdf_text_t *text);


/* Destroy (freeing contents) a given text object */
pdf_status_t
pdf_text_destroy (pdf_text_t text);


/* Duplicate text object. Returned type must be checked against NULL.  */
pdf_text_t
pdf_text_dup (const pdf_text_t text);


/* Create new text object from a Host-Encoded string. Status of the conversion
 *  is returned. */
pdf_status_t
pdf_text_new_from_host (const pdf_char_t *str,
                        const pdf_size_t size,
                        const pdf_text_host_encoding_t enc,
                        pdf_text_t *text);


/* Create new text object from a PDF string: either PDFDoc-Encoded string or
 *  UTF-16BE encoded string (BOM mandatory). Status of the conversion is
 *  returned. */
pdf_status_t
pdf_text_new_from_pdf_string (const pdf_char_t *str,
                              const pdf_size_t size,
                              pdf_char_t **remaining_str,
                              pdf_size_t *remaining_length,
                              pdf_text_t *text);


/* Create a new text object from a string of Unicode characters in a given
 *  unicode encoding. Status of the conversion is returned. */
pdf_status_t
pdf_text_new_from_unicode (const pdf_char_t *str,
                           const pdf_size_t size,
                           const enum pdf_text_unicode_encoding_e enc,
                           pdf_text_t *text);


/* Create a new text variable containing the textual representation of a
 *  given integer. Status of the conversion is returned. */
pdf_status_t
pdf_text_new_from_u32 (const pdf_u32_t number,
                       pdf_text_t *text);


/* ------------------------ Text Property Management ------------------------ */

/* Return the country associated with a text variable */
extern INLINE const pdf_char_t *
pdf_text_get_country (const pdf_text_t text);

/* Return the language associated with a text variable */
extern INLINE const pdf_char_t *
pdf_text_get_language (const pdf_text_t text);

/* Associate a text variable (full text) with a country code */
pdf_status_t
pdf_text_set_country (pdf_text_t text,
                      const pdf_char_t *code);

/* Associate a text variable (full text) with a language code */
pdf_status_t
pdf_text_set_language (pdf_text_t text,
                       const pdf_char_t *code);

/* Determine if a given text variable is empty (contains no text) */
pdf_bool_t
pdf_text_empty_p (const pdf_text_t text);

/* Replace a given set of old patterns with a single new pattern */
pdf_status_t
pdf_text_replace_multiple (pdf_text_t text,
                           const pdf_text_t new_pattern,
                           const pdf_text_t *p_old_patterns,
                           const int n_old_patterns);



/* ------------------------ Host Encoding Management ------------------------ */


/* Get Host Encoding configured by the user/system */
pdf_text_host_encoding_t
pdf_text_get_host_encoding(void);


/* Check if a given encoding is available in the system, and return a
 *  pdf_text_host_encoding_t variable with the host encoding ID if available*/
pdf_status_t
pdf_text_check_host_encoding(const pdf_char_t *encoding_name,
                             pdf_text_host_encoding_t *p_encoding);


/* Get Best Host Encoding to encode the given text object */
pdf_text_host_encoding_t
pdf_text_get_best_encoding (const pdf_text_t text,
                            const pdf_text_host_encoding_t preferred_encoding);



/* ------------------------- Text Content Management ------------------------ */

/* Get the contents of a text variable encoded in a given host encoding */
pdf_status_t
pdf_text_get_host (pdf_char_t **contents,
                   pdf_size_t *length,
                   const pdf_text_t text,
                   const pdf_text_host_encoding_t enc);


/* Get the contents of a text variable encoded in PDFDocEncoding, as a NUL
 *  terminated string */
pdf_status_t
pdf_text_get_pdfdocenc (pdf_char_t **contents,
                        const pdf_text_t text);


/* Get the contents of a text variable encoded in the given UNICODE encoding.
 *  In addition to the specific encoding required, extra options can be set,
 *  in the following way: OPTION1 | OPTION2 | OPTION3 */
pdf_status_t
pdf_text_get_unicode (pdf_char_t **contents,
                      pdf_size_t *length,
                      const pdf_text_t text,
                      const enum pdf_text_unicode_encoding_e enc,
                      const pdf_u32_t options);

/* Get the contents of a text variable in hexadecimal representation as a NUL
 *  terminated string. */
pdf_char_t *
pdf_text_get_hex (const pdf_text_t text,
                  const pdf_char_t delimiter);



/* Set a string encoded with some host encoding. */
pdf_status_t
pdf_text_set_host (pdf_text_t text,
                   const pdf_char_t *str,
                   const pdf_size_t size,
                   const pdf_text_host_encoding_t enc);


/* Set a PDFDocEncoding encoded string. Input data MUST be NUL terminated. */
pdf_status_t
pdf_text_set_pdfdocenc (pdf_text_t text,
                        const pdf_char_t *str);


/* Set a Unicode encoded string. */
pdf_status_t
pdf_text_set_unicode (pdf_text_t text,
                      const pdf_char_t *str,
                      const pdf_size_t size,
                      const enum pdf_text_unicode_encoding_e enc);


/* Concatenate the contents of two text variables. Warning!! Country/Language
 *  information MUST be the same (or inexistent in both elements) if
 *  override_langinfo is NOT set. If override_langinfo is set, the language and
 *  country information of text1 will remain unchanged, whatever the language
 *  and country information of text2 is. */
pdf_status_t
pdf_text_concat (pdf_text_t text1,
                 const pdf_text_t text2,
                 const pdf_bool_t override_langinfo);

/* Concatenate a text variable with an ascii string */
pdf_status_t
pdf_text_concat_ascii (pdf_text_t text1,
                       const pdf_char_t * ascii_str);

/* Replace a fixed pattern in the content of a given text variable */
pdf_status_t
pdf_text_replace (pdf_text_t text,
                  const pdf_text_t new_pattern,
                  const pdf_text_t old_pattern);


/* Replace a fixed ASCII pattern in the content of a given text variable */
pdf_status_t
pdf_text_replace_ascii (pdf_text_t text,
                        const pdf_char_t *new_pattern,
                        const pdf_char_t *old_pattern);

/* Filter the contents of a text variable */
pdf_status_t
pdf_text_filter (pdf_text_t text,
                 const pdf_u32_t filter);

/* Returns a read-only string to be used with printf's "%s" like format */
const pdf_char_t *
pdf_text_get_printable (pdf_text_t text);

/* ------------------------- Text Comparison -------------------------------- */

/* Compare the contents (character data) of two text variables.
 *  Returns  1 if text1 != text2
 *  Returns  0 if text1 = text2
 */
pdf_i32_t
pdf_text_cmp (const pdf_text_t text1,
              const pdf_text_t text2,
              const pdf_bool_t case_sensitive,
              pdf_status_t *p_ret_code);

/* END PUBLIC */

/* Common Code Length for both country and language codes
 *  (including last NUL char) */
#define PDF_TEXT_CCL     3

/* Structure defining the basic text type. Contents are always stored internally
 *  in UNICODE UTF32-HE encoding */
struct pdf_text_s {
  pdf_char_t *data;                 /* Contents in UTF-32HE */
  pdf_size_t size;                  /* Number of Bytes */
  pdf_list_t *word_boundaries;       /* List of Word boundaries (optional) */
  pdf_char_t lang[PDF_TEXT_CCL];    /* Associated language code (optional) */
  pdf_char_t country[PDF_TEXT_CCL]; /* Associated country code (optional) */
  pdf_bool_t modified;  /* set to PDF_TRUE each time the data is modified */
  pdf_char_t *printable;
};

/* Structure containing the word boundary information */
struct pdf_text_wb_s {
  /* Pointer to the FIRST byte of the first UTF-32 code point */
  pdf_char_t *word_start;
  /* Pointer to the FIRST byte of the last UTF-32 code point */
  pdf_char_t *word_stop;
  /* Length of the word, in BYTES */
  pdf_size_t word_size;
};


/* Returns PDF_TRUE is UTF-8 data is ASCII-7 */
pdf_bool_t
pdf_text_is_ascii7(const pdf_char_t *utf8data, const pdf_size_t size);


/* Clean contents of the text element */
void
pdf_text_clean_contents(pdf_text_t text);


/* Generate Word Boundaries list from text object (if not already done) */
pdf_bool_t pdf_text_generate_word_boundaries (pdf_text_t    text,
                                              pdf_error_t **error);
/* Create empty Word Boundaries list */
pdf_list_t *pdf_text_create_word_boundaries_list (pdf_error_t **error);
/* Destroy Word Boundaries list */
pdf_bool_t pdf_text_destroy_word_boundaries_list (pdf_list_t  **p_word_boundaries,
                                                  pdf_error_t **error);



#endif /* _PDF_TEXT_H */

/* End of pdf-text.h */

/* -*- mode: C -*-
 *
 *       File:         pdf-text.c
 *       Date:         Fri Jan 11 21:09:56 2008
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

#include <config.h>

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif /* HAVE_MALLOC_H */
#include <xalloc.h>
#include <math.h>

#include <pdf-text.h>
#include <pdf-text-encoding.h>
#include <pdf-text-host-encoding.h>
#include <pdf-text-context.h>
#include <pdf-text-filter.h>
#include <pdf-text-ucd.h>


/* Lang/Country Minimum Length, in bytes, of the Lang/Country information within
 *  a UTF16BEstring (2bytes for the first marker, 2 bytes for LANG and 2 bytes
 *  for the last marker). */
#define PDF_TEXT_LCMINL     6

/* Maximum size, in bytes, of the Lang/Country information within a UTF16BE
 *  string (Minimum size + 2 bytes for COUNTRY). */
#define PDF_TEXT_LCMAXL     8


/* Longest header length when requesting a unicode string with options is that
 * of UTF-16BE with BOM and lang/country information: 2bytes-BOM +
 * 8bytes-lang/country = 10 bytes (+ 1 NUL byte) */
#define PDF_TEXT_USHMAXL    11

typedef struct pdf_text_repl_s {
  pdf_char_t *data_ptr;
  int old_pattern_i;
} pdf_text_repl_t;

/* ---------------- Static (private) functions prototypes ------------------- */

/* This function receives as input a valid pdf_text_t element, where the
 *  language and country code informations will be stored. In addition to this,
 *  the function receives as input the data string (starting in the first
 *  marker), and stores a pointer to the continuation of the data string, after
 *  having read the language/country information. This function really assumes
 *  that the input data string contains in the first bytes the country/lang
 *  information.
 * Two options are possible:
 *   XXllXX   (6 bytes, XX is the marker, ll the language)
 *   XXllccXX (8 bytes, XX is the marker, ll the language and cc the country)
 */
static pdf_bool_t
pdf_text_get_lang_from_utf16be (pdf_text_t        *element,
                                const pdf_char_t  *str_in,
                                const pdf_size_t   str_in_length,
                                pdf_char_t       **str_out,
                                pdf_size_t        *str_out_length,
                                pdf_error_t      **error);

/* Function to get the header of a unicode string as requested in the
 * `options' field when calling `pdf_text_get_unicode'. The header can be:
 *  - BOM
 *  - BOM + Lang/Country info (only if UTF-16BE requested)
 *  - Lang/Country info (only if UTF-16BE requested)
 */
static void
pdf_text_get_unicode_string_header (enum pdf_text_unicode_encoding_e enc,
                                    pdf_u32_t         options,
                                    const pdf_char_t *language,
                                    const pdf_char_t *country,
                                    pdf_char_t        header[PDF_TEXT_USHMAXL],
                                    pdf_size_t       *header_length);

/* Function to convert a given Unicode Host Endian enumeration to the `real'
 *  endianness (BE or LE). If a non-HE enumeration is passed to the function,
 *  it will return the same enumeration value unchanged */
static enum pdf_text_unicode_encoding_e
pdf_text_transform_he_to_unicode_encoding (enum pdf_text_unicode_encoding_e enc);

/* Function to compare two given words */
static pdf_i32_t pdf_text_compare_words (const pdf_char_t  *word1,
                                         const pdf_size_t   size1,
                                         const pdf_char_t  *word2,
                                         const pdf_size_t   size2,
                                         const pdf_char_t  *language1,
                                         const pdf_char_t  *language2,
                                         pdf_error_t      **error);

/* Non-Case sensitive comparison of text objects */
static pdf_i32_t pdf_text_cmp_non_case_sensitive (const pdf_text_t   *text1,
                                                  const pdf_text_t   *text2,
                                                  pdf_error_t       **error);

/* Clean (destroy and create empty) Word Boundaries list */
static pdf_bool_t pdf_text_clean_word_boundaries_list (pdf_list_t  **p_word_boundaries,
                                                       pdf_error_t **error);
/* Fill in the Word Boundaries list using the given data */
static pdf_bool_t pdf_text_fill_word_boundaries_list (pdf_list_t        *word_boundaries,
                                                      const pdf_char_t  *data,
                                                      const pdf_size_t   size,
                                                      pdf_error_t      **error);

/* ----------------------------- Public functions ----------------------------*/

pdf_bool_t
pdf_text_init (pdf_error_t **error)
{
  /* Initiate Text module context */
  return pdf_text_context_init (error);
}

void
pdf_text_deinit (void)
{
  /* Deinit Text module context */
  if (pdf_text_context_initialized ())
    pdf_text_context_deinit ();
}

pdf_text_t *
pdf_text_new (pdf_error_t **error)
{
  pdf_text_t *text;

  /* The text global state should be initialized! */
  if (!pdf_text_context_initialized ())
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TEXT,
                     PDF_EBADCONTEXT,
                     "cannot create new text object: "
                     "context not initialized");
      return NULL;
    }

  /* Allocate memory for the new text structure */
  text = (pdf_text_t *) pdf_alloc (sizeof (struct pdf_text_s));
  if (!text)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TEXT,
                     PDF_ENOMEM,
                     "cannot create new text object: "
                     "couldn't allocate %lu bytes",
                     (unsigned long)sizeof (struct pdf_text_s));
      /* Out of memory condition */
      return NULL;
    }

  /* Initialize all contents */
  text->data = NULL;
  text->size = 0;
  text->printable = NULL;
  text->modified = PDF_FALSE;
  memset (&(text->lang[0]), 0, PDF_TEXT_CCL);
  memset (&(text->country[0]), 0, PDF_TEXT_CCL);

  /* Create empty word boundaries list */
  text->word_boundaries = pdf_text_create_word_boundaries_list (error);
  if (!text->word_boundaries)
    {
      pdf_text_destroy (text);
      return NULL;
    }

  /* Success! */
  return text;
}

void
pdf_text_destroy (pdf_text_t *text)
{
  if (!text)
    return;

  /* Dealloc memory */
  if (text->data != NULL)
    pdf_dealloc (text->data);

  if (text->printable != NULL)
    pdf_dealloc (text->printable);

  /* Destroy word boundaries list */
  pdf_text_destroy_word_boundaries_list (&text->word_boundaries);

  pdf_dealloc (text);
}

pdf_text_t *
pdf_text_dup (const pdf_text_t  *text,
              pdf_error_t      **error)
{
  pdf_text_t *element;

  PDF_ASSERT_POINTER_RETURN_VAL (text, NULL);

  /* Allocate and initialize element */
  element = pdf_text_new (error);
  if (!element)
    return NULL;

  /* Duplicate size */
  element->size = text->size;

  /* Duplicate contents (if size > 0) */
  if (element->size > 0)
    {
      element->data = (pdf_char_t *) pdf_alloc (element->size);
      if (!element->data)
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_TEXT,
                         PDF_ENOMEM,
                         "cannot duplicate text object: "
                         "couldn't allocate %lu bytes",
                         (unsigned long)element->size);
          return NULL;
        }

      memcpy (element->data, text->data, (size_t)element->size);
    }

  /* Duplicate Language code and Country code (if available) */
  memcpy (element->lang, text->lang, (size_t) PDF_TEXT_CCL);
  memcpy (element->country, text->country, (size_t) PDF_TEXT_CCL);

  /* We don't really need to duplicate the contents of the word
   *  boundaries list, as it is a side product, same with printable */

  return element;
}

pdf_text_t *
pdf_text_new_from_host (const pdf_char_t  *str,
                        const pdf_size_t   size,
                        const pdf_char_t  *enc,
                        pdf_error_t      **error)
{
  pdf_text_t *element;

  PDF_ASSERT_POINTER_RETURN_VAL (str, NULL);
  PDF_ASSERT_RETURN_VAL (size > 0, NULL);
  PDF_ASSERT_POINTER_RETURN_VAL (enc, NULL);

  /* Allocate and initialize element */
  element = pdf_text_new (error);
  if (!element)
    return NULL;

  /* Set Host Encoding contents */
  if (!pdf_text_set_host (element, enc, str, size, error))
    {
      /* Conversion went wrong... so destroy object contents */
      pdf_text_destroy (element);
      return NULL;
    }

  return element;
}

pdf_text_t *
pdf_text_new_from_pdf_string (const pdf_char_t  *str,
                              const pdf_size_t   size,
                              pdf_char_t       **remaining_str,
                              pdf_size_t        *remaining_length,
                              pdf_error_t      **error)
{
  pdf_text_t *element;
  pdf_bool_t bom_found = PDF_FALSE;
  pdf_bool_t lang_found = PDF_FALSE;

  PDF_ASSERT_POINTER_RETURN_VAL (str, NULL);
  PDF_ASSERT_RETURN_VAL (size > 0, NULL);

  /* Allocate and initialize element */
  element = pdf_text_new (error);
  if (!element)
    return NULL;

  /* First of all, check first two bytes to detect UTF-16BE BOM or lang/country
   *  code initializer.
   *  If length of the text is less than 2, then we can assume it is encoded in
   *  PDF Doc Encoding */
  if (size >= 2)
    {
      /* Check Unicode Byte Order Marker encoded in UTF-16BE */
      if (pdf_text_check_unicode_bom (str, size, PDF_TEXT_UTF16_BE, 0))
        {
          bom_found = PDF_TRUE;
          /* Check Lang/Country Code initializer */
          if ((size >= 4) &&
              (str[3] == PDF_TEXT_LCI_1) &&
              (str[2] == PDF_TEXT_LCI_0))
            {
              lang_found = PDF_TRUE;
            }
        }
      /* Check Lang/Country Code initializer (if this is the nth call to the
       *  function parsing a single UTF-16BE string.*/
      else if ((str[1] == PDF_TEXT_LCI_1) &&
               (str[0] == PDF_TEXT_LCI_0))
        {
          lang_found = PDF_TRUE;
        }
    }

  /* If either BOM or Lang Marker are found, process PDF string as encoded
   *  in UTF16-BE */
  if (bom_found || lang_found)
    {
      pdf_char_t *string_start = (pdf_char_t *)str;
      pdf_size_t string_length = size;

      /* Skip 2-bytes BOM */
      if (bom_found)
        {
          string_start += 2;
          string_length -= 2;
        }

      /* If lang/country code available, obtain and store the information */
      if ((lang_found) &&
          (!pdf_text_get_lang_from_utf16be (element,
                                            string_start,
                                            string_length,
                                            &string_start,
                                            &string_length,
                                            error)))
        {
          pdf_text_destroy (element);
          return NULL;
        }

      /* And finally convert to UTF-32... */
      if (!pdf_text_utf16be_to_utf32he (string_start,
                                        string_length,
                                        &(element->data),
                                        &(element->size),
                                        remaining_str,
                                        remaining_length,
                                        error))
        {
          pdf_text_destroy (element);
          return NULL;
        }

      /* Return newly created element */
      return element;
    }

  /* Else, process PDF string as encoded in PDF Doc Encoding */

  /* We already know that this string will be fully stored, without
   *  splitting in chunks */
  if (remaining_length != NULL)
    *remaining_length = 0;
  if (remaining_str != NULL)
    *remaining_str = NULL;

  /* And perform the conversion */
  if (!pdf_text_pdfdocenc_to_utf32he (str,
                                      size,
                                      &(element->data),
                                      &(element->size),
                                      error))
    {
      pdf_text_destroy (element);
      return NULL;
    }

  return element;
}

pdf_text_t *
pdf_text_new_from_unicode (const pdf_char_t  *str,
                           const pdf_size_t   size,
                           const enum pdf_text_unicode_encoding_e enc,
                           pdf_error_t      **error)
{
  pdf_text_t *element;

  PDF_ASSERT_POINTER_RETURN_VAL (str, NULL);
  PDF_ASSERT_RETURN_VAL (size > 0, NULL);

  /* Allocate and initialize element */
  element = pdf_text_new (error);
  if (!element)
    return NULL;

  /* Set Unicode contents */
  if (!pdf_text_set_unicode (element, str, size, enc, error))
    {
      pdf_text_destroy (element);
      return NULL;
    }

  return element;
}

pdf_text_t *
pdf_text_new_from_u32 (const pdf_u32_t   number,
                       pdf_error_t     **error)
{
  /* Longest number to hold in 32bit: 2^32 = 4294967296 (10 chars) */
  pdf_char_t temp[10 + 1];
  int n;

  /* Print number in temporal char array, and get number of output chars */
  n = sprintf (&temp[0], "%u", (unsigned int)number);

  /* Treat the generated string as UTF-8 encoded (just numbers in ASCII) */
  return pdf_text_new_from_unicode (&temp[0], n, PDF_TEXT_UTF8, error);
}

/* Return the country associated with a text variable */
const pdf_char_t *
pdf_text_get_country (const pdf_text_t *text)
{
  PDF_ASSERT_POINTER_RETURN_VAL (text, NULL);

  return (const pdf_char_t *)text->country;
}

/* Return the language associated with a text variable */
const pdf_char_t *
pdf_text_get_language (const pdf_text_t *text)
{
  PDF_ASSERT_POINTER_RETURN_VAL (text, NULL);

  return (const pdf_char_t *)text->lang;
}

/* Associate a text variable (full text) with a country code */
void
pdf_text_set_country (pdf_text_t       *text,
                      const pdf_char_t *code)
{
  PDF_ASSERT_POINTER_RETURN (text);
  PDF_ASSERT_POINTER_RETURN (code);
  PDF_ASSERT_RETURN (strlen(code) == (PDF_TEXT_CCL - 1));

  memcpy (&(text->country[0]), code, PDF_TEXT_CCL - 1);
  /* Make sure that last byte is NUL */
  text->country[PDF_TEXT_CCL - 1] = '\0';
}

/* Associate a text variable (full text) with a language code */
void
pdf_text_set_language (pdf_text_t       *text,
                       const pdf_char_t *code)
{
  PDF_ASSERT_POINTER_RETURN (text);
  PDF_ASSERT_POINTER_RETURN (code);
  PDF_ASSERT_RETURN (strlen(code) == (PDF_TEXT_CCL - 1));

  memcpy (&(text->lang[0]), code, PDF_TEXT_CCL - 1);
  /* Make sure that last byte is NUL */
  text->lang[PDF_TEXT_CCL - 1] = '\0';
}

/* Determine if a given text variable is empty (contains no text) */
pdf_bool_t
pdf_text_empty_p (const pdf_text_t *text)
{
  PDF_ASSERT_POINTER_RETURN_VAL (text, PDF_TRUE);

  return ((text->size != 0) ? PDF_FALSE : PDF_TRUE);
}

/* Get default system host encoding */
const pdf_char_t *
pdf_text_get_host_encoding (void)
{
  return pdf_text_context_get_host_encoding ();
}

/* Check if host encoding is available */
pdf_bool_t
pdf_text_check_host_encoding (const pdf_char_t  *encoding_name,
                              pdf_error_t      **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (encoding_name, PDF_FALSE);

  return pdf_text_host_encoding_is_available (encoding_name,
                                              error);
}

const pdf_char_t *
pdf_text_get_best_encoding (const pdf_text_t *text,
                            const pdf_char_t *preferred_encoding)
{
  int i;
#ifdef PDF_HOST_WIN32
  static const pdf_char_t *to_check [3] = {
    (pdf_char_t *) "CP1200",  /* UTF-16LE */
    (pdf_char_t *) "CP65001", /* UTF-8 */
    (pdf_char_t *) "CP12000"  /* UTF-32LE */
  };
#else
  static const pdf_char_t *to_check [3] = {
    (pdf_char_t *) "UTF-8",
    (pdf_char_t *) "UTF-16",
    (pdf_char_t *) "UTF-32"
  };
#endif

  PDF_ASSERT_POINTER_RETURN_VAL (text, NULL);

  /* Check for Unicode support as host encoding */
  for (i = 0; i < 3; i++)
    {
      if (pdf_text_check_host_encoding (to_check[i], NULL))
        return to_check[i];
    }

  /* If host does not support any Unicode encoding conversion, return the
   *  preferred one directly */
  return preferred_encoding;
}

pdf_char_t *
pdf_text_get_host (const pdf_text_t  *text,
                   const pdf_char_t  *enc,
                   pdf_size_t        *length,
                   pdf_error_t      **error)
{
  pdf_char_t *contents;

  PDF_ASSERT_POINTER_RETURN_VAL (text, NULL);
  PDF_ASSERT_POINTER_RETURN_VAL (enc, NULL);
  /* You should really get and check the length of the returned string */
  PDF_ASSERT_POINTER_RETURN_VAL (length, NULL);

  return (pdf_text_utf32he_to_host (text->data,
                                    text->size,
                                    enc,
                                    &contents,
                                    length,
                                    error) ?
          contents : NULL);
}

/* Get the contents of a text variable encoded in PDFDocEncoding, as a NUL
 *  terminated string */
pdf_char_t *
pdf_text_get_pdfdocenc (const pdf_text_t  *text,
                        pdf_error_t      **error)
{
  pdf_char_t *data = NULL;
  pdf_char_t *resized;
  pdf_size_t size = -1;

  PDF_ASSERT_POINTER_RETURN_VAL (text, NULL);

  if (!pdf_text_utf32he_to_pdfdocenc (text->data,
                                      text->size,
                                      &data,
                                      &size,
                                      error))
    {
      return NULL;
    }

  /* Add NUL character at the end of the array */
  resized = pdf_realloc (data, size + 1);
  if (!resized)
    {
      pdf_dealloc (data);
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TEXT,
                     PDF_ENOMEM,
                     "cannot get text contents PDF Doc encoded: "
                     "couldn't reallocate %lu bytes",
                     (unsigned long)(size + 1));
      return NULL;
    }

  resized[size] = '\0';

  return resized;
}

pdf_char_t *
pdf_text_get_unicode (const pdf_text_t  *text,
                      enum pdf_text_unicode_encoding_e enc,
                      const pdf_u32_t    options,
                      pdf_size_t        *length,
                      pdf_error_t      **error)
{
  pdf_char_t *out_data = NULL;
  pdf_size_t out_length = 0;
  pdf_bool_t ret;

  PDF_ASSERT_POINTER_RETURN_VAL (text, NULL);
  /* Lang/Country info only available for UTF-16BE */
  PDF_ASSERT_RETURN_VAL (!(options & PDF_TEXT_UTF16BE_WITH_LANGCODE) ||
                         (enc != PDF_TEXT_UTF16_BE),
                         NULL);

  /* If host endianness required, check it and convert input encoding */
  enc = pdf_text_transform_he_to_unicode_encoding (enc);

  /* If text is empty, set empty string */
  if ((text->data == NULL) ||
      (text->size == 0))
    {
      *length = 0;
      return NULL;
    }

  /* Perform conversion */
  switch (enc)
    {
    case PDF_TEXT_UTF8: /* UTF-8 */
      ret = pdf_text_utf32he_to_utf8 (text->data,
                                      text->size,
                                      &out_data,
                                      &out_length,
                                      error);
      break;
    case PDF_TEXT_UTF16_LE: /* UTF-16LE */
      ret = pdf_text_utf32he_to_utf16le (text->data,
                                         text->size,
                                         &out_data,
                                         &out_length,
                                         error);
      break;
    case PDF_TEXT_UTF16_BE: /* UTF-16BE */
      ret = pdf_text_utf32he_to_utf16be (text->data,
                                         text->size,
                                         &out_data,
                                         &out_length,
                                         error);
      break;
    case PDF_TEXT_UTF32_LE: /* UTF-32LE */
      ret = pdf_text_utf32he_to_utf32le (text->data,
                                         text->size,
                                         &out_data,
                                         &out_length,
                                         error);
      break;
    case PDF_TEXT_UTF32_BE: /* UTF-32BE */
      ret = pdf_text_utf32he_to_utf32be (text->data,
                                         text->size,
                                         &out_data,
                                         &out_length,
                                         error);
      break;
    default:
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TEXT,
                     PDF_ETEXTENC,
                     "couldn't get text contents in the given unicode "
                     "encoding (%d)",
                     enc);
      ret = PDF_FALSE;
      break;
    }

  if (!ret)
    {
      if (out_data)
        pdf_dealloc (out_data);
      return NULL;
    }

  /* Check if specific options were requested */
  if (options != PDF_TEXT_UNICODE_NO_OPTION)
    {
      pdf_char_t header[PDF_TEXT_USHMAXL];
      pdf_size_t header_size = 0;
      pdf_size_t trailer_size = 0;

      /* Compute header if needed */
      if ((options & PDF_TEXT_UNICODE_WITH_BOM) ||
          (options & PDF_TEXT_UTF16BE_WITH_LANGCODE))
        {
          /* Clear header array */
          memset (&(header[0]), 0, PDF_TEXT_USHMAXL);
          /* Get requested header (BOM and/or lang/country info) */
          pdf_text_get_unicode_string_header (enc,
                                              options,
                                              pdf_text_get_language(text),
                                              pdf_text_get_country(text),
                                              header,
                                              &header_size);
        }

      /* Compute trailer if needed */
      if (options & PDF_TEXT_UNICODE_WITH_NUL_SUFFIX)
        {
          switch (enc)
            {
            case PDF_TEXT_UTF8:
              trailer_size = 1;
              break;
            case PDF_TEXT_UTF16_BE:
            case PDF_TEXT_UTF16_LE:
            case PDF_TEXT_UTF16_HE:
              trailer_size = 2;
              break;
            case PDF_TEXT_UTF32_BE:
            case PDF_TEXT_UTF32_LE:
            case PDF_TEXT_UTF32_HE:
              trailer_size = 4;
              break;
            default:
              trailer_size = 0;
              break;
            }
        }

      if ((header_size > 0) ||
          (trailer_size > 0))
        {
          pdf_char_t *new_out_data;
          pdf_size_t new_size;

          /* Allocate memory for new string */
          new_size = out_length + header_size + trailer_size;
          new_out_data = (pdf_char_t *) pdf_alloc (new_size);
          if (!new_out_data)
            {
              pdf_set_error (error,
                             PDF_EDOMAIN_BASE_TEXT,
                             PDF_ENOMEM,
                             "couldn't get text contents in the given unicode "
                             "encoding: "
                             "couldn't reallocate %lu bytes",
                             (unsigned long)new_size);
              return NULL;
            }

          /* Store header */
          memcpy (new_out_data, &header[0], header_size);

          if ((out_data != NULL) &&
              (out_length != 0))
            {
              /* Store unicode data, if any */
              memcpy (&new_out_data[header_size], out_data, out_length);
              /* Reset output data array, if any */
              pdf_dealloc (out_data);
            }

          /* Store trailer (N-byte NUL) */
          if (trailer_size > 0)
            memset (&new_out_data[out_length+header_size], 0, trailer_size);

          out_data = new_out_data;
          out_length += (header_size + trailer_size);
        }
    }

  *length = out_length;
  return out_data;
}

pdf_char_t *
pdf_text_get_hex (const pdf_text_t *text,
                  const pdf_char_t  delimiter,
                  pdf_error_t      **error)
{
  pdf_char_t *new_str;

  if (text->size > 0)
    {
      int i;
      int j;
      unsigned int new_str_length;
      char new_hex_char [3];

      /* Get new string length. If input string has N bytes, we need:
       * - 1 byte for last NUL char
       * - 2N bytes for hexadecimal char representation of each byte...
       * - N-1 bytes for the separator ':'
       * So... a total of (1+2N+N-1) = 3N bytes are needed... */
      new_str_length = 3 * text->size;

      /* Allocate memory for new array and initialize contents to NUL */
      new_str = (pdf_char_t *) pdf_alloc (new_str_length);
      if (!new_str)
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_TEXT,
                         PDF_ENOMEM,
                         "cannot get hexadecimal representation of the text "
                         "object contents: "
                         "couldn't allocate %lu bytes",
                         (unsigned long)new_str_length);
          return NULL;
        }

      memset (new_str, 0, new_str_length);

      /* Print hexadecimal representation of each byte... */
      for (i=0, j=0; i < text->size; i++, j+=3)
        {
          /* Clear helper array... */
          memset (&new_hex_char[0], 0, 3);
          /* Print character in helper array... */
          sprintf (new_hex_char, "%02X", (unsigned int)text->data[i]);
          /* Copy to output string... */
          memcpy (&new_str[j], &new_hex_char[0], 2);
          /* And if needed, add separator */
          if (i != (text->size - 1))
            new_str[j+2] = delimiter;
        }
    }
  else
    {
      new_str = (pdf_char_t *) pdf_alloc (1);
      if (!new_str)
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_TEXT,
                         PDF_ENOMEM,
                         "cannot get hexadecimal representation of the text "
                         "object contents: "
                         "couldn't allocate 1 byte");
          return NULL;
        }
        new_str[0] = '\0';
    }

  /* Set output string */
  return new_str;
}

pdf_bool_t
pdf_text_set_host (pdf_text_t        *text,
                   const pdf_char_t  *enc,
                   const pdf_char_t  *str,
                   pdf_size_t         size,
                   pdf_error_t      **error)
{
  pdf_char_t *temp_data;
  pdf_size_t temp_size;

  PDF_ASSERT_POINTER_RETURN_VAL (str, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (enc, PDF_FALSE);
  PDF_ASSERT_RETURN_VAL (size > 0, PDF_FALSE);

  if (!pdf_text_host_to_utf32he (str,
                                 size,
                                 enc,
                                 &temp_data,
                                 &temp_size,
                                 error))
    return PDF_FALSE;

  /* Destroy previous contents of text variable, if any */
  if (!pdf_text_clean_contents (text, error))
    {
      pdf_dealloc (temp_data);
      return PDF_FALSE;
    }

  /* Really set contents */
  text->data = temp_data;
  text->size = temp_size;

  return PDF_TRUE;
}

/* Set PDF Doc Endoded string */
pdf_bool_t
pdf_text_set_pdfdocenc (pdf_text_t        *text,
                        const pdf_char_t  *str,
                        pdf_error_t      **error)
{
  pdf_char_t *temp_data;
  pdf_size_t temp_size;

  PDF_ASSERT_POINTER_RETURN_VAL (text, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (str, PDF_FALSE);

  if (!pdf_text_pdfdocenc_to_utf32he (str,
                                      strlen (str),
                                      &temp_data,
                                      &temp_size,
                                      error))
    return PDF_FALSE;

  /* Destroy previous contents of text variable, if any */
  if (!pdf_text_clean_contents (text, error))
    {
      pdf_dealloc (temp_data);
      return PDF_FALSE;
    }

  /* Really set contents */
  text->data = temp_data;
  text->size = temp_size;
  return PDF_TRUE;
}

pdf_bool_t
pdf_text_set_unicode (pdf_text_t        *text,
                      const pdf_char_t  *str,
                      pdf_size_t         size,
                      enum pdf_text_unicode_encoding_e enc,
                      pdf_error_t      **error)
{
  pdf_char_t *temp_data;
  pdf_size_t temp_size;
  pdf_bool_t ret;

  PDF_ASSERT_POINTER_RETURN_VAL (text, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (str, PDF_FALSE);
  PDF_ASSERT_RETURN_VAL (size > 0, PDF_FALSE);

  /* If host endianness required, check it and convert input encoding */
  enc = pdf_text_transform_he_to_unicode_encoding (enc);

  switch (enc)
    {
    case PDF_TEXT_UTF8: /* UTF-8 */
      ret = pdf_text_utf8_to_utf32he (str,
                                      size,
                                      &temp_data,
                                      &temp_size,
                                      error);
      break;
    case PDF_TEXT_UTF16_LE: /* UTF-16LE */
      ret = pdf_text_utf16le_to_utf32he (str,
                                         size,
                                         &temp_data,
                                         &temp_size,
                                         error);
      break;
    case PDF_TEXT_UTF16_BE: /* UTF-16BE */
      ret = pdf_text_utf16be_to_utf32he (str,
                                         size,
                                         &temp_data,
                                         &temp_size,
                                         NULL,
                                         NULL,
                                         error);
      break;
    case PDF_TEXT_UTF32_LE: /* UTF-32LE */
      ret = pdf_text_utf32le_to_utf32he (str,
                                         size,
                                         &temp_data,
                                         &temp_size,
                                         error);
      break;
    case PDF_TEXT_UTF32_BE: /* UTF-32BE */
      ret = pdf_text_utf32be_to_utf32he (str,
                                         size,
                                         &temp_data,
                                         &temp_size,
                                         error);
      break;
    default:
      ret = PDF_TRUE;
    }

  if (!ret)
    return PDF_FALSE;

  /* Destroy previous contents of text variable, if any */
  if (!pdf_text_clean_contents (text, error))
    {
      pdf_dealloc (temp_data);
      return PDF_FALSE;
    }

  /* Really set contents */
  text->data = temp_data;
  text->size = temp_size;
  return PDF_TRUE;
}

/* Concatenate the two text variables, only if country/lang info is equal */
pdf_bool_t
pdf_text_concat (pdf_text_t        *text1,
                 const pdf_text_t  *text2,
                 pdf_bool_t         override_langinfo,
                 pdf_error_t      **error)
{
  pdf_char_t *tmp;

  PDF_ASSERT_POINTER_RETURN_VAL (text1, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (text2, PDF_FALSE);

  if (!override_langinfo)
    {
      /* An error will be returned if lang code is different */
      if (strcmp (text1->lang, text2->lang) != 0)
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_TEXT,
                         PDF_ETEXTENC,
                         "cannot concatenate two text objects: "
                         "different language codes ('%s' and '%s')",
                         text1->lang,
                         text2->lang);
          return PDF_FALSE;
        }

      /* An error will be returned if country code is different */
      if (strcmp (text1->country, text2->country) != 0)
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_TEXT,
                         PDF_ETEXTENC,
                         "cannot concatenate two text objects: "
                         "different country codes ('%s' and '%s')",
                         text1->country,
                         text2->country);
          return PDF_FALSE;
        }
    }

  /* Ok, so language/country info is equal or non-existent, start
   *  concatenation */

  if (text2->size == 0)
    return PDF_TRUE;

  /* Re-allocate memory in first text element */
  tmp = (pdf_char_t *) pdf_realloc (text1->data,
                                    text1->size + text2->size);
  if (!tmp)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TEXT,
                     PDF_ENOMEM,
                     "cannot concatenate two text objects: "
                     "couldn't reallocate %lu bytes",
                     (unsigned long)(text1->size + text2->size));
      return PDF_FALSE;
    }

  text1->data = tmp;

  /* Copy contents of second element after the first one */
  memcpy (&(text1->data[text1->size]), text2->data, text2->size);

  /* Update size of first element */
  text1->size += text2->size;

  text1->modified = PDF_TRUE;

  return PDF_TRUE;
}

/* Concatenate a text variable with an ascii string */
pdf_bool_t
pdf_text_concat_ascii (pdf_text_t        *text1,
                       const pdf_char_t  *ascii_str,
                       pdf_error_t      **error)
{
  pdf_size_t len;
  pdf_char_t *tmp;
  pdf_char_t *tmp_data;
  pdf_size_t tmp_size;

  PDF_ASSERT_POINTER_RETURN_VAL (text1, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (ascii_str, PDF_FALSE);

  len = (pdf_size_t) strlen (ascii_str);

  if (len == 0)
    return PDF_TRUE;

  /* Check if input string is ascii */
  if (!pdf_text_is_ascii7 (ascii_str, len))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TEXT,
                     PDF_EBADDATA,
                     "cannot concatenate ASCII string: "
                     "not an ASCII input string");
      return PDF_FALSE;
    }

  /* ascii string is valid utf8 */
  if (!pdf_text_utf8_to_utf32he (ascii_str,
                                 len,
                                 &tmp_data,
                                 &tmp_size,
                                 error))
    return PDF_FALSE;

  tmp = (pdf_char_t *) pdf_realloc (text1->data, text1->size + tmp_size);
  if (!tmp)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TEXT,
                     PDF_ENOMEM,
                     "cannot concatenate ASCII string: "
                     "couldn't reallocate %lu bytes",
                     (unsigned long)(text1->size + tmp_size));
      return PDF_FALSE;
    }

  text1->data = tmp;
  memcpy (&(text1->data[text1->size]), tmp_data, tmp_size);
  text1->size += tmp_size;
  pdf_dealloc (tmp_data);

  return PDF_TRUE;
}

/* Default initial size of the list of replacements */
#define PDF_TEXT_ISLR 32

/* Replace a given pattern in a text object */
pdf_bool_t
pdf_text_replace (pdf_text_t        *text,
                  const pdf_text_t  *new_pattern,
                  const pdf_text_t  *old_pattern,
                  pdf_error_t      **error)
{
  return pdf_text_replace_multiple (text,
                                    new_pattern,
                                    &old_pattern,
                                    1,
                                    error);
}

/* Check replacement patterns and get minimum size */
static pdf_bool_t
pdf_text_check_replacement_patterns (const pdf_text_t **p_old_patterns,
                                     int                n_old_patterns,
                                     pdf_size_t        *p_min_old_pattern_size,
                                     pdf_error_t      **error)
{
  pdf_size_t minimum_old_pattern_size = -1;
  int i_pattern;

  for (i_pattern = 0; i_pattern < n_old_patterns; ++i_pattern)
    {
      /* Get minimum old pattern size */
      if ((i_pattern == 0) ||
          ((p_old_patterns[i_pattern])->size < minimum_old_pattern_size))
        {
          minimum_old_pattern_size = (p_old_patterns[i_pattern])->size;
        }
      /* Empty old pattern is not allowed */
      if (pdf_text_empty_p (p_old_patterns[i_pattern]))
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_TEXT,
                         PDF_ETEXTENC,
                         "cannot check replacement patterns: "
                         "input empty pattern found");
          return PDF_FALSE;
        }
    }

  /* Set output var and exit correctly */
  *p_min_old_pattern_size = minimum_old_pattern_size;
  return PDF_TRUE;
}

static pdf_bool_t
pdf_text_get_replacement_pointers (pdf_text_repl_t  **p_rep_ptrs,
                                   long              *p_n_replacements,
                                   pdf_size_t        *p_new_size,
                                   const pdf_text_t  *text,
                                   pdf_size_t         minimum_old_pattern_size,
                                   const pdf_text_t  *new_pattern,
                                   const pdf_text_t **p_old_patterns,
                                   int                n_old_patterns,
                                   pdf_error_t      **error)
{
  pdf_size_t new_size;
  int i_pattern;
  long i;
  long n_replacements;
  pdf_text_repl_t *rep_ptrs = NULL;
  long rep_ptrs_size = PDF_TEXT_ISLR/2;

  n_replacements = 0;
  i = 0;
  new_size = 0;
  while (i <= (text->size - minimum_old_pattern_size))
    {
      /* If old pattern found... */
      int old_pattern_found = 0;

      i_pattern = 0;
      while ((!old_pattern_found) &&
             (i_pattern < n_old_patterns))
        {
          if (((text->size - i) >= ((p_old_patterns[i_pattern])->size)) &&
              (memcmp (&(text->data[i]),
                       (p_old_patterns[i_pattern])->data,
                       (p_old_patterns[i_pattern])->size) == 0))
            {
              old_pattern_found = 1;
              /* Duplicate size of replacement pointers list, if needed */
              if ((!rep_ptrs) ||
                  (rep_ptrs_size == n_replacements))
                {
                  rep_ptrs = (pdf_text_repl_t *) pdf_realloc (rep_ptrs,
                                                              (2 * rep_ptrs_size *
                                                               sizeof (pdf_text_repl_t)));
                  if (!rep_ptrs)
                    {
                      pdf_set_error (error,
                                     PDF_EDOMAIN_BASE_TEXT,
                                     PDF_ENOMEM,
                                     "cannot get replacement pointers: "
                                     "couldn't reallocate %lu bytes",
                                     (unsigned long)(2 * rep_ptrs_size *
                                                     sizeof (pdf_text_repl_t)));
                      return PDF_FALSE;
                    }
                }

              /* Store pointer to old pattern */
              rep_ptrs[n_replacements].data_ptr = &(text->data[i]);
              rep_ptrs[n_replacements].old_pattern_i = i_pattern;
              n_replacements++;

              /* The index must be updated to skip the replacement */
              i += (p_old_patterns[i_pattern])->size;

              /* Update new size */
              new_size += new_pattern->size;
            }
          else
            {
              i_pattern++;
            }
        }

      if(!old_pattern_found)
        {
          i += 4;
          new_size += 4;
        }
    }

  /* Udpate new size with remaining data in old array */
  new_size += (text->size - i);

  /* Set output data and exit correctly */
  *p_new_size = new_size;
  *p_rep_ptrs = rep_ptrs;
  *p_n_replacements = n_replacements;

  return PDF_TRUE;
}

static pdf_bool_t
pdf_text_perform_replacements (pdf_text_t             *text,
                               pdf_size_t              new_size,
                               const pdf_text_t       *new_pattern,
                               const pdf_text_t      **p_old_patterns,
                               int                     n_old_patterns,
                               const pdf_text_repl_t  *rep_ptrs,
                               long                    n_replacements,
                               pdf_error_t           **error)
{
  int k;
  pdf_char_t *new_data;
  pdf_char_t *new_walker;
  pdf_char_t *old_walker;

  /* Allocate new memory chunk */
  new_data = (pdf_char_t *) pdf_alloc (new_size);
  if (!new_data)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TEXT,
                     PDF_ENOMEM,
                     "cannot perform replacements: "
                     "couldn't reallocate %lu bytes",
                     (unsigned long)(new_size));
      return PDF_FALSE;
    }

  /* Walk the list of replacements */
  new_walker = new_data;
  old_walker = text->data;
  for (k = 0; k < n_replacements; ++k)
    {
      pdf_size_t prev_size;

      /* Store the data previous to the pointer */
      prev_size = (rep_ptrs[k].data_ptr - old_walker);
      if (prev_size > 0)
        {
          memcpy (new_walker, old_walker, prev_size);
          new_walker += prev_size;
          old_walker += prev_size;
        }
      /* Perform the replacement */
      memcpy (new_walker, new_pattern->data, new_pattern->size);
      new_walker += (new_pattern->size);
      old_walker += (p_old_patterns[rep_ptrs[k].old_pattern_i]->size);
    }

  /* Add final data */
  if (((&(text->data[text->size])) - old_walker) > 0)
    {
      memcpy (new_walker,
              old_walker,
             ((&(text->data[text->size])) - old_walker));
    }

  /* Set correct final size and final content */
  pdf_dealloc (text->data);
  text->data = new_data;
  text->size = new_size;

  return PDF_TRUE;
}

pdf_bool_t
pdf_text_replace_multiple (pdf_text_t        *text,
                           const pdf_text_t  *new_pattern,
                           const pdf_text_t **p_old_patterns,
                           int                n_old_patterns,
                           pdf_error_t      **error)
{
  pdf_size_t new_size;
  pdf_size_t minimum_old_pattern_size;
  long n_replacements;
  pdf_text_repl_t *rep_ptrs = NULL;

  PDF_ASSERT_POINTER_RETURN_VAL (text, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (new_pattern, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (p_old_patterns, PDF_FALSE);
  PDF_ASSERT_RETURN_VAL (n_old_patterns > 0, PDF_FALSE);

  if (!pdf_text_check_replacement_patterns (p_old_patterns,
                                            n_old_patterns,
                                            &minimum_old_pattern_size,
                                            error))
    return PDF_FALSE;

  /* If input text is shorter than the smallest old pattern, there is no
   *  replacement to be done */
  if (minimum_old_pattern_size > text->size)
    return PDF_TRUE;

  /* First, count number of replacements to be done... a replacement pointer
   * will be stored for each replacement needed */
  new_size = 0;
  minimum_old_pattern_size = -1;
  if (!pdf_text_get_replacement_pointers (&rep_ptrs,
                                          &n_replacements,
                                          &new_size,
                                          text,
                                          minimum_old_pattern_size,
                                          new_pattern,
                                          p_old_patterns,
                                          n_old_patterns,
                                          error))
    return PDF_FALSE;

  /* Now, really perform replacements, if required */
  if (n_replacements > 0)
    {
      if (!pdf_text_perform_replacements (text,
                                          new_size,
                                          new_pattern,
                                          p_old_patterns,
                                          n_old_patterns,
                                          rep_ptrs,
                                          n_replacements,
                                          error))
        return PDF_FALSE;

      /* Dealloc list of pointers to replacements */
      if (rep_ptrs)
        pdf_dealloc (rep_ptrs);

      text->modified = PDF_TRUE;
    }

  return PDF_TRUE;
}

/* Replace a given ASCII-7 pattern in a text object */
pdf_bool_t
pdf_text_replace_ascii (pdf_text_t        *text,
                        const pdf_char_t  *new_pattern,
                        const pdf_char_t  *old_pattern,
                        pdf_error_t      **error)
{
  pdf_text_t *new_pattern_text;
  pdf_text_t *old_pattern_text;
  pdf_bool_t ret_code;

  PDF_ASSERT_POINTER_RETURN_VAL (text, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (new_pattern, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (old_pattern, PDF_FALSE);

  /* Check if patterns are real ASCII-7 valid strings */
  if ((!pdf_text_is_ascii7 (old_pattern,
                            (pdf_size_t) strlen (old_pattern))) ||
     (!pdf_text_is_ascii7 (new_pattern,
                           (pdf_size_t) strlen (new_pattern))))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TEXT,
                     PDF_EBADDATA,
                     "cannot replace ASCII string: "
                     "at least one of the requested patterns is not ASCII");
      return PDF_FALSE;
    }

  /* Ok, so load ASCII strings as if it were UTF-8 strings */

  /* Create intermediate pdf_text_t variables */
  new_pattern_text = pdf_text_new_from_unicode (new_pattern,
                                                (pdf_size_t) strlen (new_pattern),
                                                PDF_TEXT_UTF8,
                                                error);
  if (!new_pattern_text)
    return PDF_FALSE;

  old_pattern_text = pdf_text_new_from_unicode (old_pattern,
                                                (pdf_size_t) strlen (old_pattern),
                                                PDF_TEXT_UTF8,
                                                error);
  if (!old_pattern_text)
    {
      pdf_text_destroy (new_pattern_text);
      return PDF_FALSE;
    }

  /* Perform replacement */
  ret_code = pdf_text_replace (text,
                               new_pattern_text,
                               old_pattern_text,
                               error);

  /* Destroy used intermediate variables */
  pdf_text_destroy (new_pattern_text);
  pdf_text_destroy (old_pattern_text);

  return ret_code;
}

pdf_bool_t
pdf_text_filter (pdf_text_t   *text,
                 pdf_u32_t     filter,
                 pdf_error_t **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (text, PDF_FALSE);

  /* More than one filter at the same time can be requested! But Caution!
   *  UpperCase filter, LowerCase filter and TitleCase filter are mutually
   *  exclusive (at most only one of them must be enabled) */

  if ((((filter & PDF_TEXT_FILTER_UPPER_CASE) ? 1 : 0) +
       ((filter & PDF_TEXT_FILTER_LOWER_CASE) ? 1 : 0) +
       ((filter & PDF_TEXT_FILTER_TITLE_CASE) ? 1 : 0)) > 1)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TEXT,
                     PDF_ETEXTENC,
                     "cannot apply filters to text: "
                     "at most only one case conversion filter can be applied");
      return PDF_FALSE;
    }

  /* 0x00000001 */
  if ((filter & PDF_TEXT_FILTER_LINE_ENDINGS) &&
      (!pdf_text_filter_normalize_line_endings (text, error)))
    return PDF_FALSE;

  /* 0x00000010 */
  if ((filter & PDF_TEXT_FILTER_UPPER_CASE) &&
      (!pdf_text_filter_upper_case (text, error)))
    return PDF_FALSE;
  /* 0x00000100 */
  else if ((filter & PDF_TEXT_FILTER_LOWER_CASE) &&
           (!pdf_text_filter_lower_case (text, error)))
    return PDF_FALSE;
  /* 0x00001000 */
  else if ((filter & PDF_TEXT_FILTER_TITLE_CASE) &&
           (!pdf_text_filter_title_case (text, error)))
    return PDF_FALSE;

  /* 0x00010000 */
  if ((filter & PDF_TEXT_FILTER_REMOVE_AMP) &&
      (!pdf_text_filter_remove_amp (text, error)))
    return PDF_FALSE;

  /* 0x00100000 */
  if ((filter & PDF_TEXT_FILTER_NORM_WITH_FULL_WIDTH) &&
      (!pdf_text_filter_normalize_full_width_ascii (text, error)))
    return PDF_FALSE;

  /* 0x01000000 */
  if ((filter & PDF_TEXT_FILTER_REMOVE_LINE_ENDINGS) &&
      (!pdf_text_filter_remove_line_endings (text, error)))
    return PDF_FALSE;

  text->modified = PDF_TRUE;
  return PDF_TRUE;
}

const pdf_char_t *
pdf_text_get_printable (pdf_text_t *text)
{
  pdf_size_t size;

  PDF_ASSERT_POINTER_RETURN_VAL (text, NULL);

  if (text->printable != NULL)
    {
      if (!text->modified)
        return text->printable;
      pdf_dealloc (text->printable);
    }

#ifdef PDF_HOST_WIN32
  text->printable = pdf_text_get_unicode (text,
                                          PDF_TEXT_UTF16_LE,
                                          PDF_TEXT_UNICODE_WITH_NUL_SUFFIX,
                                          &size,
                                          NULL);
    return NULL;
#else
  text->printable = pdf_text_get_unicode (text,
                                          PDF_TEXT_UTF8,
                                          PDF_TEXT_UNICODE_WITH_NUL_SUFFIX,
                                          &size,
                                          NULL);
#endif /*PDF_HOST_WIN32*/

  if (text->printable)
    text->modified = PDF_FALSE;

  return text->printable;
}

pdf_i32_t
pdf_text_cmp (const pdf_text_t *text1,
              const pdf_text_t *text2,
              pdf_bool_t        case_sensitive,
              pdf_error_t      **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (text1, -1);
  PDF_ASSERT_POINTER_RETURN_VAL (text2, -1);

  /* Compare sizes of the texts */
  if (text1->size != text2->size)
    return ((text1->size > text2->size) ? 1 : -1);

  if (case_sensitive)
    return memcmp (text1->data, text2->data, text1->size);

  return pdf_text_cmp_non_case_sensitive (text1, text2, error);
}

/* -------------------------- Private functions ----------------------------- */

static pdf_i32_t
pdf_text_cmp_non_case_sensitive (const pdf_text_t  *text1,
                                 const pdf_text_t  *text2,
                                 pdf_error_t      **error)
{
  pdf_size_t size1;
  pdf_size_t size2;
  int n;

  /* Generate word boundaries list, if not already done */
  if ((!pdf_text_fill_word_boundaries_list (text1->word_boundaries,
                                            text1->data,
                                            text1->size,
                                            error)) ||
      (!pdf_text_fill_word_boundaries_list (text2->word_boundaries,
                                            text2->data,
                                            text2->size,
                                            error)))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TEXT,
                     PDF_ETEXTENC,
                     "cannot compare text non-case-sensitive: "
                     "couldn't compute word boundaries");
      return -1; /* An error happened computing word boundaries! */
    }

  size1 = pdf_list_size (text1->word_boundaries);
  size2 = pdf_list_size (text2->word_boundaries);

  /* First, compare number of words in each text */
  if (size1 != size2)
    {
      PDF_DEBUG_BASE("Different sizes...");
      return ((size1 > size2) ? 1 : -1);
    }

  /* Perform a word-per-word lower case comparison! */
  n = 0;
  while (n < size1)
    {
      struct pdf_text_wb_s *p_word1;
      struct pdf_text_wb_s *p_word2;
      pdf_i32_t ret_num;
      pdf_error_t *inner_error = NULL;

      p_word1 = (struct pdf_text_wb_s *) pdf_list_get_at (text1->word_boundaries,
                                                          n,
                                                          &inner_error);
      PDF_ASSERT_TRACE (p_word1 != NULL);

      p_word2 = (struct pdf_text_wb_s *) pdf_list_get_at (text2->word_boundaries,
                                                          n,
                                                          &inner_error);
      PDF_ASSERT_TRACE (p_word2 != NULL);

      ret_num = pdf_text_compare_words (p_word1->word_start,
                                        p_word1->word_size,
                                        p_word2->word_start,
                                        p_word2->word_size,
                                        pdf_text_get_language(text1),
                                        pdf_text_get_language(text2),
                                        &inner_error);
      if (inner_error)
        {
          pdf_propagate_error (error, inner_error);
          return -1;
        }

      /* If words are not equal, return the code */
      if (ret_num != 0)
        return ret_num;

      /* Otherwise, keep on comparing */
      ++n;
    }

  /* If arrived here, the strings are completely equal */
  return 0;
}

static pdf_i32_t
pdf_text_compare_words (const pdf_char_t  *word1,
                        const pdf_size_t   size1,
                        const pdf_char_t  *word2,
                        const pdf_size_t   size2,
                        const pdf_char_t  *language1,
                        const pdf_char_t  *language2,
                        pdf_error_t      **error)
{
  pdf_char_t *lower1;
  pdf_char_t *lower2;
  pdf_size_t new_size1;
  pdf_size_t new_size2;
  pdf_size_t worst_size;
  pdf_i32_t ret_val;

  /* Compare sizes of words */
  if (size1 != size2)
    return ((size1 > size2) ? 1 : -1);

  /* Compute new worst word length */
  worst_size = size1 * UCD_SC_MAX_EXPAND;

  /* Allocate memory for lowercases */
  lower1 = (pdf_char_t *) pdf_alloc (worst_size);
  lower2 = (pdf_char_t *) pdf_alloc (worst_size);

  if ((!lower1) || (!lower2))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TEXT,
                     PDF_ENOMEM,
                     "Couldn't allocate 2 chunks of %lu bytes",
                     (unsigned long)worst_size);
      pdf_dealloc (lower1);
      pdf_dealloc (lower2);
      return -1;
    }

  /* Lowercase words */
  if (!pdf_text_ucd_word_change_case (lower1,
                                      &new_size1,
                                      UNICODE_CASE_INFO_LOWER_CASE,
                                      word1,
                                      size1,
                                      language1,
                                      error))
    {
      pdf_dealloc (lower1);
      pdf_dealloc (lower2);
      return -1;
    }

  if (!pdf_text_ucd_word_change_case (lower2,
                                      &new_size2,
                                      UNICODE_CASE_INFO_LOWER_CASE,
                                      word2,
                                      size2,
                                      language2,
                                      error))
    {
      pdf_dealloc (lower1);
      pdf_dealloc (lower2);
      return -1;
    }

  /* Compare NEW sizes of words */
  if (new_size1 != new_size2)
    {
      pdf_dealloc (lower1);
      pdf_dealloc (lower2);
      return ((new_size1 > new_size2) ? 1 : -1);
    }

  /* Compare contents of words */
  ret_val = memcmp (lower1, lower2, new_size1);
  pdf_dealloc (lower1);
  pdf_dealloc (lower2);
  return ret_val;
}

/* Function to clean all contents of a given pdf_text_t variable */
pdf_bool_t
pdf_text_clean_contents (pdf_text_t   *text,
                         pdf_error_t **error)
{
  /* Clean list of word breaks (destroy and create empty) */
  if (!pdf_text_clean_word_boundaries_list (&(text->word_boundaries), error))
    return PDF_FALSE;

  /* Clear all contents */
  if (text->data != NULL)
    {
      pdf_dealloc (text->data);
      text->data = NULL;
    }

  /* Clean country and language info */
  memset (&(text->lang[0]), 0, PDF_TEXT_CCL);
  memset (&(text->country[0]), 0, PDF_TEXT_CCL);
  /* Reset data size */
  text->size = 0;

  text->modified = PDF_FALSE;
  if (text->printable != NULL)
    {
      pdf_dealloc (text->printable);
      text->printable = NULL;
    }
  return PDF_TRUE;
}

static pdf_bool_t
pdf_text_get_lang_from_utf16be (pdf_text_t        *element,
                                const pdf_char_t  *str_in,
                                const pdf_size_t   str_in_length,
                                pdf_char_t       **str_out,
                                pdf_size_t        *str_out_length,
                                pdf_error_t      **error)
{
  /* Country code is optional */
  pdf_bool_t country_available = PDF_FALSE;
  pdf_char_t aux[PDF_TEXT_CCL];

  /* Check last code marker position and MAXIMUM length of array.
   *  Additionally, set `str_out' and `str_out_length' */
  if ((str_in[5] != PDF_TEXT_LCI_1) ||
      (str_in[4] != PDF_TEXT_LCI_0))
    {
      /* Check last marker in bytes 6 and 7... */
      if ((str_in_length >= PDF_TEXT_LCMAXL) &&
          (str_in[7] == PDF_TEXT_LCI_1) &&
          (str_in[6] == PDF_TEXT_LCI_0))
        {
          country_available = PDF_TRUE;
          *str_out = (pdf_char_t *)str_in + PDF_TEXT_LCMAXL;
          *str_out_length = str_in_length - PDF_TEXT_LCMAXL;
        }
    }
  else
    {
      /* There is no optional country code info */
      *str_out = (pdf_char_t *)str_in + PDF_TEXT_LCMINL;
      *str_out_length = str_in_length - PDF_TEXT_LCMINL;
    }

  /* Store 2-bytes ISO 639 language code */
  memcpy (&aux[0], &str_in[2], PDF_TEXT_CCL-1);
  aux[PDF_TEXT_CCL-1] = '\0';
  pdf_text_set_language (element, (pdf_char_t *)aux);

  /* If optional country code is also available, store it... */
  if (country_available)
    {
      memcpy (&aux[0], &str_in[4], PDF_TEXT_CCL-1);
      /* Last NUL byte is already set */
      /* Store 2-bytes ISO 3166 country code */
      pdf_text_set_country (element, (pdf_char_t *)aux);
    }

  return PDF_TRUE;
}

static enum pdf_text_unicode_encoding_e
pdf_text_transform_he_to_unicode_encoding (enum pdf_text_unicode_encoding_e enc)
{
  if ((enc == PDF_TEXT_UTF16_HE) || (enc == PDF_TEXT_UTF32_HE))
    enc += (PDF_IS_BIG_ENDIAN ? PDF_TEXT_HE_TO_BE : PDF_TEXT_HE_TO_LE);
  return enc;
}

static void
pdf_text_get_unicode_string_header (enum pdf_text_unicode_encoding_e enc,
                                    pdf_u32_t         options,
                                    const pdf_char_t *language,
                                    const pdf_char_t *country,
                                    pdf_char_t        header[PDF_TEXT_USHMAXL],
                                    pdf_size_t       *header_length)
{
  short bom_bytes;
  short lang_bytes;
  pdf_text_bom_t bom;

  bom = pdf_text_get_unicode_bom (enc);

  /* Check if BOM really requested */
  bom_bytes = 0;
  if (options & PDF_TEXT_UNICODE_WITH_BOM)
    bom_bytes = bom.bom_bytes;

  /* Check if Lang/Country code really requested (only for UTF16BE!!) */
  lang_bytes = 0;
  if ((enc == PDF_TEXT_UTF16_BE) &&
      (options & PDF_TEXT_UTF16BE_WITH_LANGCODE) &&
      (strlen (language) == 2))
    {
      /* At least language is available, but country may also be
       *  available */
      lang_bytes = (strlen (country) == 2) ? PDF_TEXT_LCMAXL : PDF_TEXT_LCMINL;
    }

  /* Modify header array, if needed, to add Language/Country info and/or
   *  BOM */
  *header_length = lang_bytes + bom_bytes;
  if ((*header_length > 0) &&
      (*header_length < PDF_TEXT_USHMAXL)) /* (just in case) */
    {
      pdf_char_t *walker;

      walker = &header[0];
      /* Add BOM */
      if (bom_bytes > 0)
        {
          memcpy (walker, bom.bom_data, bom_bytes);
          /* Update walker */
          walker += bom_bytes;
        }

      /* Add Lang/Country */
      if (lang_bytes > 0)
        {
          /* Language and Country */
          if (lang_bytes == PDF_TEXT_LCMAXL)
            {
              sprintf (walker, "%c%c%2s%2s%c%c",
                       PDF_TEXT_LCI_0,PDF_TEXT_LCI_1,
                       language, country,
                       PDF_TEXT_LCI_0,PDF_TEXT_LCI_1);
            }
          /* Language only */
          else
            {
              sprintf (walker, "%c%c%2s%c%c",
                       PDF_TEXT_LCI_0,PDF_TEXT_LCI_1,
                       language,
                       PDF_TEXT_LCI_0,PDF_TEXT_LCI_1);
            }
        }
    }
}

pdf_bool_t
pdf_text_is_ascii7 (const pdf_char_t *utf8data,
                    const pdf_size_t  size)
{
  pdf_size_t i;

  for (i = 0; i < size; ++i)
    {
      /* Just check the MSB. In ASCII-7 it must be 0 */
      if (utf8data[i] & 0x80)
        return PDF_FALSE;
    }
  return PDF_TRUE;
}

/* Generate Word Boundaries list from text object */
pdf_bool_t
pdf_text_generate_word_boundaries (pdf_text_t   *text,
                                   pdf_error_t **error)
{
  return ((pdf_list_size (text->word_boundaries) == 0) ?
          pdf_text_fill_word_boundaries_list (text->word_boundaries,
                                              text->data,
                                              text->size,
                                              error) :
          PDF_TRUE);
}

void
pdf_text_destroy_word_boundaries_list (pdf_list_t **p_word_boundaries)
{
  pdf_list_iterator_t itr;
  const void *element;

  pdf_list_iterator_init (&itr, *p_word_boundaries);

  while (pdf_list_iterator_next (&itr, &element, NULL))
    {
      /* Dealloc word (pointed by the list element) */
      pdf_dealloc (element);
    }

  pdf_list_iterator_deinit (&itr);

  /* Destroy list */
  pdf_list_destroy (*p_word_boundaries);
  *p_word_boundaries = NULL;
}

/* Create empty Word Boundaries list */
pdf_list_t *
pdf_text_create_word_boundaries_list (pdf_error_t **error)
{
  /* Initialize word boundaries list */
  return pdf_list_new (NULL,
                       NULL,
                       PDF_TRUE,
                       error);
}

/* Clean (destroy and create empty) Word Boundaries list */
static pdf_bool_t
pdf_text_clean_word_boundaries_list (pdf_list_t  **p_word_boundaries,
                                     pdf_error_t **error)
{
  /* Only destroy+create if list is not empty! */
  if (pdf_list_size (*p_word_boundaries) == 0)
    /* List is already empty */
    return PDF_TRUE;

  /* Destroy element contents */
  pdf_text_destroy_word_boundaries_list (p_word_boundaries);

  /* Create empty list */
  *p_word_boundaries = pdf_text_create_word_boundaries_list (error);

  return (*p_word_boundaries != NULL ?
          PDF_TRUE :
          PDF_FALSE);
}

/* Fill in the Word Boundaries list using the given data */
static pdf_bool_t
pdf_text_fill_word_boundaries_list (pdf_list_t        *word_boundaries,
                                    const pdf_char_t  *data,
                                    const pdf_size_t   size,
                                    pdf_error_t      **error)
{
  pdf_char_t *walker;
  pdf_size_t n_bytes_left;

  /* Perform a basic check of data length */
  if (size % 4 != 0)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TEXT,
                     PDF_EBADDATA,
                     "cannot fill word boundaries list: "
                     "wrong stream size (%lu), must be multiple of 4",
                     (unsigned long) size);
      return PDF_FALSE;
    }

  /* Only try to find word boundaries if length is greater than 0! */
  if (size == 0)
    return PDF_TRUE;

  /* Initialize walker and number of bytes left */
  walker = (pdf_char_t *)data;
  n_bytes_left = size;

  while (n_bytes_left > 0)
    {
      struct pdf_text_wb_s *p_word;

      /* Allocate new word */
      p_word = (struct pdf_text_wb_s *) pdf_alloc (sizeof (struct pdf_text_wb_s));
      if (!p_word)
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_TEXT,
                         PDF_ENOMEM,
                         "cannot fill word boundaries list: "
                         "couldn't allocate %lu bytes",
                         sizeof (struct pdf_text_wb_s));
          return PDF_FALSE;
        }

      /* RULE WB1: Break at the start of text ( SOT % ) */
      p_word->word_start = walker;

      if (pdf_text_ucd_wb_detect_next (walker,
                                       n_bytes_left,
                                       &(p_word->word_stop),
                                       &n_bytes_left) != PDF_TRUE)
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_TEXT,
                         PDF_ETEXTENC,
                         "cannot fill word boundaries list: "
                         "couldn't detect next word break");
          return PDF_FALSE;
        }

      /* Compute word size in bytes */
      p_word->word_size = (p_word->word_stop - p_word->word_start) + 4;

      /* Add new word boundary to list */
      if (!pdf_list_add_last (word_boundaries,
                              p_word,
                              error))
        return PDF_FALSE;

      /* Update walker */
      walker = p_word->word_stop + 4;
    }

  return PDF_TRUE;
}

/* End of pdf-text.c */

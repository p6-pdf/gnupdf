/* -*- mode: C -*-
 *
 *       File:         pdf-text-test-common.c
 *       Date:         Mon Mar 10 18:30:01 2008
 *
 *       GNU PDF Library - Commont utilities to test the Text Module
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

#include <config.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <base/text/pdf-text-test-common.h>
#include <pdf-test-common.h>

pdf_char_t *
pdf_text_test_change_utf32_endianness(const pdf_char_t *str_in,
                                      pdf_size_t        size)
{
  pdf_char_t *str_out;

  str_out = (pdf_char_t *) pdf_alloc (size);
  if (str_out)
    {
      pdf_size_t i;

      for (i = 0; i < size; i += 4)
        {
          str_out[i + 3] = str_in[i + 0];
          str_out[i + 2] = str_in[i + 1];
          str_out[i + 1] = str_in[i + 2];
          str_out[i + 0] = str_in[i + 3];
        }
    }
  return str_out;
}

pdf_char_t *
pdf_text_test_change_utf16_endianness(const pdf_char_t *str_in,
                                      pdf_size_t        size)
{
  pdf_char_t *str_out;

  str_out = (pdf_char_t *) pdf_alloc (size);
  if (str_out)
    {
      pdf_size_t i;

      for (i = 0; i < size; i += 2)
        {
          str_out[i + 1] = str_in[i + 0];
          str_out[i + 0] = str_in[i + 1];
        }
    }
  return str_out;
}

/* Function quite similar to `pdf_text_get_hex', but using an array of bytes
 *  as input. */
pdf_char_t *
pdf_text_test_get_hex (const pdf_char_t *data,
                       const pdf_size_t  size,
                       const pdf_char_t  delimiter)
{
  int i;
  int j;
  unsigned int new_str_length;
  pdf_char_t *new_str;
  char new_hex_char [3];

  /* Get new string length. If input string has N bytes, we need:
   * - 1 byte for last NUL char
   * - 2N bytes for hexadecimal char representation of each byte...
   * - N-1 bytes for the separator ':'
   * So... a total of (1+2N+N-1) = 3N bytes are needed... */
  new_str_length =  3 * size;

  /* Allocate memory for new array and initialize contents to NUL */
  new_str = (pdf_char_t *) pdf_alloc (new_str_length);
  memset (new_str, 0, new_str_length);

  /* Print hexadecimal representation of each byte... */
  for (i = 0, j = 0; i < size; i++, j += 3)
    {
      /* Clear helper array... */
      memset (&new_hex_char[0], 0, 3);
      /* Print character in helper array... */
      sprintf (new_hex_char, "%02X", (unsigned int)data[i]);
      /* Copy to output string... */
      memcpy (&new_str[j], &new_hex_char[0], 2);
      /* And if needed, add separator */
      if (i != (size - 1))
	new_str[j+2] = delimiter;
    }

  /* Set output string */
  return new_str;
}

static const pdf_char_t *utf8_bom =    "\xEF\xBB\xBF";
static const pdf_char_t *utf16be_bom = "\xFE\xFF";
static const pdf_char_t *utf16le_bom = "\xFF\xFE";
static const pdf_char_t *utf32be_bom = "\x00\x00\xFE\xFF";
static const pdf_char_t *utf32le_bom = "\xFF\xFE\x00\x00";

const pdf_char_t *
pdf_text_test_get_bom (enum pdf_text_unicode_encoding_e  enc,
                       int                              *bom_size)
{
  switch (enc)
    {
    case PDF_TEXT_UTF8:
      *bom_size = 3;
      return utf8_bom;
    case PDF_TEXT_UTF16_BE:
      *bom_size = 2;
      return utf16be_bom;
    case PDF_TEXT_UTF16_LE:
      *bom_size = 2;
      return utf16le_bom;
    case PDF_TEXT_UTF16_HE:
      *bom_size = 2;
#if (PDF_IS_BIG_ENDIAN)
      return utf16be_bom;
#else
      return utf16le_bom;
#endif /* PDF_IS_BIG_ENDIAN */
    case PDF_TEXT_UTF32_BE:
      *bom_size = 4;
      return utf32be_bom;
    case PDF_TEXT_UTF32_LE:
      *bom_size = 4;
      return utf32le_bom;
    case PDF_TEXT_UTF32_HE:
      *bom_size = 4;
#if (PDF_IS_BIG_ENDIAN)
      return utf32be_bom;
#else
      return utf32le_bom;
#endif /* PDF_IS_BIG_ENDIAN */
    default:
      return NULL;
    }
}

/* End of pdf-text-test-common.c */

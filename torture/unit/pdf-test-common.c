/* -*- mode: C -*-
 *
 *       File:         pdf-text-common.c
 *       Date:         Thu Feb 24 22:53:57 2011
 *
 *       GNU PDF Library - Common utilities for the unit tests
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

#include <config.h>

#include <locale.h>
#include <string.h>

#include <check.h>

#include <pdf-test-common.h>

void
pdf_test_setup (void)
{
#if defined HAVE_SETLOCALE
  /* Initialize locale in the program.
   * We really need to fix a given locale, so that the unit tests
   * don't depend on where they are run */
  setlocale (LC_ALL, "en_US.utf8");
#endif /* HAVE_SETLOCALE */

  /* Initialize the PDF library */
  fail_unless (pdf_init (NULL));
}

void
pdf_test_teardown (void)
{
  pdf_finish ();
}

/* Function quite similar to `pdf_text_get_hex', but using an array of bytes
 *  as input. */
pdf_char_t *
pdf_test_get_hex (const pdf_uchar_t *data,
                  const pdf_size_t   size,
                  const pdf_char_t   delimiter)
{
  int i;
  int j;
  unsigned int new_str_length;
  pdf_uchar_t *new_str;
  pdf_uchar_t new_hex_char [3];

  /* Get new string length. If input string has N bytes, we need:
   * - 1 byte for last NUL char
   * - 2N bytes for hexadecimal char representation of each byte...
   * - N-1 bytes for the separator ':'
   * So... a total of (1+2N+N-1) = 3N bytes are needed... */
  new_str_length = (size > 0 ? (3 * size) : 1);

  /* Allocate memory for new array and initialize contents to NUL */
  new_str = (pdf_uchar_t *) pdf_alloc (new_str_length);
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

/* End of pdf-test-common.c */

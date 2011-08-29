/* -*- mode: C -*-
 *
 *       File:         pdf-tokeniser.c
 *       Date:         Tue Jun 21 22:17:01 2011
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

#include <config.h>

#include <string.h>

#include <pdf-alloc.h>
#include <pdf-tokeniser.h>

/* Decimal point used by the user */
static pdf_char_t *decimal_point;

const pdf_char_t *
pdf_tokeniser_get_decimal_point (void)
{
  PDF_ASSERT_POINTER_RETURN_VAL (decimal_point, NULL);

  return decimal_point;
}

static pdf_char_t *
guess_decimal_point (pdf_error_t **error)
{
  int len;
  pdf_char_t decpt[16];
  pdf_char_t *decimal_point;

  len = snprintf (decpt, sizeof (decpt), "%#.0f", 1.0);
  if (len <= 0 ||
      (pdf_size_t)len >= sizeof (decpt))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TOKENISER,
                     PDF_ERROR,
                     "cannot guess decimal point: "
                     "invalid length (%d)",
                     len);
      return NULL;
    }

  decimal_point = pdf_alloc (len);
  if (!decimal_point)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TOKENISER,
                     PDF_ERROR,
                     "cannot guess decimal point: "
                     "couldn't allocate '%lu' bytes",
                     len);
      return NULL;
    }

  /* this copies the trailing '\0' due to the starting offset */
  memcpy (decimal_point, &decpt[1], len);

  return decimal_point;
}

pdf_bool_t
pdf_tokeniser_init (pdf_error_t **error)
{
  /* Already initialized? */
  if (decimal_point)
    return PDF_TRUE;

  /* Guess decimal point */
  decimal_point = guess_decimal_point (error);
  return (decimal_point ? PDF_TRUE : PDF_FALSE);
}

void
pdf_tokeniser_deinit (void)
{
  if (decimal_point)
    pdf_dealloc (decimal_point);
}


/* End of pdf-tokeniser.c */

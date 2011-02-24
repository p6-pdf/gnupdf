/* -*- mode: C -*-
 *
 *       File:         pdf-text-context.c
 *       Date:         Fri Feb 25 23:58:56 2008
 *
 *       GNU PDF Library - Encoded Text Context
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

#include <string.h>
#include <stdio.h>
#include <locale.h>
#include <localename.h>    /* From gnulib sources.  */
#include <localcharset.h>  /* From gnulib sources.  */
#include <streq.h>         /* From gnulib sources.  */

#include <pdf-types.h>
#include <pdf-text.h>
#include <pdf-text-context.h>

/* Host Language code Length */
#define PDF_TEXT_HLL  3

/* Structure containing the specific context configuration for the Text
 *  Encoding Module */
typedef struct pdf_text_context_s {
  /* Host encoding configured in the system */
  pdf_text_host_encoding_t  host_encoding;
  /* 2-character LANG ID */
  pdf_char_t                host_language_id[PDF_TEXT_HLL];
  /* 2-character Country ID */
  pdf_char_t                host_country_id[PDF_TEXT_HLL];
  /* System endianness */
  enum pdf_endianness_e     host_endianness;
  /* Default EOL character in the system */
  pdf_text_eol_t            host_eol;
} pdf_text_context_t;


/* This context will be initialized only once at program startup, and it will
 *  be treated as constant from then on, so there shouldn't be any problem
 *  with multiple threading and reentrancy */
static pdf_text_context_t text_context;
static pdf_bool_t text_context_initialized = PDF_FALSE;

/* Definition of the different platform-dependent EOL types, in UTF-8. This
 *  array is based on the `enum pdf_text_eol_types' enumeration. */
static const struct pdf_text_eol_s pdf_text_eol_types [PDF_TEXT_EOLMAX] = {
  { { PDF_TEXT_DEF_CR, PDF_TEXT_DEF_LF,  0x00 } },  /* PDF_TEXT_EOL_WINDOWS */
  { { PDF_TEXT_DEF_LF, 0x00,             0x00 } },  /* PDF_TEXT_EOL_UNIX    */
  { { 0xC2,            PDF_TEXT_DEF_NEL, 0x00 } },  /* PDF_TEXT_EOL_EBCDIC  */
  { { PDF_TEXT_DEF_CR, 0x00,             0x00 } }   /* PDF_TEXT_EOL_MACOS   */
};

/* Function to detect the endianness of the system. Can be either Big Endian
 *  (like PPC systems) or Little Endian (like Intel systems). PDF_IS_BIG_ENDIAN
 *  is filled in config.h at configure level. */
static void
pdf_text_detect_host_endianness (void)
{
#if PDF_IS_BIG_ENDIAN
  PDF_DEBUG_BASE("TextContext: Host is 'Big Endian'");
  text_context.host_endianness = PDF_TEXT_BIG_ENDIAN;
#else
  PDF_DEBUG_BASE("TextContext: Host is 'Little Endian'");
  text_context.host_endianness =  PDF_TEXT_LITTLE_ENDIAN;
#endif
}

static pdf_bool_t
pdf_text_detect_host_encoding (pdf_error_t **error)
{
  const pdf_char_t *charset;
  pdf_size_t length;

  /* Initialize contents of structure */
  memset (&(text_context.host_encoding.name[0]), 0, PDF_TEXT_HENMAXL);

  /* Get host encoding and check it */
  charset = locale_charset ();
  if(!charset ||
     (length = strlen (charset)) < 3)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TEXT,
                     PDF_ETEXTENC,
                     "couldn't detect host encoding: "
                     "invalid charset, '%s'",
                     charset ? charset : "unknown");
      return PDF_FALSE;
    }

  /* Limit length to maximum length, just in case */
  length = (length > (PDF_TEXT_HENMAXL - 1) ?
            (PDF_TEXT_HENMAXL - 1) : length);
  strncpy (&(text_context.host_encoding.name[0]), charset, length);

  PDF_DEBUG_BASE ("TextContext: Host Encoding is '%s'",
                  text_context.host_encoding.name);

  return PDF_TRUE;
}

static pdf_bool_t
pdf_text_detect_host_language_and_country (pdf_error_t **error)
{
  const pdf_char_t *locale_name;

  /* Initialize context strings */
  memset (&text_context.host_language_id[0], 0, PDF_TEXT_HLL);
  memset (&text_context.host_country_id[0], 0, PDF_TEXT_HLL);

  /* Get system default locale name and check it */
  locale_name = gl_locale_name (LC_CTYPE, "LC_CTYPE");
  if (!locale_name)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TEXT,
                     PDF_ETEXTENC,
                     "couldn't detect host language and country: "
                     "locale name not detected");
      return PDF_FALSE;
    }

  if (!STREQ (locale_name, "C", 'C', 0, 0, 0, 0, 0, 0, 0, 0) &&
      !STREQ (locale_name, "POSIX", 'P', 'O', 'S', 'I', 'X', 0, 0, 0, 0))
    {
      /* Store language ID */
      strncpy (&(text_context.host_language_id[0]),
               locale_name,
               PDF_TEXT_HLL - 1);

      /* If available, store country ID */
      if((strlen (locale_name) >= 5) &&
         (locale_name[2] == '_'))
        {
          strncpy(&(text_context.host_country_id[0]),
                  &locale_name[3],
                  PDF_TEXT_HLL - 1);
        }
    }

  PDF_DEBUG_BASE("TextContext: Locale name is '%s'",
                 locale_name);
  PDF_DEBUG_BASE("TextContext: Language ID is '%.2s'",
                 text_context.host_language_id);
  PDF_DEBUG_BASE("TextContext: Country ID is '%.2s'",
                 text_context.host_country_id);
  return PDF_TRUE;
}

static void
pdf_text_detect_host_eol (void)
{
  /* The EOL sequence (a.k.a Newline function) may be represented by different
   *  characters, depending on the platform
   *
   *  Mac OS 9.x and earlier ---> CR (Carriage Return), U+000D (Not supported)
   *  Mac OS X, Unix, GNU/Linux ---> LF (Line Feed), U+000A
   *  Windows ---> CRLF (Carriage Return + Line Feed), <U+000D,U+000A>
   *  EBCDIC-based OS --> NEL (Next Line), U+0085 (Not supported)
   */
#ifdef PDF_HOST_WIN32
  {
    text_context.host_eol =
      (pdf_text_eol_t)&pdf_text_eol_types[PDF_TEXT_EOL_WINDOWS];
  }
#else
  {
    text_context.host_eol =
      (pdf_text_eol_t)&pdf_text_eol_types[PDF_TEXT_EOL_UNIX];
  }
#endif
}

pdf_bool_t
pdf_text_context_init (pdf_error_t **error)
{
  /* Get system endianness */
  pdf_text_detect_host_endianness ();

  /* Get language and country ID from locale */
  if (!pdf_text_detect_host_language_and_country (error))
    return PDF_FALSE;

  /* Get host encoding from system */
  if (!pdf_text_detect_host_encoding (error))
    return PDF_FALSE;

  /* Detect host default EOL sequence */
  pdf_text_detect_host_eol ();

  /* Mark the context as initialized */
  text_context_initialized = PDF_TRUE;

  return PDF_TRUE;
}

pdf_bool_t
pdf_text_context_initialized (void)
{
  return text_context_initialized;
}

enum pdf_endianness_e
pdf_text_context_get_host_endianness (void)
{
  return text_context.host_endianness;
}

pdf_text_host_encoding_t
pdf_text_context_get_host_encoding (void)
{
  return text_context.host_encoding;
}

const pdf_char_t *
pdf_text_context_get_host_language (void)
{
  return (const pdf_char_t *)text_context.host_language_id;
}

const pdf_char_t *
pdf_text_context_get_host_country (void)
{
  return (const pdf_char_t *)text_context.host_country_id;
}

pdf_text_eol_t
pdf_text_context_get_host_eol (enum pdf_text_eol_types eol_type)
{
  return (eol_type == PDF_TEXT_EOL_HOST ?
          text_context.host_eol :
          (pdf_text_eol_t) &(pdf_text_eol_types [eol_type]));
}

/* End of pdf-text-context.c */

/* -*- mode: C -*- Time-stamp: "2009-08-05 22:27:36 davazp"
 *
 *       File:         pdf-global.c
 *       Date:         Thu Jul  5 21:07:10 2007
 *
 *       GNU PDF Library - Global code
 *
 */

/* Copyright (C) 2007 Free Software Foundation, Inc. */

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
#include <pdf-global.h>
#include <pdf-types.h>
#include <pdf-crypt.h>
#include <pdf-text.h>
#include <pdf-time.h>

/* Global variables */

char *program_name = "libgnupdf";
char *gnupdf_version = "0.1";

struct pdf_globals_s pdf_globals = {
  PDF_FALSE
};



/* Library initialization routine */
int
pdf_init (void)
{
  int ret;

  if (pdf_globals.initialized == PDF_FALSE)
    {
      ret = pdf_crypt_init();

      if (ret != PDF_OK)
        return ret;

      ret = pdf_text_init();
      if (ret != PDF_OK)
        return ret;

      ret = pdf_time_init();
      if (ret != PDF_OK )
        return ret;

      pdf_globals.initialized = PDF_TRUE;
    }
  
  return PDF_OK;
}


/* Library finalization routine */
int
pdf_finish (void)
{
  
  return PDF_OK;
}

/* End of pdf.c */

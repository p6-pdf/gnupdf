/* -*- mode: C -*-
 *
 *       File:         pdf-global.c
 *       Date:         Thu Jul  5 21:07:10 2007
 *
 *       GNU PDF Library - Global code
 *
 */

/* Copyright (C) 2007, 2009, 2010 Free Software Foundation, Inc. */

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
#include <pthread.h>
#include <pdf-global.h>
#include <pdf-types.h>
#include <pdf-crypt.h>
#include <pdf-text.h>
#include <pdf-time.h>
#include <pdf-fsys.h>
#include <pdf-tokeniser.h>

/* Global variables */

const pdf_char_t *pdf_library_name = "libgnupdf";
const pdf_char_t *pdf_version = "0.1";

/* This structure contains global data used by the library. A variable
   of this type, called `pdf_globals', is defined in pdf.c. The
   contents of this structure are initialized in the `pdf_init'
   function. */
struct pdf_globals_s
{
  int initialized;
  pthread_mutex_t init_mutex;
};

struct pdf_globals_s pdf_globals = {
  PDF_FALSE,
  PTHREAD_MUTEX_INITIALIZER
};

/* Library initialization routine
 * TODO: Return pdf_error_t and such */
int
pdf_init (void)
{
  int ret = PDF_ERROR;

  if (pthread_mutex_lock (&(pdf_globals.init_mutex)) != 0)
    return ret;

  /* The mutex is locked within this brace. NO "return" statements allowed
   * from within this block as they will leave the mutex locked and set us
   * up for a deadlock.
   */
  if (pdf_globals.initialized)
    ret = PDF_OK;
  else
    {
      pdf_error_t *inner_error = NULL;

      if (!pdf_crypt_init (&inner_error) ||
          !pdf_text_init (&inner_error) ||
          !pdf_time_module_init (&inner_error) ||
          !pdf_fsys_init (&inner_error) ||
          !pdf_tokeniser_init (&inner_error))
        {
          /* TODO: Propagate error */
          pdf_error_destroy (inner_error);
        }
      else
        {
          pdf_globals.initialized = PDF_TRUE;
          ret = PDF_OK;
        }
    }

  pthread_mutex_unlock ( &(pdf_globals.init_mutex) );

  return ret;
}

/* Library finalization routine */
void
pdf_finish (void)
{
  pdf_tokeniser_deinit ();
  pdf_fsys_deinit ();
  pdf_text_deinit ();
}

/* End of pdf-global.c */

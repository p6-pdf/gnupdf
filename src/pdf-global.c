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

#if PDF_FSYS_HTTP
#include <curl/curl.h>
#endif

/* Global variables */

pdf_char_t *pdf_library_name = "libgnupdf";
pdf_char_t *pdf_version = "0.1";

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
  pdf_error_t *inner_error = NULL;
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
      if (pdf_crypt_init () != PDF_OK)
        {
          inner_error = pdf_error_new (PDF_EDOMAIN_BASE_ENCRYPTION,
                                       PDF_ERROR,
                                       "couldn't initialize crypt module");
        }
      else if (pdf_text_init (&inner_error) != PDF_TRUE)
        {
          /* Inner error already set here */
        }
      else if (pdf_time_init () != PDF_OK)
        {
          inner_error = pdf_error_new (PDF_EDOMAIN_BASE_TIME,
                                       PDF_ERROR,
                                       "couldn't initialize time module");
        }
      else
        {
#if PDF_FSYS_HTTP
          /* The documentation for libcurl indicates that this initialization
           * ( curl_global_init () ) is _not_ thread-safe, and that it should
           * therefore be performed during application startup while the
           * application as a whole is single-threaded. If this is not possible
           * it must be ensured that no other threads are initializing any of
           * the libraries that libcurl will be initializing.
           */

          /* From curl_global_init(3) (libcurl manpage):
           * This function is not thread safe. You must not call it when any
           * other thread in the program  (i.e.  a  thread  sharing  the same
           * memory) is running.  This doesn't just mean no other thread that
           * is using libcurl.  Because curl_global_init() calls functions of
           * other libraries that are similarly thread unsafe, it could
           * conflict with any other thread that uses these other libraries.
           */
          if (curl_global_init (CURL_GLOBAL_ALL) != 0)
            {
              inner_error = pdf_error_new (PDF_EDOMAIN_FS,
                                           PDF_ERROR,
                                           "couldn't initialize curl");
            }
#endif // PDF_FSYS_HTTP
        }

      if (!inner_error)
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
#if PDF_FSYS_HTTP
  if (pthread_mutex_lock (&(pdf_globals.init_mutex)) == 0)
    {
      /* The mutex is locked within this brace. NO "return" statements allowed
       * from within this block as they will leave the mutex locked and set us
       * up for a deadlock.
       */

      /* From curl_global_cleanup(3) (libcurl manpage):

       * This function is not thread safe. You must not call it when any other
       * thread in the program  (i.e.  a  thread  sharing  the same memory) is
       * running.  This doesn't just mean no other thread that is using
       * libcurl.  Because curl_global_cleanup(3) calls  functions  of
       * other  libraries  that  are  similarly  thread  unsafe, it could
       * conflict with any other thread that uses these other libraries.
       */

      curl_global_cleanup ();

      pthread_mutex_unlock (&(pdf_globals.init_mutex));
    }
#endif // PDF_FSYS_HTTP

  pdf_text_deinit ();
}

/* End of pdf.c */

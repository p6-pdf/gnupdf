/* -*- mode: C -*- Time-stamp: "09/11/19 16:24:34 jemarch"
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

#if PDF_FSYS_HTTP
#include <curl/curl.h>
#endif

/* Global variables */

char *pdf_library_name = "libgnupdf";
char *pdf_version = "0.1";

struct pdf_globals_s pdf_globals = {
  PDF_FALSE,
  PTHREAD_MUTEX_INITIALIZER
};



/* Library initialization routine */
int
pdf_init (void)
{
  int ret;
#if PDF_FSYS_HTTP
  CURLcode crv;
#endif

  ret = pthread_mutex_lock ( &(pdf_globals.init_mutex) );
  if (0 == ret)
    { 
      /* The mutex is locked within this brace. NO "return" statements allowed 
       * from within this block as they will leave the mutex locked and set us
       * up for a deadlock.
       */
  
      if (pdf_globals.initialized == PDF_FALSE)
        {
          ret = pdf_crypt_init ();
          
          if (PDF_OK == ret)
            {
              ret = pdf_text_init ();
            }
          
          if (PDF_OK == ret)
            {
              ret = pdf_time_init ();
            }
          
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

          if (PDF_OK == ret)
            {
              crv = curl_global_init (CURL_GLOBAL_ALL);
              if (0 != crv )
                {
                  ret = PDF_ERROR;
                }
            }
#endif // PDF_FSYS_HTTP

          if (PDF_OK == ret)
            {
              pdf_globals.initialized = PDF_TRUE;
            }
        }

      pthread_mutex_unlock ( &(pdf_globals.init_mutex) );
    }
  else
    { /* locking the mutex returned an error. This shouldn't happen. */
      ret = PDF_ERROR;
    }
  
  return ret;
}


/* Library finalization routine */
int
pdf_finish (void)
{
  int ret;
#if PDF_FSYS_HTTP
  ret = pthread_mutex_lock ( &(pdf_globals.init_mutex) );
  if (0 == ret)
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

      pthread_mutex_unlock ( &(pdf_globals.init_mutex) );
    }
#endif // PDF_FSYS_HTTP
  
  return PDF_OK;
}

/* End of pdf.c */

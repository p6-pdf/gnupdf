/* -*- mode: C -*-
 *
 *       File:         pdf-time-context.c
 *       Date:         Sun May 18 13:08:37 2008
 *
 *       GNU PDF Library - Time Module Context management
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
#include <time.h>
#include <string.h>

#include <pdf-alloc.h>
#include <pdf-time-context.h>

/* Time module context */
struct pdf_time_context_s {
  pdf_i32_t  local_time_gmt_offset;      /* Seconds west of GMT */
  pdf_bool_t local_time_daylight_save; /* True if Daylight saving times */
};

/* This context will be initialized only once at program startup, and it will
 *  be treated as constant from then on, so there shouldn't be any problem
 *  with multiple threading and reentrancy */
static struct pdf_time_context_s time_context;
pdf_bool_t time_context_initialized = PDF_FALSE;

void
pdf_time_context_deinit (void)
{
  if (time_context_initialized)
    {
      memset (&time_context, 0, sizeof (time_context));
      time_context_initialized = PDF_FALSE;
      PDF_DEBUG_BASE ("Deinitialized Text context...");
    }
}

/* Initialize time context */
pdf_bool_t
pdf_time_context_init (pdf_error_t **error)
{
  time_t tloc;
  struct tm* time_struct;

  /* If already initialized, just return */
  if (time_context_initialized)
    return PDF_TRUE;

  time (&tloc);
  time_struct = localtime (&tloc);

#if defined PDF_HOST_WIN32
  /* mingw does not support tm_gmtoff in struct tm, but it provides
   * the _timezone global variable with the difference in seconds
   * between GMT and local time. */
  time_context.local_time_gmt_offset = _timezone;
#else
  /* Set GMT offset */
  time_context.local_time_gmt_offset = time_struct->tm_gmtoff;
#endif

  /* Set flag to indicate if Daylight saving times are applied in the system
   * if needed */
  time_context.local_time_daylight_save = (time_struct->tm_isdst == 0 ?
                                           PDF_FALSE : PDF_TRUE);

  PDF_DEBUG_BASE("Initialized Time context...");
  PDF_DEBUG_BASE("  GMT offset: %d secs", time_context.local_time_gmt_offset);
  PDF_DEBUG_BASE("  Daylight saving? %s", time_context.local_time_daylight_save ? \
                 "yes":"no");

  return PDF_TRUE;
}

/* Get the GMT offset of the local time configuration. The offset is obtained as
 *  seconds west of GMT */
pdf_i32_t
pdf_time_context_get_gmt_offset (void)
{
  return time_context.local_time_gmt_offset;
}

/* End of pdf-time-context.c */

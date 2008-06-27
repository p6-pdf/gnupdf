/* -*- mode: C -*- Time-stamp: ""
 *
 *       File:         pdf-time-context.c
 *       Date:         Sun May 18 13:08:37 2008
 *
 *       GNU PDF Library - Time Module Context management
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

#include <stdio.h>
#include <time.h>

#include <pdf-time-context.h>


typedef struct pdf_time_context_s {
  pdf_i32_t   local_time_gmt_offset;      /* Seconds west of GMT */
  pdf_bool_t  local_time_daylight_save; /* True if Daylight saving times */
} pdf_time_context_t;


/* This context will be initialized only once at program startup, and it will
 *  be treated as constant from then on, so there shouldn't be any problem
 *  with multiple threading and reentrancy */
static pdf_time_context_t time_context;



/* Initialize time context. Must be done only once at program startup!.
 *  Not thread-safe! */
pdf_status_t
pdf_time_context_init(void)
{
  extern pdf_time_context_t time_context;
  
  tzset();
  
  /* Set GMT offset */
  time_context.local_time_gmt_offset = timezone;

  /* Set flag to indicate if Daylight saving times are applied in the system
   * if needed */
  time_context.local_time_daylight_save = (daylight == 0) ? PDF_FALSE : PDF_TRUE;
  
  PDF_DEBUG_BASE("Initializing Time module...");
  PDF_DEBUG_BASE("GMT offset: %d secs", time_context.local_time_gmt_offset);
  PDF_DEBUG_BASE("Daylight saving? %s",time_context.local_time_daylight_save ? \
                 "yes":"no");
    
  return PDF_OK;
}


/* Get the GMT offset of the local time configuration. The offset is obtained as
 *  seconds west of GMT */
pdf_i32_t
pdf_time_context_get_gmt_offset(void)
{
  extern pdf_time_context_t time_context;
  return time_context.local_time_gmt_offset;
}


/* End of pdf-time-context.c */

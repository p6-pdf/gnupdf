/* -*- mode: C -*-
 *
 *       File:         pdf-time.h
 *       Date:         Mon Apr 28 23:23:04 2008
 *
 *       GNU PDF Library - Header file for the Time module
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

#ifndef PDF_TIME_H
#define PDF_TIME_H

#include <config.h>

/* BEGIN PUBLIC */

#if defined(PDF_HOST_WIN32)
#  include <windows.h>
#endif /* PDF_HOST_WIN32 */

/* END PUBLIC */

#include <stdio.h>

#include <pdf-types.h>


/* A variable of type pdf_time_t contains information about a particular date
 *  and time. It is stored in seconds since 1 January 1970 (UTC). Additionally
 *  an extra variable specifies the local time, as the offset between UTC and
 *  the local time. */
struct pdf_time_s {
  pdf_i64_t seconds;
  /* A GMT offset, seconds WEST of GMT */
  pdf_i32_t  gmt_offset;
};



/* --------------------- Time Module Initialization ------------------------- */

/* Initialize Time module. Warning! Not thread-safe, must be used only once when
 *  the program starts. It will be in charge of detecting currently configured
 *  time zone (GMT offset) and so on. */
pdf_status_t
pdf_time_init(void);



/* BEGIN PUBLIC */

typedef struct pdf_time_s *pdf_time_t;


/* A variable of type pdf_time_span_t represents a span of time. */
typedef pdf_i64_t pdf_time_span_t;

/* This structure holds information about a specific time represented in
 *  calendar items: years, months, days, day of week, hours, minutes, seconds 
 *  and the relative offset with GMT. */
struct pdf_time_cal_s {
  /* A year number. */
  pdf_u32_t  year;
  /* A month number. The valid range is 1..12. */
  pdf_u32_t month;
  /* A day number. The valid range is 1..31. */
  pdf_u32_t day;
  /* Day of the week. The valid range is 1..7 (Monday to Sunday). */
  pdf_u32_t dow;
  /* An hour. The valid range is 0..23. */
  pdf_u32_t hour;
  /* A minute. The valid range is 0..59. */
  pdf_u32_t minute;
  /* A second. The valid range is 0..59. */
  pdf_u32_t second;
  /* A GMT offset, in seconds WEST of GMT */
  pdf_i32_t  gmt_offset;
};


/* This structure holds information about a time span represented in calendar
 *  items: years, months, days, hours, minutes and seconds. */
struct pdf_time_cal_span_s {
  pdf_bool_t sign;
  pdf_u32_t years;
  pdf_u32_t months;
  pdf_u32_t days;
  pdf_u32_t hours;
  pdf_u32_t minutes;
  pdf_u32_t seconds;
};


/* Enumeration containing the several supported time formats used to convert a
 *  time to a string representation or to parse a time from a string
 *  representation. */
enum pdf_time_format_e {
  PDF_TIME_FORMAT_PDF,
  PDF_TIME_FORMAT_ISO_8601,
  PDF_TIME_FORMAT_UTC_ASN1,
  PDF_TIME_FORMAT_GENERALIZED_ASN1
};



/* --------------------- Time Creation and Destruction ---------------------- */

/* Create new pdf_time_t object */
pdf_status_t
pdf_time_new (pdf_time_t *time);

/* Duplicate pdf_time_t object */
pdf_time_t
pdf_time_dup (const pdf_time_t orig);

/* Destroy pdf_time_t object */
pdf_status_t
pdf_time_destroy (pdf_time_t time_var);


/* ------------------------- Managing Time Values --------------------------- */

/* Copy the contents of a given pdf_time_t object */
pdf_status_t
pdf_time_copy (const pdf_time_t orig,
               pdf_time_t copy);

/* Clear contents of the pdf_time_t object */
pdf_status_t
pdf_time_clear (pdf_time_t time_var);


/* Set time value with a 32-bit unsigned integer */
pdf_status_t
pdf_time_set_from_u32 (pdf_time_t time_var,
                       pdf_u32_t seconds);


/* Set time value with a pdf_i64_t variable */
pdf_status_t
pdf_time_set_from_i64 (pdf_time_t time_var,
                       pdf_i64_t  seconds);


#ifdef PDF_HOST_WIN32
  /* Windows-specific function to set the time with a pdf_i64_t */
  pdf_status_t
  pdf_time_w32_set_from_filetime (pdf_time_t time_var,
                                  const FILETIME *p_filetime);
#endif


/* Add the time span represented by cal_span to the text object */
pdf_status_t
pdf_time_add_cal_span (pdf_time_t time_var,
                       const struct pdf_time_cal_span_s *p_cal_span);

/* Substract the time span represented by cal_span from the text object */
pdf_status_t
pdf_time_sub_cal_span (pdf_time_t time_var,
                       const struct pdf_time_cal_span_s *p_cal_span);

/* Add the time span contained in time_span to time. */
pdf_status_t
pdf_time_add_span (pdf_time_t time_var,
                   const pdf_time_span_t time_span);

/* Subtract the time span contained in time_span to time. */
pdf_status_t
pdf_time_sub_span (pdf_time_t time_var,
                   const pdf_time_span_t time_span);

/* Fill local_cal with the local calendar time of object. */
pdf_status_t
pdf_time_get_local_cal (const pdf_time_t time_var,
                        struct pdf_time_cal_s *p_local_cal);

/* Get the UTC calendar time of a given time variable. */
pdf_status_t
pdf_time_get_utc_cal (const pdf_time_t time_var,
                      struct pdf_time_cal_s *p_utc_cal);

/* Set the value of a time variable to a given calendar time. */
pdf_status_t
pdf_time_from_cal (pdf_time_t time_var,
                   const struct pdf_time_cal_s *p_cal_time);

/* Set the local time offset of time to the one used by the operating system. */
pdf_status_t
pdf_time_set_local_offset (pdf_time_t time_var);



/* ----------------------- Getting Time Intervals --------------------------- */

/* Get time interval as Calendar Span */
pdf_status_t
pdf_time_diff_cal (const pdf_time_t time1,
                   const pdf_time_t time2,
                   struct pdf_time_cal_span_s *p_cal_span);

/* Get time interval as Span */
pdf_status_t
pdf_time_diff (const pdf_time_t time1,
               const pdf_time_t time2,
               pdf_time_span_t  *p_time_span);


/* ---------------------------- Time comparison ----------------------------- */

/* Compares two times. */
pdf_i32_t
pdf_time_cmp (const pdf_time_t time1,
              const pdf_time_t time2);


/* ---------------------- Time printing and parsing ------------------------- */

/* Create a string representation of a given time. */
pdf_char_t *
pdf_time_to_string (const pdf_time_t time_var,
                    const enum pdf_time_format_e time_format,
                    pdf_bool_t include_trailing_apostrophe);


/* Get a string containing a time specification in some format and fill a time
 *  variable with the parsed values. */
pdf_status_t
pdf_time_from_string (pdf_time_t time_var,
                      const pdf_char_t *time_str,
                      const enum pdf_time_format_e time_format,
                      pdf_bool_t require_trailing_apostrophe);


/* ---------------------- Getting the Current Time -------------------------- */

/* Set the value of object to the current local time used by the operating
 *  system. */
pdf_status_t
pdf_time_set_to_current_local_time (pdf_time_t time_var);


/* Set the value of object to the current UTC time used by the operating
 *  system. */
pdf_status_t
pdf_time_set_to_current_utc_time (pdf_time_t time_var);


/* ----------------- Time Span Creation and Destruction --------------------- */

/* Create new time span object */
pdf_time_span_t
pdf_time_span_new (void);

/* Duplicate time span object */
pdf_time_span_t
pdf_time_span_dup (const pdf_time_span_t span);


/* Destroy time span object */
pdf_status_t
pdf_time_span_destroy (pdf_time_span_t *p_span);


/* --------------------- Managing Time Span Values -------------------------- */

/* Set the value of a time span from a 64 bits signed number */
pdf_status_t
pdf_time_span_set (pdf_time_span_t *p_span,
                   const pdf_i32_t high_value,
                   const pdf_u32_t low_value);


/* Set the value of a time span from a 32 bits signed number. */
pdf_status_t
pdf_time_span_set_from_i32 (pdf_time_span_t *p_span,
                            const pdf_i32_t seconds);


/* Change sign of time span */
pdf_status_t
pdf_time_span_negate (pdf_time_span_t *p_span);


/* Add two time spans and store the result in another time span. */
pdf_status_t
pdf_time_span_add (const pdf_time_span_t span1,
                   const pdf_time_span_t span2,
                   pdf_time_span_t *p_result);

/* Copy the value of a time span into another time span. */
pdf_status_t
pdf_time_span_copy (const pdf_time_span_t orig,
                    pdf_time_span_t *p_dest);

/* Difference two time spans and store the result (maybe negative) into another
 *  time span. */
pdf_status_t
pdf_time_span_diff (const pdf_time_span_t span1,
                    const pdf_time_span_t span2,
                    pdf_time_span_t *p_result);

/* Get the value of a time span in seconds. */
pdf_i64_t
pdf_time_span_to_secs (const pdf_time_span_t span);


/* ------------------------- Time Span Comparison --------------------------- */

/* Compare two time spans */
pdf_i32_t
pdf_time_span_cmp (const pdf_time_span_t span1,
                   const pdf_time_span_t span2);


/* ---------------------- Calendar Spans Management ------------------------- */

/* Add two calendar spans. Since the calendar spans are relative (some years
 *  has more days than another) the calendar spans are first resolved from a
 *  base time. */
pdf_status_t
pdf_time_add_cal_span_with_base (const struct pdf_time_cal_span_s *p_span1,
                                 const struct pdf_time_cal_span_s *p_span2,
                                 const pdf_time_t base_time,
                                 struct pdf_time_cal_span_s *p_result);

/* Compare two calendar spans previously resolved with a given base time. */
pdf_i32_t
pdf_time_cal_span_cmp (const struct pdf_time_cal_span_s *p_span1,
                       const struct pdf_time_cal_span_s *p_span2,
                       const pdf_time_t base_time,
                       pdf_status_t *p_ret_code);

/* Compute the difference between two calendar spans relative to a given base
 *  time and store it in a given calendar span. */
pdf_status_t
pdf_time_cal_span_diff (const struct pdf_time_cal_span_s *p_span1,
                        const struct pdf_time_cal_span_s *p_span2,
                        const pdf_time_t base_time,
                        struct pdf_time_cal_span_s *p_result);


/* END PUBLIC */

#endif /* PDF_TIME_H */

/* End of pdf-time.h */

/* -*- mode: C -*-
 *
 *       File:         pdf-time.c
 *       Date:         Mon Apr 28 23:23:04 2008
 *
 *       GNU PDF Library - Time Module source
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

#include <time.h>

#include <pdf-alloc.h>
#include <pdf-time.h>
#include <pdf-time-context.h>
#include <pdf-time-string.h>

/* Useful constants */

#define PDF_SECS_PER_DAY      86400
#define PDF_SECS_PER_HOUR      3600
#define PDF_SECS_PER_MIN         60
#define PDF_MINS_PER_HOUR        60
#define PDF_HOURS_PER_DAY        24
#define PDF_MINS_PER_DAY       1440
#define PDF_DAYS_IN_YEAR        365
#define PDF_DAYS_IN_LEAP_YEAR   366

enum pdf_time_cal_type_e {
  PDF_TIME_CAL_LOCAL,
  PDF_TIME_CAL_UTC
};

enum pdf_time_month_e {
  PDF_TIME_JANUARY    = 1,
  PDF_TIME_FEBRUARY   = 2,
  PDF_TIME_MARCH      = 3,
  PDF_TIME_APRIL      = 4,
  PDF_TIME_MAY        = 5,
  PDF_TIME_JUNE       = 6,
  PDF_TIME_JULY       = 7,
  PDF_TIME_AUGUST     = 8,
  PDF_TIME_SEPTEMBER  = 9,
  PDF_TIME_OCTOBER    = 10,
  PDF_TIME_NOVEMBER   = 11,
  PDF_TIME_DECEMBER   = 12,
  PDF_TIME_NMONTHS
};

/* Returns PDF_TRUE if the Olimpic Games [were/will be] celebrated in the given
 *  year. A leap year is divisable by 4, but not by 100, except if divisable by
 *  400 */
#define IS_LEAP_YEAR(year)                      \
  (((year % 4 == 0) &&                          \
    ((year % 100 != 0) ||                       \
     (year % 400 == 0))) ?                      \
   PDF_TRUE : PDF_FALSE)

/* Returns number of days in the given month (year-dependent due to leap
 * years) */
static pdf_u32_t
pdf_time_get_days_in_month (const pdf_u32_t             year,
                            const enum pdf_time_month_e month)
{
  switch (month)
  {
    case PDF_TIME_JANUARY:
    case PDF_TIME_MARCH:
    case PDF_TIME_MAY:
    case PDF_TIME_JULY:
    case PDF_TIME_AUGUST:
    case PDF_TIME_OCTOBER:
    case PDF_TIME_DECEMBER:
      return 31;
    case PDF_TIME_APRIL:
    case PDF_TIME_JUNE:
    case PDF_TIME_SEPTEMBER:
    case PDF_TIME_NOVEMBER:
      return 30;
    case PDF_TIME_FEBRUARY:
      return (IS_LEAP_YEAR (year) ? 29 : 28);
    default:
      return 0;
  }
}

/* Returns number of days before the given month */
static pdf_u32_t
pdf_time_get_days_before_month (const pdf_u32_t            year,
                               const enum pdf_time_month_e month)
{
  enum pdf_time_month_e walk = PDF_TIME_JANUARY;
  pdf_u32_t sum = 0;

  while (walk != month) {
    sum += pdf_time_get_days_in_month (year, walk++);
  }

  return sum;
}

/* Get Break-Down calendar from pdf_time_t */
static void
pdf_time_get_cal (const pdf_time_t               *time_var,
                  const enum pdf_time_cal_type_e  cal_type,
                  struct pdf_time_cal_s          *cal_time)
{
  /* Based on glibc's __offtime function */

  pdf_i32_t days;
  pdf_i32_t remaining;
  pdf_i32_t years;
  pdf_i32_t months;
  pdf_time_t new_time_var;

  /* Duplicate time var */
  pdf_time_init_dup (&new_time_var, time_var);

  /* pdf_time_t always stores the date & time in UTC timescale, so we only need
   *  to modify the time_var IF gmt_offset is not zero, in order to move the
   *  date & time from UTC to local time specified by the offset */
  if ((cal_type == PDF_TIME_CAL_LOCAL) &&
      (time_var->gmt_offset != 0))
    {
      pdf_time_add_span (&new_time_var, time_var->gmt_offset);
    }

  /* Get date as days */
  days = (pdf_i32_t) (new_time_var.seconds / PDF_SECS_PER_DAY);
  /* Get time in seconds */
  remaining = (pdf_i32_t) (new_time_var.seconds % PDF_SECS_PER_DAY);
  /* Get hours */
  cal_time->hour = remaining / PDF_SECS_PER_HOUR;
  /* Get remaining */
  remaining = remaining % PDF_SECS_PER_HOUR;
  /* Get minutes */
  cal_time->minute = remaining / PDF_MINS_PER_HOUR;
  /* Get seconds */
  cal_time->second = remaining % PDF_MINS_PER_HOUR;

  /* Seems that Unix origin time was thursday */
  cal_time->dow = ((days + 4) % 7);

  years = 1970;

  while ((days < 0) ||
         (days >= (IS_LEAP_YEAR (years) ?
                   (PDF_DAYS_IN_YEAR + 1) :
                   (PDF_DAYS_IN_YEAR))))
    {
      pdf_i32_t yg;

      yg = years;
      /* Compute number of years (assuming all years of 365 days) between the
       *  origin and our date */
      yg += (days / PDF_DAYS_IN_YEAR);
      /* Get number of remaining days after having added the fixed-size years
         If the number of remaining days is less than zero, go down 1 year */
      yg -= ((days % PDF_DAYS_IN_YEAR) < 0);

#define LEAPS_THRU_END_OF(y) ((y) / 4 - (y) / 100 + (y) / 400)
      /* Remove number of days due to the leap years */
      days -= (((yg - years)*PDF_DAYS_IN_YEAR) +
               (LEAPS_THRU_END_OF (yg - 1)) -
               (LEAPS_THRU_END_OF (years - 1)));
      years = yg;
    }

  /* Set year */
  cal_time->year = years; /* - 1900; */

  for (months = 11;
       days < pdf_time_get_days_before_month (cal_time->year,months);
       --months)
    continue;

  days -= pdf_time_get_days_before_month (cal_time->year,months);

  /* Set month and day of month */
  cal_time->month = months;
  if (pdf_time_get_days_in_month (cal_time->year,
                                  cal_time->month) < (days + 1))
    {
      cal_time->day = days + 1 - pdf_time_get_days_in_month (cal_time->year,
                                                             cal_time->month);
      cal_time->month += 1;
      if (cal_time->month == 13)
        {
          cal_time->month = 1;
          cal_time->year += 1;
        }
    }
  else
    {
      cal_time->day = days + 1;
    }

  /* Finally, set gmt offset */
  cal_time->gmt_offset = new_time_var.gmt_offset;

  pdf_time_deinit (&new_time_var);
}

/* Function to normalize a given date after having added YEARS */
static void
pdf_time_calendar_add_years (struct pdf_time_cal_s *p_calendar,
                             const pdf_i32_t        delta_years)
{
  pdf_i32_t years;

  if (delta_years == 0)
    return;

  /* ADD years */
  years = p_calendar->year + delta_years;

  /* The only thing to normalize is in case we reach Feb.29 in a non-leap
   *  year */
  if ((!IS_LEAP_YEAR (years)) &&
      (p_calendar->month == PDF_TIME_FEBRUARY) &&
      (p_calendar->day == 29))
    {
      p_calendar->day = 28;
    }

  PDF_ASSERT (years >= 0);
  p_calendar->year = years;
}

/* Function to normalize a given date after having added MONTHS */
static void
pdf_time_calendar_add_months (struct pdf_time_cal_s *p_calendar,
                              const pdf_i32_t        delta_months)
{
  if (delta_months == 0)
    return;

  if (delta_months > 0)
    {
      p_calendar->month += delta_months;
      while (p_calendar->month > 12)
        {
          p_calendar->month -= 12;
          p_calendar->year++;
        }
    }
  else
    {
      pdf_i32_t months;

      months = p_calendar->month + delta_months;
      while (months < 1)
        {
          months += 12;
          p_calendar->year--;
        }

      PDF_ASSERT (months >= 1);
      PDF_ASSERT (months <= 12);
      p_calendar->month = months;
    }

  /* After having added months, we could need to normalize the days */
  if (pdf_time_get_days_in_month (p_calendar->year,
                                  p_calendar->month) < p_calendar->day)
    {
      p_calendar->day = pdf_time_get_days_in_month (p_calendar->year,
                                                    p_calendar->month);
    }

  PDF_ASSERT (p_calendar->month >= 1);
  PDF_ASSERT (p_calendar->month <= 12);
}

/* Function to normalize a given date after having added DAYS */
static void
pdf_time_calendar_add_days (struct pdf_time_cal_s *p_calendar,
                            const pdf_i32_t        delta_days)
{
  pdf_i32_t delta = delta_days;

  if (delta_days == 0)
    return;

  /* ADD days */
  if (delta_days > 0)
    {
      pdf_i32_t days_in_month;

      /* Initialize days in month */
      days_in_month = pdf_time_get_days_in_month (p_calendar->year,
                                                  (enum pdf_time_month_e)p_calendar->month);
      while (delta > (days_in_month - p_calendar->day))
        {
          /* Go to start of next month */
          p_calendar->day = 1;
          pdf_time_calendar_add_months (p_calendar, 1);

          /* Update remaining delta and new days_in_month */
          delta -= (days_in_month - p_calendar->day + 1);
          days_in_month = pdf_time_get_days_in_month (p_calendar->year,
                                                      (enum pdf_time_month_e)p_calendar->month);
        }
      /* Add final delta, which doesn't require month change */
      p_calendar->day += delta;
    }
  /* SUBSTRACT days */
  else
    {
      pdf_i32_t days_in_month;

      /* Initialize days in month */
      days_in_month = p_calendar->day - 1;

      while (days_in_month <= (-delta))
        {
          /* Go to the begin of previous month */
          p_calendar->day = 1;
          pdf_time_calendar_add_months (p_calendar, -1);

          /* Update remaining delta and new days_in_month */
          delta += days_in_month;
          days_in_month = pdf_time_get_days_in_month (p_calendar->year,
                                                      (enum pdf_time_month_e)p_calendar->month);
        }
      /* Add final delta, which doesn't require month change */
      delta += days_in_month;
      p_calendar->day += delta;
    }

  PDF_ASSERT (p_calendar->month >= 1);
  PDF_ASSERT (p_calendar->month <= 12);
}

/* Function to normalize a given date after having added HOURS */
static void
pdf_time_calendar_add_hours (struct pdf_time_cal_s *p_calendar,
                             const pdf_i32_t        delta_hours)
{
  pdf_i32_t hours;
  pdf_i32_t days;
  pdf_i32_t remaining_hours;

  if (delta_hours == 0)
    return;

  /* No real problem with hours, as 1 day is always 24h */
  days = delta_hours / PDF_HOURS_PER_DAY;
  remaining_hours = delta_hours % PDF_HOURS_PER_DAY;

  /* Add remaining hours */
  hours = p_calendar->hour + remaining_hours;

  /* If we went back to the previous day, correct time and add 1 day more
   * to remove */
  if (hours < 0)
    {
      hours += PDF_HOURS_PER_DAY;
      days--;
    }
  /* If we went forward to the next day, correct time and add 1 day more
   * to add */
  else if (hours >= PDF_HOURS_PER_DAY)
    {
      hours -= PDF_HOURS_PER_DAY;
      days++;
    }

  PDF_ASSERT (hours >= 0);
  PDF_ASSERT (hours < 24);
  p_calendar->hour = hours;

  /* Add/Remove days... */
  pdf_time_calendar_add_days (p_calendar, days);
}

/* Function to normalize a given date after having added MINUTES */
static void
pdf_time_calendar_add_minutes (struct pdf_time_cal_s *p_calendar,
                               const pdf_i32_t        delta_minutes)
{
  pdf_i32_t minutes;
  pdf_i32_t hours;
  pdf_i32_t remaining_minutes;

  if (delta_minutes == 0)
    return;

  /* No real problem with minutes, as 1 hour is always 60minutes */
  hours = delta_minutes / PDF_MINS_PER_HOUR;
  remaining_minutes = delta_minutes % PDF_MINS_PER_HOUR;

  /* Add remaining minutes */
  minutes = p_calendar->minute + remaining_minutes;

  /* If we went back to the previous hour, correct time and add 1 hour more
   * to remove */
  if (minutes < 0)
    {
      minutes += PDF_MINS_PER_HOUR;
      hours--;
    }
  /* If we went forward to the next day, correct time and add 1 hour more
   * to add */
  else if (minutes >= PDF_MINS_PER_HOUR)
    {
      minutes -= PDF_MINS_PER_HOUR;
      hours++;
    }

  PDF_ASSERT (minutes >= 0);
  PDF_ASSERT (minutes < 60);
  p_calendar->minute = minutes;

  /* Add/Remove hours... */
  pdf_time_calendar_add_hours (p_calendar, hours);
}

/* Function to normalize a given date after having added SECONDS */
static void
pdf_time_calendar_add_seconds (struct pdf_time_cal_s *p_calendar,
                               const pdf_i32_t        delta_seconds)
{
  pdf_i32_t seconds;
  pdf_i32_t minutes;
  pdf_i32_t remaining_seconds;

  if (delta_seconds == 0)
    return;

  /* No real problem with minutes, as 1 hour is always 60minutes */
  minutes = delta_seconds / PDF_SECS_PER_MIN;
  remaining_seconds = delta_seconds % PDF_SECS_PER_MIN;

  /* Add remaining seconds */
  seconds = p_calendar->second + remaining_seconds;

  /* If we went back to the previous minute, correct time and add 1 minute more
   * to remove */
  if (seconds < 0)
    {
      seconds += PDF_SECS_PER_MIN;
      minutes--;
    }
  /* If we went forward to the next minute, correct time and add 1 minute more
   * to add */
  else if (seconds >= PDF_SECS_PER_MIN)
    {
      seconds -= PDF_SECS_PER_MIN;
      minutes++;
    }

  PDF_ASSERT (seconds >= 0);
  PDF_ASSERT (seconds < 60);
  p_calendar->second = seconds;

  /* Add/Remove minutes... */
  pdf_time_calendar_add_minutes (p_calendar, minutes);
}

/* --------------------- Time Module Initialization ------------------------- */

/* Initialize Time module. */
pdf_bool_t
pdf_time_module_init (pdf_error_t **error)
{
  /* Start Time context */
  return pdf_time_context_init (error);
}

/* --------------------- Time Creation and Destruction ---------------------- */

static pdf_time_t *
pdf_time_allocate (pdf_error_t **error)
{
  pdf_time_t *time_var;

  /* Allocate object & Initialize all contents */
  time_var = (pdf_time_t *) pdf_alloc (sizeof(struct pdf_time_s));
  if (!time_var)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TIME,
                     PDF_ENOMEM,
                     "cannot create new time object: "
                     "couldn't allocate %lu bytes",
                     (unsigned long)sizeof (struct pdf_time_s));
    }

  return time_var;
}

/* Create new pdf_time_t object in heap */
pdf_time_t *
pdf_time_new (pdf_error_t **error)
{
  pdf_time_t *time_var;

  time_var = pdf_time_allocate (error);
  if (time_var)
    pdf_time_clear (time_var);

  return time_var;
}

/* Duplicate pdf_time_t object in heap */
pdf_time_t *
pdf_time_dup (const pdf_time_t  *time_var_orig,
              pdf_error_t      **error)
{
  pdf_time_t *time_var;

  PDF_ASSERT_POINTER_RETURN_VAL (time_var_orig, NULL);

  time_var = pdf_time_allocate (error);
  if (time_var)
    pdf_time_copy (time_var, time_var_orig);

  return time_var;
}

/* Destroy a pdf_time_t object from heap */
void
pdf_time_destroy (pdf_time_t *time_var)
{
  if (time_var)
    pdf_dealloc (time_var);
}

/* Initializes a preallocated (heap or stack) pdf_time_t */
void
pdf_time_init (pdf_time_t *time_var)
{
  PDF_ASSERT_POINTER_RETURN (time_var);

  pdf_time_clear (time_var);
}

/* Initializes a preallocated (heap or stack) pdf_time_t with same contents as
 * another given pdf_time_t. Just a sequence of
 * pdf_time_init() + pdf_time_copy() */
void
pdf_time_init_dup (pdf_time_t       *time_var,
                   const pdf_time_t *time_var_orig)
{
  PDF_ASSERT_POINTER_RETURN (time_var_orig);
  PDF_ASSERT_POINTER_RETURN (time_var);

  pdf_time_copy (time_var, time_var_orig);
}

void
pdf_time_deinit (pdf_time_t *time_var)
{
  /* Currently a no-op */
}

/* ------------------------- Managing Time Values --------------------------- */

/* Copy the contents of a given pdf_time_t object */
void
pdf_time_copy (pdf_time_t       *copy,
               const pdf_time_t *orig)
{
  PDF_ASSERT_POINTER_RETURN (orig);
  PDF_ASSERT_POINTER_RETURN (copy);

  copy->seconds = orig->seconds;
  copy->gmt_offset = orig->gmt_offset;
}

/* Clear contents of the pdf_time_t object */
void
pdf_time_clear (pdf_time_t *time_var)
{
  /* Set time as January 1st, 1970 */
  time_var->seconds = 0;
  /* UTC */
  time_var->gmt_offset = 0;
}

/* Set time value with a pdf_i64_t variable representing UTC Unix time */
void
pdf_time_set_utc (pdf_time_t *time_var,
                  pdf_i64_t   utc_seconds)
{
  PDF_ASSERT_POINTER_RETURN (time_var);

  time_var->seconds = utc_seconds;
  time_var->gmt_offset = 0;
}

/* Set time value with a pdf_i64_t variable representing Local Unix time */
void
pdf_time_set_local (pdf_time_t *time_var,
                    pdf_i64_t   local_seconds,
                    pdf_i32_t   local_gmt_offset)
{
  PDF_ASSERT_POINTER_RETURN (time_var);

  /* pdf_time_t always stores seconds in UTC */
  time_var->seconds = local_seconds - local_gmt_offset;
  time_var->gmt_offset = local_gmt_offset;
}

#ifdef PDF_HOST_WIN32
/* Windows-specific function to set the time with a FILETIME structure */
pdf_bool_t
pdf_time_w32_set_from_filetime (pdf_time_t      *time_var,
                                const FILETIME  *filetime,
                                pdf_error_t    **error)
{
  struct pdf_time_cal_s calendar;
  SYSTEMTIME systemtime;

  /*
    BOOL WINAPI
    FileTimeToSystemTime(const FILETIME *lpFileTime,
                         LPSYSTEMTIME lpSystemTime);

    If the function succeeds, the return value is nonzero.

    typedef struct _SYSTEMTIME {
      WORD wYear;
      WORD wMonth;        (1: january, 2: february...)
      WORD wDayOfWeek;    (0:sunday, 1: monday...)
      WORD wDay;
      WORD wHour;
      WORD wMinute;
      WORD wSecond;
      WORD wMilliseconds;
    } SYSTEMTIME;

   */

  /* Convert filetime to systemtime calendar */
  if (FileTimeToSystemTime (p_filetime, &systemtime) == 0)
    {
      /* TODO: Use FormatMessage() to include a string with the exact error
       * in the pdf_error_t */
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TIME,
                     PDF_ERROR,
                     "couldn't get system time from FILETIME: %d"
                     (pdf_i32_t)GetLastError ());
      return PDF_FALSE;
    }

  /* Convert from systemtime calendar to our pdf_time_cal_s */
  calendar.year = systemtime.wYear;
  calendar.month = systemtime.wMonth;
  calendar.day = systemtime.wDay;
  calendar.hour = systemtime.wHour;
  calendar.minute = systemtime.wMinute;
  calendar.second = systemtime.wSecond;
  /* Round seconds if milliseconds available */
  if (systemtime.wMilliseconds >= 500)
    calendar.second++;

  /* Store as UTC? */
  calendar.gmt_offset = 0;

  /* For us, sunday is 7 */
  calendar.dow = (systemtime.wDayOfWeek == 0) ? 7 : systemtime.wDayOfWeek;

  return pdf_time_set_from_cal (time_var, &calendar, error);
}
#endif

/*
*  Based on the work done by Perl guys in DateTime:
*  http://search.cpan.org/dist/DateTime/lib/DateTime.pm#Adding_a_Duration_to_a_Datetime
*
*  "DateTime.pm always adds (or subtracts) days, then months, minutes, and then
*   seconds and nanoseconds. If there are any boundary overflows, these are
*   normalized at each step. For the days and months (the calendar) the local
*   (not UTC) values are used. For minutes and seconds, the local values are
*   used. This generally just works.
*
*   This means that adding one month and one day to February 28, 2003 will
*   produce the date April 1, 2003, not March 29, 2003."
*
* Thanks to S.Jansen for the link!
*/
static void
pdf_time_add_cal_span_with_sign (pdf_time_t                       *time_var,
                                 const struct pdf_time_cal_span_s *cal_span,
                                 pdf_bool_t                        negate)
{
  struct pdf_time_cal_s calendar;
  pdf_i32_t sign;

  sign = (negate ? -1 : 1);

  /* Create Calendar type from the time object */
  pdf_time_get_utc_cal (time_var, &calendar);
  pdf_time_calendar_add_days    (&calendar,  sign * cal_span->days);
  pdf_time_calendar_add_months  (&calendar,  sign * cal_span->months);
  pdf_time_calendar_add_years   (&calendar,  sign * cal_span->years);
  pdf_time_calendar_add_hours   (&calendar,  sign * cal_span->hours);
  pdf_time_calendar_add_minutes (&calendar,  sign * cal_span->minutes);
  pdf_time_calendar_add_seconds (&calendar,  sign * cal_span->seconds);

  /* Check for possible underflow condition */
  if (calendar.year < 1970)
    {
      /* If underflow, reset the time to Unix time origin */
      pdf_time_clear (time_var);
      return;
    }

  /* Generate time from the new calendar */
  pdf_time_set_from_cal (time_var, &calendar);
}

/* Add the time span represented by cal_span to the text object. */
void
pdf_time_add_cal_span (pdf_time_t                       *time_var,
                       const struct pdf_time_cal_span_s *cal_span)
{
  PDF_ASSERT_POINTER_RETURN (time_var);
  PDF_ASSERT_POINTER_RETURN (cal_span);

  pdf_time_add_cal_span_with_sign (time_var,
                                   cal_span,
                                   PDF_FALSE);
}

/* Substract the time span represented by cal_span from the text object */
void
pdf_time_sub_cal_span (pdf_time_t                       *time_var,
                       const struct pdf_time_cal_span_s *cal_span)
{
  PDF_ASSERT_POINTER_RETURN (time_var);
  PDF_ASSERT_POINTER_RETURN (cal_span);

  pdf_time_add_cal_span_with_sign (time_var,
                                   cal_span,
                                   PDF_TRUE);
}

/* Add the time span contained in time_span to time. As the time span is stored
 * in seconds, the adding is direct. */
void
pdf_time_add_span (pdf_time_t *time_var,
                   pdf_i32_t   time_span)
{
  PDF_ASSERT_POINTER_RETURN (time_var);
  PDF_ASSERT_RETURN (PDF_U64_MAX - time_var->seconds >= time_span);

  time_var->seconds += time_span;
}

/* Subtract the time span contained in time_span to time. As the time span is
 * stored in seconds. */
void
pdf_time_sub_span (pdf_time_t *time_var,
                   pdf_i32_t   time_span)
{
  PDF_ASSERT_POINTER_RETURN (time_var);
  PDF_ASSERT_RETURN (time_var->seconds >= time_span);

  time_var->seconds -= time_span;
}

/* Fill local_cal with the local calendar time of object. */
void
pdf_time_get_local_cal (const pdf_time_t      *time_var,
                        struct pdf_time_cal_s *local_cal)
{
  PDF_ASSERT_POINTER_RETURN (time_var);
  PDF_ASSERT_POINTER_RETURN (local_cal);

  pdf_time_get_cal (time_var,
                    PDF_TIME_CAL_LOCAL,
                    local_cal);
}

/* Get the UTC calendar time of a given time variable. */
void
pdf_time_get_utc_cal (const pdf_time_t      *time_var,
                      struct pdf_time_cal_s *utc_cal)
{
  PDF_ASSERT_POINTER_RETURN (time_var);
  PDF_ASSERT_POINTER_RETURN (utc_cal);

  pdf_time_get_cal (time_var,
                    PDF_TIME_CAL_UTC,
                    utc_cal);
}

/* Set the value of a time variable to a given calendar time. */
void
pdf_time_set_from_cal (pdf_time_t                  *time_var,
                       const struct pdf_time_cal_s *cal)
{
  pdf_u64_t aux;
  pdf_i32_t walker;

  PDF_ASSERT_POINTER_RETURN (time_var);
  PDF_ASSERT_POINTER_RETURN (cal);

  /* Wrong calendar contents are treated as programmer errors */
  PDF_ASSERT_RETURN (cal->year >= 1970);
  PDF_ASSERT_RETURN (cal->month >= 1);
  PDF_ASSERT_RETURN (cal->month <= 12);
  PDF_ASSERT_RETURN (cal->day > 0);
  PDF_ASSERT_RETURN (cal->day <= pdf_time_get_days_in_month (cal->year, cal->month));
  PDF_ASSERT_RETURN (cal->hour < 24);
  PDF_ASSERT_RETURN (cal->minute < 60);
  PDF_ASSERT_RETURN (cal->second < 60);

  /* Note: There is no possible overflow with this procedure to
   * get Unix time from calendar (year is 2-bytes max) */

  /* Add days per year until the current year in the calendar */
  aux = 0;
  for (walker = 1970; walker < cal->year; walker++)
    {
      aux += (IS_LEAP_YEAR (walker) ?
              PDF_DAYS_IN_LEAP_YEAR : PDF_DAYS_IN_YEAR);
    }

  /* Add days per month until the current month in the calendar */
  aux += pdf_time_get_days_before_month (cal->year, cal->month);

  /* Add days in current month until the current required day */
  aux += (cal->day - 1);

  /* Set date as seconds in the output variable */
  time_var->seconds = aux * PDF_SECS_PER_DAY;

  /* Add hours as seconds */
  time_var->seconds += (cal->hour * PDF_SECS_PER_HOUR);

  /* Add minutes as seconds */
  time_var->seconds += (cal->minute * PDF_SECS_PER_MIN);

  /* Finally, add seconds */
  time_var->seconds += cal->second;

  /* Set specific GMT offset if any */
  if (cal->gmt_offset != 0)
    {
      if (cal->gmt_offset > time_var->seconds)
        {
          /* Avoid the underflow, by just resetting seconds to zero.
           * This is very unlikely to happen, so no worth having a
           * pdf_error_t arg just for this. */
          time_var->seconds = 0;
        }
      else
        {
          /* pdf_time_t always stores seconds in UTC */
          time_var->seconds -= cal->gmt_offset;
        }
    }

  /* Store the offset */
  time_var->gmt_offset = cal->gmt_offset;
}

/* Set the local time offset of time to the one used by the operating system. */
void
pdf_time_set_local_offset (pdf_time_t *time_var)
{
  PDF_ASSERT_POINTER_RETURN (time_var);

  /* Set local GMT offset */
  time_var->gmt_offset = pdf_time_context_get_gmt_offset ();
}

/* ----------------------- Getting Time Intervals --------------------------- */

/* Get time interval as Calendar Span */
void
pdf_time_diff_cal (const pdf_time_t           *time1,
                   const pdf_time_t           *time2,
                   struct pdf_time_cal_span_s *cal_span)
{
  struct pdf_time_cal_s calendar1;
  struct pdf_time_cal_s calendar2;
  struct pdf_time_cal_s *p_big;
  struct pdf_time_cal_s *p_small;
  pdf_i32_t aux;

  PDF_ASSERT_POINTER_RETURN (time1);
  PDF_ASSERT_POINTER_RETURN (time2);
  PDF_ASSERT_POINTER_RETURN (cal_span);

  pdf_time_get_cal (time1,
                    PDF_TIME_CAL_UTC,
                    &calendar1);
  pdf_time_get_cal (time2,
                    PDF_TIME_CAL_UTC,
                    &calendar2);

  /* Check which of the dates is bigger */
  if (pdf_time_cmp (time1, time2) > 0)
    {
      cal_span->sign = PDF_TRUE;
      p_big = &calendar1;
      p_small = &calendar2;
    }
  else
    {
      cal_span->sign = PDF_FALSE;
      p_big = &calendar2;
      p_small = &calendar1;
    }

  /* Get diff of years directly (always + or 0) */
  cal_span->years = p_big->year - p_small->year;

#define DIFF_AND_CORRECT(field,spanfield,spanupperfield)     \
  do {                                                       \
    aux = p_big->field - p_small->field;                     \
    if (aux < 0) {                                           \
      cal_span->spanupperfield--;                            \
      cal_span->spanfield = (-1) * aux;                      \
    } else {                                                 \
      cal_span->spanfield = aux;                             \
    }                                                        \
  } while(0)

  /* Get diff of months (could be -) */
  DIFF_AND_CORRECT (month,   months,   years);
  /* Get diff of days (could be -) */
  DIFF_AND_CORRECT (day,     days,     months);
  /* Get diff of hours (could be -) */
  DIFF_AND_CORRECT (hour,    hours,    days);
  /* Get diff of minutes (could be -) */
  DIFF_AND_CORRECT (minute,  minutes,  hours);
  /* Get diff of seconds (could be -) */
  DIFF_AND_CORRECT (second,  seconds,  minutes);

#undef DIFF_AND_CORRECT
}

/* Get time interval as Span */
pdf_i64_t
pdf_time_diff (const pdf_time_t *time1,
               const pdf_time_t *time2)
{
  PDF_ASSERT_POINTER_RETURN_VAL (time1, -1);
  PDF_ASSERT_POINTER_RETURN_VAL (time2, -1);

  return  ((pdf_i64_t)time1->seconds - (pdf_i64_t)time2->seconds);
}

/* ---------------------------- Time comparison ----------------------------- */

/* Compares two times. */
pdf_i8_t
pdf_time_cmp (const pdf_time_t *time1,
              const pdf_time_t *time2)
{
  if (time1->seconds > time2->seconds)
    return 1;
  if (time1->seconds < time2->seconds)
    return -1;
  return 0;
}

/* ---------------------- Time printing and parsing ------------------------- */

/* Create a string representation of a given time. */
pdf_char_t *
pdf_time_to_string (const pdf_time_t                     *time_var,
                    const enum pdf_time_string_format_e   time_format,
                    pdf_u32_t                             options,
                    pdf_error_t                         **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (time_var, NULL);
  PDF_ASSERT_RETURN_VAL (time_format >= PDF_TIME_STRING_FORMAT_PDF, NULL);
  PDF_ASSERT_RETURN_VAL (time_format <= PDF_TIME_STRING_FORMAT_GENERALIZED_ASN1, NULL);

  switch (time_format)
    {
    case PDF_TIME_STRING_FORMAT_PDF:
      return pdf_time_to_string_pdf (time_var,
                                     (options & PDF_TIME_STRING_TRAILING_APOSTROPHE),
                                     error);
    case PDF_TIME_STRING_FORMAT_ISO_8601:
      return pdf_time_to_string_iso8601 (time_var, error);
    case PDF_TIME_STRING_FORMAT_UTC_ASN1:
      return pdf_time_to_string_utc_asn1 (time_var, error);
    case PDF_TIME_STRING_FORMAT_GENERALIZED_ASN1:
      return pdf_time_to_string_generalized_asn1 (time_var, error);
    default:
      /* Make compiler happy */
      return NULL;
    }
}

/* Get a string containing a time specification in some format and fill a time
 *  variable with the parsed values. */
pdf_bool_t
pdf_time_set_from_string (pdf_time_t                           *time_var,
                          const pdf_char_t                     *time_str,
                          const enum pdf_time_string_format_e   time_format,
                          pdf_u32_t                             options,
                          pdf_error_t                         **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (time_var, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (time_str, PDF_FALSE);
  PDF_ASSERT_RETURN_VAL (time_format >= PDF_TIME_STRING_FORMAT_PDF, PDF_FALSE);
  PDF_ASSERT_RETURN_VAL (time_format <= PDF_TIME_STRING_FORMAT_GENERALIZED_ASN1, PDF_FALSE);

  switch (time_format)
    {
    case PDF_TIME_STRING_FORMAT_PDF:
      return pdf_time_from_string_pdf (time_var,
                                       time_str,
                                       (options & PDF_TIME_STRING_TRAILING_APOSTROPHE),
                                       error);
    case PDF_TIME_STRING_FORMAT_ISO_8601:
      return pdf_time_from_string_iso8601 (time_var, time_str, error);
    case PDF_TIME_STRING_FORMAT_UTC_ASN1:
      return pdf_time_from_string_utc_asn1 (time_var, time_str, error);
    case PDF_TIME_STRING_FORMAT_GENERALIZED_ASN1:
      return pdf_time_from_string_generalized_asn1 (time_var, time_str, error);
    default:
      /* Make compiler happy */
      return PDF_FALSE;
    }
}

/* ---------------------- Getting the Current Time -------------------------- */

/* Set the value of object to the current local time used by the operating
 *  system. */
void
pdf_time_set_to_current_local_time (pdf_time_t *time_var)
{
  /* time() always returns UTC */
  time_var->seconds = time (NULL);
  /* Store offset in the gmt_offset */
  time_var->gmt_offset = pdf_time_context_get_gmt_offset ();
}

/* Set the value of object to the current UTC time used by the operating
 *  system. */
void
pdf_time_set_to_current_utc_time (pdf_time_t *time_var)
{
  /* time() always returns UTC */
  time_var->seconds = time (NULL);
  time_var->gmt_offset = 0;
}

/* ---------------------- Calendar Spans Management ------------------------- */

static pdf_i32_t
pdf_time_span_from_cal_span (const struct pdf_time_cal_span_s *cal_span,
                             const pdf_time_t                 *base_time)
{
  pdf_i64_t span;
  pdf_time_t new_time;

  /* Duplicate base object and add calendar span */
  pdf_time_init_dup (&new_time, base_time);
  pdf_time_add_cal_span (&new_time, cal_span);

  /* Get the difference in seconds */
  span = pdf_time_diff (&new_time, base_time);

  pdf_time_deinit (&new_time);

  return span;
}

static void
pdf_time_span_to_cal_span (pdf_i32_t                   span,
                           const pdf_time_t           *base_time,
                           struct pdf_time_cal_span_s *cal_span)
{
  pdf_time_t new_time;

  /* Duplicate base object and add calendar span,
   *  and get both initial and new times as calendars
   */
  pdf_time_init_dup (&new_time, base_time);
  pdf_time_add_span (&new_time, span);

  /* Now, directly get calendar diff */
  pdf_time_diff_cal (base_time, &new_time, cal_span);

  pdf_time_deinit (&new_time);
}

/* Add two calendar spans. Since the calendar spans are relative (some years
 *  has more days than another) the calendar spans are first resolved from a
 *  base time to get the number of seconds, and then that number is stored in
 *  the resulting calendar span */
void
pdf_time_add_cal_span_with_base (const struct pdf_time_cal_span_s *span1,
                                 const struct pdf_time_cal_span_s *span2,
                                 const pdf_time_t                 *base_time,
                                 struct pdf_time_cal_span_s       *result)
{
  pdf_i32_t span_time;

  PDF_ASSERT_POINTER_RETURN (span1);
  PDF_ASSERT_POINTER_RETURN (span2);
  PDF_ASSERT_POINTER_RETURN (base_time);
  PDF_ASSERT_POINTER_RETURN (result);

  span_time = pdf_time_span_from_cal_span (span1, base_time);
  span_time += pdf_time_span_from_cal_span (span2, base_time);

  /* Get calendar span from the new result */
  pdf_time_span_to_cal_span (span_time, base_time, result);
}

/* Compare two calendar spans previously resolved with a given base time. */
pdf_i8_t
pdf_time_cal_span_cmp (const struct pdf_time_cal_span_s *span1,
                       const struct pdf_time_cal_span_s *span2,
                       const pdf_time_t                 *base_time)
{
  /* Probably the best way to do it is convert the cal spans into pdf_time_t
   * and then compare the pdf_time_t objects */
  pdf_time_t time1;
  pdf_time_t time2;
  pdf_i8_t cmp_ret;

  PDF_ASSERT_POINTER_RETURN_VAL (span1, -1);
  PDF_ASSERT_POINTER_RETURN_VAL (span2, -1);
  PDF_ASSERT_POINTER_RETURN_VAL (base_time, -1);

  pdf_time_init_dup (&time1, base_time);
  pdf_time_init_dup (&time2, base_time);

  pdf_time_add_cal_span (&time1, span1);
  pdf_time_add_cal_span (&time2, span2);

  cmp_ret = pdf_time_cmp (&time1, &time2);

  pdf_time_deinit (&time1);
  pdf_time_deinit (&time2);

  return cmp_ret;
}

/* Compute the difference between two calendar spans relative to a given base
 *  time and store it in a given calendar span. */
void
pdf_time_cal_span_diff (const struct pdf_time_cal_span_s *span1,
                        const struct pdf_time_cal_span_s *span2,
                        const pdf_time_t                 *base_time,
                        struct pdf_time_cal_span_s       *result)
{
  pdf_i32_t span_time;

  span_time = pdf_time_span_from_cal_span (span1, base_time);
  span_time -= pdf_time_span_from_cal_span (span2, base_time);

  /* Get calendar span from the new result */
  pdf_time_span_to_cal_span (span_time, base_time, result);
}

/* End of pdf-time.c */

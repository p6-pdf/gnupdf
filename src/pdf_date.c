/* -*- mode: C -*- Time-stamp: "07/09/07 19:58:46 jemarch"
 *
 *       File:         pdf_date.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Wed Sep  5 23:38:45 2007
 *
 *       GNU PDF Library - Date common data type
 *
 */

/* Copyright (C) 2007 Jose E. Marchesi */

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

#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <xalloc.h>
#include <pdf_date.h>

static inline pdf_date_t pdf_date_alloc (void);
static inline void pdf_date_dealloc (pdf_date_t date);


pdf_date_t 
pdf_date_new_value (int year,
                    int month,
                    int day,
                    int hour,
                    int minute,
                    int second,
                    int ut_offset_hours,
                    int ut_offset_minutes)
{
  pdf_date_t new_date;

  /* Sanity check */
  if ((month < 1) || (month > 12) || 
      (day < 1) || (day > 31) || 
      (hour < 0) || (hour > 23) || 
      (minute < 0) || (minute > 59) || 
      (second < 0) || (second > 59) ||
      ((ut_offset_hours * ut_offset_minutes) < 0))
    {
      return NULL;
    }

  new_date = pdf_date_alloc ();
  new_date->year = year;
  new_date->month = month;
  new_date->day = day;
  new_date->hour = hour;
  new_date->minute = minute;
  new_date->second = second;

  if (ut_offset_hours < 0)
    {
      new_date->ut_rel = PDF_DATE_UT_BEFORE;
    }
  else if (ut_offset_hours > 0)
    {
      new_date->ut_rel = PDF_DATE_UT_AFTER;
    }
  else
    {
      new_date->ut_rel = PDF_DATE_UT_EQUAL;
    }

  new_date->ut_offset_hours = abs (ut_offset_hours);
  new_date->ut_offset_minutes = abs (ut_offset_minutes);

  return new_date;
}

void
pdf_date_destroy (pdf_date_t date)
{
  pdf_date_dealloc (date);
}

inline int 
pdf_date_year (pdf_date_t date)
{
  return date->year;
}

inline void
pdf_date_set_year (pdf_date_t date, 
                   int year)
{
  date->year = year;
}

inline int
pdf_date_month (pdf_date_t date)
{
  return date->month;
}

inline void
pdf_date_set_month (pdf_date_t date,
                    int month)
{
  date->month = month;
}

inline int
pdf_date_day (pdf_date_t date)
{
  return date->day;
}

inline void
pdf_date_set_day (pdf_date_t date,
                  int day)
{
  date->day = day;
}

inline int 
pdf_date_hour (pdf_date_t date)
{
  return date->hour;
}

inline void
pdf_date_set_hour (pdf_date_t date,
                   int hour)
{
  date->hour = hour;
}

inline int
pdf_date_minute (pdf_date_t date)
{
  return date->minute;
}

inline void
pdf_date_set_minute (pdf_date_t date,
                     int minute)
{
  date->minute = minute;
}

inline int 
pdf_date_second (pdf_date_t date)
{
  return date->second;
}

inline void
pdf_date_set_second (pdf_date_t date,
                     int second)
{
  date->second = second;
}

/* Private functions */

static inline pdf_date_t
pdf_date_alloc (void)
{
  pdf_date_t new_date;
  new_date = (pdf_date_t) xmalloc (sizeof(struct pdf_date_s));

  return new_date;
}

static inline void
pdf_date_dealloc (pdf_date_t date)
{
  free (date);
}

/* End of pdf_date.c */

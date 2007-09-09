/* -*- mode: C -*- Time-stamp: "07/09/09 02:02:32 jemarch"
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
#include <string.h>
#include <pdf_date.h>

static inline pdf_date_t pdf_date_alloc (void);
static inline void pdf_date_dealloc (pdf_date_t date);
static pdf_date_t pdf_date_parse (char *string, int size);

pdf_date_t
pdf_create_date (pdf_obj_t date_string)
{
  pdf_date_t new_date;
  char *str;
  int str_size;

  /* Sanity checks */
  if (!IS_STRING(date_string))
    {
      return NULL;
    }

  str = pdf_get_string_data (date_string);
  str_size = pdf_get_string_size (date_string);
  new_date = pdf_date_parse (str, str_size);
  free (str);

  /* May be NULL in the case of a parse error */
  return new_date;
}

pdf_date_t 
pdf_create_date_value (int year,
                       int month,
                       int day,
                       int hour,
                       int minute,
                       int second,
                       int ut_offset_hour,
                       int ut_offset_minute)
{
  pdf_date_t new_date;

  /* Sanity check */
  if ((month < 1) || (month > 12) || 
      (day < 1) || (day > 31) || 
      (hour < 0) || (hour > 23) || 
      (minute < 0) || (minute > 59) || 
      (second < 0) || (second > 59) ||
      ((ut_offset_hour * ut_offset_minute) < 0))
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

  if (ut_offset_hour < 0)
    {
      new_date->ut_rel = PDF_DATE_UT_BEFORE;
    }
  else if (ut_offset_hour > 0)
    {
      new_date->ut_rel = PDF_DATE_UT_AFTER;
    }
  else
    {
      new_date->ut_rel = PDF_DATE_UT_EQUAL;
    }

  new_date->ut_offset_hour = abs (ut_offset_hour);
  new_date->ut_offset_minute = abs (ut_offset_minute);

  return new_date;
}

void
pdf_destroy_date (pdf_date_t date)
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

inline int
pdf_date_ut_offset_hour (pdf_date_t date)
{
  int ut_offset_hour;

  ut_offset_hour = date->ut_offset_hour;
  switch (date->ut_rel)
    {
    case PDF_DATE_UT_UNKNOWN:
      {
        return -1;
        break;
      }
    case PDF_DATE_UT_BEFORE:
      {
        return ut_offset_hour * -1;
        break;
      }
    case PDF_DATE_UT_AFTER:
      {
        return ut_offset_hour;
        break;
      }
    case PDF_DATE_UT_EQUAL:
      {
        return 0;
        break;
      }
    }

  /* Not reached */
  return -2;
}

inline int
pdf_date_ut_offset_minute (pdf_date_t date)
{
  int ut_offset_minute;

  ut_offset_minute = date->ut_offset_minute;
  switch (date->ut_rel)
    {
    case PDF_DATE_UT_UNKNOWN:
      {
        return -1;
        break;
      }
    case PDF_DATE_UT_BEFORE:
      {
        return ut_offset_minute * -1;
        break;
      }
    case PDF_DATE_UT_EQUAL:
      {
        return 0;
        break;
      }
    case PDF_DATE_UT_AFTER:
      {
        return ut_offset_minute;
        break;
      }
    }

  /* Not reached */

  return -2;
}

/* Private functions */

#define DATE_EXTRACT_INT(NUM_DIGITS,VAR)                 \
  do                                                     \
    {                                                    \
      aux_str = (char *) xmalloc (NUM_DIGITS);           \
      strncpy (aux_str, (string + pos), NUM_DIGITS);     \
      aux_str[NUM_DIGITS] = '\0';                        \
      if (pdf_str2int (aux_str, VAR) == PDF_ERROR)       \
        {                                                \
          goto error;                                    \
        }                                                \
      pos = pos + NUM_DIGITS;                            \
      free (aux_str);                                    \
    }                                                    \
  while (0)

static pdf_date_t
pdf_date_parse (char *string,
                int size)
{
  pdf_date_t date;
  int pos;
  int month_p;
  int day_p;
  int hour_p;
  int minute_p;
  int second_p;
  int ut_rel_p;
  int ut_hour_p;
  int ut_minute_p;
  char *aux_str;

  month_p = PDF_FALSE;
  day_p = PDF_FALSE;
  hour_p = PDF_FALSE;
  minute_p = PDF_FALSE;
  second_p = PDF_FALSE;
  ut_rel_p = PDF_FALSE;
  ut_hour_p = PDF_FALSE;
  ut_minute_p = PDF_FALSE;

  pos = 0;

  /* Bypass the optional marker */
  if (size < 4)
    {
      return NULL;
    }
  if ((string[pos] == 'D') &&
      (string[pos + 1] == ':'))
    {
      pos = pos + 2;
      size = size - 2;
    }

  /* Seek how much information is contained into the string based on
     its size */
  if ((size < 4) || (size == 5) || (size == 7) || (size == 9) ||
      (size == 11) || (size == 13) || (size == 16) || (size == 17) 
      || (size == 19) || (size == 20) || (size > 21))
    {
      return NULL;
    }

  month_p = (size >= 6);
  day_p = (size >= 8);
  hour_p = (size >= 10);
  minute_p = (size >= 12);
  second_p = (size >= 14);
  ut_rel_p = (size >= 15);
  ut_hour_p = (size >= 18);
  ut_minute_p = (size == 21);

  /* 
   * Get information
   */

  date = pdf_date_alloc ();
  date->month = 1;
  date->day = 1;
  date->hour = 0;
  date->minute = 0;
  date->second = 0;
  date->ut_rel = PDF_DATE_UT_UNKNOWN;
  date->ut_offset_hour = 0;
  date->ut_offset_minute = 0;

  DATE_EXTRACT_INT(4, &(date->year));
  if (month_p)
    {
      DATE_EXTRACT_INT(2, &(date->month));
      if ((date->month < 1) || (date->month > 12))
        {
          goto error;
        }
    }
  if (day_p)
    {
      DATE_EXTRACT_INT(2, &(date->day));
      if ((date->day < 1) || (date->day > 31))
        {
          goto error;
        }
    }
  if (hour_p)
    {
      DATE_EXTRACT_INT(2, &(date->hour));
      if ((date->hour < 0) || (date->hour > 23))
        {
          goto error;
        }
    }
  if (minute_p)
    {
      /* Minutes: 2 digits, optional, integer */
      DATE_EXTRACT_INT(2, &(date->minute));
      if ((date->minute < 0) || (date->minute > 59))
        {
          goto error;
        }
    }
  if (second_p)
    {
      /* Seconds: 2 digits, optional, integer */
      DATE_EXTRACT_INT(2, &(date->second));
      if ((date->second < 0) || (date->second > 59))
        {
          goto error;
        }
    }
  if (ut_rel_p)
    {
      /* UT relation: 1 character, optional, enum */
      switch (string[pos])
        {
        case '+':
          {
            date->ut_rel = PDF_DATE_UT_AFTER;
            break;
          }
        case '-':
          {
            date->ut_rel = PDF_DATE_UT_BEFORE;
            break;
          }
        case 'Z':
          {
            date->ut_rel = PDF_DATE_UT_EQUAL;
            break;
          }
        default:
          {
            goto error;
          }
        }
      pos = pos + 1;
    }
  if (ut_hour_p)
    {
      /* UT offset in hours, optional, 2 digits and a character */
      DATE_EXTRACT_INT(2, &(date->ut_offset_hour));
      if ((date->ut_offset_hour < 0) || (date->ut_offset_hour > 23))
        {
          goto error;
        }
      if (string[pos] != '\'')
        {
          goto error;
        }
      pos = pos + 1;
    }
  if (ut_minute_p)
    {
      /* UT offset in minutes, optional, 2 digits and a character */
      DATE_EXTRACT_INT(2, &(date->ut_offset_minute));
      if ((date->ut_offset_minute < 0) || (date->ut_offset_minute > 59))
        {
          goto error;
        }
      if (string[pos] != '\'')
        {
          goto error;
        }
      pos = pos + 1;
    }

  return date;

 error:

  pdf_date_dealloc (date);
  return NULL;
}

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

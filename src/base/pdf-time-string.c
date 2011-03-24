/* -*- mode: C -*-
 *
 *       File:         pdf-time-string.c
 *       Date:         Sun May 18 13:08:37 2008
 *
 *       GNU PDF Library - Time Module String utilities
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

#include <config.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pdf-alloc.h>
#include <pdf-time-string.h>

/* In order to use this macro, make sure that every byte represents a digit */
#define __GET_FIELD2(str,start,dest)                            \
  (dest += ((str[start] - 48) * 10 + (str[start + 1] - 48)));


/* Macro to check if given bytes are digits or not */
#define __CHECK_MASK(mask, masklength, str, i)                          \
  while (i < masklength) {                                              \
    if (mask & (1 << i)) {                                              \
      if((str[i] < '0') ||                                              \
         (str[i] > '9')) {                                              \
        PDF_DEBUG_BASE("Expected digit and found '%c' in '%s'", str[i], str); \
        return PDF_EBADDATA;                                            \
      }                                                                 \
    }                                                                   \
    i++;                                                                \
  }

/* Maximum length of strings, including trailing NUL */
#define PDF_MAX_ISO8601_STR_LENGTH  30
#define PDF_MAX_UTCASN1_STR_LENGTH  19
#define PDF_MAX_GENASN1_STR_LENGTH  21
#define PDF_MAX_PDFDATE_STR_LENGTH  24

static pdf_bool_t
pdf_time_check_string_pdf (const pdf_char_t  *time_str,
                           const pdf_size_t   time_str_length,
                           pdf_bool_t         require_trailing_apostrophe,
                           pdf_error_t      **error)
{
  pdf_i32_t i;

  /* Check minimum length      D:YYYY */
  if (time_str_length < 6)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TIME,
                     PDF_EBADDATA,
                     "invalid PDF time string, too short (%u): '%s'",
                     time_str_length,
                     time_str);
      return PDF_FALSE;
    }

  /* Check prefix */
  if (strncmp (time_str, "D:", 2) != 0)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TIME,
                     PDF_EBADDATA,
                     "invalid PDF time string, no prefix: '%s",
                     time_str);
      return PDF_FALSE;
    }

  /* We need to check the input characters are digits when we expect digits and
   *  the opposite as well. */

#define PDF_STRING_DIGIT_MASK  0x36FFFC /* 0011 0110 1111 1111 1111 1100 */

  /* Don't really need to check again two first bytes, as already done before */
  i = 2;
  __CHECK_MASK (PDF_STRING_DIGIT_MASK, time_str_length, time_str, i);

  /* Check time zone definer */
  if ((time_str_length >=16) &&
      (time_str[16] != 'Z') &&
      (time_str[16] != '+') &&
      (time_str[16] != '-'))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TIME,
                     PDF_EBADDATA,
                     "invalid PDF time string, "
                     "invalid time zone identifier ('%c'): '%s",
                     time_str[16],
                     time_str);
      return PDF_FALSE;
    }

  /* Check additional ' characters. Remember that the last ' character is
   * mandatory depending on the require_trailing_apostrophe parameter */
  if ((time_str_length >= 20) &&
      (time_str[19] != '\''))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TIME,
                     PDF_EBADDATA,
                     "invalid PDF time string, "
                     "invalid separator ('%c'): '%s",
                     time_str[19],
                     time_str);
      return PDF_FALSE;
    }

  if (require_trailing_apostrophe)
    {
      if ((time_str_length < 23) ||
          ((time_str[22] != '\'')))
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_TIME,
                         PDF_EBADDATA,
                         "invalid PDF time string, "
                         "invalid separator ('%c'): '%s",
                         (time_str_length >= 21) ? time_str[22] : ' ',
                         time_str);
          return PDF_FALSE;
        }
    }
  else
    {
      if (time_str_length >= 23)
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_TIME,
                         PDF_EBADDATA,
                         "invalid PDF time string, invalid separator ('%c'): '%s",
                         time_str[19],
                         time_str);
          return PDF_FALSE;
        }
    }

  return PDF_TRUE;
}

pdf_bool_t
pdf_time_from_string_pdf (pdf_time_t        *time_var,
                          const pdf_char_t  *time_str,
                          pdf_bool_t         require_trailing_apostrophe,
                          pdf_error_t      **error)
{
  /*
   * From PDF Reference 1.7: ( D:YYYYMMDDHHmmSSOHH'mm' )
   * From ISO 32000:         ( D:YYYYMMDDHHmmSSOHH'mm  )
   *
   * Notes: Year is mandatory, all the other fields may appear if the preceding
   *        also appear.
   *
   *  D:   = string "D:"
   *  YYYY = four-digit year
   *  MM   = two-digit month (01=January, etc.)
   *  DD   = two-digit day of month (01 through 31)
   *  HH   = two digits of hour (00 through 23)
   *  mm   = two digits of minute (00 through 59)
   *  SS   = two digits of second (00 through 59)
   *  O    = either '+', '-' or 'Z'
   *  HH   = two digits of hour (00 through 23) for the GMT offset
   *  '    = string "'"
   *  MM   = two digits of minute (00 through 59) for the GMT offset
   *  '    = string "'"  (NOTE: Mandatory in 1.7, non-valid in ISO32000)
   */

  struct pdf_time_cal_s calendar = { 0 };
  pdf_size_t time_str_length;

  time_str_length = strlen (time_str);

  if (!pdf_time_check_string_pdf (time_str,
                                  time_str_length,
                                  require_trailing_apostrophe,
                                  error))
    {
      pdf_prefix_error (error,
                        "couldn't get time from PDF date string: ");
      return PDF_FALSE;
    }

  while (PDF_TRUE)
    {
      /* Get century */
      __GET_FIELD2 (time_str, 2, calendar.year);
      calendar.year *= 100;
      /* Get year in century */
      __GET_FIELD2 (time_str, 4, calendar.year);
      /* more than year ? */
      if (time_str_length == 6)
        {
          calendar.month = 1;
          calendar.day = 1;
          break;
        }

      /* Get month */
      __GET_FIELD2 (time_str, 6, calendar.month);
      /* more than month ? */
      if (time_str_length == 8)
        {
          calendar.day = 1;
          break;
        }

      /* Get day */
      __GET_FIELD2 (time_str, 8, calendar.day);
      /* more than day ? */
      if (time_str_length == 10)
        break;

      /* Get hour */
      __GET_FIELD2 (time_str, 10, calendar.hour);
      /* more than hour ? */
      if (time_str_length == 12)
        break;

      /* Get minute */
      __GET_FIELD2 (time_str, 12, calendar.minute);
      /* more than minute ? */
      if (time_str_length == 14)
        break;

      /* Get second */
      __GET_FIELD2 (time_str, 14, calendar.second);
      /* more than second ? */
      if (time_str_length <= 17) /* Considering timezone offset separator */
        break;

      /* Get timezone offset hours */
      __GET_FIELD2 (time_str, 17, calendar.gmt_offset);
      /* And convert it in minutes */
      calendar.gmt_offset *= 60;

      /* Get timezone offset minutes */
      if (time_str_length > 19)
        __GET_FIELD2 (time_str, 20, calendar.gmt_offset);

      /* Convert from minutes to seconds */
      calendar.gmt_offset *= 60;

      /* Set proper sign */
      if (time_str[16] == '-')
        calendar.gmt_offset *= (-1);

      /* Stop loop :-) */
      break;
    }

  /* Get time value from break-down UTC calendar !*/
  pdf_time_set_from_cal (time_var, &calendar);

  return PDF_TRUE;
}

static pdf_bool_t
pdf_time_check_string_utc_asn1 (const pdf_char_t  *time_str,
                                const pdf_size_t   time_str_length,
                                pdf_error_t      **error)
{
  pdf_i32_t i;
  pdf_i32_t base_mask;
  pdf_i32_t mask_length;
  pdf_bool_t with_gmt_offset;

#define UTCASN1_STRING_DIGIT_MASK1  0x03FF /* 0000 0011 1111 1111 */
#define UTCASN1_STRING_DIGIT_MASK2  0x0FFF /* 0000 1111 1111 1111 */

  /* Check length */
  if ((time_str_length == 11) ||
      (time_str_length == 15))
    {
      base_mask = UTCASN1_STRING_DIGIT_MASK1;
      mask_length = 10;
    }
  else if ((time_str_length == 13) ||
           (time_str_length == 17))
    {
      base_mask = UTCASN1_STRING_DIGIT_MASK2;
      mask_length = 12;
    }
  else
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TIME,
                     PDF_EBADDATA,
                     "invalid UTC-ASN1 time string, invalid length (%u): '%s'",
                     time_str_length,
                     time_str);
      return PDF_FALSE;
    }

  /* Check if GMT offset is expected */
  with_gmt_offset = (time_str_length >= 15) ? PDF_TRUE : PDF_FALSE;

  /* Check extra non-digit characters */
  if ((!with_gmt_offset) &&
      (time_str[time_str_length - 1] != 'Z'))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TIME,
                     PDF_EBADDATA,
                     "invalid UTC-ASN1 time string, expected UTC string "
                     "but no Zulu timezone identifier found: '%s'",
                     time_str);
      return PDF_FALSE;
    }

  if ((with_gmt_offset) &&
      ((time_str[time_str_length - 5] != '+') &&
       (time_str[time_str_length - 5] != '-')))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TIME,
                     PDF_EBADDATA,
                     "invalid UTC-ASN1 time string, expected non-UTC string "
                     "but no GMT offset found: '%s'",
                     time_str);
      return PDF_FALSE;
    }

  /* Check mask if base string */
  i = 0;
  __CHECK_MASK (base_mask, mask_length, time_str, i);
  /* Check mask of offset string if available */
  if (with_gmt_offset)
    {
      i = time_str_length - 4;
      __CHECK_MASK (0x000F, 4, time_str, i);
    }

  return PDF_TRUE;
}

static pdf_i32_t
pdf_time_get_century_in_sliding_window (pdf_i32_t year_in_century)
{
  pdf_i32_t full_year = -1;
  pdf_time_t current;
  pdf_i32_t century;
  struct pdf_time_cal_s current_cal;

  pdf_time_init (&current);

  pdf_time_set_to_current_utc_time (&current);
  pdf_time_get_utc_cal (&current, &current_cal);

  /* Get century from full current year */
  century = 100 * (current_cal.year / 100);

  /* Appy current century */
  full_year = century + year_in_century;

  /* Check if the century must be changed */
  if ((full_year > current_cal.year) &&
      ((full_year - current_cal.year) > 50))
    {
      full_year -= 100;
    }
  else if ((full_year < current_cal.year) &&
           ((current_cal.year - full_year) > 50))
    {
      full_year += 100;
    }

  pdf_time_deinit (&current);

  return full_year;
}

pdf_bool_t
pdf_time_from_string_utc_asn1 (pdf_time_t       *time_var,
                               const pdf_char_t *time_str,
                               pdf_error_t      **error)
{
  /*
   * yymmddhhmmZ
   * yymmddhhmmssZ
   * yymmddhhmm+hhmm
   * yymmddhhmm-hhmm
   * yymmddhhmmss+hhmm
   * yymmddhhmmss-hhmm
   *
   * Note: As year is only stored in 2 digits, a sliding window of [-50,+50]
   *       years will be used to get the century.
   */
  struct pdf_time_cal_s calendar = { 0 };
  pdf_size_t time_str_length;

  time_str_length = strlen (time_str);

  if (!pdf_time_check_string_utc_asn1 (time_str,
                                       time_str_length,
                                       error))
    {
      pdf_prefix_error (error,
                        "couldn't get time from UTC ASN1 string: ");
      return PDF_FALSE;
    }

  while (PDF_TRUE)
    {
      pdf_bool_t has_seconds = PDF_FALSE;

      /* Get year in century */
      __GET_FIELD2 (time_str, 0, calendar.year);
      /* Get 4-digit year from 2-digit year */
      calendar.year = pdf_time_get_century_in_sliding_window (calendar.year);

      /* Get month */
      __GET_FIELD2 (time_str, 2, calendar.month);

      /* Get day */
      __GET_FIELD2 (time_str, 4, calendar.day);

      /* Get hour */
      __GET_FIELD2 (time_str, 6, calendar.hour);

      /* Get minute */
      __GET_FIELD2 (time_str, 8, calendar.minute);

      /* Get second if available */
      if ((time_str[10] >= '0') &&
          (time_str[10] <= '9'))
        {
          has_seconds = PDF_TRUE;
          __GET_FIELD2 (time_str, 10, calendar.second);
        }

      /* Check if we have GMT offset */
      if (time_str[time_str_length - 1] == 'Z')
        break;

      /* Get timezone offset hours */
      __GET_FIELD2 (time_str, (has_seconds ? 13 : 11), calendar.gmt_offset);
      /* And convert it in minutes */
      calendar.gmt_offset *= 60;
      /* Get timezone offset minutes */
      __GET_FIELD2 (time_str, (has_seconds ? 15 : 13), calendar.gmt_offset);
      /* Convert from minutes to seconds */
      calendar.gmt_offset *= 60;

      /* Set proper sign */
      if (time_str[(has_seconds ? 12 : 10)] == '-')
        calendar.gmt_offset *= (-1);

      /* Stop loop :-) */
      break;
    }

  /* Get time value from break-down UTC calendar !*/
  pdf_time_set_from_cal (time_var, &calendar);

  return PDF_TRUE;
}

pdf_bool_t
pdf_time_from_string_generalized_asn1 (pdf_time_t        *time_var,
                                       const pdf_char_t  *time_str,
                                       pdf_error_t      **error)
{
  /*
   *  Year:
   *    YYYY (eg 1997)
   *  Year and month:
   *    YYYYMM (eg 199707)
   *  Complete date:
   *    YYYYMMDD (eg 19970716)
   *  Complete date plus hours and minutes:
   *    YYYYMMDDhhmmTZD (eg 199707161920+01:00)
   *  Complete date plus hours, minutes and seconds:
   *    YYYYMMDDhhmmssTZD (eg 19970716192030+01:00)
   *  Complete date plus hours, minutes, seconds and a decimal fraction of a
   *  second
   *    YYYYMMDDThhmmss.sTZD (eg 1997071619:20:30.45+01:00)
   *
   *  where:
   *
   *  YYYY = four-digit year
   *  MM   = two-digit month (01=January, etc.)
   *  DD   = two-digit day of month (01 through 31)
   *  hh   = two digits of hour (00 through 23) (am/pm NOT allowed)
   *  mm   = two digits of minute (00 through 59)
   *  ss   = two digits of second (00 through 59)
   *  s    = one or more digits representing a decimal fraction of a second
   *  TZD  = time zone designator (Z or +hh:mm or -hh:mm)
   *
   */
  struct pdf_time_cal_s calendar = { 0 };
  pdf_size_t time_str_length;

  time_str_length = strlen (time_str);

  /* Check minimum length */
  if (time_str_length < 4)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TIME,
                     PDF_EBADDATA,
                     "couldn't get time from Generalized ASN1 date string: "
                     "invalid date string, too short (%u): '%s'",
                     time_str_length,
                     time_str);
      return PDF_FALSE;
    }

  while (PDF_TRUE)
    {
      /* Get century */
      __GET_FIELD2 (time_str, 0, calendar.year);
      calendar.year *= 100;
      /* Get year in century */
      __GET_FIELD2 (time_str, 2, calendar.year);
      /* more than year ? */
      if (time_str_length == 4)
        {
          calendar.month = 1;
          calendar.day = 1;
          break;
        }

      /* Get month */
      __GET_FIELD2 (time_str, 4, calendar.month);
      /* more than month ? */
      if (time_str_length == 6)
        {
          calendar.day = 1;
          break;
        }

      /* Get day */
      __GET_FIELD2 (time_str, 6, calendar.day);
      /* more than day ? */
      if (time_str_length == 8)
        break;

      /* Get hour and minutes */
      __GET_FIELD2 (time_str, 8, calendar.hour);
      __GET_FIELD2 (time_str, 10, calendar.minute);

      /* Get second if available */
      if ((time_str[17] >= '0') &&
          (time_str[17] <= '9'))
        {
          __GET_FIELD2 (time_str, 12, calendar.second);
        }

      /* Note: Fractional part of seconds not considered */
      if (time_str[time_str_length - 1] == 'Z')
        break;

      /* Get timezone offset hours */
      __GET_FIELD2 (time_str, (time_str_length - 4), calendar.gmt_offset);
      /* And convert it in minutes */
      calendar.gmt_offset *= 60;
      /* Get timezone offset minutes */
      __GET_FIELD2 (time_str, (time_str_length - 2), calendar.gmt_offset);
      /* Convert from minutes to seconds */
      calendar.gmt_offset *= 60;

      /* Set proper sign */
      if (time_str[(time_str_length - 5)] == '-')
        calendar.gmt_offset *= (-1);

      /* Stop loop :-) */
      break;
    }

  /* Get time value from break-down calendar !*/
  pdf_time_set_from_cal (time_var, &calendar);

  return PDF_TRUE;
}

pdf_bool_t
pdf_time_from_string_iso8601 (pdf_time_t        *time_var,
                              const pdf_char_t  *time_str,
                              pdf_error_t      **error)
{
  /*
   *  Year:
   *    YYYY (eg 1997)
   *  Year and month:
   *    YYYY-MM (eg 1997-07)
   *  Complete date:
   *    YYYY-MM-DD (eg 1997-07-16)
   *  Complete date plus hours and minutes:
   *    YYYY-MM-DDThh:mmTZD (eg 1997-07-16T19:20+01:00)
   *  Complete date plus hours, minutes and seconds:
   *    YYYY-MM-DDThh:mm:ssTZD (eg 1997-07-16T19:20:30+01:00)
   *  Complete date plus hours, minutes, seconds and a decimal fraction of a
   *  second
   *    YYYY-MM-DDThh:mm:ss.sTZD (eg 1997-07-16T19:20:30.45+01:00)
   *
   *  where:
   *
   *  YYYY = four-digit year
   *  MM   = two-digit month (01=January, etc.)
   *  DD   = two-digit day of month (01 through 31)
   *  hh   = two digits of hour (00 through 23) (am/pm NOT allowed)
   *  mm   = two digits of minute (00 through 59)
   *  ss   = two digits of second (00 through 59)
   *  s    = one or more digits representing a decimal fraction of a second
   *  TZD  = time zone designator (Z or +hh:mm or -hh:mm)
   *
   */
  struct pdf_time_cal_s calendar = { 0 };
  pdf_size_t time_str_length;

  time_str_length = strlen (time_str);

  /* Check minimum length */
  if (time_str_length < 4)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TIME,
                     PDF_EBADDATA,
                     "couldn't get time from ISO-8601 date string: "
                     "invalid date string, too short (%u): '%s'",
                     time_str_length,
                     time_str);
      return PDF_FALSE;
    }

  while (PDF_TRUE)
    {
      /* Get century */
      __GET_FIELD2 (time_str, 0, calendar.year);
      calendar.year *= 100;
      /* Get year in century */
      __GET_FIELD2 (time_str, 2, calendar.year);
      /* more than year ? */
      if (time_str_length == 4)
        {
          calendar.month = 1;
          calendar.day = 1;
          break;
        }

      /* Get month */
      __GET_FIELD2 (time_str, 5, calendar.month);
      /* more than month ? */
      if (time_str_length == 7)
        {
          calendar.day = 1;
          break;
        }

      /* Get day */
      __GET_FIELD2 (time_str, 8, calendar.day);
      /* more than day ? */
      if (time_str_length == 10)
        break;

      /* Get hour and minutes */
      __GET_FIELD2 (time_str, 11, calendar.hour);
      __GET_FIELD2 (time_str, 14, calendar.minute);

      /* Get second if available */
      if ((time_str[17] >= '0') &&
          (time_str[17] <= '9') &&
          (time_str[16] == ':'))
        {
          __GET_FIELD2 (time_str, 17, calendar.second);
        }

      /* Note: Fractional part of seconds not considered */
      if (time_str[time_str_length - 1] == 'Z')
        break;

      /* Get timezone offset hours */
      __GET_FIELD2 (time_str, (time_str_length - 5), calendar.gmt_offset);
      /* And convert it in minutes */
      calendar.gmt_offset *= 60;
      /* Get timezone offset minutes */
      __GET_FIELD2 (time_str, (time_str_length - 2), calendar.gmt_offset);
      /* Convert from minutes to seconds */
      calendar.gmt_offset *= 60;

      /* Set proper sign */
      if (time_str[(time_str_length - 6)] == '-')
        calendar.gmt_offset *= (-1);

      /* Stop loop :-) */
      break;
    }

  /* Get time value from break-down calendar !*/
  pdf_time_set_from_cal (time_var, &calendar);

  return PDF_TRUE;
}

/* Get Date as a string in PDF format */
pdf_char_t *
pdf_time_to_string_pdf (const pdf_time_t  *time_var,
                        pdf_bool_t         include_trailing_apostrophe,
                        pdf_error_t      **error)
{
  pdf_char_t *str;
  struct pdf_time_cal_s calendar = { 0 };

  str = (pdf_char_t *) pdf_alloc (PDF_MAX_PDFDATE_STR_LENGTH);
  if (!str)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TIME,
                     PDF_ENOMEM,
                     "cannot create string with PDF time: "
                     "couldn't allocate %lu bytes",
                     PDF_MAX_PDFDATE_STR_LENGTH);
      return NULL;
    }

  /* D:YYYYMMDDHHmmSSOHH'mm' */
  pdf_time_get_local_cal (time_var, &calendar);

  if (calendar.gmt_offset != 0)
    {
      pdf_i32_t offset_hours;
      pdf_i32_t offset_minutes;

      offset_hours = (((calendar.gmt_offset < 0) ? (-1) : (1)) * calendar.gmt_offset) / 3600;
      offset_minutes = (((calendar.gmt_offset < 0) ? (-1) : (1)) * calendar.gmt_offset) % 3600;
      offset_minutes /= 60; /* Get only full minutes */
      sprintf (str, "D:%4d%s%d%s%d%s%d%s%d%s%d%c%s%d'%s%d",
               calendar.year,
               (calendar.month < 10 ? "0" : ""), calendar.month,
               (calendar.day < 10 ? "0" : ""), calendar.day,
               (calendar.hour < 10 ? "0" : ""), calendar.hour,
               (calendar.minute < 10 ? "0" : ""), calendar.minute,
               (calendar.second < 10 ? "0" : ""), calendar.second,
               ((calendar.gmt_offset < 0) ? '-' : '+'),
               (offset_hours < 10 ? "0" : ""), offset_hours,
               (offset_minutes < 10 ? "0" : ""), offset_minutes);

      if (include_trailing_apostrophe)
        {
          str[PDF_MAX_PDFDATE_STR_LENGTH - 2] = '\'';
          str[PDF_MAX_PDFDATE_STR_LENGTH - 1] = '\0';
        }
    }
  else
    {
      sprintf (str, "D:%4d%s%d%s%d%s%d%s%d%s%dZ",
               calendar.year,
               (calendar.month < 10 ? "0" : ""), calendar.month,
               (calendar.day < 10 ? "0" : ""), calendar.day,
               (calendar.hour < 10 ? "0" : ""), calendar.hour,
               (calendar.minute < 10 ? "0" : ""), calendar.minute,
               (calendar.second < 10 ? "0" : ""), calendar.second);
    }

  return str;
}

/* Get Date as a string in UTC-ASN1 format */
pdf_char_t *
pdf_time_to_string_utc_asn1(const pdf_time_t  *time_var,
                            pdf_error_t      **error)
{
  pdf_char_t *str;
  struct pdf_time_cal_s calendar;
  pdf_i32_t smallyear;

  str = (pdf_char_t *) pdf_alloc (PDF_MAX_UTCASN1_STR_LENGTH);
  if (!str)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TIME,
                     PDF_ENOMEM,
                     "cannot create string with UTC ASN1 time: "
                     "couldn't allocate %lu bytes",
                     PDF_MAX_UTCASN1_STR_LENGTH);
      return NULL;
    }

  pdf_time_get_local_cal (time_var, &calendar);

  /* Convert 4-digit year to 2-digit year */
  smallyear = calendar.year -1900;
  while (smallyear > 99)
    smallyear -= 100;

  if (calendar.gmt_offset != 0)
    {
      pdf_i32_t offset_hours;
      pdf_i32_t offset_minutes;

      offset_hours = (((calendar.gmt_offset < 0) ? (-1) : (1)) * calendar.gmt_offset) / 3600;
      offset_minutes = (((calendar.gmt_offset < 0) ? (-1) : (1)) * calendar.gmt_offset) % 3600;
      offset_minutes /= 60;
      /* yymmddhhmmss+hhmm
       * yymmddhhmmss-hhmm
       */
      sprintf (str, "%s%d%s%d%s%d%s%d%s%d%s%d%c%s%d%s%d",
               (smallyear < 10 ? "0" : ""), smallyear,
               (calendar.month < 10 ? "0" : ""), calendar.month,
               (calendar.day < 10 ? "0" : ""), calendar.day,
               (calendar.hour < 10 ? "0" : ""), calendar.hour,
               (calendar.minute < 10 ? "0" : ""), calendar.minute,
               (calendar.second < 10 ? "0" : ""), calendar.second,
               ((calendar.gmt_offset < 0) ? '-' : '+'),
               (offset_hours < 10 ? "0" : ""), offset_hours,
               (offset_minutes < 10 ? "0" : ""), offset_minutes);
    }
  else
    {
      /*
       * yymmddhhmmssZ
       */
      sprintf (str, "%s%d%s%d%s%d%s%d%s%d%s%dZ",
               (smallyear < 10 ? "0" : ""), smallyear,
               (calendar.month < 10 ? "0" : ""), calendar.month,
               (calendar.day < 10 ? "0" : ""), calendar.day,
               (calendar.hour < 10 ? "0" : ""), calendar.hour,
               (calendar.minute < 10 ? "0" : ""), calendar.minute,
               (calendar.second < 10 ? "0" : ""), calendar.second);
    }

  return str;
}

/* Get Date as a string in Generalized ASN1 format */
pdf_char_t *
pdf_time_to_string_generalized_asn1(const pdf_time_t  *time_var,
                                    pdf_error_t      **error)
{
  pdf_char_t *str;
  struct pdf_time_cal_s calendar;

  str = (pdf_char_t *) pdf_alloc (PDF_MAX_GENASN1_STR_LENGTH);
  if (!str)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TIME,
                     PDF_ENOMEM,
                     "cannot create string with Generalized ASN1 time: "
                     "couldn't allocate %lu bytes",
                     PDF_MAX_GENASN1_STR_LENGTH);
      return NULL;
    }

  /* YYYYMMDDhhmmssTZD (eg 19970716192030+01:00) */
  pdf_time_get_local_cal (time_var, &calendar);

  if (calendar.gmt_offset != 0)
    {
      pdf_i32_t offset_hours;
      pdf_i32_t offset_minutes;

      offset_hours = (((calendar.gmt_offset < 0) ? (-1) : (1)) * calendar.gmt_offset) / 3600;
      offset_minutes = (((calendar.gmt_offset < 0) ? (-1) : (1)) * calendar.gmt_offset) % 3600;
      offset_minutes /= 60;
      sprintf (str, "%4d%s%d%s%d%s%d%s%d%s%d%c%s%d%s%d",
               calendar.year,
               (calendar.month < 10 ? "0" : ""), calendar.month,
               (calendar.day < 10 ? "0" : ""), calendar.day,
               (calendar.hour < 10 ? "0" : ""), calendar.hour,
               (calendar.minute < 10 ? "0" : ""), calendar.minute,
               (calendar.second < 10 ? "0" : ""), calendar.second,
               ((calendar.gmt_offset < 0) ? '-' : '+'),
               (offset_hours < 10 ? "0" : ""), offset_hours,
               (offset_minutes < 10 ? "0" : ""), offset_minutes);
    }
  else
    {
      sprintf (str, "%4d%s%d%s%d%s%d%s%d%s%dZ",
               calendar.year,
               (calendar.month < 10 ? "0" : ""), calendar.month,
               (calendar.day < 10 ? "0" : ""), calendar.day,
               (calendar.hour < 10 ? "0" : ""), calendar.hour,
               (calendar.minute < 10 ? "0" : ""), calendar.minute,
               (calendar.second < 10 ? "0" : ""), calendar.second);
    }

  return str;
}

/* Get Date as a string in ISO8601 format */
pdf_char_t *
pdf_time_to_string_iso8601(const pdf_time_t  *time_var,
                           pdf_error_t      **error)
{
  pdf_char_t *str;
  struct pdf_time_cal_s calendar;

  str = (pdf_char_t *) pdf_alloc (PDF_MAX_ISO8601_STR_LENGTH);
  if (!str)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TIME,
                     PDF_ENOMEM,
                     "cannot create string with ISO8601 time: "
                     "couldn't allocate %lu bytes",
                     PDF_MAX_ISO8601_STR_LENGTH);
      return NULL;
    }

  /* YYYY-MM-DDThh:mm:ss.sTZD (eg 1997-07-16T19:20:30.45+01:00) */
  pdf_time_get_local_cal (time_var, &calendar);
  if (calendar.gmt_offset != 0)
    {
      pdf_i32_t offset_hours;
      pdf_i32_t offset_minutes;

      offset_hours = (((calendar.gmt_offset < 0) ? (-1) : (1)) * calendar.gmt_offset) / 3600;
      offset_minutes = (((calendar.gmt_offset < 0) ? (-1) : (1)) * calendar.gmt_offset) % 3600;
      offset_minutes /= 60; /* Get only full minutes */
      sprintf (str, "%4d-%s%d-%s%dT%s%d:%s%d:%s%d%c%s%d:%s%d",
               calendar.year,
               (calendar.month < 10 ? "0" : ""), calendar.month,
               (calendar.day < 10 ? "0" : ""), calendar.day,
               (calendar.hour < 10 ? "0" : ""), calendar.hour,
               (calendar.minute < 10 ? "0" : ""), calendar.minute,
               (calendar.second < 10 ? "0" : ""), calendar.second,
               ((calendar.gmt_offset < 0) ? '-' : '+'),
               (offset_hours < 10 ? "0" : ""), offset_hours,
               (offset_minutes < 10 ? "0" : ""), offset_minutes);
    }
  else
    {
      sprintf (str, "%4d-%s%d-%s%dT%s%d:%s%d:%s%dZ",
               calendar.year,
               (calendar.month < 10 ? "0" : ""), calendar.month,
               (calendar.day < 10 ? "0" : ""), calendar.day,
               (calendar.hour < 10 ? "0" : ""), calendar.hour,
               (calendar.minute < 10 ? "0" : ""), calendar.minute,
               (calendar.second < 10 ? "0" : ""), calendar.second);
    }

  return str;
}

/* End of pdf-time-string.c */

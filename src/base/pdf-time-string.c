/* -*- mode: C -*- Time-stamp: ""
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pdf-base.h>
#include <pdf-time-string.h>


#define PDF_MAX_ISO8601_STR_LENGTH  30



pdf_status_t
pdf_time_from_string_pdf(pdf_time_t time_var,
                         const pdf_char_t *time_str)
{
  /* TODO */
  return PDF_ERROR;
}

pdf_status_t
pdf_time_from_string_asn1(pdf_time_t time_var,
                          const pdf_char_t *time_str)
{
  /* TODO */
  return PDF_ERROR;
}

pdf_status_t
pdf_time_from_string_generalized_asn1(pdf_time_t time_var,
                                      const pdf_char_t *time_str)
{
  /* TODO */
  return PDF_ERROR;
}

pdf_status_t
pdf_time_from_string_iso8601(pdf_time_t time_var,
                             const pdf_char_t *time_str)
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
  struct pdf_time_cal_s calendar;
  pdf_char_t *duplicate;
  pdf_char_t *walker;
  pdf_status_t ret_code;
  pdf_i32_t    gmt_offset;
  pdf_size_t time_str_length = strlen((char *)time_str);
  
  /* Check minimum length */
  if(time_str_length < 4)
    {
      PDF_DEBUG_BASE("Invalid ISO-8601 time string (too short): '%s'",
                     time_str);
      return PDF_EBADDATA;
    }
  
  /* Initialize text walker */
  duplicate = (pdf_char_t *)pdf_alloc(time_str_length+1);
  if(duplicate == NULL)
    {
      PDF_DEBUG_BASE("Problem allocating memory");
      return PDF_ENOMEM;
    }
  memcpy(duplicate, time_str, time_str_length);
  walker = duplicate;
  
  /* Reset calendar */
  memset(&calendar, 0, sizeof(calendar));
  
  /* Get year */
  duplicate[4] = '\0';
  calendar.year = atoi((char *)duplicate);
  
  /* Get month */
  if(time_str_length >= 7)
    {
      duplicate[7] = '\0';
      calendar.month = atoi((char *)(&duplicate[5]));
      
      /* Get day */
      if(time_str_length >= 10)
        {
          duplicate[10] = '\0';
          calendar.day = atoi((char *)(&duplicate[8]));
          
          /* Get hour and minutes */
          if(time_str_length >= 16+1) /* 1 is the minimum length for TZD */
            {
              char next_field = duplicate[16];
              
              /* Get hour */
              duplicate[13] = '\0';
              calendar.hour = atoi((char *)(&duplicate[11]));
              /* Get minutes */
              duplicate[16] = '\0';
              calendar.minute = atoi((char *)(&duplicate[14]));
              
              /* Get Time Zone information */
              if(duplicate[time_str_length-1] == 'Z')
                {
                  /* Time is given in UTC... do nothing */
                  duplicate[time_str_length-1] = '\0';
                }
              else
                {
                  /* Need to parse time zone offset */
                  pdf_i32_t hours_tz;
                  pdf_i32_t minutes_tz;
                  minutes_tz = atoi((char *)(&duplicate[time_str_length-2]));
                  duplicate[time_str_length-3] = '\0';
                  hours_tz = atoi((char *)(&duplicate[time_str_length-5]));
                  
                  gmt_offset = 60*(minutes_tz + 60*hours_tz);
                  if(duplicate[time_str_length-6] == '-')
                    {
                      gmt_offset *= (-1);
                    } 
                }
              
              /* Read seconds if available */
              if(next_field == ':')
                {
                  /* Ok, seconds available. Decimal part of the seconds will be
                   * ignored if it's available */
                  duplicate[19] = '\0';
                  calendar.second = atoi((char *)(&duplicate[17]));
                }
            }
        }
    }

  /* Set calendar as if it were UTC */
  calendar.gmt_offset = 0;

/*
   pdf_char_t *str = (pdf_char_t *)pdf_alloc(PDF_MAX_ISO8601_STR_LENGTH*sizeof(pdf_char_t));
   pdf_i32_t offset_hours = calendar.gmt_offset / 3600;
   pdf_i32_t offset_minutes = calendar.gmt_offset % 3600;
   sprintf((char *)str, "%4d-%s%d-%s%dT%s%d:%s%d:%s%d.00+%s%d:%s%d", \
   calendar.year,
   (calendar.month < 10 ? "0" : ""), calendar.month,
   (calendar.day < 10 ? "0" : ""), calendar.day,
   (calendar.hour < 10 ? "0" : ""), calendar.hour,
   (calendar.minute < 10 ? "0" : ""), calendar.minute,
   (calendar.second < 10 ? "0" : ""), calendar.second,
   (offset_hours < 10 ? "0" : ""), offset_hours,
   (offset_minutes < 10 ? "0" : ""), offset_minutes);
   PDF_DEBUG_BASE("Intermediate calendar: %s", str);
*/
  
  /* Get time value from break-down UTC calendar !*/
  ret_code = pdf_time_from_cal(time_var, &calendar);
  if(ret_code == PDF_OK)
    {
      /* Now set GMT offset in pdf_time_t */
      time_var->gmt_offset = gmt_offset;
    }

  return ret_code;
}


/* Get Date as a string in PDF format */
pdf_char_t *
pdf_time_to_string_pdf(const pdf_time_t time_var)
{
  return NULL;
}


/* Get Date as a string in ASN1 format */
pdf_char_t *
pdf_time_to_string_asn1(const pdf_time_t time_var)
{
  return NULL;
}

/* Get Date as a string in Generalized ASN1 format */
pdf_char_t *
pdf_time_to_string_generalized_asn1(const pdf_time_t time_var)
{
  return NULL;
}

/* Get Date as a string in ISO8601 format */
pdf_char_t *
pdf_time_to_string_iso8601(const pdf_time_t time_var)
{
  pdf_char_t *str;
  struct pdf_time_cal_s calendar;

  str = (pdf_char_t *)pdf_alloc(PDF_MAX_ISO8601_STR_LENGTH*sizeof(pdf_char_t));
  if(str != NULL)
    {
      /* YYYY-MM-DDThh:mm:ss.sTZD (eg 1997-07-16T19:20:30.45+01:00) */
      if(pdf_time_get_local_cal(time_var, &calendar) == PDF_OK)
        {
          pdf_i32_t offset_hours = calendar.gmt_offset / 3600;
          pdf_i32_t offset_minutes = calendar.gmt_offset % 3600;
          sprintf((char *)str, "%4d-%s%d-%s%dT%s%d:%s%d:%s%d.00+%s%d:%s%d", \
                  calendar.year,
                  (calendar.month < 10 ? "0" : ""), calendar.month,
                  (calendar.day < 10 ? "0" : ""), calendar.day,
                  (calendar.hour < 10 ? "0" : ""), calendar.hour,
                  (calendar.minute < 10 ? "0" : ""), calendar.minute,
                  (calendar.second < 10 ? "0" : ""), calendar.second,
                  (offset_hours < 10 ? "0" : ""), offset_hours,
                  (offset_minutes < 10 ? "0" : ""), offset_minutes);
        }
      else
        {
          PDF_DEBUG_BASE("Could not get local calendar from pdf_time_t...");
          pdf_dealloc(str);
          str = NULL;
        }
    }

  return str;
}



/* End of pdf-time-string.c */

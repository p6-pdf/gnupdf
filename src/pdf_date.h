/* -*- mode: C -*- Time-stamp: "07/09/07 19:58:28 jemarch"
 *
 *       File:         pdf_date.h
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Wed Sep  5 22:39:10 2007
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


#ifndef PDF_DATE_H
#define PDF_DATE_H

#include <config.h>
#include <pdf_base.h>
#include <pdf_obj.h>

/* Relationship with Universal Time */

enum pdf_date_ut_rel
{
  PDF_DATE_UT_UNKNOWN = 0,
  PDF_DATE_UT_BEFORE,
  PDF_DATE_UT_EQUAL,
  PDF_DATE_UT_AFTER
};

/* Date internal structure. Note that PDF dates are always local ones,
   even if the relation with UT is not defined */

struct pdf_date_s
{
  int year;                     /* Year */
  int month;                    /* Month, 01-12 */
  int day;                      /* Day, 01-31 */
  int hour;                     /* Hour, 00-23 */
  int minute;                   /* Minute, 00-59 */
  int second;                   /* Second, 00-59 */

  enum pdf_date_ut_rel ut_rel;

  int ut_offset_hours;          /* Absolute offset from UT in hours */
  int ut_offset_minutes;        /* Absolute offset from UT in minutes */
};

typedef struct pdf_date_s *pdf_date_t;


pdf_date_t pdf_date_new (pdf_obj_t date_string);
pdf_date_t pdf_date_new_value (int year,
                               int month,
                               int day,
                               int hour,
                               int minute,
                               int second,
                               int ut_offset_hours,
                               int ut_offset_minutes);
void pdf_date_destroy (pdf_date_t date);

pdf_obj_t pdf_date_string (pdf_date_t date);

inline int pdf_date_year (pdf_date_t date);
void pdf_date_set_year (pdf_date_t date, int year);

int pdf_date_month (pdf_date_t date);
void pdf_date_set_month (pdf_date_t date, int month);

int pdf_date_day (pdf_date_t date);
void pdf_date_set_day (pdf_date_t date, int day);

int pdf_date_hour (pdf_date_t date);
void pdf_date_set_hour (pdf_date_t date, int hour);

int pdf_date_minute (pdf_date_t date);
void pdf_date_set_minute (pdf_date_t date, int minute);

int pdf_date_second (pdf_date_t date);
void pdf_date_set_second (pdf_date_t date, int second);

int pdf_date_cmp (pdf_date_t date1, pdf_date_t date2);

#endif /* pdf_date.h */

/* End of pdf_date.h */

/* -*- mode: C -*- Time-stamp: "09/04/17 00:08:14 jemarch"
 *
 *       File:         pdf-time-test-common.h
 *       Date:         Fri Feb 27 17:35:31 2009
 *
 *       GNU PDF Library - Common code for the time tests
 *
 */

/* Copyright (C) 2009 Free Software Foundation, Inc. */

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

#ifndef PDF_TIME_TEST_COMMON_H
#define PDF_TIME_TEST_COMMON_H

#include <pdf.h>

/* Size of pdf_time_cal_span_s calspan[] table */
#define DAY_TIME_SPAN_SIZE 32
#define YEAR_MONTH_SIZE 30

#define DATES_SIZE 86

#define SEC_IN_DAY 24*60*60
#define SEC_IN_LEEP_YEAR 366*SEC_IN_DAY
#define SEC_IN_NOLEEP_YEAR 365*SEC_IN_DAY

#define DATE_01_01_1970 0
#define DATE_05_03_1970 (31 + 28 + 5)*SEC_IN_DAY
#define DATE_01_01_2000 946684800

#define DATE_01_01_2009 1230768000
#define DATE_01_12_2009 DATE_01_01_2009 + SEC_IN_NOLEEP_YEAR - 31*SEC_IN_DAY
#define DATE_01_01_1950 -631152000

//void print_pdf_time_cal(pdf_time_t t);

#endif

/* End of pdf-time-test-common.h */

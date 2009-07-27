/* -*- mode: C -*- Time-stamp: "09/04/17 00:08:47 jemarch"
 *
 *       File:         pdf-time-testdata.c
 *       Date:         Fri Feb 27 17:35:31 2009
 *
 *       GNU PDF Library - Test data for the time tests
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


#include <pdf.h>
#include <base/time/pdf-time-test-common.h>

/* Table with time spans */
 struct pdf_time_cal_span_s day_time_span[DAY_TIME_SPAN_SIZE] ={
    {PDF_FALSE,0,0,0,0,0,0},        /* Zero seconds */
    {PDF_FALSE,0,0,0,0,0,1},        /* One socond */
    {PDF_FALSE,0,0,0,0,0,33},       /* 33 seconds */
    {PDF_FALSE,0,0,0,0,0,59},       /* 59 seconds */
    {PDF_FALSE,0,0,0,0,1,0},        /* 1 minute */

    {PDF_FALSE,0,0,0,0,1,1},        /* 1 minute and 1 second */
    {PDF_FALSE,0,0,0,0,1,35},       /* 1 minute and 35 seconds */
    {PDF_FALSE,0,0,0,0,1,59},       /* 1 minute and 59 seconds */
    {PDF_FALSE,0,0,0,0,33,0},       /* 33 minutes  */
    {PDF_FALSE,0,0,0,0,59,59},      /* 59 minutes and 59 seconds */

    {PDF_FALSE,0,0,0,1,0,0},        /* 1 hour */
    {PDF_FALSE,0,0,0,1,0,1},        /* 1 hour and 1 second */
    {PDF_FALSE,0,0,0,1,1,1},        /* 1 hour, 1 minute and 1 second */
    {PDF_FALSE,0,0,0,11,0,0},       /* 11 hours */
    {PDF_FALSE,0,0,0,12,0,0},       /* 12 hours */

    {PDF_FALSE,0,0,0,23,59,59},     /* 23 hours, 59 minutes and 59 seconds */
    {PDF_FALSE,0,0,1,0,0,0},        /* 1 day */
    {PDF_FALSE,0,0,6,12,35,35},     /* 6 days, 12 hours, 35 minutes and 35 seconds */
    {PDF_FALSE,0,0,11,23,59,59},    /* 11 days, 23 hours, 59 minutes and 59 seconds */
    {PDF_FALSE,0,0,11,0,0,0},       /* 11 days */

    {PDF_FALSE,0,0,20,0,0,0},       /* 20 days */
    {PDF_FALSE,0,0,20,23,59,59},    /* 20 days, 23 hours, 59 minutes and 59 seconds */
    {PDF_FALSE,0,0,27,0,0,0},       /* 27 days */
    {PDF_FALSE,0,0,27,23,59,59},    /* 27 days, 23 hours, 59 minutes and 59 seconds */
  };

/* Table with year, month spans */
struct pdf_time_cal_span_s years_months[YEAR_MONTH_SIZE] ={
    {PDF_FALSE,0,0,0,0,0,0},    /* 0 years and 0 months*/
    {PDF_FALSE,0,1,0,0,0,0},    /* 0 years and 1 month*/
    {PDF_FALSE,0,2,0,0,0,0},    /* 0 years and 2 months*/
    {PDF_FALSE,0,3,0,0,0,0},    /* 0 years and 3 months*/
    {PDF_FALSE,0,11,0,0,0,0},   /* 0 years and 11 months*/
//    {PDF_FALSE,0,0,0,0,0,0},   /* 0 years and 11 months*/


    {PDF_FALSE,1,0,0,0,0,0},    /* 1 year and 0 months*/
    {PDF_FALSE,1,1,0,0,0,0},    /* 1 year and 1 month*/
    {PDF_FALSE,1,2,0,0,0,0},    /* 1 year and 2 months*/
    {PDF_FALSE,1,3,0,0,0,0},    /* 1 year and 3 months*/
    {PDF_FALSE,1,11,0,0,0,0},    /* 1 year and 11 months*/
//    {PDF_FALSE,0,0,0,0,0,0},   /* 0 years and 11 months*/


    {PDF_FALSE,2,0,0,0,0,0},
    {PDF_FALSE,2,1,0,0,0,0},
    {PDF_FALSE,2,2,0,0,0,0},
    {PDF_FALSE,2,3,0,0,0,0},
    {PDF_FALSE,2,11,0,0,0,0},
//    {PDF_FALSE,0,0,0,0,0,0},   /* 0 years and 11 months*/


    {PDF_FALSE,3,0,0,0,0,0},
    {PDF_FALSE,3,1,0,0,0,0},
    {PDF_FALSE,3,2,0,0,0,0},
    {PDF_FALSE,3,3,0,0,0,0},
    {PDF_FALSE,3,11,0,0,0,0},
//    {PDF_FALSE,0,0,0,0,0,0},   /* 0 years and 11 months*/


    {PDF_FALSE,30,0,0,0,0,0},   /* year 2000 */
    {PDF_FALSE,30,1,0,0,0,0},   /* 30 years and 0 months*/
    {PDF_FALSE,30,2,0,0,0,0},
    {PDF_FALSE,30,3,0,0,0,0},
    {PDF_FALSE,30,11,0,0,0,0},
//    {PDF_FALSE,0,0,0,0,0,0},   /* 0 years and 11 months*/


    {PDF_FALSE,31,0,0,0,0,0},
    {PDF_FALSE,31,1,0,0,0,0},
    {PDF_FALSE,31,2,0,0,0,0},
    {PDF_FALSE,31,3,0,0,0,0},
    {PDF_FALSE,31,11,0,0,0,0},
//    {PDF_FALSE,0,0,0,0,0,0},   /* 0 years and 11 months*/

};

pdf_u32_t daysInSeconds[YEAR_MONTH_SIZE] = {
    0,
    31*SEC_IN_DAY,
    (31 + 28)*SEC_IN_DAY,
    (31 + 28 + 31)*SEC_IN_DAY,
    SEC_IN_NOLEEP_YEAR - 31*SEC_IN_DAY, /* 0 year and 11 months */
//0,
    

    SEC_IN_NOLEEP_YEAR,                               /* 1 year and 0 months */
    SEC_IN_NOLEEP_YEAR + 31*SEC_IN_DAY,               /* 1 year and 1 month */
    SEC_IN_NOLEEP_YEAR + (31 + 28)*SEC_IN_DAY,        /* 1 year and 2 months */
    SEC_IN_NOLEEP_YEAR + (31 + 28 + 31)*SEC_IN_DAY,   /* 1 year and 3 months */
    2*SEC_IN_NOLEEP_YEAR - 31*SEC_IN_DAY,             /* 1 year and 11 months */
//0,

    2*SEC_IN_NOLEEP_YEAR,
    2*SEC_IN_NOLEEP_YEAR +31*SEC_IN_DAY, 
    2*SEC_IN_NOLEEP_YEAR +(31+29)*SEC_IN_DAY,
    2*SEC_IN_NOLEEP_YEAR +(31+29+31)*SEC_IN_DAY,
    2*SEC_IN_NOLEEP_YEAR + SEC_IN_LEEP_YEAR -31*SEC_IN_DAY,
//0,
    2*SEC_IN_NOLEEP_YEAR + SEC_IN_LEEP_YEAR,
    2*SEC_IN_NOLEEP_YEAR + SEC_IN_LEEP_YEAR + 31*SEC_IN_DAY,
    2*SEC_IN_NOLEEP_YEAR + SEC_IN_LEEP_YEAR + (31+28)*SEC_IN_DAY,
    2*SEC_IN_NOLEEP_YEAR + SEC_IN_LEEP_YEAR + (31+28+31)*SEC_IN_DAY,
    3*SEC_IN_NOLEEP_YEAR + SEC_IN_LEEP_YEAR -31*SEC_IN_DAY,
//0,
    DATE_01_01_2000,                                    /* 30 years and 0 months - year 2000 */
    DATE_01_01_2000 + 31*SEC_IN_DAY,
    DATE_01_01_2000 + (31 + 29)*SEC_IN_DAY,
    DATE_01_01_2000 + (31 + 29 + 31)*SEC_IN_DAY,
    DATE_01_01_2000 + SEC_IN_LEEP_YEAR - 31*SEC_IN_DAY, /* year and 11 months */
//0,
    DATE_01_01_2000 + SEC_IN_LEEP_YEAR,
    DATE_01_01_2000 + SEC_IN_LEEP_YEAR + 31*SEC_IN_DAY,
    DATE_01_01_2000 + SEC_IN_LEEP_YEAR + (31 + 28)*SEC_IN_DAY,
    DATE_01_01_2000 + SEC_IN_LEEP_YEAR + (31 + 28 + 31)*SEC_IN_DAY,
    DATE_01_01_2000 + SEC_IN_LEEP_YEAR + SEC_IN_NOLEEP_YEAR - 31*SEC_IN_DAY
//0
};

struct pdf_time_cal_s dates[DATES_SIZE] ={
    {1970,1,1,4,0,0,0,0},     {1970,1,1,4,0,0,1,0},     {1970,1,1,4,0,0,33,0},
    {1970,1,1,4,0,0,59,0},    {1970,1,1,4,0,1,0,0},     {1970,1,1,4,0,1,1,0},
    {1970,1,1,4,0,1,35,0},    {1970,1,1,4,0,1,59,0},    {1970,1,1,4,0,33,0,0},
    {1970,1,1,4,0,59,59,0},   {1970,1,1,4,1,0,0,0},     {1970,1,1,4,1,0,1,0},
    {1970,1,1,4,1,1,1,0},     {1970,1,1,4,11,0,0,0},    {1970,1,1,4,12,0,0,0},
    {1970,1,1,4,23,59,59,0},  {1970,1,2,5,0,0,0,0},     {1970,1,7,3,12,35,35,0},
    {1970,1,12,1,23,59,59,0}, {1970,1,12,1,0,0,0,0},    {1970,1,21,3,0,0,0,0},
    {1970,1,21,3,23,59,59,0}, {1970,1,28,3,0,0,0,0},    {1970,1,28,3,23,59,59,0},
    {1970,1,29,4,0,0,0,0},    {1970,1,29,4,23,59,59,0}, {1970,1,30,5,0,0,0,0},
    {1970,1,30,5,23,59,59,0}, {1970,1,31,6,0,0,0,0},    {1970,1,31,6,23,59,59,0}, /* 30 */

    {1972,2,1,2,0,0,0,0},     {1972,2,1,2,0,0,1,0},     {1972,2,1,2,0,0,33,0},
    {1972,2,1,2,0,0,59,0},    {1972,2,1,2,0,1,0,0},     {1972,2,1,2,0,1,1,0},
    {1972,2,1,2,0,1,35,0},    {1972,2,1,2,0,1,59,0},    {1972,2,1,2,0,33,0,0},
    {1972,2,1,2,0,59,59,0},   {1972,2,1,2,1,0,0,0},     {1972,2,1,2,1,0,1,0},
    {1972,2,1,2,1,1,1,0},     {1972,2,1,2,11,0,0,0},    {1972,2,1,2,12,0,0,0},
    {1972,2,1,2,23,59,59,0},  {1972,2,2,3,0,0,0,0},     {1972,2,7,1,12,35,35,0},
    {1972,2,12,6,23,59,59,0}, {1972,2,12,6,0,0,0,0},    {1972,2,21,1,0,0,0,0},
    {1972,2,21,1,23,59,59,0}, {1972,2,28,1,0,0,0,0},    {1972,2,28,1,23,59,59,0},
    {1972,2,29,2,0,0,0,0},    {1972,2,29,2,23,59,59,0},                         /* 56 */

    {2009,12,1,2,0,0,0,0},    {2009,12,1,2,0,0,1,0},    {2009,12,1,2,0,0,33,0},
    {2009,12,1,2,0,0,59,0},   {2009,12,1,2,0,1,0,0},    {2009,12,1,2,0,1,1,0},
    {2009,12,1,2,0,1,35,0},   {2009,12,1,2,0,1,59,0},   {2009,12,1,2,0,33,0,0},
    {2009,12,1,2,0,59,59,0},  {2009,12,1,2,1,0,0,0},    {2009,12,1,2,1,0,1,0},
    {2009,12,1,2,1,1,1,0},    {2009,12,1,2,11,0,0,0},   {2009,12,1,2,12,0,0,0},
    {2009,12,1,2,23,59,59,0}, {2009,12,2,3,0,0,0,0},    {2009,12,7,1,12,35,35,0},

    {2009,12,12,6,23,59,59,0},{2009,12,12,6,0,0,0,0},   {2009,12,21,1,0,0,0,0},

    {2009,12,21,1,23,59,59,0},{2009,12,28,1,0,0,0,0},   {2009,12,28,1,23,59,59,0},
    {2009,12,29,2,0,0,0,0},   {2009,12,29,2,23,59,59,0},{2009,12,30,3,0,0,0,0},
    {2009,12,30,3,23,59,59,0},{2009,12,31,4,0,0,0,0},   {2009,12,31,4,23,59,59,0} /* 86 */

  };

/* Seconds without time from 01.01.1970 to dates[i] */
pdf_u32_t datesInSeconds[DATES_SIZE] ={
    0,1,33,
    59,60,60+1,
    60+35,60+59,33*60,
    59*60+59,60*60,60*60+1,
    3600+60+1, 11*3600, 12*3600,
    23*3600+59*60+59, 1*SEC_IN_DAY, 6*SEC_IN_DAY+12*3600+35*60+35,
    11*SEC_IN_DAY+23*3600+59*60+59, 11*SEC_IN_DAY,    20*SEC_IN_DAY,
    20*SEC_IN_DAY+23*3600+59*60+59, 27*SEC_IN_DAY, 27*SEC_IN_DAY+23*3600+59*60+59,
    28*SEC_IN_DAY,   28*SEC_IN_DAY+3600*23+59*60+59, 29*SEC_IN_DAY,
    29*SEC_IN_DAY+23*3600+59*60+59, 30*SEC_IN_DAY,    30*SEC_IN_DAY+23*3600+59*60+59, /* 30 */


    2*SEC_IN_NOLEEP_YEAR + 31*SEC_IN_DAY,2*SEC_IN_NOLEEP_YEAR + 31*SEC_IN_DAY+1,2*SEC_IN_NOLEEP_YEAR + 31*SEC_IN_DAY+33,
    2*SEC_IN_NOLEEP_YEAR + 31*SEC_IN_DAY+59, 2*SEC_IN_NOLEEP_YEAR + 31*SEC_IN_DAY+60,2*SEC_IN_NOLEEP_YEAR + 31*SEC_IN_DAY+60+1,
    2*SEC_IN_NOLEEP_YEAR + 31*SEC_IN_DAY+60+35,2*SEC_IN_NOLEEP_YEAR + 31*SEC_IN_DAY+60+59,2*SEC_IN_NOLEEP_YEAR + 31*SEC_IN_DAY+33*60,
    2*SEC_IN_NOLEEP_YEAR + 31*SEC_IN_DAY+59*60+59, 2*SEC_IN_NOLEEP_YEAR + 31*SEC_IN_DAY+3600,2*SEC_IN_NOLEEP_YEAR + 31*SEC_IN_DAY+3600+1,
    2*SEC_IN_NOLEEP_YEAR + 31*SEC_IN_DAY+3600+60+1,2*SEC_IN_NOLEEP_YEAR + 31*SEC_IN_DAY+11*3600,2*SEC_IN_NOLEEP_YEAR + 31*SEC_IN_DAY+12*3600,
    2*SEC_IN_NOLEEP_YEAR + 31*SEC_IN_DAY+23*3600+59*60+59,2*SEC_IN_NOLEEP_YEAR +  32*SEC_IN_DAY,2*SEC_IN_NOLEEP_YEAR + 37*SEC_IN_DAY+12*3600+35*60+35,
    2*SEC_IN_NOLEEP_YEAR + 42*SEC_IN_DAY+23*3600+59*60+59, 2*SEC_IN_NOLEEP_YEAR + 42*SEC_IN_DAY, 2*SEC_IN_NOLEEP_YEAR + 51*SEC_IN_DAY,
    2*SEC_IN_NOLEEP_YEAR + 51*SEC_IN_DAY+23*3600+59*60+59, 2*SEC_IN_NOLEEP_YEAR + 58*SEC_IN_DAY, 2*SEC_IN_NOLEEP_YEAR + 58*SEC_IN_DAY+23*3600+59*60+59,
    2*SEC_IN_NOLEEP_YEAR + 59*SEC_IN_DAY, 2*SEC_IN_NOLEEP_YEAR + 59*SEC_IN_DAY+23*3600+59*60+59,

    DATE_01_12_2009,DATE_01_12_2009+1,DATE_01_12_2009+33,
    DATE_01_12_2009+59,DATE_01_12_2009+1*60,DATE_01_12_2009+1*60+1,
    DATE_01_12_2009+1*60+35,DATE_01_12_2009+1*60+59,DATE_01_12_2009+33*60,
    DATE_01_12_2009+59*60+59,DATE_01_12_2009+3600,DATE_01_12_2009+3600+1,
    DATE_01_12_2009+3600+60+1,DATE_01_12_2009+11*3600,DATE_01_12_2009+12*3600,
    DATE_01_12_2009+23*3600+59*60+59 , DATE_01_12_2009 + 1*SEC_IN_DAY, DATE_01_12_2009 + 6*SEC_IN_DAY+12*3600+35*60+35,
    DATE_01_12_2009 + 11*SEC_IN_DAY+23*3600+59*60+59, DATE_01_12_2009 + 11*SEC_IN_DAY, DATE_01_12_2009 + 20*SEC_IN_DAY,
    DATE_01_12_2009 + 20*SEC_IN_DAY+23*3600+59*60+59, DATE_01_12_2009 + 27*SEC_IN_DAY, DATE_01_12_2009 + 27*SEC_IN_DAY+23*3600+59*60+59,
    DATE_01_12_2009 + 28*SEC_IN_DAY, DATE_01_12_2009 + 28*SEC_IN_DAY+23*3600+59*60+59, DATE_01_12_2009 + 29*SEC_IN_DAY,
    DATE_01_12_2009 + 29*SEC_IN_DAY+23*3600+59*60+59, DATE_01_12_2009 + 30*SEC_IN_DAY, DATE_01_12_2009 + 30*SEC_IN_DAY+23*3600+59*60+59
};

/* End of pdf-time-testdata.c */

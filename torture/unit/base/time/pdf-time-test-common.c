/* -*- mode: C -*- Time-stamp: "09/04/17 00:07:51 jemarch"
 *
 *       File:         pdf-time-test-common.c
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


#include <base/time/pdf-time-test-common.h>
#include <pdf.h>

void print_pdf_time_cal(pdf_time_t time, pdf_char_t *info){
    struct  pdf_time_cal_s cal;

    pdf_time_from_cal(time, &cal);

    printf("%s %d-%d-%d %d:%d:%d +%d | dow %d\n", info, cal.year, cal.month, cal.day, cal.hour, cal.minute, cal.second, cal.gmt_offset, cal.dow); 


}

/* End of pdf-time-test-common.c */

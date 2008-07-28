/* -*- mode: C -*- Time-stamp: "08/07/28 22:27:19 jemarch"
 *
 *       File:         pdf-time-context.h
 *       Date:         Mon Apr 28 23:23:04 2008
 *
 *       GNU PDF Library - Header file for the Time module Context
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

#ifndef PDF_TIME_CONTEXT_H
#define PDF_TIME_CONTEXT_H

#include <stdio.h>
#include <pdf-time.h>


/* Initialize time context. Must be done only once at program startup!.
 *  Not thread-safe! */
pdf_status_t
pdf_time_context_init(void);


/* Get the GMT offset of the local time configuration. The offset is obtained as
 *  minutes west of GMT */
pdf_i32_t
pdf_time_context_get_gmt_offset(void);



#endif /* PDF_TIME_CONTEXT_H */

/* End of pdf-time-context.h */

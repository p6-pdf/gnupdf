/* -*- mode: C -*-
 *
 *       File:         pdf-types-pmon.h
 *       Date:         Wed Jan 26 18:02:46 2011
 *
 *       GNU PDF Library - Basic Types Module, Progress Monitors
 *
 */

/* Copyright (C) 2008-2011 Free Software Foundation, Inc */

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

#ifndef PDF_TYPES_PMON_H
#define PDF_TYPES_PMON_H

#include <config.h>

#include <pdf-types.h>
#include <pdf-error.h>

/* BEGIN PUBLIC */

typedef pdf_status_t (*pdf_pm_begin_operation_fn_t) (void *client_data);

typedef pdf_status_t (*pdf_pm_end_operation_fn_t) (void *client_data);

typedef pdf_status_t (*pdf_pm_get_duration_fn_t) (void *client_data,
                                                  void *duration);

typedef pdf_status_t (*pdf_pm_set_duration_fn_t) (void *client_data,
                                                  void *duration);

typedef pdf_status_t (*pdf_pm_get_current_value_fn_t) (void *client_data,
                                                       void *duration);

typedef pdf_status_t (*pdf_pm_set_current_value_fn_t) (void *client_data,
                                                       void *duration);

typedef pdf_status_t (*pdf_pm_set_text_fn_t) (void *client_data,
                                              void *text);

struct pdf_pm_s
{
  pdf_pm_begin_operation_fn_t begin_operation_fn;
  pdf_pm_end_operation_fn_t end_operation_fn;
  pdf_pm_get_duration_fn_t get_duration_fn;
  pdf_pm_set_duration_fn_t set_duration_fn;
  pdf_pm_get_current_value_fn_t get_current_value_fn_t;
  pdf_pm_set_current_value_fn_t set_current_value_fn_t;
  pdf_pm_set_text_fn_t set_text_fn;
};

/* END PUBLIC */

#endif /* !PDF_TYPES_PMON_H */

/* End of pdf-types-pmon.h */

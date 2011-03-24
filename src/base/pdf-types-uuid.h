/* -*- mode: C -*-
 *
 *       File:         pdf-types-uuid.h
 *       Date:         Tue Feb  8 16:48:00 2011
 *
 *       GNU PDF Library - UUID generation and utilities
 *
 */

/* Copyright (C) 2011 Free Software Foundation, Inc. */

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

#ifndef PDF_TYPES_UUID_H
#define PDF_TYPES_UUID_H

#include <config.h>

/* BEGIN PUBLIC */

#include <uuid/uuid.h>

/* END PUBLIC */

#include <pdf-types.h>

#define PDF_UUID_SIZE 46

/* BEGIN PUBLIC */

/* Types of UUIDs */
/* Currently only time-based and random-based UUIDs are supported */
enum pdf_uuid_type_e
{
  /* Time-based */
  PDF_UUID_TIME,
  /* Random-based */
  PDF_UUID_RANDOM
};

struct pdf_uuid_s
{
  uuid_t uuid;
};

typedef struct pdf_uuid_s pdf_uuid_t;

/* UUID creation */
pdf_uuid_t pdf_uuid_generate (enum pdf_uuid_type_e type);

/* Printed ASCII representation of an UUID */
pdf_char_t * pdf_uuid_string (pdf_uuid_t uuid,
                              pdf_char_t * buffer,
                              pdf_size_t buffer_size);

/* Determine if two UUIDs are equal */
pdf_bool_t pdf_uuid_equal_p (pdf_uuid_t uuid1,
                             pdf_uuid_t uuid2);

/* END PUBLIC */

#endif /* !PDF_TYPES_UUID_H */

/* End of pdf-types-uuid.h */

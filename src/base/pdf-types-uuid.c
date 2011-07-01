/* -*- mode: C -*-
 *
 *       File:         pdf-types-uuid.c
 *       Date:         Tue Feb  8 18:38:34 2011
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

#include <config.h>

#include <uuid/uuid.h>

#include <pdf-types-uuid.h>

pdf_uuid_t
pdf_uuid_generate (enum pdf_uuid_type_e type)
{
  pdf_uuid_t new_uuid;

  switch (type)
    {
    case PDF_UUID_TIME:
      uuid_generate_time (new_uuid.uuid);
      break;
    case PDF_UUID_RANDOM:
      uuid_generate_random (new_uuid.uuid);
      break;
    default:
      uuid_generate (new_uuid.uuid);
    }

  return new_uuid;
}

pdf_char_t *
pdf_uuid_string (pdf_uuid_t uuid,
                 pdf_char_t *buffer,
                 pdf_size_t buffer_size)
{
  if (buffer_size < PDF_UUID_SIZE)
    return NULL;

  uuid_unparse (uuid.uuid, buffer);

  return buffer;
}

pdf_bool_t
pdf_uuid_equal_p (pdf_uuid_t uuid1,
                  pdf_uuid_t uuid2)
{
  return (uuid_compare (uuid1.uuid, uuid2.uuid) == 0 ? PDF_TRUE : PDF_FALSE);
}

/* End of pdf-types-uuid.c */

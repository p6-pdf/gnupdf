/* -*- mode: C -*-
 *
 *       File:         pdf-hash-helper.c
 *       Date:         Thu Jul 24 21:05:05 2008
 *
 *       GNU PDF Library - Hash helper functions
 *
 */

/* Copyright (C) 2008-2011 Free Software Foundation, Inc. */

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

#include <pdf-hash-helper.h>


static void time_dispose_fn (const pdf_time_t elt);
static void stm_dispose_fn  (const pdf_stm_t elt);


pdf_bool_t
pdf_hash_add_text (pdf_hash_t        *table,
                   const pdf_char_t  *key,
                   const pdf_text_t  *value,
                   pdf_error_t      **error)
{
  return pdf_hash_add (table,
                       key,
                       value,
                       (pdf_hash_value_dispose_fn_t) pdf_text_destroy,
                       error);
}

const pdf_text_t *
pdf_hash_get_text (pdf_hash_t       *table,
                   const pdf_char_t *key)
{
  return (const pdf_text_t *) pdf_hash_get_value (table, key);
}

pdf_bool_t
pdf_hash_add_time (pdf_hash_t        *table,
                   const pdf_char_t  *key,
                   const pdf_time_t   value,
                   pdf_error_t      **error)
{
  return pdf_hash_add (table,
                       key,
                       value,
                       (pdf_hash_value_dispose_fn_t) time_dispose_fn,
                       error);
}

const pdf_time_t
pdf_hash_get_time (pdf_hash_t       *table,
                   const pdf_char_t *key)
{
  return (const pdf_time_t) pdf_hash_get_value (table, key);
}

pdf_bool_t
pdf_hash_add_list (pdf_hash_t        *table,
                   const pdf_char_t  *key,
                   const pdf_list_t  *value,
                   pdf_error_t      **error)
{
  return pdf_hash_add (table,
                       key,
                       value,
                       (pdf_hash_value_dispose_fn_t) pdf_list_destroy,
                       error);
}

const pdf_list_t *
pdf_hash_get_list (pdf_hash_t       *table,
                   const pdf_char_t *key)
{
  return (const pdf_list_t *) pdf_hash_get_value (table, key);
}

pdf_bool_t
pdf_hash_add_hash (pdf_hash_t        *table,
                   const pdf_char_t  *key,
                   const pdf_hash_t  *value,
                   pdf_error_t      **error)
{
  return pdf_hash_add (table,
                       key,
                       value,
                       (pdf_hash_value_dispose_fn_t) pdf_hash_destroy,
                       error);
}

const pdf_hash_t *
pdf_hash_get_hash (pdf_hash_t       *table,
                   const pdf_char_t *key)
{
  return (const pdf_hash_t *) pdf_hash_get_value (table, key);
}

pdf_bool_t
pdf_hash_add_stm (pdf_hash_t        *table,
                  const pdf_char_t  *key,
                  const pdf_stm_t    value,
                  pdf_error_t      **error)
{
  return pdf_hash_add (table,
                       key,
                       value,
                       (pdf_hash_value_dispose_fn_t) stm_dispose_fn,
                       error);
}

const pdf_stm_t
pdf_hash_get_stm (pdf_hash_t        *table,
                  const pdf_char_t  *key)
{
  return (const pdf_stm_t) pdf_hash_get_value (table, key);
}

pdf_bool_t
pdf_hash_add_bool (pdf_hash_t        *table,
                   const pdf_char_t  *key,
                   const pdf_bool_t   value,
                   pdf_error_t      **error)
{
  return pdf_hash_add (table,
                       key,
                       (void *)value,
                       NULL,
                       error);
}

pdf_bool_t
pdf_hash_get_bool (pdf_hash_t       *table,
                   const pdf_char_t *key)
{
  return (pdf_bool_t) pdf_hash_get_value (table, key);
}

pdf_bool_t
pdf_hash_add_size (pdf_hash_t        *table,
                   const pdf_char_t  *key,
                   const pdf_size_t   value,
                   pdf_error_t      **error)
{
  return pdf_hash_add (table,
                       key,
                       (void *)value,
                       NULL,
                       error);
}

pdf_size_t
pdf_hash_get_size (pdf_hash_t        *table,
                   const pdf_char_t  *key)
{
  return (pdf_size_t) pdf_hash_get_value (table, key);
}

pdf_bool_t
pdf_hash_add_string (pdf_hash_t        *table,
                     const pdf_char_t  *key,
                     const pdf_char_t  *value,
                     pdf_error_t      **error)
{
  return pdf_hash_add (table,
                       key,
                       (void *)value,
                       pdf_dealloc,
                       error);
}

pdf_bool_t
pdf_hash_add_duplicated_string (pdf_hash_t        *table,
                                const pdf_char_t  *key,
                                const pdf_char_t  *value,
                                pdf_error_t      **error)
{
  return pdf_hash_add (table,
                       key,
                       (void *)strdup (value),
                       pdf_dealloc,
                       error);
}

pdf_bool_t
pdf_hash_add_static_string (pdf_hash_t        *table,
                            const pdf_char_t  *key,
                            const pdf_char_t  *value,
                            pdf_error_t      **error)
{
  return pdf_hash_add (table,
                       key,
                       (void *)value,
                       NULL,
                       error);
}

const pdf_char_t *
pdf_hash_get_string (pdf_hash_t       *table,
                     const pdf_char_t *key)
{
  return (pdf_char_t *) pdf_hash_get_value (table, key);
}

static void
time_dispose_fn (const pdf_time_t elt)
{
  pdf_time_destroy (elt);
}

static void
stm_dispose_fn (const pdf_stm_t elt)
{
  pdf_size_t flushed_bytes;

  pdf_stm_flush (elt, PDF_TRUE, &flushed_bytes);
  pdf_stm_destroy (elt);
}

/* End of pdf-hash-helper.c */

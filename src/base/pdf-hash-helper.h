/* -*- mode: C -*-
 *
 *       File:         pdf-hash-helper.h
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

#ifndef PDF_HASH_HELPER_H
#define PDF_HASH_HELPER_H

#include <pdf-types.h>
#include <pdf-hash.h>
#include <pdf-text.h>
#include <pdf-time.h>
#include <pdf-list.h>
#include <pdf-fp.h>
#include <pdf-stm.h>

/* BEGIN PUBLIC */

/* Hash helpers to add/get texts */
pdf_bool_t        pdf_hash_add_text            (pdf_hash_t        *table,
                                                const pdf_char_t  *key,
                                                const pdf_text_t  *value,
                                                pdf_error_t      **error);
pdf_bool_t        pdf_hash_add_duplicated_text (pdf_hash_t        *table,
                                                const pdf_char_t  *key,
                                                const pdf_text_t  *value,
                                                pdf_error_t      **error);
const pdf_text_t *pdf_hash_get_text            (const pdf_hash_t  *table,
                                                const pdf_char_t  *key);

/* Hash helpers to add/get times */
pdf_bool_t        pdf_hash_add_time            (pdf_hash_t        *table,
                                                const pdf_char_t  *key,
                                                const pdf_time_t  *value,
                                                pdf_error_t      **error);
pdf_bool_t        pdf_hash_add_duplicated_time (pdf_hash_t        *table,
                                                const pdf_char_t  *key,
                                                const pdf_time_t  *value,
                                                pdf_error_t      **error);
pdf_bool_t        pdf_hash_add_static_time     (pdf_hash_t        *table,
                                                const pdf_char_t  *key,
                                                const pdf_time_t  *value,
                                                pdf_error_t      **error);
const pdf_time_t *pdf_hash_get_time            (const pdf_hash_t *table,
                                                const pdf_char_t  *key);

/* Hash helpers to add/get lists */
pdf_bool_t        pdf_hash_add_list   (pdf_hash_t        *table,
                                       const pdf_char_t  *key,
                                       const pdf_list_t  *value,
                                       pdf_error_t      **error);
const pdf_list_t *pdf_hash_get_list   (const pdf_hash_t  *table,
                                       const pdf_char_t  *key);

/* Hash helpers to add/get hashes */
pdf_bool_t        pdf_hash_add_hash   (pdf_hash_t        *table,
                                       const pdf_char_t  *key,
                                       const pdf_hash_t  *value,
                                       pdf_error_t      **error);
const pdf_hash_t *pdf_hash_get_hash   (const pdf_hash_t *table,
                                       const pdf_char_t  *key);

/* Hash helpers to add/get streams */
pdf_bool_t        pdf_hash_add_stm    (pdf_hash_t        *table,
                                       const pdf_char_t  *key,
                                       const pdf_stm_t   *value,
                                       pdf_error_t      **error);
const pdf_stm_t  *pdf_hash_get_stm    (const pdf_hash_t  *table,
                                       const pdf_char_t  *key);

/* Hash helpers to add/get booleans */
pdf_bool_t        pdf_hash_add_bool   (pdf_hash_t       *table,
                                       const pdf_char_t *key,
                                       const pdf_bool_t  value,
                                       pdf_error_t      **error);
pdf_bool_t        pdf_hash_get_bool   (const pdf_hash_t *table,
                                       const pdf_char_t  *key);

/* Hash helpers to add/get integer */
pdf_bool_t        pdf_hash_add_i32    (pdf_hash_t        *table,
                                       const pdf_char_t  *key,
                                       const pdf_i32_t    value,
                                       pdf_error_t      **error);
pdf_i32_t         pdf_hash_get_i32    (pdf_hash_t       *table,
                                       const pdf_char_t *key);
pdf_bool_t        pdf_hash_add_u32    (pdf_hash_t        *table,
                                       const pdf_char_t  *key,
                                       const pdf_u32_t    value,
                                       pdf_error_t      **error);
pdf_u32_t         pdf_hash_get_u32    (pdf_hash_t       *table,
                                       const pdf_char_t *key);

/* Hash helpers to add/get sizes */
pdf_bool_t        pdf_hash_add_size   (pdf_hash_t        *table,
                                       const pdf_char_t  *key,
                                       const pdf_size_t   value,
                                       pdf_error_t      **error);
pdf_size_t        pdf_hash_get_size   (const pdf_hash_t  *table,
                                       const pdf_char_t  *key);

/* Hash helpers to add/get strings */
pdf_bool_t        pdf_hash_add_string            (pdf_hash_t        *table,
                                                  const pdf_char_t  *key,
                                                  const pdf_char_t  *value,
                                                  pdf_error_t      **error);
pdf_bool_t        pdf_hash_add_duplicated_string (pdf_hash_t        *table,
                                                  const pdf_char_t  *key,
                                                  const pdf_char_t  *value,
                                                  pdf_error_t      **error);
pdf_bool_t        pdf_hash_add_static_string     (pdf_hash_t        *table,
                                                  const pdf_char_t  *key,
                                                  const pdf_char_t  *value,
                                                  pdf_error_t      **error);
const pdf_char_t *pdf_hash_get_string            (const pdf_hash_t  *table,
                                                  const pdf_char_t  *key);

/* END PUBLIC */

#endif /* !PDF_HASH_HELPER_H */

/* End of pdf-hash-helper.h */

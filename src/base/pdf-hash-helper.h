/* -*- mode: C -*-
 *
 *       File:         pdf-hash-helper.h
 *       Date:         Thu Jul 24 21:05:05 2008
 *
 *       GNU PDF Library - Hash helper functions
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

pdf_status_t
pdf_hash_add_text (pdf_hash_t table, const char *key, const pdf_text_t *elt);

pdf_status_t
pdf_hash_get_text (pdf_hash_t table, const char *key, pdf_text_t *elt);

pdf_status_t
pdf_hash_add_time (pdf_hash_t table, const char *key, const pdf_time_t *elt);

pdf_status_t
pdf_hash_get_time (pdf_hash_t table, const char *key, pdf_time_t *elt);

pdf_status_t
pdf_hash_add_list (pdf_hash_t table, const char *key, const pdf_list_t *elt);

pdf_status_t
pdf_hash_get_list (pdf_hash_t table, const char *key, pdf_list_t *elt);

pdf_status_t
pdf_hash_add_hash (pdf_hash_t table, const char *key, const pdf_hash_t *elt);

pdf_status_t
pdf_hash_get_hash (pdf_hash_t table, const char *key, pdf_hash_t *elt);

pdf_status_t
pdf_hash_add_stm (pdf_hash_t table, const char *key, const pdf_stm_t *elt);

pdf_status_t
pdf_hash_get_stm (pdf_hash_t table, const char *key, pdf_stm_t *elt);

pdf_status_t
pdf_hash_add_size (pdf_hash_t table, const char *key, const pdf_size_t elt);

pdf_status_t
pdf_hash_get_size (pdf_hash_t table, const char *key, pdf_size_t *elt);

pdf_status_t
pdf_hash_add_bool (pdf_hash_t table, const char *key, const pdf_bool_t elt);

pdf_status_t
pdf_hash_get_bool (pdf_hash_t table, const char *key, pdf_bool_t *elt);

pdf_status_t
pdf_hash_add_string (pdf_hash_t table, const char *key, const pdf_char_t *elt);

pdf_status_t
pdf_hash_get_string (pdf_hash_t table, const char *key, pdf_char_t **elt);


/* END PUBLIC */

#endif /* !PDF_HASH_HELPER_H */

/* End of pdf-hash-helper.h */

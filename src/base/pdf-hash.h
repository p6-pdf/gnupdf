/* -*- mode: C -*-
 *
 *       File:         pdf-hash.h
 *       Date:         Sat Apr  12 12:22:05 2008
 *
 *       GNU PDF Library - Header file for the Hash module
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

#ifndef PDF_HASH_H
#define PDF_HASH_H

#include <config.h>

#include <stdio.h>

#include <pdf-list.h>

/* BEGIN PUBLIC */

/* --------------------- Hash Data Types ------------------------- */

#define PDF_HASH_ITERATOR_SIZE 12

struct pdf_hash_iterator_s
{
  void *gl_itr[PDF_HASH_ITERATOR_SIZE];
};

typedef void pdf_hash_t;
typedef struct pdf_hash_iterator_s pdf_hash_iterator_t;

typedef void (*pdf_hash_value_dispose_fn_t) (const void *value);


/* --------------------- Hash Creation and Destruction ---------------------- */

pdf_hash_t *pdf_hash_new (pdf_error_t **error);

void pdf_hash_destroy (pdf_hash_t *table);


/* --------------------- Hash Property Management --------------------------- */

pdf_size_t pdf_hash_size (const pdf_hash_t *table);


/* ------------------------- Hash Key Management ---------------------------- */

pdf_bool_t pdf_hash_key_p (const pdf_hash_t *table,
                           const pdf_char_t *key);

pdf_bool_t pdf_hash_rename_key (pdf_hash_t        *table,
                                const pdf_char_t  *key,
                                const pdf_char_t  *new_key,
                                pdf_error_t      **error);


/* --------------------- Hash Add/Remove Methods ---------------------------- */

pdf_bool_t pdf_hash_add (pdf_hash_t                   *table,
                         const pdf_char_t             *key,
                         const void                   *value,
                         pdf_hash_value_dispose_fn_t   value_disp_fn,
                         pdf_error_t                 **error);

pdf_bool_t pdf_hash_remove (pdf_hash_t       *table,
                            const pdf_char_t *key);


/* --------------------- Hash Search Methods -------------------------------- */

const void *pdf_hash_get_value (const pdf_hash_t *table,
                                const pdf_char_t *key);


/* ----------------------- Hash Iterator Methods ---------------------------- */

pdf_bool_t pdf_hash_iterator_init (pdf_hash_iterator_t *itr,
                                   const pdf_hash_t    *table);

pdf_bool_t pdf_hash_iterator_next (pdf_hash_iterator_t  *itr,
                                   const pdf_char_t    **key,
                                   const void          **value);

void pdf_hash_iterator_deinit (pdf_hash_iterator_t *itr);

/* END PUBLIC */

#endif /* PDF_HASH_H */

/* End of pdf-hash.h */

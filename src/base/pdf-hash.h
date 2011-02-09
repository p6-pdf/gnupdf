/* -*- mode: C -*-
 *
 *       File:         pdf-hash.h
 *       Date:         Sat Apr  12 12:22:05 2008
 *
 *       GNU PDF Library - Header file for the Hash module
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

#ifndef PDF_HASH_H
#define PDF_HASH_H

#include <config.h>

#include <stdio.h>

#include <pdf-list.h>


/* BEGIN PUBLIC */

#define PDF_HASH_ITERATOR_SIZE 12

typedef void (*pdf_hash_element_dispose_fn_t) (const void *elt);
typedef void (*pdf_hash_key_dispose_fn_t) (const void *key);

struct pdf_hash_iterator_s
{
  void *gl_itr[PDF_HASH_ITERATOR_SIZE];
};

typedef struct pdf_hash_s pdf_hash_t;
typedef struct pdf_hash_element_s pdf_hash_element_t;
typedef struct pdf_hash_iterator_s pdf_hash_iterator_t;

/* END PUBLIC */

struct pdf_hash_element_s
{
  const char * key;
  const void * value;
  pdf_hash_element_dispose_fn_t disp_fn;
};

struct pdf_hash_s
{
  /* Both are pdf_list_t */
  pdf_list_t *keys;
  pdf_list_t *elements;
};




/* Creating and Destroying Hash Tables */

pdf_hash_t *pdf_hash_new (pdf_hash_key_dispose_fn_t   dispose_key_fn,
                          pdf_error_t               **error);

pdf_bool_t pdf_hash_destroy (pdf_hash_t   *table,
                             pdf_error_t **error);


/* Hash Table properties */

pdf_size_t pdf_hash_size (const pdf_hash_t *table);


/* Working with keys */

pdf_bool_t pdf_hash_key_p (const pdf_hash_t  *table,
                           const pdf_char_t  *key,
                           pdf_error_t      **error);

pdf_bool_t pdf_hash_rename_key (pdf_hash_t        *table,
                                const pdf_char_t  *key,
                                const pdf_char_t  *new_key,
                                pdf_error_t      **error);


 /* Adding and removing elements */

pdf_bool_t pdf_hash_add (pdf_hash_t                     *table,
                         const pdf_char_t               *key,
                         const void                     *element,
                         pdf_hash_element_dispose_fn_t   disp_fn,
                         pdf_error_t                   **error);

pdf_bool_t pdf_hash_remove (pdf_hash_t        *table,
                            const pdf_char_t  *key,
                            pdf_error_t      **error);


/* Searching elements */

const void *pdf_hash_get (const pdf_hash_t  *table,
                          const pdf_char_t  *key,
                          pdf_error_t      **error);


/* Working with iterators  */

pdf_bool_t pdf_hash_iterator_init (pdf_hash_iterator_t  *itr,
                                   const pdf_hash_t     *table,
                                   pdf_error_t         **error);

pdf_bool_t pdf_hash_iterator_next (pdf_hash_iterator_t  *itr,
                                   const pdf_char_t    **key,
                                   pdf_error_t         **error);

void pdf_hash_iterator_deinit (pdf_hash_iterator_t *itr);

/* Basic dispose functions */

void pdf_hash_element_dealloc_fn (const void * elt);

void pdf_hash_key_dealloc_fn (const void * elt);



/* END PUBLIC */

#endif /* PDF_HASH_H */

/* End of pdf-hash.h */

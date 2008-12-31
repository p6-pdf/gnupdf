/* -*- mode: C -*- Time-stamp: "08/12/27 23:44:37 jemarch"
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

#include <stdio.h>
#include <pdf-list.h>


/* BEGIN PUBLIC */
typedef void (*pdf_hash_element_dispose_fn_t) (const void *elt);
/* END PUBLIC */

struct pdf_hash_element_s
{
  const char * key;
  const void * value;
  pdf_hash_element_dispose_fn_t disp_fn;
};

typedef struct pdf_hash_element_s pdf_hash_element_t;

/* BEGIN PUBLIC */

typedef void (*pdf_hash_key_dispose_fn_t) (const void *key);

struct pdf_hash_s
{
  /* Both are gl_lists */
  void *elements, *keys;
};

struct pdf_hash_iterator_s
{
  void *gl_itr;
};

typedef struct pdf_hash_s pdf_hash_t;
typedef struct pdf_hash_iterator_s pdf_hash_iterator_t;


/* Creating and Destroying Hash Tables */

pdf_status_t
pdf_hash_new (pdf_hash_key_dispose_fn_t dispose_key_fn, pdf_hash_t *table);

pdf_status_t
pdf_hash_destroy (pdf_hash_t table);


/* Hash Table properties */

pdf_size_t
pdf_hash_size (const pdf_hash_t table);


/* Working with keys */

pdf_bool_t
pdf_hash_key_p (const pdf_hash_t table, const char *key);

pdf_status_t
pdf_hash_rename (pdf_hash_t table, const char *key, const char *new_key);


 /* Adding and removing elements */

pdf_status_t
pdf_hash_add (pdf_hash_t table, const char *key, const void *element,
              pdf_hash_element_dispose_fn_t disp_fn);

pdf_status_t
pdf_hash_remove (pdf_hash_t table, const char *key);


/* Searching elements */

pdf_status_t
pdf_hash_get (const pdf_hash_t table, const char *key, const void **elem_pointer);


/* Working with iterators  */

pdf_status_t
pdf_hash_iterator_new (const pdf_hash_t table, pdf_hash_iterator_t *iterator);

pdf_status_t
pdf_hash_iterator_next (pdf_hash_iterator_t iterator, const char **key);

pdf_status_t
pdf_hash_iterator_destroy (pdf_hash_iterator_t iterator);

/* Basic dispose functions */

void
pdf_hash_element_dealloc_fn (const void * elt);

void
pdf_hash_key_dealloc_fn (const void * elt);



/* END PUBLIC */

#endif /* PDF_HASH_H */

/* End of pdf-hash.h */

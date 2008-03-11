/* -*- mode: C -*- Time-stamp: "2008-03-11 15:21:56 gerel"
 *
 *       File:         pdf-list.h
 *       Date:         Sat Mar 1 02:14:35 2008
 *
 *       GNU PDF Library - Header file for the List module
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

#ifndef PDF_LIST_H
#define PDF_LIST_H

#include <pdf-types.h>
#include <pdf-error.h>
#include <pdf-alloc.h>

/* BEGIN PUBLIC */

/* List module API implementation */

/* Data types */

struct pdf_list_s
{
  void *gl_list;
};
 
struct pdf_list_iterator_s
{
  void *gl_iterator;
};

struct pdf_list_node_s
{
  void *gl_node;
};

typedef struct pdf_list_s pdf_list_t;
typedef struct pdf_list_node_s pdf_list_node_t;
typedef struct pdf_list_iterator_s pdf_list_iterator_t;

typedef pdf_bool_t (*pdf_list_element_equals_fn_t) (const void *elt1, const void *elt2);
typedef pdf_size_t (*pdf_list_element_hashcode_fn_t) (const void *elt);
typedef void (*pdf_list_element_dispose_fn_t) (const void *elt);

/* Creation and destruction functions */

pdf_list_t pdf_list_create (pdf_list_element_equals_fn_t
                            equals_fn, pdf_list_element_dispose_fn_t dispose_fn,
                            pdf_bool_t allow_duplicates);
pdf_status_t pdf_list_destroy (pdf_list_t list);

/* Property management functions */

pdf_size_t pdf_list_size (pdf_list_t list);

/* Element searching functions */

pdf_list_node_t pdf_list_search (pdf_list_t list, const void* element);
pdf_list_node_t pdf_list_search_from (pdf_list_t list, pdf_size_t start_index,
                                      const void* element);
pdf_list_node_t pdf_list_search_from_to (pdf_list_t list,
                                         pdf_size_t start_index,
                                         pdf_size_t end_index,
                                         const void* element);
pdf_list_node_t pdf_list_next_node (pdf_list_t list, pdf_list_node_t node);
pdf_list_node_t pdf_list_previous_node (pdf_list_t list, pdf_list_node_t node);
pdf_size_t pdf_list_indexof (pdf_list_t list, const void*element);
pdf_size_t pdf_list_indexof_from (pdf_list_t list, pdf_size_t start_index,
                                  const void* element);
pdf_size_t pdf_list_indexof_from_to (pdf_list_t list, pdf_size_t start_index,
                                     pdf_size_t end_index, const void* element);

/* Element setting and getting functions */

const void * pdf_list_node_value (pdf_list_t list, pdf_list_node_t node);
const void * pdf_list_get_at (pdf_list_t list, pdf_size_t position);
pdf_list_node_t pdf_list_set_at (pdf_list_t list, pdf_size_t position,
                                 const void* element);

/* Element addition and removal functions */

pdf_list_node_t pdf_list_add_first (pdf_list_t list, const void* element);
pdf_list_node_t pdf_list_add_last (pdf_list_t list, const void* element);
pdf_list_node_t pdf_list_add_at (pdf_list_t list, pdf_size_t position,
                                 const void* element);
pdf_status_t pdf_list_remove_node (pdf_list_t list, pdf_list_node_t node);
pdf_status_t pdf_list_remove_at (pdf_list_t list, pdf_size_t position);
pdf_status_t pdf_list_remove (pdf_list_t list, const void * element);

/* Element iterator functions */

pdf_list_iterator_t pdf_list_iterator (pdf_list_t list);
pdf_list_iterator_t pdf_list_iterator_from_to (pdf_list_t list,
                                               pdf_size_t start_index,
                                               pdf_size_t end_index);
pdf_status_t pdf_list_iterator_next (pdf_list_iterator_t *iterator,
                                     const void **element_pointer,
                                     pdf_list_node_t *node_pointer);
pdf_status_t pdf_list_iterator_free (pdf_list_iterator_t *iterator);


/* END PUBLIC */

#endif /* PDF_LIST_H */

/* End of pdf-list.h */

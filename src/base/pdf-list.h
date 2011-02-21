/* -*- mode: C -*-
 *
 *       File:         pdf-list.h
 *       Date:         Sat Mar 1 02:14:35 2008
 *
 *       GNU PDF Library - Header file for the List module
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

#ifndef PDF_LIST_H
#define PDF_LIST_H

#include <config.h>

#include <pdf-types.h>
#include <pdf-error.h>
#include <pdf-alloc.h>

/* BEGIN PUBLIC */

/* --------------------- List Data Types ------------------------- */

#define PDF_LIST_ITERATOR_SIZE 12

/* Fixed size struct */
struct pdf_list_iterator_s
{
  void *gl_iterator[PDF_LIST_ITERATOR_SIZE];
};
typedef struct pdf_list_iterator_s pdf_list_iterator_t;

typedef void pdf_list_t;
typedef void pdf_list_node_t;

typedef pdf_bool_t (*pdf_list_element_equals_fn_t) (const void *elt1,
                                                    const void *elt2);
typedef pdf_size_t (*pdf_list_element_hashcode_fn_t) (const void *elt);
typedef void (*pdf_list_element_dispose_fn_t) (const void *elt);
typedef int (*pdf_list_element_compar_fn_t) (const void *elt1,
                                             const void *elt2);

/* --------------------- List Creation and Destruction ---------------------- */

pdf_list_t *pdf_list_new (pdf_list_element_equals_fn_t    equals_fn,
                          pdf_list_element_dispose_fn_t   dispose_fn,
                          const pdf_bool_t                allow_duplicates,
                          pdf_error_t                   **error);

void pdf_list_destroy (pdf_list_t *list);


/* --------------------- List Property Management --------------------------- */

pdf_size_t pdf_list_size (const pdf_list_t *list);


/* --------------------- List Search Methods -------------------------------- */

pdf_list_node_t *pdf_list_search (const pdf_list_t  *list,
                                  const void        *element);

pdf_list_node_t *pdf_list_search_from (const pdf_list_t  *list,
                                       const pdf_size_t   start_index,
                                       const void        *element,
                                       pdf_error_t      **error);

pdf_list_node_t *pdf_list_search_from_to (const pdf_list_t  *list,
                                          const pdf_size_t   start_index,
                                          const pdf_size_t   end_index,
                                          const void        *element,
                                          pdf_error_t      **error);

pdf_list_node_t *pdf_list_next_node (const pdf_list_t       *list,
                                     const pdf_list_node_t  *node);

pdf_list_node_t *pdf_list_previous_node (const pdf_list_t       *list,
                                         const pdf_list_node_t  *node);

pdf_size_t pdf_list_indexof (const pdf_list_t  *list,
                             const void        *element);

pdf_size_t pdf_list_indexof_from (const pdf_list_t *list,
                                  const pdf_size_t  start_index,
                                  const void       *element,
                                  pdf_error_t      **error);

pdf_size_t pdf_list_indexof_from_to (const pdf_list_t  *list,
                                     const pdf_size_t   start_index,
                                     const pdf_size_t   end_index,
                                     const void        *element,
                                     pdf_error_t      **error);


/* --------------------- List Setters/Getters ------------------------------- */

const void *pdf_list_node_value (const pdf_list_t       *list,
                                 const pdf_list_node_t  *node);

const void *pdf_list_get_at (const pdf_list_t  *list,
                             const pdf_size_t   position,
                             pdf_error_t      **error);

pdf_list_node_t *pdf_list_set_at (pdf_list_t        *list,
                                  const pdf_size_t   position,
                                  const void        *element,
                                  pdf_error_t      **error);


/* --------------------- List Add/Remove Methods ---------------------------- */

pdf_list_node_t *pdf_list_add_first (pdf_list_t   *list,
                                     const void   *element,
                                     pdf_error_t **error);

pdf_list_node_t *pdf_list_add_last (pdf_list_t   *list,
                                    const void   *element,
                                    pdf_error_t **error);

pdf_list_node_t *pdf_list_add_at (pdf_list_t        *list,
                                  const pdf_size_t   position,
                                  const void        *element,
                                  pdf_error_t      **error);

pdf_bool_t pdf_list_remove_node (pdf_list_t       *list,
                                 pdf_list_node_t  *node);

pdf_bool_t pdf_list_remove_at (pdf_list_t        *list,
                               const pdf_size_t   position,
                               pdf_error_t      **error);

pdf_bool_t pdf_list_remove (pdf_list_t *list,
                            const void *element);


/* ------------------------- Sorted List Methods ---------------------------- */

pdf_list_node_t *pdf_list_sorted_add (pdf_list_t                    *list,
                                      pdf_list_element_compar_fn_t   compar_fn,
                                      const void                    *element,
                                      pdf_error_t                  **error);

pdf_bool_t pdf_list_sorted_remove (pdf_list_t                   *list,
                                   pdf_list_element_compar_fn_t  compar_fn,
                                   const void                   *element);

pdf_list_node_t *pdf_list_sorted_search (const pdf_list_t              *list,
                                         pdf_list_element_compar_fn_t   compar_fn,
                                         const void                    *element);

pdf_list_node_t *pdf_list_sorted_search_from_to (const pdf_list_t              *list,
                                                 pdf_list_element_compar_fn_t   compar_fn,
                                                 const pdf_size_t               start_index,
                                                 const pdf_size_t               end_index,
                                                 const void                    *element,
                                                 pdf_error_t                  **error);

pdf_size_t pdf_list_sorted_indexof (const pdf_list_t             *list,
                                    pdf_list_element_compar_fn_t  compar_fn,
                                    const void                   *element);

pdf_size_t pdf_list_sorted_indexof_from_to (const pdf_list_t              *list,
                                            pdf_list_element_compar_fn_t   compar_fn,
                                            const pdf_size_t               start_index,
                                            const pdf_size_t               end_index,
                                            const void                    *element,
                                            pdf_error_t                  **error);

/* --------------------- List Element Iterator Methods ---------------------- */

void pdf_list_iterator_init (pdf_list_iterator_t  *itr,
                             const pdf_list_t     *list);

pdf_bool_t pdf_list_iterator_init_from_to (pdf_list_iterator_t  *itr,
                                           const pdf_list_t     *list,
                                           const pdf_size_t      start_index,
                                           const pdf_size_t      end_index,
                                           pdf_error_t         **error);

pdf_bool_t pdf_list_iterator_next (pdf_list_iterator_t  *itr,
                                   const void          **element_pointer,
                                   pdf_list_node_t     **node_pointer);

void pdf_list_iterator_deinit (pdf_list_iterator_t *itr);

/* END PUBLIC */

#endif /* PDF_LIST_H */

/* End of pdf-list.h */

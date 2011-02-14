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

/* END PUBLIC */

#if ! defined HAVE_INLINE && ! defined COMPILING_PDF_LIST

/* BEGIN PUBLIC */

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
                                  const void        *element,
                                  pdf_error_t      **error);

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
                                     const pdf_list_node_t  *node,
                                     pdf_list_node_t        *next,
                                     pdf_error_t           **error);

pdf_list_node_t *pdf_list_previous_node (const pdf_list_t       *list,
                                         const pdf_list_node_t  *node,
                                         pdf_error_t           **error);

pdf_size_t pdf_list_indexof (const pdf_list_t  *list,
                             const void        *element,
                             pdf_error_t      **error);

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
                                 const pdf_list_node_t  *node,
                                 pdf_error_t           **error);

const void *pdf_list_get_at (const pdf_list_t  *list,
                             const pdf_size_t   position,
                             pdf_error_t      **error);

pdf_list_node_t *pdf_list_set_at (pdf_list_t       *list,
                                  const pdf_size_t  position,
                                  const void       *element,
                                  pdf_error_t      *error);


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
                                 pdf_list_node_t  *node,
                                 pdf_error_t     **error);

pdf_bool_t pdf_list_remove_at (pdf_list_t        *list,
                               const pdf_size_t   position,
                               pdf_error_t      **error);

pdf_bool_t pdf_list_remove (pdf_list_t   *list,
                            const void   *element,
                            pdf_error_t **error);


/* ------------------------- Sorted List Methods ---------------------------- */

pdf_list_node_t *pdf_list_sorted_add (pdf_list_t                    *list,
                                      pdf_list_element_compar_fn_t   compar_fn,
                                      const void                    *element,
                                      pdf_error_t                  **error);

pdf_bool_t pdf_list_sorted_remove (pdf_list_t                    *list,
                                   pdf_list_element_compar_fn_t   compar_fn,
                                   const void                    *element,
                                   pdf_error_t                  **error);

pdf_list_node_t *pdf_list_sorted_search (const pdf_list_t              *list,
                                         pdf_list_element_compar_fn_t   compar_fn,
                                         const void                    *element,
                                         pdf_error_t                  **error);

pdf_list_node_t *pdf_list_sorted_search_from_to (const pdf_list_t              *list,
                                                 pdf_list_element_compar_fn_t   compar_fn,
                                                 const pdf_size_t               start_index,
                                                 const pdf_size_t               end_index,
                                                 const void                    *element,
                                                 pdf_error_t                  **error);

pdf_size_t pdf_list_sorted_indexof (const pdf_list_t              *list,
                                    pdf_list_element_compar_fn_t   compar_fn,
                                    const void                    *element,
                                    pdf_error_t                  **error);

pdf_size_t pdf_list_sorted_indexof_from_to (const pdf_list_t              *list,
                                            pdf_list_element_compar_fn_t   compar_fn,
                                            const pdf_size_t               start_index,
                                            const pdf_size_t               end_index,
                                            const void                    *element,
                                            pdf_error_t                  **error);

/* --------------------- List Element Iterator Methods ---------------------- */

pdf_bool_t pdf_list_iterator_init (pdf_list_iterator_t  *itr,
                                   const pdf_list_t     *list,
                                   pdf_error_t         **error);

pdf_bool_t pdf_list_iterator_init_from_to (pdf_list_iterator_t  *itr,
                                           const pdf_list_t     *list,
                                           const pdf_size_t      start_index,
                                           const pdf_size_t      end_index,
                                           pdf_error_t         **error);

pdf_bool_t pdf_list_iterator_next (pdf_list_iterator_t  *itr,
                                   const void          **element_pointer,
                                   pdf_list_node_t     **node_pointer,
                                   pdf_error_t         **error);

void pdf_list_iterator_deinit (pdf_list_iterator_t *itr);

/* END PUBLIC */

#else

/* Inlined versions of the functions */

#if defined COMPILING_PDF_LIST
# define STATIC_INLINE
#else
# define STATIC_INLINE static inline
#endif /* COMPILING_PDF_LIST */

#include <gl_array_list.h>

/* Creation and destruction functions */

STATIC_INLINE pdf_list_t *
pdf_list_new (pdf_list_element_equals_fn_t    equals_fn,
              pdf_list_element_dispose_fn_t   dispose_fn,
              const pdf_bool_t                allow_duplicates,
              pdf_error_t                   **error)
{
  gl_list_t list;

  list = gl_list_nx_create_empty (GL_ARRAY_LIST,
                                  equals_fn,
                                  NULL,
                                  dispose_fn,
                                  allow_duplicates);
  if (list == NULL)
    pdf_set_error (error,
                   PDF_EDOMAIN_BASE_LIST,
                   PDF_ENOMEM,
                   "Not enough memory: Couldn't create new list");

  return (pdf_list_t *) list;
}

STATIC_INLINE void
pdf_list_destroy (pdf_list_t *list)
{
  if (list != NULL)
    gl_list_free ((gl_list_t) list);
}

/* Property management functions */

STATIC_INLINE pdf_size_t
pdf_list_size (const pdf_list_t *list)
{
  return (list != NULL ?
          (pdf_size_t) gl_list_size ((gl_list_t) list) :
          0);
}

/* Element searching functions */

STATIC_INLINE pdf_list_node_t *
pdf_list_search (const pdf_list_t  *list,
                 const void        *element,
                 pdf_error_t      **error)
{
  /* Check input arguments */
  if (list == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: list (%p), element (%p)",
                     list,
                     element);
      return NULL;
    }

  return (pdf_list_node_t *) gl_list_search ((gl_list_t) list,
                                             element);
}

STATIC_INLINE pdf_list_node_t *
pdf_list_search_from (const pdf_list_t  *list,
                      const pdf_size_t   start_index,
                      const void        *element,
                      pdf_error_t      **error)
{
  /* Check input arguments */
  if (list == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: list (%p), element (%p)",
                     list,
                     element);
      return NULL;
    }

  /* Check range */
  if (start_index >= pdf_list_size (list))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EINVRANGE,
                     "Invalid range: start index (%lu), list size (%lu)",
                     (unsigned long) start_index,
                     (unsigned long) pdf_list_size (list));
      return NULL;
    }

  return (pdf_list_node_t *) gl_list_search_from ((gl_list_t) list,
                                                  start_index,
                                                  element);
}

STATIC_INLINE pdf_list_node_t  *
pdf_list_search_from_to (const pdf_list_t  *list,
                         const pdf_size_t   start_index,
                         const pdf_size_t   end_index,
                         const void        *element,
                         pdf_error_t      **error)
{
  pdf_size_t list_size;

  /* Check input arguments */
  if (list == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: list (%p), element (%p)",
                     list,
                     element);
      return NULL;
    }

  /* Check range */
  list_size = pdf_list_size (list);
  if ((start_index >= list_size) ||
      (end_index >= list_size) ||
      (start_index >= end_index))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EINVRANGE,
                     "Invalid range: start index (%lu), "
                     "end index (%lu), list size (%lu),",
                     (unsigned long) start_index,
                     (unsigned long) end_index,
                     (unsigned long) pdf_list_size (list));
      return NULL;
    }

  return (pdf_list_node_t *) gl_list_search_from_to ((gl_list_t) list,
                                                     start_index,
                                                     end_index,
                                                     element);
}

STATIC_INLINE pdf_list_node_t *
pdf_list_next_node (const pdf_list_t       *list,
                    const pdf_list_node_t  *node,
                    pdf_error_t           **error)
{
  /* Check input arguments */
  if (list == NULL || node == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: list (%p), node (%p)",
                     list,
                     node);
      return NULL;
    }

  return (pdf_list_node_t *) gl_list_next_node ((gl_list_t) list,
                                                (gl_list_node_t) node);
}

STATIC_INLINE pdf_list_node_t *
pdf_list_previous_node (const pdf_list_t       *list,
                        const pdf_list_node_t  *node,
                        pdf_error_t           **error)
{
  /* Check input arguments */
  if (list == NULL || node == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: list (%p), node (%p)",
                     list,
                     node);
      return NULL;
    }

  return (pdf_list_node_t *) gl_list_previous_node ((gl_list_t) list,
                                                    (gl_list_node_t) node);
}

STATIC_INLINE pdf_size_t
pdf_list_indexof (const pdf_list_t  *list,
                  const void        *element,
                  pdf_error_t      **error)
{
  /* Check input arguments */
  if (list == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: list (%p)",
                     list);
      return (pdf_size_t)-1;
    }

  /* No need to look for elements in empty lists */
  if (pdf_list_size (list) == 0)
    return (pdf_size_t)-1;

  return (pdf_size_t) gl_list_indexof ((gl_list_t) list,
                                       element);
}

STATIC_INLINE pdf_size_t
pdf_list_indexof_from (const pdf_list_t  *list,
                       const pdf_size_t   start_index,
                       const void        *element,
                       pdf_error_t      **error)
{
  pdf_size_t list_size;

  /* Check input arguments */
  if (list == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: list (%p)",
                     list);
      return (pdf_size_t)-1;
    }

  list_size = pdf_list_size (list);

  /* No need to look for elements in empty lists */
  if (list_size == 0)
    return (pdf_size_t)-1;

  /* Check range */
  if (start_index >= list_size)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EINVRANGE,
                     "Invalid range: start index (%lu), list size (%lu)",
                     (unsigned long) start_index,
                     (unsigned long) pdf_list_size (list));
      return (pdf_size_t)-1;
    }

  return (pdf_size_t) gl_list_indexof_from ((gl_list_t) list,
                                            start_index,
                                            element);
}

STATIC_INLINE pdf_size_t
pdf_list_indexof_from_to (const pdf_list_t  *list,
                          const pdf_size_t   start_index,
                          const pdf_size_t   end_index,
                          const void        *element,
                          pdf_error_t      **error)
{
  pdf_size_t list_size;

  /* Check input arguments */
  if (list == NULL || element == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: list (%p), element (%p)",
                     list,
                     element);
      return (pdf_size_t)-1;
    }

  /* Check range */
  list_size = pdf_list_size (list);

  /* No need to look for elements in empty lists */
  if (list_size == 0)
    return (pdf_size_t)-1;

  if ((start_index >= list_size) ||
      (end_index >= list_size) ||
      (start_index >= end_index))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EINVRANGE,
                     "Invalid range: start index (%lu), "
                     "end index (%lu), list size (%lu),",
                     (unsigned long) start_index,
                     (unsigned long) end_index,
                     (unsigned long) pdf_list_size (list));
      return (pdf_size_t)-1;
    }

  return (pdf_size_t) gl_list_indexof_from_to ((gl_list_t) list,
                                               start_index,
                                               end_index,
                                               element);
}

/* Element setting and getting functions */

STATIC_INLINE const void *
pdf_list_node_value (const pdf_list_t       *list,
                     const pdf_list_node_t  *node,
                     pdf_error_t           **error)
{
  /* Check input arguments */
  if (list == NULL || node == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: list (%p), node (%p)",
                     list,
                     node);
      return NULL;
    }

  return gl_list_node_value ((gl_list_t) list,
                             (gl_list_node_t) node);
}


STATIC_INLINE const void *
pdf_list_get_at (const pdf_list_t  *list,
                 const pdf_size_t   position,
                 pdf_error_t      **error)
{
  /* Check input arguments */
  if (list == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: list (%p)",
                     list);
      return NULL;
    }

  /* Check range */
  if (position >= pdf_list_size (list))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EINVRANGE,
                     "Invalid range: position (%lu), list size (%lu)",
                     (unsigned long) position,
                     (unsigned long) pdf_list_size (list));
      return NULL;
    }

  return gl_list_get_at ((gl_list_t) list,
                         position);
}


STATIC_INLINE pdf_list_node_t *
pdf_list_set_at (pdf_list_t        *list,
                 const pdf_size_t   position,
                 const void        *element,
                 pdf_error_t      **error)
{
  /* Check input arguments */
  if (list == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: list (%p)",
                     list);
      return NULL;
    }

  /* Check range */
  if (position >= pdf_list_size (list))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EINVRANGE,
                     "Invalid range: position (%lu), list size (%lu)",
                     (unsigned long) position,
                     (unsigned long) pdf_list_size (list));
      return NULL;
    }

  /* If duplicates not allowed, check if element already in list */
  if ((!((const struct gl_list_impl_base *) list)->allow_duplicates) &&
      (gl_list_search ((gl_list_t) list, element) != NULL))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EEXIST,
                     "Duplicates not allowed: "
                     "Element %p already exists in list",
                     element);
      return NULL;
    }

  return gl_list_nx_set_at ((gl_list_t) list,
                            position,
                            element);
}

/* Element addition and removal functions */

STATIC_INLINE pdf_list_node_t *
pdf_list_add_first (pdf_list_t   *list,
                    const void   *element,
                    pdf_error_t **error)
{
  pdf_list_node_t *node;

  /* Check input arguments */
  if (list == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: list (%p)",
                     list);
      return NULL;
    }

  /* If duplicates not allowed, check if element already in list */
  if ((!((const struct gl_list_impl_base *) list)->allow_duplicates) &&
      (gl_list_search ((gl_list_t) list, element) != NULL))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EEXIST,
                     "Duplicates not allowed: "
                     "Element %p already exists in list",
                     element);
      return NULL;
    }

  node =  gl_list_nx_add_first ((gl_list_t) list,
                                element);
  if (node == NULL)
    pdf_set_error (error,
                   PDF_EDOMAIN_BASE_LIST,
                   PDF_ENOMEM,
                   "Not enough memory: Couldn't add new list item");

  return node;
}

STATIC_INLINE pdf_list_node_t *
pdf_list_add_last (pdf_list_t   *list,
                   const void   *element,
                   pdf_error_t **error)
{
  pdf_list_node_t *node;

  /* Check input arguments */
  if (list == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: list (%p)",
                     list);
      return NULL;
    }

  /* If duplicates not allowed, check if element already in list */
  if ((!((const struct gl_list_impl_base *) list)->allow_duplicates) &&
      (gl_list_search ((gl_list_t) list, element) != NULL))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EEXIST,
                     "Duplicates not allowed: "
                     "Element %p already exists in list",
                     element);
      return NULL;
    }

  node =  gl_list_nx_add_last ((gl_list_t) list,
                               element);
  if (node == NULL)
    pdf_set_error (error,
                   PDF_EDOMAIN_BASE_LIST,
                   PDF_ENOMEM,
                   "Not enough memory: Couldn't add new list item");

  return node;
}

STATIC_INLINE pdf_list_node_t *
pdf_list_add_at (pdf_list_t        *list,
                 const pdf_size_t   position,
                 const void        *element,
                 pdf_error_t      **error)
{
  pdf_list_node_t *node;

  /* Check input arguments */
  if (list == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: list (%p)",
                     list);
      return NULL;
    }

  /* Check range */
  if (position >= pdf_list_size (list))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EINVRANGE,
                     "Invalid range: position (%lu), list size (%lu)",
                     (unsigned long) position,
                     (unsigned long) pdf_list_size (list));
      return NULL;
    }

  /* If duplicates not allowed, check if element already in list */
  if ((!((const struct gl_list_impl_base *) list)->allow_duplicates) &&
      (gl_list_search ((gl_list_t) list, element) != NULL))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EEXIST,
                     "Duplicates not allowed: "
                     "Element %p already exists in list",
                     element);
      return NULL;
    }

  node = gl_list_nx_add_at ((gl_list_t) list,
                            position,
                            element);
  if (node == NULL)
    pdf_set_error (error,
                   PDF_EDOMAIN_BASE_LIST,
                   PDF_ENOMEM,
                   "Not enough memory: Couldn't add new list item");

  return node;
}

STATIC_INLINE pdf_bool_t
pdf_list_remove_node (pdf_list_t       *list,
                      pdf_list_node_t  *node,
                      pdf_error_t     **error)
{
  /* Check input arguments */
  if (list == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: list (%p)",
                     list);
      return PDF_FALSE;
    }

  return (gl_list_remove_node ((gl_list_t) list,
                               (gl_list_node_t) node) ?
          PDF_TRUE : PDF_FALSE);
}

STATIC_INLINE pdf_bool_t
pdf_list_remove_at (pdf_list_t        *list,
                    const pdf_size_t   position,
                    pdf_error_t      **error)
{
  /* Check input arguments */
  if (list == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: list (%p)",
                     list);
      return PDF_FALSE;
    }

  /* Check range */
  if (position >= pdf_list_size (list))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EINVRANGE,
                     "Invalid range: position (%lu), list size (%lu)",
                     (unsigned long) position,
                     (unsigned long) pdf_list_size (list));
      return PDF_FALSE;
    }

  return (gl_list_remove_at ((gl_list_t) list, position) ?
          PDF_TRUE : PDF_FALSE);
}

STATIC_INLINE pdf_bool_t
pdf_list_remove (pdf_list_t   *list,
                 const void   *element,
                 pdf_error_t **error)
{
  /* Check input arguments */
  if (list == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: list (%p)",
                     list);
      return PDF_FALSE;
    }

  return (gl_list_remove ((gl_list_t) list, element) ?
          PDF_TRUE : PDF_FALSE);
}

/* Element iterator functions */

STATIC_INLINE pdf_bool_t
pdf_list_iterator_init (pdf_list_iterator_t  *itr,
                        const pdf_list_t     *list,
                        pdf_error_t         **error)
{
  /* Check input arguments */
  if (itr == NULL || list == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: itr (%p), list (%p)",
                     itr,
                     list);
      return PDF_FALSE;
    }

  *((gl_list_iterator_t *)itr) = gl_list_iterator ((gl_list_t) list);

  return PDF_TRUE;
}

STATIC_INLINE pdf_bool_t
pdf_list_iterator_init_from_to (pdf_list_iterator_t  *itr,
                                const pdf_list_t     *list,
                                const pdf_size_t      start_index,
                                const pdf_size_t      end_index,
                                pdf_error_t         **error)
{
  pdf_size_t list_size;

  /* Check input arguments */
  if (itr == NULL || list == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: itr (%p), list (%p)",
                     itr,
                     list);
      return PDF_FALSE;
    }

  /* Check range */
  list_size = pdf_list_size (list);
  if ((start_index >= list_size) ||
      (end_index >= list_size) ||
      (start_index >= end_index))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EINVRANGE,
                     "Invalid range: start index (%lu), "
                     "end index (%lu), list size (%lu),",
                     (unsigned long) start_index,
                     (unsigned long) end_index,
                     (unsigned long) pdf_list_size (list));
      return PDF_FALSE;
    }

  *((gl_list_iterator_t *)itr) = gl_list_iterator_from_to ((gl_list_t) list,
                                                           start_index,
                                                           end_index);
  return PDF_TRUE;
}

STATIC_INLINE pdf_bool_t
pdf_list_iterator_next (pdf_list_iterator_t  *itr,
                        const void          **element_pointer,
                        pdf_list_node_t     **node_pointer,
                        pdf_error_t         **error)
{
  const void *element;
  gl_list_node_t node;

  /* Check input arguments */
  if (itr == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: itr (%p)",
                     itr);
      return PDF_FALSE;
    }

  if (!gl_list_iterator_next (((gl_list_iterator_t *) itr),
                              &element,
                              &node))
    {
      if (element_pointer)
        *element_pointer = NULL;
      if (node_pointer)
        *node_pointer = NULL;
      return PDF_FALSE;
    }

  if (element_pointer)
    *element_pointer = element;
  if (node_pointer)
    *node_pointer = (pdf_list_node_t *) node;
  return PDF_TRUE;
}

STATIC_INLINE void
pdf_list_iterator_deinit (pdf_list_iterator_t *itr)
{
  if (itr)
    gl_list_iterator_free ((gl_list_iterator_t *) itr);
}


STATIC_INLINE pdf_list_node_t *
pdf_list_sorted_add (pdf_list_t                    *list,
                     pdf_list_element_compar_fn_t   compar_fn,
                     const void                    *element,
                     pdf_error_t                  **error)
{
  pdf_list_node_t *node;

  /* Check input arguments */
  if (list == NULL || compar_fn == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: list (%p), compar_fn (%p)",
                     list,
                     compar_fn);
      return NULL;
    }

  node = gl_sortedlist_nx_add ((gl_list_t) list,
                               compar_fn,
                               element);
  if (node == NULL)
    pdf_set_error (error,
                   PDF_EDOMAIN_BASE_LIST,
                   PDF_ENOMEM,
                   "Not enough memory: Couldn't add new list item");

  return node;
}

STATIC_INLINE pdf_bool_t
pdf_list_sorted_remove (pdf_list_t                    *list,
                        pdf_list_element_compar_fn_t   compar_fn,
                        const void                    *element,
                        pdf_error_t                  **error)
{
  /* Check input arguments */
  if (list == NULL || compar_fn == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: list (%p), compar_fn (%p)",
                     list,
                     compar_fn);
      return PDF_FALSE;
    }

  return (gl_sortedlist_remove ((gl_list_t) list,
                                compar_fn,
                                element) ?
          PDF_TRUE : PDF_FALSE);
}

STATIC_INLINE pdf_list_node_t *
pdf_list_sorted_search (const pdf_list_t              *list,
                        pdf_list_element_compar_fn_t   compar_fn,
                        const void                    *element,
                        pdf_error_t                  **error)
{
  /* Check input arguments */
  if (list == NULL || compar_fn == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: list (%p), compar_fn (%p)",
                     list,
                     compar_fn);
      return PDF_FALSE;
    }

  return (pdf_list_node_t *) gl_sortedlist_search ((gl_list_t) list,
                                                   compar_fn,
                                                   element);
}

STATIC_INLINE pdf_list_node_t *
pdf_list_sorted_search_from_to (const pdf_list_t              *list,
                                pdf_list_element_compar_fn_t   compar_fn,
                                const pdf_size_t               start_index,
                                const pdf_size_t               end_index,
                                const void                    *element,
                                pdf_error_t                  **error)
{
  pdf_size_t list_size;

  /* Check input arguments */
  if (list == NULL || compar_fn == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: list (%p), compar_fn (%p)",
                     list,
                     compar_fn);
      return PDF_FALSE;
    }

  /* Check range */
  list_size = pdf_list_size (list);
  if ((start_index >= list_size) ||
      (end_index >= list_size) ||
      (start_index >= end_index))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EINVRANGE,
                     "Invalid range: start index (%lu), "
                     "end index (%lu), list size (%lu),",
                     (unsigned long) start_index,
                     (unsigned long) end_index,
                     (unsigned long) pdf_list_size (list));
      return PDF_FALSE;
    }

  return (pdf_list_node_t *) gl_sortedlist_search_from_to((gl_list_t) list,
                                                          compar_fn,
                                                          start_index,
                                                          end_index,
                                                          element);
}

STATIC_INLINE pdf_size_t
pdf_list_sorted_indexof (const pdf_list_t              *list,
                         pdf_list_element_compar_fn_t   compar_fn,
                         const void                    *element,
                         pdf_error_t                  **error)
{
  /* Check input arguments */
  if (list == NULL || compar_fn == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: list (%p), compar_fn (%p)",
                     list,
                     compar_fn);
      return (pdf_size_t)-1;
    }

  return (pdf_size_t) gl_sortedlist_indexof ((gl_list_t) list,
                                             compar_fn,
                                             element);
}

STATIC_INLINE pdf_size_t
pdf_list_sorted_indexof_from_to (const pdf_list_t               *list,
                                 pdf_list_element_compar_fn_t   compar_fn,
                                 const pdf_size_t               start_index,
                                 const pdf_size_t               end_index,
                                 const void                    *element,
                                 pdf_error_t                  **error)
{
  pdf_size_t list_size;

  /* Check input arguments */
  if (list == NULL || compar_fn == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EBADDATA,
                     "Wrong input arguments: list (%p), compar_fn (%p)",
                     list,
                     compar_fn);
      return (pdf_size_t)-1;
    }

  /* Check range */
  list_size = pdf_list_size (list);
  if ((start_index >= list_size) ||
      (end_index >= list_size) ||
      (start_index >= end_index))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EINVRANGE,
                     "Invalid range: start index (%lu), "
                     "end index (%lu), list size (%lu),",
                     (unsigned long) start_index,
                     (unsigned long) end_index,
                     (unsigned long) pdf_list_size (list));
      return (pdf_size_t)-1;
    }

  return (pdf_size_t) gl_sortedlist_indexof_from_to ((gl_list_t) list,
                                                     compar_fn,
                                                     start_index,
                                                     end_index,
                                                     element);
}

#endif /* COMPILING_PDF_LIST */
#endif /* PDF_LIST_H */

/* End of pdf-list.h */

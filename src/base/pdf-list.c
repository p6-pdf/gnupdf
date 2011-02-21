/* -*- mode: C -*-
 *
 *       File:         pdf-list.c
 *       Date:         Sat Mar 1 02:14:35 2008
 *
 *       GNU PDF Library - Implementation of the List module
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

#include <pdf-list.h>

/* linked list implementation from gnulib */
#include <gl_linked_list.h>

/* Creation and destruction functions */

pdf_list_t *
pdf_list_new (pdf_list_element_equals_fn_t    equals_fn,
              pdf_list_element_dispose_fn_t   dispose_fn,
              const pdf_bool_t                allow_duplicates,
              pdf_error_t                   **error)
{
  gl_list_t list;

  list = gl_list_nx_create_empty (GL_LINKED_LIST,
                                  equals_fn,
                                  NULL,
                                  dispose_fn,
                                  allow_duplicates);
  if (list == NULL)
    pdf_set_error (error,
                   PDF_EDOMAIN_BASE_LIST,
                   PDF_ENOMEM,
                   "not enough memory: couldn't create new list");

  return (pdf_list_t *) list;
}

void
pdf_list_destroy (pdf_list_t *list)
{
  if (list != NULL)
    gl_list_free ((gl_list_t) list);
}

/* Property management functions */

pdf_size_t
pdf_list_size (const pdf_list_t *list)
{
  return (list != NULL ?
          (pdf_size_t) gl_list_size ((gl_list_t) list) :
          0);
}

/* Element searching functions */

pdf_list_node_t *
pdf_list_search (const pdf_list_t  *list,
                 const void        *element)
{
  PDF_ASSERT_POINTER_RETURN_VAL (list, NULL);
  PDF_ASSERT_POINTER_RETURN_VAL (element, NULL);

  return (pdf_list_node_t *) gl_list_search ((gl_list_t) list,
                                             element);
}

pdf_list_node_t *
pdf_list_search_from (const pdf_list_t  *list,
                      const pdf_size_t   start_index,
                      const void        *element,
                      pdf_error_t      **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (list, NULL);
  PDF_ASSERT_POINTER_RETURN_VAL (element, NULL);

  /* Check range */
  if (start_index >= pdf_list_size (list))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EINVRANGE,
                     "invalid range: start index (%lu), list size (%lu)",
                     (unsigned long) start_index,
                     (unsigned long) pdf_list_size (list));
      return NULL;
    }

  return (pdf_list_node_t *) gl_list_search_from ((gl_list_t) list,
                                                  start_index,
                                                  element);
}

pdf_list_node_t  *
pdf_list_search_from_to (const pdf_list_t  *list,
                         const pdf_size_t   start_index,
                         const pdf_size_t   end_index,
                         const void        *element,
                         pdf_error_t      **error)
{
  pdf_size_t list_size;

  PDF_ASSERT_POINTER_RETURN_VAL (list, NULL);
  PDF_ASSERT_POINTER_RETURN_VAL (element, NULL);

  /* Check range */
  list_size = pdf_list_size (list);
  if ((start_index >= list_size) ||
      (end_index > list_size) ||
      (start_index >= end_index))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EINVRANGE,
                     "invalid range: start index (%lu), "
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

pdf_list_node_t *
pdf_list_next_node (const pdf_list_t       *list,
                    const pdf_list_node_t  *node)
{
  PDF_ASSERT_POINTER_RETURN_VAL (list, NULL);
  PDF_ASSERT_POINTER_RETURN_VAL (node, NULL);

  return (pdf_list_node_t *) gl_list_next_node ((gl_list_t) list,
                                                (gl_list_node_t) node);
}

pdf_list_node_t *
pdf_list_previous_node (const pdf_list_t       *list,
                        const pdf_list_node_t  *node)
{
  PDF_ASSERT_POINTER_RETURN_VAL (list, NULL);
  PDF_ASSERT_POINTER_RETURN_VAL (node, NULL);

  return (pdf_list_node_t *) gl_list_previous_node ((gl_list_t) list,
                                                    (gl_list_node_t) node);
}

pdf_size_t
pdf_list_indexof (const pdf_list_t  *list,
                  const void        *element)
{
  PDF_ASSERT_POINTER_RETURN_VAL (list, (pdf_size_t)-1);
  PDF_ASSERT_POINTER_RETURN_VAL (element, (pdf_size_t)-1);

  /* No need to look for elements in empty lists */
  if (pdf_list_size (list) == 0)
    return (pdf_size_t)-1;

  return (pdf_size_t) gl_list_indexof ((gl_list_t) list,
                                       element);
}

pdf_size_t
pdf_list_indexof_from (const pdf_list_t  *list,
                       const pdf_size_t   start_index,
                       const void        *element,
                       pdf_error_t      **error)
{
  pdf_size_t list_size;

  PDF_ASSERT_POINTER_RETURN_VAL (list, (pdf_size_t)-1);
  PDF_ASSERT_POINTER_RETURN_VAL (element, (pdf_size_t)-1);

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
                     "invalid range: start index (%lu), list size (%lu)",
                     (unsigned long) start_index,
                     (unsigned long) pdf_list_size (list));
      return (pdf_size_t)-1;
    }

  return (pdf_size_t) gl_list_indexof_from ((gl_list_t) list,
                                            start_index,
                                            element);
}

pdf_size_t
pdf_list_indexof_from_to (const pdf_list_t  *list,
                          const pdf_size_t   start_index,
                          const pdf_size_t   end_index,
                          const void        *element,
                          pdf_error_t      **error)
{
  pdf_size_t list_size;

  PDF_ASSERT_POINTER_RETURN_VAL (list, (pdf_size_t)-1);
  PDF_ASSERT_POINTER_RETURN_VAL (element, (pdf_size_t)-1);

  /* Check range */
  list_size = pdf_list_size (list);

  /* No need to look for elements in empty lists */
  if (list_size == 0)
    return (pdf_size_t)-1;

  if ((start_index >= list_size) ||
      (end_index > list_size) ||
      (start_index >= end_index))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EINVRANGE,
                     "invalid range: start index (%lu), "
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

const void *
pdf_list_node_value (const pdf_list_t       *list,
                     const pdf_list_node_t  *node)
{
  PDF_ASSERT_POINTER_RETURN_VAL (list, NULL);
  PDF_ASSERT_POINTER_RETURN_VAL (node, NULL);

  return gl_list_node_value ((gl_list_t) list,
                             (gl_list_node_t) node);
}


const void *
pdf_list_get_at (const pdf_list_t  *list,
                 const pdf_size_t   position,
                 pdf_error_t      **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (list, NULL);

  /* Check range */
  if (position >= pdf_list_size (list))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EINVRANGE,
                     "invalid range: position (%lu), list size (%lu)",
                     (unsigned long) position,
                     (unsigned long) pdf_list_size (list));
      return NULL;
    }

  return gl_list_get_at ((gl_list_t) list,
                         position);
}


pdf_list_node_t *
pdf_list_set_at (pdf_list_t        *list,
                 const pdf_size_t   position,
                 const void        *element,
                 pdf_error_t      **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (list, NULL);

  /* Check range */
  if (position >= pdf_list_size (list))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EINVRANGE,
                     "invalid range: position (%lu), list size (%lu)",
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
                     "duplicates not allowed: "
                     "element %p already exists in list",
                     element);
      return NULL;
    }

  return gl_list_nx_set_at ((gl_list_t) list,
                            position,
                            element);
}

/* Element addition and removal functions */

pdf_list_node_t *
pdf_list_add_first (pdf_list_t   *list,
                    const void   *element,
                    pdf_error_t **error)
{
  pdf_list_node_t *node;

  PDF_ASSERT_POINTER_RETURN_VAL (list, NULL);
  PDF_ASSERT_POINTER_RETURN_VAL (element, NULL);

  /* If duplicates not allowed, check if element already in list */
  if ((!((const struct gl_list_impl_base *) list)->allow_duplicates) &&
      (gl_list_search ((gl_list_t) list, element) != NULL))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EEXIST,
                     "duplicates not allowed: "
                     "element %p already exists in list",
                     element);
      return NULL;
    }

  node =  gl_list_nx_add_first ((gl_list_t) list,
                                element);
  if (node == NULL)
    pdf_set_error (error,
                   PDF_EDOMAIN_BASE_LIST,
                   PDF_ENOMEM,
                   "not enough memory: couldn't add new list item");

  return node;
}

pdf_list_node_t *
pdf_list_add_last (pdf_list_t   *list,
                   const void   *element,
                   pdf_error_t **error)
{
  pdf_list_node_t *node;

  PDF_ASSERT_POINTER_RETURN_VAL (list, NULL);
  PDF_ASSERT_POINTER_RETURN_VAL (element, NULL);

  /* If duplicates not allowed, check if element already in list */
  if ((!((const struct gl_list_impl_base *) list)->allow_duplicates) &&
      (gl_list_search ((gl_list_t) list, element) != NULL))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EEXIST,
                     "duplicates not allowed: "
                     "element %p already exists in list",
                     element);
      return NULL;
    }

  node =  gl_list_nx_add_last ((gl_list_t) list,
                               element);
  if (node == NULL)
    pdf_set_error (error,
                   PDF_EDOMAIN_BASE_LIST,
                   PDF_ENOMEM,
                   "not enough memory: couldn't add new list item");

  return node;
}

pdf_list_node_t *
pdf_list_add_at (pdf_list_t        *list,
                 const pdf_size_t   position,
                 const void        *element,
                 pdf_error_t      **error)
{
  pdf_list_node_t *node;

  PDF_ASSERT_POINTER_RETURN_VAL (list, NULL);
  PDF_ASSERT_POINTER_RETURN_VAL (element, NULL);

  /* Check range */
  if (position > pdf_list_size (list))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EINVRANGE,
                     "invalid range: position (%lu), list size (%lu)",
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
                     "duplicates not allowed: "
                     "element %p already exists in list",
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
                   "not enough memory: couldn't add new list item");

  return node;
}

pdf_bool_t
pdf_list_remove_node (pdf_list_t       *list,
                      pdf_list_node_t  *node)
{
  PDF_ASSERT_POINTER_RETURN_VAL (list, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (node, PDF_FALSE);

  return (gl_list_remove_node ((gl_list_t) list,
                               (gl_list_node_t) node) ?
          PDF_TRUE : PDF_FALSE);
}

pdf_bool_t
pdf_list_remove_at (pdf_list_t        *list,
                    const pdf_size_t   position,
                    pdf_error_t      **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (list, PDF_FALSE);

  /* Check range */
  if (position >= pdf_list_size (list))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EINVRANGE,
                     "invalid range: position (%lu), list size (%lu)",
                     (unsigned long) position,
                     (unsigned long) pdf_list_size (list));
      return PDF_FALSE;
    }

  return (gl_list_remove_at ((gl_list_t) list, position) ?
          PDF_TRUE : PDF_FALSE);
}

pdf_bool_t
pdf_list_remove (pdf_list_t   *list,
                 const void   *element)
{
  PDF_ASSERT_POINTER_RETURN_VAL (list, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (element, PDF_FALSE);

  return (gl_list_remove ((gl_list_t) list, element) ?
          PDF_TRUE : PDF_FALSE);
}

/* Element iterator functions */

void
pdf_list_iterator_init (pdf_list_iterator_t *itr,
                        const pdf_list_t    *list)
{
  PDF_ASSERT_POINTER_RETURN (itr);
  PDF_ASSERT_POINTER_RETURN (list);

  *((gl_list_iterator_t *)itr) = gl_list_iterator ((gl_list_t) list);
}

pdf_bool_t
pdf_list_iterator_init_from_to (pdf_list_iterator_t  *itr,
                                const pdf_list_t     *list,
                                const pdf_size_t      start_index,
                                const pdf_size_t      end_index,
                                pdf_error_t         **error)
{
  pdf_size_t list_size;

  PDF_ASSERT_POINTER_RETURN_VAL (itr, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (list, PDF_FALSE);

  /* Check range */
  list_size = pdf_list_size (list);
  if ((start_index >= list_size) ||
      (end_index > list_size) ||
      (start_index > end_index))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EINVRANGE,
                     "invalid range: start index (%lu), "
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

pdf_bool_t
pdf_list_iterator_next (pdf_list_iterator_t  *itr,
                        const void          **element_pointer,
                        pdf_list_node_t     **node_pointer)
{
  const void *element;
  gl_list_node_t node;

  PDF_ASSERT_POINTER_RETURN_VAL (itr, PDF_FALSE);

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

void
pdf_list_iterator_deinit (pdf_list_iterator_t *itr)
{
  if (itr)
    gl_list_iterator_free ((gl_list_iterator_t *) itr);
}


pdf_list_node_t *
pdf_list_sorted_add (pdf_list_t                    *list,
                     pdf_list_element_compar_fn_t   compar_fn,
                     const void                    *element,
                     pdf_error_t                  **error)
{
  pdf_list_node_t *node;

  PDF_ASSERT_POINTER_RETURN_VAL (list, NULL);
  PDF_ASSERT_POINTER_RETURN_VAL (compar_fn, NULL);
  PDF_ASSERT_POINTER_RETURN_VAL (element, NULL);

  node = gl_sortedlist_nx_add ((gl_list_t) list,
                               compar_fn,
                               element);
  if (node == NULL)
    pdf_set_error (error,
                   PDF_EDOMAIN_BASE_LIST,
                   PDF_ENOMEM,
                   "not enough memory: couldn't add new list item");

  return node;
}

pdf_bool_t
pdf_list_sorted_remove (pdf_list_t                   *list,
                        pdf_list_element_compar_fn_t  compar_fn,
                        const void                   *element)
{
  PDF_ASSERT_POINTER_RETURN_VAL (list, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (compar_fn, PDF_FALSE);

  return (gl_sortedlist_remove ((gl_list_t) list,
                                compar_fn,
                                element) ?
          PDF_TRUE : PDF_FALSE);
}

pdf_list_node_t *
pdf_list_sorted_search (const pdf_list_t             *list,
                        pdf_list_element_compar_fn_t  compar_fn,
                        const void                   *element)
{
  PDF_ASSERT_POINTER_RETURN_VAL (list, NULL);
  PDF_ASSERT_POINTER_RETURN_VAL (compar_fn, NULL);
  PDF_ASSERT_POINTER_RETURN_VAL (element, NULL);

  return (pdf_list_node_t *) gl_sortedlist_search ((gl_list_t) list,
                                                   compar_fn,
                                                   element);
}

pdf_list_node_t *
pdf_list_sorted_search_from_to (const pdf_list_t              *list,
                                pdf_list_element_compar_fn_t   compar_fn,
                                const pdf_size_t               start_index,
                                const pdf_size_t               end_index,
                                const void                    *element,
                                pdf_error_t                  **error)
{
  pdf_size_t list_size;

  PDF_ASSERT_POINTER_RETURN_VAL (list, NULL);
  PDF_ASSERT_POINTER_RETURN_VAL (compar_fn, NULL);
  PDF_ASSERT_POINTER_RETURN_VAL (element, NULL);

  /* Check range */
  list_size = pdf_list_size (list);
  if ((start_index >= list_size) ||
      (end_index > list_size) ||
      (start_index >= end_index))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EINVRANGE,
                     "invalid range: start index (%lu), "
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

pdf_size_t
pdf_list_sorted_indexof (const pdf_list_t             *list,
                         pdf_list_element_compar_fn_t  compar_fn,
                         const void                   *element)
{
  PDF_ASSERT_POINTER_RETURN_VAL (list, (pdf_size_t)-1);
  PDF_ASSERT_POINTER_RETURN_VAL (compar_fn, (pdf_size_t)-1);
  PDF_ASSERT_POINTER_RETURN_VAL (element, (pdf_size_t)-1);

  return (pdf_size_t) gl_sortedlist_indexof ((gl_list_t) list,
                                             compar_fn,
                                             element);
}

pdf_size_t
pdf_list_sorted_indexof_from_to (const pdf_list_t               *list,
                                 pdf_list_element_compar_fn_t   compar_fn,
                                 const pdf_size_t               start_index,
                                 const pdf_size_t               end_index,
                                 const void                    *element,
                                 pdf_error_t                  **error)
{
  pdf_size_t list_size;

  PDF_ASSERT_POINTER_RETURN_VAL (list, (pdf_size_t)-1);
  PDF_ASSERT_POINTER_RETURN_VAL (compar_fn, (pdf_size_t)-1);
  PDF_ASSERT_POINTER_RETURN_VAL (element, (pdf_size_t)-1);

  /* Check range */
  list_size = pdf_list_size (list);
  if ((start_index >= list_size) ||
      (end_index > list_size) ||
      (start_index >= end_index))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_LIST,
                     PDF_EINVRANGE,
                     "invalid range: start index (%lu), "
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

/* End of pdf-list.c */

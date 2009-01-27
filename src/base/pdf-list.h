/* -*- mode: C -*- Time-stamp: "2009-01-27 13:23:28 gerel"
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

#include <config.h>

/* BEGIN PUBLIC */
#include <stdbool.h>
/* END PUBLIC */

#include <pdf-types.h>
#include <pdf-error.h>
#include <pdf-alloc.h>

/* BEGIN PUBLIC */

/* List module API implementation */

/* Data types */

#define PDF_LIST_ITERATOR_SIZE 48

struct pdf_list_s
{
  void *gl_list;
  pdf_bool_t allow_duplicates;
};

struct pdf_list_iterator_s
{
  char gl_iterator[PDF_LIST_ITERATOR_SIZE];
};

struct pdf_list_node_s
{
  void *gl_node;
};

typedef struct pdf_list_s pdf_list_t;
typedef struct pdf_list_node_s pdf_list_node_t;
typedef struct pdf_list_iterator_s pdf_list_iterator_t;

typedef bool (*pdf_list_element_equals_fn_t) (const void *elt1, const void *elt2);
typedef pdf_size_t (*pdf_list_element_hashcode_fn_t) (const void *elt);
typedef void (*pdf_list_element_dispose_fn_t) (const void *elt);
typedef int (*pdf_list_element_compar_fn_t) (const void *elt1, const void *elt2);


/* END PUBLIC */

#if ! defined(HAVE_INLINE) && ! defined(COMPILING_PDF_LIST)

/* BEGIN PUBLIC */

/* Creation and destruction functions */

pdf_status_t pdf_list_new (pdf_list_element_equals_fn_t equals_fn,
                              pdf_list_element_dispose_fn_t dispose_fn,
                              const pdf_bool_t allow_duplicates,
                              pdf_list_t *list);

pdf_status_t pdf_list_destroy (pdf_list_t list);

/* Property management functions */

pdf_size_t pdf_list_size (const pdf_list_t list);

/* Element searching functions */

pdf_status_t pdf_list_search (const pdf_list_t list, const void* element, pdf_list_node_t *node);
pdf_status_t pdf_list_search_from (const pdf_list_t list, const pdf_size_t start_index,
                                   const void* element, pdf_list_node_t *node);
pdf_status_t pdf_list_search_from_to (const pdf_list_t list,
                                      const pdf_size_t start_index,
                                      const pdf_size_t end_index,
                                      const void* element,
                                      pdf_list_node_t *node);
pdf_status_t pdf_list_next_node (const pdf_list_t list, const pdf_list_node_t node, pdf_list_node_t *next);
pdf_status_t pdf_list_previous_node (const pdf_list_t list, const pdf_list_node_t node, pdf_list_node_t *prev);
pdf_status_t pdf_list_indexof (const pdf_list_t list, const void *element, pdf_size_t *position);
pdf_status_t pdf_list_indexof_from (const pdf_list_t list, const pdf_size_t start_index,
                                    const void *element, pdf_size_t *position);
pdf_status_t pdf_list_indexof_from_to (const pdf_list_t list, const pdf_size_t start_index,
                                       const pdf_size_t end_index, const void* element,
                                       pdf_size_t *position);

/* Element setting and getting functions */

const void * pdf_list_node_value (const pdf_list_t list, const pdf_list_node_t node);
const pdf_status_t pdf_list_get_at (const pdf_list_t list, const pdf_size_t position, void **value);
pdf_status_t pdf_list_set_at (pdf_list_t list, const pdf_size_t position,
                              const void* element, pdf_list_node_t *node);

/* Element addition and removal functions */

pdf_status_t pdf_list_add_first (pdf_list_t list, const void* element, pdf_list_node_t *node);
pdf_status_t pdf_list_add_last (pdf_list_t list, const void* element, pdf_list_node_t *node);
pdf_status_t pdf_list_add_at (pdf_list_t list, const pdf_size_t position,
                              const void* element, pdf_list_node_t *node);
pdf_status_t pdf_list_remove_node (pdf_list_t list, const pdf_list_node_t node);
pdf_status_t pdf_list_remove_at (pdf_list_t list, const pdf_size_t position);
pdf_status_t pdf_list_remove (pdf_list_t list, const void * element);

/* Sorted list functions */

pdf_status_t
pdf_list_sorted_add (pdf_list_t list, pdf_list_element_compar_fn_t compar_fn,
                     const void* element, pdf_list_node_t * element_node);

pdf_status_t
pdf_list_sorted_remove (pdf_list_t list, pdf_list_element_compar_fn_t compar_fn,
                        const void * element);

pdf_status_t
pdf_list_sorted_search (const pdf_list_t list, pdf_list_element_compar_fn_t compar_fn,
                        const void* element, pdf_list_node_t *node);

pdf_status_t
pdf_list_sorted_search_from_to (const pdf_list_t list,
                                pdf_list_element_compar_fn_t compar_fn,
                                const pdf_size_t start_index, const pdf_size_t end_index,
                                const void* element, pdf_list_node_t *node);

pdf_status_t
pdf_list_sorted_indexof (const pdf_list_t list,
                         pdf_list_element_compar_fn_t compar_fn,
                         const void* element, pdf_size_t *position);

pdf_status_t
pdf_list_sorted_indexof_from_to (const pdf_list_t list,
                                 pdf_list_element_compar_fn_t compar_fn,
                                 const pdf_size_t start_index, const pdf_size_t end_index,
                                 const void* element, pdf_size_t *position);

/* Element iterator functions */

pdf_status_t pdf_list_iterator (const pdf_list_t list, pdf_list_iterator_t *itr);
pdf_status_t pdf_list_iterator_from_to (const pdf_list_t list,
                                        const pdf_size_t start_index,
                                        const pdf_size_t end_index,
                                        pdf_list_iterator_t *itr);
pdf_status_t pdf_list_iterator_next (pdf_list_iterator_t *iterator,
                                     const void **element_pointer,
                                     pdf_list_node_t *node_pointer);
pdf_status_t pdf_list_iterator_free (pdf_list_iterator_t *iterator);


/* END PUBLIC */

#else
/* Inlined versions of the functions */

#if __GNUC__ == 4 && __GNUC_MINOR__ >= 3 && defined(__GNUC_STDC_INLINE__)
#  define FUCKING_C99_SEMANTICS
#endif

/* Apple's gcc build > 5400 (since Xcode 3.0) doesn't support GNU
   inline in C99 mode */
#if __APPLE_CC__ > 5400 && ! defined(FUCKING_C99_SEMANTICS)
#  define FUCKING_C99_SEMANTICS
#endif

#if defined(COMPILING_PDF_LIST)
#  if defined(FUCKING_C99_SEMANTICS)
/* force exported copy */
#    define EXTERN_INLINE extern inline
#  else
#    define EXTERN_INLINE
#  endif
#else
/* For gcc >= 4.1 not working in C99 inline semantics by default
   (including the annoying 4.2 warnings about the extern inline
   hack) */
#  if defined(__GNUC_STDC_INLINE__) || defined(__GNUC_GNU_INLINE__)
#    define EXTERN_INLINE  static inline
#  else
#    define EXTERN_INLINE extern inline
#  endif
/* For gcc >= 4.3 with C99 semantics */
#  if defined(FUCKING_C99_SEMANTICS)
/* either inline or link to extern version at compiler's choice */
#    undef EXTERN_INLINE
#    define EXTERN_INLINE inline
#  endif
#endif /* COMPILING_PDF_LIST */

#include <gl_array_list.h>

/* Creation and destruction functions */

EXTERN_INLINE pdf_status_t
pdf_list_new (pdf_list_element_equals_fn_t equals_fn,
              pdf_list_element_dispose_fn_t dispose_fn,
              const pdf_bool_t allow_duplicates,
              pdf_list_t *list)
{
  pdf_status_t st;

  st = PDF_OK;

  if (list != NULL)
    {
      list->gl_list = gl_list_create_empty(GL_ARRAY_LIST, equals_fn, NULL,
                                           dispose_fn, allow_duplicates);
      list->allow_duplicates = allow_duplicates;

      if (list->gl_list == NULL)
        {
          st = PDF_ERROR;
        }
    }
  else
    {
      st = PDF_EBADDATA;
    }
  
  return (st);
}

EXTERN_INLINE pdf_status_t
pdf_list_destroy (pdf_list_t list)
{
  gl_list_free ((gl_list_t) list.gl_list);
  return PDF_OK;
}

/* Property management functions */

EXTERN_INLINE pdf_size_t
pdf_list_size (const pdf_list_t list)
{
  return ((pdf_size_t) gl_list_size((gl_list_t) list.gl_list));
}

/* Element searching functions */

EXTERN_INLINE pdf_status_t
pdf_list_search (const pdf_list_t list, 
                 const void* element,
                 pdf_list_node_t *node)
{
  pdf_status_t st;

  st = PDF_OK;
  if (node != NULL && element != NULL)
    {
      node->gl_node = gl_list_search ((gl_list_t)list.gl_list, element);
      if (node->gl_node == NULL)
        {
          st = PDF_ENONODE;
        }
    }
  else
    {
      st = PDF_EBADDATA;
    }
  
  return (st);
}

EXTERN_INLINE pdf_status_t
pdf_list_search_from (const pdf_list_t list, 
                      const pdf_size_t start_index,
                      const void* element,
                      pdf_list_node_t *node)
{
  pdf_status_t st;
  
  st = PDF_OK;

  if (node != NULL && element != NULL)
    {
      if ((start_index < pdf_list_size (list) && start_index > 0) ||
          (start_index == 0))
        {
          node->gl_node = gl_list_search_from((gl_list_t)list.gl_list,
                                              start_index, element);
          if (node->gl_node == NULL)
            {
              st = PDF_ENONODE;
            }
        }
      else
        {
          st = PDF_EINVRANGE;
        }
    }
  else
    {
      st = PDF_EBADDATA;
    }
  
  return (st);
}

EXTERN_INLINE pdf_status_t
pdf_list_search_from_to (const pdf_list_t list, 
                         const pdf_size_t start_index,
                         const pdf_size_t end_index,
                         const void* element,
                         pdf_list_node_t *node)
{
  pdf_status_t st;

  st = PDF_OK;
  
  if (node != NULL && element != NULL)
    {
      if (((start_index < pdf_list_size (list) && start_index > 0) ||
           (start_index == 0)) &&
          ((end_index <= pdf_list_size (list) && end_index > 0) ||
           (end_index == 0)) &&
          (start_index < end_index))
        {
          node->gl_node = gl_list_search_from_to((gl_list_t)list.gl_list,
                                                 start_index, end_index,
                                                 element);
          if (node->gl_node == NULL)
            {
              st = PDF_ENONODE;
            }
        }
      else
        {
          st = PDF_EINVRANGE;
        }
    }
  else
    {
      st = PDF_EBADDATA;
    }
  
  return (st);
}

EXTERN_INLINE pdf_status_t
pdf_list_next_node (const pdf_list_t list,
                    const pdf_list_node_t node,
                    pdf_list_node_t *next)
{
  pdf_status_t st;

  st = PDF_OK;
  
  if (next != NULL)
    {
      next->gl_node = gl_list_next_node ((gl_list_t)list.gl_list,
                                         (gl_list_node_t)node.gl_node);
      if (next->gl_node == NULL)
        {
          st = PDF_ENONODE;
        }
    }
  else
    {
      st = PDF_EBADDATA;
    }
  
  return (st);
}


EXTERN_INLINE pdf_status_t
pdf_list_previous_node (const pdf_list_t list, const pdf_list_node_t node, pdf_list_node_t *prev)
{
  pdf_status_t st;

  st = PDF_OK;
  
  if (prev != NULL)
    {
      prev->gl_node = gl_list_previous_node ((gl_list_t)list.gl_list,
                                             (gl_list_node_t)node.gl_node);
      if (prev->gl_node == NULL)
        {
          st = PDF_ENONODE;
        }
    }
  else
    {
      st = PDF_EBADDATA;
    }
  
  return (st);
}

EXTERN_INLINE pdf_status_t
pdf_list_indexof (const pdf_list_t list,
                  const void *element,
                  pdf_size_t *position)
{
  pdf_status_t st;
  
  st = PDF_OK;
  
  if (position != NULL && element != NULL)
    {
      *position = (pdf_size_t) gl_list_indexof ((gl_list_t)list.gl_list,
                                                element);
      if (*position == -1)
        {
          st = PDF_ENONODE;
        }
    }
  else
    {
      st = PDF_EBADDATA;
    }
  
  return (st);
}


EXTERN_INLINE pdf_status_t
pdf_list_indexof_from (const pdf_list_t list,
                       const pdf_size_t start_index,
                       const void *element,
                       pdf_size_t *position)
{
  pdf_status_t st;
  
  st = PDF_OK;
  
  if ((position != NULL) && (element != NULL))
    {
      if ((start_index > 0 && start_index < pdf_list_size (list)) ||
          start_index == 0)
        {
          *position = (pdf_size_t) gl_list_indexof_from ((gl_list_t)list.gl_list,
                                                         start_index, element);
          if (*position == -1)
            {
              st = PDF_ENONODE;
            }
        }
      else
        {
          st = PDF_EINVRANGE;
        }
    }
  else
    {
      st = PDF_EBADDATA;
    }
  
  return (st);
}


EXTERN_INLINE pdf_status_t
pdf_list_indexof_from_to (const pdf_list_t list,
                          const pdf_size_t start_index,
                          const pdf_size_t end_index,
                          const void *element,
                          pdf_size_t *position)
{
  pdf_status_t st;

  st = PDF_OK;
  
  if ((position != NULL) && (element != NULL))
    {
      if (((start_index > 0 && start_index < pdf_list_size (list)) ||
           start_index == 0) &&
          (end_index > 0 && end_index <= pdf_list_size (list)) &&
          (start_index < end_index))
        {
          *position = (pdf_size_t)
            gl_list_indexof_from_to ((gl_list_t)list.gl_list,
                                     start_index, end_index,
                                     element);
          if (*position == -1)
            {
              st = PDF_ENONODE;
            }
        }
      else
        {
          st = PDF_EINVRANGE;
        }
    }
  else
    {
      st = PDF_EBADDATA;
    }
  
  return (st);
}


/* Element setting and getting functions */

EXTERN_INLINE const void *
pdf_list_node_value (const pdf_list_t list, const pdf_list_node_t node)
{
  return (gl_list_node_value ((gl_list_t)list.gl_list,
                              (gl_list_node_t)node.gl_node));
}


EXTERN_INLINE pdf_status_t
pdf_list_get_at (const pdf_list_t list,
                 const pdf_size_t position,
                 const void **value)
{
  pdf_status_t st;
  
  st = PDF_OK;
  
  if (value != NULL)
    {
      if ((position > 0 && position < pdf_list_size (list)) ||
          (position == 0))
        {
          *value = gl_list_get_at ((gl_list_t)list.gl_list, position);
        }
      else
        {
          st = PDF_EINVRANGE;
        }  
    }
  else
    {
      st = PDF_EBADDATA;
    }
  
  return (st);
}


EXTERN_INLINE pdf_status_t
pdf_list_set_at (pdf_list_t list,
                 const pdf_size_t position,
                 const void *element,
                 pdf_list_node_t *node)
{
  pdf_status_t st;
  
  st = PDF_OK;
  
  if (list.allow_duplicates ||
      (gl_list_search ((gl_list_t)list.gl_list, element) == NULL))
    {
      if (((position > 0 && position < pdf_list_size (list)) ||
           (position == 0)))
        {
          if (node != NULL)
            {
              node->gl_node = gl_list_set_at ((gl_list_t)list.gl_list, position,
                                              element);
            }
          else
            {
              gl_list_set_at ((gl_list_t)list.gl_list, position, element);
            }
        }
      else
        {
          st = PDF_EINVRANGE;
        }  
    }
  else
    {      
      /* Duplicated list values are not allowed */
      st = PDF_EBADDATA;
    }
  
  return (st); 
}


/* Element addition and removal functions */


EXTERN_INLINE pdf_status_t
pdf_list_add_first (pdf_list_t list,
                    const void* element,
                    pdf_list_node_t *node)
{
  pdf_status_t st;
  gl_list_node_t gl_node;

  st = PDF_OK;
  if (list.allow_duplicates ||
      (gl_list_search ((gl_list_t)list.gl_list, element) == NULL))
    {
      gl_node = gl_list_add_first ((gl_list_t)list.gl_list, element);
      if (node != NULL)
        {
          node->gl_node = gl_node;
        }
    }
  else
    {
      /* Duplicated list elements are not allowed */
      st = PDF_EBADDATA;
    }
  
  return (st);
}


EXTERN_INLINE pdf_status_t
pdf_list_add_last (pdf_list_t list,
                   const void* element,
                   pdf_list_node_t *node)
{
  pdf_status_t st;
  gl_list_node_t gl_node;

  st = PDF_OK;
  if (list.allow_duplicates ||
      (gl_list_search ((gl_list_t)list.gl_list, element) == NULL))
    {
      gl_node = gl_list_add_last ((gl_list_t)list.gl_list, element);
      if (node != NULL)
        {
          node->gl_node = gl_node;
        }
    }
  else
    {
      /* Duplicated list elements are not allowed */
      st = PDF_EBADDATA;
    }
  
  return (st);
}


EXTERN_INLINE pdf_status_t
pdf_list_add_at (pdf_list_t list,
                 const pdf_size_t position,
                 const void *element,
                 pdf_list_node_t *node)
{
  pdf_status_t st;
  
  st = PDF_OK;

  if (list.allow_duplicates ||
      (gl_list_search ((gl_list_t)list.gl_list, element) == NULL))
    {
      if ((position > 0 && position < pdf_list_size (list)) ||
          (position == 0))
        {
          if (node != NULL)
            {
              node->gl_node = gl_list_add_at ((gl_list_t)list.gl_list,
                                              position, element);
            }
          else
            {
              gl_list_add_at ((gl_list_t)list.gl_list, position, element);
            }
        }
      else
        {
          st = PDF_EINVRANGE;
        }
    }
  else
    {
      /* Duplicated list values are not allowed */
      st = PDF_EBADDATA;
    }

  return st;
}


EXTERN_INLINE pdf_status_t
pdf_list_remove_node (pdf_list_t list, const pdf_list_node_t node)
{
  gl_list_remove_node ((gl_list_t)list.gl_list, (gl_list_node_t)node.gl_node);
  return PDF_OK;
}


EXTERN_INLINE pdf_status_t
pdf_list_remove_at (pdf_list_t list, const pdf_size_t position)
{
  pdf_status_t st;

  st = PDF_OK;

  if ((position > 0 && position < pdf_list_size (list)) ||
      (position == 0))
    {
      gl_list_remove_at ((gl_list_t)list.gl_list, position);
    }
  else
    {
      st = PDF_EINVRANGE;
    }

  return st;
}


EXTERN_INLINE pdf_status_t
pdf_list_remove (pdf_list_t list, const void * element)
{
  pdf_status_t st;
  
  st = PDF_OK;

  if (!gl_list_remove ((gl_list_t)list.gl_list, element))
    st = PDF_ENONODE;

  return st;
}



/* Element iterator functions */

EXTERN_INLINE pdf_status_t
pdf_list_iterator (const pdf_list_t list,
                   pdf_list_iterator_t *itr)
{
  pdf_status_t st;

  st = PDF_OK;

  if (itr != NULL)
    {
      *((gl_list_iterator_t*)itr->gl_iterator) =
        gl_list_iterator ((gl_list_t)list.gl_list);
    }
  else
    {
      st = PDF_EBADDATA;
    }
  
  return (st);
}


EXTERN_INLINE pdf_status_t
pdf_list_iterator_from_to (const pdf_list_t list, const pdf_size_t start_index,
                           const pdf_size_t end_index,
                           pdf_list_iterator_t *itr)
{
  pdf_status_t st;

  st = PDF_OK;

  if (itr != NULL)
    {
      if (((start_index > 0 && start_index < pdf_list_size (list)) ||
           start_index == 0) &&
          (end_index > 0 && end_index <= pdf_list_size (list)) &&
          (start_index < end_index))
        {
          *((gl_list_iterator_t*)itr->gl_iterator) =
            gl_list_iterator_from_to ((gl_list_t)list.gl_list, start_index,
                                          end_index);
        }
      else
        {
          st = PDF_EINVRANGE;
        }
    }
  else
    {
      st = PDF_EBADDATA;
    }

  return st;
}

EXTERN_INLINE pdf_status_t
pdf_list_iterator_next (pdf_list_iterator_t *iterator,
                        const void **element_pointer,
                        pdf_list_node_t *node_pointer)
{
  pdf_status_t st;

  st = PDF_OK;

  if (!gl_list_iterator_next (((gl_list_iterator_t*)iterator->gl_iterator),
                              element_pointer,
                              ((gl_list_node_t*)&node_pointer->gl_node)))
    st = PDF_ENONODE;

  return st;
}

EXTERN_INLINE pdf_status_t
pdf_list_iterator_free (pdf_list_iterator_t *iterator)
{
  gl_list_iterator_free ((gl_list_iterator_t*)(iterator->gl_iterator));

  return PDF_OK;
}


EXTERN_INLINE pdf_status_t
pdf_list_sorted_add (pdf_list_t list, pdf_list_element_compar_fn_t compar_fn,
                     const void* element, pdf_list_node_t * element_node)
{
  pdf_list_node_t node;
  pdf_status_t st;

  st = PDF_OK;
  
  if (compar_fn != NULL)
    {
      node.gl_node = gl_sortedlist_add ((gl_list_t)list.gl_list, compar_fn,
                                        element);
      if (element_node != NULL)
        {
          *element_node = node;
        }
    }
  else
    {
      st = PDF_EBADDATA;
    }
  
  return (st);

}

EXTERN_INLINE pdf_status_t
pdf_list_sorted_remove (pdf_list_t list, pdf_list_element_compar_fn_t compar_fn,
                        const void * element)
{
  pdf_status_t st;

  st = PDF_OK;

  if (compar_fn != NULL)
    {
      if (!gl_sortedlist_remove ((gl_list_t)list.gl_list, compar_fn, element))
        st = PDF_ENONODE;
    }
  else
    {
      st = PDF_EBADDATA;
    }

  return (st);

}

EXTERN_INLINE pdf_status_t
pdf_list_sorted_search (const pdf_list_t list, pdf_list_element_compar_fn_t compar_fn,
                        const void* element, pdf_list_node_t *node)
{

  pdf_status_t st;

  st = PDF_OK;

  if (compar_fn != NULL && node != NULL && element != NULL)
    {
      node->gl_node = gl_sortedlist_search ((gl_list_t)list.gl_list,
                                            compar_fn, element);
      if (node->gl_node == NULL)
        {
          st = PDF_ENONODE;
        }
    }
  else
    {
      st = PDF_EBADDATA;
    }

  return (st);
}

EXTERN_INLINE pdf_status_t
pdf_list_sorted_search_from_to (const pdf_list_t list,
                                pdf_list_element_compar_fn_t compar_fn,
                                const pdf_size_t start_index, const pdf_size_t end_index,
                                const void* element, pdf_list_node_t *node)
{
  pdf_status_t st;

  st = PDF_OK;

  if (compar_fn != NULL && node != NULL && element != NULL)
    {
      if (((start_index < pdf_list_size (list) && start_index > 0) ||
           (start_index == 0)) &&
          ((end_index <= pdf_list_size (list) && end_index > 0) ||
           (end_index == 0)) &&
          (start_index < end_index))
        {
          node->gl_node = gl_sortedlist_search_from_to((gl_list_t)list.gl_list,
                                                       compar_fn, start_index,
                                                       end_index, element);
          if (node->gl_node == NULL)
            {
              st = PDF_ENONODE;
            }
        }
      else
        {
          st = PDF_EINVRANGE;
        }
    }
  else
    {
      st = PDF_EBADDATA;
    }

  return (st);
}

EXTERN_INLINE pdf_status_t
pdf_list_sorted_indexof (const pdf_list_t list,
                         pdf_list_element_compar_fn_t compar_fn,
                         const void* element, pdf_size_t *position)
{

  pdf_status_t st;

  st = PDF_OK;

  if (compar_fn != NULL && position != NULL && element != NULL )
    {
      *position = (pdf_size_t) gl_sortedlist_indexof ((gl_list_t)list.gl_list,
                                                      compar_fn, element);
      if (*position == -1)
        {
          st = PDF_ENONODE;
        }
    }
  else
    {
      st = PDF_EBADDATA;
    }

  return (st);
}

EXTERN_INLINE pdf_status_t
pdf_list_sorted_indexof_from_to (const pdf_list_t list,
                                 pdf_list_element_compar_fn_t compar_fn,
                                 const pdf_size_t start_index, const pdf_size_t end_index,
                                 const void* element, pdf_size_t *position)
{
  pdf_status_t st;

  st = PDF_OK;

  if (compar_fn != NULL && position != NULL && element != NULL)
    {
      if (((start_index > 0 && start_index < pdf_list_size (list)) ||
           start_index == 0) &&
          (end_index > 0 && end_index <= pdf_list_size (list)) &&
          (start_index < end_index))
        {
          *position = (pdf_size_t)
            gl_sortedlist_indexof_from_to ((gl_list_t)list.gl_list, compar_fn,
                                           start_index, end_index,
                                           element);
          if (*position == -1)
            {
              st = PDF_ENONODE;
            }
        }
      else
        {
          st = PDF_EINVRANGE;
        }
    }
  else
    {
      st = PDF_EBADDATA;
    }


  return (st);
}

#endif /* COMPILING_PDF_LIST */
#endif /* PDF_LIST_H */

/* End of pdf-list.h */

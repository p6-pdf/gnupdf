/* -*- mode: C -*- Time-stamp: "2008-03-11 14:16:11 gerel"
 *
 *       File:         pdf-list.c
 *       Date:         Sat Mar 1 02:14:35 2008
 *
 *       GNU PDF Library - Implementation of the List module
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


#include <pdf-list.h>
#include <gl_array_list.h>

/* Creation and destruction functions */

inline pdf_list_t
pdf_list_create (pdf_list_element_equals_fn_t equals_fn,
                 pdf_list_element_dispose_fn_t dispose_fn,
                 pdf_bool_t allow_duplicates)
{
  pdf_list_t list;
  list.gl_list = gl_list_create_empty(GL_ARRAY_LIST, equals_fn, NULL,
                                      dispose_fn, allow_duplicates);
  return (list);
}

inline pdf_status_t
pdf_list_destroy (pdf_list_t list)
{
  gl_list_free ((gl_list_t) list.gl_list);
  return PDF_OK;
}

/* Property management functions */

inline pdf_size_t
pdf_list_size (pdf_list_t list)
{
  return ((pdf_size_t) gl_list_size((gl_list_t) list.gl_list));
}

/* Element searching functions */

inline pdf_list_node_t
pdf_list_search (pdf_list_t list, const void* element)
{

  pdf_list_node_t node;

  node.gl_node = gl_list_search ((gl_list_t)list.gl_list, element);
  
  return (node);
}

inline pdf_list_node_t
pdf_list_search_from (pdf_list_t list, pdf_size_t start_index,
                      const void* element)
{
  pdf_list_node_t node;

  node.gl_node = gl_list_search_from((gl_list_t)list.gl_list, start_index,
                                     element);
  
  return (node);
}

inline pdf_list_node_t
pdf_list_search_from_to (pdf_list_t list, pdf_size_t start_index,
                         pdf_size_t end_index, const void* element)
{
  pdf_list_node_t node;

  node.gl_node = gl_list_search_from_to((gl_list_t)list.gl_list, start_index,
                                        end_index, element);
  return (node);
}

inline pdf_list_node_t
pdf_list_next_node (pdf_list_t list, pdf_list_node_t node)
{
  pdf_list_node_t next;
  
  next.gl_node = gl_list_next_node ((gl_list_t)list.gl_list,
                                    (gl_list_node_t)node.gl_node);

  return (next);
}


inline pdf_list_node_t
pdf_list_previous_node (pdf_list_t list, pdf_list_node_t node)
{
  pdf_list_node_t prev;
  
  prev.gl_node = gl_list_previous_node ((gl_list_t)list.gl_list,
                                        (gl_list_node_t)node.gl_node);

  return (prev);
}

inline pdf_size_t
pdf_list_indexof (pdf_list_t list, const void*element)
{
  return ((pdf_size_t) gl_list_indexof ((gl_list_t)list.gl_list, element));
}


inline pdf_size_t
pdf_list_indexof_from (pdf_list_t list, pdf_size_t start_index,
                       const void* element)
{
  return ((pdf_size_t) gl_list_indexof_from ((gl_list_t)list.gl_list,
                                             start_index, element));
}


inline pdf_size_t
pdf_list_indexof_from_to (pdf_list_t list, pdf_size_t start_index,
                          pdf_size_t end_index, const void* element)
{
  return ((pdf_size_t) gl_list_indexof_from_to ((gl_list_t)list.gl_list,
                                                start_index, end_index,
                                                element));
}


/* Element setting and getting functions */

inline const void *
pdf_list_node_value (pdf_list_t list, pdf_list_node_t node)
{
  return (gl_list_node_value ((gl_list_t)list.gl_list,
                              (gl_list_node_t)node.gl_node));
}


inline const void *
pdf_list_get_at (pdf_list_t list, pdf_size_t position)
{
  return (gl_list_get_at ((gl_list_t)list.gl_list, position));
}


inline pdf_list_node_t
pdf_list_set_at (pdf_list_t list, pdf_size_t position, const void* element)
{
  pdf_list_node_t node;

  node.gl_node = gl_list_set_at ((gl_list_t)list.gl_list, position, element);

  return (node);
  
}


/* Element addition and removal functions */


inline pdf_list_node_t
pdf_list_add_first (pdf_list_t list, const void* element)
{
  pdf_list_node_t node;

  node.gl_node = gl_list_add_first ((gl_list_t)list.gl_list, element);
  
  return (node);
}


inline pdf_list_node_t
pdf_list_add_last (pdf_list_t list, const void* element)
{
  pdf_list_node_t node;

  node.gl_node = gl_list_add_last ((gl_list_t)list.gl_list, element);
  
  return (node);
}


inline pdf_list_node_t
pdf_list_add_at (pdf_list_t list, pdf_size_t position, const void* element)
{
  pdf_list_node_t node;

  node.gl_node = gl_list_add_at ((gl_list_t)list.gl_list, position, element);

  return (node);
}


inline pdf_status_t
pdf_list_remove_node (pdf_list_t list, pdf_list_node_t node)
{
  gl_list_remove_node ((gl_list_t)list.gl_list, (gl_list_node_t)node.gl_node);
  return PDF_OK;
}


inline pdf_status_t
pdf_list_remove_at (pdf_list_t list, pdf_size_t position)
{
  pdf_status_t st;

  st = PDF_OK;

  if (position >= 0 && position < pdf_list_size (list))
    gl_list_remove_at ((gl_list_t)list.gl_list, position);
  else
    st = PDF_EINVRANGE;

  return st;
}


inline pdf_status_t
pdf_list_remove (pdf_list_t list, const void * element)
{
  pdf_status_t st;
  
  st = PDF_OK;

  if (!gl_list_remove ((gl_list_t)list.gl_list, element))
    st = PDF_ENONODE;

  return st;
}



/* Element iterator functions */

inline pdf_list_iterator_t
pdf_list_iterator (pdf_list_t list)
{
  pdf_list_iterator_t itr;

  itr.gl_iterator = pdf_alloc (sizeof(gl_list_iterator_t));

  if (itr.gl_iterator != NULL)
    {
      *((gl_list_iterator_t*)itr.gl_iterator) =
        gl_list_iterator ((gl_list_t)list.gl_list);
    }
  else
    {
      pdf_perror (PDF_ENOMEM, "pdf_list_iterator()");
    }
  
  return (itr);
}


inline pdf_list_iterator_t
pdf_list_iterator_from_to (pdf_list_t list, pdf_size_t start_index,
                           pdf_size_t end_index)
{
  pdf_list_iterator_t itr;

  itr.gl_iterator = pdf_alloc (sizeof(gl_list_iterator_t));

  if (itr.gl_iterator != NULL)
    {
      *((gl_list_iterator_t*)itr.gl_iterator) =
        gl_list_iterator_from_to ((gl_list_t)list.gl_list, start_index,
                                  end_index);
    }
  else
    {
      pdf_perror (PDF_ENOMEM, "pdf_list_iterator_from_to()");
    }

  return (itr);
}

inline pdf_status_t
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

inline pdf_status_t
pdf_list_iterator_free (pdf_list_iterator_t *iterator)
{
  gl_list_iterator_free ((gl_list_iterator_t*)(iterator->gl_iterator));

  pdf_dealloc (iterator->gl_iterator);

  return PDF_OK;
}

/* End of pdf-list.c */

/* -*- mode: C -*- Time-stamp: "08/09/08 23:56:58 jemarch"
 *
 *       File:         pdf-hash.c
 *       Date:         Sat Apr  12 12:22:05 2008
 *
 *       GNU PDF Library - Implementation file for the Hash module
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

#include <config.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pdf-global.h>
#include <pdf-error.h>
#include <pdf-alloc.h>
#include <gl_array_list.h>
#include <gl_linkedhash_list.h>

#include <pdf-hash.h>

#define SIZE_BITS (sizeof (size_t) * CHAR_BIT)

/* Compute a hash code for a NUL-terminated string starting at X,
   and return the hash code.
   The result is platform dependent: it depends on the size of the 'size_t'
   type and on the signedness of the 'char' type.  */
static size_t hash_pjw (const void *x);

/* Returns true if both element string keys are equal */
static bool elem_key_equal (const void *elt1, const void *elt2);

/* Returns true if both string keys are equal */
static bool key_equal (const void *key1, const void *key2);

/* Returns 1 if str is only composed by numbers, 0 otherwise. */
static int key_numeric_p (const char *str);

/* Returns <0,=0,>0 if key1 is <,==, or > than key2 */
static int key_numeric_cmp (const char *key1, const char *key2);

/* Returns <0,=0,>0 if key1 is <,==, or > than key2 */
static int key_compare (const void *key1, const void *key2);

pdf_status_t
pdf_hash_new (pdf_hash_key_dispose_fn_t dispose_key_fn, pdf_hash_t *table)
{
  pdf_status_t st;

  st = PDF_OK;

  if (table != NULL)
    {
      table->elements = gl_list_create_empty (GL_LINKEDHASH_LIST,
                                              elem_key_equal, hash_pjw,
                                              NULL, 0);
      table->keys = gl_list_create_empty (GL_ARRAY_LIST, key_equal, NULL,
                                          dispose_key_fn, 0);

      if (table->elements == NULL || table->keys == NULL)
        {
          st = PDF_ENOMEM;
        }
    }
  else
    {
      st = PDF_EBADDATA;
    }

  return st;
}


pdf_status_t
pdf_hash_destroy (pdf_hash_t *table)
{
  gl_list_iterator_t itr;
  const void *elt;

  if (table != NULL && table->elements != NULL && table->keys != NULL)
    {
      itr = gl_list_iterator ((gl_list_t)table->elements);
      while (gl_list_iterator_next (&itr, &elt, NULL))
        {
          if (((pdf_hash_element_t*)elt)->disp_fn != NULL)
            {
              ((pdf_hash_element_t*)elt)->
                disp_fn ((void*) ((pdf_hash_element_t*)elt)->value);
            }
          pdf_dealloc((pdf_hash_element_t*)elt);
        }
      gl_list_iterator_free (&itr);
  
      gl_list_free((gl_list_t)table->elements);
      gl_list_free((gl_list_t)table->keys);
    }
  
  return PDF_OK;
}


pdf_size_t
pdf_hash_size (const pdf_hash_t table)
{
  return (gl_list_size ((gl_list_t) table.keys));
}


pdf_bool_t
pdf_hash_key_p (const pdf_hash_t table, const char *key)
{
  if (gl_sortedlist_search ((gl_list_t) table.keys, key_compare,
                      (const void*) key) != NULL)
    {
      return PDF_TRUE;
    }
  
  return PDF_FALSE;
}


pdf_status_t
pdf_hash_rename (pdf_hash_t table, const char *key, const char *new_key)
{
  pdf_status_t st;
  gl_list_node_t knode, vnode;
  pdf_hash_element_t elem, *updated;
  
  st = PDF_OK;

  if (key != NULL && new_key != NULL)
    {
      knode = gl_sortedlist_search ((gl_list_t) table.keys, key_compare, key);
      if (knode != NULL)
        {
          /* get the node from key and update its key*/
          elem.key = key;
          vnode = gl_list_search ((gl_list_t)table.elements, &elem);
          updated = (pdf_hash_element_t*)
            gl_list_node_value ((gl_list_t)table.elements, vnode);
          updated->key = new_key;

          /* update the lists */
          gl_list_remove_node ((gl_list_t)table.elements, vnode);
          gl_list_add_first ((gl_list_t)table.elements, updated);
          gl_sortedlist_remove ((gl_list_t)table.keys, key_compare, key);
          gl_sortedlist_add ((gl_list_t)table.keys, key_compare, new_key);
        }
      else
        {
          st = PDF_ERROR;
        }
    }
  else
    {
      st = PDF_EBADDATA;
    }

  return st;

}


pdf_status_t
pdf_hash_add (pdf_hash_t table, const char *key, const void *element,
              pdf_hash_element_dispose_fn_t disp_fn)
{
  pdf_status_t st;
  pdf_hash_element_t *newelt;
  st = PDF_OK;

  if (key != NULL && element != NULL)
    {
      newelt = pdf_alloc (sizeof (pdf_hash_element_t));
      if (newelt != NULL)
        {
          newelt->key = key;
          newelt->value = element;
          newelt->disp_fn = disp_fn;
          gl_list_add_first ((gl_list_t)table.elements, newelt);
          gl_sortedlist_add ((gl_list_t)table.keys, key_compare, key);
        }
      else
        {
          st = PDF_ENOMEM;
        }
    }
  else
    {
      st = PDF_EBADDATA;
    }
  

  return st;

}


pdf_status_t
pdf_hash_remove (pdf_hash_t table, const char *key)
{
  pdf_status_t st;
  pdf_hash_element_t elt;
  gl_list_node_t node;
  
  st = PDF_OK;

  if (key != NULL)
    {
      elt.key = key;
      node = gl_list_search ((gl_list_t)table.elements, &elt);
      if (node != NULL)
        {
          pdf_hash_element_t *removed;
          removed = (pdf_hash_element_t*)
                    gl_list_node_value((gl_list_t)table.elements, node);

          if (removed->disp_fn != NULL)
            {
              removed->disp_fn (removed->value);
            }

          pdf_dealloc(removed);
          gl_list_remove_node ((gl_list_t)table.elements, node);
          gl_sortedlist_remove ((gl_list_t)table.keys, key_compare, key);
        }
      else
        {
          st = PDF_ERROR;
        }
    }
  else
    {
      st = PDF_EBADDATA;
    }
  

  return st;

}


pdf_status_t
pdf_hash_search (const pdf_hash_t table, const char *key, const void **elem_pointer)
{
  pdf_status_t st;
  gl_list_node_t node;
  pdf_hash_element_t elem, *searched;

  st = PDF_OK;

  if (key != NULL && elem_pointer != NULL)
    {
      elem.key = key;
      node = gl_list_search ((gl_list_t)table.elements, &elem);
      if (node != NULL)
        {
          searched = (pdf_hash_element_t*)
            gl_list_node_value ((gl_list_t)table.elements, node);
          *elem_pointer = searched->value;
        }
      else
        {
          st = PDF_ERROR;
        }
    }
  else
    {
      st = PDF_EBADDATA;
    }  

  return st;

}


pdf_status_t
pdf_hash_iterator (const pdf_hash_t table, pdf_hash_iterator_t *iterator)
{
  pdf_status_t st;

  st = PDF_OK;

  if (iterator != NULL)
    {
      iterator->gl_itr = pdf_alloc (sizeof(gl_list_iterator_t));
      if (iterator->gl_itr != NULL)
        {
          *((gl_list_iterator_t*)iterator->gl_itr) =
            gl_list_iterator ((gl_list_t)table.keys);
        }
      else
        {
          st = PDF_ENOMEM;
        }
    }
  else
    {
      st = PDF_EBADDATA;
    }

  return st;

}


pdf_status_t
pdf_hash_iterator_next (pdf_hash_iterator_t *iterator, const char **key)
{
  pdf_status_t st;

  st = PDF_OK;

  if (iterator != NULL && key != NULL)
    {
      if (!gl_list_iterator_next((gl_list_iterator_t*)iterator->gl_itr,
                                 (const void**)key, NULL))
        {
          st = PDF_ERROR;
        }    
    }
  else
    {
      st = PDF_EBADDATA;
    }

  return st;
}


pdf_status_t
pdf_hash_iterator_free (pdf_hash_iterator_t *iterator)
{

  if (iterator != NULL && iterator->gl_itr != NULL)
    {
      gl_list_iterator_free ((gl_list_iterator_t*)iterator->gl_itr);
      pdf_dealloc (iterator->gl_itr);
    }

  return PDF_OK;
}





static size_t
hash_pjw (const void *elt)
{
  const pdf_hash_element_t *hashelem;
  const char *s;
  size_t h = 0;

  hashelem = (const pdf_hash_element_t*) elt;

  for (s = hashelem->key; *s; s++)
    h = *s + ((h << 9) | (h >> (SIZE_BITS - 9)));

  return h;
}


void
pdf_hash_element_dealloc_fn (const void * elt)
{
  pdf_dealloc(elt);
}


void
pdf_hash_key_dealloc_fn (const void * elt)
{
  pdf_dealloc(elt);
}


static bool
elem_key_equal (const void *elt1, const void *elt2)
{
  const char *key1, *key2;

  key1 = ((pdf_hash_element_t*)elt1)->key;
  key2 = ((pdf_hash_element_t*)elt2)->key;
  
  return (!strcmp(key1,key2));
}


static bool
key_equal (const void *key1, const void *key2)
{
  return (!strcmp((const char*)key1,(const char*)key2));
}


static int
key_numeric_p (const char *str)
{
  while (str != NULL)
    {
      if (*str < '0' || *str > '9')
        {
          return 0;
        }         
      str++;
    }
  return 1;
}


static int
key_numeric_cmp (const char *key1, const char *key2)
{
  unsigned int num1, num2;
  char *end_char;

  /* Note that no error checking is made here */
  num1 = (int) strtol (key1, &end_char, 10);
  num2 = (int) strtol (key2, &end_char, 10);

  if (num1 > num2)
    {
      return 1;
    }
  else if (num1 < num2)
    {
      return -1;
    }
  return 0;
}


static int
key_compare (const void *key1, const void *key2)
{
  if (key_numeric_p(key1))
    {
      if (key_numeric_p(key2))
        {
          return (key_numeric_cmp(key1, key2));
        }
      /* key2 is non-numeric so key1 > key2 */
      return 1;
    }
  /* key1 non-numeric */
  if (key_numeric_p(key2))
    {
      return -1;
    }
  /* both are non-numeric */
  return (strcmp((const char*)key1,(const char*)key2));
}


/* End of pdf-hash.c */

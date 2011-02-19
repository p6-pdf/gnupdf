/* -*- mode: C -*-
 *
 *       File:         pdf-hash.c
 *       Date:         Sat Apr  12 12:22:05 2008
 *
 *       GNU PDF Library - Implementation file for the Hash module
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

struct pdf_hash_element_s
{
  const pdf_char_t *key;
  const void *value;
  pdf_hash_value_dispose_fn_t value_disp_fn;
};

typedef struct pdf_hash_element_s pdf_hash_element_t;

/* Returns <0,=0,>0 if key1 is <,==, or > than key2 */
static int hash_element_compare (const pdf_hash_element_t *elt1,
                                 const pdf_hash_element_t *elt2);

/* Returns true if both keys in the elements are equal */
static pdf_bool_t hash_element_equal (const pdf_hash_element_t *elt1,
                                      const pdf_hash_element_t *elt2);

/* Compute a hash code for a NUL-terminated string starting at X,
 * and return the hash code.
 * The result is platform dependent: it depends on the size of the 'size_t'
 * type and on the signedness of the 'char' type.  */
static pdf_size_t hash_element_pjw (const pdf_hash_element_t *elt);

/* Dispose a hash element */
static void hash_element_dispose (pdf_hash_element_t *elt);

/* Find a hash element by key in the hash table */
static pdf_hash_element_t *hash_search_key (const pdf_hash_t *table,
                                            const pdf_char_t *key,
                                            gl_list_node_t   *p_node);

pdf_hash_t *
pdf_hash_new (pdf_error_t **error)
{
  gl_list_t list;

  list = gl_list_nx_create_empty (GL_LINKEDHASH_LIST,
                                  (gl_listelement_equals_fn) hash_element_equal,
                                  (gl_listelement_hashcode_fn) hash_element_pjw,
                                  (gl_listelement_dispose_fn) hash_element_dispose,
                                  PDF_FALSE);
  if (list == NULL)
    pdf_set_error (error,
                   PDF_EDOMAIN_BASE_HASH,
                   PDF_ENOMEM,
                   "Not enough memory: Couldn't create new hash table");

  return (pdf_hash_t *)list;
}

void
pdf_hash_destroy (pdf_hash_t *table)
{
  if (table != NULL)
    gl_list_free ((gl_list_t) table);
}

pdf_size_t
pdf_hash_size (const pdf_hash_t *table)
{
  return (table != NULL ?
          gl_list_size ((gl_list_t) table) :
          0);;
}

pdf_bool_t
pdf_hash_key_p (const pdf_hash_t *table,
                const pdf_char_t *key)
{
  PDF_ASSERT_POINTER_RETURN_VAL (table, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (key, PDF_FALSE);

  return (hash_search_key (table, key, NULL) != NULL ?
          PDF_TRUE : PDF_FALSE);
}

pdf_bool_t
pdf_hash_rename_key (pdf_hash_t        *table,
                     const pdf_char_t  *key,
                     const pdf_char_t  *new_key,
                     pdf_error_t      **error)
{
  pdf_hash_element_t *elt;
  gl_list_node_t node = NULL;
  pdf_error_t *inner_error = NULL;

  PDF_ASSERT_POINTER_RETURN_VAL (table, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (key, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (new_key, PDF_FALSE);

  elt = hash_search_key (table, key, &node);

  /* If element not found, return */
  if (elt == NULL)
    return PDF_FALSE;

  /* Add the same element with the new key */
  if (!pdf_hash_add (table,
                     new_key,
                     elt->value,
                     elt->value_disp_fn,
                     &inner_error))
    {
      pdf_propagate_error (error, inner_error);
      return PDF_FALSE;
    }

  /* Remove the element with the old key */
  gl_list_remove_node ((gl_list_t) table, node);

  return PDF_TRUE;
}

pdf_bool_t
pdf_hash_add (pdf_hash_t                   *table,
              const pdf_char_t             *key,
              const void                   *value,
              pdf_hash_value_dispose_fn_t   value_disp_fn,
              pdf_error_t                 **error)
{
  pdf_hash_element_t *elt;
  pdf_char_t *key_dup;

  PDF_ASSERT_POINTER_RETURN_VAL (table, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (key, PDF_FALSE);

  /* New hash table element */
  elt = pdf_alloc (sizeof (*elt));
  key_dup = strdup (key);
  if (elt == NULL || key_dup == NULL)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_HASH,
                     PDF_ENOMEM,
                     "Not enough memory: Couldn't add new hash table item");
      if (elt)
        pdf_dealloc (elt);
      if (key_dup)
        pdf_dealloc (key_dup);
      return PDF_FALSE;
    }

  /* Set new element contents */
  elt->key = key_dup;
  elt->value = value;
  elt->value_disp_fn = value_disp_fn;

  /* Add to the list */
  if (!gl_sortedlist_nx_add ((gl_list_t) table,
                             (gl_listelement_compar_fn) hash_element_compare,
                             elt))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_HASH,
                     PDF_ENOMEM,
                     "Not enough memory: "
                     "Couldn't add new element to the hash table");
      hash_element_dispose (elt);
      return PDF_FALSE;
    }

  return PDF_TRUE;
}

pdf_bool_t
pdf_hash_remove (pdf_hash_t       *table,
                 const pdf_char_t *key)
{
  gl_list_node_t node = NULL;

  PDF_ASSERT_POINTER_RETURN_VAL (table, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (key, PDF_FALSE);

  if (hash_search_key (table, key, &node))
    {
      /* Remove the element  */
      gl_list_remove_node ((gl_list_t) table, node);
      return PDF_TRUE;
    }

  /* Not found */
  return PDF_FALSE;
}

const void *
pdf_hash_get_value (const pdf_hash_t *table,
                    const pdf_char_t *key)
{
  pdf_hash_element_t *elt;

  PDF_ASSERT_POINTER_RETURN_VAL (table, NULL);
  PDF_ASSERT_POINTER_RETURN_VAL (key, NULL);

  elt = hash_search_key (table, key, NULL);

  return (elt != NULL ? elt->value : NULL);
}

pdf_bool_t
pdf_hash_iterator_init (pdf_hash_iterator_t *itr,
                        const pdf_hash_t    *table)
{
  PDF_ASSERT_POINTER_RETURN_VAL (itr, PDF_FALSE);
  PDF_ASSERT_POINTER_RETURN_VAL (table, PDF_FALSE);

  *((gl_list_iterator_t *)itr) = gl_list_iterator ((gl_list_t) table);

  return PDF_TRUE;
}

pdf_bool_t
pdf_hash_iterator_next (pdf_hash_iterator_t  *itr,
                        const pdf_char_t    **key,
                        const void          **value)
{
  const pdf_hash_element_t *element;
  gl_list_node_t node;

  PDF_ASSERT_POINTER_RETURN_VAL (itr, PDF_FALSE);

  if (!gl_list_iterator_next (((gl_list_iterator_t *) itr),
                              (const void **)&element,
                              &node))
    {
      if (key)
        *key = NULL;
      if (value)
        *value = NULL;
      return PDF_FALSE;
    }

  if (key)
    *key = element->key;
  if (value)
    *value = element->value;
  return PDF_TRUE;
}

void
pdf_hash_iterator_deinit (pdf_hash_iterator_t *itr)
{
  if (itr)
    gl_list_iterator_free ((gl_list_iterator_t *) itr);
}

static pdf_hash_element_t *
hash_search_key (const pdf_hash_t *table,
                 const pdf_char_t *key,
                 gl_list_node_t   *p_node)
{
  gl_list_node_t node;
  pdf_hash_element_t aux;

  /* Setup aux element in stack to use during search comparisons */
  aux.key = key;
  node = gl_sortedlist_search ((gl_list_t) table,
                               (gl_listelement_compar_fn) hash_element_compare,
                               (const void *) &aux);
  if (p_node)
    *p_node = node;

  return (pdf_hash_element_t *) (node != NULL ?
                                 gl_list_node_value ((gl_list_t) table, node) :
                                 NULL);
}

static pdf_size_t
hash_element_pjw (const pdf_hash_element_t *elt)
{
  const pdf_char_t *s;
  pdf_size_t h = 0;

  for (s = elt->key; *s; s++)
    h = *s + ((h << 9) | (h >> (SIZE_BITS - 9)));

  return h;
}

static void
hash_element_dispose (pdf_hash_element_t *elt)
{
  if (elt)
    {
      if (elt->key)
        pdf_dealloc (elt->key);
      if (elt->value_disp_fn)
        elt->value_disp_fn (elt->value);
      pdf_dealloc (elt);
    }
}

static int
hash_element_compare (const pdf_hash_element_t *elt1,
                      const pdf_hash_element_t *elt2)
{
  return strcmp (elt1->key, elt2->key);
}

static pdf_bool_t
hash_element_equal (const pdf_hash_element_t *elt1,
                    const pdf_hash_element_t *elt2)
{
  return (hash_element_compare (elt1, elt2) == 0 ?
          PDF_TRUE : PDF_FALSE);
}

/* End of pdf-hash.c */

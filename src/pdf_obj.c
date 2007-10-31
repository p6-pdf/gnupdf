/* -*- mode: C -*- Time-stamp: "07/10/31 17:37:36 jemarch"
 *
 *       File:         pdf_obj.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Sat Jul  7 03:04:30 2007
 *
 *       GNU PDF Library - PDF objects implementation
 *
 */

/* Copyright (C) 2007 Free Software Foundation, Inc. */

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

#include <string.h>

#ifdef HAVE_MALLOC_H
 #include <malloc.h>
#else
 #include <stdlib.h>
#endif /* HAVE_MALLOC_H */

#include <xalloc.h>
#include <pdf_obj.h>

/* Private functions prototypes */

static pdf_obj_t pdf_alloc_obj (void);
static void pdf_dealloc_obj (pdf_obj_t obj);
static void pdf_dealloc_obj_list_elt (const void *elt);
static bool pdf_compare_obj_list_elt (const void *elt1, const void *elt2);


static pdf_dict_entry_t pdf_alloc_dict_entry (void);
static void pdf_dealloc_dict_entry (pdf_dict_entry_t entry);
static void pdf_dealloc_dict_entry_list_elt (const void *elt);
static bool pdf_compare_dict_entry_list_elt (const void *elt1, const void *elt2);

static int pdf_string_equal_p (pdf_obj_t obj1, pdf_obj_t obj2);
static int pdf_name_equal_p (pdf_obj_t obj1, pdf_obj_t obj2);
static int pdf_array_equal_p (pdf_obj_t obj1, pdf_obj_t obj2);
static int pdf_dict_equal_p (pdf_obj_t obj1, pdf_obj_t obj2);
static int pdf_stream_equal_p (pdf_obj_t obj1, pdf_obj_t obj2);

static pdf_obj_t pdf_array_dup (pdf_obj_t obj);
static pdf_obj_t pdf_dict_dup (pdf_obj_t obj);
static pdf_obj_t pdf_stream_dup (pdf_obj_t obj);

/* Public functions */

pdf_obj_t
pdf_create_null (void)
{
  pdf_obj_t null_obj;

  null_obj = pdf_alloc_obj ();
  null_obj->type = PDF_NULL_OBJ;

  return null_obj;
}

pdf_obj_t
pdf_create_boolean (int value)
{
  pdf_obj_t bool_obj;

  bool_obj = pdf_alloc_obj ();
  bool_obj->type = PDF_BOOLEAN_OBJ;
  bool_obj->value.boolean = value;

  return bool_obj;
}

pdf_obj_t
pdf_create_integer (int value)
{
  pdf_obj_t int_obj;

  int_obj = pdf_alloc_obj ();
  int_obj->type = PDF_INT_OBJ;
  int_obj->value.integer = value;

  return int_obj;
}

pdf_obj_t
pdf_create_real (float value)
{
  pdf_obj_t real_obj;

  real_obj = pdf_alloc_obj ();
  real_obj->type = PDF_REAL_OBJ;
  real_obj->value.real = value;

  return real_obj;
}

pdf_obj_t
pdf_create_string (unsigned char *value,
                   int size)
{
  pdf_obj_t string_obj;

  string_obj = pdf_alloc_obj ();
  string_obj->type = PDF_STRING_OBJ;
  string_obj->value.string.data = 
    (unsigned char *) xmalloc (size);
  memcpy (string_obj->value.string.data,
          value,
          size);
  string_obj->value.string.size = size;

  return string_obj;
}

pdf_obj_t
pdf_create_name (unsigned char *value,
                 int size)
{
  pdf_obj_t name_obj;

  name_obj = pdf_alloc_obj ();
  name_obj->type = PDF_NAME_OBJ;
  name_obj->value.name.data =
    (unsigned char *) xmalloc (size);
  memcpy (name_obj->value.name.data,
          value,
          size);
  name_obj->value.name.size = size;

  return name_obj;
}

pdf_obj_t
pdf_create_array (void)
{
  pdf_obj_t new_array;

  new_array = pdf_alloc_obj ();
  new_array->type = PDF_ARRAY_OBJ;
  new_array->value.array.objs = 
    gl_list_create_empty (GL_ARRAY_LIST,
                          pdf_compare_obj_list_elt,
                          NULL,      /* hashcode_fn */
                          pdf_dealloc_obj_list_elt,
                          PDF_TRUE); /* allow duplicates */

  return new_array;
}

pdf_obj_t
pdf_create_dict (void)
{
  pdf_obj_t new_dict;

  new_dict = pdf_alloc_obj ();
  new_dict->type = PDF_DICT_OBJ;
  new_dict->value.dict.entries = 
    gl_list_create_empty (GL_ARRAY_LIST,
                          pdf_compare_dict_entry_list_elt,
                          NULL,      /* hashcode_fn */
                          pdf_dealloc_dict_entry_list_elt,
                          PDF_FALSE); /* disallow duplicates. */

  return new_dict;
}

pdf_obj_t
pdf_create_indirect (unsigned int on,
                     unsigned int gn)
{
  pdf_obj_t new_indirect;

  new_indirect = pdf_alloc_obj ();
  new_indirect->type = PDF_INDIRECT_OBJ;
  new_indirect->value.indirect.on = on;
  new_indirect->value.indirect.gn = gn;

  return new_indirect;
}

pdf_obj_t
pdf_create_stream (pdf_obj_t dict,
                   pdf_stm_t stm,
                   pdf_stm_pos_t data)
{
  pdf_obj_t new_stream;

  new_stream = pdf_alloc_obj ();
  new_stream->type = PDF_STREAM_OBJ;
  new_stream->value.stream.dict = pdf_obj_dup (dict);
  new_stream->value.stream.stm = stm;
  new_stream->value.stream.data = data;

  return new_stream;
}

inline int
pdf_destroy_obj (pdf_obj_t obj)
{
  pdf_dealloc_obj (obj);
  return PDF_OK;
}

inline int
pdf_get_bool (pdf_obj_t obj)
{
  return obj->value.boolean;
}

inline void
pdf_set_bool (pdf_obj_t obj,
              int value)
{
  obj->value.boolean = value;
}

inline int
pdf_get_int (pdf_obj_t obj)
{
  return obj->value.integer;
}

inline void
pdf_set_int (pdf_obj_t obj, 
             int value)
{
  obj->value.integer = value;
}

inline pdf_real_t
pdf_get_real (pdf_obj_t obj)
{
  return obj->value.real;
}

inline void
pdf_set_real (pdf_obj_t obj,
              float value)
{
  obj->value.real = value;
}

inline int
pdf_get_string_size (pdf_obj_t obj)
{
  return obj->value.string.size;
}

inline char *
pdf_get_string_data (pdf_obj_t obj)
{
  char *data;

  data = (char *) xmalloc (obj->value.string.size);
  memcpy (data,
          obj->value.string.data,
          obj->value.string.size);

  return data;
}

inline int
pdf_get_name_size (pdf_obj_t obj)
{
  return obj->value.name.size;
}

inline char *
pdf_get_name_data (pdf_obj_t obj)
{
  char *data;

  data = (char *) xmalloc (obj->value.name.size);
  memcpy (data,
          obj->value.name.data,
          obj->value.string.size);

  return data;
}

inline int
pdf_get_array_size (pdf_obj_t obj)
{
  return gl_list_size (obj->value.array.objs);
}

inline int
pdf_get_dict_size (pdf_obj_t obj)
{
  return gl_list_size (obj->value.dict.entries);
}

int
pdf_dict_key_p (pdf_obj_t obj,
                pdf_obj_t key)
{
  int entry_p;
  pdf_dict_entry_t entry;

  if ((obj->type != PDF_DICT_OBJ) ||
      (key->type != PDF_NAME_OBJ) ||
      (gl_list_size (obj->value.dict.entries) == 0))
    {
      return PDF_FALSE;
    }
  
  entry = pdf_alloc_dict_entry ();
  entry->key = pdf_obj_dup (key);
  entry->value = pdf_create_null ();

  if (gl_list_search (obj->value.dict.entries,
                      (const void *) entry) != NULL)
    {
      entry_p = PDF_TRUE;
    }
  else
    {
      entry_p = PDF_FALSE;
    }

  pdf_dealloc_dict_entry (entry);
  return entry_p;
}

pdf_obj_t
pdf_get_dict_entry (pdf_obj_t obj,
                    pdf_obj_t key)
{
  pdf_dict_entry_t entry;
  pdf_dict_entry_t result_entry;
  gl_list_node_t list_node;

  if ((obj->type != PDF_DICT_OBJ) ||
      (key->type != PDF_NAME_OBJ) ||
      (gl_list_size (obj->value.dict.entries) == 0))
    {
      return NULL;
    }

  entry = pdf_alloc_dict_entry ();
  entry->key = pdf_obj_dup (key);
  entry->value = pdf_create_null ();
  
  list_node = gl_list_search (obj->value.dict.entries,
                              entry);
  pdf_dealloc_dict_entry (entry);

  if (list_node == NULL)
    {
      return NULL;
    }
  else
    {
      result_entry = (pdf_dict_entry_t) 
        gl_list_node_value (obj->value.dict.entries, list_node);
      
      return result_entry->value;
    }

  /* Not reached */
}

int
pdf_remove_dict_entry (pdf_obj_t obj,
                       pdf_obj_t key)
{
  int status;
  pdf_dict_entry_t entry;

  if (!pdf_dict_key_p (obj, key))
    {
      return PDF_ERROR;
    }

  entry = pdf_alloc_dict_entry ();
  entry->key = pdf_obj_dup (key);
  entry->value = pdf_create_null ();

  if (gl_list_remove (obj->value.dict.entries,
                      entry))
    {
      status = PDF_OK;
    }
  else 
    {
      status = PDF_ERROR;
    }

  pdf_dealloc_dict_entry (entry);

  return status;
}

int
pdf_create_dict_entry (pdf_obj_t obj,
                       pdf_obj_t key,
                       pdf_obj_t value)
{
  pdf_dict_entry_t entry;

  if ((obj->type != PDF_DICT_OBJ) ||
      (key->type != PDF_NAME_OBJ) ||
      (pdf_dict_key_p (obj, key)))
    {
      return PDF_ERROR;
    }

  /* Create a new dictionary entry */
  entry = pdf_alloc_dict_entry ();
  entry->key = key;
  entry->value = value;
  if (gl_list_add_last (obj->value.dict.entries,
                        entry) == NULL)
    {
      pdf_dealloc_dict_entry (entry);
      return PDF_ERROR;
    }
  else
    {
      return PDF_OK;
    }
}

pdf_obj_t 
pdf_get_stream_dict (pdf_obj_t stream)
{
  if (stream->type != PDF_STREAM_OBJ)
    {
      return NULL;
    }

  return pdf_obj_dup(stream->value.stream.dict);
}

pdf_stm_t
pdf_get_stream_stm (pdf_obj_t stream)
{
  if (stream->type != PDF_STREAM_OBJ)
    {
      return NULL;
    }

  return stream->value.stream.stm;
}

pdf_stm_pos_t
pdf_get_stream_data (pdf_obj_t stream)
{
  if (stream->type != PDF_STREAM_OBJ)
    {
      return NO_POS;
    }

  return stream->value.stream.data;
}

int
pdf_obj_equal_p (pdf_obj_t obj1,
                 pdf_obj_t obj2)
{
  int equal_p;

  if (obj1->type != obj2->type)
    {
      return PDF_FALSE;
    }

  switch (obj1->type)
    {
    case PDF_NULL_OBJ:
      {
        equal_p = PDF_TRUE;
        break;
      }
    case PDF_BOOLEAN_OBJ:
      {
        equal_p = (obj1->value.boolean == obj2->value.boolean);
        break;
      }
    case PDF_INT_OBJ:
      {
        equal_p = (obj1->value.integer == obj2->value.integer);
        break;
      }
    case PDF_REAL_OBJ:
      {
        equal_p = (obj1->value.real == obj2->value.real);
        break;
      }
    case PDF_STRING_OBJ:
      {
        equal_p = pdf_string_equal_p (obj1, obj2);
        break;
      }
    case PDF_NAME_OBJ:
      {
        equal_p = pdf_name_equal_p (obj1, obj2);
        break;
      }
    case PDF_ARRAY_OBJ:
      {
        equal_p = pdf_array_equal_p (obj1, obj2);
        break;
      }
    case PDF_DICT_OBJ:
      {
        equal_p = pdf_dict_equal_p (obj1, obj2);
        break;
      }
    case PDF_INDIRECT_OBJ:
      {
        equal_p = 
          (obj1->value.indirect.on == obj2->value.indirect.on) &&
          (obj1->value.indirect.gn == obj2->value.indirect.gn);
        break;
      }
    case PDF_STREAM_OBJ:
      {
        equal_p = pdf_stream_equal_p (obj1, obj2);
        break;
      }
    default:
      {
        /* Should not be reached. Make the compiler happy */
        equal_p = PDF_FALSE;
      }
    }

  return equal_p;
}

pdf_obj_t
pdf_obj_dup (pdf_obj_t obj)
{
  pdf_obj_t new_obj;

  switch (obj->type)
    {
    case PDF_NULL_OBJ:
      {
        new_obj = pdf_create_null();
        break;
      }
    case PDF_BOOLEAN_OBJ:
      {
        new_obj = pdf_create_boolean (GET_BOOL(obj));
        break;
      }
    case PDF_INT_OBJ:
      {
        new_obj = pdf_create_integer (GET_INT(obj));
        break;
      }
    case PDF_REAL_OBJ:
      {
        new_obj = pdf_create_real (GET_REAL(obj));
        break;
      }
    case PDF_STRING_OBJ:
      {
        new_obj = pdf_create_string (obj->value.string.data,
                                     obj->value.string.size);
        break;
      }
    case PDF_NAME_OBJ:
      {
        new_obj = pdf_create_name (obj->value.name.data,
                                   obj->value.name.size);
        break;
      }
    case PDF_ARRAY_OBJ:
      {
        new_obj = pdf_array_dup (obj);
        break;
      }
    case PDF_DICT_OBJ:
      {
        new_obj = pdf_dict_dup (obj);
        break;
      }
    case PDF_INDIRECT_OBJ:
      {
        new_obj = pdf_create_indirect (obj->value.indirect.on,
                                       obj->value.indirect.gn);
        break;
      }
    case PDF_STREAM_OBJ:
      {
        new_obj = pdf_stream_dup (obj);
        break;
      }
    default:
      {
        /* Should not be reached: make the compiler happy */
        new_obj = NULL;
        break;
      }
    }

  return new_obj;
}

int
pdf_remove_array_elt (pdf_obj_t obj, 
                      int index)
{
  if ((obj->type != PDF_ARRAY_OBJ) ||
      (index < 0) ||
      (index >= gl_list_size (obj->value.array.objs)))
    {
      return PDF_ERROR;
    }

  gl_list_remove_at (obj->value.array.objs, index);
  return PDF_OK;
}

pdf_obj_t
pdf_get_array_elt (pdf_obj_t obj,
                   int index)
{
  if ((obj->type != PDF_ARRAY_OBJ) ||
      (index < 0) ||
      (index >= gl_list_size (obj->value.array.objs)))
    {
      return NULL;
    }

  return (pdf_obj_t) gl_list_get_at (obj->value.array.objs, index);
}

int
pdf_set_array_elt (pdf_obj_t obj,
                   int index,
                   pdf_obj_t elt)
{
  if ((obj->type != PDF_ARRAY_OBJ) ||
      (index < 0) ||
      (index >= gl_list_size (obj->value.array.objs)))
    {
      return PDF_ERROR;
    }

  gl_list_set_at (obj->value.array.objs,
                  index,
                  elt);
  return PDF_OK;
}

int 
pdf_add_array_elt (pdf_obj_t obj, 
                   int index, 
                   pdf_obj_t elt)
{
  if ((obj->type != PDF_ARRAY_OBJ) ||
      (index < 0) ||
      (index > gl_list_size (obj->value.array.objs)))
    {
      return PDF_ERROR;
    }

  gl_list_add_at (obj->value.array.objs,
                  index,
                  elt);

  return PDF_OK;
}

inline int
pdf_append_array_elt (pdf_obj_t obj,
                      pdf_obj_t elt)
{
  return pdf_add_array_elt (obj, 
                            pdf_get_array_size (obj),
                            elt);
}

/* Private functions */

static pdf_obj_t
pdf_alloc_obj (void)
{
  pdf_obj_t new_obj;

  new_obj = (pdf_obj_t) xmalloc (sizeof(struct pdf_obj_s));
  return new_obj;
}

static void
pdf_dealloc_obj (pdf_obj_t obj)
{
  switch (obj->type)
    {
    case PDF_STRING_OBJ:
      {
        free (obj->value.string.data);
        break;
      }
    case PDF_NAME_OBJ:
      {
        free (obj->value.name.data);
        break;
      }
    case PDF_ARRAY_OBJ:
      {
        gl_list_free (obj->value.array.objs);
        break;
      }
    case PDF_DICT_OBJ:
      {
        gl_list_free (obj->value.dict.entries);
        break;
      }
    default:
      {
        /* NOP */
        break;
      }
    }

  free (obj);
}

void
pdf_dealloc_obj_list_elt (const void* elt)
{
  pdf_dealloc_obj ((pdf_obj_t) elt);
}

static bool 
pdf_compare_obj_list_elt (const void *elt1,
                          const void *elt2)
{
  return pdf_obj_equal_p ((pdf_obj_t) elt1,
                          (pdf_obj_t) elt2);
}


static pdf_dict_entry_t 
pdf_alloc_dict_entry (void)
{
  pdf_dict_entry_t entry;

  entry = (pdf_dict_entry_t) xmalloc (sizeof(struct pdf_dict_entry_s));
  return entry;
}

static void 
pdf_dealloc_dict_entry (pdf_dict_entry_t entry)
{
  pdf_dealloc_obj (entry->key);
  pdf_dealloc_obj (entry->value);
  free (entry);
}

static void
pdf_dealloc_dict_entry_list_elt (const void *elt)
{
  pdf_dealloc_dict_entry ((pdf_dict_entry_t) elt);
}

static bool
pdf_compare_dict_entry_list_elt (const void *elt1,
                                 const void *elt2)
{
  pdf_dict_entry_t entry1;
  pdf_dict_entry_t entry2;

  entry1 = (pdf_dict_entry_t) elt1;
  entry2 = (pdf_dict_entry_t) elt2;

  return pdf_obj_equal_p (entry1->key, entry2->key);
}

static int
pdf_string_equal_p (pdf_obj_t obj1,
                    pdf_obj_t obj2)
{
  return ((obj1->value.string.size == obj2->value.string.size) &&
          (!memcmp (obj1->value.string.data,
                    obj2->value.string.data,
                    obj1->value.string.size)));
}

static int
pdf_name_equal_p (pdf_obj_t obj1,
                  pdf_obj_t obj2)
{
  return ((obj1->value.name.size == obj2->value.name.size) &&
          (!memcmp (obj1->value.name.data,
                    obj2->value.name.data,
                    obj1->value.name.size)));
}

/* Two PDF arrays are considered equal if the equal-intersection
   between the two sets of objects is empty and if the objects are
   contained in the same order */
static int
pdf_array_equal_p (pdf_obj_t obj1,
                   pdf_obj_t obj2)
{
  int equal_p;
  pdf_obj_t obj_elt1;
  pdf_obj_t obj_elt2;
  gl_list_node_t list_node1;
  gl_list_node_t list_node2;
  gl_list_iterator_t iter1;
  gl_list_iterator_t iter2;

  if ((gl_list_size (obj1->value.array.objs) !=
       gl_list_size (obj2->value.array.objs)))
    {
      return PDF_FALSE;
    }

  if (gl_list_size (obj1->value.array.objs) == 0)
    {
      return PDF_TRUE;
    }

  equal_p = PDF_TRUE;

  iter1 = gl_list_iterator (obj1->value.array.objs);
  iter2 = gl_list_iterator (obj2->value.array.objs);
  
  while (gl_list_iterator_next (&iter1, (const void **) &obj_elt1, &list_node1) &&
         gl_list_iterator_next (&iter2, (const void **) &obj_elt2, &list_node2))
    {
      /* Note the indirect recursion there => avoid loops!!! */
      if (!pdf_obj_equal_p (obj_elt1, obj_elt2))
        {
          equal_p = PDF_FALSE;
          break;
        }
    }
  
  gl_list_iterator_free (&iter1);
  gl_list_iterator_free (&iter2);

  return equal_p;
}

/* Two PDF dictionaries are considered equal if the equal-intersection
   between the two sets of objects is empty. Internal ordering doesnt
   matter. */
static int
pdf_dict_equal_p (pdf_obj_t obj1,
                  pdf_obj_t obj2)
{
  int equal_p;
  gl_list_t int_list;
  gl_list_node_t list_node1;
  gl_list_node_t list_node2;
  pdf_dict_entry_t entry_elt1;
  pdf_dict_entry_t entry_elt2;
  gl_list_iterator_t iter;
  
  if ((gl_list_size (obj1->value.dict.entries) !=
       gl_list_size (obj2->value.dict.entries)))
    {
      return PDF_FALSE;
    }
  
  if (gl_list_size (obj1->value.dict.entries) == 0)
    {
      return PDF_TRUE;
    }
  

  equal_p = PDF_TRUE;

  /* Create the int_list intersection list */
  int_list =
    gl_list_create_empty (GL_ARRAY_LIST,
                          pdf_compare_dict_entry_list_elt,
                          NULL,       /* hashcode_fn */
                          pdf_dealloc_dict_entry_list_elt,
                          PDF_FALSE); /* disallow duplicates */
  iter = gl_list_iterator (obj1->value.dict.entries);
  while (gl_list_iterator_next (&iter, (const void**) &entry_elt1, &list_node1))
    {
      entry_elt2 = (pdf_dict_entry_t) xmalloc (sizeof(struct pdf_dict_entry_s));
      entry_elt2->key = pdf_obj_dup (entry_elt1->key);
      entry_elt2->value = pdf_obj_dup (entry_elt1->value);
     
      gl_list_add_last (int_list, entry_elt2);
    }
  gl_list_iterator_free (&iter);

  /* Calculate the equal-intersection between the dictionaries */
  iter = gl_list_iterator (obj2->value.dict.entries);
  while (gl_list_iterator_next (&iter, (const void**) &entry_elt1, &list_node1))
    {
      list_node2 = gl_list_search (int_list, (const void *) entry_elt1);
      if (list_node2 != NULL)
        {
          gl_list_remove_node (int_list, list_node2);
        } 
    }
  gl_list_iterator_free (&iter);

  /* Is the intersection empty? */
  equal_p = (gl_list_size (int_list) == 0);

  /* Clean the kitchen */
  gl_list_free (int_list);

  /* Bye bye */
  return equal_p;
}

/* Two PDF streams are considered equal if both uses the same stm
   object, its dictionaries are equal and the data pointer points to
   the same position into the stm */
static int
pdf_stream_equal_p (pdf_obj_t obj1,
                    pdf_obj_t obj2)
{
  return ((obj1->value.stream.stm == obj2->value.stream.stm) &&
          (obj1->value.stream.data == obj2->value.stream.data) &&
          pdf_dict_equal_p (obj1->value.stream.dict, obj2->value.stream.dict));
}

static pdf_obj_t
pdf_array_dup (pdf_obj_t obj)
{
  pdf_obj_t new_array;
  pdf_obj_t obj_elt;
  gl_list_iterator_t iter;
  gl_list_node_t list_node;
  

  new_array = pdf_create_array ();

  iter = gl_list_iterator (obj->value.array.objs);
  while (gl_list_iterator_next (&iter, (const void**) &obj_elt, &list_node))
    {
      gl_list_add_last (new_array->value.array.objs, 
                        pdf_obj_dup (obj_elt));
    }
  gl_list_iterator_free (&iter);

  return new_array;
}

static pdf_obj_t
pdf_dict_dup (pdf_obj_t obj)
{
  pdf_obj_t new_dict;
  pdf_dict_entry_t entry_elt;
  pdf_dict_entry_t new_entry_elt;
  gl_list_iterator_t iter;
  gl_list_node_t list_node;
  
  new_dict = pdf_create_dict ();

  iter = gl_list_iterator (obj->value.dict.entries);
  while (gl_list_iterator_next (&iter, (const void**) &entry_elt, &list_node))
    {
      new_entry_elt = pdf_alloc_dict_entry ();
      new_entry_elt->key = pdf_obj_dup (entry_elt->key);
      new_entry_elt->value = pdf_obj_dup (entry_elt->value);

      gl_list_add_last (new_dict->value.dict.entries, 
                        new_entry_elt);
    }
  gl_list_iterator_free (&iter);

  return new_dict;
}

/* Note that the new stream will use the same stm */
static pdf_obj_t
pdf_stream_dup (pdf_obj_t obj)
{
  pdf_obj_t new_stream;

  new_stream = pdf_create_stream (pdf_obj_dup (obj->value.stream.dict),
                                  obj->value.stream.stm,
                                  obj->value.stream.data);

  return new_stream;
}

/* End of pdf_obj.c */

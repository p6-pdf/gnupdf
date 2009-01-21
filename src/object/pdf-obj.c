/* -*- mode: C -*- Time-stamp: "08/09/08 22:44:16 jemarch"
 *
 *       File:         pdf-obj.c
 *       Date:         Sat Jul  7 03:04:30 2007
 *
 *       GNU PDF Library - PDF objects implementation
 *
 */

/* Copyright (C) 2007, 2008 Free Software Foundation, Inc. */

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

#include <string.h>
#include <assert.h>

#include "config.h"
#include "pdf-obj.h"
#include "pdf-alloc.h"

/* Private functions prototypes */

static INLINE pdf_status_t pdf_obj_new (pdf_obj_type_t type, pdf_obj_t *obj);
static void pdf_obj_child_destroy_cb (const void *obj);
static void pdf_obj_dict_dealloc_key_cb (const void *obj);

static bool pdf_obj_child_equal_p_cb (const void *elt1, const void *elt2);
static int pdf_obj_buffer_equal_p (pdf_obj_t obj1, pdf_obj_t obj2);
static pdf_bool_t pdf_obj_array_equal_p (pdf_obj_t obj1, pdf_obj_t obj2);
static int pdf_obj_dict_equal_p (pdf_obj_t obj1, pdf_obj_t obj2);
static int pdf_stream_equal_p (pdf_obj_t obj1, pdf_obj_t obj2);

static INLINE pdf_status_t pdf_obj_array_dup (pdf_obj_t obj, pdf_obj_t *new);
static INLINE pdf_status_t pdf_obj_dict_dup (pdf_obj_t obj, pdf_obj_t *new);
static INLINE pdf_status_t pdf_obj_stream_dup (pdf_obj_t obj, pdf_obj_t *new);

static pdf_status_t pdf_obj_array_ins_priv (pdf_obj_t array, pdf_size_t index,
                                            pdf_obj_t obj, int atend);


/* General functions */

pdf_status_t
pdf_obj_null_new (pdf_obj_t *obj)
{
  return pdf_obj_new(PDF_NULL_OBJ, obj);
}

pdf_status_t
pdf_obj_boolean_new (pdf_bool_t value, pdf_obj_t *obj)
{
  pdf_status_t rv = pdf_obj_new (PDF_BOOLEAN_OBJ, obj);
  if (rv == PDF_OK)
    (*obj)->value.boolean = value;

  return rv;
}

pdf_status_t
pdf_obj_integer_new (int value, pdf_obj_t *obj)
{
  pdf_status_t rv = pdf_obj_new (PDF_INT_OBJ, obj);
  if (rv == PDF_OK)
    (*obj)->value.integer = value;

  return rv;
}

pdf_status_t
pdf_obj_real_new (float value, pdf_obj_t *obj)
{
  pdf_status_t rv = pdf_obj_new (PDF_REAL_OBJ, obj);
  if (rv == PDF_OK)
    (*obj)->value.real = value;

  return rv;
}

pdf_status_t
pdf_obj_indirect_new (unsigned int on,
                      unsigned int gn,
                      pdf_obj_t *obj)
{
  pdf_status_t rv = pdf_obj_new (PDF_INDIRECT_OBJ, obj);
  if (rv == PDF_OK)
    {
      (*obj)->value.indirect.on = on;
      (*obj)->value.indirect.gn = gn;
    }
  return rv;
}

pdf_status_t
pdf_obj_stream_new (pdf_obj_t dict,
                    pdf_stm_t stm,
                    pdf_off_t data,
                    pdf_obj_t *obj)
{
  pdf_status_t rv = pdf_obj_new (PDF_STREAM_OBJ, obj);
  if (rv == PDF_OK)
    {
      (*obj)->value.stream.dict = dict;
      (*obj)->value.stream.stm = stm;
      (*obj)->value.stream.data = data;
    }
  return rv;
}

pdf_status_t
pdf_tok_valueless_new (pdf_obj_type_t type,
                       pdf_obj_t *obj)
{
  return pdf_obj_new(type, obj);
}

pdf_obj_type_t
pdf_obj_type (const pdf_obj_t obj)
{
  return obj->type;
}

pdf_bool_t
pdf_obj_bool_value (const pdf_obj_t obj)
{
  return obj->value.boolean;
}

int
pdf_obj_int_value (const pdf_obj_t obj)
{
  return obj->value.integer;
}

pdf_real_t
pdf_obj_real_value (const pdf_obj_t obj)
{
  return obj->value.real;
}

unsigned int
pdf_obj_indirect_on (const pdf_obj_t obj)
{
  return obj->value.indirect.on;
}

unsigned int
pdf_obj_indirect_gn (const pdf_obj_t obj)
{
  return obj->value.indirect.gn;
}

/*
pdf_obj_t 
pdf_get_stream_dict (pdf_obj_t stream)
{
  if (stream->type != PDF_STREAM_OBJ)
    {
      return NULL;
    }

  return pdf_obj_dup(stream->value.stream.dict);//XXX
}
*/

/*
pdf_stm_t
pdf_get_stream_stm (pdf_obj_t stream)
{
  if (stream->type != PDF_STREAM_OBJ)
    {
      return NULL;
    }

  return stream->value.stream.stm;
}
*/

/*
pdf_off_t
pdf_get_stream_data (pdf_obj_t stream)
{
  if (stream->type != PDF_STREAM_OBJ)
    {
      return NO_POS;
    }

  return stream->value.stream.data;
}
*/

pdf_bool_t
pdf_obj_equal_p (pdf_obj_t obj1,
                 pdf_obj_t obj2)
{
  int equal_p;

  if (obj1 == obj2)
    return PDF_TRUE;
  else if (obj1->type != obj2->type)
    return PDF_FALSE;

  switch (obj1->type)
    {
    case PDF_NULL_OBJ:         /* fall through */
    case PDF_DICT_START_TOK:   /* fall through */
    case PDF_DICT_END_TOK:     /* fall through */
    case PDF_ARRAY_START_TOK:  /* fall through */
    case PDF_ARRAY_END_TOK:    /* fall through */
    case PDF_PROC_START_TOK:   /* fall through */
    case PDF_PROC_END_TOK:
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
    case PDF_STRING_OBJ:  /* fall through */
    case PDF_NAME_OBJ:    /* fall through */
    case PDF_KEYWORD_TOK:
      {
        equal_p = pdf_obj_buffer_equal_p (obj1, obj2);
        break;
      }
    case PDF_COMMENT_TOK:
      {
        equal_p = ((obj1->value.comment.continuation
                      == obj2->value.comment.continuation)
                   && pdf_obj_buffer_equal_p (obj1, obj2));
        break;
      }
    case PDF_ARRAY_OBJ:
      {
        equal_p = pdf_obj_array_equal_p (obj1, obj2);
        break;
      }
    case PDF_DICT_OBJ:
      {
        equal_p = pdf_obj_dict_equal_p (obj1, obj2);
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

pdf_status_t
pdf_obj_dup (const pdf_obj_t obj, pdf_obj_t *new)
{
  switch (obj->type)
    {
    case PDF_NULL_OBJ:         /* fall through */
    case PDF_DICT_START_TOK:   /* fall through */
    case PDF_DICT_END_TOK:     /* fall through */
    case PDF_ARRAY_START_TOK:  /* fall through */
    case PDF_ARRAY_END_TOK:    /* fall through */
    case PDF_PROC_START_TOK:   /* fall through */
    case PDF_PROC_END_TOK:
      return pdf_tok_valueless_new (obj->type, new);

    case PDF_BOOLEAN_OBJ:
      return pdf_obj_boolean_new (GET_BOOL(obj), new);

    case PDF_INT_OBJ:
      return pdf_obj_integer_new (GET_INT(obj), new);

    case PDF_REAL_OBJ:
      return pdf_obj_real_new (GET_REAL(obj), new);

    case PDF_STRING_OBJ:
      return pdf_obj_string_new (obj->value.buffer.data,
                                 obj->value.buffer.size,
                                 new);
    case PDF_NAME_OBJ:
      return pdf_obj_name_new (obj->value.buffer.data,
                               obj->value.buffer.size,
                               new);
    case PDF_KEYWORD_TOK:
      return pdf_tok_keyword_new (obj->value.buffer.data,
                                  obj->value.buffer.size,
                                  new);
    case PDF_COMMENT_TOK:
      return pdf_tok_comment_new (obj->value.comment.data,
                                  obj->value.comment.size,
                                  obj->value.comment.continuation,
                                  new);
    case PDF_ARRAY_OBJ:
      return pdf_obj_array_dup (obj, new);

    case PDF_DICT_OBJ:
      return pdf_obj_dict_dup (obj, new);

    case PDF_INDIRECT_OBJ:
      return pdf_obj_indirect_new (obj->value.indirect.on,
                                   obj->value.indirect.gn,
                                   new);
    case PDF_STREAM_OBJ:
      return pdf_obj_stream_dup (obj, new);

    default:
      /* Should not be reached: make the compiler happy */
      return PDF_EBADDATA;
    }
}

/* Private functions */

static INLINE pdf_status_t
pdf_obj_new (pdf_obj_type_t type, pdf_obj_t *obj)
{
  pdf_obj_t new_obj;

  new_obj = (pdf_obj_t) pdf_alloc (sizeof(struct pdf_obj_s));
  if (!new_obj)
    return PDF_ENOMEM;

  new_obj->type = type;
  *obj = new_obj;
  return PDF_OK;
}

pdf_status_t
pdf_obj_destroy (pdf_obj_t obj)
{
  assert(obj);
  switch (obj->type)
    {
    case PDF_STRING_OBJ:   /* fall through */
    case PDF_NAME_OBJ:     /* fall through */
    case PDF_KEYWORD_TOK:  /* fall through */
    case PDF_COMMENT_TOK:
      {
        pdf_dealloc (obj->value.buffer.data);
        break;
      }
    case PDF_ARRAY_OBJ:
      {
        pdf_list_destroy (obj->value.array);
        break;
      }
    case PDF_DICT_OBJ:
      {
        pdf_hash_destroy (obj->value.dict);
        break;
      }
    default:
      {
        /* NOP */
        break;
      }
    }

  pdf_dealloc (obj);
  return PDF_OK;
}


/* Note that the new stream will use the same stm */
//TODO: avoid storing stm? rename .data to .offset
static INLINE pdf_status_t
pdf_obj_stream_dup (pdf_obj_t obj, pdf_obj_t *new)
{
  pdf_status_t rv;
  pdf_obj_t new_dict = NULL;

  rv = pdf_obj_dup(obj->value.stream.dict, &new_dict);
  if (rv != PDF_OK)
    goto fail;

  rv = pdf_obj_stream_new (new_dict,
                           obj->value.stream.stm,
                           obj->value.stream.data,
                           new);
  if (rv != PDF_OK)
    goto fail;

  return PDF_OK;

fail:
  if (new_dict)
    pdf_obj_destroy(new_dict);
  return rv;
}





/*** objects with buffers *************************************/

static pdf_status_t
pdf_obj_buffer_new (pdf_obj_type_t type,
                    const pdf_char_t *value,
                    pdf_size_t size,
                    pdf_bool_t nullterm,
                    pdf_obj_t *obj)
{
  pdf_obj_t new_obj = NULL;
  pdf_status_t rv = pdf_obj_new(type, &new_obj);
  if (rv != PDF_OK)
    goto fail;

  rv = PDF_ENOMEM;
  new_obj->value.buffer.data = pdf_alloc (nullterm ? size + 1 : size);
  if (!new_obj->value.buffer.data)
    goto fail;

  new_obj->value.buffer.size = size;
  memcpy (new_obj->value.buffer.data, value, size);
  if (nullterm)
    new_obj->value.buffer.data[size] = 0;

  *obj = new_obj;
  return PDF_OK;

fail:
  if (new_obj)
    pdf_dealloc (new_obj);
  return rv;
}

static int
pdf_obj_buffer_equal_p (pdf_obj_t obj1,
                        pdf_obj_t obj2)
{
  struct pdf_obj_buffer_s *buf1 = &obj1->value.buffer;
  struct pdf_obj_buffer_s *buf2 = &obj2->value.buffer;
  return (buf1->size == buf2->size
           && ( buf1->data == buf2->data
                || !memcmp(buf1->data, buf2->data, buf1->size) ));
}


/** names *****/

pdf_status_t
pdf_obj_name_new (const pdf_char_t *value,
                  pdf_size_t size,
                  pdf_obj_t *obj)
{
  return pdf_obj_buffer_new (PDF_NAME_OBJ, value, size, 1, obj);
}

pdf_size_t
pdf_obj_name_size (pdf_obj_t name)
{
  return name->value.buffer.size;
}

const pdf_char_t *
pdf_obj_name_data (pdf_obj_t name)
{
  return name->value.buffer.data;
}


/** strings *****/

pdf_status_t
pdf_obj_string_new (const pdf_char_t *value,
                    pdf_size_t size,
                    pdf_obj_t *obj)
{
  return pdf_obj_buffer_new (PDF_STRING_OBJ, value, size, 0, obj);
}

pdf_size_t
pdf_obj_string_size (pdf_obj_t obj)
{
  return obj->value.buffer.size;
}

const pdf_char_t *
pdf_obj_string_data (pdf_obj_t obj)
{
  return obj->value.buffer.data;
}


/** comments *****/

pdf_status_t
pdf_tok_comment_new (const pdf_char_t *value,
                     pdf_size_t size,
                     pdf_bool_t continuation,
                     pdf_obj_t *obj)
{
  pdf_status_t rv = pdf_obj_buffer_new (PDF_COMMENT_TOK, value, size, 0, obj);
  if (rv == PDF_OK)
    (*obj)->value.comment.continuation = !!continuation;

  return rv;
}

pdf_size_t
pdf_tok_comment_size (pdf_obj_t comment)
{
  return comment->value.buffer.size;
}

const pdf_char_t *
pdf_tok_comment_data (pdf_obj_t comment)
{
  return comment->value.buffer.data;
}


/** keywords *****/

pdf_status_t
pdf_tok_keyword_new (const pdf_char_t *value,
                     pdf_size_t size,
                     pdf_obj_t *obj)
{
  return pdf_obj_buffer_new (PDF_KEYWORD_TOK, value, size, 1, obj);
}

pdf_size_t
pdf_tok_keyword_size (pdf_obj_t keyword)
{
  return keyword->value.buffer.size;
}

const pdf_char_t *
pdf_tok_keyword_data (pdf_obj_t keyword)
{
  return keyword->value.buffer.data;
}


/*** static functions for container types (array/dict) ********/

static pdf_status_t
pdf_obj_child_new (pdf_obj_t key, pdf_obj_t value, pdf_obj_child_t *elt)
{
  pdf_obj_child_t new = pdf_alloc(sizeof(*new));
  if (!new)
    return PDF_ENOMEM;

  new->owned = 1;
  new->key = key;
  new->value = value;
printf("alloc child k=%p v=%p ch=%p own=%d\n", key,value,new,new->owned);

  *elt = new;
  return PDF_OK;
}

static void
pdf_obj_child_destroy_cb (const void *ptr)
{
  pdf_obj_child_t elt = (pdf_obj_child_t)ptr;
  if (elt->owned)
    {
printf("del child k=%p v=%p ch=%p own=%d\n",
         elt->key,elt->value, ptr,elt->owned);
      elt->owned = 0;
      pdf_obj_destroy (elt->value);
      if (elt->key)
        pdf_obj_destroy (elt->key);
    }
  pdf_dealloc(elt);
}

static bool
pdf_obj_child_equal_p_cb (const void *ptr1,
                          const void *ptr2)
{
  pdf_obj_child_t elt1 = (pdf_obj_child_t)ptr1;
  pdf_obj_child_t elt2 = (pdf_obj_child_t)ptr2;
  return pdf_obj_equal_p (elt1->value, elt2->value);
}


/*** arrays ***************************************************/

pdf_status_t
pdf_obj_array_new (pdf_obj_t *array)
{
  pdf_status_t rv;
  pdf_obj_t new_array = NULL;

  rv = pdf_obj_new (PDF_ARRAY_OBJ, &new_array);
  if (rv != PDF_OK)
    goto fail;

  rv = pdf_list_new (pdf_obj_child_equal_p_cb,
                     pdf_obj_child_destroy_cb,
                     PDF_TRUE,  /* allow duplicates */
                     &new_array->value.array);
  if (rv != PDF_OK)
    goto fail;

  *array = new_array;
  return PDF_OK;

fail:
  if (new_array)
    pdf_dealloc(new_array);
  return rv;
}


static pdf_status_t
pdf_obj_array_dup (pdf_obj_t array, pdf_obj_t *new)
{
return PDF_ERROR;//TODO
#if 0
  pdf_obj_t new_array;
  pdf_obj_t obj_elt;
  pdf_list_iterator_t iter;
  pdf_list_node_t list_node;
  

  new_array = pdf_create_array ();

  iter = pdf_list_iterator (obj->value.array);
  while (pdf_list_iterator_next (&iter, (const void**) &obj_elt, &list_node))
    {
      pdf_list_add_last (new_array->value.array, 
                        pdf_obj_dup (obj_elt));
    }
  pdf_list_iterator_free (&iter);

  return new_array;
#endif
}


pdf_size_t
pdf_obj_array_size (pdf_obj_t array)
{
  return pdf_list_size (array->value.array);
}


/* Two PDF arrays are considered equal if the equal-intersection
   between the two sets of objects is empty and if the objects are
   contained in the same order */
static pdf_bool_t
pdf_obj_array_equal_p (pdf_obj_t array1,
                       pdf_obj_t array2)
{
  int equal_p;
  pdf_status_t rv;
  pdf_obj_child_t elt1, elt2;
  pdf_list_node_t node1, node2;
  pdf_list_iterator_t iter1, iter2;

  if (pdf_list_size (array1->value.array)
       != pdf_list_size (array2->value.array))
    {
      return PDF_FALSE;
    }

  if (pdf_list_size (array1->value.array) == 0)
    {
      return PDF_TRUE;
    }

  equal_p = PDF_FALSE;

  /* FIXME: this shouldn't be able to fail */

  rv = pdf_list_iterator (array1->value.array, &iter1);
  if (rv != PDF_OK)
    goto free1;

  rv = pdf_list_iterator (array2->value.array, &iter2);
  if (rv != PDF_OK)
    goto free2;

  equal_p = PDF_TRUE;
  while (pdf_list_iterator_next (&iter1, (const void **) &elt1, &node1)
          && pdf_list_iterator_next (&iter2, (const void **) &elt2, &node2))
    {
      /* Note the indirect recursion there => avoid loops!!! */
      if (!pdf_obj_equal_p (elt1->value, elt2->value))
        {
          equal_p = PDF_FALSE;
          break;
        }
    }

free2:
  pdf_list_iterator_free (&iter2);
free1:
  pdf_list_iterator_free (&iter1);

  return equal_p;
}


pdf_status_t pdf_obj_array_get (const pdf_obj_t array,
                                pdf_size_t index,
                                pdf_obj_t *obj)
{
  pdf_status_t rv;
  pdf_obj_child_t elt;
  if ((array->type != PDF_ARRAY_OBJ)
       || (index >= pdf_list_size (array->value.array)))
    return PDF_EBADDATA;

  rv = pdf_list_get_at (array->value.array, index, (const void**)&elt);
  if (rv == PDF_OK)
    *obj = elt->value;

  return rv;
}


pdf_status_t pdf_obj_array_replace (pdf_obj_t array,
                                    pdf_size_t index,
                                    const pdf_obj_t new_obj,
                                    pdf_obj_t *old_obj)
{
  pdf_status_t rv;
  pdf_obj_child_t elt;
  if (array->type != PDF_ARRAY_OBJ)
    return PDF_EBADDATA;

  rv = pdf_list_get_at (array->value.array, index, (const void**)&elt);
  if (rv != PDF_OK)
    return rv;

printf("replace ch %p\n",elt);
  assert(!elt->owned);
  if (old_obj)
    *old_obj = elt->value;
  else
    {
      elt->owned = 0;
      pdf_obj_destroy(elt->value);
    }

  elt->owned = 1;
  elt->value = new_obj;
  return PDF_OK;
}


pdf_status_t pdf_obj_array_set (pdf_obj_t array,
                                pdf_size_t index,
                                const pdf_obj_t obj)
{
  return pdf_obj_array_replace (array, index, obj, NULL);
}


pdf_status_t
pdf_obj_array_ins_priv (pdf_obj_t array,
                        pdf_size_t index,
                        pdf_obj_t obj,
                        int atend)
{
  pdf_status_t rv;
  pdf_obj_child_t elt = NULL;
  pdf_size_t size;

  if (array->type != PDF_ARRAY_OBJ)
    return PDF_EBADDATA;

  size = pdf_list_size (array->value.array);
  if (atend)
    index = size;

  rv = pdf_obj_child_new (NULL, obj, &elt);
  if (rv != PDF_OK)
    goto fail;

  /* add_at doesn't work for the last element */
  if (index == size)
    rv = pdf_list_add_last (array->value.array, elt, NULL);
  else
    rv = pdf_list_add_at (array->value.array, index, elt, NULL);

  if (rv != PDF_OK)
    goto fail;

  return PDF_OK;

fail:
  if (elt)
    pdf_dealloc(elt);
  return rv;
}

pdf_status_t
pdf_obj_array_insert (pdf_obj_t array,
                      pdf_size_t index,
                      pdf_obj_t obj)
{
  return pdf_obj_array_ins_priv (array, index, obj, 0 /* use index */);
}

pdf_status_t
pdf_obj_array_append (pdf_obj_t array,
                      const pdf_obj_t obj)
{
  return pdf_obj_array_ins_priv (array, 0 /*unused*/, obj, 1 /*at end*/);
}


pdf_status_t
pdf_obj_array_extract (pdf_obj_t array,
                       pdf_size_t index,
                       pdf_obj_t *obj)
{
  if (array->type != PDF_ARRAY_OBJ)
    return PDF_EBADDATA;

  if (obj)
    {
      pdf_status_t rv;
      pdf_obj_child_t elt;

      rv = pdf_list_get_at (array->value.array, index, (const void**)&elt);
      if (rv != PDF_OK)
        return rv;

      *obj = elt->value;
printf("extract ch %p\n",elt);
      assert(elt->owned);
      elt->owned = 0;
    }

  return pdf_list_remove_at (array->value.array, index);
}

pdf_status_t
pdf_obj_array_remove (pdf_obj_t array,
                      pdf_size_t index)
{
  return pdf_obj_array_extract (array, index, NULL);
}

pdf_status_t
pdf_obj_array_pop_end (pdf_obj_t array,
                       pdf_obj_t *obj)
{
  return pdf_obj_array_extract (array, pdf_obj_array_size(array)-1, obj);
}


pdf_status_t
pdf_obj_array_clear (pdf_obj_t array)
{
  /* Clear the array, but don't free its objects (presumably because
   * they're still owned by someone else). */

printf("arr clear\n");
  while (pdf_obj_array_size(array))
    {
      if (pdf_obj_array_pop_end(array, NULL) != PDF_OK)
        return PDF_ERROR;  /* shouldn't happen */
    }
  return PDF_OK;
}

pdf_status_t
pdf_obj_array_clear_nodestroy (pdf_obj_t array)
{
  /* Clear the array, but don't free its objects (presumably because
   * they're still owned by someone else). */

  while (pdf_obj_array_size(array))
    {
      pdf_obj_t dummy;
      if (pdf_obj_array_pop_end(array, &dummy) != PDF_OK)
        return PDF_ERROR;  /* shouldn't happen */
    }
  return PDF_OK;
}


/*** dictionaries *********************************************/

pdf_status_t
pdf_obj_dict_new (pdf_obj_t *obj)
{
  pdf_status_t rv;
  pdf_obj_t new_dict = NULL;

  rv = pdf_obj_new (PDF_DICT_OBJ, &new_dict);
  if (rv != PDF_OK)
    goto fail;

  rv = pdf_hash_new (pdf_obj_dict_dealloc_key_cb,
                     &new_dict->value.dict);
  if (rv != PDF_OK)
    goto fail;

  *obj = new_dict;
  return PDF_OK;

fail:
  if (new_dict)
    pdf_dealloc(new_dict);
  return rv;
}

static void
pdf_obj_dict_dealloc_key_cb (const void *obj)
{
  /* key is deallocated with its value in pdf_obj_child_destroy_cb */
}

/* Two PDF dictionaries are considered equal if the equal-intersection
   between the two sets of objects is empty. Internal ordering doesnt
   matter. */
static int
pdf_obj_dict_equal_p (pdf_obj_t obj1,
                      pdf_obj_t obj2)
{
return 0;
#if 0
  int equal_p;
  pdf_list_t int_list;
  pdf_list_node_t list_node1;
  pdf_list_node_t list_node2;
  pdf_dict_entry_t entry_elt1;
  pdf_dict_entry_t entry_elt2;
  pdf_list_iterator_t iter;
  
  if ((pdf_list_size (obj1->value.dict.entries) !=
       pdf_list_size (obj2->value.dict.entries)))
    {
      return PDF_FALSE;
    }
  
  if (pdf_list_size (obj1->value.dict.entries) == 0)
    {
      return PDF_TRUE;
    }
  

  equal_p = PDF_TRUE;

  /* Create the int_list intersection list */
  int_list =
    pdf_list_new (pdf_compare_dict_entry_list_elt,
                  pdf_dealloc_dict_entry_list_elt,
                  PDF_FALSE); /* disallow duplicates */
  iter = pdf_list_iterator (obj1->value.dict.entries);
  while (pdf_list_iterator_next (&iter, (const void**) &entry_elt1, &list_node1))
    {
      entry_elt2 = (pdf_dict_entry_t) xmalloc (sizeof(struct pdf_dict_entry_s));
      entry_elt2->key = pdf_obj_dup (entry_elt1->key);
      entry_elt2->value = pdf_obj_dup (entry_elt1->value);
     
      pdf_list_add_last (int_list, entry_elt2);
    }
  pdf_list_iterator_free (&iter);

  /* Calculate the equal-intersection between the dictionaries */
  iter = pdf_list_iterator (obj2->value.dict.entries);
  while (pdf_list_iterator_next (&iter, (const void**) &entry_elt1, &list_node1))
    {
      list_node2 = pdf_list_search (int_list, (const void *) entry_elt1);
      if (list_node2 != NULL)
        {
          pdf_list_remove_node (int_list, list_node2);
        } 
    }
  pdf_list_iterator_free (&iter);

  /* Is the intersection empty? */
  equal_p = (pdf_list_size (int_list) == 0);

  /* Clean the kitchen */
  pdf_list_destroy (int_list);

  /* Bye bye */
  return equal_p;
#endif
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
          pdf_obj_dict_equal_p (obj1->value.stream.dict, obj2->value.stream.dict));
}


static pdf_status_t
pdf_obj_dict_dup (pdf_obj_t obj, pdf_obj_t *new)
{
return PDF_ERROR;//TODO
#if 0
  pdf_obj_t new_dict;
  pdf_dict_entry_t entry_elt;
  pdf_dict_entry_t new_entry_elt;
  pdf_list_iterator_t iter;
  pdf_list_node_t list_node;
  
  new_dict = pdf_create_dict ();

  iter = pdf_list_iterator (obj->value.dict.entries);
  while (pdf_list_iterator_next (&iter, (const void**) &entry_elt, &list_node))
    {
      new_entry_elt = pdf_alloc_dict_entry ();
      new_entry_elt->key = pdf_obj_dup (entry_elt->key);
      new_entry_elt->value = pdf_obj_dup (entry_elt->value);

      pdf_list_add_last (new_dict->value.dict.entries, 
                        new_entry_elt);
    }
  pdf_list_iterator_free (&iter);

  return new_dict;
#endif
}

pdf_size_t
pdf_obj_dict_size (const pdf_obj_t dict)
{
  return pdf_hash_size (dict->value.dict);
}

pdf_status_t
pdf_obj_dict_get (const pdf_obj_t dict,
              const pdf_obj_t key,
              pdf_obj_t *value)
{
  if ((dict->type != PDF_DICT_OBJ)
      || (key->type != PDF_NAME_OBJ))
    return PDF_EBADDATA;

  return pdf_hash_get(dict->value.dict,
                      (char*)key->value.buffer.data,
                      (void*)value);
}

pdf_bool_t
pdf_obj_dict_key_p (const pdf_obj_t dict,
                    const pdf_obj_t key)
{
  return ((dict->type == PDF_DICT_OBJ)
           && (key->type == PDF_NAME_OBJ)
           && pdf_hash_key_p(dict->value.dict,
                             (char*)key->value.buffer.data));
}

pdf_status_t
pdf_obj_dict_remove (pdf_obj_t dict,
                 pdf_obj_t key)
{
  if ((dict->type != PDF_DICT_OBJ)
      || (key->type != PDF_NAME_OBJ))
    return PDF_EBADDATA;
  else if (!pdf_obj_dict_key_p (dict, key))
    return PDF_ENONODE;

  return pdf_hash_remove(dict->value.dict,
                         (char*)key->value.buffer.data);
}

pdf_status_t
pdf_obj_dict_add (pdf_obj_t dict,
                  pdf_obj_t key,
                  pdf_obj_t value)
{
  pdf_status_t rv;
  pdf_obj_child_t elt = NULL;
  char *keystr = NULL;
  if ((dict->type != PDF_DICT_OBJ)
      || (key->type != PDF_NAME_OBJ))
    return PDF_EBADDATA;
  else if (pdf_obj_dict_key_p (dict, key))
    return PDF_EEXIST;

  rv = pdf_obj_child_new (key, value, &elt);
  if (rv != PDF_OK)
    goto fail;

  keystr = (char*)key->value.buffer.data;
  rv = pdf_hash_add(dict->value.dict, keystr, elt, pdf_obj_child_destroy_cb);
  if (rv != PDF_OK)
    goto fail;

  return PDF_OK;

fail:
  if (elt)
    pdf_dealloc(elt);
  return rv;
}

pdf_status_t
pdf_obj_dict_clear_nodestroy (pdf_obj_t dict)
{
  /* Clear the dict, but don't free its objects (presumably because
   * they're still owned by someone else). */

//  pdf_obj_t dummy;
  //TODO
  return PDF_ERROR;//XXX
}

/* End of pdf_obj.c */

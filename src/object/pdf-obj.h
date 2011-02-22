/* -*- mode: C -*-
 *
 *       File:         pdf-obj.h
 *       Date:         Sat Jul  7 01:10:11 2007
 *
 *       GNU PDF Library - PDF objects
 *
 */

/* Copyright (C) 2007, 2008, 2011 Free Software Foundation, Inc. */

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

#ifndef PDF_OBJ_H
#define PDF_OBJ_H

#include <config.h>
#include <pdf-base.h>
#include <pdf-obj-doc.h>

/* BEGIN PUBLIC */

/* Note that pdf_obj_t is an opaque type, even if we are including the
   definition of struct pdf_obj_s here for stack allocation
   purposes. */

struct pdf_obj_s
{
  pdf_u32_t f;
  pdf_i32_t v;
  void     *p;
};

typedef struct pdf_obj_s pdf_obj_t;

/* The PDF NULL object has a type and a value that are unequal to
   those of any other object. There is only one possible value for
   this object type: PDF_OBJ_NULL. */

const pdf_obj_t PDF_OBJ_NULL = {0, 0, NULL};

/* A pdf_obj_t variable can be of any of the following types.  */

enum pdf_obj_type_e
{
  PDF_OBJ_REAL,
  PDF_OBJ_INTEGER,
  PDF_OBJ_BOOLEAN,
  PDF_OBJ_NAME,
  PDF_OBJ_STRING,
  PDF_OBJ_ARRAY,
  PDF_OBJ_DICT,
  PDF_OBJ_STREAM,
  PDF_OBJ_NULL
};

/* The ID of an object is a positive integer (> 0).  It uniquely
   represents an object into an object document.  */

typedef pdf_u32_t pdf_obj_id_t;

/* The generation number of an object is a non-negative integer.  */

typedef pdf_u32_t pdf_obj_gen_t;


/* ---------- Generic functions to manipulate objects ----------- */

pdf_obj_t      pdf_obj_dup          (pdf_obj_t      obj,
                                     pdf_obj_doc_t *dest_doc,
                                     pdf_bool_t     copy_indirect);

pdf_bool_t     pdf_obj_equal_p      (pdf_obj_t obj1,
                                     pdf_obj_t obj2)

enum pdf_obj_type_e pdf_obj_get_type (pdf_obj_t obj);;

pdf_obj_t      pdf_obj_destroy      (pdf_obj_t obj);
pdf_list_t    *pdf_obj_get_objs     (pdf-obj_t obj);
pdf_obj_doc_t *pdf_obj_get_doc      (pdf_obj_t obj);
pdf_obj_gen_t  pdf_obj_get_gen      (pdf_obj_t obj);
pdf_obj_id_t   pdf_obj_get_id       (pdf_obj_t obj);
pdf_bool_t     pdf_obj_compressed_p (pdf_obj_t obj);
pdf_bool_t     pdf_obj_indirect_p   (pdf_obj_t obj);
pdf_size_t     pdf_obj_size         (pdf_obj_t obj);

/* --------------------- strong references ---------------------- */

void pdf_obj_acquire (pdf_obj_t obj);
void pdf_obj_release (pdf_obj_t obj);

/* --------------------- compressed objects --------------------- */

pdf_bool_t pdf_obj_get_compressibility (pdf_obj_t  obj);
pdf_bool_t pdf_obj_set_compressibility (pdf_obj_t  obj,
                                        pdf_bool_t compressible);

/* --------------------- real objects --------------------------- */

pdf_obj_t  pdf_obj_real_new   (pdf_obj_doc_t *doc,
                               pdf_bool_t     indirect,
                               pdf_real_t     value);

pdf_real_t pdf_obj_real_value (pdf_obj_t real);

/* --------------------- integer objects ------------------------ */

pdf_obj_t pdf_obj_integer_new (pdf_obj_doc_t *doc,
                               pdf_bool_t     indirect,
                               pdf_i32_t      value);

pdf_i32_t pdf_obj_integer_value (pdf_obj_t integer);

/* --------------------- boolean objects ------------------------ */

pdf_obj_t pdf_obj_boolean_new (pdf_obj_doc_t *doc,
                               pdf_bool_t     indirect,
                               pdf_bool_t     value);

pdf_bool_t pdf_obj_boolean_value (pdf_obj_t boolean);

/* --------------------- name objects --------------------------- */

pdf_obj_t pdf_obj_name_new (pdf_obj_doc_t *doc,
                            pdf_bool_t     indirect,
                            pdf_char_t    *value);

const pdf_char_t *pdf_obj_name (pdf_obj_t name);

/* --------------------- string objects ------------------------- */

pdf_obj_t pdf_obj_string_new (pdf_obj_doc_t *doc,
                              pdf_bool_t     indirect,
                              pdf_char_t    *str,
                              pdf_size_t     size);

const pdf_chart *pdf_obj_string (pdf_obj_t   str,
                                 pdf_size_t *size);

pdf_bool_t pdf_obj_string_hex_p   (pdf_obj str);
pdf_bool_t pdf_obj_string_hex_set (pdf_obj_t  str,
                                   pdf_bool_t hex);


/* --------------------- array objects -------------------------- */

pdf_obj_t    pdf_obj_array_new  (pdf_obj_doc_t *doc,
                                 pdf_bool_t     indirect,
                                 pdf_size_t     size);

pdf_obj_t    pdf_obj_array_get  (pdf_obj_t array,
                                 pdf_size_t index);
pdf_status_t pdf_obj_array_set  (pdf_obj_t  array,
                                 pdf_size_t index,
                                 pdf_obj_t  obj);

pdf_bool_t   pdf_obj_array_weak_p   (pdf_obj_t  array,
                                     pdf_size_t index);
pdf_bool_t   pdf_obj_array_weak_set (pdf_obj_t  array,
                                     pdf_size_t index,
                                     pdf_bool_t weak);

pdf_status_t pdf_obj_array_remove   (pdf_obj_t array,
                                     pdf_obj_t obj);
pdf_status_t pdf_obj_array_remove_t (pdf_obj_t  array,
                                     pdf_size_t index);

/* --------------------- dictionary objects --------------------- */

pdf_obj_t pdf_obj_dict_new (pdf_obj_doc_t *doc,
                            pdf_bool_t     indirect);


pdf_obj_t    pdf_obj_dict_get     (pdf_obj_t dict, pdf_obj_t key);
pdf_obj_t    pdf_obj_dict_get_str (pdf_obj_t dict,
                                   const pdf_char_t *key);

pdf_status_t pdf_obj_dict_set     (pdf_obj_t dict,
                                   pdf_obj_t key,
                                   pdf_obj_t value);
pdf_status_t pdf_obj_dict_set_str (pdf_obj_t dict,
                                   const pdf_char_t *key,
                                   pdf_obj_t  value);

pdf_status_t pdf_obj_dict_remove     (pdf_obj_t dict, pdf_obj_t key);
pdf_status_t pdf_obj_dict_remove_str (pdf_obj_t dict,
                                      const pdf_char_t *key);

pdf_bool_t   pdf_obj_dict_key_p     (pdf_obj_t dict, pdf_obj_t key);
pdf_bool_t   pdf_obj_dict_key_str_p (pdf_obj_t dict,
                                     const pdf_char_t *key);

pdf_bool_t   pdf_obj_dict_weak_p     (pdf_obj_t dict, pdf_obj_t key);
pdf_bool_t   pdf_obj_dict_weak_str_p (pdf_obj_t dict,
                                      const pdf_char_t *key);

pdf_staus_t  pdf_obj_dict_weak_set     (pdf_obj_t  dict,
                                        pdf_obj_t  key,
                                        pdf_bool_t weak);
pdf_status_t pdf_obj_dict_weak_set_str (pdf_obj_t         dict,
                                        const pdf_char_t *key,
                                        pdf_bool_t        weak);

/* --------------------- stream objects ------------------------- */

pdf_obj_t pdf_obj_stream_new (pdf_obj_doc_t *doc,
                              pdf_stm_t      stm,
                              pdf_off_t      size,
                              pdf_obj_t      attrs_dict);

pdf_obj_t pdf_obj_stream_dict (pdf_obj_t stream);
pdf_off_t pdf_obj_stream_pos  (pdf_obj_t stream);
pdf_obj_t pdf_obj_stream_open (pdf_obj_t stream,
                               enum pdf_obj_stm_open_mode_e mode);


/* END PUBLIC */

#endif /* pdf_obj.h */

/* End of pdf_obj.h */

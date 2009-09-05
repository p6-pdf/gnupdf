/* -*- mode: C -*- Time-stamp: "09/09/05 15:54:16 jemarch"
 *
 *       File:         pdf-hash-helper.c
 *       Date:         Thu Jul 24 21:05:05 2008
 *
 *       GNU PDF Library - Hash helper functions
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

#include <pdf-hash-helper.h>


static void text_dispose_fn (const void *elt);
static void time_dispose_fn (const void *elt);
static void list_dispose_fn (const void *elt);
static void hash_dispose_fn (const void *elt);
static void stm_dispose_fn (const void *elt);


pdf_status_t
pdf_hash_add_text (pdf_hash_t table, const char *key, const pdf_text_t *elt)
{
  return (pdf_hash_add(table,key,elt,text_dispose_fn));
}

pdf_status_t
pdf_hash_get_text (pdf_hash_t table, const char *key, pdf_text_t *elt)
{
  return pdf_hash_get (table, key, (const void **) &elt);
}

pdf_status_t
pdf_hash_add_time (pdf_hash_t table, const char *key, const pdf_time_t *elt)
{
  return (pdf_hash_add(table,key,elt,time_dispose_fn));
}

pdf_status_t
pdf_hash_get_time (pdf_hash_t table, const char *key, pdf_time_t *elt)
{
  return pdf_hash_get (table, key, (const void **) &elt);
}


pdf_status_t
pdf_hash_add_list (pdf_hash_t table, const char *key, const pdf_list_t *elt)
{
  return (pdf_hash_add(table,key,elt,list_dispose_fn));
}

pdf_status_t
pdf_hash_get_list (pdf_hash_t table, const char *key, pdf_list_t *elt)
{
  return pdf_hash_get (table, key, (const void **) &elt);
}

pdf_status_t
pdf_hash_add_hash (pdf_hash_t table, const char *key, const pdf_hash_t *elt)
{
  return (pdf_hash_add(table,key,elt,hash_dispose_fn));
}

pdf_status_t
pdf_hash_get_hash (pdf_hash_t table, const char *key, pdf_hash_t *elt)
{
  return pdf_hash_get (table, key, (const void **) &elt);
}


pdf_status_t
pdf_hash_add_stm (pdf_hash_t table, const char *key, const pdf_stm_t *elt)
{
  return (pdf_hash_add(table,key,elt,stm_dispose_fn));
}

pdf_status_t
pdf_hash_get_stm (pdf_hash_t table, const char *key, pdf_stm_t *elt)
{
  return pdf_hash_get (table, key, (const void **) &elt);
}

pdf_status_t
pdf_hash_add_bool (pdf_hash_t table, const char *key, const pdf_bool_t elt)
{
  pdf_bool_t *vbool;

  vbool = pdf_alloc (sizeof(pdf_bool_t));
  if (vbool == NULL)
    {
      return PDF_ERROR;
    }

  *vbool = elt;

  return (pdf_hash_add (table, key, vbool, pdf_dealloc));
}

pdf_status_t
pdf_hash_get_bool (pdf_hash_t table, const char *key, pdf_bool_t *elt)
{
  pdf_status_t ret;
  pdf_bool_t *vbool;

  ret = pdf_hash_get (table, key, (const void **) &vbool);
  if (ret == PDF_OK)
    {
      *elt = *vbool;
    }

  return ret;
}

pdf_status_t
pdf_hash_add_size (pdf_hash_t table, const char *key, const pdf_size_t elt)
{
  pdf_size_t *size;

  size = pdf_alloc (sizeof(pdf_size_t));
  if (size == NULL)
    {
      return PDF_ERROR;
    }

  *size = elt;

  return (pdf_hash_add (table, key, size, pdf_dealloc));
}

pdf_status_t
pdf_hash_get_size (pdf_hash_t table, const char *key, pdf_size_t *elt)
{
  pdf_status_t ret;
  pdf_size_t *size;
  ret = pdf_hash_get (table, key, (const void **) &size);
  if (ret == PDF_OK)
    {
      *elt = *size;
    }

  return ret;
}

pdf_status_t
pdf_hash_add_string (pdf_hash_t table, const char *key, const pdf_char_t *elt)
{
  pdf_char_t *string;
  pdf_size_t string_size;

  string_size = strlen ((char *) elt) + 1;
  string = pdf_alloc (string_size);
  if (string == NULL)
    {
      return PDF_ERROR;
    }

  string = (pdf_char_t *) strncpy ((char *) string, (char *) elt, string_size);

  return (pdf_hash_add (table, key, string, pdf_dealloc));
}

pdf_status_t
pdf_hash_get_string (pdf_hash_t table, const char *key, pdf_char_t **elt)
{
  return pdf_hash_get (table, key, (const void **) elt);
}

static void
text_dispose_fn (const void *elt)
{
  pdf_text_t *destroyed = (pdf_text_t*) elt;
  pdf_text_destroy(*destroyed);
}

static void
time_dispose_fn (const void *elt)
{
  pdf_time_t * destroyed = (pdf_time_t*) elt;
  pdf_time_destroy(*destroyed);
}

static void
list_dispose_fn (const void *elt)
{
  pdf_list_t * destroyed = (pdf_list_t*) elt;
  pdf_list_destroy(*destroyed);
}

static void
hash_dispose_fn (const void *elt)
{
  pdf_hash_destroy (*((pdf_hash_t*)elt));
}

static void
stm_dispose_fn (const void *elt)
{
  pdf_size_t flushed_bytes;

  pdf_stm_t * stm = (pdf_stm_t*) elt;
  pdf_stm_flush (*stm, PDF_TRUE, &flushed_bytes);
  pdf_stm_destroy (*stm);
}


/* End of pdf-hash-helper.c */

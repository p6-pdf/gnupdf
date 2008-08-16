/* -*- mode: C -*- Time-stamp: "08/08/16 13:27:26 jemarch"
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
pdf_hash_add_time (pdf_hash_t table, const char *key, const pdf_time_t *elt)
{
  return (pdf_hash_add(table,key,elt,time_dispose_fn));
}


pdf_status_t
pdf_hash_add_list (pdf_hash_t table, const char *key, const pdf_list_t *elt)
{
  return (pdf_hash_add(table,key,elt,list_dispose_fn));
}


pdf_status_t
pdf_hash_add_hash (pdf_hash_t table, const char *key, const pdf_hash_t *elt)
{
  return (pdf_hash_add(table,key,elt,hash_dispose_fn));
}


pdf_status_t
pdf_hash_add_stm (pdf_hash_t table, const char *key, const pdf_stm_t *elt)
{
  return (pdf_hash_add(table,key,elt,stm_dispose_fn));
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
  pdf_hash_destroy ((pdf_hash_t*)elt);
}

static void
stm_dispose_fn (const void *elt)
{
  pdf_stm_t * stm = (pdf_stm_t*) elt;
  pdf_stm_flush (*stm);
  pdf_stm_destroy (*stm);
}


/* End of pdf-hash-helper.c */

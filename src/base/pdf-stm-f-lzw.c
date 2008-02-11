/* -*- mode: C -*- Time-stamp: "08/02/11 01:04:16 jemarch"
 *
 *       File:         pdf-stm-f-lzw.c
 *       Date:         Wed Aug 15 14:41:18 2007
 *
 *       GNU PDF Library - LWZ encoder/decoder filter
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

#include <config.h>

#ifdef HAVE_MALLOC_H
 #include <malloc.h>
#else
 #include <stdlib.h>
#endif /* HAVE_MALLOC_H */

#include <xalloc.h>
#include <string.h>
#include <pdf-stm-f-lzw.h>

#define MIN_BITSIZE 9
#define MAX_BITSIZE 12
#define MAX_DICTSIZE (1 << MAX_BITSIZE)

#define MAX_COMPRESSION_FACTOR 1.5

#define NULL_INDEX ~0U

enum {
  LZW_RESET_CODE = 256,
  LZW_EOD_CODE,
  LZW_FIRST_CODE
} lzw_special_codes;



/* -- LZW code output/input -- */

/*
 * Object to read and write codes of variable bitsize in a buffer.
 * Warning: using both get and put functions may break the buffer.
 */
typedef struct lzw_buffer_s
{
  pdf_char_t* curp;
  pdf_char_t* endp;
  unsigned long valbuf;
  unsigned valbits;
  unsigned bitsize;
  unsigned maxval;
} lzw_buffer_t;

static void
lzw_buffer_init (lzw_buffer_t* b, 
		 pdf_char_t* ptr,
		 int size,
		 int bitsize)
{
  b->curp = ptr;
  b->endp = ptr + size;
  b->valbuf = 0;
  b->valbits = 0;
  b->bitsize = bitsize;
  b->maxval = (1 << bitsize) - 1;
}

static unsigned int
lzw_buffer_get_code (lzw_buffer_t* b)
{
  unsigned long r;
  
  while (b->valbits <= 24)
  {
    if (b->curp > b->endp)
      return NULL_INDEX;
    
    b->valbuf |= (unsigned long) *b->curp++ << (24 - b->valbits);
    b->valbits += 8;
  }

  r = b->valbuf >> (32 - b->bitsize);
  b->valbuf <<= b->bitsize;
  b->valbits -= b->bitsize;
  
  return r;
}

/* Once finished, call with 0 as code value to flush the buffer. */
static void
lzw_buffer_put_code (lzw_buffer_t* b,
		     unsigned int code)
{
  b->valbuf |= (unsigned long) code << (32 - b->bitsize - b->valbits);
  b->valbits += b->bitsize;

  while (b->valbits >= 8)
    {
      *b->curp++ = b->valbuf >> 24;
      b->valbuf <<= 8;
      b->valbits -= 8;
    }
}

static int
lzw_buffer_inc_bitsize (lzw_buffer_t* b)
{
  if (b->bitsize == MAX_BITSIZE)
    return PDF_ERROR;
  
  ++b->bitsize;
  b->maxval = (1 << b->bitsize) - 1;

  return PDF_OK;
}

static void
lzw_buffer_set_bitsize (lzw_buffer_t* b,
			int newsize)
{
  b->bitsize = newsize;
  b->maxval = (1 << newsize) - 1;
}


/* -- LZW dictionary handler -- */

/*
 * The strings are stored in a non balanced ordered binary tree.
 */
typedef struct lzw_string_s
{
  unsigned prefix;   /* Prefix string code */
  pdf_char_t suffix; /* Appended character */

  unsigned first; /* First string with the same prefix.  */
  unsigned left;  /* Next string with smaller suffix and same prefix. */
  unsigned right; /* Next string with greater suffix and same prefix. */
} lzw_string_t;

static void
lzw_string_init (lzw_string_t* s)
{
  memset(s, 0xFF, sizeof(lzw_string_t));
}


typedef struct lzw_dict_s
{
  lzw_string_t table[MAX_DICTSIZE];
  unsigned size;
} lzw_dict_t;

static void
lzw_dict_init (lzw_dict_t* d)
{
  int i;
  
  memset(d->table, 0xFF, sizeof(lzw_string_t) * MAX_DICTSIZE);

  for (i = 0; i < LZW_FIRST_CODE; i++)
    {
      d->table[i].suffix = i;
    }

  d->size = LZW_FIRST_CODE;
}

static int
lzw_dict_add (lzw_dict_t* d,
	      lzw_string_t* s)
{
  unsigned index;
  int must_add;
  
  if (s->prefix == NULL_INDEX)
    {
      s->prefix = s->suffix;
      return PDF_FALSE; /* The string is a basic character, found! */
    }
  
  index = d->table[s->prefix].first;

  if (index == NULL_INDEX)
    {
      d->table[s->prefix].first = d->size;
    }
  else
    {
      must_add = PDF_FALSE;
      while (!must_add)
	{
	  if (s->suffix == d->table[index].suffix)
	    {
	      s->prefix = index;
	      return PDF_FALSE; /* The string is already in the table, found! */
	    }
	  else if (s->suffix < d->table[index].suffix)
	    {
	      if (d->table[index].left == NULL_INDEX)
		{
		  d->table[index].left = d->size;
		  must_add = PDF_TRUE;
		}
	      else
		{
		  index = d->table[index].left;
		}
	    }
	  else
	    {
	      if (d->table[index].right == NULL_INDEX)
		{
		  d->table[index].right = d->size;
		  must_add = PDF_TRUE;
		}
	      else
		{
		  index = d->table[index].right;
		}
	    }
	}
    }

  d->table[d->size++] = *s;

  return PDF_TRUE;
}

#define lzw_dict_reset lzw_dict_init

static void
lzw_dict_fast_add (lzw_dict_t* d,
		   unsigned prefix,
		   pdf_char_t suffix)
{
  d->table[d->size].prefix = prefix;
  d->table[d->size].suffix = suffix;
  d->size++;
}

static void
lzw_dict_decode (lzw_dict_t* d,
		 unsigned code,
		 pdf_char_t** decode,
		 unsigned* size)
{
  *size = 0;

  do {
    *(*decode)-- = d->table[code].suffix;
    ++(*size);
    code = d->table[code].prefix;
  } while (code != NULL_INDEX);
  (*decode)++;
  
}

/* -- The encoder -- */

static int
pdf_stm_f_lzw_encode (pdf_stm_f_lzw_data_t data,
                      pdf_char_t *in,
                      pdf_stm_pos_t in_size,
                      pdf_char_t **out,
                      pdf_stm_pos_t *out_size)
{
  lzw_buffer_t buffer;
  lzw_dict_t dict;
  lzw_string_t string;
  
  /* Allocate buffer with enough space. */
  *out_size = in_size * MAX_COMPRESSION_FACTOR;
  if ((*out = (pdf_char_t *) xmalloc (*out_size)) == NULL)
    {
      *out_size = 0;
      return PDF_ERROR;
    }

  /* Do the actual encoding. */
  lzw_buffer_init(&buffer, *out, *out_size, MIN_BITSIZE);
  lzw_dict_init(&dict);
  lzw_string_init(&string);

  lzw_buffer_put_code(&buffer, LZW_RESET_CODE);

  while (--in_size >= 0)
    {
      string.suffix = *in++;

      if (lzw_dict_add(&dict, &string))
	{
	  lzw_buffer_put_code(&buffer, string.prefix);
	  string.prefix = string.suffix;

	  if (buffer.maxval - data->early_change == dict.size)
	    {
	      if (!lzw_buffer_inc_bitsize(&buffer))
		{
		  lzw_buffer_put_code(&buffer, LZW_RESET_CODE);
		  lzw_buffer_set_bitsize(&buffer, MIN_BITSIZE);
		  lzw_dict_reset(&dict);
		}
	    }
	}
    }
  
  lzw_buffer_put_code(&buffer, string.prefix);
  if (buffer.maxval - data->early_change == dict.size)
    lzw_buffer_inc_bitsize(&buffer);
  lzw_buffer_put_code(&buffer, LZW_EOD_CODE);
  lzw_buffer_put_code(&buffer, 0);

  /* Resize buffer to fit the data. */
  *out_size = (buffer.curp - *out);
  if ((*out = xrealloc(*out, *out_size)) == NULL)
    {
      *out_size = 0;
      return PDF_ERROR;
    }

  return PDF_OK;
}

/* -- The decoder -- */

/* Utility to write to the output. */

typedef struct lzw_writer_s
{
  pdf_char_t* buf;
  pdf_char_t* cur;
  int writen;
  int allocated;
} lzw_writer_t;

static int
lzw_writer_init (lzw_writer_t* s,
		 int size)
{
  if ((s->buf = xmalloc(size)) == NULL)
    {
      return PDF_ERROR;
    }

  s->cur = s->buf;
  s->writen = 0;
  s->allocated = size;
  
  return PDF_OK;
}

static int
lzw_writer_fit (lzw_writer_t* s)
{
  if ((s->buf = xrealloc(s->buf, s->writen)) == NULL)
    {
      return PDF_ERROR;
    }

  s->cur = s->buf + s->writen;
  s->allocated = s->writen;
  
  return PDF_OK;
}

static int
lzw_writer_put (lzw_writer_t* s,
		pdf_char_t* data,
		unsigned size)
{
  if (s->allocated < s->writen + size)
    {
      s->allocated = s->allocated * 2 + 1;
      if ((s->buf = xrealloc(s->buf, s->allocated)) == NULL)
	{
	  return PDF_ERROR;
	}
      s->cur = s->buf + s->writen;
    }

  memcpy(s->cur, data, size);
  s->cur += size;
  s->writen += size;
  
  return PDF_OK;
}

static void
lzw_writer_destroy (lzw_writer_t* s)
{
  free(s->buf);
}

static int
pdf_stm_f_lzw_decode (pdf_stm_f_lzw_data_t data,
                      pdf_char_t *in,
                      pdf_stm_pos_t in_size,
                      pdf_char_t **out,
                      pdf_stm_pos_t *out_size)
{
  pdf_char_t dec_buf[MAX_DICTSIZE];
  pdf_char_t* decoded;
  unsigned dec_size;
  
  unsigned new_code;
  unsigned old_code;
   
  lzw_buffer_t buffer;
  lzw_dict_t dict;
  lzw_writer_t writer;

  *out = NULL;
  *out_size = 0;

  if (lzw_writer_init(&writer, in_size) == PDF_ERROR)
    return PDF_ERROR;
  
  lzw_buffer_init(&buffer, in, in_size,  MIN_BITSIZE);
  lzw_dict_init(&dict);
  old_code = NULL_INDEX;
  
  do {
    lzw_buffer_set_bitsize(&buffer, MIN_BITSIZE);
    lzw_dict_reset(&dict);

    do {
      new_code = lzw_buffer_get_code(&buffer);
    } while(new_code == LZW_RESET_CODE);
    
    if (new_code == NULL_INDEX)
      {
	lzw_writer_destroy(&writer);
	return PDF_ERROR;
      }
    
    if (new_code != LZW_EOD_CODE)
      {
	if (lzw_writer_put(&writer, (pdf_char_t*)&new_code, 1) == PDF_ERROR)
	  return PDF_ERROR;
	
	old_code = new_code;
	new_code = lzw_buffer_get_code(&buffer);
      }
    
    while (new_code != LZW_EOD_CODE && new_code != LZW_RESET_CODE)
      {
	decoded = &(dec_buf[MAX_DICTSIZE-2]);
	
	if (new_code < dict.size) /* Is new code in the dict? */
	  {
	    lzw_dict_decode(&dict, new_code, &decoded, &dec_size);
	    lzw_dict_fast_add(&dict, old_code, decoded[0]);
	  }
	else
	  {
	    lzw_dict_decode(&dict, old_code, &decoded, &dec_size);
	    lzw_dict_fast_add(&dict, old_code, decoded[0]);
	    decoded[dec_size++] = decoded[0];
	  }

	if (lzw_writer_put(&writer, decoded, dec_size) == PDF_ERROR)
	    return PDF_ERROR;

	if (dict.size == buffer.maxval - 1 - data->early_change)
	  if (!lzw_buffer_inc_bitsize(&buffer));
	    /* break; We must wait for the reset code, don't reset yet. */
	  	
	old_code = new_code;
	new_code = lzw_buffer_get_code(&buffer);
	
	if (new_code == NULL_INDEX)
	  {
	    lzw_writer_destroy(&writer);
	    return PDF_ERROR;
	  }
      }
  } while (new_code != LZW_EOD_CODE);
  
  if (lzw_writer_fit(&writer) == PDF_ERROR)
    return PDF_ERROR;
  
  *out = writer.buf;
  *out_size = writer.writen;

  return PDF_OK;  
}


/* -- PDF Filter functions --*/

int 
pdf_stm_f_lzw_init (void **filter_data,
                    void *conf_data)
{
  pdf_stm_f_lzw_data_t *data;
  pdf_stm_f_lzw_conf_t conf;

  data = (pdf_stm_f_lzw_data_t *) filter_data;
  conf = (pdf_stm_f_lzw_conf_t) conf_data;

  /* Create the private data storage */
  *data =
    (pdf_stm_f_lzw_data_t) xmalloc (sizeof(struct pdf_stm_f_lzw_data_s));
  (*data)->mode = conf->mode;
  (*data)->early_change = conf->early_change;

  return PDF_OK;
}

int
pdf_stm_f_lzw_apply (void *filter_data,
                     pdf_char_t *in, pdf_stm_pos_t in_size,
                     pdf_char_t **out, pdf_stm_pos_t *out_size)
{
  pdf_stm_f_lzw_data_t data;

  data = (pdf_stm_f_lzw_data_t) filter_data;
  switch (data->mode)
    {
    case PDF_STM_F_LZW_MODE_ENCODE:
      {
        return pdf_stm_f_lzw_encode (data, in, in_size, out, out_size);
      }
    case PDF_STM_F_LZW_MODE_DECODE:
      {
        return pdf_stm_f_lzw_decode (data, in, in_size, out, out_size);
      }
    default:
      {
        return PDF_ERROR;
      }
    }
  
  /* Not reached */
}

int
pdf_stm_f_lzw_dealloc (void **filter_data)
{
  pdf_stm_f_lzw_data_t *data;

  data = (pdf_stm_f_lzw_data_t *) filter_data;
  free (*data);

  return PDF_OK;
}

/* End of pdf_stm_f_lzw.c */

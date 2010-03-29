/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-lzw.c
 *       Date:         Wed Aug 15 14:41:18 2007
 *
 *       GNU PDF Library - LZW encoder/decoder filter
 *
 */

/* Copyright (C) 2007, 2008, 2009 Free Software Foundation, Inc. */

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

#include <pdf-alloc.h>
#include <pdf-hash-helper.h>
#include <pdf-stm-f-lzw.h>

#define LZW_DEFAULT_EARLY_CHANGE  1

#define LZW_CACHE_SIZE    (sizeof (lzw_code_t) << 3)
#define LZW_MIN_BITSIZE   9
#define LZW_MAX_BITSIZE   12
#define LZW_MAX_DICTSIZE  (1 << LZW_MAX_BITSIZE)
#define LZW_NULL_INDEX    ~0U

enum lzw_special_codes_e
  {
    LZW_RESET_CODE = 256,
    LZW_EOD_CODE,
    LZW_FIRST_CODE
  };

typedef unsigned lzw_code_t;
#define LZW_CODE_SIZE (sizeof (lzw_code_t) << 3)

/* -- LZW code output/input -- */
/*
 * Object to read and write codes of variable bitsize in a buffer.
 * Warning: using both get and put functions may break the buffer.
 */
struct lzw_buffer_s
{
  pdf_buffer_t buf;
  pdf_char_t cache [LZW_CACHE_SIZE];
  pdf_size_t cache_rp;
  pdf_size_t cache_wp;
  lzw_code_t valbuf;
  lzw_code_t maxval;
  int bitsize;
  int valbits;
};
typedef struct lzw_buffer_s* lzw_buffer_t;

static void
lzw_buffer_init (lzw_buffer_t b,
		 int bitsize)
{
  b->buf = NULL;
  b->valbuf = 0;
  b->valbits = 0;
  b->bitsize = bitsize;
  b->maxval = (1 << bitsize) - 1;
  b->cache_wp = 0;
  b->cache_rp = 0;
}

static void
lzw_buffer_set (lzw_buffer_t b,
		pdf_buffer_t buf)
{
  b->buf = buf;
}

static pdf_status_t
lzw_buffer_get_code (lzw_buffer_t b,
		     lzw_code_t* code,
		     pdf_bool_t finish_p)
{
  lzw_code_t r;

  while (b->valbits <= LZW_CODE_SIZE - 8 && !finish_p)
    {
      if (pdf_buffer_eob_p (b->buf))
	{
	  return PDF_ENINPUT;
	}
      else
	{
	  b->valbuf |=
	    (lzw_code_t) b->buf->data [b->buf->rp++] 
	    << (LZW_CODE_SIZE - 8 - b->valbits);
	
	  b->valbits += 8;
	}
    }

  if (b->valbits < b->bitsize)
    {
      return PDF_EEOF;
    }

  r = b->valbuf >> (LZW_CODE_SIZE - b->bitsize);
  b->valbuf <<= b->bitsize;
  b->valbits -= b->bitsize;
  
  *code = r;

  return PDF_OK;
}

/* Once finished, call with 0 as code value to flush the buffer. */
static void
lzw_buffer_put_code (lzw_buffer_t b,
		     lzw_code_t code)
{
  b->valbuf |= (lzw_code_t) code << (LZW_CODE_SIZE - b->bitsize - b->valbits);
  b->valbits += b->bitsize;

  while (b->valbits >= 8)
    {
      if (pdf_buffer_full_p (b->buf))
	{
	  b->cache [b->cache_wp++] = 
	    (pdf_char_t) (b->valbuf >> (LZW_CODE_SIZE - 8));
	}
      else
	{
	  b->buf->data [b->buf->wp++] = 
	    (pdf_char_t) (b->valbuf >> (LZW_CODE_SIZE - 8));
	}
      b->valbuf <<= 8;
      b->valbits -= 8;
    }
}

static pdf_status_t
lzw_buffer_flush (lzw_buffer_t b)
{
  while (b->cache_rp != b->cache_wp &&
	 !pdf_buffer_full_p (b->buf))
    {
      b->buf->data [b->buf->wp++] = b->cache [b->cache_rp++];
    }

  if (pdf_buffer_full_p (b->buf))
    {
      return PDF_ENOUTPUT;
    }
  
  b->cache_wp = 0;
  b->cache_rp = 0;
  
  return PDF_OK;
}

static pdf_status_t
lzw_buffer_inc_bitsize (lzw_buffer_t b)
{
  if (b->bitsize == LZW_MAX_BITSIZE)
    {
      return PDF_ERROR;
    }
  
  ++b->bitsize;
  b->maxval = (1 << b->bitsize) - 1;

  return PDF_OK;
}

static void
lzw_buffer_set_bitsize (lzw_buffer_t b,
			int newsize)
{
  b->bitsize = newsize;
  b->maxval = (1 << newsize) - 1;
}

/* -- LZW dictionary handler -- */

/*
 * The strings are stored in a non balanced ordered binary tree.
 */
struct lzw_string_s
{
  lzw_code_t prefix;   /* Prefix string code */
  pdf_char_t suffix; /* Appended character */

  lzw_code_t first; /* First string with the same prefix.  */
  lzw_code_t left;  /* Next string with smaller suffix and same prefix. */
  lzw_code_t right; /* Next string with greater suffix and same prefix. */
};

typedef struct lzw_string_s* lzw_string_t;

static void
lzw_string_init (lzw_string_t s)
{
  memset (s, 0xFF, sizeof (struct lzw_string_s));
}

struct lzw_dict_s
{
  struct lzw_string_s table [LZW_MAX_DICTSIZE];
  pdf_size_t size;
};
typedef struct lzw_dict_s* lzw_dict_t;

static void
lzw_dict_init (lzw_dict_t d)
{
  int i;
  
  memset (d->table,
	  0xFF,
	  sizeof (struct lzw_string_s) * LZW_MAX_DICTSIZE);

  for (i = 0; i < LZW_FIRST_CODE; i++)
    {
      d->table[i].suffix = (pdf_char_t) (i & 0xFF);
    }

  d->size = LZW_FIRST_CODE;
}

static pdf_bool_t
lzw_dict_add (lzw_dict_t d,
	      lzw_string_t s)
{
  lzw_code_t index;
  pdf_bool_t must_add;
  
  if (s->prefix == LZW_NULL_INDEX)
    {
      s->prefix = s->suffix;
      return PDF_FALSE; /* The string is a basic character, found! */
    }
  
  index = d->table[s->prefix].first;

  if (index == LZW_NULL_INDEX)
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
	      if (d->table[index].left == LZW_NULL_INDEX)
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
	      if (d->table[index].right == LZW_NULL_INDEX)
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

static void
lzw_dict_reset (lzw_dict_t dict)
{
  lzw_dict_init (dict);
}

static void
lzw_dict_fast_add (lzw_dict_t d,
		   lzw_code_t prefix,
		   pdf_char_t suffix)
{
  d->table[d->size].prefix = prefix;
  d->table[d->size].suffix = suffix;
  d->size++;
}

static void
lzw_dict_decode (lzw_dict_t d,
		 lzw_code_t code,
		 pdf_char_t** decode,
		 pdf_size_t* size)
{
  *size = 0;

  do
    {
      *(*decode)-- = d->table[code].suffix;
      ++(*size);
      code = d->table[code].prefix;
    }
  while (code != LZW_NULL_INDEX);

  (*decode)++;
}

/* -- THE ENCODER -- */

struct lzwenc_state_s
{
  /* cached params */
  pdf_i32_t early_change;

  /* encoding state */
  pdf_bool_t           must_reset_p;
  struct lzw_buffer_s  buffer;
  struct lzw_dict_s    dict;
  struct lzw_string_s  string;

  pdf_bool_t           really_finish_p;
};
typedef struct lzwenc_state_s* lzwenc_state_t;

pdf_status_t
pdf_stm_f_lzwenc_init (pdf_hash_t params,
		       void **ext_state)
{
  pdf_char_t *early_change_str;
  lzwenc_state_t state;

  state = pdf_alloc (sizeof (struct lzwenc_state_s));
  if (!state)
    {
      return PDF_ERROR;
    }
  
  state->early_change = 1; /* set default */
  
  if (pdf_hash_key_p (params, "EarlyChange") == PDF_TRUE)
    {
      pdf_hash_get_string (params, "EarlyChange", &early_change_str);
      if (early_change_str[0] == '0')
        {
          state->early_change = 0;
        }
    }

  lzw_buffer_init (&state->buffer, LZW_MIN_BITSIZE);
  lzw_dict_init (&state->dict);
  lzw_string_init (&state->string);
  state->must_reset_p = PDF_TRUE;
  state->really_finish_p = PDF_FALSE;
  
  *ext_state = state;
  return PDF_OK;
}

pdf_status_t
pdf_stm_f_lzwenc_apply (pdf_hash_t params,
			void *ext_state,
			pdf_buffer_t in,
			pdf_buffer_t out,
			pdf_bool_t finish_p)
{
  pdf_status_t ret;
  lzwenc_state_t st;

  ret = PDF_OK;
  st  = ext_state;
  lzw_buffer_set (&st->buffer, out);

  ret = lzw_buffer_flush (&st->buffer);
  if (ret != PDF_OK)
    {
      return ret;
    }
  
  if (st->really_finish_p)
    {
      return PDF_EEOF;
    }
  
  if (st->must_reset_p)
    {
      lzw_buffer_put_code (&st->buffer, LZW_RESET_CODE);
      st->must_reset_p = PDF_FALSE;
    }
  
  while (!pdf_buffer_eob_p (in) &&
	 !pdf_buffer_full_p (out))
    {
      st->string.suffix = in->data [in->rp++];
      if (lzw_dict_add (&st->dict, &st->string))
	{
	  lzw_buffer_put_code (&st->buffer, st->string.prefix);
	  st->string.prefix = st->string.suffix;

	  if (st->buffer.maxval - st->early_change == st->dict.size)
	    {
	      if (!lzw_buffer_inc_bitsize(&st->buffer))
		{
		  lzw_buffer_put_code (&st->buffer, LZW_RESET_CODE);
		  lzw_buffer_set_bitsize (&st->buffer, LZW_MIN_BITSIZE);
		  lzw_dict_reset (&st->dict);
		}
	    }
	}
    }

  if (finish_p)
    {
      lzw_buffer_put_code (&st->buffer, st->string.prefix);
      if ((st->buffer.maxval - st->early_change) == st->dict.size)
	{
	  lzw_buffer_inc_bitsize(&st->buffer);
	}
      
      lzw_buffer_put_code (&st->buffer, LZW_EOD_CODE);
      lzw_buffer_put_code (&st->buffer, 0); /* flush */

      if (pdf_buffer_full_p (out))
	{
	  ret = PDF_ENOUTPUT;
	}
      else
	{
	  ret = PDF_EEOF;
	}

      st->really_finish_p = PDF_TRUE;
    }
  else if (pdf_buffer_full_p (out))
    {
      ret = PDF_ENOUTPUT;
    }
  else if (pdf_buffer_eob_p (in))
    {
      ret = PDF_ENINPUT;
    }
  
  return ret;
}

pdf_status_t
pdf_stm_f_lzwenc_dealloc_state (void *state)
{
  pdf_dealloc (state);
  return PDF_OK;
}

/* -- THE DECODER -- */

enum lzwdec_state
  {
    LZWDEC_STATE_START,
    LZWDEC_STATE_CLEAN,
    LZWDEC_STATE_WRITE,
    LZWDEC_STATE_READ,
    LZWDEC_STATE_LOOP_WRITE,
    LZWDEC_STATE_LOOP_READ
  };

struct lzwdec_state_s
{
  /* cached params */
  pdf_i32_t early_change;

  /* state */
  pdf_char_t  dec_buf [LZW_MAX_DICTSIZE];
  pdf_char_t* decoded;
  pdf_size_t  dec_size;
  
  lzw_code_t new_code;
  lzw_code_t old_code;

  /* flow managment */
  enum lzwdec_state state_pos;
  pdf_status_t tmp_ret;
  
  struct lzw_buffer_s buffer;
  struct lzw_dict_s   dict;
};
typedef struct lzwdec_state_s* lzwdec_state_t;


pdf_status_t
pdf_stm_f_lzwdec_init (pdf_hash_t params,
		       void **ext_state)
{
  pdf_char_t *early_change_str;
  lzwdec_state_t state;

  state = pdf_alloc (sizeof (struct lzwdec_state_s));
  if (!state)
    {
      return PDF_ERROR;
    }
    
  state->early_change = 1; /* set default */
  
  if (pdf_hash_key_p (params, "EarlyChange") == PDF_TRUE)
    {
      pdf_hash_get_string (params, "EarlyChange", &early_change_str);
      if (early_change_str[0] == '0')
        {
          state->early_change = 0;
        }
    }

  lzw_buffer_init (&state->buffer, LZW_MIN_BITSIZE);
  lzw_dict_init (&state->dict);
  state->old_code = LZW_NULL_INDEX;
  state->decoded = state->dec_buf + (LZW_MAX_DICTSIZE-2);
  state->dec_size = 0;
  state->state_pos = LZWDEC_STATE_START;
  state->tmp_ret = 0;
    
  *ext_state = state;
  return PDF_OK;
}

pdf_status_t
lzwdec_put_decoded (lzwdec_state_t st, pdf_buffer_t out)
{
  pdf_status_t ret;
  pdf_size_t to_write;
  
  ret = PDF_OK;

  if (st->dec_size)
    {
      /* output the decoded string */
      to_write = st->dec_size;
      if (st->dec_size > (out->size - out->wp))
	{
	  to_write = out->size - out->wp;
	  ret = PDF_ENOUTPUT;
	}

      memcpy (out->data + out->wp, st->decoded, to_write);
      out->wp += to_write;
      st->decoded += to_write;
      st->dec_size -= to_write;
    }

  return ret;
}

pdf_status_t
lzwdec_put_code (lzwdec_state_t st,
		 pdf_buffer_t out,
		 unsigned long  code)
{
  if (pdf_buffer_full_p (out))
    {
      return PDF_ENOUTPUT;
    }
  
  out->data [out->wp++] = (pdf_char_t) (code & 0xFF);
  
  return PDF_OK;
}

#define LZWDEC_CHECK(st, pos, what)		\
  do { (st)->state_pos = (pos);			\
    if (((st)->tmp_ret = (what)) != PDF_OK)	\
      { return ((st)->tmp_ret); }		\
  } while (0);

pdf_status_t
pdf_stm_f_lzwdec_apply (pdf_hash_t params,
			void *ext_state,
			pdf_buffer_t in,
			pdf_buffer_t out,
			pdf_bool_t finish_p)
{
  lzwdec_state_t st;

  st = ext_state;
  lzw_buffer_set (&st->buffer, in);

  switch (st->state_pos)
    {
    case LZWDEC_STATE_START:
      break;
    case LZWDEC_STATE_CLEAN:
      goto lzwdec_state_clean;
    case LZWDEC_STATE_WRITE:
      goto lzwdec_state_write;
    case LZWDEC_STATE_READ:
      goto lzwdec_state_read;
    case LZWDEC_STATE_LOOP_WRITE:
      goto lzwdec_state_loop_write;
    case LZWDEC_STATE_LOOP_READ:
      goto lzwdec_state_loop_read;
    default:
      break;
    }
  
  do
    {
      /* need a reset */
      lzw_buffer_set_bitsize (&st->buffer, LZW_MIN_BITSIZE);
      lzw_dict_reset (&st->dict);
      
      do 
	{
	lzwdec_state_clean:
	  LZWDEC_CHECK (st, LZWDEC_STATE_CLEAN,
			lzw_buffer_get_code (&st->buffer, &st->new_code, finish_p));
	}
      while (st->new_code == LZW_RESET_CODE);
      
      if (st->new_code != LZW_EOD_CODE)
	{
	lzwdec_state_write:
	  LZWDEC_CHECK (st, LZWDEC_STATE_WRITE,
			lzwdec_put_code (st, out, st->new_code));

	  st->old_code = st->new_code;
	lzwdec_state_read:
	  LZWDEC_CHECK (st, LZWDEC_STATE_READ,
			lzw_buffer_get_code (&st->buffer, &st->new_code, finish_p));  
	}
      
      while (st->new_code != LZW_EOD_CODE &&
	     st->new_code != LZW_RESET_CODE)
	{
	  st->decoded = st->dec_buf + (LZW_MAX_DICTSIZE-2);

	  /* Is new code in the dict? */
	  if (st->new_code < st->dict.size)
	    {
	      lzw_dict_decode (&st->dict, st->new_code,
			       &st->decoded, &st->dec_size);
	      lzw_dict_fast_add (&st->dict, st->old_code, st->decoded[0]);
	    }
	  else
	    {
	      lzw_dict_decode (&st->dict, st->old_code,
			       &st->decoded, &st->dec_size);
	      lzw_dict_fast_add (&st->dict, st->old_code, st->decoded[0]);
	      st->decoded [st->dec_size++] = st->decoded [0];
	    }

	  /* output the decoded string */
	lzwdec_state_loop_write:
	  LZWDEC_CHECK (st, LZWDEC_STATE_LOOP_WRITE,
			lzwdec_put_decoded (st, out));
	  
	  if (st->dict.size == st->buffer.maxval - 1 - st->early_change)
	    {
	      if (!lzw_buffer_inc_bitsize (&st->buffer));
	      /* break; We must wait for the reset code, don't reset yet. */
	    }
	  
	  /* get next code */
	  st->old_code = st->new_code;
	lzwdec_state_loop_read:
	  LZWDEC_CHECK (st, LZWDEC_STATE_LOOP_READ,
			lzw_buffer_get_code (&st->buffer, &st->new_code, finish_p));
	}
    }
  while (st->new_code != LZW_EOD_CODE);

  st->state_pos = LZWDEC_STATE_START;
  
  return PDF_EEOF;
}

pdf_status_t
pdf_stm_f_lzwdec_dealloc_state (void *state)
{
  pdf_dealloc (state);
  return PDF_OK;
}

/* End of pdf_stm_f_lzw.c */

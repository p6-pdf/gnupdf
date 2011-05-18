/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-lzw.c
 *       Date:         Wed Aug 15 14:41:18 2007
 *
 *       GNU PDF Library - LZW encoder/decoder filter
 *
 */

/* Copyright (C) 2007-2011 Free Software Foundation, Inc. */

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

#include <pdf-types.h>
#include <pdf-types-buffer.h>
#include <pdf-hash.h>
#include <pdf-alloc.h>
#include <pdf-hash-helper.h>
#include <pdf-stm-f-lzw.h>

static pdf_bool_t stm_f_lzwenc_init (const pdf_hash_t  *params,
                                     void             **state,
                                     pdf_error_t      **error);

static void stm_f_lzwenc_deinit (void *state);

static enum pdf_stm_filter_apply_status_e stm_f_lzwenc_apply (void          *state,
                                                              pdf_buffer_t  *in,
                                                              pdf_buffer_t  *out,
                                                              pdf_bool_t     finish,
                                                              pdf_error_t  **error);

static pdf_bool_t stm_f_lzwdec_init (const pdf_hash_t  *params,
                                     void             **state,
                                     pdf_error_t      **error);

static void stm_f_lzwdec_deinit (void *state);

static enum pdf_stm_filter_apply_status_e stm_f_lzwdec_apply (void          *state,
                                                              pdf_buffer_t  *in,
                                                              pdf_buffer_t  *out,
                                                              pdf_bool_t     finish,
                                                              pdf_error_t  **error);

/* -- Filter implementations -- */

static const pdf_stm_filter_impl_t enc_impl = {
  .init_fn   = stm_f_lzwenc_init,
  .apply_fn  = stm_f_lzwenc_apply,
  .deinit_fn = stm_f_lzwenc_deinit,
};

static const pdf_stm_filter_impl_t dec_impl = {
  .init_fn   = stm_f_lzwdec_init,
  .apply_fn  = stm_f_lzwdec_apply,
  .deinit_fn = stm_f_lzwdec_deinit,
};

const pdf_stm_filter_impl_t *
pdf_stm_f_lzwenc_get (void)
{
  return &enc_impl;
}

const pdf_stm_filter_impl_t *
pdf_stm_f_lzwdec_get (void)
{
  return &dec_impl;
}

/* -- LZW helper definitions -- */

typedef unsigned lzw_code_t;
#define LZW_DEFAULT_EARLY_CHANGE  1
#define LZW_CODE_SIZE (sizeof (lzw_code_t) << 3)
#define LZW_CACHE_SIZE    (sizeof (lzw_code_t) << 3)
#define LZW_MIN_BITSIZE   9
#define LZW_MAX_BITSIZE   12
#define LZW_MAX_DICTSIZE  (1 << LZW_MAX_BITSIZE)
#define LZW_NULL_INDEX    ~0U

enum lzw_special_codes_e {
  LZW_RESET_CODE = 256,
  LZW_EOD_CODE,
  LZW_FIRST_CODE
};

/* -- LZW code output/input -- */
/*
 * Object to read and write codes of variable bitsize in a buffer.
 * Warning: using both get and put functions may break the buffer.
 */
struct lzw_buffer_s
{
  pdf_buffer_t *buf;
  pdf_uchar_t cache [LZW_CACHE_SIZE];
  pdf_size_t cache_rp;
  pdf_size_t cache_wp;
  lzw_code_t valbuf;
  lzw_code_t maxval;
  int bitsize;
  int valbits;
};

static void
lzw_buffer_init (struct lzw_buffer_s *b,
                 int                  bitsize)
{
  b->buf = NULL;
  b->valbuf = 0;
  b->valbits = 0;
  b->bitsize = bitsize;
  b->maxval = (1 << bitsize) - 1;
  b->cache_wp = 0;
  b->cache_rp = 0;
}

static enum pdf_stm_filter_apply_status_e
lzw_buffer_get_code (struct lzw_buffer_s *b,
                     pdf_bool_t           finish,
                     lzw_code_t          *code)   /* out */
{
  lzw_code_t r;

  while (b->valbits <= LZW_CODE_SIZE - 8 && !finish)
    {
      if (!pdf_buffer_eob_p (b->buf))
        {
          b->valbuf |=
            (lzw_code_t) b->buf->data [b->buf->rp++]
            << (LZW_CODE_SIZE - 8 - b->valbits);

          b->valbits += 8;
        }
      else
        return PDF_STM_FILTER_APPLY_STATUS_NO_INPUT;
    }

  if (b->valbits < b->bitsize)
    return PDF_STM_FILTER_APPLY_STATUS_EOF;

  r = b->valbuf >> (LZW_CODE_SIZE - b->bitsize);
  b->valbuf <<= b->bitsize;
  b->valbits -= b->bitsize;

  *code = r;

  return PDF_STM_FILTER_APPLY_STATUS_OK;
}

/* Once finished, call with 0 as code value to flush the buffer. */
static void
lzw_buffer_put_code (struct lzw_buffer_s *b,
                     lzw_code_t           code)
{
  b->valbuf |= (lzw_code_t) code << (LZW_CODE_SIZE - b->bitsize - b->valbits);
  b->valbits += b->bitsize;

  while (b->valbits >= 8)
    {
      if (pdf_buffer_full_p (b->buf))
        {
          b->cache [b->cache_wp++] =
            (pdf_uchar_t) (b->valbuf >> (LZW_CODE_SIZE - 8));
        }
      else
        {
          b->buf->data [b->buf->wp++] =
            (pdf_uchar_t) (b->valbuf >> (LZW_CODE_SIZE - 8));
        }
      b->valbuf <<= 8;
      b->valbits -= 8;
    }
}

static pdf_bool_t
lzw_buffer_flush (struct lzw_buffer_s *b)
{
  while (b->cache_rp != b->cache_wp &&
         !pdf_buffer_full_p (b->buf))
    {
      b->buf->data [b->buf->wp++] = b->cache [b->cache_rp++];
    }

  if (pdf_buffer_full_p (b->buf))
    return PDF_FALSE;

  b->cache_wp = 0;
  b->cache_rp = 0;

  return PDF_TRUE;
}

static pdf_bool_t
lzw_buffer_inc_bitsize (struct lzw_buffer_s *b)
{
  if (b->bitsize == LZW_MAX_BITSIZE)
    return PDF_FALSE;

  ++b->bitsize;
  b->maxval = (1 << b->bitsize) - 1;

  return PDF_TRUE;
}

static void
lzw_buffer_set_bitsize (struct lzw_buffer_s *b,
                        int                  newsize)
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
  pdf_uchar_t suffix; /* Appended character */

  lzw_code_t first; /* First string with the same prefix.  */
  lzw_code_t left;  /* Next string with smaller suffix and same prefix. */
  lzw_code_t right; /* Next string with greater suffix and same prefix. */
};

struct lzw_dict_s
{
  struct lzw_string_s table [LZW_MAX_DICTSIZE];
  pdf_size_t size;
};

static void
lzw_string_init (struct lzw_string_s *s)
{
  memset (s, 0xFF, sizeof (struct lzw_string_s));
}

static void
lzw_dict_init (struct lzw_dict_s *d)
{
  int i;

  memset (d->table,
          0xFF,
          sizeof (struct lzw_string_s) * LZW_MAX_DICTSIZE);

  for (i = 0; i < LZW_FIRST_CODE; i++)
    {
      d->table[i].suffix = (pdf_uchar_t) (i & 0xFF);
    }

  d->size = LZW_FIRST_CODE;
}

static pdf_bool_t
lzw_dict_add (struct lzw_dict_s   *d,
              struct lzw_string_s *s)
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
lzw_dict_reset (struct lzw_dict_s *dict)
{
  lzw_dict_init (dict);
}

static void
lzw_dict_fast_add (struct lzw_dict_s *d,
                   lzw_code_t         prefix,
                   pdf_uchar_t        suffix)
{
  d->table[d->size].prefix = prefix;
  d->table[d->size].suffix = suffix;
  d->size++;
}

static void
lzw_dict_decode (struct lzw_dict_s  *d,
                 lzw_code_t          code,
                 pdf_uchar_t       **decode,
                 pdf_size_t         *size)
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
  pdf_bool_t           must_reset;
  struct lzw_buffer_s  buffer;
  struct lzw_dict_s    dict;
  struct lzw_string_s  string;

  pdf_bool_t           really_finish;
};

static pdf_bool_t
stm_f_lzwenc_init (const pdf_hash_t  *params,
                   void             **state,
                   pdf_error_t      **error)
{
  struct lzwenc_state_s *filter_state;

  filter_state = pdf_alloc (sizeof (struct lzwenc_state_s));
  if (!filter_state)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "cannot create LZW encoder internal state: "
                     "couldn't allocate %lu bytes",
                     (unsigned long)sizeof (struct lzwenc_state_s));
      return PDF_FALSE;
    }

  filter_state->early_change = 1; /* set default */

  /* EarlyChange is optional! */
  if (params &&
      pdf_hash_key_p (params, "EarlyChange"))
    {
      const pdf_char_t *early_change_str;

      /* TODO: How come EarlyChange should be '0'? */
      early_change_str = pdf_hash_get_string (params, "EarlyChange");
      if (early_change_str[0] == '0')
        filter_state->early_change = 0;
    }

  lzw_buffer_init (&filter_state->buffer, LZW_MIN_BITSIZE);
  lzw_dict_init (&filter_state->dict);
  lzw_string_init (&filter_state->string);
  filter_state->must_reset = PDF_TRUE;
  filter_state->really_finish = PDF_FALSE;

  *state = filter_state;
  return PDF_TRUE;
}

static enum pdf_stm_filter_apply_status_e
stm_f_lzwenc_apply (void          *state,
                    pdf_buffer_t  *in,
                    pdf_buffer_t  *out,
                    pdf_bool_t     finish,
                    pdf_error_t  **error)
{
  struct lzwenc_state_s *st;

  st  = state;
  st->buffer.buf = out;

  if (!lzw_buffer_flush (&st->buffer))
    return PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT;

  if (st->really_finish)
    return PDF_STM_FILTER_APPLY_STATUS_EOF;

  if (st->must_reset)
    {
      lzw_buffer_put_code (&st->buffer, LZW_RESET_CODE);
      st->must_reset = PDF_FALSE;
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
              if (!lzw_buffer_inc_bitsize (&st->buffer))
                {
                  lzw_buffer_put_code (&st->buffer, LZW_RESET_CODE);
                  lzw_buffer_set_bitsize (&st->buffer, LZW_MIN_BITSIZE);
                  lzw_dict_reset (&st->dict);
                }
            }
        }
    }

  if (finish)
    {
      lzw_buffer_put_code (&st->buffer, st->string.prefix);
      if ((st->buffer.maxval - st->early_change) == st->dict.size)
        {
          lzw_buffer_inc_bitsize (&st->buffer);
        }

      lzw_buffer_put_code (&st->buffer, LZW_EOD_CODE);
      lzw_buffer_put_code (&st->buffer, 0); /* flush */

      st->really_finish = PDF_TRUE;

      return (pdf_buffer_full_p (out) ?
              PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT :
              PDF_STM_FILTER_APPLY_STATUS_EOF);
    }

  if (pdf_buffer_full_p (out))
      return PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT;

  /* Default, request more input */
  return PDF_STM_FILTER_APPLY_STATUS_NO_INPUT;
}

static void
stm_f_lzwenc_deinit (void *state)
{
  pdf_dealloc (state);
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
  pdf_uchar_t  dec_buf [LZW_MAX_DICTSIZE];
  pdf_uchar_t *decoded;
  pdf_size_t   dec_size;

  lzw_code_t new_code;
  lzw_code_t old_code;

  /* flow managment */
  enum lzwdec_state state_pos;
  pdf_error_t *tmp_error;

  struct lzw_buffer_s buffer;
  struct lzw_dict_s   dict;
};

static pdf_bool_t
stm_f_lzwdec_init (const pdf_hash_t  *params,
                   void             **state,
                   pdf_error_t      **error)
{
  struct lzwdec_state_s *filter_state;

  filter_state = pdf_alloc (sizeof (struct lzwdec_state_s));
  if (!filter_state)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "cannot create LZW decoder internal state: "
                     "couldn't allocate %lu bytes",
                     (unsigned long)sizeof (struct lzwdec_state_s));
      return PDF_FALSE;
    }

  filter_state->early_change = 1; /* set default */

  /* EarlyChange is optional! */
  if (pdf_hash_key_p (params, "EarlyChange"))
    {
      const pdf_char_t *early_change_str;

      /* TODO: How come EarlyChange should be '0'? */
      early_change_str = pdf_hash_get_string (params, "EarlyChange");
      if (early_change_str[0] == '0')
        filter_state->early_change = 0;
    }

  lzw_buffer_init (&filter_state->buffer, LZW_MIN_BITSIZE);
  lzw_dict_init (&filter_state->dict);
  filter_state->old_code = LZW_NULL_INDEX;
  filter_state->decoded = filter_state->dec_buf + (LZW_MAX_DICTSIZE-2);
  filter_state->dec_size = 0;
  filter_state->state_pos = LZWDEC_STATE_START;
  filter_state->tmp_error = NULL;

  *state = filter_state;
  return PDF_TRUE;
}

static void
stm_f_lzwdec_deinit (void *state)
{
  pdf_dealloc (state);
}

/* Returns PDF_FALSE if no more output */
static pdf_bool_t
lzwdec_put_decoded (struct lzwdec_state_s *st,
                    pdf_buffer_t          *out)
{
  pdf_bool_t no_output = PDF_FALSE;

  if (st->dec_size)
    {
      pdf_size_t to_write;

      PDF_ASSERT (out->size >= out->wp);

      /* output the decoded string */
      to_write = st->dec_size;
      if (st->dec_size > (out->size - out->wp))
        {
          to_write = out->size - out->wp;
          no_output = PDF_TRUE;
        }

      memcpy (out->data + out->wp, st->decoded, to_write);
      out->wp += to_write;
      st->decoded += to_write;
      st->dec_size -= to_write;
    }

  return !no_output;
}

/* Returns PDF_FALSE if no more output */
static pdf_bool_t
lzwdec_put_code (struct lzwdec_state_s *st,
                 pdf_buffer_t          *out,
                 unsigned long          code)
{
  if (pdf_buffer_full_p (out))
    return PDF_FALSE;

  out->data [out->wp++] = (pdf_uchar_t) (code & 0xFF);
  return PDF_TRUE;
}

#define LZWDEC_CHECK(st, pos, what)                         \
  do {                                                      \
    enum pdf_stm_filter_apply_status_e ret;                 \
                                                            \
    (st)->state_pos = (pos);                                \
    if ((ret = (what)) != PDF_STM_FILTER_APPLY_STATUS_OK)	\
      { return ret; }                                       \
  } while (0);

static enum pdf_stm_filter_apply_status_e
stm_f_lzwdec_apply (void          *state,
                    pdf_buffer_t  *in,
                    pdf_buffer_t  *out,
                    pdf_bool_t     finish,
                    pdf_error_t  **error)
{
  struct lzwdec_state_s *st;

  st = state;
  st->buffer.buf = in;

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
          LZWDEC_CHECK (st,
                        LZWDEC_STATE_CLEAN,
                        lzw_buffer_get_code (&st->buffer,
                                             finish,
                                             &st->new_code));
        }
      while (st->new_code == LZW_RESET_CODE);

      if (st->new_code != LZW_EOD_CODE)
        {
        lzwdec_state_write:
          st->state_pos = LZWDEC_STATE_WRITE;
          if (!lzwdec_put_code (st, out, st->new_code))
            return PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT; /* No more output */

          st->old_code = st->new_code;

        lzwdec_state_read:
          LZWDEC_CHECK (st,
                        LZWDEC_STATE_READ,
                        lzw_buffer_get_code (&st->buffer,
                                             finish,
                                             &st->new_code));
        }

      while (st->new_code != LZW_EOD_CODE &&
             st->new_code != LZW_RESET_CODE)
        {
          st->decoded = st->dec_buf + (LZW_MAX_DICTSIZE - 2);

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
          st->state_pos = LZWDEC_STATE_LOOP_WRITE;
          if (!lzwdec_put_decoded (st, out))
            return PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT; /* No more output */

          if (st->dict.size == st->buffer.maxval - 1 - st->early_change)
            {
              if (!lzw_buffer_inc_bitsize (&st->buffer));
              /* break; We must wait for the reset code, don't reset yet. */
            }

          /* get next code */
          st->old_code = st->new_code;

        lzwdec_state_loop_read:
          LZWDEC_CHECK (st,
                        LZWDEC_STATE_LOOP_READ,
                        lzw_buffer_get_code (&st->buffer,
                                             finish,
                                             &st->new_code));
        }
    }
  while (st->new_code != LZW_EOD_CODE);

  st->state_pos = LZWDEC_STATE_START;
  return PDF_STM_FILTER_APPLY_STATUS_EOF;
}

/* End of pdf_stm_f_lzw.c */

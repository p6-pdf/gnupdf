/* -*- mode: C -*-
 *
 *       File:         pdf-token-reader.c
 *       Date:         Mon Dec 29 00:45:09 2008
 *
 *       GNU PDF Library - Stream tokeniser
 *
 */

/* Copyright (C) 2008, 2009 Free Software Foundation, Inc. */

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

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include <pdf-token-reader.h>

static INLINE pdf_status_t store_char (pdf_token_reader_t reader,
                                       pdf_char_t ch);
static INLINE pdf_status_t store_char_grow (pdf_token_reader_t reader,
                                            pdf_char_t ch);
static pdf_status_t exit_state (pdf_token_reader_t reader, pdf_u32_t flags,
                                pdf_token_t *token);
static INLINE pdf_status_t enter_state (pdf_token_reader_t reader,
                                        enum pdf_token_reader_state_e state);
static pdf_status_t flush_token (pdf_token_reader_t reader, pdf_u32_t flags,
                                 pdf_token_t *token);
static pdf_status_t handle_char (pdf_token_reader_t reader, pdf_u32_t flags,
                                 pdf_char_t ch, pdf_token_t *token);
static INLINE pdf_status_t handle_string_char (pdf_token_reader_t reader,
                                               pdf_u32_t flags,
                                               pdf_char_t ch,
                                               pdf_token_t *token);
static INLINE pdf_status_t handle_hexstring_char (pdf_token_reader_t reader,
                                                  pdf_u32_t flags,
                                                  pdf_char_t ch,
                                                  pdf_token_t *token);
static int recognise_number (pdf_buffer_t buffer, int *int_value);
static INLINE int parse_integer (pdf_buffer_t buffer, int *int_value,
                                 int *int_state);
static INLINE pdf_status_t parse_real (pdf_buffer_t buffer,
                                       char *locale_dec_pt,
                                       double *value);
static INLINE int validate_real (pdf_buffer_t buffer, int int_state);


pdf_status_t
pdf_token_reader_new (pdf_stm_t stm, pdf_token_reader_t *reader)
{
  pdf_status_t err;
  pdf_token_reader_t new_tokr;

  err = PDF_ENOMEM;
  new_tokr = pdf_alloc (sizeof (*new_tokr));
  if (!new_tokr)
    goto fail;

  new_tokr->beg_pos = 0;
  new_tokr->state_pos = 0;

  /* determine the current locale's decimal point
   * (avoid using localeconv since it may not be thread-safe) */
  new_tokr->decimal_point = NULL;
  {
    int len;
    char decpt[16];

    err = PDF_ERROR;
    len = snprintf (decpt, sizeof (decpt), "%#.0f", 1.0);
    if (len <= 0 || (pdf_size_t)len >= sizeof (decpt))  /* shouldn't happen */
      goto fail;

    err = PDF_ENOMEM;
    new_tokr->decimal_point = pdf_alloc (len);
    if (!new_tokr->decimal_point)
      goto fail;

    /* this copies the trailing '\0' due to the starting offset */
    memcpy (new_tokr->decimal_point, &decpt[1], len);
  }

  /* buffer_size_min is the default buffer size, which is also the maximum
   * size for keywords, names, numbers, etc.; strings and comments will
   * enlarge the buffer to whatever size is needed. */
  new_tokr->buffer_size_min = 32768;
  new_tokr->buffer = pdf_buffer_new (new_tokr->buffer_size_min);
  if (!new_tokr->buffer)
    goto fail;

  new_tokr->stream = stm;
  pdf_token_reader_reset (new_tokr);

  *reader = new_tokr;
  return PDF_OK;

fail:
  if (new_tokr)
    {
      if (new_tokr->decimal_point)
        pdf_dealloc (new_tokr->decimal_point);
      pdf_dealloc (new_tokr);
    }

  return err;
}

static void
reset_buffer (pdf_token_reader_t reader)
{
  reader->buffer->wp = 0;
  if (reader->buffer->size > reader->buffer_size_min)
    {
      /* Try to shrink the buffer, but don't worry if it fails. */
      pdf_buffer_resize (reader->buffer, reader->buffer_size_min);
    }
}

pdf_status_t
pdf_token_reader_reset (pdf_token_reader_t reader)
{
  enter_state (reader, PDF_TOKR_STATE_NONE);
  reader->substate = 0;
  reset_buffer (reader);
  return PDF_OK;
}

pdf_status_t
pdf_token_reader_destroy (pdf_token_reader_t reader)
{
  if (!reader) return PDF_EBADDATA;

  assert (reader->buffer);
  if (reader->buffer)
    pdf_buffer_destroy (reader->buffer);
  pdf_dealloc (reader->decimal_point);
  pdf_dealloc (reader);

  return PDF_OK;
}

static INLINE pdf_char_t
hexval (pdf_char_t ch)
{
  if (ch >= 48 && ch <= 48+9)  /* '0'--'9' */
    return ch - 48;
  if (ch >= 64+1 && ch <= 64+6)  /* 'A'--'F' */
    return ch - (64+1) + 10;
  if (ch >= 96+1 && ch <= 96+6)  /* 'a'--'f' */
    return ch - (96+1) + 10;
  return 255;
}


/* Tries to handle the given character and possibly produce a token.
 * Sets (*token) if a token is produced, and leaves it unmodified otherwise.
 *
 * Returns PDF_OK if the character was accepted. Otherwise, an error code
 * is returned, and the call can be repeated later with the same ch value.
 * A token may be produced even if the character isn't accepted.
 */
static pdf_status_t
handle_char (pdf_token_reader_t reader, pdf_u32_t flags,
             pdf_char_t ch, pdf_token_t *token)
{
  pdf_status_t rv;

  /* first, handle the states that shouldn't be exited when whitespace
   * or a delimiter is seen */

  switch (reader->state)
    {
    case PDF_TOKR_STATE_EOF:
      return PDF_EEOF;

    case PDF_TOKR_STATE_STRING:
      return handle_string_char (reader, flags, ch, token);

    case PDF_TOKR_STATE_HEXSTRING:
      return handle_hexstring_char (reader, flags, ch, token);

    case PDF_TOKR_STATE_DICTEND:
      if (ch != 62)  /* '>' */
        return PDF_EBADFILE;
      reader->substate = 1;  /* saw the closing '>' */
      return exit_state (reader, flags, token);

    case PDF_TOKR_STATE_COMMENT:
      if (pdf_is_eol_char (ch))
        {
          rv = exit_state (reader, flags, token);
          if (rv != PDF_OK)
            return rv;

          /* don't accept this character, but process it next time */
          return PDF_EAGAIN;
        }

      if (!(flags & PDF_TOKEN_RET_COMMENTS))
        reader->substate = 1;
      if (reader->substate == 1)
        return PDF_OK;  /* we don't care about this comment */

      return store_char_grow (reader, ch);

    default: ;
    }

  /* now handle delimiters and whitespace */

  if (pdf_is_wspace_char (ch))
    {
      if (reader->state)
        {
          rv = exit_state (reader, flags, token);
          if (rv != PDF_OK)
            return rv;

          /* avoid reading this byte so PDF_TOKEN_END_AT_STREAM
           * will work properly if it's '\r' */
          return PDF_EAGAIN;
        }

      if ((flags & PDF_TOKEN_END_AT_STREAM) && ch == 10)  /* LF */
        {
          /* found the beginning of a stream */
          enter_state (reader, PDF_TOKR_STATE_EOF);
        }
      return PDF_OK;
    }
  else if ((flags & PDF_TOKEN_END_AT_STREAM) && ch != 37)  /* 37=='%' */
    {
      /* only allow whitespace/comments after the "stream" keyword */
      return PDF_EBADFILE;
    }

  if (pdf_is_delim_char (ch))
    {
      /* set state 0 (UNINIT), substate 0, bufpos 0 */
      if (reader->state)
        {
          rv = exit_state (reader, flags, token);
          if (rv != PDF_OK)
            return rv;
          return PDF_EAGAIN;
        }

      switch (ch)
        {
        case 37:  /* '%' */
          enter_state (reader, PDF_TOKR_STATE_COMMENT);
          return PDF_OK;
        case 40:  /* '(' */
          enter_state (reader, PDF_TOKR_STATE_STRING);
          reader->intparam = 0;
          return PDF_OK;
        case 41:  /* ')' */
          /* this shouldn't occur outside the STRING and COMMENT states */
          return PDF_EBADFILE;
        case 47:  /* '/' */
          enter_state (reader, PDF_TOKR_STATE_NAME);
          return PDF_OK;
        case 60:  /* '<' */
          enter_state (reader, PDF_TOKR_STATE_HEXSTRING);
          return PDF_OK;
        case 62:  /* '>' */
          enter_state (reader, PDF_TOKR_STATE_DICTEND);
          return PDF_OK;
        case 91:  /* '[' */
          /* fall through */
        case 93:  /* ']' */
          /* fall through */
        case 123: /* '{' */
          /* fall through */
        case 125: /* '}' */
          /* exit_state may have emitted a token, so we can't emit another
           * one now; we'll do it when exiting the PENDING state */
          enter_state (reader, PDF_TOKR_STATE_PENDING);
          reader->charparam = ch;
          return PDF_OK;
        }

      /* not reached (all delimiter chars should be handled) */
      assert (0);
    }

  /* ch is a regular character */

  switch (reader->state)
    {
    case PDF_TOKR_STATE_PENDING:
      rv = exit_state (reader, flags, token);
      if (rv != PDF_OK)
        return rv;
      return PDF_EAGAIN;

    case PDF_TOKR_STATE_NONE:
      enter_state (reader, PDF_TOKR_STATE_KEYWORD);
      /* fall through */

    case PDF_TOKR_STATE_KEYWORD:
      /* Note: numbers are treated as keywords until flush_token is called. */
      return store_char (reader, ch);

    case PDF_TOKR_STATE_NAME:
      if (reader->substate == 0)
        {
          if ((ch < 0x21) || (ch > 0x7e))
            {
              /* Invalid character in a name.  */
              return PDF_EBADFILE;
            }

          if (ch != 35  /* '#' */
              || (flags & PDF_TOKEN_NO_NAME_ESCAPES) )
            return store_char (reader, ch);

          reader->substate = 1;
          return PDF_OK;
        }

      if ( (ch = hexval (ch)) >= 16 )
        return PDF_EBADFILE;

      if (reader->substate == 1)  /* the first hex digit of an escape */
        {
          reader->substate = 2;
          reader->charparam = ch;
          return PDF_OK;
        }

      ch = (reader->charparam << 4) | ch;
      if (ch == 0)  /* the PDF spec forbids "#00" */
        return PDF_EBADFILE;

      rv = store_char (reader, ch);
      if (rv == PDF_OK) reader->substate = 0;
      return rv;

    default:
      assert (0);
      return PDF_ERROR;
  }
}


static INLINE int
can_store_char (const pdf_token_reader_t reader)
{
  return reader->buffer->wp < reader->buffer->size;
}

static pdf_status_t
enlarge_buffer (pdf_token_reader_t reader)
{
  pdf_size_t size = reader->buffer->size, newsize = size * 2;
  if (newsize < size)
    return PDF_EIMPLLIMIT;

  return pdf_buffer_resize (reader->buffer, newsize);
}

static INLINE pdf_status_t
store_char (pdf_token_reader_t reader, pdf_char_t ch)
{
  if (!can_store_char (reader))
    return PDF_EIMPLLIMIT;
  reader->buffer->data[reader->buffer->wp++] = ch;
  return PDF_OK;
}

static INLINE pdf_status_t
store_char_grow (pdf_token_reader_t reader, pdf_char_t ch)
{
  if (!can_store_char (reader))
    {
      pdf_status_t rv = enlarge_buffer(reader);
      if (rv != PDF_OK)
        return rv;
    }
  reader->buffer->data[reader->buffer->wp++] = ch;
  return PDF_OK;
}

static INLINE pdf_status_t
enter_state (pdf_token_reader_t reader,
             enum pdf_token_reader_state_e state)
{
  reader->state = state;
  reader->state_pos = pdf_stm_tell (reader->stream);

  return PDF_OK;
}

static pdf_status_t
flush_token (pdf_token_reader_t reader, pdf_u32_t flags, pdf_token_t *token)
{
  pdf_status_t rv;
  pdf_token_t new_tok;
  pdf_char_t *data = reader->buffer->data;
  int datasize = reader->buffer->wp;

  switch (reader->state)
    {
    case PDF_TOKR_STATE_NONE:
      return PDF_OK;  /* no state to exit */

    case PDF_TOKR_STATE_EOF:
      return PDF_EEOF;  /* can't continue parsing after EOF */

    case PDF_TOKR_STATE_COMMENT:
      if ((reader->substate == 1) || !(flags & PDF_TOKEN_RET_COMMENTS))
        goto finish;  /* don't return a token */

      rv = pdf_token_comment_new (data, datasize, &new_tok);
      break;

    case PDF_TOKR_STATE_KEYWORD:
      {
        int value;
        int ntyp = recognise_number (reader->buffer, &value);
        if (ntyp == 1)
          rv = pdf_token_integer_new (value, &new_tok);
        else if (ntyp == 2)
          {
            double realvalue;
            rv = parse_real (reader->buffer,
                             reader->decimal_point,
                             &realvalue);
            if (rv != PDF_OK)
              return rv;
            rv = pdf_token_real_new ((float)realvalue, &new_tok);
          }
        else
          rv = pdf_token_keyword_new (data, datasize, &new_tok);
      }
      break;

    case PDF_TOKR_STATE_NAME:
      if (reader->substate != 0)  /* reading an escape sequence */
        return PDF_EBADFILE;

      rv = pdf_token_name_new (data, datasize, &new_tok);
      break;

    case PDF_TOKR_STATE_STRING:
      if (reader->intparam >= 0)  /* didn't see the closing ')' */
        return PDF_EBADFILE;

      rv = pdf_token_string_new (data, datasize, &new_tok);
      break;

    case PDF_TOKR_STATE_HEXSTRING:
      if (reader->substate != 3)  /* didn't see the closing '>' */
        return PDF_EBADFILE;

      rv = pdf_token_string_new (data, datasize, &new_tok);
      break;

    case PDF_TOKR_STATE_DICTEND:
      if (reader->substate != 1)  /* didn't see a second '>' */
        return PDF_EBADFILE;

      rv = pdf_token_valueless_new (PDF_TOKEN_DICT_END, &new_tok);
      break;

    case PDF_TOKR_STATE_PENDING:
      switch (reader->charparam)
        {
        case 60:  /* '<' */
          rv = pdf_token_valueless_new (PDF_TOKEN_DICT_START, &new_tok);
          break;
        case 91:  /* '[' */
          rv = pdf_token_valueless_new (PDF_TOKEN_ARRAY_START, &new_tok);
          break;
        case 93:  /* ']' */
          rv = pdf_token_valueless_new (PDF_TOKEN_ARRAY_END, &new_tok);
          break;
        case 123: /* '{' */
          rv = pdf_token_valueless_new (PDF_TOKEN_PROC_START, &new_tok);
          break;
        case 125: /* '}' */
          rv = pdf_token_valueless_new (PDF_TOKEN_PROC_END, &new_tok);
          break;
        default:
          assert (0);
          return PDF_ERROR;
        }
      break;

    default:
      assert (0);
      return PDF_ERROR;
    }

  if (rv != PDF_OK)
    return rv;

  *token = new_tok;

  /* Set the beginning position of this state */
  reader->beg_pos = reader->state_pos;

finish:
  reset_buffer (reader);
  return PDF_OK;
}


static pdf_status_t
exit_state (pdf_token_reader_t reader, pdf_u32_t flags, pdf_token_t *token)
{
  pdf_status_t rv = flush_token (reader, flags, token);
  if (rv == PDF_OK)
    {
      reader->state = PDF_TOKR_STATE_NONE;
      reader->substate = 0;
    }
  return rv;
}


static INLINE pdf_status_t
handle_string_char (pdf_token_reader_t reader,
                    pdf_u32_t flags,
                    pdf_char_t ch,
                    pdf_token_t *token)
{
  pdf_status_t rv;
start:
  switch (reader->substate)
    {
      case 1:  /* ignore LF */
        reader->substate = 0;
        if (ch == 10)
          return PDF_OK;
        /* fall through */

      case 0:  /* no special state */
        {
          if (ch == 92)  /* '\\' */
            {
              reader->substate = 2;  /* start an escape sequence */
              return PDF_OK;
            }
          else if (ch == 41 && reader->intparam <= 0)  /* ')'; end of string */
            {
              reader->intparam = -1;
              return exit_state (reader, flags, token);
            }

          pdf_bool_t wasCR = (ch == 13);
          if (wasCR)
            ch = 10;  /* treat as LF */
          rv = store_char_grow (reader, ch);

          if (rv == PDF_OK)
            {
              if (wasCR)  /* '\r' */
                reader->substate = 1;  /* ignore the next char if it's LF */
              else if (ch == 40)  /* '(' */
                ++reader->intparam;
              else if (ch == 41)  /* ')' */
                --reader->intparam;
            }

          return rv;
        }

      case 2:  /* just saw a '\\' (starting an escape sequence) */
        reader->substate = 0;
        if (ch == 98)  /* 'b' */
          ch = 8;  /* BS: backspace */
        else if (ch == 102)  /* 'f' */
          ch = 12;  /* FF: formfeed */
        else if (ch == 110)  /* 'n' */
          ch = 10;  /* NL: newline */
        else if (ch == 114)  /* 'r' */
          ch = 13;  /* CR: carriage return */
        else if (ch == 116)  /* 't' */
          ch = 9;  /* HT: horizontal tab */
        else if (ch == 10)  /* NL */
          return PDF_OK;  /* ignore the line break */
        else if (ch == 13)  /* CR */
        {
          /* ignore the line break; also ignore the next byte if it's LF */
          reader->substate = 1;
          return PDF_OK;
        }
        else if (ch >= 48 && ch <= 48+7)  /* digits '0'--'7' */
          {
            /* starting an octal escape; we'll read three digits even if the
             * first is '4'--'7' (and calculate the final char modulo 256),
             * since the PDF/PS specs say to ignore high-order overflow */
            reader->substate = 3;
            reader->charparam = (ch-48);
            return PDF_OK;
          }

        /* for any other character, including '(', ')', and '\\',
         * store the same character (dropping the leading backslash) */
        return store_char_grow (reader, ch);

      case 3:  /* saw 1 digit of an octal escape */
        /* fall through */
      case 4:  /* saw 2 digits of an octal escape */
        if (ch < 48 || ch > 48+7)  /* not digits '0'--'7' */
          {
            rv = store_char_grow (reader, reader->charparam);
            if (rv != PDF_OK) return rv;

            /* ch isn't part of the escape sequence, so retry */
            reader->substate = 0;
            goto start;
          }

        /* ch is a digit from '0'--'7' */
        reader->charparam = ((reader->charparam & 0x1f) << 3) | (ch - 48);
        if (reader->substate == 4)  /* this was the final digit */
          {
            rv = store_char_grow (reader, reader->charparam);
            if (rv != PDF_OK) return rv;

            reader->substate = 0;
            return PDF_OK;
          }

        reader->substate = 4;
        return PDF_OK;

      default:
        assert (0);
    }
}


static INLINE pdf_status_t
handle_hexstring_char (pdf_token_reader_t reader,
                       pdf_u32_t flags,
                       pdf_char_t ch,
                       pdf_token_t *token)
{
  pdf_status_t rv;

  if (reader->substate == 0)
    {
      /* this is the first character after the initial '<' */
      if (ch == 60)  /* '<' */
        {
          /* this was actually the start of a dictionary */
          reader->state = PDF_TOKR_STATE_PENDING;
          reader->charparam = ch;
          return exit_state (reader, flags, token);
        }

      reader->substate = 1;
    }

  if (pdf_is_wspace_char (ch))
    return PDF_OK;

  if (ch == 62)  /* '>': end of hex string */
    {
      if (reader->substate == 2)
        {
          /* the last digit is missing; assume it's '0' */
          rv = store_char_grow (reader, reader->charparam << 4);
          if (rv != PDF_OK) return rv;
        }

      reader->substate = 3;  /* saw end of string */
      return exit_state (reader, flags, token);
    }

  if ( (ch = hexval (ch)) == 255 )
    return PDF_EBADFILE;

  if (reader->substate == 1)  /* first character in a pair */
    {
      reader->substate = 2;
      reader->charparam = ch;
      return PDF_OK;
    }

  rv = store_char_grow (reader, (reader->charparam << 4) | ch);
  if (rv == PDF_OK)
    reader->substate = 1;
  return rv;
}

pdf_status_t
pdf_token_read (pdf_token_reader_t reader, pdf_u32_t flags, pdf_token_t *token)
{
  pdf_status_t rv;
  pdf_char_t ch;
  pdf_token_t new_token = NULL;

  if (!reader || !reader->stream || !token)
    return PDF_EBADDATA;

  while ( (rv = pdf_stm_peek_char (reader->stream, &ch)) == PDF_OK )
    {
      rv = handle_char (reader, flags, ch, &new_token);
      if (rv == PDF_OK)
        {
          /* The character we peeked at was accepted, so get rid of it. */
          pdf_stm_read_char (reader->stream, &ch);
        }

      if (new_token)
        {
          /* Don't return an error code if we got a valid token.
           * We'll probably see the same error on the next call since we
           * didn't call read_char. */
          assert (rv == PDF_OK || rv == PDF_EAGAIN);
          goto ret_token;
        }
      else if (rv != PDF_OK && rv != PDF_EAGAIN)
        return rv;
    }

  /* peek_char returned an error code (rv) */
  if (rv != PDF_EEOF)
    return rv;

  rv = exit_state (reader, flags, &new_token);
  if (rv != PDF_OK)
    return rv;

  reader->state = PDF_TOKR_STATE_EOF;
  if (new_token)
    goto ret_token;
  else
    return PDF_EEOF;

ret_token:
  assert (new_token);
  *token = new_token;
  return PDF_OK;
}

pdf_size_t
pdf_token_reader_begin_pos (pdf_token_reader_t reader)
{
  return reader->beg_pos;
}

static INLINE int
parse_integer (pdf_buffer_t buffer, int *int_value, int *int_state)
{
  /* Parse an ASCII integer with the given radix, at the beginning of
   * the buffer (possibly leaving unread bytes at the end).
   *
   * Return value is 0 on failure, or a bitmask otherwise:
   *  1 = valid integer
   *  2 = signed
   *  4 = overflowed (no value stored in *int_value)
   */

  int sign = 0, tmpint = 0, overflowed = 0, ret;
  /* Integer states (int_state):
   *   0 = at start (looking for sign or digits)
   *   1 = saw sign
   *   2 = saw digits
   */

  *int_state = 0;
  for (; buffer->rp < buffer->wp; ++buffer->rp)
    {
      int chval;
      pdf_char_t ch = buffer->data[buffer->rp];
      if (ch == 43 || ch == 45)  /* '+','-' */
        {
          if (*int_state != 0)
            goto out;

          *int_state = 1;
          sign = (ch == 43) ? 1 : -1;
          continue;
        }

      chval = ch - 48;  /* assume this is a digit */
      if (chval < 0 || chval > 9)
        goto out;  /* not a valid number */

      *int_state = 2;
      if (overflowed)
        continue;

      /* convert the digits to an integer, if possible */
      if (sign < 0)
        {
          chval = -chval;
          if ( tmpint < (INT_MIN/10)
              || (tmpint == (INT_MIN/10) && chval < (INT_MIN%10)) )
            {
              overflowed = 1;  /* would overflow */
              continue;
            }
        }
      else
        {
          if ( tmpint > (INT_MAX/10)
              || (tmpint == (INT_MAX/10) && chval > (INT_MAX%10)) )
            {
              overflowed = 1;  /* would overflow */
              continue;
            }
        }

      tmpint += chval + (tmpint * 9);
    }

out:
  if (*int_state != 2)
    return 0;  /* never saw any digits */

  ret = 1;
  if (sign) ret += 2;
  if (overflowed)
    ret += 4;
  else
    *int_value = tmpint;

  return ret;
}


static INLINE int
validate_real (pdf_buffer_t buffer, int int_state)
{
  /* Determines whether the given number is a valid PS/PDF real number;
   * assumes the initial sign was already read (if present), and any data
   * before buffer->rp is a valid integer.
   *
   * Return value:
   *   0 = not a real number
   *   1 = valid PDF/PS real
   */

  int seen_point = 0;
  /* Integer states (int_state):
   *   0 = at start
   *   1 = saw sign
   *   2 = saw digits
   */

  for (; buffer->rp < buffer->wp; ++buffer->rp)
    {
      pdf_char_t ch = buffer->data[buffer->rp];
      if (ch == 46)  /* '.' */
        {
          if (!seen_point)
            seen_point = 1;
          else
            return 0;
        }
      else if (ch == 43 || ch == 45)  /* '+','-' */
        {
          if (int_state == 0)
            int_state = 1;
          else
            return 0;
        }
      else if (ch >= 48+0 && ch <= 48+9)
        int_state = 2;
      else
        return 0;
    }

  return (int_state == 2);  /* only valid if we saw a digit */
}


/* Given a buffer containing a validated PDF real (in ASCII), convert it to a
 * double by translating it to the execution character set, replacing '.' with
 * the locale's decimal point, and calling strtod. */
static INLINE pdf_status_t
parse_real (pdf_buffer_t buffer, char *locale_dec_pt, double *value)
{
  pdf_status_t ret;
  size_t tmplen, wpos, ptlen;
  char *tmp, *endptr;

  ptlen = strlen (locale_dec_pt);
  /* we may remove 1 byte ('.') and replace it with ptlen bytes */
  tmplen = buffer->wp - 1 + ptlen;

  tmp = pdf_alloc (tmplen + 1);
  if (!tmp)
    return PDF_ENOMEM;

  wpos = 0;
  ret = PDF_ERROR;  /* nothing should fail if the buffer was validated */
  for (buffer->rp = 0; buffer->rp < buffer->wp; ++buffer->rp)
    {
      pdf_char_t ch = buffer->data[buffer->rp];
      if (wpos >= tmplen)
        goto out;

      if (ch == 46)  /* '.' */
        {
          if (wpos + ptlen > tmplen)
            goto out;

          memcpy (tmp + wpos, locale_dec_pt, ptlen);
          wpos += ptlen;
        }
      else if (ch == 43)  /* '+' */
        tmp[wpos++] = '+';
      else if (ch == 45)  /* '-' */
        tmp[wpos++] = '-';
      else if (ch >= 48+0 && ch <= 48+9)  /* '0'--'9' */
        tmp[wpos++] = '0' + (ch-48);
      else
        goto out;
    }

  /* null-terminate the new string, and call strtod to get its value
   * (strtof would also work if it's available) */
  tmp[wpos] = '\0';
  *value = strtod (tmp, &endptr);
  if (endptr == tmp + wpos)
    ret = PDF_OK;

out:
  pdf_dealloc (tmp);
  return ret;
}


/*
 * Return value:
 *   0 = not a number
 *   1 = integer (stored in *int_value)
 *   2 = real
 */
static int
recognise_number (pdf_buffer_t buffer, int *int_value)
{
  int rv, tmpint = 0, int_state = 0;

  /* try to parse as an integer */

  buffer->rp = 0;
  rv = parse_integer (buffer, &tmpint, &int_state);

  if (buffer->rp < buffer->wp)  /* didn't look at the whole buffer */
    {
      rv = validate_real (buffer, int_state);
      if (rv == 1)
        return 2;

      return 0;
    }

  if (!rv) return 0;
  else if (rv & 4)
    return 2;  /* integer overflowed, but could be read as a real */

  *int_value = tmpint;
  return 1;
}


/* End of pdf-token-reader.c */

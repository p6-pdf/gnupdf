/* -*- mode: C -*- Time-stamp: "2009-01-05 08:53:24 mgold"
 *
 *       File:         pdf-rd-tokeniser.c
 *       Date:         Mon Dec 29 00:45:09 2008
 *
 *       GNU PDF Library - Stream tokeniser
 *
 */

/* Copyright (C) 2008 Michael Gold */

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

#include "config.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "pdf-rd-tokeniser.h"

static INLINE int can_store_char (pdf_tokeniser_t context);
static INLINE pdf_status_t store_char (pdf_tokeniser_t context, pdf_char_t ch);
static pdf_status_t exit_state (pdf_tokeniser_t context, pdf_obj_t *token);
static pdf_status_t flush_token (pdf_tokeniser_t context, pdf_obj_t *token);
static pdf_status_t handle_char (pdf_tokeniser_t context,
                                 pdf_char_t ch,
                                 pdf_obj_t *token);
static INLINE pdf_status_t handle_string_char (pdf_tokeniser_t context,
                                               pdf_char_t ch,
                                               pdf_obj_t *token);
static INLINE pdf_status_t handle_hexstring_char (pdf_tokeniser_t context,
                                                  pdf_char_t ch,
                                                  pdf_obj_t *token);
static int recognize_number (pdf_buffer_t buffer, int *int_value);
static INLINE int parse_integer (pdf_buffer_t buffer, int *int_value,
                                 int *int_state);
static INLINE pdf_status_t parse_real (pdf_buffer_t buffer,
                                       char *locale_dec_pt,
                                       double *value);
static INLINE int validate_real (pdf_buffer_t buffer, int int_state);


pdf_status_t
pdf_tokeniser_new (pdf_stm_t stm, pdf_tokeniser_t *context)
{
  pdf_status_t err;
  pdf_tokeniser_t new_tokr;

  err = PDF_ENOMEM;
  new_tokr = pdf_alloc (sizeof (*new_tokr));
  if (!new_tokr)
    goto fail;

  /* determine the current locale's decimal point
   * (avoid using localeconv since it may not be thread-safe) */
  new_tokr->decimal_point = NULL;
  {
    int len;
    char decpt[16];

    err = PDF_ERROR;
    len = snprintf (decpt, sizeof (decpt), "%#.0f", 1.0);
    if (len <= 0 || len >= sizeof (decpt))  /* shouldn't happen */
      goto fail;

    err = PDF_ENOMEM;
    new_tokr->decimal_point = pdf_alloc (len);
    if (!new_tokr->decimal_point)
      goto fail;

    /* this copies the trailing '\0' due to the starting offset */
    memcpy (new_tokr->decimal_point, &decpt[1], len);
  }

  /* max string size 32767 + terminating '\0' */
  new_tokr->buffer = pdf_buffer_new (32768);
  if (!new_tokr->buffer)
    goto fail;

  new_tokr->flags = 0;
  new_tokr->stream = stm;
  pdf_tokeniser_reset_state (new_tokr);

  *context = new_tokr;
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

pdf_status_t
pdf_tokeniser_reset_state (pdf_tokeniser_t context)
{
  context->state = PDF_TOKENISER_STATE_NONE;
  context->substate = 0;
  context->findstream = 0;
  return PDF_OK;
}

pdf_status_t
pdf_tokeniser_destroy (pdf_tokeniser_t context)
{
  if (!context) return PDF_EBADDATA;

  assert (context->buffer);
  if (context->buffer)
    pdf_buffer_destroy (context->buffer);
  pdf_dealloc (context->decimal_point);
  pdf_dealloc (context);

  return PDF_OK;
}

static INLINE int
pdf_is_wspace_char (pdf_char_t ch)
{
  /* ASCII codes for NUL, HT, LF, FF, CR, SP */
  return (ch == 0 || ch == 9 || ch == 10 || ch == 12 || ch == 13 || ch == 32);
}

static INLINE int
pdf_is_delim_char (pdf_char_t ch)
{
  /* ASCII codes for '%', '(', ')', '/'; '<', '>', '[', ']'; '{', '}' */
  return (ch == 37 || ch == 40 || ch == 41 || ch == 47
       || ch == 60 || ch == 62 || ch == 91 || ch == 93
       || ch == 123 || ch == 125);
}

static INLINE int
pdf_is_eol_char (pdf_char_t ch)
{
  return ch == 10 || ch == 13;
}

static INLINE int
pdf_is_regular_char (pdf_char_t ch)
{
  return !pdf_is_wspace_char (ch) && !pdf_is_delim_char (ch);
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
handle_char (pdf_tokeniser_t context, pdf_char_t ch, pdf_obj_t *token)
{
  pdf_status_t rv;

  /* first, handle the states that shouldn't be exited when whitespace
   * or a delimiter is seen */

  switch (context->state)
    {
    case PDF_TOKENISER_STATE_EOF:
      return PDF_EEOF;

    case PDF_TOKENISER_STATE_STRING:
      return handle_string_char (context, ch, token);

    case PDF_TOKENISER_STATE_HEXSTRING:
      return handle_hexstring_char (context, ch, token);

    case PDF_TOKENISER_STATE_DICTEND:
      if (ch != 62)  /* '>' */
        return PDF_EBADFILE;
      context->substate = 1;  /* saw the closing '>' */
      return exit_state (context, token);

    case PDF_TOKENISER_STATE_COMMENT:
      if (pdf_is_eol_char (ch))
        {
          rv = exit_state (context, token);
          if (rv != PDF_OK)
            return rv;

          /* don't accept this character, but process it next time */
          return PDF_EAGAIN;
        }

      if (store_char (context, ch) != PDF_OK)
        {
          /* the comment buffer is full, so split the token */
          rv = flush_token (context, token);
          if (rv != PDF_OK)
            return rv;

          context->intparam = 1;  /* mark the next token as a continuation */
          return store_char (context, ch);  /* can't fail this time */
        }

      return PDF_OK;

    default: ;
    }

  /* now handle delimiters and whitespace */

  if (pdf_is_wspace_char (ch))
    {
      if (context->state)
        {
          rv = exit_state (context, token);
          if (rv != PDF_OK)
            return rv;

          /* avoid reading this byte so pdf_tokeniser_end_at_stream
           * will work properly if it's '\r' */
          return PDF_EAGAIN;
        }

      if (context->findstream && ch == 10)  /* LF */
        {
          /* found the beginning of a stream */
          context->state = PDF_TOKENISER_STATE_EOF;
          context->findstream = 0;
        }
      return PDF_OK;
    }
  else if (context->findstream && ch != 37)  /* 37=='%' */
    {
      /* only allow whitespace/comments after the "stream" keyword */
      return PDF_EBADFILE;
    }

  if (pdf_is_delim_char (ch))
    {
      /* set state 0 (UNINIT), substate 0, bufpos 0 */
      if (context->state)
        {
          rv = exit_state (context, token);
          if (rv != PDF_OK)
            return rv;
          return PDF_EAGAIN;
        }

      switch (ch)
        {
        case 37:  /* '%' */
          context->state = PDF_TOKENISER_STATE_COMMENT;
          context->intparam = 0;
          return store_char (context, ch);
        case 40:  /* '(' */
          context->state = PDF_TOKENISER_STATE_STRING;
          context->intparam = 0;
          return PDF_OK;
        case 41:  /* ')' */
          /* this shouldn't occur outside the STRING and COMMENT states */
          return PDF_EBADFILE;
        case 47:  /* '/' */
          context->state = PDF_TOKENISER_STATE_NAME;
          return PDF_OK;
        case 60:  /* '<' */
          context->state = PDF_TOKENISER_STATE_HEXSTRING;
          return PDF_OK;
        case 62:  /* '>' */
          context->state = PDF_TOKENISER_STATE_DICTEND;
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
          context->state = PDF_TOKENISER_STATE_PENDING;
          context->charparam = ch;
          return PDF_OK;
        }

      /* not reached (all delimiter chars should be handled) */
      assert (0);
    }

  /* ch is a regular character */

  switch (context->state)
    {
    case PDF_TOKENISER_STATE_PENDING:
      rv = exit_state (context, token);
      if (rv != PDF_OK)
        return rv;
      return PDF_EAGAIN;

    case PDF_TOKENISER_STATE_NONE:
      context->state = PDF_TOKENISER_STATE_KEYWORD;
      /* fall through */

    case PDF_TOKENISER_STATE_KEYWORD:
      return store_char (context, ch);

    case PDF_TOKENISER_STATE_NAME:
      if (context->substate == 0)
        {
          if (ch != 35  /* '#' */
              || (context->flags & PDF_TOKENISER_FLAG_PDF11) )
            return store_char (context, ch);

          context->substate = 1;
          return PDF_OK;
        }

      if ( (ch = hexval (ch)) >= 16 )
        return PDF_EBADFILE;

      if (context->substate == 1)  /* the first hex digit of an escape */
        {
          context->substate = 2;
          context->charparam = ch;
          return PDF_OK;
        }

      ch = (context->charparam << 4) | ch;
      if (ch == 0)  /* the PDF spec forbids "#00" */
        return PDF_EBADFILE;

      rv = store_char (context, ch);
      if (rv == PDF_OK) context->substate = 0;
      return rv;

    default:
      assert (0);
  }

  return store_char (context, ch);
}


static INLINE int
can_store_char (const pdf_tokeniser_t context)
{
  return context->buffer->wp < (context->buffer->size - 1);
}

static INLINE pdf_status_t
store_char (pdf_tokeniser_t context, pdf_char_t ch)
{
  if (!can_store_char (context))
    return PDF_EIMPLLIMIT;
  context->buffer->data[context->buffer->wp++] = ch;
  return PDF_OK;
}


static pdf_status_t
flush_token (pdf_tokeniser_t context, pdf_obj_t *token)
{
  pdf_status_t rv;
  pdf_obj_t obj;
  pdf_char_t *data = context->buffer->data;
  int datasize = context->buffer->wp;

  switch (context->state)
    {
    case PDF_TOKENISER_STATE_NONE:
      return PDF_OK;  /* no state to exit */

    case PDF_TOKENISER_STATE_EOF:
      return PDF_EEOF;  /* can't continue parsing after EOF */

    case PDF_TOKENISER_STATE_COMMENT:
      if (!(context->flags & PDF_TOKENISER_FLAG_RET_COMMENTS))
        goto finish;

      rv = pdf_tok_comment_new (data, datasize,
          context->intparam /*continuation*/, &obj);
      break;

    case PDF_TOKENISER_STATE_KEYWORD:
      {
        int value;
        int ntyp = recognize_number (context->buffer, &value);
        if (ntyp == 1)
          rv = pdf_obj_integer_new (value, &obj);
        else if (ntyp == 2)
          {
            double realvalue;
            rv = parse_real (context->buffer,
                             context->decimal_point,
                             &realvalue);
            if (rv != PDF_OK)
              return rv;
            rv = pdf_obj_real_new ((float)realvalue, &obj);
          }
        else
          rv = pdf_tok_keyword_new (data, datasize, &obj);
      }
      break;

    case PDF_TOKENISER_STATE_NAME:
      if (context->substate != 0)  /* reading an escape sequence */
        return PDF_EBADFILE;

      rv = pdf_obj_name_new (data, datasize, &obj);
      break;

    case PDF_TOKENISER_STATE_STRING:
      if (context->intparam >= 0)  /* didn't see the closing ')' */
        return PDF_EBADFILE;

      rv = pdf_obj_string_new (data, datasize, &obj);
      break;

    case PDF_TOKENISER_STATE_HEXSTRING:
      if (context->substate != 3)  /* didn't see the closing '>' */
        return PDF_EBADFILE;

      rv = pdf_obj_string_new (data, datasize, &obj);
      break;

    case PDF_TOKENISER_STATE_DICTEND:
      if (context->substate != 1)  /* didn't see a second '>' */
        return PDF_EBADFILE;

      rv = pdf_tok_valueless_new (PDF_DICT_END_TOK, &obj);
      break;

    case PDF_TOKENISER_STATE_PENDING:
      switch (context->charparam)
        {
        case 60:  /* '<' */
          rv = pdf_tok_valueless_new (PDF_DICT_START_TOK, &obj);
          break;
        case 91:  /* '[' */
          rv = pdf_tok_valueless_new (PDF_ARRAY_START_TOK, &obj);
          break;
        case 93:  /* ']' */
          rv = pdf_tok_valueless_new (PDF_ARRAY_END_TOK, &obj);
          break;
        case 123: /* '{' */
          rv = pdf_tok_valueless_new (PDF_PROC_START_TOK, &obj);
          break;
        case 125: /* '}' */
          rv = pdf_tok_valueless_new (PDF_PROC_END_TOK, &obj);
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

  *token = obj;
finish:
  context->buffer->wp = 0;
  return PDF_OK;
}


static pdf_status_t
exit_state (pdf_tokeniser_t context, pdf_obj_t *token)
{
  pdf_status_t rv = flush_token (context, token);
  if (rv == PDF_OK)
    {
      context->state = PDF_TOKENISER_STATE_NONE;
      context->substate = 0;
    }
  return rv;
}


static INLINE pdf_status_t
handle_string_char (pdf_tokeniser_t context,
                    pdf_char_t ch,
                    pdf_obj_t *token)
{
  pdf_status_t rv;
start:
  switch (context->substate)
    {
      case 1:  /* ignore LF */
        context->substate = 0;
        if (ch == 10)
          return PDF_OK;
        /* fall through */

      case 0:  /* no special state */
        if (ch == 92)  /* '\\' */
          {
            context->substate = 2;  /* start an escape sequence */
            return PDF_OK;
          }
        else if (ch == 41 && context->intparam <= 0)  /* ')'; end of string */
          {
            context->intparam = -1;
            return exit_state (context, token);
          }

        if (!can_store_char (context))
          return PDF_EIMPLLIMIT;
        else if (ch == 40)  /* '(' */
          ++context->intparam;
        else if (ch == 41)  /* ')' */
          --context->intparam;
        else if (ch == 13)  /* '\r' */
          {
            ch = 10;  /* treat as LF */
            context->substate = 1;  /* ignore the next char if it's LF */
          }

        return store_char (context, ch);

      case 2:  /* just saw a '\\' (starting an escape sequence) */
        context->substate = 0;
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
          context->substate = 1;
          return PDF_OK;
        }
        else if (ch >= 48 && ch <= 48+7)  /* digits '0'--'7' */
          {
            /* starting an octal escape; we'll read three digits even if the
             * first is '4'--'7' (and calculate the final char modulo 256),
             * since the PDF/PS specs say to ignore high-order overflow */
            context->substate = 3;
            context->charparam = (ch-48);
            return PDF_OK;
          }

        /* for any other character, including '(', ')', and '\\',
         * store the same character (dropping the leading backslash) */
        return store_char (context, ch);

      case 3:  /* saw 1 digit of an octal escape */
        /* fall through */
      case 4:  /* saw 2 digits of an octal escape */
        if (ch < 48 || ch > 48+7)  /* not digits '0'--'7' */
          {
            rv = store_char (context, context->charparam);
            if (rv != PDF_OK) return rv;

            /* ch isn't part of the escape sequence, so retry */
            context->substate = 0;
            goto start;
          }

        /* ch is a digit from '0'--'7' */
        context->charparam = ((context->charparam & 0x1f) << 3) | (ch - 48);
        if (context->substate == 4)  /* this was the final digit */
          {
            rv = store_char (context, context->charparam);
            if (rv != PDF_OK) return rv;

            context->substate = 0;
            return PDF_OK;
          }

        context->substate = 4;
        return PDF_OK;

      default:
        assert (0);
    }
}


static INLINE pdf_status_t
handle_hexstring_char (pdf_tokeniser_t context,
                       pdf_char_t ch,
                       pdf_obj_t *token)
{
  pdf_status_t rv;

  if (context->substate == 0)
    {
      /* this is the first character after the initial '<' */
      if (ch == 60)  /* '<' */
        {
          /* this was actually the start of a dictionary */
          context->state = PDF_TOKENISER_STATE_PENDING;
          context->charparam = ch;
          return exit_state (context, token);
        }

      context->substate = 1;
    }

  if (pdf_is_wspace_char (ch))
    return PDF_OK;

  if (ch == 62)  /* '>': end of hex string */
    {
      if (context->substate == 2)
        {
          /* the last digit is missing; assume it's '0' */
          rv = store_char (context, context->charparam << 4);
          if (rv != PDF_OK) return rv;
        }

      context->substate = 3;  /* saw end of string */
      return exit_state (context, token);
    }

  if ( (ch = hexval (ch)) == 255 )
    return PDF_EBADFILE;

  if (context->substate == 1)  /* first character in a pair */
    {
      context->substate = 2;
      context->charparam = ch;
      return PDF_OK;
    }

  rv = store_char (context, (context->charparam << 4) | ch);
  if (rv == PDF_OK)
    context->substate = 1;
  return rv;
}

pdf_status_t
pdf_tokeniser_read (pdf_tokeniser_t context, pdf_obj_t *token)
{
  pdf_status_t rv;
  pdf_char_t ch;
  pdf_obj_t new_token = NULL;

  if (!context || !context->stream || !token)
    return PDF_EBADDATA;

  while ( (rv = pdf_stm_peek_char (context->stream, &ch)) == PDF_OK )
    {
      rv = handle_char (context, ch, &new_token);
      if (rv == PDF_OK)
        {
          /* The character we peeked at was accepted, so get rid of it. */
          pdf_stm_read_char (context->stream, &ch);
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

  rv = exit_state (context, &new_token);
  if (rv != PDF_OK)
    return rv;

  context->state = PDF_TOKENISER_STATE_EOF;
  if (new_token)
    goto ret_token;
  else
    return PDF_EEOF;

ret_token:
  assert (new_token);
  *token = new_token;
  return PDF_OK;
}


pdf_status_t
pdf_tokeniser_end_at_stream (pdf_tokeniser_t context)
{
  if (context->state && (context->state != PDF_TOKENISER_STATE_COMMENT))
    return PDF_EBADFILE;

  context->findstream = 1;
  return PDF_OK;
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
recognize_number (pdf_buffer_t buffer, int *int_value)
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


/* End of pdf-rd-tokeniser.c */

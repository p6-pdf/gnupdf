/* -*- mode: C -*-
 *
 *       File:         pdf-token-reader.c
 *       Date:         Mon Dec 29 00:45:09 2008
 *
 *       GNU PDF Library - Stream tokeniser
 *
 */

/* Copyright (C) 2008-2011 Free Software Foundation, Inc. */

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

enum pdf_token_reader_state_e
{
  /* PDF_TOKR_ is used as an internal prefix for the token reader. */
  PDF_TOKR_STATE_NONE = 0,
  PDF_TOKR_STATE_COMMENT,
  PDF_TOKR_STATE_KEYWORD,
  PDF_TOKR_STATE_NAME,
  PDF_TOKR_STATE_STRING,
  PDF_TOKR_STATE_HEXSTRING,
  PDF_TOKR_STATE_DICTEND,
  PDF_TOKR_STATE_PENDING,
  PDF_TOKR_STATE_EOF
};

/* Token reader states (from pdf_token_reader_state_e):
 * NONE - Initial state; not reading a token.
 * COMMENT - Reading a comment.  buffer collects the comment bytes.
 *   Substates:
 *     0 - normal state
 *     1 - don't produce a token
 * KEYWORD - Reading some regular characters into buffer; this could result
 *           in a symbol like "null", or a number.
 * NAME - Reading a name (which starts with '/').
 *   Substates:
 *     0 - normal state
 *     1 - just read a '#' (escape prefix)
 *     2 - read the first hex digit after '#'; the value is in charparam
 *   buffer collects the name, excluding the initial '/'.
 * STRING - Reading a literal string (enclosed in '(', ')').
 *   Substates:
 *     0 - normal state
 *     1 - ignore the next byte if its value is 10 (ASCII LF;
 *         this is used to treat CRLF as a single line ending)
 *     2 - just saw a backslash (escape prefix)
 *     3 - read 1 octal digit; the value is in charparam
 *     4 - read 2 octal digits; the value is in charparam
 *   intparam is the bracket nesting level; ')' at level 0 ends the string.
 *   buffer collects the string.
 * HEXSTRING - Reading a hex string.
 *   Substates:
 *     0 - initial state: we just saw the opening '<', and if the next byte is
 *         also '<' this is the start of a dictionary rather than a string
 *     1 - normal state (the next hex digit will be the first in a pair)
 *     2 - read the first hex digit; its value is in charparam
 *     3 - end state; saw the closing '>'
 *   buffer collects the string.
 * DICTEND - Just got a '>'; expecting another.
 *   Substates:
 *     0 - starting state
 *     1 - saw the second '>'
 * PENDING - Need to emit a token (determined by charparam) ASAP.
 * EOF - Can't continue tokenising (reached EOF, or beginning of stream)
 */

/* Internal state */
struct pdf_token_reader_s {
  pdf_stm_t *stream;  /* stream to read bytes from */
  pdf_char_t *decimal_point;

  pdf_size_t state_pos;
  pdf_size_t beg_pos; /* Beginning position of the last read token in
                         the input stream */

  /* variables used by the state machine (described above) */
  int state;
  int substate;
  pdf_char_t charparam;
  int intparam;
  pdf_buffer_t *buffer;
  /***/
  pdf_size_t buffer_size_min;
};

/* Returns 255 on invalid hex values */
#define HEXVAL(ch)                              \
  ((ch >= '0' && ch <= '9') ? ch - '0' :        \
   ((ch >= 'A' && ch <= 'F') ? ch - 'A' + 10 :  \
    ((ch >= 'a' && ch <= 'f') ? ch - 'a' + 10 : \
     255)))

/* TODO: Make this part of the tokeniser module initialization */
static pdf_char_t *
guess_decimal_point (pdf_error_t **error)
{
  int len;
  pdf_char_t decpt[16];
  pdf_char_t *decimal_point;

  len = snprintf (decpt, sizeof (decpt), "%#.0f", 1.0);
  if (len <= 0 ||
      (pdf_size_t)len >= sizeof (decpt))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TOKENISER,
                     PDF_ERROR,
                     "cannot guess decimal point: "
                     "invalid length (%d)",
                     len);
      return NULL;
    }

  decimal_point = pdf_alloc (len);
  if (!decimal_point)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TOKENISER,
                     PDF_ERROR,
                     "cannot guess decimal point: "
                     "couldn't allocate '%lu' bytes",
                     len);
      return NULL;
    }

  /* this copies the trailing '\0' due to the starting offset */
  memcpy (decimal_point, &decpt[1], len);

  return decimal_point;
}

static pdf_bool_t
reset_buffer (pdf_token_reader_t  *reader,
              pdf_error_t        **error)
{
  reader->buffer->wp = 0;

  /* Try to shrink the buffer */
  return ((reader->buffer->size > reader->buffer_size_min &&
           !pdf_buffer_resize (reader->buffer,
                               reader->buffer_size_min,
                               error)) ? PDF_FALSE : PDF_TRUE);
}

pdf_token_reader_t *
pdf_token_reader_new (pdf_stm_t    *stm,
                      pdf_error_t **error)
{
  pdf_token_reader_t *tokr = NULL;

  tokr = pdf_alloc (sizeof (struct pdf_token_reader_s));
  if (!tokr)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TOKENISER,
                     PDF_ENOMEM,
                     "couldn't allocate '%lu' bytes",
                     (unsigned long)sizeof (struct pdf_token_reader_s));
      return NULL;
    }

  tokr->beg_pos = 0;
  tokr->state_pos = 0;

  /* determine the current locale's decimal point
   * (avoid using localeconv since it may not be thread-safe) */
  tokr->decimal_point = guess_decimal_point (error);
  if (!tokr->decimal_point)
    {
      pdf_token_reader_destroy (tokr);
      return NULL;
    }

  /* buffer_size_min is the default buffer size, which is also the maximum
   * size for keywords, names, numbers, etc.; strings and comments will
   * enlarge the buffer to whatever size is needed. */
  tokr->buffer_size_min = 32768;
  tokr->buffer = pdf_buffer_new (tokr->buffer_size_min, error);
  if (!tokr->buffer)
    {
      pdf_token_reader_destroy (tokr);
      return NULL;
    }

  tokr->stream = stm;
  if (!pdf_token_reader_reset (tokr, error))
    {
      pdf_token_reader_destroy (tokr);
      return NULL;
    }

  return tokr;
}

static void
enter_state (pdf_token_reader_t            *reader,
             enum pdf_token_reader_state_e  state)
{
  reader->state = state;
  reader->state_pos = pdf_stm_tell (reader->stream);
}

pdf_bool_t
pdf_token_reader_reset (pdf_token_reader_t  *reader,
                        pdf_error_t        **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (reader, PDF_FALSE);

  enter_state (reader, PDF_TOKR_STATE_NONE);
  reader->substate = 0;
  return reset_buffer (reader, error);
}

void
pdf_token_reader_destroy (pdf_token_reader_t *reader)
{
  PDF_ASSERT_POINTER_RETURN (reader);

  if (reader->buffer)
    pdf_buffer_destroy (reader->buffer);
  if (reader->decimal_point)
    pdf_dealloc (reader->decimal_point);
  pdf_dealloc (reader);
}

static pdf_i32_t
parse_integer (pdf_buffer_t *buffer,
               pdf_i32_t    *int_value,
               pdf_i32_t    *int_state)
{
  /* Parse an ASCII integer with the given radix, at the beginning of
   * the buffer (possibly leaving unread bytes at the end).
   *
   * Return value is 0 on failure, or a bitmask otherwise:
   *  1 = valid integer
   *  2 = signed
   *  4 = overflowed (no value stored in *int_value)
   */

  pdf_i32_t sign = 0;
  pdf_i32_t tmpint = 0;
  pdf_bool_t overflowed = PDF_FALSE;
  pdf_i32_t ret;

  /* Integer states (int_state):
   *   0 = at start (looking for sign or digits)
   *   1 = saw sign
   *   2 = saw digits
   */

  *int_state = 0;
  for (; buffer->rp < buffer->wp; ++buffer->rp)
    {
      pdf_i32_t chval;
      pdf_char_t ch;

      ch = buffer->data[buffer->rp];

      if (ch == '+' || ch == '-')
        {
          if (*int_state != 0)
            break;

          *int_state = 1;
          sign = (ch == '-') ? 1 : -1;
          continue;
        }

      chval = ch - '0';  /* assume this is a digit */
      if (chval < 0 || chval > 9)
        break;  /* not a valid number */

      *int_state = 2;
      if (overflowed)
        continue;

      /* convert the digits to an integer, if possible */
      if (sign < 0)
        {
          chval = -chval;
          if (tmpint < (INT_MIN / 10) ||
              (tmpint == (INT_MIN / 10) &&
               chval < (INT_MIN % 10)))
            {
              overflowed = PDF_TRUE;  /* would overflow */
              continue;
            }
        }
      else
        {
          if (tmpint > (INT_MAX / 10) ||
              (tmpint == (INT_MAX / 10) &&
               chval > (INT_MAX % 10)))
            {
              overflowed = PDF_TRUE;  /* would overflow */
              continue;
            }
        }

      tmpint += chval + (tmpint * 9);
    }

  if (*int_state != 2)
    return 0;  /* never saw any digits */

  ret = 1;
  if (sign)
    ret += 2;

  if (overflowed)
    ret += 4;
  else
    *int_value = tmpint;

  return ret;
}

static pdf_bool_t
validate_real (pdf_buffer_t *buffer,
               pdf_i32_t     int_state)
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
      pdf_char_t ch;

      ch = buffer->data[buffer->rp];
      if (ch == '.')  /* '.' */
        {
          if (!seen_point)
            seen_point = 1;
          else
            return PDF_FALSE;
        }
      else if (ch == '+' || ch == '-')
        {
          if (int_state == 0)
            int_state = 1;
          else
            return PDF_FALSE;
        }
      else if (ch >= '0' && ch <= '9')
        int_state = 2;
      else
        return PDF_FALSE;
    }

    /* only valid if we saw a digit */
  return (int_state == 2 ? PDF_TRUE : PDF_FALSE);
}

/* Given a buffer containing a validated PDF real (in ASCII), convert it to a
 * double by translating it to the execution character set, replacing '.' with
 * the locale's decimal point, and calling strtod. */
static pdf_bool_t
parse_real (pdf_buffer_t  *buffer,
            pdf_char_t    *locale_dec_pt,
            double        *value,
            pdf_error_t  **error)
{
  pdf_size_t tmplen;
  pdf_size_t wpos;
  pdf_size_t ptlen;
  pdf_char_t *tmp;
  pdf_char_t *endptr;

  ptlen = strlen (locale_dec_pt);
  /* we may remove 1 byte ('.') and replace it with ptlen bytes */
  tmplen = buffer->wp - 1 + ptlen;

  tmp = pdf_alloc (tmplen + 1);
  if (!tmp)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TOKENISER,
                     PDF_ENOMEM,
                     "cannot parse real: "
                     "couldn't allocate '%lu' bytes",
                     (unsigned long)(tmplen + 1));
      return PDF_FALSE;
    }

  wpos = 0;
  for (buffer->rp = 0; buffer->rp < buffer->wp; ++buffer->rp)
    {
      pdf_char_t ch;

      ch = buffer->data[buffer->rp];
      if (wpos >= tmplen)
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_TOKENISER,
                         PDF_ERROR,
                         "cannot parse real: "
                         "out of bounds, pos(%lf) >= len(%lf)",
                         wpos, tmplen);
          pdf_dealloc (tmp);
          return PDF_FALSE;
        }

      if (ch == '.')
        {
          if (wpos + ptlen > tmplen)
            {
              pdf_set_error (error,
                             PDF_EDOMAIN_BASE_TOKENISER,
                             PDF_ERROR,
                             "cannot parse real: "
                             "out of bounds, pos(%lf) + ptlen(%lf) > len(%lf)",
                             wpos, ptlen, tmplen);
              pdf_dealloc (tmp);
              return PDF_FALSE;
            }

          memcpy (tmp + wpos, locale_dec_pt, ptlen);
          wpos += ptlen;
        }
      else if (ch == '+' ||
               ch == '-' ||
               (ch >= '0' && ch <= '9'))
        {
          tmp[wpos++] = ch;
        }
      else
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_TOKENISER,
                         PDF_ERROR,
                         "cannot parse real: "
                         "unexpected value: %c (%d)",
                         ch, ch);
          pdf_dealloc (tmp);
          return PDF_FALSE;
        }
    }

  /* null-terminate the new string, and call strtod to get its value
   * (strtof would also work if it's available) */
  tmp[wpos] = '\0';
  *value = strtod (tmp, &endptr);
  if (endptr == tmp + wpos)
    {
      pdf_dealloc (tmp);
      return PDF_TRUE;
    }

  pdf_dealloc (tmp);
  return PDF_FALSE;
}

/*
 * Return value:
 *   0 = not a number
 *   1 = integer (stored in *int_value)
 *   2 = real
 */
static pdf_i32_t
recognise_number (pdf_buffer_t *buffer,
                  pdf_i32_t    *int_value)
{
  pdf_i32_t rv;
  pdf_i32_t tmpint = 0;
  pdf_i32_t int_state = 0;

  /* try to parse as an integer */

  buffer->rp = 0;
  rv = parse_integer (buffer, &tmpint, &int_state);

  if (buffer->rp < buffer->wp)  /* didn't look at the whole buffer */
    return (validate_real (buffer, int_state) ? 2 : 0);

  if (!rv)
    return 0;
  else if (rv & 4)
    return 2;  /* integer overflowed, but could be read as a real */

  *int_value = tmpint;
  return 1;
}

static pdf_bool_t
flush_token (pdf_token_reader_t  *reader,
             pdf_u32_t            flags,
             pdf_bool_t          *eof,
             pdf_token_t        **token,
             pdf_error_t        **error)
{
  pdf_token_t *new_token;
  pdf_char_t *data = (pdf_char_t *)reader->buffer->data;
  int datasize = reader->buffer->wp;

  switch (reader->state)
    {
    case PDF_TOKR_STATE_NONE:
      /* no state to exit */
      return PDF_TRUE;

    case PDF_TOKR_STATE_EOF:
      /* can't continue parsing after EOF */
      *eof = PDF_TRUE;
      return PDF_TRUE;

    case PDF_TOKR_STATE_COMMENT:
      {
        if (reader->substate == 1 ||
            !(flags & PDF_TOKEN_RET_COMMENTS))
          {
            /* don't return a token */
            return reset_buffer (reader, error);
          }

        new_token = pdf_token_comment_new (data, datasize, error);
      }
      break;

    case PDF_TOKR_STATE_KEYWORD:
      {
        int value;
        int ntyp;

        ntyp = recognise_number (reader->buffer, &value);
        if (ntyp == 1)
          {
            new_token = pdf_token_integer_new (value, error);
          }
        else if (ntyp == 2)
          {
            double realvalue;

            if (!parse_real (reader->buffer,
                             reader->decimal_point,
                             &realvalue,
                             error))
              return PDF_FALSE;

            new_token = pdf_token_real_new ((float)realvalue, error);
          }
        else
          {
            new_token = pdf_token_keyword_new (data, datasize, error);
          }
      }
      break;

    case PDF_TOKR_STATE_NAME:
      {
        if (reader->substate != 0)  /* reading an escape sequence */
          {
            pdf_set_error (error,
                           PDF_EDOMAIN_BASE_TOKENISER,
                           PDF_EBADFILE,
                           "cannot flush token: invalid state");
            return PDF_FALSE;
          }

        new_token = pdf_token_name_new (data, datasize, error);
      }
      break;

    case PDF_TOKR_STATE_STRING:
      {
        if (reader->intparam >= 0)  /* didn't see the closing ')' */
          {
            pdf_set_error (error,
                           PDF_EDOMAIN_BASE_TOKENISER,
                           PDF_EBADFILE,
                           "cannot flush token: "
                           "(string) no closing ')'");
            return PDF_FALSE;
          }

        new_token = pdf_token_string_new (data, datasize, error);
      }
      break;

    case PDF_TOKR_STATE_HEXSTRING:
      {
        if (reader->substate != 3)  /* didn't see the closing '>' */
          {
            pdf_set_error (error,
                           PDF_EDOMAIN_BASE_TOKENISER,
                           PDF_EBADFILE,
                           "cannot flush token: "
                           "(hexstring) no closing '>'");
            return PDF_FALSE;
          }

        new_token = pdf_token_string_new (data, datasize, error);
      }
      break;

    case PDF_TOKR_STATE_DICTEND:
      {
        if (reader->substate != 1)  /* didn't see a second '>' */
          {
            pdf_set_error (error,
                           PDF_EDOMAIN_BASE_TOKENISER,
                           PDF_EBADFILE,
                           "cannot flush token: "
                           "(dictend) no second '>'");
            return PDF_FALSE;
          }

        new_token = pdf_token_valueless_new (PDF_TOKEN_DICT_END, error);
      }
      break;

    case PDF_TOKR_STATE_PENDING:
      {
        switch (reader->charparam)
          {
          case '<':
            new_token = pdf_token_valueless_new (PDF_TOKEN_DICT_START, error);
            break;
          case '[':
            new_token = pdf_token_valueless_new (PDF_TOKEN_ARRAY_START, error);
            break;
          case ']':
            new_token = pdf_token_valueless_new (PDF_TOKEN_ARRAY_END, error);
            break;
          case '{':
            new_token = pdf_token_valueless_new (PDF_TOKEN_PROC_START, error);
            break;
          case '}':
            new_token = pdf_token_valueless_new (PDF_TOKEN_PROC_END, error);
            break;
          default:
            pdf_set_error (error,
                           PDF_EDOMAIN_BASE_TOKENISER,
                           PDF_ERROR,
                           "cannot flush token: "
                           "invalid char found '%c' (%d)",
                           reader->charparam,
                           reader->charparam);
            return PDF_FALSE;
          }
      }
      break;

    default:
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TOKENISER,
                     PDF_ERROR,
                     "cannot flush token: "
                     "invalid state (%d)",
                     reader->state);
      return PDF_FALSE;
    }

  /* If no token generated, return already set error */
  if (!new_token)
    {
      pdf_prefix_error (error, "cannot flush token: ");
      return PDF_FALSE;
    }

  /* Set output new token */
  *token = new_token;

  /* Set the beginning position of this state */
  reader->beg_pos = reader->state_pos;

  return reset_buffer (reader, error);
}

static pdf_bool_t
exit_state (pdf_token_reader_t  *reader,
            pdf_u32_t            flags,
            pdf_bool_t          *eof,
            pdf_token_t        **token,
            pdf_error_t        **error)
{
  if (!flush_token (reader, flags, eof, token, error))
    return PDF_FALSE;

  reader->state = PDF_TOKR_STATE_NONE;
  reader->substate = 0;
  return PDF_TRUE;
}

#define CAN_STORE_CHAR(reader)                 \
  (reader->buffer->wp < reader->buffer->size ? \
   PDF_TRUE : PDF_FALSE)

static pdf_bool_t
enlarge_buffer (pdf_token_reader_t  *reader,
                pdf_error_t        **error)
{
  pdf_size_t size;
  pdf_size_t new_size;

  size = reader->buffer->size;
  new_size = size * 2;

  if (new_size < size)
    PDF_ASSERT_TRACE_NOT_REACHED ();

  return pdf_buffer_resize (reader->buffer, new_size, error);
}

static pdf_bool_t
store_char (pdf_token_reader_t  *reader,
            pdf_char_t           ch,
            pdf_error_t        **error)
{
  if (!CAN_STORE_CHAR (reader))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TOKENISER,
                     PDF_ENOMEM,
                     "cannot store char: "
                     "not enough empty space");
      return PDF_FALSE;
    }

  reader->buffer->data[reader->buffer->wp++] = ch;
  return PDF_TRUE;
}

static pdf_bool_t
store_char_grow (pdf_token_reader_t  *reader,
                 pdf_char_t           ch,
                 pdf_error_t        **error)
{
  if (!CAN_STORE_CHAR (reader) &&
      !enlarge_buffer (reader, error))
    return PDF_FALSE;

  reader->buffer->data[reader->buffer->wp++] = ch;
  return PDF_TRUE;
}

static pdf_bool_t
handle_string_char (pdf_token_reader_t  *reader,
                    pdf_u32_t            flags,
                    pdf_char_t           ch,
                    pdf_bool_t          *eof,
                    pdf_token_t        **token,
                    pdf_error_t        **error)
{
  while (PDF_TRUE)
    {
      switch (reader->substate)
        {
        case 1:  /* ignore LF */
          {
            reader->substate = 0;
            if (ch == '\n')
              return PDF_TRUE;
          } /* fall through */

        case 0:  /* no special state */
          {
            pdf_bool_t was_cr;

            if (ch == '\\')
              {
                /* start an escape sequence */
                reader->substate = 2;
                return PDF_TRUE;
              }

            if (ch == ')' &&
                reader->intparam <= 0)  /* ')'; end of string */
              {
                reader->intparam = -1;
                return exit_state (reader, flags, eof, token, error);
              }

            was_cr = (ch == '\r');
            if (was_cr)
              ch = '\n';  /* treat as LF */

            if (!store_char_grow (reader, ch, error))
              return PDF_FALSE;

            if (was_cr)
              {
                /* ignore the next char if it's LF */
                reader->substate = 1;
              }
            else if (ch == '(')
              ++reader->intparam;
            else if (ch == ')')
              --reader->intparam;

            return PDF_TRUE;
          }

        case 2:  /* just saw a '\\' (starting an escape sequence) */
          {
            reader->substate = 0;

            if (ch == 'b')
              ch = 8;  /* BS: backspace */
            else if (ch == 'f')
              ch = 12;  /* FF: formfeed */
            else if (ch == 'n')
              ch = '\n';  /* NL: newline */
            else if (ch == 'r')
              ch = '\r';  /* CR: carriage return */
            else if (ch == 't')
              ch = '\t';  /* HT: horizontal tab */
            else if (ch == '\n')  /* NL */
              return PDF_TRUE;  /* ignore the line break */
            else if (ch == '\r')  /* CR */
              {
                /* ignore the line break; also ignore the next byte if it's LF */
                reader->substate = 1;
                return PDF_TRUE;
              }
            else if (ch >= '0' && ch <= '7')
              {
                /* starting an octal escape; we'll read three digits even if the
                 * first is '4'--'7' (and calculate the final char modulo 256),
                 * since the PDF/PS specs say to ignore high-order overflow */
                reader->substate = 3;
                reader->charparam = (ch - '0');
                return PDF_TRUE;
              }

            /* for any other character, including '(', ')', and '\\',
             * store the same character (dropping the leading backslash) */
            return store_char_grow (reader, ch, error);
          }

        case 3:  /* saw 1 digit of an octal escape */
          {
          } /* fall through */

        case 4:  /* saw 2 digits of an octal escape */
          {
            if (ch < '0' || ch > '7')
              {
                if (!store_char_grow (reader, reader->charparam, error))
                  return PDF_FALSE;

                /* ch isn't part of the escape sequence, so retry */
                reader->substate = 0;
                continue;
              }

            /* ch is a digit from '0'--'7' */
            reader->charparam = ((reader->charparam & 0x1f) << 3) | (ch - '0');
            if (reader->substate == 4)  /* this was the final digit */
              {
                if (!store_char_grow (reader, reader->charparam, error))
                  return PDF_FALSE;

                reader->substate = 0;
                return PDF_TRUE;
              }

            reader->substate = 4;
            return PDF_TRUE;
          }

        default:
          {
            pdf_set_error (error,
                           PDF_EDOMAIN_BASE_TOKENISER,
                           PDF_ERROR,
                           "cannot handle string char: "
                           "invalid substate (%d)",
                           reader->substate);
            return PDF_FALSE;
          }
        }
    }
}

static pdf_bool_t
handle_hexstring_char (pdf_token_reader_t  *reader,
                       pdf_u32_t           flags,
                       pdf_char_t          ch,
                       pdf_bool_t         *eof,
                       pdf_token_t       **token,
                       pdf_error_t       **error)
{
  if (reader->substate == 0)
    {
      /* this is the first character after the initial '<' */
      if (ch == '<') /* '>': start of hex string */
        {
          /* this was actually the start of a dictionary */
          reader->state = PDF_TOKR_STATE_PENDING;
          reader->charparam = ch;
          return exit_state (reader, flags, eof, token, error);
        }

      reader->substate = 1;
    }

  /* whitespaces are skipped */
  if (pdf_is_wspace_char (ch))
    return PDF_TRUE;

  if (ch == '>')  /* '>': end of hex string */
    {
      if (reader->substate == 2)
        {
          /* the last digit is missing; assume it's '0' */
          if (!store_char_grow (reader,
                                reader->charparam << 4,
                                error))
            return PDF_FALSE;
        }

      reader->substate = 3;  /* saw end of string */
      return exit_state (reader, flags, eof, token, error);
    }

  if ((ch = HEXVAL (ch)) == 255)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TOKENISER,
                     PDF_EBADFILE,
                     "cannot handle hexstring char: "
                     "not an hex character '%c' (%d)",
                     ch, ch);
      return PDF_FALSE;
    }

  if (reader->substate == 1)  /* first character in a pair */
    {
      reader->substate = 2;
      reader->charparam = ch;
      return PDF_TRUE;
    }

  if (!store_char_grow (reader,
                        (reader->charparam << 4) | ch,
                        error))
    return PDF_FALSE;

  reader->substate = 1;
  return PDF_TRUE;
}

/* Tries to handle the given character and possibly produce a token.
 * Sets (*token) if a token is produced, and leaves it unmodified otherwise.
 *
 * Returns PDF_OK if the character was accepted. Otherwise, an error code
 * is returned, and the call can be repeated later with the same ch value.
 * A token may be produced even if the character isn't accepted.
 */
static pdf_bool_t
handle_char (pdf_token_reader_t  *reader,
             pdf_u32_t            flags,
             pdf_char_t           ch,
             pdf_bool_t          *again,
             pdf_bool_t          *eof,
             pdf_token_t        **token,
             pdf_error_t        **error)
{
  /* first, handle the states that shouldn't be exited when whitespace
   * or a delimiter is seen */

  switch (reader->state)
    {
    case PDF_TOKR_STATE_EOF:
      {
        *eof = PDF_TRUE;
        return PDF_TRUE;
      }

    case PDF_TOKR_STATE_STRING:
      return handle_string_char (reader, flags, ch, eof, token, error);

    case PDF_TOKR_STATE_HEXSTRING:
      return handle_hexstring_char (reader, flags, ch, eof, token, error);

    case PDF_TOKR_STATE_DICTEND:
      {
        if (ch != '>')
          {
            pdf_set_error (error,
                           PDF_EDOMAIN_BASE_TOKENISER,
                           PDF_EBADFILE,
                           "cannot handle char: "
                           "no dict end");
            return PDF_FALSE;
          }

        reader->substate = 1;  /* saw the closing '>' */
        return exit_state (reader, flags, eof, token, error);
      }

    case PDF_TOKR_STATE_COMMENT:
      {
        if (pdf_is_eol_char (ch))
          {
            if (!exit_state (reader, flags, eof, token, error))
              return PDF_FALSE;

            /* don't accept this character, but process it next time */
            *again = PDF_TRUE;
            return PDF_TRUE;
          }

        if (!(flags & PDF_TOKEN_RET_COMMENTS))
          reader->substate = 1;

        if (reader->substate == 1)
          return PDF_TRUE;  /* we don't care about this comment */

        return store_char_grow (reader, ch, error);
      }

    default:
      /* Nothing to do */
      break;
    }

  /* now handle delimiters and whitespace */

  if (pdf_is_wspace_char (ch))
    {
      if (reader->state)
        {
          if (!exit_state (reader, flags, eof, token, error))
            return PDF_FALSE;

          /* avoid reading this byte so PDF_TOKEN_END_AT_STREAM
           * will work properly if it's '\r' */
          *again = PDF_TRUE;
          return PDF_TRUE;
        }

      if ((flags & PDF_TOKEN_END_AT_STREAM) && ch == '\n')  /* LF */
        {
          /* found the beginning of a stream */
          enter_state (reader, PDF_TOKR_STATE_EOF);
        }

      return PDF_TRUE;
    }

  if ((flags & PDF_TOKEN_END_AT_STREAM) && ch != '%')
    {
      /* only allow whitespace/comments after the "stream" keyword */
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TOKENISER,
                     PDF_EBADFILE,
                     "cannot handle char: "
                     "only whitespace/comments allowed after 'stream' keyword");
      return PDF_FALSE;
    }

  if (pdf_is_delim_char (ch))
    {
      /* set state 0 (UNINIT), substate 0, bufpos 0 */
      if (reader->state)
        {
          if (!exit_state (reader, flags, eof, token, error))
            return PDF_FALSE;

          *again = PDF_TRUE;
          return PDF_TRUE;
        }

      switch (ch)
        {
        case '%':
          {
            enter_state (reader, PDF_TOKR_STATE_COMMENT);
            return PDF_TRUE;
          }

        case '(':
          {
            enter_state (reader, PDF_TOKR_STATE_STRING);
            reader->intparam = 0;
            return PDF_TRUE;
          }

        case ')':
          {
            /* this shouldn't occur outside the STRING and COMMENT states */
            pdf_set_error (error,
                           PDF_EDOMAIN_BASE_TOKENISER,
                           PDF_EBADFILE,
                           "cannot handle char: "
                           "wrong ')' out of string and comment states");
            return PDF_FALSE;
          }

        case '/':
          {
            enter_state (reader, PDF_TOKR_STATE_NAME);
            return PDF_TRUE;
          }

        case '<':
          {
            enter_state (reader, PDF_TOKR_STATE_HEXSTRING);
            return PDF_TRUE;
          }

        case '>':
          {
            enter_state (reader, PDF_TOKR_STATE_DICTEND);
            return PDF_TRUE;
          }
        case '[':
        case ']':
        case '{':
        case '}':
          {
            /* exit_state may have emitted a token, so we can't emit another
             * one now; we'll do it when exiting the PENDING state */
            enter_state (reader, PDF_TOKR_STATE_PENDING);
            reader->charparam = ch;
            return PDF_TRUE;
          }
        default:
          {
            /* not reached (all delimiter chars should be handled) */
            PDF_ASSERT_TRACE_NOT_REACHED ();
            return PDF_FALSE;
          }
        }
    }

  /* ch is a regular character */

  switch (reader->state)
    {
    case PDF_TOKR_STATE_PENDING:
      {
        if (!exit_state (reader, flags, eof, token, error))
          return PDF_FALSE;

        *again = PDF_TRUE;
        return PDF_TRUE;
      }

    case PDF_TOKR_STATE_NONE:
      enter_state (reader, PDF_TOKR_STATE_KEYWORD);
      /* fall through */

    case PDF_TOKR_STATE_KEYWORD:
      /* Note: numbers are treated as keywords until flush_token is called. */
      return store_char (reader, ch, error);

    case PDF_TOKR_STATE_NAME:
      {
        if (reader->substate == 0)
          {
            if ((ch < 0x21) || (ch > 0x7e))
              {
              /* Invalid character in a name.  */
                pdf_set_error (error,
                               PDF_EDOMAIN_BASE_TOKENISER,
                               PDF_EBADFILE,
                               "cannot handle char: "
                               "invalid character in a name (%d)",
                               ch);
                return PDF_FALSE;
              }

            if (ch != '#' ||
                (flags & PDF_TOKEN_NO_NAME_ESCAPES))
              return store_char (reader, ch, error);

            reader->substate = 1;
            return PDF_TRUE;
          }

        if ((ch = HEXVAL (ch)) == 255)
          {
            pdf_set_error (error,
                           PDF_EDOMAIN_BASE_TOKENISER,
                           PDF_EBADFILE,
                           "cannot handle char: "
                           "not an hex character '%c' (%d)",
                           ch, ch);
            return PDF_FALSE;
          }

        if (reader->substate == 1)  /* the first hex digit of an escape */
          {
            reader->substate = 2;
            reader->charparam = ch;
            return PDF_TRUE;
          }

        ch = (reader->charparam << 4) | ch;
        if (ch == 0)  /* the PDF spec forbids "#00" */
          {
            pdf_set_error (error,
                           PDF_EDOMAIN_BASE_TOKENISER,
                           PDF_EBADFILE,
                           "cannot handle char: "
                           "'#00' is forbidden");
            return PDF_FALSE;
          }

        if (!store_char (reader, ch, error))
          return PDF_FALSE;

        reader->substate = 0;
        return PDF_TRUE;
      }

    default:
      {
        pdf_set_error (error,
                       PDF_EDOMAIN_BASE_TOKENISER,
                       PDF_ERROR,
                       "cannot handle char: "
                       "invalid state (%d)",
                       reader->state);
        return PDF_FALSE;
      }
    }
}

pdf_token_t *
pdf_token_read (pdf_token_reader_t  *reader,
                pdf_u32_t            flags,
                pdf_error_t        **error)
{
  pdf_token_t *new_token = NULL;
  pdf_bool_t eof;
  pdf_uchar_t ch;

  PDF_ASSERT_POINTER_RETURN_VAL (reader, NULL);
  PDF_ASSERT_POINTER_RETURN_VAL (reader->stream, NULL);

  while (pdf_stm_peek_char (reader->stream, &ch, error))
    {
      pdf_bool_t again = PDF_FALSE;

      eof = PDF_FALSE;
      if (!handle_char (reader,
                        flags,
                        (pdf_char_t)ch,
                        &again,
                        &eof,
                        &new_token,
                        error))
        return NULL;

      /* On EOF, return NULL without error */
      if (eof)
        return NULL;

      if (!again)
        {
          /* The character we peeked at was accepted, so get rid of it. */
          if (!pdf_stm_read_char (reader->stream, &ch, error))
            {
              if (new_token)
                pdf_token_destroy (new_token);
              return PDF_FALSE;
            }
        }

      if (new_token)
        {
          /* Don't return an error code if we got a valid token.
           * We'll probably see the same error on the next call since we
           * didn't call read_char. */
          return new_token;
        }

      /* Keep on loop. Note that if again == TRUE,
       * we peek again the same char */
    }

  eof = PDF_FALSE;
  if (!exit_state (reader, flags, &eof, &new_token, error))
    return NULL;

  /* On EOF, return NULL without error */
  if (eof)
    return NULL;

  reader->state = PDF_TOKR_STATE_EOF;

  if (new_token)
    return new_token;

  /* Mark EOF */
  return NULL;
}

pdf_size_t
pdf_token_reader_begin_pos (pdf_token_reader_t *reader)
{
  PDF_ASSERT_POINTER_RETURN_VAL (reader, (pdf_size_t)0);

  return reader->beg_pos;
}

/* End of pdf-token-reader.c */

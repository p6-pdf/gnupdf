/* -*- mode: C -*-
 *
 *       File:         pdf-token-writer.c
 *       Date:         Wed Sep 23 04:37:51 2009
 *
 *       GNU PDF Library - Stream token writer
 *
 */

/* Copyright (C) 2009-2011 Free Software Foundation, Inc. */

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

#include <stdlib.h>
#include <string.h>

#include <inttypes.h>
#include <unistr.h>

#include <pdf-tokeniser.h>
#include <pdf-token-writer.h>

/* Internal state */
struct pdf_token_writer_s {
  pdf_stm_t *stream;  /* stream to write bytes to */

  pdf_bool_t in_keyword;
  pdf_size_t line_length;
  pdf_size_t buffered_line_length;
  pdf_size_t max_line_length;

  int state; /* This field is used by the writing routines to denote
                several states in the writing of the tokens.  Thus its
                concrete meaning depends on the specific writing
                routine.  See the write_*_token functions in
                pdf-token-writer.c for more information. */
  pdf_size_t pos;
  pdf_size_t paren_quoting_start, paren_quoting_end;
  pdf_bool_t utf8;
  pdf_buffer_t *buffer;
};

/* Returns 255 on invalid hex values */
#define HEXCHAR(value)                          \
  (value < 10 ? value + '0' :                   \
   (value < 16 ? value - 10 + 'A' :             \
    255))

/* PDF32000 7.5.1: "lines that are not part of stream object data
 * are limited to no more than 255 characters"... */
#define PDF_TOKW_MAX_LINE_LENGTH 255

/* The buffer size is mostly arbitrary, but the buffer must be large
 * enough for snprintf to write any possible floating point value.
 * Any number over 50 should be fine. */
#define PDF_TOKW_BUFFER_SIZE 32768

pdf_token_writer_t *
pdf_token_writer_new (pdf_stm_t    *stm,
                      pdf_error_t **error)
{
  pdf_token_writer_t *tokw = NULL;

  PDF_ASSERT_POINTER_RETURN_VAL (stm, NULL);

  /* Allow only write streams */
  if (pdf_stm_get_mode (stm) != PDF_STM_WRITE)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TOKENISER,
                     PDF_EBADDATA,
                     "cannot create token writer: "
                     "write stream needed");
      return NULL;
    }

  tokw = pdf_alloc (sizeof (struct pdf_token_writer_s));
  if (!tokw)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TOKENISER,
                     PDF_ENOMEM,
                     "cannot create token writer: "
                     "couldn't allocate '%lu' bytes",
                     (unsigned long)sizeof (struct pdf_token_writer_s));
      return NULL;
    }

  tokw->buffer = pdf_buffer_new (PDF_TOKW_BUFFER_SIZE, error);
  if (!tokw->buffer)
    {
      pdf_token_writer_destroy (tokw);
      return NULL;
    }

  /* set max_line_length to 0 for no maximum */
  tokw->max_line_length = PDF_TOKW_MAX_LINE_LENGTH;
  tokw->stream = stm;

  if (!pdf_token_writer_reset (tokw, error))
    {
      pdf_token_writer_destroy (tokw);
      return NULL;
    }

  return tokw;
}

pdf_bool_t
pdf_token_writer_reset (pdf_token_writer_t  *writer,
                        pdf_error_t        **error)
{
  PDF_ASSERT_POINTER_RETURN_VAL (writer, PDF_FALSE);

  writer->state = 0;
  writer->in_keyword = PDF_FALSE;
  writer->line_length = 0;
  return PDF_TRUE;
}

void
pdf_token_writer_destroy (pdf_token_writer_t *writer)
{
  if (!writer)
    return;

  if (writer->buffer)
    pdf_buffer_destroy (writer->buffer);
  pdf_dealloc (writer);
}

/***** Unbuffered output *****/

/* Write data to the stream.  All output passes through this function. */
static pdf_bool_t
write_data (pdf_token_writer_t  *writer,
            const pdf_uchar_t   *data,
            pdf_size_t           len,
            pdf_size_t          *written,
            pdf_error_t        **error)
{
  pdf_size_t i;
  pdf_error_t *inner_error = NULL;
  pdf_size_t n_written;

  if (!pdf_stm_write (writer->stream, data, len, &n_written, &inner_error) &&
      inner_error)
    {
      if (written)
        *written = n_written;
      pdf_propagate_error (error, inner_error);
      return PDF_FALSE;
    }

  for (i = 0; i < n_written; ++i)
    {
      writer->line_length++;
      if (pdf_is_eol_char (data[i]))
        writer->line_length = 0;

      writer->in_keyword = pdf_is_regular_char (data[i]);
    }

  if (written)
    *written = n_written;
  return PDF_TRUE;
}

static pdf_bool_t
write_char (pdf_token_writer_t  *writer,
            pdf_uchar_t          ch,
            pdf_error_t        **error)
{
  return write_data (writer, &ch, 1, NULL, error);
}

/* Write data starting at writer->pos, incrementing writer->pos as needed. */
static pdf_bool_t
write_data_using_pos (pdf_token_writer_t  *writer,
                      const pdf_uchar_t   *data,
                      pdf_size_t           len,
                      pdf_error_t        **error)
{
  pdf_size_t written;

  /* Just in case... */
  if (writer->pos > len)
    {
      PDF_ASSERT_TRACE_NOT_REACHED ();
      return PDF_FALSE;
    }

  while (writer->pos < len)
    {
      if (!write_data (writer,
                       data + writer->pos,
                       len - writer->pos,
                       &written,
                       error))
        return PDF_FALSE;

      writer->pos += written;
    }

  return PDF_TRUE;
}

/***** Buffered output, buffer management *****/

/* Write all buffered data to the stream. */
static pdf_bool_t
flush_buffer (pdf_token_writer_t  *writer,
              pdf_error_t        **error)
{
  pdf_size_t len;

  while ((len = writer->buffer->wp - writer->buffer->rp) > 0)
    {
      pdf_size_t written;

      if (!write_data (writer,
                       writer->buffer->data + writer->buffer->rp,
                       len,
                       &written,
                       error))
        return PDF_FALSE;

      writer->buffer->rp += written;
    }

  pdf_buffer_rewind (writer->buffer);
  return PDF_TRUE;
}

/* Flush the buffer if there are less than 'len' bytes free. */
static pdf_bool_t
reserve_buffer_space (pdf_token_writer_t  *writer,
                      pdf_size_t           len,
                      pdf_error_t        **error)
{
  if (writer->buffer->wp + len > writer->buffer->size)
    {
      if (!flush_buffer (writer, error))
        return PDF_FALSE;

      PDF_ASSERT_RETURN_VAL (len < writer->buffer->size, PDF_FALSE);
      PDF_ASSERT_RETURN_VAL (writer->buffer->wp == 0, PDF_FALSE);
    }

  return PDF_TRUE;
}

/* Write a character into the buffer; this assumes it will fit. */
static void
write_buffered_char_nocheck (pdf_token_writer_t *writer,
                             pdf_char_t          ch)
{
  writer->buffer->data[writer->buffer->wp++] = ch;
  if (pdf_is_eol_char(ch))
    writer->buffered_line_length = 0;
  else
    writer->buffered_line_length++;
}

/* Write a character into the buffer.  The buffer is flushed only if
 * there's no room to write the character. */
static pdf_bool_t
write_buffered_char (pdf_token_writer_t  *writer,
                     pdf_char_t           ch,
                     pdf_error_t        **error)
{
  if (!reserve_buffer_space (writer, 1, error))
    return PDF_FALSE;

  write_buffered_char_nocheck (writer, ch);
  return PDF_TRUE;
}

/***** Misc. utility functions *****/

/* Prepare to write a new token, adding some whitespace if necessary. */
static pdf_bool_t
start_token (pdf_token_writer_t  *writer,
             pdf_bool_t           need_wspace,
             pdf_size_t           len,
             pdf_error_t        **error)
{
  const pdf_uchar_t *add_char = NULL;
  static const pdf_uchar_t wspace_char = ' ';
  static const pdf_uchar_t newline_char = '\n';

  if (need_wspace && writer->in_keyword)
    {
      add_char = &wspace_char;
      len++;
    }

  /* If the token would make this line too long, start a new line. */
  if (writer->line_length + len > writer->max_line_length &&
      writer->max_line_length > 0)
    {
      add_char = &newline_char;
    }

  return ((add_char &&
           !write_data (writer, add_char, 1, NULL, error)) ?
          PDF_FALSE : PDF_TRUE);
}

/***** Numeric tokens *****/

/* Encode snprintf output for PDF.  'len' is the return value of snprintf.
 * Re-encodes bytes 0 to 'len' of writer->buffer and resets buffer->rp/wp. */
static pdf_bool_t
encode_buffer_number (pdf_token_writer_t  *writer,
                      pdf_error_t        **error,
                      const pdf_char_t    *number_format,
                      ...)
{
  int len;
  va_list args;

  va_start (args, number_format);

  /* Print the number in the buffer */
  len = vsnprintf ((char *)writer->buffer->data,
                   writer->buffer->size,
                   number_format,
                   args);
  va_end (args);

  if (len < 0)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TOKENISER,
                     PDF_ERROR,
                     "couldn't encode number in buffer");
      return PDF_FALSE;
    }
  else if (len >= writer->buffer->size)
    {
      /* TODO: Can't we just reserve more buffer space? */
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_TOKENISER,
                     PDF_EAGAIN,
                     "couldn't encode number in buffer: "
                     "not enough space (%u needed, %u available)",
                     (unsigned long)len,
                     (unsigned long)writer->buffer->size);
      return PDF_FALSE;
    }

  writer->buffer->wp = 0;
  writer->buffer->rp = 0;
  while (writer->buffer->rp < len)
    {
      pdf_char_t ch = writer->buffer->data[writer->buffer->rp];

      if ((ch == '-') ||
          (ch >= '0' && ch <= '9'))
        {
          writer->buffer->rp++;
          writer->buffer->data[writer->buffer->wp++] = ch;
        }
      else
        {
          /* This should be a decimal point; check it. */
          const pdf_char_t *decimal_point;
          pdf_size_t declen;

          decimal_point = pdf_tokeniser_get_decimal_point ();
          declen = strlen (decimal_point);

          if (memcmp (writer->buffer->data + writer->buffer->rp,
                      decimal_point,
                      declen) != 0)
            {
              pdf_set_error (error,
                             PDF_EDOMAIN_BASE_TOKENISER,
                             PDF_ERROR,
                             "couldn't encode number in buffer: "
                             "expecting decimal point, unexpected char found");
              return PDF_FALSE;
            }

          writer->buffer->rp += declen;
          writer->buffer->data[writer->buffer->wp++] = '.';
        }
    }

  writer->buffer->rp = 0;
  return PDF_TRUE;  /* success */
}

static pdf_bool_t
write_integer_token (pdf_token_writer_t  *writer,
                     const pdf_token_t   *token,
                     pdf_error_t        **error)
{
  switch (writer->state)
    {
    case 0:
      if (!encode_buffer_number (writer,
                                 error,
                                 "%"PRId32,
                                 pdf_token_get_integer_value (token)))
        return PDF_FALSE;
      writer->state++;
      /* fall through */

    case 1:
      if (!start_token (writer,
                        PDF_TRUE /* need_wspace */,
                        writer->buffer->wp,
                        error))
        return PDF_FALSE;
      writer->state++;
      /* fall through */

    case 2:
      return flush_buffer (writer, error);
    default:
      PDF_ASSERT_TRACE_NOT_REACHED ();
      return PDF_FALSE;
    }
}

static pdf_bool_t
write_real_token (pdf_token_writer_t  *writer,
                  const pdf_token_t   *token,
                  pdf_error_t        **error)
{
  switch (writer->state)
    {
    case 0:
      {
        pdf_real_t value;

        value = pdf_token_get_real_value (token);
        if (isnan (value) || isinf (value))
          {
            pdf_set_error (error,
                           PDF_EDOMAIN_BASE_TOKENISER,
                           PDF_EBADDATA,
                           "cannot write real token: %s value",
                           isnan (value) ? "NaN" : "Infinite");
            return PDF_FALSE;
          }

        if (!encode_buffer_number (writer,
                                   error,
                                   "%#f",
                                   value))
          return PDF_FALSE;

        /* strip trailing zeroes */
        while (writer->buffer->wp &&
               writer->buffer->data[writer->buffer->wp - 1] == '0')
          writer->buffer->wp--;
      }
      writer->state++;
      /* fall through */

    case 1:
      if (!start_token (writer,
                        PDF_TRUE /* need_wspace */,
                        writer->buffer->wp,
                        error))
        return PDF_FALSE;;
      writer->state++;
      /* fall through */

    case 2:
      return flush_buffer (writer, error);

    default:
      PDF_ASSERT_TRACE_NOT_REACHED ();
      return PDF_FALSE;
    }
}

/***** String tokens *****/

static pdf_bool_t
should_escape_strchar (pdf_u32_t  flags,
                       pdf_char_t ch,
                       pdf_bool_t quote_parens,
                       pdf_bool_t is_utf8)
{
  if (ch == '\\' || ch == '\r')
    return PDF_TRUE;

  if (ch == '(' || ch == ')')
    return quote_parens;

  if (flags & PDF_TOKEN_READABLE_STRINGS)
    {
      if (ch == 127 ||
          (ch < 32 && ch != 10) ||
          (ch >= 128 && !is_utf8))
        return PDF_TRUE;
    }

  return PDF_FALSE;
}

static pdf_i32_t
str_escape_len (const pdf_char_t *data,
                pdf_size_t        len,
                pdf_size_t        pos)
{
  switch (data[pos])
    {
      /* characters with two-character escape codes */
      case  8:
      case  9:
      case 10:
      case 12:
      case 13:
      case 40:
      case 41:
      case 92:
        return 2;
    }

  if (data[pos] >= 0100)
    return 4;  /* escaped using a backslash and 3 octal characters */

  if (pos + 1 < len)
    {
      if (data[pos + 1] >= '0' && data[pos + 1] <= '9')
        return 4;  /* need to write a 3-character octal number */
    }

  return (data[pos] >= 010) ? 3 : 2;
}

static void
scan_string (pdf_token_writer_t *writer,
             pdf_u32_t           flags,
             const pdf_char_t   *data,
             pdf_size_t          len,
             pdf_bool_t         *use_hex)
{
  pdf_size_t i;
  pdf_size_t enc_bytes = 0;

  /* Match parentheses, and determine the portion of the string
   * in which they should be quoted. */
  writer->paren_quoting_start = 0;
  writer->paren_quoting_end = len;

  for (i = 0; i < len; ++i)
    {
      if (data[i] == '(')
        {
          pdf_size_t j;

          for (j = writer->paren_quoting_end - 1; j > i; --j)
            {
              /* find a matching ')' */
              if (data[j] == ')')
                {
                  writer->paren_quoting_end = j;
                  writer->paren_quoting_start = i + 1;
                  break;
                }
            }
        }
    }

  /* Determine the size of the escaped string. */
  writer->utf8 = ((flags & PDF_TOKEN_READABLE_STRINGS) &&
                  (u8_check ((uint8_t *) data, len) == NULL));

  for (i = 0; i < len; ++i)
    {
      if (should_escape_strchar (flags,
                                 data[i],
                                 ((i >= writer->paren_quoting_start &&
                                   i < writer->paren_quoting_end) ?
                                  PDF_TRUE : PDF_FALSE),
                                 writer->utf8))
        enc_bytes += str_escape_len (data, len, i);
      else
        enc_bytes++;
    }
  *use_hex = (enc_bytes > len * 2);
}

static pdf_bool_t
write_string_char (pdf_token_writer_t  *writer,
                   pdf_u32_t            flags,
                   const pdf_char_t    *data,
                   pdf_char_t           len,
                   pdf_size_t           pos,
                   pdf_error_t        **error)
{
  const pdf_char_t *output;
  pdf_size_t outlen = 1;
  pdf_char_t esc[4] = { '\\', 0, 0, 0 };
  pdf_char_t ch;
  pdf_bool_t quote_parens;
  pdf_size_t i;

  output = data + pos;
  ch = data[pos];
  quote_parens = (pos >= writer->paren_quoting_start &&
                  pos < writer->paren_quoting_end);

  if (should_escape_strchar (flags, ch, quote_parens, writer->utf8))
    {
      /* escape the character */
      output = esc;
      outlen = 2;

      switch (ch)
        {
        case  8: esc[1] = 'b'; break;
        case  9: esc[1] = 't'; break;
        case 10: esc[1] = 'n'; break;
        case 12: esc[1] = 'f'; break;
        case 13: esc[1] = 'r'; break;
        case 40:  /* '('; fall through */
        case 41:  /* ')'; fall through */
        case 92:  /* '\\' */
          esc[1] = ch;
          break;
        default: /* use an octal escape */
          {
            pdf_size_t digits;
            pdf_char_t nextch;

            nextch = (pos + 1 < len) ? data[pos + 1] : 0;
            if (nextch >= '0' && nextch <= '9')
              digits = 3;  /* must use 3 octal characters */
            else if (ch > 0100)
              digits = 3;
            else if (ch > 010)
              digits = 2;
            else
              digits = 1;

            outlen = 1;
            switch (digits)
              {
                /* fall through each case */
              case 3: esc[outlen++] = HEXCHAR (ch / 0100);
              case 2: esc[outlen++] = HEXCHAR ((ch % 0100) / 010);
              case 1: esc[outlen++] = HEXCHAR (ch % 010);
              }
          }
        }
    }

  /* If the line will be too long, split it (the length cannot be equal to
   * the maximum, since this would leave no room for the backslash). */
  if (writer->max_line_length > 0 &&
      !pdf_is_eol_char (output[0]) &&
      writer->buffered_line_length + outlen >= writer->max_line_length)
    {
      if (!reserve_buffer_space (writer, 2, error))
        return PDF_FALSE;

      write_buffered_char_nocheck (writer, '\\');
      write_buffered_char_nocheck (writer, '\n');  /* newline */
    }

  if (!reserve_buffer_space (writer, outlen, error))
    return PDF_FALSE;

  for (i = 0; i < outlen; ++i)
    write_buffered_char_nocheck (writer, output[i]);

  return PDF_TRUE;
}

static pdf_bool_t
write_string_token (pdf_token_writer_t  *writer,
                    pdf_u32_t           flags,
                    const pdf_token_t  *token,
                    pdf_error_t       **error)
{
  const pdf_char_t *data;
  pdf_size_t size;

  data = pdf_token_get_string_data (token);
  size = pdf_token_get_string_size (token);

  switch (writer->state)
    {
    case 0:
      {
        pdf_bool_t use_hex = (flags & PDF_TOKEN_HEX_STRINGS);

        if (!use_hex)
          scan_string (writer, flags, data, size, &use_hex);

        if (use_hex)
          goto hexstring_start;
      }
      writer->state++;
      /* fall through */

    case 1:
      {
        /* Passing a correct length to start_token isn't important
         * since we can split the string across multiple lines. */
        pdf_size_t dummy_len = PDF_MIN (20, 2 + size);

        if (!start_token (writer,
                          PDF_FALSE /* need_wspace */,
                          dummy_len,
                          error))
          return PDF_FALSE;
      }

      pdf_buffer_rewind (writer->buffer);
      writer->buffered_line_length = writer->line_length;
      write_buffered_char_nocheck (writer, '(');
      writer->pos = 0;
      writer->state++;
      /* fall through */

    case 2:
      while (writer->pos < size)
        {
          if (!write_string_char (writer,
                                  flags,
                                  data,
                                  size,
                                  writer->pos,
                                  error))
            return PDF_FALSE;
          writer->pos++;
        }
      writer->state++;
      /* fall through */

    case 3:
      if (!write_buffered_char (writer, ')', error))
        return PDF_FALSE;
      writer->state++;
      /* fall through */

    case 4:
      return flush_buffer (writer, error);

      /*** hex strings ***/
    hexstring_start:
      writer->state = 101;
    case 101:
      {
        pdf_size_t dummy_len = PDF_MIN (20, 2 + size * 2);

        if (!start_token (writer,
                          PDF_FALSE /* need_wspace */,
                          dummy_len,
                          error))
          return PDF_FALSE;
      }

      pdf_buffer_rewind (writer->buffer);
      writer->buffered_line_length = writer->line_length;
      write_buffered_char_nocheck (writer, '<');
      writer->pos = 0;
      writer->state++;
      /* fall through */

    case 102:
      while (writer->pos < size)
        {
          pdf_char_t ch;

          /* If this line would be too long, start a new one. */
          if (writer->buffered_line_length + 2 > writer->max_line_length
              && writer->max_line_length > 0)
            {
              if (!write_buffered_char (writer, '\n', error))
                return PDF_FALSE;
            }

          ch = data[writer->pos];
          if (!reserve_buffer_space (writer, 2, error))
            return PDF_FALSE;

          write_buffered_char_nocheck (writer, HEXCHAR (ch / 16));
          if (writer->pos != (size - 1) ||
              (ch % 16) != 0)
            write_buffered_char_nocheck (writer, HEXCHAR (ch % 16));
          writer->pos++;
        }
        writer->state++;
        /* fall through */

      case 103:
        if (!write_buffered_char (writer, '>', error))
          return PDF_FALSE;

        writer->state++;
        /* fall through */

      case 104:
        return flush_buffer (writer, error);

      default:
        PDF_ASSERT_TRACE_NOT_REACHED ();
        return PDF_FALSE;
    }
}

/***** Other tokens *****/

static pdf_bool_t
should_escape_namechar (pdf_u32_t   flags,
                        pdf_char_t  ch,
                        pdf_bool_t *escape)
{
  if (!ch)
    return PDF_FALSE;

  *escape = !pdf_is_regular_char (ch);
  if (flags & PDF_TOKEN_NO_NAME_ESCAPES)
    {
      if (*escape)
        return PDF_FALSE;
    }
  else
    {
      *escape = (*escape ||
                 ch == '#' ||
                 ch < 33 ||
                 ch >= 127);
    }

  return PDF_TRUE;
}

static pdf_bool_t
write_name_token (pdf_token_writer_t  *writer,
                  pdf_u32_t            flags,
                  const pdf_token_t   *token,
                  pdf_error_t        **error)
{
  pdf_size_t size;
  const pdf_char_t *data;

  data = pdf_token_get_name_data (token);
  size = pdf_token_get_name_size (token);

  switch (writer->state)
    {
    case 0:
      {
        pdf_size_t i;

        /* Validate the name; also calculate the encoded size
         * and store it in ->pos temporarily. */

        writer->pos = 1 + size;
        for (i = 0; i < size; ++i)
          {
            pdf_bool_t escape;

            if (!should_escape_namechar (flags, data[i], &escape))
              {
                pdf_set_error (error,
                               PDF_EDOMAIN_BASE_TOKENISER,
                               PDF_EBADDATA,
                               "cannot write name token: "
                               "bad name");
                return PDF_FALSE;
              }

            if (escape)
              writer->pos += 2;  /* 2 hex characters */
          }

        pdf_buffer_rewind (writer->buffer);
        write_buffered_char_nocheck (writer, '/');
      }
      writer->state++;
      /* fall through */

    case 1:
      if (!start_token (writer,
                        PDF_FALSE, /* need_wspace */
                        writer->pos, /* encoded token length */
                        error))
        return PDF_FALSE;

      writer->pos = 0;
      writer->state++;
      /* fall through */

    case 2:
      while (writer->pos < size)
        {
          pdf_bool_t escape;
          pdf_char_t ch;

          ch = data[writer->pos];
          if (!should_escape_namechar (flags, ch, &escape))
            {
              pdf_set_error (error,
                             PDF_EDOMAIN_BASE_TOKENISER,
                             PDF_EBADDATA,
                             "cannot write name token: "
                             "bad name");
              return PDF_FALSE;
            }

          if (escape)
            {
              if (!reserve_buffer_space (writer, 3, error))
                return PDF_FALSE;

              write_buffered_char_nocheck (writer, '#');
              write_buffered_char_nocheck (writer, HEXCHAR (ch / 16));
              write_buffered_char_nocheck (writer, HEXCHAR (ch % 16));
            }
          else
            {
              if (!write_buffered_char (writer, ch, error))
                return PDF_FALSE;
            }
          writer->pos++;
        }
      writer->state++;
      /* fall through */

      case 3:
        return flush_buffer (writer, error);

      default:
        PDF_ASSERT_TRACE_NOT_REACHED ();
        return PDF_FALSE;
    }
}

static pdf_bool_t
write_keyword_token (pdf_token_writer_t  *writer,
                     const pdf_token_t   *token,
                     pdf_error_t        **error)
{
  const pdf_char_t *data;
  pdf_size_t size;

  data = pdf_token_get_keyword_data (token);
  size = pdf_token_get_keyword_size (token);

  switch (writer->state)
    {
    case 0:
      if (memchr (data, 0, size))
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_TOKENISER,
                         PDF_EBADDATA,
                         "cannot write keyword token: "
                         "contains NUL bytes");
          return PDF_FALSE;
        }
      writer->state++;
      /* fall through */

    case 1:
      if (!start_token (writer,
                        PDF_TRUE /* need_wspace */,
                        size,
                        error))
        return PDF_FALSE;

      writer->pos = 0;
      writer->state++;
      /* fall through */

    case 2:
      return write_data_using_pos (writer,
                                   (pdf_uchar_t *)pdf_token_get_keyword_data (token),
                                   size,
                                   error);

    default:
      PDF_ASSERT_TRACE_NOT_REACHED ();
      return PDF_FALSE;
    }
}

static pdf_bool_t
write_comment_token (pdf_token_writer_t  *writer,
                     const pdf_token_t   *token,
                     pdf_error_t        **error)
{
  const pdf_char_t *data;
  pdf_size_t size;

  data = pdf_token_get_comment_data (token);
  size = pdf_token_get_comment_size (token);

  switch (writer->state)
    {
    case 0:
      {
        /* A comment can't span multiple lines. */
        pdf_size_t i;

        for (i = 0; i < size; ++i)
          {
            if (pdf_is_eol_char (data[i]))
              {
                pdf_set_error (error,
                               PDF_EDOMAIN_BASE_TOKENISER,
                               PDF_EBADDATA,
                               "cannot write comment token: "
                               "comment must not span multiple lines");
                return PDF_FALSE;
              }
          }
      }
      writer->state++;
      /* fall through */

      case 1:
        if (!start_token (writer,
                          PDF_FALSE, /* need_wspace */
                          size + 1,
                          error))
          return PDF_FALSE;
        writer->state++;
        /* fall through */

      case 2:
        if (!write_char (writer, '%', error))
          return PDF_FALSE;
        writer->pos = 0;
        writer->state++;
        /* fall through */

      case 3:
        if (!write_data_using_pos (writer, (pdf_uchar_t *)data, size, error))
          return PDF_FALSE;
        writer->state++;
        /* fall through */

      case 4:
        return write_char (writer, '\n', error);

      default:
        PDF_ASSERT_TRACE_NOT_REACHED ();
        return PDF_FALSE;
    }
}

static pdf_bool_t
write_valueless_token (pdf_token_writer_t  *writer,
                       pdf_char_t           ch,
                       pdf_size_t           len,
                       pdf_error_t        **error)
{
  pdf_uchar_t buf[2] = {ch , ch};

  switch (writer->state)
    {
    case 0:
      if (!start_token (writer,
                        PDF_FALSE, /* need_wspace */
                        len,
                        error))
        return PDF_FALSE;
      writer->pos = 0;
      writer->state++;
      /* fall through */

    case 1:
      return write_data_using_pos (writer, buf, len, error);

    default:
      PDF_ASSERT_TRACE_NOT_REACHED ();
      return PDF_FALSE;
    }
}

/***** Token dispatching *****/

pdf_bool_t
pdf_token_writer_write (pdf_token_writer_t  *writer,
                        pdf_u32_t            flags,
                        const pdf_token_t   *token,
                        pdf_error_t        **error)
{
  pdf_bool_t rv;

  switch (pdf_token_get_type (token))
    {
    case PDF_TOKEN_INTEGER:
      rv = write_integer_token (writer, token, error);
      break;
    case PDF_TOKEN_REAL:
      rv = write_real_token (writer, token, error);
      break;
    case PDF_TOKEN_STRING:
      rv = write_string_token (writer, flags, token, error);
      break;
    case PDF_TOKEN_NAME:
      rv = write_name_token (writer, flags, token, error);
      break;
    case PDF_TOKEN_KEYWORD:
      rv = write_keyword_token (writer, token, error);
      break;
    case PDF_TOKEN_COMMENT:
      rv = write_comment_token (writer, token, error);
      break;
    case PDF_TOKEN_DICT_START:
      rv = write_valueless_token (writer, '<', 2, error);
      break;
    case PDF_TOKEN_DICT_END:
      rv = write_valueless_token (writer, '>', 2, error);
      break;
    case PDF_TOKEN_ARRAY_START:
      rv = write_valueless_token (writer, '[', 1, error);
      break;
    case PDF_TOKEN_ARRAY_END:
      rv = write_valueless_token (writer, ']', 1, error);
      break;
    case PDF_TOKEN_PROC_START:
      rv = write_valueless_token (writer, '{', 1, error);
      break;
    case PDF_TOKEN_PROC_END:
      rv = write_valueless_token (writer, '}', 1, error);
      break;
    default:
      PDF_ASSERT_TRACE_NOT_REACHED ();
      return PDF_FALSE;
    }

  if (rv)
    writer->state = 0;
  return rv;
}

/* End of pdf-token-writer.c */

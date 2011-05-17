/* -*- mode: C -*-
 *
 *       File:         pdf-token-writer.c
 *       Date:         Wed Sep 23 04:37:51 2009
 *
 *       GNU PDF Library - Stream token writer
 *
 */

/* Copyright (C) 2009 Free Software Foundation, Inc. */

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
#include <inttypes.h>
#include <math.h>

#include <pdf-token-writer.h>
#include <unistr.h>

pdf_status_t
pdf_token_writer_new (pdf_stm_t *stm, pdf_token_writer_t *writer)
{
  pdf_status_t err;
  pdf_token_writer_t new_tokw;

  err = PDF_ENOMEM;
  new_tokw = pdf_alloc (sizeof (*new_tokw));
  if (!new_tokw)
    goto fail;

  /* determine the current locale's decimal point
   * (avoid using localeconv since it may not be thread-safe) */
  new_tokw->decimal_point = NULL;
  {
    int len;
    char decpt[16];

    err = PDF_ERROR;
    len = snprintf (decpt, sizeof (decpt), "%#.0f", 1.0);
    if (len <= 0 || (pdf_size_t)len >= sizeof (decpt))  /* shouldn't happen */
      goto fail;

    err = PDF_ENOMEM;
    new_tokw->decimal_point = pdf_alloc (len);
    if (!new_tokw->decimal_point)
      goto fail;

    /* this copies the trailing '\0' due to the starting offset */
    memcpy (new_tokw->decimal_point, &decpt[1], len);
  }

  /* set max_line_length to 0 for no maximum */
  new_tokw->max_line_length = PDF_TOKW_MAX_LINE_LENGTH;

  err = PDF_ENOMEM;
  new_tokw->buffer = pdf_buffer_new (PDF_TOKW_BUFFER_SIZE, NULL);
  /* TODO: get and propagate error */
  if (!new_tokw->buffer)
    goto fail;

  err = PDF_EBADDATA;
  if (!stm || pdf_stm_get_mode (stm) != PDF_STM_WRITE)
    goto fail;
  new_tokw->stream = stm;

  pdf_token_writer_reset (new_tokw);

  *writer = new_tokw;
  return PDF_OK;

fail:
  if (new_tokw)
    pdf_dealloc (new_tokw->decimal_point);
  pdf_dealloc (new_tokw);

  return err;
}

pdf_status_t
pdf_token_writer_reset (pdf_token_writer_t writer)
{
  writer->stage = 0;
  writer->in_keyword = PDF_FALSE;
  writer->line_length = 0;
  return PDF_OK;
}

pdf_status_t
pdf_token_writer_destroy (pdf_token_writer_t writer)
{
  if (!writer) return PDF_EBADDATA;

  assert (writer->buffer);
  if (writer->buffer)
    pdf_buffer_destroy (writer->buffer);
  pdf_dealloc (writer->decimal_point);
  pdf_dealloc (writer);

  return PDF_OK;
}


/***** Unbuffered output *****/

/* Write data to the stream.  All output passes through this function. */
static pdf_status_t
write_data (pdf_token_writer_t writer, const pdf_uchar_t *data,
            pdf_size_t len, pdf_size_t *written)
{
  pdf_size_t i;
  pdf_status_t rv;
  pdf_error_t *inner_error = NULL;

  if (!pdf_stm_write (writer->stream, data, len, written, &inner_error) &&
      inner_error)
    {
      /* TODO: Propagate error */
      rv = pdf_error_get_status (inner_error);
      pdf_error_destroy (inner_error);
      return rv;
    }

  for (i = 0; i < *written; ++i)
    {
      pdf_uchar_t ch = data[i];

      ++writer->line_length;
      if (pdf_is_eol_char (ch))
        writer->line_length = 0;

      writer->in_keyword = pdf_is_regular_char (ch);
    }
  return PDF_OK;
}

/* Write a single character. */
static INLINE pdf_status_t
write_char (pdf_token_writer_t writer, pdf_uchar_t ch)
{
  pdf_size_t written;
  return write_data (writer, &ch, 1, &written);
}

/* Write data starting at writer->pos, incrementing writer->pos as needed. */
static INLINE pdf_status_t
write_data_using_pos (pdf_token_writer_t writer,
                      const pdf_uchar_t *data, pdf_size_t len)
{
  pdf_status_t rv;
  pdf_size_t written;
  if (writer->pos > len)
    return PDF_EBADDATA;

  while (writer->pos < len)
    {
      rv = write_data (writer, data + writer->pos, len - writer->pos,
                       &written);
      if (rv != PDF_OK)
        return rv;

      writer->pos += written;
    }
  return PDF_OK;
}


/***** Buffered output, buffer management *****/

/* Write all buffered data to the stream. */
static pdf_status_t
flush_buffer (pdf_token_writer_t writer)
{
  pdf_buffer_t *buf = writer->buffer;
  pdf_size_t len;
  while ( (len = buf->wp - buf->rp) > 0 )
    {
      pdf_size_t written;
      pdf_status_t rv = write_data (writer,
                                    buf->data + buf->rp,
                                    len, &written);
      if (rv != PDF_OK)
        return rv;

      buf->rp += written;
    }

  pdf_buffer_rewind (buf);
  return PDF_OK;
}

/* Flush the buffer if there are less than 'len' bytes free. */
static INLINE pdf_status_t
reserve_buffer_space (pdf_token_writer_t writer, pdf_size_t len)
{
  if (writer->buffer->wp + len > writer->buffer->size)
    {
      pdf_status_t rv = flush_buffer (writer);
      if (rv != PDF_OK)
        return rv;

      assert (len < writer->buffer->size);
      assert (writer->buffer->wp == 0);
    }
  return PDF_OK;
}

/* Write a character into the buffer; this assumes it will fit. */
static INLINE void
write_buffered_char_nocheck (pdf_token_writer_t writer, pdf_char_t ch)
{
  writer->buffer->data[writer->buffer->wp++] = ch;
  if (pdf_is_eol_char(ch))
    writer->buffered_line_length = 0;
  else
    ++writer->buffered_line_length;
}

/* Write a character into the buffer.  The buffer is flushed only if
 * there's no room to write the character. */
static INLINE pdf_status_t
write_buffered_char (pdf_token_writer_t writer, pdf_char_t ch)
{
  pdf_status_t rv = reserve_buffer_space (writer, 1);
  if (rv == PDF_OK)
    write_buffered_char_nocheck (writer, ch);
  return rv;
}


/***** Misc. utility functions *****/

/* Takes a number from 0 to 15 and returns the ASCII code for the
 * corresponding hexadecimal digit. */
static INLINE pdf_char_t
hexchar (pdf_char_t value)
{
  if (value < 10)
    return 48 + value;       /* '0'--'9' */
  else if (value < 16)
    return 65 + value - 10;  /* 'A'--'F' */
  return 255;
}

/* Prepare to write a new token, adding some whitespace if necessary. */
static pdf_status_t
start_token (pdf_token_writer_t writer, pdf_bool_t need_wspace,
             pdf_size_t len)
{
  pdf_bool_t add_wspace = (need_wspace && writer->in_keyword);
  pdf_char_t wspace_char = 32;  /* space */

  if (add_wspace)
    ++len;

  /* If the token would make this line too long, start a new line. */
  if (writer->line_length + len > writer->max_line_length
       && writer->max_line_length > 0)
    {
      add_wspace = PDF_TRUE;
      wspace_char = 10;  /* newline */
    }

  if (add_wspace)
    {
      pdf_status_t rv = write_char (writer, wspace_char);
      if (rv != PDF_OK) return rv;
    }

  return PDF_OK;
}


/***** Numeric tokens *****/

/* Encode snprintf output for PDF.  'len' is the return value of snprintf.
 * Re-encodes bytes 0 to 'len' of writer->buffer and resets buffer->rp/wp. */
static pdf_bool_t
encode_buffer_number (pdf_token_writer_t writer, int len)
{
  pdf_buffer_t *buf = writer->buffer;
  if (len < 0 || len >= buf->size)
    return PDF_FALSE;  /* snprintf failed, or truncated its output. */

  buf->wp = buf->rp = 0;
  while (buf->rp < len)
    {
      char ch = buf->data[buf->rp];
      if (ch == '-')
        {
          ++buf->rp;
          buf->data[buf->wp++] = 45;  /* '-' */
        }
      else if (ch >= '0' && ch <= '9')
        {
          ++buf->rp;
          buf->data[buf->wp++] = 48 + (ch - '0');
        }
      else
        {
          /* This should be a decimal point; check it. */
          pdf_size_t declen = strlen (writer->decimal_point);
          int cmp = memcmp (buf->data + buf->rp,
                            writer->decimal_point, declen);
          if (cmp != 0)
            return PDF_FALSE;  /* unexpected char */

          buf->rp += declen;
          buf->data[buf->wp++] = 46;  /* '.' */
        }
    }
  buf->rp = 0;
  return PDF_TRUE;  /* success */
}

static INLINE pdf_status_t
write_integer_token (pdf_token_writer_t writer, pdf_token_t token)
{
  pdf_status_t rv;
  switch (writer->stage)
    {
      case 0:
        {
          pdf_i32_t value = pdf_token_get_integer_value (token);
          int len = snprintf ((pdf_char_t *)writer->buffer->data,
                              writer->buffer->size, "%"PRId32, value);
          if (!encode_buffer_number (writer, len)) return PDF_ERROR;
        }
        ++writer->stage;  /* fall through */
      case 1:
        rv = start_token (writer, PDF_TRUE /*need_wspace*/,
                          writer->buffer->wp);
        if (rv != PDF_OK) return rv;
        ++writer->stage;  /* fall through */
      case 2:
        return flush_buffer (writer);
      default:
        return PDF_EBADDATA;
    }
}

static INLINE pdf_status_t
write_real_token (pdf_token_writer_t writer, pdf_token_t token)
{
  pdf_status_t rv;
  switch (writer->stage)
    {
      case 0:
        {
          pdf_buffer_t *buf = writer->buffer;
          pdf_real_t value = pdf_token_get_real_value (token);
          if (isnan(value) || isinf(value))
            return PDF_EBADDATA;

          /* The '#' flag forces snprintf to write a decimal point. */
          int len = snprintf ((pdf_char_t *)buf->data,
                              buf->size, "%#f", (double)value);
          if (!encode_buffer_number (writer, len)) return PDF_ERROR;

          /* strip trailing zeroes */
          while (buf->wp && buf->data[buf->wp-1] == 48 /* '0' */)
            --buf->wp;
        }
        ++writer->stage;  /* fall through */
      case 1:
        rv = start_token (writer, PDF_TRUE /*need_wspace*/,
                          writer->buffer->wp);
        if (rv != PDF_OK) return rv;
        ++writer->stage;  /* fall through */
      case 2:
        return flush_buffer (writer);
      default:
        return PDF_EBADDATA;
    }
}


/***** String tokens *****/

static INLINE pdf_bool_t
should_escape_strchar (pdf_u32_t flags, pdf_char_t ch,
                       pdf_bool_t quote_parens, pdf_bool_t is_utf8)
{
  if (ch == 92 /* '\\' */ || ch == 13 /* CR */)
    return PDF_TRUE;
  if (ch == 40 /* '(' */ || ch == 41 /* ')' */)
    return quote_parens;

  if (flags & PDF_TOKEN_READABLE_STRINGS)
    {
      if (ch == 127 || (ch < 32 && ch != 10)
           || (ch >= 128 && !is_utf8))
        return PDF_TRUE;
    }

  return PDF_FALSE;
}

static INLINE int
str_escape_len (const pdf_char_t *data, pdf_size_t len, pdf_size_t pos)
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

  if (pos+1 < len)
    {
      if (data[pos+1] >= 48 && data[pos+1] <= 57)  /* '0'..'9' */
        return 4;  /* need to write a 3-character octal number */
    }

  return (data[pos] >= 010) ? 3 : 2;
}

static void
scan_string (pdf_token_writer_t writer, pdf_u32_t flags,
             const pdf_char_t *data, pdf_size_t len, pdf_bool_t *use_hex)
{
  /* Match parentheses, and determine the portion of the string
   * in which they should be quoted. */
  writer->paren_quoting_start = 0;
  writer->paren_quoting_end = len;
  pdf_size_t i, j;
  for (i = 0; i < len; ++i)
    {
      if (data[i] == 40)       /* '(' */
        {
          for (j = writer->paren_quoting_end - 1; j > i; --j)
            {
              /* find a matching ')' */
              if (data[j] == 41)
                {
                  writer->paren_quoting_end = j;
                  writer->paren_quoting_start = i + 1;
                  break;
                }
            }
        }
    }

  /* Determine the size of the escaped string. */
  writer->utf8 = (flags & PDF_TOKEN_READABLE_STRINGS)
    && (u8_check ((uint8_t *) data, len) == NULL);
  pdf_size_t enc_bytes = 0;
  for (i = 0; i < len; ++i)
    {
      pdf_bool_t quote_parens = (i >= writer->paren_quoting_start
                                  && i < writer->paren_quoting_end);
      if (should_escape_strchar (flags, data[i], quote_parens, writer->utf8))
        enc_bytes += str_escape_len (data, len, i);
      else
        ++enc_bytes;
    }
  *use_hex = (enc_bytes > len*2);
}

static INLINE pdf_status_t
write_string_char (pdf_token_writer_t writer, pdf_u32_t flags,
                   const pdf_char_t *data, pdf_char_t len,
                   pdf_size_t pos)
{
  assert (len > 0);
  pdf_status_t rv;
  const pdf_char_t *output = data + pos;
  pdf_size_t outlen = 1;
  pdf_char_t esc[4] = {92 /* '\\' */, 0, 0, 0};

  pdf_char_t ch = data[pos];
  pdf_bool_t quote_parens = (pos >= writer->paren_quoting_start
                              && pos < writer->paren_quoting_end);
  if (should_escape_strchar (flags, ch, quote_parens, writer->utf8))
    {
      /* escape the character */
      output = esc;
      outlen = 2;
      switch (ch)
        {
          case  8: esc[1] =  98; break; /* 'b' */
          case  9: esc[1] = 116; break; /* 't' */
          case 10: esc[1] = 110; break; /* 'n' */
          case 12: esc[1] = 102; break; /* 'f' */
          case 13: esc[1] = 114; break; /* 'r' */
          case 40:  /* '('; fall through */
          case 41:  /* ')'; fall through */
          case 92:  /* '\\' */
            esc[1] = ch;
            break;
          default: /* use an octal escape */
            {
              pdf_size_t digits;
              pdf_char_t nextch = (pos+1 < len) ? data[pos+1] : 0;
              if (nextch >= 48 && nextch <= 57)  /* '0'..'9' */
                digits = 3;  /* must use 3 octal characters */
              else if (ch > 0100) digits = 3;
              else if (ch > 010) digits = 2;
              else digits = 1;

              outlen = 1;
              switch (digits)
                {
                  /* fall through each case */
                  case 3: esc[outlen++] = hexchar (ch / 0100);
                  case 2: esc[outlen++] = hexchar ((ch % 0100) / 010);
                  case 1: esc[outlen++] = hexchar (ch % 010);
                }
              }
        }
    }

  /* If the line will be too long, split it (the length cannot be equal to
   * the maximum, since this would leave no room for the backslash). */
  if (writer->max_line_length > 0 && !pdf_is_eol_char (output[0])
       && writer->buffered_line_length + outlen >= writer->max_line_length)
    {
      rv = reserve_buffer_space (writer, 2);
      if (rv != PDF_OK) return rv;
      write_buffered_char_nocheck (writer, 92);  /* '\\' */
      write_buffered_char_nocheck (writer, 10);  /* newline */
      assert (writer->buffered_line_length == 0);
    }

  rv = reserve_buffer_space (writer, outlen);
  if (rv == PDF_OK)
    {
      pdf_size_t i;
      for (i = 0; i < outlen; ++i)
        write_buffered_char_nocheck (writer, output[i]);
    }
  return rv;
}

static INLINE pdf_status_t
write_string_token (pdf_token_writer_t writer, pdf_u32_t flags,
                    pdf_token_t token)
{
  pdf_status_t rv;
  const pdf_char_t *data = pdf_token_get_string_data (token);
  pdf_size_t size = pdf_token_get_string_size (token);

  switch (writer->stage)
    {
      case 0:
        {
          pdf_bool_t use_hex = (flags & PDF_TOKEN_HEX_STRINGS);
          if (!use_hex)
            scan_string (writer, flags, data, size, &use_hex);

          if (use_hex)
            goto hexstring_start;
        }
        ++writer->stage;  /* fall through */
      case 1:
        {
          /* Passing a correct length to start_token isn't important
           * since we can split the string across multiple lines. */
          pdf_size_t dummy_len = PDF_MIN(20, 2 + size);
          rv = start_token (writer, PDF_FALSE /*need_wspace*/, dummy_len);
          if (rv != PDF_OK) return rv;
        }

        pdf_buffer_rewind (writer->buffer);
        writer->buffered_line_length = writer->line_length;
        write_buffered_char_nocheck (writer, 40 /* '(' */);
        writer->pos = 0;
        ++writer->stage;  /* fall through */
      case 2:
        while (writer->pos < size)
          {
            rv = write_string_char (writer, flags, data, size, writer->pos);
            if (rv != PDF_OK) return rv;

            ++writer->pos;
          }
        ++writer->stage;  /* fall through */
      case 3:
        rv = write_buffered_char (writer, 41 /* ')' */);
        if (rv != PDF_OK) return rv;
        ++writer->stage;  /* fall through */
      case 4:
        return flush_buffer (writer);

      /*** hex strings ***/
hexstring_start:
      writer->stage = 101;
      case 101:
        {
          pdf_size_t dummy_len = PDF_MIN(20, 2 + size*2);
          rv = start_token (writer, PDF_FALSE /*need_wspace*/, dummy_len);
          if (rv != PDF_OK) return rv;
        }

        pdf_buffer_rewind (writer->buffer);
        writer->buffered_line_length = writer->line_length;
        write_buffered_char_nocheck (writer, 60 /* '<' */);
        writer->pos = 0;
        ++writer->stage;  /* fall through */
      case 102:
        while (writer->pos < size)
          {
            /* If this line would be too long, start a new one. */
            if (writer->buffered_line_length + 2 > writer->max_line_length
                 && writer->max_line_length > 0)
              {
                rv = write_buffered_char (writer, 10);  /* newline */
                if (rv != PDF_OK) return rv;
                assert (writer->buffered_line_length == 0);
              }

            pdf_char_t ch = data[writer->pos];
            rv = reserve_buffer_space (writer, 2);
            if (rv != PDF_OK) return rv;

            write_buffered_char_nocheck (writer, hexchar (ch / 16));
            if (writer->pos == size-1 && (ch%16) == 0)
              ;  /* don't write a final 0 */
            else
              write_buffered_char_nocheck (writer, hexchar (ch % 16));
            ++writer->pos;
          }
        ++writer->stage;  /* fall through */
      case 103:
        rv = write_buffered_char (writer, 62 /* '>' */);
        if (rv != PDF_OK) return rv;
        ++writer->stage;  /* fall through */
      case 104:
        return flush_buffer (writer);
      default:
        return PDF_EBADDATA;
    }
}


/***** Other tokens *****/

static INLINE pdf_status_t
should_escape_namechar (pdf_u32_t flags, pdf_char_t ch, pdf_bool_t *escape)
{
  if (!ch)
    return PDF_EBADDATA;

  *escape = !pdf_is_regular_char (ch);
  if (flags & PDF_TOKEN_NO_NAME_ESCAPES)
    {
      if (*escape)
        return PDF_EBADDATA;
    }
  else
    {
      *escape = *escape || ch == 35 /* '#' */
                || ch < 33 || ch >= 127;
    }
  return PDF_OK;
}

static INLINE pdf_status_t
write_name_token (pdf_token_writer_t writer, pdf_u32_t flags,
                  pdf_token_t token)
{
  pdf_status_t rv;
  pdf_size_t size = pdf_token_get_name_size (token);
  const pdf_char_t *data = pdf_token_get_name_data (token);
  switch (writer->stage)
    {
      case 0:
        /* Validate the name; also calculate the encoded size
         * and store it in ->pos temporarily. */
        writer->pos = 1 + size;
        {
          pdf_size_t i;
          for (i = 0; i < size; ++i)
            {
              pdf_bool_t escape;
              rv = should_escape_namechar (flags, data[i], &escape);
              if (rv != PDF_OK) return rv;  /* bad name */

              if (escape)
                writer->pos += 2;  /* 2 hex characters */
            }
        }
        pdf_buffer_rewind (writer->buffer);
        write_buffered_char_nocheck (writer, 47 /* '/' */);
        ++writer->stage;  /* fall through */
      case 1:
        rv = start_token (writer, PDF_FALSE /*need_wspace*/,
                          writer->pos /* encoded token length */);
        if (rv != PDF_OK) return rv;

        writer->pos = 0;
        ++writer->stage;  /* fall through */
      case 2:
        while (writer->pos < size)
          {
            pdf_bool_t escape;
            pdf_char_t ch = data[writer->pos];
            rv = should_escape_namechar (flags, ch, &escape);
            if (rv != PDF_OK) return rv;  /* bad name */

            if (escape)
              {
                rv = reserve_buffer_space (writer, 3);
                if (rv != PDF_OK) return rv;

                write_buffered_char_nocheck (writer, 35 /* '#' */);
                write_buffered_char_nocheck (writer, hexchar (ch / 16));
                write_buffered_char_nocheck (writer, hexchar (ch % 16));
              }
            else
              {
                rv = write_buffered_char (writer, ch);
                if (rv != PDF_OK) return rv;
              }
            ++writer->pos;
          }
        ++writer->stage;  /* fall through */
      case 3:
        return flush_buffer (writer);
      default:
        return PDF_EBADDATA;
    }
}

static INLINE pdf_status_t
write_keyword_token (pdf_token_writer_t writer, pdf_token_t token)
{
  const pdf_char_t *data = pdf_token_get_keyword_data (token);
  pdf_size_t size = pdf_token_get_keyword_size (token);
  pdf_status_t rv;
  switch (writer->stage)
    {
      case 0:
        if (memchr (data, 0, size))
          return PDF_EBADDATA;  /* data contains a null byte */
        ++writer->stage;  /* fall through */
      case 1:
        rv = start_token (writer, PDF_TRUE /*need_wspace*/, size);
        if (rv != PDF_OK) return rv;

        writer->pos = 0;
        ++writer->stage;  /* fall through */
      case 2:
        return write_data_using_pos (writer,
                                     (pdf_uchar_t *)pdf_token_get_keyword_data (token),
                                     size);
      default:
        return PDF_EBADDATA;
    }
}

static INLINE pdf_status_t
write_comment_token (pdf_token_writer_t writer, pdf_token_t token)
{
  const pdf_char_t *data = pdf_token_get_comment_data (token);
  pdf_size_t size = pdf_token_get_comment_size (token);
  pdf_status_t rv;
  switch (writer->stage)
    {
      case 0:
        {
          /* A comment can't span multiple lines. */
          pdf_size_t i;
          for (i = 0; i < size; ++i)
            {
              if (pdf_is_eol_char(data[i]))
              return PDF_EBADDATA;
            }
        }
        ++writer->stage;  /* fall through */
      case 1:
        rv = start_token (writer, PDF_FALSE /*need_wspace*/, size+1);
        if (rv != PDF_OK) return rv;
        ++writer->stage;  /* fall through */
      case 2:
        rv = write_char (writer, 37 /* '%' */);
        if (rv != PDF_OK) return rv;

        writer->pos = 0;
        ++writer->stage;  /* fall through */
      case 3:
        rv = write_data_using_pos (writer, (pdf_uchar_t *)data, size);
        if (rv != PDF_OK) return rv;
        ++writer->stage;  /* fall through */
      case 4:
        return write_char (writer, 10 /* '\n' */);
      default:
        return PDF_EBADDATA;
    }
}

static INLINE pdf_status_t
write_valueless_token (pdf_token_writer_t writer,
                       pdf_char_t ch, pdf_size_t len)
{
  pdf_char_t buf[2] = {ch,ch};
  pdf_status_t rv;
  assert (len == 1 || len == 2);

  switch (writer->stage)
    {
      case 0:
        rv = start_token (writer, PDF_FALSE /*need_wspace*/, len);
        if (rv != PDF_OK) return rv;

        writer->pos = 0;
        ++writer->stage;  /* fall through */
      case 1:
        return write_data_using_pos (writer, (pdf_uchar_t *)buf, len);
      default:
        return PDF_EBADDATA;
    }
}

/***** Token dispatching *****/

pdf_status_t
pdf_token_write (pdf_token_writer_t writer, pdf_u32_t flags, pdf_token_t token)
{
  pdf_status_t rv;
  switch (pdf_token_get_type (token))
    {
      case PDF_TOKEN_INTEGER:
        rv = write_integer_token (writer, token);
        break;
      case PDF_TOKEN_REAL:
        rv = write_real_token (writer, token);
        break;
      case PDF_TOKEN_STRING:
        rv = write_string_token (writer, flags, token);
        break;
      case PDF_TOKEN_NAME:
        rv = write_name_token (writer, flags, token);
        break;
      case PDF_TOKEN_KEYWORD:
        rv = write_keyword_token (writer, token);
        break;
      case PDF_TOKEN_COMMENT:
        rv = write_comment_token (writer, token);
        break;
      case PDF_TOKEN_DICT_START:
        rv = write_valueless_token (writer, 60 /* '<' */, 2);
        break;
      case PDF_TOKEN_DICT_END:
        rv = write_valueless_token (writer, 62 /* '>' */, 2);
        break;
      case PDF_TOKEN_ARRAY_START:
        rv = write_valueless_token (writer, 91 /* '[' */, 1);
        break;
      case PDF_TOKEN_ARRAY_END:
        rv = write_valueless_token (writer, 93 /* ']' */, 1);
        break;
      case PDF_TOKEN_PROC_START:
        rv = write_valueless_token (writer, 123 /* '{' */, 1);
        break;
      case PDF_TOKEN_PROC_END:
        rv = write_valueless_token (writer, 125 /* '}' */, 1);
        break;
      default:
        assert (0);
        return PDF_ERROR;
    }

  if (rv == PDF_OK)
    writer->stage = 0;
  return rv;
}

/* End of pdf-token-writer.c */

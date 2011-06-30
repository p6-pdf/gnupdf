/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-ahex.c
 *       Date:         Fri Jul 13 17:08:41 2007
 *
 *       GNU PDF Library - ASCII Hex decoder filter
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

#include <pdf-types.h>
#include <pdf-types-buffer.h>
#include <pdf-hash.h>
#include <pdf-stm-f-ahex.h>

/* Define AHEX encoder */
PDF_STM_FILTER_DEFINE (pdf_stm_f_ahexenc_get,
                       stm_f_ahex_init,
                       stm_f_ahexenc_apply,
                       stm_f_ahex_deinit);

/* Define AHEX decoder */
PDF_STM_FILTER_DEFINE (pdf_stm_f_ahexdec_get,
                       stm_f_ahex_init,
                       stm_f_ahexdec_apply,
                       stm_f_ahex_deinit);

#define PDF_STM_F_AHEX_LINE_WIDTH 60

/* Internal state, same for encoder and decoder */
struct pdf_stm_f_ahex_s
{
  pdf_i32_t last_nibble;
  pdf_size_t written_bytes;
};

static pdf_u32_t pdf_stm_f_ahex_white_p (pdf_u32_t hex);

static pdf_u32_t pdf_stm_f_ahex_hex_p (pdf_u32_t hex);

static pdf_u32_t pdf_stm_f_ahex_hex2int (pdf_u32_t hex);

static pdf_uchar_t pdf_stm_f_ahex_int2hex (pdf_u32_t n);

/* Common implementation */

static pdf_bool_t
stm_f_ahex_init (const pdf_hash_t  *params,
                 void             **state,
                 pdf_error_t      **error)
{
  struct pdf_stm_f_ahex_s *filter_state;

  /* This filter uses no parameters */
  /* Allocate the internal state structure */
  filter_state = pdf_alloc (sizeof (struct pdf_stm_f_ahex_s));
  if (!filter_state)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "cannot create ASCII-Hex encoder/decoder internal state: "
                     "couldn't allocate %lu bytes",
                     (unsigned long)sizeof (struct pdf_stm_f_ahex_s));
      return PDF_FALSE;
    }

  /* Initialize fields */
  filter_state->last_nibble = -1;
  filter_state->written_bytes = 0;

  *state = (void *)filter_state;

  return PDF_TRUE;
}

static void
stm_f_ahex_deinit (void *state)
{
  pdf_dealloc (state);
}

/* Encoder implementation */

static enum pdf_stm_filter_apply_status_e
stm_f_ahexenc_apply (void          *state,
                     pdf_buffer_t  *in,
                     pdf_buffer_t  *out,
                     pdf_bool_t     finish,
                     pdf_error_t  **error)
{
  struct pdf_stm_f_ahex_s *filter_state;
  pdf_uchar_t first_nibble;
  pdf_uchar_t second_nibble;
  pdf_uchar_t in_char;

  if (finish)
    {
      if (pdf_buffer_full_p (out))
        return PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT;
      out->data[out->wp++] = '>';
      return PDF_STM_FILTER_APPLY_STATUS_EOF;
    }

  filter_state = (struct pdf_stm_f_ahex_s *) state;

  while (!pdf_buffer_full_p (out))
    {
      if ((filter_state->written_bytes != 0) &&
          (filter_state->written_bytes % PDF_STM_F_AHEX_LINE_WIDTH) == 0)
        {
          /* Write down a newline character */
          out->data[out->wp] = '\n';
          out->wp++;
          filter_state->written_bytes = 0;
          continue;
        }

      /* Write down any pending nibble, if needed, without consuming
         any input byte */
      if (filter_state->last_nibble != -1)
        {
          out->data[out->wp] = filter_state->last_nibble;
          out->wp++;
          filter_state->written_bytes++;
          filter_state->last_nibble = -1;
          continue;
        }

      /* Try to consume an input byte */
      if (!pdf_buffer_eob_p (in))
        {
          /* For each byte in the input we should generate two bytes in the
             output. */
          in_char = in->data[in->rp];

          /* Determine the hex digits to write for this input character */
          if (filter_state->last_nibble != -1)
            {
              first_nibble = (pdf_uchar_t) filter_state->last_nibble;
              second_nibble = pdf_stm_f_ahex_int2hex (in_char >> 4);
            }
          else
            {
              first_nibble = pdf_stm_f_ahex_int2hex (in_char >> 4);
              second_nibble = pdf_stm_f_ahex_int2hex (in_char);
            }
          in->rp++;

          /* Write the hex digits into the output buffer, if
             possible */

          /* First nibble */
          out->data[out->wp] = first_nibble;
          out->wp++;
          filter_state->written_bytes++;

          /* Maybe write the second nibble */
          if (pdf_buffer_full_p (out))
            {
              filter_state->last_nibble = second_nibble;
            }
          else
            {
              out->data[out->wp] = second_nibble;
              out->wp++;
              filter_state->written_bytes++;
            }
        }
      else
        {
          /* We need more input */
          break;
        }
    }

  return (pdf_buffer_full_p (out) ?
          PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT :
          PDF_STM_FILTER_APPLY_STATUS_NO_INPUT);
}

static enum pdf_stm_filter_apply_status_e
stm_f_ahexdec_apply (void          *state,
                     pdf_buffer_t  *in,
                     pdf_buffer_t  *out,
                     pdf_bool_t     finish,
                     pdf_error_t  **error)
{
  struct pdf_stm_f_ahex_s *filter_state;

  filter_state = (struct pdf_stm_f_ahex_s *) state;

  while (!pdf_buffer_full_p (out))
    {
      if (pdf_buffer_eob_p (in))
        {
          /* Need more input */
          break;
        }

      /* Skip white characters */
      if (pdf_stm_f_ahex_white_p ((pdf_u32_t) in->data[in->rp]))
        {
          in->rp++;
          continue;
        }

      /* Detect the end of the hex data */
      if (in->data[in->rp] == '>')
        {
          if (filter_state->last_nibble == -1)
            {
              /* We are done :'D */
              in->rp++;
            }
          else
            {
              /* Found an even number of hex digits. We assume that
                 the second nibble is 0, so generate a byte of data
                 and finish */
              out->data[out->wp] =
                pdf_stm_f_ahex_hex2int (filter_state->last_nibble) << 4;
              out->wp++;
              filter_state->last_nibble = -1;
            }

          return PDF_STM_FILTER_APPLY_STATUS_EOF;
        }

      /* Detect an invalid character */
      if (!pdf_stm_f_ahex_hex_p ((pdf_u32_t) in->data[in->rp]))
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_STM,
                         PDF_ERROR,
                         "Invalid character found in ASCII-Hex decoder: %u",
                         (pdf_u32_t) in->data[in->rp]);
          return PDF_STM_FILTER_APPLY_STATUS_ERROR;
        }

      /* Process this character. This is the first or the second part
         of a mibble. */
      if (filter_state->last_nibble == -1)
        {
          pdf_u32_t first_nibble;

          /* Get the first nibble */
          first_nibble = (pdf_u32_t) in->data[in->rp];
          in->rp++;

          filter_state->last_nibble = first_nibble;
        }
      else
        {
          pdf_u32_t first_nibble;
          pdf_u32_t second_nibble;

          /* Get the second nibble */
          second_nibble = (pdf_u32_t) in->data[in->rp];
          in->rp++;

          /* Generate one byte of data */
          first_nibble = filter_state->last_nibble;
          out->data[out->wp] = ((pdf_stm_f_ahex_hex2int (first_nibble) << 4)
                                + pdf_stm_f_ahex_hex2int (second_nibble));
          out->wp++;

          filter_state->last_nibble = -1;
        }
    }

  return (pdf_buffer_eob_p (in) ?
          PDF_STM_FILTER_APPLY_STATUS_NO_INPUT :
          PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT);
}

/* Helper functions */

static pdf_u32_t
pdf_stm_f_ahex_white_p (pdf_u32_t hex)
{
  return ((hex == '\0')    /* Null */
          || (hex == '\t') /* Tab */
          || (hex == '\n') /* Line feed */
          || (hex == '\f') /* Form feed */
          || (hex == '\r') /* Carriage return */
          || (hex == 32)); /* Space character */
}

static pdf_u32_t
pdf_stm_f_ahex_hex_p (pdf_u32_t hex)
{
  return (((hex >= 'a') && (hex <= 'f')) ||
          ((hex >= 'A') && (hex <= 'F')) ||
          ((hex >= '0') && (hex <= '9')));
}

static const pdf_uchar_t to_hex[16] = "0123456789ABCDEF";

static pdf_uchar_t
pdf_stm_f_ahex_int2hex (pdf_u32_t hex)
{
  return to_hex[hex & 0x0f];
}

static pdf_u32_t
pdf_stm_f_ahex_hex2int (pdf_u32_t hex)
{
  if ((hex >= 'a') && (hex <= 'f'))
    return (hex - 'a') + 0xA;

  if ((hex >= 'A') && (hex <= 'F'))
    return (hex - 'A') + 0xA;

  if ((hex >= '0') && (hex <= '9'))
    return (hex - '0');

  return -1;
}

/* End of pdf_stm_f_ahex.c */

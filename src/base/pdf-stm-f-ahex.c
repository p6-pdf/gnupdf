/* -*- mode: C -*- Time-stamp: "08/09/27 00:50:51 jemarch"
 *
 *       File:         pdf-stm-f-ahex.c
 *       Date:         Fri Jul 13 17:08:41 2007
 *
 *       GNU PDF Library - ASCII Hex decoder filter
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
#include <pdf-stm-f-ahex.h>

static pdf_u32_t pdf_stm_f_ahex_white_p (pdf_u32_t hex);
static pdf_u32_t pdf_stm_f_ahex_hex_p (pdf_u32_t hex);
static pdf_u32_t pdf_stm_f_ahex_hex2int (pdf_u32_t hex);
static pdf_char_t pdf_stm_f_ahex_int2hex (pdf_u32_t n);

pdf_status_t
pdf_stm_f_ahexenc_init (pdf_hash_t params,
                        void **state)
{
  pdf_stm_f_ahexenc_t filter_state;

  /* This filter uses no parameters */
  /* Allocate the internal state structure */
  filter_state = pdf_alloc (sizeof (struct pdf_stm_f_ahexenc_s));

  if (state == NULL)
    {
      return PDF_ERROR;
    }
  
  /* Initialize fields */
  filter_state->last_nibble = -1;
  filter_state->written_bytes = 0;

  *state = (void *) filter_state;

  return PDF_OK;
}

#define PDF_STM_F_AHEX_LINE_WIDTH 60

pdf_status_t
pdf_stm_f_ahexenc_apply (pdf_hash_t params,
                         void *state,
                         pdf_stm_buffer_t in,
                         pdf_stm_buffer_t out,
                         pdf_bool_t finish_p)
{
  pdf_status_t ret;
  pdf_stm_f_ahexenc_t filter_state;
  pdf_char_t first_nibble;
  pdf_char_t second_nibble;
  pdf_char_t in_char;
  pdf_bool_t wrote_newline;
  
  filter_state = (pdf_stm_f_ahexenc_t) state;
  wrote_newline = PDF_FALSE;
  while ((!pdf_stm_buffer_eob_p (in)) &&
         (!pdf_stm_buffer_full_p (out)))
    {
      if ((!wrote_newline) &&
          (filter_state->written_bytes != 0) &&
          ((filter_state->written_bytes % PDF_STM_F_AHEX_LINE_WIDTH) == 0))
        {
          /* Insert a new line character */
          out->data[out->wp] = '\n';
          out->wp++;
          wrote_newline = PDF_TRUE;

          continue;
        }
      else
        {
          wrote_newline = PDF_FALSE;
        }

      /* For each byte in the input we should generate two bytes in the
         output. */
      in_char = in->data[in->rp];

      /* Determine the hex digits to write for this input character */
      if (filter_state->last_nibble != -1)
        {
          first_nibble = (pdf_char_t) filter_state->last_nibble;
          second_nibble = pdf_stm_f_ahex_int2hex (in_char >> 4);
        }
      else
        {
          first_nibble = pdf_stm_f_ahex_int2hex (in_char >> 4);
          second_nibble = pdf_stm_f_ahex_int2hex (in_char);
        }
      in->rp++;

      /* Write the hex digits to the output buffer, if possible */

      /* First nibble */
      out->data[out->wp] = first_nibble;
      out->wp++;
      filter_state->written_bytes++;

      /* Maybe write the second nibble */
      if (pdf_stm_buffer_full_p (out))
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

  if (finish_p)
    {
      /* The end of the encoded data is a 3E '>' ASCII character */
      if (!pdf_stm_buffer_full_p (out))
        {
          out->data[out->wp] = '>';
          out->wp++;
        }
      else
        {
          /* This should not be reached, but just in case */
          return PDF_ERROR;
        }
    }

  if ((in->wp - in->rp) == 0)
    {
      ret = PDF_EEOF;
    }
  else
    {
      ret = PDF_OK;
    }

  return ret;
}

pdf_status_t
pdf_stm_f_ahexenc_dealloc_state (void *state)
{
  pdf_stm_f_ahexenc_t ahexenc_state;

  ahexenc_state = (pdf_stm_f_ahexenc_t) state;
  pdf_dealloc (ahexenc_state);

  return PDF_OK;
}

pdf_status_t
pdf_stm_f_ahexdec_init (pdf_hash_t params,
                        void **state)
{
  pdf_stm_f_ahexdec_t ahexdec_state;
  pdf_status_t ret;

  /* This filter uses no parameters */
  /* Allocate the internal state structure */
  ahexdec_state = pdf_alloc (sizeof(struct pdf_stm_f_ahexdec_s));
  if (ahexdec_state != NULL)
    {
      ahexdec_state->last_nibble = PDF_EOF;
      ahexdec_state->written_bytes = 0;

      *state = ahexdec_state;
      ret = PDF_OK;
    }
  else
    {
      ret = PDF_ERROR;
    }

  return ret;
}

pdf_status_t
pdf_stm_f_ahexdec_apply (pdf_hash_t params,
                         void *state,
                         pdf_stm_buffer_t in,
                         pdf_stm_buffer_t out,
                         pdf_bool_t finish_p)
{
  pdf_status_t ret;
  pdf_stm_f_ahexdec_t filter_state;
  pdf_i32_t first_nibble;
  pdf_i32_t second_nibble;

  ret = PDF_OK;
  first_nibble = PDF_EOF;
  second_nibble = PDF_EOF;
  filter_state = (pdf_stm_f_ahexdec_t) state;

  while ((!pdf_stm_buffer_eob_p (in)) &&
         (!pdf_stm_buffer_full_p (out)))
    {
      /* Skip white characters */
      if (pdf_stm_f_ahex_white_p ((pdf_u32_t) in->data[in->rp]))
        {
          in->rp++;
          continue;
        }

      /* Detect the end of the hex data */
      if (in->data[in->rp] == '>')
        {
          if (filter_state->last_nibble == PDF_EOF)
            {
              /* We are done :'D */
              in->rp++;
              ret = PDF_EEOF;
              break;
            }
          else
            {
              /* Found an even number of hex digits. We assume that
                 the second nibble is 0, so generate a byte of data
                 and finish */
              out->data[out->wp] =
                pdf_stm_f_ahex_hex2int (filter_state->last_nibble) << 4;
              out->wp++;
              filter_state->last_nibble = PDF_EOF;
              ret = PDF_EEOF;
              break;
            }
        }

      /* Detect an invalid character */
      if (!pdf_stm_f_ahex_hex_p ((pdf_u32_t) in->data[in->rp]))
        {
          ret = PDF_ERROR;
          break;
        }

      /* Process this character. This is the first or the second part
         of a mibble. */
      if (filter_state->last_nibble == PDF_EOF)
        {
          /* Get the first nibble */
          first_nibble = (pdf_u32_t) in->data[in->rp];
          in->rp++;

          filter_state->last_nibble = first_nibble;
        }
      else
        {
          /* Get the second nibble */
          second_nibble = (pdf_u32_t) in->data[in->rp];
          in->rp++;

          /* Generate one byte of data */
          out->data[out->wp] = (pdf_stm_f_ahex_hex2int (first_nibble) << 4)
            + pdf_stm_f_ahex_hex2int (second_nibble);
          out->wp++;

          filter_state->last_nibble = PDF_EOF;
        }
    }
  
  if ((ret == PDF_OK) &&
      (pdf_stm_buffer_eob_p (in)))
    {
      ret = PDF_EEOF;
    }

  return ret;
}

pdf_status_t
pdf_stm_f_ahexdec_dealloc_state (void *state)
{
  pdf_stm_f_ahexenc_t ahexenc_state;

  ahexenc_state = (pdf_stm_f_ahexenc_t) state;
  pdf_dealloc (ahexenc_state);

  return PDF_OK;
}

/* Private functions */

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

static const pdf_char_t to_hex[16] = "0123456789ABCDEF";

static pdf_char_t
pdf_stm_f_ahex_int2hex (pdf_u32_t hex)
{
  return to_hex[hex & 0x0f];
}

static pdf_u32_t
pdf_stm_f_ahex_hex2int (pdf_u32_t hex)
{
  if ((hex >= 'a') && (hex <= 'f'))
    {
      return (hex - 'a') + 0xA;
    }
  if ((hex >= 'A') && (hex <= 'F'))
    {
      return (hex - 'A') + 0xA;
    }
  if ((hex >= '0') && (hex <= '9'))
    {
      return (hex - '0');
    }

  return -1;
}

/* End of pdf_stm_f_ahex.c */

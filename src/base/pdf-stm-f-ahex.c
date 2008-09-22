/* -*- mode: C -*- Time-stamp: "08/09/22 23:13:52 jemarch"
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
  *state = (void *) filter_state;

  return PDF_OK;
}

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
  
  filter_state = (pdf_stm_f_ahexenc_t) state;

  while ((!pdf_stm_buffer_eob_p (in)) &&
         (!pdf_stm_buffer_full_p (out)))
    {
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

      /* Maybe write the second nibble */
      if (pdf_stm_buffer_full_p (out))
        {
          filter_state->last_nibble = second_nibble;
        }
      else
        {
          out->data[out->wp] = second_nibble;
          out->wp++;
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
pdf_stm_f_ahexdec_init (pdf_hash_t params,
                        void **state)
{
  /* This filter uses no parameters */

  /* Allocate the internal state structure */
  /* FIXME */

  return PDF_OK;
}

pdf_status_t
pdf_stm_f_ahexdec_apply (pdf_hash_t params,
                         void *state,
                         pdf_stm_buffer_t in,
                         pdf_stm_buffer_t out,
                         pdf_bool_t finish_p)
{
  
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

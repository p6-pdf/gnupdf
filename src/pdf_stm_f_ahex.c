/* -*- mode: C -*- Time-stamp: "08/01/20 20:03:32 jemarch"
 *
 *       File:         pdf_stm_f_ahex.c
 *       Date:         Fri Jul 13 17:08:41 2007
 *
 *       GNU PDF Library - ASCII Hex decoder filter
 *
 */

/* Copyright (C) 2007 Free Software Foundation, Inc. */

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

#ifdef HAVE_MALLOC_H
 #include <malloc.h>
#else
 #include <stdlib.h>
#endif /* HAVE_MALLOC_H */

#include <xalloc.h>
#include <string.h>
#include <stdio.h>
#include <pdf_stm_f_ahex.h>

static int pdf_stm_f_ahex_white_p (int hex);
static int pdf_stm_f_ahex_hex_p (int hex);
static int pdf_stm_f_ahex_hex2int (int hex);
static pdf_char_t pdf_stm_f_ahex_int2hex (int n);
static int pdf_stm_f_ahex_encode (pdf_char_t *in, pdf_stm_pos_t in_size,
                                  pdf_char_t **out, pdf_stm_pos_t *out_size);
static int pdf_stm_f_ahex_decode (pdf_char_t *in, pdf_stm_pos_t in_size,
                                  pdf_char_t **out, pdf_stm_pos_t *out_size);



int
pdf_stm_f_ahex_init (void **filter_data,
                     void *conf_data)
{
  pdf_stm_f_ahex_data_t *data;
  pdf_stm_f_ahex_conf_t conf;

  data = (pdf_stm_f_ahex_data_t *) filter_data;
  conf = (pdf_stm_f_ahex_conf_t) conf_data;

  /* Create the private data storage */
  *data =
    (pdf_stm_f_ahex_data_t) xmalloc (sizeof(struct pdf_stm_f_ahex_data_s));
  (*data)->mode = conf->mode;

  return PDF_OK;
}

int 
pdf_stm_f_ahex_apply (void *filter_data,
                      pdf_char_t *in, pdf_stm_pos_t in_size,
                      pdf_char_t **out, pdf_stm_pos_t *out_size)
{
  pdf_stm_f_ahex_data_t data;

  data = (pdf_stm_f_ahex_data_t) filter_data;
  switch (data->mode)
    {
    case PDF_STM_F_AHEX_MODE_ENCODE:
      {
        return pdf_stm_f_ahex_encode (in, in_size, out, out_size);
      }
    case PDF_STM_F_AHEX_MODE_DECODE:
      {
        return pdf_stm_f_ahex_decode (in, in_size, out, out_size);
      }
    default:
      {
        return PDF_ERROR;
      }
    }

  /* Not reached */
}

int
pdf_stm_f_ahex_dealloc (void **filter_data)
{
  pdf_stm_f_ahex_data_t *data;
  
  data = (pdf_stm_f_ahex_data_t *) filter_data;
  free (*data);

  return PDF_OK;
}

/* Private functions */

static int
pdf_stm_f_ahex_white_p (int hex)
{
  return ((hex == '\0')    /* Null */
          || (hex == '\t') /* Tab */
          || (hex == '\n') /* Line feed */
          || (hex == '\f') /* Form feed */
          || (hex == '\r') /* Carriage return */
          || (hex == 32)); /* Space character */
}

static int
pdf_stm_f_ahex_hex_p (int hex)
{
  return (((hex >= 'a') && (hex <= 'f')) ||
          ((hex >= 'A') && (hex <= 'F')) ||
          ((hex >= '0') && (hex <= '9')));
}

static const pdf_char_t to_hex[16] = "0123456789ABCDEF";

static pdf_char_t
pdf_stm_f_ahex_int2hex (int hex)
{
  return to_hex[hex & 0x0f];
}

static int
pdf_stm_f_ahex_hex2int (int hex)
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

#define AHEX_ENC_LINE_LENGTH 80

static int
pdf_stm_f_ahex_encode (pdf_char_t *in, 
                       pdf_stm_pos_t in_size,
                       pdf_char_t **out,
                       pdf_stm_pos_t *out_size)
{
  pdf_stm_pos_t pos_in;
  pdf_stm_pos_t pos_out;
  pdf_stm_pos_t num_lines;
  pdf_stm_pos_t line_length;

  *out_size = 
    (in_size * 2) +                         /* Expansion of 1:2 */
    1 +                                     /* EOD marker */
    ((in_size * 2) / AHEX_ENC_LINE_LENGTH); /* line splitting */
  *out = (pdf_char_t *) xmalloc (*out_size);

  pos_out = 0;
  num_lines = 0;
  line_length = 0;
  for (pos_in = 0;
       pos_in < in_size;
       pos_in++)
    {
      pos_out = (pos_in * 2) + num_lines;
      
      (*out)[pos_out] = pdf_stm_f_ahex_int2hex (in[pos_in] >> 4);
      (*out)[pos_out + 1] = pdf_stm_f_ahex_int2hex (in[pos_in]);

      line_length = line_length + 2;

      if (line_length >= AHEX_ENC_LINE_LENGTH)
        {
          (*out)[pos_out + 2] = '\n';
          line_length = 0;
          num_lines++;
        }
    }

  /* Insert the EOD marker */
  (*out)[*out_size - 1] = '>';

  return PDF_OK;
}

static int
pdf_stm_f_ahex_decode (pdf_char_t *in,
                       pdf_stm_pos_t in_size,
                       pdf_char_t **out,
                       pdf_stm_pos_t *out_size)
{
  pdf_stm_pos_t in_pos;
  pdf_stm_pos_t out_pos;
  int odd_p;

  /* Decompression ratio is 2:1
     but take care about whitespaces and the EOD marker */
  *out = (pdf_char_t *) xmalloc (in_size);
  
  in_pos = 0;
  out_pos = 0;
  odd_p = PDF_TRUE;
  while (in_pos < in_size)
    {
      if (pdf_stm_f_ahex_hex_p (in[in_pos]))
        {
          if (odd_p)
            {
              /* First nibble */
              (*out)[out_pos] = 0;
              (*out)[out_pos] = pdf_stm_f_ahex_hex2int (in[in_pos]) << 4;
              odd_p = !odd_p;
            }
          else
            {
              /* Second nibble */
              (*out)[out_pos] += pdf_stm_f_ahex_hex2int (in[in_pos]);
              odd_p = !odd_p;
              out_pos++;
            }

          in_pos++;
        }
      else if (pdf_stm_f_ahex_white_p (in[in_pos]))
        {
          in_pos++;
        }
      else if (in[in_pos] == '>')
        {
          break;
        }
      else
        {
          goto error;
        }
    }

  /* Adjust output buffer size */
  *out_size = out_pos;
  *out = (pdf_char_t *) xrealloc (*out, *out_size);
  
  return PDF_OK;

 error:
  
  return PDF_ERROR;
}


/* End of pdf_stm_f_ahex.c */

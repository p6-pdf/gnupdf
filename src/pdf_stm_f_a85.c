/* -*- mode: C -*- Time-stamp: "07/07/15 21:33:49 jemarch"
 *
 *       File:         pdf_stm_f_a85.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Sun Jul 15 06:01:42 2007
 *
 *       GNU PDF Library - ASCII85 encoder/decoder
 *
 */

/* Copyright (C) 2007 Jose E. Marchesi */

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

/* The code of this file is based in the work of Paul Haahr, put in
   the public domain by the author.

   Many thanks Paul! */

#include <config.h>
#include <malloc.h>
#include <xalloc.h>
#include <string.h>
#include <stdio.h>
#include <pdf_stm_f_a85.h>

static int pdf_stm_f_a85_white_p (int hex);
static int pdf_stm_f_a85_encode (pdf_char_t *in, pdf_stm_pos_t in_size,
                                 pdf_char_t **out, pdf_stm_pos_t *out_size);
static int pdf_stm_f_a85_decode (pdf_char_t *in, pdf_stm_pos_t in_size,
                                 pdf_char_t **out, pdf_stm_pos_t *out_size);

int
pdf_stm_f_a85_init (void **filter_data,
                    void *conf_data)
{
  pdf_stm_f_a85_data_t *data;
  pdf_stm_f_a85_conf_t conf;

  data = (pdf_stm_f_a85_data_t *) filter_data;
  conf = (pdf_stm_f_a85_conf_t) conf_data;

  /* Create the private data storage */
  *data =
    (pdf_stm_f_a85_data_t) xmalloc (sizeof(struct pdf_stm_f_a85_data_s));
  (*data)->mode = conf->mode;

  return PDF_OK;
}

int
pdf_stm_f_a85_apply (void *filter_data,
                     pdf_char_t *in, pdf_stm_pos_t in_size,
                     pdf_char_t **out, pdf_stm_pos_t *out_size)
{
  pdf_stm_f_a85_data_t data;

  data = (pdf_stm_f_a85_data_t) filter_data;
  switch (data->mode)
    {
    case PDF_STM_F_A85_MODE_ENCODE:
      {
        return pdf_stm_f_a85_encode (in, in_size, out, out_size);
      }
    case PDF_STM_F_A85_MODE_DECODE:
      {
        return pdf_stm_f_a85_decode (in, in_size, out, out_size);
      }
    default:
      {
        return PDF_ERROR;
      }
    }

  /* Not reached */
}

int
pdf_stm_f_a85_dealloc (void **filter_data)
{
  pdf_stm_f_a85_data_t *data;

  data = (pdf_stm_f_a85_data_t *) filter_data;
  free (*data);

  return PDF_OK;
}

/* Private functions */

static int
pdf_stm_f_a85_white_p (int hex)
{
  return ((hex == '\0') || /* Null */
          (hex == '\t') || /* Tab */
          (hex == '\n') || /* Line feed */
          (hex == '\f') || /* Form feed */
          (hex == '\r') || /* Carriage return */
          (hex == 32));    /* Space character */
}

#define A85_ENC_LINE_LENGTH 80

static int
pdf_stm_f_a85_encode (pdf_char_t *in,
                      pdf_stm_pos_t in_size,
                      pdf_char_t **out,
                      pdf_stm_pos_t *out_size)
{
  unsigned long tuple;
  pdf_stm_pos_t pos_in;
  pdf_stm_pos_t pos_out;
  pdf_stm_pos_t line_length;
  int avail;
  char buf[5];
  int i;
  
  /* ASCII base-85 encoding produces 5 ASCII characters for every 4
     bytes in the input data.

     But take care for the EOD marker (~>) and the newlines.

     Note that since 0000 is coded with 'z', this is an upper bound
     rather than the final length of the output buffer. 

     Note also that A85_ENC_LINE_LENGTH should be an integral divisor of 4. */
  if ((in_size % 4) == 0)
    {
      *out_size = (in_size / 4) * 5 +                /* Expansion of 4:5 */
        2 +                                          /* EOD marker */
        (((in_size / 4) * 5) / A85_ENC_LINE_LENGTH); /* line splitting */
    }
  else
    {
      *out_size = ((in_size / 4) + 1) * 5 +                /* Expansion of 4:5 */
        2 +                                                /* EOD marker */
        ((((in_size / 4) + 1) * 5) / A85_ENC_LINE_LENGTH); /* line splitting */
    }
  *out = (pdf_char_t *) xmalloc (*out_size);

  pos_out = 0;
  tuple = 0;
  line_length = 0;
  for (pos_in = 0;
       pos_in < in_size;
       pos_in = pos_in + 4)
    {
      tuple = 0;
      avail = in_size - pos_in;

      tuple = tuple | (in[pos_in] << 24);
      tuple = tuple | 
        ((avail >= 2) ? (in[pos_in + 1] << 16) : 0);
      tuple = tuple | 
        ((avail >= 3) ? (in[pos_in + 2] << 8) : 0);
      tuple = tuple | 
        ((avail >= 4) ? in[pos_in + 3] : 0);
      
      if ((tuple == 0) &&
          (avail >= 4))
        {
          /* Four 0s in row */
          (*out)[pos_out] = 'z';
          pos_out++;
          line_length++;
          if (line_length > A85_ENC_LINE_LENGTH)
            {
              line_length = 0;
              (*out)[pos_out] = '\n';
            }
        }
      else
        {
          /* Encode this tuple in base-85 */
          for (i = 0;
               i < 5;
               i++)
            {
              buf[i] = tuple % 85;
              tuple = tuple / 85;
            }
          for (i = 4;
               i >= 0;
               i--)
            {
              (*out)[pos_out] = buf[i] + '!';
              pos_out++;
              line_length++;
              if (line_length > A85_ENC_LINE_LENGTH)
                {
                  line_length = 0;
                  (*out)[pos_out] = '\n';
                  pos_out++;
                }
            }
        }
    }

  /* Insert the EOD marker */
  (*out)[pos_out] = '~';
  pos_out++;
  (*out)[pos_out] = '>'; 
  pos_out++;

  /* Adjust memory if needed */
  if (*out_size > pos_out)
    {
      *out_size = pos_out;
      *out = (pdf_char_t *) xrealloc (*out,
                                      *out_size);
    }

  return PDF_OK;
}

static unsigned long pdf_stm_f_a85_pow85[] =
  {85*85*85*85, 85*85*85, 85*85, 85, 1};

static int
pdf_stm_f_a85_decode (pdf_char_t *in,
                      pdf_stm_pos_t in_size,
                      pdf_char_t **out,
                      pdf_stm_pos_t *out_size)
{
  pdf_stm_pos_t pos_in;
  pdf_stm_pos_t pos_out;
  int c;
  int count;
  unsigned long tuple;

  *out = (pdf_char_t *) xmalloc (in_size);

  tuple = 0;
  count = 0;
  pos_out = 0;
  for (pos_in = 0;
       pos_in < in_size;
       pos_in++)
    {
      c = in[pos_in];

      if (pdf_stm_f_a85_white_p (c))
        {
          /* Ignore whitespace characters */
          continue;
        }

      if (c == 'z')
        {
          if (count != 0)
            {
              /* Error: z inside ascii85 5-tuple */
              goto error;
            }
          (*out)[pos_out++] = 0;
          (*out)[pos_out++] = 0;
          (*out)[pos_out++] = 0;
          (*out)[pos_out++] = 0;
          
          continue;
        }

      if (c == '~')
        {
          break;
        }
     
      if ((c < '!' || c > 'u'))
        {
          /* Illegal character in ascii85 stream */
          goto error;
        }

      /* Decode this character */
      tuple =  tuple + (c - '!') * pdf_stm_f_a85_pow85[count];
      count = count + 1;
      
      if (count == 5)
        {
          /* Decode this tuple to base-256 four bytes */
          (*out)[pos_out++] = tuple >> 24;
          (*out)[pos_out++] = tuple >> 16;
          (*out)[pos_out++] = tuple >> 8;
          (*out)[pos_out++] = tuple;
    
          count = 0;
          tuple = 0;
        }
    }

  /* Adjust memory */
  *out_size = pos_out;
  *out = (pdf_char_t *) xrealloc (*out, 
                                  *out_size);
  return PDF_OK;

 error:
  free (*out);
  return PDF_ERROR;
}


/* End of pdf_stm_f_a85.c */

/* -*- mode: C -*- Time-stamp: "07/07/28 19:11:42 jemarch"
 *
 *       File:         pdf_stm_f_pred.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Thu Jul 12 01:21:33 2007
 *
 *       GNU PDF Library - Predictor stream filter
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

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <xalloc.h>
#include <string.h>
#include <pdf_base.h>
#include <pdf_stm_f_pred.h>

#define PNG_ENC_PREDICTOR_P(pred)                      \
  ((pred) == PDF_STM_F_PREDENC_PNG_NONE_ALL_ROWS ||    \
   (pred) == PDF_STM_F_PREDENC_PNG_SUB_ALL_ROWS ||     \
   (pred) == PDF_STM_F_PREDENC_PNG_UP_ALL_ROWS ||      \
   (pred) == PDF_STM_F_PREDENC_PNG_AVERAGE_ALL_ROWS || \
   (pred) == PDF_STM_F_PREDENC_PNG_PAETH_ALL_ROWS ||   \
   (pred) == PDF_STM_F_PREDENC_PNG_OPTIMUM)

/* Dirty util */
typedef struct bit_ptr_s 
{
  pdf_char_t* ptr;
  int offset;
  int mask;
  int block_size; 
} bit_ptr_t;

static void
bit_ptr_init (bit_ptr_t* bp, 
              pdf_char_t* ptr, 
              int block_size)
{
  int i;
  bp->ptr = ptr;
  bp->offset = 0;
  bp->block_size = block_size;
  bp->mask = 0;
  for (i = 0; i < bp->block_size; i++) 
    {
      bp->mask <<= 1;
      bp->mask |= 1;
    }
}

#define BIT_PTR_ADV(bp)                  \
  if ((bp).offset + (bp).block_size < 8) \
    {                                    \
    (bp).offset += (bp).block_size;      \
    }                                    \
  else                                   \
    {                                    \
      (bp).ptr++;                        \
      (bp).offset = 0;                   \
    }
    
#define BIT_PTR_GET(bp)                                 \
  ((*(bp).ptr & ((bp).mask << bp.offset)) >> bp.offset)

#define BIT_PTR_SET(bp, e)                                              \
  (*(bp).ptr = (*(bp).ptr & ~((bp).mask << (bp).offset)) | ((e) << (bp).offset))


int
pdf_stm_f_pred_init (void **filter_data,
                     void *conf_data)
{
  int actual_len;
  
  pdf_stm_f_pred_data_t *data;
  pdf_stm_f_pred_conf_t conf;

  data = (pdf_stm_f_pred_data_t *) filter_data;
  conf = (pdf_stm_f_pred_conf_t) conf_data;

  /* Create the private data storage */
  *data =
    (pdf_stm_f_pred_data_t) xmalloc (sizeof(struct pdf_stm_f_pred_data_s));
  
  (*data)->mode = conf->mode;
  (*data)->predictor = conf->predictor;
  (*data)->colors = conf->colors;
  (*data)->bits_per_component = conf->bits_per_component;
  (*data)->columns = conf->columns;
  
  /* We need the number of full bytes that each row has. As defined in the
   * PNG standard we can assume that if greater than 8 the bits per component
   * is multiplier of eight. */
  actual_len = conf->columns * conf->colors * conf->bits_per_component;
  (*data)->scanline_len = (actual_len >> 3) + (actual_len & 7); 
    
  return PDF_OK;
}

static void
encode_row_none(pdf_char_t* cur, 
                pdf_char_t* prev, 
                pdf_char_t* out,
                pdf_stm_f_pred_data_t data)
{
  memcpy(out, cur, data->scanline_len);
}

static void
encode_row_sub(pdf_char_t* cur, 
               pdf_char_t* prev, 
               pdf_char_t* out,
               pdf_stm_f_pred_data_t data)
{
  int i;
  *out++ = *cur++;
  for (i = 1; i < data->scanline_len; i++) 
    {
      *out = *cur - *(cur-1);
      cur++;
    }
}

static void
encode_row_up(pdf_char_t* cur, 
              pdf_char_t* prev, 
              pdf_char_t* out,
              pdf_stm_f_pred_data_t data)
{
  int i;
  if (prev != NULL) 
    {
      for (i = 0; i < data->scanline_len; i++)
        {
          *out++ = *cur++ - *prev++;
        }
    } 
  else
    {
      memcpy(out, cur, data->scanline_len);
    }
}

static void
encode_row_average(pdf_char_t* cur, 
                   pdf_char_t* prev, 
                   pdf_char_t* out,
                   pdf_stm_f_pred_data_t data)
{
  int i;
  if (prev != NULL) 
    {
      *out++ = *cur++ - (*prev++ >> 1);

      for (i = 1; i < data->scanline_len; i++) 
        {
          *out++ = *cur - (pdf_char_t)(((int)*(cur-1) + (int)*prev) >> 1);
          cur++;
          prev++;
        }
    } 
  else 
    {
      *out++ = *cur++;

      for (i = 1; i < data->scanline_len; i++) 
        {
          *out++ = *cur - (*(cur-1) >> 1);
          cur++;
        }
    }
}

static void
encode_row_paeth (pdf_char_t* cur, 
                  pdf_char_t* prev, 
                  pdf_char_t* out,
                  pdf_stm_f_pred_data_t data)
{
  int i;
  int p;
  int pa;
  int pb;
  int pc;

  if (prev != NULL) 
    {
      *out++ = *cur++ - *prev++; 

      for (i = 0; i < data->scanline_len; i++) 
        {
          p = *(cur-1) + *(prev) - *(prev-1);
          pa = abs(p - *(cur-1));
          pb = abs(p - *(prev));
          pc = abs(p - *(prev-1));

          if (pa <= pb && pa <= pc) 
            {
              *out++ = *cur - *(cur-1);
            }
          else 
            {
              if (pb <= pc) 
                {
                  *out++ = *cur - *(prev);
                }
              else 
                { 
                  *out++ = *cur - *(prev-1);
                }
            }

          cur++;
          prev++;
        }
    } 
  else 
    {
      *out++ = *cur++;

      for (i = 1; i < data->scanline_len; i++) 
        {
          *out = *cur - *(cur-1);
          cur++;
        }
    }
}

static void
encode_row_sub_color16 (pdf_char_t* cur, 
                        pdf_char_t* prev, 
                        pdf_char_t* out,
                        pdf_stm_f_pred_data_t data)
{  
  int i;
  int j;

  unsigned short *this;
  unsigned short *next;
  unsigned short *sout;

  this = (unsigned short*) cur;
  next = (unsigned short*) cur;
  sout = (unsigned short*) out;

  for (j = 0; j < data->colors; j++)
    {
      *sout++ = *next++;
    }

  for (i = 1; i < data->columns; i++)
    {
      for (j = 0; j < data->colors; j++)
        {
          *sout++ = *next++ - *this++;
        }
    }
}

static void
encode_row_sub_color8 (pdf_char_t* cur, 
                       pdf_char_t* prev, 
                       pdf_char_t* out,
                       pdf_stm_f_pred_data_t data)
{
  int i;
  int j;
  pdf_char_t *this;
  pdf_char_t *next;
  pdf_char_t *sout;

  this = cur; 
  next = cur;
  sout = out;

  for (j = 0; j < data->colors; j++)
    {
      *sout++ = *next++;
    }
  for (i = 1; i < data->columns; i++)
    {
      for (j = 0; j < data->colors; j++)
        {
          *sout++ = *next++ - *this++;
        }
    }
}

static void
encode_row_sub_colorl8 (pdf_char_t *cur, 
                        pdf_char_t *prev, 
                        pdf_char_t *out,
                        pdf_stm_f_pred_data_t data)
{
  int i;
  int j;
  bit_ptr_t this;
  bit_ptr_t next;
  bit_ptr_t sout;
  
  bit_ptr_init (&this, cur, data->bits_per_component);
  bit_ptr_init (&next, cur, data->bits_per_component);
  bit_ptr_init (&sout, out, data->bits_per_component);
  
  for (j = 0; j < data->colors; j++) 
    {
      BIT_PTR_SET(sout, BIT_PTR_GET(next));
      BIT_PTR_ADV(sout);
      BIT_PTR_ADV(next);
    }
  for (i = 1; i < data->columns; i++) 
    {
      for (j = 0; j < data->colors; j++) 
        {
          BIT_PTR_SET(sout, BIT_PTR_GET(next) - BIT_PTR_GET(this));
          BIT_PTR_ADV(sout);
          BIT_PTR_ADV(next);
          BIT_PTR_ADV(this);
        }
    }
}

static int
encode_row (pdf_char_t *cur, 
            pdf_char_t *prev, 
            pdf_char_t *out,
            pdf_stm_f_pred_data_t data)
{
  switch (data->predictor) 
    {
    case PDF_STM_F_PREDENC_NO_PREDICTION:
    case PDF_STM_F_PREDENC_PNG_NONE_ALL_ROWS:
      {
        encode_row_none(cur, prev, out, data);
        break;
      }
    case PDF_STM_F_PREDENC_TIFF_PREDICTOR_2:
      {
        switch (data->bits_per_component) 
          {
          case 16:
            {
              encode_row_sub_color16(cur, prev, out, data);
              break;
            }
          case 8:
            {
              encode_row_sub_color8(cur, prev, out, data);
              break;
            }
          case 4: case 2: case 1:
            {
              encode_row_sub_colorl8(cur, prev, out, data);
              break;
            }
          default:
            {
              return PDF_ERROR;
              /* Make stupid compilers happy */
              break;
            }
          }

        break;
      }
    case PDF_STM_F_PREDENC_PNG_SUB_ALL_ROWS:
      {
        encode_row_sub (cur, prev, out, data);
        break;
      }
    case PDF_STM_F_PREDENC_PNG_UP_ALL_ROWS:
      {
        encode_row_up(cur, prev, out, data);
        break;
      }
    case PDF_STM_F_PREDENC_PNG_AVERAGE_ALL_ROWS:
      {
        encode_row_average(cur, prev, out, data);
        break;
      }
    case PDF_STM_F_PREDENC_PNG_PAETH_ALL_ROWS:
      {
        encode_row_paeth(cur, prev, out, data);
        break;
      }
    default:
      {
        return PDF_ERROR;
        /* Make stupid compilers happy */
        break;
      }
    }

  return PDF_OK;
}


static int
pdf_stm_f_pred_encode (pdf_stm_f_pred_data_t data,
                       pdf_char_t *in, 
                       pdf_stm_pos_t in_size,
                       pdf_char_t **out, 
                       pdf_stm_pos_t *out_size)
{  
  pdf_char_t *curr;
  pdf_char_t *prev;
  pdf_char_t *buf;
  
  int is_png_predictor = PNG_ENC_PREDICTOR_P(data->predictor);
  int i;

  curr = in;
  prev = NULL;
  buf = NULL;
  
  if (in_size % data->scanline_len != 0) 
    { 
      /* Not all rows al full. */
      return PDF_ERROR;
    }
  
  *out_size = is_png_predictor ? in_size + in_size/data->scanline_len : in_size;
    
  *out = (pdf_char_t *) xmalloc (*out_size);
  if (*out == NULL) 
    {
      *out_size = 0;
      return PDF_ERROR;
    }
  
  buf = *out;
  
  for (i = 0; i < in_size; i += data->scanline_len) 
    {
      if (is_png_predictor)
        {
          *buf++ = data->predictor;
        }
    
      if (encode_row(curr, prev, buf, data) == PDF_ERROR) 
        {
          free(*out);
          *out = NULL;
          *out_size = 0;
          return PDF_ERROR;
        }
    
      prev = curr;
      curr += data->scanline_len;
      buf  += data->scanline_len;
    }

  return PDF_OK;
}

static void
decode_row_none (pdf_char_t* in, 
                 pdf_char_t* cur, 
                 pdf_char_t* prev,
                 pdf_stm_f_pred_data_t data)
{
  memcpy (cur, in, data->scanline_len);
}

static void
decode_row_sub (pdf_char_t* in, 
                pdf_char_t* cur, 
                pdf_char_t* prev,
                pdf_stm_f_pred_data_t data)
{
  int i;

  *cur++ = *in++;

  for (i = 1; i < data->scanline_len; i++) 
    {
      *cur = *in++ + *(cur-1);
      cur++; 
    }
}

static void
decode_row_up (pdf_char_t* in, 
               pdf_char_t* cur, 
               pdf_char_t* prev,
               pdf_stm_f_pred_data_t data)
{
  int i;

  if (prev != NULL) 
    {
      for (i = 0; i < data->scanline_len; i++)
        {
          *cur++ = *in++ + *prev++;
        }

    } 
  else
    {
      memcpy (cur, in, data->scanline_len);
    }
}

static void
decode_row_average (pdf_char_t* in, 
                    pdf_char_t* cur, 
                    pdf_char_t* prev,
                    pdf_stm_f_pred_data_t data)
{
  int i;

  if (prev != NULL) 
    {
      *cur++ = *in++ + (*prev++ >> 1);
      
      for (i = 1; i < data->scanline_len; i++) 
        {
          *cur = *in++ + (pdf_char_t)(((int)*(cur-1) + (int)*prev) >> 1);
          cur++;
          prev++;
        }
    } 
  else 
    {
      *cur++ = *in++;

      for (i = 1; i < data->scanline_len; i++) 
        {
          *cur = *in++ + (*(cur-1) >> 1);
          cur++;
        }
    }
}

static void
decode_row_paeth (pdf_char_t* in, 
                  pdf_char_t* cur, 
                  pdf_char_t* prev,
                  pdf_stm_f_pred_data_t data)
{
  int p;
  int pa;
  int pb;
  int pc;
  int i;

  if (prev != NULL) 
    {
      *cur++ = *in++ + *prev++; 

      for (i = 0; i < data->scanline_len; i++) 
        {
          p = *(cur-1) + *(prev) - *(prev-1);
          pa = abs(p - *(cur-1));
          pb = abs(p - *(prev));
          pc = abs(p - *(prev-1));

          if (pa <= pb && pa <= pc) 
            {
              *cur = *in + *(cur-1);
            }
          else 
            {
              if (pb <= pc) 
                {
                  *cur = *in + *(prev);
                }
              else 
                {
                  *cur = *in + *(prev-1);
                }
            }

          in++;
          prev++;
          cur++;
        }
    } 
  else 
    {
      *cur++ = *in++;

      for (i = 1; i < data->scanline_len; i++) 
        {
          *cur = *in++ + *(cur-1);
          cur++; 
        }
    }
}

static void
decode_row_sub_color16 (pdf_char_t* in, pdf_char_t* cur, pdf_char_t* prev,
                        pdf_stm_f_pred_data_t data)
{
  int i;
  int j;
  unsigned short* this;
  unsigned short* next;
  unsigned short* sin;

  this = (unsigned short*) cur;
  next = (unsigned short*) cur;
  sin = (unsigned short*) in;

  for (j = 0; j < data->colors; j++)
    {
      *next++ = *sin++;
    }
  for (i = 1; i < data->columns; i++)
    {
      for (j = 0; j < data->colors; j++)
        {
          *next++ = *sin++ + *this++;
        }
    }
}

static void
decode_row_sub_color8  (pdf_char_t* in, 
                        pdf_char_t* cur, 
                        pdf_char_t* prev,
                        pdf_stm_f_pred_data_t data)
{
  int i;
  int j;
  pdf_char_t *this;
  pdf_char_t *next;
  pdf_char_t *sin;

  this = cur;
  next = cur;
  sin = in;

  for (j = 0; j < data->colors; j++)
    {
      *next++ = *sin++;
    }

  for (i = 1; i < data->columns; i++)
    {
      for (j = 0; j < data->colors; j++)
        {
          *next++ = *sin++ + *this++;
        }
    }
}


static void
decode_row_sub_colorl8 (pdf_char_t* in, 
                        pdf_char_t* cur, 
                        pdf_char_t* prev,
                        pdf_stm_f_pred_data_t data)
{
  int i;
  int j;
  bit_ptr_t this;
  bit_ptr_t next;
  bit_ptr_t sin;
  
  bit_ptr_init (&this, cur, data->bits_per_component);
  bit_ptr_init (&next, cur, data->bits_per_component);
  bit_ptr_init (&sin,  in,  data->bits_per_component);
  
  for (j = 0; j < data->colors; j++) 
    {
      BIT_PTR_SET(next, BIT_PTR_GET(sin));
      BIT_PTR_ADV(next);
      BIT_PTR_ADV(sin);
    }
  for (i = 1; i < data->columns; i++) 
    {
      for (j = 0; j < data->colors; j++) 
        {
          BIT_PTR_SET(next, BIT_PTR_GET(sin) + BIT_PTR_GET(this));
          BIT_PTR_ADV(sin);
          BIT_PTR_ADV(next);
          BIT_PTR_ADV(this);
        }
    }
}

static int
decode_row (pdf_char_t* in, 
            pdf_char_t* cur, 
            pdf_char_t* prev,
            pdf_stm_f_pred_data_t data, 
            pdf_char_t predictor)
{
  switch (predictor) 
    {
    case PDF_STM_F_PREDDEC_TIFF_PREDICTOR_2:
      {
        switch(data->bits_per_component) 
          {
          case 16:
            {
              decode_row_sub_color16(in, cur, prev, data);
              break;
            }
          case 8:
            {
              decode_row_sub_color8(in, cur, prev, data);
              break;
            }
          case 4:
          case 2:
          case 1:
            {
              decode_row_sub_colorl8(in, cur, prev, data);
              break;
            }
          default: 
            {
              break;
            }
          }
        break;
      }
    case PDF_STM_F_PREDDEC_NO_PREDICTION:
    case PDF_STM_F_PREDDEC_PNG_NONE:
      {
        decode_row_none (in, cur, prev, data);
        break;
      }
    case PDF_STM_F_PREDDEC_PNG_SUB:
      {
        decode_row_sub (in, cur, prev, data);
        break;
      }
    case PDF_STM_F_PREDDEC_PNG_UP:
      {
        decode_row_up (in, cur, prev, data);
        break;
      }
    case PDF_STM_F_PREDDEC_PNG_AVERAGE:
      {
        decode_row_average (in, cur, prev, data);
        break;
      }
    case PDF_STM_F_PREDDEC_PNG_PAETH:
      {
        decode_row_paeth (in, cur, prev, data);
        break;
      }
    default:
      {
        return PDF_ERROR;
        /* Make stupid compilers happy */
        break;
      }
    }
  
  return PDF_OK;
}

static int
pdf_stm_f_pred_decode (pdf_stm_f_pred_data_t data,
                       pdf_char_t *in, 
                       pdf_stm_pos_t in_size,
                       pdf_char_t **out, 
                       pdf_stm_pos_t *out_size)
{  
  pdf_char_t *curr = NULL;
  pdf_char_t *prev = NULL;
  int is_png_predictor;
  int i;
  pdf_char_t predictor;

  curr = NULL;
  prev = NULL;
  is_png_predictor = data->predictor == PDF_STM_F_PREDDEC_PNG;
  predictor = data->predictor;
    
  if (is_png_predictor ? 
       in_size * data->scanline_len % (data->scanline_len + 1) != 0 :
       in_size % data->scanline_len != 0 ) 
    { 
      /* Not all rows al full. */
      return PDF_ERROR;
    }
  
  *out_size = is_png_predictor ? in_size * data->scanline_len / (data->scanline_len + 1) : in_size;
    
  if ((*out = (pdf_char_t *) xmalloc (*out_size)) == NULL) 
    {
      *out_size = 0;
      return PDF_ERROR;
    }
  
  curr = *out;
  for (i = 0; i < in_size; i += data->scanline_len + is_png_predictor) 
    {
      if (is_png_predictor)
        {
          predictor = *in++;
        }

      if (decode_row(in, curr, prev, data, predictor) == PDF_ERROR) 
        {
          free(*out);
          *out = NULL;
          *out_size = 0;
          return PDF_ERROR;
        }
    
      prev = curr;
      curr += data->scanline_len;
      in   += data->scanline_len;
    }

  return PDF_OK;
}

int
pdf_stm_f_pred_apply (void *filter_data,
                      pdf_char_t *in, pdf_stm_pos_t in_size,
                      pdf_char_t **out, pdf_stm_pos_t *out_size)
{
  switch (((pdf_stm_f_pred_data_t)filter_data)->mode) 
    {
    case PDF_STM_F_PRED_MODE_ENCODE:
      {
        return pdf_stm_f_pred_encode (filter_data, in, in_size, out, out_size);
      }
    case PDF_STM_F_PRED_MODE_DECODE:
      {
        return pdf_stm_f_pred_decode (filter_data, in, in_size, out, out_size);
      }
    default:
      {
        return PDF_ERROR;
      }
    }
}

int
pdf_stm_f_pred_dealloc (void **filter_data)
{
  pdf_stm_f_pred_data_t *data;

  data = (pdf_stm_f_pred_data_t *) filter_data;
  free (*data);
  
  return PDF_OK;
}

/* End of pdf_stm_f_pred.c */


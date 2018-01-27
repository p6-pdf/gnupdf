/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-pred.c
 *       Date:         Thu Jul 12 01:21:33 2007
 *
 *       GNU PDF Library - Predictor stream filter
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pdf-alloc.h>
#include <pdf-stm-f-pred.h>
#include <pdf-hash-helper.h>

/* Define predictor encoder */
PDF_STM_FILTER_DEFINE (pdf_stm_f_predenc_get,
                       stm_f_pred_init,
                       stm_f_predenc_apply,
                       stm_f_pred_deinit);

/* Define predictor decoder */
PDF_STM_FILTER_DEFINE (pdf_stm_f_preddec_get,
                       stm_f_pred_init,
                       stm_f_preddec_apply,
                       stm_f_pred_deinit);

#define PRED_PARAM_PREDICTOR "Predictor"
#define PRED_PARAM_COLORS "Colors"
#define PRED_PARAM_BPC "BitsPerComponent"
#define PRED_PARAM_COLUMNS "Columns"

/* prediction modes */
typedef enum
{
  PDF_STM_F_PREDDICT_NO_PREDICTION = 1,
  PDF_STM_F_PREDDICT_TIFF_PREDICTOR_2 = 2,
  PDF_STM_F_PREDDICT_PNG_NONE_ALL_ROWS = 10,
  PDF_STM_F_PREDDICT_PNG_SUB_ALL_ROWS = 11,
  PDF_STM_F_PREDDICT_PNG_UP_ALL_ROWS = 12,
  PDF_STM_F_PREDDICT_PNG_AVERAGE_ALL_ROWS = 13,
  PDF_STM_F_PREDDICT_PNG_PAETH_ALL_ROWS = 14,
  PDF_STM_F_PREDDICT_PNG_OPTIMUM = 15
} pdf_stm_f_predenc_method_t;

typedef enum
{
  PDF_STM_F_PREDDEC_NO_PREDICTION = 1,
  PDF_STM_F_PREDDEC_TIFF_PREDICTOR_2 = 2,
  PDF_STM_F_PREDDEC_PNG = 3
} pdf_stm_f_preddec_type_t;

typedef enum
{
  PDF_STM_F_PREDDEC_PNG_NONE = 0,
  PDF_STM_F_PREDDEC_PNG_SUB = 1,
  PDF_STM_F_PREDDEC_PNG_UP = 2,
  PDF_STM_F_PREDDEC_PNG_AVERAGE = 3,
  PDF_STM_F_PREDDEC_PNG_PAETH = 4
} pdf_stm_f_predec_png_type_t;


/* END PUBLIC */

/* Configuration structure */

/* Private state */

typedef struct pdf_stm_f_pred_s
{
  pdf_stm_f_preddec_type_t type; /* the overall type of predictor */
  int predictor; /* A code that selects the predictor algorithm. If
                    the value of this entry is 1, the filter assumes
                    that the normal algorithm was used to encode the
                    data, without prediction. If the value is greater
                    than 1, the filter assumes that the data was
                    differenced before being encoded, and `predictor'
                    selects the predictor algorithm */

  /* The following parameters are only useful when `predictor' > 1 */
  pdf_size_t colors;              /* Number of interleaved color components
                                     per sample. Valid values are 1 or
                                     greater. Default value: 1 */
  pdf_size_t bits_per_component;  /* The number of bits used to represent
                                     each color component in a sample. Valid
                                     values are 1, 2, 4, 8 and 16. Default
                                     value: 1 */
  pdf_size_t columns;             /* The number of samples in each
                                     row. Default value: 1 */

  pdf_size_t scanline_len;        /* will be calculated from params */

  /* previous and current buffers for scanlines (rows) 
     encoder uses prev_row_buf for last input row to the filter
     decoder uses prev_row_buf for last filtered output row */
  pdf_buffer_t *prev_row_buf;
  pdf_buffer_t *curr_row_buf;

  /* buffer for output scanline (already filtered) */
  pdf_buffer_t *out_row_buf;
} pdf_stm_f_pred_t;


#define PNG_ENC_PREDICTOR_P(pred)                      \
  ((pred) == PDF_STM_F_PREDENC_PNG_NONE_ALL_ROWS ||    \
   (pred) == PDF_STM_F_PREDENC_PNG_SUB_ALL_ROWS ||     \
   (pred) == PDF_STM_F_PREDENC_PNG_UP_ALL_ROWS ||      \
   (pred) == PDF_STM_F_PREDENC_PNG_AVERAGE_ALL_ROWS || \
   (pred) == PDF_STM_F_PREDENC_PNG_PAETH_ALL_ROWS ||   \
   (pred) == PDF_STM_F_PREDENC_PNG_OPTIMUM)

#define PNG_DEC_PREDICTOR_P(pred)                      \
  ((pred) == PDF_STM_F_PREDDEC_PNG ||                  \
   (pred) == PDF_STM_F_PREDDEC_PNG_NONE ||             \
   (pred) == PDF_STM_F_PREDDEC_PNG_SUB ||              \
   (pred) == PDF_STM_F_PREDDEC_PNG_UP ||               \
   (pred) == PDF_STM_F_PREDDEC_PNG_AVERAGE ||          \
   (pred) == PDF_STM_F_PREDDEC_PNG_PAETH)

/*
 * This is a bit level pointer object optimized for the needs in the
 * predictor class optimized for its usage within the needs of the predictor
 * coding and decoding clases.
 *
 * This adds a limitation: the block_size must be between 1 and 8 and must be
 * a power of two.
 *
 * TODO: Check endianess.
 */
typedef struct pred_bit_ptr_s 
{
  pdf_uchar_t* ptr;
  int offset;
  int mask;
  int block_size;
} pred_bit_ptr_t;

static void
pred_bit_ptr_init (pred_bit_ptr_t* bp, 
              pdf_uchar_t* ptr, 
              int block_size)
{
  bp->ptr = ptr;
  bp->offset = 8 - block_size;
  bp->block_size = block_size;
  bp->mask = (~0U << bp->block_size);
  bp->mask = ~bp->mask;
}

#define PRED_BIT_PTR_ADV(bp)             \
  if ((bp).offset > 0)                   \
    {                                    \
      (bp).offset -= (bp).block_size;    \
    }                                    \
  else                                   \
    {                                    \
      (bp).ptr++;                        \
      (bp).offset = 8 - (bp).block_size; \
    }

#define PRED_BIT_PTR_GET(bp)                             \
  ((*(bp).ptr >> (bp).offset) & (bp).mask)

#define PRED_BIT_PTR_SET(bp, e)                          \
  (*(bp).ptr = (*(bp).ptr & ~((bp).mask << (bp).offset)) \
   | (((e) & (bp).mask) << (bp).offset))

static pdf_bool_t 
stm_f_pred_init (const pdf_hash_t  *params,
                 void             **state,
                 pdf_error_t      **error)
{
  pdf_size_t actual_len;
  pdf_stm_f_pred_t* filter_state;
  /* Predictor decides if we need more parameters; so check it first */
  if (!params || !pdf_hash_key_p (params, PRED_PARAM_PREDICTOR))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_EBADDATA,
                     "cannot initialize predictor encoder/decoder: "
                     "parameter '"PRED_PARAM_PREDICTOR"' missing");
      return PDF_FALSE;
    }

  /* We demand all parameters if predictor > 1 */ 
  if (pdf_hash_get_size (params, PRED_PARAM_PREDICTOR) > 1) 
    {
      if (!pdf_hash_key_p (params, PRED_PARAM_COLORS) ||
          !pdf_hash_key_p (params, PRED_PARAM_BPC) ||
          !pdf_hash_key_p (params, PRED_PARAM_COLUMNS))
        {
          pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_EBADDATA,
                     "cannot initialize predictor encoder/decoder: "
                     "parameters missing ('"PRED_PARAM_COLORS"': %s, "
                     "'"PRED_PARAM_BPC"': %s, '"PRED_PARAM_COLUMNS"': %s)",
                     ((params && pdf_hash_key_p (params, PRED_PARAM_COLORS)) ?
                      "available" : "missing"),
                     ((params && pdf_hash_key_p (params, PRED_PARAM_BPC)) ?
                      "available" : "missing"),
                     ((params && pdf_hash_key_p (params, PRED_PARAM_COLUMNS)) ?
                      "available" : "missing"));
          return PDF_FALSE;
        }
    }

  /* Create the private filter_state storage */
  filter_state = pdf_alloc (sizeof (struct pdf_stm_f_pred_s));
  if (!filter_state)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "cannot create predictor encoder/decoder internal state: "
                     "couldn't allocate %lu bytes",
                     (unsigned long) sizeof (pdf_stm_f_pred_t));
      return PDF_FALSE;
    }

   pdf_stm_f_predenc_method_t method = pdf_hash_get_size (params, PRED_PARAM_PREDICTOR);

  filter_state->colors = pdf_hash_get_size (params, PRED_PARAM_COLORS);
  filter_state->bits_per_component = pdf_hash_get_size (params, PRED_PARAM_BPC);
  filter_state->columns = pdf_hash_get_size (params, PRED_PARAM_COLUMNS);

  /* as no parameters for predictor 1 (NO_PREDICTION) is needed */
  if (method == PDF_STM_F_PREDDICT_NO_PREDICTION)
    {
      /* set default values */
      filter_state->type = PDF_STM_F_PREDDEC_NO_PREDICTION;
      filter_state->colors = 1;
      filter_state->bits_per_component = 1;
      filter_state->columns = 1;
    }
  /* else wise check for bad parameter values */
  else
    {
      if (filter_state->colors < 1
          || filter_state->columns < 1
          || filter_state->bits_per_component < 1)
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_STM,
                         PDF_EBADDATA,
                         "cannot initialize predictor encoder/decoder: "
                         "bad parameter values ('"PRED_PARAM_COLORS"': %s, "
                         "'"PRED_PARAM_BPC"': %s, '"PRED_PARAM_COLUMNS"': %s)",
                         ((filter_state->colors < 1) ?
                          "bad" : "ok"),
                         ((filter_state->bits_per_component < 1) ?
                          "bad" : "ok"),
                         ((filter_state->columns < 1) ?
                          "bad" : "ok"));
          return PDF_FALSE;
        }

      if ((method >= PDF_STM_F_PREDDICT_PNG_NONE_ALL_ROWS)
	  && (method <= PDF_STM_F_PREDDICT_PNG_OPTIMUM)) {
	filter_state->type = PDF_STM_F_PREDDEC_PNG;
	filter_state->predictor = method == PDF_STM_F_PREDDICT_PNG_OPTIMUM
	  ? PDF_STM_F_PREDDICT_PNG_PAETH_ALL_ROWS
	  : method - 10;
      }
      else {
	if (method == PDF_STM_F_PREDDICT_TIFF_PREDICTOR_2) {
	  filter_state->type = PDF_STM_F_PREDDEC_TIFF_PREDICTOR_2;
	}
	else {
	  pdf_set_error (error,
			 PDF_EDOMAIN_BASE_STM,
			 PDF_EBADDATA,
			 "cannot initialize predictor encoder/decoder: "
			 "bad parameter values ('"PRED_PARAM_PREDICTOR"')");
	  return PDF_FALSE;
	}
      }
  }

  /* We need the number of full bytes that each row has. As defined in the
   * PNG standard we can assume that if greater than 8 the bits per component
   * is multiplier of eight. */
  actual_len = filter_state->columns * filter_state->colors * 
               filter_state->bits_per_component;
  filter_state->scanline_len = (actual_len >> 3) + (actual_len & 7); 

  /* one extra byte for PNG predictor */
  filter_state->prev_row_buf = pdf_buffer_new (filter_state->scanline_len + 1,
                                               error);
  if (!(filter_state->prev_row_buf))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "cannot create predictor encoder/decoder internal buffer: "
                     "couldn't allocate %lu bytes",
                     (unsigned long) filter_state->scanline_len);
      return PDF_FALSE;
    }

  /* hint for further optimizing (if necessary): if scanlines are smaller than 
     in and out buffers, curr_row_buf and out_row_buf are not needed and two 
     memcpys per scanline can be omitted */
  filter_state->curr_row_buf = pdf_buffer_new (filter_state->scanline_len + 1, 
                                               error);
  if (!(filter_state->curr_row_buf))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "cannot create predictor encoder/decoder internal buffer: "
                     "couldn't allocate %lu bytes",
                     (unsigned long) filter_state->scanline_len);
      return PDF_FALSE;
    }
  filter_state->out_row_buf = pdf_buffer_new (filter_state->scanline_len + 1, 
                                              error);
  if (!(filter_state->out_row_buf))
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "cannot create predictor encoder/decoder internal buffer: "
                     "couldn't allocate %lu bytes",
                     (unsigned long) filter_state->scanline_len + 1);
      return PDF_FALSE;
    }

  *state = filter_state;
  return PDF_TRUE;
}


static void
encode_row_none(pdf_uchar_t* cur, 
                pdf_uchar_t* prev, 
                pdf_uchar_t* out,
                pdf_stm_f_pred_t* state) 
{
  memcpy(out, cur, state->scanline_len);
}

static void
encode_row_sub(pdf_uchar_t* cur, 
               pdf_uchar_t* prev, 
               pdf_uchar_t* out,
               pdf_stm_f_pred_t* state)
{
  pdf_size_t i;
  *out++ = *cur++;
  for (i = 1; i < state->scanline_len; i++) 
    {
      *out++ = *cur - *(cur-1);
      cur++;
    }
}

static void
encode_row_up(pdf_uchar_t* cur, 
              pdf_uchar_t* prev, 
              pdf_uchar_t* out,
              pdf_stm_f_pred_t* state)
{
  pdf_size_t i;
  if (prev != NULL) 
    {
      for (i = 0; i < state->scanline_len; i++)
        {
          *out++ = *cur++ - *prev++;
        }
    }
  else
    {
      memcpy(out, cur, state->scanline_len);
    }
}

static void
encode_row_average(pdf_uchar_t* cur,
                   pdf_uchar_t* prev,
                   pdf_uchar_t* out,
                   pdf_stm_f_pred_t* state)
{
  pdf_size_t i;
  if (prev != NULL)
    {
      *out++ = *cur++ - (*prev++ >> 1);

      for (i = 1; i < state->scanline_len; i++) 
        {
          *out++ = *cur - (pdf_uchar_t)(((int)*(cur-1) + (int)*prev) >> 1);
          cur++;
          prev++;
        }
    } 
  else 
    {
      *out++ = *cur++;

      for (i = 1; i < state->scanline_len; i++) 
        {
          *out++ = *cur - (*(cur-1) >> 1);
          cur++;
        }
    }
}

static void
encode_row_paeth (pdf_uchar_t* cur, 
                  pdf_uchar_t* prev, 
                  pdf_uchar_t* out,
                  pdf_stm_f_pred_t* state)
{
  pdf_size_t i;
  int p;
  int pa;
  int pb;
  int pc;

  if (prev != NULL) 
    {
      *out++ = *cur++ - *prev++; 

      for (i = 0; i < state->scanline_len; i++) 
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

      for (i = 1; i < state->scanline_len; i++) 
        {
          *out++ = *cur - *(cur-1);
          cur++;
        }
    }
}

static void
encode_row_sub_color16 (pdf_uchar_t* cur, 
                        pdf_uchar_t* prev, 
                        pdf_uchar_t* out,
                        pdf_stm_f_pred_t* state)
{  
  pdf_size_t i;
  pdf_size_t j;

  unsigned short *this;
  unsigned short *next;
  unsigned short *sout;

  this = (unsigned short*) cur;
  next = (unsigned short*) cur;
  sout = (unsigned short*) out;

  for (j = 0; j < state->colors; j++)
    {
      *sout++ = *next++;
    }

  for (i = 1; i < state->columns; i++)
    {
      for (j = 0; j < state->colors; j++)
        {
          *sout++ = *next++ - *this++;
        }
    }
}

static void
encode_row_sub_color8 (pdf_uchar_t* cur, 
                       pdf_uchar_t* prev, 
                       pdf_uchar_t* out,
                       pdf_stm_f_pred_t* state)
{
  pdf_size_t i;
  pdf_size_t j;
  pdf_uchar_t *this;
  pdf_uchar_t *next;
  pdf_uchar_t *sout;

  this = cur; 
  next = cur;
  sout = out;

  for (j = 0; j < state->colors; j++)
    {
      *sout++ = *next++;
    }
  for (i = 1; i < state->columns; i++)
    {
      for (j = 0; j < state->colors; j++)
        {
          *sout++ = *next++ - *this++;
        }
    }
}

static void
encode_row_sub_colorl8 (pdf_uchar_t *cur, 
                        pdf_uchar_t *prev, 
                        pdf_uchar_t *out,
                        pdf_stm_f_pred_t* state)
{
  pdf_size_t i;
  pdf_size_t j;
  pred_bit_ptr_t this;
  pred_bit_ptr_t next;
  pred_bit_ptr_t sout;
  
  pred_bit_ptr_init (&this, cur, state->bits_per_component);
  pred_bit_ptr_init (&next, cur, state->bits_per_component);
  pred_bit_ptr_init (&sout, out, state->bits_per_component);
  
  for (j = 0; j < state->colors; j++) 
    {
      PRED_BIT_PTR_SET(sout, PRED_BIT_PTR_GET(next));
      PRED_BIT_PTR_ADV(sout);
      PRED_BIT_PTR_ADV(next);
    }
  for (i = 1; i < state->columns; i++) 
    {
      for (j = 0; j < state->colors; j++) 
        {
          PRED_BIT_PTR_SET(sout, 
                           PRED_BIT_PTR_GET(next) - PRED_BIT_PTR_GET(this));
          PRED_BIT_PTR_ADV(sout);
          PRED_BIT_PTR_ADV(next);
          PRED_BIT_PTR_ADV(this);
        }
    }
}

static int
encode_row (pdf_uchar_t *cur,
            pdf_uchar_t *prev,
            pdf_uchar_t *out,
            pdf_stm_f_pred_t* state,
            pdf_error_t **error)
{
  switch (state->type)
    {
    case PDF_STM_F_PREDDEC_NO_PREDICTION:
      {
        encode_row_none(cur, prev, out, state);
        break;
      }
    case PDF_STM_F_PREDDEC_TIFF_PREDICTOR_2:
      {
        switch (state->bits_per_component) 
          {
          case 16:
            {
              encode_row_sub_color16(cur, prev, out, state);
              break;
            }
          case 8:
            {
              encode_row_sub_color8(cur, prev, out, state);
              break;
            }
          case 4: case 2: case 1:
            {
              encode_row_sub_colorl8(cur, prev, out, state);
              break;
            }
          default:
            {
              pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_EBADDATA,
                     "bad bits_per_component value: "
                     "expected 1, 2, 4, 8, 16, got %d",
                     (state->bits_per_component));
              return PDF_ERROR;
              break;
            }
          }

        break;
      }
    case PDF_STM_F_PREDDEC_PNG: {
      switch (state->predictor) {
      case PDF_STM_F_PREDDEC_PNG_NONE:
	{
	  encode_row_none (cur, prev, out, state);
	  break;
      }
      case PDF_STM_F_PREDDEC_PNG_SUB:
	{
	  encode_row_sub (cur, prev, out, state);
	  break;
      }
      case PDF_STM_F_PREDDEC_PNG_UP:
	{
	  encode_row_up(cur, prev, out, state);
	  break;
	}
      case PDF_STM_F_PREDDEC_PNG_AVERAGE:
	{
	  encode_row_average(cur, prev, out, state);
	  break;
	}
      case PDF_STM_F_PREDDEC_PNG_PAETH:
	{
	  encode_row_paeth(cur, prev, out, state);
	  break;
	}
      default:
	{
	  pdf_set_error (error,
			 PDF_EDOMAIN_BASE_STM,
			 PDF_EBADDATA,
			 "bad png predictor value in input data: "
			 "expected 0, 1, 2, 3, 4 got %d",
			 (state->predictor));
	  return PDF_ERROR;
	  break;
	}
      }
    }
    }
  return PDF_OK;
}

static void
decode_row_none (pdf_uchar_t* in, 
                 pdf_uchar_t* cur, 
                 pdf_uchar_t* prev,
                 pdf_stm_f_pred_t* state)
{
  memcpy (cur, in, state->scanline_len);
}

static void
decode_row_sub (pdf_uchar_t* in, 
                pdf_uchar_t* cur, 
                pdf_uchar_t* prev,
                pdf_stm_f_pred_t* state)
{
  pdf_size_t i;

  *cur++ = *in++;

  for (i = 1; i < state->scanline_len; i++) 
    {
      *cur = *in++ + *(cur-1);
      cur++; 
    }
}

static void
decode_row_up (pdf_uchar_t* in, 
               pdf_uchar_t* cur, 
               pdf_uchar_t* prev,
               pdf_stm_f_pred_t* state)
{
  pdf_size_t i;

  if (prev != NULL) 
    {
      for (i = 0; i < state->scanline_len; i++)
        {
          *cur++ = *in++ + *prev++;
        }

    } 
  else
    {
      memcpy (cur, in, state->scanline_len);
    }
}

static void
decode_row_average (pdf_uchar_t* in, 
                    pdf_uchar_t* cur, 
                    pdf_uchar_t* prev,
                    pdf_stm_f_pred_t* state)
{
  pdf_size_t i;

  if (prev != NULL) 
    {
      *cur++ = *in++ + (*prev++ >> 1);

      for (i = 1; i < state->scanline_len; i++) 
        {
          *cur = *in++ + (pdf_uchar_t)(((int)*(cur-1) + (int)*prev) >> 1);
          cur++;
          prev++;
        }
    } 
  else 
    {
      *cur++ = *in++;

      for (i = 1; i < state->scanline_len; i++) 
        {
          *cur = *in++ + (*(cur-1) >> 1);
          cur++;
        }
    }
}

static void
decode_row_paeth (pdf_uchar_t* in, 
                  pdf_uchar_t* cur, 
                  pdf_uchar_t* prev,
                  pdf_stm_f_pred_t* state)
{
  int p;
  int pa;
  int pb;
  int pc;
  pdf_size_t i;

  if (prev != NULL) 
    {
      *cur++ = *in++ + *prev++; 

      for (i = 0; i < state->scanline_len; i++) 
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

      for (i = 1; i < state->scanline_len; i++) 
        {
          *cur = *in++ + *(cur-1);
          cur++; 
        }
    }
}

static void
decode_row_sub_color16 (pdf_uchar_t* in, pdf_uchar_t* cur, pdf_uchar_t* prev,
                        pdf_stm_f_pred_t* state)
{
  pdf_size_t i;
  pdf_size_t j;
  unsigned short* this;
  unsigned short* next;
  unsigned short* sin;

  this = (unsigned short*) cur;
  next = (unsigned short*) cur;
  sin = (unsigned short*) in;

  for (j = 0; j < state->colors; j++)
    {
      *next++ = *sin++;
    }
  for (i = 1; i < state->columns; i++)
    {
      for (j = 0; j < state->colors; j++)
        {
          *next++ = *sin++ + *this++;
        }
    }
}

static void
decode_row_sub_color8  (pdf_uchar_t* in, 
                        pdf_uchar_t* cur, 
                        pdf_uchar_t* prev,
                        pdf_stm_f_pred_t* state)
{
  pdf_size_t i;
  pdf_size_t j;
  pdf_uchar_t *this;
  pdf_uchar_t *next;
  pdf_uchar_t *sin;

  this = cur;
  next = cur;
  sin = in;

  for (j = 0; j < state->colors; j++)
    {
      *next++ = *sin++;
    }

  for (i = 1; i < state->columns; i++)
    {
      for (j = 0; j < state->colors; j++)
        {
          *next++ = *sin++ + *this++;
        }
    }
}


static void
decode_row_sub_colorl8 (pdf_uchar_t* in, 
                        pdf_uchar_t* cur, 
                        pdf_uchar_t* prev,
                        pdf_stm_f_pred_t* state)
{
  pdf_size_t i;
  pdf_size_t j;
  pred_bit_ptr_t this;
  pred_bit_ptr_t next;
  pred_bit_ptr_t sin;
  
  pred_bit_ptr_init (&this, cur, state->bits_per_component);
  pred_bit_ptr_init (&next, cur, state->bits_per_component);
  pred_bit_ptr_init (&sin,  in,  state->bits_per_component);
  
  for (j = 0; j < state->colors; j++) 
    {
      PRED_BIT_PTR_SET(next, PRED_BIT_PTR_GET(sin));
      PRED_BIT_PTR_ADV(next);
      PRED_BIT_PTR_ADV(sin);
    }
  for (i = 1; i < state->columns; i++) 
    {
      for (j = 0; j < state->colors; j++) 
        {
          PRED_BIT_PTR_SET(next, 
                           PRED_BIT_PTR_GET(sin) + PRED_BIT_PTR_GET(this));
          PRED_BIT_PTR_ADV(sin);
          PRED_BIT_PTR_ADV(next);
          PRED_BIT_PTR_ADV(this);
        }
    }
}

static int
decode_row (pdf_uchar_t* in,
            pdf_uchar_t* cur,
            pdf_uchar_t* prev,
            pdf_stm_f_pred_t* state,
            pdf_error_t **error)
{
  switch (state->type)
    {
    case PDF_STM_F_PREDDEC_TIFF_PREDICTOR_2:
      {
        switch(state->bits_per_component)
          {
          case 16:
            {
              decode_row_sub_color16(in, cur, prev, state);
              break;
            }
          case 8:
            {
              decode_row_sub_color8(in, cur, prev, state);
              break;
            }
          case 4:
          case 2:
          case 1:
            {
              decode_row_sub_colorl8(in, cur, prev, state);
              break;
            }
          default:
            {
              pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_EBADDATA,
                     "bad bits_per_component value: "
                     "expected 16, 8, 4, 2, 1, got %d",
                     (state->bits_per_component));
              return PDF_ERROR;
              break;
            }
          }
        break;
      }
    case PDF_STM_F_PREDDEC_NO_PREDICTION:
      {
        decode_row_none (in, cur, prev, state);
        break;
      }
    case PDF_STM_F_PREDDEC_PNG: {
	switch (state->predictor) {
	case PDF_STM_F_PREDDEC_PNG_NONE:
	  {
	    decode_row_none (in, cur, prev, state);
	    break;
	  }
	case PDF_STM_F_PREDDEC_PNG_SUB:
	  {
	    decode_row_sub (in, cur, prev, state);
	    break;
	  }
	case PDF_STM_F_PREDDEC_PNG_UP:
	  {
	    decode_row_up (in, cur, prev, state);
	    break;
	  }
	case PDF_STM_F_PREDDEC_PNG_AVERAGE:
	  {
	    decode_row_average (in, cur, prev, state);
	    break;
	  }
	case PDF_STM_F_PREDDEC_PNG_PAETH:
	  {
	    decode_row_paeth (in, cur, prev, state);
	    break;
	  }
	default:
	  {
	    pdf_set_error (error,
			   PDF_EDOMAIN_BASE_STM,
			   PDF_EBADDATA,
			   "bad png predictor value for decode: "
               "expected 0, 1, 2, 3, 4 got %d",
               (state->predictor));
	    return PDF_ERROR;
	    break;
	  }
	}
      }
    }

  return PDF_OK;
}

static enum pdf_stm_filter_apply_status_e
stm_f_predenc_apply (void          *state,
                     pdf_buffer_t  *in,
                     pdf_buffer_t  *out,
                     pdf_bool_t     finish,
                     pdf_error_t  **error)
{
  pdf_stm_f_pred_t *fs = state; /* filter state */

  pdf_bool_t is_png_predictor;
  is_png_predictor = fs->type == PDF_STM_F_PREDDEC_PNG;

  pdf_uchar_t *curr_row;
  pdf_uchar_t *prev_row = NULL;
  pdf_uchar_t *out_buf;

  pdf_size_t in_size;
  PDF_ASSERT (in->wp >= in->rp);

  size_t tocpy;

  /* copy all at once instead of copying each scanline for predictor 1; 
     this is needed because we may not know real scanline_len and therefor use 
     scanline_len = 1 which would be very slow */
  if (fs->type == PDF_STM_F_PREDDEC_NO_PREDICTION)
    {
      tocpy = PDF_MIN (out->size - out->wp, in->wp - in->rp);
      memcpy(out->data + out->wp, in->data + in->rp, tocpy);
      out->wp += tocpy;
      in->rp += tocpy;
    }

  /* copy in->data to buffer and filter it */
  do
    {
      in_size = in->wp - in->rp;
      tocpy = PDF_MIN (fs->curr_row_buf->size - fs->curr_row_buf->wp - 1,
                       in_size);

      memcpy (fs->curr_row_buf->data + fs->curr_row_buf->wp, in->data + in->rp, 
              tocpy);

      fs->curr_row_buf->wp += tocpy;
      in->rp += tocpy;

      /* one scanline is in curr_row_buf ready for filtering */
      if (fs->curr_row_buf->wp - fs->curr_row_buf->rp == fs->scanline_len)
        {
          /* check if out buffer has enough space left */
          pdf_size_t left;
          left = fs->out_row_buf->size - fs->out_row_buf->wp;
          /* PNG predictor needs extra byte per row */
          if (left >= fs->scanline_len + (is_png_predictor? 1 : 0))
            {
              /* write/read PNG predictor at first byte of a row */
              if (is_png_predictor)
                fs->out_row_buf->data[fs->out_row_buf->wp++] = fs->predictor;

              curr_row = (pdf_uchar_t*) fs->curr_row_buf->data;
              out_buf  = (pdf_uchar_t*) fs->out_row_buf->data 
                                       + fs->out_row_buf->wp;

              /* at first row encode_row expects prev_row to be NULL */
              if (fs->prev_row_buf->wp == 0)
                prev_row = NULL;
              else
                prev_row = (pdf_uchar_t*) fs->prev_row_buf->data;

              if (encode_row (curr_row, prev_row, out_buf, fs, error) 
                  == PDF_ERROR)
                return PDF_STM_FILTER_APPLY_STATUS_ERROR;

              fs->out_row_buf->wp += fs->scanline_len;

              /* instead of copying curr to prev, swap addresses */
              pdf_buffer_t *swap_tmp_buf = fs->prev_row_buf;
              fs->prev_row_buf = fs->curr_row_buf;
              fs->curr_row_buf = swap_tmp_buf;

              pdf_buffer_rewind (fs->curr_row_buf);
            }
       }

      /* out_row_buf has data that can be written to out buffer */
      if (out->size - out->wp > 0 && !pdf_buffer_eob_p (fs->out_row_buf))
        {
          tocpy = PDF_MIN (fs->out_row_buf->wp - fs->out_row_buf->rp, 
                           out->size - out->wp);

          memcpy (out->data + out->wp, 
                  fs->out_row_buf->data + fs->out_row_buf->rp, 
                  tocpy);

          out->wp += tocpy;
          fs->out_row_buf->rp += tocpy;
        }
      if (pdf_buffer_eob_p (fs->out_row_buf))
        {
          pdf_buffer_rewind (fs->out_row_buf);
        }

    }
  while (!pdf_buffer_eob_p (in) && !pdf_buffer_full_p (out));

  /* if we do PNG prediction (is_png_predictor) and in->size == out->size
   * it happens that out->data is full and we have not read all of in->data */
  if (pdf_buffer_full_p (out) && !pdf_buffer_eob_p (in))
    return PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT;

  /* final call of this filter; empty out_row_buf */
  if (finish && in->wp - in->rp < 1)
    {
      /* out_row_buf has data because out is full */
      if (!pdf_buffer_eob_p (fs->out_row_buf))
        return PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT;
      /* input % scanline_len != 0 */
      if (!pdf_buffer_eob_p (fs->curr_row_buf))
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_STM,
                         PDF_EBADDATA,
                         "filtering with predictor encoder is incomplete: "
                         "out of data in the middle of a scanline "
                         "or wrong columns parameter detected");
          return PDF_STM_FILTER_APPLY_STATUS_ERROR;
        }
      return PDF_STM_FILTER_APPLY_STATUS_EOF;
    }

  return PDF_STM_FILTER_APPLY_STATUS_NO_INPUT;
}

static enum pdf_stm_filter_apply_status_e
stm_f_preddec_apply (void          *state,
                     pdf_buffer_t  *in,
                     pdf_buffer_t  *out,
                     pdf_bool_t     finish,
                     pdf_error_t  **error)
{
  pdf_stm_f_pred_t *fs = state; /* filter state */

  pdf_bool_t is_png_predictor;
  is_png_predictor = fs->type == PDF_STM_F_PREDDEC_PNG;

  pdf_uchar_t *curr_row;
  pdf_uchar_t *prev_row = NULL;
  pdf_uchar_t *out_buf;

  pdf_size_t in_size;
  PDF_ASSERT (in->wp >= in->rp);

  size_t tocpy;

  /* copy all at once instead of copying each scanline for predictor 1; 
     this is needed because we may not know real scanline_len and therefor use 
     scanline_len = 1 which would be very slow */
  if (fs->predictor == PDF_STM_F_PREDDEC_NO_PREDICTION)
    {
      tocpy = PDF_MIN (out->size - out->wp, in->wp - in->rp);
      memcpy(out->data + out->wp, in->data + in->rp, tocpy);
      out->wp += tocpy;
      in->rp += tocpy;
    }

  /* copy in->data to buffer and filter it */
  do
    {
      in_size = in->wp - in->rp;
      tocpy = fs->curr_row_buf->size - fs->curr_row_buf->wp;
      /* PNG predictor needs 1 byte more in a scanline */
      tocpy -= (is_png_predictor? 0 : 1);
      tocpy = PDF_MIN (in_size, tocpy);

      memcpy (fs->curr_row_buf->data + fs->curr_row_buf->wp, in->data + in->rp,
              tocpy);

      fs->curr_row_buf->wp += tocpy;
      in->rp += tocpy;

      /* curr_row_buf has a scanline (+PNG predictor) and is ready to filter */
      if (fs->scanline_len == fs->curr_row_buf->wp - fs->curr_row_buf->rp
                              - (is_png_predictor ? 1 : 0))
        {
          /* check if out buffer has enough space left */
          pdf_size_t left;
          left = fs->out_row_buf->size - fs->out_row_buf->wp;
          /* PNG predictor needs extra byte per row */
          if (left >= fs->scanline_len + (is_png_predictor? 1 : 0))
            {
              /* write/read PNG predictor at first byte of a row */
              if (is_png_predictor)
                fs->predictor = fs->curr_row_buf->data[fs->curr_row_buf->rp++];

              curr_row = (pdf_uchar_t*) fs->curr_row_buf->data 
                                       + fs->curr_row_buf->rp;
              out_buf  = (pdf_uchar_t*) fs->out_row_buf->data 
                                       + fs->out_row_buf->wp;

              /* at first row decode_row expects prev_row to be NULL */
              if (fs->prev_row_buf->wp == 0)
                prev_row = NULL;
              else
                prev_row = (pdf_uchar_t*) fs->prev_row_buf->data;

              if (decode_row (curr_row, out_buf, prev_row, fs, error)
                  == PDF_ERROR)
                return PDF_STM_FILTER_APPLY_STATUS_ERROR;

              fs->out_row_buf->wp += fs->scanline_len;

              /* copying out_row to prev_row */
              memcpy (fs->prev_row_buf->data, fs->out_row_buf->data,
                  fs->out_row_buf->size);
              fs->prev_row_buf->rp = fs->out_row_buf->rp;
              fs->prev_row_buf->wp = fs->out_row_buf->wp;

              pdf_buffer_rewind (fs->curr_row_buf);
            }
       }

      /* out_row_buf has data that can be written to out buffer */
      if (out->size - out->wp > 0 && !pdf_buffer_eob_p (fs->out_row_buf))
        {
          tocpy = PDF_MIN (fs->out_row_buf->wp - fs->out_row_buf->rp,
              out->size - out->wp);

          memcpy (out->data + out->wp,
              fs->out_row_buf->data + fs->out_row_buf->rp, tocpy);

          out->wp += tocpy;
          fs->out_row_buf->rp += tocpy;
        }
      if (pdf_buffer_eob_p (fs->out_row_buf))
        {
          pdf_buffer_rewind (fs->out_row_buf);
        }
    }
  while (!pdf_buffer_eob_p (in));

  /* final call of this filter; empty out_row_buf */
  if (finish && in->wp - in->rp < 1)
    {
      /* out_row_buf has data because out is full */
      if (!pdf_buffer_eob_p (fs->out_row_buf))
          return PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT;
      /* input % scanline_len != 0 */
      if (!pdf_buffer_eob_p (fs->curr_row_buf))
        {
          pdf_set_error (error,
                         PDF_EDOMAIN_BASE_STM,
                         PDF_EBADDATA,
                         "filtering with predictor decoder is incomplete: "
                         "out of data in the middle of a scanline "
                         "or wrong columns parameter detected");
          return PDF_STM_FILTER_APPLY_STATUS_ERROR;
        }

      return PDF_STM_FILTER_APPLY_STATUS_EOF;
    }

  return PDF_STM_FILTER_APPLY_STATUS_NO_INPUT;
}

static void
stm_f_pred_deinit (void *state)
{
  pdf_stm_f_pred_t *filter_state = state;

  pdf_buffer_destroy (filter_state->prev_row_buf);
  pdf_buffer_destroy (filter_state->curr_row_buf);
  pdf_buffer_destroy (filter_state->out_row_buf);
  pdf_dealloc (state);
}

/* End of pdf_stm_f_pred.c */

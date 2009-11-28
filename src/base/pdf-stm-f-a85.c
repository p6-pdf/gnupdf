/* -*- mode: C -*- Time-stamp: "09/01/11 22:04:55 jemarch"
 *
 *       File:         pdf-stm-f-a85.c
 *       Date:         Mon Jul  9 22:01:41 2007
 *
 *       GNU PDF Library - ASCII85 stream filter
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
#include <string.h>

#include <pdf-types.h>
#include <pdf-stm-f-a85.h>

#include <ctype.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif

/* 
 * static function prototypes 
 */

static pdf_status_t
pdf_stm_f_a85enc_wr_tuple (pdf_u32_t tuple, pdf_u8_t tuple_bytes, 
                           void *state, pdf_buffer_t out);

static pdf_bool_t
quad_can_fit (pdf_buffer_t buffer);

static pdf_status_t
pdf_stm_f_a85dec_wr_quad (const pdf_char_t *quint, const pdf_size_t outcount,
                          pdf_buffer_t out, pdf_stm_f_a85_t filter_state);

static pdf_status_t
pdf_stm_f_a85dec_getnext (pdf_buffer_t in, pdf_char_t *pc);

static pdf_status_t
pdf_stm_f_a85_write_outbuff (const pdf_char_t c, pdf_buffer_t out, 
                             pdf_stm_f_a85_t filter_state);

static pdf_status_t
pdf_stm_f_a85_flush_outbuff (pdf_buffer_t out, pdf_stm_f_a85_t filter_state);

/*
 * Public functions
 */

pdf_status_t
pdf_stm_f_a85enc_init (pdf_hash_t params,
                       void **state)
{
    
  pdf_stm_f_a85_t filter_state;
  
  filter_state = pdf_alloc (sizeof (struct pdf_stm_f_a85_s));
  if (filter_state==NULL)
    {
      return PDF_ERROR;
    }
  
  /* Initialization */

  filter_state->line_length = 0;
  filter_state->spare_count = 0;
  filter_state->output_count = 0;
  filter_state->terminated = PDF_FALSE;
  
  *state = (void *) filter_state;
  return PDF_OK;
}

/* pdf_stm_f_a85_write_out helps deal with the possibility of extremely 
 * small output buffer sizes - like 1 byte for example.  Since this
 * filter needs to work with 4 to 5 input bytes at a time, and since it
 * then produces 1 to 8 output bytes at a time, it is necessary to be
 * able to buffer up to 8 output bytes internally, and then wait for
 * them to be sent to the output stream.
 *
 * pdf_stm_f_a85_write_out sends data directly to the output stream if
 * there is room, and if no data is already waiting to go out.
 * If there is data waiting to go out, then it appends to the end of 
 * the output buffer.
 *
 * All writing to the out stream from this filter should take place
 * through this function. 
 */

static pdf_status_t
pdf_stm_f_a85_write_out (const pdf_char_t c, pdf_buffer_t out, 
                         pdf_stm_f_a85_t filter_state)
{
  pdf_status_t retval = PDF_OK;

  if (!pdf_buffer_full_p (out) && (0 == filter_state->output_count))
    {
      out->data[out->wp++] = c;
      retval = PDF_OK;
    }
  else
    {
      if (filter_state->output_count < A85_OUTPUT_BUFF_LEN)
        {
          filter_state->output_buff[filter_state->output_count++] = c;
          retval = PDF_OK;
        }
      else
        {
          retval = PDF_ERROR;
        }
    }

  return retval;
}

/* pdf_stm_f_a85_flush_outbuff helps deal with the possibility of extremely 
 * small output buffer sizes - like 1 byte for example.  Since this
 * filter needs to work with 4 to 5 input bytes at a time, and since it
 * then produces 1 to 8 output bytes at a time, it is necessary to be
 * able to buffer up to 8 output bytes internally, and then wait for
 * them to be sent to the output stream.
 *
 * Because this output buffer is sized to handle the output data from a single
 * input group (up to 8 bytes for the encoding (5 output data bytes, plus a 
 * newline, plus the terminating sequence)) it is necessary that the output
 * buffer be empty before starting to generate another set of output.
 * 
 * pdf_stm_f_a85_flush_outbuff must return PDF_OK before starting to generate
 * a new set of output bytes.  This ensures that all call to _write_out for
 * this set of output bytes will succeed, and no data will be lost.
 *
 */
static pdf_status_t
pdf_stm_f_a85_flush_outbuff (pdf_buffer_t out, pdf_stm_f_a85_t filter_state)
{
  pdf_status_t retval = PDF_OK;
  pdf_size_t i;
  pdf_size_t k;

  i = 0;

  while ((i<filter_state->output_count) && (PDF_OK == retval))
    {
      if (pdf_buffer_full_p(out))
        {
          retval = PDF_ENOUTPUT;
        }
      else
        {
          out->data[out->wp++] = filter_state->output_buff[i++];
        }
    }

  if ((PDF_ENOUTPUT == retval) || (i == filter_state->output_count))
    { /* done flushing (to the extent possible) */
      /* shift remaining bytes back to the beginning of buffer */
      
      for (k=0; (k+i)<filter_state->output_count; k++)
        {
          filter_state->output_buff[k] = filter_state->output_buff[k+i];
        }
      
      filter_state->output_count -= i;
      
    }
  else
    { /* This should not be reachable */
      retval = PDF_ERROR;
    }
  
  return retval;
}



static pdf_status_t
pdf_stm_f_a85enc_wr_tuple (pdf_u32_t tuple,
                           pdf_u8_t tuple_bytes, /* always 4 for a full tuple */
                           void *state,
                           pdf_buffer_t out)
{
  pdf_status_t retval = PDF_OK;
  pdf_stm_f_a85_t filter_state;
  pdf_char_t buf[5];
  int i;

  filter_state = (pdf_stm_f_a85_t) state;

  if (tuple == 0 && 4 == tuple_bytes) /* partial tuple doesn't get z handling */
    {
      /* Four 0s in row */
      
      retval = pdf_stm_f_a85_write_out ('z', out, filter_state);
      //      *!*!*!* examine this
      filter_state->line_length++;
      
      if (filter_state->line_length >= A85_ENC_LINE_LENGTH)
        {
          retval = pdf_stm_f_a85_write_out ('\n', out, filter_state);
          //      *!*!*!* examine this
          filter_state->line_length = 0;
        }
    }
  else
    {
      /* Encode this tuple in base-85 */
      for (i = 0;
           i < 5;
           i++)
        {
          buf[i] = (pdf_char_t)(tuple % 85);
          tuple = tuple / 85;
        }
      
      for (i = tuple_bytes;
           i >= 0;
           i--)
        {
          retval = pdf_stm_f_a85_write_out (buf[i] + (pdf_char_t)'!', 
                                            out, filter_state);
          //      *!*!*!* examine this
          
          filter_state->line_length++;
          if (filter_state->line_length >= A85_ENC_LINE_LENGTH)
            {
              retval = pdf_stm_f_a85_write_out ('\n', out, filter_state);
              //      *!*!*!* examine this
              filter_state->line_length = 0;
            }
        }
    }
  
  return retval;
}

pdf_status_t
pdf_stm_f_a85enc_apply (pdf_hash_t params, void *state, pdf_buffer_t in,
                        pdf_buffer_t out, pdf_bool_t finish_p)
{
  pdf_size_t in_size;
  pdf_stm_f_a85_t filter_state;

  pdf_u32_t tuple;
  pdf_size_t read_bytes;
  pdf_size_t pos_in;
  int avail;
  int i;
  pdf_char_t buf[5];
  pdf_char_t temp_byte;
  pdf_status_t retval = PDF_OK;

  filter_state = (pdf_stm_f_a85_t) state;
  
  /* Fill the output buffer with the contents of the input buffer, but
     note that the second may be bigger than the former */

  /* ASCII base-85 encoding produces 5 ASCII characters for every 4
     bytes in the input data.

     But take care of the EOD marker (~>) and the newlines/whitespace.

     Note that since 0000 is coded with 'z', this is an upper bound
     rather than the final length of the output buffer. 

     Note also that A85_ENC_LINE_LENGTH should be an integral divisor of 4. */

  
  /* Make sure that the output_buff is flushed and ready to accept another */
  /* batch of data */

  retval = pdf_stm_f_a85_flush_outbuff (out, filter_state);

  if (PDF_OK != retval)
    { /* Probably this is PDF_ENOUTPUT - meaning output_buff still has data */
      return retval;
    }

  /* Pull any leftover data from last _apply from state->spare_bytes */

  if (filter_state->spare_count)
    {
      avail = in->wp - in->rp;
      if ((avail + filter_state->spare_count) >= 4)
        { /* There is a full tuple available */

          tuple = 0;

          i = 0;

          tuple = tuple | (filter_state->spare_bytes[i++] << 24);
          filter_state->spare_count--;
          
          if (filter_state->spare_count)
            {
              temp_byte = filter_state->spare_bytes[i++];
              filter_state->spare_count--;
            }
          else
            {
              temp_byte = in->data[in->rp++];
            }
          
          tuple = tuple | (temp_byte << 16);

          if (filter_state->spare_count)
            { /* don't bother incrementing i here - not used again */
              temp_byte = filter_state->spare_bytes[i];
              filter_state->spare_count--;
            }
          else
            {
              temp_byte = in->data[in->rp++];
            }
          
          tuple = tuple | (temp_byte << 8);

          tuple = tuple | in->data[in->rp++];

          retval = pdf_stm_f_a85enc_wr_tuple (tuple, 4, filter_state, out);

        } /* End of if there is a full tuple available */
      else
        {
          /* There was no full tuple available - do nothing. 
             If finish_p is set, then a partial tuple will be written later */
          retval = PDF_ENINPUT;
        }

    } /* end of if spare_count */

  retval = pdf_stm_f_a85_flush_outbuff (out, filter_state);
  
  if (PDF_OK != retval)
    { /* Probably this is PDF_ENOUTPUT - meaning output_buff still has data */
      return retval;
    }


  /* Now do all normal tuples */

  in_size = in->wp - in->rp;
  

  if ((PDF_OK == retval) && (in_size >= 4))
    {
      
      for (pos_in = 0;
           (pos_in < in_size) && (PDF_OK == retval);
           pos_in += 4)
        {
          tuple = 0;
          avail = in_size - pos_in;

          if (avail >= 4)
            {
              tuple = tuple | (in->data[in->rp] << 24);
              tuple = tuple | (in->data[in->rp + 1] << 16);
              tuple = tuple | (in->data[in->rp + 2] << 8);
              tuple = tuple | in->data[in->rp + 3];

              in->rp += 4;

              retval = pdf_stm_f_a85enc_wr_tuple (tuple, 4, filter_state, out);

              retval = pdf_stm_f_a85_flush_outbuff (out, filter_state);

            } /* end of whole tuple available */

        } /* end for loop */

      if (PDF_OK == retval)
        { /* Loop terminated by PDF_ENINPUT */
          retval = PDF_ENINPUT;
        }
          
    } /* End if retval == PDF_OK && whole tuple available */
  else if ((PDF_OK == retval) && (in_size < 4))
    {
      retval = PDF_ENINPUT;
    }
  /* Now if finish_p do a partial tuple if there are leftover bytes */

  if (finish_p && (PDF_ENINPUT == retval))
    {
      
      /* If the length of the binary data to be encoded is not a multiple of 4 
         bytes, the last, partial group of 4 is used to produce a last, partial
         group of 5 output  characters. Given n (1, 2 or 3) bytes of binary 
         data, the encoder first appends 4 - n zero bytes to make a complete 
         group of 4. It then encodes this group in the usual way, but without 
         applying the special z case. Finally, it writes only the first n + 1 
         characters of the resulting group of 5. */
      
      in_size = filter_state->spare_count + (in->wp - in->rp);

      if (in_size)
        {

          for (i=0; i<(4-in_size); i++)
            {
              buf[i] = 0;
            }
          
          read_bytes = 0;
              
          for (i=4-in_size;
               i<4;
               i++)
            {
              
              avail = in->wp - in->rp;
              
              if (filter_state->spare_count || avail)
                {
                  if (filter_state->spare_count)
                    {
                      buf[i] = filter_state->spare_bytes[read_bytes++];
                      filter_state->spare_count--;
                    }
                  else
                    {
                      buf[i] = in->data[in->rp++];
                    }
                }
              else
                {
                  buf[i] = 0;
                }
              
            }
          
          tuple = 0;
          tuple = tuple | buf[0] << 24;
          tuple = tuple | buf[1] << 16;
          tuple = tuple | buf[2] << 8;
          tuple = tuple | buf[3];

          retval = pdf_stm_f_a85enc_wr_tuple (tuple, in_size, filter_state, out);
          

        } /* end of if(in_size) */
      else
        {
          /* There was no partial tuple to write out. Do Nothing. */
        }

      if (!filter_state->terminated)
        {
          /* Insert the EOD marker */
          retval = pdf_stm_f_a85_write_out ('~', out, filter_state);
          retval = pdf_stm_f_a85_write_out ('>', out, filter_state);

          filter_state->terminated = PDF_TRUE;
        }

    }
  else if (PDF_ENINPUT == retval)
    { /* finish_p not set, so save partial tuple (if present) for next call */
      avail = in->wp - in->rp;
      while (avail)
        {
          if (filter_state->spare_count >= A85_SPARE_BYTES_LEN)
            {
              retval = PDF_ERROR;
            }
          else
            {
              filter_state->spare_bytes[filter_state->spare_count++] = 
                in->data[in->rp++];
            }
          avail--;
        }
    }


  if (finish_p && (PDF_OK == retval))
    {
      retval = pdf_stm_f_a85_flush_outbuff (out, filter_state);

      if (PDF_OK == retval)
        { /* We have completed this filter job: */
          retval = PDF_EEOF;
        }
    }
  
  return retval;

}

pdf_status_t
pdf_stm_f_a85enc_dealloc_state (void *state)
{
  pdf_stm_f_a85_t a85_state;
  a85_state = (pdf_stm_f_a85_t) state;
  pdf_dealloc (a85_state);
  
  return PDF_OK;
}

static pdf_bool_t
quad_can_fit (pdf_buffer_t buffer)
{
  return (buffer->size >= (buffer->wp +4));
}

/* Do not Pass this function a 'z' quint. */
/* Make certain that there is room in the output buffer before calling */
static pdf_status_t
pdf_stm_f_a85dec_wr_quad (const pdf_char_t *quint, const pdf_size_t outcount,
                          pdf_buffer_t out, pdf_stm_f_a85_t filter_state)
{
  pdf_status_t retval = PDF_OK;
  pdf_u32_t quad;
  int i;
  pdf_char_t outbytes[4];

  if ((outcount < 1) || (outcount > 4))
    {
      retval = PDF_ERROR;
      return retval;
    }

  /* Error check on quint contents */

  if (quint[0] > 82)
    { /* 83 * 85^4 is greater than 2^32 all by itself. */
      retval = PDF_ERROR;
    }
  
  if ((quint[1] > 84) || (quint[2] > 84) || 
      (quint[3] > 84) || (quint[4] > 84) )
    { /* any character here greater than 84 is an error */
      retval = PDF_ERROR;
    }
  
  /* Begin Conversion to quad. */
  quad = (pdf_u32_t)quint[0] * 85u;
  quad = (quad + (pdf_u32_t)quint[1]) * 85u;
  quad = (quad + (pdf_u32_t)quint[2]) * 85u;
  
  /* Now we could be adding up to too big a number */
  /* Go in smaller steps and check for potential overflow: */
  quad = (quad + (pdf_u32_t)quint[3]);
  
  if (quad > (pdf_u32_t)((pdf_u32_t)UINT32_MAX / (pdf_u32_t)85u))
    { /* This would overflow, so it's an error. Bail */
      retval = PDF_ERROR;
    }
  else
    {
      quad = quad * 85u; /* Complete the incorporation of quint[3] */
      
      if (((pdf_u32_t)(UINT32_MAX - quad)) >= (pdf_u32_t)quint[4])
        {
          quad += quint[4];
        }
      else
        {
          retval = PDF_ERROR;
        }
    }

  if (PDF_OK == retval)
    {
      for (i=0; i<4; i++)
        {
          outbytes[3 - i] = quad % 256;
          quad = quad / 256;
        }

      for (i=4-outcount; i<4; i++)
        {
          retval = pdf_stm_f_a85_write_out (outbytes[i], out, filter_state);
        }
    }
  
  return retval;
}


pdf_status_t
pdf_stm_f_a85dec_init (pdf_hash_t params, void **state)
{
    
  pdf_stm_f_a85_t filter_state;
  
  filter_state = pdf_alloc (sizeof (struct pdf_stm_f_a85_s));
  if (filter_state==NULL)
    {
      return PDF_ERROR;
    }
  
  /* Initialization */
  
  filter_state->line_length = 0;
  filter_state->spare_count = 0;
  filter_state->output_count = 0;
  
  
  *state = (void *) filter_state;
  return PDF_OK;
}

/* Get the next non-whitespace character */
static pdf_status_t
pdf_stm_f_a85dec_getnext (pdf_buffer_t in, pdf_char_t *pc)
{
  /* Get the next non-whitespace character */
  int done = FALSE;
  pdf_status_t retval = PDF_OK;
  pdf_char_t c;

  while (!done)
    {
      if (in->wp > in->rp)
        {
          c = in->data[in->rp++];
          if (!isspace(c))
            { /* Found a non-whitespace character. We're done. */
              done = TRUE;
              retval = PDF_OK;
              *pc = c;
            } 
        }
      else
        { /* ran out of input data. We're done. */
          retval = PDF_ENINPUT;
          done = TRUE;
        }
    }

  return retval;
}

#define A85_INVALID_TERM_IDX 255

pdf_status_t
pdf_stm_f_a85dec_apply (pdf_hash_t params, void *state, pdf_buffer_t in,
                        pdf_buffer_t out, pdf_bool_t finish_p)
{
  pdf_stm_f_a85_t filter_state;
  pdf_char_t quint[5];
  pdf_char_t tr_quint[5];
  pdf_size_t q_idx;
  int i;
  int done;
  int oldone; /* Outer Loop Done */
  pdf_size_t term_idx;
  pdf_status_t retval = PDF_OK;

  filter_state = (pdf_stm_f_a85_t) state;
  
  /* Fill the output buffer with the contents of the input buffer, but
     note that the second may be bigger than the former */

  /* Only process the next bit of input if the output buffer is empty */
  retval = pdf_stm_f_a85_flush_outbuff (out, filter_state);
  if (PDF_OK != retval)
    { 
      return retval;
    }


  oldone = PDF_FALSE;

  while (!oldone)
    {

      q_idx = 0;
      term_idx = A85_INVALID_TERM_IDX;

      /* First pull any leftover bytes from last _apply(...) and prepend here */
      if (filter_state->spare_count)
        {
          /* First pull the spare_bytes into the quint */
          for (i=0; i<filter_state->spare_count; i++)
            {
              quint[q_idx] = filter_state->spare_bytes[i];
              if ('~' == quint[q_idx])
                {
                  term_idx = q_idx;
                }
              q_idx++;
            }
          filter_state->spare_count = 0;
        }
      
      /* Now attempt to fill up the rest of the quint from the input stream */
      done = FALSE;
      for (i=q_idx; ((i<5) && !done); i++)
        {
          retval = pdf_stm_f_a85dec_getnext(in, &(quint[q_idx]));
          if (PDF_ENINPUT == retval)
            { /* Ran out of input before filling quint */
              done = TRUE;
            }
          else
            {
              if ('~' == quint[q_idx])
                {
                  term_idx = q_idx;
                }

              if ((0 == q_idx) && ('z' == quint[0]))
                {
                  done = PDF_TRUE;
                }
              
              q_idx++;
            }
        }


      if (A85_INVALID_TERM_IDX == term_idx)
        { /* Normal Processing  */
          if (5 != q_idx)
            {
              if ((1 == q_idx) && ('z' == quint[0]) && (PDF_OK == retval))
                { /* special case 'z' encodes value 0x00000000 */
                  retval = pdf_stm_f_a85_write_out (0, out, filter_state);
                  retval = pdf_stm_f_a85_write_out (0, out, filter_state);
                  retval = pdf_stm_f_a85_write_out (0, out, filter_state);
                  retval = pdf_stm_f_a85_write_out (0, out, filter_state);
                  
                  q_idx = 0;
                }
              else if (PDF_ENINPUT == retval)
                { /* Not full dataset.  Save this data for later */
                  for (i=0; i<q_idx; i++)
                    {
                      filter_state->spare_bytes[i] = quint[i];
                    }
                  filter_state->spare_count = q_idx;
                  q_idx = 0; /* Clear this so that the output loop is skipped.*/
                  oldone = PDF_TRUE;
                }
              else if (finish_p)
                { /* finish specified, but valid term. sequence not present */
                  retval = PDF_ERROR;
                  oldone = PDF_TRUE;
                }
              else
                { /* some other error than PDF_ENINPUT */
                  /* Let this other error pass through. */
                  oldone = PDF_TRUE;
                }
            }
          else if ((5 == q_idx) && (PDF_OK == retval))
            { /* 5 good output bytes present */
              /* remove the offset from each byte */
              for (i=0; i<5; i++)
                {
                  tr_quint[i] = quint[i] - '!';
                  
                }
              
              retval = pdf_stm_f_a85dec_wr_quad (tr_quint, 4, out, filter_state);
              
              if (PDF_OK != retval)
                {
                  /* Something wrong in writing out data. Save for next call */
                  for (i=0; i<q_idx; i++)
                    {
                      filter_state->spare_bytes[i] = quint[i];
                    }
                  filter_state->spare_count = q_idx;
                  oldone = PDF_TRUE;
                }
              
            }
        }
      else
        { /* Start of termination pattern detected. Not-To-Be-Processed bytes */

          if ((0 == term_idx) && (2 == q_idx)
              && ('>' == quint[1]) && (PDF_ENINPUT == retval))
            { /* just term pattern. */
              
              /* Do nothing here */
              
            }
          else if (1 == term_idx)
            { /* There is a single byte to process before term sequence */
              /* This is an error */
              retval = PDF_ERROR;
              oldone = TRUE;
            }
          else if ((2 == term_idx) && (4 == q_idx)
                   && ('>' == quint[3]) && (PDF_ENINPUT == retval))
            { /* There are Two data bytes to work on */
              tr_quint[0] = 0;
              tr_quint[1] = 0;
              tr_quint[2] = 0;
              tr_quint[3] = quint[0] - '!';
              tr_quint[4] = quint[1] - '!';

              retval = pdf_stm_f_a85dec_wr_quad (tr_quint, 1, out, filter_state);

              /* Put term sequence at beginning of quint */
              q_idx = 0;
              quint[q_idx++] = quint[2];
              quint[q_idx++] = quint[3];

            }
          else if ((3 == term_idx) && (5 == q_idx) && ('>' == quint[4]))
            { /* There are Three data bytes and a term pattern */
              tr_quint[0] = 0;
              tr_quint[1] = 0;
              tr_quint[2] = quint[0] - '!';
              tr_quint[3] = quint[1] - '!';
              tr_quint[4] = quint[2] - '!';

              retval = pdf_stm_f_a85dec_wr_quad (tr_quint, 2, out, filter_state);

              /* Put term sequence at beginning of quint */
              q_idx = 0;
              quint[q_idx++] = quint[3];
              quint[q_idx++] = quint[4];

            }
          else if ((4 == term_idx) && (5 == q_idx))
            { /* There are Four data bytes to work on, and 1/2 of term patt. */
              tr_quint[0] = 0;
              tr_quint[1] = quint[0] - '!';
              tr_quint[2] = quint[1] - '!';
              tr_quint[3] = quint[2] - '!';
              tr_quint[4] = quint[3] - '!';

              retval = pdf_stm_f_a85dec_wr_quad (tr_quint, 3, out, filter_state);

              q_idx = 0;
              quint[q_idx++] = quint[4];

            }
          else
            { /* Some kind of invalid condition is present. */
              retval = PDF_ERROR;
              oldone = TRUE;
            }

          /* Now check to see if we have valid finishing conditions: */
          if ((PDF_ENINPUT == retval) && (2 == q_idx)
              && ('~' == quint[0]) && ('>' == quint[1]))
            {
              if (finish_p)
                {
                  /* All we have is term code, and finish is requested. fini */
                  retval = PDF_EEOF;
                  oldone = TRUE;
                }
              else
                { /* finish_p not set. save the bytes for later */
                  for (i=0; i<q_idx; i++)
                    {
                      filter_state->spare_bytes[i] = quint[i];
                    }
                  filter_state->spare_count = q_idx;
                  oldone = TRUE;
                }
            }
        }

      if (!oldone && PDF_OK == retval)
        {
          retval = pdf_stm_f_a85_flush_outbuff (out, filter_state);

          if (PDF_OK != retval)
            {
              oldone = PDF_TRUE;
            }
        }

    }
  return retval;
}

pdf_status_t
pdf_stm_f_a85dec_dealloc_state (void *state)
{
  pdf_stm_f_a85_t a85_state;
  a85_state = (pdf_stm_f_a85_t) state;
  pdf_dealloc (a85_state);
  
  return PDF_OK;
}

/* End of pdf_stm_f_a85.c */

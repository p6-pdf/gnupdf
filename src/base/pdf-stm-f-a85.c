/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-a85.c
 *       Date:         Mon Jul  9 22:01:41 2007
 *
 *       GNU PDF Library - ASCII85 stream filter
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
#include <string.h>
#include <ctype.h>

#include <pdf-types.h>
#include <pdf-stm-f-a85.h>
#include <pdf-types-buffer.h>
#include <pdf-hash.h>

/* Define A85 encoder */
PDF_STM_FILTER_DEFINE (pdf_stm_f_a85enc_get,
                       stm_f_a85_init,
                       stm_f_a85enc_apply,
                       stm_f_a85_deinit);

/* Define A85 decoder */
PDF_STM_FILTER_DEFINE (pdf_stm_f_a85dec_get,
                       stm_f_a85_init,
                       stm_f_a85dec_apply,
                       stm_f_a85_deinit);

/* If something goes wrong in writing out data, 5 bytes could be leftover */
#define A85_SPARE_BYTES_LEN 5
#define A85_OUTPUT_BUFF_LEN 8

/* defined as 79 so that the newline character completes the 80 char line */
/* (of course that only happens when newline is a single character...)    */
#define A85_ENC_LINE_LENGTH 79

/* Internal state */
struct pdf_stm_f_a85_s
{
  /* spare_bytes will store leftover bytes if finish is not set */
  pdf_size_t line_length;
  pdf_size_t spare_count; /* Number of input bytes in spare_bytes */
  pdf_size_t output_count; /* Number of output bytes left in output_buff */
  pdf_bool_t terminated;
  pdf_char_t spare_bytes[A85_SPARE_BYTES_LEN];
  pdf_char_t output_buff[A85_OUTPUT_BUFF_LEN];
};

/* Common implementation */

static pdf_bool_t
stm_f_a85_init (const pdf_hash_t  *params,
                void             **state,
                pdf_error_t      **error)
{
  struct pdf_stm_f_a85_s *filter_state;

  /* This filter uses no parameters */
  /* Allocate the internal state structure */
  filter_state = pdf_alloc (sizeof (struct pdf_stm_f_a85_s));
  if (!filter_state)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "cannot create ASCII-85 encoder/decoder internal state: "
                     "couldn't allocate %lu bytes",
                     (unsigned long)sizeof (struct pdf_stm_f_a85_s));
      return PDF_FALSE;
    }

  /* Initialization */
  filter_state->line_length = 0;
  filter_state->spare_count = 0;
  filter_state->output_count = 0;
  filter_state->terminated = PDF_FALSE;

  *state = (void *) filter_state;
  return PDF_TRUE;
}

static void
stm_f_a85_deinit (void *state)
{
  pdf_dealloc (state);
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
 *
 * it is critical that pdf_stm_a85_flush_outbuff return PDF_OK
 * (meaning that it's output buffers are empty) before starting
 * to send a new group of output bytes with pdf_stm_f_a85_write_out.
 *
 */

static pdf_bool_t
stm_f_a85_write_out (pdf_char_t              c,
                     pdf_buffer_t           *out,
                     struct pdf_stm_f_a85_s *filter_state)
{
  if (!pdf_buffer_full_p (out) &&
      filter_state->output_count == 0)
    {
      out->data[out->wp++] = c;
      return PDF_TRUE;
    }

  if (filter_state->output_count < A85_OUTPUT_BUFF_LEN)
    {
      filter_state->output_buff[filter_state->output_count++] = c;
      return PDF_TRUE;
    }

  return PDF_FALSE;
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
stm_f_a85_flush_outbuff (pdf_buffer_t           *out,
                         struct pdf_stm_f_a85_s *filter_state)
{
  pdf_status_t retval = PDF_OK;
  pdf_size_t i;
  pdf_size_t k;

  i = 0;

  while ((i < filter_state->output_count) && (PDF_OK == retval))
    {
      if (pdf_buffer_full_p (out))
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

      for (k = 0; (k + i) < filter_state->output_count; k++)
        {
          filter_state->output_buff[k] = filter_state->output_buff[k + i];
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
stm_f_a85enc_wr_tuple (pdf_u32_t     tuple,
                       pdf_u8_t      tuple_bytes, /* always 4 for a full tuple */
                       void         *state,
                       pdf_buffer_t *out)
{
  struct pdf_stm_f_a85_s *filter_state;
  pdf_char_t buf[5];
  int i;

  filter_state = (struct pdf_stm_f_a85_s *) state;

  /* partial tuple doesn't get z handling */
  if (tuple == 0 &&
      tuple_bytes == 4)
    {
      /* Four 0s in row */

      /*
       * PDF_FALSE from pdf_stm_f_a85_write_out means full buffer
       * This should never happen because buffer should have been
       * flushed before starting this set of output data. All
       * subsequent calls to _write_out will fail with PDF_FALSE
       * also, so it is reasonable to ignore the pass or fail
       * status of these invocations within this function. The
       * error will be reported back to the caller of this function.
       */
      if (!stm_f_a85_write_out ('z', out, filter_state))
        return PDF_ERROR;

      filter_state->line_length++;
      if (filter_state->line_length >= A85_ENC_LINE_LENGTH)
        {
          if (!stm_f_a85_write_out ('\n', out, filter_state))
            return PDF_ERROR;

          filter_state->line_length = 0;
        }
      return PDF_OK;
    }

  /* Encode this tuple in base-85 */
  for (i = 0; i < 5; i++)
    {
      buf[i] = (pdf_char_t) (tuple % 85);
      tuple = tuple / 85;
    }

  for (i = tuple_bytes; i >= 0; i--)
    {
      if (!stm_f_a85_write_out (buf[i] + (pdf_char_t) '!',
                                out, filter_state))
        return PDF_ERROR;

      filter_state->line_length++;
      if (filter_state->line_length >= A85_ENC_LINE_LENGTH)
        {
          if (!stm_f_a85_write_out ('\n', out, filter_state))
            return PDF_ERROR;

          filter_state->line_length = 0;
        }
    }

  return PDF_OK;
}

static pdf_status_t
a85enc_apply (void         *state,
              pdf_buffer_t *in,
              pdf_buffer_t *out,
              pdf_bool_t    finish)
{
  pdf_size_t in_size;
  struct pdf_stm_f_a85_s *filter_state;

  pdf_u32_t tuple;
  pdf_size_t read_bytes;
  pdf_size_t pos_in;
  int avail;
  int i;
  pdf_char_t buf[5];
  pdf_char_t temp_byte;
  pdf_status_t retval = PDF_OK;

  filter_state = (struct pdf_stm_f_a85_s *) state;

  /*
   * ASCII base-85 encoding produces 5 ASCII characters for every 4
   * bytes in the input data.
   *
   * But take care of the EOD marker (~>) and the newlines/whitespace.
   *
   * Note that 0x00000000 is coded with 'z',
   */

  /*
   * Make sure that the output_buff is flushed and ready to accept another
   * batch of data
   */

  retval = stm_f_a85_flush_outbuff (out, filter_state);

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

          retval = stm_f_a85enc_wr_tuple (tuple, 4, filter_state, out);
        } /* End of if there is a full tuple available */
      else
        {
          /* There was no full tuple available - do nothing.
             If finish is set, then a partial tuple will be written later */
          retval = PDF_ENINPUT;
        }
    } /* end of if spare_count */

  if (retval == PDF_ERROR)
    return retval;

  retval = stm_f_a85_flush_outbuff (out, filter_state);

  if (PDF_OK != retval)
    { /* Probably this is PDF_ENOUTPUT - meaning output_buff still has data */
      return retval;
    }

  /* Now do all normal tuples */

  in_size = in->wp - in->rp;


  if ((PDF_OK == retval) && (in_size >= 4))
    {

      for (pos_in = 0; (pos_in < in_size) && (PDF_OK == retval); pos_in += 4)
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

              retval = stm_f_a85enc_wr_tuple (tuple, 4, filter_state, out);
              if (retval != PDF_OK)
                return retval;

              retval = stm_f_a85_flush_outbuff (out, filter_state);

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
  /* Now if finish do a partial tuple if there are leftover bytes */

  if (finish && (PDF_ENINPUT == retval))
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

          for (i = 0; i < (4 - in_size); i++)
            {
              buf[i] = 0;
            }

          read_bytes = 0;

          for (i = (4 - in_size); i < 4; i++)
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

          retval =
            stm_f_a85enc_wr_tuple (tuple, in_size, filter_state, out);


        } /* end of if (in_size) */
      else
        {
          /* There was no partial tuple to write out. Do Nothing. */
        }

      if (!filter_state->terminated)
        {
          /* Insert the EOD marker */
          if (!stm_f_a85_write_out ('~', out, filter_state) ||
              !stm_f_a85_write_out ('>', out, filter_state))
            return PDF_ERROR;

          filter_state->terminated = PDF_TRUE;
        }

    }
  else if (PDF_ENINPUT == retval)
    { /* finish not set, so save partial tuple (if present) for next call */
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


  if (finish && (PDF_OK == retval))
    {
      retval = stm_f_a85_flush_outbuff (out, filter_state);

      if (PDF_OK == retval)
        { /* We have completed this filter job: */
          retval = PDF_EEOF;
        }
    }

  return retval;

}

/* Do not Pass this function a 'z' quint. */
/* Make certain that there is room in the output buffer before calling */
static pdf_status_t
stm_f_a85dec_wr_quad (const pdf_char_t       *quint,
                      const pdf_size_t        outcount,
                      pdf_buffer_t           *out,
                      struct pdf_stm_f_a85_s *filter_state)
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
      (quint[3] > 84) || (quint[4] > 84))
    { /* any character here greater than 84 is an error */
      retval = PDF_ERROR;
    }

  /* Begin Conversion to quad. */
  quad = (pdf_u32_t) quint[0] * 85u;
  quad = (quad + (pdf_u32_t) quint[1]) * 85u;
  quad = (quad + (pdf_u32_t) quint[2]) * 85u;

  /* Now we could be adding up to too big a number */
  /* Go in smaller steps and check for potential overflow: */
  quad = (quad + (pdf_u32_t) quint[3]);

  if (quad > (pdf_u32_t) ((pdf_u32_t) UINT32_MAX / (pdf_u32_t) 85u))
    { /* This would overflow, so it's an error. Bail */
      retval = PDF_ERROR;
    }
  else
    {
      quad = quad * 85u; /* Complete the incorporation of quint[3] */

      if (((pdf_u32_t) (UINT32_MAX - quad)) >= (pdf_u32_t) quint[4])
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
      for (i = 0; i < 4; i++)
        {
          outbytes[3 - i] = quad % 256;
          quad = quad / 256;
        }

      for (i = (4 - outcount); i < 4; i++)
        {
          if (!stm_f_a85_write_out (outbytes[i], out, filter_state))
            return PDF_ERROR;
        }
    }

  return retval;
}

/* Get the next non-whitespace character */
static pdf_status_t
stm_f_a85dec_getnext (pdf_buffer_t *in,
                      pdf_char_t   *pc)
{
  /* Get the next non-whitespace character */
  int done = PDF_FALSE;
  pdf_status_t retval = PDF_OK;
  pdf_char_t c;

  while (!done)
    {
      if (in->wp > in->rp)
        {
          c = in->data[in->rp++];
          if (!isspace (c))
            { /* Found a non-whitespace character. We're done. */
              done = PDF_TRUE;
              retval = PDF_OK;
              *pc = c;
            }
        }
      else
        { /* ran out of input data. We're done. */
          retval = PDF_ENINPUT;
          done = PDF_TRUE;
        }
    }

  return retval;
}

#define A85_INVALID_TERM_IDX 255

static pdf_status_t
a85dec_apply (void         *state,
              pdf_buffer_t *in,
              pdf_buffer_t *out,
              pdf_bool_t    finish)
{
  struct pdf_stm_f_a85_s *filter_state;
  pdf_char_t quint[5];
  pdf_char_t tr_quint[5];
  pdf_size_t q_idx;
  int i;
  int done;
  int oldone; /* Outer Loop Done */
  pdf_size_t term_idx;
  pdf_status_t retval = PDF_OK;
  int k;

  filter_state = (struct pdf_stm_f_a85_s *) state;

  /* Fill the output buffer with the contents of the input buffer, but
     note that the second may be bigger than the former */

  /* Only process the next bit of input if the output buffer is empty */
  retval = stm_f_a85_flush_outbuff (out, filter_state);
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
          for (i = 0; i < filter_state->spare_count; i++)
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
      done = PDF_FALSE;
      for (i = q_idx; ((i < 5) && !done); i++)
        {
          retval = stm_f_a85dec_getnext (in, &(quint[q_idx]));
          if (PDF_ENINPUT == retval)
            { /* Ran out of input before filling quint */
              done = PDF_TRUE;
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
                  if (!stm_f_a85_write_out (0, out, filter_state) ||
                      !stm_f_a85_write_out (0, out, filter_state) ||
                      !stm_f_a85_write_out (0, out, filter_state) ||
                      !stm_f_a85_write_out (0, out, filter_state))
                    return PDF_FALSE;
                }
              else if (PDF_ENINPUT == retval)
                { /* Not full dataset.  Save this data for later */
                  for (i = 0; i < q_idx; i++)
                    {
                      filter_state->spare_bytes[i] = quint[i];
                    }
                  filter_state->spare_count = q_idx;
                  oldone = PDF_TRUE;
                }
              else if (finish)
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
              for (i = 0; i < 5; i++)
                {
                  tr_quint[i] = quint[i] - '!';

                }

              retval = stm_f_a85dec_wr_quad (tr_quint, 4, out,
                                             filter_state);

              if (PDF_OK != retval)
                {
                  /* Something wrong in writing out data. Save for next call */
                  for (i = 0; i < q_idx; i++)
                    {
                      filter_state->spare_bytes[i] = quint[i];
                    }
                  filter_state->spare_count = q_idx;
                  oldone = PDF_TRUE;
                }

            }
        }
      else
        { /* Start of termination pattern detected. */

          if (1 == term_idx)
            {
              /* There is a single byte to process before term sequence */
              /* This is an error */
              retval = PDF_ERROR;
              oldone = PDF_TRUE;
            }
          else if (term_idx > 1)
            {
              /* Prepare quint for handoff to wr_quad */
              for (i = 0; i < (5 - term_idx); i++)
                { /* Pad with zeroes */
                  tr_quint[i] = 0;
                }

              for (i = 0; i < term_idx; i++)
                { /* Pull in data bytes */
                  tr_quint[(5 - term_idx) + i] = quint[i] - '!';
                }

              retval = stm_f_a85dec_wr_quad (tr_quint, (term_idx - 1), out,
                                             filter_state);

              /* Now put remaining parts of the term. seq. at beg. of quint */
              k = 0;
              for (i = term_idx; i < q_idx; i++)
                {
                  quint[k++] = quint[i];
                }
              q_idx = k;

            }
          else
            {
              /* 0 == term_idx - only term sequence - no quad output */
            }

          /* Now check to see if we have valid finishing conditions: */
          if ((PDF_ENINPUT == retval) && (2 == q_idx)
              && ('~' == quint[0]) && ('>' == quint[1]))
            {
              if (finish)
                {
                  /* All we have is term code, and finish is requested. fini */
                  retval = PDF_EEOF;
                  oldone = PDF_TRUE;
                }
              else
                { /* finish not set. save the bytes for later */
                  for (i = 0; i < q_idx; i++)
                    {
                      filter_state->spare_bytes[i] = quint[i];
                    }
                  filter_state->spare_count = q_idx;
                  oldone = PDF_TRUE;
                }
            }
        }

      if (!oldone && PDF_OK == retval)
        {
          retval = stm_f_a85_flush_outbuff (out, filter_state);

          if (PDF_OK != retval)
            {
              oldone = PDF_TRUE;
            }
        }

    }
  return retval;
}

/* NOTE:
 * This method is just a helper method until the A85 filter is fully ported to
 * the new API */
static enum pdf_stm_filter_apply_status_e
stm_f_a85enc_apply (void          *state,
                    pdf_buffer_t  *in,
                    pdf_buffer_t  *out,
                    pdf_bool_t     finish,
                    pdf_error_t  **error)
{
  pdf_status_t ret;

  ret = a85enc_apply (state,
                      in,
                      out,
                      finish);
  switch (ret)
    {
    case PDF_OK:
    case PDF_ENINPUT:
      return PDF_STM_FILTER_APPLY_STATUS_NO_INPUT;
    case PDF_ENOUTPUT:
      return PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT;
    case PDF_EEOF:
      return PDF_STM_FILTER_APPLY_STATUS_EOF;
    default:
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     ret,
                     "Generic error applying ASCII-85 encoder");
      return PDF_STM_FILTER_APPLY_STATUS_ERROR;
    }
}

/* NOTE:
 * This method is just a helper method until the A85 filter is fully ported to
 * the new API */
static enum pdf_stm_filter_apply_status_e
stm_f_a85dec_apply (void          *state,
                    pdf_buffer_t  *in,
                    pdf_buffer_t  *out,
                    pdf_bool_t     finish,
                    pdf_error_t  **error)
{
  pdf_status_t ret;

  ret = a85dec_apply (state,
                      in,
                      out,
                      finish);
  switch (ret)
    {
    case PDF_OK:
    case PDF_ENINPUT:
      return PDF_STM_FILTER_APPLY_STATUS_NO_INPUT;
    case PDF_ENOUTPUT:
      return PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT;
    case PDF_EEOF:
      return PDF_STM_FILTER_APPLY_STATUS_EOF;
    default:
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     ret,
                     "Generic error applying ASCII-85 decoder");
      return PDF_STM_FILTER_APPLY_STATUS_ERROR;
    }
}

/* End of pdf_stm_f_a85.c */

/* -*- mode: C -*-
 *
 *       File:         pdf-stm-f-rl.c
 *       Date:         Sun Jul 15 22:01:18 2007
 *
 *       GNU PDF Library - RunLength encoder/decoder
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

#include <string.h>
#include <stdio.h>

#include <pdf-alloc.h>
#include <pdf-stm-f-rl.h>
#include <pdf-types.h>
#include <pdf-types-buffer.h>
#include <pdf-hash.h>

typedef enum
{
  PDF_STM_F_RL_WRL,
  PDF_STM_F_RL_NONE
} pdf_stm_f_rl_enc_e;

/* Internal state */
struct pdf_stm_f_rl_s
{
  pdf_uchar_t curchar;
  pdf_uchar_t rlchar;
  pdf_uchar_t dec_count;
  pdf_i32_t rl;
  pdf_bool_t run_p;
  pdf_bool_t dec_p;
  pdf_stm_f_rl_enc_e enc_p;
};

static pdf_bool_t encode_rl_char (struct pdf_stm_f_rl_s *st,
                                  pdf_buffer_t          *out);

static pdf_bool_t decode_rl_char (struct pdf_stm_f_rl_s *st,
                                  pdf_buffer_t          *out);

static int copy_next_bytes (struct pdf_stm_f_rl_s *st,
                            pdf_buffer_t          *in,
                            pdf_buffer_t          *out);

static pdf_bool_t stm_f_rl_init (const pdf_hash_t  *params,
                                 void             **state,
                                 pdf_error_t      **error);

static void stm_f_rl_deinit (void *state);

static enum pdf_stm_filter_apply_status_e stm_f_rlenc_apply (void          *state,
                                                             pdf_buffer_t  *in,
                                                             pdf_buffer_t  *out,
                                                             pdf_bool_t     finish,
                                                             pdf_error_t  **error);

static enum pdf_stm_filter_apply_status_e stm_f_rldec_apply (void          *state,
                                                             pdf_buffer_t  *in,
                                                             pdf_buffer_t  *out,
                                                             pdf_bool_t     finish,
                                                             pdf_error_t  **error);
/* Filter implementations */

static const pdf_stm_filter_impl_t enc_impl = {
  .init_fn   = stm_f_rl_init,
  .apply_fn  = stm_f_rlenc_apply,
  .deinit_fn = stm_f_rl_deinit,
};

static const pdf_stm_filter_impl_t dec_impl = {
  .init_fn   = stm_f_rl_init,
  .apply_fn  = stm_f_rldec_apply,
  .deinit_fn = stm_f_rl_deinit,
};

const pdf_stm_filter_impl_t *
pdf_stm_f_rlenc_get (void)
{
  return &enc_impl;
}

const pdf_stm_filter_impl_t *
pdf_stm_f_rldec_get (void)
{
  return &dec_impl;
}

/* Common implementation */

static pdf_bool_t
stm_f_rl_init (const pdf_hash_t  *params,
               void             **state,
               pdf_error_t      **error)
{
  struct pdf_stm_f_rl_s *filter_state;

  filter_state = pdf_alloc (sizeof (struct pdf_stm_f_rl_s));
  if (!filter_state)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_STM,
                     PDF_ENOMEM,
                     "cannot create RunLength encoder/decoder internal state: "
                     "couldn't allocate %lu bytes",
                     (unsigned long)sizeof (struct pdf_stm_f_rl_s));
      return PDF_FALSE;
    }

  /* Initialize fields */
  filter_state->curchar = 0;
  filter_state->rlchar = 0;
  filter_state->run_p = PDF_FALSE;
  filter_state->rl = -1;
  filter_state->dec_p = PDF_FALSE;;
  filter_state->dec_count = 0;
  filter_state->enc_p = PDF_STM_F_RL_NONE;

  *state = (void *) filter_state;

  return PDF_TRUE;
}

static void
stm_f_rl_deinit (void *state)
{
  pdf_dealloc (state);
}

/* Encoder implementation */

static enum pdf_stm_filter_apply_status_e
stm_f_rlenc_apply (void          *state,
                   pdf_buffer_t  *in,
                   pdf_buffer_t  *out,
                   pdf_bool_t     finish,
                   pdf_error_t  **error)
{
  struct pdf_stm_f_rl_s *filter_state;

  filter_state = (struct pdf_stm_f_rl_s *) state;

  while (!pdf_buffer_eob_p (in))
    {
      filter_state->curchar = in->data[in->rp];

      /* we're not encoding any character yet */
      if (!filter_state->run_p)
        {
          filter_state->rlchar = filter_state->curchar;
          filter_state->run_p = PDF_TRUE;
          filter_state->rl++;
          in->rp++;
        }
      /* we're encoding some character now */
      else if (filter_state->curchar == filter_state->rlchar &&
               filter_state->rl < 127)
        {
          filter_state->rl++;
          in->rp++;
        }
      /*
       * the rl code is too long or the rl char is different,
       * so we write what we encoded so far.
       */
      else
        {
          if (!encode_rl_char (filter_state, out))
            return PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT;

          filter_state->rl = -1;
          filter_state->run_p = PDF_FALSE;
        }
    }

  /*
   * we may have finished with some history, we save it if needed,
   * then we add the EOD.
   */
  if (finish)
    {
      if (filter_state->run_p)
        {
          if (!encode_rl_char (filter_state, out))
            return PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT;

          filter_state->rl = -1;
          filter_state->run_p = PDF_FALSE;
        }

      /* Insert EOD marker */
      if (pdf_buffer_full_p (out))
        return PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT;
      out->data[out->wp++] = 128;
      return PDF_STM_FILTER_APPLY_STATUS_EOF;
    }

  return PDF_STM_FILTER_APPLY_STATUS_NO_INPUT;
}

static enum pdf_stm_filter_apply_status_e
stm_f_rldec_apply (void          *state,
                   pdf_buffer_t  *in,
                   pdf_buffer_t  *out,
                   pdf_bool_t     finish,
                   pdf_error_t  **error)
{
  struct pdf_stm_f_rl_s *filter_state;

  filter_state = (struct pdf_stm_f_rl_s *) state;

  while (!pdf_buffer_eob_p (in))
    {
      filter_state->curchar = in->data[in->rp];

      /* we're not decoding any character yet */
      if (!filter_state->run_p)
        {
          filter_state->rlchar = filter_state->curchar;
          filter_state->run_p = PDF_TRUE;
          in->rp++;
        }
      /* copy the following 1 to 128 bytes literally */
      else if (filter_state->rlchar < 128)
        {
          int copied;

          copied = copy_next_bytes (filter_state, in, out);
          if (copied < 0)
            return PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT;
          if (copied > 0)
            return PDF_STM_FILTER_APPLY_STATUS_NO_INPUT;
          filter_state->run_p = PDF_FALSE;
        }
      /* copy the next char 257 - length (2 to 128) times */
      else if (filter_state->rlchar > 128)
        {
          if (!decode_rl_char (filter_state, out))
            return PDF_STM_FILTER_APPLY_STATUS_NO_OUTPUT;
          filter_state->run_p = PDF_FALSE;
          in->rp++;
        }
      /* EOD mark */
      else
        {
          filter_state->run_p = PDF_FALSE;
          return PDF_STM_FILTER_APPLY_STATUS_EOF;
        }
    }

  return PDF_STM_FILTER_APPLY_STATUS_NO_INPUT;
}

/* Helper functions */

static pdf_bool_t
encode_rl_char (struct pdf_stm_f_rl_s *st,
                pdf_buffer_t          *out)
{
  if (pdf_buffer_full_p (out))
    return PDF_FALSE;

  switch (st->enc_p)
    {
    case PDF_STM_F_RL_NONE:
      out->data[out->wp++] = (st->rl == 0) ? 0 : 256 - st->rl;
      st->enc_p = PDF_STM_F_RL_WRL;
      /* Continue to RL_WRL! */
    case PDF_STM_F_RL_WRL:
      out->data[out->wp++] = st->rlchar;
      st->enc_p = PDF_STM_F_RL_NONE;
      break;
    }

  return PDF_TRUE;
}

static pdf_bool_t
decode_rl_char (struct pdf_stm_f_rl_s *st,
                pdf_buffer_t          *out)
{
  if (!st->dec_p)
    {
      st->dec_count = 257 - st->rlchar;
      st->dec_p = PDF_TRUE;
    }

  while (st->dec_count > 0)
    {
      if (pdf_buffer_full_p (out))
        return PDF_FALSE;

      out->data[out->wp++] = st->curchar;
      st->dec_count--;
    }

  st->dec_p = PDF_FALSE;
  return PDF_TRUE;
}

static int
copy_next_bytes (struct pdf_stm_f_rl_s *st,
                 pdf_buffer_t          *in,
                 pdf_buffer_t          *out)
{
  if (!st->dec_p)
    {
      if (pdf_buffer_full_p (out))
        return -1;

      st->dec_count = st->rlchar + 1;
      st->dec_p = PDF_TRUE;
      out->data[out->wp++] = st->curchar;
      in->rp++;
      st->dec_count--;
    }

  while (st->dec_count > 0)
    {
      if (pdf_buffer_eob_p (in))
        return 1;

      if (pdf_buffer_full_p (out))
        return -1;

      out->data[out->wp] = in->data[in->rp];
      out->wp++;
      in->rp++;
      st->dec_count--;
    }

  st->dec_p = PDF_FALSE;
  return 0;
}

/* End of pdf_stm_f_rl.c */

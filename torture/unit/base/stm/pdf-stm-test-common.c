/* -*- mode: C -*-
 *
 *       File:         pdf-stm-test-common.c
 *       Date:         Mon May 02 15:41:04 2011
 *
 *       GNU PDF Library - Common utilities for the STM tests
 *
 */

/* Copyright (C) 2011 Free Software Foundation, Inc. */

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
#include <check.h>
#include <pdf.h>
#include <pdf-test-common.h>
#include "pdf-stm-test-common.h"

static pdf_size_t
get_loop_real_size_in_bytes (enum loop_rw_size_e lrws,
                             pdf_size_t          size)
{
  switch (lrws)
    {
    case LOOP_RW_SIZE_ONE:
      return 1;
    case LOOP_RW_SIZE_TWO:
      return 2;
    case LOOP_RW_SIZE_HALF:
      return (size / 2);
    case LOOP_RW_SIZE_EXACT:
      return size;
    case LOOP_RW_SIZE_DOUBLE:
      return (size * 2);
    }

  /* Never reached, make compiler happy */
  fprintf (stderr, "Got invalid lrs '%u'\n", lrws);
  fail ();
  return 0;
}

/*
 * Common method to test reading from streams with a single filter (or none)
 * installed. Specific loop_read_size given.
 */
static void
common_read (const pdf_char_t           *test_name,
             enum pdf_stm_filter_type_e  filter_type,
             const pdf_hash_t           *filter_params,
             pdf_size_t                  stm_cache_size,
             enum loop_rw_size_e         loop_read_size_e,
             const pdf_char_t           *input,
             pdf_size_t                  input_size,
             const pdf_char_t           *expected,
             pdf_size_t                  expected_size)
{
  pdf_error_t *error = NULL;
  pdf_stm_t *stm;
  pdf_char_t *output_buffer;
  pdf_size_t output_buffer_size;
  pdf_size_t total_read_bytes;
  pdf_size_t loop_read_size;

  /* Get real loop read size to be used */
  loop_read_size = get_loop_real_size_in_bytes (loop_read_size_e,
                                                input_size);
  /* If we are not going to read anything, nothing to test, return */
  if (loop_read_size == 0)
    return;

  /* Create the return buffer, with enough size for the real
   * output plus an extra loop read size */
  output_buffer_size = expected_size + loop_read_size;
  output_buffer = pdf_alloc (output_buffer_size);
  fail_unless (output_buffer != NULL);

  /* Create the stream for the INPUT */
  stm  = pdf_stm_mem_new ((pdf_uchar_t *)input,
                          input_size,
                          stm_cache_size,
                          PDF_STM_READ,
                          &error);
  fail_unless (stm != NULL);
  fail_if (error != NULL);

  /* If any known filter required, set it up */
  if (filter_type != PDF_STM_FILTER_UNKNOWN)
    {
      fail_unless (pdf_stm_install_filter (stm,
                                           filter_type,
                                           filter_params,
                                           &error) == PDF_TRUE);
      fail_if (error != NULL);
    }

  /* Read data from the stream */
  total_read_bytes = 0;
  while (PDF_TRUE)
    {
      pdf_size_t read_bytes;

      /* Make sure we don't overwrite any memory in the output buffer */
      fail_if (total_read_bytes + loop_read_size > output_buffer_size);

      read_bytes = 0;
      if (!pdf_stm_read (stm,
                         &output_buffer[total_read_bytes],
                         loop_read_size,
                         &read_bytes,
                         &error) &&
          error)
        {
          /* If expected string is NULL, then we will be expecting an error
           * during decoding */
          if (expected)
            {
              fprintf (stderr,
                       "%s: error reading from stream: %s\n",
                       test_name,
                       pdf_error_get_message (error));
              fail ();
            }
          else
            {
              break;
            }
        }

      /* Update total read bytes */
      total_read_bytes += read_bytes;

      if (expected)
        {
          fail_if (read_bytes > loop_read_size);
          fail_if (error != NULL);

          /* EOF? */
          if (read_bytes < loop_read_size)
            break;
        }
    }

#ifdef STM_MODULE_ADDITIONAL_TEST_TRACES
    {
      pdf_char_t *input_hex;
      pdf_char_t *output_hex;

      input_hex = pdf_test_get_hex (input, input_size, ':');
      output_hex = pdf_test_get_hex (output_buffer, total_read_bytes, ':');

      if (expected)
        {
          pdf_char_t *expected_hex;

          expected_hex = pdf_test_get_hex (expected, expected_size, ':');
          printf ("%s: %s\n"
                  "\tLoop read size: '%lu' bytes\n"
                  "\tInp. '%lu' bytes: '%s'\n"
                  "\tRead '%lu' bytes: '%s'\n"
                  "\tExp. '%lu' bytes: '%s'\n",
                  test_name,
                  (strcmp (output_hex, expected_hex) != 0 ? "----> FAILED" : ""),
                  (unsigned long)loop_read_size,
                  (unsigned long)input_size,
                  input_hex,
                  (unsigned long)total_read_bytes,
                  output_hex,
                  (unsigned long)expected_size,
                  expected_hex);
          pdf_dealloc (expected_hex);
        }
      else
        {
          printf ("%s: %s\n"
                  "\tLoop read size: '%lu' bytes\n"
                  "\tInp. '%lu' bytes: '%s'\n"
                  "\tRead '%lu' bytes: '%s'\n",
                  test_name,
                  (error == NULL ? "----> FAILED" : ""),
                  (unsigned long)loop_read_size,
                  (unsigned long)input_size,
                  input_hex,
                  (unsigned long)total_read_bytes,
                  output_hex);
        }

      pdf_dealloc (input_hex);
      pdf_dealloc (output_hex);
    }
#endif /* STM_MODULE_ADDITIONAL_TEST_TRACES */

  if (expected)
    {
      /* Finished reading, now compare expected output with the one we just got */
      fail_unless (total_read_bytes == expected_size);
      fail_unless (memcmp (output_buffer, expected, expected_size) == 0);
    }
  else
    {
      fail_unless (error != NULL);
      pdf_error_destroy (error);
    }

  /* Cleanup */
  pdf_dealloc (output_buffer);
  pdf_stm_destroy (stm);
}

/*
 * Common method to test writing to streams with a single filter (or none)
 * installed. Specific loop_read_size given.
 */
static void
common_write (const pdf_char_t           *test_name,
              enum pdf_stm_filter_type_e  filter_type,
              const pdf_hash_t           *filter_params,
              pdf_size_t                  stm_cache_size,
              enum loop_rw_size_e         loop_write_size_e,
              const pdf_char_t           *input,
              pdf_size_t                  input_size,
              const pdf_char_t           *expected,
              pdf_size_t                  expected_size)
{
  pdf_error_t *error = NULL;
  pdf_stm_t *stm;
  pdf_uchar_t *output_buffer;
  pdf_size_t output_buffer_size;
  pdf_size_t total_written_bytes;
  pdf_size_t loop_write_size;
  pdf_bool_t eof;

  /* Get real loop read size to be used */
  loop_write_size = get_loop_real_size_in_bytes (loop_write_size_e,
                                                 input_size);
  /* If we are not going to write anything, nothing to test, return */
  if (loop_write_size == 0)
    return;

  /* Create the output buffer, with enough size for the real
   * output plus an extra loop write size */
  output_buffer_size = expected_size + loop_write_size;
  output_buffer = pdf_alloc (output_buffer_size);
  fail_unless (output_buffer != NULL);

  /* Create the stream for the OUTPUT */
  stm  = pdf_stm_mem_new (output_buffer,
                          output_buffer_size,
                          stm_cache_size,
                          PDF_STM_WRITE,
                          &error);
  fail_unless (stm != NULL);
  fail_if (error != NULL);

  /* If any known filter required, set it up */
  if (filter_type != PDF_STM_FILTER_UNKNOWN)
    {
      fail_unless (pdf_stm_install_filter (stm,
                                           filter_type,
                                           filter_params,
                                           &error) == PDF_TRUE);
      fail_if (error != NULL);
    }

  /* Write data to the stream */
  total_written_bytes = 0;
  while (PDF_TRUE)
    {
      pdf_size_t written_bytes;
      pdf_size_t to_write;

      written_bytes = 0;
      to_write = (input_size - total_written_bytes >= loop_write_size ?
                  loop_write_size :
                  (input_size - total_written_bytes));

      /* EOF? */
      if (to_write == 0)
        break;

      if (!pdf_stm_write (stm,
                          &input[total_written_bytes],
                          to_write,
                          &written_bytes,
                          &error) &&
          error)
        {
          /* If expected string is NULL, then we will be expecting an error
           * during decoding */
          if (expected)
            {
              fprintf (stderr,
                       "%s: error writing to stream: %s\n",
                       test_name,
                       pdf_error_get_message (error));
              fail ();
            }
          else
            {
              break;
            }
        }

      /* Update total written bytes */
      total_written_bytes += written_bytes;

      if (expected)
        {
          fail_if (written_bytes > loop_write_size);
          fail_if (error != NULL);
        }
    }

  /* NOTE: expected errors should also be handled during final flush */
  pdf_clear_error (&error);
  if (!pdf_stm_flush (stm, PDF_TRUE, NULL, &error) &&
      error)
    {
      /* If expected string is NULL, then we will be expecting an error
       * during decoding */
      if (expected)
        {
          fprintf (stderr,
                   "%s: error flushing to stream: %s\n",
                   test_name,
                   pdf_error_get_message (error));
          fail ();
        }
    }

#ifdef STM_MODULE_ADDITIONAL_TEST_TRACES
    {
      pdf_char_t *input_hex;
      pdf_char_t *output_hex;

      input_hex = pdf_test_get_hex (input, input_size, ':');
      output_hex = pdf_test_get_hex (output_buffer, expected_size, ':');

      if (expected)
        {
          pdf_char_t *expected_hex;

          expected_hex = pdf_test_get_hex (expected, expected_size, ':');

          printf ("%s: %s\n"
                  "\tLoop write size: '%lu' bytes\n"
                  "\tInp. '%lu' bytes: '%s'\n"
                  "\tWri. '%lu' bytes\n"
                  "\tOut. '%s'\n"
                  "\tExp. '%lu' bytes: '%s'\n",
                  test_name,
                  (strcmp (output_hex, expected_hex) != 0 ? "----> FAILED" : ""),
                  (unsigned long)loop_write_size,
                  (unsigned long)input_size,
                  input_hex,
                  (unsigned long)total_written_bytes,
                  output_hex,
                  (unsigned long)expected_size,
                  expected_hex);

          pdf_dealloc (expected_hex);
        }
      else
        {
          printf ("%s: %s\n"
                  "\tLoop write size: '%lu' bytes\n"
                  "\tInp. '%lu' bytes: '%s'\n",
                  test_name,
                  (error == NULL ? "----> FAILED" : ""),
                  (unsigned long)loop_write_size,
                  (unsigned long)input_size,
                  input_hex);
        }

      pdf_dealloc (input_hex);
      pdf_dealloc (output_hex);
    }
#endif /* STM_MODULE_ADDITIONAL_TEST_TRACES */

  if (expected)
    {
      /* Finished reading, now compare expected output with the one we just got */
      fail_unless (memcmp (output_buffer, expected, expected_size) == 0);
    }
  else
    {
      fail_unless (error != NULL);
      pdf_error_destroy (error);
    }

  /* Cleanup */
  pdf_dealloc (output_buffer);
  pdf_stm_destroy (stm);
}

void
pdf_stm_test_common (const pdf_char_t           *test_name,
                     enum test_type_e            test_type,
                     enum test_op_e              test_operation,
                     enum pdf_stm_filter_type_e  filter_type,
                     const pdf_hash_t           *filter_params,
                     pdf_size_t                  stm_cache_size,
                     enum loop_rw_size_e         loop_rw_size,
                     const pdf_char_t           *decoded,
                     pdf_size_t                  decoded_size,
                     const pdf_char_t           *encoded,
                     pdf_size_t                  encoded_size)
{
  const pdf_char_t *input;
  pdf_size_t input_size;
  const pdf_char_t *expected;
  pdf_size_t expected_size;
  void (* common_operation) (const pdf_char_t *,
                             enum pdf_stm_filter_type_e,
                             const pdf_hash_t *,
                             pdf_size_t,
                             enum loop_rw_size_e,
                             const pdf_char_t *,
                             pdf_size_t,
                             const pdf_char_t *,
                             pdf_size_t);

  if (test_operation == TEST_OP_READ)
    {
      common_operation = common_read;
    }
  else if (test_operation == TEST_OP_WRITE)
    {
      common_operation = common_write;
    }
  else
    fail ();

  if (test_type == TEST_TYPE_DECODER)
    {
      input = encoded;
      input_size = encoded_size;
      expected = decoded;
      expected_size = decoded_size;
    }
  else if (test_type == TEST_TYPE_ENCODER)
    {
      input = decoded;
      input_size = decoded_size;
      expected = encoded;
      expected_size = encoded_size;
    }
  else
    fail ();

  common_operation (test_name,
                    filter_type,
                    filter_params,
                    stm_cache_size,
                    loop_rw_size,
                    input,
                    input_size,
                    expected,
                    expected_size);
}

/* End of pdf-stm-test-common.c */

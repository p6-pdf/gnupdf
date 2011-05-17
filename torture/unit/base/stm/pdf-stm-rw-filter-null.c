/* -*- mode: C -*-
 *
 *       File:         pdf-stm-rw-filter-null.c
 *       Date:         Mon May 02 16:35:12 2011
 *
 *       GNU PDF Library - Unit tests for pdf_stm_read() and pdf_stm_write with
 *                         NO explicit filter or with an additional NULL filter.
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

static const pdf_char_t *test_strings[] = {
  "a",
  "ab",
  "abc",
  "0123456789",
  "GNU's Not Unix",
  NULL,
};

/* Note: The NULL filter is fully simmetric, this is, input and output are
 * always the same, so test type can be considered always as decoder. */
static const struct test_params_s tests_params[] = {
  /* No   Test type          Test operation   Loop read size       Cache size */
  {	 1,   TEST_TYPE_DECODER, TEST_OP_READ,    LOOP_RW_SIZE_ONE,    0 },
  {	 2,   TEST_TYPE_DECODER, TEST_OP_READ,    LOOP_RW_SIZE_TWO,    0 },
  {	 3,   TEST_TYPE_DECODER, TEST_OP_READ,    LOOP_RW_SIZE_HALF,   0 },
  {	 4,   TEST_TYPE_DECODER, TEST_OP_READ,    LOOP_RW_SIZE_EXACT,  0 },
  {	 5,   TEST_TYPE_DECODER, TEST_OP_READ,    LOOP_RW_SIZE_DOUBLE, 0 },

  {	 6,   TEST_TYPE_DECODER, TEST_OP_WRITE,   LOOP_RW_SIZE_ONE,    0 },
  {	 7,   TEST_TYPE_DECODER, TEST_OP_WRITE,   LOOP_RW_SIZE_TWO,    0 },
  {  8,   TEST_TYPE_DECODER, TEST_OP_WRITE,   LOOP_RW_SIZE_HALF,   0 },
  {	 9,   TEST_TYPE_DECODER, TEST_OP_WRITE,   LOOP_RW_SIZE_EXACT,  0 },
  {	 10,  TEST_TYPE_DECODER, TEST_OP_WRITE,   LOOP_RW_SIZE_DOUBLE, 0 },
};

static void
common_test_none (const pdf_char_t *function_name,
                  int               test_index)
{
  int i;
  const struct test_params_s *params = &tests_params[test_index - 1];

  /* Sanity check */
  fail_if (test_index != params->idx);

  for (i = 0; test_strings[i]; i++)
    {
      /* Passing UNKNOWN as filter avoids installing any additional filter */
      pdf_stm_test_common (function_name,
                           params->type,
                           params->operation,
                           PDF_STM_FILTER_UNKNOWN,
                           NULL,
                           params->stm_cache_size,
                           params->loop_size,
                           test_strings[i],
                           strlen (test_strings[i]),
                           test_strings[i],
                           strlen (test_strings[i]));
    }
}

/*
 * Test: pdf_stm_read_001-005
 * Description:
 *   Read some bytes from a reading memory buffer. Different tests are done
 *   for different buffer sizes.
 * Success condition:
 *   The read data should be ok.
 */
START_TEST (pdf_stm_read_filter_none_001) { common_test_none (__FUNCTION__,  1); } END_TEST
START_TEST (pdf_stm_read_filter_none_002) { common_test_none (__FUNCTION__,  2); } END_TEST
START_TEST (pdf_stm_read_filter_none_003) { common_test_none (__FUNCTION__,  3); } END_TEST
START_TEST (pdf_stm_read_filter_none_004) { common_test_none (__FUNCTION__,  4); } END_TEST
START_TEST (pdf_stm_read_filter_none_005) { common_test_none (__FUNCTION__,  5); } END_TEST

/*
 * Test: pdf_stm_write_001-005
 * Description:
 *   Write some bytes to a writing memory buffer. Different tests are done
 *   for different buffer sizes.
 * Success condition:
 *   The written data should be ok.
 */
START_TEST (pdf_stm_write_filter_none_001) { common_test_none (__FUNCTION__,  6); } END_TEST
START_TEST (pdf_stm_write_filter_none_002) { common_test_none (__FUNCTION__,  7); } END_TEST
START_TEST (pdf_stm_write_filter_none_003) { common_test_none (__FUNCTION__,  8); } END_TEST
START_TEST (pdf_stm_write_filter_none_004) { common_test_none (__FUNCTION__,  9); } END_TEST
START_TEST (pdf_stm_write_filter_none_005) { common_test_none (__FUNCTION__, 10); } END_TEST

static void
common_test_null (const pdf_char_t *function_name,
                  int               test_index)
{
  int i;
  const struct test_params_s *params = &tests_params[test_index - 1];

  /* Sanity check */
  fail_if (test_index != params->idx);

  for (i = 0; test_strings[i]; i++)
    {
      /* Passing UNKNOWN as filter avoids installing any additional filter */
      pdf_stm_test_common (function_name,
                           params->type,
                           params->operation,
                           PDF_STM_FILTER_NULL,
                           NULL,
                           params->stm_cache_size,
                           params->loop_size,
                           test_strings[i],
                           strlen (test_strings[i]),
                           test_strings[i],
                           strlen (test_strings[i]));
    }
}

/*
 * Test: pdf_stm_read_filter_null_001-005
 * Description:
 *   Read some bytes from a reading memory buffer, with an additional NULL
 *   filter installed. Different tests are done for different buffer sizes.
 * Success condition:
 *   The read data should be ok.
 */
START_TEST (pdf_stm_read_filter_null_001) { common_test_null (__FUNCTION__,  1); } END_TEST
START_TEST (pdf_stm_read_filter_null_002) { common_test_null (__FUNCTION__,  2); } END_TEST
START_TEST (pdf_stm_read_filter_null_003) { common_test_null (__FUNCTION__,  3); } END_TEST
START_TEST (pdf_stm_read_filter_null_004) { common_test_null (__FUNCTION__,  4); } END_TEST
START_TEST (pdf_stm_read_filter_null_005) { common_test_null (__FUNCTION__,  5); } END_TEST

/*
 * Test: pdf_stm_write_filter_null_001-005
 * Description:
 *   Write some bytes to a writing memory buffer, with an additional NULL
 *   filter installed. Different tests are done for different buffer sizes.
 * Success condition:
 *   The written data should be ok.
 */
START_TEST (pdf_stm_write_filter_null_001) { common_test_null (__FUNCTION__,  6); } END_TEST
START_TEST (pdf_stm_write_filter_null_002) { common_test_null (__FUNCTION__,  7); } END_TEST
START_TEST (pdf_stm_write_filter_null_003) { common_test_null (__FUNCTION__,  8); } END_TEST
START_TEST (pdf_stm_write_filter_null_004) { common_test_null (__FUNCTION__,  9); } END_TEST
START_TEST (pdf_stm_write_filter_null_005) { common_test_null (__FUNCTION__, 10); } END_TEST

static void
common_file_stream_read_test (pdf_bool_t install_null_filter)
{
  pdf_error_t *error = NULL;
  const pdf_char_t *filename = "tmp.test";
  const pdf_char_t *file_contents = "GNU";
  pdf_stm_t *stm;
  pdf_size_t written_bytes;
  pdf_size_t read_bytes;
  pdf_fsys_file_t file;
  pdf_char_t data[4] = { 0 };
  pdf_text_t *path;

  /* Create the file path */
  path = pdf_text_new_from_unicode (filename, strlen (filename),
                                    PDF_TEXT_UTF8,
                                    &error);
  fail_unless (path != NULL);
  fail_if (error != NULL);

  /* Open new file */
  fail_unless (pdf_fsys_file_open (NULL,
                                   path,
                                   PDF_FSYS_OPEN_MODE_WRITE,
                                   &file) == PDF_OK);

  written_bytes = 0;
  fail_unless (pdf_fsys_file_write (file,
                                    file_contents,
                                    strlen (file_contents),
                                    &written_bytes) == PDF_OK);
  fail_if (written_bytes != strlen (file_contents));
  pdf_fsys_file_close (file);

  fail_unless (pdf_fsys_file_open (NULL,
                                   path,
                                   PDF_FSYS_OPEN_MODE_READ,
                                   &file) == PDF_OK);

  /* Create the stream */
  stm = pdf_stm_file_new (file,
                          0,
                          0, /* Use the default cache size */
                          PDF_STM_READ,
                          &error);
  fail_unless (stm != NULL);
  fail_if (error != NULL);

  fail_unless (pdf_stm_read (stm,
                             data,
                             3,
                             &read_bytes,
                             &error) == PDF_TRUE);
  fail_unless (read_bytes == 3);
  fail_unless (strncmp (data, "GNU", 3) == 0);

  /* Free resources */
  pdf_stm_destroy (stm);
  pdf_fsys_file_close (file);
  pdf_text_destroy (path);
}

/*
 * Test: pdf_stm_read_filter_none_006
 * Description:
 *   Read some bytes from a read file stream.
 * Success condition:
 *   The read data should be consistent.
 */
START_TEST (pdf_stm_read_filter_none_006)
{
  common_file_stream_read_test (PDF_FALSE);
}
END_TEST

/*
 * Test: pdf_stm_read_filter_null_006
 * Description:
 *   Read some bytes from a read file stream with null filter.
 * Success condition:
 *   The read data should be consistent.
 */
START_TEST (pdf_stm_read_filter_null_006)
{
  common_file_stream_read_test (PDF_TRUE);
}
END_TEST

static void
common_file_stream_write_test (pdf_bool_t install_null_filter)
{
  pdf_error_t *error = NULL;
  const pdf_char_t *filename = "tmp.test";
  const pdf_char_t *file_contents = "GNU";
  pdf_stm_t *stm;
  pdf_size_t written_bytes;
  pdf_size_t read_bytes;
  pdf_fsys_file_t file;
  pdf_char_t data[4] = { 0 };
  pdf_text_t *path;

  /* Create the file path */
  path = pdf_text_new_from_unicode (filename, strlen (filename),
                                    PDF_TEXT_UTF8,
                                    &error);
  fail_unless (path != NULL);
  fail_if (error != NULL);

  /* Open new file for writing */
  fail_unless (pdf_fsys_file_open (NULL,
                                   path,
                                   PDF_FSYS_OPEN_MODE_WRITE,
                                   &file) == PDF_OK);

  /* Create the stream */
  stm = pdf_stm_file_new (file,
                          0,
                          0, /* Use the default cache size */
                          PDF_STM_WRITE,
                          &error);
  fail_unless (stm != NULL);
  fail_if (error != NULL);

  /* Write some data into the stream */
  fail_unless (pdf_stm_write (stm,
                              file_contents,
                              strlen (file_contents),
                              &written_bytes,
                              &error) == PDF_TRUE);
  fail_unless (written_bytes == 3);
  fail_if (error != NULL);

  /* Destroy data */
  pdf_stm_destroy (stm);
  pdf_fsys_file_close (file);

  /* Read file data and compare */
  fail_unless (pdf_fsys_file_open (NULL,
                                   path,
                                   PDF_FSYS_OPEN_MODE_READ,
                                   &file) == PDF_OK);
  fail_unless (pdf_fsys_file_read (file,
                                   data,
                                   3,
                                   &read_bytes) == PDF_OK);
  fail_unless (read_bytes == 3);
  fail_unless (strcmp (data, file_contents) == 0);

  /* Free resources */
  pdf_fsys_file_close (file);
  pdf_text_destroy (path);
}

/*
 * Test: pdf_stm_write_filter_none_006
 * Description:
 *   Write some bytes from a write file stream.
 * Success condition:
 *   The write data should be consistent.
 */
START_TEST (pdf_stm_write_filter_none_006)
{
  common_file_stream_write_test (PDF_FALSE);
}
END_TEST

/*
 * Test: pdf_stm_write_filter_null_006
 * Description:
 *   Write some bytes from a write file stream with null filter.
 * Success condition:
 *   The write data should be consistent.
 */
START_TEST (pdf_stm_write_filter_null_006)
{
  common_file_stream_write_test (PDF_TRUE);
}
END_TEST

/*
 * Test case creation functions
 */

TCase *
test_pdf_stm_rw_filter_none (void)
{
  TCase *tc = tcase_create ("pdf_stm_rw_filter_none");

  tcase_add_test (tc, pdf_stm_read_filter_none_001);
  tcase_add_test (tc, pdf_stm_read_filter_none_002);
  tcase_add_test (tc, pdf_stm_read_filter_none_003);
  tcase_add_test (tc, pdf_stm_read_filter_none_004);
  tcase_add_test (tc, pdf_stm_read_filter_none_005);
  tcase_add_test (tc, pdf_stm_read_filter_none_006);

  tcase_add_test (tc, pdf_stm_write_filter_none_001);
  tcase_add_test (tc, pdf_stm_write_filter_none_002);
  tcase_add_test (tc, pdf_stm_write_filter_none_003);
  tcase_add_test (tc, pdf_stm_write_filter_none_004);
  tcase_add_test (tc, pdf_stm_write_filter_none_005);
  tcase_add_test (tc, pdf_stm_write_filter_none_006);

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/*
 * Test case creation function
 */
TCase *
test_pdf_stm_rw_filter_null (void)
{
  TCase *tc = tcase_create ("pdf_stm_rw_filter_null");

  tcase_add_test (tc, pdf_stm_read_filter_null_001);
  tcase_add_test (tc, pdf_stm_read_filter_null_002);
  tcase_add_test (tc, pdf_stm_read_filter_null_003);
  tcase_add_test (tc, pdf_stm_read_filter_null_004);
  tcase_add_test (tc, pdf_stm_read_filter_null_005);
  tcase_add_test (tc, pdf_stm_read_filter_null_006);

  tcase_add_test (tc, pdf_stm_write_filter_null_001);
  tcase_add_test (tc, pdf_stm_write_filter_null_002);
  tcase_add_test (tc, pdf_stm_write_filter_null_003);
  tcase_add_test (tc, pdf_stm_write_filter_null_004);
  tcase_add_test (tc, pdf_stm_write_filter_null_005);
  tcase_add_test (tc, pdf_stm_write_filter_null_006);

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-stm-read-filter-null.c */

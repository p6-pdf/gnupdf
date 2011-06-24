/* -*- mode: C -*-
 *
 *       File:         pdf-stm-rw-filter-aesv2.c
 *       Date:         Thu May 05 23:43:22 2011
 *
 *       GNU PDF Library - Unit tests for pdf_stm_read() and pdf_stm_write()
 *                         with AESV2 filter.
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

struct test_strings_s {
  /* Encoded string */
  pdf_size_t encoded_size;
  const pdf_char_t *encoded;
  /* Decoded string */
  pdf_size_t decoded_size;
  const pdf_char_t *decoded;
  /* Key: length without trailing '\0' */
  pdf_size_t key_size;
  const pdf_char_t *key;
};

static const struct test_strings_s test_strings[] = {
  {
    48,
    "\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA"  /* iv vector*/
    "\xBB\x03\x13\xEE\x2F\x65\x43\xA9\x04\xF2\x8E\xFF\x68\x59\x05\x35"  /* content */
    "\x81\x7E\x24\x33\x5F\x18\x72\x12\x8C\xB8\x4C\x23\x59\xD0\xE0\x36", /* padding */
    33,
    "\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA" /* iv */
    "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F" /* content */
    "\x0F",
    16,
    "\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xAA\xBB\xCC\xDD\xEE\xFF"
  },
  { 0, NULL, 0, NULL, 0, NULL }
};

static const struct test_params_s tests_params[] = {
  /* No   Test type          Test operation   Loop read size       Cache size */
  {	 1,   TEST_TYPE_DECODER, TEST_OP_READ,    LOOP_RW_SIZE_ONE,    0 },
  {	 2,   TEST_TYPE_DECODER, TEST_OP_READ,    LOOP_RW_SIZE_TWO,    0 },
  {	 3,   TEST_TYPE_DECODER, TEST_OP_READ,    LOOP_RW_SIZE_HALF,   0 },
  {	 4,   TEST_TYPE_DECODER, TEST_OP_READ,    LOOP_RW_SIZE_EXACT,  0 },
  {	 5,   TEST_TYPE_DECODER, TEST_OP_READ,    LOOP_RW_SIZE_DOUBLE, 0 },

  {	 6,   TEST_TYPE_ENCODER, TEST_OP_READ,    LOOP_RW_SIZE_ONE,    0 },
  {	 7,   TEST_TYPE_ENCODER, TEST_OP_READ,    LOOP_RW_SIZE_TWO,    0 },
  {	 8,   TEST_TYPE_ENCODER, TEST_OP_READ,    LOOP_RW_SIZE_HALF,   0 },
  {	 9,   TEST_TYPE_ENCODER, TEST_OP_READ,    LOOP_RW_SIZE_EXACT,  0 },
  {	 10,  TEST_TYPE_ENCODER, TEST_OP_READ,    LOOP_RW_SIZE_DOUBLE, 0 },

  {	 11,  TEST_TYPE_DECODER, TEST_OP_WRITE,   LOOP_RW_SIZE_ONE,    0 },
  {	 12,  TEST_TYPE_DECODER, TEST_OP_WRITE,   LOOP_RW_SIZE_TWO,    0 },
  {  13,  TEST_TYPE_DECODER, TEST_OP_WRITE,   LOOP_RW_SIZE_HALF,   0 },
  {	 14,  TEST_TYPE_DECODER, TEST_OP_WRITE,   LOOP_RW_SIZE_EXACT,  0 },
  {	 15,  TEST_TYPE_DECODER, TEST_OP_WRITE,   LOOP_RW_SIZE_DOUBLE, 0 },

  {	 16,  TEST_TYPE_ENCODER, TEST_OP_WRITE,   LOOP_RW_SIZE_ONE,    0 },
  {	 17,  TEST_TYPE_ENCODER, TEST_OP_WRITE,   LOOP_RW_SIZE_TWO,    0 },
  {	 18,  TEST_TYPE_ENCODER, TEST_OP_WRITE,   LOOP_RW_SIZE_HALF,   0 },
  {	 19,  TEST_TYPE_ENCODER, TEST_OP_WRITE,   LOOP_RW_SIZE_EXACT,  0 },
  {	 20,  TEST_TYPE_ENCODER, TEST_OP_WRITE,   LOOP_RW_SIZE_DOUBLE, 0 },
};

static void
common_test_aesv2 (const pdf_char_t *function_name,
                   int               test_index)
{
  int i;
  const struct test_params_s *params = &tests_params[test_index - 1];

  /* Sanity check */
  fail_if (test_index != params->idx);

  for (i = 0; test_strings[i].encoded; i++)
    {
      pdf_hash_t *filter_params;

      filter_params = pdf_hash_new (NULL);
      pdf_hash_add (filter_params, "Key", test_strings[i].key, NULL, NULL);
      pdf_hash_add_size (filter_params, "KeySize", test_strings[i].key_size, NULL);

      pdf_stm_test_common (function_name,
                           params->type,
                           params->operation,
                           (params->type == TEST_TYPE_ENCODER ?
                            PDF_STM_FILTER_AESV2_ENC :
                            PDF_STM_FILTER_AESV2_DEC),
                           filter_params,
                           params->stm_cache_size,
                           params->loop_size,
                           test_strings[i].decoded,
                           test_strings[i].decoded_size,
                           test_strings[i].encoded,
                           test_strings[i].encoded_size);

      pdf_hash_destroy (filter_params);
    }
}

/*
 * Test: pdf_stm_read_filter_aesv2_dec_001-005
 * Description:
 *   Test AESV2 decoder filter with different read loop sizes
 * Success condition:
 *   The read data should be ok.
 */
START_TEST (pdf_stm_read_filter_aesv2_dec_001) { common_test_aesv2 (__FUNCTION__,  1); } END_TEST
START_TEST (pdf_stm_read_filter_aesv2_dec_002) { common_test_aesv2 (__FUNCTION__,  2); } END_TEST
START_TEST (pdf_stm_read_filter_aesv2_dec_003) { common_test_aesv2 (__FUNCTION__,  3); } END_TEST
START_TEST (pdf_stm_read_filter_aesv2_dec_004) { common_test_aesv2 (__FUNCTION__,  4); } END_TEST
START_TEST (pdf_stm_read_filter_aesv2_dec_005) { common_test_aesv2 (__FUNCTION__,  5); } END_TEST

/*
 * Test: pdf_stm_read_filter_aesv2_enc_001-005
 * Description:
 *   Test AESV2 encoder filter with different read loop sizes
 * Success condition:
 *   The read data should be ok.
 */
START_TEST (pdf_stm_read_filter_aesv2_enc_001) { common_test_aesv2 (__FUNCTION__,  6); } END_TEST
START_TEST (pdf_stm_read_filter_aesv2_enc_002) { common_test_aesv2 (__FUNCTION__,  7); } END_TEST
START_TEST (pdf_stm_read_filter_aesv2_enc_003) { common_test_aesv2 (__FUNCTION__,  8); } END_TEST
START_TEST (pdf_stm_read_filter_aesv2_enc_004) { common_test_aesv2 (__FUNCTION__,  9); } END_TEST
START_TEST (pdf_stm_read_filter_aesv2_enc_005) { common_test_aesv2 (__FUNCTION__, 10); } END_TEST

/*
 * Test: pdf_stm_write_filter_aesv2_dec_001-005
 * Description:
 *   Test AESV2 decoder filter with different write loop sizes
 * Success condition:
 *   The written data should be ok.
 */
START_TEST (pdf_stm_write_filter_aesv2_dec_001) { common_test_aesv2 (__FUNCTION__, 11); } END_TEST
START_TEST (pdf_stm_write_filter_aesv2_dec_002) { common_test_aesv2 (__FUNCTION__, 12); } END_TEST
START_TEST (pdf_stm_write_filter_aesv2_dec_003) { common_test_aesv2 (__FUNCTION__, 13); } END_TEST
START_TEST (pdf_stm_write_filter_aesv2_dec_004) { common_test_aesv2 (__FUNCTION__, 14); } END_TEST
START_TEST (pdf_stm_write_filter_aesv2_dec_005) { common_test_aesv2 (__FUNCTION__, 15); } END_TEST

/*
 * Test: pdf_stm_write_filter_aesv2_enc_001-005
 * Description:
 *   Test AESV2 encoder filter with different write loop sizes
 * Success condition:
 *   The written data should be ok.
 */
START_TEST (pdf_stm_write_filter_aesv2_enc_001) { common_test_aesv2 (__FUNCTION__, 16); } END_TEST
START_TEST (pdf_stm_write_filter_aesv2_enc_002) { common_test_aesv2 (__FUNCTION__, 17); } END_TEST
START_TEST (pdf_stm_write_filter_aesv2_enc_003) { common_test_aesv2 (__FUNCTION__, 18); } END_TEST
START_TEST (pdf_stm_write_filter_aesv2_enc_004) { common_test_aesv2 (__FUNCTION__, 19); } END_TEST
START_TEST (pdf_stm_write_filter_aesv2_enc_005) { common_test_aesv2 (__FUNCTION__, 20); } END_TEST

/*
 * Test case creation functions
 */

TCase *
test_pdf_stm_rw_filter_aesv2 (void)
{
  TCase *tc = tcase_create ("pdf_stm_rw_filter_aesv2");

  tcase_add_test (tc, pdf_stm_read_filter_aesv2_dec_001);
  tcase_add_test (tc, pdf_stm_read_filter_aesv2_dec_002);
  tcase_add_test (tc, pdf_stm_read_filter_aesv2_dec_003);
  tcase_add_test (tc, pdf_stm_read_filter_aesv2_dec_004);
  tcase_add_test (tc, pdf_stm_read_filter_aesv2_dec_005);

  tcase_add_test (tc, pdf_stm_read_filter_aesv2_enc_001);
  tcase_add_test (tc, pdf_stm_read_filter_aesv2_enc_002);
  tcase_add_test (tc, pdf_stm_read_filter_aesv2_enc_003);
  tcase_add_test (tc, pdf_stm_read_filter_aesv2_enc_004);
  tcase_add_test (tc, pdf_stm_read_filter_aesv2_enc_005);

  tcase_add_test (tc, pdf_stm_write_filter_aesv2_dec_001);
  tcase_add_test (tc, pdf_stm_write_filter_aesv2_dec_002);
  tcase_add_test (tc, pdf_stm_write_filter_aesv2_dec_003);
  tcase_add_test (tc, pdf_stm_write_filter_aesv2_dec_004);
  tcase_add_test (tc, pdf_stm_write_filter_aesv2_dec_005);

  tcase_add_test (tc, pdf_stm_write_filter_aesv2_enc_001);
  tcase_add_test (tc, pdf_stm_write_filter_aesv2_enc_002);
  tcase_add_test (tc, pdf_stm_write_filter_aesv2_enc_003);
  tcase_add_test (tc, pdf_stm_write_filter_aesv2_enc_004);
  tcase_add_test (tc, pdf_stm_write_filter_aesv2_enc_005);

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-stm-rw-filter-aesv2.c */

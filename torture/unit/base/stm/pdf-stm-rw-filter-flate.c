/* -*- mode: C -*-
 *
 *       File:         pdf-stm-read-filter-flate.c
 *       Date:         Thu May 05 22:56:09 2011
 *
 *       GNU PDF Library - Unit tests for pdf_stm_read with FLATE filter.
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
  /* FLATE-Encoded string */
  pdf_size_t encoded_size;
  const pdf_char_t *encoded;
  /* Decoded string */
  pdf_size_t decoded_size;
  const pdf_char_t *decoded;
};

static const struct test_strings_s test_strings[] = {
  {
    41,
    "\x78\x9c\x33\x34\x32\x32\x36\x36\x36\x01\x02\x53\x10\x30\x03\x03"
    "\x73\x08\xb0\x80\x02\x4b\x18\x30\x30\x1c\x55\x3d\xaa\x7a\x54\xf5"
    "\x88\x52\xcd\x00\x00\xe1\x0b\xdf\xfc",
    1059,
    "1223334444555556666667777777888888889999999990"
    "1223334444555556666667777777888888889999999990"
    "1223334444555556666667777777888888889999999990"
    "1223334444555556666667777777888888889999999990"
    "1223334444555556666667777777888888889999999990"
    "1223334444555556666667777777888888889999999990"
    "1223334444555556666667777777888888889999999990"
    "1223334444555556666667777777888888889999999990"
    "1223334444555556666667777777888888889999999990"
    "1223334444555556666667777777888888889999999990"
    "1223334444555556666667777777888888889999999990"
    "1223334444555556666667777777888888889999999990"
    "1223334444555556666667777777888888889999999990"
    "1223334444555556666667777777888888889999999990"
    "1223334444555556666667777777888888889999999990"
    "1223334444555556666667777777888888889999999990"
    "1223334444555556666667777777888888889999999990"
    "1223334444555556666667777777888888889999999990"
    "1223334444555556666667777777888888889999999990"
    "1223334444555556666667777777888888889999999990"
    "1223334444555556666667777777888888889999999990"
    "1223334444555556666667777777888888889999999990"
    "1223334444555556666667777777888888889999999990"
  },
  { 0, NULL, 0, NULL }
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
common_test_flate (const pdf_char_t *function_name,
                   int               test_index)
{
  int i;
  const struct test_params_s *params = &tests_params[test_index - 1];

  /* Sanity check */
  fail_if (test_index != params->idx);

  for (i = 0; test_strings[i].encoded; i++)
    {
      pdf_stm_test_common (function_name,
                           params->type,
                           params->operation,
                           (params->type == TEST_TYPE_ENCODER ?
                            PDF_STM_FILTER_FLATE_ENC :
                            PDF_STM_FILTER_FLATE_DEC),
                           NULL,
                           params->stm_cache_size,
                           params->loop_size,
                           test_strings[i].decoded,
                           test_strings[i].decoded_size,
                           test_strings[i].encoded,
                           test_strings[i].encoded_size);
    }
}

/*
 * Test: pdf_stm_read_filter_flate_dec_001-005
 * Description:
 *   Test FLATE decoder filter with different read loop sizes
 * Success condition:
 *   The read data should be ok.
 */
START_TEST (pdf_stm_read_filter_flate_dec_001) { common_test_flate (__FUNCTION__,  1); } END_TEST
START_TEST (pdf_stm_read_filter_flate_dec_002) { common_test_flate (__FUNCTION__,  2); } END_TEST
START_TEST (pdf_stm_read_filter_flate_dec_003) { common_test_flate (__FUNCTION__,  3); } END_TEST
START_TEST (pdf_stm_read_filter_flate_dec_004) { common_test_flate (__FUNCTION__,  4); } END_TEST
START_TEST (pdf_stm_read_filter_flate_dec_005) { common_test_flate (__FUNCTION__,  5); } END_TEST

/*
 * Test: pdf_stm_read_filter_flate_enc_001-005
 * Description:
 *   Test FLATE encoder filter with different read loop sizes
 * Success condition:
 *   The read data should be ok.
 */
START_TEST (pdf_stm_read_filter_flate_enc_001) { common_test_flate (__FUNCTION__,  6); } END_TEST
START_TEST (pdf_stm_read_filter_flate_enc_002) { common_test_flate (__FUNCTION__,  7); } END_TEST
START_TEST (pdf_stm_read_filter_flate_enc_003) { common_test_flate (__FUNCTION__,  8); } END_TEST
START_TEST (pdf_stm_read_filter_flate_enc_004) { common_test_flate (__FUNCTION__,  9); } END_TEST
START_TEST (pdf_stm_read_filter_flate_enc_005) { common_test_flate (__FUNCTION__, 10); } END_TEST

/*
 * Test: pdf_stm_write_filter_flate_dec_001-005
 * Description:
 *   Test FLATE decoder filter with different write loop sizes
 * Success condition:
 *   The written data should be ok.
 */
START_TEST (pdf_stm_write_filter_flate_dec_001) { common_test_flate (__FUNCTION__, 11); } END_TEST
START_TEST (pdf_stm_write_filter_flate_dec_002) { common_test_flate (__FUNCTION__, 12); } END_TEST
START_TEST (pdf_stm_write_filter_flate_dec_003) { common_test_flate (__FUNCTION__, 13); } END_TEST
START_TEST (pdf_stm_write_filter_flate_dec_004) { common_test_flate (__FUNCTION__, 14); } END_TEST
START_TEST (pdf_stm_write_filter_flate_dec_005) { common_test_flate (__FUNCTION__, 15); } END_TEST

/*
 * Test: pdf_stm_write_filter_flate_enc_001-005
 * Description:
 *   Test FLATE encoder filter with different write loop sizes
 * Success condition:
 *   The written data should be ok.
 */
START_TEST (pdf_stm_write_filter_flate_enc_001) { common_test_flate (__FUNCTION__, 16); } END_TEST
START_TEST (pdf_stm_write_filter_flate_enc_002) { common_test_flate (__FUNCTION__, 17); } END_TEST
START_TEST (pdf_stm_write_filter_flate_enc_003) { common_test_flate (__FUNCTION__, 18); } END_TEST
START_TEST (pdf_stm_write_filter_flate_enc_004) { common_test_flate (__FUNCTION__, 19); } END_TEST
START_TEST (pdf_stm_write_filter_flate_enc_005) { common_test_flate (__FUNCTION__, 20); } END_TEST

/*
 * Test case creation functions
 */

TCase *
test_pdf_stm_rw_filter_flate (void)
{
  TCase *tc = tcase_create ("pdf_stm_rw_filter_flate");

  tcase_add_test (tc, pdf_stm_read_filter_flate_dec_001);
  tcase_add_test (tc, pdf_stm_read_filter_flate_dec_002);
  tcase_add_test (tc, pdf_stm_read_filter_flate_dec_003);
  tcase_add_test (tc, pdf_stm_read_filter_flate_dec_004);
  tcase_add_test (tc, pdf_stm_read_filter_flate_dec_005);

  tcase_add_test (tc, pdf_stm_read_filter_flate_enc_001);
  tcase_add_test (tc, pdf_stm_read_filter_flate_enc_002);
  tcase_add_test (tc, pdf_stm_read_filter_flate_enc_003);
  tcase_add_test (tc, pdf_stm_read_filter_flate_enc_004);
  tcase_add_test (tc, pdf_stm_read_filter_flate_enc_005);

  tcase_add_test (tc, pdf_stm_write_filter_flate_dec_001);
  tcase_add_test (tc, pdf_stm_write_filter_flate_dec_002);
  tcase_add_test (tc, pdf_stm_write_filter_flate_dec_003);
  tcase_add_test (tc, pdf_stm_write_filter_flate_dec_004);
  tcase_add_test (tc, pdf_stm_write_filter_flate_dec_005);

  tcase_add_test (tc, pdf_stm_write_filter_flate_enc_001);
  tcase_add_test (tc, pdf_stm_write_filter_flate_enc_002);
  tcase_add_test (tc, pdf_stm_write_filter_flate_enc_003);
  tcase_add_test (tc, pdf_stm_write_filter_flate_enc_004);
  tcase_add_test (tc, pdf_stm_write_filter_flate_enc_005);

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-stm-read-filter-flate.c */

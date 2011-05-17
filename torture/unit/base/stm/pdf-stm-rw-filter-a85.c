/* -*- mode: C -*-
 *
 *       File:         pdf-stm-read-filter-a85.c
 *       Date:         Tue May 10 20:34:34 2011
 *
 *       GNU PDF Library - Unit tests for pdf_stm_read with A85 filter.
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

/*
 * The following is the spec from the gnupdf site for the ASCII85 filter
 * I have placed within this text requirement identifiers in the form **(#n)**
 * to identify requirements coverage in the test cases.
 *
 * From gnupdf.org:
 *
 * The ASCII85Decode filter decodes data that has been encoded in ASCII
 * base-85 encoding and produces binary data. The following paragraphs
 * describe the process for encoding binary data in ASCII base-85; the
 * ASCII85Decode filter reverses this process.
 *
 * The ASCII base-85 encoding uses the characters ! through u and the
 * character z, with the 2-character sequence ~> as its EOD marker. The
 * ASCII85Decode filter ignores all white-space characters **(#1)**. Any other
 * characters **(#2)**, and any character sequences that represent impossible
 * combinations in the ASCII base-85 encoding **(#3)**, cause an error.
 *
 * Specifically, ASCII base-85 encoding produces 5 ASCII characters for
 * every 4 bytes of binary data. Each group of 4binary input bytes,
 * `(b1 b2 b3 b4)`, is converted to a group of 5 output bytes,
 * `(c1 c2 c3 c4 c5)`, using the relation:
 *
 * (b1 * 256^3) + (b2 * 256^2) + (b3 * 256^1) + b4 =
 *
 * (c1 * 85^4) + (c2 * 85^3) + (c3 * 85^2) + (c4 * 85^1) + c5
 *
 * In other words, 4 bytes of binary data are interpreted as a base-256
 * number and then converted to a base-85 number. The five bytes of the
 * base-85 number are then converted to ASCII characters by adding 33
 * (the ASCII code for the character !) to each. The resulting encoded
 * data contains only printable ASCII characters with codes in the
 * range 33 (!) to 117 (u). As a special case, if all five bytes are 0,
 * they are represented by the character with code 122 (z) **(#4)** instead of
 * by five exclamation points (!!!!!).
 *
 * If the length of the binary data to be encoded is not a multiple of
 * 4 bytes, the last, partial group of 4 is used to produce a last,
 * partial group of 5 output  characters. Given n (1, 2 or 3) bytes of
 * binary data, the encoder first appends 4 - n zero bytes to make a
 * complete group of 4. It then encodes this group in the usual way,
 * but without applying the special z case **(#5)**. Finally, it writes only the
 * first n + 1 characters of the resulting group of 5 **(#6)**. These characters
 * are immediately followed by the ~> EOD marker.
 *
 * The following conditions (which never occur in a correctly encoded
 * byte sequence) cause errors during decoding:
 *
 *  * The value represented by a group of 5 characters is >= 2^32 **(#7)**.
 *  * A z character occurs in the middle of a group. **(#8)**
 *  * A final partial group contains only one character. (**(#9)**
 *
 */

struct test_strings_s {
  /* A85-Encoded string */
  pdf_size_t encoded_size;
  const pdf_char_t *encoded;
  /* Decoded string */
  pdf_size_t decoded_size;
  const pdf_char_t *decoded;
  /* Flag to identify if decoded->encoded relationship is also valid */
  pdf_bool_t can_test_encode;
};

static const struct test_strings_s test_strings[] = {
  {
    18, "!!\x2A\x2D\x27z\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47~>",
    16, "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B",
    PDF_TRUE
  },
  {
    20, "!!\x2A\x2D\x27z\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47\x21\x2D~>",
    17, "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C",
    PDF_TRUE
  },
  {
    /* (#1) */
    23, "\t!!\x2A\x2D\x27z\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47~>\t\n\n\n",
    16, "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B",
    PDF_FALSE
  },
  {
    /* (#1) */
    23, "!!\x2A\x2D\x27z\x22\x39\n\n\n\x65\x75\x37\x23\x52\x4C\x68\x47\n~\n>",
    16, "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B",
    PDF_FALSE
  },
  {
    /* (#1) */
    23, "!!\x2A\x2D\x27z\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47\x21\n\x2D\n~\n>",
    17, "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C",
    PDF_FALSE
  },
  {
    21, "!!\x2A\x2D\x27z\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47\x21\x45\x3A~>",
    18, "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D",
    PDF_TRUE
  },
  {
    /* (#6) */
    4, "!!~>",
    1, "\x00",
    PDF_TRUE
  },
  {
    5, "!!!~>",
    2, "\x00\x00",
    PDF_TRUE
  },
  {
    /* (#5, #6) */
    6, "!!!!~>",
    3, "\x00\x00\x00",
    PDF_TRUE
  },
  {
    /* (#4) */
    3, "z~>",
    4, "\x00\x00\x00\x00",
    PDF_TRUE
  },
  {
    /* (#1) */
    13, " z\t~\n>       ",
    4, "\x00\x00\x00\x00",
    PDF_FALSE
  },
  {
    /* (#8) Note that this z is in an invalid position */
    18, "!!\x2A\x2Dz\x27\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47~>",
    0, NULL,
    PDF_FALSE
  },
  {
    /* (#8) Note that this z is in an invalid position */
    18, "!!\x2Az\x2D\x27\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47~>",
    0, NULL,
    PDF_FALSE
  },
  {
    /* (#8) Preceding z is in an invalid position */
    18, "!!z\x2A\x2D\x27\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47~>",
    0, NULL,
    PDF_FALSE
  },
  {
    /* (#8) Preceding z is in an invalid position */
    18, "!z!\x2A\x2D\x27\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47~>",
    0, NULL,
    PDF_FALSE
  },
  {
    /* (#9) Note that terminating group is only one byte */
    19, "!!\x2A\x2D\x27z\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47\x48~>",
    0, NULL,
    PDF_FALSE
  },
  {
    /* (#7) Note that Third group is above 2^32 limit */
    18, "!!\x2A\x2D\x27z\x73\x75\x75\x75\x75\x23\x52\x4C\x68\x47~>",
    0, NULL,
    PDF_FALSE
  },
  {
    /* (#7) Note that Third group is above 2^32 limit */
    18, "!!\x2A\x2D\x27z\x74\x21\x21\x21\x21\x23\x52\x4C\x68\x47~>",
    0, NULL,
    PDF_FALSE
  },
  {
    /* (#7) Note that Third group is above 2^32 limit */
    18, "!!\x2A\x2D\x27z\x73\x38\x57\x2D\x22\x23\x52\x4C\x68\x47~>",
    0, NULL,
    PDF_FALSE
  },
  {
    /* (#7) Note that Third group is above 2^32 limit */
    18, "!!\x2A\x2D\x27z\x73\x38\x57\x2D\x23\x23\x52\x4C\x68\x47~>",
    0, NULL,
    PDF_FALSE
  },
  {
    /* (#7) Note that Third group is above 2^32 limit */
    18, "!!\x2A\x2D\x27z\x73\x38\x57\x2E\x21\x23\x52\x4C\x68\x47~>",
    0, NULL,
    PDF_FALSE
  },
  {
    /* (#7) Note that Third group is above 2^32 limit */
    18, "!!\x2A\x2D\x27z\x73\x38\x58\x2D\x21\x23\x52\x4C\x68\x47~>",
    0, NULL,
    PDF_FALSE
  },
  {
    /* (#7) Note that Third group is above 2^32 limit */
    18, "!!\x2A\x2D\x27z\x73\x39\x57\x2D\x21\x23\x52\x4C\x68\x47~>",
    0, NULL,
    PDF_FALSE
  },
  {
    /* (#2) Note that Last character is out of allowed range */
    18, "!!\x2A\x2D\x27z\x63\x39\x57\x2D\x21\x23\x52\x4C\x68\x76~>",
    0, NULL,
    PDF_FALSE
  },
  {
    /* (#3) Note that Last character is out of allowed range */
    18, "!!\x2A\x2D\x27z\x63\x39\x57\x2D\x21\x23\x52\x4C\x68\x00~>",
    0, NULL,
    PDF_FALSE
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
common_test_a85 (const pdf_char_t *function_name,
                  int               test_index)
{
  int i;
  const struct test_params_s *params = &tests_params[test_index - 1];

  /* Sanity check */
  fail_if (test_index != params->idx);

  for (i = 0; test_strings[i].encoded; i++)
    {
      if (params->type == TEST_TYPE_DECODER ||
          test_strings[i].can_test_encode)
        {
          pdf_stm_test_common (function_name,
                               params->type,
                               params->operation,
                               (params->type == TEST_TYPE_ENCODER ?
                                PDF_STM_FILTER_A85_ENC :
                                PDF_STM_FILTER_A85_DEC),
                               NULL,
                               params->stm_cache_size,
                               params->loop_size,
                               test_strings[i].decoded,
                               test_strings[i].decoded_size,
                               test_strings[i].encoded,
                               test_strings[i].encoded_size);
        }
    }
}

/*
 * Test: pdf_stm_read_filter_a85_dec_001-005
 * Description:
 *   Test A85 decoder filter with different read loop sizes
 * Success condition:
 *   The read data should be ok.
 */
START_TEST (pdf_stm_read_filter_a85_dec_001) { common_test_a85 (__FUNCTION__,  1); } END_TEST
START_TEST (pdf_stm_read_filter_a85_dec_002) { common_test_a85 (__FUNCTION__,  2); } END_TEST
START_TEST (pdf_stm_read_filter_a85_dec_003) { common_test_a85 (__FUNCTION__,  3); } END_TEST
START_TEST (pdf_stm_read_filter_a85_dec_004) { common_test_a85 (__FUNCTION__,  4); } END_TEST
START_TEST (pdf_stm_read_filter_a85_dec_005) { common_test_a85 (__FUNCTION__,  5); } END_TEST

/*
 * Test: pdf_stm_read_filter_a85_enc_001-005
 * Description:
 *   Test A85 encoder filter with different read loop sizes
 * Success condition:
 *   The read data should be ok.
 */
START_TEST (pdf_stm_read_filter_a85_enc_001) { common_test_a85 (__FUNCTION__,  6); } END_TEST
START_TEST (pdf_stm_read_filter_a85_enc_002) { common_test_a85 (__FUNCTION__,  7); } END_TEST
START_TEST (pdf_stm_read_filter_a85_enc_003) { common_test_a85 (__FUNCTION__,  8); } END_TEST
START_TEST (pdf_stm_read_filter_a85_enc_004) { common_test_a85 (__FUNCTION__,  9); } END_TEST
START_TEST (pdf_stm_read_filter_a85_enc_005) { common_test_a85 (__FUNCTION__, 10); } END_TEST

/*
 * Test: pdf_stm_write_filter_a85_dec_001-005
 * Description:
 *   Test A85 decoder filter with different write loop sizes
 * Success condition:
 *   The written data should be ok.
 */
START_TEST (pdf_stm_write_filter_a85_dec_001) { common_test_a85 (__FUNCTION__, 11); } END_TEST
START_TEST (pdf_stm_write_filter_a85_dec_002) { common_test_a85 (__FUNCTION__, 12); } END_TEST
START_TEST (pdf_stm_write_filter_a85_dec_003) { common_test_a85 (__FUNCTION__, 13); } END_TEST
START_TEST (pdf_stm_write_filter_a85_dec_004) { common_test_a85 (__FUNCTION__, 14); } END_TEST
START_TEST (pdf_stm_write_filter_a85_dec_005) { common_test_a85 (__FUNCTION__, 15); } END_TEST

/*
 * Test: pdf_stm_write_filter_a85_enc_001-005
 * Description:
 *   Test A85 encoder filter with different write loop sizes
 * Success condition:
 *   The written data should be ok.
 */
START_TEST (pdf_stm_write_filter_a85_enc_001) { common_test_a85 (__FUNCTION__, 16); } END_TEST
START_TEST (pdf_stm_write_filter_a85_enc_002) { common_test_a85 (__FUNCTION__, 17); } END_TEST
START_TEST (pdf_stm_write_filter_a85_enc_003) { common_test_a85 (__FUNCTION__, 18); } END_TEST
START_TEST (pdf_stm_write_filter_a85_enc_004) { common_test_a85 (__FUNCTION__, 19); } END_TEST
START_TEST (pdf_stm_write_filter_a85_enc_005) { common_test_a85 (__FUNCTION__, 20); } END_TEST

/*
 * Test case creation functions
 */

TCase *
test_pdf_stm_rw_filter_a85 (void)
{
  TCase *tc = tcase_create ("pdf_stm_rw_filter_a85");

  tcase_add_test (tc, pdf_stm_read_filter_a85_dec_001);
  tcase_add_test (tc, pdf_stm_read_filter_a85_dec_002);
  tcase_add_test (tc, pdf_stm_read_filter_a85_dec_003);
  tcase_add_test (tc, pdf_stm_read_filter_a85_dec_004);
  tcase_add_test (tc, pdf_stm_read_filter_a85_dec_005);

  tcase_add_test (tc, pdf_stm_read_filter_a85_enc_001);
  tcase_add_test (tc, pdf_stm_read_filter_a85_enc_002);
  tcase_add_test (tc, pdf_stm_read_filter_a85_enc_003);
  tcase_add_test (tc, pdf_stm_read_filter_a85_enc_004);
  tcase_add_test (tc, pdf_stm_read_filter_a85_enc_005);

  tcase_add_test (tc, pdf_stm_write_filter_a85_dec_001);
  tcase_add_test (tc, pdf_stm_write_filter_a85_dec_002);
  tcase_add_test (tc, pdf_stm_write_filter_a85_dec_003);
  tcase_add_test (tc, pdf_stm_write_filter_a85_dec_004);
  tcase_add_test (tc, pdf_stm_write_filter_a85_dec_005);

  tcase_add_test (tc, pdf_stm_write_filter_a85_enc_001);
  tcase_add_test (tc, pdf_stm_write_filter_a85_enc_002);
  tcase_add_test (tc, pdf_stm_write_filter_a85_enc_003);
  tcase_add_test (tc, pdf_stm_write_filter_a85_enc_004);
  tcase_add_test (tc, pdf_stm_write_filter_a85_enc_005);

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-stm-read-filter-a85.c */

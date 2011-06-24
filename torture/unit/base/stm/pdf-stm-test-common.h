/* -*- mode: C -*-
 *
 *       File:         pdf-stm-test-common.h
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

#ifndef _PDF_STM_TEST_COMMON_H
#define _PDF_STM_TEST_COMMON_H

#include <config.h>

#include <string.h>
#include <check.h>
#include <pdf.h>
#include <pdf-test-common.h>

/* Types of tests */
enum test_type_e {
  TEST_TYPE_ENCODER,
  TEST_TYPE_DECODER
};

/* Types of operations to test */
enum test_op_e {
  TEST_OP_READ,
  TEST_OP_WRITE
};

/* Different sizes to be used while reading from the stream in a loop */
enum loop_rw_size_e {
  LOOP_RW_SIZE_ONE = 0, /* byte per byte */
  LOOP_RW_SIZE_TWO,     /* two bytes */
  LOOP_RW_SIZE_HALF,    /* half of the input string */
  LOOP_RW_SIZE_EXACT,   /* whole input string */
  LOOP_RW_SIZE_DOUBLE,  /* try to read/write more than the whole input string */
};

/* Helper struct definition to define tests */
struct test_params_s {
  /* Test index */
  int idx;
  /* Test type */
  enum test_type_e type;
  /* Test operation */
  enum test_op_e operation;
  /* Number of bytes to read/write in each loop */
  enum loop_rw_size_e loop_size;
  /* Stream cache size, 0 if default */
  pdf_size_t stm_cache_size;
};

/* Common tester method */
void pdf_stm_test_common (const pdf_char_t           *test_name,
                          enum test_type_e            test_type,
                          enum test_op_e              test_operation,
                          enum pdf_stm_filter_type_e  filter_type,
                          const pdf_hash_t           *filter_params,
                          pdf_size_t                  stm_cache_size,
                          enum loop_rw_size_e         loop_rw_size_e,
                          const pdf_char_t           *decoded,
                          pdf_size_t                  decoded_size,
                          const pdf_char_t           *encoded,
                          pdf_size_t                  encoded_size);

#endif /* _PDF_STM_TEST_COMMON_H */

/* End of pdf-stm-test-common.h */

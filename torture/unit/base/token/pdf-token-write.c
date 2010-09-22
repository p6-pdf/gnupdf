/* -*- mode: C -*- Time-stamp: "2010-09-22 20:13:59 jemarch"
 *
 *       File:         pdf-token-write.c
 *       Date:         Tue Sep 21 21:08:07 2010
 *
 *       GNU PDF Library - Unit tests for pdf_token_write
 *
 */

/* Copyright (C) 2010 Free Software Foundation, Inc. */

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
#include <stdlib.h>

#include <pdf.h>


/* Write a token in an in-memory buffer and compare results.  */
static void
write_and_check (pdf_token_t token,
                 pdf_u32_t flags,
                 pdf_char_t *expected,
                 pdf_size_t expected_size,
                 pdf_size_t max_size)
{
  pdf_stm_t stm;
  pdf_char_t *buffer;
  pdf_token_writer_t writer;

  /* Create the in-memory stream.  */
  fail_if ((buffer = pdf_alloc (max_size)) == NULL);
  fail_if (pdf_stm_mem_new (buffer, max_size, 0, PDF_STM_WRITE, &stm)
           != PDF_OK);

  /* Create the token writer.  */
  fail_if (pdf_token_writer_new (stm, &writer) != PDF_OK);
  
  /* Write the token.  */
  fail_if (pdf_token_write (writer, flags, token) != PDF_OK);
  
  /* Destroy writer and stream.  */
  fail_if (pdf_token_writer_destroy (writer));
  fail_if (pdf_stm_destroy (stm) != PDF_OK);

  /* Compare results.  */
  printf ("XXX: %s\n", buffer);
  fail_unless (strncmp (buffer, expected, expected_size) == 0);
}

/*
 * Test: pdf_token_write_integer_positive
 * Description:
 *   Write a positive integer token into an in-memory stream, and
 *   check whether the resulting textual representation is the
 *   expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_integer_positive)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_integer_new (10, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "10", 2, 100);
}
END_TEST

/*
 * Test: pdf_token_write_integer_negative
 * Description:
 *   Write a negative integer token into an in-memory stream, and
 *   check whether the resulting textual representation is the
 *   expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_integer_negative)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_integer_new (-10, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "-10", 3, 100);
}
END_TEST

/*
 * Test: pdf_token_write_real_positive
 * Description:
 *   Write a positive real token into an in-memory stream, and check
 *   whether the resulting textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_real_positive)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_real_new (10.2, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "10.2", 3, 100);
}
END_TEST

/*
 * Test: pdf_token_write_real_round
 * Description:
 *   Write a rounded real token into an in-memory stream, and check
 *   whether the resulting textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_real_round)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_real_new (10.0, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "10.", 3, 100);
}
END_TEST

/*
 * Test: pdf_token_write_real_negative
 * Description:
 *   Write a negative real token into an in-memory stream, and check
 *   whether the resulting textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_real_negative)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_real_new (-10.0, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "-10.0", 4, 100);
}
END_TEST

/*
 * Test: pdf_token_write_string_nonempty
 * Description:
 *   Write a non empty string token into an in-memory stream, and
 *   check whether the resulting textual representation is the
 *   expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_nonempty)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("abc", 3, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "(abc)", 5, 100);
}
END_TEST

/*
 * Test: pdf_token_write_string_empty
 * Description:
 *   Write an empty string token into an in-memory stream, and
 *   check whether the resulting textual representation is the
 *   expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_empty)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("", 0, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "()", 2, 100);
}
END_TEST

/*
 * Test: pdf_token_write_string_nonempty_hex
 * Description:
 *   Write a non empty string token into an in-memory stream using the
 *   hexadecimal format , and check whether the resulting textual
 *   representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_nonempty_hex)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("abc", 3, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   PDF_TOKEN_HEX_STRINGS,  /* Flags.  */
                   "<616263>", 8, 100);
}
END_TEST

/*
 * Test: pdf_token_write_string_empty_hex
 * Description:
 *   Write an empty string token into an in-memory stream using the
 *   hexadecimal format , and check whether the resulting textual
 *   representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_empty_hex)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("", 0, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   PDF_TOKEN_HEX_STRINGS,  /* Flags.  */
                   "<>", 2, 100);
}
END_TEST

/*
 * Test: pdf_token_write_string_lf
 * Description:
 *   Write a string token containing line feeds (0Ah) characters into
 *   an in-memory stream using the hexadecimal format , and check
 *   whether the resulting textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_lf)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("a\nb\nc", 0, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "(a\\nb\\nc)", 11, 100);
}
END_TEST

/*
 * Test: pdf_token_write_string_cr
 * Description:
 *   Write a string token containing carriage return (0Dh) characters
 *   into an in-memory stream using the hexadecimal format , and check
 *   whether the resulting textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_cr)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("a\rb\rc", 0, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "(a\\rb\\rc)", 11, 100);
}
END_TEST

/*
 * Test: pdf_token_write_string_ht
 * Description:
 *   Write a string token containing horizontal tab (09h) characters
 *   into an in-memory stream using the hexadecimal format , and check
 *   whether the resulting textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_ht)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("a\tb\tc", 0, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "(a\\tb\\tc)", 11, 100);
}
END_TEST

/*
 * Test: pdf_token_write_string_bs
 * Description:
 *   Write a string token containing backspace (09h) characters
 *   into an in-memory stream using the hexadecimal format , and check
 *   whether the resulting textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_bs)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("a\bb\bc", 0, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "(a\\bb\\bc)", 11, 100);
}
END_TEST

/*
 * Test: pdf_token_write_string_ff
 * Description:
 *   Write a string token containing form feed (ffh) characters
 *   into an in-memory stream using the hexadecimal format , and check
 *   whether the resulting textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_ff)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("a\fb\fc", 0, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "(a\\fb\\fc)", 11, 100);
}
END_TEST

/*
 * Test: pdf_token_write_string_leftp
 * Description:
 *   Write a string token containing left parenthesis (28h) characters
 *   into an in-memory stream using the hexadecimal format , and check
 *   whether the resulting textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_leftp)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("a(b(c", 0, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "(a\\(b\\(c)", 11, 100);
}
END_TEST

/*
 * Test: pdf_token_write_string_rightp
 * Description:
 *   Write a string token containing right parenthesis (29h) characters
 *   into an in-memory stream using the hexadecimal format , and check
 *   whether the resulting textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_rightp)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("a)b)c", 0, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "(a\\)b\\)c)", 11, 100);
}
END_TEST

/*
 * Test: pdf_token_write_string_rs
 * Description:
 *   Write a string token containing reverse solidus (5Ch) characters
 *   into an in-memory stream using the hexadecimal format , and check
 *   whether the resulting textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_rs)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("a\\b\\c", 0, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "(a\\\\b\\\\)", 12, 100);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_token_write (void)
{
  TCase *tc = tcase_create ("pdf_token_write");
  tcase_add_test (tc, pdf_token_write_integer_positive);
  tcase_add_test (tc, pdf_token_write_integer_negative);
  tcase_add_test (tc, pdf_token_write_real_positive);
  tcase_add_test (tc, pdf_token_write_real_round);
  tcase_add_test (tc, pdf_token_write_real_negative);
  tcase_add_test (tc, pdf_token_write_string_nonempty);
  tcase_add_test (tc, pdf_token_write_string_empty);
  tcase_add_test (tc, pdf_token_write_string_nonempty_hex);
  tcase_add_test (tc, pdf_token_write_string_empty_hex);
  tcase_add_test (tc, pdf_token_write_string_lf);
  tcase_add_test (tc, pdf_token_write_string_cr);
  tcase_add_test (tc, pdf_token_write_string_ht);
  tcase_add_test (tc, pdf_token_write_string_bs);
  tcase_add_test (tc, pdf_token_write_string_ff);
  tcase_add_test (tc, pdf_token_write_string_leftp);
  tcase_add_test (tc, pdf_token_write_string_rightp);
  tcase_add_test (tc, pdf_token_write_string_rs);

  return tc;
}

/* End of pdf-token-write.c */

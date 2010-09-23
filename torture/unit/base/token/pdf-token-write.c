/* -*- mode: C -*- Time-stamp: "2010-09-23 20:11:03 jemarch"
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
  printf ("XXX: %s YYY: %s\n", buffer, expected);
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
 *   an in-memory stream, and check whether the resulting textual
 *   representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_lf)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("a\nb\nc", 5, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "(a\nb\nc)", 7, 100);
}
END_TEST

/*
 * Test: pdf_token_write_string_cr
 * Description:
 *   Write a string token containing carriage return (0Dh) characters
 *   into an in-memory stream, and check whether the resulting textual
 *   representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_cr)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("a\rb\rc", 5, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "(a\\rb\\rc)", 9, 100);
}
END_TEST

/*
 * Test: pdf_token_write_string_ht
 * Description:
 *   Write a string token containing horizontal tab (09h) characters
 *   into an in-memory stream, and check whether the resulting textual
 *   representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_ht)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("a\tb\tc", 5, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "(a\tb\tc)", 7, 100);
}
END_TEST

/*
 * Test: pdf_token_write_string_ht_readable
 * Description:
 *   Write a string token containing horizontal tab (09h) characters
 *   into an in-memory stream using the readable format, and check
 *   whether the resulting textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_ht_readable)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("a\tb\tc", 5, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   PDF_TOKEN_READABLE_STRINGS,  /* Flags.  */
                   "(a\\tb\\tc)", 9, 100);
}
END_TEST

/*
 * Test: pdf_token_write_string_ff
 * Description:
 *   Write a string token containing form feed (ffh) characters into
 *   an in-memory stream, and check whether the resulting textual
 *   representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_ff)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("a\fb\fc", 5, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "(a\fb\fc)", 7, 100);
}
END_TEST


/*
 * Test: pdf_token_write_string_ff_readable
 * Description:
 *   Write a string token containing form feed (ffh) characters into
 *   an in-memory stream using the readable format, and check whether
 *   the resulting textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_ff_readable)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("a\fb\fc", 5, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   PDF_TOKEN_READABLE_STRINGS,  /* Flags.  */
                   "(a\\fb\\fc)", 9, 100);
}
END_TEST

/*
 * Test: pdf_token_write_string_leftp
 * Description:
 *   Write a string token containing left parenthesis (28h) characters
 *   into an in-memory stream, and check whether the resulting textual
 *   representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_leftp)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("a(b(c", 5, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "(a\\(b\\(c)", 9, 100);
}
END_TEST

/*
 * Test: pdf_token_write_string_rightp
 * Description:
 *   Write a string token containing right parenthesis (29h)
 *   characters into an in-memory stream, and check whether the
 *   resulting textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_rightp)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("a)b)c", 5, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "(a\\)b\\)c)", 9, 100);
}
END_TEST

/*
 * Test: pdf_token_write_string_rs
 * Description:
 *   Write a string token containing reverse solidus (5Ch) characters
 *   into an in-memory stream, and check whether the resulting textual
 *   representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_rs)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("a\\b\\c", 5, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "(a\\\\b\\\\c)", 9, 100);
}
END_TEST

/*
 * Test: pdf_token_write_string_octal
 * Description:
 *   Write a string token containing non-printable octal characters
 *   into an in-memory stream, and check whether the resulting textual
 *   representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_octal)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("a\0007c", 3, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "(a\0007c)", 5, 100);
}
END_TEST

/*
 * Test: pdf_token_write_string_octal_readable
 * Description:
 *   Write a string token containing non-printable octal characters
 *   into an in-memory stream using the readable format, and check
 *   whether the resulting textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_octal_readable)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("a\0007c", 3, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   PDF_TOKEN_READABLE_STRINGS,  /* Flags.  */
                   "(a\\007c)", 9, 100);
}
END_TEST

/*
 * Test: pdf_token_write_string_null
 * Description:
 *   Write a string token containing a null (00H) character, and check
 *   whether the resulting textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_string_null)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_string_new ("a\0000c", 3, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "(a\00000c)", 9, 100);
}
END_TEST

/*
 * Test: pdf_token_write_name_nonempty
 * Description:
 *   Write a non empty name token, and check whether the resulting
 *   textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_name_nonempty)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_name_new ("abc", 3, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "/abc", 4, 100);
}
END_TEST

/*
 * Test: pdf_token_write_name_empty
 * Description:
 *   Write an empty name token, and check whether the resulting
 *   textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_name_empty)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_name_new ("", 0, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "/", 1, 100);
}
END_TEST

/*
 * Test: pdf_token_write_name_nonregular
 * Description:
 *   Write a name token containing a non-regular character, and check
 *   whether the resulting textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_name_nonregular)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_name_new ("a\nb", 3, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "/a#0Ab", 6, 100);
}
END_TEST

/*
 * Test: pdf_token_write_name_alphanum
 * Description:
 *   Write a name token with alphanumeric characters, and check
 *   whether the resulting textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_name_alphanum)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_name_new ("Name1", 5, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "/Name1", 6, 100);
}
END_TEST

/*
 * Test: pdf_token_write_name_long
 * Description:
 *   Write a long name token, and check whether the resulting textual
 *   representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_name_long)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_name_new ("ASomewhatLongerName", 19, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "/ASomewhatLongerName", 20, 100);
}
END_TEST

/*
 * Test: pdf_token_write_name_various
 * Description:
 *   Write a name token with heterogeneous characters, and check
 *   whether the resulting textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_name_various)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_name_new ("A;Name_With-Various***Characters?", 33, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "/A;Name_With-Various***Characters?", 34, 100);
}
END_TEST

/*
 * Test: pdf_token_write_name_real
 * Description:
 *   Write a name token that looks like a real, and check
 *   whether the resulting textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_name_real)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_name_new ("1.2", 3, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "/1.2", 4, 100);
}
END_TEST

/*
 * Test: pdf_token_write_name_dollars
 * Description:
 *   Write a name token containing dollar characters, and check
 *   whether the resulting textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_name_dollars)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_name_new ("$$", 2, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "/$$", 3, 100);
}
END_TEST

/*
 * Test: pdf_token_write_name_at
 * Description:
 *   Write a name token containing an at character, and check
 *   whether the resulting textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_name_at)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_name_new ("@pattern", 8, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "/@pattern", 9, 100);
}
END_TEST

/*
 * Test: pdf_token_write_name_dot
 * Description:
 *   Write a name token beginning with a dot character, and check
 *   whether the resulting textual representation is the expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_name_dot)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_name_new (".notdef", 7, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "/.notdef", 8, 100);
}
END_TEST

/*
 * Test: pdf_token_write_name_ns
 * Description:
 *   Write a name token containing a number sign character (#), and
 *   check whether the resulting textual representation is the
 *   expected one.
 * Success condition:
 *   The written representation of the token is correct.
 */
START_TEST(pdf_token_write_name_ns)
{
  pdf_token_t token;

  pdf_init ();

  /* Create the token.  */
  fail_if (pdf_token_name_new ("The_Key_of_F#_Minor", 19, &token)
           != PDF_OK);

  /* Check.  */
  write_and_check (token,
                   0,  /* Flags.  */
                   "/The_Key_of_F#23_Minor", 22, 100);
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
  tcase_add_test (tc, pdf_token_write_string_ht_readable);
  tcase_add_test (tc, pdf_token_write_string_ff);
  tcase_add_test (tc, pdf_token_write_string_ff_readable);
  tcase_add_test (tc, pdf_token_write_string_leftp);
  tcase_add_test (tc, pdf_token_write_string_rightp);
  tcase_add_test (tc, pdf_token_write_string_rs);
  tcase_add_test (tc, pdf_token_write_string_octal);
  tcase_add_test (tc, pdf_token_write_string_octal_readable);
  tcase_add_test (tc, pdf_token_write_string_null);
  tcase_add_test (tc, pdf_token_write_name_nonempty);
  tcase_add_test (tc, pdf_token_write_name_empty);
  tcase_add_test (tc, pdf_token_write_name_nonregular);
  tcase_add_test (tc, pdf_token_write_name_alphanum);
  tcase_add_test (tc, pdf_token_write_name_long);
  tcase_add_test (tc, pdf_token_write_name_various);
  tcase_add_test (tc, pdf_token_write_name_real);
  tcase_add_test (tc, pdf_token_write_name_dollars);
  tcase_add_test (tc, pdf_token_write_name_at);
  tcase_add_test (tc, pdf_token_write_name_dot);
  tcase_add_test (tc, pdf_token_write_name_ns);

  return tc;
}

/* End of pdf-token-write.c */

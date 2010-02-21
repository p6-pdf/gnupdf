/* -*- mode: C -*-
 *
 *       File:         pdf-token-read.c
 *       Date:         Wed Jan 14 05:44:48 2009
 *
 *       GNU PDF Library - Unit tests for pdf_token_read
 *
 */

#include <config.h>

#include <string.h>
#include <check.h>
#include <stdlib.h>
#include <assert.h>

#include <pdf.h>

#define STR_AND_LEN(s) (pdf_char_t*)(s),(sizeof(s)-1)

/* Initialize an in-memory reader stream (pdf_stm_t *stm)
 * with the given string (a char* constant). */
#define INIT_STM_STR(stm,str) do{ \
    fail_unless(PDF_OK==pdf_stm_mem_new( STR_AND_LEN(str),  \
        0 /*cache_size*/, PDF_STM_READ /*mode*/, (stm) ));  \
  }while(0)

#define INIT_TOKR(tokr,stm) do{ fail_unless(     \
  PDF_OK==pdf_token_reader_new((stm),(tokr)) ); }while(0)
#define _EXPECT_TOK(tokr,flags,tokexpr) do{               \
    pdf_token_t _exp_tok;                                 \
    fail_unless(PDF_OK == (tokexpr));                     \
    fail_unless(get_token( (tokr), (flags), _exp_tok ));  \
    pdf_token_destroy(_exp_tok);                          \
  }while(0)
#define EXPECT_VALUELESS(tokr,flags,val) _EXPECT_TOK((tokr),  \
  (flags), pdf_token_valueless_new(val, &_exp_tok) )
#define EXPECT_KEYWORD(tokr,flags,val) _EXPECT_TOK((tokr),    \
  (flags), pdf_token_keyword_new(STR_AND_LEN(val), &_exp_tok) )
#define EXPECT_NAME(tokr,flags,val) _EXPECT_TOK((tokr),       \
  (flags), pdf_token_name_new(STR_AND_LEN(val), &_exp_tok) )
#define EXPECT_INTEGER(tokr,flags,val) _EXPECT_TOK((tokr),    \
  (flags), pdf_token_integer_new(val, &_exp_tok) )
#define EXPECT_REAL(tokr,flags,val) _EXPECT_TOK((tokr),       \
  (flags), pdf_token_real_new(val, &_exp_tok) )
#define EXPECT_STRING(tokr,flags,val) _EXPECT_TOK((tokr),     \
  (flags), pdf_token_string_new(STR_AND_LEN(val), &_exp_tok) )

static const pdf_char_t non_regular_chars[] = {
  /* whitespace chars */
  '\0', '\t', '\n', '\f', '\r', ' ',
  /* delimiters */
  '(', ')', '<', '>', '[', ']', '{', '}', '/', '%'
};

static void *nonnull(void *ptr)
{
  if (ptr) return ptr;

  fprintf(stderr, "error: got unexpected null pointer; aborting");
  abort();
}

/* Read a token; return 1 if it's equal to 'expected', or 0 otherwise. */
static int
get_token(pdf_token_reader_t tokr, pdf_u32_t flags, pdf_token_t expected)
{
  int ret = 0;
  pdf_token_t token = NULL;
  pdf_status_t rv = pdf_token_read(tokr, flags, &token);

  nonnull(expected);
  ret = (rv == PDF_OK) && pdf_token_equal_p(token, expected);

  if (token) pdf_token_destroy(token);
  return ret;
}

static int
tokr_eof(pdf_token_reader_t tokr, pdf_i32_t flags)
{
  pdf_token_t token = NULL;
  pdf_status_t rv = pdf_token_read(tokr, flags, &token);
  if (token) pdf_token_destroy(token);
  return (rv == PDF_EEOF);
}

/* Initializes the array of regular chars. */
static void
init_regular_chars_array (pdf_char_t *regular_chars)
{
  pdf_u32_t  array_idx = 0;
  pdf_u32_t  ch = 0;
  pdf_bool_t is_non_regular_char = PDF_FALSE;
  pdf_u32_t  nreg_idx = 0;

  for (ch = 0; ch < 256; ch++)
    {
      /* check if the current char is non-regular */
      is_non_regular_char = PDF_FALSE;
      for (nreg_idx = 0; nreg_idx < sizeof (non_regular_chars); nreg_idx++)
        {
          if (ch == non_regular_chars[nreg_idx])
            {
              is_non_regular_char = PDF_TRUE;
              break;
            }
        }

      if (!is_non_regular_char)
        {
          assert (array_idx < 256 - sizeof (non_regular_chars));
          regular_chars[array_idx] = (pdf_char_t) ch;
          array_idx++;
        }
    }
}

/* Applies a test function to each char in an array. */
static void
test_char_array (void (*test_fn) (pdf_char_t),
                 const pdf_char_t *array,
                 size_t size)
{
  pdf_char_t ch = '\0';
  pdf_u32_t  idx = 0;

  pdf_init ();
  for (idx = 0; idx < size; idx++)
    {
      ch = array[idx];
      test_fn (ch);
    }
  pdf_finish ();
}

/*
 * Checks if the reader creates a name token
 * from a stream containing the given char escaped.
 */
void
ok_for_escaped_char (pdf_char_t ch)
{
  pdf_char_t         name_buf[2];
  pdf_stm_t          stm;
  pdf_char_t         stream_buf[8];
  pdf_token_reader_t tokr;

  /* Nulls are not allowed inside names.
     PDF spec, section 7.3.5, page 16 */
  if ('\0' == ch)
    return;

  sprintf (stream_buf, "/#%02x", ch);
  INIT_STM_STR (&stm, stream_buf);
  INIT_TOKR (&tokr, stm);

  name_buf[0] = ch;
  name_buf[1] = '\0';
  EXPECT_NAME (tokr, 0, name_buf);

  pdf_token_reader_destroy (tokr);
  pdf_stm_destroy (stm);
}

/*
 * Checks if the reader fails to create a name token
 * for a non-escaped char.
 */
void
fail_for_raw_char (pdf_char_t ch)
{
  pdf_bool_t         ok = PDF_FALSE;
  pdf_status_t       rv;
  pdf_stm_t          stm;
  const pdf_char_t  *str = NULL;
  pdf_char_t         stream_buf[3];
  pdf_token_t        token;
  pdf_token_reader_t tokr;

  stream_buf[0] = '/';
  stream_buf[1] = ch;
  stream_buf[2] = '\0';
  INIT_STM_STR (&stm, stream_buf);
  INIT_TOKR (&tokr, stm);

  rv = pdf_token_read (tokr, 0, &token);
  if (PDF_OK == rv)
    {
      str = pdf_token_get_name_data (token);
      ok = (strlen (str) == 0);
    }
  else
    ok = PDF_TRUE;
  fail_unless (ok);

  if (token)
    pdf_token_destroy (token);

  pdf_token_reader_destroy (tokr);
  pdf_stm_destroy (stm);
}

/*
 * Test: pdf_token_read_toktypes
 * Description:
 *   Read various tokens from an in-memory stream, and check whether they
 *   match the expected values.
 * Success condition:
 *   Each token matches the expected one (according to pdf_token_equal_p),
 *   and no tokens remain afterwards.
 */
START_TEST(pdf_token_read_toktypes)
{
  pdf_stm_t stm;
  pdf_token_reader_t tokr;

  pdf_init();

  INIT_STM_STR(&stm, "abc}{/kw/k#20w%com\n"
      "]1 2.0[>><4142434a4F4>"
      "(str(\\551ng(\0\r)\\60x\\\r\n)>>)"
      "<<1208925819614629174706176"          /* 2^80 */
      "\0\n \r\t\f"                          /* whitespace */
      "(\\n\\r\\t\\b\\f\\)\\(\\\\\\X\08)");  /* string escapes */
  INIT_TOKR(&tokr, stm);

  EXPECT_KEYWORD( tokr, 0, "abc" );
  EXPECT_VALUELESS( tokr, 0, PDF_TOKEN_PROC_END );
  EXPECT_VALUELESS( tokr, 0, PDF_TOKEN_PROC_START );
  EXPECT_NAME( tokr, 0, "kw" );
  EXPECT_NAME( tokr, 0, "k w" );
  EXPECT_VALUELESS( tokr, 0, PDF_TOKEN_ARRAY_END );
  EXPECT_INTEGER( tokr, 0, 1 );
  EXPECT_REAL( tokr, 0, 2 );
  EXPECT_VALUELESS( tokr, 0, PDF_TOKEN_ARRAY_START );
  EXPECT_VALUELESS( tokr, 0, PDF_TOKEN_DICT_END );
  EXPECT_STRING( tokr, 0, "ABCJO@" );
  EXPECT_STRING( tokr, 0, "str(ing(\0\n)0x)>>" );
  EXPECT_VALUELESS( tokr, 0, PDF_TOKEN_DICT_START );
  EXPECT_REAL( tokr, 0, 1208925819614629174706176.0 );  /* 2^80 */
  EXPECT_STRING( tokr, 0, "\n\r\t\b\f)(\\X\0\070" );

  fail_unless( tokr_eof(tokr, 0) );
}
END_TEST

/*
 * Test: pdf_token_read_eos
 * Description:
 *   Test the PDF_TOKEN_END_AT_STREAM flag.
 * Success condition:
 *   The stream should be positioned after the '\n' character (at '<'), and
 *   the token reader should act as if it reached the end of the input file.
 */
START_TEST(pdf_token_read_eos)
{
  pdf_stm_t stm;
  pdf_token_reader_t tokr;
  pdf_char_t ch;

  pdf_init();

  INIT_STM_STR(&stm, "stream    \r\t  %com  \n"
      ">UNPARSABLE DATA IN STREAM\n\n\n\n\n\nx");
  INIT_TOKR(&tokr, stm);

  EXPECT_KEYWORD( tokr, 0, "stream" );
  fail_unless( tokr_eof(tokr, PDF_TOKEN_END_AT_STREAM) );
  pdf_init();

  fail_unless( PDF_OK==pdf_stm_peek_char(stm, &ch) );
  fail_unless( ch == '>' );
  /*TODO: verify stream position */
}
END_TEST

/*
 * Test: pdf_token_read_longstring
 * Description:
 *   Try to read a string longer than the normal buffer size.
 * Success condition:
 *   A token containing the string should be produced.
 */
START_TEST(pdf_token_read_longstring)
{
  const pdf_size_t filesize = 42000;
  pdf_char_t *file;
  pdf_stm_t stm;
  pdf_token_reader_t tokr;
  pdf_token_t token = NULL;
  pdf_status_t rv;
  pdf_size_t i, j;

  pdf_init();

  /* make long string '(XX'...'XX)' */
  file = nonnull(pdf_alloc(filesize));
  memset(file, 'X', filesize);
  file[0] = '(';
  file[filesize-1] = ')';

  fail_unless(PDF_OK == pdf_stm_mem_new( file, filesize,
      0 /*cache_size*/, PDF_STM_READ /*mode*/, &stm ));
  INIT_TOKR(&tokr, stm);

  rv = pdf_token_read(tokr, 0, &token);
  fail_unless(rv == PDF_OK);
  if (token)
    {
      fail_unless(pdf_token_get_type(token) == PDF_TOKEN_STRING);
      fail_unless(pdf_token_get_string_size(token) == filesize-2);
      pdf_token_destroy(token);
    }

  fail_unless( tokr_eof(tokr, 0) );
  pdf_dealloc(file);
}

END_TEST

/*
 * Test: pdf_token_comments
 * Description:
 *   Checks if the token reader treats comments as white-space.
 *   See the PDF specification, page 13, section 7.2.3.
 *   The reader receives some strings containing comments.
 * Success condition:
 *   The tokens should be returned as if no comments existed.
 */
START_TEST (pdf_token_comments)
{
  pdf_stm_t stm;
  pdf_token_reader_t tokr;

  pdf_init ();
  INIT_STM_STR (&stm, "12% foo\n"
      "34");
  INIT_TOKR (&tokr, stm);

  EXPECT_INTEGER (tokr, 0, 12);
  EXPECT_INTEGER (tokr, 0, 34);

  pdf_token_reader_destroy (tokr);
  pdf_stm_destroy (stm);
  pdf_finish ();
}
END_TEST

/*
 * Test: pdf_token_reverse_solidus
 * Description:
 *   Checks if the reader ignores reverse solidus chars which are not
 *   followed by n, r, t, b, f,  (, ), \ or numbers for octal values,
 *   while extracting a string.
 *   See the PDF specification, page 15, section 7.3.4.2, 2nd paragraph.
 *   The test gives the reader a string in which some chars that do not
 *   belong to the list above are preceded by \.
 * Success condition:
 *   The reader should return a string token without the reverse
 *   solidus characters.
 */
START_TEST (pdf_token_reverse_solidus)
{
  pdf_stm_t stm;
  pdf_token_reader_t tokr;

  pdf_init ();
  INIT_STM_STR (&stm, "(1 \\x 2 \\9 3 \\% 4)");
  INIT_TOKR (&tokr, stm);

  EXPECT_STRING (tokr, 0, "1 x 2 9 3 % 4");

  pdf_token_reader_destroy (tokr);
  pdf_stm_destroy (stm);
  pdf_finish ();
}
END_TEST

/*
 * Test: pdf_token_solidus_eol
 * Description:
 *   Checks if the reader recognizes a string with lines terminated
 *   by backslashes as a single-line string.
 *   See the PDF specification, page 15, section 7.3.4.2, 3rd paragraph.
 *   The reader receives a string with newlines preceded by \.
 * Success condition:
 *   The token returned by the reader should be a string
 *   without line breaks.
 */
START_TEST (pdf_token_solidus_eol)
{
  pdf_stm_t stm;
  pdf_token_reader_t tokr;

  pdf_init ();
  INIT_STM_STR (&stm, "(simple \\\n"
    "multi-\\\r"
    "line \\\r\n"
    "string)");
  INIT_TOKR (&tokr, stm);

  EXPECT_STRING (tokr, 0, "simple multi-line string");

  pdf_token_reader_destroy (tokr);
  pdf_stm_destroy (stm);
  pdf_finish ();
}
END_TEST

/*
 * Test: pdf_token_eol
 * Description:
 *   Tests if the reader treats end-of-line markers \r and
 *   \r\n as \n.
 *   See the PDF specification, page 15, section 7.3.4.2, 4th paragraph.
 *   The test passes to the reader a string containing the different
 *   eol markers.
 * Success condition:
 *   The token generated from that string should be equivalent to
 *   a token containing the same string with only \n markers.
 */
START_TEST (pdf_token_eol)
{
  pdf_stm_t stm;
  pdf_token_reader_t tokr;

  pdf_init ();
  INIT_STM_STR (&stm, "(this \n is \r a \r\n string)");
  INIT_TOKR (&tokr, stm);

  EXPECT_STRING (tokr, 0, "this \n is \n a \n string");

  pdf_token_reader_destroy (tokr);
  pdf_stm_destroy (stm);
  pdf_finish ();
}
END_TEST

/*
 * Test: pdf_token_octal_overflow
 * Description:
 *   Tests if the reader ignores the higher-order bits from
 *   octal numbers larger than 255 when they are used to
 *   represent chars inside a string.
 *   See the PDF specification, page 16, 2nd paragraph.
 *   The reader receives a string with octals larger than 255.
 * Success condition:
 *   The generated token should be equal to the one created
 *   from the chars that correspond to the least significant bits
 *   of the octal numbers.
 */
START_TEST (pdf_token_octal_overflow)
{
  pdf_stm_t stm;
  pdf_token_reader_t tokr;

  pdf_init ();
  /* simple string: hello
     octal codes of chars: 150, 145, 154, 154, 157
     codes with added bits: 350, 545, 754, 554, 357
   */
  INIT_STM_STR (&stm, "(\\350\\545\\754\\554\\357)");
  INIT_TOKR (&tokr, stm);

  EXPECT_STRING (tokr, 0, "hello");

  pdf_token_reader_destroy (tokr);
  pdf_stm_destroy (stm);
  pdf_finish ();
}
END_TEST

/*
 * Test: pdf_token_octals
 * Description:
 *   Tests if the reader is able to recognize correctly octals with
 *   different number of digits.
 *   See the PDF specification, page 16, 2nd paragraph.
 *   One string is rewritten with some chars represented as octal numbers
 *   having 2 or 3 digits.
 * Success condition:
 *   The returned string should match the original.
 */
START_TEST (pdf_token_octals)
{
  pdf_stm_t stm;
  pdf_token_reader_t tokr;

  pdf_init ();
  INIT_STM_STR (&stm, "(1\0623\64 \06567\70)");
  INIT_TOKR (&tokr, stm);

  EXPECT_STRING (tokr, 0, "1234 5678");

  pdf_token_reader_destroy (tokr);
  pdf_stm_destroy (stm);
  pdf_finish ();
}
END_TEST

/*
 * Test: pdf_token_hex_string
 * Description:
 *   Tests if the reader ignores white-space chars inside a hexadecimal
 *   string.
 *   See the PDF specification, page 16, section 7.3.4.3, 2nd paragraph.
 *   The test passes to the reader a hex string with various white-space
 *   chars.
 * Success condition:
 *   The reader should ignore the white-spaces.
 */
START_TEST (pdf_token_hex_string)
{
  pdf_stm_t stm;
  pdf_token_reader_t tokr;

  pdf_init ();

  /* h e l l o -> 68 65 6c 6c 6f */
  INIT_STM_STR (&stm, "<68 \t 65 \r 6c \n 6c \f 6f>");
  INIT_TOKR (&tokr, stm);

  EXPECT_STRING (tokr, 0, "hello");

  pdf_token_reader_destroy (tokr);
  pdf_stm_destroy (stm);
  pdf_finish ();
}
END_TEST

/*
 * Test: pdf_token_odd_hex_string
 * Description:
 *   Checks that the reader automatically adds a zero at the end of
 *   a hex string if its number of digits is odd.
 *   See the PDF specification, page 16, section 7.3.4.3, 2nd paragraph.
 *   The test creates an odd hex string from a string that ends with
 *   a char whose last hex digit is zero. Then it sends the odd string
 *   to the reader.
 * Success condition:
 *   The reader should return a token which is equivalent to the
 *   original string.
 */
START_TEST (pdf_token_odd_hex_string)
{
  pdf_stm_t stm;
  pdf_token_reader_t tokr;

  pdf_init ();

  /* 4 3 2 1 0 -> 34 33 32 31 30 -> 343332313 */
  INIT_STM_STR (&stm, "<343332313>");
  INIT_TOKR (&tokr, stm);

  EXPECT_STRING (tokr, 0, "43210");

  pdf_token_reader_destroy (tokr);
  pdf_stm_destroy (stm);
  pdf_finish ();
}
END_TEST

/*
 * Test: pdf_token_number_sign
 * Description:
 *   Checks if the reader recognizes the escaped number sign inside
 *   a name.
 *   See the PDF specification, page 17, item a, at the top.
 *   The reader receives a string containing the sequence "#23".
 * Success condition:
 *   The result should have only the number sign (#).
 */
START_TEST (pdf_token_number_sign)
{
  pdf_stm_t stm;
  pdf_token_reader_t tokr;

  pdf_init ();
  INIT_STM_STR (&stm, "/The_Key_of_F#23_Minor");
  INIT_TOKR (&tokr, stm);

  EXPECT_NAME (tokr, 0, "The_Key_of_F#_Minor");

  pdf_token_reader_destroy (tokr);
  pdf_stm_destroy (stm);
  pdf_finish ();
}
END_TEST

/*
 * Test: pdf_token_regular_char_as_number
 * Description:
 *   Tests if the reader is able to read names with escaped regular chars.
 *   See the PDF specification, page 17, item b, at the top.
 *   The test creates names with all regular chars.
 * Success condition:
 *   All the name tokens should be correctly returned.
 */
START_TEST (pdf_token_regular_char_as_number)
{
  pdf_char_t regular_chars[256 - sizeof (non_regular_chars)];
  init_regular_chars_array (regular_chars);
  test_char_array (&ok_for_escaped_char, regular_chars,
                   sizeof (regular_chars));
}
END_TEST

/*
 * Test: pdf_token_regular_chars_inside_range
 * Description:
 *   Checks if the reader accepts names with regular chars inside the
 *   range 21h (exclamation mark) to 7Eh (tilde).
 *   See the PDF specification, page 17, item b, at the top.
 *   The reader receives names with these chars.
 * Success condition:
 *   The name tokens should be returned.
 */
void test_ok_inside_range (pdf_char_t ch)
{
  const pdf_char_t first_range = 0x21;
  const pdf_char_t last_range = 0x7E;

  /* this always should be followed by a number */
  if ('#' == ch)
    return;

  if (first_range <= ch && ch <= last_range)
    {
      pdf_char_t         name_buf[2];
      pdf_stm_t          stm;
      pdf_char_t         stream_buf[8];
      pdf_token_reader_t tokr;

      sprintf (stream_buf, "/%c", ch);
      INIT_STM_STR (&stm, stream_buf);
      INIT_TOKR (&tokr, stm);

      name_buf[0] = ch;
      name_buf[1] = '\0';
      EXPECT_NAME (tokr, 0, name_buf);

      pdf_token_reader_destroy (tokr);
      pdf_stm_destroy (stm);
    }
}

START_TEST (pdf_token_regular_chars_inside_range)
{
  pdf_char_t regular_chars[256 - sizeof (non_regular_chars)];
  init_regular_chars_array (regular_chars);
  test_char_array (&test_ok_inside_range, regular_chars,
                   sizeof (regular_chars));
}
END_TEST

/*
 * Test: pdf_token_non_regular_chars
 * Description:
 *   Tests if the reader handles white-spaces and delimiters inside
 *   names correctly,
 *   See the PDF specification, page 17, item c, at the top.
 *   The reader is given names with escaped and raw chars.
 * Success condition:
 *   The names with escaped chars should be accepted. The ones
 *   with raw chars should be rejected.
 */
START_TEST (pdf_token_non_regular_chars)
{
  /* verify that tokens that contain non-regular chars written
     as numbers are valid */
  test_char_array (&ok_for_escaped_char, non_regular_chars,
                   sizeof (non_regular_chars));

  /* verify that tokens that contain non-regular chars written
     directly are invalid */
  test_char_array (&fail_for_raw_char, non_regular_chars,
                   sizeof (non_regular_chars));
}
END_TEST

/*
 * Test: pdf_token_regular_chars_outside_range
 * Description:
 *   Checks if the reader rejects names with regular chars outside the
 *   range 21h (exclamation mark) to 7Eh (tilde).
 *   See the PDF specification, page 17.
 *   The reader receives names with these chars.
 * Success condition:
 *   The names should be rejected.
 */
void test_fail_outside_range (pdf_char_t ch)
{
  const pdf_char_t first_range = 0x21;
  const pdf_char_t last_range = 0x7E;

  if (ch < first_range || last_range < ch)
    fail_for_raw_char (ch);
}

START_TEST (pdf_token_regular_chars_outside_range)
{
  pdf_char_t regular_chars[256 - sizeof (non_regular_chars)];
  init_regular_chars_array (regular_chars);
  test_char_array (&test_fail_outside_range, regular_chars,
                   sizeof (regular_chars));
}
END_TEST

/*
 * Test: pdf_token_empty_name
 * Description:
 *   Checks if the reader accepts a single solidus char as a name.
 *   See the PDF specification, page 17.
 *   The reader receives one string containing a solidus.
 * Success condition:
 *   A name token containing an empty string should be returned.
 */
START_TEST (pdf_token_empty_name)
{
  pdf_stm_t stm;
  pdf_token_reader_t tokr;

  pdf_init ();
  INIT_STM_STR (&stm, "/");
  INIT_TOKR (&tokr, stm);

  EXPECT_NAME (tokr, 0, "");

  pdf_token_reader_destroy (tokr);
  pdf_stm_destroy (stm);
  pdf_finish ();
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_token_read (void)
{
  TCase *tc = tcase_create("pdf_token_read");
  tcase_add_test(tc, pdf_token_read_toktypes);
  tcase_add_test(tc, pdf_token_read_eos);
  tcase_add_test(tc, pdf_token_read_longstring);

  tcase_add_test (tc, pdf_token_comments);
  tcase_add_test (tc, pdf_token_reverse_solidus);
  tcase_add_test (tc, pdf_token_solidus_eol);
  tcase_add_test (tc, pdf_token_eol);
  tcase_add_test (tc, pdf_token_octals);
  tcase_add_test (tc, pdf_token_hex_string);
  tcase_add_test (tc, pdf_token_odd_hex_string);
  tcase_add_test (tc, pdf_token_number_sign);
  tcase_add_test (tc, pdf_token_regular_char_as_number);
  tcase_add_test (tc, pdf_token_regular_chars_inside_range);
  tcase_add_test (tc, pdf_token_non_regular_chars);
  tcase_add_test (tc, pdf_token_regular_chars_outside_range);
  tcase_add_test (tc, pdf_token_empty_name);

  return tc;
}

/* End of pdf-token-read.c */

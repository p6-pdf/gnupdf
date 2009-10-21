/* -*- mode: C -*- Time-stamp: "2009-10-21 06:43:46 mgold"
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
 * Test case creation function
 */
TCase *
test_pdf_token_read (void)
{
  TCase *tc = tcase_create("pdf_token_read");
  tcase_add_test(tc, pdf_token_read_toktypes);
  tcase_add_test(tc, pdf_token_read_eos);
  tcase_add_test(tc, pdf_token_read_longstring);

  return tc;
}

/* End of pdf-token-read.c */

/* -*- mode: C -*- Time-stamp: "2009-01-14 05:44:48 mgold"
 *
 *       File:         pdf-tokeniser-read.c
 *       Date:         Wed Jan 14 05:44:48 2009
 *
 *       GNU PDF Library - Unit tests for pdf_tokeniser_read
 *
 */

#include <string.h>
#include <check.h>
#include <stdlib.h>
#include "config.h"
#include "pdf.h"

#define STR_AND_LEN(s) (s),(sizeof(s)-1)

/* Initialize an in-memory reader stream (pdf_stm_t *stm)
 * with the given string (a char* constant). */
#define INIT_STM_STR(stm,str) do{ \
    fail_unless(PDF_OK==pdf_stm_mem_new( STR_AND_LEN(str),  \
        0 /*cache_size*/, PDF_STM_READ /*mode*/, (stm) ));  \
  }while(0)

#define INIT_TOKR(tokr,stm) do{ fail_unless(     \
  PDF_OK==pdf_tokeniser_new((stm),(tokr)) ); }while(0)
#define _EXPECT_TOK(tokr,tokexpr) do{            \
    pdf_obj_t _exp_tok;                          \
    fail_unless(PDF_OK == (tokexpr));            \
    fail_unless(get_token( (tokr), _exp_tok ));  \
    pdf_obj_destroy(_exp_tok);                   \
  }while(0)
#define EXPECT_VALUELESS(tokr,val) _EXPECT_TOK((tokr),  \
  pdf_tok_valueless_new(val, &_exp_tok) )
#define EXPECT_KEYWORD(tokr,val) _EXPECT_TOK((tokr),    \
  pdf_tok_keyword_new(STR_AND_LEN(val), &_exp_tok) )
#define EXPECT_NAME(tokr,val) _EXPECT_TOK((tokr),       \
  pdf_obj_name_new(STR_AND_LEN(val), &_exp_tok) )
#define EXPECT_INTEGER(tokr,val) _EXPECT_TOK((tokr),    \
  pdf_obj_integer_new(val, &_exp_tok) )
#define EXPECT_REAL(tokr,val) _EXPECT_TOK((tokr),       \
  pdf_obj_real_new(val, &_exp_tok) )
#define EXPECT_STRING(tokr,val) _EXPECT_TOK((tokr),     \
  pdf_obj_string_new(STR_AND_LEN(val), &_exp_tok) )

static void *nonnull(void *ptr)
{
  if (ptr) return ptr;

  fprintf(stderr, "error: got unexpected null pointer; aborting");
  abort();
}

/* Read a token; return 1 if it's equal to 'expected', or 0 otherwise. */
static int
get_token(pdf_tokeniser_t tokr, pdf_obj_t expected)
{
  int ret = 0;
  pdf_obj_t token = NULL;
  pdf_status_t rv = pdf_tokeniser_read(tokr, &token);

  nonnull(expected);
  ret = (rv == PDF_OK) && pdf_obj_equal_p(token, expected);

  if (token) pdf_obj_destroy(token);
  return ret;
}

static int
tokr_eof(pdf_tokeniser_t tokr)
{
  pdf_obj_t token = NULL;
  pdf_status_t rv = pdf_tokeniser_read(tokr, &token);
if(token)
{
printf("tok type %d\n", pdf_obj_type(token));
}
else if (rv != PDF_EEOF)
{
printf("eof rv %d\n", rv);
}
  if (token) pdf_obj_destroy(token);
  return (rv == PDF_EEOF);
}

/*
 * Test: pdf_tokeniser_read_001
 * Description:
 *   Read various tokens from an in-memory stream, and check whether they
 *   match the expected values.
 * Success condition:
 *   Each token matches the expected one (according to pdf_obj_equal_p),
 *   and no tokens remain afterwards.
 */
START_TEST(pdf_tokeniser_read_001)
{
  pdf_stm_t stm;
  pdf_tokeniser_t tokr;

  INIT_STM_STR(&stm, "abc}{/kw/k#20w%com\n"
      "]1 2.0[>><4142434a4F4>"
      "(str(\\551ng(\0\r)\\60x\\\r\n)>>)"
      "<<1208925819614629174706176"          /* 2^80 */
      "\0\n \r\t\f"                          /* whitespace */
      "(\\n\\r\\t\\b\\f\\)\\(\\\\\\X\08)");  /* string escapes */
  INIT_TOKR(&tokr, stm);

  EXPECT_KEYWORD( tokr, "abc" );
  EXPECT_VALUELESS( tokr, PDF_PROC_END_TOK );
  EXPECT_VALUELESS( tokr, PDF_PROC_START_TOK );
  EXPECT_NAME( tokr, "kw" );
  EXPECT_NAME( tokr, "k w" );
  EXPECT_VALUELESS( tokr, PDF_ARRAY_END_TOK );
  EXPECT_INTEGER( tokr, 1 );
  EXPECT_REAL( tokr, 2 );
  EXPECT_VALUELESS( tokr, PDF_ARRAY_START_TOK );
  EXPECT_VALUELESS( tokr, PDF_DICT_END_TOK );
  EXPECT_STRING( tokr, "ABCJO@" );
  EXPECT_STRING( tokr, "str(ing(\0\n)0x)>>" );
  EXPECT_VALUELESS( tokr, PDF_DICT_START_TOK );
  EXPECT_REAL( tokr, 1208925819614629174706176.0 );  /* 2^80 */
  EXPECT_STRING( tokr, "\n\r\t\b\f)(\\X\0\070" );

  fail_unless( tokr_eof(tokr) );
}
END_TEST

/*
 * Test: pdf_tokeniser_read_002
 * Description:
 TODO
 * Success condition:
 TODO
 */
START_TEST(pdf_tokeniser_read_002)
{
  pdf_stm_t stm;
  pdf_tokeniser_t tokr;
  pdf_char_t ch;

  INIT_STM_STR(&stm, "stream    \n\t  %com  \r"
      ">UNPARSABLE DATA IN STREAM\n\n\n\n\n\nx");
  INIT_TOKR(&tokr, stm);

  EXPECT_KEYWORD( tokr, "stream" );
  fail_unless( PDF_OK==pdf_tokeniser_end_at_stream(tokr) );
  fail_unless( tokr_eof(tokr) );
  fail_unless( PDF_OK==pdf_stm_peek_char(stm, &ch) );
  fail_unless( ch == '>' );
  /*TODO: verify stream position */
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_tokeniser_read (void)
{
  TCase *tc = tcase_create("pdf_tokeniser_read");
  tcase_add_test(tc, pdf_tokeniser_read_001);
  tcase_add_test(tc, pdf_tokeniser_read_002);

  return tc;
}

/* End of pdf-tokeniser-read.c */

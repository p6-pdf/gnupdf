#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "pdf-rd-tokeniser.h"
#include "pdf-obj.h"
#include "pdf-error.h"

void print_token(pdf_obj_t token)
{
  char str[256];
  char *typ;
  int show_buf = 0, sz;
  strcpy(str, "(no text)");
  switch(token->type)
    {
    case PDF_COMMENT_TOK:
      typ = "COMMENT";
      show_buf = 1;
      break;
    case PDF_KEYWORD_TOK:
      typ = "KEYWORD";
      show_buf = 1;
      break;
    case PDF_INT_OBJ:
      typ = "INTEGER";
      snprintf(str, sizeof(str), "%d", token->value.integer);
      str[sizeof(str)-1]=0;
      break;
    case PDF_REAL_OBJ:
      typ = "REAL";
      snprintf(str, sizeof(str), "%f", token->value.real);
      str[sizeof(str)-1]=0;
      break;
    case PDF_NAME_OBJ:
      typ = "NAME";
      show_buf = 1;
      break;
    case PDF_STRING_OBJ:
      typ = "STRING";
      show_buf = 1;
      break;
    case PDF_DICT_START_TOK:
      typ = "DICT_START";
      break;
    case PDF_DICT_END_TOK:
      typ = "DICT_END";
      break;
    case PDF_ARRAY_START_TOK:
      typ = "ARRAY_START";
      break;
    case PDF_ARRAY_END_TOK:
      typ = "ARRAY_END";
      break;
    case PDF_PROC_START_TOK:
      typ = "PROC_START";
      break;
    case PDF_PROC_END_TOK:
      typ = "PROC_END";
      break;
    default:
      typ = "(unknown)";
    }

  if (show_buf)
    {
      int sz = token->value.buffer.size;
      char *buf = malloc(sz + 1);
      if (!buf) abort();
      memcpy(buf, token->value.buffer.data, sz);
      buf[sz] = '\0';
      printf("token %s(%d): %s\n", typ, sz, buf);
      free(buf);
    }
  else
    {
      printf("token %s: %s\n", typ, str);
    }
};

void print_file(FILE *file)
{
  pdf_status_t rv;
  pdf_tokeniser_t tokeniser;
  pdf_obj_t token;
  pdf_stm_t stm;

  rv = pdf_stm_cfile_new (file, 0, 0 /*cache_size*/, PDF_STM_READ, &stm);
  if (rv != PDF_OK)
    {
      fprintf(stderr, "failed to create stream\n");
      return;
    }

  rv = pdf_tokeniser_new(stm, &tokeniser);
  if (rv != PDF_OK)
    {
      fprintf(stderr, "failed to create tokeniser\n");
      return;
    }

  while (( rv = pdf_tokeniser_read(tokeniser, &token) ) == PDF_OK)
    print_token(token);

  if (rv != PDF_EEOF)
    {
      fprintf(stderr, "read_token error %d\n", rv);
      return;
    }

  fprintf(stderr, "done\n");
}

int main(int argc, char **argv)
{
  print_file(stdin);
  return 0;
}

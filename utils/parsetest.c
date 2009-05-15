#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "pdf-rd-parser.h"
#include "pdf-obj.h"
#include "pdf-error.h"
#include "gl_list.h"

void print_file(FILE *file)
{
  pdf_status_t rv;
  pdf_parser_t parser = NULL;
  pdf_obj_t stack;
  pdf_stm_t stm = NULL;

  if (file == NULL)
    {
      fprintf(stderr, "file==NULL\n");
      return;
    }

  rv = pdf_stm_cfile_new (file, 0, 0 /*cache_size*/, PDF_STM_READ, &stm);
  if (rv != PDF_OK)
    {
      fprintf(stderr, "failed to create stream\n");
      goto out;
    }

  rv = pdf_parser_new(stm, &parser);
  if (rv != PDF_OK)
    {
      fprintf(stderr, "failed to create parser\n");
      goto out;
    }

  while (( rv = pdf_parser_read_to_command(parser, &stack) ) == PDF_OK)
    {
      pdf_size_t stacksize = pdf_obj_array_size(stack);
      pdf_obj_t cmdobj = NULL;
      int atstream;

      print_obj(stack);

      rv = pdf_obj_array_get(stack, stacksize-1, &cmdobj);
      assert (rv == PDF_OK);
      assert (cmdobj && pdf_obj_type(cmdobj) == PDF_KEYWORD_TOK);

      /* We can't easily read a stream's length since the value may be an
       * indirect object, so we'll stop at the first stream. */
      atstream = pdf_tok_keyword_equal_p(cmdobj, "stream");

      rv = pdf_obj_array_clear(stack);
      assert (rv == PDF_OK);

      if (atstream)
        {
          printf("stopping at stream keyword\n");
          goto out;
        }
    }

  if (rv != PDF_EEOF)
    {
      fprintf(stderr, "parser read error %d\n", rv);
      goto out;
    }

  fprintf(stderr, "done\n");
out:
  if (parser) pdf_parser_destroy(parser);
  if (stm) pdf_stm_destroy(stm);
}

int main(int argc, char **argv)
{
  setlocale(LC_ALL, "");
  //print_file(stdin);

  FILE *file = fopen("/home/michael/howtoread-pdf.pdf", "rb");
  print_file(file);
  if (file) fclose(file);
  return 0;
}

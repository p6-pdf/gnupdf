#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "pdf-rd-parser.h"
#include "pdf-obj.h"
#include "pdf-error.h"

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
      print_obj(stack);
      rv = pdf_obj_array_clear(stack);
      assert (rv == PDF_OK);
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

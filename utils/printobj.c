#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "pdf-rd-parser.h"
#include "pdf-obj.h"
#include "pdf-error.h"

static char *fmtbin(const char *data, int size)
{
  int i;
  char *ret, *retp;

  if (size == -1)
    size = strlen(data);

  ret = malloc(3 + (size*4));
  if (!ret) abort();

  retp = ret;
  (*retp++) = '"';
  for (i = 0; i < size; ++i)
    {
      unsigned char ch = data[i];
      if (ch == '"' || ch == '\\')
        {
          *(retp++) = '\\';
          *(retp++) = ch;
        }
      else if (data[i] >= 32 && data[i] < 127)
        *(retp++) = ch;
      else
        {
          sprintf(retp, "\\x%02x", (int)ch);
          retp += 4;
        }
    }
  (*retp++) = '"';
  (*retp++) = '\0';
  return ret;
}

static void print_obj_priv(pdf_obj_t obj, int indent)
{
  char tabbuf[10], tmpbuf[256];
  const char *typ, *str = NULL;
  int strsize = -1, recurse = 0;
  pdf_size_t i;

  switch(pdf_obj_type(obj))
    {
    case PDF_NULL_OBJ:
      typ = "NULL";

    case PDF_BOOLEAN_OBJ:
      typ = "BOOLEAN";
      str = pdf_obj_bool_value(obj) ? "true" : "false";
      break;

    case PDF_INT_OBJ:
      typ = "INT";
      snprintf(tmpbuf, sizeof(tmpbuf), "%d", pdf_obj_int_value(obj));
      str = tmpbuf;
      break;

    case PDF_REAL_OBJ:
      typ = "REAL";
      snprintf(tmpbuf, sizeof(tmpbuf), "%f", pdf_obj_real_value(obj));
      str = tmpbuf;
      break;

    case PDF_STRING_OBJ:
      typ = "STRING";
      str = pdf_obj_string_data(obj);
      strsize = pdf_obj_string_size(obj);
      break;

    case PDF_NAME_OBJ:
      typ = "NAME";
      str = pdf_obj_string_data(obj);
      strsize = pdf_obj_name_size(obj);
      break;

    case PDF_ARRAY_OBJ:
      typ = "ARRAY";
      recurse = 1;
      break;

    case PDF_DICT_OBJ:
      typ = "DICT";
      recurse = 2;
      break;

    case PDF_INDIRECT_OBJ:
      typ = "INDIRECT";
      snprintf(tmpbuf, sizeof(tmpbuf), "%d,%d",
               pdf_obj_indirect_on(obj),
               pdf_obj_indirect_gn(obj) );
      str = tmpbuf;
      break;

    case PDF_STREAM_OBJ:
      typ = "STREAM";
      snprintf(tmpbuf, sizeof(tmpbuf), "%d,",
               pdf_obj_stream_data(obj) );
      str = tmpbuf;

      /* print the dictionary too */
      recurse = 2;
      obj = pdf_obj_stream_dict(obj);
      break;

    case PDF_COMMENT_TOK:
      typ = "COMMENT";
      str = pdf_tok_comment_data(obj);
      strsize = pdf_tok_comment_size(obj);
      break;

    case PDF_KEYWORD_TOK:
      typ = "KEYWORD";
      str = pdf_tok_keyword_data(obj);
      strsize = pdf_tok_keyword_size(obj);
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
      typ = "[unknown]";
      sprintf(tmpbuf, "%d", pdf_obj_type(obj));
      str = tmpbuf;
    }

  for (i = 0; i < indent && i < sizeof(tabbuf)-1; ++i)
    tabbuf[i] = '\t';
  tabbuf[i] = '\0';

  if (recurse)
    {
      char *op = (recurse==1) ? "[" : "<<";
      char *ed = (recurse==1) ? "]" : ">>";
      char *ext = str ? str : "";

      printf("%s%s(%s%s\n", tabbuf, typ, ext, op);
      if (recurse == 1)
        {
          pdf_size_t size = pdf_obj_array_size(obj);
          for (i = 0; i < size; ++i)
            {
              pdf_obj_t child;
              pdf_obj_array_get(obj, i, &child);
              print_obj_priv(child, indent+1);
            }
        }
      else
        {
          printf("%s....TODO [size %d]\n", tabbuf, pdf_obj_dict_size(obj));
        }
      printf("%s%s)\n", tabbuf, ed);
    }
  else
    {
      if (str == NULL)
        {
          tmpbuf[0] = '\0';
          str = tmpbuf;
        }
      if (str != tmpbuf) str = fmtbin(str, strsize);
      printf("%s%s(%s)\n", tabbuf, typ, str);
      if (str != tmpbuf)
      {
      free((void*)str);
      }
    }

};

void print_obj(pdf_obj_t obj)
{
  print_obj_priv(obj, 0);
}

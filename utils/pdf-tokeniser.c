/* -*- mode: C -*- Time-stamp: "2009-10-23 06:12:29 mgold"
 *
 *       File:         pdf-tokeniser.c
 *       Date:         Wed May 20 05:25:40 2009
 *
 *       GNU PDF Library - Read standard input using a token reader,
 *                         and print the resulting tokens to stdout.
 */

/* Copyright (C) 2009 Free Software Foundation, Inc. */

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

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <pdf.h>

#include <pdf-tokeniser.h>

/*
 * Command line options management
 */

static const struct option GNU_longOptions[] =
  {
    {"help", no_argument, NULL, HELP_ARG},
    {"usage", no_argument, NULL, USAGE_ARG},
    {"version", no_argument, NULL, VERSION_ARG},
    {"token-writer", no_argument, NULL, TOKW_ARG},
    {"reader-flags", required_argument, NULL, READER_FLAGS_ARG},
    {"writer-flags", required_argument, NULL, WRITER_FLAGS_ARG},
    {NULL, 0, NULL, 0}
  };

/* Messages */

char *pdf_tokeniser_version_msg = "pdf_tokeniser 0.1";

char *pdf_tokeniser_usage_msg = "\
Usage: pdf_tokeniser [OPTIONS]\n\
Read standard input using a token reader, and print the resulting tokens \n\
to stdout.\n\
\n\
available options\n\
  --help                              print a help message and exit\n\
  --usage                             print a usage message and exit\n\
  --version                           show pdf-tokeniser version and exit\n\
  --token-writer                      generate output using the token writer\n\
  --reader-flags=INTEGER              specify token reader flags\n\
  --writer-flags=INTEGER              specify token writer flags\n\
";

char *pdf_tokeniser_help_msg = "";

static char *
fmtbin (const char *data, int size)
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

static void
print_tok (pdf_token_t token)
{
  char tmpbuf[256];
  const char *typ, *str = NULL;
  int strsize = -1;
  pdf_size_t i;

  switch(pdf_token_get_type(token))
    {
    case PDF_TOKEN_INTEGER:
      typ = "INTEGER";
      snprintf(tmpbuf, sizeof(tmpbuf), "%d", pdf_token_get_integer_value(token));
      str = tmpbuf;
      break;

    case PDF_TOKEN_REAL:
      typ = "REAL";
      snprintf(tmpbuf, sizeof(tmpbuf), "%f", pdf_token_get_real_value(token));
      str = tmpbuf;
      break;

    case PDF_TOKEN_STRING:
      typ = "STRING";
      str = pdf_token_get_string_data(token);
      strsize = pdf_token_get_string_size(token);
      break;

    case PDF_TOKEN_NAME:
      typ = "NAME";
      str = pdf_token_get_name_data(token);
      strsize = pdf_token_get_name_size(token);
      break;

    case PDF_TOKEN_COMMENT:
      typ = "COMMENT";
      str = pdf_token_get_comment_data(token);
      strsize = pdf_token_get_comment_size(token);
      break;

    case PDF_TOKEN_KEYWORD:
      typ = "KEYWORD";
      str = pdf_token_get_keyword_data(token);
      strsize = pdf_token_get_keyword_size(token);
      break;

    case PDF_TOKEN_DICT_START:
      typ = "DICT_START";
      break;
    case PDF_TOKEN_DICT_END:
      typ = "DICT_END";
      break;
    case PDF_TOKEN_ARRAY_START:
      typ = "ARRAY_START";
      break;
    case PDF_TOKEN_ARRAY_END:
      typ = "ARRAY_END";
      break;
    case PDF_TOKEN_PROC_START:
      typ = "PROC_START";
      break;
    case PDF_TOKEN_PROC_END:
      typ = "PROC_END";
      break;

    default:
      typ = "[unknown]";
      sprintf(tmpbuf, "%d", pdf_token_get_type(token));
      str = tmpbuf;
    }

  if (str == NULL)
    {
      tmpbuf[0] = '\0';
      str = tmpbuf;
    }
  if (str != tmpbuf) str = fmtbin(str, strsize);
  printf("%s(%s)\n", typ, str);
  if (str != tmpbuf)
    {
      free((char *) str);
    }
};

void
print_file (FILE *file, pdf_bool_t use_tokw,
            pdf_u32_t reader_flags, pdf_u32_t writer_flags)
{
  pdf_status_t rv;
  pdf_token_reader_t reader = NULL;
  pdf_token_writer_t writer = NULL;
  pdf_token_t token;
  pdf_stm_t stm_in = NULL;
  pdf_stm_t stm_out = NULL;

  rv = pdf_stm_cfile_new (file, 0, 0 /*cache_size*/, PDF_STM_READ, &stm_in);
  if (rv != PDF_OK)
    {
      fprintf(stderr, "failed to create input stream\n");
      goto out;
    }

  rv = pdf_token_reader_new(stm_in, &reader);
  if (rv != PDF_OK)
    {
      fprintf(stderr, "failed to create reader\n");
      goto out;
    }

  if (use_tokw)
    {
      rv = pdf_stm_cfile_new (stdout, 0, 0 /*cache_size*/,
                              PDF_STM_WRITE, &stm_out);
      if (rv != PDF_OK)
        {
          fprintf(stderr, "failed to create output stream\n");
          goto out;
        }

      rv = pdf_token_writer_new(stm_out, &writer);
      if (rv != PDF_OK)
        {
          fprintf(stderr, "failed to create writer\n");
          goto out;
        }
    }

  while (( rv = pdf_token_read(reader, reader_flags, &token) ) == PDF_OK)
    {
      if (use_tokw)
        {
          rv = pdf_token_write(writer, writer_flags, token);
          if (rv != PDF_OK)
            {
              fprintf(stderr, "pdf_token_write error %d\n", rv);
              goto out;
            }
        }
      else
        print_tok(token);

      pdf_token_destroy(token);
    }

  if (rv != PDF_EEOF)
    {
      fprintf(stderr, "read_token error %d\n", rv);
      goto out;
    }

  fprintf(stderr, "done\n");
out:
  if (writer) pdf_token_writer_destroy(writer);
  if (stm_out) pdf_stm_destroy(stm_out);
  if (reader) pdf_token_reader_destroy(reader);
  if (stm_in) pdf_stm_destroy(stm_in);
}

int
main (int argc, char **argv)
{
  char c;
  pdf_bool_t use_tokw = PDF_FALSE;
  pdf_u32_t reader_flags = 0, writer_flags = 0;

  /*  set_program_name (argv[0]); */

  while ((c = getopt_long (argc,
                           argv,
                           "",
                           GNU_longOptions,
                           NULL)) != -1)
    {
      switch (c)
        {
          /* COMMON ARGUMENTS */
        case HELP_ARG:
          {
            fprintf (stdout, "%s\n", pdf_tokeniser_usage_msg);
            exit (0);
            break;
          }
        case USAGE_ARG:
          {
            fprintf (stdout, "%s\n", pdf_tokeniser_usage_msg);
            exit (0);
            break;
          }
        case VERSION_ARG:
          {
            fprintf (stdout, "%s\n", pdf_tokeniser_version_msg);
            exit (0);
            break;
          }
        case TOKW_ARG:
          {
            use_tokw = 1;
            break;
          }
        case READER_FLAGS_ARG:
          {
            reader_flags = atoi(optarg);
            break;
          }
        case WRITER_FLAGS_ARG:
          {
            writer_flags = atoi(optarg);
            break;
          }
        default:
          {
            break;
          }
        }
    }

  setlocale(LC_ALL, "");
  print_file(stdin, use_tokw, reader_flags, writer_flags);
  return 0;
}

/* -*- mode: C -*-
 *
 *       File:         pdf-tokeniser.c
 *       Date:         Wed May 20 05:25:40 2009
 *
 *       GNU PDF Library - Read standard input using a token reader,
 *                         and print the resulting tokens to stdout.
 */

/* Copyright (C) 2009-2011 Free Software Foundation, Inc. */

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
#include <errno.h>
#include <xalloc.h>

#include <pdf.h>
#include <pdf-utils.h>

/*
 * Global variables
 */

char *program_name; /* Initialized in main() */

/*
 * Command line options management
 */

enum
{
  HELP_ARG,
  VERSION_ARG,
  TOKW_ARG,
  READER_FLAGS_ARG,
  WRITER_FLAGS_ARG
};

static const struct option GNU_longOptions[] =
  {
    {"help", no_argument, NULL, HELP_ARG},
    {"version", no_argument, NULL, VERSION_ARG},
    {"token-writer", no_argument, NULL, TOKW_ARG},
    {"reader-flags", required_argument, NULL, READER_FLAGS_ARG},
    {"writer-flags", required_argument, NULL, WRITER_FLAGS_ARG},
    {NULL, 0, NULL, 0}
  };

/* Messages */

PDF_UTILS_COPYRIGHT_DOC ("pdf-tokeniser");

char *pdf_tokeniser_help_msg = "\
Usage: pdf_tokeniser [OPTIONS]\n\
Read standard input using a token reader, and print the resulting tokens \n\
to stdout.\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
  --help                              print a help message and exit\n\
  --usage                             print a usage message and exit\n\
  --version                           show pdf-tokeniser version and exit\n\
  --token-writer                      generate output using the token writer\n\
  --reader-flags=INTEGER              specify token reader flags\n\
  --writer-flags=INTEGER              specify token writer flags\n\
\n"
  PDF_UTILS_HELP_FOOTER_DOC ("pdf-tokeniser");

static char *
fmtbin (const char *data, int size)
{
  int i;
  char *ret, *retp;

  if (size == -1)
    size = strlen (data);

  ret = malloc (3 + (size * 4));
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
print_token (pdf_token_t *token)
{
  char tmpbuf[256];
  const pdf_char_t *typ;
  char *str = NULL;
  int strsize = -1;
  pdf_size_t i;

  switch (pdf_token_get_type (token))
    {
    case PDF_TOKEN_INTEGER:
      typ = "INTEGER";
      snprintf (tmpbuf, sizeof (tmpbuf), "%d", pdf_token_get_integer_value (token));
      str = xstrdup (tmpbuf);
      break;

    case PDF_TOKEN_REAL:
      typ = "REAL";
      snprintf (tmpbuf, sizeof(tmpbuf), "%f", pdf_token_get_real_value (token));
      str = xstrdup (tmpbuf);
      break;

    case PDF_TOKEN_STRING:
      typ = "STRING";
      str = xstrdup (pdf_token_get_string_data (token));
      strsize = pdf_token_get_string_size (token);
      break;

    case PDF_TOKEN_NAME:
      typ = "NAME";
      str = xstrdup (pdf_token_get_name_data (token));
      strsize = pdf_token_get_name_size (token);
      break;

    case PDF_TOKEN_COMMENT:
      typ = "COMMENT";
      str = xstrdup (pdf_token_get_comment_data (token));
      strsize = pdf_token_get_comment_size (token);
      break;

    case PDF_TOKEN_KEYWORD:
      typ = "KEYWORD";
      str = xstrdup (pdf_token_get_keyword_data (token));
      strsize = pdf_token_get_keyword_size (token);
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
      sprintf (tmpbuf, "%d", pdf_token_get_type (token));
      str = xstrdup (tmpbuf);
    }

  if (str == NULL)
    {
      tmpbuf[0] = '\0';
      str = tmpbuf;
    }

  if (str != tmpbuf)
    str = fmtbin (str, strsize);

  printf ("%s(%s)\n", typ, str);
  free (str);
};

void
print_file (FILE       *file,
            pdf_bool_t  use_tokw,
            pdf_u32_t   reader_flags,
            pdf_u32_t   writer_flags)
{
  pdf_token_reader_t *reader;
  pdf_token_writer_t *writer;
  pdf_token_t *token;
  pdf_stm_t *stm_in;
  pdf_stm_t *stm_out;
  pdf_error_t *error = NULL;

  stm_in = pdf_stm_cfile_new (file,
                              0,
                              0 /*cache_size*/,
                              PDF_STM_READ,
                              &error);
  if (!stm_in)
    {
      fprintf (stderr,
               "failed to create input stream: %s\n",
               error ? pdf_error_get_message (error) : "unknown error");
      goto out;
    }

  reader = pdf_token_reader_new (stm_in, &error);
  if (!reader)
    {
      fprintf (stderr,
               "failed to create reader: '%s'\n",
               error ? pdf_error_get_message (error) : "unknown error");
      goto out;
    }

  if (use_tokw)
    {
      stm_out = pdf_stm_cfile_new (stdout,
                                   0,
                                   0 /*cache_size*/,
                                   PDF_STM_WRITE,
                                   &error);
      if (!stm_out)
        {
          fprintf (stderr,
                   "failed to create output stream: %s\n",
                   pdf_error_get_message (error));
          goto out;
        }

      writer = pdf_token_writer_new (stm_out, &error);
      if (!writer)
        {
          fprintf (stderr,
                   "failed to create writer: %s\n",
                   error ? pdf_error_get_message (error) : "unknown error");
          goto out;
        }
    }

  while ((token = pdf_token_read (reader, reader_flags, &error)) != NULL)
    {
      if (writer)
        {
          if (!pdf_token_write (writer, writer_flags, token, &error))
            {
              fprintf (stderr,
                       "pdf_token_write error: %s\n",
                       error ? pdf_error_get_message (error) : "unknown error");
              goto out;
            }
        }
      else
        print_token (token);

      pdf_token_destroy (token);
    }

  if (error)
    {
      fprintf (stderr,
               "error reading token: '%s'\n",
               error ? pdf_error_get_message (error) : "unknown error");
      goto out;
    }

  fprintf (stderr, "done\n");

out:
  if (error)
    pdf_error_destroy (error);
  if (writer)
    pdf_token_writer_destroy (writer);
  if (stm_out)
    pdf_stm_destroy (stm_out);
  if (reader)
    pdf_token_reader_destroy (reader);
  if (stm_in)
    pdf_stm_destroy (stm_in);
}

static pdf_u32_t
parse_u32_arg (const char *argvalue,
               const char *argname,
               const char *appname)
{
  char *end;
  pdf_u32_t ret;
  unsigned long int tmp;

  errno = 0;
  tmp = strtoul (argvalue, &end, 0);
  ret = (pdf_u32_t)tmp;
  if (errno || *end != '\0' || tmp != ret)
    {
      fprintf (stderr, "%s: invalid argument `%s' for `--%s'\n",
               appname, argvalue, argname);
      exit (EXIT_FAILURE);
    }
  return ret;
}

int
main (int argc, char **argv)
{
  char c;
  pdf_bool_t use_tokw = PDF_FALSE;
  pdf_u32_t reader_flags = 0, writer_flags = 0;

  set_program_name (argv[0]);

#if defined HAVE_SETLOCALE
  /* Initialize locale in the program */
  setlocale (LC_ALL, "");
#endif /* HAVE_SETLOCALE */

  pdf_init ();

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
            fprintf (stdout, "%s\n", pdf_tokeniser_help_msg);
            exit (EXIT_SUCCESS);
            break;
          }

        case VERSION_ARG:
          {
            fprintf (stdout, "%s\n", pdf_utils_version_msg);
            exit (EXIT_SUCCESS);
            break;
          }

        case TOKW_ARG:
          {
            use_tokw = 1;
            break;
          }

        case READER_FLAGS_ARG:
          {
            reader_flags = parse_u32_arg (optarg, "reader-flags", argv[0]);
            break;
          }

        case WRITER_FLAGS_ARG:
          {
            writer_flags = parse_u32_arg (optarg, "writer-flags", argv[0]);
            break;
          }

        default:
          {
            break;
          }
        }
    }

  print_file (stdin, use_tokw, reader_flags, writer_flags);

  return EXIT_SUCCESS;
}

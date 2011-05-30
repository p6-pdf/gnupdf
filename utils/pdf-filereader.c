/* -*- mode: C -*-
 *
 *       File:         pdf-filereader.c
 *       Date:         Thu Dec 2 23:35:55 2010
 *
 *       GNU PDF Library - Process input/output with the
 *                         standard PDF Filesystem interface.
 *
 */

/* Copyright (C) 2010-2011 Free Software Foundation, Inc. */

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
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>

#include <pdf.h>

/*
 * Command line options management
 */

enum
{
  /* Args with short options */
  PROPS_FILE_ARG  = 'p',
  COPY_FILE_ARG   = 'c',
  LIST_ARG        = 'l',
  INPUT_FILE_ARG  = 'i',
  OUTPUT_FILE_ARG = 'o',
  /* Args without short options */
  FSYS_ARG        = 100,
  HELP_ARG,
  VERSION_ARG,
};

static const char *short_options = "pcli:o:";
static const struct option long_options[] =
  {
    { "help",        no_argument,       NULL, HELP_ARG        },
    { "version",     no_argument,       NULL, VERSION_ARG     },
    { "props-file",  no_argument,       NULL, PROPS_FILE_ARG  },
    { "copy-file",   no_argument,       NULL, COPY_FILE_ARG   },
    { "list",        no_argument,       NULL, LIST_ARG        },
    { "input-file",  required_argument, NULL, INPUT_FILE_ARG  },
    { "output-file", required_argument, NULL, OUTPUT_FILE_ARG },
    { "fsys",        required_argument, NULL, FSYS_ARG        },
    { NULL,          0,                 NULL,  0              }
  };

/* Messages */

const char *pdf_filereader_version_msg = "pdf-filereader 0.1";

const char *pdf_filereader_usage_msg = "\
\n\
Usage: pdf-filereader [ACTION] [OPTIONS]\n\
\n\
actions:\n\
  -p, --props-file                    print input file properties\n\
  -c, --copy-file                     read an input file and copy its contents\n\
                                       into an output file.\n\
  -l, --list                          list supported filesystems\n\
\n\
options:\n\
  -i FILE, --input-file=FILE          use a given file as the input\n\
                                       (applies to --copy-file and --props-file).\n\
  -o FILE, --output-file=FILE         use a given file as the output\n\
                                       (applies to --copy-file).\n\
  --fsys=FSYS                         read using the specified filesystem backend,\n\
                                       instead of the default Disk filesystem\n\
                                       (applies to --copy-file).\n\
\n\
  --help                              print a help message and exit\n\
  --version                           show pdf-filter version and exit\n\
";

/*
 * Data types:
 */

struct pdf_filereader_args_s {
  const char *program_name;

  pdf_bool_t action_copy_file;
  pdf_bool_t action_props_file;
  pdf_bool_t action_list;

  pdf_text_t *input_file;
  pdf_char_t *input_file_printable;
  pdf_text_t *output_file;
  pdf_char_t *output_file_printable;
  pdf_bool_t print_props;
  const pdf_fsys_t *fsys;
  const char *fsys_id;
};

/*
 * Global variables
 */

static struct pdf_filereader_args_s reader_args;

/*
 * Function Prototypes:
 */

static void parse_args (int argc, char *argv[]);

void printout_fsys_props (struct pdf_fsys_item_props_s *p_props);


/*
 * Function Implementations
 */

static void
parse_args (int argc, char *argv[])
{
  int c;

  reader_args.program_name = argv[0];
  reader_args.fsys_id = PDF_FSYS_DISK_ID;
  reader_args.fsys = pdf_fsys_get (PDF_FSYS_DISK_ID);

  while ((c = getopt_long (argc,
                           argv,
                           short_options,
                           long_options,
                           NULL)) != -1)
    {
      pdf_error_t *error = NULL;

      switch (c)
        {
          /* Common */
        case HELP_ARG:
          {
            fprintf (stdout, "%s\n", pdf_filereader_usage_msg);
            exit (EXIT_SUCCESS);
            break;
          }
        case VERSION_ARG:
          {
            fprintf (stdout, "%s\n", pdf_filereader_version_msg);
            exit (EXIT_SUCCESS);
            break;
          }

          /* Actions */
        case PROPS_FILE_ARG:
          {
            reader_args.action_props_file = PDF_TRUE;
            break;
          }
        case COPY_FILE_ARG:
          {
            reader_args.action_copy_file = PDF_TRUE;
            break;
          }
        case LIST_ARG:
          {
            reader_args.action_list = PDF_TRUE;
            break;
          }

          /* Options */
        case INPUT_FILE_ARG:
          {
            reader_args.input_file =
              pdf_text_new_from_host (optarg,
                                      strlen (optarg),
                                      pdf_text_get_host_encoding (),
                                      &error);
            if (!reader_args.input_file)
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "couldn't create host-encoded path: %s",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            reader_args.input_file_printable =
              pdf_text_get_unicode (reader_args.input_file,
                                    PDF_TEXT_UTF8,
                                    PDF_TEXT_UNICODE_WITH_NUL_SUFFIX,
                                    NULL,
                                    &error);
            if (!reader_args.input_file_printable)
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "couldn't create printable path: %s",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            break;
          }
        case OUTPUT_FILE_ARG:
          {
            reader_args.output_file =
              pdf_text_new_from_host (optarg,
                                      strlen (optarg),
                                      pdf_text_get_host_encoding (),
                                      &error);
            if (!reader_args.output_file)
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "couldn't create host-encoded path: %s",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            reader_args.output_file_printable =
              pdf_text_get_unicode (reader_args.output_file,
                                    PDF_TEXT_UTF8,
                                    PDF_TEXT_UNICODE_WITH_NUL_SUFFIX,
                                    NULL,
                                    &error);
            if (!reader_args.output_file_printable)
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "couldn't create printable path: %s",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            break;
          }
        case FSYS_ARG:
          {
            reader_args.fsys_id = optarg;
            reader_args.fsys = pdf_fsys_get (reader_args.fsys_id);
            if (!reader_args.fsys)
              {
                pdf_error (PDF_EBADDATA,
                           stderr,
                           "unsupported filesystem given: %s",
                           reader_args.fsys_id);
                exit (EXIT_FAILURE);
              }
            break;
          }
        }
    } /* end of while getopt_long */

}

static void
foreach_print_fsys_id (void                    *user_data,
                       const pdf_char_t        *id,
                       const struct pdf_fsys_s *implementation)
{
  printf ("\t%s\n", id);
}

static void
action_list (void)
{
  printf ("Available filesystems:\n");
  pdf_fsys_foreach (foreach_print_fsys_id, NULL);
  printf ("\n");
}

static void
action_props_file (void)
{
  pdf_error_t *error = NULL;
  struct pdf_fsys_item_props_s props;
  pdf_char_t *aux;

  /* Ensure required arguments are present */
  if (!reader_args.input_file)
    {
      pdf_error (PDF_EBADDATA,
                 stderr,
                 "listing file properties action (with --props-file) needs an "
                 "input file as argument (with --input-file)");
      exit (EXIT_FAILURE);
    }

  if (!pdf_fsys_get_item_props (reader_args.fsys,
                                reader_args.input_file,
                                &props,
                                &error))
    {
      pdf_error (pdf_error_get_status (error),
                 stderr,
                 "couldn't get item '%s' properties: %s",
                 reader_args.input_file_printable,
                 pdf_error_get_message (error));
      exit (EXIT_FAILURE);
    }

  /* Print item properties */
  printf ("Properties of item '%s' (%s filesystem):\n",
          reader_args.input_file_printable,
          reader_args.fsys_id);

  printf ("\thidden:       %s\n", props.is_hidden ? "TRUE" : "FALSE");
  printf ("\treadable:     %s\n", props.is_readable ? "TRUE" : "FALSE");
  printf ("\twritable:     %s\n", props.is_writable ? "TRUE" : "FALSE");

  aux = pdf_time_to_string (&props.creation_date,
                            PDF_TIME_STRING_FORMAT_PDF,
                            PDF_TIME_STRING_TRAILING_APOSTROPHE,
                            NULL);
  printf ("\tcreation:     %s\n", aux ? aux : "???");
  pdf_dealloc (aux);

  aux = pdf_time_to_string (&props.modification_date,
                            PDF_TIME_STRING_FORMAT_PDF,
                            PDF_TIME_STRING_TRAILING_APOSTROPHE,
                            NULL);
  printf ("\tmodification: %s\n", aux ? aux : "???");
  pdf_dealloc (aux);

  printf ("\tfile size:    %ld\n", (long)props.file_size);
  printf ("\tfolder size:  %u\n", (unsigned)props.folder_size);

  printf ("\n");
}

static void
action_copy_file (void)
{
  pdf_error_t *error = NULL;
  pdf_fsys_file_t *ifile;
  pdf_fsys_file_t *ofile;
  pdf_bool_t eof = PDF_FALSE;
#define BUFFER_SIZE 1024
  pdf_char_t buffer [BUFFER_SIZE];

  /* Ensure required arguments are present */
  if (!reader_args.input_file)
    {
      pdf_error (PDF_EBADDATA,
                 stderr,
                 "copying file action (with --copy-file) needs an "
                 "input file as argument (with --input-file)");
      exit (EXIT_FAILURE);
    }
  if (!reader_args.output_file)
    {
      pdf_error (PDF_EBADDATA,
                 stderr,
                 "copying file action (with --copy-file) needs an "
                 "output file as argument (with --output-file)");
      exit (EXIT_FAILURE);
    }

  /* Open the input file for reading */
  ifile = pdf_fsys_file_open (reader_args.fsys,
                              reader_args.input_file,
                              PDF_FSYS_OPEN_MODE_READ,
                              &error);
  if (!ifile)
    {
      pdf_error (pdf_error_get_status (error),
                 stderr,
                 "couldn't open file '%s' for reading: %s",
                 reader_args.input_file_printable,
                 pdf_error_get_message (error));
      exit (EXIT_FAILURE);
    }

  /* Open the output file for writing */
  ofile = pdf_fsys_file_open (reader_args.fsys,
                              reader_args.output_file,
                              PDF_FSYS_OPEN_MODE_WRITE,
                              &error);
  if (!ofile)
    {
      pdf_error (pdf_error_get_status (error),
                 stderr,
                 "couldn't open file '%s' for writing: %s",
                 reader_args.output_file_printable,
                 pdf_error_get_message (error));
      exit (EXIT_FAILURE);
    }

  printf ("Copying from '%s' to '%s'...\n",
          reader_args.input_file_printable,
          reader_args.output_file_printable);

  while (!eof)
    {
      pdf_size_t read_bytes = 0;

      if (!pdf_fsys_file_read (ifile,
                               buffer,
                               BUFFER_SIZE,
                               &read_bytes,
                               &error))
        {
          if (!error)
            eof = PDF_TRUE;
          else
            {
              pdf_error (pdf_error_get_status (error),
                         stderr,
                         "couldn't read from file '%s': %s",
                         reader_args.input_file_printable,
                         pdf_error_get_message (error));
              exit (EXIT_FAILURE);
            }
        }

      if (read_bytes > 0)
        {
          pdf_size_t written_bytes = 0;

          if (!pdf_fsys_file_write (ofile,
                                    buffer,
                                    read_bytes,
                                    &written_bytes,
                                    &error) &&
              error)
            {
              pdf_error (pdf_error_get_status (error),
                         stderr,
                         "couldn't write to file '%s': %s",
                         reader_args.output_file_printable,
                         pdf_error_get_message (error));
              exit (EXIT_FAILURE);
            }
          printf ("\tcopied %lu bytes...\n",
                  (unsigned long)read_bytes);
        }
    }

  pdf_fsys_file_close (ifile, NULL);
  pdf_fsys_file_close (ofile, NULL);
}

int
main (int argc, char *argv[])
{
#if defined HAVE_SETLOCALE
  /* Initialize locale in the program */
  setlocale (LC_ALL, "");
#endif /* HAVE_SETLOCALE */

  pdf_init ();

  /* Initialize context */
  parse_args (argc, argv);

  /* Go run actions. Multiple actions, even if independent are allowed */

  if (reader_args.action_list)
    action_list ();

  if (reader_args.action_props_file)
    action_props_file ();

  if (reader_args.action_copy_file)
    action_copy_file ();

  pdf_finish ();
}

/* End of pdf-filereader.c */

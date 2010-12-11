/* -*- mode: C -*- Time-stamp: "2010-12-11 19:34:35 jemarch"
 *
 *       File:         pdf-filereader.c
 *       Date:         Thu Dec 2 23:35:55 2010
 *
 *       GNU PDF Library - Process input/output with the
 *                         standard PDF Filesystem interface.
 *
 */

/* Copyright (C) 2010 Free Software Foundation, Inc. */

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
  HELP_ARG,
  USAGE_ARG,
  VERSION_ARG,
  INFILE_ARG,
  OUTFILE_ARG,
  PROPS_ARG,
  DISK_ARG,
};

static const struct option GNU_longOptions[] =
  {
    {"help", no_argument, NULL, HELP_ARG},
    {"usage", no_argument, NULL, USAGE_ARG},
    {"version", no_argument, NULL, VERSION_ARG},
    {"input-file", required_argument, NULL, INFILE_ARG},
    {"output-file", required_argument, NULL, OUTFILE_ARG},
    {"print-props", no_argument, NULL, PROPS_ARG},
    {"fsys-disk", no_argument, NULL, DISK_ARG},
    {NULL, 0, NULL, 0}
  };

/* Messages */

char *pdf_filereader_version_msg = "pdf_filereader 0.1";

char *pdf_filereader_usage_msg = "\
Usage: pdf_filereader [OPTIONS]\n\
Read file given with input-file argument and write to file given by \n\
output-file argument.\n\
\n\
mandatory options:\n\
\n\
  -i FILE, --input-file=FILE          Use a given file as the input.\n\
  -o FILE, --output-file=FILE         Use a given file as the output.\n\
\n\
available options:\n\
\n\
  --print-props                       print file properties\n\
  --fsys-disk                         read using fsys-disk instead of fsys-http\n\
                                      (default=fsys-http if fsys-http present.\n\
                                      default=fsys-disk fsys-http absent)\n\
\n\
  --help                              print a help message and exit\n\
  --usage                             print a usage message and exit\n\
  --version                           show pdf-filter version and exit\n\
";

char *pdf_filereader_help_msg = "try --usage";

/*
 * Data types:
 */

struct pdf_filereader_args_s {
  char * program_name;
  char * in_fname;
  char * out_fname;
  pdf_bool_t print_props;
  pdf_bool_t use_disk_fs;
};

typedef struct pdf_filereader_args_s pdf_filereader_args_t;

/*
 * Global variables
 */

pdf_filereader_args_t reader_args;

/*
 * Function Prototypes:
 */

void
parse_args (int argc, char *argv[]);

void 
printout_fsys_props (struct pdf_fsys_item_props_s *p_props);

int
main (int argc, char *argv[]);


/*
 * Function Implementations
 */

#define BUF_LEN 1024

int
main (int argc, char *argv[])
{
  pdf_status_t ret_stat;
  pdf_text_t input_path = NULL;
  int rv = 0;
  struct pdf_fsys_item_props_s path_props;
  pdf_fsys_t use_this_fsys = NULL;
  pdf_fsys_file_t the_file = NULL;
  pdf_size_t len;
  pdf_char_t buff[BUF_LEN];

  FILE *f = NULL;

  /* Initialized global vars */
  reader_args.program_name = strdup (argv[0]);
  reader_args.in_fname = NULL;
  reader_args.out_fname = NULL;
  reader_args.print_props = PDF_FALSE;
  reader_args.use_disk_fs = PDF_FALSE;

  ret_stat = pdf_text_init ();
  
  if (PDF_OK != ret_stat)
    {
      fprintf (stderr, "ERROR: pdf_text_init failed: %d\n", ret_stat);
    }

  parse_args (argc, argv);

  if (PDF_OK == ret_stat)
    {
#if PDF_FSYS_HTTP
      if (reader_args.use_disk_fs)
#endif
        { 
          use_this_fsys = pdf_fsys_create (pdf_fsys_disk_implementation);
        }
#if PDF_FSYS_HTTP
      else /* Use HTTP fs (default) */
        {
          use_this_fsys = pdf_fsys_create (pdf_fsys_http_implementation);
        }
#endif
      if (NULL == use_this_fsys)
        {
          ret_stat = PDF_ERROR;
          fprintf (stderr, "ERROR: Failed to create pdf-fsys.\n");
        }
    }

  if ((PDF_OK == ret_stat)
      && (NULL != reader_args.in_fname)
      && (NULL != reader_args.out_fname))
    { /* Input and Output filename given. Proceed. */

      /* Convert in_fname to pdf_text_t */

      ret_stat = pdf_text_new_from_host ( (pdf_char_t *)reader_args.in_fname,
                                          strlen (reader_args.in_fname),
                                          pdf_text_get_host_encoding (),
                                          &input_path);

      if (PDF_OK == ret_stat) 
        {
          if (NULL == input_path)
            {
              ret_stat = PDF_ERROR;
              rv = PDF_ERROR;
              fprintf (stderr, "\nERROR: NULL input_path!\n");
            }
          else
            {
              memset (&path_props, 0, sizeof (struct pdf_fsys_item_props_s));

              ret_stat = pdf_fsys_get_item_props (use_this_fsys, input_path, 
                                                  &path_props);
              if (PDF_OK != ret_stat)
                {
                  if (PDF_EBADNAME == ret_stat)
                    {
                      fprintf (stderr, "\nERROR: invalid path name given.\n");
                    }
                  else
                    {
                      fprintf (stderr, "\nERROR: get_item_props returned %d\n",
                               ret_stat);
                    }
                }
            }
        }
      else
        {
          fprintf (stderr, "\nERROR: pdf_text_new_from_host: %d\n", ret_stat);
          rv = ret_stat;
        }

      if ((PDF_OK == ret_stat) && (reader_args.print_props))
        {
          printout_fsys_props (&path_props);
        }

      /* get item props succeeded - now open file! */
      
      ret_stat = pdf_fsys_file_open (use_this_fsys, input_path, 
                                     PDF_FSYS_OPEN_MODE_READ, &the_file);

      if (PDF_OK == ret_stat)
        {

          f = fopen (reader_args.out_fname, "w");

          do 
            {
              
              len = 0;
              
              ret_stat = pdf_fsys_file_read (the_file, buff, BUF_LEN, &len);
              
              if ((PDF_OK == ret_stat) 
                  || ((PDF_EEOF == ret_stat) && (0 != len)))
                {
                  fwrite (buff, 1, len, f);
                }
              else
                {
                  fprintf (stderr, "\nERROR: file_read: %d/%d -- len:%d\n", 
                           ret_stat, PDF_EEOF, len);
                  rv = ret_stat;
                }

            } while (PDF_OK == ret_stat);
          
          fclose (f);
          pdf_fsys_file_close (the_file);

        }
      else
        {
          fprintf (stderr, "\nERROR: pdf_fsys_file_open: %d\n", ret_stat);
          rv = ret_stat;
        }

    }
  else
    { /* Print usage message */
      fprintf (stdout, "%s\n", pdf_filereader_usage_msg);      
    }

  return rv;
}

void
parse_args (int argc, char *argv[])
{
  pdf_status_t ret;
  pdf_bool_t finish;
  char c;


  finish = PDF_FALSE;


  while (!finish &&
	 (ret = getopt_long (argc,
			     argv,
			     "i:o:",
			     GNU_longOptions, 
			     NULL)) != -1)
    {
      c = ret;
      switch (c)
        {
	  /* COMMON ARGUMENTS */
        case HELP_ARG:
          {
            fprintf (stdout, "%s\n", pdf_filereader_usage_msg);
            exit (0);
            break;
          }
        case VERSION_ARG:
          {
            fprintf (stdout, "%s\n", pdf_filereader_version_msg);
            exit (0);
            break;
          }
        case USAGE_ARG:
          {
            fprintf (stdout, "%s\n", pdf_filereader_usage_msg);
            exit (0);
            break;
          }
        case INFILE_ARG:
        case 'i':
          {
            reader_args.in_fname = pdf_alloc (strlen (optarg) + 1 );
            strcpy (reader_args.in_fname, optarg);
            break;
          }
        case OUTFILE_ARG:
        case 'o':
          {
            reader_args.out_fname = pdf_alloc (strlen (optarg) + 1);
            strcpy (reader_args.out_fname, optarg);
            break;
          }
        case PROPS_ARG:
          {
            reader_args.print_props = PDF_TRUE;
            break;
          }
        case DISK_ARG:
          {
            reader_args.use_disk_fs = PDF_TRUE;
            break;
          }
        }

    } /* end of while getopt_long */
  
}

void 
printout_fsys_props (struct pdf_fsys_item_props_s *p_props)
{
/* Filesystem item properties */
/*
struct pdf_fsys_item_props_s
{
  pdf_bool_t is_hidden;
  pdf_bool_t is_readable;
  pdf_bool_t is_writable;
  pdf_time_t creation_date;
  pdf_time_t modification_date;
  pdf_u32_t file_size_high;
  pdf_u32_t file_size_low;
  pdf_u32_t folder_size;
};
*/
    if (NULL != p_props)
      {
        fprintf (stderr, "is_hidden: %s\n", 
                 p_props->is_hidden ? "TRUE" : "FALSE");

        fprintf (stderr, "is_readable: %s\n", 
                 p_props->is_readable ? "TRUE" : "FALSE");

        fprintf (stderr, "is_writable: %s\n", 
                 p_props->is_writable ? "TRUE" : "FALSE");

        fprintf (stderr, "creation_date: Not displayed\n");
        
        fprintf (stderr, "modification_date: Not displayed\n");
        
        fprintf (stderr, "file_size_high: %d\n", p_props->file_size_high);

        fprintf (stderr, "file_size_low: %d\n", p_props->file_size_low);

        fprintf (stderr, "folder_size: %d\n", p_props->folder_size);

      }

    return;
}

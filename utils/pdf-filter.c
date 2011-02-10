/* -*- mode: C -*-
 *
 *       File:         pdf-filter.c
 *       Date:         Tue Jul 10 18:42:07 2007
 *
 *       GNU PDF Library - Process input/output with the
 *                         standard PDF Filters.
 *
 */

/* Copyright (C) 2007, 2008 Free Software Foundation, Inc. */

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

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>

#ifdef HAVE_MALLOC_H
#  include <malloc.h>
#else
#  include <stdlib.h>
#endif /* HAVE_MALLOC_H */

#include <sys/stat.h>

#include <xalloc.h>
#include <pdf.h>

#include <pdf-utils.h>

/*
 * Global variables
 */

char *program_name;  /* Initialized in main() */

/*
 * Command line options management
 */

enum
{
  HELP_ARG,
  VERSION_ARG,
  READ_ARG,
  INFILE_ARG,
  OUTFILE_ARG,
  CACHE_ARG,
  NULL_FILTER_ARG,
  ASCIIHEXDEC_FILTER_ARG,
  ASCIIHEXENC_FILTER_ARG,
  ASCII85DEC_FILTER_ARG,
  ASCII85ENC_FILTER_ARG,
#if 0
  CCITTFAXDEC_FILTER_ARG,
  JXPDEC_FILTER_ARG,
  PREDENC_FILTER_ARG,
  PREDDEC_FILTER_ARG,
#endif /* 0 */
#ifdef HAVE_LIBJPEG
  DCTDEC_FILTER_ARG,
#endif /* HAVE_LIBJPEG */
#ifdef HAVE_LIBZ
  FLATEDEC_FILTER_ARG,
  FLATEENC_FILTER_ARG,
#endif /* HAVE_LIBZ */
  RUNLENGTHDEC_FILTER_ARG,
  RUNLENGTHENC_FILTER_ARG,
#ifdef HAVE_LIBJBIG2DEC
  JBIG2DEC_FILTER_ARG,
  JBIG2DEC_GLOBAL_SEGMENTS_ARG,
  JBIG2DEC_PAGE_SIZE,
#endif /* HAVE_LIBJBIG2DEC */
  LZWENC_FILTER_ARG,
  LZWDEC_FILTER_ARG,
  LZW_EARLYCHANGE_ARG,
  MD5ENC_FILTER_ARG,
  KEY_ARG,
  AESENC_FILTER_ARG,
  AESDEC_FILTER_ARG,
  V2ENC_FILTER_ARG,
  V2DEC_FILTER_ARG
};

static const struct option GNU_longOptions[] =
  {
    {"help", no_argument, NULL, HELP_ARG},
    {"version", no_argument, NULL, VERSION_ARG},
    {"readmode", no_argument, NULL, READ_ARG},
    {"input-file", required_argument, NULL, INFILE_ARG},
    {"output-file", required_argument, NULL, OUTFILE_ARG},
    {"cache", required_argument, NULL, CACHE_ARG},
    {"null", no_argument, NULL, NULL_FILTER_ARG},
    {"ahexdec", no_argument, NULL, ASCIIHEXDEC_FILTER_ARG},
    {"ahexenc", no_argument, NULL, ASCIIHEXENC_FILTER_ARG},
    {"lzwenc", no_argument, NULL, LZWENC_FILTER_ARG},
    {"lzwdec", no_argument, NULL, LZWDEC_FILTER_ARG},
    {"lzw-earlychange", no_argument, NULL, LZW_EARLYCHANGE_ARG},
    {"a85dec", no_argument, NULL, ASCII85DEC_FILTER_ARG},
    {"a85enc", no_argument, NULL, ASCII85ENC_FILTER_ARG},
#if 0
    {"cfaxdec", no_argument, NULL, CCITTFAXDEC_FILTER_ARG},
    {"jxpdec", no_argument, NULL, JXPDEC_FILTER_ARG},
    {"predenc", no_argument, NULL, PREDENC_FILTER_ARG},
    {"preddec", no_argument, NULL, PREDDEC_FILTER_ARG},
#endif /* 0 */
#ifdef PDF_HAVE_LIBJPEG
    {"dctdec", no_argument, NULL, DCTDEC_FILTER_ARG},
#endif /* PDF_HAVE_LIBJPEG */
#ifdef PDF_HAVE_LIBZ
    {"flatedec", no_argument, NULL, FLATEDEC_FILTER_ARG},
    {"flateenc", no_argument, NULL, FLATEENC_FILTER_ARG},
#endif /* PDF_HAVE_LIBZ */
    {"rldec", no_argument, NULL, RUNLENGTHDEC_FILTER_ARG},
    {"rlenc", no_argument, NULL, RUNLENGTHENC_FILTER_ARG},
#ifdef PDF_HAVE_LIBJBIG2DEC
    {"jbig2dec", no_argument, NULL, JBIG2DEC_FILTER_ARG},
    {"jbig2dec-globals", required_argument, NULL, JBIG2DEC_GLOBAL_SEGMENTS_ARG},
#endif /* PDF_HAVE_LIBJBIG2DEC */
    {"md5enc", no_argument, NULL, MD5ENC_FILTER_ARG},
    {"key", required_argument, NULL, KEY_ARG},
    {"aesenc", no_argument, NULL, AESENC_FILTER_ARG},
    {"aesdec", no_argument, NULL, AESDEC_FILTER_ARG},
    {"v2enc", no_argument, NULL, V2ENC_FILTER_ARG},
    {"v2dec", no_argument, NULL, V2DEC_FILTER_ARG},
    {NULL, 0, NULL, 0}
  };

/* Messages */

PDF_UTILS_COPYRIGHT_DOC ("pdf-filter");

char *pdf_filter_help_msg = "\
Usage: pdf_filter [[OPTIONS] [FILTER FILTER_ARGS]...]\n\
Filter the standard input with the specified PDF standard filters and \n\
write the result in the standard output.\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
  --readmode                          test the stream in read mode instead\n\
                                       of write mode.\n\
  -i FILE, --input-file=FILE          Use a given file as the input.\n\
  -o FILE, --output-file=FILE         Use a given file as the output.\n\
  --cache=NUM                         set the stream cache size.\n\n\
filters\n\
  --null                              use the NULL filter\n\
  --ahexdec                           use the ASCII Hex decoder filter\n\
  --ahexenc                           use the ASCII Hex encoder filter\n\
  --lzwenc                            use the LZW encoder filter\n\
  --lzwdec                            use the LZW decoder filter\n\
  --a85dec                            use the ASCII 85 decoder filter\n\
  --a85enc                            use the ASCII 85 encoder filter\n"
#if 0
"\
  --jxpdec                            use the JXP decoder filter\n\
  --predenc                           use the predictor encoder filter\n\
  --preddec                           use the predictor decoder filter\n"
#endif /* 0 */
#ifdef PDF_HAVE_LIBJPEG
"\
  --dctdec                            use the DCT decoder filter\n"
#endif /* PDF_HAVE_LIBZ */
#ifdef PDF_HAVE_LIBZ
"\
  --flatedec                          use the Flate decoder filter\n\
  --flateenc                          use the Flate encoder filter\n"
#endif /* PDF_HAVE_LIBZ */
"\
  --rldec                             use the Run Length decoder filter\n\
  --rlenc                             use the Run Length encoder filter\n\
  --cfaxdec                           use the CCITT Fax decoder filter\n\
  --jbig2dec                          use the JBIG2 decoder filter\n\
  --md5enc                            use the MD5 encoder filter\n\
  --aesenc                            use the AESv2 encoder filter\n\
  --aesdec                            use the AESv2 decoder filter\n\
  --v2enc                             use the V2 encoder filter\n\
  --v2dec                             use the V2 decoder filter\n\
  --help                              print a help message and exit\n\
  --version                           print a version message and exit\n\
\n\
Filter properties\n"
#if 0
"\
  --preddec-type=NUM                  code for next preddec filters type\n\
  --predenc-type=NUM                  code for next predenc filters type\n\
  --pred-colors=NUM                   next predictors colors per sample\n\
  --pred-bpc=NUM                      next predictors bits per color component\n\
  --pred-columns=NUM                  next predictors number of samples per row\n"
#endif /* 0 */
"\
  --lzw-earlychange                   toggles earlychange for next lzw filters\n\
  --jbig2dec-globals=FILE             file containing global segments\n\
\n"
  PDF_UTILS_HELP_FOOTER_DOC ("pdf-filter");

static pdf_stm_t create_stream (int         argc,
                                char       *argv[],
                                pdf_bool_t *mode,
                                int        *last_ci,
                                pdf_bool_t *read_pdf_fsys,
                                pdf_bool_t *write_pdf_fsys,
                                pdf_stm_t  *fsys_stm);

static void install_filters (int        argc,
                             char      *argv[],
                             pdf_stm_t  stm,
                             int        ci);

static void process_stream (pdf_stm_t  stm,
                            pdf_bool_t read_mode,
                            pdf_bool_t read_pdf_fsys,
                            pdf_bool_t write_pdf_fsys,
                            pdf_stm_t  fsys_stm);

static void open_file (pdf_char_t                *name,
                       pdf_fsys_file_t           *file,
                       enum pdf_fsys_file_mode_e  mode);

int
main (int argc, char *argv[])
{
  pdf_stm_t stm,fsys_stm;
  pdf_bool_t read_mode,read_pdf_fsys,write_pdf_fsys;
  int last_ci;
  pdf_status_t destroy_ret;

  set_program_name (argv[0]);

  stm = create_stream (argc, argv, &read_mode, &last_ci, &read_pdf_fsys,
                       &write_pdf_fsys, &fsys_stm);
  install_filters (argc, argv, stm, last_ci);
  process_stream (stm, read_mode, read_pdf_fsys,write_pdf_fsys, fsys_stm);
  destroy_ret = pdf_stm_destroy (stm);
  if ((destroy_ret != PDF_OK) && (destroy_ret != PDF_EEOF))
    {
      /* Only writing streams report errors on pdf_stm_destroy */
      pdf_error (destroy_ret, stderr, "writing to stream");
      exit (EXIT_FAILURE);
    }

  if (read_pdf_fsys || write_pdf_fsys)
    {
      destroy_ret = pdf_stm_destroy (fsys_stm);
      if (destroy_ret != PDF_OK)
        {
          /* Only writing streams report errors on pdf_stm_destroy */
          pdf_error (destroy_ret, stderr, "writing to stream");
          exit (EXIT_FAILURE);
        }
    }

  return 0;
}

static void process_stream (pdf_stm_t  stm,
                            pdf_bool_t read_mode,
                            pdf_bool_t read_pdf_fsys,
                            pdf_bool_t write_pdf_fsys,
                            pdf_stm_t  fsys_stm)
{
#define BUF_SIZE 256

  pdf_status_t ret;
  pdf_size_t read_bytes;
  pdf_size_t written_bytes;
  pdf_char_t buf [BUF_SIZE];

  if (read_mode)
    {
      /* Read from the buffer which will process anything on stdin
         and push to stdout */
      do
        {
          ret = pdf_stm_read (stm, buf, BUF_SIZE, &read_bytes);
          if ((ret != PDF_OK) && (ret != PDF_EEOF))
            {
              pdf_error (ret, stderr, "reading from stream");
              exit (EXIT_FAILURE);
            }

          if (write_pdf_fsys)
            {
              ret = pdf_stm_write (fsys_stm, buf, read_bytes, &written_bytes);
              if (ret != PDF_OK)
                {
                  pdf_error (ret, stderr, "writing to stream");
                  exit (EXIT_FAILURE);
                }
            }
          else
            {
              if(fwrite (buf, 1, read_bytes, stdout) != read_bytes)
                {
                  fprintf(stderr,"fwrite failed (%ld)", (long)read_bytes);
                }
            }
        }
      while (read_bytes == BUF_SIZE);
    }
  else
    {
      /* Write stdin into the write stream,
	 which will be transparently writting the output to stdout. */
      do
        {
          if (read_pdf_fsys)
            {
              ret = pdf_stm_read (fsys_stm, buf, BUF_SIZE, &read_bytes);
              if ((ret != PDF_OK) && (ret != PDF_EEOF))
                {
                  pdf_error (ret, stderr, "reading from stream");
                  exit (EXIT_FAILURE);
                }

            }
          else
            {
              read_bytes = fread (buf, 1, BUF_SIZE, stdin);
            }
          ret = pdf_stm_write (stm, buf, read_bytes, &written_bytes);
          if (ret != PDF_OK)
            {
              pdf_error (ret, stderr, "writing to stream");
              exit (EXIT_FAILURE);
            }

        }
      while (read_bytes == BUF_SIZE);
    }

#undef BUF_SIZE
}

static pdf_stm_t
create_stream (int         argc,
               char       *argv[],
               pdf_bool_t *read_mode,
               int        *last_ci,
               pdf_bool_t *read_pdf_fsys,
               pdf_bool_t *write_pdf_fsys,
               pdf_stm_t  *fsys_stm)
{
  int ci;
  char c;
  pdf_status_t ret;
  pdf_size_t cache_size;
  pdf_stm_t stm;
  pdf_bool_t finish;
  pdf_char_t *infile_name=NULL,*outfile_name=NULL;
  pdf_char_t *endptr = NULL; /* Used in strtol */
  pdf_fsys_file_t infile, outfile;

  finish = PDF_FALSE;
  cache_size = 0;
  *read_mode = PDF_FALSE;

  while (!finish &&
	 (ci = getopt_long (argc,
			     argv,
			     "i:o:",
			     GNU_longOptions,
			     NULL)) != -1)
    {
      c = ci;
      switch (c)
        {
	  /* COMMON ARGUMENTS */
        case HELP_ARG:
          {
            fprintf (stdout, "%s\n", pdf_filter_help_msg);
            exit (EXIT_SUCCESS);
            break;
          }
        case VERSION_ARG:
          {
            fprintf (stdout, "%s\n", pdf_utils_version_msg);
            exit (EXIT_SUCCESS);
            break;
          }
	case READ_ARG:
	  {
	    *read_mode = PDF_TRUE;
	    break;
	  }
	case CACHE_ARG:
	  {
	    cache_size = strtol (optarg, (char **) &endptr, 10);

            if ((endptr != NULL) && (*endptr != '\0'))
              {
                /* Error parsing the number */
                fprintf (stdout, "%s\n", pdf_filter_help_msg);
                exit (EXIT_FAILURE);
              }

	    break;
	  }
        case INFILE_ARG:
        case 'i':
          {
            infile_name = pdf_alloc (strlen(optarg)+1);
            strcpy (infile_name, optarg);
            break;
          }
        case OUTFILE_ARG:
        case 'o':
          {
            outfile_name = pdf_alloc (strlen(optarg)+1);
            strcpy (outfile_name, optarg);
            break;
          }
	case '?':
	default:
	  {
	    finish = PDF_TRUE;
	    break;
	  }
	}
    }

  /*
    Store the return value, which may be the next param identifier,
    so the install_filters or any other function can continue processing
    arguments
  */
  *last_ci = ci;

  if (pdf_text_init() != PDF_OK)
    {
      fprintf (stderr, "Error initializing the text module.\n");
      exit (EXIT_FAILURE);
    }

  *read_pdf_fsys = PDF_FALSE;
  *write_pdf_fsys = PDF_FALSE;
  if (infile_name == NULL && outfile_name == NULL)
    {
      ret = pdf_stm_cfile_new (*read_mode ? stdin : stdout,
			       0,
			       cache_size,
			       *read_mode ? PDF_STM_READ : PDF_STM_WRITE,
			       &stm);
      if (ret != PDF_OK)
        {
          pdf_error (ret, stderr, "while creating the write stream");
          exit (EXIT_FAILURE);
        }
    }
  else
    {
      if (infile_name != NULL)
        {
          open_file (infile_name, &infile, PDF_FSYS_OPEN_MODE_READ);
          *read_pdf_fsys = PDF_TRUE;
          ret = pdf_stm_file_new (infile,0,cache_size,PDF_STM_READ,
                                  *read_mode ? &stm : fsys_stm);
          if (ret != PDF_OK)
            {
              pdf_error (ret, stderr, "while creating the read stream");
              exit (EXIT_FAILURE);
            }
        }
      else
        {
          ret = pdf_stm_cfile_new (stdin,0,cache_size,PDF_STM_READ,
                                   *read_mode ? &stm : fsys_stm);
          if (ret != PDF_OK)
            {
              pdf_error (ret, stderr, "while creating the read stream");
              exit (EXIT_FAILURE);
            }
        }

      if (outfile_name != NULL)
        {
          open_file (outfile_name, &outfile, PDF_FSYS_OPEN_MODE_WRITE);
          *write_pdf_fsys = PDF_TRUE;
          ret = pdf_stm_file_new (outfile,0,cache_size,PDF_STM_WRITE,
                                  *read_mode ? fsys_stm : &stm);
          if (ret != PDF_OK)
            {
              pdf_error (ret, stderr, "while creating the write stream");
              exit (EXIT_FAILURE);
            }
        }
      else
        {
          ret = pdf_stm_cfile_new (stdout,0,cache_size,PDF_STM_WRITE,
                                   *read_mode ? fsys_stm : &stm);
          if (ret != PDF_OK)
            {
              pdf_error (ret, stderr, "while creating the write stream");
              exit (EXIT_FAILURE);
            }

        }
    }

  return stm;
}

static void
install_filters (int        argc,
                 char      *argv[],
                 pdf_stm_t  stm,
                 int        ci)
{
  char c;
  pdf_status_t ret;
  pdf_hash_t *filter_params;
  FILE *file;
  char *jbig2dec_global_segments = NULL;
  char *key = NULL;
  pdf_size_t jbig2dec_global_segments_size = 0;
  pdf_status_t status;
  pdf_bool_t lzw_earlychange = PDF_FALSE;
  pdf_error_t *error = NULL;

  /* Initialize the crypt module */
  if (pdf_crypt_init () != PDF_OK)
    {
      fprintf(stderr, "Error calling pdf_crypt_init().\n");
      exit (EXIT_FAILURE);
    }

  /* Install filters */
  do
    {
      c = ci;
      switch (c)
        {
          /* FILTER INSTALLERS */
        case NULL_FILTER_ARG:
          {
            filter_params = pdf_hash_new (&error);
            if (filter_params == NULL)
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "Couldn't create hash table: '%s'",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_NULL,
                                             filter_params);
            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the null filter");
                exit (EXIT_FAILURE);
              }

            break;
          }
        case ASCIIHEXDEC_FILTER_ARG:
          {
            filter_params = pdf_hash_new (&error);
            if (filter_params == NULL)
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "Couldn't create hash table: '%s'",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_AHEX_DEC,
                                             filter_params);
            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the ASCII hex decoder filter");
                exit (EXIT_FAILURE);
              }

            break;
          }
        case ASCIIHEXENC_FILTER_ARG:
          {
            filter_params = pdf_hash_new (&error);
            if (filter_params == NULL)
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "Couldn't create hash table: '%s'",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_AHEX_ENC,
                                             filter_params);
            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the ASCII hex encoder filter");
                exit (EXIT_FAILURE);
              }

            break;
          }
#ifdef PDF_HAVE_LIBJPEG
        case DCTDEC_FILTER_ARG:
          {
            filter_params = pdf_hash_new (&error);
            if (filter_params == NULL)
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "Couldn't create hash table: '%s'",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            pdf_stm_install_filter (stm,
                                    PDF_STM_FILTER_DCT_DEC,
                                    filter_params);

            /* Note that a reference to this memory remains into the
             *  stream */
            jbig2dec_global_segments = NULL;
            jbig2dec_global_segments_size = 0;

            break;
          }
#endif /* PDF_HAVE_LIBJPEG */

        case ASCII85DEC_FILTER_ARG:
          {
            filter_params = pdf_hash_new (&error);
            if (filter_params == NULL)
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "Couldn't create hash table: '%s'",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_A85_DEC,
                                             filter_params);
            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the A85 decoder filter");
                exit (EXIT_FAILURE);
              }

            break;
          }
        case ASCII85ENC_FILTER_ARG:
          {
            filter_params = pdf_hash_new (&error);
            if (filter_params == NULL)
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "Couldn't create hash table: '%s'",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_A85_ENC,
                                             filter_params);
            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the A85 encoder filter");
                exit (EXIT_FAILURE);
              }

            break;
          }
#if 0
        case CCITTFAXDEC_FILTER_ARG:
          {
            break;
          }
        case JXPDEC_FILTER_ARG:
          {
            break;
          }
        case PREDENC_FILTER_ARG:
          {
            /* pdf_stm_install_predenc_filter (input,
               PDF_STM_FILTER_READ,
               args.pred_enc_type,
               args.pred_colors,
               args.pred_bpc,
               args.pred_columns); */
            break;
          }
        case PREDDEC_FILTER_ARG:
          {
            /* pdf_stm_install_preddec_filter (input,
               PDF_STM_FILTER_READ,
               args.pred_dec_type,
               args.pred_colors,
               args.pred_bpc,
               args.pred_columns); */
            break;
          }
#endif /* 0 */
        case LZW_EARLYCHANGE_ARG:
          {
            lzw_earlychange = PDF_TRUE;
            break;
          }
        case LZWENC_FILTER_ARG:
          {
            filter_params = pdf_hash_new (&error);
            if (filter_params == NULL)
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "Couldn't create hash table: '%s'",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            if (!pdf_hash_add_bool (filter_params, "EarlyChange", lzw_earlychange, &error))
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "while creating the LZW encoder filter: '%s'",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_LZW_ENC,
                                             filter_params);

            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while creating the LZW encoder filter");
                exit (EXIT_FAILURE);
              }

            break;
          }
        case LZWDEC_FILTER_ARG:
          {
            filter_params = pdf_hash_new (&error);
            if (filter_params == NULL)
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "Couldn't create hash table: '%s'",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            if (!pdf_hash_add_bool (filter_params, "EarlyChange", lzw_earlychange, &error))
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "while creating the LZW decoder filter: '%s'",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_LZW_DEC,
                                             filter_params);

            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while creating the LZW decoder filter");
                exit (EXIT_FAILURE);
              }

            break;
          }
#ifdef PDF_HAVE_LIBZ
        case FLATEDEC_FILTER_ARG:
          {
            filter_params = pdf_hash_new (&error);
            if (filter_params == NULL)
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "Couldn't create hash table: '%s'",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_FLATE_DEC,
                                             filter_params);
            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the flate decoder filter");
                exit (EXIT_FAILURE);
              }

            break;
          }
        case FLATEENC_FILTER_ARG:
          {
            filter_params = pdf_hash_new (&error);
            if (filter_params == NULL)
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "Couldn't create hash table: '%s'",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_FLATE_ENC,
                                             filter_params);
            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the flate encoder filter");
                exit (EXIT_FAILURE);
              }

            break;
          }
#endif /* PDF_HAVE_LIBZ */
        case RUNLENGTHDEC_FILTER_ARG:
          {
            filter_params = pdf_hash_new (&error);
            if (filter_params == NULL)
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "Couldn't create hash table: '%s'",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_RL_DEC,
                                             filter_params);

            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the RunLength decoder filter");
                exit (EXIT_FAILURE);
              }

            break;
          }
        case RUNLENGTHENC_FILTER_ARG:
          {
            filter_params = pdf_hash_new (&error);
            if (filter_params == NULL)
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "Couldn't create hash table: '%s'",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_RL_ENC,
                                             filter_params);

            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the RunLength encoder filter");
                exit (EXIT_FAILURE);
              }

            break;
          }
#ifdef PDF_HAVE_LIBJBIG2DEC
        case JBIG2DEC_GLOBAL_SEGMENTS_ARG:
          {
            struct stat fstats;

            stat (optarg, &fstats);

            /* Load the contents of a jbig2 global segments file */
            file = fopen (optarg, "r");
            if (file == NULL)
              {
                fprintf (stderr, "error: invalid jbig2 global segments file\n");
                exit (EXIT_FAILURE);
              }

            jbig2dec_global_segments = pdf_alloc (fstats.st_size);
            jbig2dec_global_segments_size = fstats.st_size;
            if (fread (jbig2dec_global_segments,
                       1, fstats.st_size, file) != fstats.st_size)
              {
                fprintf (stderr, "error: reading jbig2 global segments file\n");
                exit (EXIT_FAILURE);
              }

            fclose (file);
            break;
          }
        case JBIG2DEC_FILTER_ARG:
          {
            filter_params = pdf_hash_new (&error);
            if (filter_params == NULL)
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "Couldn't create hash table: '%s'",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            if (jbig2dec_global_segments != NULL)
              {
                if (!pdf_hash_add_string (filter_params, "GlobalStreamsBuffer", jbig2dec_global_segments, &error))
                  {
                    pdf_error (pdf_error_get_status (error),
                               stderr,
                               "Couldn't add new GlobalStreamsBuffer hash: '%s'",
                               pdf_error_get_message (error));
                    exit (EXIT_FAILURE);
                  }
                if (!pdf_hash_add_size (filter_params, "GlobalStreamsSize", strlen (jbig2dec_global_segments) + 1, &error))
                  {
                    pdf_error (pdf_error_get_status (error),
                               stderr,
                               "Couldn't add new GlobalStreamsSize hash: '%s'",
                               pdf_error_get_message (error));
                    exit (EXIT_FAILURE);
                  }
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_JBIG2_DEC,
                                             filter_params);

            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the JBIG2 decoder filter");
                exit (EXIT_FAILURE);
              }

            /* Note that a reference to this memory remains into the
               stream */
            jbig2dec_global_segments = NULL;
            jbig2dec_global_segments_size = 0;

            break;
          }
#endif /* PDF_HAVE_LIBJBIG2DEC */
        case MD5ENC_FILTER_ARG:
          {
            filter_params = pdf_hash_new (&error);
            if (filter_params == NULL)
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "Couldn't create hash table: '%s'",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_MD5_ENC,
                                             filter_params);

            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the MD5 encoder filter");
                exit (EXIT_FAILURE);
              }

            break;
          }
        case KEY_ARG:
          {
            if (key != NULL)
              {
                pdf_dealloc (key);
                key = NULL;
              }

            key = strdup (optarg);
            break;
          }
        case AESENC_FILTER_ARG:
          {
            filter_params = pdf_hash_new (&error);
            if (filter_params == NULL)
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "Couldn't create hash table: '%s'",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            if (key != NULL)
              {
                if (!pdf_hash_add_string (filter_params, "Key", key, &error))
                  {
                    pdf_error (pdf_error_get_status (error),
                               stderr,
                               "Couldn't add new Key hash: '%s'",
                               pdf_error_get_message (error));
                    exit (EXIT_FAILURE);
                  }
                if (!pdf_hash_add_size (filter_params, "KeySize", strlen (key), &error))
                  {
                    pdf_error (pdf_error_get_status (error),
                               stderr,
                               "Couldn't add new KeySize hash: '%s'",
                               pdf_error_get_message (error));
                    exit (EXIT_FAILURE);
                  }
              }
            else
              {
                fprintf (stderr, "You should specify a key for the aesenc filter.\n");
                exit (EXIT_FAILURE);
              }


            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_AESV2_ENC,
                                             filter_params);

            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the AESV2 encoder filter");
                exit (EXIT_FAILURE);
              }

            break;
          }
        case AESDEC_FILTER_ARG:
          {
            filter_params = pdf_hash_new (&error);
            if (filter_params == NULL)
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "Couldn't create hash table: '%s'",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            if (key != NULL)
              {
                if (!pdf_hash_add_string (filter_params, "Key", key, &error))
                  {
                    pdf_error (pdf_error_get_status (error),
                               stderr,
                               "Couldn't add new Key hash: '%s'",
                               pdf_error_get_message (error));
                    exit (EXIT_FAILURE);
                  }
                if (!pdf_hash_add_size (filter_params, "KeySize", strlen (key), &error))
                  {
                    pdf_error (pdf_error_get_status (error),
                               stderr,
                               "Couldn't add new KeySize hash: '%s'",
                               pdf_error_get_message (error));
                    exit (EXIT_FAILURE);
                  }
              }
            else
              {
                fprintf (stderr, "You should specify a key for the aesdec filter.\n");
                exit (EXIT_FAILURE);
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_AESV2_DEC,
                                             filter_params);

            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the AESV2 decoder filter");
                exit (EXIT_FAILURE);
              }

            break;
          }
        case V2ENC_FILTER_ARG:
          {
            filter_params = pdf_hash_new (&error);
            if (filter_params == NULL)
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "Couldn't create hash table: '%s'",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            if (key != NULL)
              {
                if (!pdf_hash_add_string (filter_params, "Key", key, &error))
                  {
                    pdf_error (pdf_error_get_status (error),
                               stderr,
                               "Couldn't add new Key hash: '%s'",
                               pdf_error_get_message (error));
                    exit (EXIT_FAILURE);
                  }
                if (!pdf_hash_add_size (filter_params, "KeySize", strlen (key), &error))
                  {
                    pdf_error (pdf_error_get_status (error),
                               stderr,
                               "Couldn't add new KeySize hash: '%s'",
                               pdf_error_get_message (error));
                    exit (EXIT_FAILURE);
                  }
              }
            else
              {
                fprintf (stderr, "You should specify a key for the v2enc filter.\n");
                exit (EXIT_FAILURE);
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_V2_ENC,
                                             filter_params);

            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the V2 encoder filter");
                exit (EXIT_FAILURE);
              }

            break;
          }
        case V2DEC_FILTER_ARG:
          {
            filter_params = pdf_hash_new (&error);
            if (filter_params == NULL)
              {
                pdf_error (pdf_error_get_status (error),
                           stderr,
                           "Couldn't create hash table: '%s'",
                           pdf_error_get_message (error));
                exit (EXIT_FAILURE);
              }

            if (key != NULL)
              {
                if (!pdf_hash_add_string (filter_params, "Key", key, &error))
                  {
                    pdf_error (pdf_error_get_status (error),
                               stderr,
                               "Couldn't add new Key hash: '%s'",
                               pdf_error_get_message (error));
                    exit (EXIT_FAILURE);
                  }
                if (!pdf_hash_add_size (filter_params, "KeySize", strlen (key), &error))
                  {
                    pdf_error (pdf_error_get_status (error),
                               stderr,
                               "Couldn't add new KeySize hash: '%s'",
                               pdf_error_get_message (error));
                    exit (EXIT_FAILURE);
                  }
              }
            else
              {
                fprintf (stderr, "You should specify a key for the v2dec filter.\n");
                exit (EXIT_FAILURE);
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_V2_DEC,
                                             filter_params);

            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the V2 decoder filter");
                exit (EXIT_FAILURE);
              }

            break;
          }
	  /* FILTER OPTIONS: */
	  /* TODO */
	  /* ERROR: */
	case '?':
          {
            /* Error, usage and exit */
            fprintf (stdout, "%s\n", pdf_filter_help_msg);
            exit (EXIT_FAILURE);
            break;
          }
        }
    }
  while ((ci = getopt_long (argc,
                            argv,
                            "",
                            GNU_longOptions,
                            NULL)) != -1);

}

static void
open_file (pdf_char_t                *name,
           pdf_fsys_file_t           *file,
           enum pdf_fsys_file_mode_e  mode)
{
  pdf_status_t ret;
  pdf_text_t path;
  pdf_char_t * rem;
  pdf_size_t rem_len;

  ret = pdf_text_new_from_pdf_string (name,
                                      strlen(name),
                                      &rem,
                                      &rem_len,
                                      &path);
  if (ret != PDF_OK)
    {
      pdf_error (ret, stderr, "while creating pdf text path");
      exit (EXIT_FAILURE);
    }

  ret = pdf_fsys_file_open (NULL, path, mode, file);
  if (ret != PDF_OK)
    {
      pdf_error (ret, stderr, "while opening file '%s'", name);
      exit (EXIT_FAILURE);
    }
}

/* End of pdf_filter.c */

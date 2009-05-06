/* -*- mode: C -*- Time-stamp: "09/05/06 19:48:59 jemarch"
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
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>

#ifdef HAVE_MALLOC_H
#include <malloc.h>
#else
#include <stdlib.h>
#endif /* HAVE_MALLOC_H */

#include <sys/stat.h>

#include <xalloc.h>
#include <pdf-filter.h>
#include <pdf.h>

/*
 * Command line options management 
 */

static struct option GNU_longOptions[] =
  {
    {"help", no_argument, NULL, HELP_ARG},
    {"usage", no_argument, NULL, USAGE_ARG},
    {"version", no_argument, NULL, VERSION_ARG},
    {"readmode", no_argument, NULL, READ_ARG},
    {"input-file", required_argument, NULL, INFILE_ARG},
    {"output-file", required_argument, NULL, OUTFILE_ARG},
    {"cache", required_argument, NULL, CACHE_ARG},
    {"null", no_argument, NULL, NULL_FILTER_ARG},
    {"ahexdec", no_argument, NULL, ASCIIHEXDEC_FILTER_ARG},
    {"ahexenc", no_argument, NULL, ASCIIHEXENC_FILTER_ARG},
#if 0
    {"a85dec", no_argument, NULL, ASCII85DEC_FILTER_ARG},
    {"a85enc", no_argument, NULL, ASCII85ENC_FILTER_ARG},
    {"lzwenc", no_argument, NULL, LZWENC_FILTER_ARG},
    {"lzwdec", no_argument, NULL, LZWDEC_FILTER_ARG},
    {"cfaxdec", no_argument, NULL, CCITTFAXDEC_FILTER_ARG},
    {"jxpdec", no_argument, NULL, JXPDEC_FILTER_ARG},
    {"predenc", no_argument, NULL, PREDENC_FILTER_ARG},
    {"preddec", no_argument, NULL, PREDDEC_FILTER_ARG},
#endif /* 0 */
#ifdef HAVE_LIBJPEG
    {"dctdec", no_argument, NULL, DCTDEC_FILTER_ARG},
#endif
#ifdef HAVE_LIBZ
    {"flatedec", no_argument, NULL, FLATEDEC_FILTER_ARG},
    {"flateenc", no_argument, NULL, FLATEENC_FILTER_ARG},
#endif /* HAVE_LIBZ */
    {"rldec", no_argument, NULL, RUNLENGTHDEC_FILTER_ARG},
    {"rlenc", no_argument, NULL, RUNLENGTHENC_FILTER_ARG},
#ifdef HAVE_LIBJBIG2DEC
    {"jbig2dec", no_argument, NULL, JBIG2DEC_FILTER_ARG},
    {"jbig2dec-globals", required_argument, NULL, JBIG2DEC_GLOBAL_SEGMENTS_ARG},
#endif /* HAVE_LIBJBIG2DEC */
    {"md5enc", no_argument, NULL, MD5ENC_FILTER_ARG},
    {"key", required_argument, NULL, KEY_ARG},
    {"aesenc", no_argument, NULL, AESENC_FILTER_ARG},
    {"aesdec", no_argument, NULL, AESDEC_FILTER_ARG},
    {"v2enc", no_argument, NULL, V2ENC_FILTER_ARG},
    {"v2dec", no_argument, NULL, V2DEC_FILTER_ARG},
    {NULL, 0, NULL, 0}
  };

/* Messages */

char *pdf_filter_version_msg = "pdf_filter 0.1";

char *pdf_filter_usage_msg = "\
Usage: pdf_filter [[OPTIONS] [FILTER FILTER_ARGS]...]\n\
Filter the standard input with the specified PDF standard filters and \n\
write the result in the standard output.\n\
\n\
available options\n\
  --readmode                          test the stream in read mode instead\n\
                                       of write mode.\n\
  -i FILE, --input-file=FILE          Use a given file as the input.\n\
  -o FILE, --output-file=FILE         Use a given file as the output.\n\
  --cache=NUM                         set the stream cache size.\n\n\
available filters\n\
  --null                              use the NULL filter\n\
  --ahexdec                           use the ASCII Hex decoder filter\n\
  --ahexenc                           use the ASCII Hex encoder filter\n"
#if 0
"  --a85dec                            use the ASCII 85 decoder filter\n\
  --a85enc                            use the ASCII 85 encoder filter\n\
  --lzwenc                            use the LZW encoder filter\n\
  --lzwdec                            use the LZW decoder filter\n
  --dctdec                            use the DCT decoder filter\n\
  --jxpdec                            use the JXP decoder filter\n\
  --predenc                           use the predictor encoder filter\n\
  --preddec                           use the predictor decoder filter\n"
#endif /* 0 */
#ifdef HAVE_LIBZ
  "  --flatedec                          use the Flate decoder filter\n\
  --flateenc                          use the Flate encoder filter\n"
#endif /* HAVE_LIBZ */
  "  --rldec                             use the Run Length decoder filter\n\
  --rlenc                             use the Run Length encoder filter\n\
  --cfaxdec                           use the CCITT Fax decoder filter\n\
  --jbig2dec                          use the JBIG2 decoder filter\n\
  --md5enc                            use the MD5 encoder filter\n\
  --aesenc                            use the AESv2 encoder filter\n\
  --aesdec                            use the AESv2 decoder filter\n\
  --v2enc                             use the V2 encoder filter\n\
  --v2dec                             use the V2 decoder filter\n\
  --help                              print a help message and exit\n\
  --usage                             print a usage message and exit\n\
  --version                           show pdf_filter version and exit\n\
\nfilter properties\n"
#if 0
"  --lzw-earlychange                   toggles earlychange for next lzw filters\n\
  --preddec-type=NUM                  code for next preddec filters type\n\
  --predenc-type=NUM                  code for next predenc filters type\n\
  --pred-colors=NUM                   next predictors colors per sample\n\
  --pred-bpc=NUM                      next predictors bits per color component\n\
  --pred-columns=NUM                  next predictors number of samples per row\n"
#endif /* 0 */
"  --jbig2dec-globals=FILE             file containing global segments\n\
";

char *pdf_filter_help_msg = "";

static pdf_stm_t
create_stream (int argc, char* argv[], pdf_bool_t* mode, pdf_status_t* last_ret,
               pdf_bool_t * read_pdf_fsys, pdf_bool_t * write_pdf_fsys,
               pdf_stm_t * fsys_stm);

static void
install_filters (int argc, char* argv[], pdf_stm_t stm, pdf_status_t ret);

static void
process_stream (pdf_stm_t, pdf_bool_t mode, pdf_bool_t read_pdf_fsys,
                pdf_bool_t write_pdf_fsys, pdf_stm_t fsys_stm);

static
open_file (pdf_char_t * name, pdf_fsys_file_t * file,
           enum pdf_fsys_file_mode_e mode);

int
main (int argc, char *argv[])
{
  pdf_stm_t stm,fsys_stm;
  pdf_bool_t read_mode,read_pdf_fsys,write_pdf_fsys;
  pdf_status_t last_ret;
  pdf_status_t destroy_ret;
  
  stm = create_stream (argc, argv, &read_mode, &last_ret, &read_pdf_fsys,
                       &write_pdf_fsys, &fsys_stm);
  install_filters (argc, argv, stm, last_ret);
  process_stream (stm, read_mode, read_pdf_fsys,write_pdf_fsys, fsys_stm);
  destroy_ret = pdf_stm_destroy (stm);
  if ((destroy_ret != PDF_OK) && (destroy_ret != PDF_EEOF))
    {
      /* Only writing streams report errors on pdf_stm_destroy */
      pdf_error (destroy_ret, stderr, "writing to stream");
      exit(1);
    }

  if (read_pdf_fsys || write_pdf_fsys)
    {
      destroy_ret = pdf_stm_destroy (fsys_stm);
      if (destroy_ret != PDF_OK)
        {
          /* Only writing streams report errors on pdf_stm_destroy */
          pdf_error (destroy_ret, stderr, "writing to stream");
          exit(1);
        }
    }
 
  return 0;
}

static void
process_stream (pdf_stm_t stm, pdf_bool_t read_mode, pdf_bool_t read_pdf_fsys,
                pdf_bool_t write_pdf_fsys, pdf_stm_t fsys_stm)
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
              exit (1);
            }            

          if (write_pdf_fsys)
            {
              ret = pdf_stm_write (fsys_stm, buf, read_bytes, &written_bytes);
              if (ret != PDF_OK)
                {
                  pdf_error (ret, stderr, "writing to stream");
                  exit (1);
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
                  exit (1);
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
              exit (1);
            }            

        }
      while (read_bytes == BUF_SIZE);
    }

#undef BUF_SIZE
}

static pdf_stm_t
create_stream (int argc, char* argv[], pdf_bool_t* read_mode,
	       pdf_status_t* last_ret, pdf_bool_t * read_pdf_fsys,
               pdf_bool_t * write_pdf_fsys, pdf_stm_t * fsys_stm)
{
  char c;
  pdf_status_t ret;
  pdf_size_t cache_size;
  pdf_stm_t stm;
  pdf_bool_t finish;
  pdf_char_t *infile_name=NULL,*outfile_name=NULL;
  pdf_fsys_file_t infile, outfile;

  finish = PDF_FALSE;
  cache_size = 0;
  *read_mode = PDF_FALSE; 
    
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
            fprintf (stdout, "%s\n", pdf_filter_usage_msg);
            exit (0);
            break;
          }
        case VERSION_ARG:
          {
            fprintf (stdout, "%s\n", pdf_filter_version_msg);
            exit (0);
            break;
          }
        case USAGE_ARG:
          {
            fprintf (stdout, "%s\n", pdf_filter_usage_msg);
            exit (0);
            break;
          }
	case READ_ARG:
	  {
	    *read_mode = PDF_TRUE;
	    break;
	  }
	case CACHE_ARG:
	  {
	    cache_size = atoi (optarg);
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
  *last_ret = ret;

  if (pdf_text_init() != PDF_OK)
    {
      fprintf (stderr, "Error initializing the text module.\n");
      exit(1);
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
          exit (1);
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
              exit (1);
            }
        }
      else
        {
          ret = pdf_stm_cfile_new (stdin,0,cache_size,PDF_STM_READ,
                                   *read_mode ? &stm : fsys_stm);
          if (ret != PDF_OK)
            {
              pdf_error (ret, stderr, "while creating the read stream");
              exit (1);
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
              exit (1);
            }
        }
      else
        {
          ret = pdf_stm_cfile_new (stdout,0,cache_size,PDF_STM_WRITE,
                                   *read_mode ? fsys_stm : &stm);
          if (ret != PDF_OK)
            {
              pdf_error (ret, stderr, "while creating the write stream");
              exit (1);
            }

        }
    }

  return stm;
}

static void
install_filters (int argc, char* argv[], pdf_stm_t stm, pdf_status_t ret)
{
  char c;
  pdf_hash_t filter_params;
  FILE *file;
  char *jbig2dec_global_segments = NULL;
  char *key = NULL;
  pdf_size_t jbig2dec_global_segments_size = 0;
  pdf_status_t status;

  /* Initialize the crypt module */
  if (pdf_crypt_init () != PDF_OK)
    {
      fprintf(stderr, "Error calling pdf_crypt_init().\n");
      exit (1);
    }

  /* Install filters */
  do
    {
      c = ret;
      switch (c)
	{
	  /* FILTER INSTALLERS */
        case NULL_FILTER_ARG:
          {
            ret = pdf_hash_new (NULL, &filter_params);
            if (ret != PDF_OK)
              {
                pdf_error (ret, stderr, "while creating the null filter parameters hash table");
                exit (1);
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_NULL,
                                             filter_params);
            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the null filter");
                exit (1);
              }

            break;
          }
        case ASCIIHEXDEC_FILTER_ARG:
          {
            ret = pdf_hash_new (NULL, &filter_params);
            if (ret != PDF_OK)
              {
                pdf_error (ret, stderr, "while creating the ahexdec filter parameters hash table");
                exit (1);
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_AHEX_DEC,
                                             filter_params);
            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the ASCII hex decoder filter");
                exit (1);
              }

            break;
          }
        case ASCIIHEXENC_FILTER_ARG:
          {
            ret = pdf_hash_new (NULL, &filter_params);
            if (ret != PDF_OK)
              {
                pdf_error (ret, stderr, "while creating the ahexenc filter parameters hash table");
                exit (1);
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_AHEX_ENC,
                                             filter_params);
            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the ASCII hex encoder filter");
                exit (1);
              }

            break;
          }
#if 0
        case ASCII85DEC_FILTER_ARG:
          {
            break;
          }
        case ASCII85ENC_FILTER_ARG:
          {
            break;
          }
	case LZWENC_FILTER_ARG:
          {
            break;
          }
        case LZWDEC_FILTER_ARG:
          {
            break;
          }
        case CCITTFAXDEC_FILTER_ARG:
          {
            break;
          }
#ifdef HAVE_LIBJPEG
        case DCTDEC_FILTER_ARG:
          {
            ret = pdf_hash_new (NULL, &filter_params);
            if (ret != PDF_OK)
              {
                pdf_error (ret, stderr, "while creating the dctdec filter parameters hash table");
                exit (1);
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
#endif /* HAVE_LIBJPEG */
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
#ifdef HAVE_LIBZ
        case FLATEDEC_FILTER_ARG:
          {
            ret = pdf_hash_new (NULL, &filter_params);
            if (ret != PDF_OK)
              {
                pdf_error (ret, stderr, "while creating the flatedec filter parameters hash table");
                exit (1);
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_FLATE_DEC,
                                             filter_params);
            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the flate decoder filter");
                exit (1);
              }

            break;
          }
        case FLATEENC_FILTER_ARG:
          {
            ret = pdf_hash_new (NULL, &filter_params);
            if (ret != PDF_OK)
              {
                pdf_error (ret, stderr, "while creating the flateenc filter parameters hash table");
                exit (1);
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_FLATE_ENC,
                                             filter_params);
            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the flate encoder filter");
                exit (1);
              }

            break;
          }
#endif /* HAVE_LIBZ */
        case RUNLENGTHDEC_FILTER_ARG:
          {
            ret = pdf_hash_new (NULL, &filter_params);
            if (ret != PDF_OK)
              {
                pdf_error (ret, stderr, "while creating the rldec filter parameters hash table");
                exit (1);
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_RL_DEC,
                                             filter_params);

            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the RunLength decoder filter");
                exit (1);
              }

            break;
          }
        case RUNLENGTHENC_FILTER_ARG:
          {
            ret = pdf_hash_new (NULL, &filter_params);
            if (ret != PDF_OK)
              {
                pdf_error (ret, stderr, "while creating the rlenc filter parameters hash table");
                exit (1);
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_RL_ENC,
                                             filter_params);

            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the RunLength encoder filter");
                exit (1);
              }

            break;
          }
#ifdef HAVE_LIBJBIG2DEC
        case JBIG2DEC_GLOBAL_SEGMENTS_ARG:
          {
            struct stat fstats;


            stat (optarg, &fstats);

            /* Load the contents of a jbig2 global segments file */
            file = fopen (optarg, "r");
            if (file == NULL)
              {
                fprintf (stderr, "error: invalid jbig2 global segments file\n");
                exit (1);
              }

            jbig2dec_global_segments = pdf_alloc (fstats.st_size);
            jbig2dec_global_segments_size = fstats.st_size;
            if (fread (jbig2dec_global_segments,
                       1, fstats.st_size, file) != fstats.st_size)
              {
                fprintf (stderr, "error: reading jbig2 global segments file\n");
                exit (1);
              }

            fclose (file);
            break;
          }
        case JBIG2DEC_FILTER_ARG:
          {
            ret = pdf_hash_new (NULL, &filter_params);
            if (ret != PDF_OK)
              {
                pdf_error (ret, stderr, "while creating the jbig2dec filter parameters hash table");
                exit (1);
              }

            if (jbig2dec_global_segments != NULL)
              {
                pdf_hash_add_string (filter_params, "GlobalStreamsBuffer", jbig2dec_global_segments);
                pdf_hash_add_size (filter_params, "GlobalStreamsSize", strlen (jbig2dec_global_segments) + 1);
              }


            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_JBIG2_DEC,
                                             filter_params);

            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the JBIG2 decoder filter");
                exit (1);
              }

            /* Note that a reference to this memory remains into the
               stream */
            jbig2dec_global_segments = NULL;
            jbig2dec_global_segments_size = 0;

            break;
          }
#endif /* HAVE_LIBJBIG2DEC */
        case MD5ENC_FILTER_ARG:
          {
            ret = pdf_hash_new (NULL, &filter_params);
            if (ret != PDF_OK)
              {
                pdf_error (ret, stderr, "while creating the md5enc filter parameters hash table");
                exit (1);
              }

            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_MD5_ENC,
                                             filter_params);

            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the MD5 encoder filter");
                exit (1);
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
            ret = pdf_hash_new (NULL, &filter_params);
            if (ret != PDF_OK)
              {
                pdf_error (ret, stderr, "while creating the aesenc filter parameters hash table");
                exit (1);
              }

            if (key != NULL)
              {
                pdf_hash_add_string (filter_params, "Key", key);
                pdf_hash_add_size (filter_params, "KeySize", strlen (key));
              }
            else
              {
                fprintf (stderr, "You should specify a key for the aesenc filter.\n");
                exit (1);
              }

            
            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_AESV2_ENC,
                                             filter_params);

            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the AESV2 encoder filter");
                exit (1);
              }

            break;
          }
        case AESDEC_FILTER_ARG:
          {
            ret = pdf_hash_new (NULL, &filter_params);
            if (ret != PDF_OK)
              {
                pdf_error (ret, stderr, "while creating the aesdec filter parameters hash table");
                exit (1);
              }

            if (key != NULL)
              {
                pdf_hash_add_string (filter_params, "Key", key);
                pdf_hash_add_size (filter_params, "KeySize", strlen (key));
              }
            else
              {
                fprintf (stderr, "You should specify a key for the aesdec filter.\n");
                exit (1);
              }

            
            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_AESV2_DEC,
                                             filter_params);

            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the AESV2 decoder filter");
                exit (1);
              }

            break;
          }
        case V2ENC_FILTER_ARG:
          {
            ret = pdf_hash_new (NULL, &filter_params);
            if (ret != PDF_OK)
              {
                pdf_error (ret, stderr, "while creating the v2enc filter parameters hash table");
                exit (1);
              }

            if (key != NULL)
              {
                pdf_hash_add_string (filter_params, "Key", key);
                pdf_hash_add_size (filter_params, "KeySize", strlen (key));
              }
            else
              {
                fprintf (stderr, "You should specify a key for the v2enc filter.\n");
                exit (1);
              }

            
            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_V2_ENC,
                                             filter_params); 

            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the V2 encoder filter");
                exit (1);
              }

            break;
          }
        case V2DEC_FILTER_ARG:
          {
            ret = pdf_hash_new (NULL, &filter_params);
            if (ret != PDF_OK)
              {
                pdf_error (ret, stderr, "while creating the v2dec filter parameters hash table");
                exit (1);
              }

            if (key != NULL)
              {
                pdf_hash_add_string (filter_params, "Key", key);
                pdf_hash_add_size (filter_params, "KeySize", strlen (key));
              }
            else
              {
                fprintf (stderr, "You should specify a key for the v2dec filter.\n");
                exit (1);
              }

            
            status = pdf_stm_install_filter (stm,
                                             PDF_STM_FILTER_V2_DEC,
                                             filter_params); 

            if (status != PDF_OK)
              {
                pdf_error (status, stderr, "while installing the V2 decoder filter");
                exit (1);
              }

            break;
          }
	  /* FILTER OPTIONS: */
	  /* TODO */
	  /* ERROR: */
	case '?':
          {
            /* Error, usage and exit */
            fprintf (stdout, "%s\n", pdf_filter_usage_msg);
            exit (1);
            break;
          }
        }
    }
  while ((ret = getopt_long (argc,
			     argv,
			     "",
			     GNU_longOptions, 
			     NULL)) != -1);

}

static
open_file (pdf_char_t * name, pdf_fsys_file_t * file,
           enum pdf_fsys_file_mode_e mode)
{
  pdf_status_t ret;
  pdf_text_t path;
  pdf_char_t * rem;
  pdf_size_t rem_len;

  ret = pdf_text_new_from_pdf_string(name, strlen(name), &rem, &rem_len, &path);
  if (ret != PDF_OK)
    {
      pdf_error (ret, stderr, "while creating pdf text path");
      exit (1);
    }

  ret = pdf_fsys_file_open (NULL, path, mode, file);
  if (ret != PDF_OK)
    {
      pdf_error (ret, stderr, "while opening file '%s'", name);
      exit (1);
    } 
}

/* End of pdf_filter.c */

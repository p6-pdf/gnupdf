/* -*- mode: C -*- Time-stamp: "08/10/03 02:03:01 jemarch"
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

#ifdef HAVE_MALLOC_H
 #include <malloc.h>
#else
 #include <stdlib.h>
#endif /* HAVE_MALLOC_H */

#include <xalloc.h>
#include <pdf-filter.h>
#include <pdf.h>

/*
 * Some default values for filters with arguments
 */

#define DEF_LZW_EARLY_CHANGE PDF_FALSE
#define DEF_PRED_ENC_TYPE PDF_STM_F_PREDENC_TIFF_PREDICTOR_2
#define DEF_PRED_DEC_TYPE PDF_STM_F_PREDDEC_TIFF_PREDICTOR_2
#define DEF_PRED_COLORS 3
#define DEF_PRED_BPC 8
#define DEF_PRED_COLUMS 32

/*
 * Command line options management 
 */

static struct option GNU_longOptions[] =
{
  {"help", no_argument, NULL, HELP_ARG},
  {"usage", no_argument, NULL, USAGE_ARG},
  {"version", no_argument, NULL, VERSION_ARG},
  {"null", no_argument, NULL, NULL_FILTER_ARG},
  {"ahexdec", no_argument, NULL, ASCIIHEXDEC_FILTER_ARG},
  {"ahexenc", no_argument, NULL, ASCIIHEXENC_FILTER_ARG},
  {"a85dec", no_argument, NULL, ASCII85DEC_FILTER_ARG},
  {"a85enc", no_argument, NULL, ASCII85ENC_FILTER_ARG},
  {"lzwenc", no_argument, NULL, LZWENC_FILTER_ARG},
  {"lzwdec", no_argument, NULL, LZWDEC_FILTER_ARG},
#ifdef HAVE_LIBZ
  {"flatedec", no_argument, NULL, FLATEDEC_FILTER_ARG},
  {"flateenc", no_argument, NULL, FLATEENC_FILTER_ARG},
#endif /* HAVE_LIBZ */
  {"rldec", no_argument, NULL, RUNLENGTHDEC_FILTER_ARG},
  {"rlenc", no_argument, NULL, RUNLENGTHENC_FILTER_ARG},
  {"cfaxdec", no_argument, NULL, CCITTFAXDEC_FILTER_ARG},
  {"jbig2dec", no_argument, NULL, JBIG2DEC_FILTER_ARG},
  {"dctdec", no_argument, NULL, DCTDEC_FILTER_ARG},
  {"jxpdec", no_argument, NULL, JXPDEC_FILTER_ARG},
  {"predenc", no_argument, NULL, PREDENC_FILTER_ARG},
  {"preddec", no_argument, NULL, PREDDEC_FILTER_ARG},
  {"lzw-earlychange", no_argument, NULL, LZW_EARLY_CHANGE_ARG},
  {"predenc-type", required_argument, NULL, PREDENC_TYPE_ARG},
  {"preddec-type", required_argument, NULL, PREDDEC_TYPE_ARG},
  {"pred-colors", required_argument, NULL, PRED_COLORS_ARG},
  {"pred-bpc", required_argument, NULL, PRED_BPC_ARG},
  {"pred-columns", required_argument, NULL, PRED_COLUMNS_ARG},
  {NULL, 0, NULL, 0}
};

/* Messages */

char *pdf_filter_version_msg = "pdf_filter 0.1";

char *pdf_filter_usage_msg = "\
Usage: pdf_filter [[FILTER FILTER_ARGS]...]\n\
Filter the standard input with the specified PDF standard filters and \n\
write the result in the standard output.\n\
\n\
available filters\n\
  --null                              use the NULL filter\n\
  --ahexdec                           use the ASCII Hex decoder filter\n\
  --ahexenc                           use the ASCII Hex encoder filter\n\
  --a85dec                            use the ASCII 85 decoder filter\n\
  --a85enc                            use the ASCII 85 encoder filter\n\
  --lzwenc                            use the LZW encoder filter\n\
  --lzwdec                            use the LZW decoder filter\n"
#ifdef HAVE_LIBZ
"  --flatedec                          use the Flate decoder filter\n\
  --flateenc                          use the Flate encoder filter\n"
#endif /* HAVE_LIBZ */
"  --rldec                             use the Run Length decoder filter\n\
  --rlenc                             use the Run Length encoder filter\n\
  --cfaxdec                           use the CCITT Fax decoder filter\n\
  --jbig2dec                          use the JBIG2 decoder filter\n\
  --dctdec                            use the DCT decoder filter\n\
  --jxpdec                            use the JXP decoder filter\n\
  --predenc                           use the predictor encoder filter\n\
  --preddec                           use the predictor dncoder filter\n\
  --help                              print a help message and exit\n\
  --usage                             print a usage message and exit\n\
  --version                           show pdf_filter version and exit\n\
\nfilter properties\n\
  --lzw-earlychange                   toggles earlychange for next lzw filters\n\
  --preddec-type=NUM                  code for next preddec filters type\n\
  --predenc-type=NUM                  code for next predenc filters type\n\
  --pred-colors=NUM                   next predictors colors per sample\n\
  --pred-bpc=NUM                      next predictors bits per color component\n\
  --pred-columns=NUM                  next predictors number of samples per row\n\
";

char *pdf_filter_help_msg = "";

typedef struct filter_args_s
{
  int lzw_early_change;
  int pred_enc_type;
  int pred_dec_type;
  int pred_colors;
  int pred_bpc;
  int pred_columns;
} filter_args_t;

static void
filter_args_init(filter_args_t* a)
{
  /*  a->lzw_early_change = DEF_LZW_EARLY_CHANGE;
  a->pred_enc_type = DEF_PRED_ENC_TYPE;
  a->pred_dec_type = DEF_PRED_DEC_TYPE;
  a->pred_colors = DEF_PRED_COLORS;
  a->pred_bpc = DEF_PRED_BPC;
  a->pred_columns = DEF_PRED_COLUMS; */
}

int
main (int argc, char *argv[])
{
  char c;
  pdf_stm_t stm;
  pdf_char_t *buf;
  pdf_size_t buf_size;
  pdf_status_t ret;
  pdf_hash_t null_filter_params;
  pdf_hash_t ahexenc_filter_params;
  pdf_hash_t ahexdec_filter_params;
  pdf_hash_t rlenc_filter_params;
  pdf_hash_t rldec_filter_params;
  /*  pdf_hash_t a85enc_filter_params;
      pdf_hash_t a85dec_filter_params; */
  pdf_char_t *line;
  pdf_size_t line_bytes;
  pdf_size_t read_bytes;
  pdf_size_t written_bytes;

  /* Create the output buffer */
  buf_size = 4096;
  buf = pdf_alloc (buf_size);

  /* Create a writing memory stream */
  ret = pdf_stm_mem_new (buf,
                         buf_size,
                         1, /* Use the default cache size */
                         PDF_STM_WRITE,
                         &stm);
  if (ret != PDF_OK)
    {
      pdf_error (ret, stderr, "while creating the write stream");
      exit (1);
    }

  /*  filter_args_init(&args); */

  /* Manage command line arguments */
  while ((ret = getopt_long (argc,
                           argv,
                           "",
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
	  /* FILTER INSTALLERS */
        case NULL_FILTER_ARG:
          {
            ret = pdf_hash_new (NULL, &null_filter_params);
            if (ret != PDF_OK)
              {
                pdf_error (ret, stderr, "while creating the null filter parameters hash table");
                exit (1);
              }

            pdf_stm_install_filter (stm,
                                    PDF_STM_FILTER_NULL,
                                    null_filter_params);
            break;
          }
        case ASCIIHEXDEC_FILTER_ARG:
          {
            ret = pdf_hash_new (NULL, &ahexdec_filter_params);
            if (ret != PDF_OK)
              {
                pdf_error (ret, stderr, "while creating the ahexdec filter parameters hash table");
                exit (1);
              }

            pdf_stm_install_filter (stm,
                                    PDF_STM_FILTER_AHEX_DEC,
                                    ahexenc_filter_params);
            break;
          }
        case ASCIIHEXENC_FILTER_ARG:
          {
            ret = pdf_hash_new (NULL, &ahexenc_filter_params);
            if (ret != PDF_OK)
              {
                pdf_error (ret, stderr, "while creating the ahexenc filter parameters hash table");
                exit (1);
              }

            pdf_stm_install_filter (stm,
                                    PDF_STM_FILTER_AHEX_ENC,
                                    ahexenc_filter_params);
            break;
          }
        case ASCII85DEC_FILTER_ARG:
          {
            /*            ret = pdf_hash_new (NULL, &a85dec_filter_params);
            if (ret != PDF_OK)
              {
                pdf_error (ret, stderr, "while creating the a85dec filter parameters hash table");
                exit (1);
              }

            pdf_stm_install_filter (stm,
                                    PDF_STM_FILTER_A85_DEC,
                                    a85dec_filter_params); */
            break;
          }
        case ASCII85ENC_FILTER_ARG:
          {
            /*            ret = pdf_hash_new (NULL, &a85enc_filter_params);
            if (ret != PDF_OK)
              {
                pdf_error (ret, stderr, "while creating the a85enc filter parameters hash table");
                exit (1);
              }

            pdf_stm_install_filter (stm,
                                    PDF_STM_FILTER_A85_ENC,
                                    a85enc_filter_params); */
            break;
          }
	case LZWENC_FILTER_ARG:
          {
	    /* pdf_stm_install_lzwenc_filter (input,
               PDF_STM_FILTER_READ,
               args.lzw_early_change); */
            break;
          }
        case LZWDEC_FILTER_ARG:
          {
	    /* pdf_stm_install_lzwdec_filter (input,
               PDF_STM_FILTER_READ,
               args.lzw_early_change); */
            break;
          }
#ifdef HAVE_LIBZ
        case FLATEDEC_FILTER_ARG:
          {
            /* pdf_stm_install_flatedec_filter (input,
               PDF_STM_FILTER_READ); */
            break;
          }
        case FLATEENC_FILTER_ARG:
          {
            /* pdf_stm_install_flateenc_filter (input,
               PDF_STM_FILTER_READ); */
            break;
          }
#endif /* HAVE_LIBZ */
        case RUNLENGTHDEC_FILTER_ARG:
          {
            ret = pdf_hash_new (NULL, &rldec_filter_params);
            if (ret != PDF_OK)
              {
                pdf_error (ret, stderr, "while creating the rldec filter parameters hash table");
                exit (1);
              }

            pdf_stm_install_filter (stm,
                                    PDF_STM_FILTER_RL_DEC,
                                    rldec_filter_params);
            break;
          }
        case RUNLENGTHENC_FILTER_ARG:
          {
            ret = pdf_hash_new (NULL, &rlenc_filter_params);
            if (ret != PDF_OK)
              {
                pdf_error (ret, stderr, "while creating the rlenc filter parameters hash table");
                exit (1);
              }

            pdf_stm_install_filter (stm,
                                    PDF_STM_FILTER_RL_ENC,
                                    rlenc_filter_params);
            break;
          }
        case CCITTFAXDEC_FILTER_ARG:
          {
            break;
          }
        case JBIG2DEC_FILTER_ARG:
          {
            break;
          }
        case DCTDEC_FILTER_ARG:
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
	  /* FILTER OPTIONS: */
	case PREDDEC_TYPE_ARG:
	  {
	    /* args.pred_dec_type = atoi(optarg); */
	    break;
	  }
	case PREDENC_TYPE_ARG:
	  {
	    /* args.pred_enc_type = atoi(optarg); */
	    break;
	  }
	case PRED_COLORS_ARG:
	  {
	    /* args.pred_colors = atoi(optarg); */
	    break;
	  }
	case PRED_BPC_ARG:
	  {
	    /* args.pred_bpc = atoi(optarg); */
	    break;
	  }
	case PRED_COLUMNS_ARG:
	  {
	    /* args.pred_columns = atoi(optarg); */
	    break;
	  }
	case LZW_EARLY_CHANGE_ARG:
	  {
	    /* args.lzw_early_change = !args.lzw_early_change; */
	    break;
	  }
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

  /* Write stdin into the write stream */
  line = NULL;
  line_bytes = 0;
  while ((read_bytes = getline ((char **) &line, &line_bytes, stdin)) != EOF)
    {
      pdf_stm_seek (stm, 0);
      pdf_stm_write (stm,
                     line,
                     read_bytes);

      pdf_stm_flush (stm, PDF_FALSE);
      fwrite ((char *) buf,
              pdf_stm_tell (stm),
              1,
              stdout);

      pdf_dealloc (line);
      line = NULL;
      line_bytes = 0;
    }

  pdf_stm_seek (stm, 0);
  written_bytes = pdf_stm_flush (stm, PDF_TRUE);
  fwrite ((char *) buf,
          pdf_stm_tell (stm),
          1,
          stdout);

  /* Cleanup */
  free (buf);
  pdf_stm_destroy (stm);
  
  return 0;
}


/* End of pdf_filter.c */

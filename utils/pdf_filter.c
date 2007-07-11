/* -*- mode: C -*- Time-stamp: "07/07/11 20:12:47 jemarch"
 *
 *       File:         pdf_filter.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Tue Jul 10 18:42:07 2007
 *
 *       GNU PDF Library - Process input/output with the
 *                         standard PDF Filters.
 *
 */

#include <config.h>
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <malloc.h>
#include <xalloc.h>
#include <pdf_filter.h>
#include <pdf_stm.h>

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
  {"a85dec", no_argument, NULL, ASCII85DEC_FILTER_ARG},
  {"lzwdec", no_argument, NULL, LZWDEC_FILTER_ARG},
#ifdef HAVE_LIBZ
  {"flatedec", no_argument, NULL, FLATEDEC_FILTER_ARG},
#endif /* HAVE_LIBZ */
  {"rldec", no_argument, NULL, RUNLENGTHDEC_FILTER_ARG},
  {"cfaxdec", no_argument, NULL, CCITTFAXDEC_FILTER_ARG},
  {"jbig2dec", no_argument, NULL, JBIG2DEC_FILTER_ARG},
  {"dctdec", no_argument, NULL, DCTDEC_FILTER_ARG},
  {"jxpdec", no_argument, NULL, JXPDEC_FILTER_ARG},
  {NULL, 0, NULL, 0}
};

/* Messages */

char *pdf_filter_usage_msg = "\
Usage: pdf_filter [OPTIONS]\n\
Filter the standard input with the specified PDF standard filters and \n\
write the result in the standard output.\n\
  --null                              use the NULL filter\n\
  --ahexdec                           use the ASCII Hex decoder filter\n\
  --a85dec                            use the ASCII 85 decoder filter\n\
  --lzwdec                            use the LZW decoder filter\n"
#ifdef HAVE_LIBZ
"  --flatedec                          use the Flate decoder filter\n"
#endif /* HAVE_LIBZ */
"  --rldec                             use the Run Length decoder filter\n\
  --cfaxdec                           use the CCITT Fax decoder filter\n\
  --jbig2dec                          use the JBIG2 decoder filter\n\
  --dctdec                            use the DCT decoder filter\n\
  --jxpdec                            use the JXP decoder filter\n\
  --help                              print a help message and exit\n\
  --usage                             print a usage message and exit\n\
  --version                           show pdf_filter version and exit\n\
For filter-specific documentation use --help\n\
";

char *pdf_filter_help_msg = "";

int
main (int argc, char *argv[])
{
  char c;
  pdf_stm_t input;
  size_t line_bytes;
  size_t readed;
  char *line;
  unsigned char *output_buffer;

  /* Initialization */
  input = pdf_create_mem_stm (0,         /* Initial 0 length */
                              PDF_FALSE, /* Dont initialize */
                              0,         /* Init character */
                              PDF_TRUE); /* Auto-resize when necessary */
  

  /* Manage command line arguments */
  while ((c = getopt_long (argc,
                           argv,
                           "",
                           GNU_longOptions, 
                           NULL)) != -1)
    {
      switch (c)
        {
        case HELP_ARG:
          {
            fprintf (stdout, "%s\n", pdf_filter_help_msg);
            exit (0);
            break;
          }
        case USAGE_ARG:
          {
            fprintf (stdout, "%s\n", pdf_filter_usage_msg);
            exit (0);
            break;
          }
        case NULL_FILTER_ARG:
          {
            pdf_stm_install_null_filter (input, 
                                         PDF_STM_FILTER_READ);
            break;
          }
        case ASCIIHEXDEC_FILTER_ARG:
          {
            break;
          }
        case ASCII85DEC_FILTER_ARG:
          {
            break;
          }
        case LZWDEC_FILTER_ARG:
          {
            break;
          }
#ifdef HAVE_LIBZ
        case FLATEDEC_FILTER_ARG:
          {
            pdf_stm_install_fdec_filter (input,
                                         PDF_STM_FILTER_READ,
                                         NO_PREDICTION,
                                         0,
                                         0,
                                         0);
            break;
          }
#endif /* HAVE_LIBZ */
        case RUNLENGTHDEC_FILTER_ARG:
          {
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
        case '?':
          {
            /* Error, usage and exit */
            fprintf (stdout, "%s\n", pdf_filter_usage_msg);
            exit (1);
            break;
          }
        }
    }

  /* Read stdin into the pdf stream */
  line = NULL;
  line_bytes = 0;
  while ((readed = getline (&line, &line_bytes, stdin)) != EOF)
    {
      pdf_stm_write (input,
                     line,
                     readed);
      free (line);
      line = NULL;
      line_bytes = 0;
    }

  /* Write the filtered pdf stream contents into stdout */
  output_buffer = (unsigned char *) xmalloc (pdf_stm_size (input));
  pdf_stm_seek (input, 0);
  readed = pdf_stm_read (input, 
                         &output_buffer,
                         pdf_stm_size (input));
  fwrite (output_buffer,
          readed,
          1,
          stdout);

  /* Cleanup */
  free (output_buffer);
  pdf_stm_close (input);
  
  return 0;
}


/* End of pdf_filter.c */

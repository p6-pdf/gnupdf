/* -*- mode: C -*-
 *
 *       File:         runtests.c
 *       Date:         Sat Feb 23 21:40:43 2008
 *
 *       GNU PDF Library - Unit testing driver
 *
 */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include <check.h>

char *program_name; /* Initialized in main () */

extern Suite *tsuite_alloc (void);
extern Suite *tsuite_stm (void);
extern Suite *tsuite_list (void);
extern Suite *tsuite_text (void);
extern Suite *tsuite_hash (void);
extern Suite *tsuite_types (void);
extern Suite *tsuite_time (void);
extern Suite *tsuite_crypt (void);
extern Suite *tsuite_error (void);
extern Suite *tsuite_fp (void);
extern Suite *tsuite_token (void);
extern Suite *tsuite_fsys (void);


/* Command line arguments used in getopt.  */
enum
{
  HELP_ARG,
  VERSION_ARG,
  MODULE_ARG,
  FUNCTION_ARG
};

static const struct option gnu_longoptions[] =
  {
    {"help", no_argument, NULL, HELP_ARG},
    {"module", required_argument, NULL, MODULE_ARG},
    {"function", required_argument, NULL, FUNCTION_ARG},
    {NULL, 0, NULL, 0}
  };

char *runtests_version_msg = "runtests (GNU PDF Library)\n\
Copyright (C) 2010 Free Software Foundation.\n\
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n\
This is free software: you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.";

char *runtests_help_msg = "\
Usage: runtests [OPTION]...\n\
Run the unit tests of the GNU PDF Library.\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
  -m, --module=NAME                   execute the unit tests of the specified module.\n\
  -f, --function=NAME                 execute the unit tests of the specified function.\n\
      --help                          print a help message and exit.\n\
      --version                       show recinf version and exit.\n\
\n\
Examples:\n\
\n\
        runtests\n\
        runtests -m fsys\n\
        runtests -f pdf_fsys_file_close\n\
\n\
Report runtests bugs to pdf-devel@gnu.org\n\
GNU PDF home page: <http://www.gnupdf.org/>\n\
General help using GNU software: <http://www.gnu.org/gethelp/>\
";
  
int
main (int argc, char **argv)
{
  int failures;
  SRunner *sr;
  char *sname, *tcname;
  char c, ret;

  program_name = strdup (argv[0]);
  
  sname = NULL;
  tcname = NULL;

  /* Parse command line arguments.  */
  while ((ret = getopt_long (argc,
                             argv,
                             "m:f:",
                             gnu_longoptions,
                             NULL)) != -1)
    {
      c = ret;
      switch (c)
        {
        case HELP_ARG:
          {
            fprintf (stdout, "%s\n", runtests_help_msg);
            exit (EXIT_SUCCESS);
            break;
          }
        case VERSION_ARG:
          {
            fprintf (stdout, "%s\n", runtests_version_msg);
            exit (EXIT_SUCCESS);
            break;
          }
        case MODULE_ARG:
        case 'm':
          {
            sname = strdup (optarg);
            break;
          }
        case FUNCTION_ARG:
        case 'f':
          {
            tcname = strdup (optarg);
            break;
          }
        default:
          {
            exit (EXIT_FAILURE);
          }
        }
    }

  /* Create empty suite runner */
  sr = srunner_create (NULL);
  /* Start adding suites */
  srunner_add_suite (sr, tsuite_alloc ());
  srunner_add_suite (sr, tsuite_list ());
  srunner_add_suite (sr, tsuite_text ());
  srunner_add_suite (sr, tsuite_hash ());
  srunner_add_suite (sr, tsuite_types ());
  srunner_add_suite (sr, tsuite_time ());
  srunner_add_suite (sr, tsuite_crypt ());
  srunner_add_suite (sr, tsuite_error ());
  srunner_add_suite (sr, tsuite_stm ());
  srunner_add_suite (sr, tsuite_fp ());
  srunner_add_suite (sr, tsuite_token ()); 
  srunner_add_suite (sr, tsuite_fsys());

  /* Set log file */
  srunner_set_log (sr, "ut.log");

  /* Run all test suites */
  srunner_run (sr, sname, tcname, CK_ENV);
  failures = srunner_ntests_failed (sr);
  srunner_free (sr);
  
  return (failures == 0) ? 0 : 1;
}

/* End of runtests.c */


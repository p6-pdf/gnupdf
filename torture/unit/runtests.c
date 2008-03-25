/* -*- mode: C -*- Time-stamp: ""
 *
 *       File:         runtests.c
 *       Date:         Sat Feb 23 21:40:43 2008
 *
 *       GNU PDF Library - Unit testing driver
 *
 */

#include <check.h>

extern Suite *tsuite_alloc (void);
extern Suite *tsuite_stm (void);
extern Suite *tsuite_list (void);
extern Suite *tsuite_text (void);

int
main (int argc, char **argv)
{
  int failures;
  SRunner *sr;

  /* Create empty suite runner */
  sr = srunner_create (NULL);
  /* Start adding suites */
  srunner_add_suite (sr, tsuite_alloc ());
  srunner_add_suite (sr, tsuite_stm ());
  srunner_add_suite (sr, tsuite_list ());
  srunner_add_suite (sr, tsuite_text ());
  /* Set log file */
  srunner_set_log (sr, "ut.log");
  /* Run all test suites */
  srunner_run_all (sr, CK_VERBOSE);
  failures = srunner_ntests_failed (sr);
  srunner_free (sr);
  
  return (failures == 0) ? 0 : 1;
}

/* End of runtests.c */


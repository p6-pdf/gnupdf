/* -*- mode: C -*- Time-stamp: "08/02/23 22:01:24 jemarch"
 *
 *       File:         runtests.c
 *       Date:         Sat Feb 23 21:40:43 2008
 *
 *       GNU PDF Library - Unit testing driver
 *
 */

#include <check.h>

extern Suite *tsuite_stm (void);

int
main (int argc, char **argv)
{
  int failures;
  SRunner *sr;
  Suite *s;

  s = tsuite_stm ();

  sr = srunner_create (s);
  srunner_run_all (sr, CK_VERBOSE);
  failures = srunner_ntests_failed (sr);
  srunner_free (sr);
  
  return (failures == 0) ? 0 : 1;
}

/* End of runtests.c */

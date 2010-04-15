/* -*- mode: C -*-
 *
 *       File:         check.c
 *       Date:         Fri Apr 18 20:29:04 2008
 *
 *       GNU PDF Library - Simple way to run unit tests without Check...
 *
 */

/* Copyright (C) 2008 Free Software Foundation, Inc. */

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

#include <check.h>


#define INITIAL_SIZE 128

FILE *pf_log;
char *suite_running;
char *tc_running;


Suite *suite_create (const char *name)
{
  Suite *newelem = (Suite *)calloc(1,sizeof(Suite));
  if(newelem != NULL)
    {
      newelem->name = (name != NULL) ? strdup(name) : NULL;
      newelem->n = 0;
      newelem->size = INITIAL_SIZE;
      newelem->elements = (TCase **)calloc(newelem->size, sizeof(TCase *));
    }
  return newelem;
}

void suite_add_tcase (Suite *s, TCase *tc)
{
  if(s->n == s->size)
    {
      s->size *= 2;
      s->elements = (TCase **)realloc(s->elements, sizeof(TCase *)*s->size);
    }
  s->elements[s->n++] = tc;
}

TCase *tcase_create (const char *name)
{
  TCase *newelem = (TCase *)calloc(1,sizeof(TCase));
  if(newelem != NULL)
    {
      newelem->name = (name != NULL) ? strdup(name) : NULL;
      newelem->n = 0;
      newelem->size = INITIAL_SIZE;
      newelem->elements = (TFun *)calloc(newelem->size, sizeof(TFun));
    }
  return newelem;
}

void tcase_add_test (TCase *tc, TFun tf)
{
  if(tc->n == tc->size)
    {
      tc->size *= 2;
      tc->elements = (TFun *)realloc(tc->elements, sizeof(TFun)*tc->size);
    }
  tc->elements[tc->n++] = tf;
}

SRunner *srunner_create (Suite *s)
{
  SRunner *newelem = (SRunner *)calloc(1,sizeof(SRunner));
  if(newelem != NULL)
    {
      newelem->n = 0;
      newelem->size = INITIAL_SIZE;
      newelem->elements = (Suite **)calloc(newelem->size, sizeof(Suite *));
    }
  if(s != NULL)
    {
      srunner_add_suite(newelem, s);
    }
  return newelem;
}

void srunner_add_suite (SRunner *sr, Suite *s)
{
  if(sr->n == sr->size)
    {
      sr->size *= 2;
      sr->elements = (Suite **)realloc(sr->elements, sizeof(Suite *)*sr->size);
    }
  sr->elements[sr->n++] = s;
}

void srunner_free (SRunner *sr)
{
  extern FILE *pf_log;
  long i;
  /* Walk test suites in suite runner */
  for(i = 0; i < sr->n; i++)
    {
      Suite *suite = sr->elements[i];
      long j;
      /* Walk test cases in suite */
      for(j=0; j < suite->n; j++)
        {
          TCase *tc = suite->elements[j];
          free(tc->name);
          free(tc->elements);
          free(tc);
        }
      free(suite->name);
      free(suite->elements);
      free(suite);
    }
  free(sr->elements);
  free(sr);
  
  /* Close log file if it was open */
  if(pf_log != NULL)
    {
      fclose(pf_log);
    }
}

void srunner_run_all (SRunner *sr, enum print_output print_mode)
{
  extern char *tc_running;
  extern FILE *pf_log;
  long i;
  fprintf(stdout,"[No-Check] Running all suites...\n\n\n");
  /* Walk test suites in suite runner */
  for(i = 0; i < sr->n; i++)
    {
      long j;
      Suite *suite = sr->elements[i];
      if(pf_log != NULL)
        {
          fprintf(pf_log,"Running suite %s\n", suite->name);
        }
      /* Walk test cases in suite */
      for(j=0; j < suite->n; j++)
        {
          long k;
          TCase *tc = suite->elements[j];
          tc_running = tc->name;
          /* Walk tests in test case... */
          for(k=0; k < tc->n; k++)
            {
              TFun function = tc->elements[k];
              /* Execute TEST */
              function(0);
            }
        }
    }
}
                


void _fail_unless (int result,
                   const char *test,
                   const char *file,
                   int line,
                   const char *expr,
                   ...)
{
  extern char *tc_running;
  extern FILE *pf_log;
  va_list args;
  FILE *pfs[2];
  short i;
  
  pfs[0] = stdout;
  pfs[1] = pf_log;
  
  va_start (args, expr);
  for(i=0; i <=1; i++)
    {
      if(pfs[i] != NULL)
        {
          fprintf(pfs[i], "%s:%d:%c:%s:%s: ",
                  file, line,
                  (result ? 'P' : 'F'),
                  tc_running, test);
          vfprintf (pfs[i], expr, args);
          fprintf(pfs[i], "\n");
        }
    }
  va_end (args);
}

void tcase_fn_start (const char *fname, const char *file, int line)
{
  /* Do nothing */
}


void srunner_set_log (SRunner *sr, const char *fname)
{
  extern FILE *pf_log;

  /* Open log file */
  pf_log = fopen(fname, "w");
}

int srunner_ntests_failed (SRunner *sr)
{
  /* Do nothing */
  return 0;
}

void srunner_set_fork_status (SRunner *sr, enum print_output print_mode)
{
  /* Do nothing */
}

/* End of check.c */

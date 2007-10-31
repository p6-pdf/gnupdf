/* -*- mode: C -*- Time-stamp: "07/10/31 19:27:30 jemarch"
 *
 *       File:         test-base.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Sat Sep  8 22:32:13 2007
 *
 *       GNU PDF Library - Test suite for the base module
 *
 */

/* Copyright (C) 2007 Free Software Foundation, Inc. */

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

#ifdef HAVE_MALLOC_H
 #include <malloc.h>
#else
 #include <stdlib.h>
#endif /* HAVE_MALLOC_H */

#include <xalloc.h>
#include <string.h>
#include <stdio.h>
#include <pdf_base.h>

int
main ()
{
  int success;
  char *str;
  int res_int;
  long res_long;
  double res_double;

  success = PDF_TRUE;

  /* string to integer */
  str = (char *) xmalloc (4);
  strcpy (str, "100");
  if (pdf_str2int (str, &res_int) == PDF_ERROR)
    {
      printf ("Error converting a correct string to integer: %s\n", str);
      success = PDF_FALSE;
    }
  if (res_int != 100)
    {
      printf ("Incorrect result converting \"%s\" to integer\n", str);
      success = PDF_FALSE;
    }
  free (str);

  str = (char *) xmalloc (4);
  strcpy (str, "20a");
  if (pdf_str2int (str, &res_int) == PDF_OK)
    {
      printf ("Success converting an incorrect string to integer: %s\n", str);
      success = PDF_FALSE;
    }

  free (str);

  str = (char *) xmalloc (4);
  strcpy (str, "a20");
  if (pdf_str2int (str, &res_int) == PDF_OK)
    {
      printf ("Success converting an incorrect string to integer: %s\n", str);
      success = PDF_FALSE;
    }
  free (str);

  str = (char *) xmalloc (4);
  strcpy (str, "   ");
  if (pdf_str2int (str, &res_int) == PDF_OK)
    {
      printf ("Success converting an empty string to integer\n", str);
      success = PDF_FALSE;
    }
  free (str);

  /* string to long */
  str = (char *) xmalloc (4);
  strcpy (str, "100");
  if (pdf_str2long (str, &res_long) == PDF_ERROR)
    {
      printf ("Error converting a correct string to long: %s\n", str);
      success = PDF_FALSE;
    }
  if (res_long != 100)
    {
      printf ("Incorrect result converting \"%s\" to long\n", str);
      success = PDF_FALSE;
    }
  free (str);

  str = (char *) xmalloc (4);
  strcpy (str, "20a");
  if (pdf_str2long (str, &res_long) == PDF_OK)
    {
      printf ("Success converting an incorrect string to long: %s\n", str);
      success = PDF_FALSE;
    }
  free (str);

  str = (char *) xmalloc (4);
  strcpy (str, "a20");
  if (pdf_str2long (str, &res_long) == PDF_OK)
    {
      printf ("Success converting an incorrect string to long: %s\n", str);
      success = PDF_FALSE;
    }
  free (str);

  str = (char *) xmalloc (4);
  strcpy (str, "   ");
  if (pdf_str2long (str, &res_long) == PDF_OK)
    {
      printf ("Success converting an empty string to long\n", str);
      success = PDF_FALSE;
    }
  free (str);


  /* string to double */

  str = (char *) xmalloc (4);
  strcpy (str, "1.2");
  if (pdf_str2double (str, &res_double) == PDF_ERROR)
    {
      printf ("Error converting a correct string to double: %s\n", str);
      success = PDF_FALSE;
    }
  free (str);

  str = (char *) xmalloc (4);
  strcpy (str, "2..");
  if (pdf_str2double (str, &res_double) == PDF_OK)
    {
      printf ("Success converting an incorrect string to double: %s\n", str);
      success = PDF_FALSE;
    }
  free (str);

  str = (char *) xmalloc (4);
  strcpy (str, "..2");
  if (pdf_str2double (str, &res_double) == PDF_OK)
    {
      printf ("Success converting an incorrect string to double: %s\n", str);
      success = PDF_FALSE;
    }
  free (str);

  str = (char *) xmalloc (4);
  strcpy (str, "   ");
  if (pdf_str2double (str, &res_double) == PDF_OK)
    {
      printf ("Success converting an empty string to double\n", str);
      success = PDF_FALSE;
    }
  free (str);


  
  if (success)
    {
      return 0;
    }
  else
    {
      return 1;
    }
}

/* End of test-base.c */

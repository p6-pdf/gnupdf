/* -*- mode: C -*- Time-stamp: "07/07/10 12:18:56 jemarch"
 *
 *       File:         test-stm_openclose.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Mon Jul  9 22:51:16 2007
 *
 *       GNU PDF Library - Testsuite, opening and closing streams
 *
 */

/* Copyright (C) 2007 Jose E. Marchesi */

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
#include <stdio.h>
#include <pdf_stm.h>

int 
main ()
{
  int status;
  pdf_stm_t stm;

  /* Test the file backend */
  stm = pdf_create_file_stm ("non-existing-file", 
                             PDF_STM_OPEN_MODE_READ);
  if (stm != NULL)
    {
      printf ("Error trying to open a non-existing file.\n");
      pdf_stm_close (stm);
      status = PDF_ERROR;
    }

  stm = pdf_create_file_stm ("torture-file-1", 
                             PDF_STM_OPEN_MODE_READ);

  if (stm == NULL)
    {
      printf ("Error trying to open a file in read mode.\n");
      status = PDF_ERROR;
    }
  else
    {
      pdf_stm_close (stm);
    }

  stm = pdf_create_file_stm ("temporal-test-file", 
                             PDF_STM_OPEN_MODE_WRITE);

  if (stm == NULL)
    {
      printf ("Error trying to open a file in write mode.\n");
      status = PDF_ERROR;
    }
  else
    {
      if (!pdf_stm_close (stm))
        {
          printf ("Error trying to close a file.\n");
          status = PDF_ERROR;
        }      
    }

  stm = pdf_create_file_stm ("temporal-test-file", 
                             PDF_STM_OPEN_MODE_WRITE);

  if (stm == NULL)
    {
      printf ("Error trying to open a file in write mode.\n");
      status = PDF_ERROR;
    }
  else
    {
      if (!pdf_stm_close (stm))
        {
          printf ("Error trying to close a file.\n");
          status = PDF_ERROR;
        }      
    }

  stm = pdf_create_file_stm ("temporal-test-file",
                             PDF_STM_OPEN_MODE_APPEND);

  if (stm == NULL)
    {
      printf ("Error trying to open a file in append mode.\n");
      status = PDF_ERROR;
    }
  else
    {
      if (!pdf_stm_close (stm))
        {
          printf ("Error trying to close a file.\n");
          status = PDF_ERROR;
        }      
    }

  /* Test the memory backend */


  return status;
}

/* End of test-stm_openclose.c */

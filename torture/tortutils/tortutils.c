/* -*- mode: C -*-
 *
 *       File:         tortutils.c
 *       Date:         Mon Nov 24 22:18:12 2008
 *
 *       GNU PDF Library - Utility functions for testing
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <xalloc.h>

#include <tortutils.h>


/* Path containing the test data. 
 * TEST_DATA_PATH should be defined by the preprocessor */
static char *tortu_data_path = TEST_DATA_PATH;

/*
 * Public functions
 */

char *
tortu_get_data_file_path (char *datafilename)
{
  char *path;

#ifdef PDF_HOST_WIN32
  static char *separator = "\\";
#else
  static char *separator = "/";
#endif /* PDF_HOST_WIN32 */

  path = xstrdup (tortu_data_path);
  path = xrealloc (path, strlen(path) 
                   + strlen(separator) 
                   + strlen(datafilename)
                   + 2);
  path = strcat (path, separator);
  path = strcat (path, datafilename);

  return path;
}

int
tortu_data_file_p (char *datafilename)
{
  int ret;
  struct stat file_stat;
  char *file_path;

  file_path = tortu_get_data_file_path (datafilename);
  if (stat (file_path, &file_stat) == 0)
    {
      ret = TORTU_TRUE;
    }
  else
    { 
      ret = TORTU_FALSE;
    }

  free (file_path);

  return ret;
}

int
tortu_cmp_data_file (char *datafilename,
                     off_t offset,
                     char *buf,
                     size_t buf_size)
{
  int ret;
  char *file_path;
  FILE *fp;
  char *file_data;
  size_t readed;

  if (!tortu_data_file_p (datafilename))
    {
      return TORTU_FALSE;
    }

  file_path = tortu_get_data_file_path (datafilename);

  /* Open the file for reading */
  fp = fopen (file_path, "r");
  if (fp == NULL)
    {
      free (file_path);
      return TORTU_FALSE;
    }

  /* Seek to the requested position */
  if (fseek (fp, offset, SEEK_SET) != offset)
    {
      free (file_path);
      return TORTU_FALSE;
    }

  /* Read the requested chunk from the file */
  file_data = xmalloc (buf_size);
  readed = fread (file_data, 1, buf_size, fp);
  if ((readed == buf_size) &&
      (memcmp (file_data, buf, buf_size) == 0))
    {
      ret = TORTU_TRUE;
    }
  else
    {
      ret = TORTU_FALSE;
    }

  free (file_path);              
  free (file_data);
  return ret;
}

/* End of tortutils.c */

/* -*- mode: C -*- Time-stamp: "08/11/25 20:53:49 jemarch"
 *
 *       File:         tortutils.h
 *       Date:         Mon Nov 24 22:11:23 2008
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

#ifndef TORTUTILS_H
#define TORTUTILS_H

#include <config.h>

#if defined _WIN32_ || defined __WIN32__
#  define TORTU_HOST_WIN32
#  include <windows.h>
#endif

#define TORTU_TRUE 1
#define TORTU_FALSE 0

/*
 * Functions
 */

/* Get the path to the data files. The returned pointer should
   be freed by the caller */
char *tortu_get_data_path (char *datafilename);

/* Return whether a data file exists */
int tortu_data_file_p (char *datafilename);

/* Compare the contents of a memory buffer with the contents of a data
   file */
int tortu_cmp_data_file (char *datafilename, off_t offset, char *buf, size_t buf_size);

#endif /* tortutils.h */

/* End of tortutils.h */

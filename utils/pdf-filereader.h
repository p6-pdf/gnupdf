/* -*- mode: C -*- Time-stamp: "09/11/25 00:11:39 jemarch"
 *
 *       File:         pdf-filereader.h
 *       Date:         Thu Dec 2 23:35:55 2010
 *
 *       GNU PDF Library - Process input/output with the 
 *                         standard PDF filesystem.
 *
 */

/* Copyright (C) 2010 Free Software Foundation, Inc. */

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

#ifndef PDF_FILEREADER_H
#define PDF_FILEREADER_H

/* Command line arguments */

enum
{
  HELP_ARG,
  USAGE_ARG,
  VERSION_ARG,
  INFILE_ARG,
  OUTFILE_ARG,
  PROPS_ARG,
  DISK_ARG,
};

struct pdf_filereader_args_s {
  char * program_name;
  char * in_fname;
  char * out_fname;
  pdf_bool_t print_props;
  pdf_bool_t use_disk_fs;
};

typedef struct pdf_filereader_args_s pdf_filereader_args_t;


#endif /* pdf_filereader.h */

/* End of pdf_filereader.h */

/* -*- mode: C -*- Time-stamp: "09/02/03 22:47:52 jemarch"
 *
 *       File:         pdf-filter.h
 *       Date:         Tue Jul 10 18:40:55 2007
 *
 *       GNU PDF Library - Process input/output with the 
 *                         standard PDF filters.
 *
 */

/* Copyright (C) 2007, 2008 Free Software Foundation, Inc. */

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

#ifndef PDF_FILTER_H
#define PDF_FILTER_H

#include <config.h>

/* Command line arguments */

enum
{
  HELP_ARG,
  USAGE_ARG,
  VERSION_ARG,
  READ_ARG,
  INFILE_ARG,
  OUTFILE_ARG,
  CACHE_ARG,
  NULL_FILTER_ARG,
  ASCIIHEXDEC_FILTER_ARG,
  ASCIIHEXENC_FILTER_ARG,
#if 0
  ASCII85DEC_FILTER_ARG,
  ASCII85ENC_FILTER_ARG,
  LZWENC_FILTER_ARG,
  LZWDEC_FILTER_ARG,
  CCITTFAXDEC_FILTER_ARG,
  DCTDEC_FILTER_ARG,
  JXPDEC_FILTER_ARG,
  PREDENC_FILTER_ARG,
  PREDDEC_FILTER_ARG,
#endif /* 0 */
#ifdef HAVE_LIBZ
  FLATEDEC_FILTER_ARG,
  FLATEENC_FILTER_ARG,
#endif /* HAVE_LIBZ */
  RUNLENGTHDEC_FILTER_ARG,
  RUNLENGTHENC_FILTER_ARG,
#ifdef HAVE_LIBJBIG2DEC
  JBIG2DEC_FILTER_ARG,
  JBIG2DEC_GLOBAL_SEGMENTS_ARG,
  JBIG2DEC_PAGE_SIZE,
#endif /* HAVE_LIBJBIG2DEC */
  MD5ENC_FILTER_ARG,
  KEY_ARG,
  AESENC_FILTER_ARG,
  AESDEC_FILTER_ARG,
  V2ENC_FILTER_ARG,
  V2DEC_FILTER_ARG
};

#endif /* pdf_filter.h */

/* End of pdf_filter.h */

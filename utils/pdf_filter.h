/* -*- mode: C -*- Time-stamp: "07/10/23 12:59:49 jemarch"
 *
 *       File:         pdf_filter.h
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Tue Jul 10 18:40:55 2007
 *
 *       GNU PDF Library - Process input/output with the 
 *                         standard PDF filters.
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

#ifndef PDF_FILTER_H
#define PDF_FILTER_H

#include <config.h>

/* Command line arguments */

enum
{
  HELP_ARG,
  USAGE_ARG,
  VERSION_ARG,
  NULL_FILTER_ARG,
  ASCIIHEXDEC_FILTER_ARG,
  ASCIIHEXENC_FILTER_ARG,
  ASCII85DEC_FILTER_ARG,
  ASCII85ENC_FILTER_ARG,
  LZWENC_FILTER_ARG,
  LZWDEC_FILTER_ARG,
  FLATEDEC_FILTER_ARG,
  FLATEENC_FILTER_ARG,
  RUNLENGTHDEC_FILTER_ARG,
  RUNLENGTHENC_FILTER_ARG,
  CCITTFAXDEC_FILTER_ARG,
  JBIG2DEC_FILTER_ARG,
  DCTDEC_FILTER_ARG,
  JXPDEC_FILTER_ARG,
  LZW_EARLY_CHANGE_ARG,
  PREDENC_FILTER_ARG,
  PREDDEC_FILTER_ARG,
  PREDENC_TYPE_ARG,
  PREDDEC_TYPE_ARG,
  PRED_COLORS_ARG,
  PRED_BPC_ARG,
  PRED_COLUMNS_ARG
};

#endif /* pdf_filter.h */

/* End of pdf_filter.h */

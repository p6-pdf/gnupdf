/* -*- mode: C -*- Time-stamp: "07/07/31 21:39:39 jemarch"
 *
 *       File:         pdf_filter.h
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Tue Jul 10 18:40:55 2007
 *
 *       GNU PDF Library - Process input/output with the 
 *                         standard PDF filters.
 *
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
  LZWDEC_FILTER_ARG,
  FLATEDEC_FILTER_ARG,
  FLATEENC_FILTER_ARG,
  RUNLENGTHDEC_FILTER_ARG,
  RUNLENGTHENC_FILTER_ARG,
  CCITTFAXDEC_FILTER_ARG,
  JBIG2DEC_FILTER_ARG,
  DCTDEC_FILTER_ARG,
  JXPDEC_FILTER_ARG,
  PREDENC_FILTER_ARG,
  PREDDEC_FILTER_ARG,
  TIFF_ARG,
  PNG_SUB_ARG,
  PNG_UP_ARG,
  PNG_AVERAGE_ARG,
  PNG_PAETH_ARG,
  COLORS_ARG,
  BPC_ARG,
  COLUMNS_ARG
};

struct predictor_args_s
{
  int colors;
  int colors_p;
  int bpc;
  int bpc_p;
  int columns;
  int columns_p;
};

#endif /* pdf_filter.h */

/* End of pdf_filter.h */

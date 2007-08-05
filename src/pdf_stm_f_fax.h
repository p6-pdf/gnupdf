/* -*- mode: C -*- Time-stamp: "07/08/05 20:13:45 jemarch"
 *
 *       File:         pdf_stm_f_fax.h
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Sun Aug  5 19:46:07 2007
 *
 *       GNU PDF Library - CCITT Fax encoder/decoder
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

#ifndef PDF_STM_F_FAX_H
#define PDF_STM_F_FAX_H

#include <config.h>
#include <pdf_base.h>

/* Configuration data */

enum pdf_stm_f_fax_mode_t
{
  PDF_STM_F_FAX_MODE_ENCODE,
  PDF_STM_F_FAX_MODE_DECODE
};

struct pdf_stm_f_fax_conf_s
{
  int mode;
  int k;                         /* A code identifying the encoding
                                    scheme used: a negative value
                                    means to use a pure
                                    two-dimensional encoding (Group
                                    4). A zero value means to use a
                                    pure one-dimensional encoding
                                    (Group 3, 1-D). Finally, a
                                    positive value means to use a
                                    mixed one- and two- dimensional
                                    encoding (Group 3, 2-D), in which
                                    a line encoded one-dimensionally
                                    can be followed by at most `k-1'
                                    lines encoded two-dimensionally */
  int end_of_line_p;             /* A flag indicating whether
                                    end-of-line bit patterns are
                                    required to be present in the
                                    encoding. The CCITTFaxDecode
                                    filter always accepts end-of-line
                                    bit patterns, but requires them
                                    only if EndOfLine is true. Default
                                    value is false */ 
  int encoded_byte_align_p;      /* A flag indicating whether the
                                    filter expects extra 0 bits before
                                    each encoded line so that the line
                                    begins on a byte boundary. If
                                    true, the filter skips over
                                    encoded bits to begin decoding
                                    each line at a byte boundary. If
                                    false, the filter does not expect
                                    extra bits in the encoded
                                    representation. Default value is
                                    false */
  int columns;                   /* The width of the image in
                                    pixels. If the value is not a
                                    multiple of 8, the filter adjusts
                                    the width of the unencoded image
                                    to the next multiple of 8 so that
                                    each line starts on a byte
                                    boundary. Default value is 1728 */
  int rows;                      /* The height of the image in scan
                                    lines. If the value is 0 or
                                    absent, the image's height is not
                                    predetermined, and the encoded
                                    data must be terminated by an
                                    end-of-block bit pattern or by the
                                    end of the filter's data. Default
                                    value is 0 */
  int end_of_block_p;            /* A flag indicating whether the
                                    filter expects the encoded data to
                                    be terminated by an end-of-block
                                    pattern, overriding the Rows
                                    parameter. If false, the filter
                                    stops when it has decoded the
                                    number of lines indicated by Rows
                                    or when its data has been
                                    exhausted, whichever occurs
                                    first. The end-of-block pattern is
                                    the CCITT end-of-facsimile-block
                                    (EOFB) or return-to-control (RTC)
                                    appropriate for the K
                                    parameter. Default value is
                                    true */
  int blackls1_p;                /* A flag indicating whether 1 bits
                                    are to be interpreted as black
                                    pixels and 0 bits as white pixels,
                                    the reverse of the normal PDF
                                    convention for image data. Default
                                    value is false */
  int damaged_rows_before_error; /* The number of damaged rows of data
                                    to be tolerated before an error
                                    occurs. This entry applies only if
                                    EndOfLine is true and K is
                                    non-negative. Tolerating a damaged
                                    row means locating its end in the
                                    encoded data by searching for an
                                    EndOfLine pattern and then
                                    substituting decoded data from the
                                    previous row if the previous row
                                    was not damaged, or a white scan
                                    line if the previous row was also
                                    damaged. Default value is 0 */
};

typedef struct pdf_stm_f_fax_conf_s *pdf_stm_f_fax_conf_t;

/* Private data */

struct pdf_stm_f_fax_data_s
{
  int mode;
  int k;
  int end_of_line_p;
  int encoded_byte_align_p;
  int columns;
  int rows;
  int end_of_block_p;
  int blackls1_p;
  int damaged_rows_before_error;
};

typedef struct pdf_stm_f_fax_data_s *pdf_stm_f_fax_data_t;

/* Filter API implementation */

int pdf_stm_f_fax_init (void **filter_data, void *conf_data);
int pdf_stm_f_fax_apply (void *filter_data,
                         pdf_char_t *in, pdf_stm_pos_t in_size,
                         pdf_char_t **out, pdf_stm_pos_t *out_size);
int pdf_stm_f_fax_dealloc (void **filter_data);

#endif /* pdf_stm_f_fax.h */

/* End of pdf_stm_f_fax.h */

/* -*- mode: C -*-
 *
 *       File:         pdf-text-filter.c
 *       Date:         Thu Mar 27 20:25:17 2008
 *
 *       GNU PDF Library - Text Module - pdf_text_filter test case
 *
 */

/* Copyright (C) 2008-2011 Free Software Foundation, Inc. */

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
#include <string.h>
#include <pdf.h>
#include <check.h>
#include <base/text/pdf-text-test-common.h>
#include <pdf-test-common.h>

struct test_params {
  /* Test index */
  int idx;

  /* Test parameters */
  pdf_u32_t filters;

  /* Some tests need a specific language */
  const pdf_char_t *language;

  /* Input string setup */
  const pdf_char_t *input;
  long input_size; /* -1 if NUL-terminated */
  enum pdf_text_unicode_encoding_e input_enc;

  /* Expected string setup */
  const pdf_char_t *expected;
  long expected_size; /* -1 if NUL-terminated */
  enum pdf_text_unicode_encoding_e expected_enc;
};

/*
 * Test: pdf_text_get_unicode_001-069
 * Description:
 *
 *   Check text filters.
 *
 * Success conditions:
 *    1. pdf_text_filter() returns PDF_TRUE
 *    2. The returned string must be the expected one, not NUL terminated.
 *    3. The returned length must be the expected one.
 */

/* Here are all the parameters we'll test, and their different
 * combinations. */
static const struct test_params tests[] = {
  /* Apply the `Normalize line endings' filter to a text object which contains
   * different types of line endings. */
  {
    1,
    PDF_TEXT_FILTER_LINE_ENDINGS,
    NULL,
    "GNU's Not Unix\r\nGNU's Not Unix\rGNU's Not Unix\nGNU's Not Unix\xC2\x85",
    -1,
    PDF_TEXT_UTF8,
#ifdef PDF_HOST_WIN32
    "GNU's Not Unix\r\nGNU's Not Unix\r\nGNU's Not Unix\r\nGNU's Not Unix\r\n",
#else
    "GNU's Not Unix\nGNU's Not Unix\nGNU's Not Unix\nGNU's Not Unix\n",
#endif
    -1,
    PDF_TEXT_UTF8
  },

  /* Apply the `Normalize line endings' filter to an empty text object. */
  { 2, PDF_TEXT_FILTER_LINE_ENDINGS, NULL, NULL, 0, PDF_TEXT_UTF8, NULL, 0, PDF_TEXT_UTF8 },

  /* Apply the `Remove line endings' filter to a text object which contains
   * different types of line endings. */
  {
    3,
    PDF_TEXT_FILTER_REMOVE_LINE_ENDINGS,
    NULL,
    "GNU's Not Unix. \r\nGNU's Not Unix. \rGNU's Not Unix. \nGNU's Not Unix. \xC2\x85",
    -1,
    PDF_TEXT_UTF8,
    "GNU's Not Unix. GNU's Not Unix. GNU's Not Unix. GNU's Not Unix. ",
    -1,
    PDF_TEXT_UTF8
  },

  /* Apply the `Remove line endings' filter to an empty text object. */
  { 4, PDF_TEXT_FILTER_REMOVE_LINE_ENDINGS, NULL, NULL, 0, PDF_TEXT_UTF8, NULL, 0, PDF_TEXT_UTF8 },

  /* Apply the `Remove ampersands' filter to a text object which contains
   *single and double ampersands. */
  {
    5,
    PDF_TEXT_FILTER_REMOVE_AMP,
    NULL,
    "GNU & GNU && GNU",
    -1,
    PDF_TEXT_UTF8,
    "GNU GNU & GNU",
    -1,
    PDF_TEXT_UTF8
  },

  /* Apply the `Remove ampersands' filter to an empty text object. */
  { 6, PDF_TEXT_FILTER_REMOVE_AMP, NULL, NULL, 0, PDF_TEXT_UTF8, NULL, 0, PDF_TEXT_UTF8 },

  /* Apply the `Normalize with full width' filter to a text object that contains
   * code points that have a valid full width representation. */
  {
    7,
    PDF_TEXT_FILTER_NORM_WITH_FULL_WIDTH,
    NULL,
    "\x00\x00\x00\x20\x00\x00\x00G\x00\x00\x00N\x00\x00\x00U\x00\x00\x00 ",
    20,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\xFF\x27\x00\x00\xFF\x2E\x00\x00\xFF\x35\x00\x00\x00 ",
    20,
    PDF_TEXT_UTF32_BE
  },

  /* Apply the `Normalize with full width' filter to a text object that contains
   * code points that have a valid full width representation. */
  {
    8,
    PDF_TEXT_FILTER_NORM_WITH_FULL_WIDTH,
    NULL,
    "\x00\x00\x6C\x34\x00\x00\x00\x20\x00\x01\xD1\x1E",
    12,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x6C\x34\x00\x00\x00\x20\x00\x01\xD1\x1E",
    12,
    PDF_TEXT_UTF32_BE
  },

  /* Apply the `Normalize with full width' filter to an empty text object. */
  { 9, PDF_TEXT_FILTER_NORM_WITH_FULL_WIDTH, NULL, NULL, 0, PDF_TEXT_UTF8, NULL, 0, PDF_TEXT_UTF8 },

  /* Apply the `Upper case' filter to a text object that contains
   * simple-case-conversion characters. */
  {
    10,
    PDF_TEXT_FILTER_UPPER_CASE,
    NULL,
    "\x00\x00\x00\x20\x00\x00\x00""g""\x00\x00\x00""n""\x00\x00\x00""u""\x00\x00\x00\x20",
    20,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""G""\x00\x00\x00""N""\x00\x00\x00""U""\x00\x00\x00\x20",
    20,
    PDF_TEXT_UTF32_BE
  },

  /* Apply the `Upper case' filter to a text object that contains
   * special-case-conversion characters with no context condition. */
  {
    11,
    PDF_TEXT_FILTER_UPPER_CASE,
    NULL,
    "\x00\x00\x00\x20\x00\x00\x1E\x98\x00\x00\x1F\xE4\x00\x00\x1F\x80\x00\x00\x1F\xC4",
    20,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00\x57\x00\x00\x03\x0A\x00\x00\x03\xA1\x00\x00\x03\x13\x00\x00\x1F\x08\x00\x00\x03\x99\x00\x00\x03\x89\x00\x00\x03\x99",
    36,
    PDF_TEXT_UTF32_BE
  },

  /*  Apply the `Upper case' filter to a text object that contains
   *  special-case-conversion characters with Final_Sigma context condition.
   *
   * Final_Sigma condition in Special Case algorithms
   *  When the given character is preceded by a sequence consisting of a
   *  cased unicode point (with case information) and a case-ignorable
   *  (without case information) sequence of points. The given character
   *  must no be followed by a sequence consisting of a case-ignorable
   *  sequence and a cased letter.
   *
   * - 0x03A3 point is the one with the Final_Sigma condition
   * - 0x0055 point is the cased point
   * - 0x003A point is the case-ignorable point (MidLetter WB property)
   * - 0x0020 point (whitespace) is used as word separator to have three
   *    different words.
   * If condition is fulfilled, expected uppercase character for 0x03A3 is
   *  the same 0x03A3.
   */
  {
    /* 1st check, the sequence of previous case-ignorable points is empty */
    12,
    PDF_TEXT_FILTER_UPPER_CASE,
    NULL,
    "\x00\x00\x00\x47\x00\x00\x00\x4E\x00\x00\x00\x55\x00\x00\x00\x20\x00\x00\x00\x55\x00\x00\x03\xA3\x00\x00\x00\x20\x00\x00\x00\x47\x00\x00\x00\x4E\x00\x00\x00\x55",
    40,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x47\x00\x00\x00\x4E\x00\x00\x00\x55\x00\x00\x00\x20\x00\x00\x00\x55\x00\x00\x03\xA3\x00\x00\x00\x20\x00\x00\x00\x47\x00\x00\x00\x4E\x00\x00\x00\x55",
    40,
    PDF_TEXT_UTF32_BE
  },
  {
    /* 2nd check, the sequence of previous case-ignorable points is NOT empty */
    13,
    PDF_TEXT_FILTER_UPPER_CASE,
    NULL,
    "\x00\x00\x00\x47\x00\x00\x00\x4E\x00\x00\x00\x55\x00\x00\x00\x20\x00\x00\x00\x55\x00\x00\x00\x3A\x00\x00\x03\xA3\x00\x00\x00\x20\x00\x00\x00\x47\x00\x00\x00\x4E\x00\x00\x00\x55",
    44,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x47\x00\x00\x00\x4E\x00\x00\x00\x55\x00\x00\x00\x20\x00\x00\x00\x55\x00\x00\x00\x3A\x00\x00\x03\xA3\x00\x00\x00\x20\x00\x00\x00\x47\x00\x00\x00\x4E\x00\x00\x00\x55",
    44,
    PDF_TEXT_UTF32_BE
  },

  /* Apply the `Upper case' filter to a text object that contains
   * special-case-conversion characters with After_Soft_Dotted context
   * condition.
   *
   * 0307; 0307; ; ; lt After_Soft_Dotted; # COMBINING DOT ABOVE
   */
  {
    14,
    PDF_TEXT_FILTER_UPPER_CASE,
    "lt",
    "\x00\x00\x00\x20\x00\x00\x00""i""\x00\x00\x03\x07",
    12,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""I",
    8,
    PDF_TEXT_UTF32_BE
  },

  /* Apply the `Upper case' filter to a text object that contains
   * special-case-conversion characters with More_Above context condition.
   *
   * Code:0049; Lower:0069 0307; Title:0049; Upper:0049; lt More_Above; # LATIN CAPITAL LETTER I
   */
  {
    15,
    PDF_TEXT_FILTER_UPPER_CASE,
    "lt",
    "\x00\x00\x00\x20\x00\x00\x00\x49\x00\x00\x03\x0B",
    12,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00\x49\x00\x00\x03\x0B",
    12,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Upper case' filter to a text object that contains
   *   special-case-conversion characters with Before_Dot context condition.
   *
   * NOTE: Actually there is no special condition involving "Before_Dot" as is
   *       (there are a couple of conditions involving the negated one:
   *       Not_Before_Dot). But probably is a good idea to at least leave
   *       the test, just in case in future updates of the Unicode standard
   *       one of this type appears
  */
  { 16, PDF_TEXT_FILTER_UPPER_CASE, NULL, NULL, 0, PDF_TEXT_UTF32_BE, NULL, 0, PDF_TEXT_UTF32_BE },

  /*
   * Apply the `Upper case' filter to a text object that contains
   * special-case-conversion characters with After_I context condition.
   *
   * Code: 0307;Lower: ; Title: 0307; Upper: 0307; tr After_I; # COMBINING DOT ABOVE
   */
  {
    17,
    PDF_TEXT_FILTER_UPPER_CASE,
    "tr",
    "\x00\x00\x00\x20\x00\x00\x00""I""\x00\x00\x03\x07",
    12,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""I""\x00\x00\x03\x07",
    12,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Upper case' filter to a text object that contains
   * special-case-conversion characters with language-dependent context
   * condition.
   *
   * # When uppercasing, i turns into a dotted capital I
   * 0069; 0069; 0130; 0130; tr; # LATIN SMALL LETTER I
   */
  {
    18,
    PDF_TEXT_FILTER_UPPER_CASE,
    "tr",
    "\x00\x00\x00\x20\x00\x00\x00\x69",
    8,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x01\x30",
    8,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Upper case' filter to a text object that contains
   * special-case-conversion characters with language-dependent and After_I
   * context condition (to check two context conditions at the same time).
   *
   * Note: As long as After_I is always coming with a language condition (at least
   * in the Unicode standard 5.0), this test is equal to test #17
   *
   * Code: 0307;Lower: ; Title: 0307; Upper: 0307; tr After_I; # COMBINING DOT ABOVE
   */
  {
    19,
    PDF_TEXT_FILTER_UPPER_CASE,
    "tr",
    "\x00\x00\x00\x20\x00\x00\x00""I""\x00\x00\x03\x07",
    12,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""I""\x00\x00\x03\x07",
    12,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Upper case' filter to a text object that contains
   * special-case-conversion characters with language-dependent and
   * Not_Before_Dot context condition (to check two context conditions at the
   * same time, where one of them is negated with Not_).
   *
   * # When lowercasing, unless an I is before a dot_above, it turns into a
   *   dotless i.
   * 0049; 0131; 0049; 0049; tr Not_Before_Dot; # LATIN CAPITAL LETTER I
   */
  {
    20,
    PDF_TEXT_FILTER_UPPER_CASE,
    "tr",
    "\x00\x00\x00\x20\x00\x00\x00\x49\x00\x00\x00\x50",
    12,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00\x49\x00\x00\x00\x50",
    12,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Upper case' filter to a text object that contains
   * special-case-conversion characters with Final_Sigma context condition,
   * where the condition is not fulfilled.
   *
   * Final_Sigma condition in Special Case algorithms
   *  When the given character is preceded by a sequence consisting of a
   *  cased unicode point (with case information) and a case-ignorable
   *  (without case information) sequence of points. The given character
   *  must no be followed by a sequence consisting of a case-ignorable
   *  sequence and a cased letter.
   *
   * The condition won't be fulfilled because:
   *  1st check. The previous cased point is not available
   *  2nd check. A next cased point is available
   *
   * - 0x03A3 point is the one with the Final_Sigma condition
   * - 0x0055 point is the cased point
   * - 0x0027 point is the case-ignorable point
   * - 0x0020 point (whitespace) is used as word separator to have three
   *    different words.
   *
   * If condition is not fulfilled, expected uppercase character for 0x03A3 is
   *  the same 0x03A3.
   */
  {
    /* 1st check, the previous cased point is not available */
    21,
    PDF_TEXT_FILTER_UPPER_CASE,
    NULL,
    "\x00\x00\x00\x47\x00\x00\x00\x4E\x00\x00\x00\x55\x00\x00\x00\x20\x00\x00\x00\x27\x00\x00\x03\xA3\x00\x00\x00\x20\x00\x00\x00\x47\x00\x00\x00\x4E\x00\x00\x00\x55",
    40,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x47\x00\x00\x00\x4E\x00\x00\x00\x55\x00\x00\x00\x20\x00\x00\x00\x27\x00\x00\x03\xA3\x00\x00\x00\x20\x00\x00\x00\x47\x00\x00\x00\x4E\x00\x00\x00\x55",
    40,
    PDF_TEXT_UTF32_BE,
  },
  {
    /* 2nd check, the sequence of previous case-ignorable points is NOT empty */
    22,
    PDF_TEXT_FILTER_UPPER_CASE,
    NULL,
    "\x00\x00\x00\x47\x00\x00\x00\x4E\x00\x00\x00\x55\x00\x00\x00\x20\x00\x00\x00\x55\x00\x00\x00\x27\x00\x00\x03\xA3\x00\x00\x00\x55\x00\x00\x00\x20\x00\x00\x00\x47\x00\x00\x00\x4E\x00\x00\x00\x55",
    48,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x47\x00\x00\x00\x4E\x00\x00\x00\x55\x00\x00\x00\x20\x00\x00\x00\x55\x00\x00\x00\x27\x00\x00\x03\xA3\x00\x00\x00\x55\x00\x00\x00\x20\x00\x00\x00\x47\x00\x00\x00\x4E\x00\x00\x00\x55",
    48,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Upper case' filter to a text object that contains
   * special-case-conversion characters with After_Soft_Dotted context
   * condition, where the condition is not fulfilled.
   *
   * 0x030C has combining class 230, so condition should not be fulfilled
   */
  {
    23,
    PDF_TEXT_FILTER_UPPER_CASE,
    "lt",
    "\x00\x00\x00\x20\x00\x00\x00""i""\x00\x00\x03\x0C\x00\x00\x03\x07",
    16,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""I""\x00\x00\x03\x0C\x00\x00\x03\x07",
    16,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Upper case' filter to a text object that contains
   * special-case-conversion characters with More_Above context condition,
   * where the condition is not fulfilled.
   *
   * Code:0049; Lower:0069 0307; Title:0049; Upper:0049;
   * lt More_Above; # LATIN CAPITAL LETTER I
   * As 0xC0 has combining class 0, the condition must not be fullfilled
   */
  {
    24,
    PDF_TEXT_FILTER_UPPER_CASE,
    NULL,
    "\x00\x00\x00\x20\x00\x00\x00\x49\x00\x00\x00\xC0\x00\x00\x03\x0B",
    16,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00\x49\x00\x00\x00\xC0\x00\x00\x03\x0B",
    16,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Upper case' filter to a text object that contains
   * special-case-conversion characters with Before_Dot context condition,
   * where the condition is not fulfilled.
   *
   * NOTE: Actually there is no special condition involving "Before_Dot" as is
   *       (there are a couple of conditions involving the negated one:
   *       Not_Before_Dot). But probably is a good idea to at least leave
   *       the test, just in case in future updates of the Unicode standard
   *       one of this type appears
  */
  { 25, PDF_TEXT_FILTER_UPPER_CASE, NULL, NULL, 0, PDF_TEXT_UTF32_BE, NULL, 0, PDF_TEXT_UTF32_BE },

  /*
   * Apply the `Upper case' filter to a text object that contains
   * special-case-conversion characters with After_I context condition,
   * where the condition is not fulfilled.
   *
   * Code: 0307;Lower: ; Title: 0307; Upper: 0307; tr After_I; # COMBINING DOT ABOVE
   * 0x030C has combining class 230, so condition should not be fulfilled
   */
  {
    26,
    PDF_TEXT_FILTER_UPPER_CASE,
    NULL,
    "\x00\x00\x00\x20\x00\x00\x00""I""\x00\x00\x03\x0C\x00\x00\x03\x07",
    16,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""I""\x00\x00\x03\x0C\x00\x00\x03\x07",
    16,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Upper case' filter to a text object that contains
   * special-case-conversion characters with language-dependent context
   * condition, where the condition is not fulfilled.
   *
   * # When uppercasing, i turns into a dotted capital I
   * 0069; 0069; 0130; 0130; tr; # LATIN SMALL LETTER I
   */
  {
    27,
    PDF_TEXT_FILTER_UPPER_CASE,
    NULL,
    "\x00\x00\x00\x20\x00\x00\x00""i",
    8,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""I",
    8,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Upper case' filter to a text object that contains
   * special-case-conversion characters with language-dependent and After_I
   * context condition (to check two context conditions at the same time),
   * where the condition is not fulfilled.
   *
   * Note: As long as After_I is always coming with a language condition (at least
   * in the Unicode standard 5.0), this test is equal to #26
   *
   * Code: 0307;Lower: ; Title: 0307; Upper: 0307; tr After_I; # COMBINING DOT ABOVE
   * 0x030C has combining class 230, so condition should not be fulfilled
   */
  {
    28,
    PDF_TEXT_FILTER_UPPER_CASE,
    NULL,
    "\x00\x00\x00\x20\x00\x00\x00""I""\x00\x00\x03\x0C\x00\x00\x03\x07",
    16,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""I""\x00\x00\x03\x0C\x00\x00\x03\x07",
    16,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Upper case' filter to a text object that contains
   * special-case-conversion characters with language-dependent and
   * Not_Before_Dot context condition (to check two context conditions at the
   * same time, where one of them is negated with Not_), where the condition is
   * not fulfilled.
   *
   * # When lowercasing, unless an I is before a dot_above, it turns into a
   *   dotless i.
   * 0049; 0131; 0049; 0049; tr Not_Before_Dot; # LATIN CAPITAL LETTER I
   * Not Not_Before_Dot... is equal to Before_Dot :-)
   */
  {
    29,
    PDF_TEXT_FILTER_UPPER_CASE,
    "tr",
    "\x00\x00\x00\x20\x00\x00\x00\x49\x00\x00\x03\x07\x00\x00\x00\x50",
    16,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00\x49\x00\x00\x03\x07\x00\x00\x00\x50",
    16,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Lower case' filter to a text object that contains
   * simple-case-conversion characters.
   */
  {
    30,
    PDF_TEXT_FILTER_LOWER_CASE,
    NULL,
    "\x00\x00\x00\x20\x00\x00\x00""G""\x00\x00\x00""N""\x00\x00\x00""U""\x00\x00\x00\x20",
    20,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""g""\x00\x00\x00""n""\x00\x00\x00""u""\x00\x00\x00\x20",
    20,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Lower case' filter to a text object that contains
   * special-case-conversion characters with no context condition.
   */
  {
    31,
    PDF_TEXT_FILTER_LOWER_CASE,
    NULL,
    "\x00\x00\x00\x20\x00\x00\x01\x30",
    8,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00\x69\x00\x00\x03\x07",
    12,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Lower case' filter to a text object that contains
   * special-case-conversion characters with Final_Sigma context condition.
   *
   * Final_Sigma condition in Special Case algorithms
   *  When the given character is preceded by a sequence consisting of a
   *  cased unicode point (with case information) and a case-ignorable
   *  (without case information) sequence of points. The given character
   *  must no be followed by a sequence consisting of a case-ignorable
   *  sequence and a cased letter.
   *
   * - 0x03A3 point is the one with the Final_Sigma condition
   * - 0x0055 point is the cased point
   * - 0x003A point is the case-ignorable point (MidLetter WB property)
   * - 0x0020 point (whitespace) is used as word separator to have three
   *    different words.
   * If condition is fulfilled, expected smallcase character for 0x03A3 is
   *  different: 0x03C2.
   */
  {
    /* 1st check, the sequence of previous case-ignorable points is empty */
    32,
    PDF_TEXT_FILTER_LOWER_CASE,
    NULL,
    "\x00\x00\x00G\x00\x00\x00N\x00\x00\x00U\x00\x00\x00\x20\x00\x00\x00G\x00\x00\x03\xA3\x00\x00\x00\x20\x00\x00\x00G\x00\x00\x00N\x00\x00\x00U",
    40,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00g\x00\x00\x00n\x00\x00\x00u\x00\x00\x00\x20\x00\x00\x00g\x00\x00\x03\xC2\x00\x00\x00\x20\x00\x00\x00g\x00\x00\x00n\x00\x00\x00u",
    40,
    PDF_TEXT_UTF32_BE
  },
  /* 2nd check, the sequence of previous case-ignorable points is NOT empty */
  {
    33,
    PDF_TEXT_FILTER_LOWER_CASE,
    NULL,
    "\x00\x00\x00G\x00\x00\x00N\x00\x00\x00U\x00\x00\x00\x20\x00\x00\x00G\x00\x00\x00\x3A\x00\x00\x03\xA3\x00\x00\x00\x20\x00\x00\x00G\x00\x00\x00N\x00\x00\x00U",
    44,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00g\x00\x00\x00n\x00\x00\x00u\x00\x00\x00\x20\x00\x00\x00g\x00\x00\x00\x3A\x00\x00\x03\xC2\x00\x00\x00\x20\x00\x00\x00g\x00\x00\x00n\x00\x00\x00u",
    44,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Lower case' filter to a text object that contains
   * special-case-conversion characters with After_Soft_Dotted context
   * condition.
   */
  {
    34,
    PDF_TEXT_FILTER_LOWER_CASE,
    "lt",
    "\x00\x00\x00\x20\x00\x00\x00""i""\x00\x00\x03\x07",
    12,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""i""\x00\x00\x03\x07",
    12,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Lower case' filter to a text object that contains
   * special-case-conversion characters with More_Above context condition.
   *
   * Code:0049; Lower:0069 0307; Title:0049; Upper:0049;
   * lt More_Above; # LATIN CAPITAL LETTER I
   */
  {
    35,
    PDF_TEXT_FILTER_LOWER_CASE,
    "lt",
    "\x00\x00\x00\x20\x00\x00\x00\x49\x00\x00\x03\x0B",
    12,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00\x69\x00\x00\x03\x07\x00\x00\x03\x0B",
    16,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Lower case' filter to a text object that contains
   * special-case-conversion characters with Before_Dot context condition.
   *
   * NOTE: Actually there is no special condition involving "Before_Dot" as is
   *       (there are a couple of conditions involving the negated one:
   *       Not_Before_Dot). But probably is a good idea to at least leave
   *       the test, just in case in future updates of the Unicode standard
   *       one of this type appears
   */
  { 36, PDF_TEXT_FILTER_LOWER_CASE, NULL, NULL, 0, PDF_TEXT_UTF32_BE, NULL, 0, PDF_TEXT_UTF32_BE },

  /*
   * Apply the `Lower case' filter to a text object that contains
   * special-case-conversion characters with After_I context condition.
   *
   * Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE
   */
  {
    37,
    PDF_TEXT_FILTER_LOWER_CASE,
    "tr",
    "\x00\x00\x00\x20\x00\x00\x00""I""\x00\x00\x03\x07",
    12,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""i",
    8,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Lower case' filter to a text object that contains
   * special-case-conversion characters with language-dependent context
   * condition.
   *
   * 00CC; 0069 0307 0300; 00CC; 00CC; lt; # LATIN CAPITAL LETTER I WITH GRAVE
   */
  {
    38,
    PDF_TEXT_FILTER_LOWER_CASE,
    "lt",
    "\x00\x00\x00\x20\x00\x00\x00\xCC",
    8,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00\x69\x00\x00\x03\x07\x00\x00\x03\x00",
    16,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Lower case' filter to a text object that contains
   * special-case-conversion characters with language-dependent and After_I
   * context condition (to check two context conditions at the same time).
   * Note: As long as After_I is always coming with a language condition (at least
   * in the Unicode standard 5.0), this test is equal to #36
   *
   * Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE
   */
  {
    39,
    PDF_TEXT_FILTER_LOWER_CASE,
    "tr",
    "\x00\x00\x00\x20\x00\x00\x00""I""\x00\x00\x03\x07",
    12,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""i",
    8,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Lower case' filter to a text object that contains
   * special-case-conversion characters with language-dependent and
   * Not_Before_Dot context condition (to check two context conditions at the
   * same time, where one of them is negated with Not_).
   *
   * # When lowercasing, unless an I is before a dot_above, it turns into a
   *   dotless i.
   * 0049; 0131; 0049; 0049; tr Not_Before_Dot; # LATIN CAPITAL LETTER I
   */
  {
    40,
    PDF_TEXT_FILTER_LOWER_CASE,
    "tr",
    "\x00\x00\x00\x20\x00\x00\x00\x49\x00\x00\x00\x50",
    12,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x01\x31\x00\x00\x00\x70",
    12,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Lower case' filter to a text object that contains
   * special-case-conversion characters with Final_Sigma context condition,
   * where the condition is not fulfilled.
   *
   * Final_Sigma condition in Special Case algorithms
   *  When the given character is preceded by a sequence consisting of a
   *  cased unicode point (with case information) and a case-ignorable
   *  (without case information) sequence of points. The given character
   *  must no be followed by a sequence consisting of a case-ignorable
   *  sequence and a cased letter.
   *
   * The condition won't be fulfilled because:
   *  1st check. The previous cased point is not available
   *  2nd check. A next cased point is available
   *
   * - 0x03A3 point is the one with the Final_Sigma condition
   * - 0x0055 point is the cased point
   * - 0x0027 point is the case-ignorable point
   * - 0x0020 point (whitespace) is used as word separator to have three
   *    different words.
   *
   * If condition is not fulfilled, expected lowercase character for 0x03A3 is
   *  the non-final lowercase sigma 0x03C3.
   */
  {
    /* 1st check, the previous cased point is not available */
    41,
    PDF_TEXT_FILTER_LOWER_CASE,
    NULL,
    "\x00\x00\x00""G""\x00\x00\x00""N""\x00\x00\x00""U""\x00\x00\x00\x20\x00\x00\x00\x27\x00\x00\x03\xA3\x00\x00\x00\x20\x00\x00\x00""G""\x00\x00\x00""N""\x00\x00\x00""U",
    40,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00""g""\x00\x00\x00""n""\x00\x00\x00""u""\x00\x00\x00\x20\x00\x00\x00\x27\x00\x00\x03\xC3\x00\x00\x00\x20\x00\x00\x00""g""\x00\x00\x00""n""\x00\x00\x00""u",
    40,
    PDF_TEXT_UTF32_BE
  },
  {
    /* 2nd check, a next cased point is available */
    42,
    PDF_TEXT_FILTER_LOWER_CASE,
    NULL,
    "\x00\x00\x00""G""\x00\x00\x00""N""\x00\x00\x00""U""\x00\x00\x00\x20\x00\x00\x00""G""\x00\x00\x00\x27\x00\x00\x03\xA3\x00\x00\x00""G""\x00\x00\x00\x20\x00\x00\x00""G""\x00\x00\x00""N""\x00\x00\x00""U",
    48,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00""g""\x00\x00\x00""n""\x00\x00\x00""u""\x00\x00\x00\x20\x00\x00\x00""g""\x00\x00\x00\x27\x00\x00\x03\xC3\x00\x00\x00""g""\x00\x00\x00\x20\x00\x00\x00""g""\x00\x00\x00""n""\x00\x00\x00""u",
    48,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Lower case' filter to a text object that contains
   * special-case-conversion characters with After_Soft_Dotted context
   * condition, where the condition is not fulfilled.
   *
   * 0x030C has combining class 230, so condition should not be fulfilled
   */
  {
    43,
    PDF_TEXT_FILTER_LOWER_CASE,
    "lt",
    "\x00\x00\x00\x20\x00\x00\x00""i""\x00\x00\x03\x0C\x00\x00\x03\x07",
    16,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""i""\x00\x00\x03\x0C\x00\x00\x03\x07",
    16,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Lower case' filter to a text object that contains
   * special-case-conversion characters with More_Above context condition,
   * where the condition is not fulfilled.
   *
   * Code:0049; Lower:0069 0307; Title:0049; Upper:0049;
   * lt More_Above; # LATIN CAPITAL LETTER I
   * As 0xC0 has combining class 0, the condition must not be fullfilled
   */
  {
    44,
    PDF_TEXT_FILTER_LOWER_CASE,
    NULL,
    "\x00\x00\x00\x20\x00\x00\x00\x49\x00\x00\x00\xC0\x00\x00\x03\x0B",
    16,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00\x69\x00\x00\x00\xE0\x00\x00\x03\x0B",
    16,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Lower case' filter to a text object that contains
   * special-case-conversion characters with Before_Dot context condition,
   * where the condition is not fulfilled.
   *
   * NOTE: Actually there is no special condition involving "Before_Dot" as is
   *       (there are a couple of conditions involving the negated one:
   *       Not_Before_Dot). But probably is a good idea to at least leave
   *       the test, just in case in future updates of the Unicode standard
   *       one of this type appears
   */
  { 45, PDF_TEXT_FILTER_LOWER_CASE, NULL, NULL, 0, PDF_TEXT_UTF32_BE, NULL, 0, PDF_TEXT_UTF32_BE },

  /*
   * Apply the `Lower case' filter to a text object that contains
   * special-case-conversion characters with After_I context condition,
   * where the condition is not fulfilled.
   *
   * Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE
   * 0x030C has combining class 230, so condition should not be fulfilled
   */
  {
    46,
    PDF_TEXT_FILTER_LOWER_CASE,
    "tr",
    "\x00\x00\x00\x20\x00\x00\x00""I""\x00\x00\x03\x0C\x00\x00\x03\x07",
    16,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x01\x31\x00\x00\x03\x0C\x00\x00\x03\x07",
    16,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Lower case' filter to a text object that contains
   * special-case-conversion characters with language-dependent context
   * condition, where the condition is not fulfilled.
   *
   * 00CC; 0069 0307 0300; 00CC; 00CC; lt; # LATIN CAPITAL LETTER I WITH GRAVE
   * If condition not fulfilled, simple lowercase gives: 00EC
   */
  {
    47,
    PDF_TEXT_FILTER_LOWER_CASE,
    NULL,
    "\x00\x00\x00\x20\x00\x00\x00\xCC",
    8,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00\xEC",
    8,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Lower case' filter to a text object that contains
   * special-case-conversion characters with language-dependent and After_I
   * context condition (to check two context conditions at the same time),
   * where the condition is not fulfilled.
   * Note: As long as After_I is always coming with a language condition (at least
   * in the Unicode standard 5.0), this test is equal to #46
   *
   * Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE
   * 0x030C has combining class 230, so condition should not be fulfilled
   */
  {
    48,
    PDF_TEXT_FILTER_LOWER_CASE,
    "tr",
    "\x00\x00\x00\x20\x00\x00\x00""I""\x00\x00\x03\x0C\x00\x00\x03\x07",
    16,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x01\x31\x00\x00\x03\x0C\x00\x00\x03\x07",
    16,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Lower case' filter to a text object that contains
   * special-case-conversion characters with language-dependent and
   * Not_Before_Dot context condition (to check two context conditions at the
   * same time, where one of them is negated with Not_), where the condition is
   * not fulfilled.
   *
   *  # When lowercasing, unless an I is before a dot_above, it turns into a
   *   dotless i.
   * 0049; 0131; 0049; 0049; tr Not_Before_Dot; # LATIN CAPITAL LETTER I
   * Not Not_Before_Dot... is equal to Before_Dot :-)
   */
  {
    49,
    PDF_TEXT_FILTER_LOWER_CASE,
    "tr",
    "\x00\x00\x00\x20\x00\x00\x00\x49\x00\x00\x03\x07\x00\x00\x00\x50",
    16,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00\x69\x00\x00\x00\x70",
    12,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Title case' filter to a text object that contains
   * simple-case-conversion characters.
   */
  {
    50,
    PDF_TEXT_FILTER_TITLE_CASE,
    NULL,
    "\x00\x00\x00\x20\x00\x00\x00""G""\x00\x00\x00""N""\x00\x00\x00""U""\x00\x00\x00\x20",
    20,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""G""\x00\x00\x00""n""\x00\x00\x00""u""\x00\x00\x00\x20",
    20,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Title case' filter to a text object that contains
   * special-case-conversion characters with no context condition.
   */
  {
    51,
    PDF_TEXT_FILTER_TITLE_CASE,
    NULL,
    "\x00\x00\x00\x20\x00\x00\x00""A""\x00\x00\x01\x30",
    12,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""A""\x00\x00\x00\x69\x00\x00\x03\x07",
    16,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Title case' filter to a text object that contains
   * special-case-conversion characters with Final_Sigma context condition.
   *
   * Final_Sigma condition in Special Case algorithms
   *  When the given character is preceded by a sequence consisting of a
   *  cased unicode point (with case information) and a case-ignorable
   *  (without case information) sequence of points. The given character
   *  must no be followed by a sequence consisting of a case-ignorable
   *  sequence and a cased letter.
   *
   * - 0x03A3 point is the one with the Final_Sigma condition
   * - 0x0055 point is the cased point
   * - 0x003A point is the case-ignorable point (MidLetter WB property)
   * - 0x0020 point (whitespace) is used as word separator to have three
   *    different words.
   * If condition is fulfilled, expected titlecase character for 0x03A3 is
   *  the same 0x03A3. But WARNING!!! Only the first character in the word
   *  is titlecased, and due to the constraints of the Final_Sigma condition,
   *  the character with the Final_Sigma will always be at the end of the word,
   *  so the title case option will probably be NEVER used. In this case, the
   *  finalsigma lowercase unicode point will be used (0x03C2) !!.
   */
  {
    /* 1st check, the sequence of previous case-ignorable points is empty */
    52,
    PDF_TEXT_FILTER_TITLE_CASE,
    NULL,
    "\x00\x00\x00""G""\x00\x00\x00""N""\x00\x00\x00""U""\x00\x00\x00\x20\x00\x00\x00""G""\x00\x00\x03\xA3\x00\x00\x00\x20\x00\x00\x00""G""\x00\x00\x00""N""\x00\x00\x00""U",
    40,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00""G""\x00\x00\x00""n""\x00\x00\x00""u""\x00\x00\x00\x20\x00\x00\x00""G""\x00\x00\x03\xC2\x00\x00\x00\x20\x00\x00\x00""G""\x00\x00\x00""n""\x00\x00\x00""u",
    40,
    PDF_TEXT_UTF32_BE
  },
  /* 2nd check, the sequence of previous case-ignorable points is NOT empty */
  {
    53,
    PDF_TEXT_FILTER_TITLE_CASE,
    NULL,
    "\x00\x00\x00""G""\x00\x00\x00""N""\x00\x00\x00""U""\x00\x00\x00\x20\x00\x00\x00""G""\x00\x00\x00\x3A\x00\x00\x03\xA3\x00\x00\x00\x20\x00\x00\x00""G""\x00\x00\x00""N""\x00\x00\x00""U",
    44,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00""G""\x00\x00\x00""n""\x00\x00\x00""u""\x00\x00\x00\x20\x00\x00\x00""G""\x00\x00\x00\x3A\x00\x00\x03\xC2\x00\x00\x00\x20\x00\x00\x00""G""\x00\x00\x00""n""\x00\x00\x00""u",
    44,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Title case' filter to a text object that contains
   * special-case-conversion characters with After_Soft_Dotted context
   * condition.
   */
  {
    54,
    PDF_TEXT_FILTER_TITLE_CASE,
    "lt",
    "\x00\x00\x00\x20\x00\x00\x00""i""\x00\x00\x03\x07",
    12,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""I""\x00\x00\x03\x07",
    12,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Title case' filter to a text object that contains
   * special-case-conversion characters with More_Above context condition.
   *
   * Code:0049; Lower:0069 0307; Title:0049; Upper:0049;
   * lt More_Above; # LATIN CAPITAL LETTER I
   */
  {
    55,
    PDF_TEXT_FILTER_TITLE_CASE,
    "lt",
    "\x00\x00\x00\x20\x00\x00\x00""a""\x00\x00\x00\x49\x00\x00\x03\x0B",
    16,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""A""\x00\x00\x00\x69\x00\x00\x03\x07\x00\x00\x03\x0B",
    20,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Title case' filter to a text object that contains
   * special-case-conversion characters with Before_Dot context condition.
   *
   * NOTE: Actually there is no special condition involving "Before_Dot" as is
   *       (there are a couple of conditions involving the negated one:
   *       Not_Before_Dot). But probably is a good idea to at least leave
   *       the test, just in case in future updates of the Unicode standard
   *       one of this type appears
   */
  { 56, PDF_TEXT_FILTER_TITLE_CASE, NULL, NULL, 0, PDF_TEXT_UTF32_BE, NULL, 0, PDF_TEXT_UTF32_BE },

  /*
   * Apply the `Title case' filter to a text object that contains
   * special-case-conversion characters with After_I context condition.
   *
   * Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE
   */
  {
    57,
    PDF_TEXT_FILTER_TITLE_CASE,
    "tr",
    "\x00\x00\x00\x20\x00\x00\x00""a""\x00\x00\x00""I""\x00\x00\x03\x07",
    16,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""A""\x00\x00\x00""i",
    12,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Title case' filter to a text object that contains
   * special-case-conversion characters with language-dependent context
   * condition.
   *
   * 00CC; 0069 0307 0300; 00CC; 00CC; lt; # LATIN CAPITAL LETTER I WITH GRAVE
   */
  {
    58,
    PDF_TEXT_FILTER_TITLE_CASE,
    "lt",
    "\x00\x00\x00\x20\x00\x00\x00""a""\x00\x00\x00\xCC",
    12,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""A""\x00\x00\x00\x69\x00\x00\x03\x07\x00\x00\x03\x00",
    20,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Title case' filter to a text object that contains
   * special-case-conversion characters with language-dependent and After_I
   * context condition (to check two context conditions at the same time).
   * Note: As long as After_I is always coming with a language condition (at least
   * in the Unicode standard 5.0), this test is equal to #57
   *
   * Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE
   */
  {
    59,
    PDF_TEXT_FILTER_TITLE_CASE,
    "tr",
    "\x00\x00\x00\x20\x00\x00\x00""a""\x00\x00\x00""I""\x00\x00\x03\x07",
    16,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""A""\x00\x00\x00""i",
    12,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Title case' filter to a text object that contains
   * special-case-conversion characters with language-dependent and
   * Not_Before_Dot context condition (to check two context conditions at the
   * same time, where one of them is negated with Not_).
   *
   * # When lowercasing, unless an I is before a dot_above, it turns into a
   *   dotless i.
   * 0049; 0131; 0049; 0049; tr Not_Before_Dot; # LATIN CAPITAL LETTER I
   */
  {
    60,
    PDF_TEXT_FILTER_TITLE_CASE,
    "tr",
    "\x00\x00\x00\x20\x00\x00\x00""a""\x00\x00\x00\x49\x00\x00\x00\x50",
    12,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""A""\x00\x00\x01\x31\x00\x00\x00\x70",
    12,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Title case' filter to a text object that contains
   * special-case-conversion characters with Final_Sigma context condition,
   * where the condition is not fulfilled.
   *
   * Final_Sigma condition in Special Case algorithms
   *  When the given character is preceded by a sequence consisting of a
   *  cased unicode point (with case information) and a case-ignorable
   *  (without case information) sequence of points. The given character
   *  must no be followed by a sequence consisting of a case-ignorable
   *  sequence and a cased letter.
   *
   * The condition won't be fulfilled because:
   *  1st check. The previous cased point is not available
   *  2nd check. A next cased point is available
   *
   * - 0x03A3 point is the one with the Final_Sigma condition
   * - 0x0055 point is the cased point
   * - 0x003A point is the case-ignorable point (MidLetter WB property)
   * - 0x0020 point (whitespace) is used as word separator to have three
   *    different words.
   *
   * If condition is not fulfilled, expected titlecase character for 0x03A3 is
   *  the same 0x03A3. But WARNING!!! Only the first character in the word
   *  is titlecased, and due to the constraints of the Final_Sigma condition,
   *  the character with the Final_Sigma will always be at the end of the word,
   *  so the title case option will probably be NEVER used. In this case, the
   *  non-final lowercase sigma nd uppercase unicode points will be used!!.
   */
  {
    /* 1st check, the previous cased point is not available */
    61,
    PDF_TEXT_FILTER_TITLE_CASE,
    NULL,
    "\x00\x00\x00""G""\x00\x00\x00""N""\x00\x00\x00""U""\x00\x00\x00\x20\x00\x00\x00\x3A\x00\x00\x03\xA3\x00\x00\x00\x20\x00\x00\x00""G""\x00\x00\x00""N""\x00\x00\x00""U",
    40,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00""G""\x00\x00\x00""n""\x00\x00\x00""u""\x00\x00\x00\x20\x00\x00\x00\x3A\x00\x00\x03\xA3\x00\x00\x00\x20\x00\x00\x00""G""\x00\x00\x00""n""\x00\x00\x00""u",
    40,
    PDF_TEXT_UTF32_BE
  },
  {
    /* 2nd check, a next cased point is available */
    62,
    PDF_TEXT_FILTER_TITLE_CASE,
    NULL,
    "\x00\x00\x00""G""\x00\x00\x00""N""\x00\x00\x00""U""\x00\x00\x00\x20\x00\x00\x00""G""\x00\x00\x00\x3A\x00\x00\x03\xA3\x00\x00\x00""G""\x00\x00\x00\x20\x00\x00\x00""G""\x00\x00\x00""N""\x00\x00\x00""U",
    48,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00""G""\x00\x00\x00""n""\x00\x00\x00""u""\x00\x00\x00\x20\x00\x00\x00""G""\x00\x00\x00\x3A\x00\x00\x03\xC3\x00\x00\x00""g""\x00\x00\x00\x20\x00\x00\x00""G""\x00\x00\x00""n""\x00\x00\x00""u",
    48,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Title case' filter to a text object that contains
   * special-case-conversion characters with After_Soft_Dotted context
   * condition, where the condition is not fulfilled.
   *
   * 0x030C has combining class 230, so condition should not be fulfilled
   */
  {
    63,
    PDF_TEXT_FILTER_TITLE_CASE,
    "lt",
    "\x00\x00\x00\x20\x00\x00\x00""a""\x00\x00\x00""i""\x00\x00\x03\x0C\x00\x00\x03\x07",
    20,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""A""\x00\x00\x00""i""\x00\x00\x03\x0C\x00\x00\x03\x07",
    20,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Title case' filter to a text object that contains
   * special-case-conversion characters with More_Above context condition,
   * where the condition is not fulfilled.
   *
   * Code:0049; Lower:0069 0307; Title:0049; Upper:0049;
   * lt More_Above; # LATIN CAPITAL LETTER I
   * As 0xC0 has combining class 0, the condition must not be fullfilled
   */
  {
    64,
    PDF_TEXT_FILTER_TITLE_CASE,
    NULL,
    "\x00\x00\x00\x20\x00\x00\x00""a""\x00\x00\x00\x49\x00\x00\x00\xC0\x00\x00\x03\x0B",
    20,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""A""\x00\x00\x00\x69\x00\x00\x00\xE0\x00\x00\x03\x0B",
    20,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Title case' filter to a text object that contains
   * special-case-conversion characters with Before_Dot context condition,
   * where the condition is not fulfilled.
   *
   * NOTE: Actually there is no special condition involving "Before_Dot" as is
   *       (there are a couple of conditions involving the negated one:
   *       Not_Before_Dot). But probably is a good idea to at least leave
   *       the test, just in case in future updates of the Unicode standard
   *       one of this type appears
   */
  { 65, PDF_TEXT_FILTER_TITLE_CASE, NULL, NULL, 0, PDF_TEXT_UTF32_BE, NULL, 0, PDF_TEXT_UTF32_BE },

  /*
   * Apply the `Title case' filter to a text object that contains
   * special-case-conversion characters with After_I context condition,
   * where the condition is not fulfilled.
   *
   * Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE
   * 0x030C has combining class 230, so condition should not be fulfilled
   */
  {
    66,
    PDF_TEXT_FILTER_TITLE_CASE,
    "tr",
    "\x00\x00\x00\x20\x00\x00\x00""a""\x00\x00\x00""I""\x00\x00\x03\x0C\x00\x00\x03\x07",
    20,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""A""\x00\x00\x01\x31\x00\x00\x03\x0C\x00\x00\x03\x07",
    20,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Title case' filter to a text object that contains
   * special-case-conversion characters with language-dependent context
   * condition, where the condition is not fulfilled.
   *
   * 00CC; 0069 0307 0300; 00CC; 00CC; lt; # LATIN CAPITAL LETTER I WITH GRAVE
   * If condition not fulfilled, simple lowercase gives: 00EC
   */
  {
    67,
    PDF_TEXT_FILTER_TITLE_CASE,
    NULL,
    "\x00\x00\x00\x20\x00\x00\x00""a""\x00\x00\x00\xCC",
    12,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""A""\x00\x00\x00\xEC",
    12,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Title case' filter to a text object that contains
   * special-case-conversion characters with language-dependent and After_I
   * context condition (to check two context conditions at the same time),
   * where the condition is not fulfilled.
   * Note: As long as After_I is always coming with a language condition (at least
   * in the Unicode standard 5.0), this test is equal to #46
   *
   * Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE
   * 0x030C has combining class 230, so condition should not be fulfilled
   */
  {
    68,
    PDF_TEXT_FILTER_TITLE_CASE,
    "tr",
    "\x00\x00\x00\x20\x00\x00\x00""a""\x00\x00\x00""I""\x00\x00\x03\x0C\x00\x00\x03\x07",
    20,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""A""\x00\x00\x01\x31\x00\x00\x03\x0C\x00\x00\x03\x07",
    20,
    PDF_TEXT_UTF32_BE
  },

  /*
   * Apply the `Title case' filter to a text object that contains
   * special-case-conversion characters with language-dependent and
   * Not_Before_Dot context condition (to check two context conditions at the
   * same time, where one of them is negated with Not_), where the condition is
   * not fulfilled.
   *
   *  # When lowercasing, unless an I is before a dot_above, it turns into a
   *   dotless i.
   * 0049; 0131; 0049; 0049; tr Not_Before_Dot; # LATIN CAPITAL LETTER I
   * Not Not_Before_Dot... is equal to Before_Dot :-)
   */
  {
    69,
    PDF_TEXT_FILTER_TITLE_CASE,
    "tr",
    "\x00\x00\x00\x20\x00\x00\x00""a""\x00\x00\x00\x49\x00\x00\x03\x07",
    16,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00""A""\x00\x00\x00\x69",
    12,
    PDF_TEXT_UTF32_BE
  }
};

static void
common_test (const pdf_char_t *function_name,
             int               test_index)
{
  const struct test_params *params;
  pdf_error_t *error = NULL;
  pdf_text_t *text;
  pdf_char_t *output_data;
  pdf_size_t output_size;

  params = &tests[test_index - 1];
  fail_if (params->idx != test_index);

  if (params->input)
    {
      pdf_size_t input_size;

      /* Create text object with given contents */
      input_size = (params->input_size >= 0 ?
                    params->input_size :
                    strlen (params->input));
      text = pdf_text_new_from_unicode (params->input,
                                        input_size,
                                        params->input_enc,
                                        &error);
    }
  else
    {
      /* Create empty text object */
      text = pdf_text_new (&error);
    }

  fail_unless (text != NULL);
  fail_if (error != NULL);

  /* Set language if any to be set */
  if (params->language)
    pdf_text_set_language (text, params->language);

  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless (pdf_text_filter (text,
                                params->filters,
                                &error) == PDF_TRUE);
  fail_if (error != NULL);

  /* Get output as UTF-32BE */
  output_data = pdf_text_get_unicode (text,
                                      params->expected_enc,
                                      PDF_TEXT_UNICODE_NO_OPTION,
                                      &output_size,
                                      &error);
  fail_if (error != NULL);

  if (params->expected)
    {
      pdf_size_t expected_size;

#ifdef TEXT_MODULE_ADDITIONAL_TEST_TRACES
      if (params->expected_enc == PDF_TEXT_UTF32_BE)
        {
          PRINT_CONTENTS (function_name, 0,
                          text,
                          params->expected, params->expected_size,
                          0);
        }
#endif /* TEXT_MODULE_ADDITIONAL_TEST_TRACES */

      /* If any input given, some output is expected */
      fail_unless (output_data != NULL);

      /* 2. The contents of the output text object must be the expected ones. */
      expected_size = (params->expected_size >= 0 ?
                       params->expected_size :
                       strlen (params->expected));
      fail_unless (output_size == expected_size);
      fail_unless (memcmp (output_data, params->expected, output_size) == 0);

      pdf_dealloc (output_data);
    }
  else
    {
      /* If no input was given, no output is expected */
      fail_unless (output_data == NULL);
      fail_unless (output_size == 0);
    }

  pdf_text_destroy (text);
}

START_TEST (pdf_text_filter_001) { common_test(__FUNCTION__,  1); } END_TEST
START_TEST (pdf_text_filter_002) { common_test(__FUNCTION__,  2); } END_TEST

START_TEST (pdf_text_filter_003) { common_test(__FUNCTION__,  3); } END_TEST
START_TEST (pdf_text_filter_004) { common_test(__FUNCTION__,  4); } END_TEST

START_TEST (pdf_text_filter_005) { common_test(__FUNCTION__,  5); } END_TEST
START_TEST (pdf_text_filter_006) { common_test(__FUNCTION__,  6); } END_TEST

START_TEST (pdf_text_filter_007) { common_test(__FUNCTION__,  7); } END_TEST
START_TEST (pdf_text_filter_008) { common_test(__FUNCTION__,  8); } END_TEST
START_TEST (pdf_text_filter_009) { common_test(__FUNCTION__,  9); } END_TEST

START_TEST (pdf_text_filter_010) { common_test(__FUNCTION__, 10); } END_TEST
START_TEST (pdf_text_filter_011) { common_test(__FUNCTION__, 11); } END_TEST
START_TEST (pdf_text_filter_012) { common_test(__FUNCTION__, 12); } END_TEST
START_TEST (pdf_text_filter_013) { common_test(__FUNCTION__, 13); } END_TEST
START_TEST (pdf_text_filter_014) { common_test(__FUNCTION__, 14); } END_TEST
START_TEST (pdf_text_filter_015) { common_test(__FUNCTION__, 15); } END_TEST
START_TEST (pdf_text_filter_016) { common_test(__FUNCTION__, 16); } END_TEST
START_TEST (pdf_text_filter_017) { common_test(__FUNCTION__, 17); } END_TEST
START_TEST (pdf_text_filter_018) { common_test(__FUNCTION__, 18); } END_TEST
START_TEST (pdf_text_filter_019) { common_test(__FUNCTION__, 19); } END_TEST
START_TEST (pdf_text_filter_020) { common_test(__FUNCTION__, 20); } END_TEST
START_TEST (pdf_text_filter_021) { common_test(__FUNCTION__, 21); } END_TEST
START_TEST (pdf_text_filter_022) { common_test(__FUNCTION__, 22); } END_TEST
START_TEST (pdf_text_filter_023) { common_test(__FUNCTION__, 23); } END_TEST
START_TEST (pdf_text_filter_024) { common_test(__FUNCTION__, 24); } END_TEST
START_TEST (pdf_text_filter_025) { common_test(__FUNCTION__, 25); } END_TEST
START_TEST (pdf_text_filter_026) { common_test(__FUNCTION__, 26); } END_TEST
START_TEST (pdf_text_filter_027) { common_test(__FUNCTION__, 27); } END_TEST
START_TEST (pdf_text_filter_028) { common_test(__FUNCTION__, 28); } END_TEST
START_TEST (pdf_text_filter_029) { common_test(__FUNCTION__, 29); } END_TEST

START_TEST (pdf_text_filter_030) { common_test(__FUNCTION__, 30); } END_TEST
START_TEST (pdf_text_filter_031) { common_test(__FUNCTION__, 31); } END_TEST
START_TEST (pdf_text_filter_032) { common_test(__FUNCTION__, 32); } END_TEST
START_TEST (pdf_text_filter_033) { common_test(__FUNCTION__, 33); } END_TEST
START_TEST (pdf_text_filter_034) { common_test(__FUNCTION__, 34); } END_TEST
START_TEST (pdf_text_filter_035) { common_test(__FUNCTION__, 35); } END_TEST
START_TEST (pdf_text_filter_036) { common_test(__FUNCTION__, 36); } END_TEST
START_TEST (pdf_text_filter_037) { common_test(__FUNCTION__, 37); } END_TEST
START_TEST (pdf_text_filter_038) { common_test(__FUNCTION__, 38); } END_TEST
START_TEST (pdf_text_filter_039) { common_test(__FUNCTION__, 39); } END_TEST
START_TEST (pdf_text_filter_040) { common_test(__FUNCTION__, 40); } END_TEST
START_TEST (pdf_text_filter_041) { common_test(__FUNCTION__, 41); } END_TEST
START_TEST (pdf_text_filter_042) { common_test(__FUNCTION__, 42); } END_TEST
START_TEST (pdf_text_filter_043) { common_test(__FUNCTION__, 43); } END_TEST
START_TEST (pdf_text_filter_044) { common_test(__FUNCTION__, 44); } END_TEST
START_TEST (pdf_text_filter_045) { common_test(__FUNCTION__, 45); } END_TEST
START_TEST (pdf_text_filter_046) { common_test(__FUNCTION__, 46); } END_TEST
START_TEST (pdf_text_filter_047) { common_test(__FUNCTION__, 47); } END_TEST
START_TEST (pdf_text_filter_048) { common_test(__FUNCTION__, 48); } END_TEST
START_TEST (pdf_text_filter_049) { common_test(__FUNCTION__, 49); } END_TEST

START_TEST (pdf_text_filter_050) { common_test(__FUNCTION__, 50); } END_TEST
START_TEST (pdf_text_filter_051) { common_test(__FUNCTION__, 51); } END_TEST
START_TEST (pdf_text_filter_052) { common_test(__FUNCTION__, 52); } END_TEST
START_TEST (pdf_text_filter_053) { common_test(__FUNCTION__, 53); } END_TEST
START_TEST (pdf_text_filter_054) { common_test(__FUNCTION__, 54); } END_TEST
START_TEST (pdf_text_filter_055) { common_test(__FUNCTION__, 55); } END_TEST
START_TEST (pdf_text_filter_056) { common_test(__FUNCTION__, 56); } END_TEST
START_TEST (pdf_text_filter_057) { common_test(__FUNCTION__, 57); } END_TEST
START_TEST (pdf_text_filter_058) { common_test(__FUNCTION__, 58); } END_TEST
START_TEST (pdf_text_filter_059) { common_test(__FUNCTION__, 59); } END_TEST
START_TEST (pdf_text_filter_060) { common_test(__FUNCTION__, 60); } END_TEST
START_TEST (pdf_text_filter_061) { common_test(__FUNCTION__, 61); } END_TEST
START_TEST (pdf_text_filter_062) { common_test(__FUNCTION__, 62); } END_TEST
START_TEST (pdf_text_filter_063) { common_test(__FUNCTION__, 63); } END_TEST
START_TEST (pdf_text_filter_064) { common_test(__FUNCTION__, 64); } END_TEST
START_TEST (pdf_text_filter_065) { common_test(__FUNCTION__, 65); } END_TEST
START_TEST (pdf_text_filter_066) { common_test(__FUNCTION__, 66); } END_TEST
START_TEST (pdf_text_filter_067) { common_test(__FUNCTION__, 67); } END_TEST
START_TEST (pdf_text_filter_068) { common_test(__FUNCTION__, 68); } END_TEST
START_TEST (pdf_text_filter_069) { common_test(__FUNCTION__, 69); } END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_text_filter (void)
{
  TCase *tc = tcase_create("pdf_text_filter");

  tcase_add_test (tc, pdf_text_filter_001); /* Normalize line endings */
  tcase_add_test (tc, pdf_text_filter_002); /* Normalize line endings */

  tcase_add_test (tc, pdf_text_filter_003); /* Remove line endings */
  tcase_add_test (tc, pdf_text_filter_004); /* Remove line endings */

  tcase_add_test (tc, pdf_text_filter_005); /* Remove ampersands */
  tcase_add_test (tc, pdf_text_filter_006); /* Remove ampersands */

  tcase_add_test (tc, pdf_text_filter_007); /* Normalize with full width */
  tcase_add_test (tc, pdf_text_filter_008); /* Normalize with full width */
  tcase_add_test (tc, pdf_text_filter_009); /* Normalize with full width */

  /* Upper case */
  tcase_add_test (tc, pdf_text_filter_010); /* Upper */
  tcase_add_test (tc, pdf_text_filter_011); /* Upper-Special */
  tcase_add_test (tc, pdf_text_filter_012); /* Upper-Special Final_Sigma (1) */
  tcase_add_test (tc, pdf_text_filter_013); /* Upper-Special Final_Sigma (2) */
  tcase_add_test (tc, pdf_text_filter_014); /* Upper-Special After_Soft_Dotted */
  tcase_add_test (tc, pdf_text_filter_015); /* Upper-Special More_Above */
  tcase_add_test (tc, pdf_text_filter_016); /* Upper-Special Before_Dot  */
  tcase_add_test (tc, pdf_text_filter_017); /* Upper-Special After_I  */
  tcase_add_test (tc, pdf_text_filter_018); /* Upper-Special 'tr'  */
  tcase_add_test (tc, pdf_text_filter_019); /* Upper-Special After_I and 'tr'  */
  tcase_add_test (tc, pdf_text_filter_020); /* Upper-Special Not_Before_Dot  */
  tcase_add_test (tc, pdf_text_filter_021); /* Upper-Special Not Final_Sigma (1) */
  tcase_add_test (tc, pdf_text_filter_022); /* Upper-Special Not Final_Sigma (2) */
  tcase_add_test (tc, pdf_text_filter_023); /* Upper-Special Not After_Soft_Dotted */
  tcase_add_test (tc, pdf_text_filter_024); /* Upper-Special Not More_Above */
  tcase_add_test (tc, pdf_text_filter_025); /* Upper-Special Not Before_Dot  */
  tcase_add_test (tc, pdf_text_filter_026); /* Upper-Special Not After_I  */
  tcase_add_test (tc, pdf_text_filter_027); /* Upper-Special Not 'tr'  */
  tcase_add_test (tc, pdf_text_filter_028); /* Upper-Special Not After_I and 'tr'  */
  tcase_add_test (tc, pdf_text_filter_029); /* Upper-Special Not Not_Before_Dot  */

  /* Lower case */
  tcase_add_test (tc, pdf_text_filter_030); /* Lower */
  tcase_add_test (tc, pdf_text_filter_031); /* Lower-Special */
  tcase_add_test (tc, pdf_text_filter_032); /* Lower-Special Final_Sigma (1) */
  tcase_add_test (tc, pdf_text_filter_033); /* Lower-Special Final_Sigma (2) */
  tcase_add_test (tc, pdf_text_filter_034); /* Lower-Special After_Soft_Dotted */
  tcase_add_test (tc, pdf_text_filter_035); /* Lower-Special More_Above */
  tcase_add_test (tc, pdf_text_filter_036); /* Lower-Special Before_Dot  */
  tcase_add_test (tc, pdf_text_filter_037); /* Lower-Special After_I  */
  tcase_add_test (tc, pdf_text_filter_038); /* Lower-Special 'tr'  */
  tcase_add_test (tc, pdf_text_filter_039); /* Lower-Special After_I and 'tr'  */
  tcase_add_test (tc, pdf_text_filter_040); /* Lower-Special Not_Before_Dot  */
  tcase_add_test (tc, pdf_text_filter_041); /* Lower-Special Not Final_Sigma (1) */
  tcase_add_test (tc, pdf_text_filter_042); /* Lower-Special Not Final_Sigma (2) */
  tcase_add_test (tc, pdf_text_filter_043); /* Lower-Special Not After_Soft_Dotted */
  tcase_add_test (tc, pdf_text_filter_044); /* Lower-Special Not More_Above */
  tcase_add_test (tc, pdf_text_filter_045); /* Lower-Special Not Before_Dot  */
  tcase_add_test (tc, pdf_text_filter_046); /* Lower-Special Not After_I  */
  tcase_add_test (tc, pdf_text_filter_047); /* Lower-Special Not 'tr'  */
  tcase_add_test (tc, pdf_text_filter_048); /* Lower-Special Not After_I and 'tr'  */
  tcase_add_test (tc, pdf_text_filter_049); /* Lower-Special Not Not_Before_Dot  */

  /* Title case */
  tcase_add_test (tc, pdf_text_filter_050); /* Title */
  tcase_add_test (tc, pdf_text_filter_051); /* Title-Special */
  tcase_add_test (tc, pdf_text_filter_052); /* Title-Special Final_Sigma (1) */
  tcase_add_test (tc, pdf_text_filter_053); /* Title-Special Final_Sigma (2) */
  tcase_add_test (tc, pdf_text_filter_054); /* Title-Special After_Soft_Dotted */
  tcase_add_test (tc, pdf_text_filter_055); /* Title-Special More_Above */
  tcase_add_test (tc, pdf_text_filter_056); /* Title-Special Before_Dot  */
  tcase_add_test (tc, pdf_text_filter_057); /* Title-Special After_I  */
  tcase_add_test (tc, pdf_text_filter_058); /* Title-Special 'tr'  */
  tcase_add_test (tc, pdf_text_filter_059); /* Title-Special After_I and 'tr'  */
  tcase_add_test (tc, pdf_text_filter_060); /* Title-Special Not_Before_Dot  */
  tcase_add_test (tc, pdf_text_filter_061); /* Title-Special Not Final_Sigma (1) */
  tcase_add_test (tc, pdf_text_filter_062); /* Title-Special Not Final_Sigma (2) */
  tcase_add_test (tc, pdf_text_filter_063); /* Title-Special Not After_Soft_Dotted */
  tcase_add_test (tc, pdf_text_filter_064); /* Title-Special Not More_Above */
  tcase_add_test (tc, pdf_text_filter_065); /* Title-Special Not Before_Dot  */
  tcase_add_test (tc, pdf_text_filter_066); /* Title-Special Not After_I  */
  tcase_add_test (tc, pdf_text_filter_067); /* Title-Special Not 'tr'  */
  tcase_add_test (tc, pdf_text_filter_068); /* Title-Special Not After_I and 'tr'  */
  tcase_add_test (tc, pdf_text_filter_069); /* Title-Special Not Not_Before_Dot  */

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-text-filter.c */

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
 * Test: pdf_text_get_unicode_001-063
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
    "\x00\x00\x00 \x00\x00\x00G\x00\x00\x00N\x00\x00\x00U\x00\x00\x00 ",
    20,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00 \x00\x00\xFF\x27\x00\x00\xFF\x2E\x00\x00\xFF\x35\x00\x00\x00 ",
    20,
    PDF_TEXT_UTF32_BE
  },

  /* Apply the `Normalize with full width' filter to a text object that contains
   * code points that have a valid full width representation. */
  {
    8,
    PDF_TEXT_FILTER_NORM_WITH_FULL_WIDTH,
    NULL,
    "\x00\x00\x6C\x34\x00\x00\x00 \x00\x01\xD1\x1E",
    12,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x6C\x34\x00\x00\x00 \x00\x01\xD1\x1E",
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
    "\x00\x00\x00 \x00\x00\x00g\x00\x00\x00n\x00\x00\x00u\x00\x00\x00 ",
    20,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00 \x00\x00\x00""G\x00\x00\x00N\x00\x00\x00U\x00\x00\x00 ",
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
    "\x00\x00\x00\x20\x00\x00\x00i\x00\x00\x03\x07",
    12,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00I",
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
    "\x00\x00\x00\x20\x00\x00\x00I\x00\x00\x03\x07",
    12,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00I\x00\x00\x03\x07",
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
    "\x00\x00\x00\x20\x00\x00\x00I\x00\x00\x03\x07",
    12,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00I\x00\x00\x03\x07",
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
    "\x00\x00\x00\x20\x00\x00\x00i\x00\x00\x03\x0C\x00\x00\x03\x07",
    16,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00I\x00\x00\x03\x0C\x00\x00\x03\x07",
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
    "\x00\x00\x00\x20\x00\x00\x00I\x00\x00\x03\x0C\x00\x00\x03\x07",
    16,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00I\x00\x00\x03\x0C\x00\x00\x03\x07",
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
    "\x00\x00\x00\x20\x00\x00\x00i",
    8,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00I",
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
    "\x00\x00\x00\x20\x00\x00\x00I\x00\x00\x03\x0C\x00\x00\x03\x07",
    16,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00I\x00\x00\x03\x0C\x00\x00\x03\x07",
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
    "\x00\x00\x00 \x00\x00\x00G\x00\x00\x00N\x00\x00\x00U\x00\x00\x00 ",
    20,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00 \x00\x00\x00g\x00\x00\x00n\x00\x00\x00u\x00\x00\x00 ",
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
    "\x00\x00\x00G\x00\x00\x00N\x00\x00\x00U\x00\x00\x00 \x00\x00\x00G\x00\x00\x03\xA3\x00\x00\x00 \x00\x00\x00G\x00\x00\x00N\x00\x00\x00U",
    40,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00g\x00\x00\x00n\x00\x00\x00u\x00\x00\x00 \x00\x00\x00g\x00\x00\x03\xC2\x00\x00\x00 \x00\x00\x00g\x00\x00\x00n\x00\x00\x00u",
    40,
    PDF_TEXT_UTF32_BE
  },
  /* 2nd check, the sequence of previous case-ignorable points is NOT empty */
  {
    33,
    PDF_TEXT_FILTER_LOWER_CASE,
    NULL,
    "\x00\x00\x00G\x00\x00\x00N\x00\x00\x00U\x00\x00\x00 \x00\x00\x00G\x00\x00\x00\x3A\x00\x00\x03\xA3\x00\x00\x00 \x00\x00\x00G\x00\x00\x00N\x00\x00\x00U",
    44,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00g\x00\x00\x00n\x00\x00\x00u\x00\x00\x00 \x00\x00\x00g\x00\x00\x00\x3A\x00\x00\x03\xC2\x00\x00\x00 \x00\x00\x00g\x00\x00\x00n\x00\x00\x00u",
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
    "\x00\x00\x00\x20\x00\x00\x00i\x00\x00\x03\x07",
    12,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00i\x00\x00\x03\x07",
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
    "\x00\x00\x00\x20\x00\x00\x00I\x00\x00\x03\x07",
    12,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00i",
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
    "\x00\x00\x00\x20\x00\x00\x00I\x00\x00\x03\x07",
    12,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00i",
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
    "\x00\x00\x00G\x00\x00\x00N\x00\x00\x00U\x00\x00\x00 \x00\x00\x00\x27\x00\x00\x03\xA3\x00\x00\x00 \x00\x00\x00G\x00\x00\x00N\x00\x00\x00U",
    40,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00g\x00\x00\x00n\x00\x00\x00u\x00\x00\x00 \x00\x00\x00\x27\x00\x00\x03\xC3\x00\x00\x00 \x00\x00\x00g\x00\x00\x00n\x00\x00\x00u",
    40,
    PDF_TEXT_UTF32_BE
  },
  {
    /* 2nd check, a next cased point is available */
    42,
    PDF_TEXT_FILTER_LOWER_CASE,
    NULL,
    "\x00\x00\x00G\x00\x00\x00N\x00\x00\x00U\x00\x00\x00 \x00\x00\x00G\x00\x00\x00\x27\x00\x00\x03\xA3\x00\x00\x00G\x00\x00\x00 \x00\x00\x00G\x00\x00\x00N\x00\x00\x00U",
    48,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00g\x00\x00\x00n\x00\x00\x00u\x00\x00\x00 \x00\x00\x00g\x00\x00\x00\x27\x00\x00\x03\xC3\x00\x00\x00g\x00\x00\x00 \x00\x00\x00g\x00\x00\x00n\x00\x00\x00u",
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
    "\x00\x00\x00\x20\x00\x00\x00i\x00\x00\x03\x0C\x00\x00\x03\x07",
    16,
    PDF_TEXT_UTF32_BE,
    "\x00\x00\x00\x20\x00\x00\x00i\x00\x00\x03\x0C\x00\x00\x03\x07",
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
    "\x00\x00\x00\x20\x00\x00\x00I\x00\x00\x03\x0C\x00\x00\x03\x07",
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
    "\x00\x00\x00\x20\x00\x00\x00I\x00\x00\x03\x0C\x00\x00\x03\x07",
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


/* /\* */
/*  * Test: pdf_text_filter_001 */
/*  * Description: */
/*  *   Apply the `Normalize line endings' filter to a text object which contains */
/*  *   different types of line endings. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_001) */
/* { */
/*   pdf_error_t *error = NULL; */
/*   pdf_text_t *text; */
/*   const pdf_char_t *input_data = "GNU's Not Unix" "\r""\n" */
/*                                  "GNU's Not Unix" "\r" */
/*                                  "GNU's Not Unix" "\n" */
/*                                  "GNU's Not Unix" "\xC2\x85"; */
/* #ifdef PDF_HOST_WIN32 */
/*   const pdf_char_t *expected_data = "GNU's Not Unix" "\r""\n" */
/*                                     "GNU's Not Unix" "\r""\n" */
/*                                     "GNU's Not Unix" "\r""\n" */
/*                                     "GNU's Not Unix" "\r""\n"; */
/* #else */
/*   const pdf_char_t *expected_data = "GNU's Not Unix" "\n" */
/*                                     "GNU's Not Unix" "\n" */
/*                                     "GNU's Not Unix" "\n" */
/*                                     "GNU's Not Unix" "\n"; */
/* #endif */
/*   pdf_size_t input_size; */
/*   pdf_size_t expected_size; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */

/*   input_size = strlen (input_data); */
/*   expected_size = strlen (expected_data); */

/*   /\* Create text object with given contents *\/ */
/*   text = pdf_text_new_from_unicode (input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF8, */
/*                                     &error); */
/*   fail_unless (text != NULL); */
/*   fail_if (error != NULL); */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless (pdf_text_filter (text, */
/*                                 PDF_TEXT_FILTER_LINE_ENDINGS, */
/*                                 &error) == PDF_TRUE); */
/*   fail_if (error != NULL); */

/*   /\* Get output as UTF-32BE *\/ */
/*   output_data = pdf_text_get_unicode (text, */
/*                                       PDF_TEXT_UTF8, */
/*                                       PDF_TEXT_UNICODE_NO_OPTION, */
/*                                       &output_size, */
/*                                       &error); */
/*   fail_unless (output_data != NULL); */
/*   fail_if (error != NULL); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless (output_size == expected_size); */
/*   fail_unless (memcmp (output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc (output_data); */
/*   pdf_text_destroy (text); */
/* } */
/* END_TEST */

/* /\* */
/*  * Test: pdf_text_filter_002 */
/*  * Description: */
/*  *   Apply the `Normalize line endings' filter to an empty text object. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must remain unchanged. */
/*  *\/ */
/* START_TEST (pdf_text_filter_002) */
/* { */
/*   pdf_error_t *error = NULL; */
/*   pdf_text_t *text; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */

/*   /\* Create empty text object *\/ */
/*   text = pdf_text_new (&error); */
/*   fail_unless (text != NULL); */
/*   fail_if (error != NULL); */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless (pdf_text_filter (text, */
/*                                 PDF_TEXT_FILTER_LINE_ENDINGS, */
/*                                 &error) == PDF_TRUE); */
/*   fail_if (error != NULL); */

/*   /\* Get output as UTF-32BE *\/ */
/*   output_data = pdf_text_get_unicode (text, */
/*                                       PDF_TEXT_UTF8, */
/*                                       PDF_TEXT_UNICODE_NO_OPTION, */
/*                                       &output_size, */
/*                                       &error); */
/*   fail_unless (output_data == NULL); */
/*   fail_unless (output_size == 0); */
/*   fail_if (error != NULL); */

/*   pdf_dealloc (output_data); */
/*   pdf_text_destroy (text); */
/* } */
/* END_TEST */

/* /\* */
/*  * Test: pdf_text_filter_003 */
/*  * Description: */
/*  *   Apply the `Remove line endings' filter to a text object which contains */
/*  *   different types of line endings. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_003) */
/* { */
/*   pdf_error_t *error = NULL; */
/*   pdf_text_t *text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "GNU's Not Unix. " "\r""\n" \ */
/*                                                 "GNU's Not Unix. " "\r" \ */
/*                                                 "GNU's Not Unix. " "\n" \ */
/*                                                 "GNU's Not Unix. " "\xC2\x85"; */
/*   const pdf_char_t *expected_data = (pdf_char_t *) "GNU's Not Unix. " \ */
/*                                                    "GNU's Not Unix. " \ */
/*                                                    "GNU's Not Unix. " \ */
/*                                                    "GNU's Not Unix. "; */
/*   pdf_size_t input_size; */
/*   pdf_size_t expected_size; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */

/*   input_size = strlen (input_data); */
/*   expected_size = strlen (expected_data); */

/*   /\* Create text object with given contents *\/ */
/*   text = pdf_text_new_from_unicode (input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF8, */
/*                                     &error); */
/*   fail_unless (text != NULL); */
/*   fail_if (error != NULL); */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless (pdf_text_filter (text, */
/*                                 PDF_TEXT_FILTER_REMOVE_LINE_ENDINGS, */
/*                                 &error) == PDF_TRUE); */

/*   /\* Get output as UTF-32BE *\/ */
/*   output_data = pdf_text_get_unicode (text, */
/*                                       PDF_TEXT_UTF8, */
/*                                       PDF_TEXT_UNICODE_NO_OPTION, */
/*                                       &output_size, */
/*                                       &error); */
/*   fail_unless (output_data != NULL); */
/*   fail_if (error != NULL); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless (output_size == expected_size); */
/*   fail_unless (memcmp (output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc (output_data); */
/*   pdf_text_destroy (text); */
/* } */
/* END_TEST */

/* /\* */
/*  * Test: pdf_text_filter_004 */
/*  * Description: */
/*  *   Apply the `Remove line endings' filter to an empty text object. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must remain unchanged. */
/*  *\/ */
/* START_TEST (pdf_text_filter_004) */
/* { */
/*   pdf_error_t *error = NULL; */
/*   pdf_text_t *text; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */

/*   /\* Create text object with given contents *\/ */
/*   text = pdf_text_new (&error); */
/*   fail_unless (text != NULL); */
/*   fail_if (error != NULL); */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless (pdf_text_filter (text, */
/*                                 PDF_TEXT_FILTER_REMOVE_LINE_ENDINGS, */
/*                                 &error) == PDF_TRUE); */

/*   /\* Get output as UTF-32BE *\/ */
/*   output_data = pdf_text_get_unicode (text, */
/*                                       PDF_TEXT_UTF8, */
/*                                       PDF_TEXT_UNICODE_NO_OPTION, */
/*                                       &output_size, */
/*                                       &error); */
/*   fail_unless (output_data == NULL); */
/*   fail_unless (output_size == 0); */
/*   fail_if (error != NULL); */

/*   pdf_dealloc (output_data); */
/*   pdf_text_destroy (text); */
/* } */
/* END_TEST */

/* /\* */
/*  * Test: pdf_text_filter_005 */
/*  * Description: */
/*  *   Apply the `Remove ampersands' filter to a text object which contains */
/*  *   single and double ampersands. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_005) */
/* { */
/*   pdf_error_t *error = NULL; */
/*   pdf_text_t *text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "GNU & GNU && GNU"; */
/*   const pdf_char_t *expected_data = (pdf_char_t *) "GNU GNU & GNU"; */
/*   pdf_size_t input_size; */
/*   pdf_size_t expected_size; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */

/*   input_size = (long) strlen (input_data); */
/*   expected_size = (long) strlen (expected_data); */

/*   /\* Create text object with given contents *\/ */
/*   text = pdf_text_new_from_unicode (input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF8, */
/*                                     &error); */
/*   fail_unless (text != NULL); */
/*   fail_if (error != NULL); */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless (pdf_text_filter (text, */
/*                                 PDF_TEXT_FILTER_REMOVE_AMP, */
/*                                 &error) == PDF_TRUE); */

/*   /\* Get output as UTF-32BE *\/ */
/*   output_data = pdf_text_get_unicode (text, */
/*                                       PDF_TEXT_UTF8, */
/*                                       PDF_TEXT_UNICODE_NO_OPTION, */
/*                                       &output_size, */
/*                                       &error); */
/*   fail_unless (output_data != NULL); */
/*   fail_if (error != NULL); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless (output_size == expected_size); */
/*   fail_unless (memcmp (output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc (output_data); */
/*   pdf_text_destroy (text);} */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_006 */
/*  * Description: */
/*  *   Apply the `Remove ampersands' filter to an empty text object. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must remain unchanged. */
/*  *\/ */
/* START_TEST (pdf_text_filter_006) */
/* { */



/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) ""; */
/*   const pdf_char_t *expected_data = input_data; */
/*   pdf_size_t input_size = 0; */
/*   pdf_size_t expected_size = 0; */
/*   pdf_char_t *output_data = NULL; */
/*   pdf_size_t output_size = 0; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF8, &text) != PDF_OK); */
/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_006:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)(long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_REMOVE_AMP) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_006:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)(long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF8, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must remain unchanged. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(output_data == NULL); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */



/* /\* */
/*  * Test: pdf_text_filter_007 */
/*  * Description: */
/*  *   Apply the `Normalize with full width' filter to a text object that contains */
/*  *   code points that have a valid full width representation. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_007) */
/* { */



/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00"" " \ */
/*                                                 "\x00\x00\x00""G" \ */
/*                                                 "\x00\x00\x00""N" \ */
/*                                                 "\x00\x00\x00""U" \ */
/*                                                 "\x00\x00\x00"" "; */
/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00"" " \ */
/*                                                 "\x00\x00\xFF\x27" \ */
/*                                                 "\x00\x00\xFF\x2E" \ */
/*                                                 "\x00\x00\xFF\x35" \ */
/*                                                 "\x00\x00\x00"" "; */
/*   pdf_size_t input_size = 20; */
/*   pdf_size_t expected_size = 20; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_007:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, */
/*                               PDF_TEXT_FILTER_NORM_WITH_FULL_WIDTH) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_007:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_008 */
/*  * Description: */
/*  *   Apply the `Normalize with full width' filter to a text object that contains */
/*  *   code points that have a valid full width representation. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_008) */
/* { */



/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x6C\x34" \ */
/*                                                 "\x00\x00\x00"" " \ */
/*                                                 "\x00\x01\xD1\x1E"; */
/*   const pdf_char_t *expected_data = input_data; */
/*   pdf_size_t input_size = 12; */
/*   pdf_size_t expected_size = input_size; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_007:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, */
/*                               PDF_TEXT_FILTER_NORM_WITH_FULL_WIDTH) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_007:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_009 */
/*  * Description: */
/*  *   Apply the `Normalize with full width' filter to an empty text object. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must remain unchanged. */
/*  *\/ */
/* START_TEST (pdf_text_filter_009) */
/* { */



/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) ""; */
/*   const pdf_char_t *expected_data = input_data; */
/*   pdf_size_t input_size = 0; */
/*   pdf_size_t expected_size = 0; */
/*   pdf_char_t *output_data = NULL; */
/*   pdf_size_t output_size = 0; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF8, &text) != PDF_OK); */
/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_006:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, */
/*                               PDF_TEXT_FILTER_NORM_WITH_FULL_WIDTH) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_006:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF8, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must remain unchanged. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(output_data == NULL); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_010 */
/*  * Description: */
/*  *   Apply the `Upper case' filter to a text object that contains */
/*  *   simple-case-conversion characters. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *   3. The length of the output text must be equal to the length of the */
/*  *      input text. */
/*  *\/ */
/* START_TEST (pdf_text_filter_010) */
/* { */



/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00"" " \ */
/*                                                 "\x00\x00\x00""g" \ */
/*                                                 "\x00\x00\x00""n" \ */
/*                                                 "\x00\x00\x00""u" \ */
/*                                                 "\x00\x00\x00"" "; */
/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00"" " \ */
/*                                                    "\x00\x00\x00""G" \ */
/*                                                    "\x00\x00\x00""N" \ */
/*                                                    "\x00\x00\x00""U" \ */
/*                                                    "\x00\x00\x00"" "; */
/*   pdf_size_t input_size = 20; */
/*   pdf_size_t expected_size = input_size; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_010:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_010:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 3. The length of the output text must be equal to the length of the */
/*    *      input text. *\/ */
/*   fail_unless(output_size == expected_size); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_011 */
/*  * Description: */
/*  *   Apply the `Upper case' filter to a text object that contains */
/*  *   special-case-conversion characters with no context condition. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_011) */
/* { */



/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x1E\x98" \ */
/*                                                 "\x00\x00\x1F\xE4" \ */
/*                                                 "\x00\x00\x1F\x80" \ */
/*                                                 "\x00\x00\x1F\xC4"; */
/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                           "\x00\x00\x00\x57\x00\x00\x03\x0A" \ */
/*                                           "\x00\x00\x03\xA1\x00\x00\x03\x13" \ */
/*                                           "\x00\x00\x1F\x08\x00\x00\x03\x99" \ */
/*                                           "\x00\x00\x03\x89\x00\x00\x03\x99"; */
/*   pdf_size_t input_size = 20; */
/*   pdf_size_t expected_size = 36; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_011:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_011:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_012 */
/*  * Description: */
/*  *   Apply the `Upper case' filter to a text object that contains */
/*  *   special-case-conversion characters with Final_Sigma context condition. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_012) */
/* { */



/*   /\* Final_Sigma condition in Special Case algorithms */
/*    *  When the given character is preceded by a sequence consisting of a */
/*    *  cased unicode point (with case information) and a case-ignorable */
/*    *  (without case information) sequence of points. The given character */
/*    *  must no be followed by a sequence consisting of a case-ignorable */
/*    *  sequence and a cased letter. */
/*    * */
/*    * - 0x03A3 point is the one with the Final_Sigma condition */
/*    * - 0x0055 point is the cased point */
/*    * - 0x003A point is the case-ignorable point (MidLetter WB property) */
/*    * - 0x0020 point (whitespace) is used as word separator to have three */
/*    *    different words. */
/*    * If condition is fulfilled, expected uppercase character for 0x03A3 is */
/*    *  the same 0x03A3. */
/*    *\/ */





/*   /\* 1st check, the sequence of previous case-ignorable points is empty *\/ */
/*   if(1) */
/*   { */
/*     const pdf_char_t *input_data = (pdf_char_t *) \ */
/*                                    "\x00\x00\x00\x47" "\x00\x00\x00\x4E" \ */
/*                                    "\x00\x00\x00\x55" "\x00\x00\x00\x20" \ */
/*                                    "\x00\x00\x00\x55" \ */
/*                                    "\x00\x00\x03\xA3" "\x00\x00\x00\x20" \ */
/*                                    "\x00\x00\x00\x47" "\x00\x00\x00\x4E" \ */
/*                                    "\x00\x00\x00\x55"; */
/*     const pdf_char_t *expected_data = input_data; */
/*     const pdf_size_t input_size = 40; */
/*     pdf_char_t *output_data; */
/*     pdf_size_t output_size; */
/*     pdf_text_t text; */

/*     /\* Create text object with given contents *\/ */
/*     fail_if(pdf_text_new_from_unicode(input_data, */
/*                                       input_size, */
/*                                       PDF_TEXT_UTF32_BE, &text) != PDF_OK); */

/*     /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*     fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK); */

/*     if(INTERACTIVE_DEBUG) */
/*       { */
/*         pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*         fail_if(internal_hex == NULL); */
/*         printf("pdf_text_filter:12:1:Internal> '%s'\n", internal_hex); */
/*         pdf_dealloc(internal_hex); */
/*       } */

/*     /\* Get output as UTF-32BE *\/ */
/*     fail_unless(pdf_text_get_unicode(&output_data, */
/*                                      &output_size, */
/*                                      text, */
/*                                      PDF_TEXT_UTF32_BE, */
/*                                      PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*     /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*     fail_unless(output_size == input_size); */
/*     fail_unless(memcmp(output_data, expected_data, input_size) == 0); */

/*     pdf_dealloc(output_data); */
/*     pdf_text_destroy(text); */
/*   } */

/*   /\* 2nd check, the sequence of previous case-ignorable points is NOT empty *\/ */
/*   if(1) */
/*     { */
/*       const pdf_char_t *input_data = (pdf_char_t *) \ */
/*                                      "\x00\x00\x00\x47" "\x00\x00\x00\x4E" \ */
/*                                      "\x00\x00\x00\x55" "\x00\x00\x00\x20" \ */
/*                                      "\x00\x00\x00\x55" "\x00\x00\x00\x3A" \ */
/*                                      "\x00\x00\x03\xA3" "\x00\x00\x00\x20" \ */
/*                                      "\x00\x00\x00\x47" "\x00\x00\x00\x4E" \ */
/*                                      "\x00\x00\x00\x55"; */
/*       const pdf_char_t *expected_data = input_data; */
/*       const pdf_size_t input_size = 44; */
/*       pdf_char_t *output_data; */
/*       pdf_size_t output_size; */
/*       pdf_text_t text; */

/*       /\* Create text object with given contents *\/ */
/*       fail_if(pdf_text_new_from_unicode(input_data, */
/*                                         input_size, */
/*                                         PDF_TEXT_UTF32_BE, &text) != PDF_OK); */

/*       /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*       fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK); */

/*       if(INTERACTIVE_DEBUG) */
/*         { */
/*           pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*           fail_if(internal_hex == NULL); */
/*           printf("pdf_text_filter:12:2:Internal> '%s'\n", internal_hex); */
/*           pdf_dealloc(internal_hex); */
/*         } */

/*       /\* Get output as UTF-32BE *\/ */
/*       fail_unless(pdf_text_get_unicode(&output_data, */
/*                                        &output_size, */
/*                                        text, */
/*                                        PDF_TEXT_UTF32_BE, */
/*                                        PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*       /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*       fail_unless(output_size == input_size); */
/*       fail_unless(memcmp(output_data, expected_data, input_size) == 0); */

/*       pdf_dealloc(output_data); */
/*       pdf_text_destroy(text); */
/*     } */

/* } */
/* END_TEST */

/* /\* */
/*  * Test: pdf_text_filter_013 */
/*  * Description: */
/*  *   Apply the `Upper case' filter to a text object that contains */
/*  *   special-case-conversion characters with After_Soft_Dotted context */
/*  *   condition. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_013) */
/* { */



/*   /\* 0307; 0307; ; ; lt After_Soft_Dotted; # COMBINING DOT ABOVE *\/ */
/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00" "i" */
/*                                                 "\x00\x00\x03\x07"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                    "\x00\x00\x00" "I"; */
/*   pdf_size_t input_size = 12; */
/*   pdf_size_t expected_size = 8; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'lt' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_013:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_013:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */

/* /\* */
/*  * Test: pdf_text_filter_014 */
/*  * Description: */
/*  *   Apply the `Upper case' filter to a text object that contains */
/*  *   special-case-conversion characters with More_Above context condition. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_014) */
/* { */



/*   /\* Code:0049; Lower:0069 0307; Title:0049; Upper:0049; */
/*    * lt More_Above; # LATIN CAPITAL LETTER I *\/ */

/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00\x49" */
/*                                                 "\x00\x00\x03\x0B"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                    "\x00\x00\x00\x49" */
/*                                                    "\x00\x00\x03\x0B"; */
/*   pdf_size_t input_size = 12; */
/*   pdf_size_t expected_size = 12; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'lt' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_014:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_014:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */


/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_015 */
/*  * Description: */
/*  *   Apply the `Upper case' filter to a text object that contains */
/*  *   special-case-conversion characters with Before_Dot context condition. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_015) */
/* { */
/*   /\* NOTE: Actually there is no special condition involving "Before_Dot" as is */
/*    *       (there are a couple of conditions involving the negated one: */
/*    *       Not_Before_Dot). But probably is a good idea to at least leave */
/*    *       the test, just in case in future updates of the Unicode standard */
/*    *       one of this type appears *\/ */
/*    fail_if(0); */
/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_016 */
/*  * Description: */
/*  *   Apply the `Upper case' filter to a text object that contains */
/*  *   special-case-conversion characters with After_I context condition. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_016) */
/* { */



/*   /\* Code: 0307;Lower: ; Title: 0307; Upper: 0307; */
/*    * tr After_I; # COMBINING DOT ABOVE *\/ */

/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00""I" \ */
/*                                                 "\x00\x00\x03\x07"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                    "\x00\x00\x00""I" \ */
/*                                                    "\x00\x00\x03\x07"; */
/*   pdf_size_t input_size = 12; */
/*   pdf_size_t expected_size = 12; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'tr' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_016:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_016:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_017 */
/*  * Description: */
/*  *   Apply the `Upper case' filter to a text object that contains */
/*  *   special-case-conversion characters with language-dependent context */
/*  *   condition. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_017) */
/* { */



/*   /\* # When uppercasing, i turns into a dotted capital I */
/*    * 0069; 0069; 0130; 0130; tr; # LATIN SMALL LETTER I *\/ */

/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00\x69"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                    "\x00\x00\x01\x30"; */
/*   pdf_size_t input_size = 8; */
/*   pdf_size_t expected_size = 8; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'tr' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_017:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_017:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_018 */
/*  * Description: */
/*  *   Apply the `Upper case' filter to a text object that contains */
/*  *   special-case-conversion characters with language-dependent and After_I */
/*  *    context condition (to check two context conditions at the same time). */
/*  * Note: As long as After_I is always coming with a language condition (at least */
/*  *   in the Unicode standard 5.0), this test is equal to pdf_text_filter_016 */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_018) */
/* { */



/*   /\* Code: 0307;Lower: ; Title: 0307; Upper: 0307; */
/*    * tr After_I; # COMBINING DOT ABOVE *\/ */

/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00""I" \ */
/*                                                 "\x00\x00\x03\x07"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                    "\x00\x00\x00""I" \ */
/*                                                    "\x00\x00\x03\x07"; */
/*   pdf_size_t input_size = 12; */
/*   pdf_size_t expected_size = 12; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'tr' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_016:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_016:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */



/* /\* */
/*  * Test: pdf_text_filter_019 */
/*  * Description: */
/*  *   Apply the `Upper case' filter to a text object that contains */
/*  *   special-case-conversion characters with language-dependent and */
/*  *   Not_Before_Dot context condition (to check two context conditions at the */
/*  *   same time, where one of them is negated with Not_). */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_019) */
/* { */



/*   /\* # When lowercasing, unless an I is before a dot_above, it turns into a */
/*    *   dotless i. */
/*    * 0049; 0131; 0049; 0049; tr Not_Before_Dot; # LATIN CAPITAL LETTER I *\/ */

/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00\x49" \ */
/*                                                 "\x00\x00\x00\x50"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                    "\x00\x00\x00\x49" \ */
/*                                                    "\x00\x00\x00\x50"; */
/*   pdf_size_t input_size = 12; */
/*   pdf_size_t expected_size = 12; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'tr' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_016:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_016:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */

/* /\* */
/*  * Test: pdf_text_filter_020 */
/*  * Description: */
/*  *   Apply the `Upper case' filter to a text object that contains */
/*  *   special-case-conversion characters with Final_Sigma context condition, */
/*  *   where the condition is not fulfilled. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_020) */
/* { */



/*   /\* Final_Sigma condition in Special Case algorithms */
/*    *  When the given character is preceded by a sequence consisting of a */
/*    *  cased unicode point (with case information) and a case-ignorable */
/*    *  (without case information) sequence of points. The given character */
/*    *  must no be followed by a sequence consisting of a case-ignorable */
/*    *  sequence and a cased letter. */
/*    * */
/*    * The condition won't be fulfilled because: */
/*    *  1st check. The previous cased point is not available */
/*    *  2nd check. A next cased point is available */
/*    * */
/*    * - 0x03A3 point is the one with the Final_Sigma condition */
/*    * - 0x0055 point is the cased point */
/*    * - 0x0027 point is the case-ignorable point */
/*    * - 0x0020 point (whitespace) is used as word separator to have three */
/*    *    different words. */
/*    * */
/*    * If condition is not fulfilled, expected uppercase character for 0x03A3 is */
/*    *  the same 0x03A3. */
/*    *\/ */





/*   /\* 1st check, the previous cased point is not available *\/ */
/*   if(1) */
/*     { */
/*       const pdf_char_t *input_data = (pdf_char_t *) \ */
/*         "\x00\x00\x00\x47" "\x00\x00\x00\x4E"                           \ */
/*                                      "\x00\x00\x00\x55" "\x00\x00\x00\x20" \ */
/*                                      "\x00\x00\x00\x27" */
/*                                      "\x00\x00\x03\xA3" "\x00\x00\x00\x20" \ */
/*                                      "\x00\x00\x00\x47" "\x00\x00\x00\x4E" \ */
/*                                      "\x00\x00\x00\x55"; */
/*       const pdf_char_t *expected_data = input_data; */
/*       const pdf_size_t input_size = 40; */
/*       pdf_char_t *output_data; */
/*       pdf_size_t output_size; */
/*       pdf_text_t text; */

/*       /\* Create text object with given contents *\/ */
/*       fail_if(pdf_text_new_from_unicode(input_data, */
/*                                         input_size, */
/*                                         PDF_TEXT_UTF32_BE, &text) != PDF_OK); */

/*       /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*       fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK); */

/*       if(INTERACTIVE_DEBUG) */
/*         { */
/*           pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*           fail_if(internal_hex == NULL); */
/*           printf("pdf_text_filter:20:1:Internal> '%s'\n", internal_hex); */
/*           pdf_dealloc(internal_hex); */
/*         } */

/*       /\* Get output as UTF-32BE *\/ */
/*       fail_unless(pdf_text_get_unicode(&output_data, */
/*                                        &output_size, */
/*                                        text, */
/*                                        PDF_TEXT_UTF32_BE, */
/*                                        PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*       /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*       fail_unless(output_size == input_size); */
/*       fail_unless(memcmp(output_data, expected_data, input_size) == 0); */

/*       pdf_dealloc(output_data); */
/*       pdf_text_destroy(text); */
/*     } */

/*   /\* 2nd check, the sequence of previous case-ignorable points is NOT empty *\/ */
/*   if(1) */
/*     { */
/*       const pdf_char_t *input_data = (pdf_char_t *) \ */
/*                                      "\x00\x00\x00\x47" "\x00\x00\x00\x4E" \ */
/*                                      "\x00\x00\x00\x55" "\x00\x00\x00\x20" \ */
/*                                      "\x00\x00\x00\x55" "\x00\x00\x00\x27" \ */
/*                                      "\x00\x00\x03\xA3" "\x00\x00\x00\x55" \ */
/*                                      "\x00\x00\x00\x20" \ */
/*                                      "\x00\x00\x00\x47" "\x00\x00\x00\x4E" \ */
/*                                      "\x00\x00\x00\x55"; */
/*       const pdf_char_t *expected_data = input_data; */
/*       const pdf_size_t input_size = 48; */
/*       pdf_char_t *output_data; */
/*       pdf_size_t output_size; */
/*       pdf_text_t text; */

/*       /\* Create text object with given contents *\/ */
/*       fail_if(pdf_text_new_from_unicode(input_data, */
/*                                         input_size, */
/*                                         PDF_TEXT_UTF32_BE, &text) != PDF_OK); */

/*       /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*       fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK); */

/*       if(INTERACTIVE_DEBUG) */
/*         { */
/*           pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*           fail_if(internal_hex == NULL); */
/*           printf("pdf_text_filter:20:2:Internal> '%s'\n", internal_hex); */
/*           pdf_dealloc(internal_hex); */
/*         } */

/*       /\* Get output as UTF-32BE *\/ */
/*       fail_unless(pdf_text_get_unicode(&output_data, */
/*                                        &output_size, */
/*                                        text, */
/*                                        PDF_TEXT_UTF32_BE, */
/*                                        PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*       /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*       fail_unless(output_size == input_size); */
/*       fail_unless(memcmp(output_data, expected_data, input_size) == 0); */

/*       pdf_dealloc(output_data); */
/*       pdf_text_destroy(text); */
/*     } */

/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_021 */
/*  * Description: */
/*  *   Apply the `Upper case' filter to a text object that contains */
/*  *   special-case-conversion characters with After_Soft_Dotted context */
/*  *   condition, where the condition is not fulfilled. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_021) */
/* { */



/*   /\* 0x030C has combining class 230, so condition should not be fulfilled *\/ */
/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00" "i" \ */
/*                                                 "\x00\x00\x03\x0C" \ */
/*                                                 "\x00\x00\x03\x07"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                    "\x00\x00\x00" "I" \ */
/*                                                    "\x00\x00\x03\x0C" \ */
/*                                                    "\x00\x00\x03\x07"; */
/*   pdf_size_t input_size = 16; */
/*   pdf_size_t expected_size = 16; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'lt' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_021:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_021:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */

/* /\* */
/*  * Test: pdf_text_filter_022 */
/*  * Description: */
/*  *   Apply the `Upper case' filter to a text object that contains */
/*  *   special-case-conversion characters with More_Above context condition, */
/*  *   where the condition is not fulfilled. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_022) */
/* { */



/*   /\* Code:0049; Lower:0069 0307; Title:0049; Upper:0049; */
/*    * lt More_Above; # LATIN CAPITAL LETTER I */
/*    * As 0xC0 has combining class 0, the condition must not be fullfilled*\/ */

/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00\x49" \ */
/*                                                 "\x00\x00\x00\xC0" \ */
/*                                                 "\x00\x00\x03\x0B"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                    "\x00\x00\x00\x49" \ */
/*                                                    "\x00\x00\x00\xC0" \ */
/*                                                    "\x00\x00\x03\x0B"; */
/*   pdf_size_t input_size = 16; */
/*   pdf_size_t expected_size = 16; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'lt' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_022:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_022:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_023 */
/*  * Description: */
/*  *   Apply the `Upper case' filter to a text object that contains */
/*  *   special-case-conversion characters with Before_Dot context condition, */
/*  *   where the condition is not fulfilled. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_023) */
/* { */
/*   /\* NOTE: Actually there is no special condition involving "Before_Dot" as is */
/*    *       (there are a couple of conditions involving the negated one: */
/*    *       Not_Before_Dot). But probably is a good idea to at least leave */
/*    *       the test, just in case in future updates of the Unicode standard */
/*    *       one of this type appears *\/ */
/*   fail_if(0); */
/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_024 */
/*  * Description: */
/*  *   Apply the `Upper case' filter to a text object that contains */
/*  *   special-case-conversion characters with After_I context condition, */
/*  *   where the condition is not fulfilled. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_024) */
/* { */



/*   /\* Code: 0307;Lower: ; Title: 0307; Upper: 0307; */
/*    * tr After_I; # COMBINING DOT ABOVE *\/ */
/*   /\* 0x030C has combining class 230, so condition should not be fulfilled *\/ */

/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00""I" \ */
/*                                                 "\x00\x00\x03\x0C" \ */
/*                                                 "\x00\x00\x03\x07"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                    "\x00\x00\x00""I" \ */
/*                                                    "\x00\x00\x03\x0C" \ */
/*                                                    "\x00\x00\x03\x07"; */
/*   pdf_size_t input_size = 16; */
/*   pdf_size_t expected_size = 16; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'tr' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_024:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_024:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_025 */
/*  * Description: */
/*  *   Apply the `Upper case' filter to a text object that contains */
/*  *   special-case-conversion characters with language-dependent context */
/*  *   condition, where the condition is not fulfilled. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_025) */
/* { */



/*   /\* # When uppercasing, i turns into a dotted capital I */
/*    * 0069; 0069; 0130; 0130; tr; # LATIN SMALL LETTER I *\/ */

/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00""i"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                    "\x00\x00\x00""I"; */
/*   pdf_size_t input_size = 8; */
/*   pdf_size_t expected_size = 8; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'en' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"en") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_025:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_025:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_026 */
/*  * Description: */
/*  *   Apply the `Upper case' filter to a text object that contains */
/*  *   special-case-conversion characters with language-dependent and After_I */
/*  *    context condition (to check two context conditions at the same time), */
/*  *    where the condition is not fulfilled. */
/*  * Note: As long as After_I is always coming with a language condition (at least */
/*  *   in the Unicode standard 5.0), this test is equal to pdf_text_filter_024 */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_026) */
/* { */



/*   /\* Code: 0307;Lower: ; Title: 0307; Upper: 0307; */
/*    * tr After_I; # COMBINING DOT ABOVE *\/ */
/*   /\* 0x030C has combining class 230, so condition should not be fulfilled *\/ */

/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00""I" \ */
/*                                                 "\x00\x00\x03\x0C" \ */
/*                                                 "\x00\x00\x03\x07"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                    "\x00\x00\x00""I" \ */
/*                                                    "\x00\x00\x03\x0C" \ */
/*                                                    "\x00\x00\x03\x07"; */
/*   pdf_size_t input_size = 16; */
/*   pdf_size_t expected_size = 16; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'tr' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_026:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_026:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */



/* /\* */
/*  * Test: pdf_text_filter_027 */
/*  * Description: */
/*  *   Apply the `Upper case' filter to a text object that contains */
/*  *   special-case-conversion characters with language-dependent and */
/*  *   Not_Before_Dot context condition (to check two context conditions at the */
/*  *   same time, where one of them is negated with Not_), where the condition is */
/*  *   not fulfilled. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_027) */
/* { */



/*   /\* # When lowercasing, unless an I is before a dot_above, it turns into a */
/*    *   dotless i. */
/*    * 0049; 0131; 0049; 0049; tr Not_Before_Dot; # LATIN CAPITAL LETTER I */
/*    * Not Not_Before_Dot... is equal to Before_Dot :-) *\/ */

/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00\x49" \ */
/*                                                 "\x00\x00\x03\x07" \ */
/*                                                 "\x00\x00\x00\x50"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                    "\x00\x00\x00\x49" \ */
/*                                                    "\x00\x00\x03\x07" \ */
/*                                                    "\x00\x00\x00\x50"; */
/*   pdf_size_t input_size = 16; */
/*   pdf_size_t expected_size = 16; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'tr' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_027:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_UPPER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_027:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */



/* /\* */
/*  * Test: pdf_text_filter_028 */
/*  * Description: */
/*  *   Apply the `Lower case' filter to a text object that contains */
/*  *   simple-case-conversion characters. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *   3. The length of the output text must be equal to the length of the */
/*  *      input text. */
/*  *\/ */
/* START_TEST (pdf_text_filter_028) */
/* { */



/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00"" " \ */
/*                                                 "\x00\x00\x00""G" \ */
/*                                                 "\x00\x00\x00""N" \ */
/*                                                 "\x00\x00\x00""U" \ */
/*                                                 "\x00\x00\x00"" "; */
/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00"" " \ */
/*                                                    "\x00\x00\x00""g" \ */
/*                                                    "\x00\x00\x00""n" \ */
/*                                                    "\x00\x00\x00""u" \ */
/*                                                    "\x00\x00\x00"" "; */
/*   pdf_size_t input_size = 20; */
/*   pdf_size_t expected_size = input_size; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_028:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_028:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 3. The length of the output text must be equal to the length of the */
/*    *      input text. *\/ */
/*   fail_unless(output_size == expected_size); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_029 */
/*  * Description: */
/*  *   Apply the `Lower case' filter to a text object that contains */
/*  *   special-case-conversion characters with no context condition. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_029) */
/* { */



/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x01\x30"; */
/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                           "\x00\x00\x00\x69\x00\x00\x03\x07"; */
/*   pdf_size_t input_size = 8; */
/*   pdf_size_t expected_size = 12; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_029:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_029:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */



/* /\* */
/*  * Test: pdf_text_filter_030 */
/*  * Description: */
/*  *   Apply the `Lower case' filter to a text object that contains */
/*  *   special-case-conversion characters with Final_Sigma context condition. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_030) */
/* { */



/*   /\* Final_Sigma condition in Special Case algorithms */
/*    *  When the given character is preceded by a sequence consisting of a */
/*    *  cased unicode point (with case information) and a case-ignorable */
/*    *  (without case information) sequence of points. The given character */
/*    *  must no be followed by a sequence consisting of a case-ignorable */
/*    *  sequence and a cased letter. */
/*    * */
/*    * - 0x03A3 point is the one with the Final_Sigma condition */
/*    * - 0x0055 point is the cased point */
/*    * - 0x003A point is the case-ignorable point (MidLetter WB property) */
/*    * - 0x0020 point (whitespace) is used as word separator to have three */
/*    *    different words. */
/*    * If condition is fulfilled, expected smallcase character for 0x03A3 is */
/*    *  different: 0x03C2. */
/*    *\/ */





/*   /\* 1st check, the sequence of previous case-ignorable points is empty *\/ */
/*   if(1) */
/*     { */
/*       const pdf_char_t *input_data = (pdf_char_t *) \ */
/*                                      "\x00\x00\x00""G" "\x00\x00\x00""N" \ */
/*                                      "\x00\x00\x00""U" "\x00\x00\x00"" " \ */
/*                                      "\x00\x00\x00""G" \ */
/*                                      "\x00\x00\x03\xA3" "\x00\x00\x00"" " \ */
/*                                      "\x00\x00\x00""G" "\x00\x00\x00""N" \ */
/*                                      "\x00\x00\x00""U"; */
/*       const pdf_char_t *expected_data = (pdf_char_t *) \ */
/*                                         "\x00\x00\x00""g" "\x00\x00\x00""n" \ */
/*                                         "\x00\x00\x00""u" "\x00\x00\x00"" " \ */
/*                                         "\x00\x00\x00""g" \ */
/*                                         "\x00\x00\x03\xC2" "\x00\x00\x00"" " \ */
/*                                         "\x00\x00\x00""g" "\x00\x00\x00""n" \ */
/*                                         "\x00\x00\x00""u"; */
/*       const pdf_size_t input_size = 40; */
/*       pdf_char_t *output_data; */
/*       pdf_size_t output_size; */
/*       pdf_text_t text; */

/*       /\* Create text object with given contents *\/ */
/*       fail_if(pdf_text_new_from_unicode(input_data, */
/*                                         input_size, */
/*                                         PDF_TEXT_UTF32_BE, &text) != PDF_OK); */

/*       if(INTERACTIVE_DEBUG) */
/*         { */
/*           pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*           fail_if(internal_hex == NULL); */
/*           printf("pdf_text_filter:30:1:Before> '%s'\n", internal_hex); */
/*           pdf_dealloc(internal_hex); */
/*         } */

/*       /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*       fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK); */

/*       if(INTERACTIVE_DEBUG) */
/*         { */
/*           pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*           fail_if(internal_hex == NULL); */
/*           printf("pdf_text_filter:30:1:After> '%s'\n", internal_hex); */
/*           pdf_dealloc(internal_hex); */
/*         } */

/*       /\* Get output as UTF-32BE *\/ */
/*       fail_unless(pdf_text_get_unicode(&output_data, */
/*                                        &output_size, */
/*                                        text, */
/*                                        PDF_TEXT_UTF32_BE, */
/*                                        PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*       /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*       fail_unless(output_size == input_size); */
/*       fail_unless(memcmp(output_data, expected_data, input_size) == 0); */

/*       pdf_dealloc(output_data); */
/*       pdf_text_destroy(text); */
/*     } */

/*   /\* 2nd check, the sequence of previous case-ignorable points is NOT empty *\/ */
/*   if(1) */
/*     { */
/*       const pdf_char_t *input_data = (pdf_char_t *) \ */
/*                                      "\x00\x00\x00""G" "\x00\x00\x00""N" \ */
/*                                      "\x00\x00\x00""U" "\x00\x00\x00"" " \ */
/*                                      "\x00\x00\x00""G" "\x00\x00\x00\x3A" \ */
/*                                      "\x00\x00\x03\xA3" "\x00\x00\x00"" " \ */
/*                                      "\x00\x00\x00""G" "\x00\x00\x00""N" \ */
/*                                      "\x00\x00\x00""U"; */
/*       const pdf_char_t *expected_data = (pdf_char_t *) \ */
/*                                         "\x00\x00\x00""g" "\x00\x00\x00""n" \ */
/*                                         "\x00\x00\x00""u" "\x00\x00\x00"" " \ */
/*                                         "\x00\x00\x00""g" "\x00\x00\x00\x3A" \ */
/*                                         "\x00\x00\x03\xC2" "\x00\x00\x00"" " \ */
/*                                         "\x00\x00\x00""g" "\x00\x00\x00""n" \ */
/*                                         "\x00\x00\x00""u"; */
/*       const pdf_size_t input_size = 44; */
/*       pdf_char_t *output_data; */
/*       pdf_size_t output_size; */
/*       pdf_text_t text; */

/*       /\* Create text object with given contents *\/ */
/*       fail_if(pdf_text_new_from_unicode(input_data, */
/*                                         input_size, */
/*                                         PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*       if(INTERACTIVE_DEBUG) */
/*         { */
/*           pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*           fail_if(internal_hex == NULL); */
/*           printf("pdf_text_filter:30:2:Before> '%s'\n", internal_hex); */
/*           pdf_dealloc(internal_hex); */
/*         } */

/*       /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*       fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK); */

/*       if(INTERACTIVE_DEBUG) */
/*         { */
/*           pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*           fail_if(internal_hex == NULL); */
/*           printf("pdf_text_filter:30:2:After> '%s'\n", internal_hex); */
/*           pdf_dealloc(internal_hex); */
/*         } */

/*       /\* Get output as UTF-32BE *\/ */
/*       fail_unless(pdf_text_get_unicode(&output_data, */
/*                                        &output_size, */
/*                                        text, */
/*                                        PDF_TEXT_UTF32_BE, */
/*                                        PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*       /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*       fail_unless(output_size == input_size); */
/*       fail_unless(memcmp(output_data, expected_data, input_size) == 0); */

/*       pdf_dealloc(output_data); */
/*       pdf_text_destroy(text); */
/*     } */

/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_031 */
/*  * Description: */
/*  *   Apply the `Lower case' filter to a text object that contains */
/*  *   special-case-conversion characters with After_Soft_Dotted context */
/*  *   condition. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_031) */
/* { */



/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00" "i" \ */
/*                                                 "\x00\x00\x03\x07"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                    "\x00\x00\x00" "i" \ */
/*                                                    "\x00\x00\x03\x07"; */
/*   pdf_size_t input_size = 12; */
/*   pdf_size_t expected_size = 12; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'lt' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_031:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_031:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */

/* /\* */
/*  * Test: pdf_text_filter_032 */
/*  * Description: */
/*  *   Apply the `Lower case' filter to a text object that contains */
/*  *   special-case-conversion characters with More_Above context condition. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_032) */
/* { */



/*   /\* Code:0049; Lower:0069 0307; Title:0049; Upper:0049; */
/*    * lt More_Above; # LATIN CAPITAL LETTER I *\/ */

/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00\x49" */
/*                                                 "\x00\x00\x03\x0B"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                               "\x00\x00\x00\x69\x00\x00\x03\x07" */
/*                                                    "\x00\x00\x03\x0B"; */
/*   pdf_size_t input_size = 12; */
/*   pdf_size_t expected_size = 16; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'lt' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_032:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_032:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_033 */
/*  * Description: */
/*  *   Apply the `Lower case' filter to a text object that contains */
/*  *   special-case-conversion characters with Before_Dot context condition. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_033) */
/* { */
/*   /\* NOTE: Actually there is no special condition involving "Before_Dot" as is */
/*    *       (there are a couple of conditions involving the negated one: */
/*    *       Not_Before_Dot). But probably is a good idea to at least leave */
/*    *       the test, just in case in future updates of the Unicode standard */
/*    *       one of this type appears *\/ */
/*   fail_if(0); */
/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_034 */
/*  * Description: */
/*  *   Apply the `Lower case' filter to a text object that contains */
/*  *   special-case-conversion characters with After_I context condition. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_034) */
/* { */



/*   /\* Code: 0307;Lower: ; Title: 0307; Upper: 0307; */
/*    * tr After_I; # COMBINING DOT ABOVE *\/ */

/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00""I" \ */
/*                                                 "\x00\x00\x03\x07"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                    "\x00\x00\x00""i"; */
/*   pdf_size_t input_size = 12; */
/*   pdf_size_t expected_size = 8; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'tr' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_034:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_034:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_035 */
/*  * Description: */
/*  *   Apply the `Lower case' filter to a text object that contains */
/*  *   special-case-conversion characters with language-dependent context */
/*  *   condition. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_035) */
/* { */



/*   /\* 00CC; 0069 0307 0300; 00CC; 00CC; lt; # LATIN CAPITAL LETTER I WITH GRAVE*\/ */

/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00\xCC"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                   "\x00\x00\x00\x69\x00\x00\x03\x07\x00\x00\x03\x00"; */
/*   pdf_size_t input_size = 8; */
/*   pdf_size_t expected_size = 16; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'lt' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_035:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_035:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_036 */
/*  * Description: */
/*  *   Apply the `Lower case' filter to a text object that contains */
/*  *   special-case-conversion characters with language-dependent and After_I */
/*  *    context condition (to check two context conditions at the same time). */
/*  * Note: As long as After_I is always coming with a language condition (at least */
/*  *   in the Unicode standard 5.0), this test is equal to pdf_text_filter_034 */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_036) */
/* { */



/*   /\* Code: 0307;Lower: ; Title: 0307; Upper: 0307; */
/*    * tr After_I; # COMBINING DOT ABOVE *\/ */

/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00""I" \ */
/*                                                 "\x00\x00\x03\x07"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                    "\x00\x00\x00""i"; */
/*   pdf_size_t input_size = 12; */
/*   pdf_size_t expected_size = 8; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'tr' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_016:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_016:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */



/* /\* */
/*  * Test: pdf_text_filter_037 */
/*  * Description: */
/*  *   Apply the `Lower case' filter to a text object that contains */
/*  *   special-case-conversion characters with language-dependent and */
/*  *   Not_Before_Dot context condition (to check two context conditions at the */
/*  *   same time, where one of them is negated with Not_). */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_037) */
/* { */



/*   /\* # When lowercasing, unless an I is before a dot_above, it turns into a */
/*    *   dotless i. */
/*    * 0049; 0131; 0049; 0049; tr Not_Before_Dot; # LATIN CAPITAL LETTER I *\/ */

/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00\x49" \ */
/*                                                 "\x00\x00\x00\x50"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                    "\x00\x00\x01\x31" \ */
/*                                                    "\x00\x00\x00\x70"; */
/*   pdf_size_t input_size = 12; */
/*   pdf_size_t expected_size = 12; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'tr' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_037:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_037:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_038 */
/*  * Description: */
/*  *   Apply the `Lower case' filter to a text object that contains */
/*  *   special-case-conversion characters with Final_Sigma context condition, */
/*  *   where the condition is not fulfilled. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_038) */
/* { */



/*   /\* Final_Sigma condition in Special Case algorithms */
/*    *  When the given character is preceded by a sequence consisting of a */
/*    *  cased unicode point (with case information) and a case-ignorable */
/*    *  (without case information) sequence of points. The given character */
/*    *  must no be followed by a sequence consisting of a case-ignorable */
/*    *  sequence and a cased letter. */
/*    * */
/*    * The condition won't be fulfilled because: */
/*    *  1st check. The previous cased point is not available */
/*    *  2nd check. A next cased point is available */
/*    * */
/*    * - 0x03A3 point is the one with the Final_Sigma condition */
/*    * - 0x0055 point is the cased point */
/*    * - 0x0027 point is the case-ignorable point */
/*    * - 0x0020 point (whitespace) is used as word separator to have three */
/*    *    different words. */
/*    * */
/*    * If condition is not fulfilled, expected lowercase character for 0x03A3 is */
/*    *  the non-final lowercase sigma 0x03C3. */
/*    *\/ */





/*   /\* 1st check, the previous cased point is not available *\/ */
/*   if(1) */
/*     { */
/*       const pdf_char_t *input_data = (pdf_char_t *) \ */
/*                                      "\x00\x00\x00""G" "\x00\x00\x00""N" \ */
/*                                      "\x00\x00\x00""U" "\x00\x00\x00"" " \ */
/*                                      "\x00\x00\x00\x27" \ */
/*                                      "\x00\x00\x03\xA3" "\x00\x00\x00"" " \ */
/*                                      "\x00\x00\x00""G" "\x00\x00\x00""N" \ */
/*                                      "\x00\x00\x00""U"; */
/*       const pdf_char_t *expected_data = (pdf_char_t *) \ */
/*                                         "\x00\x00\x00""g" "\x00\x00\x00""n" \ */
/*                                         "\x00\x00\x00""u" "\x00\x00\x00"" " \ */
/*                                         "\x00\x00\x00\x27" \ */
/*                                         "\x00\x00\x03\xC3" "\x00\x00\x00"" " \ */
/*                                         "\x00\x00\x00""g" "\x00\x00\x00""n" \ */
/*                                         "\x00\x00\x00""u"; */
/*       const pdf_size_t input_size = 40; */
/*       pdf_char_t *output_data; */
/*       pdf_size_t output_size; */
/*       pdf_text_t text; */

/*       /\* Create text object with given contents *\/ */
/*       fail_if(pdf_text_new_from_unicode(input_data, */
/*                                         input_size, */
/*                                         PDF_TEXT_UTF32_BE, &text) != PDF_OK); */

/*       /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*       fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK); */

/*       if(INTERACTIVE_DEBUG) */
/*         { */
/*           pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*           fail_if(internal_hex == NULL); */
/*           printf("pdf_text_filter:38:1:Internal> '%s'\n", internal_hex); */
/*           pdf_dealloc(internal_hex); */
/*         } */

/*       /\* Get output as UTF-32BE *\/ */
/*       fail_unless(pdf_text_get_unicode(&output_data, */
/*                                        &output_size, */
/*                                        text, */
/*                                        PDF_TEXT_UTF32_BE, */
/*                                        PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*       /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*       fail_unless(output_size == input_size); */
/*       fail_unless(memcmp(output_data, expected_data, input_size) == 0); */

/*       pdf_dealloc(output_data); */
/*       pdf_text_destroy(text); */
/*     } */

/*   /\* 2nd check, a next cased point is available *\/ */
/*   if(1) */
/*     { */
/*       const pdf_char_t *input_data = (pdf_char_t *) \ */
/*                                      "\x00\x00\x00""G" "\x00\x00\x00""N" \ */
/*                                      "\x00\x00\x00""U" "\x00\x00\x00"" " \ */
/*                                      "\x00\x00\x00""G" "\x00\x00\x00\x27" \ */
/*                                      "\x00\x00\x03\xA3" "\x00\x00\x00""G" \ */
/*                                      "\x00\x00\x00"" " \ */
/*                                      "\x00\x00\x00""G" "\x00\x00\x00""N" \ */
/*                                      "\x00\x00\x00""U"; */
/*       const pdf_char_t *expected_data = (pdf_char_t *) \ */
/*                                         "\x00\x00\x00""g" "\x00\x00\x00""n" \ */
/*                                         "\x00\x00\x00""u" "\x00\x00\x00"" " \ */
/*                                         "\x00\x00\x00""g" "\x00\x00\x00\x27" \ */
/*                                         "\x00\x00\x03\xC3" "\x00\x00\x00""g" \ */
/*                                         "\x00\x00\x00"" " \ */
/*                                         "\x00\x00\x00""g" "\x00\x00\x00""n" \ */
/*                                         "\x00\x00\x00""u"; */
/*       const pdf_size_t input_size = 48; */
/*       pdf_char_t *output_data; */
/*       pdf_size_t output_size; */
/*       pdf_text_t text; */

/*       /\* Create text object with given contents *\/ */
/*       fail_if(pdf_text_new_from_unicode(input_data, */
/*                                         input_size, */
/*                                         PDF_TEXT_UTF32_BE, &text) != PDF_OK); */

/*       /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*       fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK); */

/*       if(INTERACTIVE_DEBUG) */
/*         { */
/*           pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*           fail_if(internal_hex == NULL); */
/*           printf("pdf_text_filter:38:2:Internal> '%s'\n", internal_hex); */
/*           pdf_dealloc(internal_hex); */
/*         } */

/*       /\* Get output as UTF-32BE *\/ */
/*       fail_unless(pdf_text_get_unicode(&output_data, */
/*                                        &output_size, */
/*                                        text, */
/*                                        PDF_TEXT_UTF32_BE, */
/*                                        PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*       /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*       fail_unless(output_size == input_size); */
/*       fail_unless(memcmp(output_data, expected_data, input_size) == 0); */

/*       pdf_dealloc(output_data); */
/*       pdf_text_destroy(text); */
/*     } */

/* } */
/* END_TEST */



/* /\* */
/*  * Test: pdf_text_filter_039 */
/*  * Description: */
/*  *   Apply the `Lower case' filter to a text object that contains */
/*  *   special-case-conversion characters with After_Soft_Dotted context */
/*  *   condition, where the condition is not fulfilled. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_039) */
/* { */



/*   /\* 0x030C has combining class 230, so condition should not be fulfilled *\/ */
/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00" "i" \ */
/*                                                 "\x00\x00\x03\x0C" \ */
/*                                                 "\x00\x00\x03\x07"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                    "\x00\x00\x00" "i" \ */
/*                                                    "\x00\x00\x03\x0C" \ */
/*                                                    "\x00\x00\x03\x07"; */
/*   pdf_size_t input_size = 16; */
/*   pdf_size_t expected_size = 16; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'lt' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_039:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_039:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_040 */
/*  * Description: */
/*  *   Apply the `Lower case' filter to a text object that contains */
/*  *   special-case-conversion characters with More_Above context condition, */
/*  *   where the condition is not fulfilled. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_040) */
/* { */



/*   /\* Code:0049; Lower:0069 0307; Title:0049; Upper:0049; */
/*    * lt More_Above; # LATIN CAPITAL LETTER I */
/*    * As 0xC0 has combining class 0, the condition must not be fullfilled *\/ */

/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00\x49" \ */
/*                                                 "\x00\x00\x00\xC0" \ */
/*                                                 "\x00\x00\x03\x0B"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                    "\x00\x00\x00\x69" \ */
/*                                                    "\x00\x00\x00\xE0" \ */
/*                                                    "\x00\x00\x03\x0B"; */
/*   pdf_size_t input_size = 16; */
/*   pdf_size_t expected_size = 16; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'lt' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_040:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_040:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_041 */
/*  * Description: */
/*  *   Apply the `Lower case' filter to a text object that contains */
/*  *   special-case-conversion characters with Before_Dot context condition, */
/*  *   where the condition is not fulfilled. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_041) */
/* { */
/*   /\* NOTE: Actually there is no special condition involving "Before_Dot" as is */
/*    *       (there are a couple of conditions involving the negated one: */
/*    *       Not_Before_Dot). But probably is a good idea to at least leave */
/*    *       the test, just in case in future updates of the Unicode standard */
/*    *       one of this type appears *\/ */
/*   fail_if(0); */
/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_042 */
/*  * Description: */
/*  *   Apply the `Lower case' filter to a text object that contains */
/*  *   special-case-conversion characters with After_I context condition, */
/*  *   where the condition is not fulfilled. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_042) */
/* { */



/*   /\* Code: 0307;Lower: ; Title: 0307; Upper: 0307; */
/*    * tr After_I; # COMBINING DOT ABOVE *\/ */
/*   /\* 0x030C has combining class 230, so condition should not be fulfilled *\/ */

/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00""I" \ */
/*                                                 "\x00\x00\x03\x0C" \ */
/*                                                 "\x00\x00\x03\x07"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                    "\x00\x00\x01\x31" \ */
/*                                                    "\x00\x00\x03\x0C" \ */
/*                                                    "\x00\x00\x03\x07"; */
/*   pdf_size_t input_size = 16; */
/*   pdf_size_t expected_size = 16; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'tr' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_042:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_042:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_043 */
/*  * Description: */
/*  *   Apply the `Lower case' filter to a text object that contains */
/*  *   special-case-conversion characters with language-dependent context */
/*  *   condition, where the condition is not fulfilled. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_043) */
/* { */



/*   /\* 00CC; 0069 0307 0300; 00CC; 00CC; lt; # LATIN CAPITAL LETTER I WITH GRAVE */
/*    * If condition not fulfilled, simple lowercase gives: 00EC *\/ */

/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00\xCC"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                    "\x00\x00\x00\xEC"; */
/*   pdf_size_t input_size = 8; */
/*   pdf_size_t expected_size = 8; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'en' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"en") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_043:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_043:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */


/* /\* */
/*  * Test: pdf_text_filter_044 */
/*  * Description: */
/*  *   Apply the `Lower case' filter to a text object that contains */
/*  *   special-case-conversion characters with language-dependent and After_I */
/*  *    context condition (to check two context conditions at the same time), */
/*  *    where the condition is not fulfilled. */
/*  * Note: As long as After_I is always coming with a language condition (at least */
/*  *   in the Unicode standard 5.0), this test is equal to pdf_text_filter_042 */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_044) */
/* { */



/*   /\* Code: 0307;Lower: ; Title: 0307; Upper: 0307; */
/*    * tr After_I; # COMBINING DOT ABOVE *\/ */
/*   /\* 0x030C has combining class 230, so condition should not be fulfilled *\/ */

/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00""I" \ */
/*                                                 "\x00\x00\x03\x0C" \ */
/*                                                 "\x00\x00\x03\x07"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                    "\x00\x00\x01\x31" \ */
/*                                                    "\x00\x00\x03\x0C" \ */
/*                                                    "\x00\x00\x03\x07"; */
/*   pdf_size_t input_size = 16; */
/*   pdf_size_t expected_size = 16; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'tr' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_044:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_044:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */



/* /\* */
/*  * Test: pdf_text_filter_045 */
/*  * Description: */
/*  *   Apply the `Lower case' filter to a text object that contains */
/*  *   special-case-conversion characters with language-dependent and */
/*  *   Not_Before_Dot context condition (to check two context conditions at the */
/*  *   same time, where one of them is negated with Not_), where the condition is */
/*  *   not fulfilled. */
/*  * Success conditions: */
/*  *   1. The call to  pdf_text_filter should return PDF_OK. */
/*  *   2. The contents of the output text object must be the expected ones. */
/*  *\/ */
/* START_TEST (pdf_text_filter_045) */
/* { */



/*   /\* # When lowercasing, unless an I is before a dot_above, it turns into a */
/*    *   dotless i. */
/*    * 0049; 0131; 0049; 0049; tr Not_Before_Dot; # LATIN CAPITAL LETTER I */
/*    * Not Not_Before_Dot... is equal to Before_Dot :-) *\/ */

/*   pdf_text_t text; */
/*   const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                 "\x00\x00\x00\x49" \ */
/*                                                 "\x00\x00\x03\x07" \ */
/*                                                 "\x00\x00\x00\x50"; */

/*   const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \ */
/*                                                    "\x00\x00\x00\x69" \ */
/*                                                    "\x00\x00\x00\x70"; */
/*   pdf_size_t input_size = 16; */
/*   pdf_size_t expected_size = 12; */
/*   pdf_char_t *output_data; */
/*   pdf_size_t output_size; */




/*   /\* Create text object with given contents *\/ */
/*   fail_if(pdf_text_new_from_unicode(input_data, */
/*                                     input_size, */
/*                                     PDF_TEXT_UTF32_BE, &text) != PDF_OK); */
/*   /\* This test needs another context condition: 'tr' language code *\/ */
/*   fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK); */


/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_045:1:BeforeInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* 1. The call to  pdf_text_filter should return PDF_OK. *\/ */
/*   fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_LOWER_CASE) == PDF_OK); */

/*   if(INTERACTIVE_DEBUG) */
/*     { */
/*       pdf_char_t *internal_hex = NULL; */
/*       pdf_size_t actual_size; */
/*       pdf_char_t *actual_data; */
/*       fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text, */
/*                                        PDF_TEXT_UTF32_HE,0) == PDF_OK); */
/*       internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':'); */
/*       fail_if(internal_hex == NULL); */
/*       printf("pdf_text_filter_045:1:AfterInternal> '%s' (%ld)\n", */
/*              internal_hex, (long)strlen(internal_hex)); */
/*       pdf_dealloc(internal_hex); */
/*     } */

/*   /\* Get output as UTF-32BE *\/ */
/*   fail_unless(pdf_text_get_unicode(&output_data, */
/*                                    &output_size, */
/*                                    text, */
/*                                    PDF_TEXT_UTF32_BE, */
/*                                    PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK); */

/*   /\* 2. The contents of the output text object must be the expected ones. *\/ */
/*   fail_unless(output_size == expected_size); */
/*   fail_unless(memcmp(output_data, expected_data, expected_size) == 0); */

/*   pdf_dealloc(output_data); */
/*   pdf_text_destroy(text); */

/* } */
/* END_TEST */

#if 0

/*
 * Test: pdf_text_filter_046
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   simple-case-conversion characters.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 *   3. The length of the output text must be equal to the length of the
 *      input text.
 */
START_TEST (pdf_text_filter_046)
{



  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00"" " \
                                                "\x00\x00\x00""G" \
                                                "\x00\x00\x00""N" \
                                                "\x00\x00\x00""U" \
                                                "\x00\x00\x00"" ";
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00"" " \
                                                   "\x00\x00\x00""G" \
                                                   "\x00\x00\x00""n" \
                                                   "\x00\x00\x00""u" \
                                                   "\x00\x00\x00"" ";
  pdf_size_t input_size = 20;
  pdf_size_t expected_size = input_size;
  pdf_char_t *output_data;
  pdf_size_t output_size;




  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE, &text) != PDF_OK);
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_046:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);

  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_046:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);

  /* 3. The length of the output text must be equal to the length of the
   *      input text. */
  fail_unless(output_size == expected_size);

  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);

  pdf_dealloc(output_data);
  pdf_text_destroy(text);

}
END_TEST


/*
 * Test: pdf_text_filter_047
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with no context condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST (pdf_text_filter_047)
{



  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""A" \
                                                "\x00\x00\x01\x30";
  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""A" \
                                                "\x00\x00\x00\x69\x00\x00\x03\x07";
  pdf_size_t input_size = 12;
  pdf_size_t expected_size = 16;
  pdf_char_t *output_data;
  pdf_size_t output_size;




  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE, &text) != PDF_OK);
  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_047:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);

  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_047:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);

  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);

  pdf_dealloc(output_data);
  pdf_text_destroy(text);

}
END_TEST



/*
 * Test: pdf_text_filter_048
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with Final_Sigma context condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST (pdf_text_filter_048)
{



  /* Final_Sigma condition in Special Case algorithms
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





  /* 1st check, the sequence of previous case-ignorable points is empty */
  if(1)
    {
      const pdf_char_t *input_data = (pdf_char_t *) \
                                     "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                     "\x00\x00\x00""U" "\x00\x00\x00"" " \
                                     "\x00\x00\x00""G" \
                                     "\x00\x00\x03\xA3" "\x00\x00\x00"" " \
                                     "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                     "\x00\x00\x00""U";
      const pdf_char_t *expected_data = (pdf_char_t *) \
                                     "\x00\x00\x00""G" "\x00\x00\x00""n" \
                                     "\x00\x00\x00""u" "\x00\x00\x00"" " \
                                     "\x00\x00\x00""G" \
                                     "\x00\x00\x03\xC2" "\x00\x00\x00"" " \
                                     "\x00\x00\x00""G" "\x00\x00\x00""n" \
                                     "\x00\x00\x00""u";
      const pdf_size_t input_size = 40;
      pdf_char_t *output_data;
      pdf_size_t output_size;
      pdf_text_t text;

      /* Create text object with given contents */
      fail_if(pdf_text_new_from_unicode(input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_BE, &text) != PDF_OK);

      /* 1. The call to  pdf_text_filter should return PDF_OK. */
      fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);

      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
          fail_if(internal_hex == NULL);
          printf("pdf_text_filter:48:1:Internal> '%s'\n", internal_hex);
          pdf_dealloc(internal_hex);
        }

      /* Get output as UTF-32BE */
      fail_unless(pdf_text_get_unicode(&output_data,
                                       &output_size,
                                       text,
                                       PDF_TEXT_UTF32_BE,
                                       PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);

      /* 2. The contents of the output text object must be the expected ones. */
      fail_unless(output_size == input_size);
      fail_unless(memcmp(output_data, expected_data, input_size) == 0);

      pdf_dealloc(output_data);
      pdf_text_destroy(text);
    }

  /* 2nd check, the sequence of previous case-ignorable points is NOT empty */
  if(1)
    {
      const pdf_char_t *input_data = (pdf_char_t *) \
                                        "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                        "\x00\x00\x00""U" "\x00\x00\x00"" " \
                                        "\x00\x00\x00""G" "\x00\x00\x00\x3A" \
                                        "\x00\x00\x03\xA3" "\x00\x00\x00"" " \
                                        "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                        "\x00\x00\x00""U";
      const pdf_char_t *expected_data = (pdf_char_t *) \
                                        "\x00\x00\x00""G" "\x00\x00\x00""n" \
                                        "\x00\x00\x00""u" "\x00\x00\x00"" " \
                                        "\x00\x00\x00""G" "\x00\x00\x00\x3A" \
                                        "\x00\x00\x03\xC2" "\x00\x00\x00"" " \
                                        "\x00\x00\x00""G" "\x00\x00\x00""n" \
                                        "\x00\x00\x00""u";

      const pdf_size_t input_size = 44;
      pdf_char_t *output_data;
      pdf_size_t output_size;
      pdf_text_t text;

      /* Create text object with given contents */
      fail_if(pdf_text_new_from_unicode(input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_BE, &text) != PDF_OK);

      /* 1. The call to  pdf_text_filter should return PDF_OK. */
      fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);

      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
          fail_if(internal_hex == NULL);
          printf("pdf_text_filter:48:2:Internal> '%s'\n", internal_hex);
          pdf_dealloc(internal_hex);
        }

      /* Get output as UTF-32BE */
      fail_unless(pdf_text_get_unicode(&output_data,
                                       &output_size,
                                       text,
                                       PDF_TEXT_UTF32_BE,
                                       PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);

      /* 2. The contents of the output text object must be the expected ones. */
      fail_unless(output_size == input_size);
      fail_unless(memcmp(output_data, expected_data, input_size) == 0);

      pdf_dealloc(output_data);
      pdf_text_destroy(text);
    }

}
END_TEST


/*
 * Test: pdf_text_filter_049
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with After_Soft_Dotted context
 *   condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST (pdf_text_filter_049)
{



  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00" "i" \
                                                "\x00\x00\x03\x07";

  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00" "I" \
                                                   "\x00\x00\x03\x07";
  pdf_size_t input_size = 12;
  pdf_size_t expected_size = 12;
  pdf_char_t *output_data;
  pdf_size_t output_size;




  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE, &text) != PDF_OK);
  /* This test needs another context condition: 'lt' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK);


  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_049:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);

  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_049:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);

  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);

  pdf_dealloc(output_data);
  pdf_text_destroy(text);

}
END_TEST

/*
 * Test: pdf_text_filter_050
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with More_Above context condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST (pdf_text_filter_050)
{



  /* Code:0049; Lower:0069 0307; Title:0049; Upper:0049;
   * lt More_Above; # LATIN CAPITAL LETTER I */

  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""a" \
                                                "\x00\x00\x00\x49"
                                                "\x00\x00\x03\x0B";

  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""A" \
                                                "\x00\x00\x00\x69\x00\x00\x03\x07"
                                                "\x00\x00\x03\x0B";
  pdf_size_t input_size = 16;
  pdf_size_t expected_size = 20;
  pdf_char_t *output_data;
  pdf_size_t output_size;




  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE, &text) != PDF_OK);
  /* This test needs another context condition: 'lt' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK);


  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_050:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);

  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_050:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);

  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);

  pdf_dealloc(output_data);
  pdf_text_destroy(text);

}
END_TEST


/*
 * Test: pdf_text_filter_051
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with Before_Dot context condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST (pdf_text_filter_051)
{
  /* NOTE: Actually there is no special condition involving "Before_Dot" as is
   *       (there are a couple of conditions involving the negated one:
   *       Not_Before_Dot). But probably is a good idea to at least leave
   *       the test, just in case in future updates of the Unicode standard
   *       one of this type appears */
  fail_if(0);
}
END_TEST


/*
 * Test: pdf_text_filter_052
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with After_I context condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST (pdf_text_filter_052)
{



  /* Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE */

  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""a" \
                                                "\x00\x00\x00""I" \
                                                "\x00\x00\x03\x07";

  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""A" \
                                                   "\x00\x00\x00""i";
  pdf_size_t input_size = 16;
  pdf_size_t expected_size = 12;
  pdf_char_t *output_data;
  pdf_size_t output_size;




  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE, &text) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);


  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_052:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);

  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_052:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);

  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);

  pdf_dealloc(output_data);
  pdf_text_destroy(text);

}
END_TEST


/*
 * Test: pdf_text_filter_053
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent context
 *   condition.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST (pdf_text_filter_053)
{



  /* 00CC; 0069 0307 0300; 00CC; 00CC; lt; # LATIN CAPITAL LETTER I WITH GRAVE*/

  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""a" \
                                                "\x00\x00\x00\xCC";

  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""A" \
                                  "\x00\x00\x00\x69\x00\x00\x03\x07\x00\x00\x03\x00";
  pdf_size_t input_size = 12;
  pdf_size_t expected_size = 20;
  pdf_char_t *output_data;
  pdf_size_t output_size;




  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE, &text) != PDF_OK);
  /* This test needs another context condition: 'lt' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK);


  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_053:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);

  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_053:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);

  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);

  pdf_dealloc(output_data);
  pdf_text_destroy(text);

}
END_TEST


/*
 * Test: pdf_text_filter_054
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent and After_I
 *    context condition (to check two context conditions at the same time).
 * Note: As long as After_I is always coming with a language condition (at least
 *   in the Unicode standard 5.0), this test is equal to pdf_text_filter_034
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST (pdf_text_filter_054)
{



  /* Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE */

  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""a" \
                                                "\x00\x00\x00""I" \
                                                "\x00\x00\x03\x07";

  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""A" \
                                                   "\x00\x00\x00""i";
  pdf_size_t input_size = 16;
  pdf_size_t expected_size = 12;
  pdf_char_t *output_data;
  pdf_size_t output_size;




  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE, &text) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);


  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_054:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);

  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_054:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);

  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);

  pdf_dealloc(output_data);
  pdf_text_destroy(text);

}
END_TEST



/*
 * Test: pdf_text_filter_055
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent and
 *   Not_Before_Dot context condition (to check two context conditions at the
 *   same time, where one of them is negated with Not_).
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST (pdf_text_filter_055)
{



  /* # When lowercasing, unless an I is before a dot_above, it turns into a
   *   dotless i.
   * 0049; 0131; 0049; 0049; tr Not_Before_Dot; # LATIN CAPITAL LETTER I */

  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""a" \
                                                "\x00\x00\x00\x49" \
                                                "\x00\x00\x00\x50";

  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""A" \
                                                   "\x00\x00\x01\x31" \
                                                   "\x00\x00\x00\x70";
  pdf_size_t input_size = 16;
  pdf_size_t expected_size = 16;
  pdf_char_t *output_data;
  pdf_size_t output_size;




  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE, &text) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);


  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_055:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);

  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_055:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);

  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);

  pdf_dealloc(output_data);
  pdf_text_destroy(text);

}
END_TEST


/*
 * Test: pdf_text_filter_056
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with Final_Sigma context condition,
 *   where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST (pdf_text_filter_056)
{



  /* Final_Sigma condition in Special Case algorithms
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





  /* 1st check, the previous cased point is not available (so word starts in
   *  the sigma --> uppercase sigma */
  if(1)
    {
      const pdf_char_t *input_data = (pdf_char_t *) \
                                     "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                     "\x00\x00\x00""U" "\x00\x00\x00"" " \
                                     "\x00\x00\x00\x3A"
                                     "\x00\x00\x03\xA3" "\x00\x00\x00"" " \
                                     "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                     "\x00\x00\x00""U";
      const pdf_char_t *expected_data = (pdf_char_t *) \
                                     "\x00\x00\x00""G" "\x00\x00\x00""n" \
                                     "\x00\x00\x00""u" "\x00\x00\x00"" " \
                                     "\x00\x00\x00\x3A"
                                     "\x00\x00\x03\xA3" "\x00\x00\x00"" " \
                                     "\x00\x00\x00""G" "\x00\x00\x00""n" \
                                     "\x00\x00\x00""u";
      const pdf_size_t input_size = 40;
      pdf_char_t *output_data;
      pdf_size_t output_size;
      pdf_text_t text;

      /* Create text object with given contents */
      fail_if(pdf_text_new_from_unicode(input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_BE, &text) != PDF_OK);

      /* 1. The call to  pdf_text_filter should return PDF_OK. */
      fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);

      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
          fail_if(internal_hex == NULL);
          printf("pdf_text_filter:56:1:Internal> '%s'\n", internal_hex);
          pdf_dealloc(internal_hex);
        }

      /* Get output as UTF-32BE */
      fail_unless(pdf_text_get_unicode(&output_data,
                                       &output_size,
                                       text,
                                       PDF_TEXT_UTF32_BE,
                                       PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);

      /* 2. The contents of the output text object must be the expected ones. */
      fail_unless(output_size == input_size);
      fail_unless(memcmp(output_data, expected_data, input_size) == 0);

      pdf_dealloc(output_data);
      pdf_text_destroy(text);
    }

  /* 2nd check, the sequence of previous case-ignorable points is NOT empty, but
   *  the previous cased point is available --> non-final lowercase sigma is
   *  used */
  if(1)
    {
      const pdf_char_t *input_data = (pdf_char_t *) \
                                     "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                     "\x00\x00\x00""U" "\x00\x00\x00"" " \
                                     "\x00\x00\x00""G" "\x00\x00\x00\x3A" \
                                     "\x00\x00\x03\xA3" "\x00\x00\x00""G" \
                                     "\x00\x00\x00"" " \
                                     "\x00\x00\x00""G" "\x00\x00\x00""N" \
                                     "\x00\x00\x00""U";
      const pdf_char_t *expected_data = (pdf_char_t *) \
                                        "\x00\x00\x00""G" "\x00\x00\x00""n" \
                                        "\x00\x00\x00""u" "\x00\x00\x00"" " \
                                        "\x00\x00\x00""G" "\x00\x00\x00\x3A" \
                                        "\x00\x00\x03\xC3" "\x00\x00\x00""g" \
                                        "\x00\x00\x00"" " \
                                        "\x00\x00\x00""G" "\x00\x00\x00""n" \
                                        "\x00\x00\x00""u";
      const pdf_size_t input_size = 48;
      pdf_char_t *output_data;
      pdf_size_t output_size;
      pdf_text_t text;

      /* Create text object with given contents */
      fail_if(pdf_text_new_from_unicode(input_data,
                                        input_size,
                                        PDF_TEXT_UTF32_BE, &text) != PDF_OK);

      /* 1. The call to  pdf_text_filter should return PDF_OK. */
      fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);

      if(INTERACTIVE_DEBUG)
        {
          pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
          fail_if(internal_hex == NULL);
          printf("pdf_text_filter:56:2:Internal> '%s'\n", internal_hex);
          pdf_dealloc(internal_hex);
        }

      /* Get output as UTF-32BE */
      fail_unless(pdf_text_get_unicode(&output_data,
                                       &output_size,
                                       text,
                                       PDF_TEXT_UTF32_BE,
                                       PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);

      /* 2. The contents of the output text object must be the expected ones. */
      fail_unless(output_size == input_size);
      fail_unless(memcmp(output_data, expected_data, input_size) == 0);

      pdf_dealloc(output_data);
      pdf_text_destroy(text);
    }

}
END_TEST


/*
 * Test: pdf_text_filter_057
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with After_Soft_Dotted context
 *   condition, where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST (pdf_text_filter_057)
{



  /* 0x030C has combining class 230, so condition should not be fulfilled */
  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00" "a" \
                                                "\x00\x00\x00" "i" \
                                                "\x00\x00\x03\x0C" \
                                                "\x00\x00\x03\x07";

  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00" "A" \
                                                   "\x00\x00\x00" "i" \
                                                   "\x00\x00\x03\x0C" \
                                                   "\x00\x00\x03\x07";
  pdf_size_t input_size = 20;
  pdf_size_t expected_size = 20;
  pdf_char_t *output_data;
  pdf_size_t output_size;




  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE, &text) != PDF_OK);
  /* This test needs another context condition: 'lt' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK);


  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_057:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);

  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_057:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);

  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);

  pdf_dealloc(output_data);
  pdf_text_destroy(text);

}
END_TEST


/*
 * Test: pdf_text_filter_058
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with More_Above context condition,
 *   where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST (pdf_text_filter_058)
{



  /* Code:0049; Lower:0069 0307; Title:0049; Upper:0049;
   * lt More_Above; # LATIN CAPITAL LETTER I
   * As 0xC0 has combining class 0, the condition must not be fullfilled */

  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""a" \
                                                "\x00\x00\x00\x49" \
                                                "\x00\x00\x00\xC0" \
                                                "\x00\x00\x03\x0B";

  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""A" \
                                                   "\x00\x00\x00\x69" \
                                                   "\x00\x00\x00\xE0" \
                                                   "\x00\x00\x03\x0B";
  pdf_size_t input_size = 20;
  pdf_size_t expected_size = 20;
  pdf_char_t *output_data;
  pdf_size_t output_size;




  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE, &text) != PDF_OK);
  /* This test needs another context condition: 'lt' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"lt") != PDF_OK);


  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_058:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);

  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_058:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);

  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);

  pdf_dealloc(output_data);
  pdf_text_destroy(text);

}
END_TEST


/*
 * Test: pdf_text_filter_059
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with Before_Dot context condition,
 *   where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST (pdf_text_filter_059)
{
  /* NOTE: Actually there is no special condition involving "Before_Dot" as is
   *       (there are a couple of conditions involving the negated one:
   *       Not_Before_Dot). But probably is a good idea to at least leave
   *       the test, just in case in future updates of the Unicode standard
   *       one of this type appears */
  fail_if(0);
}
END_TEST


/*
 * Test: pdf_text_filter_060
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with After_I context condition,
 *   where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST (pdf_text_filter_060)
{



  /* Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE */
  /* 0x030C has combining class 230, so condition should not be fulfilled */

  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""a" \
                                                "\x00\x00\x00""I" \
                                                "\x00\x00\x03\x0C" \
                                                "\x00\x00\x03\x07";

  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""A" \
                                                   "\x00\x00\x01\x31" \
                                                   "\x00\x00\x03\x0C" \
                                                   "\x00\x00\x03\x07";
  pdf_size_t input_size = 20;
  pdf_size_t expected_size = 20;
  pdf_char_t *output_data;
  pdf_size_t output_size;




  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE, &text) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);


  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_060:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);

  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_060:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);

  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);

  pdf_dealloc(output_data);
  pdf_text_destroy(text);


}
END_TEST


/*
 * Test: pdf_text_filter_061
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent context
 *   condition, where the condition is not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST (pdf_text_filter_061)
{



  /* 00CC; 0069 0307 0300; 00CC; 00CC; lt; # LATIN CAPITAL LETTER I WITH GRAVE
   * If condition not fulfilled, simple lowercase gives: 00EC */

  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""a" \
                                                "\x00\x00\x00\xCC";

  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""A" \
                                                   "\x00\x00\x00\xEC";
  pdf_size_t input_size = 12;
  pdf_size_t expected_size = 12;
  pdf_char_t *output_data;
  pdf_size_t output_size;




  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE, &text) != PDF_OK);
  /* This test needs another context condition: 'en' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"en") != PDF_OK);


  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_061:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);

  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_061:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);

  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);

  pdf_dealloc(output_data);
  pdf_text_destroy(text);

}
END_TEST


/*
 * Test: pdf_text_filter_062
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent and After_I
 *    context condition (to check two context conditions at the same time),
 *    where the condition is not fulfilled.
 * Note: As long as After_I is always coming with a language condition (at least
 *   in the Unicode standard 5.0), this test is equal to pdf_text_filter_060
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST (pdf_text_filter_062)
{



  /* Code: 0307;Lower: ; Title: 0307; Upper: 0307;
   * tr After_I; # COMBINING DOT ABOVE */
  /* 0x030C has combining class 230, so condition should not be fulfilled */

  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""a" \
                                                "\x00\x00\x00""I" \
                                                "\x00\x00\x03\x0C" \
                                                "\x00\x00\x03\x07";

  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""A" \
                                                   "\x00\x00\x01\x31" \
                                                   "\x00\x00\x03\x0C" \
                                                   "\x00\x00\x03\x07";
  pdf_size_t input_size = 20;
  pdf_size_t expected_size = 20;
  pdf_char_t *output_data;
  pdf_size_t output_size;




  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE, &text) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);


  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_062:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);

  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_062:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);

  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);

  pdf_dealloc(output_data);
  pdf_text_destroy(text);

}
END_TEST



/*
 * Test: pdf_text_filter_063
 * Description:
 *   Apply the `Title case' filter to a text object that contains
 *   special-case-conversion characters with language-dependent and
 *   Not_Before_Dot context condition (to check two context conditions at the
 *   same time, where one of them is negated with Not_), where the condition is
 *   not fulfilled.
 * Success conditions:
 *   1. The call to  pdf_text_filter should return PDF_OK.
 *   2. The contents of the output text object must be the expected ones.
 */
START_TEST (pdf_text_filter_063)
{



  /* # When lowercasing, unless an I is before a dot_above, it turns into a
   *   dotless i.
   * 0049; 0131; 0049; 0049; tr Not_Before_Dot; # LATIN CAPITAL LETTER I
   * Not Not_Before_Dot... is equal to Before_Dot :-) */

  pdf_text_t text;
  const pdf_char_t *input_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                "\x00\x00\x00""a" \
                                                "\x00\x00\x00\x49" \
                                                "\x00\x00\x03\x07";

  const pdf_char_t *expected_data = (pdf_char_t *) "\x00\x00\x00\x20" \
                                                   "\x00\x00\x00""A" \
                                                   "\x00\x00\x00\x69";
  pdf_size_t input_size = 16;
  pdf_size_t expected_size = 12;
  pdf_char_t *output_data;
  pdf_size_t output_size;




  /* Create text object with given contents */
  fail_if(pdf_text_new_from_unicode(input_data,
                                    input_size,
                                    PDF_TEXT_UTF32_BE, &text) != PDF_OK);
  /* This test needs another context condition: 'tr' language code */
  fail_if(pdf_text_set_language(text,(pdf_char_t *)"tr") != PDF_OK);


  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_063:1:BeforeInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* 1. The call to  pdf_text_filter should return PDF_OK. */
  fail_unless(pdf_text_filter(text, PDF_TEXT_FILTER_TITLE_CASE) == PDF_OK);

  if(INTERACTIVE_DEBUG)
    {
      pdf_char_t *internal_hex = NULL;
      pdf_size_t actual_size;
      pdf_char_t *actual_data;
      fail_unless(pdf_text_get_unicode(&actual_data, &actual_size, text,
                                       PDF_TEXT_UTF32_HE,0) == PDF_OK);
      internal_hex = pdf_text_test_get_hex(actual_data,actual_size,':');
      fail_if(internal_hex == NULL);
      printf("pdf_text_filter_063:1:AfterInternal> '%s' (%ld)\n",
             internal_hex, (long)strlen(internal_hex));
      pdf_dealloc(internal_hex);
    }

  /* Get output as UTF-32BE */
  fail_unless(pdf_text_get_unicode(&output_data,
                                   &output_size,
                                   text,
                                   PDF_TEXT_UTF32_BE,
                                   PDF_TEXT_UNICODE_NO_OPTION) == PDF_OK);

  /* 2. The contents of the output text object must be the expected ones. */
  fail_unless(output_size == expected_size);
  fail_unless(memcmp(output_data, expected_data, expected_size) == 0);

  pdf_dealloc(output_data);
  pdf_text_destroy(text);

}
END_TEST

#endif


/*
 * Test case creation function
 */
TCase *
test_pdf_text_filter (void)
{
  TCase *tc = tcase_create("pdf_text_filter");

  tcase_add_test(tc, pdf_text_filter_001); /* Normalize line endings */
  tcase_add_test(tc, pdf_text_filter_002); /* Normalize line endings */

  tcase_add_test(tc, pdf_text_filter_003); /* Remove line endings */
  tcase_add_test(tc, pdf_text_filter_004); /* Remove line endings */

  tcase_add_test(tc, pdf_text_filter_005); /* Remove ampersands */
  tcase_add_test(tc, pdf_text_filter_006); /* Remove ampersands */

  tcase_add_test(tc, pdf_text_filter_007); /* Normalize with full width */
  tcase_add_test(tc, pdf_text_filter_008); /* Normalize with full width */
  tcase_add_test(tc, pdf_text_filter_009); /* Normalize with full width */

  /* Upper case */
  tcase_add_test(tc, pdf_text_filter_010); /* Upper */
  tcase_add_test(tc, pdf_text_filter_011); /* Upper-Special */
  tcase_add_test(tc, pdf_text_filter_012); /* Upper-Special Final_Sigma (1) */
  tcase_add_test(tc, pdf_text_filter_013); /* Upper-Special Final_Sigma (2) */
  tcase_add_test(tc, pdf_text_filter_014); /* Upper-Special After_Soft_Dotted */
  tcase_add_test(tc, pdf_text_filter_015); /* Upper-Special More_Above */
  tcase_add_test(tc, pdf_text_filter_016); /* Upper-Special Before_Dot  */
  tcase_add_test(tc, pdf_text_filter_017); /* Upper-Special After_I  */
  tcase_add_test(tc, pdf_text_filter_018); /* Upper-Special 'tr'  */
  tcase_add_test(tc, pdf_text_filter_019); /* Upper-Special After_I and 'tr'  */
  tcase_add_test(tc, pdf_text_filter_020); /* Upper-Special Not_Before_Dot  */
  tcase_add_test(tc, pdf_text_filter_021); /* Upper-Special Not Final_Sigma (1) */
  tcase_add_test(tc, pdf_text_filter_022); /* Upper-Special Not Final_Sigma (2) */
  tcase_add_test(tc, pdf_text_filter_023); /* Upper-Special Not After_Soft_Dotted */
  tcase_add_test(tc, pdf_text_filter_024); /* Upper-Special Not More_Above */
  tcase_add_test(tc, pdf_text_filter_025); /* Upper-Special Not Before_Dot  */
  tcase_add_test(tc, pdf_text_filter_026); /* Upper-Special Not After_I  */
  tcase_add_test(tc, pdf_text_filter_027); /* Upper-Special Not 'tr'  */
  tcase_add_test(tc, pdf_text_filter_028); /* Upper-Special Not After_I and 'tr'  */
  tcase_add_test(tc, pdf_text_filter_029); /* Upper-Special Not Not_Before_Dot  */

  /* Lower case */
  tcase_add_test(tc, pdf_text_filter_030); /* Lower */
  tcase_add_test(tc, pdf_text_filter_031); /* Lower-Special */
  tcase_add_test(tc, pdf_text_filter_032); /* Lower-Special Final_Sigma (1) */
  tcase_add_test(tc, pdf_text_filter_033); /* Lower-Special Final_Sigma (2) */
  tcase_add_test(tc, pdf_text_filter_034); /* Lower-Special After_Soft_Dotted */
  tcase_add_test(tc, pdf_text_filter_035); /* Lower-Special More_Above */
  tcase_add_test(tc, pdf_text_filter_036); /* Lower-Special Before_Dot  */
  tcase_add_test(tc, pdf_text_filter_037); /* Lower-Special After_I  */
  tcase_add_test(tc, pdf_text_filter_038); /* Lower-Special 'tr'  */
  tcase_add_test(tc, pdf_text_filter_039); /* Lower-Special After_I and 'tr'  */
  tcase_add_test(tc, pdf_text_filter_040); /* Lower-Special Not_Before_Dot  */
  tcase_add_test(tc, pdf_text_filter_041); /* Lower-Special Not Final_Sigma (1) */
  tcase_add_test(tc, pdf_text_filter_042); /* Lower-Special Not Final_Sigma (2) */
  tcase_add_test(tc, pdf_text_filter_043); /* Lower-Special Not After_Soft_Dotted */
  tcase_add_test(tc, pdf_text_filter_044); /* Lower-Special Not More_Above */
  tcase_add_test(tc, pdf_text_filter_045); /* Lower-Special Not Before_Dot  */
  tcase_add_test(tc, pdf_text_filter_046); /* Lower-Special Not After_I  */
  tcase_add_test(tc, pdf_text_filter_047); /* Lower-Special Not 'tr'  */
  tcase_add_test(tc, pdf_text_filter_048); /* Lower-Special Not After_I and 'tr'  */
  tcase_add_test(tc, pdf_text_filter_049); /* Lower-Special Not Not_Before_Dot  */



#if 0

  /* Title case */
  tcase_add_test(tc, pdf_text_filter_046); /* Title */
  tcase_add_test(tc, pdf_text_filter_047); /* Title-Special */
  tcase_add_test(tc, pdf_text_filter_048); /* Title-Special, Final_Sigma */
  tcase_add_test(tc, pdf_text_filter_049); /* Title-Special After_Soft_Dotted */
  tcase_add_test(tc, pdf_text_filter_050); /* Title-Special More_Above */
  tcase_add_test(tc, pdf_text_filter_051); /* Title-Special Before_Dot  */
  tcase_add_test(tc, pdf_text_filter_052); /* Title-Special After_I  */
  tcase_add_test(tc, pdf_text_filter_053); /* Title-Special 'tr'  */
  tcase_add_test(tc, pdf_text_filter_054); /* Title-Special After_I and 'tr'  */
  tcase_add_test(tc, pdf_text_filter_055); /* Title-Special Not_Before_Dot  */
  tcase_add_test(tc, pdf_text_filter_056); /* Title-Special, Not Final_Sigma */
  tcase_add_test(tc, pdf_text_filter_057); /* Title-Special Not After_Soft_Dotted */
  tcase_add_test(tc, pdf_text_filter_058); /* Title-Special Not More_Above */
  tcase_add_test(tc, pdf_text_filter_059); /* Title-Special Not Before_Dot  */
  tcase_add_test(tc, pdf_text_filter_060); /* Title-Special Not After_I  */
  tcase_add_test(tc, pdf_text_filter_061); /* Title-Special Not 'tr'  */
  tcase_add_test(tc, pdf_text_filter_062); /* Title-Special Not After_I and 'tr'  */
  tcase_add_test(tc, pdf_text_filter_063); /* Title-Special Not Not_Before_Dot  */
#endif

  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-text-filter.c */

/* -*- mode: C -*-
 *
 *       File:         pdf-text-ucd-wordbreak.c
 *       Date:         Sun Mar 09 12:59:01 2008
 *
 *       GNU PDF Library - Unicode Character Database - WordBreak Property
 *
 *    WARNING! The contents of the arrays in this file are self-generated
 *     from the WordBreakProperty.txt file, using the `pdf_text_generate_ucd'
 *     utility
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

#include <string.h>

#include <pdf-text-ucd-wordbreak.h>

typedef struct _unicode_wordbreak_info_s {
  pdf_u32_t interval_start;
  pdf_u32_t interval_stop;
  enum pdf_text_ucd_wb_property_e wb_property;
} unicode_wordbreak_info_t;

/*************** START OF SELF-GENERATED DATA *********************************/

/* Index of first 'CR' interval */
#define UCD_WB_CR_F 0
/* Index of last 'CR' interval */
#define UCD_WB_CR_L 0
/* Index of first 'LF' interval */
#define UCD_WB_LF_F 1
/* Index of last 'LF' interval */
#define UCD_WB_LF_L 1
/* Index of first 'NEWLINE' interval */
#define UCD_WB_NEWLINE_F 2
/* Index of last 'NEWLINE' interval */
#define UCD_WB_NEWLINE_L 5
/* Index of first 'EXTEND' interval */
#define UCD_WB_EXTEND_F 6
/* Index of last 'EXTEND' interval */
#define UCD_WB_EXTEND_L 257
/* Index of first 'FORMAT' interval */
#define UCD_WB_FORMAT_F 258
/* Index of last 'FORMAT' interval */
#define UCD_WB_FORMAT_L 272
/* Index of first 'KATAKANA' interval */
#define UCD_WB_KATAKANA_F 273
/* Index of last 'KATAKANA' interval */
#define UCD_WB_KATAKANA_L 284
/* Index of first 'ALETTER' interval */
#define UCD_WB_ALETTER_F 285
/* Index of last 'ALETTER' interval */
#define UCD_WB_ALETTER_L 668
/* Index of first 'MIDLETTER' interval */
#define UCD_WB_MIDLETTER_F 669
/* Index of last 'MIDLETTER' interval */
#define UCD_WB_MIDLETTER_L 676
/* Index of first 'MIDNUM' interval */
#define UCD_WB_MIDNUM_F 677
/* Index of last 'MIDNUM' interval */
#define UCD_WB_MIDNUM_L 690
/* Index of first 'MIDNUMLET' interval */
#define UCD_WB_MIDNUMLET_F 691
/* Index of last 'MIDNUMLET' interval */
#define UCD_WB_MIDNUMLET_L 698
/* Index of first 'NUMERIC' interval */
#define UCD_WB_NUMERIC_F 699
/* Index of last 'NUMERIC' interval */
#define UCD_WB_NUMERIC_L 731
/* Index of first 'EXTENDNUMLET' interval */
#define UCD_WB_EXTENDNUMLET_F 732
/* Index of last 'EXTENDNUMLET' interval */
#define UCD_WB_EXTENDNUMLET_L 737
#define UCD_WB_INFO_N   738  /* Maximum number of elements in array    */


static unicode_wordbreak_info_t unicode_wordbreak_info[UCD_WB_INFO_N] = {
  { 0x000D, 0x000D, PDF_TEXT_UCD_WBP_CR  }, /* 0 */
  { 0x000A, 0x000A, PDF_TEXT_UCD_WBP_LF  }, /* 1 */
  { 0x000B, 0x000C, PDF_TEXT_UCD_WBP_Newline  }, /* 2 */
  { 0x0085, 0x0085, PDF_TEXT_UCD_WBP_Newline  }, /* 3 */
  { 0x2028, 0x2028, PDF_TEXT_UCD_WBP_Newline  }, /* 4 */
  { 0x2029, 0x2029, PDF_TEXT_UCD_WBP_Newline  }, /* 5 */
  { 0x0300, 0x036F, PDF_TEXT_UCD_WBP_Extend  }, /* 6 */
  { 0x0483, 0x0487, PDF_TEXT_UCD_WBP_Extend  }, /* 7 */
  { 0x0488, 0x0489, PDF_TEXT_UCD_WBP_Extend  }, /* 8 */
  { 0x0591, 0x05BD, PDF_TEXT_UCD_WBP_Extend  }, /* 9 */
  { 0x05BF, 0x05BF, PDF_TEXT_UCD_WBP_Extend  }, /* 10 */
  { 0x05C1, 0x05C2, PDF_TEXT_UCD_WBP_Extend  }, /* 11 */
  { 0x05C4, 0x05C5, PDF_TEXT_UCD_WBP_Extend  }, /* 12 */
  { 0x05C7, 0x05C7, PDF_TEXT_UCD_WBP_Extend  }, /* 13 */
  { 0x0610, 0x061A, PDF_TEXT_UCD_WBP_Extend  }, /* 14 */
  { 0x064B, 0x065E, PDF_TEXT_UCD_WBP_Extend  }, /* 15 */
  { 0x0670, 0x0670, PDF_TEXT_UCD_WBP_Extend  }, /* 16 */
  { 0x06D6, 0x06DC, PDF_TEXT_UCD_WBP_Extend  }, /* 17 */
  { 0x06DE, 0x06DE, PDF_TEXT_UCD_WBP_Extend  }, /* 18 */
  { 0x06DF, 0x06E4, PDF_TEXT_UCD_WBP_Extend  }, /* 19 */
  { 0x06E7, 0x06E8, PDF_TEXT_UCD_WBP_Extend  }, /* 20 */
  { 0x06EA, 0x06ED, PDF_TEXT_UCD_WBP_Extend  }, /* 21 */
  { 0x0711, 0x0711, PDF_TEXT_UCD_WBP_Extend  }, /* 22 */
  { 0x0730, 0x074A, PDF_TEXT_UCD_WBP_Extend  }, /* 23 */
  { 0x07A6, 0x07B0, PDF_TEXT_UCD_WBP_Extend  }, /* 24 */
  { 0x07EB, 0x07F3, PDF_TEXT_UCD_WBP_Extend  }, /* 25 */
  { 0x0901, 0x0902, PDF_TEXT_UCD_WBP_Extend  }, /* 26 */
  { 0x0903, 0x0903, PDF_TEXT_UCD_WBP_Extend  }, /* 27 */
  { 0x093C, 0x093C, PDF_TEXT_UCD_WBP_Extend  }, /* 28 */
  { 0x093E, 0x0940, PDF_TEXT_UCD_WBP_Extend  }, /* 29 */
  { 0x0941, 0x0948, PDF_TEXT_UCD_WBP_Extend  }, /* 30 */
  { 0x0949, 0x094C, PDF_TEXT_UCD_WBP_Extend  }, /* 31 */
  { 0x094D, 0x094D, PDF_TEXT_UCD_WBP_Extend  }, /* 32 */
  { 0x0951, 0x0954, PDF_TEXT_UCD_WBP_Extend  }, /* 33 */
  { 0x0962, 0x0963, PDF_TEXT_UCD_WBP_Extend  }, /* 34 */
  { 0x0981, 0x0981, PDF_TEXT_UCD_WBP_Extend  }, /* 35 */
  { 0x0982, 0x0983, PDF_TEXT_UCD_WBP_Extend  }, /* 36 */
  { 0x09BC, 0x09BC, PDF_TEXT_UCD_WBP_Extend  }, /* 37 */
  { 0x09BE, 0x09C0, PDF_TEXT_UCD_WBP_Extend  }, /* 38 */
  { 0x09C1, 0x09C4, PDF_TEXT_UCD_WBP_Extend  }, /* 39 */
  { 0x09C7, 0x09C8, PDF_TEXT_UCD_WBP_Extend  }, /* 40 */
  { 0x09CB, 0x09CC, PDF_TEXT_UCD_WBP_Extend  }, /* 41 */
  { 0x09CD, 0x09CD, PDF_TEXT_UCD_WBP_Extend  }, /* 42 */
  { 0x09D7, 0x09D7, PDF_TEXT_UCD_WBP_Extend  }, /* 43 */
  { 0x09E2, 0x09E3, PDF_TEXT_UCD_WBP_Extend  }, /* 44 */
  { 0x0A01, 0x0A02, PDF_TEXT_UCD_WBP_Extend  }, /* 45 */
  { 0x0A03, 0x0A03, PDF_TEXT_UCD_WBP_Extend  }, /* 46 */
  { 0x0A3C, 0x0A3C, PDF_TEXT_UCD_WBP_Extend  }, /* 47 */
  { 0x0A3E, 0x0A40, PDF_TEXT_UCD_WBP_Extend  }, /* 48 */
  { 0x0A41, 0x0A42, PDF_TEXT_UCD_WBP_Extend  }, /* 49 */
  { 0x0A47, 0x0A48, PDF_TEXT_UCD_WBP_Extend  }, /* 50 */
  { 0x0A4B, 0x0A4D, PDF_TEXT_UCD_WBP_Extend  }, /* 51 */
  { 0x0A51, 0x0A51, PDF_TEXT_UCD_WBP_Extend  }, /* 52 */
  { 0x0A70, 0x0A71, PDF_TEXT_UCD_WBP_Extend  }, /* 53 */
  { 0x0A75, 0x0A75, PDF_TEXT_UCD_WBP_Extend  }, /* 54 */
  { 0x0A81, 0x0A82, PDF_TEXT_UCD_WBP_Extend  }, /* 55 */
  { 0x0A83, 0x0A83, PDF_TEXT_UCD_WBP_Extend  }, /* 56 */
  { 0x0ABC, 0x0ABC, PDF_TEXT_UCD_WBP_Extend  }, /* 57 */
  { 0x0ABE, 0x0AC0, PDF_TEXT_UCD_WBP_Extend  }, /* 58 */
  { 0x0AC1, 0x0AC5, PDF_TEXT_UCD_WBP_Extend  }, /* 59 */
  { 0x0AC7, 0x0AC8, PDF_TEXT_UCD_WBP_Extend  }, /* 60 */
  { 0x0AC9, 0x0AC9, PDF_TEXT_UCD_WBP_Extend  }, /* 61 */
  { 0x0ACB, 0x0ACC, PDF_TEXT_UCD_WBP_Extend  }, /* 62 */
  { 0x0ACD, 0x0ACD, PDF_TEXT_UCD_WBP_Extend  }, /* 63 */
  { 0x0AE2, 0x0AE3, PDF_TEXT_UCD_WBP_Extend  }, /* 64 */
  { 0x0B01, 0x0B01, PDF_TEXT_UCD_WBP_Extend  }, /* 65 */
  { 0x0B02, 0x0B03, PDF_TEXT_UCD_WBP_Extend  }, /* 66 */
  { 0x0B3C, 0x0B3C, PDF_TEXT_UCD_WBP_Extend  }, /* 67 */
  { 0x0B3E, 0x0B3E, PDF_TEXT_UCD_WBP_Extend  }, /* 68 */
  { 0x0B3F, 0x0B3F, PDF_TEXT_UCD_WBP_Extend  }, /* 69 */
  { 0x0B40, 0x0B40, PDF_TEXT_UCD_WBP_Extend  }, /* 70 */
  { 0x0B41, 0x0B44, PDF_TEXT_UCD_WBP_Extend  }, /* 71 */
  { 0x0B47, 0x0B48, PDF_TEXT_UCD_WBP_Extend  }, /* 72 */
  { 0x0B4B, 0x0B4C, PDF_TEXT_UCD_WBP_Extend  }, /* 73 */
  { 0x0B4D, 0x0B4D, PDF_TEXT_UCD_WBP_Extend  }, /* 74 */
  { 0x0B56, 0x0B56, PDF_TEXT_UCD_WBP_Extend  }, /* 75 */
  { 0x0B57, 0x0B57, PDF_TEXT_UCD_WBP_Extend  }, /* 76 */
  { 0x0B62, 0x0B63, PDF_TEXT_UCD_WBP_Extend  }, /* 77 */
  { 0x0B82, 0x0B82, PDF_TEXT_UCD_WBP_Extend  }, /* 78 */
  { 0x0BBE, 0x0BBF, PDF_TEXT_UCD_WBP_Extend  }, /* 79 */
  { 0x0BC0, 0x0BC0, PDF_TEXT_UCD_WBP_Extend  }, /* 80 */
  { 0x0BC1, 0x0BC2, PDF_TEXT_UCD_WBP_Extend  }, /* 81 */
  { 0x0BC6, 0x0BC8, PDF_TEXT_UCD_WBP_Extend  }, /* 82 */
  { 0x0BCA, 0x0BCC, PDF_TEXT_UCD_WBP_Extend  }, /* 83 */
  { 0x0BCD, 0x0BCD, PDF_TEXT_UCD_WBP_Extend  }, /* 84 */
  { 0x0BD7, 0x0BD7, PDF_TEXT_UCD_WBP_Extend  }, /* 85 */
  { 0x0C01, 0x0C03, PDF_TEXT_UCD_WBP_Extend  }, /* 86 */
  { 0x0C3E, 0x0C40, PDF_TEXT_UCD_WBP_Extend  }, /* 87 */
  { 0x0C41, 0x0C44, PDF_TEXT_UCD_WBP_Extend  }, /* 88 */
  { 0x0C46, 0x0C48, PDF_TEXT_UCD_WBP_Extend  }, /* 89 */
  { 0x0C4A, 0x0C4D, PDF_TEXT_UCD_WBP_Extend  }, /* 90 */
  { 0x0C55, 0x0C56, PDF_TEXT_UCD_WBP_Extend  }, /* 91 */
  { 0x0C62, 0x0C63, PDF_TEXT_UCD_WBP_Extend  }, /* 92 */
  { 0x0C82, 0x0C83, PDF_TEXT_UCD_WBP_Extend  }, /* 93 */
  { 0x0CBC, 0x0CBC, PDF_TEXT_UCD_WBP_Extend  }, /* 94 */
  { 0x0CBE, 0x0CBE, PDF_TEXT_UCD_WBP_Extend  }, /* 95 */
  { 0x0CBF, 0x0CBF, PDF_TEXT_UCD_WBP_Extend  }, /* 96 */
  { 0x0CC0, 0x0CC4, PDF_TEXT_UCD_WBP_Extend  }, /* 97 */
  { 0x0CC6, 0x0CC6, PDF_TEXT_UCD_WBP_Extend  }, /* 98 */
  { 0x0CC7, 0x0CC8, PDF_TEXT_UCD_WBP_Extend  }, /* 99 */
  { 0x0CCA, 0x0CCB, PDF_TEXT_UCD_WBP_Extend  }, /* 100 */
  { 0x0CCC, 0x0CCD, PDF_TEXT_UCD_WBP_Extend  }, /* 101 */
  { 0x0CD5, 0x0CD6, PDF_TEXT_UCD_WBP_Extend  }, /* 102 */
  { 0x0CE2, 0x0CE3, PDF_TEXT_UCD_WBP_Extend  }, /* 103 */
  { 0x0D02, 0x0D03, PDF_TEXT_UCD_WBP_Extend  }, /* 104 */
  { 0x0D3E, 0x0D40, PDF_TEXT_UCD_WBP_Extend  }, /* 105 */
  { 0x0D41, 0x0D44, PDF_TEXT_UCD_WBP_Extend  }, /* 106 */
  { 0x0D46, 0x0D48, PDF_TEXT_UCD_WBP_Extend  }, /* 107 */
  { 0x0D4A, 0x0D4C, PDF_TEXT_UCD_WBP_Extend  }, /* 108 */
  { 0x0D4D, 0x0D4D, PDF_TEXT_UCD_WBP_Extend  }, /* 109 */
  { 0x0D57, 0x0D57, PDF_TEXT_UCD_WBP_Extend  }, /* 110 */
  { 0x0D62, 0x0D63, PDF_TEXT_UCD_WBP_Extend  }, /* 111 */
  { 0x0D82, 0x0D83, PDF_TEXT_UCD_WBP_Extend  }, /* 112 */
  { 0x0DCA, 0x0DCA, PDF_TEXT_UCD_WBP_Extend  }, /* 113 */
  { 0x0DCF, 0x0DD1, PDF_TEXT_UCD_WBP_Extend  }, /* 114 */
  { 0x0DD2, 0x0DD4, PDF_TEXT_UCD_WBP_Extend  }, /* 115 */
  { 0x0DD6, 0x0DD6, PDF_TEXT_UCD_WBP_Extend  }, /* 116 */
  { 0x0DD8, 0x0DDF, PDF_TEXT_UCD_WBP_Extend  }, /* 117 */
  { 0x0DF2, 0x0DF3, PDF_TEXT_UCD_WBP_Extend  }, /* 118 */
  { 0x0E31, 0x0E31, PDF_TEXT_UCD_WBP_Extend  }, /* 119 */
  { 0x0E34, 0x0E3A, PDF_TEXT_UCD_WBP_Extend  }, /* 120 */
  { 0x0E47, 0x0E4E, PDF_TEXT_UCD_WBP_Extend  }, /* 121 */
  { 0x0EB1, 0x0EB1, PDF_TEXT_UCD_WBP_Extend  }, /* 122 */
  { 0x0EB4, 0x0EB9, PDF_TEXT_UCD_WBP_Extend  }, /* 123 */
  { 0x0EBB, 0x0EBC, PDF_TEXT_UCD_WBP_Extend  }, /* 124 */
  { 0x0EC8, 0x0ECD, PDF_TEXT_UCD_WBP_Extend  }, /* 125 */
  { 0x0F18, 0x0F19, PDF_TEXT_UCD_WBP_Extend  }, /* 126 */
  { 0x0F35, 0x0F35, PDF_TEXT_UCD_WBP_Extend  }, /* 127 */
  { 0x0F37, 0x0F37, PDF_TEXT_UCD_WBP_Extend  }, /* 128 */
  { 0x0F39, 0x0F39, PDF_TEXT_UCD_WBP_Extend  }, /* 129 */
  { 0x0F3E, 0x0F3F, PDF_TEXT_UCD_WBP_Extend  }, /* 130 */
  { 0x0F71, 0x0F7E, PDF_TEXT_UCD_WBP_Extend  }, /* 131 */
  { 0x0F7F, 0x0F7F, PDF_TEXT_UCD_WBP_Extend  }, /* 132 */
  { 0x0F80, 0x0F84, PDF_TEXT_UCD_WBP_Extend  }, /* 133 */
  { 0x0F86, 0x0F87, PDF_TEXT_UCD_WBP_Extend  }, /* 134 */
  { 0x0F90, 0x0F97, PDF_TEXT_UCD_WBP_Extend  }, /* 135 */
  { 0x0F99, 0x0FBC, PDF_TEXT_UCD_WBP_Extend  }, /* 136 */
  { 0x0FC6, 0x0FC6, PDF_TEXT_UCD_WBP_Extend  }, /* 137 */
  { 0x102B, 0x102C, PDF_TEXT_UCD_WBP_Extend  }, /* 138 */
  { 0x102D, 0x1030, PDF_TEXT_UCD_WBP_Extend  }, /* 139 */
  { 0x1031, 0x1031, PDF_TEXT_UCD_WBP_Extend  }, /* 140 */
  { 0x1032, 0x1037, PDF_TEXT_UCD_WBP_Extend  }, /* 141 */
  { 0x1038, 0x1038, PDF_TEXT_UCD_WBP_Extend  }, /* 142 */
  { 0x1039, 0x103A, PDF_TEXT_UCD_WBP_Extend  }, /* 143 */
  { 0x103B, 0x103C, PDF_TEXT_UCD_WBP_Extend  }, /* 144 */
  { 0x103D, 0x103E, PDF_TEXT_UCD_WBP_Extend  }, /* 145 */
  { 0x1056, 0x1057, PDF_TEXT_UCD_WBP_Extend  }, /* 146 */
  { 0x1058, 0x1059, PDF_TEXT_UCD_WBP_Extend  }, /* 147 */
  { 0x105E, 0x1060, PDF_TEXT_UCD_WBP_Extend  }, /* 148 */
  { 0x1062, 0x1064, PDF_TEXT_UCD_WBP_Extend  }, /* 149 */
  { 0x1067, 0x106D, PDF_TEXT_UCD_WBP_Extend  }, /* 150 */
  { 0x1071, 0x1074, PDF_TEXT_UCD_WBP_Extend  }, /* 151 */
  { 0x1082, 0x1082, PDF_TEXT_UCD_WBP_Extend  }, /* 152 */
  { 0x1083, 0x1084, PDF_TEXT_UCD_WBP_Extend  }, /* 153 */
  { 0x1085, 0x1086, PDF_TEXT_UCD_WBP_Extend  }, /* 154 */
  { 0x1087, 0x108C, PDF_TEXT_UCD_WBP_Extend  }, /* 155 */
  { 0x108D, 0x108D, PDF_TEXT_UCD_WBP_Extend  }, /* 156 */
  { 0x108F, 0x108F, PDF_TEXT_UCD_WBP_Extend  }, /* 157 */
  { 0x135F, 0x135F, PDF_TEXT_UCD_WBP_Extend  }, /* 158 */
  { 0x1712, 0x1714, PDF_TEXT_UCD_WBP_Extend  }, /* 159 */
  { 0x1732, 0x1734, PDF_TEXT_UCD_WBP_Extend  }, /* 160 */
  { 0x1752, 0x1753, PDF_TEXT_UCD_WBP_Extend  }, /* 161 */
  { 0x1772, 0x1773, PDF_TEXT_UCD_WBP_Extend  }, /* 162 */
  { 0x17B6, 0x17B6, PDF_TEXT_UCD_WBP_Extend  }, /* 163 */
  { 0x17B7, 0x17BD, PDF_TEXT_UCD_WBP_Extend  }, /* 164 */
  { 0x17BE, 0x17C5, PDF_TEXT_UCD_WBP_Extend  }, /* 165 */
  { 0x17C6, 0x17C6, PDF_TEXT_UCD_WBP_Extend  }, /* 166 */
  { 0x17C7, 0x17C8, PDF_TEXT_UCD_WBP_Extend  }, /* 167 */
  { 0x17C9, 0x17D3, PDF_TEXT_UCD_WBP_Extend  }, /* 168 */
  { 0x17DD, 0x17DD, PDF_TEXT_UCD_WBP_Extend  }, /* 169 */
  { 0x180B, 0x180D, PDF_TEXT_UCD_WBP_Extend  }, /* 170 */
  { 0x18A9, 0x18A9, PDF_TEXT_UCD_WBP_Extend  }, /* 171 */
  { 0x1920, 0x1922, PDF_TEXT_UCD_WBP_Extend  }, /* 172 */
  { 0x1923, 0x1926, PDF_TEXT_UCD_WBP_Extend  }, /* 173 */
  { 0x1927, 0x1928, PDF_TEXT_UCD_WBP_Extend  }, /* 174 */
  { 0x1929, 0x192B, PDF_TEXT_UCD_WBP_Extend  }, /* 175 */
  { 0x1930, 0x1931, PDF_TEXT_UCD_WBP_Extend  }, /* 176 */
  { 0x1932, 0x1932, PDF_TEXT_UCD_WBP_Extend  }, /* 177 */
  { 0x1933, 0x1938, PDF_TEXT_UCD_WBP_Extend  }, /* 178 */
  { 0x1939, 0x193B, PDF_TEXT_UCD_WBP_Extend  }, /* 179 */
  { 0x19B0, 0x19C0, PDF_TEXT_UCD_WBP_Extend  }, /* 180 */
  { 0x19C8, 0x19C9, PDF_TEXT_UCD_WBP_Extend  }, /* 181 */
  { 0x1A17, 0x1A18, PDF_TEXT_UCD_WBP_Extend  }, /* 182 */
  { 0x1A19, 0x1A1B, PDF_TEXT_UCD_WBP_Extend  }, /* 183 */
  { 0x1B00, 0x1B03, PDF_TEXT_UCD_WBP_Extend  }, /* 184 */
  { 0x1B04, 0x1B04, PDF_TEXT_UCD_WBP_Extend  }, /* 185 */
  { 0x1B34, 0x1B34, PDF_TEXT_UCD_WBP_Extend  }, /* 186 */
  { 0x1B35, 0x1B35, PDF_TEXT_UCD_WBP_Extend  }, /* 187 */
  { 0x1B36, 0x1B3A, PDF_TEXT_UCD_WBP_Extend  }, /* 188 */
  { 0x1B3B, 0x1B3B, PDF_TEXT_UCD_WBP_Extend  }, /* 189 */
  { 0x1B3C, 0x1B3C, PDF_TEXT_UCD_WBP_Extend  }, /* 190 */
  { 0x1B3D, 0x1B41, PDF_TEXT_UCD_WBP_Extend  }, /* 191 */
  { 0x1B42, 0x1B42, PDF_TEXT_UCD_WBP_Extend  }, /* 192 */
  { 0x1B43, 0x1B44, PDF_TEXT_UCD_WBP_Extend  }, /* 193 */
  { 0x1B6B, 0x1B73, PDF_TEXT_UCD_WBP_Extend  }, /* 194 */
  { 0x1B80, 0x1B81, PDF_TEXT_UCD_WBP_Extend  }, /* 195 */
  { 0x1B82, 0x1B82, PDF_TEXT_UCD_WBP_Extend  }, /* 196 */
  { 0x1BA1, 0x1BA1, PDF_TEXT_UCD_WBP_Extend  }, /* 197 */
  { 0x1BA2, 0x1BA5, PDF_TEXT_UCD_WBP_Extend  }, /* 198 */
  { 0x1BA6, 0x1BA7, PDF_TEXT_UCD_WBP_Extend  }, /* 199 */
  { 0x1BA8, 0x1BA9, PDF_TEXT_UCD_WBP_Extend  }, /* 200 */
  { 0x1BAA, 0x1BAA, PDF_TEXT_UCD_WBP_Extend  }, /* 201 */
  { 0x1C24, 0x1C2B, PDF_TEXT_UCD_WBP_Extend  }, /* 202 */
  { 0x1C2C, 0x1C33, PDF_TEXT_UCD_WBP_Extend  }, /* 203 */
  { 0x1C34, 0x1C35, PDF_TEXT_UCD_WBP_Extend  }, /* 204 */
  { 0x1C36, 0x1C37, PDF_TEXT_UCD_WBP_Extend  }, /* 205 */
  { 0x1DC0, 0x1DE6, PDF_TEXT_UCD_WBP_Extend  }, /* 206 */
  { 0x1DFE, 0x1DFF, PDF_TEXT_UCD_WBP_Extend  }, /* 207 */
  { 0x200C, 0x200D, PDF_TEXT_UCD_WBP_Extend  }, /* 208 */
  { 0x20D0, 0x20DC, PDF_TEXT_UCD_WBP_Extend  }, /* 209 */
  { 0x20DD, 0x20E0, PDF_TEXT_UCD_WBP_Extend  }, /* 210 */
  { 0x20E1, 0x20E1, PDF_TEXT_UCD_WBP_Extend  }, /* 211 */
  { 0x20E2, 0x20E4, PDF_TEXT_UCD_WBP_Extend  }, /* 212 */
  { 0x20E5, 0x20F0, PDF_TEXT_UCD_WBP_Extend  }, /* 213 */
  { 0x2DE0, 0x2DFF, PDF_TEXT_UCD_WBP_Extend  }, /* 214 */
  { 0x302A, 0x302F, PDF_TEXT_UCD_WBP_Extend  }, /* 215 */
  { 0x3099, 0x309A, PDF_TEXT_UCD_WBP_Extend  }, /* 216 */
  { 0xA66F, 0xA66F, PDF_TEXT_UCD_WBP_Extend  }, /* 217 */
  { 0xA670, 0xA672, PDF_TEXT_UCD_WBP_Extend  }, /* 218 */
  { 0xA67C, 0xA67D, PDF_TEXT_UCD_WBP_Extend  }, /* 219 */
  { 0xA802, 0xA802, PDF_TEXT_UCD_WBP_Extend  }, /* 220 */
  { 0xA806, 0xA806, PDF_TEXT_UCD_WBP_Extend  }, /* 221 */
  { 0xA80B, 0xA80B, PDF_TEXT_UCD_WBP_Extend  }, /* 222 */
  { 0xA823, 0xA824, PDF_TEXT_UCD_WBP_Extend  }, /* 223 */
  { 0xA825, 0xA826, PDF_TEXT_UCD_WBP_Extend  }, /* 224 */
  { 0xA827, 0xA827, PDF_TEXT_UCD_WBP_Extend  }, /* 225 */
  { 0xA880, 0xA881, PDF_TEXT_UCD_WBP_Extend  }, /* 226 */
  { 0xA8B4, 0xA8C3, PDF_TEXT_UCD_WBP_Extend  }, /* 227 */
  { 0xA8C4, 0xA8C4, PDF_TEXT_UCD_WBP_Extend  }, /* 228 */
  { 0xA926, 0xA92D, PDF_TEXT_UCD_WBP_Extend  }, /* 229 */
  { 0xA947, 0xA951, PDF_TEXT_UCD_WBP_Extend  }, /* 230 */
  { 0xA952, 0xA953, PDF_TEXT_UCD_WBP_Extend  }, /* 231 */
  { 0xAA29, 0xAA2E, PDF_TEXT_UCD_WBP_Extend  }, /* 232 */
  { 0xAA2F, 0xAA30, PDF_TEXT_UCD_WBP_Extend  }, /* 233 */
  { 0xAA31, 0xAA32, PDF_TEXT_UCD_WBP_Extend  }, /* 234 */
  { 0xAA33, 0xAA34, PDF_TEXT_UCD_WBP_Extend  }, /* 235 */
  { 0xAA35, 0xAA36, PDF_TEXT_UCD_WBP_Extend  }, /* 236 */
  { 0xAA43, 0xAA43, PDF_TEXT_UCD_WBP_Extend  }, /* 237 */
  { 0xAA4C, 0xAA4C, PDF_TEXT_UCD_WBP_Extend  }, /* 238 */
  { 0xAA4D, 0xAA4D, PDF_TEXT_UCD_WBP_Extend  }, /* 239 */
  { 0xFB1E, 0xFB1E, PDF_TEXT_UCD_WBP_Extend  }, /* 240 */
  { 0xFE00, 0xFE0F, PDF_TEXT_UCD_WBP_Extend  }, /* 241 */
  { 0xFE20, 0xFE26, PDF_TEXT_UCD_WBP_Extend  }, /* 242 */
  { 0xFF9E, 0xFF9F, PDF_TEXT_UCD_WBP_Extend  }, /* 243 */
  { 0x101FD, 0x101FD, PDF_TEXT_UCD_WBP_Extend  }, /* 244 */
  { 0x10A01, 0x10A03, PDF_TEXT_UCD_WBP_Extend  }, /* 245 */
  { 0x10A05, 0x10A06, PDF_TEXT_UCD_WBP_Extend  }, /* 246 */
  { 0x10A0C, 0x10A0F, PDF_TEXT_UCD_WBP_Extend  }, /* 247 */
  { 0x10A38, 0x10A3A, PDF_TEXT_UCD_WBP_Extend  }, /* 248 */
  { 0x10A3F, 0x10A3F, PDF_TEXT_UCD_WBP_Extend  }, /* 249 */
  { 0x1D165, 0x1D166, PDF_TEXT_UCD_WBP_Extend  }, /* 250 */
  { 0x1D167, 0x1D169, PDF_TEXT_UCD_WBP_Extend  }, /* 251 */
  { 0x1D16D, 0x1D172, PDF_TEXT_UCD_WBP_Extend  }, /* 252 */
  { 0x1D17B, 0x1D182, PDF_TEXT_UCD_WBP_Extend  }, /* 253 */
  { 0x1D185, 0x1D18B, PDF_TEXT_UCD_WBP_Extend  }, /* 254 */
  { 0x1D1AA, 0x1D1AD, PDF_TEXT_UCD_WBP_Extend  }, /* 255 */
  { 0x1D242, 0x1D244, PDF_TEXT_UCD_WBP_Extend  }, /* 256 */
  { 0xE0100, 0xE01EF, PDF_TEXT_UCD_WBP_Extend  }, /* 257 */
  { 0x00AD, 0x00AD, PDF_TEXT_UCD_WBP_Format  }, /* 258 */
  { 0x0600, 0x0603, PDF_TEXT_UCD_WBP_Format  }, /* 259 */
  { 0x06DD, 0x06DD, PDF_TEXT_UCD_WBP_Format  }, /* 260 */
  { 0x070F, 0x070F, PDF_TEXT_UCD_WBP_Format  }, /* 261 */
  { 0x17B4, 0x17B5, PDF_TEXT_UCD_WBP_Format  }, /* 262 */
  { 0x200B, 0x200B, PDF_TEXT_UCD_WBP_Format  }, /* 263 */
  { 0x200E, 0x200F, PDF_TEXT_UCD_WBP_Format  }, /* 264 */
  { 0x202A, 0x202E, PDF_TEXT_UCD_WBP_Format  }, /* 265 */
  { 0x2060, 0x2064, PDF_TEXT_UCD_WBP_Format  }, /* 266 */
  { 0x206A, 0x206F, PDF_TEXT_UCD_WBP_Format  }, /* 267 */
  { 0xFEFF, 0xFEFF, PDF_TEXT_UCD_WBP_Format  }, /* 268 */
  { 0xFFF9, 0xFFFB, PDF_TEXT_UCD_WBP_Format  }, /* 269 */
  { 0x1D173, 0x1D17A, PDF_TEXT_UCD_WBP_Format  }, /* 270 */
  { 0xE0001, 0xE0001, PDF_TEXT_UCD_WBP_Format  }, /* 271 */
  { 0xE0020, 0xE007F, PDF_TEXT_UCD_WBP_Format  }, /* 272 */
  { 0x3031, 0x3035, PDF_TEXT_UCD_WBP_Katakana  }, /* 273 */
  { 0x309B, 0x309C, PDF_TEXT_UCD_WBP_Katakana  }, /* 274 */
  { 0x30A0, 0x30A0, PDF_TEXT_UCD_WBP_Katakana  }, /* 275 */
  { 0x30A1, 0x30FA, PDF_TEXT_UCD_WBP_Katakana  }, /* 276 */
  { 0x30FC, 0x30FE, PDF_TEXT_UCD_WBP_Katakana  }, /* 277 */
  { 0x30FF, 0x30FF, PDF_TEXT_UCD_WBP_Katakana  }, /* 278 */
  { 0x31F0, 0x31FF, PDF_TEXT_UCD_WBP_Katakana  }, /* 279 */
  { 0x32D0, 0x32FE, PDF_TEXT_UCD_WBP_Katakana  }, /* 280 */
  { 0x3300, 0x3357, PDF_TEXT_UCD_WBP_Katakana  }, /* 281 */
  { 0xFF66, 0xFF6F, PDF_TEXT_UCD_WBP_Katakana  }, /* 282 */
  { 0xFF70, 0xFF70, PDF_TEXT_UCD_WBP_Katakana  }, /* 283 */
  { 0xFF71, 0xFF9D, PDF_TEXT_UCD_WBP_Katakana  }, /* 284 */
  { 0x0041, 0x005A, PDF_TEXT_UCD_WBP_ALetter  }, /* 285 */
  { 0x0061, 0x007A, PDF_TEXT_UCD_WBP_ALetter  }, /* 286 */
  { 0x00AA, 0x00AA, PDF_TEXT_UCD_WBP_ALetter  }, /* 287 */
  { 0x00B5, 0x00B5, PDF_TEXT_UCD_WBP_ALetter  }, /* 288 */
  { 0x00BA, 0x00BA, PDF_TEXT_UCD_WBP_ALetter  }, /* 289 */
  { 0x00C0, 0x00D6, PDF_TEXT_UCD_WBP_ALetter  }, /* 290 */
  { 0x00D8, 0x00F6, PDF_TEXT_UCD_WBP_ALetter  }, /* 291 */
  { 0x00F8, 0x01BA, PDF_TEXT_UCD_WBP_ALetter  }, /* 292 */
  { 0x01BB, 0x01BB, PDF_TEXT_UCD_WBP_ALetter  }, /* 293 */
  { 0x01BC, 0x01BF, PDF_TEXT_UCD_WBP_ALetter  }, /* 294 */
  { 0x01C0, 0x01C3, PDF_TEXT_UCD_WBP_ALetter  }, /* 295 */
  { 0x01C4, 0x0293, PDF_TEXT_UCD_WBP_ALetter  }, /* 296 */
  { 0x0294, 0x0294, PDF_TEXT_UCD_WBP_ALetter  }, /* 297 */
  { 0x0295, 0x02AF, PDF_TEXT_UCD_WBP_ALetter  }, /* 298 */
  { 0x02B0, 0x02C1, PDF_TEXT_UCD_WBP_ALetter  }, /* 299 */
  { 0x02C6, 0x02D1, PDF_TEXT_UCD_WBP_ALetter  }, /* 300 */
  { 0x02E0, 0x02E4, PDF_TEXT_UCD_WBP_ALetter  }, /* 301 */
  { 0x02EC, 0x02EC, PDF_TEXT_UCD_WBP_ALetter  }, /* 302 */
  { 0x02EE, 0x02EE, PDF_TEXT_UCD_WBP_ALetter  }, /* 303 */
  { 0x0370, 0x0373, PDF_TEXT_UCD_WBP_ALetter  }, /* 304 */
  { 0x0374, 0x0374, PDF_TEXT_UCD_WBP_ALetter  }, /* 305 */
  { 0x0376, 0x0377, PDF_TEXT_UCD_WBP_ALetter  }, /* 306 */
  { 0x037A, 0x037A, PDF_TEXT_UCD_WBP_ALetter  }, /* 307 */
  { 0x037B, 0x037D, PDF_TEXT_UCD_WBP_ALetter  }, /* 308 */
  { 0x0386, 0x0386, PDF_TEXT_UCD_WBP_ALetter  }, /* 309 */
  { 0x0388, 0x038A, PDF_TEXT_UCD_WBP_ALetter  }, /* 310 */
  { 0x038C, 0x038C, PDF_TEXT_UCD_WBP_ALetter  }, /* 311 */
  { 0x038E, 0x03A1, PDF_TEXT_UCD_WBP_ALetter  }, /* 312 */
  { 0x03A3, 0x03F5, PDF_TEXT_UCD_WBP_ALetter  }, /* 313 */
  { 0x03F7, 0x0481, PDF_TEXT_UCD_WBP_ALetter  }, /* 314 */
  { 0x048A, 0x0523, PDF_TEXT_UCD_WBP_ALetter  }, /* 315 */
  { 0x0531, 0x0556, PDF_TEXT_UCD_WBP_ALetter  }, /* 316 */
  { 0x0559, 0x0559, PDF_TEXT_UCD_WBP_ALetter  }, /* 317 */
  { 0x0561, 0x0587, PDF_TEXT_UCD_WBP_ALetter  }, /* 318 */
  { 0x05D0, 0x05EA, PDF_TEXT_UCD_WBP_ALetter  }, /* 319 */
  { 0x05F0, 0x05F2, PDF_TEXT_UCD_WBP_ALetter  }, /* 320 */
  { 0x05F3, 0x05F3, PDF_TEXT_UCD_WBP_ALetter  }, /* 321 */
  { 0x0621, 0x063F, PDF_TEXT_UCD_WBP_ALetter  }, /* 322 */
  { 0x0640, 0x0640, PDF_TEXT_UCD_WBP_ALetter  }, /* 323 */
  { 0x0641, 0x064A, PDF_TEXT_UCD_WBP_ALetter  }, /* 324 */
  { 0x066E, 0x066F, PDF_TEXT_UCD_WBP_ALetter  }, /* 325 */
  { 0x0671, 0x06D3, PDF_TEXT_UCD_WBP_ALetter  }, /* 326 */
  { 0x06D5, 0x06D5, PDF_TEXT_UCD_WBP_ALetter  }, /* 327 */
  { 0x06E5, 0x06E6, PDF_TEXT_UCD_WBP_ALetter  }, /* 328 */
  { 0x06EE, 0x06EF, PDF_TEXT_UCD_WBP_ALetter  }, /* 329 */
  { 0x06FA, 0x06FC, PDF_TEXT_UCD_WBP_ALetter  }, /* 330 */
  { 0x06FF, 0x06FF, PDF_TEXT_UCD_WBP_ALetter  }, /* 331 */
  { 0x0710, 0x0710, PDF_TEXT_UCD_WBP_ALetter  }, /* 332 */
  { 0x0712, 0x072F, PDF_TEXT_UCD_WBP_ALetter  }, /* 333 */
  { 0x074D, 0x07A5, PDF_TEXT_UCD_WBP_ALetter  }, /* 334 */
  { 0x07B1, 0x07B1, PDF_TEXT_UCD_WBP_ALetter  }, /* 335 */
  { 0x07CA, 0x07EA, PDF_TEXT_UCD_WBP_ALetter  }, /* 336 */
  { 0x07F4, 0x07F5, PDF_TEXT_UCD_WBP_ALetter  }, /* 337 */
  { 0x07FA, 0x07FA, PDF_TEXT_UCD_WBP_ALetter  }, /* 338 */
  { 0x0904, 0x0939, PDF_TEXT_UCD_WBP_ALetter  }, /* 339 */
  { 0x093D, 0x093D, PDF_TEXT_UCD_WBP_ALetter  }, /* 340 */
  { 0x0950, 0x0950, PDF_TEXT_UCD_WBP_ALetter  }, /* 341 */
  { 0x0958, 0x0961, PDF_TEXT_UCD_WBP_ALetter  }, /* 342 */
  { 0x0971, 0x0971, PDF_TEXT_UCD_WBP_ALetter  }, /* 343 */
  { 0x0972, 0x0972, PDF_TEXT_UCD_WBP_ALetter  }, /* 344 */
  { 0x097B, 0x097F, PDF_TEXT_UCD_WBP_ALetter  }, /* 345 */
  { 0x0985, 0x098C, PDF_TEXT_UCD_WBP_ALetter  }, /* 346 */
  { 0x098F, 0x0990, PDF_TEXT_UCD_WBP_ALetter  }, /* 347 */
  { 0x0993, 0x09A8, PDF_TEXT_UCD_WBP_ALetter  }, /* 348 */
  { 0x09AA, 0x09B0, PDF_TEXT_UCD_WBP_ALetter  }, /* 349 */
  { 0x09B2, 0x09B2, PDF_TEXT_UCD_WBP_ALetter  }, /* 350 */
  { 0x09B6, 0x09B9, PDF_TEXT_UCD_WBP_ALetter  }, /* 351 */
  { 0x09BD, 0x09BD, PDF_TEXT_UCD_WBP_ALetter  }, /* 352 */
  { 0x09CE, 0x09CE, PDF_TEXT_UCD_WBP_ALetter  }, /* 353 */
  { 0x09DC, 0x09DD, PDF_TEXT_UCD_WBP_ALetter  }, /* 354 */
  { 0x09DF, 0x09E1, PDF_TEXT_UCD_WBP_ALetter  }, /* 355 */
  { 0x09F0, 0x09F1, PDF_TEXT_UCD_WBP_ALetter  }, /* 356 */
  { 0x0A05, 0x0A0A, PDF_TEXT_UCD_WBP_ALetter  }, /* 357 */
  { 0x0A0F, 0x0A10, PDF_TEXT_UCD_WBP_ALetter  }, /* 358 */
  { 0x0A13, 0x0A28, PDF_TEXT_UCD_WBP_ALetter  }, /* 359 */
  { 0x0A2A, 0x0A30, PDF_TEXT_UCD_WBP_ALetter  }, /* 360 */
  { 0x0A32, 0x0A33, PDF_TEXT_UCD_WBP_ALetter  }, /* 361 */
  { 0x0A35, 0x0A36, PDF_TEXT_UCD_WBP_ALetter  }, /* 362 */
  { 0x0A38, 0x0A39, PDF_TEXT_UCD_WBP_ALetter  }, /* 363 */
  { 0x0A59, 0x0A5C, PDF_TEXT_UCD_WBP_ALetter  }, /* 364 */
  { 0x0A5E, 0x0A5E, PDF_TEXT_UCD_WBP_ALetter  }, /* 365 */
  { 0x0A72, 0x0A74, PDF_TEXT_UCD_WBP_ALetter  }, /* 366 */
  { 0x0A85, 0x0A8D, PDF_TEXT_UCD_WBP_ALetter  }, /* 367 */
  { 0x0A8F, 0x0A91, PDF_TEXT_UCD_WBP_ALetter  }, /* 368 */
  { 0x0A93, 0x0AA8, PDF_TEXT_UCD_WBP_ALetter  }, /* 369 */
  { 0x0AAA, 0x0AB0, PDF_TEXT_UCD_WBP_ALetter  }, /* 370 */
  { 0x0AB2, 0x0AB3, PDF_TEXT_UCD_WBP_ALetter  }, /* 371 */
  { 0x0AB5, 0x0AB9, PDF_TEXT_UCD_WBP_ALetter  }, /* 372 */
  { 0x0ABD, 0x0ABD, PDF_TEXT_UCD_WBP_ALetter  }, /* 373 */
  { 0x0AD0, 0x0AD0, PDF_TEXT_UCD_WBP_ALetter  }, /* 374 */
  { 0x0AE0, 0x0AE1, PDF_TEXT_UCD_WBP_ALetter  }, /* 375 */
  { 0x0B05, 0x0B0C, PDF_TEXT_UCD_WBP_ALetter  }, /* 376 */
  { 0x0B0F, 0x0B10, PDF_TEXT_UCD_WBP_ALetter  }, /* 377 */
  { 0x0B13, 0x0B28, PDF_TEXT_UCD_WBP_ALetter  }, /* 378 */
  { 0x0B2A, 0x0B30, PDF_TEXT_UCD_WBP_ALetter  }, /* 379 */
  { 0x0B32, 0x0B33, PDF_TEXT_UCD_WBP_ALetter  }, /* 380 */
  { 0x0B35, 0x0B39, PDF_TEXT_UCD_WBP_ALetter  }, /* 381 */
  { 0x0B3D, 0x0B3D, PDF_TEXT_UCD_WBP_ALetter  }, /* 382 */
  { 0x0B5C, 0x0B5D, PDF_TEXT_UCD_WBP_ALetter  }, /* 383 */
  { 0x0B5F, 0x0B61, PDF_TEXT_UCD_WBP_ALetter  }, /* 384 */
  { 0x0B71, 0x0B71, PDF_TEXT_UCD_WBP_ALetter  }, /* 385 */
  { 0x0B83, 0x0B83, PDF_TEXT_UCD_WBP_ALetter  }, /* 386 */
  { 0x0B85, 0x0B8A, PDF_TEXT_UCD_WBP_ALetter  }, /* 387 */
  { 0x0B8E, 0x0B90, PDF_TEXT_UCD_WBP_ALetter  }, /* 388 */
  { 0x0B92, 0x0B95, PDF_TEXT_UCD_WBP_ALetter  }, /* 389 */
  { 0x0B99, 0x0B9A, PDF_TEXT_UCD_WBP_ALetter  }, /* 390 */
  { 0x0B9C, 0x0B9C, PDF_TEXT_UCD_WBP_ALetter  }, /* 391 */
  { 0x0B9E, 0x0B9F, PDF_TEXT_UCD_WBP_ALetter  }, /* 392 */
  { 0x0BA3, 0x0BA4, PDF_TEXT_UCD_WBP_ALetter  }, /* 393 */
  { 0x0BA8, 0x0BAA, PDF_TEXT_UCD_WBP_ALetter  }, /* 394 */
  { 0x0BAE, 0x0BB9, PDF_TEXT_UCD_WBP_ALetter  }, /* 395 */
  { 0x0BD0, 0x0BD0, PDF_TEXT_UCD_WBP_ALetter  }, /* 396 */
  { 0x0C05, 0x0C0C, PDF_TEXT_UCD_WBP_ALetter  }, /* 397 */
  { 0x0C0E, 0x0C10, PDF_TEXT_UCD_WBP_ALetter  }, /* 398 */
  { 0x0C12, 0x0C28, PDF_TEXT_UCD_WBP_ALetter  }, /* 399 */
  { 0x0C2A, 0x0C33, PDF_TEXT_UCD_WBP_ALetter  }, /* 400 */
  { 0x0C35, 0x0C39, PDF_TEXT_UCD_WBP_ALetter  }, /* 401 */
  { 0x0C3D, 0x0C3D, PDF_TEXT_UCD_WBP_ALetter  }, /* 402 */
  { 0x0C58, 0x0C59, PDF_TEXT_UCD_WBP_ALetter  }, /* 403 */
  { 0x0C60, 0x0C61, PDF_TEXT_UCD_WBP_ALetter  }, /* 404 */
  { 0x0C85, 0x0C8C, PDF_TEXT_UCD_WBP_ALetter  }, /* 405 */
  { 0x0C8E, 0x0C90, PDF_TEXT_UCD_WBP_ALetter  }, /* 406 */
  { 0x0C92, 0x0CA8, PDF_TEXT_UCD_WBP_ALetter  }, /* 407 */
  { 0x0CAA, 0x0CB3, PDF_TEXT_UCD_WBP_ALetter  }, /* 408 */
  { 0x0CB5, 0x0CB9, PDF_TEXT_UCD_WBP_ALetter  }, /* 409 */
  { 0x0CBD, 0x0CBD, PDF_TEXT_UCD_WBP_ALetter  }, /* 410 */
  { 0x0CDE, 0x0CDE, PDF_TEXT_UCD_WBP_ALetter  }, /* 411 */
  { 0x0CE0, 0x0CE1, PDF_TEXT_UCD_WBP_ALetter  }, /* 412 */
  { 0x0D05, 0x0D0C, PDF_TEXT_UCD_WBP_ALetter  }, /* 413 */
  { 0x0D0E, 0x0D10, PDF_TEXT_UCD_WBP_ALetter  }, /* 414 */
  { 0x0D12, 0x0D28, PDF_TEXT_UCD_WBP_ALetter  }, /* 415 */
  { 0x0D2A, 0x0D39, PDF_TEXT_UCD_WBP_ALetter  }, /* 416 */
  { 0x0D3D, 0x0D3D, PDF_TEXT_UCD_WBP_ALetter  }, /* 417 */
  { 0x0D60, 0x0D61, PDF_TEXT_UCD_WBP_ALetter  }, /* 418 */
  { 0x0D7A, 0x0D7F, PDF_TEXT_UCD_WBP_ALetter  }, /* 419 */
  { 0x0D85, 0x0D96, PDF_TEXT_UCD_WBP_ALetter  }, /* 420 */
  { 0x0D9A, 0x0DB1, PDF_TEXT_UCD_WBP_ALetter  }, /* 421 */
  { 0x0DB3, 0x0DBB, PDF_TEXT_UCD_WBP_ALetter  }, /* 422 */
  { 0x0DBD, 0x0DBD, PDF_TEXT_UCD_WBP_ALetter  }, /* 423 */
  { 0x0DC0, 0x0DC6, PDF_TEXT_UCD_WBP_ALetter  }, /* 424 */
  { 0x0F00, 0x0F00, PDF_TEXT_UCD_WBP_ALetter  }, /* 425 */
  { 0x0F40, 0x0F47, PDF_TEXT_UCD_WBP_ALetter  }, /* 426 */
  { 0x0F49, 0x0F6C, PDF_TEXT_UCD_WBP_ALetter  }, /* 427 */
  { 0x0F88, 0x0F8B, PDF_TEXT_UCD_WBP_ALetter  }, /* 428 */
  { 0x10A0, 0x10C5, PDF_TEXT_UCD_WBP_ALetter  }, /* 429 */
  { 0x10D0, 0x10FA, PDF_TEXT_UCD_WBP_ALetter  }, /* 430 */
  { 0x10FC, 0x10FC, PDF_TEXT_UCD_WBP_ALetter  }, /* 431 */
  { 0x1100, 0x1159, PDF_TEXT_UCD_WBP_ALetter  }, /* 432 */
  { 0x115F, 0x11A2, PDF_TEXT_UCD_WBP_ALetter  }, /* 433 */
  { 0x11A8, 0x11F9, PDF_TEXT_UCD_WBP_ALetter  }, /* 434 */
  { 0x1200, 0x1248, PDF_TEXT_UCD_WBP_ALetter  }, /* 435 */
  { 0x124A, 0x124D, PDF_TEXT_UCD_WBP_ALetter  }, /* 436 */
  { 0x1250, 0x1256, PDF_TEXT_UCD_WBP_ALetter  }, /* 437 */
  { 0x1258, 0x1258, PDF_TEXT_UCD_WBP_ALetter  }, /* 438 */
  { 0x125A, 0x125D, PDF_TEXT_UCD_WBP_ALetter  }, /* 439 */
  { 0x1260, 0x1288, PDF_TEXT_UCD_WBP_ALetter  }, /* 440 */
  { 0x128A, 0x128D, PDF_TEXT_UCD_WBP_ALetter  }, /* 441 */
  { 0x1290, 0x12B0, PDF_TEXT_UCD_WBP_ALetter  }, /* 442 */
  { 0x12B2, 0x12B5, PDF_TEXT_UCD_WBP_ALetter  }, /* 443 */
  { 0x12B8, 0x12BE, PDF_TEXT_UCD_WBP_ALetter  }, /* 444 */
  { 0x12C0, 0x12C0, PDF_TEXT_UCD_WBP_ALetter  }, /* 445 */
  { 0x12C2, 0x12C5, PDF_TEXT_UCD_WBP_ALetter  }, /* 446 */
  { 0x12C8, 0x12D6, PDF_TEXT_UCD_WBP_ALetter  }, /* 447 */
  { 0x12D8, 0x1310, PDF_TEXT_UCD_WBP_ALetter  }, /* 448 */
  { 0x1312, 0x1315, PDF_TEXT_UCD_WBP_ALetter  }, /* 449 */
  { 0x1318, 0x135A, PDF_TEXT_UCD_WBP_ALetter  }, /* 450 */
  { 0x1380, 0x138F, PDF_TEXT_UCD_WBP_ALetter  }, /* 451 */
  { 0x13A0, 0x13F4, PDF_TEXT_UCD_WBP_ALetter  }, /* 452 */
  { 0x1401, 0x166C, PDF_TEXT_UCD_WBP_ALetter  }, /* 453 */
  { 0x166F, 0x1676, PDF_TEXT_UCD_WBP_ALetter  }, /* 454 */
  { 0x1681, 0x169A, PDF_TEXT_UCD_WBP_ALetter  }, /* 455 */
  { 0x16A0, 0x16EA, PDF_TEXT_UCD_WBP_ALetter  }, /* 456 */
  { 0x16EE, 0x16F0, PDF_TEXT_UCD_WBP_ALetter  }, /* 457 */
  { 0x1700, 0x170C, PDF_TEXT_UCD_WBP_ALetter  }, /* 458 */
  { 0x170E, 0x1711, PDF_TEXT_UCD_WBP_ALetter  }, /* 459 */
  { 0x1720, 0x1731, PDF_TEXT_UCD_WBP_ALetter  }, /* 460 */
  { 0x1740, 0x1751, PDF_TEXT_UCD_WBP_ALetter  }, /* 461 */
  { 0x1760, 0x176C, PDF_TEXT_UCD_WBP_ALetter  }, /* 462 */
  { 0x176E, 0x1770, PDF_TEXT_UCD_WBP_ALetter  }, /* 463 */
  { 0x1820, 0x1842, PDF_TEXT_UCD_WBP_ALetter  }, /* 464 */
  { 0x1843, 0x1843, PDF_TEXT_UCD_WBP_ALetter  }, /* 465 */
  { 0x1844, 0x1877, PDF_TEXT_UCD_WBP_ALetter  }, /* 466 */
  { 0x1880, 0x18A8, PDF_TEXT_UCD_WBP_ALetter  }, /* 467 */
  { 0x18AA, 0x18AA, PDF_TEXT_UCD_WBP_ALetter  }, /* 468 */
  { 0x1900, 0x191C, PDF_TEXT_UCD_WBP_ALetter  }, /* 469 */
  { 0x1A00, 0x1A16, PDF_TEXT_UCD_WBP_ALetter  }, /* 470 */
  { 0x1B05, 0x1B33, PDF_TEXT_UCD_WBP_ALetter  }, /* 471 */
  { 0x1B45, 0x1B4B, PDF_TEXT_UCD_WBP_ALetter  }, /* 472 */
  { 0x1B83, 0x1BA0, PDF_TEXT_UCD_WBP_ALetter  }, /* 473 */
  { 0x1BAE, 0x1BAF, PDF_TEXT_UCD_WBP_ALetter  }, /* 474 */
  { 0x1C00, 0x1C23, PDF_TEXT_UCD_WBP_ALetter  }, /* 475 */
  { 0x1C4D, 0x1C4F, PDF_TEXT_UCD_WBP_ALetter  }, /* 476 */
  { 0x1C5A, 0x1C77, PDF_TEXT_UCD_WBP_ALetter  }, /* 477 */
  { 0x1C78, 0x1C7D, PDF_TEXT_UCD_WBP_ALetter  }, /* 478 */
  { 0x1D00, 0x1D2B, PDF_TEXT_UCD_WBP_ALetter  }, /* 479 */
  { 0x1D2C, 0x1D61, PDF_TEXT_UCD_WBP_ALetter  }, /* 480 */
  { 0x1D62, 0x1D77, PDF_TEXT_UCD_WBP_ALetter  }, /* 481 */
  { 0x1D78, 0x1D78, PDF_TEXT_UCD_WBP_ALetter  }, /* 482 */
  { 0x1D79, 0x1D9A, PDF_TEXT_UCD_WBP_ALetter  }, /* 483 */
  { 0x1D9B, 0x1DBF, PDF_TEXT_UCD_WBP_ALetter  }, /* 484 */
  { 0x1E00, 0x1F15, PDF_TEXT_UCD_WBP_ALetter  }, /* 485 */
  { 0x1F18, 0x1F1D, PDF_TEXT_UCD_WBP_ALetter  }, /* 486 */
  { 0x1F20, 0x1F45, PDF_TEXT_UCD_WBP_ALetter  }, /* 487 */
  { 0x1F48, 0x1F4D, PDF_TEXT_UCD_WBP_ALetter  }, /* 488 */
  { 0x1F50, 0x1F57, PDF_TEXT_UCD_WBP_ALetter  }, /* 489 */
  { 0x1F59, 0x1F59, PDF_TEXT_UCD_WBP_ALetter  }, /* 490 */
  { 0x1F5B, 0x1F5B, PDF_TEXT_UCD_WBP_ALetter  }, /* 491 */
  { 0x1F5D, 0x1F5D, PDF_TEXT_UCD_WBP_ALetter  }, /* 492 */
  { 0x1F5F, 0x1F7D, PDF_TEXT_UCD_WBP_ALetter  }, /* 493 */
  { 0x1F80, 0x1FB4, PDF_TEXT_UCD_WBP_ALetter  }, /* 494 */
  { 0x1FB6, 0x1FBC, PDF_TEXT_UCD_WBP_ALetter  }, /* 495 */
  { 0x1FBE, 0x1FBE, PDF_TEXT_UCD_WBP_ALetter  }, /* 496 */
  { 0x1FC2, 0x1FC4, PDF_TEXT_UCD_WBP_ALetter  }, /* 497 */
  { 0x1FC6, 0x1FCC, PDF_TEXT_UCD_WBP_ALetter  }, /* 498 */
  { 0x1FD0, 0x1FD3, PDF_TEXT_UCD_WBP_ALetter  }, /* 499 */
  { 0x1FD6, 0x1FDB, PDF_TEXT_UCD_WBP_ALetter  }, /* 500 */
  { 0x1FE0, 0x1FEC, PDF_TEXT_UCD_WBP_ALetter  }, /* 501 */
  { 0x1FF2, 0x1FF4, PDF_TEXT_UCD_WBP_ALetter  }, /* 502 */
  { 0x1FF6, 0x1FFC, PDF_TEXT_UCD_WBP_ALetter  }, /* 503 */
  { 0x2071, 0x2071, PDF_TEXT_UCD_WBP_ALetter  }, /* 504 */
  { 0x207F, 0x207F, PDF_TEXT_UCD_WBP_ALetter  }, /* 505 */
  { 0x2090, 0x2094, PDF_TEXT_UCD_WBP_ALetter  }, /* 506 */
  { 0x2102, 0x2102, PDF_TEXT_UCD_WBP_ALetter  }, /* 507 */
  { 0x2107, 0x2107, PDF_TEXT_UCD_WBP_ALetter  }, /* 508 */
  { 0x210A, 0x2113, PDF_TEXT_UCD_WBP_ALetter  }, /* 509 */
  { 0x2115, 0x2115, PDF_TEXT_UCD_WBP_ALetter  }, /* 510 */
  { 0x2119, 0x211D, PDF_TEXT_UCD_WBP_ALetter  }, /* 511 */
  { 0x2124, 0x2124, PDF_TEXT_UCD_WBP_ALetter  }, /* 512 */
  { 0x2126, 0x2126, PDF_TEXT_UCD_WBP_ALetter  }, /* 513 */
  { 0x2128, 0x2128, PDF_TEXT_UCD_WBP_ALetter  }, /* 514 */
  { 0x212A, 0x212D, PDF_TEXT_UCD_WBP_ALetter  }, /* 515 */
  { 0x212F, 0x2134, PDF_TEXT_UCD_WBP_ALetter  }, /* 516 */
  { 0x2135, 0x2138, PDF_TEXT_UCD_WBP_ALetter  }, /* 517 */
  { 0x2139, 0x2139, PDF_TEXT_UCD_WBP_ALetter  }, /* 518 */
  { 0x213C, 0x213F, PDF_TEXT_UCD_WBP_ALetter  }, /* 519 */
  { 0x2145, 0x2149, PDF_TEXT_UCD_WBP_ALetter  }, /* 520 */
  { 0x214E, 0x214E, PDF_TEXT_UCD_WBP_ALetter  }, /* 521 */
  { 0x2160, 0x2182, PDF_TEXT_UCD_WBP_ALetter  }, /* 522 */
  { 0x2183, 0x2184, PDF_TEXT_UCD_WBP_ALetter  }, /* 523 */
  { 0x2185, 0x2188, PDF_TEXT_UCD_WBP_ALetter  }, /* 524 */
  { 0x24B6, 0x24E9, PDF_TEXT_UCD_WBP_ALetter  }, /* 525 */
  { 0x2C00, 0x2C2E, PDF_TEXT_UCD_WBP_ALetter  }, /* 526 */
  { 0x2C30, 0x2C5E, PDF_TEXT_UCD_WBP_ALetter  }, /* 527 */
  { 0x2C60, 0x2C6F, PDF_TEXT_UCD_WBP_ALetter  }, /* 528 */
  { 0x2C71, 0x2C7C, PDF_TEXT_UCD_WBP_ALetter  }, /* 529 */
  { 0x2C7D, 0x2C7D, PDF_TEXT_UCD_WBP_ALetter  }, /* 530 */
  { 0x2C80, 0x2CE4, PDF_TEXT_UCD_WBP_ALetter  }, /* 531 */
  { 0x2D00, 0x2D25, PDF_TEXT_UCD_WBP_ALetter  }, /* 532 */
  { 0x2D30, 0x2D65, PDF_TEXT_UCD_WBP_ALetter  }, /* 533 */
  { 0x2D6F, 0x2D6F, PDF_TEXT_UCD_WBP_ALetter  }, /* 534 */
  { 0x2D80, 0x2D96, PDF_TEXT_UCD_WBP_ALetter  }, /* 535 */
  { 0x2DA0, 0x2DA6, PDF_TEXT_UCD_WBP_ALetter  }, /* 536 */
  { 0x2DA8, 0x2DAE, PDF_TEXT_UCD_WBP_ALetter  }, /* 537 */
  { 0x2DB0, 0x2DB6, PDF_TEXT_UCD_WBP_ALetter  }, /* 538 */
  { 0x2DB8, 0x2DBE, PDF_TEXT_UCD_WBP_ALetter  }, /* 539 */
  { 0x2DC0, 0x2DC6, PDF_TEXT_UCD_WBP_ALetter  }, /* 540 */
  { 0x2DC8, 0x2DCE, PDF_TEXT_UCD_WBP_ALetter  }, /* 541 */
  { 0x2DD0, 0x2DD6, PDF_TEXT_UCD_WBP_ALetter  }, /* 542 */
  { 0x2DD8, 0x2DDE, PDF_TEXT_UCD_WBP_ALetter  }, /* 543 */
  { 0x2E2F, 0x2E2F, PDF_TEXT_UCD_WBP_ALetter  }, /* 544 */
  { 0x3005, 0x3005, PDF_TEXT_UCD_WBP_ALetter  }, /* 545 */
  { 0x303B, 0x303B, PDF_TEXT_UCD_WBP_ALetter  }, /* 546 */
  { 0x303C, 0x303C, PDF_TEXT_UCD_WBP_ALetter  }, /* 547 */
  { 0x3105, 0x312D, PDF_TEXT_UCD_WBP_ALetter  }, /* 548 */
  { 0x3131, 0x318E, PDF_TEXT_UCD_WBP_ALetter  }, /* 549 */
  { 0x31A0, 0x31B7, PDF_TEXT_UCD_WBP_ALetter  }, /* 550 */
  { 0xA000, 0xA014, PDF_TEXT_UCD_WBP_ALetter  }, /* 551 */
  { 0xA015, 0xA015, PDF_TEXT_UCD_WBP_ALetter  }, /* 552 */
  { 0xA016, 0xA48C, PDF_TEXT_UCD_WBP_ALetter  }, /* 553 */
  { 0xA500, 0xA60B, PDF_TEXT_UCD_WBP_ALetter  }, /* 554 */
  { 0xA60C, 0xA60C, PDF_TEXT_UCD_WBP_ALetter  }, /* 555 */
  { 0xA610, 0xA61F, PDF_TEXT_UCD_WBP_ALetter  }, /* 556 */
  { 0xA62A, 0xA62B, PDF_TEXT_UCD_WBP_ALetter  }, /* 557 */
  { 0xA640, 0xA65F, PDF_TEXT_UCD_WBP_ALetter  }, /* 558 */
  { 0xA662, 0xA66D, PDF_TEXT_UCD_WBP_ALetter  }, /* 559 */
  { 0xA66E, 0xA66E, PDF_TEXT_UCD_WBP_ALetter  }, /* 560 */
  { 0xA67F, 0xA67F, PDF_TEXT_UCD_WBP_ALetter  }, /* 561 */
  { 0xA680, 0xA697, PDF_TEXT_UCD_WBP_ALetter  }, /* 562 */
  { 0xA717, 0xA71F, PDF_TEXT_UCD_WBP_ALetter  }, /* 563 */
  { 0xA722, 0xA76F, PDF_TEXT_UCD_WBP_ALetter  }, /* 564 */
  { 0xA770, 0xA770, PDF_TEXT_UCD_WBP_ALetter  }, /* 565 */
  { 0xA771, 0xA787, PDF_TEXT_UCD_WBP_ALetter  }, /* 566 */
  { 0xA788, 0xA788, PDF_TEXT_UCD_WBP_ALetter  }, /* 567 */
  { 0xA78B, 0xA78C, PDF_TEXT_UCD_WBP_ALetter  }, /* 568 */
  { 0xA7FB, 0xA801, PDF_TEXT_UCD_WBP_ALetter  }, /* 569 */
  { 0xA803, 0xA805, PDF_TEXT_UCD_WBP_ALetter  }, /* 570 */
  { 0xA807, 0xA80A, PDF_TEXT_UCD_WBP_ALetter  }, /* 571 */
  { 0xA80C, 0xA822, PDF_TEXT_UCD_WBP_ALetter  }, /* 572 */
  { 0xA840, 0xA873, PDF_TEXT_UCD_WBP_ALetter  }, /* 573 */
  { 0xA882, 0xA8B3, PDF_TEXT_UCD_WBP_ALetter  }, /* 574 */
  { 0xA90A, 0xA925, PDF_TEXT_UCD_WBP_ALetter  }, /* 575 */
  { 0xA930, 0xA946, PDF_TEXT_UCD_WBP_ALetter  }, /* 576 */
  { 0xAA00, 0xAA28, PDF_TEXT_UCD_WBP_ALetter  }, /* 577 */
  { 0xAA40, 0xAA42, PDF_TEXT_UCD_WBP_ALetter  }, /* 578 */
  { 0xAA44, 0xAA4B, PDF_TEXT_UCD_WBP_ALetter  }, /* 579 */
  { 0xAC00, 0xD7A3, PDF_TEXT_UCD_WBP_ALetter  }, /* 580 */
  { 0xFB00, 0xFB06, PDF_TEXT_UCD_WBP_ALetter  }, /* 581 */
  { 0xFB13, 0xFB17, PDF_TEXT_UCD_WBP_ALetter  }, /* 582 */
  { 0xFB1D, 0xFB1D, PDF_TEXT_UCD_WBP_ALetter  }, /* 583 */
  { 0xFB1F, 0xFB28, PDF_TEXT_UCD_WBP_ALetter  }, /* 584 */
  { 0xFB2A, 0xFB36, PDF_TEXT_UCD_WBP_ALetter  }, /* 585 */
  { 0xFB38, 0xFB3C, PDF_TEXT_UCD_WBP_ALetter  }, /* 586 */
  { 0xFB3E, 0xFB3E, PDF_TEXT_UCD_WBP_ALetter  }, /* 587 */
  { 0xFB40, 0xFB41, PDF_TEXT_UCD_WBP_ALetter  }, /* 588 */
  { 0xFB43, 0xFB44, PDF_TEXT_UCD_WBP_ALetter  }, /* 589 */
  { 0xFB46, 0xFBB1, PDF_TEXT_UCD_WBP_ALetter  }, /* 590 */
  { 0xFBD3, 0xFD3D, PDF_TEXT_UCD_WBP_ALetter  }, /* 591 */
  { 0xFD50, 0xFD8F, PDF_TEXT_UCD_WBP_ALetter  }, /* 592 */
  { 0xFD92, 0xFDC7, PDF_TEXT_UCD_WBP_ALetter  }, /* 593 */
  { 0xFDF0, 0xFDFB, PDF_TEXT_UCD_WBP_ALetter  }, /* 594 */
  { 0xFE70, 0xFE74, PDF_TEXT_UCD_WBP_ALetter  }, /* 595 */
  { 0xFE76, 0xFEFC, PDF_TEXT_UCD_WBP_ALetter  }, /* 596 */
  { 0xFF21, 0xFF3A, PDF_TEXT_UCD_WBP_ALetter  }, /* 597 */
  { 0xFF41, 0xFF5A, PDF_TEXT_UCD_WBP_ALetter  }, /* 598 */
  { 0xFFA0, 0xFFBE, PDF_TEXT_UCD_WBP_ALetter  }, /* 599 */
  { 0xFFC2, 0xFFC7, PDF_TEXT_UCD_WBP_ALetter  }, /* 600 */
  { 0xFFCA, 0xFFCF, PDF_TEXT_UCD_WBP_ALetter  }, /* 601 */
  { 0xFFD2, 0xFFD7, PDF_TEXT_UCD_WBP_ALetter  }, /* 602 */
  { 0xFFDA, 0xFFDC, PDF_TEXT_UCD_WBP_ALetter  }, /* 603 */
  { 0x10000, 0x1000B, PDF_TEXT_UCD_WBP_ALetter  }, /* 604 */
  { 0x1000D, 0x10026, PDF_TEXT_UCD_WBP_ALetter  }, /* 605 */
  { 0x10028, 0x1003A, PDF_TEXT_UCD_WBP_ALetter  }, /* 606 */
  { 0x1003C, 0x1003D, PDF_TEXT_UCD_WBP_ALetter  }, /* 607 */
  { 0x1003F, 0x1004D, PDF_TEXT_UCD_WBP_ALetter  }, /* 608 */
  { 0x10050, 0x1005D, PDF_TEXT_UCD_WBP_ALetter  }, /* 609 */
  { 0x10080, 0x100FA, PDF_TEXT_UCD_WBP_ALetter  }, /* 610 */
  { 0x10140, 0x10174, PDF_TEXT_UCD_WBP_ALetter  }, /* 611 */
  { 0x10280, 0x1029C, PDF_TEXT_UCD_WBP_ALetter  }, /* 612 */
  { 0x102A0, 0x102D0, PDF_TEXT_UCD_WBP_ALetter  }, /* 613 */
  { 0x10300, 0x1031E, PDF_TEXT_UCD_WBP_ALetter  }, /* 614 */
  { 0x10330, 0x10340, PDF_TEXT_UCD_WBP_ALetter  }, /* 615 */
  { 0x10341, 0x10341, PDF_TEXT_UCD_WBP_ALetter  }, /* 616 */
  { 0x10342, 0x10349, PDF_TEXT_UCD_WBP_ALetter  }, /* 617 */
  { 0x1034A, 0x1034A, PDF_TEXT_UCD_WBP_ALetter  }, /* 618 */
  { 0x10380, 0x1039D, PDF_TEXT_UCD_WBP_ALetter  }, /* 619 */
  { 0x103A0, 0x103C3, PDF_TEXT_UCD_WBP_ALetter  }, /* 620 */
  { 0x103C8, 0x103CF, PDF_TEXT_UCD_WBP_ALetter  }, /* 621 */
  { 0x103D1, 0x103D5, PDF_TEXT_UCD_WBP_ALetter  }, /* 622 */
  { 0x10400, 0x1044F, PDF_TEXT_UCD_WBP_ALetter  }, /* 623 */
  { 0x10450, 0x1049D, PDF_TEXT_UCD_WBP_ALetter  }, /* 624 */
  { 0x10800, 0x10805, PDF_TEXT_UCD_WBP_ALetter  }, /* 625 */
  { 0x10808, 0x10808, PDF_TEXT_UCD_WBP_ALetter  }, /* 626 */
  { 0x1080A, 0x10835, PDF_TEXT_UCD_WBP_ALetter  }, /* 627 */
  { 0x10837, 0x10838, PDF_TEXT_UCD_WBP_ALetter  }, /* 628 */
  { 0x1083C, 0x1083C, PDF_TEXT_UCD_WBP_ALetter  }, /* 629 */
  { 0x1083F, 0x1083F, PDF_TEXT_UCD_WBP_ALetter  }, /* 630 */
  { 0x10900, 0x10915, PDF_TEXT_UCD_WBP_ALetter  }, /* 631 */
  { 0x10920, 0x10939, PDF_TEXT_UCD_WBP_ALetter  }, /* 632 */
  { 0x10A00, 0x10A00, PDF_TEXT_UCD_WBP_ALetter  }, /* 633 */
  { 0x10A10, 0x10A13, PDF_TEXT_UCD_WBP_ALetter  }, /* 634 */
  { 0x10A15, 0x10A17, PDF_TEXT_UCD_WBP_ALetter  }, /* 635 */
  { 0x10A19, 0x10A33, PDF_TEXT_UCD_WBP_ALetter  }, /* 636 */
  { 0x12000, 0x1236E, PDF_TEXT_UCD_WBP_ALetter  }, /* 637 */
  { 0x12400, 0x12462, PDF_TEXT_UCD_WBP_ALetter  }, /* 638 */
  { 0x1D400, 0x1D454, PDF_TEXT_UCD_WBP_ALetter  }, /* 639 */
  { 0x1D456, 0x1D49C, PDF_TEXT_UCD_WBP_ALetter  }, /* 640 */
  { 0x1D49E, 0x1D49F, PDF_TEXT_UCD_WBP_ALetter  }, /* 641 */
  { 0x1D4A2, 0x1D4A2, PDF_TEXT_UCD_WBP_ALetter  }, /* 642 */
  { 0x1D4A5, 0x1D4A6, PDF_TEXT_UCD_WBP_ALetter  }, /* 643 */
  { 0x1D4A9, 0x1D4AC, PDF_TEXT_UCD_WBP_ALetter  }, /* 644 */
  { 0x1D4AE, 0x1D4B9, PDF_TEXT_UCD_WBP_ALetter  }, /* 645 */
  { 0x1D4BB, 0x1D4BB, PDF_TEXT_UCD_WBP_ALetter  }, /* 646 */
  { 0x1D4BD, 0x1D4C3, PDF_TEXT_UCD_WBP_ALetter  }, /* 647 */
  { 0x1D4C5, 0x1D505, PDF_TEXT_UCD_WBP_ALetter  }, /* 648 */
  { 0x1D507, 0x1D50A, PDF_TEXT_UCD_WBP_ALetter  }, /* 649 */
  { 0x1D50D, 0x1D514, PDF_TEXT_UCD_WBP_ALetter  }, /* 650 */
  { 0x1D516, 0x1D51C, PDF_TEXT_UCD_WBP_ALetter  }, /* 651 */
  { 0x1D51E, 0x1D539, PDF_TEXT_UCD_WBP_ALetter  }, /* 652 */
  { 0x1D53B, 0x1D53E, PDF_TEXT_UCD_WBP_ALetter  }, /* 653 */
  { 0x1D540, 0x1D544, PDF_TEXT_UCD_WBP_ALetter  }, /* 654 */
  { 0x1D546, 0x1D546, PDF_TEXT_UCD_WBP_ALetter  }, /* 655 */
  { 0x1D54A, 0x1D550, PDF_TEXT_UCD_WBP_ALetter  }, /* 656 */
  { 0x1D552, 0x1D6A5, PDF_TEXT_UCD_WBP_ALetter  }, /* 657 */
  { 0x1D6A8, 0x1D6C0, PDF_TEXT_UCD_WBP_ALetter  }, /* 658 */
  { 0x1D6C2, 0x1D6DA, PDF_TEXT_UCD_WBP_ALetter  }, /* 659 */
  { 0x1D6DC, 0x1D6FA, PDF_TEXT_UCD_WBP_ALetter  }, /* 660 */
  { 0x1D6FC, 0x1D714, PDF_TEXT_UCD_WBP_ALetter  }, /* 661 */
  { 0x1D716, 0x1D734, PDF_TEXT_UCD_WBP_ALetter  }, /* 662 */
  { 0x1D736, 0x1D74E, PDF_TEXT_UCD_WBP_ALetter  }, /* 663 */
  { 0x1D750, 0x1D76E, PDF_TEXT_UCD_WBP_ALetter  }, /* 664 */
  { 0x1D770, 0x1D788, PDF_TEXT_UCD_WBP_ALetter  }, /* 665 */
  { 0x1D78A, 0x1D7A8, PDF_TEXT_UCD_WBP_ALetter  }, /* 666 */
  { 0x1D7AA, 0x1D7C2, PDF_TEXT_UCD_WBP_ALetter  }, /* 667 */
  { 0x1D7C4, 0x1D7CB, PDF_TEXT_UCD_WBP_ALetter  }, /* 668 */
  { 0x003A, 0x003A, PDF_TEXT_UCD_WBP_MidLetter  }, /* 669 */
  { 0x00B7, 0x00B7, PDF_TEXT_UCD_WBP_MidLetter  }, /* 670 */
  { 0x0387, 0x0387, PDF_TEXT_UCD_WBP_MidLetter  }, /* 671 */
  { 0x05F4, 0x05F4, PDF_TEXT_UCD_WBP_MidLetter  }, /* 672 */
  { 0x2027, 0x2027, PDF_TEXT_UCD_WBP_MidLetter  }, /* 673 */
  { 0xFE13, 0xFE13, PDF_TEXT_UCD_WBP_MidLetter  }, /* 674 */
  { 0xFE55, 0xFE55, PDF_TEXT_UCD_WBP_MidLetter  }, /* 675 */
  { 0xFF1A, 0xFF1A, PDF_TEXT_UCD_WBP_MidLetter  }, /* 676 */
  { 0x002C, 0x002C, PDF_TEXT_UCD_WBP_MidNum  }, /* 677 */
  { 0x003B, 0x003B, PDF_TEXT_UCD_WBP_MidNum  }, /* 678 */
  { 0x037E, 0x037E, PDF_TEXT_UCD_WBP_MidNum  }, /* 679 */
  { 0x0589, 0x0589, PDF_TEXT_UCD_WBP_MidNum  }, /* 680 */
  { 0x060C, 0x060D, PDF_TEXT_UCD_WBP_MidNum  }, /* 681 */
  { 0x066C, 0x066C, PDF_TEXT_UCD_WBP_MidNum  }, /* 682 */
  { 0x07F8, 0x07F8, PDF_TEXT_UCD_WBP_MidNum  }, /* 683 */
  { 0x2044, 0x2044, PDF_TEXT_UCD_WBP_MidNum  }, /* 684 */
  { 0xFE10, 0xFE10, PDF_TEXT_UCD_WBP_MidNum  }, /* 685 */
  { 0xFE14, 0xFE14, PDF_TEXT_UCD_WBP_MidNum  }, /* 686 */
  { 0xFE50, 0xFE50, PDF_TEXT_UCD_WBP_MidNum  }, /* 687 */
  { 0xFE54, 0xFE54, PDF_TEXT_UCD_WBP_MidNum  }, /* 688 */
  { 0xFF0C, 0xFF0C, PDF_TEXT_UCD_WBP_MidNum  }, /* 689 */
  { 0xFF1B, 0xFF1B, PDF_TEXT_UCD_WBP_MidNum  }, /* 690 */
  { 0x0027, 0x0027, PDF_TEXT_UCD_WBP_MidNumLet  }, /* 691 */
  { 0x002E, 0x002E, PDF_TEXT_UCD_WBP_MidNumLet  }, /* 692 */
  { 0x2018, 0x2018, PDF_TEXT_UCD_WBP_MidNumLet  }, /* 693 */
  { 0x2019, 0x2019, PDF_TEXT_UCD_WBP_MidNumLet  }, /* 694 */
  { 0x2024, 0x2024, PDF_TEXT_UCD_WBP_MidNumLet  }, /* 695 */
  { 0xFE52, 0xFE52, PDF_TEXT_UCD_WBP_MidNumLet  }, /* 696 */
  { 0xFF07, 0xFF07, PDF_TEXT_UCD_WBP_MidNumLet  }, /* 697 */
  { 0xFF0E, 0xFF0E, PDF_TEXT_UCD_WBP_MidNumLet  }, /* 698 */
  { 0x0030, 0x0039, PDF_TEXT_UCD_WBP_Numeric  }, /* 699 */
  { 0x0660, 0x0669, PDF_TEXT_UCD_WBP_Numeric  }, /* 700 */
  { 0x066B, 0x066B, PDF_TEXT_UCD_WBP_Numeric  }, /* 701 */
  { 0x06F0, 0x06F9, PDF_TEXT_UCD_WBP_Numeric  }, /* 702 */
  { 0x07C0, 0x07C9, PDF_TEXT_UCD_WBP_Numeric  }, /* 703 */
  { 0x0966, 0x096F, PDF_TEXT_UCD_WBP_Numeric  }, /* 704 */
  { 0x09E6, 0x09EF, PDF_TEXT_UCD_WBP_Numeric  }, /* 705 */
  { 0x0A66, 0x0A6F, PDF_TEXT_UCD_WBP_Numeric  }, /* 706 */
  { 0x0AE6, 0x0AEF, PDF_TEXT_UCD_WBP_Numeric  }, /* 707 */
  { 0x0B66, 0x0B6F, PDF_TEXT_UCD_WBP_Numeric  }, /* 708 */
  { 0x0BE6, 0x0BEF, PDF_TEXT_UCD_WBP_Numeric  }, /* 709 */
  { 0x0C66, 0x0C6F, PDF_TEXT_UCD_WBP_Numeric  }, /* 710 */
  { 0x0CE6, 0x0CEF, PDF_TEXT_UCD_WBP_Numeric  }, /* 711 */
  { 0x0D66, 0x0D6F, PDF_TEXT_UCD_WBP_Numeric  }, /* 712 */
  { 0x0E50, 0x0E59, PDF_TEXT_UCD_WBP_Numeric  }, /* 713 */
  { 0x0ED0, 0x0ED9, PDF_TEXT_UCD_WBP_Numeric  }, /* 714 */
  { 0x0F20, 0x0F29, PDF_TEXT_UCD_WBP_Numeric  }, /* 715 */
  { 0x1040, 0x1049, PDF_TEXT_UCD_WBP_Numeric  }, /* 716 */
  { 0x1090, 0x1099, PDF_TEXT_UCD_WBP_Numeric  }, /* 717 */
  { 0x17E0, 0x17E9, PDF_TEXT_UCD_WBP_Numeric  }, /* 718 */
  { 0x1810, 0x1819, PDF_TEXT_UCD_WBP_Numeric  }, /* 719 */
  { 0x1946, 0x194F, PDF_TEXT_UCD_WBP_Numeric  }, /* 720 */
  { 0x19D0, 0x19D9, PDF_TEXT_UCD_WBP_Numeric  }, /* 721 */
  { 0x1B50, 0x1B59, PDF_TEXT_UCD_WBP_Numeric  }, /* 722 */
  { 0x1BB0, 0x1BB9, PDF_TEXT_UCD_WBP_Numeric  }, /* 723 */
  { 0x1C40, 0x1C49, PDF_TEXT_UCD_WBP_Numeric  }, /* 724 */
  { 0x1C50, 0x1C59, PDF_TEXT_UCD_WBP_Numeric  }, /* 725 */
  { 0xA620, 0xA629, PDF_TEXT_UCD_WBP_Numeric  }, /* 726 */
  { 0xA8D0, 0xA8D9, PDF_TEXT_UCD_WBP_Numeric  }, /* 727 */
  { 0xA900, 0xA909, PDF_TEXT_UCD_WBP_Numeric  }, /* 728 */
  { 0xAA50, 0xAA59, PDF_TEXT_UCD_WBP_Numeric  }, /* 729 */
  { 0x104A0, 0x104A9, PDF_TEXT_UCD_WBP_Numeric  }, /* 730 */
  { 0x1D7CE, 0x1D7FF, PDF_TEXT_UCD_WBP_Numeric  }, /* 731 */
  { 0x005F, 0x005F, PDF_TEXT_UCD_WBP_ExtendNumLet  }, /* 732 */
  { 0x203F, 0x2040, PDF_TEXT_UCD_WBP_ExtendNumLet  }, /* 733 */
  { 0x2054, 0x2054, PDF_TEXT_UCD_WBP_ExtendNumLet  }, /* 734 */
  { 0xFE33, 0xFE34, PDF_TEXT_UCD_WBP_ExtendNumLet  }, /* 735 */
  { 0xFE4D, 0xFE4F, PDF_TEXT_UCD_WBP_ExtendNumLet  }, /* 736 */
  { 0xFF3F, 0xFF3F, PDF_TEXT_UCD_WBP_ExtendNumLet  }, /* 737 */
};

/***************** END OF SELF-GENERATED DATA *********************************/

static pdf_bool_t
pdf_text_ucd_wb_in_interval (pdf_u32_t character,
                             pdf_u32_t first_interval,
                             pdf_u32_t last_interval)
{
  int i;

  for (i=first_interval; i<=last_interval; ++i)
    {
      if ((character >= unicode_wordbreak_info[i].interval_start) &&
          (character <= unicode_wordbreak_info[i].interval_stop))
        {
          return PDF_TRUE;
        }
    }
  return PDF_FALSE;
}

/* Returns true if the given UTF-32HE unicode point has the CR value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_cr (pdf_u32_t character)
{
  return pdf_text_ucd_wb_in_interval (character,
                                      UCD_WB_CR_F,
                                      UCD_WB_CR_L);
}

/* Returns true if the given UTF-32HE unicode point has the LF value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_lf (pdf_u32_t character)
{
  return pdf_text_ucd_wb_in_interval (character,
                                      UCD_WB_LF_F,
                                      UCD_WB_LF_L);
}

/* Returns true if the given UTF-32HE unicode point has the Newline value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_newline (pdf_u32_t character)
{
  return pdf_text_ucd_wb_in_interval (character,
                                      UCD_WB_NEWLINE_F,
                                      UCD_WB_NEWLINE_L);
}

/* Returns true if the given UTF-32HE unicode point has the Extend value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_extend (pdf_u32_t character)
{
  return pdf_text_ucd_wb_in_interval (character,
                                      UCD_WB_EXTEND_F,
                                      UCD_WB_EXTEND_L);
}

/* Returns true if the given UTF-32HE unicode point has the Format value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_format (pdf_u32_t character)
{
  return pdf_text_ucd_wb_in_interval (character,
                                      UCD_WB_FORMAT_F,
                                      UCD_WB_FORMAT_L);
}

/* Returns true if the given UTF-32HE unicode point has the Katakana value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_katakana (pdf_u32_t character)
{
  return pdf_text_ucd_wb_in_interval (character,
                                      UCD_WB_KATAKANA_F,
                                      UCD_WB_KATAKANA_L);
}

/* Returns true if the given UTF-32HE unicode point has the ALetter value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_aletter (pdf_u32_t character)
{
  return pdf_text_ucd_wb_in_interval (character,
                                      UCD_WB_ALETTER_F,
                                      UCD_WB_ALETTER_L);
}

/* Returns true if the given UTF-32HE unicode point has the MidLetter value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_midletter (pdf_u32_t character)
{
  return pdf_text_ucd_wb_in_interval (character,
                                      UCD_WB_MIDLETTER_F,
                                      UCD_WB_MIDLETTER_L);
}

/* Returns true if the given UTF-32HE unicode point has the MidNum value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_midnum (pdf_u32_t character)
{
  return pdf_text_ucd_wb_in_interval (character,
                                      UCD_WB_MIDNUM_F,
                                      UCD_WB_MIDNUM_L);
}

/* Returns true if the given UTF-32HE unicode point has the MidNumLet value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_midnumlet (pdf_u32_t character)
{
  return pdf_text_ucd_wb_in_interval (character,
                                      UCD_WB_MIDNUMLET_F,
                                      UCD_WB_MIDNUMLET_L);
}

/* Returns true if the given UTF-32HE unicode point has the Numeric value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_numeric (pdf_u32_t character)
{
  return pdf_text_ucd_wb_in_interval (character,
                                      UCD_WB_NUMERIC_F,
                                      UCD_WB_NUMERIC_L);
}

/* Returns true if the given UTF-32HE unicode point has the ExtendNumLet value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_extendnumlet (pdf_u32_t character)
{
  return pdf_text_ucd_wb_in_interval (character,
                                      UCD_WB_EXTENDNUMLET_F,
                                      UCD_WB_EXTENDNUMLET_L);
}


enum pdf_text_ucd_wb_property_e
pdf_text_ucd_wb_get_property (pdf_u32_t character)
{
  if (pdf_text_ucd_wb_is_aletter (character))
    return PDF_TEXT_UCD_WBP_ALetter;
  if (pdf_text_ucd_wb_is_midletter (character))
    return PDF_TEXT_UCD_WBP_MidLetter;
  if (pdf_text_ucd_wb_is_numeric (character))
    return PDF_TEXT_UCD_WBP_Numeric;
  if (pdf_text_ucd_wb_is_midnum (character))
    return PDF_TEXT_UCD_WBP_MidNum;
  if (pdf_text_ucd_wb_is_midnumlet (character))
    return PDF_TEXT_UCD_WBP_MidNumLet;
  if (pdf_text_ucd_wb_is_format (character))
    return PDF_TEXT_UCD_WBP_Format;
  if (pdf_text_ucd_wb_is_cr (character))
    return PDF_TEXT_UCD_WBP_CR;
  if (pdf_text_ucd_wb_is_lf (character))
    return PDF_TEXT_UCD_WBP_LF;
  if (pdf_text_ucd_wb_is_newline (character))
    return PDF_TEXT_UCD_WBP_Newline;
  if (pdf_text_ucd_wb_is_extend (character))
    return PDF_TEXT_UCD_WBP_Extend;
  if (pdf_text_ucd_wb_is_katakana (character))
    return PDF_TEXT_UCD_WBP_Katakana;
  if (pdf_text_ucd_wb_is_extendnumlet (character))
    return PDF_TEXT_UCD_WBP_ExtendNumLet;
  return PDF_TEXT_UCD_WBP_None;
}

/* Maximum number of code points needed for a word break check */
#define PDF_TEXT_UCD_MWBCP  4

/* Word break property information */
typedef struct pdf_text_ucd_wb_s {
  pdf_char_t *walker;
  pdf_u32_t utf32val;
  enum pdf_text_ucd_wb_property_e wbp;
} pdf_text_ucd_wb_t;

/* RULE WB3: Do not break within CRLF (CR x LF) */
static pdf_bool_t
pdf_text_ucd_wb_rule_3 (const pdf_text_ucd_wb_t buffer[PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[1].wbp == PDF_TEXT_UCD_WBP_CR) &&
          (buffer[2].wbp == PDF_TEXT_UCD_WBP_LF)) ?
          PDF_TRUE : PDF_FALSE);
}

/* RULE WB3a: Break before and after Newlines (including CR and LF) */
static pdf_bool_t
pdf_text_ucd_wb_rule_3a (const pdf_text_ucd_wb_t buffer[PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[1].wbp == PDF_TEXT_UCD_WBP_CR) ||
           (buffer[1].wbp == PDF_TEXT_UCD_WBP_LF) ||
           (buffer[1].wbp == PDF_TEXT_UCD_WBP_Newline)) ?
          PDF_TRUE : PDF_FALSE);
}

/* RULE WB3b: Break before and after Newlines (including CR and LF) */
static pdf_bool_t
pdf_text_ucd_wb_rule_3b (const pdf_text_ucd_wb_t buffer[PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[2].wbp == PDF_TEXT_UCD_WBP_CR) ||
           (buffer[2].wbp == PDF_TEXT_UCD_WBP_LF) ||
           (buffer[2].wbp == PDF_TEXT_UCD_WBP_Newline)) ?
          PDF_TRUE : PDF_FALSE);
}

/* RULE WB4: Ignore Format and Extend characters, except when they appear at
 *  the beginning of a region of text. */
static pdf_bool_t
pdf_text_ucd_wb_rule_4 (const pdf_text_ucd_wb_t buffer[PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[2].wbp == PDF_TEXT_UCD_WBP_Extend) ||
           (buffer[2].wbp == PDF_TEXT_UCD_WBP_Format)) ?
          PDF_TRUE : PDF_FALSE);
}

/* RULE WB5: Do not break between most letters (ALetter X ALetter) */
static pdf_bool_t
pdf_text_ucd_wb_rule_5 (const pdf_text_ucd_wb_t buffer[PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[1].wbp == PDF_TEXT_UCD_WBP_ALetter) &&
           (buffer[2].wbp == PDF_TEXT_UCD_WBP_ALetter)) ?
          PDF_TRUE : PDF_FALSE);
}

/* RULE WB6: Do not break letters across certain puntuation
 *  (ALetter X MidLetter ALetter) */
static pdf_bool_t
pdf_text_ucd_wb_rule_6 (const pdf_text_ucd_wb_t buffer[PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[1].wbp == PDF_TEXT_UCD_WBP_ALetter) &&
           (buffer[2].wbp == PDF_TEXT_UCD_WBP_MidLetter) &&
           (buffer[3].wbp == PDF_TEXT_UCD_WBP_ALetter)) ?
          PDF_TRUE : PDF_FALSE);
}

/* RULE WB7: Do not break letters across certain punctuation
 *  (ALetter MidLetter X ALetter) */
static pdf_bool_t
pdf_text_ucd_wb_rule_7 (const pdf_text_ucd_wb_t buffer[PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[0].wbp == PDF_TEXT_UCD_WBP_ALetter) &&
           (buffer[1].wbp == PDF_TEXT_UCD_WBP_MidLetter) &&
           (buffer[2].wbp == PDF_TEXT_UCD_WBP_ALetter)) ?
          PDF_TRUE : PDF_FALSE);
}

/* RULE WB8: Do not break within sequences of digits, or digits adjacent
 *  to letters (Numeric X Numeric) */
static pdf_bool_t
pdf_text_ucd_wb_rule_8 (const pdf_text_ucd_wb_t buffer[PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[1].wbp == PDF_TEXT_UCD_WBP_Numeric) &&
           (buffer[2].wbp == PDF_TEXT_UCD_WBP_Numeric)) ?
          PDF_TRUE : PDF_FALSE);
}

/* RULE WB9: Do not break within sequences of digits, or digits adjacent
 *  to letters (ALetter X Numeric) */
static pdf_bool_t
pdf_text_ucd_wb_rule_9 (const pdf_text_ucd_wb_t buffer[PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[1].wbp == PDF_TEXT_UCD_WBP_ALetter) &&
           (buffer[2].wbp == PDF_TEXT_UCD_WBP_Numeric)) ?
          PDF_TRUE : PDF_FALSE);
}

/* RULE WB10: Do not break within sequences of digits, or digits adjacent
 *  to letters (Numeric X ALetter) */
static pdf_bool_t
pdf_text_ucd_wb_rule_10 (const pdf_text_ucd_wb_t buffer[PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[0].wbp == PDF_TEXT_UCD_WBP_Numeric) &&
           (buffer[1].wbp == PDF_TEXT_UCD_WBP_ALetter)) ? PDF_TRUE : PDF_FALSE);
}

/* RULE WB11: Do not break within sequences such as "3.2"
 *  (Numeric MidNum X Numeric) */
static pdf_bool_t
pdf_text_ucd_wb_rule_11 (const pdf_text_ucd_wb_t buffer[PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[0].wbp == PDF_TEXT_UCD_WBP_Numeric) &&
           (buffer[1].wbp == PDF_TEXT_UCD_WBP_MidNum) &&
           (buffer[2].wbp == PDF_TEXT_UCD_WBP_Numeric)) ?
          PDF_TRUE : PDF_FALSE);
}

/* RULE WB12: Do not break within sequences such as "3.2"
 *  (Numeric X MidNum Numeric) */
static pdf_bool_t
pdf_text_ucd_wb_rule_12 (const pdf_text_ucd_wb_t buffer[PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[1].wbp == PDF_TEXT_UCD_WBP_Numeric) &&
           (buffer[2].wbp == PDF_TEXT_UCD_WBP_MidNum) &&
           (buffer[3].wbp == PDF_TEXT_UCD_WBP_Numeric)) ?
          PDF_TRUE : PDF_FALSE);
}

/* RULE WB13: Do not break between Katakana */
static pdf_bool_t
pdf_text_ucd_wb_rule_13 (const pdf_text_ucd_wb_t buffer[PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[1].wbp == PDF_TEXT_UCD_WBP_Katakana) &&
           (buffer[2].wbp == PDF_TEXT_UCD_WBP_Katakana)) ?
          PDF_TRUE :PDF_FALSE);
}

/* RULE WB13a: Do not break from extenders
 *  ((ALetter | Numeric | Katakana | ExtendNumLet) X ExtendNumLet) */
static pdf_bool_t
pdf_text_ucd_wb_rule_13a (const pdf_text_ucd_wb_t buffer[PDF_TEXT_UCD_MWBCP])
{
  return ((((buffer[1].wbp == PDF_TEXT_UCD_WBP_ALetter) ||
            (buffer[1].wbp == PDF_TEXT_UCD_WBP_Numeric) ||
            (buffer[1].wbp == PDF_TEXT_UCD_WBP_Katakana) ||
            (buffer[1].wbp == PDF_TEXT_UCD_WBP_ExtendNumLet)) &&
           (buffer[2].wbp == PDF_TEXT_UCD_WBP_ExtendNumLet)) ?
          PDF_TRUE : PDF_FALSE);
}

/* RULE WB13b: Do not break from extenders
 *  (ExtendNumLet) X (ALetter | Numeric | Katakana ) */
static pdf_bool_t
pdf_text_ucd_wb_rule_13b (const pdf_text_ucd_wb_t buffer[PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[1].wbp == PDF_TEXT_UCD_WBP_ExtendNumLet) &&
           ((buffer[2].wbp == PDF_TEXT_UCD_WBP_ALetter) ||
            (buffer[2].wbp == PDF_TEXT_UCD_WBP_Numeric) ||
            (buffer[2].wbp == PDF_TEXT_UCD_WBP_Katakana))) ?
          PDF_TRUE : PDF_FALSE);
}

/* Check rules and stop if any of them is true (meaning that shouldn't be a
 *  word break) */
static pdf_bool_t
pdf_text_ucd_wb_check_rules (const pdf_text_ucd_wb_t buffer[PDF_TEXT_UCD_MWBCP])
{
  return (((pdf_text_ucd_wb_rule_3(buffer)) ||
           (pdf_text_ucd_wb_rule_3a(buffer)) ||
           (pdf_text_ucd_wb_rule_3b(buffer)) ||
           (pdf_text_ucd_wb_rule_4(buffer)) ||
           (pdf_text_ucd_wb_rule_5(buffer)) ||
           (pdf_text_ucd_wb_rule_6(buffer)) ||
           (pdf_text_ucd_wb_rule_7(buffer)) ||
           (pdf_text_ucd_wb_rule_8(buffer)) ||
           (pdf_text_ucd_wb_rule_9(buffer)) ||
           (pdf_text_ucd_wb_rule_10(buffer)) ||
           (pdf_text_ucd_wb_rule_11(buffer)) ||
           (pdf_text_ucd_wb_rule_12(buffer)) ||
           (pdf_text_ucd_wb_rule_13(buffer)) ||
           (pdf_text_ucd_wb_rule_13a(buffer)) ||
           (pdf_text_ucd_wb_rule_13b(buffer))) ?
          PDF_TRUE : PDF_FALSE);
}

/* Word boundary search algorithm, based on Unicode Standard Annex #29
 *  "Text Boundaries".
 * - `current' points to the next byte after the word break (so it points to the
 *    FIRST byte of the word).
 * - `next' will point to the previous byte before the next word break (so it
 *    points to the LAST byte of the word).
 */
pdf_bool_t
pdf_text_ucd_wb_detect_next (const pdf_char_t  *current,
                             const pdf_size_t   n_bytes_left_in,
                             pdf_char_t       **next,
                             pdf_size_t        *n_bytes_left_out)
{
  /* Buffer to store the unicode points as they are being parsed in the
   *  algorithm. Indexes are treated as follows:
   *     [0] [1] x [2] [3]
   *  This means that possible word breaks are ALWAYS considered between code
   *   points [1] and [2], being [0] the previous character to [1] and being
   *   [3] the next character to [2].
   *  When the buffer is updated, the code points are moved one position to the
   *   the left, so that code point in [0] disappears and a new code point
   *   enters in [3], and the word break is again checked between [1] and [2].
   */
  pdf_text_ucd_wb_t buffer[PDF_TEXT_UCD_MWBCP];

  pdf_u32_t i;
  pdf_size_t n_bytes;
  pdf_bool_t found;

  /* Check validity of input number of bytes */
  if (n_bytes_left_in % 4 != 0)
    return PDF_FALSE;

  /* Check if the string is just one character long */
  if (n_bytes_left_in == 4)
    {
      *n_bytes_left_out = 0;
      *next = (pdf_char_t *)current;
      return PDF_TRUE;
    }

  /* Initialize buffer with first 3 unicode points, stored in [1],[2],[3] */
  for (i = 0; i < PDF_TEXT_UCD_MWBCP; ++i)
    {
      if ((i > 0) &&
          (n_bytes_left_in >= (4*i)))
        {
          /* Store pointer */
          buffer[i].walker = (pdf_char_t *)(&current[4*(i-1)]);
          /* Store unsigned 32-bit number */
          memcpy (&(buffer[i].utf32val), buffer[i].walker, 4);
          /* Get Word-Break property value from character */
          buffer[i].wbp = pdf_text_ucd_wb_get_property(buffer[i].utf32val);
        }
      else
        {
          buffer[i].walker = NULL;
          buffer[i].utf32val = 0x0;
          buffer[i].wbp = PDF_TEXT_UCD_WBP_None;
        }
    }

  n_bytes = n_bytes_left_in;
  found = 0;

  /* Start walking the unicode points. At each loop at least 2 unicode points
   *  (8 bytes) must be available to check the word break!!!! */
  while ((!found) &&
         (n_bytes >= 8))
    {
      /* If any of the rules returns true, don't break word */
      if (pdf_text_ucd_wb_check_rules (buffer))
        {
          /* If word break is not found, continue with next UTF-32 point */
          /* Update number of bytes pending */
          n_bytes -= 4;

          /* Shift left contents of the buffer */
          for (i = 1; i < PDF_TEXT_UCD_MWBCP; ++i)
            {
              buffer[i-1] = buffer[i];
            }

          /* Insert new buffer element in position [3], if available */
          if (n_bytes >= 12)
            {
              buffer[3].walker = buffer[2].walker + 4;
              /* Store unsigned 32-bit number */
              memcpy (&(buffer[3].utf32val), buffer[3].walker, 4);
              /* Get Word-Break property value from character */
              buffer[3].wbp = pdf_text_ucd_wb_get_property (buffer[3].utf32val);
            }
        }
      else
        {
          /* RULE WB14: Otherwise, break everywhere (including around
           * ideographs) */
          found = 1;
        }
    }

  /* The exit of the loop could be due to two different reasons:
   *  1. A word break was found in the loop. If it is found, the contents of
   *    the buffer remain unchanged, so the word break is between [1] and [2],
   *    and n_bytes considers the bytes of [1]
   *  2. RULE WB2: Break at end of text ( % EOT). In this case, n_bytes will be
   *    equal to 4, and the contents of the buffer would have been shifted left
   *    so that the last character is pointed by [1].
   *  So, perfect, both cases can be handled in the same way.
   */

  *next = buffer[1].walker;
  *n_bytes_left_out = n_bytes - 4;

  return PDF_TRUE;
}

/* End of pdf-text-ucd-wordbreak.c */

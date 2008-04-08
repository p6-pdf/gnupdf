/* -*- mode: C -*- Time-stamp: ""
 *
 *       File:         pdf-text-ucd-proplist.c
 *       Date:         Mon Mar 31 19:58:17 2008
 *
 *       GNU PDF Library - Unicode Character Database - Propertly List
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


#include <pdf-text-ucd-proplist.h>

/* Indexes of intervals in array */
#define UCD_PL_WHITE_SPACE_F              0
#define UCD_PL_WHITE_SPACE_L              11
#define UCD_PL_BIDI_CONTROL_F             12
#define UCD_PL_BIDI_CONTROL_L             13
#define UCD_PL_JOIN_CONTROL_F             14
#define UCD_PL_JOIN_CONTROL_L             14
#define UCD_PL_DASH_F                     15
#define UCD_PL_DASH_L                     31
#define UCD_PL_HYPHEN_F                   32
#define UCD_PL_HYPHEN_L                   41
#define UCD_PL_QUOTATION_MARK_F           42
#define UCD_PL_QUOTATION_MARK_L           68
#define UCD_PL_TERMINAL_PUNCTUATION_F     69
#define UCD_PL_TERMINAL_PUNCTUATION_L     116
#define UCD_PL_OTHER_MATH_F               117
#define UCD_PL_OTHER_MATH_L               220
#define UCD_PL_HEX_DIGIT_F                221
#define UCD_PL_HEX_DIGIT_L                226
#define UCD_PL_ASCII_HEX_DIGIT_F          227
#define UCD_PL_ASCII_HEX_DIGIT_L          229
#define UCD_PL_OTHER_ALPHABETIC_F         230
#define UCD_PL_OTHER_ALPHABETIC_L         379
#define UCD_PL_IDEOGRAPHIC_F              380
#define UCD_PL_IDEOGRAPHIC_L              389
#define UCD_PL_DIACRITIC_F                390
#define UCD_PL_DIACRITIC_L                490
#define UCD_PL_EXTENDER_F                 491
#define UCD_PL_EXTENDER_L                 503
#define UCD_PL_OTHER_LOWERCASE_F          504
#define UCD_PL_OTHER_LOWERCASE_L          514
#define UCD_PL_OTHER_UPPERCASE_F          515
#define UCD_PL_OTHER_UPPERCASE_L          516
#define UCD_PL_NONCHARACTER_CODE_POINT_F  517
#define UCD_PL_NONCHARACTER_CODE_POINT_L  534
#define UCD_PL_OTHER_GRAPHEME_EXTEND_F    535
#define UCD_PL_OTHER_GRAPHEME_EXTEND_L    549
#define UCD_PL_IDS_BINARY_OPERATOR_F      550
#define UCD_PL_IDS_BINARY_OPERATOR_L      551
#define UCD_PL_IDS_TRINARY_OPERATOR_F     552
#define UCD_PL_IDS_TRINARY_OPERATOR_L     552
#define UCD_PL_RADICAL_F                  553
#define UCD_PL_RADICAL_L                  555
#define UCD_PL_UNIFIED_IDEOGRAPH_F        556
#define UCD_PL_UNIFIED_IDEOGRAPH_L        565
#define UCD_PL_OTHER_DEFAULT_IGNORABLE_CODE_POINT_F   566
#define UCD_PL_OTHER_DEFAULT_IGNORABLE_CODE_POINT_L   575
#define UCD_PL_DEPRECATED_F               576
#define UCD_PL_DEPRECATED_L               579
#define UCD_PL_SOFT_DOTTED_F              580
#define UCD_PL_SOFT_DOTTED_L              609
#define UCD_PL_LOGICAL_ORDER_EXCEPTION_F  610
#define UCD_PL_LOGICAL_ORDER_EXCEPTION_L  611
#define UCD_PL_OTHER_ID_START_F           612
#define UCD_PL_OTHER_ID_START_L           614
#define UCD_PL_OTHER_ID_CONTINUE_F        615
#define UCD_PL_OTHER_ID_CONTINUE_L        615
#define UCD_PL_STERM_F                    616
#define UCD_PL_STERM_L                    645
#define UCD_PL_VARIATION_SELECTOR_F       646
#define UCD_PL_VARIATION_SELECTOR_L       648
#define UCD_PL_PATTERN_WHITE_SPACE_F      649
#define UCD_PL_PATTERN_WHITE_SPACE_L      659
#define UCD_PL_PATTERN_SYNTAX_F           660
#define UCD_PL_PATTERN_SYNTAX_L           895
#define UCD_PL_INFO_N   896  /* Maximum number of elements in array    */

typedef struct _unicode_proplist_info_s {
  pdf_u32_t interval_start;
  pdf_u32_t interval_stop;
  enum pdf_text_ucd_proplist_e proplist;
} unicode_proplist_info_t;

static unicode_proplist_info_t unicode_proplist_info[UCD_PL_INFO_N] = {
  { 0x0009, 0x000D, PDF_TEXT_UCD_PL_White_Space  }, /* 0 */
  { 0x0020, 0x0020, PDF_TEXT_UCD_PL_White_Space  }, /* 1 */
  { 0x0085, 0x0085, PDF_TEXT_UCD_PL_White_Space  }, /* 2 */
  { 0x00A0, 0x00A0, PDF_TEXT_UCD_PL_White_Space  }, /* 3 */
  { 0x1680, 0x1680, PDF_TEXT_UCD_PL_White_Space  }, /* 4 */
  { 0x180E, 0x180E, PDF_TEXT_UCD_PL_White_Space  }, /* 5 */
  { 0x2000, 0x200A, PDF_TEXT_UCD_PL_White_Space  }, /* 6 */
  { 0x2028, 0x2028, PDF_TEXT_UCD_PL_White_Space  }, /* 7 */
  { 0x2029, 0x2029, PDF_TEXT_UCD_PL_White_Space  }, /* 8 */
  { 0x202F, 0x202F, PDF_TEXT_UCD_PL_White_Space  }, /* 9 */
  { 0x205F, 0x205F, PDF_TEXT_UCD_PL_White_Space  }, /* 10 */
  { 0x3000, 0x3000, PDF_TEXT_UCD_PL_White_Space  }, /* 11 */
  { 0x200E, 0x200F, PDF_TEXT_UCD_PL_Bidi_Control  }, /* 12 */
  { 0x202A, 0x202E, PDF_TEXT_UCD_PL_Bidi_Control  }, /* 13 */
  { 0x200C, 0x200D, PDF_TEXT_UCD_PL_Join_Control  }, /* 14 */
  { 0x002D, 0x002D, PDF_TEXT_UCD_PL_Dash  }, /* 15 */
  { 0x058A, 0x058A, PDF_TEXT_UCD_PL_Dash  }, /* 16 */
  { 0x05BE, 0x05BE, PDF_TEXT_UCD_PL_Dash  }, /* 17 */
  { 0x1806, 0x1806, PDF_TEXT_UCD_PL_Dash  }, /* 18 */
  { 0x2010, 0x2015, PDF_TEXT_UCD_PL_Dash  }, /* 19 */
  { 0x2053, 0x2053, PDF_TEXT_UCD_PL_Dash  }, /* 20 */
  { 0x207B, 0x207B, PDF_TEXT_UCD_PL_Dash  }, /* 21 */
  { 0x208B, 0x208B, PDF_TEXT_UCD_PL_Dash  }, /* 22 */
  { 0x2212, 0x2212, PDF_TEXT_UCD_PL_Dash  }, /* 23 */
  { 0x2E17, 0x2E17, PDF_TEXT_UCD_PL_Dash  }, /* 24 */
  { 0x301C, 0x301C, PDF_TEXT_UCD_PL_Dash  }, /* 25 */
  { 0x3030, 0x3030, PDF_TEXT_UCD_PL_Dash  }, /* 26 */
  { 0x30A0, 0x30A0, PDF_TEXT_UCD_PL_Dash  }, /* 27 */
  { 0xFE31, 0xFE32, PDF_TEXT_UCD_PL_Dash  }, /* 28 */
  { 0xFE58, 0xFE58, PDF_TEXT_UCD_PL_Dash  }, /* 29 */
  { 0xFE63, 0xFE63, PDF_TEXT_UCD_PL_Dash  }, /* 30 */
  { 0xFF0D, 0xFF0D, PDF_TEXT_UCD_PL_Dash  }, /* 31 */
  { 0x002D, 0x002D, PDF_TEXT_UCD_PL_Hyphen  }, /* 32 */
  { 0x00AD, 0x00AD, PDF_TEXT_UCD_PL_Hyphen  }, /* 33 */
  { 0x058A, 0x058A, PDF_TEXT_UCD_PL_Hyphen  }, /* 34 */
  { 0x1806, 0x1806, PDF_TEXT_UCD_PL_Hyphen  }, /* 35 */
  { 0x2010, 0x2011, PDF_TEXT_UCD_PL_Hyphen  }, /* 36 */
  { 0x2E17, 0x2E17, PDF_TEXT_UCD_PL_Hyphen  }, /* 37 */
  { 0x30FB, 0x30FB, PDF_TEXT_UCD_PL_Hyphen  }, /* 38 */
  { 0xFE63, 0xFE63, PDF_TEXT_UCD_PL_Hyphen  }, /* 39 */
  { 0xFF0D, 0xFF0D, PDF_TEXT_UCD_PL_Hyphen  }, /* 40 */
  { 0xFF65, 0xFF65, PDF_TEXT_UCD_PL_Hyphen  }, /* 41 */
  { 0x0022, 0x0022, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 42 */
  { 0x0027, 0x0027, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 43 */
  { 0x00AB, 0x00AB, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 44 */
  { 0x00BB, 0x00BB, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 45 */
  { 0x2018, 0x2018, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 46 */
  { 0x2019, 0x2019, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 47 */
  { 0x201A, 0x201A, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 48 */
  { 0x201B, 0x201C, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 49 */
  { 0x201D, 0x201D, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 50 */
  { 0x201E, 0x201E, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 51 */
  { 0x201F, 0x201F, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 52 */
  { 0x2039, 0x2039, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 53 */
  { 0x203A, 0x203A, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 54 */
  { 0x300C, 0x300C, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 55 */
  { 0x300D, 0x300D, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 56 */
  { 0x300E, 0x300E, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 57 */
  { 0x300F, 0x300F, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 58 */
  { 0x301D, 0x301D, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 59 */
  { 0x301E, 0x301F, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 60 */
  { 0xFE41, 0xFE41, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 61 */
  { 0xFE42, 0xFE42, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 62 */
  { 0xFE43, 0xFE43, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 63 */
  { 0xFE44, 0xFE44, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 64 */
  { 0xFF02, 0xFF02, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 65 */
  { 0xFF07, 0xFF07, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 66 */
  { 0xFF62, 0xFF62, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 67 */
  { 0xFF63, 0xFF63, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 68 */
  { 0x0021, 0x0021, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 69 */
  { 0x002C, 0x002C, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 70 */
  { 0x002E, 0x002E, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 71 */
  { 0x003A, 0x003B, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 72 */
  { 0x003F, 0x003F, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 73 */
  { 0x037E, 0x037E, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 74 */
  { 0x0387, 0x0387, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 75 */
  { 0x0589, 0x0589, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 76 */
  { 0x05C3, 0x05C3, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 77 */
  { 0x060C, 0x060C, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 78 */
  { 0x061B, 0x061B, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 79 */
  { 0x061F, 0x061F, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 80 */
  { 0x06D4, 0x06D4, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 81 */
  { 0x0700, 0x070A, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 82 */
  { 0x070C, 0x070C, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 83 */
  { 0x07F8, 0x07F9, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 84 */
  { 0x0964, 0x0965, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 85 */
  { 0x0E5A, 0x0E5B, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 86 */
  { 0x0F08, 0x0F08, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 87 */
  { 0x0F0D, 0x0F12, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 88 */
  { 0x104A, 0x104B, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 89 */
  { 0x1361, 0x1368, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 90 */
  { 0x166D, 0x166E, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 91 */
  { 0x16EB, 0x16ED, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 92 */
  { 0x17D4, 0x17D6, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 93 */
  { 0x17DA, 0x17DA, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 94 */
  { 0x1802, 0x1805, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 95 */
  { 0x1808, 0x1809, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 96 */
  { 0x1944, 0x1945, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 97 */
  { 0x1B5A, 0x1B5B, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 98 */
  { 0x1B5D, 0x1B5F, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 99 */
  { 0x203C, 0x203D, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 100 */
  { 0x2047, 0x2049, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 101 */
  { 0x3001, 0x3002, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 102 */
  { 0xA876, 0xA877, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 103 */
  { 0xFE50, 0xFE52, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 104 */
  { 0xFE54, 0xFE57, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 105 */
  { 0xFF01, 0xFF01, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 106 */
  { 0xFF0C, 0xFF0C, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 107 */
  { 0xFF0E, 0xFF0E, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 108 */
  { 0xFF1A, 0xFF1B, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 109 */
  { 0xFF1F, 0xFF1F, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 110 */
  { 0xFF61, 0xFF61, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 111 */
  { 0xFF64, 0xFF64, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 112 */
  { 0x1039F, 0x1039F, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 113 */
  { 0x103D0, 0x103D0, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 114 */
  { 0x1091F, 0x1091F, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 115 */
  { 0x12470, 0x12473, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 116 */
  { 0x005E, 0x005E, PDF_TEXT_UCD_PL_Other_Math  }, /* 117 */
  { 0x03D0, 0x03D2, PDF_TEXT_UCD_PL_Other_Math  }, /* 118 */
  { 0x03D5, 0x03D5, PDF_TEXT_UCD_PL_Other_Math  }, /* 119 */
  { 0x03F0, 0x03F1, PDF_TEXT_UCD_PL_Other_Math  }, /* 120 */
  { 0x03F4, 0x03F5, PDF_TEXT_UCD_PL_Other_Math  }, /* 121 */
  { 0x2016, 0x2016, PDF_TEXT_UCD_PL_Other_Math  }, /* 122 */
  { 0x2032, 0x2034, PDF_TEXT_UCD_PL_Other_Math  }, /* 123 */
  { 0x2040, 0x2040, PDF_TEXT_UCD_PL_Other_Math  }, /* 124 */
  { 0x2061, 0x2063, PDF_TEXT_UCD_PL_Other_Math  }, /* 125 */
  { 0x207D, 0x207D, PDF_TEXT_UCD_PL_Other_Math  }, /* 126 */
  { 0x207E, 0x207E, PDF_TEXT_UCD_PL_Other_Math  }, /* 127 */
  { 0x208D, 0x208D, PDF_TEXT_UCD_PL_Other_Math  }, /* 128 */
  { 0x208E, 0x208E, PDF_TEXT_UCD_PL_Other_Math  }, /* 129 */
  { 0x20D0, 0x20DC, PDF_TEXT_UCD_PL_Other_Math  }, /* 130 */
  { 0x20E1, 0x20E1, PDF_TEXT_UCD_PL_Other_Math  }, /* 131 */
  { 0x20E5, 0x20E6, PDF_TEXT_UCD_PL_Other_Math  }, /* 132 */
  { 0x20EC, 0x20EF, PDF_TEXT_UCD_PL_Other_Math  }, /* 133 */
  { 0x2102, 0x2102, PDF_TEXT_UCD_PL_Other_Math  }, /* 134 */
  { 0x210A, 0x2113, PDF_TEXT_UCD_PL_Other_Math  }, /* 135 */
  { 0x2115, 0x2115, PDF_TEXT_UCD_PL_Other_Math  }, /* 136 */
  { 0x2119, 0x211D, PDF_TEXT_UCD_PL_Other_Math  }, /* 137 */
  { 0x2124, 0x2124, PDF_TEXT_UCD_PL_Other_Math  }, /* 138 */
  { 0x2128, 0x2128, PDF_TEXT_UCD_PL_Other_Math  }, /* 139 */
  { 0x2129, 0x2129, PDF_TEXT_UCD_PL_Other_Math  }, /* 140 */
  { 0x212C, 0x212D, PDF_TEXT_UCD_PL_Other_Math  }, /* 141 */
  { 0x212F, 0x2131, PDF_TEXT_UCD_PL_Other_Math  }, /* 142 */
  { 0x2133, 0x2134, PDF_TEXT_UCD_PL_Other_Math  }, /* 143 */
  { 0x2135, 0x2138, PDF_TEXT_UCD_PL_Other_Math  }, /* 144 */
  { 0x213C, 0x213F, PDF_TEXT_UCD_PL_Other_Math  }, /* 145 */
  { 0x2145, 0x2149, PDF_TEXT_UCD_PL_Other_Math  }, /* 146 */
  { 0x23B7, 0x23B7, PDF_TEXT_UCD_PL_Other_Math  }, /* 147 */
  { 0x23D0, 0x23D0, PDF_TEXT_UCD_PL_Other_Math  }, /* 148 */
  { 0x27C5, 0x27C5, PDF_TEXT_UCD_PL_Other_Math  }, /* 149 */
  { 0x27C6, 0x27C6, PDF_TEXT_UCD_PL_Other_Math  }, /* 150 */
  { 0x27E6, 0x27E6, PDF_TEXT_UCD_PL_Other_Math  }, /* 151 */
  { 0x27E7, 0x27E7, PDF_TEXT_UCD_PL_Other_Math  }, /* 152 */
  { 0x27E8, 0x27E8, PDF_TEXT_UCD_PL_Other_Math  }, /* 153 */
  { 0x27E9, 0x27E9, PDF_TEXT_UCD_PL_Other_Math  }, /* 154 */
  { 0x27EA, 0x27EA, PDF_TEXT_UCD_PL_Other_Math  }, /* 155 */
  { 0x27EB, 0x27EB, PDF_TEXT_UCD_PL_Other_Math  }, /* 156 */
  { 0x2983, 0x2983, PDF_TEXT_UCD_PL_Other_Math  }, /* 157 */
  { 0x2984, 0x2984, PDF_TEXT_UCD_PL_Other_Math  }, /* 158 */
  { 0x2985, 0x2985, PDF_TEXT_UCD_PL_Other_Math  }, /* 159 */
  { 0x2986, 0x2986, PDF_TEXT_UCD_PL_Other_Math  }, /* 160 */
  { 0x2987, 0x2987, PDF_TEXT_UCD_PL_Other_Math  }, /* 161 */
  { 0x2988, 0x2988, PDF_TEXT_UCD_PL_Other_Math  }, /* 162 */
  { 0x2989, 0x2989, PDF_TEXT_UCD_PL_Other_Math  }, /* 163 */
  { 0x298A, 0x298A, PDF_TEXT_UCD_PL_Other_Math  }, /* 164 */
  { 0x298B, 0x298B, PDF_TEXT_UCD_PL_Other_Math  }, /* 165 */
  { 0x298C, 0x298C, PDF_TEXT_UCD_PL_Other_Math  }, /* 166 */
  { 0x298D, 0x298D, PDF_TEXT_UCD_PL_Other_Math  }, /* 167 */
  { 0x298E, 0x298E, PDF_TEXT_UCD_PL_Other_Math  }, /* 168 */
  { 0x298F, 0x298F, PDF_TEXT_UCD_PL_Other_Math  }, /* 169 */
  { 0x2990, 0x2990, PDF_TEXT_UCD_PL_Other_Math  }, /* 170 */
  { 0x2991, 0x2991, PDF_TEXT_UCD_PL_Other_Math  }, /* 171 */
  { 0x2992, 0x2992, PDF_TEXT_UCD_PL_Other_Math  }, /* 172 */
  { 0x2993, 0x2993, PDF_TEXT_UCD_PL_Other_Math  }, /* 173 */
  { 0x2994, 0x2994, PDF_TEXT_UCD_PL_Other_Math  }, /* 174 */
  { 0x2995, 0x2995, PDF_TEXT_UCD_PL_Other_Math  }, /* 175 */
  { 0x2996, 0x2996, PDF_TEXT_UCD_PL_Other_Math  }, /* 176 */
  { 0x2997, 0x2997, PDF_TEXT_UCD_PL_Other_Math  }, /* 177 */
  { 0x2998, 0x2998, PDF_TEXT_UCD_PL_Other_Math  }, /* 178 */
  { 0x29D8, 0x29D8, PDF_TEXT_UCD_PL_Other_Math  }, /* 179 */
  { 0x29D9, 0x29D9, PDF_TEXT_UCD_PL_Other_Math  }, /* 180 */
  { 0x29DA, 0x29DA, PDF_TEXT_UCD_PL_Other_Math  }, /* 181 */
  { 0x29DB, 0x29DB, PDF_TEXT_UCD_PL_Other_Math  }, /* 182 */
  { 0x29FC, 0x29FC, PDF_TEXT_UCD_PL_Other_Math  }, /* 183 */
  { 0x29FD, 0x29FD, PDF_TEXT_UCD_PL_Other_Math  }, /* 184 */
  { 0xFE61, 0xFE61, PDF_TEXT_UCD_PL_Other_Math  }, /* 185 */
  { 0xFE63, 0xFE63, PDF_TEXT_UCD_PL_Other_Math  }, /* 186 */
  { 0xFE68, 0xFE68, PDF_TEXT_UCD_PL_Other_Math  }, /* 187 */
  { 0xFF3C, 0xFF3C, PDF_TEXT_UCD_PL_Other_Math  }, /* 188 */
  { 0xFF3E, 0xFF3E, PDF_TEXT_UCD_PL_Other_Math  }, /* 189 */
  { 0x1D400, 0x1D454, PDF_TEXT_UCD_PL_Other_Math  }, /* 190 */
  { 0x1D456, 0x1D49C, PDF_TEXT_UCD_PL_Other_Math  }, /* 191 */
  { 0x1D49E, 0x1D49F, PDF_TEXT_UCD_PL_Other_Math  }, /* 192 */
  { 0x1D4A2, 0x1D4A2, PDF_TEXT_UCD_PL_Other_Math  }, /* 193 */
  { 0x1D4A5, 0x1D4A6, PDF_TEXT_UCD_PL_Other_Math  }, /* 194 */
  { 0x1D4A9, 0x1D4AC, PDF_TEXT_UCD_PL_Other_Math  }, /* 195 */
  { 0x1D4AE, 0x1D4B9, PDF_TEXT_UCD_PL_Other_Math  }, /* 196 */
  { 0x1D4BB, 0x1D4BB, PDF_TEXT_UCD_PL_Other_Math  }, /* 197 */
  { 0x1D4BD, 0x1D4C3, PDF_TEXT_UCD_PL_Other_Math  }, /* 198 */
  { 0x1D4C5, 0x1D505, PDF_TEXT_UCD_PL_Other_Math  }, /* 199 */
  { 0x1D507, 0x1D50A, PDF_TEXT_UCD_PL_Other_Math  }, /* 200 */
  { 0x1D50D, 0x1D514, PDF_TEXT_UCD_PL_Other_Math  }, /* 201 */
  { 0x1D516, 0x1D51C, PDF_TEXT_UCD_PL_Other_Math  }, /* 202 */
  { 0x1D51E, 0x1D539, PDF_TEXT_UCD_PL_Other_Math  }, /* 203 */
  { 0x1D53B, 0x1D53E, PDF_TEXT_UCD_PL_Other_Math  }, /* 204 */
  { 0x1D540, 0x1D544, PDF_TEXT_UCD_PL_Other_Math  }, /* 205 */
  { 0x1D546, 0x1D546, PDF_TEXT_UCD_PL_Other_Math  }, /* 206 */
  { 0x1D54A, 0x1D550, PDF_TEXT_UCD_PL_Other_Math  }, /* 207 */
  { 0x1D552, 0x1D6A5, PDF_TEXT_UCD_PL_Other_Math  }, /* 208 */
  { 0x1D6A8, 0x1D6C0, PDF_TEXT_UCD_PL_Other_Math  }, /* 209 */
  { 0x1D6C2, 0x1D6DA, PDF_TEXT_UCD_PL_Other_Math  }, /* 210 */
  { 0x1D6DC, 0x1D6FA, PDF_TEXT_UCD_PL_Other_Math  }, /* 211 */
  { 0x1D6FC, 0x1D714, PDF_TEXT_UCD_PL_Other_Math  }, /* 212 */
  { 0x1D716, 0x1D734, PDF_TEXT_UCD_PL_Other_Math  }, /* 213 */
  { 0x1D736, 0x1D74E, PDF_TEXT_UCD_PL_Other_Math  }, /* 214 */
  { 0x1D750, 0x1D76E, PDF_TEXT_UCD_PL_Other_Math  }, /* 215 */
  { 0x1D770, 0x1D788, PDF_TEXT_UCD_PL_Other_Math  }, /* 216 */
  { 0x1D78A, 0x1D7A8, PDF_TEXT_UCD_PL_Other_Math  }, /* 217 */
  { 0x1D7AA, 0x1D7C2, PDF_TEXT_UCD_PL_Other_Math  }, /* 218 */
  { 0x1D7C4, 0x1D7CB, PDF_TEXT_UCD_PL_Other_Math  }, /* 219 */
  { 0x1D7CE, 0x1D7FF, PDF_TEXT_UCD_PL_Other_Math  }, /* 220 */
  { 0x0030, 0x0039, PDF_TEXT_UCD_PL_Hex_Digit  }, /* 221 */
  { 0x0041, 0x0046, PDF_TEXT_UCD_PL_Hex_Digit  }, /* 222 */
  { 0x0061, 0x0066, PDF_TEXT_UCD_PL_Hex_Digit  }, /* 223 */
  { 0xFF10, 0xFF19, PDF_TEXT_UCD_PL_Hex_Digit  }, /* 224 */
  { 0xFF21, 0xFF26, PDF_TEXT_UCD_PL_Hex_Digit  }, /* 225 */
  { 0xFF41, 0xFF46, PDF_TEXT_UCD_PL_Hex_Digit  }, /* 226 */
  { 0x0030, 0x0039, PDF_TEXT_UCD_PL_ASCII_Hex_Digit  }, /* 227 */
  { 0x0041, 0x0046, PDF_TEXT_UCD_PL_ASCII_Hex_Digit  }, /* 228 */
  { 0x0061, 0x0066, PDF_TEXT_UCD_PL_ASCII_Hex_Digit  }, /* 229 */
  { 0x0345, 0x0345, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 230 */
  { 0x05B0, 0x05BD, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 231 */
  { 0x05BF, 0x05BF, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 232 */
  { 0x05C1, 0x05C2, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 233 */
  { 0x05C4, 0x05C5, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 234 */
  { 0x05C7, 0x05C7, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 235 */
  { 0x0610, 0x0615, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 236 */
  { 0x064B, 0x0657, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 237 */
  { 0x0659, 0x065E, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 238 */
  { 0x0670, 0x0670, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 239 */
  { 0x06D6, 0x06DC, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 240 */
  { 0x06E1, 0x06E4, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 241 */
  { 0x06E7, 0x06E8, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 242 */
  { 0x06ED, 0x06ED, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 243 */
  { 0x0711, 0x0711, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 244 */
  { 0x0730, 0x073F, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 245 */
  { 0x07A6, 0x07B0, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 246 */
  { 0x0901, 0x0902, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 247 */
  { 0x0903, 0x0903, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 248 */
  { 0x093E, 0x0940, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 249 */
  { 0x0941, 0x0948, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 250 */
  { 0x0949, 0x094C, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 251 */
  { 0x0962, 0x0963, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 252 */
  { 0x0981, 0x0981, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 253 */
  { 0x0982, 0x0983, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 254 */
  { 0x09BE, 0x09C0, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 255 */
  { 0x09C1, 0x09C4, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 256 */
  { 0x09C7, 0x09C8, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 257 */
  { 0x09CB, 0x09CC, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 258 */
  { 0x09D7, 0x09D7, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 259 */
  { 0x09E2, 0x09E3, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 260 */
  { 0x0A01, 0x0A02, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 261 */
  { 0x0A03, 0x0A03, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 262 */
  { 0x0A3E, 0x0A40, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 263 */
  { 0x0A41, 0x0A42, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 264 */
  { 0x0A47, 0x0A48, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 265 */
  { 0x0A4B, 0x0A4C, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 266 */
  { 0x0A70, 0x0A71, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 267 */
  { 0x0A81, 0x0A82, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 268 */
  { 0x0A83, 0x0A83, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 269 */
  { 0x0ABE, 0x0AC0, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 270 */
  { 0x0AC1, 0x0AC5, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 271 */
  { 0x0AC7, 0x0AC8, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 272 */
  { 0x0AC9, 0x0AC9, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 273 */
  { 0x0ACB, 0x0ACC, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 274 */
  { 0x0AE2, 0x0AE3, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 275 */
  { 0x0B01, 0x0B01, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 276 */
  { 0x0B02, 0x0B03, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 277 */
  { 0x0B3E, 0x0B3E, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 278 */
  { 0x0B3F, 0x0B3F, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 279 */
  { 0x0B40, 0x0B40, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 280 */
  { 0x0B41, 0x0B43, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 281 */
  { 0x0B47, 0x0B48, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 282 */
  { 0x0B4B, 0x0B4C, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 283 */
  { 0x0B56, 0x0B56, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 284 */
  { 0x0B57, 0x0B57, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 285 */
  { 0x0B82, 0x0B82, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 286 */
  { 0x0BBE, 0x0BBF, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 287 */
  { 0x0BC0, 0x0BC0, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 288 */
  { 0x0BC1, 0x0BC2, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 289 */
  { 0x0BC6, 0x0BC8, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 290 */
  { 0x0BCA, 0x0BCC, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 291 */
  { 0x0BD7, 0x0BD7, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 292 */
  { 0x0C01, 0x0C03, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 293 */
  { 0x0C3E, 0x0C40, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 294 */
  { 0x0C41, 0x0C44, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 295 */
  { 0x0C46, 0x0C48, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 296 */
  { 0x0C4A, 0x0C4C, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 297 */
  { 0x0C55, 0x0C56, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 298 */
  { 0x0C82, 0x0C83, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 299 */
  { 0x0CBE, 0x0CBE, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 300 */
  { 0x0CBF, 0x0CBF, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 301 */
  { 0x0CC0, 0x0CC4, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 302 */
  { 0x0CC6, 0x0CC6, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 303 */
  { 0x0CC7, 0x0CC8, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 304 */
  { 0x0CCA, 0x0CCB, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 305 */
  { 0x0CCC, 0x0CCC, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 306 */
  { 0x0CD5, 0x0CD6, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 307 */
  { 0x0CE2, 0x0CE3, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 308 */
  { 0x0D02, 0x0D03, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 309 */
  { 0x0D3E, 0x0D40, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 310 */
  { 0x0D41, 0x0D43, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 311 */
  { 0x0D46, 0x0D48, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 312 */
  { 0x0D4A, 0x0D4C, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 313 */
  { 0x0D57, 0x0D57, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 314 */
  { 0x0D82, 0x0D83, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 315 */
  { 0x0DCF, 0x0DD1, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 316 */
  { 0x0DD2, 0x0DD4, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 317 */
  { 0x0DD6, 0x0DD6, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 318 */
  { 0x0DD8, 0x0DDF, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 319 */
  { 0x0DF2, 0x0DF3, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 320 */
  { 0x0E31, 0x0E31, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 321 */
  { 0x0E34, 0x0E3A, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 322 */
  { 0x0E4D, 0x0E4D, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 323 */
  { 0x0EB1, 0x0EB1, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 324 */
  { 0x0EB4, 0x0EB9, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 325 */
  { 0x0EBB, 0x0EBC, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 326 */
  { 0x0ECD, 0x0ECD, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 327 */
  { 0x0F71, 0x0F7E, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 328 */
  { 0x0F7F, 0x0F7F, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 329 */
  { 0x0F80, 0x0F81, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 330 */
  { 0x0F90, 0x0F97, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 331 */
  { 0x0F99, 0x0FBC, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 332 */
  { 0x102C, 0x102C, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 333 */
  { 0x102D, 0x1030, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 334 */
  { 0x1031, 0x1031, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 335 */
  { 0x1032, 0x1032, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 336 */
  { 0x1036, 0x1036, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 337 */
  { 0x1038, 0x1038, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 338 */
  { 0x1056, 0x1057, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 339 */
  { 0x1058, 0x1059, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 340 */
  { 0x135F, 0x135F, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 341 */
  { 0x1712, 0x1713, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 342 */
  { 0x1732, 0x1733, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 343 */
  { 0x1752, 0x1753, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 344 */
  { 0x1772, 0x1773, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 345 */
  { 0x17B6, 0x17B6, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 346 */
  { 0x17B7, 0x17BD, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 347 */
  { 0x17BE, 0x17C5, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 348 */
  { 0x17C6, 0x17C6, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 349 */
  { 0x17C7, 0x17C8, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 350 */
  { 0x18A9, 0x18A9, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 351 */
  { 0x1920, 0x1922, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 352 */
  { 0x1923, 0x1926, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 353 */
  { 0x1927, 0x1928, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 354 */
  { 0x1929, 0x192B, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 355 */
  { 0x1930, 0x1931, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 356 */
  { 0x1932, 0x1932, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 357 */
  { 0x1933, 0x1938, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 358 */
  { 0x19B0, 0x19C0, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 359 */
  { 0x19C8, 0x19C9, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 360 */
  { 0x1A17, 0x1A18, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 361 */
  { 0x1A19, 0x1A1B, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 362 */
  { 0x1B00, 0x1B03, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 363 */
  { 0x1B04, 0x1B04, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 364 */
  { 0x1B35, 0x1B35, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 365 */
  { 0x1B36, 0x1B3A, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 366 */
  { 0x1B3B, 0x1B3B, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 367 */
  { 0x1B3C, 0x1B3C, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 368 */
  { 0x1B3D, 0x1B41, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 369 */
  { 0x1B42, 0x1B42, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 370 */
  { 0x1B43, 0x1B43, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 371 */
  { 0x24B6, 0x24E9, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 372 */
  { 0xA823, 0xA824, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 373 */
  { 0xA825, 0xA826, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 374 */
  { 0xA827, 0xA827, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 375 */
  { 0xFB1E, 0xFB1E, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 376 */
  { 0x10A01, 0x10A03, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 377 */
  { 0x10A05, 0x10A06, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 378 */
  { 0x10A0C, 0x10A0F, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 379 */
  { 0x3006, 0x3006, PDF_TEXT_UCD_PL_Ideographic  }, /* 380 */
  { 0x3007, 0x3007, PDF_TEXT_UCD_PL_Ideographic  }, /* 381 */
  { 0x3021, 0x3029, PDF_TEXT_UCD_PL_Ideographic  }, /* 382 */
  { 0x3038, 0x303A, PDF_TEXT_UCD_PL_Ideographic  }, /* 383 */
  { 0x3400, 0x4DB5, PDF_TEXT_UCD_PL_Ideographic  }, /* 384 */
  { 0x4E00, 0x9FBB, PDF_TEXT_UCD_PL_Ideographic  }, /* 385 */
  { 0xF900, 0xFA2D, PDF_TEXT_UCD_PL_Ideographic  }, /* 386 */
  { 0xFA70, 0xFAD9, PDF_TEXT_UCD_PL_Ideographic  }, /* 387 */
  { 0x20000, 0x2A6D6, PDF_TEXT_UCD_PL_Ideographic  }, /* 388 */
  { 0x2F800, 0x2FA1D, PDF_TEXT_UCD_PL_Ideographic  }, /* 389 */
  { 0x005E, 0x005E, PDF_TEXT_UCD_PL_Diacritic  }, /* 390 */
  { 0x0060, 0x0060, PDF_TEXT_UCD_PL_Diacritic  }, /* 391 */
  { 0x00A8, 0x00A8, PDF_TEXT_UCD_PL_Diacritic  }, /* 392 */
  { 0x00AF, 0x00AF, PDF_TEXT_UCD_PL_Diacritic  }, /* 393 */
  { 0x00B4, 0x00B4, PDF_TEXT_UCD_PL_Diacritic  }, /* 394 */
  { 0x00B7, 0x00B7, PDF_TEXT_UCD_PL_Diacritic  }, /* 395 */
  { 0x00B8, 0x00B8, PDF_TEXT_UCD_PL_Diacritic  }, /* 396 */
  { 0x02B0, 0x02C1, PDF_TEXT_UCD_PL_Diacritic  }, /* 397 */
  { 0x02C2, 0x02C5, PDF_TEXT_UCD_PL_Diacritic  }, /* 398 */
  { 0x02C6, 0x02D1, PDF_TEXT_UCD_PL_Diacritic  }, /* 399 */
  { 0x02D2, 0x02DF, PDF_TEXT_UCD_PL_Diacritic  }, /* 400 */
  { 0x02E0, 0x02E4, PDF_TEXT_UCD_PL_Diacritic  }, /* 401 */
  { 0x02E5, 0x02ED, PDF_TEXT_UCD_PL_Diacritic  }, /* 402 */
  { 0x02EE, 0x02EE, PDF_TEXT_UCD_PL_Diacritic  }, /* 403 */
  { 0x02EF, 0x02FF, PDF_TEXT_UCD_PL_Diacritic  }, /* 404 */
  { 0x0300, 0x034E, PDF_TEXT_UCD_PL_Diacritic  }, /* 405 */
  { 0x0350, 0x0357, PDF_TEXT_UCD_PL_Diacritic  }, /* 406 */
  { 0x035D, 0x0362, PDF_TEXT_UCD_PL_Diacritic  }, /* 407 */
  { 0x0374, 0x0375, PDF_TEXT_UCD_PL_Diacritic  }, /* 408 */
  { 0x037A, 0x037A, PDF_TEXT_UCD_PL_Diacritic  }, /* 409 */
  { 0x0384, 0x0385, PDF_TEXT_UCD_PL_Diacritic  }, /* 410 */
  { 0x0483, 0x0486, PDF_TEXT_UCD_PL_Diacritic  }, /* 411 */
  { 0x0559, 0x0559, PDF_TEXT_UCD_PL_Diacritic  }, /* 412 */
  { 0x0591, 0x05A1, PDF_TEXT_UCD_PL_Diacritic  }, /* 413 */
  { 0x05A3, 0x05BD, PDF_TEXT_UCD_PL_Diacritic  }, /* 414 */
  { 0x05BF, 0x05BF, PDF_TEXT_UCD_PL_Diacritic  }, /* 415 */
  { 0x05C1, 0x05C2, PDF_TEXT_UCD_PL_Diacritic  }, /* 416 */
  { 0x05C4, 0x05C4, PDF_TEXT_UCD_PL_Diacritic  }, /* 417 */
  { 0x064B, 0x0652, PDF_TEXT_UCD_PL_Diacritic  }, /* 418 */
  { 0x0657, 0x0658, PDF_TEXT_UCD_PL_Diacritic  }, /* 419 */
  { 0x06DF, 0x06E0, PDF_TEXT_UCD_PL_Diacritic  }, /* 420 */
  { 0x06E5, 0x06E6, PDF_TEXT_UCD_PL_Diacritic  }, /* 421 */
  { 0x06EA, 0x06EC, PDF_TEXT_UCD_PL_Diacritic  }, /* 422 */
  { 0x0730, 0x074A, PDF_TEXT_UCD_PL_Diacritic  }, /* 423 */
  { 0x07A6, 0x07B0, PDF_TEXT_UCD_PL_Diacritic  }, /* 424 */
  { 0x07EB, 0x07F3, PDF_TEXT_UCD_PL_Diacritic  }, /* 425 */
  { 0x07F4, 0x07F5, PDF_TEXT_UCD_PL_Diacritic  }, /* 426 */
  { 0x093C, 0x093C, PDF_TEXT_UCD_PL_Diacritic  }, /* 427 */
  { 0x094D, 0x094D, PDF_TEXT_UCD_PL_Diacritic  }, /* 428 */
  { 0x0951, 0x0954, PDF_TEXT_UCD_PL_Diacritic  }, /* 429 */
  { 0x09BC, 0x09BC, PDF_TEXT_UCD_PL_Diacritic  }, /* 430 */
  { 0x09CD, 0x09CD, PDF_TEXT_UCD_PL_Diacritic  }, /* 431 */
  { 0x0A3C, 0x0A3C, PDF_TEXT_UCD_PL_Diacritic  }, /* 432 */
  { 0x0A4D, 0x0A4D, PDF_TEXT_UCD_PL_Diacritic  }, /* 433 */
  { 0x0ABC, 0x0ABC, PDF_TEXT_UCD_PL_Diacritic  }, /* 434 */
  { 0x0ACD, 0x0ACD, PDF_TEXT_UCD_PL_Diacritic  }, /* 435 */
  { 0x0B3C, 0x0B3C, PDF_TEXT_UCD_PL_Diacritic  }, /* 436 */
  { 0x0B4D, 0x0B4D, PDF_TEXT_UCD_PL_Diacritic  }, /* 437 */
  { 0x0BCD, 0x0BCD, PDF_TEXT_UCD_PL_Diacritic  }, /* 438 */
  { 0x0C4D, 0x0C4D, PDF_TEXT_UCD_PL_Diacritic  }, /* 439 */
  { 0x0CBC, 0x0CBC, PDF_TEXT_UCD_PL_Diacritic  }, /* 440 */
  { 0x0CCD, 0x0CCD, PDF_TEXT_UCD_PL_Diacritic  }, /* 441 */
  { 0x0D4D, 0x0D4D, PDF_TEXT_UCD_PL_Diacritic  }, /* 442 */
  { 0x0DCA, 0x0DCA, PDF_TEXT_UCD_PL_Diacritic  }, /* 443 */
  { 0x0E47, 0x0E4C, PDF_TEXT_UCD_PL_Diacritic  }, /* 444 */
  { 0x0E4E, 0x0E4E, PDF_TEXT_UCD_PL_Diacritic  }, /* 445 */
  { 0x0EC8, 0x0ECC, PDF_TEXT_UCD_PL_Diacritic  }, /* 446 */
  { 0x0F18, 0x0F19, PDF_TEXT_UCD_PL_Diacritic  }, /* 447 */
  { 0x0F35, 0x0F35, PDF_TEXT_UCD_PL_Diacritic  }, /* 448 */
  { 0x0F37, 0x0F37, PDF_TEXT_UCD_PL_Diacritic  }, /* 449 */
  { 0x0F39, 0x0F39, PDF_TEXT_UCD_PL_Diacritic  }, /* 450 */
  { 0x0F3E, 0x0F3F, PDF_TEXT_UCD_PL_Diacritic  }, /* 451 */
  { 0x0F82, 0x0F84, PDF_TEXT_UCD_PL_Diacritic  }, /* 452 */
  { 0x0F86, 0x0F87, PDF_TEXT_UCD_PL_Diacritic  }, /* 453 */
  { 0x0FC6, 0x0FC6, PDF_TEXT_UCD_PL_Diacritic  }, /* 454 */
  { 0x1037, 0x1037, PDF_TEXT_UCD_PL_Diacritic  }, /* 455 */
  { 0x1039, 0x1039, PDF_TEXT_UCD_PL_Diacritic  }, /* 456 */
  { 0x17C9, 0x17D3, PDF_TEXT_UCD_PL_Diacritic  }, /* 457 */
  { 0x17DD, 0x17DD, PDF_TEXT_UCD_PL_Diacritic  }, /* 458 */
  { 0x1939, 0x193B, PDF_TEXT_UCD_PL_Diacritic  }, /* 459 */
  { 0x1B34, 0x1B34, PDF_TEXT_UCD_PL_Diacritic  }, /* 460 */
  { 0x1B44, 0x1B44, PDF_TEXT_UCD_PL_Diacritic  }, /* 461 */
  { 0x1B6B, 0x1B73, PDF_TEXT_UCD_PL_Diacritic  }, /* 462 */
  { 0x1D2C, 0x1D61, PDF_TEXT_UCD_PL_Diacritic  }, /* 463 */
  { 0x1D62, 0x1D6A, PDF_TEXT_UCD_PL_Diacritic  }, /* 464 */
  { 0x1DC4, 0x1DCA, PDF_TEXT_UCD_PL_Diacritic  }, /* 465 */
  { 0x1DFE, 0x1DFF, PDF_TEXT_UCD_PL_Diacritic  }, /* 466 */
  { 0x1FBD, 0x1FBD, PDF_TEXT_UCD_PL_Diacritic  }, /* 467 */
  { 0x1FBF, 0x1FC1, PDF_TEXT_UCD_PL_Diacritic  }, /* 468 */
  { 0x1FCD, 0x1FCF, PDF_TEXT_UCD_PL_Diacritic  }, /* 469 */
  { 0x1FDD, 0x1FDF, PDF_TEXT_UCD_PL_Diacritic  }, /* 470 */
  { 0x1FED, 0x1FEF, PDF_TEXT_UCD_PL_Diacritic  }, /* 471 */
  { 0x1FFD, 0x1FFE, PDF_TEXT_UCD_PL_Diacritic  }, /* 472 */
  { 0x302A, 0x302F, PDF_TEXT_UCD_PL_Diacritic  }, /* 473 */
  { 0x3099, 0x309A, PDF_TEXT_UCD_PL_Diacritic  }, /* 474 */
  { 0x309B, 0x309C, PDF_TEXT_UCD_PL_Diacritic  }, /* 475 */
  { 0x30FC, 0x30FC, PDF_TEXT_UCD_PL_Diacritic  }, /* 476 */
  { 0xA717, 0xA71A, PDF_TEXT_UCD_PL_Diacritic  }, /* 477 */
  { 0xA720, 0xA721, PDF_TEXT_UCD_PL_Diacritic  }, /* 478 */
  { 0xFB1E, 0xFB1E, PDF_TEXT_UCD_PL_Diacritic  }, /* 479 */
  { 0xFE20, 0xFE23, PDF_TEXT_UCD_PL_Diacritic  }, /* 480 */
  { 0xFF3E, 0xFF3E, PDF_TEXT_UCD_PL_Diacritic  }, /* 481 */
  { 0xFF40, 0xFF40, PDF_TEXT_UCD_PL_Diacritic  }, /* 482 */
  { 0xFF70, 0xFF70, PDF_TEXT_UCD_PL_Diacritic  }, /* 483 */
  { 0xFF9E, 0xFF9F, PDF_TEXT_UCD_PL_Diacritic  }, /* 484 */
  { 0xFFE3, 0xFFE3, PDF_TEXT_UCD_PL_Diacritic  }, /* 485 */
  { 0x1D167, 0x1D169, PDF_TEXT_UCD_PL_Diacritic  }, /* 486 */
  { 0x1D16D, 0x1D172, PDF_TEXT_UCD_PL_Diacritic  }, /* 487 */
  { 0x1D17B, 0x1D182, PDF_TEXT_UCD_PL_Diacritic  }, /* 488 */
  { 0x1D185, 0x1D18B, PDF_TEXT_UCD_PL_Diacritic  }, /* 489 */
  { 0x1D1AA, 0x1D1AD, PDF_TEXT_UCD_PL_Diacritic  }, /* 490 */
  { 0x00B7, 0x00B7, PDF_TEXT_UCD_PL_Extender  }, /* 491 */
  { 0x02D0, 0x02D1, PDF_TEXT_UCD_PL_Extender  }, /* 492 */
  { 0x0640, 0x0640, PDF_TEXT_UCD_PL_Extender  }, /* 493 */
  { 0x07FA, 0x07FA, PDF_TEXT_UCD_PL_Extender  }, /* 494 */
  { 0x0E46, 0x0E46, PDF_TEXT_UCD_PL_Extender  }, /* 495 */
  { 0x0EC6, 0x0EC6, PDF_TEXT_UCD_PL_Extender  }, /* 496 */
  { 0x1843, 0x1843, PDF_TEXT_UCD_PL_Extender  }, /* 497 */
  { 0x3005, 0x3005, PDF_TEXT_UCD_PL_Extender  }, /* 498 */
  { 0x3031, 0x3035, PDF_TEXT_UCD_PL_Extender  }, /* 499 */
  { 0x309D, 0x309E, PDF_TEXT_UCD_PL_Extender  }, /* 500 */
  { 0x30FC, 0x30FE, PDF_TEXT_UCD_PL_Extender  }, /* 501 */
  { 0xA015, 0xA015, PDF_TEXT_UCD_PL_Extender  }, /* 502 */
  { 0xFF70, 0xFF70, PDF_TEXT_UCD_PL_Extender  }, /* 503 */
  { 0x02B0, 0x02B8, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 504 */
  { 0x02C0, 0x02C1, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 505 */
  { 0x02E0, 0x02E4, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 506 */
  { 0x0345, 0x0345, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 507 */
  { 0x037A, 0x037A, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 508 */
  { 0x1D2C, 0x1D61, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 509 */
  { 0x1D78, 0x1D78, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 510 */
  { 0x1D9B, 0x1DBF, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 511 */
  { 0x2090, 0x2094, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 512 */
  { 0x2170, 0x217F, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 513 */
  { 0x24D0, 0x24E9, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 514 */
  { 0x2160, 0x216F, PDF_TEXT_UCD_PL_Other_Uppercase  }, /* 515 */
  { 0x24B6, 0x24CF, PDF_TEXT_UCD_PL_Other_Uppercase  }, /* 516 */
  { 0xFDD0, 0xFDEF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 517 */
  { 0xFFFE, 0xFFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 518 */
  { 0x1FFFE, 0x1FFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 519 */
  { 0x2FFFE, 0x2FFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 520 */
  { 0x3FFFE, 0x3FFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 521 */
  { 0x4FFFE, 0x4FFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 522 */
  { 0x5FFFE, 0x5FFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 523 */
  { 0x6FFFE, 0x6FFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 524 */
  { 0x7FFFE, 0x7FFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 525 */
  { 0x8FFFE, 0x8FFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 526 */
  { 0x9FFFE, 0x9FFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 527 */
  { 0xAFFFE, 0xAFFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 528 */
  { 0xBFFFE, 0xBFFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 529 */
  { 0xCFFFE, 0xCFFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 530 */
  { 0xDFFFE, 0xDFFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 531 */
  { 0xEFFFE, 0xEFFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 532 */
  { 0xFFFFE, 0xFFFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 533 */
  { 0x10FFFE, 0x10FFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 534 */
  { 0x09BE, 0x09BE, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 535 */
  { 0x09D7, 0x09D7, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 536 */
  { 0x0B3E, 0x0B3E, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 537 */
  { 0x0B57, 0x0B57, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 538 */
  { 0x0BBE, 0x0BBE, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 539 */
  { 0x0BD7, 0x0BD7, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 540 */
  { 0x0CC2, 0x0CC2, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 541 */
  { 0x0CD5, 0x0CD6, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 542 */
  { 0x0D3E, 0x0D3E, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 543 */
  { 0x0D57, 0x0D57, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 544 */
  { 0x0DCF, 0x0DCF, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 545 */
  { 0x0DDF, 0x0DDF, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 546 */
  { 0x200C, 0x200D, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 547 */
  { 0x1D165, 0x1D165, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 548 */
  { 0x1D16E, 0x1D172, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 549 */
  { 0x2FF0, 0x2FF1, PDF_TEXT_UCD_PL_IDS_Binary_Operator  }, /* 550 */
  { 0x2FF4, 0x2FFB, PDF_TEXT_UCD_PL_IDS_Binary_Operator  }, /* 551 */
  { 0x2FF2, 0x2FF3, PDF_TEXT_UCD_PL_IDS_Trinary_Operator  }, /* 552 */
  { 0x2E80, 0x2E99, PDF_TEXT_UCD_PL_Radical  }, /* 553 */
  { 0x2E9B, 0x2EF3, PDF_TEXT_UCD_PL_Radical  }, /* 554 */
  { 0x2F00, 0x2FD5, PDF_TEXT_UCD_PL_Radical  }, /* 555 */
  { 0x3400, 0x4DB5, PDF_TEXT_UCD_PL_Unified_Ideograph  }, /* 556 */
  { 0x4E00, 0x9FBB, PDF_TEXT_UCD_PL_Unified_Ideograph  }, /* 557 */
  { 0xFA0E, 0xFA0F, PDF_TEXT_UCD_PL_Unified_Ideograph  }, /* 558 */
  { 0xFA11, 0xFA11, PDF_TEXT_UCD_PL_Unified_Ideograph  }, /* 559 */
  { 0xFA13, 0xFA14, PDF_TEXT_UCD_PL_Unified_Ideograph  }, /* 560 */
  { 0xFA1F, 0xFA1F, PDF_TEXT_UCD_PL_Unified_Ideograph  }, /* 561 */
  { 0xFA21, 0xFA21, PDF_TEXT_UCD_PL_Unified_Ideograph  }, /* 562 */
  { 0xFA23, 0xFA24, PDF_TEXT_UCD_PL_Unified_Ideograph  }, /* 563 */
  { 0xFA27, 0xFA29, PDF_TEXT_UCD_PL_Unified_Ideograph  }, /* 564 */
  { 0x20000, 0x2A6D6, PDF_TEXT_UCD_PL_Unified_Ideograph  }, /* 565 */
  { 0x034F, 0x034F, PDF_TEXT_UCD_PL_Other_Default_Ignorable_Code_Point  }, /* 566 */
  { 0x115F, 0x1160, PDF_TEXT_UCD_PL_Other_Default_Ignorable_Code_Point  }, /* 567 */
  { 0x2064, 0x2069, PDF_TEXT_UCD_PL_Other_Default_Ignorable_Code_Point  }, /* 568 */
  { 0x3164, 0x3164, PDF_TEXT_UCD_PL_Other_Default_Ignorable_Code_Point  }, /* 569 */
  { 0xFFA0, 0xFFA0, PDF_TEXT_UCD_PL_Other_Default_Ignorable_Code_Point  }, /* 570 */
  { 0xFFF0, 0xFFF8, PDF_TEXT_UCD_PL_Other_Default_Ignorable_Code_Point  }, /* 571 */
  { 0xE0000, 0xE0000, PDF_TEXT_UCD_PL_Other_Default_Ignorable_Code_Point  }, /* 572 */
  { 0xE0002, 0xE001F, PDF_TEXT_UCD_PL_Other_Default_Ignorable_Code_Point  }, /* 573 */
  { 0xE0080, 0xE00FF, PDF_TEXT_UCD_PL_Other_Default_Ignorable_Code_Point  }, /* 574 */
  { 0xE01F0, 0xE0FFF, PDF_TEXT_UCD_PL_Other_Default_Ignorable_Code_Point  }, /* 575 */
  { 0x0340, 0x0341, PDF_TEXT_UCD_PL_Deprecated  }, /* 576 */
  { 0x17A3, 0x17A3, PDF_TEXT_UCD_PL_Deprecated  }, /* 577 */
  { 0x17D3, 0x17D3, PDF_TEXT_UCD_PL_Deprecated  }, /* 578 */
  { 0x206A, 0x206F, PDF_TEXT_UCD_PL_Deprecated  }, /* 579 */
  { 0x0069, 0x006A, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 580 */
  { 0x012F, 0x012F, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 581 */
  { 0x0249, 0x0249, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 582 */
  { 0x0268, 0x0268, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 583 */
  { 0x029D, 0x029D, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 584 */
  { 0x02B2, 0x02B2, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 585 */
  { 0x03F3, 0x03F3, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 586 */
  { 0x0456, 0x0456, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 587 */
  { 0x0458, 0x0458, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 588 */
  { 0x1D62, 0x1D62, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 589 */
  { 0x1D96, 0x1D96, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 590 */
  { 0x1DA4, 0x1DA4, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 591 */
  { 0x1DA8, 0x1DA8, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 592 */
  { 0x1E2D, 0x1E2D, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 593 */
  { 0x1ECB, 0x1ECB, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 594 */
  { 0x2071, 0x2071, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 595 */
  { 0x2148, 0x2149, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 596 */
  { 0x1D422, 0x1D423, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 597 */
  { 0x1D456, 0x1D457, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 598 */
  { 0x1D48A, 0x1D48B, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 599 */
  { 0x1D4BE, 0x1D4BF, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 600 */
  { 0x1D4F2, 0x1D4F3, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 601 */
  { 0x1D526, 0x1D527, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 602 */
  { 0x1D55A, 0x1D55B, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 603 */
  { 0x1D58E, 0x1D58F, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 604 */
  { 0x1D5C2, 0x1D5C3, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 605 */
  { 0x1D5F6, 0x1D5F7, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 606 */
  { 0x1D62A, 0x1D62B, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 607 */
  { 0x1D65E, 0x1D65F, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 608 */
  { 0x1D692, 0x1D693, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 609 */
  { 0x0E40, 0x0E44, PDF_TEXT_UCD_PL_Logical_Order_Exception  }, /* 610 */
  { 0x0EC0, 0x0EC4, PDF_TEXT_UCD_PL_Logical_Order_Exception  }, /* 611 */
  { 0x2118, 0x2118, PDF_TEXT_UCD_PL_Other_ID_Start  }, /* 612 */
  { 0x212E, 0x212E, PDF_TEXT_UCD_PL_Other_ID_Start  }, /* 613 */
  { 0x309B, 0x309C, PDF_TEXT_UCD_PL_Other_ID_Start  }, /* 614 */
  { 0x1369, 0x1371, PDF_TEXT_UCD_PL_Other_ID_Continue  }, /* 615 */
  { 0x0021, 0x0021, PDF_TEXT_UCD_PL_STerm  }, /* 616 */
  { 0x002E, 0x002E, PDF_TEXT_UCD_PL_STerm  }, /* 617 */
  { 0x003F, 0x003F, PDF_TEXT_UCD_PL_STerm  }, /* 618 */
  { 0x055C, 0x055C, PDF_TEXT_UCD_PL_STerm  }, /* 619 */
  { 0x055E, 0x055E, PDF_TEXT_UCD_PL_STerm  }, /* 620 */
  { 0x0589, 0x0589, PDF_TEXT_UCD_PL_STerm  }, /* 621 */
  { 0x061F, 0x061F, PDF_TEXT_UCD_PL_STerm  }, /* 622 */
  { 0x06D4, 0x06D4, PDF_TEXT_UCD_PL_STerm  }, /* 623 */
  { 0x0700, 0x0702, PDF_TEXT_UCD_PL_STerm  }, /* 624 */
  { 0x07F9, 0x07F9, PDF_TEXT_UCD_PL_STerm  }, /* 625 */
  { 0x0964, 0x0965, PDF_TEXT_UCD_PL_STerm  }, /* 626 */
  { 0x104A, 0x104B, PDF_TEXT_UCD_PL_STerm  }, /* 627 */
  { 0x1362, 0x1362, PDF_TEXT_UCD_PL_STerm  }, /* 628 */
  { 0x1367, 0x1368, PDF_TEXT_UCD_PL_STerm  }, /* 629 */
  { 0x166E, 0x166E, PDF_TEXT_UCD_PL_STerm  }, /* 630 */
  { 0x1803, 0x1803, PDF_TEXT_UCD_PL_STerm  }, /* 631 */
  { 0x1809, 0x1809, PDF_TEXT_UCD_PL_STerm  }, /* 632 */
  { 0x1944, 0x1945, PDF_TEXT_UCD_PL_STerm  }, /* 633 */
  { 0x1B5A, 0x1B5B, PDF_TEXT_UCD_PL_STerm  }, /* 634 */
  { 0x1B5E, 0x1B5F, PDF_TEXT_UCD_PL_STerm  }, /* 635 */
  { 0x203C, 0x203D, PDF_TEXT_UCD_PL_STerm  }, /* 636 */
  { 0x2047, 0x2049, PDF_TEXT_UCD_PL_STerm  }, /* 637 */
  { 0x3002, 0x3002, PDF_TEXT_UCD_PL_STerm  }, /* 638 */
  { 0xA876, 0xA877, PDF_TEXT_UCD_PL_STerm  }, /* 639 */
  { 0xFE52, 0xFE52, PDF_TEXT_UCD_PL_STerm  }, /* 640 */
  { 0xFE56, 0xFE57, PDF_TEXT_UCD_PL_STerm  }, /* 641 */
  { 0xFF01, 0xFF01, PDF_TEXT_UCD_PL_STerm  }, /* 642 */
  { 0xFF0E, 0xFF0E, PDF_TEXT_UCD_PL_STerm  }, /* 643 */
  { 0xFF1F, 0xFF1F, PDF_TEXT_UCD_PL_STerm  }, /* 644 */
  { 0xFF61, 0xFF61, PDF_TEXT_UCD_PL_STerm  }, /* 645 */
  { 0x180B, 0x180D, PDF_TEXT_UCD_PL_Variation_Selector  }, /* 646 */
  { 0xFE00, 0xFE0F, PDF_TEXT_UCD_PL_Variation_Selector  }, /* 647 */
  { 0xE0100, 0xE01EF, PDF_TEXT_UCD_PL_Variation_Selector  }, /* 648 */
  { 0x0009, 0x000D, PDF_TEXT_UCD_PL_Pattern_White_Space  }, /* 649 */
  { 0x0020, 0x0020, PDF_TEXT_UCD_PL_Pattern_White_Space  }, /* 650 */
  { 0x0085, 0x0085, PDF_TEXT_UCD_PL_Pattern_White_Space  }, /* 651 */
  { 0x200E, 0x200F, PDF_TEXT_UCD_PL_Pattern_White_Space  }, /* 652 */
  { 0x2028, 0x2028, PDF_TEXT_UCD_PL_Pattern_White_Space  }, /* 653 */
  { 0x2029, 0x2029, PDF_TEXT_UCD_PL_Pattern_White_Space  }, /* 654 */
  { 0x0021, 0x0023, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 655 */
  { 0x0024, 0x0024, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 656 */
  { 0x0025, 0x0027, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 657 */
  { 0x0028, 0x0028, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 658 */
  { 0x0029, 0x0029, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 659 */
  { 0x002A, 0x002A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 660 */
  { 0x002B, 0x002B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 661 */
  { 0x002C, 0x002C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 662 */
  { 0x002D, 0x002D, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 663 */
  { 0x002E, 0x002F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 664 */
  { 0x003A, 0x003B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 665 */
  { 0x003C, 0x003E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 666 */
  { 0x003F, 0x0040, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 667 */
  { 0x005B, 0x005B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 668 */
  { 0x005C, 0x005C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 669 */
  { 0x005D, 0x005D, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 670 */
  { 0x005E, 0x005E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 671 */
  { 0x0060, 0x0060, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 672 */
  { 0x007B, 0x007B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 673 */
  { 0x007C, 0x007C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 674 */
  { 0x007D, 0x007D, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 675 */
  { 0x007E, 0x007E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 676 */
  { 0x00A1, 0x00A1, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 677 */
  { 0x00A2, 0x00A5, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 678 */
  { 0x00A6, 0x00A7, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 679 */
  { 0x00A9, 0x00A9, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 680 */
  { 0x00AB, 0x00AB, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 681 */
  { 0x00AC, 0x00AC, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 682 */
  { 0x00AE, 0x00AE, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 683 */
  { 0x00B0, 0x00B0, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 684 */
  { 0x00B1, 0x00B1, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 685 */
  { 0x00B6, 0x00B6, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 686 */
  { 0x00BB, 0x00BB, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 687 */
  { 0x00BF, 0x00BF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 688 */
  { 0x00D7, 0x00D7, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 689 */
  { 0x00F7, 0x00F7, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 690 */
  { 0x2010, 0x2015, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 691 */
  { 0x2016, 0x2017, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 692 */
  { 0x2018, 0x2018, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 693 */
  { 0x2019, 0x2019, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 694 */
  { 0x201A, 0x201A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 695 */
  { 0x201B, 0x201C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 696 */
  { 0x201D, 0x201D, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 697 */
  { 0x201E, 0x201E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 698 */
  { 0x201F, 0x201F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 699 */
  { 0x2020, 0x2027, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 700 */
  { 0x2030, 0x2038, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 701 */
  { 0x2039, 0x2039, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 702 */
  { 0x203A, 0x203A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 703 */
  { 0x203B, 0x203E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 704 */
  { 0x2041, 0x2043, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 705 */
  { 0x2044, 0x2044, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 706 */
  { 0x2045, 0x2045, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 707 */
  { 0x2046, 0x2046, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 708 */
  { 0x2047, 0x2051, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 709 */
  { 0x2052, 0x2052, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 710 */
  { 0x2053, 0x2053, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 711 */
  { 0x2055, 0x205E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 712 */
  { 0x2190, 0x2194, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 713 */
  { 0x2195, 0x2199, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 714 */
  { 0x219A, 0x219B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 715 */
  { 0x219C, 0x219F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 716 */
  { 0x21A0, 0x21A0, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 717 */
  { 0x21A1, 0x21A2, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 718 */
  { 0x21A3, 0x21A3, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 719 */
  { 0x21A4, 0x21A5, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 720 */
  { 0x21A6, 0x21A6, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 721 */
  { 0x21A7, 0x21AD, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 722 */
  { 0x21AE, 0x21AE, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 723 */
  { 0x21AF, 0x21CD, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 724 */
  { 0x21CE, 0x21CF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 725 */
  { 0x21D0, 0x21D1, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 726 */
  { 0x21D2, 0x21D2, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 727 */
  { 0x21D3, 0x21D3, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 728 */
  { 0x21D4, 0x21D4, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 729 */
  { 0x21D5, 0x21F3, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 730 */
  { 0x21F4, 0x22FF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 731 */
  { 0x2300, 0x2307, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 732 */
  { 0x2308, 0x230B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 733 */
  { 0x230C, 0x231F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 734 */
  { 0x2320, 0x2321, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 735 */
  { 0x2322, 0x2328, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 736 */
  { 0x2329, 0x2329, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 737 */
  { 0x232A, 0x232A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 738 */
  { 0x232B, 0x237B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 739 */
  { 0x237C, 0x237C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 740 */
  { 0x237D, 0x239A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 741 */
  { 0x239B, 0x23B3, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 742 */
  { 0x23B4, 0x23DB, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 743 */
  { 0x23DC, 0x23E1, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 744 */
  { 0x23E2, 0x23E7, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 745 */
  { 0x23E8, 0x23FF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 746 */
  { 0x2400, 0x2426, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 747 */
  { 0x2427, 0x243F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 748 */
  { 0x2440, 0x244A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 749 */
  { 0x244B, 0x245F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 750 */
  { 0x2500, 0x25B6, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 751 */
  { 0x25B7, 0x25B7, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 752 */
  { 0x25B8, 0x25C0, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 753 */
  { 0x25C1, 0x25C1, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 754 */
  { 0x25C2, 0x25F7, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 755 */
  { 0x25F8, 0x25FF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 756 */
  { 0x2600, 0x266E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 757 */
  { 0x266F, 0x266F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 758 */
  { 0x2670, 0x269C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 759 */
  { 0x269D, 0x269F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 760 */
  { 0x26A0, 0x26B2, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 761 */
  { 0x26B3, 0x2700, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 762 */
  { 0x2701, 0x2704, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 763 */
  { 0x2705, 0x2705, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 764 */
  { 0x2706, 0x2709, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 765 */
  { 0x270A, 0x270B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 766 */
  { 0x270C, 0x2727, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 767 */
  { 0x2728, 0x2728, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 768 */
  { 0x2729, 0x274B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 769 */
  { 0x274C, 0x274C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 770 */
  { 0x274D, 0x274D, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 771 */
  { 0x274E, 0x274E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 772 */
  { 0x274F, 0x2752, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 773 */
  { 0x2753, 0x2755, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 774 */
  { 0x2756, 0x2756, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 775 */
  { 0x2757, 0x2757, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 776 */
  { 0x2758, 0x275E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 777 */
  { 0x275F, 0x2760, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 778 */
  { 0x2761, 0x2767, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 779 */
  { 0x2768, 0x2768, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 780 */
  { 0x2769, 0x2769, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 781 */
  { 0x276A, 0x276A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 782 */
  { 0x276B, 0x276B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 783 */
  { 0x276C, 0x276C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 784 */
  { 0x276D, 0x276D, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 785 */
  { 0x276E, 0x276E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 786 */
  { 0x276F, 0x276F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 787 */
  { 0x2770, 0x2770, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 788 */
  { 0x2771, 0x2771, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 789 */
  { 0x2772, 0x2772, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 790 */
  { 0x2773, 0x2773, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 791 */
  { 0x2774, 0x2774, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 792 */
  { 0x2775, 0x2775, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 793 */
  { 0x2794, 0x2794, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 794 */
  { 0x2795, 0x2797, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 795 */
  { 0x2798, 0x27AF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 796 */
  { 0x27B0, 0x27B0, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 797 */
  { 0x27B1, 0x27BE, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 798 */
  { 0x27BF, 0x27BF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 799 */
  { 0x27C0, 0x27C4, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 800 */
  { 0x27C5, 0x27C5, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 801 */
  { 0x27C6, 0x27C6, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 802 */
  { 0x27C7, 0x27CA, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 803 */
  { 0x27CB, 0x27CF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 804 */
  { 0x27D0, 0x27E5, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 805 */
  { 0x27E6, 0x27E6, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 806 */
  { 0x27E7, 0x27E7, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 807 */
  { 0x27E8, 0x27E8, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 808 */
  { 0x27E9, 0x27E9, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 809 */
  { 0x27EA, 0x27EA, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 810 */
  { 0x27EB, 0x27EB, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 811 */
  { 0x27EC, 0x27EF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 812 */
  { 0x27F0, 0x27FF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 813 */
  { 0x2800, 0x28FF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 814 */
  { 0x2900, 0x2982, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 815 */
  { 0x2983, 0x2983, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 816 */
  { 0x2984, 0x2984, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 817 */
  { 0x2985, 0x2985, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 818 */
  { 0x2986, 0x2986, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 819 */
  { 0x2987, 0x2987, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 820 */
  { 0x2988, 0x2988, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 821 */
  { 0x2989, 0x2989, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 822 */
  { 0x298A, 0x298A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 823 */
  { 0x298B, 0x298B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 824 */
  { 0x298C, 0x298C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 825 */
  { 0x298D, 0x298D, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 826 */
  { 0x298E, 0x298E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 827 */
  { 0x298F, 0x298F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 828 */
  { 0x2990, 0x2990, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 829 */
  { 0x2991, 0x2991, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 830 */
  { 0x2992, 0x2992, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 831 */
  { 0x2993, 0x2993, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 832 */
  { 0x2994, 0x2994, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 833 */
  { 0x2995, 0x2995, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 834 */
  { 0x2996, 0x2996, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 835 */
  { 0x2997, 0x2997, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 836 */
  { 0x2998, 0x2998, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 837 */
  { 0x2999, 0x29D7, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 838 */
  { 0x29D8, 0x29D8, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 839 */
  { 0x29D9, 0x29D9, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 840 */
  { 0x29DA, 0x29DA, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 841 */
  { 0x29DB, 0x29DB, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 842 */
  { 0x29DC, 0x29FB, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 843 */
  { 0x29FC, 0x29FC, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 844 */
  { 0x29FD, 0x29FD, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 845 */
  { 0x29FE, 0x2AFF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 846 */
  { 0x2B00, 0x2B1A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 847 */
  { 0x2B1B, 0x2B1F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 848 */
  { 0x2B20, 0x2B23, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 849 */
  { 0x2B24, 0x2BFF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 850 */
  { 0x2E00, 0x2E01, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 851 */
  { 0x2E02, 0x2E02, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 852 */
  { 0x2E03, 0x2E03, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 853 */
  { 0x2E04, 0x2E04, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 854 */
  { 0x2E05, 0x2E05, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 855 */
  { 0x2E06, 0x2E08, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 856 */
  { 0x2E09, 0x2E09, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 857 */
  { 0x2E0A, 0x2E0A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 858 */
  { 0x2E0B, 0x2E0B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 859 */
  { 0x2E0C, 0x2E0C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 860 */
  { 0x2E0D, 0x2E0D, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 861 */
  { 0x2E0E, 0x2E16, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 862 */
  { 0x2E17, 0x2E17, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 863 */
  { 0x2E18, 0x2E1B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 864 */
  { 0x2E1C, 0x2E1C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 865 */
  { 0x2E1D, 0x2E1D, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 866 */
  { 0x2E1E, 0x2E7F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 867 */
  { 0x3001, 0x3003, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 868 */
  { 0x3008, 0x3008, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 869 */
  { 0x3009, 0x3009, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 870 */
  { 0x300A, 0x300A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 871 */
  { 0x300B, 0x300B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 872 */
  { 0x300C, 0x300C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 873 */
  { 0x300D, 0x300D, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 874 */
  { 0x300E, 0x300E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 875 */
  { 0x300F, 0x300F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 876 */
  { 0x3010, 0x3010, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 877 */
  { 0x3011, 0x3011, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 878 */
  { 0x3012, 0x3013, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 879 */
  { 0x3014, 0x3014, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 880 */
  { 0x3015, 0x3015, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 881 */
  { 0x3016, 0x3016, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 882 */
  { 0x3017, 0x3017, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 883 */
  { 0x3018, 0x3018, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 884 */
  { 0x3019, 0x3019, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 885 */
  { 0x301A, 0x301A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 886 */
  { 0x301B, 0x301B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 887 */
  { 0x301C, 0x301C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 888 */
  { 0x301D, 0x301D, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 889 */
  { 0x301E, 0x301F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 890 */
  { 0x3020, 0x3020, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 891 */
  { 0x3030, 0x3030, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 892 */
  { 0xFD3E, 0xFD3E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 893 */
  { 0xFD3F, 0xFD3F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 894 */
  { 0xFE45, 0xFE46, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 895 */
};



static pdf_bool_t
pdf_text_ucd_pl_in_interval(pdf_u32_t character,
                            pdf_u32_t first_interval,
                            pdf_u32_t last_interval)
{
  extern unicode_proplist_info_t unicode_proplist_info[UCD_PL_INFO_N];
  int i;
  
  for(i=first_interval; i<=last_interval; ++i)
    {
      if((character >= unicode_proplist_info[i].interval_start) && \
         (character <= unicode_proplist_info[i].interval_stop))
        {
          return PDF_TRUE;
        }
    }
  return PDF_FALSE;
}


/* Returns true if the given UTF-32HE unicode point has the Soft_Dotted
 *  property */
pdf_bool_t
pdf_text_ucd_pl_is_Soft_Dotted(pdf_u32_t character)
{
  return pdf_text_ucd_pl_in_interval(character,
                                     UCD_PL_SOFT_DOTTED_F, \
                                     UCD_PL_SOFT_DOTTED_L);
}


/* NOTE: If needed, other properties can also be questioned like in 
 *  `pdf_text_ucd_pl_is_Soft_Dotted'. By now, this is not necessary. */


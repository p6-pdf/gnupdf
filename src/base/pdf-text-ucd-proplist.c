/* -*- mode: C -*- Time-stamp: ""
 *
 *       File:         pdf-text-ucd-proplist.c
 *       Date:         Mon Mar 31 19:58:17 2008
 *
 *       GNU PDF Library - Unicode Character Database - Propertly List
 *
 *    WARNING! The contents of the arrays in this file are self-generated
 *     from the UnicodeData.txt and SpecialCasing.txt files, using the
 *     `pdf_text_generate_ucd' utility
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

typedef struct _unicode_proplist_info_s {
  pdf_u32_t interval_start;
  pdf_u32_t interval_stop;
  enum pdf_text_ucd_proplist_e proplist;
} unicode_proplist_info_t;


/*************** START OF SELF-GENERATED DATA *********************************/

/* Indexes of intervals in array */
/* Index of first 'WHITE_SPACE' interval */
#define UCD_PL_WHITE_SPACE_F 0 
/* Index of last 'WHITE_SPACE' interval */
#define UCD_PL_WHITE_SPACE_L 11 
/* Index of first 'BIDI_CONTROL' interval */
#define UCD_PL_BIDI_CONTROL_F 12 
/* Index of last 'BIDI_CONTROL' interval */
#define UCD_PL_BIDI_CONTROL_L 13 
/* Index of first 'JOIN_CONTROL' interval */
#define UCD_PL_JOIN_CONTROL_F 14 
/* Index of last 'JOIN_CONTROL' interval */
#define UCD_PL_JOIN_CONTROL_L 14 
/* Index of first 'DASH' interval */
#define UCD_PL_DASH_F 15 
/* Index of last 'DASH' interval */
#define UCD_PL_DASH_L 32 
/* Index of first 'HYPHEN' interval */
#define UCD_PL_HYPHEN_F 33 
/* Index of last 'HYPHEN' interval */
#define UCD_PL_HYPHEN_L 42 
/* Index of first 'QUOTATION_MARK' interval */
#define UCD_PL_QUOTATION_MARK_F 43 
/* Index of last 'QUOTATION_MARK' interval */
#define UCD_PL_QUOTATION_MARK_L 69 
/* Index of first 'TERMINAL_PUNCTUATION' interval */
#define UCD_PL_TERMINAL_PUNCTUATION_F 70 
/* Index of last 'TERMINAL_PUNCTUATION' interval */
#define UCD_PL_TERMINAL_PUNCTUATION_L 124 
/* Index of first 'OTHER_MATH' interval */
#define UCD_PL_OTHER_MATH_F 125 
/* Index of last 'OTHER_MATH' interval */
#define UCD_PL_OTHER_MATH_L 262 
/* Index of first 'HEX_DIGIT' interval */
#define UCD_PL_HEX_DIGIT_F 263 
/* Index of last 'HEX_DIGIT' interval */
#define UCD_PL_HEX_DIGIT_L 268 
/* Index of first 'ASCII_HEX_DIGIT' interval */
#define UCD_PL_ASCII_HEX_DIGIT_F 269 
/* Index of last 'ASCII_HEX_DIGIT' interval */
#define UCD_PL_ASCII_HEX_DIGIT_L 271 
/* Index of first 'OTHER_ALPHABETIC' interval */
#define UCD_PL_OTHER_ALPHABETIC_F 272 
/* Index of last 'OTHER_ALPHABETIC' interval */
#define UCD_PL_OTHER_ALPHABETIC_L 457 
/* Index of first 'IDEOGRAPHIC' interval */
#define UCD_PL_IDEOGRAPHIC_F 458 
/* Index of last 'IDEOGRAPHIC' interval */
#define UCD_PL_IDEOGRAPHIC_L 468 
/* Index of first 'DIACRITIC' interval */
#define UCD_PL_DIACRITIC_F 469 
/* Index of last 'DIACRITIC' interval */
#define UCD_PL_DIACRITIC_L 588 
/* Index of first 'EXTENDER' interval */
#define UCD_PL_EXTENDER_F 589 
/* Index of last 'EXTENDER' interval */
#define UCD_PL_EXTENDER_L 604 
/* Index of first 'OTHER_LOWERCASE' interval */
#define UCD_PL_OTHER_LOWERCASE_F 605 
/* Index of last 'OTHER_LOWERCASE' interval */
#define UCD_PL_OTHER_LOWERCASE_L 617 
/* Index of first 'OTHER_UPPERCASE' interval */
#define UCD_PL_OTHER_UPPERCASE_F 618 
/* Index of last 'OTHER_UPPERCASE' interval */
#define UCD_PL_OTHER_UPPERCASE_L 619 
/* Index of first 'NONCHARACTER_CODE_POINT' interval */
#define UCD_PL_NONCHARACTER_CODE_POINT_F 620 
/* Index of last 'NONCHARACTER_CODE_POINT' interval */
#define UCD_PL_NONCHARACTER_CODE_POINT_L 637 
/* Index of first 'OTHER_GRAPHEME_EXTEND' interval */
#define UCD_PL_OTHER_GRAPHEME_EXTEND_F 638 
/* Index of last 'OTHER_GRAPHEME_EXTEND' interval */
#define UCD_PL_OTHER_GRAPHEME_EXTEND_L 653 
/* Index of first 'IDS_BINARY_OPERATOR' interval */
#define UCD_PL_IDS_BINARY_OPERATOR_F 654 
/* Index of last 'IDS_BINARY_OPERATOR' interval */
#define UCD_PL_IDS_BINARY_OPERATOR_L 655 
/* Index of first 'IDS_TRINARY_OPERATOR' interval */
#define UCD_PL_IDS_TRINARY_OPERATOR_F 656 
/* Index of last 'IDS_TRINARY_OPERATOR' interval */
#define UCD_PL_IDS_TRINARY_OPERATOR_L 656 
/* Index of first 'RADICAL' interval */
#define UCD_PL_RADICAL_F 657 
/* Index of last 'RADICAL' interval */
#define UCD_PL_RADICAL_L 659 
/* Index of first 'UNIFIED_IDEOGRAPH' interval */
#define UCD_PL_UNIFIED_IDEOGRAPH_F 660 
/* Index of last 'UNIFIED_IDEOGRAPH' interval */
#define UCD_PL_UNIFIED_IDEOGRAPH_L 669 
/* Index of first 'OTHER_DEFAULT_IGNORABLE_CODE_POINT' interval */
#define UCD_PL_OTHER_DEFAULT_IGNORABLE_CODE_POINT_F 670 
/* Index of last 'OTHER_DEFAULT_IGNORABLE_CODE_POINT' interval */
#define UCD_PL_OTHER_DEFAULT_IGNORABLE_CODE_POINT_L 679 
/* Index of first 'DEPRECATED' interval */
#define UCD_PL_DEPRECATED_F 680 
/* Index of last 'DEPRECATED' interval */
#define UCD_PL_DEPRECATED_L 685 
/* Index of first 'SOFT_DOTTED' interval */
#define UCD_PL_SOFT_DOTTED_F 686 
/* Index of last 'SOFT_DOTTED' interval */
#define UCD_PL_SOFT_DOTTED_L 716 
/* Index of first 'LOGICAL_ORDER_EXCEPTION' interval */
#define UCD_PL_LOGICAL_ORDER_EXCEPTION_F 717 
/* Index of last 'LOGICAL_ORDER_EXCEPTION' interval */
#define UCD_PL_LOGICAL_ORDER_EXCEPTION_L 718 
/* Index of first 'OTHER_ID_START' interval */
#define UCD_PL_OTHER_ID_START_F 719 
/* Index of last 'OTHER_ID_START' interval */
#define UCD_PL_OTHER_ID_START_L 721 
/* Index of first 'OTHER_ID_CONTINUE' interval */
#define UCD_PL_OTHER_ID_CONTINUE_F 722 
/* Index of last 'OTHER_ID_CONTINUE' interval */
#define UCD_PL_OTHER_ID_CONTINUE_L 724 
/* Index of first 'STERM' interval */
#define UCD_PL_STERM_F 725 
/* Index of last 'STERM' interval */
#define UCD_PL_STERM_L 761 
/* Index of first 'VARIATION_SELECTOR' interval */
#define UCD_PL_VARIATION_SELECTOR_F 762 
/* Index of last 'VARIATION_SELECTOR' interval */
#define UCD_PL_VARIATION_SELECTOR_L 764 
/* Index of first 'PATTERN_WHITE_SPACE' interval */
#define UCD_PL_PATTERN_WHITE_SPACE_F 765 
/* Index of last 'PATTERN_WHITE_SPACE' interval */
#define UCD_PL_PATTERN_WHITE_SPACE_L 770 
/* Index of first 'PATTERN_SYNTAX' interval */
#define UCD_PL_PATTERN_SYNTAX_F 771 
/* Index of last 'PATTERN_SYNTAX' interval */
#define UCD_PL_PATTERN_SYNTAX_L 1037 
#define UCD_PL_INFO_N   1038  /* Maximum number of elements in array    */


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
  { 0x2E1A, 0x2E1A, PDF_TEXT_UCD_PL_Dash  }, /* 25 */
  { 0x301C, 0x301C, PDF_TEXT_UCD_PL_Dash  }, /* 26 */
  { 0x3030, 0x3030, PDF_TEXT_UCD_PL_Dash  }, /* 27 */
  { 0x30A0, 0x30A0, PDF_TEXT_UCD_PL_Dash  }, /* 28 */
  { 0xFE31, 0xFE32, PDF_TEXT_UCD_PL_Dash  }, /* 29 */
  { 0xFE58, 0xFE58, PDF_TEXT_UCD_PL_Dash  }, /* 30 */
  { 0xFE63, 0xFE63, PDF_TEXT_UCD_PL_Dash  }, /* 31 */
  { 0xFF0D, 0xFF0D, PDF_TEXT_UCD_PL_Dash  }, /* 32 */
  { 0x002D, 0x002D, PDF_TEXT_UCD_PL_Hyphen  }, /* 33 */
  { 0x00AD, 0x00AD, PDF_TEXT_UCD_PL_Hyphen  }, /* 34 */
  { 0x058A, 0x058A, PDF_TEXT_UCD_PL_Hyphen  }, /* 35 */
  { 0x1806, 0x1806, PDF_TEXT_UCD_PL_Hyphen  }, /* 36 */
  { 0x2010, 0x2011, PDF_TEXT_UCD_PL_Hyphen  }, /* 37 */
  { 0x2E17, 0x2E17, PDF_TEXT_UCD_PL_Hyphen  }, /* 38 */
  { 0x30FB, 0x30FB, PDF_TEXT_UCD_PL_Hyphen  }, /* 39 */
  { 0xFE63, 0xFE63, PDF_TEXT_UCD_PL_Hyphen  }, /* 40 */
  { 0xFF0D, 0xFF0D, PDF_TEXT_UCD_PL_Hyphen  }, /* 41 */
  { 0xFF65, 0xFF65, PDF_TEXT_UCD_PL_Hyphen  }, /* 42 */
  { 0x0022, 0x0022, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 43 */
  { 0x0027, 0x0027, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 44 */
  { 0x00AB, 0x00AB, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 45 */
  { 0x00BB, 0x00BB, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 46 */
  { 0x2018, 0x2018, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 47 */
  { 0x2019, 0x2019, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 48 */
  { 0x201A, 0x201A, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 49 */
  { 0x201B, 0x201C, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 50 */
  { 0x201D, 0x201D, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 51 */
  { 0x201E, 0x201E, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 52 */
  { 0x201F, 0x201F, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 53 */
  { 0x2039, 0x2039, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 54 */
  { 0x203A, 0x203A, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 55 */
  { 0x300C, 0x300C, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 56 */
  { 0x300D, 0x300D, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 57 */
  { 0x300E, 0x300E, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 58 */
  { 0x300F, 0x300F, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 59 */
  { 0x301D, 0x301D, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 60 */
  { 0x301E, 0x301F, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 61 */
  { 0xFE41, 0xFE41, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 62 */
  { 0xFE42, 0xFE42, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 63 */
  { 0xFE43, 0xFE43, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 64 */
  { 0xFE44, 0xFE44, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 65 */
  { 0xFF02, 0xFF02, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 66 */
  { 0xFF07, 0xFF07, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 67 */
  { 0xFF62, 0xFF62, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 68 */
  { 0xFF63, 0xFF63, PDF_TEXT_UCD_PL_Quotation_Mark  }, /* 69 */
  { 0x0021, 0x0021, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 70 */
  { 0x002C, 0x002C, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 71 */
  { 0x002E, 0x002E, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 72 */
  { 0x003A, 0x003B, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 73 */
  { 0x003F, 0x003F, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 74 */
  { 0x037E, 0x037E, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 75 */
  { 0x0387, 0x0387, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 76 */
  { 0x0589, 0x0589, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 77 */
  { 0x05C3, 0x05C3, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 78 */
  { 0x060C, 0x060C, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 79 */
  { 0x061B, 0x061B, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 80 */
  { 0x061F, 0x061F, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 81 */
  { 0x06D4, 0x06D4, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 82 */
  { 0x0700, 0x070A, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 83 */
  { 0x070C, 0x070C, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 84 */
  { 0x07F8, 0x07F9, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 85 */
  { 0x0964, 0x0965, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 86 */
  { 0x0E5A, 0x0E5B, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 87 */
  { 0x0F08, 0x0F08, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 88 */
  { 0x0F0D, 0x0F12, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 89 */
  { 0x104A, 0x104B, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 90 */
  { 0x1361, 0x1368, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 91 */
  { 0x166D, 0x166E, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 92 */
  { 0x16EB, 0x16ED, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 93 */
  { 0x17D4, 0x17D6, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 94 */
  { 0x17DA, 0x17DA, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 95 */
  { 0x1802, 0x1805, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 96 */
  { 0x1808, 0x1809, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 97 */
  { 0x1944, 0x1945, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 98 */
  { 0x1B5A, 0x1B5B, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 99 */
  { 0x1B5D, 0x1B5F, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 100 */
  { 0x1C3B, 0x1C3F, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 101 */
  { 0x1C7E, 0x1C7F, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 102 */
  { 0x203C, 0x203D, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 103 */
  { 0x2047, 0x2049, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 104 */
  { 0x2E2E, 0x2E2E, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 105 */
  { 0x3001, 0x3002, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 106 */
  { 0xA60D, 0xA60F, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 107 */
  { 0xA876, 0xA877, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 108 */
  { 0xA8CE, 0xA8CF, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 109 */
  { 0xA92F, 0xA92F, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 110 */
  { 0xAA5D, 0xAA5F, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 111 */
  { 0xFE50, 0xFE52, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 112 */
  { 0xFE54, 0xFE57, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 113 */
  { 0xFF01, 0xFF01, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 114 */
  { 0xFF0C, 0xFF0C, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 115 */
  { 0xFF0E, 0xFF0E, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 116 */
  { 0xFF1A, 0xFF1B, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 117 */
  { 0xFF1F, 0xFF1F, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 118 */
  { 0xFF61, 0xFF61, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 119 */
  { 0xFF64, 0xFF64, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 120 */
  { 0x1039F, 0x1039F, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 121 */
  { 0x103D0, 0x103D0, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 122 */
  { 0x1091F, 0x1091F, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 123 */
  { 0x12470, 0x12473, PDF_TEXT_UCD_PL_Terminal_Punctuation  }, /* 124 */
  { 0x005E, 0x005E, PDF_TEXT_UCD_PL_Other_Math  }, /* 125 */
  { 0x03D0, 0x03D2, PDF_TEXT_UCD_PL_Other_Math  }, /* 126 */
  { 0x03D5, 0x03D5, PDF_TEXT_UCD_PL_Other_Math  }, /* 127 */
  { 0x03F0, 0x03F1, PDF_TEXT_UCD_PL_Other_Math  }, /* 128 */
  { 0x03F4, 0x03F5, PDF_TEXT_UCD_PL_Other_Math  }, /* 129 */
  { 0x2016, 0x2016, PDF_TEXT_UCD_PL_Other_Math  }, /* 130 */
  { 0x2032, 0x2034, PDF_TEXT_UCD_PL_Other_Math  }, /* 131 */
  { 0x2040, 0x2040, PDF_TEXT_UCD_PL_Other_Math  }, /* 132 */
  { 0x2061, 0x2064, PDF_TEXT_UCD_PL_Other_Math  }, /* 133 */
  { 0x207D, 0x207D, PDF_TEXT_UCD_PL_Other_Math  }, /* 134 */
  { 0x207E, 0x207E, PDF_TEXT_UCD_PL_Other_Math  }, /* 135 */
  { 0x208D, 0x208D, PDF_TEXT_UCD_PL_Other_Math  }, /* 136 */
  { 0x208E, 0x208E, PDF_TEXT_UCD_PL_Other_Math  }, /* 137 */
  { 0x20D0, 0x20DC, PDF_TEXT_UCD_PL_Other_Math  }, /* 138 */
  { 0x20E1, 0x20E1, PDF_TEXT_UCD_PL_Other_Math  }, /* 139 */
  { 0x20E5, 0x20E6, PDF_TEXT_UCD_PL_Other_Math  }, /* 140 */
  { 0x20EB, 0x20EF, PDF_TEXT_UCD_PL_Other_Math  }, /* 141 */
  { 0x2102, 0x2102, PDF_TEXT_UCD_PL_Other_Math  }, /* 142 */
  { 0x210A, 0x2113, PDF_TEXT_UCD_PL_Other_Math  }, /* 143 */
  { 0x2115, 0x2115, PDF_TEXT_UCD_PL_Other_Math  }, /* 144 */
  { 0x2119, 0x211D, PDF_TEXT_UCD_PL_Other_Math  }, /* 145 */
  { 0x2124, 0x2124, PDF_TEXT_UCD_PL_Other_Math  }, /* 146 */
  { 0x2128, 0x2128, PDF_TEXT_UCD_PL_Other_Math  }, /* 147 */
  { 0x2129, 0x2129, PDF_TEXT_UCD_PL_Other_Math  }, /* 148 */
  { 0x212C, 0x212D, PDF_TEXT_UCD_PL_Other_Math  }, /* 149 */
  { 0x212F, 0x2131, PDF_TEXT_UCD_PL_Other_Math  }, /* 150 */
  { 0x2133, 0x2134, PDF_TEXT_UCD_PL_Other_Math  }, /* 151 */
  { 0x2135, 0x2138, PDF_TEXT_UCD_PL_Other_Math  }, /* 152 */
  { 0x213C, 0x213F, PDF_TEXT_UCD_PL_Other_Math  }, /* 153 */
  { 0x2145, 0x2149, PDF_TEXT_UCD_PL_Other_Math  }, /* 154 */
  { 0x2195, 0x2199, PDF_TEXT_UCD_PL_Other_Math  }, /* 155 */
  { 0x219C, 0x219F, PDF_TEXT_UCD_PL_Other_Math  }, /* 156 */
  { 0x21A1, 0x21A2, PDF_TEXT_UCD_PL_Other_Math  }, /* 157 */
  { 0x21A4, 0x21A5, PDF_TEXT_UCD_PL_Other_Math  }, /* 158 */
  { 0x21A7, 0x21A7, PDF_TEXT_UCD_PL_Other_Math  }, /* 159 */
  { 0x21A9, 0x21AD, PDF_TEXT_UCD_PL_Other_Math  }, /* 160 */
  { 0x21B0, 0x21B1, PDF_TEXT_UCD_PL_Other_Math  }, /* 161 */
  { 0x21B6, 0x21B7, PDF_TEXT_UCD_PL_Other_Math  }, /* 162 */
  { 0x21BC, 0x21CD, PDF_TEXT_UCD_PL_Other_Math  }, /* 163 */
  { 0x21D0, 0x21D1, PDF_TEXT_UCD_PL_Other_Math  }, /* 164 */
  { 0x21D3, 0x21D3, PDF_TEXT_UCD_PL_Other_Math  }, /* 165 */
  { 0x21D5, 0x21DB, PDF_TEXT_UCD_PL_Other_Math  }, /* 166 */
  { 0x21DD, 0x21DD, PDF_TEXT_UCD_PL_Other_Math  }, /* 167 */
  { 0x21E4, 0x21E5, PDF_TEXT_UCD_PL_Other_Math  }, /* 168 */
  { 0x23B4, 0x23B5, PDF_TEXT_UCD_PL_Other_Math  }, /* 169 */
  { 0x23B7, 0x23B7, PDF_TEXT_UCD_PL_Other_Math  }, /* 170 */
  { 0x23D0, 0x23D0, PDF_TEXT_UCD_PL_Other_Math  }, /* 171 */
  { 0x23E2, 0x23E2, PDF_TEXT_UCD_PL_Other_Math  }, /* 172 */
  { 0x25A0, 0x25A1, PDF_TEXT_UCD_PL_Other_Math  }, /* 173 */
  { 0x25AE, 0x25B6, PDF_TEXT_UCD_PL_Other_Math  }, /* 174 */
  { 0x25BC, 0x25C0, PDF_TEXT_UCD_PL_Other_Math  }, /* 175 */
  { 0x25C6, 0x25C7, PDF_TEXT_UCD_PL_Other_Math  }, /* 176 */
  { 0x25CA, 0x25CB, PDF_TEXT_UCD_PL_Other_Math  }, /* 177 */
  { 0x25CF, 0x25D3, PDF_TEXT_UCD_PL_Other_Math  }, /* 178 */
  { 0x25E2, 0x25E2, PDF_TEXT_UCD_PL_Other_Math  }, /* 179 */
  { 0x25E4, 0x25E4, PDF_TEXT_UCD_PL_Other_Math  }, /* 180 */
  { 0x25E7, 0x25EC, PDF_TEXT_UCD_PL_Other_Math  }, /* 181 */
  { 0x2605, 0x2606, PDF_TEXT_UCD_PL_Other_Math  }, /* 182 */
  { 0x2640, 0x2640, PDF_TEXT_UCD_PL_Other_Math  }, /* 183 */
  { 0x2642, 0x2642, PDF_TEXT_UCD_PL_Other_Math  }, /* 184 */
  { 0x2660, 0x2663, PDF_TEXT_UCD_PL_Other_Math  }, /* 185 */
  { 0x266D, 0x266E, PDF_TEXT_UCD_PL_Other_Math  }, /* 186 */
  { 0x27C5, 0x27C5, PDF_TEXT_UCD_PL_Other_Math  }, /* 187 */
  { 0x27C6, 0x27C6, PDF_TEXT_UCD_PL_Other_Math  }, /* 188 */
  { 0x27E6, 0x27E6, PDF_TEXT_UCD_PL_Other_Math  }, /* 189 */
  { 0x27E7, 0x27E7, PDF_TEXT_UCD_PL_Other_Math  }, /* 190 */
  { 0x27E8, 0x27E8, PDF_TEXT_UCD_PL_Other_Math  }, /* 191 */
  { 0x27E9, 0x27E9, PDF_TEXT_UCD_PL_Other_Math  }, /* 192 */
  { 0x27EA, 0x27EA, PDF_TEXT_UCD_PL_Other_Math  }, /* 193 */
  { 0x27EB, 0x27EB, PDF_TEXT_UCD_PL_Other_Math  }, /* 194 */
  { 0x27EC, 0x27EC, PDF_TEXT_UCD_PL_Other_Math  }, /* 195 */
  { 0x27ED, 0x27ED, PDF_TEXT_UCD_PL_Other_Math  }, /* 196 */
  { 0x27EE, 0x27EE, PDF_TEXT_UCD_PL_Other_Math  }, /* 197 */
  { 0x27EF, 0x27EF, PDF_TEXT_UCD_PL_Other_Math  }, /* 198 */
  { 0x2983, 0x2983, PDF_TEXT_UCD_PL_Other_Math  }, /* 199 */
  { 0x2984, 0x2984, PDF_TEXT_UCD_PL_Other_Math  }, /* 200 */
  { 0x2985, 0x2985, PDF_TEXT_UCD_PL_Other_Math  }, /* 201 */
  { 0x2986, 0x2986, PDF_TEXT_UCD_PL_Other_Math  }, /* 202 */
  { 0x2987, 0x2987, PDF_TEXT_UCD_PL_Other_Math  }, /* 203 */
  { 0x2988, 0x2988, PDF_TEXT_UCD_PL_Other_Math  }, /* 204 */
  { 0x2989, 0x2989, PDF_TEXT_UCD_PL_Other_Math  }, /* 205 */
  { 0x298A, 0x298A, PDF_TEXT_UCD_PL_Other_Math  }, /* 206 */
  { 0x298B, 0x298B, PDF_TEXT_UCD_PL_Other_Math  }, /* 207 */
  { 0x298C, 0x298C, PDF_TEXT_UCD_PL_Other_Math  }, /* 208 */
  { 0x298D, 0x298D, PDF_TEXT_UCD_PL_Other_Math  }, /* 209 */
  { 0x298E, 0x298E, PDF_TEXT_UCD_PL_Other_Math  }, /* 210 */
  { 0x298F, 0x298F, PDF_TEXT_UCD_PL_Other_Math  }, /* 211 */
  { 0x2990, 0x2990, PDF_TEXT_UCD_PL_Other_Math  }, /* 212 */
  { 0x2991, 0x2991, PDF_TEXT_UCD_PL_Other_Math  }, /* 213 */
  { 0x2992, 0x2992, PDF_TEXT_UCD_PL_Other_Math  }, /* 214 */
  { 0x2993, 0x2993, PDF_TEXT_UCD_PL_Other_Math  }, /* 215 */
  { 0x2994, 0x2994, PDF_TEXT_UCD_PL_Other_Math  }, /* 216 */
  { 0x2995, 0x2995, PDF_TEXT_UCD_PL_Other_Math  }, /* 217 */
  { 0x2996, 0x2996, PDF_TEXT_UCD_PL_Other_Math  }, /* 218 */
  { 0x2997, 0x2997, PDF_TEXT_UCD_PL_Other_Math  }, /* 219 */
  { 0x2998, 0x2998, PDF_TEXT_UCD_PL_Other_Math  }, /* 220 */
  { 0x29D8, 0x29D8, PDF_TEXT_UCD_PL_Other_Math  }, /* 221 */
  { 0x29D9, 0x29D9, PDF_TEXT_UCD_PL_Other_Math  }, /* 222 */
  { 0x29DA, 0x29DA, PDF_TEXT_UCD_PL_Other_Math  }, /* 223 */
  { 0x29DB, 0x29DB, PDF_TEXT_UCD_PL_Other_Math  }, /* 224 */
  { 0x29FC, 0x29FC, PDF_TEXT_UCD_PL_Other_Math  }, /* 225 */
  { 0x29FD, 0x29FD, PDF_TEXT_UCD_PL_Other_Math  }, /* 226 */
  { 0xFE61, 0xFE61, PDF_TEXT_UCD_PL_Other_Math  }, /* 227 */
  { 0xFE63, 0xFE63, PDF_TEXT_UCD_PL_Other_Math  }, /* 228 */
  { 0xFE68, 0xFE68, PDF_TEXT_UCD_PL_Other_Math  }, /* 229 */
  { 0xFF3C, 0xFF3C, PDF_TEXT_UCD_PL_Other_Math  }, /* 230 */
  { 0xFF3E, 0xFF3E, PDF_TEXT_UCD_PL_Other_Math  }, /* 231 */
  { 0x1D400, 0x1D454, PDF_TEXT_UCD_PL_Other_Math  }, /* 232 */
  { 0x1D456, 0x1D49C, PDF_TEXT_UCD_PL_Other_Math  }, /* 233 */
  { 0x1D49E, 0x1D49F, PDF_TEXT_UCD_PL_Other_Math  }, /* 234 */
  { 0x1D4A2, 0x1D4A2, PDF_TEXT_UCD_PL_Other_Math  }, /* 235 */
  { 0x1D4A5, 0x1D4A6, PDF_TEXT_UCD_PL_Other_Math  }, /* 236 */
  { 0x1D4A9, 0x1D4AC, PDF_TEXT_UCD_PL_Other_Math  }, /* 237 */
  { 0x1D4AE, 0x1D4B9, PDF_TEXT_UCD_PL_Other_Math  }, /* 238 */
  { 0x1D4BB, 0x1D4BB, PDF_TEXT_UCD_PL_Other_Math  }, /* 239 */
  { 0x1D4BD, 0x1D4C3, PDF_TEXT_UCD_PL_Other_Math  }, /* 240 */
  { 0x1D4C5, 0x1D505, PDF_TEXT_UCD_PL_Other_Math  }, /* 241 */
  { 0x1D507, 0x1D50A, PDF_TEXT_UCD_PL_Other_Math  }, /* 242 */
  { 0x1D50D, 0x1D514, PDF_TEXT_UCD_PL_Other_Math  }, /* 243 */
  { 0x1D516, 0x1D51C, PDF_TEXT_UCD_PL_Other_Math  }, /* 244 */
  { 0x1D51E, 0x1D539, PDF_TEXT_UCD_PL_Other_Math  }, /* 245 */
  { 0x1D53B, 0x1D53E, PDF_TEXT_UCD_PL_Other_Math  }, /* 246 */
  { 0x1D540, 0x1D544, PDF_TEXT_UCD_PL_Other_Math  }, /* 247 */
  { 0x1D546, 0x1D546, PDF_TEXT_UCD_PL_Other_Math  }, /* 248 */
  { 0x1D54A, 0x1D550, PDF_TEXT_UCD_PL_Other_Math  }, /* 249 */
  { 0x1D552, 0x1D6A5, PDF_TEXT_UCD_PL_Other_Math  }, /* 250 */
  { 0x1D6A8, 0x1D6C0, PDF_TEXT_UCD_PL_Other_Math  }, /* 251 */
  { 0x1D6C2, 0x1D6DA, PDF_TEXT_UCD_PL_Other_Math  }, /* 252 */
  { 0x1D6DC, 0x1D6FA, PDF_TEXT_UCD_PL_Other_Math  }, /* 253 */
  { 0x1D6FC, 0x1D714, PDF_TEXT_UCD_PL_Other_Math  }, /* 254 */
  { 0x1D716, 0x1D734, PDF_TEXT_UCD_PL_Other_Math  }, /* 255 */
  { 0x1D736, 0x1D74E, PDF_TEXT_UCD_PL_Other_Math  }, /* 256 */
  { 0x1D750, 0x1D76E, PDF_TEXT_UCD_PL_Other_Math  }, /* 257 */
  { 0x1D770, 0x1D788, PDF_TEXT_UCD_PL_Other_Math  }, /* 258 */
  { 0x1D78A, 0x1D7A8, PDF_TEXT_UCD_PL_Other_Math  }, /* 259 */
  { 0x1D7AA, 0x1D7C2, PDF_TEXT_UCD_PL_Other_Math  }, /* 260 */
  { 0x1D7C4, 0x1D7CB, PDF_TEXT_UCD_PL_Other_Math  }, /* 261 */
  { 0x1D7CE, 0x1D7FF, PDF_TEXT_UCD_PL_Other_Math  }, /* 262 */
  { 0x0030, 0x0039, PDF_TEXT_UCD_PL_Hex_Digit  }, /* 263 */
  { 0x0041, 0x0046, PDF_TEXT_UCD_PL_Hex_Digit  }, /* 264 */
  { 0x0061, 0x0066, PDF_TEXT_UCD_PL_Hex_Digit  }, /* 265 */
  { 0xFF10, 0xFF19, PDF_TEXT_UCD_PL_Hex_Digit  }, /* 266 */
  { 0xFF21, 0xFF26, PDF_TEXT_UCD_PL_Hex_Digit  }, /* 267 */
  { 0xFF41, 0xFF46, PDF_TEXT_UCD_PL_Hex_Digit  }, /* 268 */
  { 0x0030, 0x0039, PDF_TEXT_UCD_PL_ASCII_Hex_Digit  }, /* 269 */
  { 0x0041, 0x0046, PDF_TEXT_UCD_PL_ASCII_Hex_Digit  }, /* 270 */
  { 0x0061, 0x0066, PDF_TEXT_UCD_PL_ASCII_Hex_Digit  }, /* 271 */
  { 0x0345, 0x0345, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 272 */
  { 0x05B0, 0x05BD, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 273 */
  { 0x05BF, 0x05BF, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 274 */
  { 0x05C1, 0x05C2, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 275 */
  { 0x05C4, 0x05C5, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 276 */
  { 0x05C7, 0x05C7, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 277 */
  { 0x0610, 0x061A, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 278 */
  { 0x064B, 0x0657, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 279 */
  { 0x0659, 0x065E, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 280 */
  { 0x0670, 0x0670, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 281 */
  { 0x06D6, 0x06DC, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 282 */
  { 0x06E1, 0x06E4, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 283 */
  { 0x06E7, 0x06E8, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 284 */
  { 0x06ED, 0x06ED, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 285 */
  { 0x0711, 0x0711, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 286 */
  { 0x0730, 0x073F, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 287 */
  { 0x07A6, 0x07B0, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 288 */
  { 0x0901, 0x0902, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 289 */
  { 0x0903, 0x0903, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 290 */
  { 0x093E, 0x0940, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 291 */
  { 0x0941, 0x0948, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 292 */
  { 0x0949, 0x094C, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 293 */
  { 0x0962, 0x0963, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 294 */
  { 0x0981, 0x0981, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 295 */
  { 0x0982, 0x0983, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 296 */
  { 0x09BE, 0x09C0, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 297 */
  { 0x09C1, 0x09C4, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 298 */
  { 0x09C7, 0x09C8, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 299 */
  { 0x09CB, 0x09CC, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 300 */
  { 0x09D7, 0x09D7, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 301 */
  { 0x09E2, 0x09E3, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 302 */
  { 0x0A01, 0x0A02, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 303 */
  { 0x0A03, 0x0A03, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 304 */
  { 0x0A3E, 0x0A40, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 305 */
  { 0x0A41, 0x0A42, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 306 */
  { 0x0A47, 0x0A48, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 307 */
  { 0x0A4B, 0x0A4C, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 308 */
  { 0x0A51, 0x0A51, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 309 */
  { 0x0A70, 0x0A71, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 310 */
  { 0x0A75, 0x0A75, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 311 */
  { 0x0A81, 0x0A82, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 312 */
  { 0x0A83, 0x0A83, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 313 */
  { 0x0ABE, 0x0AC0, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 314 */
  { 0x0AC1, 0x0AC5, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 315 */
  { 0x0AC7, 0x0AC8, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 316 */
  { 0x0AC9, 0x0AC9, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 317 */
  { 0x0ACB, 0x0ACC, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 318 */
  { 0x0AE2, 0x0AE3, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 319 */
  { 0x0B01, 0x0B01, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 320 */
  { 0x0B02, 0x0B03, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 321 */
  { 0x0B3E, 0x0B3E, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 322 */
  { 0x0B3F, 0x0B3F, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 323 */
  { 0x0B40, 0x0B40, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 324 */
  { 0x0B41, 0x0B44, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 325 */
  { 0x0B47, 0x0B48, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 326 */
  { 0x0B4B, 0x0B4C, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 327 */
  { 0x0B56, 0x0B56, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 328 */
  { 0x0B57, 0x0B57, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 329 */
  { 0x0B62, 0x0B63, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 330 */
  { 0x0B82, 0x0B82, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 331 */
  { 0x0BBE, 0x0BBF, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 332 */
  { 0x0BC0, 0x0BC0, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 333 */
  { 0x0BC1, 0x0BC2, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 334 */
  { 0x0BC6, 0x0BC8, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 335 */
  { 0x0BCA, 0x0BCC, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 336 */
  { 0x0BD7, 0x0BD7, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 337 */
  { 0x0C01, 0x0C03, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 338 */
  { 0x0C3E, 0x0C40, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 339 */
  { 0x0C41, 0x0C44, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 340 */
  { 0x0C46, 0x0C48, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 341 */
  { 0x0C4A, 0x0C4C, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 342 */
  { 0x0C55, 0x0C56, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 343 */
  { 0x0C62, 0x0C63, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 344 */
  { 0x0C82, 0x0C83, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 345 */
  { 0x0CBE, 0x0CBE, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 346 */
  { 0x0CBF, 0x0CBF, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 347 */
  { 0x0CC0, 0x0CC4, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 348 */
  { 0x0CC6, 0x0CC6, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 349 */
  { 0x0CC7, 0x0CC8, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 350 */
  { 0x0CCA, 0x0CCB, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 351 */
  { 0x0CCC, 0x0CCC, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 352 */
  { 0x0CD5, 0x0CD6, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 353 */
  { 0x0CE2, 0x0CE3, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 354 */
  { 0x0D02, 0x0D03, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 355 */
  { 0x0D3E, 0x0D40, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 356 */
  { 0x0D41, 0x0D44, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 357 */
  { 0x0D46, 0x0D48, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 358 */
  { 0x0D4A, 0x0D4C, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 359 */
  { 0x0D57, 0x0D57, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 360 */
  { 0x0D62, 0x0D63, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 361 */
  { 0x0D82, 0x0D83, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 362 */
  { 0x0DCF, 0x0DD1, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 363 */
  { 0x0DD2, 0x0DD4, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 364 */
  { 0x0DD6, 0x0DD6, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 365 */
  { 0x0DD8, 0x0DDF, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 366 */
  { 0x0DF2, 0x0DF3, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 367 */
  { 0x0E31, 0x0E31, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 368 */
  { 0x0E34, 0x0E3A, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 369 */
  { 0x0E4D, 0x0E4D, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 370 */
  { 0x0EB1, 0x0EB1, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 371 */
  { 0x0EB4, 0x0EB9, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 372 */
  { 0x0EBB, 0x0EBC, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 373 */
  { 0x0ECD, 0x0ECD, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 374 */
  { 0x0F71, 0x0F7E, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 375 */
  { 0x0F7F, 0x0F7F, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 376 */
  { 0x0F80, 0x0F81, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 377 */
  { 0x0F90, 0x0F97, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 378 */
  { 0x0F99, 0x0FBC, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 379 */
  { 0x102B, 0x102C, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 380 */
  { 0x102D, 0x1030, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 381 */
  { 0x1031, 0x1031, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 382 */
  { 0x1032, 0x1036, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 383 */
  { 0x1038, 0x1038, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 384 */
  { 0x103B, 0x103C, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 385 */
  { 0x103D, 0x103E, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 386 */
  { 0x1056, 0x1057, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 387 */
  { 0x1058, 0x1059, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 388 */
  { 0x105E, 0x1060, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 389 */
  { 0x1062, 0x1062, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 390 */
  { 0x1067, 0x1068, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 391 */
  { 0x1071, 0x1074, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 392 */
  { 0x1082, 0x1082, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 393 */
  { 0x1083, 0x1084, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 394 */
  { 0x1085, 0x1086, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 395 */
  { 0x135F, 0x135F, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 396 */
  { 0x1712, 0x1713, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 397 */
  { 0x1732, 0x1733, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 398 */
  { 0x1752, 0x1753, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 399 */
  { 0x1772, 0x1773, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 400 */
  { 0x17B6, 0x17B6, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 401 */
  { 0x17B7, 0x17BD, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 402 */
  { 0x17BE, 0x17C5, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 403 */
  { 0x17C6, 0x17C6, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 404 */
  { 0x17C7, 0x17C8, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 405 */
  { 0x18A9, 0x18A9, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 406 */
  { 0x1920, 0x1922, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 407 */
  { 0x1923, 0x1926, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 408 */
  { 0x1927, 0x1928, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 409 */
  { 0x1929, 0x192B, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 410 */
  { 0x1930, 0x1931, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 411 */
  { 0x1932, 0x1932, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 412 */
  { 0x1933, 0x1938, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 413 */
  { 0x19B0, 0x19C0, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 414 */
  { 0x19C8, 0x19C9, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 415 */
  { 0x1A17, 0x1A18, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 416 */
  { 0x1A19, 0x1A1B, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 417 */
  { 0x1B00, 0x1B03, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 418 */
  { 0x1B04, 0x1B04, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 419 */
  { 0x1B35, 0x1B35, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 420 */
  { 0x1B36, 0x1B3A, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 421 */
  { 0x1B3B, 0x1B3B, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 422 */
  { 0x1B3C, 0x1B3C, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 423 */
  { 0x1B3D, 0x1B41, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 424 */
  { 0x1B42, 0x1B42, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 425 */
  { 0x1B43, 0x1B43, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 426 */
  { 0x1B80, 0x1B81, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 427 */
  { 0x1B82, 0x1B82, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 428 */
  { 0x1BA1, 0x1BA1, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 429 */
  { 0x1BA2, 0x1BA5, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 430 */
  { 0x1BA6, 0x1BA7, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 431 */
  { 0x1BA8, 0x1BA9, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 432 */
  { 0x1C24, 0x1C2B, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 433 */
  { 0x1C2C, 0x1C33, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 434 */
  { 0x1C34, 0x1C35, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 435 */
  { 0x24B6, 0x24E9, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 436 */
  { 0x2DE0, 0x2DFF, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 437 */
  { 0xA823, 0xA824, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 438 */
  { 0xA825, 0xA826, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 439 */
  { 0xA827, 0xA827, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 440 */
  { 0xA880, 0xA881, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 441 */
  { 0xA8B4, 0xA8C3, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 442 */
  { 0xA926, 0xA92A, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 443 */
  { 0xA947, 0xA951, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 444 */
  { 0xA952, 0xA952, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 445 */
  { 0xAA29, 0xAA2E, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 446 */
  { 0xAA2F, 0xAA30, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 447 */
  { 0xAA31, 0xAA32, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 448 */
  { 0xAA33, 0xAA34, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 449 */
  { 0xAA35, 0xAA36, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 450 */
  { 0xAA43, 0xAA43, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 451 */
  { 0xAA4C, 0xAA4C, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 452 */
  { 0xAA4D, 0xAA4D, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 453 */
  { 0xFB1E, 0xFB1E, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 454 */
  { 0x10A01, 0x10A03, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 455 */
  { 0x10A05, 0x10A06, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 456 */
  { 0x10A0C, 0x10A0F, PDF_TEXT_UCD_PL_Other_Alphabetic  }, /* 457 */
  { 0x3006, 0x3006, PDF_TEXT_UCD_PL_Ideographic  }, /* 458 */
  { 0x3007, 0x3007, PDF_TEXT_UCD_PL_Ideographic  }, /* 459 */
  { 0x3021, 0x3029, PDF_TEXT_UCD_PL_Ideographic  }, /* 460 */
  { 0x3038, 0x303A, PDF_TEXT_UCD_PL_Ideographic  }, /* 461 */
  { 0x3400, 0x4DB5, PDF_TEXT_UCD_PL_Ideographic  }, /* 462 */
  { 0x4E00, 0x9FC3, PDF_TEXT_UCD_PL_Ideographic  }, /* 463 */
  { 0xF900, 0xFA2D, PDF_TEXT_UCD_PL_Ideographic  }, /* 464 */
  { 0xFA30, 0xFA6A, PDF_TEXT_UCD_PL_Ideographic  }, /* 465 */
  { 0xFA70, 0xFAD9, PDF_TEXT_UCD_PL_Ideographic  }, /* 466 */
  { 0x20000, 0x2A6D6, PDF_TEXT_UCD_PL_Ideographic  }, /* 467 */
  { 0x2F800, 0x2FA1D, PDF_TEXT_UCD_PL_Ideographic  }, /* 468 */
  { 0x005E, 0x005E, PDF_TEXT_UCD_PL_Diacritic  }, /* 469 */
  { 0x0060, 0x0060, PDF_TEXT_UCD_PL_Diacritic  }, /* 470 */
  { 0x00A8, 0x00A8, PDF_TEXT_UCD_PL_Diacritic  }, /* 471 */
  { 0x00AF, 0x00AF, PDF_TEXT_UCD_PL_Diacritic  }, /* 472 */
  { 0x00B4, 0x00B4, PDF_TEXT_UCD_PL_Diacritic  }, /* 473 */
  { 0x00B7, 0x00B7, PDF_TEXT_UCD_PL_Diacritic  }, /* 474 */
  { 0x00B8, 0x00B8, PDF_TEXT_UCD_PL_Diacritic  }, /* 475 */
  { 0x02B0, 0x02C1, PDF_TEXT_UCD_PL_Diacritic  }, /* 476 */
  { 0x02C2, 0x02C5, PDF_TEXT_UCD_PL_Diacritic  }, /* 477 */
  { 0x02C6, 0x02D1, PDF_TEXT_UCD_PL_Diacritic  }, /* 478 */
  { 0x02D2, 0x02DF, PDF_TEXT_UCD_PL_Diacritic  }, /* 479 */
  { 0x02E0, 0x02E4, PDF_TEXT_UCD_PL_Diacritic  }, /* 480 */
  { 0x02E5, 0x02EB, PDF_TEXT_UCD_PL_Diacritic  }, /* 481 */
  { 0x02EC, 0x02EC, PDF_TEXT_UCD_PL_Diacritic  }, /* 482 */
  { 0x02ED, 0x02ED, PDF_TEXT_UCD_PL_Diacritic  }, /* 483 */
  { 0x02EE, 0x02EE, PDF_TEXT_UCD_PL_Diacritic  }, /* 484 */
  { 0x02EF, 0x02FF, PDF_TEXT_UCD_PL_Diacritic  }, /* 485 */
  { 0x0300, 0x034E, PDF_TEXT_UCD_PL_Diacritic  }, /* 486 */
  { 0x0350, 0x0357, PDF_TEXT_UCD_PL_Diacritic  }, /* 487 */
  { 0x035D, 0x0362, PDF_TEXT_UCD_PL_Diacritic  }, /* 488 */
  { 0x0374, 0x0374, PDF_TEXT_UCD_PL_Diacritic  }, /* 489 */
  { 0x0375, 0x0375, PDF_TEXT_UCD_PL_Diacritic  }, /* 490 */
  { 0x037A, 0x037A, PDF_TEXT_UCD_PL_Diacritic  }, /* 491 */
  { 0x0384, 0x0385, PDF_TEXT_UCD_PL_Diacritic  }, /* 492 */
  { 0x0483, 0x0487, PDF_TEXT_UCD_PL_Diacritic  }, /* 493 */
  { 0x0559, 0x0559, PDF_TEXT_UCD_PL_Diacritic  }, /* 494 */
  { 0x0591, 0x05A1, PDF_TEXT_UCD_PL_Diacritic  }, /* 495 */
  { 0x05A3, 0x05BD, PDF_TEXT_UCD_PL_Diacritic  }, /* 496 */
  { 0x05BF, 0x05BF, PDF_TEXT_UCD_PL_Diacritic  }, /* 497 */
  { 0x05C1, 0x05C2, PDF_TEXT_UCD_PL_Diacritic  }, /* 498 */
  { 0x05C4, 0x05C4, PDF_TEXT_UCD_PL_Diacritic  }, /* 499 */
  { 0x064B, 0x0652, PDF_TEXT_UCD_PL_Diacritic  }, /* 500 */
  { 0x0657, 0x0658, PDF_TEXT_UCD_PL_Diacritic  }, /* 501 */
  { 0x06DF, 0x06E0, PDF_TEXT_UCD_PL_Diacritic  }, /* 502 */
  { 0x06E5, 0x06E6, PDF_TEXT_UCD_PL_Diacritic  }, /* 503 */
  { 0x06EA, 0x06EC, PDF_TEXT_UCD_PL_Diacritic  }, /* 504 */
  { 0x0730, 0x074A, PDF_TEXT_UCD_PL_Diacritic  }, /* 505 */
  { 0x07A6, 0x07B0, PDF_TEXT_UCD_PL_Diacritic  }, /* 506 */
  { 0x07EB, 0x07F3, PDF_TEXT_UCD_PL_Diacritic  }, /* 507 */
  { 0x07F4, 0x07F5, PDF_TEXT_UCD_PL_Diacritic  }, /* 508 */
  { 0x093C, 0x093C, PDF_TEXT_UCD_PL_Diacritic  }, /* 509 */
  { 0x094D, 0x094D, PDF_TEXT_UCD_PL_Diacritic  }, /* 510 */
  { 0x0951, 0x0954, PDF_TEXT_UCD_PL_Diacritic  }, /* 511 */
  { 0x0971, 0x0971, PDF_TEXT_UCD_PL_Diacritic  }, /* 512 */
  { 0x09BC, 0x09BC, PDF_TEXT_UCD_PL_Diacritic  }, /* 513 */
  { 0x09CD, 0x09CD, PDF_TEXT_UCD_PL_Diacritic  }, /* 514 */
  { 0x0A3C, 0x0A3C, PDF_TEXT_UCD_PL_Diacritic  }, /* 515 */
  { 0x0A4D, 0x0A4D, PDF_TEXT_UCD_PL_Diacritic  }, /* 516 */
  { 0x0ABC, 0x0ABC, PDF_TEXT_UCD_PL_Diacritic  }, /* 517 */
  { 0x0ACD, 0x0ACD, PDF_TEXT_UCD_PL_Diacritic  }, /* 518 */
  { 0x0B3C, 0x0B3C, PDF_TEXT_UCD_PL_Diacritic  }, /* 519 */
  { 0x0B4D, 0x0B4D, PDF_TEXT_UCD_PL_Diacritic  }, /* 520 */
  { 0x0BCD, 0x0BCD, PDF_TEXT_UCD_PL_Diacritic  }, /* 521 */
  { 0x0C4D, 0x0C4D, PDF_TEXT_UCD_PL_Diacritic  }, /* 522 */
  { 0x0CBC, 0x0CBC, PDF_TEXT_UCD_PL_Diacritic  }, /* 523 */
  { 0x0CCD, 0x0CCD, PDF_TEXT_UCD_PL_Diacritic  }, /* 524 */
  { 0x0D4D, 0x0D4D, PDF_TEXT_UCD_PL_Diacritic  }, /* 525 */
  { 0x0DCA, 0x0DCA, PDF_TEXT_UCD_PL_Diacritic  }, /* 526 */
  { 0x0E47, 0x0E4C, PDF_TEXT_UCD_PL_Diacritic  }, /* 527 */
  { 0x0E4E, 0x0E4E, PDF_TEXT_UCD_PL_Diacritic  }, /* 528 */
  { 0x0EC8, 0x0ECC, PDF_TEXT_UCD_PL_Diacritic  }, /* 529 */
  { 0x0F18, 0x0F19, PDF_TEXT_UCD_PL_Diacritic  }, /* 530 */
  { 0x0F35, 0x0F35, PDF_TEXT_UCD_PL_Diacritic  }, /* 531 */
  { 0x0F37, 0x0F37, PDF_TEXT_UCD_PL_Diacritic  }, /* 532 */
  { 0x0F39, 0x0F39, PDF_TEXT_UCD_PL_Diacritic  }, /* 533 */
  { 0x0F3E, 0x0F3F, PDF_TEXT_UCD_PL_Diacritic  }, /* 534 */
  { 0x0F82, 0x0F84, PDF_TEXT_UCD_PL_Diacritic  }, /* 535 */
  { 0x0F86, 0x0F87, PDF_TEXT_UCD_PL_Diacritic  }, /* 536 */
  { 0x0FC6, 0x0FC6, PDF_TEXT_UCD_PL_Diacritic  }, /* 537 */
  { 0x1037, 0x1037, PDF_TEXT_UCD_PL_Diacritic  }, /* 538 */
  { 0x1039, 0x103A, PDF_TEXT_UCD_PL_Diacritic  }, /* 539 */
  { 0x1087, 0x108C, PDF_TEXT_UCD_PL_Diacritic  }, /* 540 */
  { 0x108D, 0x108D, PDF_TEXT_UCD_PL_Diacritic  }, /* 541 */
  { 0x108F, 0x108F, PDF_TEXT_UCD_PL_Diacritic  }, /* 542 */
  { 0x17C9, 0x17D3, PDF_TEXT_UCD_PL_Diacritic  }, /* 543 */
  { 0x17DD, 0x17DD, PDF_TEXT_UCD_PL_Diacritic  }, /* 544 */
  { 0x1939, 0x193B, PDF_TEXT_UCD_PL_Diacritic  }, /* 545 */
  { 0x1B34, 0x1B34, PDF_TEXT_UCD_PL_Diacritic  }, /* 546 */
  { 0x1B44, 0x1B44, PDF_TEXT_UCD_PL_Diacritic  }, /* 547 */
  { 0x1B6B, 0x1B73, PDF_TEXT_UCD_PL_Diacritic  }, /* 548 */
  { 0x1BAA, 0x1BAA, PDF_TEXT_UCD_PL_Diacritic  }, /* 549 */
  { 0x1C36, 0x1C37, PDF_TEXT_UCD_PL_Diacritic  }, /* 550 */
  { 0x1C78, 0x1C7D, PDF_TEXT_UCD_PL_Diacritic  }, /* 551 */
  { 0x1D2C, 0x1D61, PDF_TEXT_UCD_PL_Diacritic  }, /* 552 */
  { 0x1D62, 0x1D6A, PDF_TEXT_UCD_PL_Diacritic  }, /* 553 */
  { 0x1DC4, 0x1DCF, PDF_TEXT_UCD_PL_Diacritic  }, /* 554 */
  { 0x1DFE, 0x1DFF, PDF_TEXT_UCD_PL_Diacritic  }, /* 555 */
  { 0x1FBD, 0x1FBD, PDF_TEXT_UCD_PL_Diacritic  }, /* 556 */
  { 0x1FBF, 0x1FC1, PDF_TEXT_UCD_PL_Diacritic  }, /* 557 */
  { 0x1FCD, 0x1FCF, PDF_TEXT_UCD_PL_Diacritic  }, /* 558 */
  { 0x1FDD, 0x1FDF, PDF_TEXT_UCD_PL_Diacritic  }, /* 559 */
  { 0x1FED, 0x1FEF, PDF_TEXT_UCD_PL_Diacritic  }, /* 560 */
  { 0x1FFD, 0x1FFE, PDF_TEXT_UCD_PL_Diacritic  }, /* 561 */
  { 0x2E2F, 0x2E2F, PDF_TEXT_UCD_PL_Diacritic  }, /* 562 */
  { 0x302A, 0x302F, PDF_TEXT_UCD_PL_Diacritic  }, /* 563 */
  { 0x3099, 0x309A, PDF_TEXT_UCD_PL_Diacritic  }, /* 564 */
  { 0x309B, 0x309C, PDF_TEXT_UCD_PL_Diacritic  }, /* 565 */
  { 0x30FC, 0x30FC, PDF_TEXT_UCD_PL_Diacritic  }, /* 566 */
  { 0xA66F, 0xA66F, PDF_TEXT_UCD_PL_Diacritic  }, /* 567 */
  { 0xA67C, 0xA67D, PDF_TEXT_UCD_PL_Diacritic  }, /* 568 */
  { 0xA67F, 0xA67F, PDF_TEXT_UCD_PL_Diacritic  }, /* 569 */
  { 0xA717, 0xA71F, PDF_TEXT_UCD_PL_Diacritic  }, /* 570 */
  { 0xA720, 0xA721, PDF_TEXT_UCD_PL_Diacritic  }, /* 571 */
  { 0xA788, 0xA788, PDF_TEXT_UCD_PL_Diacritic  }, /* 572 */
  { 0xA8C4, 0xA8C4, PDF_TEXT_UCD_PL_Diacritic  }, /* 573 */
  { 0xA92B, 0xA92D, PDF_TEXT_UCD_PL_Diacritic  }, /* 574 */
  { 0xA92E, 0xA92E, PDF_TEXT_UCD_PL_Diacritic  }, /* 575 */
  { 0xA953, 0xA953, PDF_TEXT_UCD_PL_Diacritic  }, /* 576 */
  { 0xFB1E, 0xFB1E, PDF_TEXT_UCD_PL_Diacritic  }, /* 577 */
  { 0xFE20, 0xFE26, PDF_TEXT_UCD_PL_Diacritic  }, /* 578 */
  { 0xFF3E, 0xFF3E, PDF_TEXT_UCD_PL_Diacritic  }, /* 579 */
  { 0xFF40, 0xFF40, PDF_TEXT_UCD_PL_Diacritic  }, /* 580 */
  { 0xFF70, 0xFF70, PDF_TEXT_UCD_PL_Diacritic  }, /* 581 */
  { 0xFF9E, 0xFF9F, PDF_TEXT_UCD_PL_Diacritic  }, /* 582 */
  { 0xFFE3, 0xFFE3, PDF_TEXT_UCD_PL_Diacritic  }, /* 583 */
  { 0x1D167, 0x1D169, PDF_TEXT_UCD_PL_Diacritic  }, /* 584 */
  { 0x1D16D, 0x1D172, PDF_TEXT_UCD_PL_Diacritic  }, /* 585 */
  { 0x1D17B, 0x1D182, PDF_TEXT_UCD_PL_Diacritic  }, /* 586 */
  { 0x1D185, 0x1D18B, PDF_TEXT_UCD_PL_Diacritic  }, /* 587 */
  { 0x1D1AA, 0x1D1AD, PDF_TEXT_UCD_PL_Diacritic  }, /* 588 */
  { 0x00B7, 0x00B7, PDF_TEXT_UCD_PL_Extender  }, /* 589 */
  { 0x02D0, 0x02D1, PDF_TEXT_UCD_PL_Extender  }, /* 590 */
  { 0x0640, 0x0640, PDF_TEXT_UCD_PL_Extender  }, /* 591 */
  { 0x07FA, 0x07FA, PDF_TEXT_UCD_PL_Extender  }, /* 592 */
  { 0x0E46, 0x0E46, PDF_TEXT_UCD_PL_Extender  }, /* 593 */
  { 0x0EC6, 0x0EC6, PDF_TEXT_UCD_PL_Extender  }, /* 594 */
  { 0x1843, 0x1843, PDF_TEXT_UCD_PL_Extender  }, /* 595 */
  { 0x1C36, 0x1C36, PDF_TEXT_UCD_PL_Extender  }, /* 596 */
  { 0x1C7B, 0x1C7B, PDF_TEXT_UCD_PL_Extender  }, /* 597 */
  { 0x3005, 0x3005, PDF_TEXT_UCD_PL_Extender  }, /* 598 */
  { 0x3031, 0x3035, PDF_TEXT_UCD_PL_Extender  }, /* 599 */
  { 0x309D, 0x309E, PDF_TEXT_UCD_PL_Extender  }, /* 600 */
  { 0x30FC, 0x30FE, PDF_TEXT_UCD_PL_Extender  }, /* 601 */
  { 0xA015, 0xA015, PDF_TEXT_UCD_PL_Extender  }, /* 602 */
  { 0xA60C, 0xA60C, PDF_TEXT_UCD_PL_Extender  }, /* 603 */
  { 0xFF70, 0xFF70, PDF_TEXT_UCD_PL_Extender  }, /* 604 */
  { 0x02B0, 0x02B8, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 605 */
  { 0x02C0, 0x02C1, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 606 */
  { 0x02E0, 0x02E4, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 607 */
  { 0x0345, 0x0345, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 608 */
  { 0x037A, 0x037A, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 609 */
  { 0x1D2C, 0x1D61, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 610 */
  { 0x1D78, 0x1D78, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 611 */
  { 0x1D9B, 0x1DBF, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 612 */
  { 0x2090, 0x2094, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 613 */
  { 0x2170, 0x217F, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 614 */
  { 0x24D0, 0x24E9, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 615 */
  { 0x2C7D, 0x2C7D, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 616 */
  { 0xA770, 0xA770, PDF_TEXT_UCD_PL_Other_Lowercase  }, /* 617 */
  { 0x2160, 0x216F, PDF_TEXT_UCD_PL_Other_Uppercase  }, /* 618 */
  { 0x24B6, 0x24CF, PDF_TEXT_UCD_PL_Other_Uppercase  }, /* 619 */
  { 0xFDD0, 0xFDEF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 620 */
  { 0xFFFE, 0xFFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 621 */
  { 0x1FFFE, 0x1FFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 622 */
  { 0x2FFFE, 0x2FFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 623 */
  { 0x3FFFE, 0x3FFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 624 */
  { 0x4FFFE, 0x4FFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 625 */
  { 0x5FFFE, 0x5FFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 626 */
  { 0x6FFFE, 0x6FFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 627 */
  { 0x7FFFE, 0x7FFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 628 */
  { 0x8FFFE, 0x8FFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 629 */
  { 0x9FFFE, 0x9FFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 630 */
  { 0xAFFFE, 0xAFFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 631 */
  { 0xBFFFE, 0xBFFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 632 */
  { 0xCFFFE, 0xCFFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 633 */
  { 0xDFFFE, 0xDFFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 634 */
  { 0xEFFFE, 0xEFFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 635 */
  { 0xFFFFE, 0xFFFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 636 */
  { 0x10FFFE, 0x10FFFF, PDF_TEXT_UCD_PL_Noncharacter_Code_Point  }, /* 637 */
  { 0x09BE, 0x09BE, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 638 */
  { 0x09D7, 0x09D7, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 639 */
  { 0x0B3E, 0x0B3E, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 640 */
  { 0x0B57, 0x0B57, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 641 */
  { 0x0BBE, 0x0BBE, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 642 */
  { 0x0BD7, 0x0BD7, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 643 */
  { 0x0CC2, 0x0CC2, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 644 */
  { 0x0CD5, 0x0CD6, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 645 */
  { 0x0D3E, 0x0D3E, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 646 */
  { 0x0D57, 0x0D57, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 647 */
  { 0x0DCF, 0x0DCF, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 648 */
  { 0x0DDF, 0x0DDF, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 649 */
  { 0x200C, 0x200D, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 650 */
  { 0xFF9E, 0xFF9F, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 651 */
  { 0x1D165, 0x1D165, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 652 */
  { 0x1D16E, 0x1D172, PDF_TEXT_UCD_PL_Other_Grapheme_Extend  }, /* 653 */
  { 0x2FF0, 0x2FF1, PDF_TEXT_UCD_PL_IDS_Binary_Operator  }, /* 654 */
  { 0x2FF4, 0x2FFB, PDF_TEXT_UCD_PL_IDS_Binary_Operator  }, /* 655 */
  { 0x2FF2, 0x2FF3, PDF_TEXT_UCD_PL_IDS_Trinary_Operator  }, /* 656 */
  { 0x2E80, 0x2E99, PDF_TEXT_UCD_PL_Radical  }, /* 657 */
  { 0x2E9B, 0x2EF3, PDF_TEXT_UCD_PL_Radical  }, /* 658 */
  { 0x2F00, 0x2FD5, PDF_TEXT_UCD_PL_Radical  }, /* 659 */
  { 0x3400, 0x4DB5, PDF_TEXT_UCD_PL_Unified_Ideograph  }, /* 660 */
  { 0x4E00, 0x9FC3, PDF_TEXT_UCD_PL_Unified_Ideograph  }, /* 661 */
  { 0xFA0E, 0xFA0F, PDF_TEXT_UCD_PL_Unified_Ideograph  }, /* 662 */
  { 0xFA11, 0xFA11, PDF_TEXT_UCD_PL_Unified_Ideograph  }, /* 663 */
  { 0xFA13, 0xFA14, PDF_TEXT_UCD_PL_Unified_Ideograph  }, /* 664 */
  { 0xFA1F, 0xFA1F, PDF_TEXT_UCD_PL_Unified_Ideograph  }, /* 665 */
  { 0xFA21, 0xFA21, PDF_TEXT_UCD_PL_Unified_Ideograph  }, /* 666 */
  { 0xFA23, 0xFA24, PDF_TEXT_UCD_PL_Unified_Ideograph  }, /* 667 */
  { 0xFA27, 0xFA29, PDF_TEXT_UCD_PL_Unified_Ideograph  }, /* 668 */
  { 0x20000, 0x2A6D6, PDF_TEXT_UCD_PL_Unified_Ideograph  }, /* 669 */
  { 0x034F, 0x034F, PDF_TEXT_UCD_PL_Other_Default_Ignorable_Code_Point  }, /* 670 */
  { 0x115F, 0x1160, PDF_TEXT_UCD_PL_Other_Default_Ignorable_Code_Point  }, /* 671 */
  { 0x2065, 0x2069, PDF_TEXT_UCD_PL_Other_Default_Ignorable_Code_Point  }, /* 672 */
  { 0x3164, 0x3164, PDF_TEXT_UCD_PL_Other_Default_Ignorable_Code_Point  }, /* 673 */
  { 0xFFA0, 0xFFA0, PDF_TEXT_UCD_PL_Other_Default_Ignorable_Code_Point  }, /* 674 */
  { 0xFFF0, 0xFFF8, PDF_TEXT_UCD_PL_Other_Default_Ignorable_Code_Point  }, /* 675 */
  { 0xE0000, 0xE0000, PDF_TEXT_UCD_PL_Other_Default_Ignorable_Code_Point  }, /* 676 */
  { 0xE0002, 0xE001F, PDF_TEXT_UCD_PL_Other_Default_Ignorable_Code_Point  }, /* 677 */
  { 0xE0080, 0xE00FF, PDF_TEXT_UCD_PL_Other_Default_Ignorable_Code_Point  }, /* 678 */
  { 0xE01F0, 0xE0FFF, PDF_TEXT_UCD_PL_Other_Default_Ignorable_Code_Point  }, /* 679 */
  { 0x0340, 0x0341, PDF_TEXT_UCD_PL_Deprecated  }, /* 680 */
  { 0x17A3, 0x17A3, PDF_TEXT_UCD_PL_Deprecated  }, /* 681 */
  { 0x17D3, 0x17D3, PDF_TEXT_UCD_PL_Deprecated  }, /* 682 */
  { 0x206A, 0x206F, PDF_TEXT_UCD_PL_Deprecated  }, /* 683 */
  { 0xE0001, 0xE0001, PDF_TEXT_UCD_PL_Deprecated  }, /* 684 */
  { 0xE0020, 0xE007F, PDF_TEXT_UCD_PL_Deprecated  }, /* 685 */
  { 0x0069, 0x006A, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 686 */
  { 0x012F, 0x012F, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 687 */
  { 0x0249, 0x0249, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 688 */
  { 0x0268, 0x0268, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 689 */
  { 0x029D, 0x029D, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 690 */
  { 0x02B2, 0x02B2, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 691 */
  { 0x03F3, 0x03F3, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 692 */
  { 0x0456, 0x0456, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 693 */
  { 0x0458, 0x0458, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 694 */
  { 0x1D62, 0x1D62, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 695 */
  { 0x1D96, 0x1D96, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 696 */
  { 0x1DA4, 0x1DA4, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 697 */
  { 0x1DA8, 0x1DA8, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 698 */
  { 0x1E2D, 0x1E2D, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 699 */
  { 0x1ECB, 0x1ECB, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 700 */
  { 0x2071, 0x2071, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 701 */
  { 0x2148, 0x2149, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 702 */
  { 0x2C7C, 0x2C7C, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 703 */
  { 0x1D422, 0x1D423, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 704 */
  { 0x1D456, 0x1D457, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 705 */
  { 0x1D48A, 0x1D48B, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 706 */
  { 0x1D4BE, 0x1D4BF, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 707 */
  { 0x1D4F2, 0x1D4F3, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 708 */
  { 0x1D526, 0x1D527, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 709 */
  { 0x1D55A, 0x1D55B, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 710 */
  { 0x1D58E, 0x1D58F, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 711 */
  { 0x1D5C2, 0x1D5C3, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 712 */
  { 0x1D5F6, 0x1D5F7, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 713 */
  { 0x1D62A, 0x1D62B, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 714 */
  { 0x1D65E, 0x1D65F, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 715 */
  { 0x1D692, 0x1D693, PDF_TEXT_UCD_PL_Soft_Dotted  }, /* 716 */
  { 0x0E40, 0x0E44, PDF_TEXT_UCD_PL_Logical_Order_Exception  }, /* 717 */
  { 0x0EC0, 0x0EC4, PDF_TEXT_UCD_PL_Logical_Order_Exception  }, /* 718 */
  { 0x2118, 0x2118, PDF_TEXT_UCD_PL_Other_ID_Start  }, /* 719 */
  { 0x212E, 0x212E, PDF_TEXT_UCD_PL_Other_ID_Start  }, /* 720 */
  { 0x309B, 0x309C, PDF_TEXT_UCD_PL_Other_ID_Start  }, /* 721 */
  { 0x00B7, 0x00B7, PDF_TEXT_UCD_PL_Other_ID_Continue  }, /* 722 */
  { 0x0387, 0x0387, PDF_TEXT_UCD_PL_Other_ID_Continue  }, /* 723 */
  { 0x1369, 0x1371, PDF_TEXT_UCD_PL_Other_ID_Continue  }, /* 724 */
  { 0x0021, 0x0021, PDF_TEXT_UCD_PL_STerm  }, /* 725 */
  { 0x002E, 0x002E, PDF_TEXT_UCD_PL_STerm  }, /* 726 */
  { 0x003F, 0x003F, PDF_TEXT_UCD_PL_STerm  }, /* 727 */
  { 0x055C, 0x055C, PDF_TEXT_UCD_PL_STerm  }, /* 728 */
  { 0x055E, 0x055E, PDF_TEXT_UCD_PL_STerm  }, /* 729 */
  { 0x0589, 0x0589, PDF_TEXT_UCD_PL_STerm  }, /* 730 */
  { 0x061F, 0x061F, PDF_TEXT_UCD_PL_STerm  }, /* 731 */
  { 0x06D4, 0x06D4, PDF_TEXT_UCD_PL_STerm  }, /* 732 */
  { 0x0700, 0x0702, PDF_TEXT_UCD_PL_STerm  }, /* 733 */
  { 0x07F9, 0x07F9, PDF_TEXT_UCD_PL_STerm  }, /* 734 */
  { 0x0964, 0x0965, PDF_TEXT_UCD_PL_STerm  }, /* 735 */
  { 0x104A, 0x104B, PDF_TEXT_UCD_PL_STerm  }, /* 736 */
  { 0x1362, 0x1362, PDF_TEXT_UCD_PL_STerm  }, /* 737 */
  { 0x1367, 0x1368, PDF_TEXT_UCD_PL_STerm  }, /* 738 */
  { 0x166E, 0x166E, PDF_TEXT_UCD_PL_STerm  }, /* 739 */
  { 0x1803, 0x1803, PDF_TEXT_UCD_PL_STerm  }, /* 740 */
  { 0x1809, 0x1809, PDF_TEXT_UCD_PL_STerm  }, /* 741 */
  { 0x1944, 0x1945, PDF_TEXT_UCD_PL_STerm  }, /* 742 */
  { 0x1B5A, 0x1B5B, PDF_TEXT_UCD_PL_STerm  }, /* 743 */
  { 0x1B5E, 0x1B5F, PDF_TEXT_UCD_PL_STerm  }, /* 744 */
  { 0x1C3B, 0x1C3C, PDF_TEXT_UCD_PL_STerm  }, /* 745 */
  { 0x1C7E, 0x1C7F, PDF_TEXT_UCD_PL_STerm  }, /* 746 */
  { 0x203C, 0x203D, PDF_TEXT_UCD_PL_STerm  }, /* 747 */
  { 0x2047, 0x2049, PDF_TEXT_UCD_PL_STerm  }, /* 748 */
  { 0x2E2E, 0x2E2E, PDF_TEXT_UCD_PL_STerm  }, /* 749 */
  { 0x3002, 0x3002, PDF_TEXT_UCD_PL_STerm  }, /* 750 */
  { 0xA60E, 0xA60F, PDF_TEXT_UCD_PL_STerm  }, /* 751 */
  { 0xA876, 0xA877, PDF_TEXT_UCD_PL_STerm  }, /* 752 */
  { 0xA8CE, 0xA8CF, PDF_TEXT_UCD_PL_STerm  }, /* 753 */
  { 0xA92F, 0xA92F, PDF_TEXT_UCD_PL_STerm  }, /* 754 */
  { 0xAA5D, 0xAA5F, PDF_TEXT_UCD_PL_STerm  }, /* 755 */
  { 0xFE52, 0xFE52, PDF_TEXT_UCD_PL_STerm  }, /* 756 */
  { 0xFE56, 0xFE57, PDF_TEXT_UCD_PL_STerm  }, /* 757 */
  { 0xFF01, 0xFF01, PDF_TEXT_UCD_PL_STerm  }, /* 758 */
  { 0xFF0E, 0xFF0E, PDF_TEXT_UCD_PL_STerm  }, /* 759 */
  { 0xFF1F, 0xFF1F, PDF_TEXT_UCD_PL_STerm  }, /* 760 */
  { 0xFF61, 0xFF61, PDF_TEXT_UCD_PL_STerm  }, /* 761 */
  { 0x180B, 0x180D, PDF_TEXT_UCD_PL_Variation_Selector  }, /* 762 */
  { 0xFE00, 0xFE0F, PDF_TEXT_UCD_PL_Variation_Selector  }, /* 763 */
  { 0xE0100, 0xE01EF, PDF_TEXT_UCD_PL_Variation_Selector  }, /* 764 */
  { 0x0009, 0x000D, PDF_TEXT_UCD_PL_Pattern_White_Space  }, /* 765 */
  { 0x0020, 0x0020, PDF_TEXT_UCD_PL_Pattern_White_Space  }, /* 766 */
  { 0x0085, 0x0085, PDF_TEXT_UCD_PL_Pattern_White_Space  }, /* 767 */
  { 0x200E, 0x200F, PDF_TEXT_UCD_PL_Pattern_White_Space  }, /* 768 */
  { 0x2028, 0x2028, PDF_TEXT_UCD_PL_Pattern_White_Space  }, /* 769 */
  { 0x2029, 0x2029, PDF_TEXT_UCD_PL_Pattern_White_Space  }, /* 770 */
  { 0x0021, 0x0023, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 771 */
  { 0x0024, 0x0024, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 772 */
  { 0x0025, 0x0027, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 773 */
  { 0x0028, 0x0028, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 774 */
  { 0x0029, 0x0029, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 775 */
  { 0x002A, 0x002A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 776 */
  { 0x002B, 0x002B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 777 */
  { 0x002C, 0x002C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 778 */
  { 0x002D, 0x002D, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 779 */
  { 0x002E, 0x002F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 780 */
  { 0x003A, 0x003B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 781 */
  { 0x003C, 0x003E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 782 */
  { 0x003F, 0x0040, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 783 */
  { 0x005B, 0x005B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 784 */
  { 0x005C, 0x005C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 785 */
  { 0x005D, 0x005D, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 786 */
  { 0x005E, 0x005E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 787 */
  { 0x0060, 0x0060, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 788 */
  { 0x007B, 0x007B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 789 */
  { 0x007C, 0x007C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 790 */
  { 0x007D, 0x007D, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 791 */
  { 0x007E, 0x007E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 792 */
  { 0x00A1, 0x00A1, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 793 */
  { 0x00A2, 0x00A5, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 794 */
  { 0x00A6, 0x00A7, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 795 */
  { 0x00A9, 0x00A9, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 796 */
  { 0x00AB, 0x00AB, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 797 */
  { 0x00AC, 0x00AC, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 798 */
  { 0x00AE, 0x00AE, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 799 */
  { 0x00B0, 0x00B0, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 800 */
  { 0x00B1, 0x00B1, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 801 */
  { 0x00B6, 0x00B6, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 802 */
  { 0x00BB, 0x00BB, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 803 */
  { 0x00BF, 0x00BF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 804 */
  { 0x00D7, 0x00D7, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 805 */
  { 0x00F7, 0x00F7, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 806 */
  { 0x2010, 0x2015, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 807 */
  { 0x2016, 0x2017, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 808 */
  { 0x2018, 0x2018, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 809 */
  { 0x2019, 0x2019, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 810 */
  { 0x201A, 0x201A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 811 */
  { 0x201B, 0x201C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 812 */
  { 0x201D, 0x201D, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 813 */
  { 0x201E, 0x201E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 814 */
  { 0x201F, 0x201F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 815 */
  { 0x2020, 0x2027, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 816 */
  { 0x2030, 0x2038, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 817 */
  { 0x2039, 0x2039, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 818 */
  { 0x203A, 0x203A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 819 */
  { 0x203B, 0x203E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 820 */
  { 0x2041, 0x2043, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 821 */
  { 0x2044, 0x2044, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 822 */
  { 0x2045, 0x2045, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 823 */
  { 0x2046, 0x2046, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 824 */
  { 0x2047, 0x2051, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 825 */
  { 0x2052, 0x2052, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 826 */
  { 0x2053, 0x2053, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 827 */
  { 0x2055, 0x205E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 828 */
  { 0x2190, 0x2194, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 829 */
  { 0x2195, 0x2199, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 830 */
  { 0x219A, 0x219B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 831 */
  { 0x219C, 0x219F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 832 */
  { 0x21A0, 0x21A0, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 833 */
  { 0x21A1, 0x21A2, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 834 */
  { 0x21A3, 0x21A3, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 835 */
  { 0x21A4, 0x21A5, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 836 */
  { 0x21A6, 0x21A6, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 837 */
  { 0x21A7, 0x21AD, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 838 */
  { 0x21AE, 0x21AE, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 839 */
  { 0x21AF, 0x21CD, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 840 */
  { 0x21CE, 0x21CF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 841 */
  { 0x21D0, 0x21D1, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 842 */
  { 0x21D2, 0x21D2, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 843 */
  { 0x21D3, 0x21D3, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 844 */
  { 0x21D4, 0x21D4, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 845 */
  { 0x21D5, 0x21F3, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 846 */
  { 0x21F4, 0x22FF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 847 */
  { 0x2300, 0x2307, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 848 */
  { 0x2308, 0x230B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 849 */
  { 0x230C, 0x231F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 850 */
  { 0x2320, 0x2321, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 851 */
  { 0x2322, 0x2328, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 852 */
  { 0x2329, 0x2329, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 853 */
  { 0x232A, 0x232A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 854 */
  { 0x232B, 0x237B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 855 */
  { 0x237C, 0x237C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 856 */
  { 0x237D, 0x239A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 857 */
  { 0x239B, 0x23B3, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 858 */
  { 0x23B4, 0x23DB, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 859 */
  { 0x23DC, 0x23E1, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 860 */
  { 0x23E2, 0x23E7, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 861 */
  { 0x23E8, 0x23FF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 862 */
  { 0x2400, 0x2426, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 863 */
  { 0x2427, 0x243F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 864 */
  { 0x2440, 0x244A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 865 */
  { 0x244B, 0x245F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 866 */
  { 0x2500, 0x25B6, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 867 */
  { 0x25B7, 0x25B7, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 868 */
  { 0x25B8, 0x25C0, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 869 */
  { 0x25C1, 0x25C1, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 870 */
  { 0x25C2, 0x25F7, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 871 */
  { 0x25F8, 0x25FF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 872 */
  { 0x2600, 0x266E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 873 */
  { 0x266F, 0x266F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 874 */
  { 0x2670, 0x269D, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 875 */
  { 0x269E, 0x269F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 876 */
  { 0x26A0, 0x26BC, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 877 */
  { 0x26BD, 0x26BF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 878 */
  { 0x26C0, 0x26C3, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 879 */
  { 0x26C4, 0x2700, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 880 */
  { 0x2701, 0x2704, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 881 */
  { 0x2705, 0x2705, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 882 */
  { 0x2706, 0x2709, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 883 */
  { 0x270A, 0x270B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 884 */
  { 0x270C, 0x2727, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 885 */
  { 0x2728, 0x2728, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 886 */
  { 0x2729, 0x274B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 887 */
  { 0x274C, 0x274C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 888 */
  { 0x274D, 0x274D, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 889 */
  { 0x274E, 0x274E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 890 */
  { 0x274F, 0x2752, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 891 */
  { 0x2753, 0x2755, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 892 */
  { 0x2756, 0x2756, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 893 */
  { 0x2757, 0x2757, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 894 */
  { 0x2758, 0x275E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 895 */
  { 0x275F, 0x2760, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 896 */
  { 0x2761, 0x2767, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 897 */
  { 0x2768, 0x2768, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 898 */
  { 0x2769, 0x2769, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 899 */
  { 0x276A, 0x276A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 900 */
  { 0x276B, 0x276B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 901 */
  { 0x276C, 0x276C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 902 */
  { 0x276D, 0x276D, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 903 */
  { 0x276E, 0x276E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 904 */
  { 0x276F, 0x276F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 905 */
  { 0x2770, 0x2770, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 906 */
  { 0x2771, 0x2771, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 907 */
  { 0x2772, 0x2772, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 908 */
  { 0x2773, 0x2773, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 909 */
  { 0x2774, 0x2774, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 910 */
  { 0x2775, 0x2775, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 911 */
  { 0x2794, 0x2794, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 912 */
  { 0x2795, 0x2797, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 913 */
  { 0x2798, 0x27AF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 914 */
  { 0x27B0, 0x27B0, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 915 */
  { 0x27B1, 0x27BE, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 916 */
  { 0x27BF, 0x27BF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 917 */
  { 0x27C0, 0x27C4, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 918 */
  { 0x27C5, 0x27C5, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 919 */
  { 0x27C6, 0x27C6, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 920 */
  { 0x27C7, 0x27CA, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 921 */
  { 0x27CB, 0x27CB, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 922 */
  { 0x27CC, 0x27CC, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 923 */
  { 0x27CD, 0x27CF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 924 */
  { 0x27D0, 0x27E5, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 925 */
  { 0x27E6, 0x27E6, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 926 */
  { 0x27E7, 0x27E7, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 927 */
  { 0x27E8, 0x27E8, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 928 */
  { 0x27E9, 0x27E9, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 929 */
  { 0x27EA, 0x27EA, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 930 */
  { 0x27EB, 0x27EB, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 931 */
  { 0x27EC, 0x27EC, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 932 */
  { 0x27ED, 0x27ED, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 933 */
  { 0x27EE, 0x27EE, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 934 */
  { 0x27EF, 0x27EF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 935 */
  { 0x27F0, 0x27FF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 936 */
  { 0x2800, 0x28FF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 937 */
  { 0x2900, 0x2982, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 938 */
  { 0x2983, 0x2983, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 939 */
  { 0x2984, 0x2984, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 940 */
  { 0x2985, 0x2985, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 941 */
  { 0x2986, 0x2986, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 942 */
  { 0x2987, 0x2987, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 943 */
  { 0x2988, 0x2988, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 944 */
  { 0x2989, 0x2989, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 945 */
  { 0x298A, 0x298A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 946 */
  { 0x298B, 0x298B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 947 */
  { 0x298C, 0x298C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 948 */
  { 0x298D, 0x298D, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 949 */
  { 0x298E, 0x298E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 950 */
  { 0x298F, 0x298F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 951 */
  { 0x2990, 0x2990, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 952 */
  { 0x2991, 0x2991, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 953 */
  { 0x2992, 0x2992, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 954 */
  { 0x2993, 0x2993, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 955 */
  { 0x2994, 0x2994, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 956 */
  { 0x2995, 0x2995, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 957 */
  { 0x2996, 0x2996, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 958 */
  { 0x2997, 0x2997, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 959 */
  { 0x2998, 0x2998, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 960 */
  { 0x2999, 0x29D7, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 961 */
  { 0x29D8, 0x29D8, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 962 */
  { 0x29D9, 0x29D9, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 963 */
  { 0x29DA, 0x29DA, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 964 */
  { 0x29DB, 0x29DB, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 965 */
  { 0x29DC, 0x29FB, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 966 */
  { 0x29FC, 0x29FC, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 967 */
  { 0x29FD, 0x29FD, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 968 */
  { 0x29FE, 0x2AFF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 969 */
  { 0x2B00, 0x2B2F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 970 */
  { 0x2B30, 0x2B44, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 971 */
  { 0x2B45, 0x2B46, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 972 */
  { 0x2B47, 0x2B4C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 973 */
  { 0x2B4D, 0x2B4F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 974 */
  { 0x2B50, 0x2B54, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 975 */
  { 0x2B55, 0x2BFF, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 976 */
  { 0x2E00, 0x2E01, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 977 */
  { 0x2E02, 0x2E02, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 978 */
  { 0x2E03, 0x2E03, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 979 */
  { 0x2E04, 0x2E04, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 980 */
  { 0x2E05, 0x2E05, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 981 */
  { 0x2E06, 0x2E08, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 982 */
  { 0x2E09, 0x2E09, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 983 */
  { 0x2E0A, 0x2E0A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 984 */
  { 0x2E0B, 0x2E0B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 985 */
  { 0x2E0C, 0x2E0C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 986 */
  { 0x2E0D, 0x2E0D, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 987 */
  { 0x2E0E, 0x2E16, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 988 */
  { 0x2E17, 0x2E17, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 989 */
  { 0x2E18, 0x2E19, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 990 */
  { 0x2E1A, 0x2E1A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 991 */
  { 0x2E1B, 0x2E1B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 992 */
  { 0x2E1C, 0x2E1C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 993 */
  { 0x2E1D, 0x2E1D, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 994 */
  { 0x2E1E, 0x2E1F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 995 */
  { 0x2E20, 0x2E20, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 996 */
  { 0x2E21, 0x2E21, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 997 */
  { 0x2E22, 0x2E22, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 998 */
  { 0x2E23, 0x2E23, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 999 */
  { 0x2E24, 0x2E24, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1000 */
  { 0x2E25, 0x2E25, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1001 */
  { 0x2E26, 0x2E26, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1002 */
  { 0x2E27, 0x2E27, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1003 */
  { 0x2E28, 0x2E28, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1004 */
  { 0x2E29, 0x2E29, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1005 */
  { 0x2E2A, 0x2E2E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1006 */
  { 0x2E2F, 0x2E2F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1007 */
  { 0x2E30, 0x2E30, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1008 */
  { 0x2E31, 0x2E7F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1009 */
  { 0x3001, 0x3003, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1010 */
  { 0x3008, 0x3008, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1011 */
  { 0x3009, 0x3009, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1012 */
  { 0x300A, 0x300A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1013 */
  { 0x300B, 0x300B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1014 */
  { 0x300C, 0x300C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1015 */
  { 0x300D, 0x300D, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1016 */
  { 0x300E, 0x300E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1017 */
  { 0x300F, 0x300F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1018 */
  { 0x3010, 0x3010, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1019 */
  { 0x3011, 0x3011, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1020 */
  { 0x3012, 0x3013, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1021 */
  { 0x3014, 0x3014, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1022 */
  { 0x3015, 0x3015, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1023 */
  { 0x3016, 0x3016, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1024 */
  { 0x3017, 0x3017, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1025 */
  { 0x3018, 0x3018, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1026 */
  { 0x3019, 0x3019, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1027 */
  { 0x301A, 0x301A, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1028 */
  { 0x301B, 0x301B, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1029 */
  { 0x301C, 0x301C, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1030 */
  { 0x301D, 0x301D, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1031 */
  { 0x301E, 0x301F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1032 */
  { 0x3020, 0x3020, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1033 */
  { 0x3030, 0x3030, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1034 */
  { 0xFD3E, 0xFD3E, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1035 */
  { 0xFD3F, 0xFD3F, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1036 */
  { 0xFE45, 0xFE46, PDF_TEXT_UCD_PL_Pattern_Syntax  }, /* 1037 */
};


/***************** END OF SELF-GENERATED DATA *********************************/


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


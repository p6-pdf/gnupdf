/* -*- mode: C -*- Time-stamp: ""
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

#include <pdf-text-ucd-wordbreak.h>
#include <pdf-text-context.h>

#define UCD_WB_FORMAT_F       0     /* Index of first 'Format' interval       */
#define UCD_WB_FORMAT_L       14    /* Index of last 'Format' interval        */
#define UCD_WB_KATAKANA_F     15    /* Index of first 'Katakana' interval     */
#define UCD_WB_KATAKANA_L     25    /* Index of last 'Katakana' interval      */
#define UCD_WB_ALETTER_F      26    /* Index of first 'ALetter' interval      */
#define UCD_WB_ALETTER_L      420   /* Index of last 'ALetter' interval       */
#define UCD_WB_MIDLETTER_F    421   /* Index of first 'MidLetter' interval    */
#define UCD_WB_MIDLETTER_L    426   /* Index of last 'MidLetter' interval     */
#define UCD_WB_MIDNUM_F       427   /* Index of first 'MidNum' interval       */
#define UCD_WB_MIDNUM_L       436   /* Index of last 'MidNum' interval        */
#define UCD_WB_NUMERIC_F      437   /* Index of first 'Numeric' interval      */
#define UCD_WB_NUMERIC_L      461   /* Index of last 'Numeric' interval       */
#define UCD_WB_EXTENDNUMLET_F 462   /* Index of first 'ExtendNumLet' interval */
#define UCD_WB_EXTENDNUMLET_L 467   /* Index of last 'ExtendNumLet' interval  */
#define UCD_WB_INFO_N         468   /* Maximum number of elements in array    */

typedef struct _unicode_wordbreak_info_s {
  pdf_u32_t interval_start;
  pdf_u32_t interval_stop;
  enum pdf_text_ucd_wb_property_e wb_property;
} unicode_wordbreak_info_t;

static unicode_wordbreak_info_t unicode_wordbreak_info[] = {
  { 0x00AD, 0x00AD, PDF_TEXT_UCD_WBP_Format  }, /* 0 */
  { 0x0600, 0x0603, PDF_TEXT_UCD_WBP_Format  }, /* 1 */
  { 0x06DD, 0x06DD, PDF_TEXT_UCD_WBP_Format  }, /* 2 */
  { 0x070F, 0x070F, PDF_TEXT_UCD_WBP_Format  }, /* 3 */
  { 0x17B4, 0x17B5, PDF_TEXT_UCD_WBP_Format  }, /* 4 */
  { 0x200B, 0x200B, PDF_TEXT_UCD_WBP_Format  }, /* 5 */
  { 0x200E, 0x200F, PDF_TEXT_UCD_WBP_Format  }, /* 6 */
  { 0x202A, 0x202E, PDF_TEXT_UCD_WBP_Format  }, /* 7 */
  { 0x2060, 0x2063, PDF_TEXT_UCD_WBP_Format  }, /* 8 */
  { 0x206A, 0x206F, PDF_TEXT_UCD_WBP_Format  }, /* 9 */
  { 0xFEFF, 0xFEFF, PDF_TEXT_UCD_WBP_Format  }, /* 10 */
  { 0xFFF9, 0xFFFB, PDF_TEXT_UCD_WBP_Format  }, /* 11 */
  { 0x1D173, 0x1D17A, PDF_TEXT_UCD_WBP_Format  }, /* 12 */
  { 0xE0001, 0xE0001, PDF_TEXT_UCD_WBP_Format  }, /* 13 */
  { 0xE0020, 0xE007F, PDF_TEXT_UCD_WBP_Format  }, /* 14 */
  { 0x3031, 0x3035, PDF_TEXT_UCD_WBP_Katakana  }, /* 15 */
  { 0x309B, 0x309C, PDF_TEXT_UCD_WBP_Katakana  }, /* 16 */
  { 0x30A0, 0x30A0, PDF_TEXT_UCD_WBP_Katakana  }, /* 17 */
  { 0x30A1, 0x30FA, PDF_TEXT_UCD_WBP_Katakana  }, /* 18 */
  { 0x30FC, 0x30FE, PDF_TEXT_UCD_WBP_Katakana  }, /* 19 */
  { 0x30FF, 0x30FF, PDF_TEXT_UCD_WBP_Katakana  }, /* 20 */
  { 0x31F0, 0x31FF, PDF_TEXT_UCD_WBP_Katakana  }, /* 21 */
  { 0xFF66, 0xFF6F, PDF_TEXT_UCD_WBP_Katakana  }, /* 22 */
  { 0xFF70, 0xFF70, PDF_TEXT_UCD_WBP_Katakana  }, /* 23 */
  { 0xFF71, 0xFF9D, PDF_TEXT_UCD_WBP_Katakana  }, /* 24 */
  { 0xFF9E, 0xFF9F, PDF_TEXT_UCD_WBP_Katakana  }, /* 25 */
  { 0x0041, 0x005A, PDF_TEXT_UCD_WBP_ALetter  }, /* 26 */
  { 0x0061, 0x007A, PDF_TEXT_UCD_WBP_ALetter  }, /* 27 */
  { 0x00AA, 0x00AA, PDF_TEXT_UCD_WBP_ALetter  }, /* 28 */
  { 0x00B5, 0x00B5, PDF_TEXT_UCD_WBP_ALetter  }, /* 29 */
  { 0x00BA, 0x00BA, PDF_TEXT_UCD_WBP_ALetter  }, /* 30 */
  { 0x00C0, 0x00D6, PDF_TEXT_UCD_WBP_ALetter  }, /* 31 */
  { 0x00D8, 0x00F6, PDF_TEXT_UCD_WBP_ALetter  }, /* 32 */
  { 0x00F8, 0x01BA, PDF_TEXT_UCD_WBP_ALetter  }, /* 33 */
  { 0x01BB, 0x01BB, PDF_TEXT_UCD_WBP_ALetter  }, /* 34 */
  { 0x01BC, 0x01BF, PDF_TEXT_UCD_WBP_ALetter  }, /* 35 */
  { 0x01C0, 0x01C3, PDF_TEXT_UCD_WBP_ALetter  }, /* 36 */
  { 0x01C4, 0x0293, PDF_TEXT_UCD_WBP_ALetter  }, /* 37 */
  { 0x0294, 0x0294, PDF_TEXT_UCD_WBP_ALetter  }, /* 38 */
  { 0x0295, 0x02AF, PDF_TEXT_UCD_WBP_ALetter  }, /* 39 */
  { 0x02B0, 0x02C1, PDF_TEXT_UCD_WBP_ALetter  }, /* 40 */
  { 0x02C6, 0x02D1, PDF_TEXT_UCD_WBP_ALetter  }, /* 41 */
  { 0x02E0, 0x02E4, PDF_TEXT_UCD_WBP_ALetter  }, /* 42 */
  { 0x02EE, 0x02EE, PDF_TEXT_UCD_WBP_ALetter  }, /* 43 */
  { 0x037A, 0x037A, PDF_TEXT_UCD_WBP_ALetter  }, /* 44 */
  { 0x037B, 0x037D, PDF_TEXT_UCD_WBP_ALetter  }, /* 45 */
  { 0x0386, 0x0386, PDF_TEXT_UCD_WBP_ALetter  }, /* 46 */
  { 0x0388, 0x038A, PDF_TEXT_UCD_WBP_ALetter  }, /* 47 */
  { 0x038C, 0x038C, PDF_TEXT_UCD_WBP_ALetter  }, /* 48 */
  { 0x038E, 0x03A1, PDF_TEXT_UCD_WBP_ALetter  }, /* 49 */
  { 0x03A3, 0x03CE, PDF_TEXT_UCD_WBP_ALetter  }, /* 50 */
  { 0x03D0, 0x03F5, PDF_TEXT_UCD_WBP_ALetter  }, /* 51 */
  { 0x03F7, 0x0481, PDF_TEXT_UCD_WBP_ALetter  }, /* 52 */
  { 0x048A, 0x0513, PDF_TEXT_UCD_WBP_ALetter  }, /* 53 */
  { 0x0531, 0x0556, PDF_TEXT_UCD_WBP_ALetter  }, /* 54 */
  { 0x0559, 0x0559, PDF_TEXT_UCD_WBP_ALetter  }, /* 55 */
  { 0x0561, 0x0587, PDF_TEXT_UCD_WBP_ALetter  }, /* 56 */
  { 0x05D0, 0x05EA, PDF_TEXT_UCD_WBP_ALetter  }, /* 57 */
  { 0x05F0, 0x05F2, PDF_TEXT_UCD_WBP_ALetter  }, /* 58 */
  { 0x05F3, 0x05F3, PDF_TEXT_UCD_WBP_ALetter  }, /* 59 */
  { 0x0621, 0x063A, PDF_TEXT_UCD_WBP_ALetter  }, /* 60 */
  { 0x0640, 0x0640, PDF_TEXT_UCD_WBP_ALetter  }, /* 61 */
  { 0x0641, 0x064A, PDF_TEXT_UCD_WBP_ALetter  }, /* 62 */
  { 0x066E, 0x066F, PDF_TEXT_UCD_WBP_ALetter  }, /* 63 */
  { 0x0671, 0x06D3, PDF_TEXT_UCD_WBP_ALetter  }, /* 64 */
  { 0x06D5, 0x06D5, PDF_TEXT_UCD_WBP_ALetter  }, /* 65 */
  { 0x06E5, 0x06E6, PDF_TEXT_UCD_WBP_ALetter  }, /* 66 */
  { 0x06EE, 0x06EF, PDF_TEXT_UCD_WBP_ALetter  }, /* 67 */
  { 0x06FA, 0x06FC, PDF_TEXT_UCD_WBP_ALetter  }, /* 68 */
  { 0x06FF, 0x06FF, PDF_TEXT_UCD_WBP_ALetter  }, /* 69 */
  { 0x0710, 0x0710, PDF_TEXT_UCD_WBP_ALetter  }, /* 70 */
  { 0x0712, 0x072F, PDF_TEXT_UCD_WBP_ALetter  }, /* 71 */
  { 0x074D, 0x076D, PDF_TEXT_UCD_WBP_ALetter  }, /* 72 */
  { 0x0780, 0x07A5, PDF_TEXT_UCD_WBP_ALetter  }, /* 73 */
  { 0x07B1, 0x07B1, PDF_TEXT_UCD_WBP_ALetter  }, /* 74 */
  { 0x07CA, 0x07EA, PDF_TEXT_UCD_WBP_ALetter  }, /* 75 */
  { 0x07F4, 0x07F5, PDF_TEXT_UCD_WBP_ALetter  }, /* 76 */
  { 0x07FA, 0x07FA, PDF_TEXT_UCD_WBP_ALetter  }, /* 77 */
  { 0x0903, 0x0903, PDF_TEXT_UCD_WBP_ALetter  }, /* 78 */
  { 0x0904, 0x0939, PDF_TEXT_UCD_WBP_ALetter  }, /* 79 */
  { 0x093D, 0x093D, PDF_TEXT_UCD_WBP_ALetter  }, /* 80 */
  { 0x093E, 0x0940, PDF_TEXT_UCD_WBP_ALetter  }, /* 81 */
  { 0x0949, 0x094C, PDF_TEXT_UCD_WBP_ALetter  }, /* 82 */
  { 0x0950, 0x0950, PDF_TEXT_UCD_WBP_ALetter  }, /* 83 */
  { 0x0958, 0x0961, PDF_TEXT_UCD_WBP_ALetter  }, /* 84 */
  { 0x097B, 0x097F, PDF_TEXT_UCD_WBP_ALetter  }, /* 85 */
  { 0x0982, 0x0983, PDF_TEXT_UCD_WBP_ALetter  }, /* 86 */
  { 0x0985, 0x098C, PDF_TEXT_UCD_WBP_ALetter  }, /* 87 */
  { 0x098F, 0x0990, PDF_TEXT_UCD_WBP_ALetter  }, /* 88 */
  { 0x0993, 0x09A8, PDF_TEXT_UCD_WBP_ALetter  }, /* 89 */
  { 0x09AA, 0x09B0, PDF_TEXT_UCD_WBP_ALetter  }, /* 90 */
  { 0x09B2, 0x09B2, PDF_TEXT_UCD_WBP_ALetter  }, /* 91 */
  { 0x09B6, 0x09B9, PDF_TEXT_UCD_WBP_ALetter  }, /* 92 */
  { 0x09BD, 0x09BD, PDF_TEXT_UCD_WBP_ALetter  }, /* 93 */
  { 0x09BF, 0x09C0, PDF_TEXT_UCD_WBP_ALetter  }, /* 94 */
  { 0x09C7, 0x09C8, PDF_TEXT_UCD_WBP_ALetter  }, /* 95 */
  { 0x09CB, 0x09CC, PDF_TEXT_UCD_WBP_ALetter  }, /* 96 */
  { 0x09CE, 0x09CE, PDF_TEXT_UCD_WBP_ALetter  }, /* 97 */
  { 0x09DC, 0x09DD, PDF_TEXT_UCD_WBP_ALetter  }, /* 98 */
  { 0x09DF, 0x09E1, PDF_TEXT_UCD_WBP_ALetter  }, /* 99 */
  { 0x09F0, 0x09F1, PDF_TEXT_UCD_WBP_ALetter  }, /* 100 */
  { 0x0A03, 0x0A03, PDF_TEXT_UCD_WBP_ALetter  }, /* 101 */
  { 0x0A05, 0x0A0A, PDF_TEXT_UCD_WBP_ALetter  }, /* 102 */
  { 0x0A0F, 0x0A10, PDF_TEXT_UCD_WBP_ALetter  }, /* 103 */
  { 0x0A13, 0x0A28, PDF_TEXT_UCD_WBP_ALetter  }, /* 104 */
  { 0x0A2A, 0x0A30, PDF_TEXT_UCD_WBP_ALetter  }, /* 105 */
  { 0x0A32, 0x0A33, PDF_TEXT_UCD_WBP_ALetter  }, /* 106 */
  { 0x0A35, 0x0A36, PDF_TEXT_UCD_WBP_ALetter  }, /* 107 */
  { 0x0A38, 0x0A39, PDF_TEXT_UCD_WBP_ALetter  }, /* 108 */
  { 0x0A3E, 0x0A40, PDF_TEXT_UCD_WBP_ALetter  }, /* 109 */
  { 0x0A59, 0x0A5C, PDF_TEXT_UCD_WBP_ALetter  }, /* 110 */
  { 0x0A5E, 0x0A5E, PDF_TEXT_UCD_WBP_ALetter  }, /* 111 */
  { 0x0A72, 0x0A74, PDF_TEXT_UCD_WBP_ALetter  }, /* 112 */
  { 0x0A83, 0x0A83, PDF_TEXT_UCD_WBP_ALetter  }, /* 113 */
  { 0x0A85, 0x0A8D, PDF_TEXT_UCD_WBP_ALetter  }, /* 114 */
  { 0x0A8F, 0x0A91, PDF_TEXT_UCD_WBP_ALetter  }, /* 115 */
  { 0x0A93, 0x0AA8, PDF_TEXT_UCD_WBP_ALetter  }, /* 116 */
  { 0x0AAA, 0x0AB0, PDF_TEXT_UCD_WBP_ALetter  }, /* 117 */
  { 0x0AB2, 0x0AB3, PDF_TEXT_UCD_WBP_ALetter  }, /* 118 */
  { 0x0AB5, 0x0AB9, PDF_TEXT_UCD_WBP_ALetter  }, /* 119 */
  { 0x0ABD, 0x0ABD, PDF_TEXT_UCD_WBP_ALetter  }, /* 120 */
  { 0x0ABE, 0x0AC0, PDF_TEXT_UCD_WBP_ALetter  }, /* 121 */
  { 0x0AC9, 0x0AC9, PDF_TEXT_UCD_WBP_ALetter  }, /* 122 */
  { 0x0ACB, 0x0ACC, PDF_TEXT_UCD_WBP_ALetter  }, /* 123 */
  { 0x0AD0, 0x0AD0, PDF_TEXT_UCD_WBP_ALetter  }, /* 124 */
  { 0x0AE0, 0x0AE1, PDF_TEXT_UCD_WBP_ALetter  }, /* 125 */
  { 0x0B02, 0x0B03, PDF_TEXT_UCD_WBP_ALetter  }, /* 126 */
  { 0x0B05, 0x0B0C, PDF_TEXT_UCD_WBP_ALetter  }, /* 127 */
  { 0x0B0F, 0x0B10, PDF_TEXT_UCD_WBP_ALetter  }, /* 128 */
  { 0x0B13, 0x0B28, PDF_TEXT_UCD_WBP_ALetter  }, /* 129 */
  { 0x0B2A, 0x0B30, PDF_TEXT_UCD_WBP_ALetter  }, /* 130 */
  { 0x0B32, 0x0B33, PDF_TEXT_UCD_WBP_ALetter  }, /* 131 */
  { 0x0B35, 0x0B39, PDF_TEXT_UCD_WBP_ALetter  }, /* 132 */
  { 0x0B3D, 0x0B3D, PDF_TEXT_UCD_WBP_ALetter  }, /* 133 */
  { 0x0B40, 0x0B40, PDF_TEXT_UCD_WBP_ALetter  }, /* 134 */
  { 0x0B47, 0x0B48, PDF_TEXT_UCD_WBP_ALetter  }, /* 135 */
  { 0x0B4B, 0x0B4C, PDF_TEXT_UCD_WBP_ALetter  }, /* 136 */
  { 0x0B5C, 0x0B5D, PDF_TEXT_UCD_WBP_ALetter  }, /* 137 */
  { 0x0B5F, 0x0B61, PDF_TEXT_UCD_WBP_ALetter  }, /* 138 */
  { 0x0B71, 0x0B71, PDF_TEXT_UCD_WBP_ALetter  }, /* 139 */
  { 0x0B83, 0x0B83, PDF_TEXT_UCD_WBP_ALetter  }, /* 140 */
  { 0x0B85, 0x0B8A, PDF_TEXT_UCD_WBP_ALetter  }, /* 141 */
  { 0x0B8E, 0x0B90, PDF_TEXT_UCD_WBP_ALetter  }, /* 142 */
  { 0x0B92, 0x0B95, PDF_TEXT_UCD_WBP_ALetter  }, /* 143 */
  { 0x0B99, 0x0B9A, PDF_TEXT_UCD_WBP_ALetter  }, /* 144 */
  { 0x0B9C, 0x0B9C, PDF_TEXT_UCD_WBP_ALetter  }, /* 145 */
  { 0x0B9E, 0x0B9F, PDF_TEXT_UCD_WBP_ALetter  }, /* 146 */
  { 0x0BA3, 0x0BA4, PDF_TEXT_UCD_WBP_ALetter  }, /* 147 */
  { 0x0BA8, 0x0BAA, PDF_TEXT_UCD_WBP_ALetter  }, /* 148 */
  { 0x0BAE, 0x0BB9, PDF_TEXT_UCD_WBP_ALetter  }, /* 149 */
  { 0x0BBF, 0x0BBF, PDF_TEXT_UCD_WBP_ALetter  }, /* 150 */
  { 0x0BC1, 0x0BC2, PDF_TEXT_UCD_WBP_ALetter  }, /* 151 */
  { 0x0BC6, 0x0BC8, PDF_TEXT_UCD_WBP_ALetter  }, /* 152 */
  { 0x0BCA, 0x0BCC, PDF_TEXT_UCD_WBP_ALetter  }, /* 153 */
  { 0x0C01, 0x0C03, PDF_TEXT_UCD_WBP_ALetter  }, /* 154 */
  { 0x0C05, 0x0C0C, PDF_TEXT_UCD_WBP_ALetter  }, /* 155 */
  { 0x0C0E, 0x0C10, PDF_TEXT_UCD_WBP_ALetter  }, /* 156 */
  { 0x0C12, 0x0C28, PDF_TEXT_UCD_WBP_ALetter  }, /* 157 */
  { 0x0C2A, 0x0C33, PDF_TEXT_UCD_WBP_ALetter  }, /* 158 */
  { 0x0C35, 0x0C39, PDF_TEXT_UCD_WBP_ALetter  }, /* 159 */
  { 0x0C41, 0x0C44, PDF_TEXT_UCD_WBP_ALetter  }, /* 160 */
  { 0x0C60, 0x0C61, PDF_TEXT_UCD_WBP_ALetter  }, /* 161 */
  { 0x0C82, 0x0C83, PDF_TEXT_UCD_WBP_ALetter  }, /* 162 */
  { 0x0C85, 0x0C8C, PDF_TEXT_UCD_WBP_ALetter  }, /* 163 */
  { 0x0C8E, 0x0C90, PDF_TEXT_UCD_WBP_ALetter  }, /* 164 */
  { 0x0C92, 0x0CA8, PDF_TEXT_UCD_WBP_ALetter  }, /* 165 */
  { 0x0CAA, 0x0CB3, PDF_TEXT_UCD_WBP_ALetter  }, /* 166 */
  { 0x0CB5, 0x0CB9, PDF_TEXT_UCD_WBP_ALetter  }, /* 167 */
  { 0x0CBD, 0x0CBD, PDF_TEXT_UCD_WBP_ALetter  }, /* 168 */
  { 0x0CBE, 0x0CBE, PDF_TEXT_UCD_WBP_ALetter  }, /* 169 */
  { 0x0CC0, 0x0CC1, PDF_TEXT_UCD_WBP_ALetter  }, /* 170 */
  { 0x0CC3, 0x0CC4, PDF_TEXT_UCD_WBP_ALetter  }, /* 171 */
  { 0x0CC7, 0x0CC8, PDF_TEXT_UCD_WBP_ALetter  }, /* 172 */
  { 0x0CCA, 0x0CCB, PDF_TEXT_UCD_WBP_ALetter  }, /* 173 */
  { 0x0CDE, 0x0CDE, PDF_TEXT_UCD_WBP_ALetter  }, /* 174 */
  { 0x0CE0, 0x0CE1, PDF_TEXT_UCD_WBP_ALetter  }, /* 175 */
  { 0x0D02, 0x0D03, PDF_TEXT_UCD_WBP_ALetter  }, /* 176 */
  { 0x0D05, 0x0D0C, PDF_TEXT_UCD_WBP_ALetter  }, /* 177 */
  { 0x0D0E, 0x0D10, PDF_TEXT_UCD_WBP_ALetter  }, /* 178 */
  { 0x0D12, 0x0D28, PDF_TEXT_UCD_WBP_ALetter  }, /* 179 */
  { 0x0D2A, 0x0D39, PDF_TEXT_UCD_WBP_ALetter  }, /* 180 */
  { 0x0D3F, 0x0D40, PDF_TEXT_UCD_WBP_ALetter  }, /* 181 */
  { 0x0D46, 0x0D48, PDF_TEXT_UCD_WBP_ALetter  }, /* 182 */
  { 0x0D4A, 0x0D4C, PDF_TEXT_UCD_WBP_ALetter  }, /* 183 */
  { 0x0D60, 0x0D61, PDF_TEXT_UCD_WBP_ALetter  }, /* 184 */
  { 0x0D82, 0x0D83, PDF_TEXT_UCD_WBP_ALetter  }, /* 185 */
  { 0x0D85, 0x0D96, PDF_TEXT_UCD_WBP_ALetter  }, /* 186 */
  { 0x0D9A, 0x0DB1, PDF_TEXT_UCD_WBP_ALetter  }, /* 187 */
  { 0x0DB3, 0x0DBB, PDF_TEXT_UCD_WBP_ALetter  }, /* 188 */
  { 0x0DBD, 0x0DBD, PDF_TEXT_UCD_WBP_ALetter  }, /* 189 */
  { 0x0DC0, 0x0DC6, PDF_TEXT_UCD_WBP_ALetter  }, /* 190 */
  { 0x0DD0, 0x0DD1, PDF_TEXT_UCD_WBP_ALetter  }, /* 191 */
  { 0x0DD8, 0x0DDE, PDF_TEXT_UCD_WBP_ALetter  }, /* 192 */
  { 0x0DF2, 0x0DF3, PDF_TEXT_UCD_WBP_ALetter  }, /* 193 */
  { 0x0F00, 0x0F00, PDF_TEXT_UCD_WBP_ALetter  }, /* 194 */
  { 0x0F40, 0x0F47, PDF_TEXT_UCD_WBP_ALetter  }, /* 195 */
  { 0x0F49, 0x0F6A, PDF_TEXT_UCD_WBP_ALetter  }, /* 196 */
  { 0x0F7F, 0x0F7F, PDF_TEXT_UCD_WBP_ALetter  }, /* 197 */
  { 0x0F88, 0x0F8B, PDF_TEXT_UCD_WBP_ALetter  }, /* 198 */
  { 0x10A0, 0x10C5, PDF_TEXT_UCD_WBP_ALetter  }, /* 199 */
  { 0x10D0, 0x10FA, PDF_TEXT_UCD_WBP_ALetter  }, /* 200 */
  { 0x10FC, 0x10FC, PDF_TEXT_UCD_WBP_ALetter  }, /* 201 */
  { 0x1100, 0x1159, PDF_TEXT_UCD_WBP_ALetter  }, /* 202 */
  { 0x115F, 0x11A2, PDF_TEXT_UCD_WBP_ALetter  }, /* 203 */
  { 0x11A8, 0x11F9, PDF_TEXT_UCD_WBP_ALetter  }, /* 204 */
  { 0x1200, 0x1248, PDF_TEXT_UCD_WBP_ALetter  }, /* 205 */
  { 0x124A, 0x124D, PDF_TEXT_UCD_WBP_ALetter  }, /* 206 */
  { 0x1250, 0x1256, PDF_TEXT_UCD_WBP_ALetter  }, /* 207 */
  { 0x1258, 0x1258, PDF_TEXT_UCD_WBP_ALetter  }, /* 208 */
  { 0x125A, 0x125D, PDF_TEXT_UCD_WBP_ALetter  }, /* 209 */
  { 0x1260, 0x1288, PDF_TEXT_UCD_WBP_ALetter  }, /* 210 */
  { 0x128A, 0x128D, PDF_TEXT_UCD_WBP_ALetter  }, /* 211 */
  { 0x1290, 0x12B0, PDF_TEXT_UCD_WBP_ALetter  }, /* 212 */
  { 0x12B2, 0x12B5, PDF_TEXT_UCD_WBP_ALetter  }, /* 213 */
  { 0x12B8, 0x12BE, PDF_TEXT_UCD_WBP_ALetter  }, /* 214 */
  { 0x12C0, 0x12C0, PDF_TEXT_UCD_WBP_ALetter  }, /* 215 */
  { 0x12C2, 0x12C5, PDF_TEXT_UCD_WBP_ALetter  }, /* 216 */
  { 0x12C8, 0x12D6, PDF_TEXT_UCD_WBP_ALetter  }, /* 217 */
  { 0x12D8, 0x1310, PDF_TEXT_UCD_WBP_ALetter  }, /* 218 */
  { 0x1312, 0x1315, PDF_TEXT_UCD_WBP_ALetter  }, /* 219 */
  { 0x1318, 0x135A, PDF_TEXT_UCD_WBP_ALetter  }, /* 220 */
  { 0x1380, 0x138F, PDF_TEXT_UCD_WBP_ALetter  }, /* 221 */
  { 0x13A0, 0x13F4, PDF_TEXT_UCD_WBP_ALetter  }, /* 222 */
  { 0x1401, 0x166C, PDF_TEXT_UCD_WBP_ALetter  }, /* 223 */
  { 0x166F, 0x1676, PDF_TEXT_UCD_WBP_ALetter  }, /* 224 */
  { 0x1681, 0x169A, PDF_TEXT_UCD_WBP_ALetter  }, /* 225 */
  { 0x16A0, 0x16EA, PDF_TEXT_UCD_WBP_ALetter  }, /* 226 */
  { 0x16EE, 0x16F0, PDF_TEXT_UCD_WBP_ALetter  }, /* 227 */
  { 0x1700, 0x170C, PDF_TEXT_UCD_WBP_ALetter  }, /* 228 */
  { 0x170E, 0x1711, PDF_TEXT_UCD_WBP_ALetter  }, /* 229 */
  { 0x1720, 0x1731, PDF_TEXT_UCD_WBP_ALetter  }, /* 230 */
  { 0x1740, 0x1751, PDF_TEXT_UCD_WBP_ALetter  }, /* 231 */
  { 0x1760, 0x176C, PDF_TEXT_UCD_WBP_ALetter  }, /* 232 */
  { 0x176E, 0x1770, PDF_TEXT_UCD_WBP_ALetter  }, /* 233 */
  { 0x1820, 0x1842, PDF_TEXT_UCD_WBP_ALetter  }, /* 234 */
  { 0x1843, 0x1843, PDF_TEXT_UCD_WBP_ALetter  }, /* 235 */
  { 0x1844, 0x1877, PDF_TEXT_UCD_WBP_ALetter  }, /* 236 */
  { 0x1880, 0x18A8, PDF_TEXT_UCD_WBP_ALetter  }, /* 237 */
  { 0x1900, 0x191C, PDF_TEXT_UCD_WBP_ALetter  }, /* 238 */
  { 0x1923, 0x1926, PDF_TEXT_UCD_WBP_ALetter  }, /* 239 */
  { 0x1929, 0x192B, PDF_TEXT_UCD_WBP_ALetter  }, /* 240 */
  { 0x1930, 0x1931, PDF_TEXT_UCD_WBP_ALetter  }, /* 241 */
  { 0x1933, 0x1938, PDF_TEXT_UCD_WBP_ALetter  }, /* 242 */
  { 0x1A00, 0x1A16, PDF_TEXT_UCD_WBP_ALetter  }, /* 243 */
  { 0x1A19, 0x1A1B, PDF_TEXT_UCD_WBP_ALetter  }, /* 244 */
  { 0x1B04, 0x1B04, PDF_TEXT_UCD_WBP_ALetter  }, /* 245 */
  { 0x1B05, 0x1B33, PDF_TEXT_UCD_WBP_ALetter  }, /* 246 */
  { 0x1B35, 0x1B35, PDF_TEXT_UCD_WBP_ALetter  }, /* 247 */
  { 0x1B3B, 0x1B3B, PDF_TEXT_UCD_WBP_ALetter  }, /* 248 */
  { 0x1B3D, 0x1B41, PDF_TEXT_UCD_WBP_ALetter  }, /* 249 */
  { 0x1B43, 0x1B43, PDF_TEXT_UCD_WBP_ALetter  }, /* 250 */
  { 0x1B45, 0x1B4B, PDF_TEXT_UCD_WBP_ALetter  }, /* 251 */
  { 0x1D00, 0x1D2B, PDF_TEXT_UCD_WBP_ALetter  }, /* 252 */
  { 0x1D2C, 0x1D61, PDF_TEXT_UCD_WBP_ALetter  }, /* 253 */
  { 0x1D62, 0x1D77, PDF_TEXT_UCD_WBP_ALetter  }, /* 254 */
  { 0x1D78, 0x1D78, PDF_TEXT_UCD_WBP_ALetter  }, /* 255 */
  { 0x1D79, 0x1D9A, PDF_TEXT_UCD_WBP_ALetter  }, /* 256 */
  { 0x1D9B, 0x1DBF, PDF_TEXT_UCD_WBP_ALetter  }, /* 257 */
  { 0x1E00, 0x1E9B, PDF_TEXT_UCD_WBP_ALetter  }, /* 258 */
  { 0x1EA0, 0x1EF9, PDF_TEXT_UCD_WBP_ALetter  }, /* 259 */
  { 0x1F00, 0x1F15, PDF_TEXT_UCD_WBP_ALetter  }, /* 260 */
  { 0x1F18, 0x1F1D, PDF_TEXT_UCD_WBP_ALetter  }, /* 261 */
  { 0x1F20, 0x1F45, PDF_TEXT_UCD_WBP_ALetter  }, /* 262 */
  { 0x1F48, 0x1F4D, PDF_TEXT_UCD_WBP_ALetter  }, /* 263 */
  { 0x1F50, 0x1F57, PDF_TEXT_UCD_WBP_ALetter  }, /* 264 */
  { 0x1F59, 0x1F59, PDF_TEXT_UCD_WBP_ALetter  }, /* 265 */
  { 0x1F5B, 0x1F5B, PDF_TEXT_UCD_WBP_ALetter  }, /* 266 */
  { 0x1F5D, 0x1F5D, PDF_TEXT_UCD_WBP_ALetter  }, /* 267 */
  { 0x1F5F, 0x1F7D, PDF_TEXT_UCD_WBP_ALetter  }, /* 268 */
  { 0x1F80, 0x1FB4, PDF_TEXT_UCD_WBP_ALetter  }, /* 269 */
  { 0x1FB6, 0x1FBC, PDF_TEXT_UCD_WBP_ALetter  }, /* 270 */
  { 0x1FBE, 0x1FBE, PDF_TEXT_UCD_WBP_ALetter  }, /* 271 */
  { 0x1FC2, 0x1FC4, PDF_TEXT_UCD_WBP_ALetter  }, /* 272 */
  { 0x1FC6, 0x1FCC, PDF_TEXT_UCD_WBP_ALetter  }, /* 273 */
  { 0x1FD0, 0x1FD3, PDF_TEXT_UCD_WBP_ALetter  }, /* 274 */
  { 0x1FD6, 0x1FDB, PDF_TEXT_UCD_WBP_ALetter  }, /* 275 */
  { 0x1FE0, 0x1FEC, PDF_TEXT_UCD_WBP_ALetter  }, /* 276 */
  { 0x1FF2, 0x1FF4, PDF_TEXT_UCD_WBP_ALetter  }, /* 277 */
  { 0x1FF6, 0x1FFC, PDF_TEXT_UCD_WBP_ALetter  }, /* 278 */
  { 0x2071, 0x2071, PDF_TEXT_UCD_WBP_ALetter  }, /* 279 */
  { 0x207F, 0x207F, PDF_TEXT_UCD_WBP_ALetter  }, /* 280 */
  { 0x2090, 0x2094, PDF_TEXT_UCD_WBP_ALetter  }, /* 281 */
  { 0x2102, 0x2102, PDF_TEXT_UCD_WBP_ALetter  }, /* 282 */
  { 0x2107, 0x2107, PDF_TEXT_UCD_WBP_ALetter  }, /* 283 */
  { 0x210A, 0x2113, PDF_TEXT_UCD_WBP_ALetter  }, /* 284 */
  { 0x2115, 0x2115, PDF_TEXT_UCD_WBP_ALetter  }, /* 285 */
  { 0x2119, 0x211D, PDF_TEXT_UCD_WBP_ALetter  }, /* 286 */
  { 0x2124, 0x2124, PDF_TEXT_UCD_WBP_ALetter  }, /* 287 */
  { 0x2126, 0x2126, PDF_TEXT_UCD_WBP_ALetter  }, /* 288 */
  { 0x2128, 0x2128, PDF_TEXT_UCD_WBP_ALetter  }, /* 289 */
  { 0x212A, 0x212D, PDF_TEXT_UCD_WBP_ALetter  }, /* 290 */
  { 0x212F, 0x2134, PDF_TEXT_UCD_WBP_ALetter  }, /* 291 */
  { 0x2135, 0x2138, PDF_TEXT_UCD_WBP_ALetter  }, /* 292 */
  { 0x2139, 0x2139, PDF_TEXT_UCD_WBP_ALetter  }, /* 293 */
  { 0x213C, 0x213F, PDF_TEXT_UCD_WBP_ALetter  }, /* 294 */
  { 0x2145, 0x2149, PDF_TEXT_UCD_WBP_ALetter  }, /* 295 */
  { 0x214E, 0x214E, PDF_TEXT_UCD_WBP_ALetter  }, /* 296 */
  { 0x2160, 0x2182, PDF_TEXT_UCD_WBP_ALetter  }, /* 297 */
  { 0x2183, 0x2184, PDF_TEXT_UCD_WBP_ALetter  }, /* 298 */
  { 0x24B6, 0x24E9, PDF_TEXT_UCD_WBP_ALetter  }, /* 299 */
  { 0x2C00, 0x2C2E, PDF_TEXT_UCD_WBP_ALetter  }, /* 300 */
  { 0x2C30, 0x2C5E, PDF_TEXT_UCD_WBP_ALetter  }, /* 301 */
  { 0x2C60, 0x2C6C, PDF_TEXT_UCD_WBP_ALetter  }, /* 302 */
  { 0x2C74, 0x2C77, PDF_TEXT_UCD_WBP_ALetter  }, /* 303 */
  { 0x2C80, 0x2CE4, PDF_TEXT_UCD_WBP_ALetter  }, /* 304 */
  { 0x2D00, 0x2D25, PDF_TEXT_UCD_WBP_ALetter  }, /* 305 */
  { 0x2D30, 0x2D65, PDF_TEXT_UCD_WBP_ALetter  }, /* 306 */
  { 0x2D6F, 0x2D6F, PDF_TEXT_UCD_WBP_ALetter  }, /* 307 */
  { 0x2D80, 0x2D96, PDF_TEXT_UCD_WBP_ALetter  }, /* 308 */
  { 0x2DA0, 0x2DA6, PDF_TEXT_UCD_WBP_ALetter  }, /* 309 */
  { 0x2DA8, 0x2DAE, PDF_TEXT_UCD_WBP_ALetter  }, /* 310 */
  { 0x2DB0, 0x2DB6, PDF_TEXT_UCD_WBP_ALetter  }, /* 311 */
  { 0x2DB8, 0x2DBE, PDF_TEXT_UCD_WBP_ALetter  }, /* 312 */
  { 0x2DC0, 0x2DC6, PDF_TEXT_UCD_WBP_ALetter  }, /* 313 */
  { 0x2DC8, 0x2DCE, PDF_TEXT_UCD_WBP_ALetter  }, /* 314 */
  { 0x2DD0, 0x2DD6, PDF_TEXT_UCD_WBP_ALetter  }, /* 315 */
  { 0x2DD8, 0x2DDE, PDF_TEXT_UCD_WBP_ALetter  }, /* 316 */
  { 0x3005, 0x3005, PDF_TEXT_UCD_WBP_ALetter  }, /* 317 */
  { 0x303B, 0x303B, PDF_TEXT_UCD_WBP_ALetter  }, /* 318 */
  { 0x303C, 0x303C, PDF_TEXT_UCD_WBP_ALetter  }, /* 319 */
  { 0x3105, 0x312C, PDF_TEXT_UCD_WBP_ALetter  }, /* 320 */
  { 0x3131, 0x318E, PDF_TEXT_UCD_WBP_ALetter  }, /* 321 */
  { 0x31A0, 0x31B7, PDF_TEXT_UCD_WBP_ALetter  }, /* 322 */
  { 0xA000, 0xA014, PDF_TEXT_UCD_WBP_ALetter  }, /* 323 */
  { 0xA015, 0xA015, PDF_TEXT_UCD_WBP_ALetter  }, /* 324 */
  { 0xA016, 0xA48C, PDF_TEXT_UCD_WBP_ALetter  }, /* 325 */
  { 0xA717, 0xA71A, PDF_TEXT_UCD_WBP_ALetter  }, /* 326 */
  { 0xA800, 0xA801, PDF_TEXT_UCD_WBP_ALetter  }, /* 327 */
  { 0xA803, 0xA805, PDF_TEXT_UCD_WBP_ALetter  }, /* 328 */
  { 0xA807, 0xA80A, PDF_TEXT_UCD_WBP_ALetter  }, /* 329 */
  { 0xA80C, 0xA822, PDF_TEXT_UCD_WBP_ALetter  }, /* 330 */
  { 0xA823, 0xA824, PDF_TEXT_UCD_WBP_ALetter  }, /* 331 */
  { 0xA827, 0xA827, PDF_TEXT_UCD_WBP_ALetter  }, /* 332 */
  { 0xA840, 0xA873, PDF_TEXT_UCD_WBP_ALetter  }, /* 333 */
  { 0xAC00, 0xD7A3, PDF_TEXT_UCD_WBP_ALetter  }, /* 334 */
  { 0xFA30, 0xFA6A, PDF_TEXT_UCD_WBP_ALetter  }, /* 335 */
  { 0xFB00, 0xFB06, PDF_TEXT_UCD_WBP_ALetter  }, /* 336 */
  { 0xFB13, 0xFB17, PDF_TEXT_UCD_WBP_ALetter  }, /* 337 */
  { 0xFB1D, 0xFB1D, PDF_TEXT_UCD_WBP_ALetter  }, /* 338 */
  { 0xFB1F, 0xFB28, PDF_TEXT_UCD_WBP_ALetter  }, /* 339 */
  { 0xFB2A, 0xFB36, PDF_TEXT_UCD_WBP_ALetter  }, /* 340 */
  { 0xFB38, 0xFB3C, PDF_TEXT_UCD_WBP_ALetter  }, /* 341 */
  { 0xFB3E, 0xFB3E, PDF_TEXT_UCD_WBP_ALetter  }, /* 342 */
  { 0xFB40, 0xFB41, PDF_TEXT_UCD_WBP_ALetter  }, /* 343 */
  { 0xFB43, 0xFB44, PDF_TEXT_UCD_WBP_ALetter  }, /* 344 */
  { 0xFB46, 0xFBB1, PDF_TEXT_UCD_WBP_ALetter  }, /* 345 */
  { 0xFBD3, 0xFD3D, PDF_TEXT_UCD_WBP_ALetter  }, /* 346 */
  { 0xFD50, 0xFD8F, PDF_TEXT_UCD_WBP_ALetter  }, /* 347 */
  { 0xFD92, 0xFDC7, PDF_TEXT_UCD_WBP_ALetter  }, /* 348 */
  { 0xFDF0, 0xFDFB, PDF_TEXT_UCD_WBP_ALetter  }, /* 349 */
  { 0xFE70, 0xFE74, PDF_TEXT_UCD_WBP_ALetter  }, /* 350 */
  { 0xFE76, 0xFEFC, PDF_TEXT_UCD_WBP_ALetter  }, /* 351 */
  { 0xFF21, 0xFF3A, PDF_TEXT_UCD_WBP_ALetter  }, /* 352 */
  { 0xFF41, 0xFF5A, PDF_TEXT_UCD_WBP_ALetter  }, /* 353 */
  { 0xFFA0, 0xFFBE, PDF_TEXT_UCD_WBP_ALetter  }, /* 354 */
  { 0xFFC2, 0xFFC7, PDF_TEXT_UCD_WBP_ALetter  }, /* 355 */
  { 0xFFCA, 0xFFCF, PDF_TEXT_UCD_WBP_ALetter  }, /* 356 */
  { 0xFFD2, 0xFFD7, PDF_TEXT_UCD_WBP_ALetter  }, /* 357 */
  { 0xFFDA, 0xFFDC, PDF_TEXT_UCD_WBP_ALetter  }, /* 358 */
  { 0x10000, 0x1000B, PDF_TEXT_UCD_WBP_ALetter  }, /* 359 */
  { 0x1000D, 0x10026, PDF_TEXT_UCD_WBP_ALetter  }, /* 360 */
  { 0x10028, 0x1003A, PDF_TEXT_UCD_WBP_ALetter  }, /* 361 */
  { 0x1003C, 0x1003D, PDF_TEXT_UCD_WBP_ALetter  }, /* 362 */
  { 0x1003F, 0x1004D, PDF_TEXT_UCD_WBP_ALetter  }, /* 363 */
  { 0x10050, 0x1005D, PDF_TEXT_UCD_WBP_ALetter  }, /* 364 */
  { 0x10080, 0x100FA, PDF_TEXT_UCD_WBP_ALetter  }, /* 365 */
  { 0x10140, 0x10174, PDF_TEXT_UCD_WBP_ALetter  }, /* 366 */
  { 0x10300, 0x1031E, PDF_TEXT_UCD_WBP_ALetter  }, /* 367 */
  { 0x10330, 0x10340, PDF_TEXT_UCD_WBP_ALetter  }, /* 368 */
  { 0x10341, 0x10341, PDF_TEXT_UCD_WBP_ALetter  }, /* 369 */
  { 0x10342, 0x10349, PDF_TEXT_UCD_WBP_ALetter  }, /* 370 */
  { 0x1034A, 0x1034A, PDF_TEXT_UCD_WBP_ALetter  }, /* 371 */
  { 0x10380, 0x1039D, PDF_TEXT_UCD_WBP_ALetter  }, /* 372 */
  { 0x103A0, 0x103C3, PDF_TEXT_UCD_WBP_ALetter  }, /* 373 */
  { 0x103C8, 0x103CF, PDF_TEXT_UCD_WBP_ALetter  }, /* 374 */
  { 0x103D1, 0x103D5, PDF_TEXT_UCD_WBP_ALetter  }, /* 375 */
  { 0x10400, 0x1044F, PDF_TEXT_UCD_WBP_ALetter  }, /* 376 */
  { 0x10450, 0x1049D, PDF_TEXT_UCD_WBP_ALetter  }, /* 377 */
  { 0x10800, 0x10805, PDF_TEXT_UCD_WBP_ALetter  }, /* 378 */
  { 0x10808, 0x10808, PDF_TEXT_UCD_WBP_ALetter  }, /* 379 */
  { 0x1080A, 0x10835, PDF_TEXT_UCD_WBP_ALetter  }, /* 380 */
  { 0x10837, 0x10838, PDF_TEXT_UCD_WBP_ALetter  }, /* 381 */
  { 0x1083C, 0x1083C, PDF_TEXT_UCD_WBP_ALetter  }, /* 382 */
  { 0x1083F, 0x1083F, PDF_TEXT_UCD_WBP_ALetter  }, /* 383 */
  { 0x10900, 0x10915, PDF_TEXT_UCD_WBP_ALetter  }, /* 384 */
  { 0x10A00, 0x10A00, PDF_TEXT_UCD_WBP_ALetter  }, /* 385 */
  { 0x10A10, 0x10A13, PDF_TEXT_UCD_WBP_ALetter  }, /* 386 */
  { 0x10A15, 0x10A17, PDF_TEXT_UCD_WBP_ALetter  }, /* 387 */
  { 0x10A19, 0x10A33, PDF_TEXT_UCD_WBP_ALetter  }, /* 388 */
  { 0x12000, 0x1236E, PDF_TEXT_UCD_WBP_ALetter  }, /* 389 */
  { 0x12400, 0x12462, PDF_TEXT_UCD_WBP_ALetter  }, /* 390 */
  { 0x1D400, 0x1D454, PDF_TEXT_UCD_WBP_ALetter  }, /* 391 */
  { 0x1D456, 0x1D49C, PDF_TEXT_UCD_WBP_ALetter  }, /* 392 */
  { 0x1D49E, 0x1D49F, PDF_TEXT_UCD_WBP_ALetter  }, /* 393 */
  { 0x1D4A2, 0x1D4A2, PDF_TEXT_UCD_WBP_ALetter  }, /* 394 */
  { 0x1D4A5, 0x1D4A6, PDF_TEXT_UCD_WBP_ALetter  }, /* 395 */
  { 0x1D4A9, 0x1D4AC, PDF_TEXT_UCD_WBP_ALetter  }, /* 396 */
  { 0x1D4AE, 0x1D4B9, PDF_TEXT_UCD_WBP_ALetter  }, /* 397 */
  { 0x1D4BB, 0x1D4BB, PDF_TEXT_UCD_WBP_ALetter  }, /* 398 */
  { 0x1D4BD, 0x1D4C3, PDF_TEXT_UCD_WBP_ALetter  }, /* 399 */
  { 0x1D4C5, 0x1D505, PDF_TEXT_UCD_WBP_ALetter  }, /* 400 */
  { 0x1D507, 0x1D50A, PDF_TEXT_UCD_WBP_ALetter  }, /* 401 */
  { 0x1D50D, 0x1D514, PDF_TEXT_UCD_WBP_ALetter  }, /* 402 */
  { 0x1D516, 0x1D51C, PDF_TEXT_UCD_WBP_ALetter  }, /* 403 */
  { 0x1D51E, 0x1D539, PDF_TEXT_UCD_WBP_ALetter  }, /* 404 */
  { 0x1D53B, 0x1D53E, PDF_TEXT_UCD_WBP_ALetter  }, /* 405 */
  { 0x1D540, 0x1D544, PDF_TEXT_UCD_WBP_ALetter  }, /* 406 */
  { 0x1D546, 0x1D546, PDF_TEXT_UCD_WBP_ALetter  }, /* 407 */
  { 0x1D54A, 0x1D550, PDF_TEXT_UCD_WBP_ALetter  }, /* 408 */
  { 0x1D552, 0x1D6A5, PDF_TEXT_UCD_WBP_ALetter  }, /* 409 */
  { 0x1D6A8, 0x1D6C0, PDF_TEXT_UCD_WBP_ALetter  }, /* 410 */
  { 0x1D6C2, 0x1D6DA, PDF_TEXT_UCD_WBP_ALetter  }, /* 411 */
  { 0x1D6DC, 0x1D6FA, PDF_TEXT_UCD_WBP_ALetter  }, /* 412 */
  { 0x1D6FC, 0x1D714, PDF_TEXT_UCD_WBP_ALetter  }, /* 413 */
  { 0x1D716, 0x1D734, PDF_TEXT_UCD_WBP_ALetter  }, /* 414 */
  { 0x1D736, 0x1D74E, PDF_TEXT_UCD_WBP_ALetter  }, /* 415 */
  { 0x1D750, 0x1D76E, PDF_TEXT_UCD_WBP_ALetter  }, /* 416 */
  { 0x1D770, 0x1D788, PDF_TEXT_UCD_WBP_ALetter  }, /* 417 */
  { 0x1D78A, 0x1D7A8, PDF_TEXT_UCD_WBP_ALetter  }, /* 418 */
  { 0x1D7AA, 0x1D7C2, PDF_TEXT_UCD_WBP_ALetter  }, /* 419 */
  { 0x1D7C4, 0x1D7CB, PDF_TEXT_UCD_WBP_ALetter  }, /* 420 */
  { 0x0027, 0x0027, PDF_TEXT_UCD_WBP_MidLetter  }, /* 421 */
  { 0x003A, 0x003A, PDF_TEXT_UCD_WBP_MidLetter  }, /* 422 */
  { 0x00B7, 0x00B7, PDF_TEXT_UCD_WBP_MidLetter  }, /* 423 */
  { 0x05F4, 0x05F4, PDF_TEXT_UCD_WBP_MidLetter  }, /* 424 */
  { 0x2019, 0x2019, PDF_TEXT_UCD_WBP_MidLetter  }, /* 425 */
  { 0x2027, 0x2027, PDF_TEXT_UCD_WBP_MidLetter  }, /* 426 */
  { 0x002C, 0x002C, PDF_TEXT_UCD_WBP_MidNum  }, /* 427 */
  { 0x002E, 0x002E, PDF_TEXT_UCD_WBP_MidNum  }, /* 428 */
  { 0x003B, 0x003B, PDF_TEXT_UCD_WBP_MidNum  }, /* 429 */
  { 0x037E, 0x037E, PDF_TEXT_UCD_WBP_MidNum  }, /* 430 */
  { 0x0589, 0x0589, PDF_TEXT_UCD_WBP_MidNum  }, /* 431 */
  { 0x060D, 0x060D, PDF_TEXT_UCD_WBP_MidNum  }, /* 432 */
  { 0x07F8, 0x07F8, PDF_TEXT_UCD_WBP_MidNum  }, /* 433 */
  { 0x2044, 0x2044, PDF_TEXT_UCD_WBP_MidNum  }, /* 434 */
  { 0xFE10, 0xFE10, PDF_TEXT_UCD_WBP_MidNum  }, /* 435 */
  { 0xFE13, 0xFE14, PDF_TEXT_UCD_WBP_MidNum  }, /* 436 */
  { 0x0030, 0x0039, PDF_TEXT_UCD_WBP_Numeric  }, /* 437 */
  { 0x0660, 0x0669, PDF_TEXT_UCD_WBP_Numeric  }, /* 438 */
  { 0x066B, 0x066C, PDF_TEXT_UCD_WBP_Numeric  }, /* 439 */
  { 0x06F0, 0x06F9, PDF_TEXT_UCD_WBP_Numeric  }, /* 440 */
  { 0x07C0, 0x07C9, PDF_TEXT_UCD_WBP_Numeric  }, /* 441 */
  { 0x0966, 0x096F, PDF_TEXT_UCD_WBP_Numeric  }, /* 442 */
  { 0x09E6, 0x09EF, PDF_TEXT_UCD_WBP_Numeric  }, /* 443 */
  { 0x0A66, 0x0A6F, PDF_TEXT_UCD_WBP_Numeric  }, /* 444 */
  { 0x0AE6, 0x0AEF, PDF_TEXT_UCD_WBP_Numeric  }, /* 445 */
  { 0x0B66, 0x0B6F, PDF_TEXT_UCD_WBP_Numeric  }, /* 446 */
  { 0x0BE6, 0x0BEF, PDF_TEXT_UCD_WBP_Numeric  }, /* 447 */
  { 0x0C66, 0x0C6F, PDF_TEXT_UCD_WBP_Numeric  }, /* 448 */
  { 0x0CE6, 0x0CEF, PDF_TEXT_UCD_WBP_Numeric  }, /* 449 */
  { 0x0D66, 0x0D6F, PDF_TEXT_UCD_WBP_Numeric  }, /* 450 */
  { 0x0E50, 0x0E59, PDF_TEXT_UCD_WBP_Numeric  }, /* 451 */
  { 0x0ED0, 0x0ED9, PDF_TEXT_UCD_WBP_Numeric  }, /* 452 */
  { 0x0F20, 0x0F29, PDF_TEXT_UCD_WBP_Numeric  }, /* 453 */
  { 0x1040, 0x1049, PDF_TEXT_UCD_WBP_Numeric  }, /* 454 */
  { 0x17E0, 0x17E9, PDF_TEXT_UCD_WBP_Numeric  }, /* 455 */
  { 0x1810, 0x1819, PDF_TEXT_UCD_WBP_Numeric  }, /* 456 */
  { 0x1946, 0x194F, PDF_TEXT_UCD_WBP_Numeric  }, /* 457 */
  { 0x19D0, 0x19D9, PDF_TEXT_UCD_WBP_Numeric  }, /* 458 */
  { 0x1B50, 0x1B59, PDF_TEXT_UCD_WBP_Numeric  }, /* 459 */
  { 0x104A0, 0x104A9, PDF_TEXT_UCD_WBP_Numeric  }, /* 460 */
  { 0x1D7CE, 0x1D7FF, PDF_TEXT_UCD_WBP_Numeric  }, /* 461 */
  { 0x005F, 0x005F, PDF_TEXT_UCD_WBP_ExtendNumLet  }, /* 462 */
  { 0x203F, 0x2040, PDF_TEXT_UCD_WBP_ExtendNumLet  }, /* 463 */
  { 0x2054, 0x2054, PDF_TEXT_UCD_WBP_ExtendNumLet  }, /* 464 */
  { 0xFE33, 0xFE34, PDF_TEXT_UCD_WBP_ExtendNumLet  }, /* 465 */
  { 0xFE4D, 0xFE4F, PDF_TEXT_UCD_WBP_ExtendNumLet  }, /* 466 */
  { 0xFF3F, 0xFF3F, PDF_TEXT_UCD_WBP_ExtendNumLet  } /* 467 */
};



static pdf_bool_t
pdf_text_ucd_wb_in_interval(pdf_u32_t character,
                            pdf_u32_t first_interval,
                            pdf_u32_t last_interval)
{
  extern unicode_wordbreak_info_t unicode_wordbreak_info[UCD_WB_INFO_N];
  int i;
  
  for(i=first_interval; i<=UCD_WB_FORMAT_L; ++i)
    {
      if((character >= unicode_wordbreak_info[i].interval_start) && \
         (character <= unicode_wordbreak_info[i].interval_stop))
        {
          return PDF_TRUE;
        }
    }
  return PDF_FALSE;
}


/* Returns true if the given UTF-32HE unicode point has the Format value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_format(pdf_u32_t character)
{
  return pdf_text_ucd_wb_in_interval(character, UCD_WB_FORMAT_F, \
                                     UCD_WB_FORMAT_L);
}


/* Returns true if the given UTF-32HE unicode point has the Katakana value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_katakana(pdf_u32_t character)
{
  return pdf_text_ucd_wb_in_interval(character, UCD_WB_KATAKANA_F, \
                                     UCD_WB_KATAKANA_L);
}


/* Returns true if the given UTF-32HE unicode point has the ALetter value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_aletter(pdf_u32_t character)
{
  return pdf_text_ucd_wb_in_interval(character, UCD_WB_ALETTER_F, \
                                     UCD_WB_ALETTER_L);
}



/* Returns true if the given UTF-32HE unicode point has the MidLetter value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_midletter(pdf_u32_t character)
{
  return pdf_text_ucd_wb_in_interval(character, UCD_WB_MIDLETTER_F, \
                                     UCD_WB_MIDLETTER_L);
}


/* Returns true if the given UTF-32HE unicode point has the MidNum value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_midnum(pdf_u32_t character)
{
  return pdf_text_ucd_wb_in_interval(character, UCD_WB_MIDNUM_F, \
                                     UCD_WB_MIDNUM_L);
}


/* Returns true if the given UTF-32HE unicode point has the Numeric value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_numeric(pdf_u32_t character)
{
  return pdf_text_ucd_wb_in_interval(character, UCD_WB_NUMERIC_F, \
                                     UCD_WB_NUMERIC_L);
}


/* Returns true if the given UTF-32HE unicode point has the ExtendNumLet value
 *  in the WordBreak property */
pdf_bool_t
pdf_text_ucd_wb_is_extendnumlet(pdf_u32_t character)
{
  return pdf_text_ucd_wb_in_interval(character, UCD_WB_EXTENDNUMLET_F, \
                                     UCD_WB_EXTENDNUMLET_L);
}


enum pdf_text_ucd_wb_property_e
pdf_text_ucd_wb_get_property(pdf_u32_t character)
{
  if(pdf_text_ucd_wb_is_aletter(character))
    {
      return PDF_TEXT_UCD_WBP_ALetter;
    }
  else if(pdf_text_ucd_wb_is_midletter(character))
    {
      return PDF_TEXT_UCD_WBP_MidLetter;
    }
  else if(pdf_text_ucd_wb_is_numeric(character))
    {
      return PDF_TEXT_UCD_WBP_Numeric;
    }
  else if(pdf_text_ucd_wb_is_midnum(character))
    {
      return PDF_TEXT_UCD_WBP_MidNum;
    }
  else if(pdf_text_ucd_wb_is_format(character))
    {
      return PDF_TEXT_UCD_WBP_Format;
    }
  else if(pdf_text_ucd_wb_is_katakana(character))
    {
      return PDF_TEXT_UCD_WBP_Katakana;
    }
  else if(pdf_text_ucd_wb_is_extendnumlet(character))
    {
      return PDF_TEXT_UCD_WBP_ExtendNumLet;
    }
  else
    {
      return PDF_TEXT_UCD_WBP_None;
    }
}


/* Maximum number of code points needed for a word break check */
#define PDF_TEXT_UCD_MWBCP  4

/* Word break property information */
typedef struct pdf_text_ucd_wb_s {
  pdf_char_t *walker;
  pdf_u32_t  utf32val;
  enum pdf_text_ucd_wb_property_e wbp;
} pdf_text_ucd_wb_t;


/* RULE WB3: Do not break within CRLF (CR x LF) */
static pdf_bool_t
pdf_text_ucd_wb_rule_3(const pdf_text_ucd_wb_t buffer [PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[1].utf32val == PDF_TEXT_DEF_CR) && \
          (buffer[2].utf32val == PDF_TEXT_DEF_LF)) ? PDF_TRUE : PDF_FALSE);
}

/* RULE WB5: Do not break between most letters (ALetter X ALetter) */
static pdf_bool_t
pdf_text_ucd_wb_rule_5(const pdf_text_ucd_wb_t buffer [PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[1].wbp == PDF_TEXT_UCD_WBP_ALetter) && \
           (buffer[2].wbp == PDF_TEXT_UCD_WBP_ALetter)) ? PDF_TRUE : PDF_FALSE);
}
    
/* RULE WB6: Do not break letters across certain puntuation 
 *  (ALetter X MidLetter ALetter) */
static pdf_bool_t
pdf_text_ucd_wb_rule_6(const pdf_text_ucd_wb_t buffer [PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[1].wbp == PDF_TEXT_UCD_WBP_ALetter) && \
           (buffer[2].wbp == PDF_TEXT_UCD_WBP_MidLetter) && \
           (buffer[3].wbp == PDF_TEXT_UCD_WBP_ALetter)) ? PDF_TRUE : PDF_FALSE);
}


/* RULE WB7: Do not break letters across certain punctuation
 *  (ALetter MidLetter X ALetter) */
static pdf_bool_t
pdf_text_ucd_wb_rule_7(const pdf_text_ucd_wb_t buffer [PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[0].wbp == PDF_TEXT_UCD_WBP_ALetter) && \
           (buffer[1].wbp == PDF_TEXT_UCD_WBP_MidLetter) && \
           (buffer[2].wbp == PDF_TEXT_UCD_WBP_ALetter)) ? PDF_TRUE : PDF_FALSE);
}


/* RULE WB8: Do not break within sequences of digits, or digits adjacent
 *  to letters (Numeric X Numeric) */
static pdf_bool_t
pdf_text_ucd_wb_rule_8(const pdf_text_ucd_wb_t buffer [PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[1].wbp == PDF_TEXT_UCD_WBP_Numeric) && \
           (buffer[2].wbp == PDF_TEXT_UCD_WBP_Numeric)) ? PDF_TRUE : PDF_FALSE);
}

/* RULE WB9: Do not break within sequences of digits, or digits adjacent
 *  to letters (ALetter X Numeric) */
static pdf_bool_t
pdf_text_ucd_wb_rule_9(const pdf_text_ucd_wb_t buffer [PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[1].wbp == PDF_TEXT_UCD_WBP_ALetter) && \
           (buffer[2].wbp == PDF_TEXT_UCD_WBP_Numeric)) ? PDF_TRUE : PDF_FALSE);
}


/* RULE WB10: Do not break within sequences of digits, or digits adjacent
 *  to letters (Numeric X ALetter) */
static pdf_bool_t
pdf_text_ucd_wb_rule_10(const pdf_text_ucd_wb_t buffer [PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[0].wbp == PDF_TEXT_UCD_WBP_Numeric) && \
           (buffer[1].wbp == PDF_TEXT_UCD_WBP_ALetter)) ? PDF_TRUE : PDF_FALSE);
}


/* RULE WB11: Do not break within sequences such as "3.2"
 *  (Numeric MidNum X Numeric) */
static pdf_bool_t
pdf_text_ucd_wb_rule_11(const pdf_text_ucd_wb_t buffer [PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[0].wbp == PDF_TEXT_UCD_WBP_Numeric) && \
           (buffer[1].wbp == PDF_TEXT_UCD_WBP_MidNum) && \
           (buffer[2].wbp == PDF_TEXT_UCD_WBP_Numeric)) ? PDF_TRUE : PDF_FALSE);
}

/* RULE WB12: Do not break within sequences such as "3.2"
 *  (Numeric X MidNum Numeric) */
static pdf_bool_t
pdf_text_ucd_wb_rule_12(const pdf_text_ucd_wb_t buffer [PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[1].wbp == PDF_TEXT_UCD_WBP_Numeric) && \
           (buffer[2].wbp == PDF_TEXT_UCD_WBP_MidNum) && \
           (buffer[3].wbp == PDF_TEXT_UCD_WBP_Numeric)) ? PDF_TRUE : PDF_FALSE);
}

/* RULE WB13: Do not break between Katakana */
static pdf_bool_t
pdf_text_ucd_wb_rule_13(const pdf_text_ucd_wb_t buffer [PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[1].wbp == PDF_TEXT_UCD_WBP_Katakana) && \
           (buffer[2].wbp == PDF_TEXT_UCD_WBP_Katakana)) ? PDF_TRUE :PDF_FALSE);
}

/* RULE WB13a: Do not break from extenders
 *  ((ALetter | Numeric | Katakana | ExtendNumLet) X ExtendNumLet) */
static pdf_bool_t
pdf_text_ucd_wb_rule_13a(const pdf_text_ucd_wb_t buffer [PDF_TEXT_UCD_MWBCP])
{
  return ((((buffer[1].wbp == PDF_TEXT_UCD_WBP_ALetter) || \
            (buffer[1].wbp == PDF_TEXT_UCD_WBP_Numeric) || \
            (buffer[1].wbp == PDF_TEXT_UCD_WBP_Katakana) || \
            (buffer[1].wbp == PDF_TEXT_UCD_WBP_ExtendNumLet)) && \
           (buffer[2].wbp == PDF_TEXT_UCD_WBP_ExtendNumLet)) ? \
          PDF_TRUE : PDF_FALSE);
}

/* RULE WB13b: Do not break from extenders
 *  (ExtendNumLet) X (ALetter | Numeric | Katakana ) */
static pdf_bool_t
pdf_text_ucd_wb_rule_13b(const pdf_text_ucd_wb_t buffer [PDF_TEXT_UCD_MWBCP])
{
  return (((buffer[1].wbp == PDF_TEXT_UCD_WBP_ExtendNumLet) && \
           ((buffer[2].wbp == PDF_TEXT_UCD_WBP_ALetter) || \
            (buffer[2].wbp == PDF_TEXT_UCD_WBP_Numeric) || \
            (buffer[2].wbp == PDF_TEXT_UCD_WBP_Katakana))) ? \
          PDF_TRUE : PDF_FALSE);
}

/* Check rules and stop if any of them is true (meaning that shouldn't be a
 *  word break) */
static pdf_bool_t
pdf_text_ucd_wb_check_rules(const pdf_text_ucd_wb_t buffer [PDF_TEXT_UCD_MWBCP])
{
  return (((pdf_text_ucd_wb_rule_3(buffer)) || \
           (pdf_text_ucd_wb_rule_5(buffer)) || \
           (pdf_text_ucd_wb_rule_6(buffer)) || \
           (pdf_text_ucd_wb_rule_7(buffer)) || \
           (pdf_text_ucd_wb_rule_8(buffer)) || \
           (pdf_text_ucd_wb_rule_9(buffer)) || \
           (pdf_text_ucd_wb_rule_10(buffer)) || \
           (pdf_text_ucd_wb_rule_11(buffer)) || \
           (pdf_text_ucd_wb_rule_12(buffer)) || \
           (pdf_text_ucd_wb_rule_13(buffer)) || \
           (pdf_text_ucd_wb_rule_13a(buffer)) || \
           (pdf_text_ucd_wb_rule_13b(buffer))) ? PDF_TRUE : PDF_FALSE);
}


/* Word boundary search algorithm, based on Unicode Standard Annex #29
 *  "Text Boundaries".
 * - `current' points to the next byte after the word break (so it points to the
 *    FIRST byte of the word).
 * - `next' will point to the previous byte before the next word break (so it
 *    points to the LAST byte of the word).
 */
pdf_status_t
pdf_text_ucd_wb_detect_next(const pdf_char_t *current,
                            const pdf_size_t n_bytes_left_in,
                            pdf_char_t **next,
                            pdf_size_t *n_bytes_left_out)
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
  pdf_text_ucd_wb_t buffer [PDF_TEXT_UCD_MWBCP];

  pdf_u32_t i;
  pdf_size_t n_bytes;
  pdf_bool_t found;
  
  /* Check validity of input number of bytes */
  if(n_bytes_left_in % 4 != 0)
    {
      return PDF_EINVAL;
    }
  
  /* Check if the string is just one character long */
  if(n_bytes_left_in == 4)
    {
      *n_bytes_left_out = 0;
      *next = (pdf_char_t *)current;
      return PDF_OK;
    }
  
  /* Initialize buffer with first 3 unicode points, stored in [1],[2],[3] */
  for(i=0; i<PDF_TEXT_UCD_MWBCP; ++i)
    {
      if((i>0) && \
         (n_bytes_left_in >= (4*i)))
        {
          /* Store pointer */
          buffer[i].walker = (pdf_char_t *)(&current[4*(i-1)]);
          /* Store unsigned 32-bit number */
          memcpy(&(buffer[i].utf32val), buffer[i].walker, 4);
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
  while((!found) && \
        (n_bytes >= 8))
    {
      /* If any of the rules returns true, don't break word */
      if(pdf_text_ucd_wb_check_rules(buffer))
        {
          /* If word break is not found, continue with next UTF-32 point */
          /* Update number of bytes pending */
          n_bytes -= 4;

          /* Shift left contents of the buffer */
          for(i=1; i<PDF_TEXT_UCD_MWBCP; ++i)
            {
              buffer[i-1] = buffer[i];
            }
          
          /* Insert new buffer element in position [3], if available */
          if(n_bytes >= 12)
            {
              buffer[3].walker = buffer[2].walker + 4;
              /* Store unsigned 32-bit number */
              memcpy(&(buffer[3].utf32val), buffer[3].walker, 4);
              /* Get Word-Break property value from character */
              buffer[3].wbp =pdf_text_ucd_wb_get_property(buffer[3].utf32val);
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
  
  return PDF_OK;
}

/* End of pdf-text-ucd-wordbreak.c */

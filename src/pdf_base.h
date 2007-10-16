/* -*- mode: C -*- Time-stamp: "07/09/18 04:59:46 jemarch"
 *
 *       File:         pdf_base.h
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Fri Jul  6 20:22:05 2007
 *
 *       GNU PDF Library - Common resources for the library modules
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

#ifndef _PDF_BASE_H
#define _PDF_BASE_H

#include <config.h>
#include <sys/types.h> /* for off_t */

#ifdef HAVE_INLINE
#define INLINE inline
#else
#define INLINE
#endif /* HAVE_INLINE */

#define PDF_TRUE 1
#define PDF_FALSE 0

#define PDF_OK PDF_TRUE
#define PDF_ERROR PDF_FALSE

/* A variable of type `pdf_stm_pos_t' contain a byte-offset relative to
   the beginning of a stream object. 

   Please be careful manipulating `pdf_stm_pos_t' values. Its value is
   assured to be a signed scalable one, but its size may be wider than
   a long. 

   Note that this is a ugly hack: we define `pdf_stm_pos_t' in
   `pdf_base.h' just because the stream backends need to use it. Its
   proper place would be `pdf_stm.h'. */

#define NO_POS -1
typedef off_t pdf_stm_pos_t;

/* PDF manages bytes with values in the range 0-255 */

typedef unsigned char pdf_char_t;


/* PDF integers and reals are implemented using basic C types. */

typedef int pdf_integer_t;
typedef float pdf_real_t;

/* PDF_EOF to store an EOF marker in integers */
#define PDF_EOF -1

/* Macro to access the global state of the library pdf_globals */
#define PDF_GLOBALS extern struct pdf_globals_s pdf_globals;

#define PDF_NAMES 163 /* Number of preallocated PDF names */

/* Limits in the size of PDF objects */
#define PDF_NAME_MAX 127

/* Macros to access preallocated PDF names. The `pdf_globals' variable
   is defined in `pdf.c' and its data type is defined in `pdf.h' */

#define PDF_GNAME(num) (pdf_globals.names[(num)])

#define PDF_N_LENGTH                    PDF_GNAME(0)
#define PDF_S_LENGTH                    "Length"
#define PDF_N_FILTER                    PDF_GNAME(1)
#define PDF_S_FILTER                    "Filter"
#define PDF_N_SUBFILTER                 PDF_GNAME(2)
#define PDF_S_SUBFILTER                 "SubFilter"
#define PDF_N_DECODE_PARMS              PDF_GNAME(3)
#define PDF_S_DECODE_PARMS              "DecodeParms"
#define PDF_N_F                         PDF_GNAME(4)
#define PDF_S_F                         "F"
#define PDF_N_F_FILTER                  PDF_GNAME(5)
#define PDF_S_F_FILTER                  "FFilter"
#define PDF_N_F_DECODE_PARMS            PDF_GNAME(6)
#define PDF_S_F_DECODE_PARMS            "FDecodeParms"
#define PDF_N_DL                        PDF_GNAME(7)
#define PDF_S_DL                        "DL"
#define PDF_N_ASCII_HEX_DECODE          PDF_GNAME(8)
#define PDF_S_ASCII_HEX_DECODE          "ASCIIHexDecode"
#define PDF_N_ASCII85_DECODE            PDF_GNAME(9)
#define PDF_S_ASCII85_DECODE            "ASCII85Decode"
#define PDF_N_LZW_DECODE                PDF_GNAME(10)
#define PDF_S_LZW_DECODE                "LZWDecode"
#define PDF_N_FLATE_DECODE              PDF_GNAME(11)
#define PDF_S_FLATE_DECODE              "FlateDecode"
#define PDF_N_RUN_LENGTH_DECODE         PDF_GNAME(12)
#define PDF_S_RUN_LENGTH_DECODE         "RunLengthDecode"
#define PDF_N_CCITT_FAX_DECODE          PDF_GNAME(13)
#define PDF_S_CCITT_FAX_DECODE          "CCITTFaxDecode"
#define PDF_N_JBIG2_DECODE              PDF_GNAME(14)
#define PDF_S_JBIG2_DECODE              "JBIG2Decode"
#define PDF_N_DCT_DECODE                PDF_GNAME(15)
#define PDF_S_DCT_DECODE                "DCTDecode"
#define PDF_N_JPX_DECODE                PDF_GNAME(16)
#define PDF_S_JPX_DECODE                "JPXDecode"
#define PDF_N_CRYPT                     PDF_GNAME(17)
#define PDF_S_CRYPT                     "Crypt"
#define PDF_N_PREDICTOR                 PDF_GNAME(18)
#define PDF_S_PREDICTOR                 "Predictor"
#define PDF_N_COLORS                    PDF_GNAME(19)
#define PDF_S_COLORS                    "Colors"
#define PDF_N_BITS_PER_COMPONENT        PDF_GNAME(20)
#define PDF_S_BITS_PER_COMPONENT        "BitsPerComponent"
#define PDF_N_COLUMNS                   PDF_GNAME(21)
#define PDF_S_COLUMNS                   "Columns"
#define PDF_N_EARLY_CHANGE              PDF_GNAME(22)
#define PDF_S_EARLY_CHANGE              "EarlyChange"
#define PDF_N_K                         PDF_GNAME(23)
#define PDF_S_K                         "K"
#define PDF_N_END_OF_LINE               PDF_GNAME(24)
#define PDF_S_END_OF_LINE               "EndOfLine"
#define PDF_N_ENCODED_BYTE_ALIGN        PDF_GNAME(25)
#define PDF_S_ENCODED_BYTE_ALIGN        "EncodedByteAlign"
#define PDF_N_ROWS                      PDF_GNAME(26)
#define PDF_S_ROWS                      "Rows"
#define PDF_N_END_OF_BLOCK              PDF_GNAME(27)
#define PDF_S_END_OF_BLOCK              "EndOfBlock"
#define PDF_N_BLACKIS1                  PDF_GNAME(28)
#define PDF_S_BLACKIS1                  "BlackIs1"
#define PDF_N_DAMAGED_ROWS_BEFORE_ERROR PDF_GNAME(29)
#define PDF_S_DAMAGED_ROWS_BEFORE_ERROR "DamagedRowsBeforeError"
#define PDF_N_JBIG2_GLOBALS             PDF_GNAME(30)
#define PDF_S_JBIG2_GLOBALS             "JBIG2Globals"
#define PDF_N_COLOR_TRANSFORM           PDF_GNAME(31)
#define PDF_S_COLOR_TRANSFORM           "ColorTransform"
#define PDF_N_TYPE                      PDF_GNAME(32)
#define PDF_S_TYPE                      "Type"
#define PDF_N_SUBTYPE                   PDF_GNAME(33)
#define PDF_S_SUBTYPE                   "SubType"
#define PDF_N_NAME                      PDF_GNAME(34)
#define PDF_S_NAME                      "Name"
#define PDF_N_PREV                      PDF_GNAME(35)
#define PDF_S_PREV                      "Prev"
#define PDF_N_ROOT                      PDF_GNAME(36)
#define PDF_S_ROOT                      "Root"
#define PDF_N_ENCRYPT                   PDF_GNAME(37)
#define PDF_S_ENCRYPT                   "Encrypt"
#define PDF_N_ENCRYPT_METADATA          PDF_GNAME(38)
#define PDF_S_ENCRYPT_METADATA          "EncryptMetadata"
#define PDF_N_INFO                      PDF_GNAME(39)
#define PDF_S_INFO                      "Info"
#define PDF_N_ID                        PDF_GNAME(40)
#define PDF_S_ID                        "ID"
#define PDF_N_FIRST                     PDF_GNAME(41)
#define PDF_S_FIRST                     "First"
#define PDF_N_EXTENDS                   PDF_GNAME(42)
#define PDF_S_EXTENDS                   "Extends"
#define PDF_N_INDEX                     PDF_GNAME(43)
#define PDF_S_INDEX                     "Index"
#define PDF_N_W                         PDF_GNAME(44)
#define PDF_S_W                         "W"
#define PDF_N_XREF_STM                  PDF_GNAME(45)
#define PDF_S_XREF_STM                  "XrefStm"
#define PDF_N_V                         PDF_GNAME(46)
#define PDF_S_V                         "V"
#define PDF_N_CF                        PDF_GNAME(47)
#define PDF_S_CF                        "CF"
#define PDF_N_CFM                       PDF_GNAME(48)
#define PDF_S_CFM                       "CFM"
#define PDF_N_NONE                      PDF_GNAME(49)
#define PDF_S_NONE                      "None"
#define PDF_N_V2                        PDF_GNAME(50)
#define PDF_S_V2                        "V2"
#define PDF_N_AES_V2                    PDF_GNAME(51)
#define PDF_S_AES_V2                    "AESV2"
#define PDF_N_AUTH_EVENT                PDF_GNAME(52)
#define PDF_S_AUTH_EVENT                "AuthEvent"
#define PDF_N_IDENTITY                  PDF_GNAME(53)
#define PDF_S_IDENTITY                  "Identity"
#define PDF_N_STM_F                     PDF_GNAME(54)
#define PDF_S_STM_F                     "StmF"
#define PDF_N_STR_F                     PDF_GNAME(55)
#define PDF_S_STR_F                     "StrF"
#define PDF_N_EFF                       PDF_GNAME(56)
#define PDF_S_EFF                       "EFF"
#define PDF_N_R                         PDF_GNAME(57)
#define PDF_S_R                         "R"
#define PDF_N_O                         PDF_GNAME(58)
#define PDF_S_O                         "O"
#define PDF_N_U                         PDF_GNAME(59)
#define PDF_S_U                         "U"
#define PDF_N_P                         PDF_GNAME(60)
#define PDF_S_P                         "P"
#define PDF_N_RECIPIENTS                PDF_GNAME(61)
#define PDF_S_RECIPIENTS                "Recipients"
#define PDF_N_FUNCTION_TYPE             PDF_GNAME(62)
#define PDF_S_FUNCTION_TYPE             "FunctionType"
#define PDF_N_DOMAIN                    PDF_GNAME(63)
#define PDF_S_DOMAIN                    "Domain"
#define PDF_N_RANGE                     PDF_GNAME(64)
#define PDF_S_RANGE                     "Range"
#define PDF_N_SIZE                      PDF_GNAME(65)
#define PDF_S_SIZE                      "Size"
#define PDF_N_BITS_PER_SAMPLE           PDF_GNAME(66)
#define PDF_S_BITS_PER_SAMPLE           "BitsPerSample"
#define PDF_N_ORDER                     PDF_GNAME(67)
#define PDF_S_ORDER                     "Order"
#define PDF_N_ENCODE                    PDF_GNAME(68)
#define PDF_S_ENCODE                    "Encode"
#define PDF_N_DECODE                    PDF_GNAME(69)
#define PDF_S_DECODE                    "Decode"
#define PDF_N_C0                        PDF_GNAME(70)
#define PDF_S_C0                        "C0"
#define PDF_N_C1                        PDF_GNAME(71)
#define PDF_S_C1                        "C1"
#define PDF_N_N                         PDF_GNAME(72)
#define PDF_S_N                         "N"
#define PDF_N_FUNCTIONS                 PDF_GNAME(73)
#define PDF_S_FUNCTIONS                 "Functions"
#define PDF_N_BOUNDS                    PDF_GNAME(74)
#define PDF_S_BOUNDS                    "Bounds"
#define PDF_N_VERSION                   PDF_GNAME(75)
#define PDF_S_VERSION                   "Version"
#define PDF_N_PAGES                     PDF_GNAME(76)
#define PDF_S_PAGES                     "Pages"
#define PDF_N_PAGE_LABELS               PDF_GNAME(77)
#define PDF_S_PAGE_LABELS               "PageLabels"
#define PDF_N_NAMES                     PDF_GNAME(78)
#define PDF_S_NAMES                     "Names"
#define PDF_N_DESTS                     PDF_GNAME(79)
#define PDF_S_DESTS                     "Dests"
#define PDF_N_VIEWER_PREFERENCES        PDF_GNAME(80)
#define PDF_S_VIEWER_PREFERENCES        "ViewerPreferences"
#define PDF_N_PAGE_LAYOUT               PDF_GNAME(81)
#define PDF_S_PAGE_LAYOUT               "PageLayout"
#define PDF_N_PAGE_MODE                 PDF_GNAME(82)
#define PDF_S_PAGE_MODE                 "PageMode"
#define PDF_N_OUTLINES                  PDF_GNAME(83)
#define PDF_S_OUTLINES                  "Outlines"
#define PDF_N_THREADS                   PDF_GNAME(84)
#define PDF_S_THREADS                   "Threads"
#define PDF_N_OPEN_ACTION               PDF_GNAME(85)
#define PDF_S_OPEN_ACTION               "OpenAction"
#define PDF_N_AA                        PDF_GNAME(86)
#define PDF_S_AA                        "AA"
#define PDF_N_URI                       PDF_GNAME(87)
#define PDF_S_URI                       "URI"
#define PDF_N_METADATA                  PDF_GNAME(88)
#define PDF_S_METADATA                  "Metadata"
#define PDF_N_STRUCT_TREE_ROOT          PDF_GNAME(89)
#define PDF_S_STRUCT_TREE_ROOT          "StructTreeRoot"
#define PDF_N_MARK_INFO                 PDF_GNAME(90)
#define PDF_S_MARK_INFO                 "MarkInfo"
#define PDF_N_LANG                      PDF_GNAME(91)
#define PDF_S_LANG                      "Lang"
#define PDF_N_SPIDER_INFO               PDF_GNAME(92)
#define PDF_S_SPIDER_INFO               "SpiderInfo"
#define PDF_N_OUTPUT_INTENTS            PDF_GNAME(93)
#define PDF_S_OUTPUT_INTENTS            "OutputIntents"
#define PDF_N_PIECE_INFO                PDF_GNAME(94)
#define PDF_S_PIECE_INFO                "PieceInfo"
#define PDF_N_OC_PROPERTIES             PDF_GNAME(95)
#define PDF_S_OC_PROPERTIES             "OCProperties"
#define PDF_N_PERMS                     PDF_GNAME(96)
#define PDF_S_PERMS                     "Perms"
#define PDF_N_LEGAL                     PDF_GNAME(97)
#define PDF_S_LEGAL                     "Legal"
#define PDF_N_REQUIREMENTS              PDF_GNAME(98)
#define PDF_S_REQUIREMENTS              "Requirements"
#define PDF_N_COLLECTION                PDF_GNAME(99)
#define PDF_S_COLLECTION                "Collection"
#define PDF_N_NEEDS_RENDERING           PDF_GNAME(100)
#define PDF_S_NEEDS_RENDERING           "NeedsRendering"
#define PDF_N_PARENT                    PDF_GNAME(101)
#define PDF_S_PARENT                    "Parent"
#define PDF_N_KIDS                      PDF_GNAME(102)
#define PDF_S_KIDS                      "Kids"
#define PDF_N_COUNT                     PDF_GNAME(103)
#define PDF_S_COUNT                     "Count"
#define PDF_N_LAST_MODIFIED             PDF_GNAME(104)
#define PDF_S_LAST_MODIFIED             "LastModified"
#define PDF_N_RESOURCES                 PDF_GNAME(105)
#define PDF_S_RESOURCES                 "Resources"
#define PDF_N_MEDIA_BOX                 PDF_GNAME(106)
#define PDF_S_MEDIA_BOX                 "MediaBox"
#define PDF_N_CROP_BOX                  PDF_GNAME(107)
#define PDF_S_CROP_BOX                  "CropBox"
#define PDF_N_BLEED_BOX                 PDF_GNAME(108)
#define PDF_S_BLEED_BOX                 "BleedBox"
#define PDF_N_TRIM_BOX                  PDF_GNAME(109)
#define PDF_S_TRIM_BOX                  "TrimBox"
#define PDF_N_ART_BOX                   PDF_GNAME(110)
#define PDF_S_ART_BOX                   "ArtBox"
#define PDF_N_BOX_COLOR_INFO            PDF_GNAME(111)
#define PDF_S_BOX_COLOR_INFO            "BoxColorInfo"
#define PDF_N_CONTENTS                  PDF_GNAME(112)
#define PDF_S_CONTENTS                  "Contents"
#define PDF_N_ROTATE                    PDF_GNAME(113)
#define PDF_S_ROTATE                    "Rotate"
#define PDF_N_GROUP                     PDF_GNAME(114)
#define PDF_S_GROUP                     "Group"
#define PDF_N_THUMB                     PDF_GNAME(115)
#define PDF_S_THUMB                     "Thumb"
#define PDF_N_B                         PDF_GNAME(116)
#define PDF_S_B                         "B"
#define PDF_N_DUR                       PDF_GNAME(117)
#define PDF_S_DUR                       "Dur"
#define PDF_N_TRANS                     PDF_GNAME(118)
#define PDF_S_TRANS                     "Trans"
#define PDF_N_ANNOTS                    PDF_GNAME(119)
#define PDF_S_ANNOTS                    "Annots"
#define PDF_N_STRUCT_PARENTS            PDF_GNAME(120)
#define PDF_S_STRUCT_PARENTS            "StructParents"
#define PDF_N_PZ                        PDF_GNAME(121)
#define PDF_S_PZ                        "PZ"
#define PDF_N_SEPARATION_INFO           PDF_GNAME(122)
#define PDF_S_SEPARATION_INFO           "SeparationInfo"
#define PDF_N_TABS                      PDF_GNAME(123)
#define PDF_S_TABS                      "Tabs"
#define PDF_N_TEMPLATE_INSTANTIATE      PDF_GNAME(124)
#define PDF_S_TEMPLATE_INSTANTIATE      "TemplateInstantiated"
#define PDF_N_PRES_STEPS                PDF_GNAME(125)
#define PDF_S_PRES_STEPS                "PresSteps"
#define PDF_N_USER_UNIT                 PDF_GNAME(126)
#define PDF_S_USER_UNIT                 "UserUnit"
#define PDF_N_VP                        PDF_GNAME(127)
#define PDF_S_VP                        "VP"
#define PDF_N_AP                        PDF_GNAME(128)
#define PDF_S_AP                        "AP"
#define PDF_N_JAVASCRIPT                PDF_GNAME(129)
#define PDF_S_JAVASCRIPT                "JavaScript"
#define PDF_N_TEMPLATES                 PDF_GNAME(130)
#define PDF_S_TEMPLATES                 "Templates"
#define PDF_N_IDS                       PDF_GNAME(131)
#define PDF_S_IDS                       "IDS"
#define PDF_N_URLS                      PDF_GNAME(132)
#define PDF_S_URLS                      "URLS"
#define PDF_N_EMBEDDED_FILES            PDF_GNAME(133)
#define PDF_S_EMBEDDED_FILES            "EmbeddedFiles"
#define PDF_N_ALTERNATE_PRESENTATIONS   PDF_GNAME(134)
#define PDF_S_ALTERNATE_PRESENTATIONS   "AlternatePresentations"
#define PDF_N_RENDITIONS                PDF_GNAME(135)
#define PDF_S_RENDITIONS                "Renditions"
#define PDF_N_EXT_G_STATE               PDF_GNAME(136)
#define PDF_S_EXT_G_STATE               "ExtGState"
#define PDF_N_COLOR_SPACE               PDF_GNAME(137)
#define PDF_S_COLOR_SPACE               "ColorSpace"
#define PDF_N_PATTERN                   PDF_GNAME(138)
#define PDF_S_PATTERN                   "Pattern"
#define PDF_N_SHADING                   PDF_GNAME(139)
#define PDF_S_SHADING                   "Shading"
#define PDF_N_XOBJECT                   PDF_GNAME(140)
#define PDF_S_XOBJECT                   "XObject"
#define PDF_N_FONT                      PDF_GNAME(141)
#define PDF_S_FONT                      "Font"
#define PDF_N_PROC_SET                  PDF_GNAME(142)
#define PDF_S_PROC_SET                  "ProcSet"
#define PDF_N_PROPERTIES                PDF_GNAME(143)
#define PDF_S_PROPERTIES                "Properties"
#define PDF_N_LIMITS                    PDF_GNAME(144)
#define PDF_S_LIMITS                    "Limits"
#define PDF_N_NUMS                      PDF_GNAME(145)
#define PDF_S_NUMS                      "Nums"
#define PDF_N_FS                        PDF_GNAME(146)
#define PDF_S_FS                        "FS"
#define PDF_N_UF                        PDF_GNAME(147)
#define PDF_S_UF                        "UF"
#define PDF_N_DOS                       PDF_GNAME(148)
#define PDF_S_DOS                       "DOS"
#define PDF_N_MAC                       PDF_GNAME(149)
#define PDF_S_MAC                       "Mac"
#define PDF_N_UNIX                      PDF_GNAME(150)
#define PDF_S_UNIX                      "Unix"
#define PDF_N_EF                        PDF_GNAME(151)
#define PDF_S_EF                        "EF"
#define PDF_N_RF                        PDF_GNAME(152)
#define PDF_S_RF                        "RF"
#define PDF_N_DESC                      PDF_GNAME(153)
#define PDF_S_DESC                      "Desc"
#define PDF_N_CI                        PDF_GNAME(154)
#define PDF_S_CI                        "CI"
#define PDF_N_PARAMS                    PDF_GNAME(155)
#define PDF_S_PARAMS                    "Params"
#define PDF_N_CREATION_DATE             PDF_GNAME(156)
#define PDF_S_CREATION_DATE             "CreationDate"
#define PDF_N_MOD_DATE                  PDF_GNAME(157)
#define PDF_S_MOD_DATE                  "ModDate"
#define PDF_N_CHECKSUM                  PDF_GNAME(158)
#define PDF_S_CHECKSUM                  "CheckSum" 
#define PDF_N_CREATOR                   PDF_GNAME(159)
#define PDF_S_CREATOR                   "Creator"
#define PDF_N_RES_FORK                  PDF_GNAME(160)
#define PDF_S_RES_FORK                  "ResFork"
#define PDF_N_D                         PDF_GNAME(161)
#define PDF_S_D                         "D"
#define PDF_N_ACROFORM                  PDF_GNAME(162)
#define PDF_S_ACROFORM                  "AcroForm"



/* String utility functions */

int pdf_str2int (char *string, 
                 int *num);
int pdf_str2long (char *string, 
                  long *num);
int pdf_str2double (char *string, 
                    double *num);

/* Geometry */

typedef pdf_real_t *pdf_point_t;

#define P_X(point) (point)[0]
#define P_Y(point) (point)[1]

pdf_point_t pdf_create_point (void);
void pdf_destroy_point (pdf_point_t point);
pdf_point_t pdf_point_dup (pdf_point_t point);

/* Interpolation functions */

double pdf_interp_lineal (double x1, double y1,
                          double x2, double y2,
                          double x);

double pdf_interp_exp_coef_m (double x1, double x2,
                              double y1, double y2);
double pdf_interp_exp_coef_k (double x1, double y1,
                              double m);
double pdf_interp_exp (double m, double k,
                       double x);

#endif /* pdf_base.h */

/* End of pdf_base.h */

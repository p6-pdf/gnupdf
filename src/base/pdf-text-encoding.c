/* -*- mode: C -*- Time-stamp: ""
 *
 *       File:         pdf-text-encoding.c
 *       Date:         Fri Jan 11 21:09:56 2008
 *
 *       GNU PDF Library - Encoded Text handling utilities - Encoding
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

#include <string.h>
#include <stdio.h>
#include <pdf-text-encoding.h>
#include <pdf-text-context.h>


#define PDF_TEXT_CHANGE_ENDIANNESS_16BIT(number) \
  ((0x00FF & number) << 8) | ((0xFF00 & number) >> 8)

#define PDF_TEXT_CHANGE_ENDIANNESS_32BIT(number) \
  (((0x000000FF & number) << 24) | \
   ((0x0000FF00 & number) << 8 ) | \
   ((0x00FF0000 & number) >> 8 ) | \
   ((0xFF000000 & number) >> 24))


/* Mapping between PDF Doc Encoding and UNICODE UTF32 (Host Endian!)
 * Obtained from PDF Reference v1.7, appendix D.2 */
#define PDFDOCENC_MAX            256
static const pdf_u32_t pdfdocenc_map [PDFDOCENC_MAX] = { /* INDEXES */
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 00, 07 */
  0x0000, 0x0009, 0x000A, 0x0000, 0x0000, 0x000D, 0x0000, 0x0000, /* 08, 0F */
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, /* 10, 17 */
  0x02D8, 0x02C7, 0x02C6, 0x02D9, 0x02DD, 0x02DB, 0x02DA, 0x02DC, /* 18, 1F */
  0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, /* 20, 27 */
  0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F, /* 28, 2F */
  0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, /* 30, 37 */
  0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F, /* 38, 3F */
  0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, /* 40, 47 */
  0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F, /* 48, 4F */
  0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, /* 50, 57 */
  0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F, /* 58, 5F */
  0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, /* 60, 67 */
  0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F, /* 68, 6F */
  0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, /* 70, 77 */
  0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x0000, /* 78, 7F */
  0x2022, 0x2020, 0x2021, 0x2026, 0x2014, 0x2013, 0x0192, 0x2044, /* 80, 87 */
  0x2039, 0x203A, 0x2212, 0x2030, 0x201E, 0x201C, 0x201D, 0x2018, /* 88, 8F */
  0x2019, 0x201A, 0x2122, 0xFB01, 0xFB02, 0x0141, 0x0152, 0x0160, /* 90, 97 */
  0x0178, 0x017D, 0x0131, 0x0142, 0x0153, 0x0161, 0x017E, 0x0000, /* 98, 9F */
  0x20AC, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7, /* A0, A7 */
  0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x0000, 0x00AE, 0x00AF, /* A8, AF */
  0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7, /* B0, B7 */
  0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF, /* B8, BF */
  0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7, /* C0, C7 */
  0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF, /* C8, CF */
  0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7, /* D0, D7 */
  0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x00DF, /* D8, DF */
  0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7, /* E0, E7 */
  0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF, /* E8, EF */
  0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F7, /* F0, F7 */
  0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x00FE, 0x00FF  /* F8, FF */
};

/* Definition of the interval type */
struct pdfdocenc_map_interval_s {
  pdf_u8_t interval_start;
  pdf_u8_t interval_stop;
};
typedef struct pdfdocenc_map_interval_s pdfdocenc_map_interval_t;

/* Direct intervals */
#define PDFDOCENC_MDI  10
static const pdfdocenc_map_interval_t pdfdocenc_map_direct[PDFDOCENC_MDI] = {
  { 0x09, 0x0A },
  { 0x0D, 0x0D },
  { 0x20, 0x7E },
  { 0xA1, 0xAC },
  { 0xAE, 0xFF }
};

/* Indirect intervals */
#define PDFDOCENC_MII  10
static const pdfdocenc_map_interval_t pdfdocenc_map_indirect[PDFDOCENC_MII] = {
  { 0x18, 0x1F },
  { 0x80, 0x9E },
  { 0xA0, 0xA0 }
};

/* Undefined intervals, probably not really needed
#define PDFDOCENC_MUI  10
static const pdfdocenc_map_interval_t pdfdocenc_map_undefined[PDFDOCENC_MUI] = {
  { 0x00, 0x08 },
  { 0x0B, 0x0C },
  { 0x0E, 0x17 },
  { 0x7F, 0x7F },
  { 0x9F, 0x9F },
  { 0xAD, 0xAD }
};
  */


/* Mapping of the first char in a UTF-8 character representation, which
 *  determines the number of bytes that will be needed to represent the
 *  character:
 *     0xxx xxxx -> 1 byte  [00,7F]
 *     110x xxxx -> 2 bytes [C0,DF]
 *     1110 xxxx -> 3 bytes [E0,EF]
 *     1111 0xxx -> 4 bytes [F0,F7]
 *  Longer byte sequences are not allowed to represent Unicode points.
 */
static const unsigned char n_bytes_in_utf8_char [256] = {
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /* 00, 1F */
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /* 20, 3F */
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /* 40, 5F */
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /* 60, 7F */
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 80, 9F */
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* A0, BF */
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, /* C0, DF */
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0  /* E0, FF */
};


/* UNICODE BOM bytes encoded in the different built-in UNICODE encodings */
static const pdf_text_bom_t unicode_bom [PDF_TEXT_MAX_UNICODE_ENC] = {
  { {239,187,191,  0}, 3 },    /* PDF_TEXT_UTF8 */
  { {254,255,  0,  0}, 2 },    /* PDF_TEXT_UTF16_BE */
  { {255,254,  0,  0}, 2 },    /* PDF_TEXT_UTF16_LE */
  { {  0,  0,  0,  0}, 0 },
  { {  0,  0,254,255}, 4 },    /* PDF_TEXT_UTF32_BE */
  { {255,254,  0,  0}, 4 },    /* PDF_TEXT_UTF32_LE */
  { {  0,  0,  0,  0}, 0 }
};


/******************** PDF Doc Encoding to UTF-32 conversion *******************/

static pdf_text_utf32_char_t
pdf_text_pdfdocenc_point_to_utf32he_point(const pdf_char_t pdfdocenc_val)
{
  extern const pdf_u32_t pdfdocenc_map [PDFDOCENC_MAX];
  pdf_text_utf32_char_t utf32val;
  utf32val.i =  pdfdocenc_map[(int)pdfdocenc_val];
  return utf32val;
}

/* Static function to convert from PDF Doc Encoding to UTF-32HE, lossless */
pdf_status_t
pdf_text_pdfdocenc_to_utf32he(const pdf_char_t    *input_data,
                              const pdf_size_t    input_length,
                              pdf_char_t          **p_output_data,
                              pdf_size_t          *p_output_length)
{
  /* Note: PDF Doc Encoding has always 8 bits per character.
   *  This means that, if length of origin string is N bytes, the number of
   *  required bytes for the UTF32 representation of the string is 4N.
   *  (Each PDFDocEncoding byte is expanded to 4 bytes in UTF32. */
  
  pdf_size_t i;  /* index for the origin string data */
  pdf_size_t j;  /* index for the destination string data */
  pdf_char_t *data;
  pdf_size_t new_length;
  
  /* Get new string length... */
  new_length = 4 * input_length;
  /* Create destination string with correct size (but empty!) */
  data = (pdf_char_t *)pdf_alloc(new_length);
  if(data == NULL)
    {
      return PDF_ENOMEM;
    }
  
  for(i = 0, j = 0; i < input_length; i++, j+=4)
    {
      pdf_text_utf32_char_t utf32val;
      /* Get value... */
      utf32val = pdf_text_pdfdocenc_point_to_utf32he_point(input_data[i]);
      if(utf32val.i == 0)
        {
          /* Oops, the given input byte is UNDEFINED in PDF Doc Encoding */
          pdf_dealloc(data);
          return PDF_EINVAL;
        }
      /* Copy converted value to output */
      memcpy(&(data[j]), &(utf32val), 4);
    }
  
  /* Everything went ok, set output data */
  *p_output_data = data;
  *p_output_length = new_length;
  
  return PDF_OK;
}


/******************** UTF-32 to PDF Doc Encoding conversion *******************/

static pdf_char_t
pdf_text_utf32he_point_to_pdfdocenc_point(const pdf_text_utf32_char_t utf32val)
{
  
  pdf_u8_t i;

  
  /* If the given UTF-32 point is encoded in a single byte, then direct
   *  conversion is possible */
  if(utf32val.i <= 0xFF)
    {
      extern const pdfdocenc_map_interval_t pdfdocenc_map_direct[PDFDOCENC_MDI];
      /* Check if direct conversion is possible */
      for(i=0; i<PDFDOCENC_MDI; ++i)
        {
          if((utf32val.i <= pdfdocenc_map_direct[i].interval_stop) && \
             (utf32val.i >= pdfdocenc_map_direct[i].interval_start))
            {
              /* If the unicode char is among this intervals, direct conversion
               * is possible (single byte!) */
              return (pdf_char_t)utf32val.i;
            }
        }
    }
  else
    {
      extern const pdf_u32_t pdfdocenc_map [PDFDOCENC_MAX];
      extern const pdfdocenc_map_interval_t \
                    pdfdocenc_map_indirect[PDFDOCENC_MII];
      /* Check if indirect conversion is possible */
      for(i=0; i<PDFDOCENC_MII; ++i)
        {
          
          /* Simple search in the interval */
          pdf_size_t search_index = pdfdocenc_map_indirect[i].interval_start;
          while((search_index <= pdfdocenc_map_indirect[i].interval_stop) )
            {
              if(pdfdocenc_map[search_index] == utf32val.i)
                {
                  /* Directly apply search index as character */
                  return (pdf_char_t) search_index;
                }
              search_index++;
            }
        }
    }

  /* If neither Direct conversion nor Indirect conversion are available,
   *  the given character is UNDEFINED. Set default character when there is no 
   *  direct mapping to PDF Doc Encoding. This means that every conversion
   *  to PDF Doc Encoding will NEVER fail if there is no mapping of a code point
   *  in PDF Doc Encoding */
  return (pdf_char_t) '?';
}


/* Static function to convert from UTF-32HE to PDF Doc Encoding, with loss of
 information */
pdf_status_t
pdf_text_utf32he_to_pdfdocenc(const pdf_char_t    *input_data,
                              const pdf_size_t    input_length,
                              pdf_char_t          **p_output_data,
                              pdf_size_t          *p_output_length)
{
  /* Note: UTF-32 has always 32 bits per character.
   This means that, if length of origin string is 4N bytes, the number of
   required bytes for the PDFDocEncoding representation of the string is N.
   (Each UNICODE 4-byte character is represented as 1-byte character in
   PDFDocEncoding). This means that LOSS of information could happen */
  
  int i;  /* index for the origin string data */
  int j;  /* index for the destination string data */
  
  /* Check if the length of the origin string is multiple of 4 bytes */
  if(input_length % 4 != 0)
    {
      PDF_DEBUG_BASE("Input length must be multiple of 4! Invalid UTF-32 data."
                     "(Length: %d)", (int)input_length);
      return PDF_EINVAL;
    }
  
  /* Get new string length... */
  *p_output_length = input_length / 4;
  
  /* Create destination string with correct size (but empty!) */
  *p_output_data = (pdf_char_t *)pdf_alloc(*p_output_length);
  if(*p_output_data == NULL)
    {
      return PDF_ENOMEM;
    }
  
  for(i = 0, j = 0; i < input_length; i+=4, j++)
    {
      pdf_text_utf32_char_t   utf32val; /* UNICODE char */
      /* Get UTF-32 char to convert */
      memcpy(&utf32val, &input_data[i], 4);
      /* Convert character to PDF Doc Encoding */
      (*p_output_data)[j] = pdf_text_utf32he_point_to_pdfdocenc_point(utf32val);
    }
  
  return PDF_OK;
}


/*********************** UTF-32 to UTF-32 conversions *************************/

/* Function to convert from UTF32-HE to UTF32-HE, lossless */
pdf_status_t
pdf_text_utf32he_to_utf32he(const pdf_char_t    *input_data,
                            const pdf_size_t    input_length,
                            const pdf_bool_t    swap,
                            const pdf_bool_t    check_input_he,
                            const pdf_bool_t    check_output_he,
                            pdf_char_t          **p_output_data,
                            pdf_size_t          *p_output_length)
{
  pdf_size_t walker;
  pdf_size_t bom_bytes = 0;
  pdf_char_t *new_data = NULL;
  pdf_size_t new_size = 0;

  if(input_length % 4 != 0)
    {
      /* Invalid number of bytes! */
      PDF_DEBUG_BASE("Input length must be multiple of 4! Invalid UTF-32 data."
                     " (Length: %d)", (int)input_length);
      return PDF_EINVAL;
    }
  
  /* Check if BOM is present... and skip it if so */
  if(pdf_text_check_unicode_bom (input_data, input_length,
                                 PDF_TEXT_UTF32_HE, swap))
    {
      /* Skip BOM */
      bom_bytes = 4;
    }
  
  /* Allocate memory */
  new_size = input_length - bom_bytes;
  /* Create destination string with correct size (but empty!) */
  new_data = (pdf_char_t *)pdf_alloc(new_size);
  if(new_data == NULL)
    {
      return PDF_ENOMEM;
    }

  /* Change endianness of each 32bit value... */
  for(walker = bom_bytes; walker < input_length; walker+=4)
    {
      pdf_text_utf32_char_t utf32val;
      memcpy(&utf32val, &input_data[walker], 4);
      
      /* Check code point validity (if the input is Host Endian) */
      /* Code point must not be a surrogate code unit, and must be in the
       * U+00000000 - U+0010FFFF range */
      if(check_input_he)
        {
          if((utf32val.i > 0x10FFFF) || \
              ((utf32val.i >= 0xD800) && \
               (utf32val.i <= 0xDFFF)))
            {
              /* Invalid UTF-32 code point received */
              PDF_DEBUG_BASE("Invalid input UTF-32HE code point: "
                             "%.2X:%.2X:%.2X:%.2X",
                             utf32val.c[0],
                             utf32val.c[1],
                             utf32val.c[2],
                             utf32val.c[3]);
              return PDF_EBADDATA;
            }
        }
      
      /* Swap bytes */
      if(swap)
        {
          utf32val.i = PDF_TEXT_CHANGE_ENDIANNESS_32BIT(utf32val.i);
        }
      
      /* Check code point validity (if the output is Host Endian) */
      /* Code point must not be a surrogate code unit, and must be in the
       * U+00000000 - U+0010FFFF range */
      if(check_output_he)
        {
          if((utf32val.i > 0x10FFFF) || \
             ((utf32val.i >= 0xD800) && \
              (utf32val.i <= 0xDFFF)))
            {
              /* Invalid UTF-32 code point received */
              PDF_DEBUG_BASE("Invalid output UTF-32HE code point: "
                             "%.2X:%.2X:%.2X:%.2X",
                             utf32val.c[0],
                             utf32val.c[1],
                             utf32val.c[2],
                             utf32val.c[3]);
              return PDF_EBADDATA;
            }
        }

      /* Copy value */
      memcpy(&(new_data[walker-bom_bytes]), &utf32val, 4);
    }
  
  /* Really set output data */
  *p_output_data = new_data;
  *p_output_length = new_size;
  
  return PDF_OK;
}


/*********************** UTF-16 to UTF-32 conversions *************************/

/* Static function to convert a given UTF-16HE character (with one or two words)
 *  to UTF-32HE. The number of bytes (2 or 4) used from the input UTF-16BE point
 *  is returned (or 0 if the UTF-16HE point is not valid */
static pdf_size_t
pdf_text_utf16he_point_to_utf32he_point(pdf_text_utf16_char_t utf16val[2],
                                        pdf_text_utf32_char_t *p_utf32val)
{
  pdf_size_t n_bytes;
  
  /* Ok, so how can we know if the UTF16 character is encoded using 2 or
   * 4 bytes? A surrogate pair consists on two 16-bit values of the 
   * UTF16 encoding. Each word (16bit-value) within the surrogate pair 
   * doesn't represent a valid character, as it is enclosed in the 
   * following interval: U+D800 - U+DFFF. This means that if the first 
   * word analysed is outside this interval, it will be treated 
   * separately. If the first word is within this interval, it is 
   * expected to have the second word within the interval as well. If 
   * this doesn't happen it will be treated as a badly formatted UTF16
   * string. In fact, there are two different intervals within the surrogate
   * points themselves: the High surrogate point will be in the U+D800 - 
   * U+DBFF interval, and the Low surrogate point will be in the U+DC00 - 
   * U+DFFF interval. */
  
  if(((utf16val[0].i) >= 0xD800) && \
     ((utf16val[0].i) <= 0xDFFF))
    {
      /* To have a valid surrogate pair, the first UTF-16 value must be the High
       *  surrogate code unit, and the second UTF-16 value must be the Low
       *  surrogate code unit. */
      if(((utf16val[0].i) <= 0xDFFF) && \
         ((utf16val[1].i) >= 0xDC00) && \
         ((utf16val[1].i) <= 0xDFFF))
        {
          /* Yes, second word is within the validity interval, it seems a
           * correct 32-bit representation of a character in UTF16BE */
          n_bytes = 4;
          (*p_utf32val).i = 0x10000 + \
                            (((utf16val[0].i) - 0xD800) << 10) + \
                            ((utf16val[1].i) - 0xDC00);
        }
      /* else Oops, invalid UTF-16HE surrogate pair! Input data is not well
       * formed... */
      else
        {
          PDF_DEBUG_BASE("Invalid UTF-16HE point! %.2X:%.2X:%.2X:%.2X",
                         utf16val[0].c[0], utf16val[0].c[1],
                         utf16val[1].c[0], utf16val[1].c[1]);
          n_bytes = 0;
        }
    }
  else
    {
      /* No multiword representation, just 16bits for this character
       * So conversion is direct... */
      n_bytes = 2;
      (*p_utf32val).i = (utf16val[0]).i;
    }
  return n_bytes;
}


/* Function to convert from UTF16-HE to UTF32-HE, lossless */
pdf_status_t
pdf_text_utf16he_to_utf32he(const pdf_char_t    *input_data,
                            const pdf_size_t    input_length,
                            const pdf_bool_t    swap,
                            pdf_char_t          **p_output_data,
                            pdf_size_t          *p_output_length,
                            pdf_char_t          **p_remaining_data,
                            pdf_size_t          *p_remaining_length)
{
  /* Note: UTF-16 has either 16 or 32 bits per character.
   *  This means that, if length of origin string is N bytes, the number of
   *  required bytes for the UTF-32 representation of the string is 2N in
   *  the worst case (in the case of having all the UTF-16 characters encoded
   *  with 16bits).
   *  (Each UTF-16 is expanded to 4 bytes in UTF-32. */
  
  pdf_char_t *data;
  pdf_size_t new_string_length;
  pdf_size_t new_string_length_worst;
  pdf_size_t delta_in_utf16be;
  int i;  /* index for the origin string data */
  int j;  /* index for the destination string data */
  pdf_text_utf16_char_t utf16val[2];
  pdf_text_utf32_char_t utf32val;
  short stop_conversion = PDF_FALSE;
  short check_lang_code = PDF_FALSE;
  int bom_bytes = 0;
  
  /* Check if length is multiple of 2 (data must come in pairs of bytes!) */
  if((input_length < 2) || \
     (input_length % 2) != 0)
    {
      PDF_DEBUG_BASE("Input length must be multiple of 2 and greater than 2!"
                     " Invalid UTF-16 data. (Length: %d)", (int)input_length);
      return PDF_EINVAL;
    }
  
  /* Check if BOM is present... and skip it if so */
  if(pdf_text_check_unicode_bom (input_data, input_length,
                                 PDF_TEXT_UTF16_HE, swap))
    {
      /* Skip BOM */
      bom_bytes = 2;
    }
  
  /* Get new string worst length... (don't consider BOM bytes) */
  new_string_length_worst = 2 * (input_length - bom_bytes);
  /* Create destination string with worst size (but empty!) */
  data = (pdf_char_t *)pdf_alloc(new_string_length_worst);
  if(data == NULL)
    {
      return PDF_ENOMEM;
    }
  
  /* Initiate final string length */
  new_string_length = 0;
  
  /* Initiate indexes */
  i = bom_bytes;  /* Skipping BOM if present... */
  j = 0;
  
  /* Check if specific country/language could be found */
  if((p_remaining_length != NULL) && \
     (p_remaining_data != NULL))
    {
      check_lang_code = PDF_TRUE;
    }
  
  /* This while loop will be done until the end of the input data OR until
   *  the moment a new country/language code identifier is found. But, this
   *  extra stop condition will only be available if valid `p_remaining_data'
   *  and `p_remaining_length' pointers are given as input. */
  while((i < input_length) && \
        (!stop_conversion))
    {
      if((check_lang_code) && \
         (input_data[i+1] == PDF_TEXT_LCI_1) && \
         (input_data[i] == PDF_TEXT_LCI_0))
        {
          /* Stop conversion... due to new lang/code initializer */
          stop_conversion = PDF_TRUE;
          /* Set the output remaining data... */
          *p_remaining_length = input_length - i;
          *p_remaining_data = (pdf_char_t *) &input_data[i];
        }
      else
        {
          /* Store the UTF-16(BE/LE) data in the intermediate variable */
          utf16val[0].c[0] = input_data[i];
          utf16val[0].c[1] = input_data[i+1];
          if((i+3) < input_length)
            {
              utf16val[1].c[0] = input_data[i+2];
              utf16val[1].c[1] = input_data[i+3];
            }
          /* else, last point should be only 1-word length */
          else
            {
              utf16val[1].c[0] = 0x00;
              utf16val[1].c[1] = 0x00;
            }
          
          if(swap)
            {
              /* Input data must be swapped in order to convert it to
               *  host endian */
              utf16val[0].i = PDF_TEXT_CHANGE_ENDIANNESS_16BIT(utf16val[0].i);
              utf16val[1].i = PDF_TEXT_CHANGE_ENDIANNESS_16BIT(utf16val[1].i);
            }
          
          /* Change UTF-16HE point to UTF-32HE point */
          delta_in_utf16be = pdf_text_utf16he_point_to_utf32he_point(utf16val,
                                                                     &utf32val);
          if(delta_in_utf16be == 0)
            {
              /* Oops, invalid UTF-16HE point found! */
              pdf_dealloc(data);
              PDF_DEBUG_BASE("Conversion from UTF-16 to UTF-32HE stopped");
              return PDF_EBADDATA;
            }
          
          /* Finally, store the UTF-32 representation of the char in the output
           * string... */
          data[j] = utf32val.c[0];
          data[j+1] = utf32val.c[1];
          data[j+2] = utf32val.c[2];
          data[j+3] = utf32val.c[3];
          
          /* Update final string length after having added this character */
          new_string_length+=4;
          
          /* Update indexes */
          i += delta_in_utf16be;
          j += 4;
        }
    }
  
  /* Everything went ok, set output data */
  *p_output_data = data;
  /* Set output length... */
  *p_output_length = new_string_length;
  
  /* Check if the stop flag was set due to finding lang/country code
   *  initializer. If not found, set zero remaining length and NULL
   *  remaining str */
  if((!stop_conversion) && \
     (p_remaining_length != NULL) && \
     (p_remaining_data != NULL))
    {
      *p_remaining_length = 0;
      *p_remaining_data = NULL;
    }
  
  /* Now, if the real output string length is not equal to the worst string
   * length, we will reallocate memory for the correct size. This will only
   * happen when at least one character is not encoded with 32bits in UTF-16. */
  if(new_string_length != new_string_length_worst)
    {
      /* Recreate object with correct size... */
      *p_output_data = (pdf_char_t *)pdf_realloc(*p_output_data,
                                                 new_string_length);
      if(*p_output_data == NULL)
        {
          return PDF_ENOMEM;
        }
    }  
  return PDF_OK;
}



/*********************** UTF-32 to UTF-16 conversions *************************/


/* Static function to convert a given UTF-32HE character to UTF-16HE. The number
 * of bytes used in the output UTF-16HE point is returned (or 0
 *  if the UTF-16HE point is not valid */
static pdf_size_t
pdf_text_utf32he_point_to_utf16he_point(pdf_text_utf32_char_t utf32val,
                                        pdf_text_utf16_char_t utf16val[2])
{
  pdf_size_t n_bytes;

  if((utf32val.i >= 0xD800) && \
     (utf32val.i <= 0xDFFF))
    {
      PDF_DEBUG_BASE("Invalid UTF-32HE point (surrogate pair found)! "
                     "%.2X:%.2X:%.2X:%.2X",
                     utf32val.c[0], utf32val.c[1],
                     utf32val.c[2], utf32val.c[3]);
      n_bytes = 0;
    }
  /* Check if multiword (32bits) representation is needed */
  else if( utf32val.i >= 0x10000 )
    {
      /* Ok so it seems a multiword representation...
       * Now check input UTF-32HE representation to see if it really is a
       *  Unicode point (from 0x00000 to 0x10FFFF) */
      if (utf32val.i <= 0x10FFFF)
        {
          /* 32 bits are required for this char */
          n_bytes = 4;
          utf32val.i -= 0x10000;
          /* Process higher 10 bits, by shifting to the right 10 bits */
          (utf16val[0]).i = (utf32val.i >> 10) + 0xD800;
          /* Process lower 10 bits, by masking the value with 0x03FF */  
          (utf16val[1]).i = (utf32val.i & 0x03FF) + 0xDC00;
        }
      else
        {
          /* else Oops, invalid 32-bit character! Input data is not well
           * formed... */
          PDF_DEBUG_BASE("Invalid UTF-32HE point! %.2X:%.2X:%.2X:%.2X",
                         utf32val.c[0], utf32val.c[1],
                         utf32val.c[2], utf32val.c[3]);
          n_bytes = 0;
        }
    }
  else
    {
      n_bytes = 2;
      /* No multiword representation, just 16bits for this character
       * So conversion is direct.... */
      (utf16val[0]).i = utf32val.i;
      (utf16val[1]).i = 0x0000;
    }
  return n_bytes;
}


/*  Function to convert from UTF-32HE to UTF-16, lossless */
pdf_status_t
pdf_text_utf32he_to_utf16he(const pdf_char_t *input_data,
                            const pdf_size_t input_length,
                            pdf_char_t       **p_output_data,
                            pdf_size_t       *p_output_length,
                            pdf_bool_t       swap)
{
  /* Note: UTF-16BE has either 16 or 32 bits per character.
   This means that, if length of origin string is 4N bytes, the number of
   required bytes for the UTF16BE representation of the string is 4N in
   the worst case. (When all the UTF16be representations have 32bits)
   */
  pdf_size_t new_string_length;
  pdf_size_t new_string_length_worst;
  pdf_size_t delta_in_utf16be;
  int i;  /* index for the origin string data */
  int j;  /* index for the destination string data */
  pdf_text_utf16_char_t utf16val[2];
  pdf_text_utf32_char_t utf32val;
  pdf_char_t *data;
  
  /* Get new string length (worst case)... */
  new_string_length_worst = input_length;
  /* Create destination string with correct size (but empty!) */
  data = (pdf_char_t *)pdf_alloc(new_string_length_worst);
  if(data == NULL)
    {
      return PDF_ENOMEM;
    }
  
  /* Initiate real string length, without considering marker bytes */
  new_string_length = 0;
  
  for( i = 0, j = 0; i < input_length; i += 4, j += delta_in_utf16be )
    {
      /* Get UCS4 char, as a direct memory copy from the input array */
      memcpy(&utf32val, &(input_data[i]), 4);
      
      delta_in_utf16be = pdf_text_utf32he_point_to_utf16he_point(utf32val,
                                                                 utf16val);

      if(delta_in_utf16be == 0)
        {
          /* Oops, invalid UTF-16HE point found! */
          pdf_dealloc(data);
          PDF_DEBUG_BASE("Conversion from UTF-32HE to UTF-16 stopped");
          return PDF_EBADDATA;
        }
      
      /* Change endianness of each output word if required */
      if(swap)
        {
          /* Change to BE */
          (utf16val[0]).i = PDF_TEXT_CHANGE_ENDIANNESS_16BIT((utf16val[0]).i);
          (utf16val[1]).i = PDF_TEXT_CHANGE_ENDIANNESS_16BIT((utf16val[1]).i);
        }
      
      /* Finally, store the UTF16BE representation of the char in the output
       * string... */
      memcpy(&(data[j]), &utf16val[0], delta_in_utf16be);
      /* Update new string legth... */
      new_string_length += delta_in_utf16be;
    }
  
  /* If everything went ok, set output data */
  *p_output_data = data;
  /* Set final output length of the generated string */
  *p_output_length = new_string_length;
  
  /* If the real required string length is not equal to the initial worst length
   * then update string with correct length. */
  if(new_string_length != new_string_length_worst)
    {
      /* Recreate object with correct smaller size... */
      *p_output_data = (pdf_char_t *)pdf_realloc(*p_output_data,
                                                 new_string_length);
      if(*p_output_data == NULL)
        {
          return PDF_ENOMEM;
        }
    }
  
  return PDF_OK;
}


/************************ UTF-8 to UTF-32 conversions *************************/

/* Static function to convert a given UTF-8 character to UTF-32HE. The number
 * of bytes used in the input UTF-8 point is returned (or 0 if the UTF-8 point
 * is not valid */
static pdf_size_t
pdf_text_utf8_point_to_utf32he_point(const pdf_text_utf8_char_t utf8val[4],
                                     const pdf_size_t n_bytes,
                                     pdf_text_utf32_char_t *p_utf32val)
{
  int c;  /* index for the utf-8 representation of every char */

  /* Check validity of the UTF-8 bytes:
   *  - First byte can be neither 0xFF nor 0xFE
   *  - The following bytes must be in the [80-BF] range! (10xxxxxx) */
  for(c=0; c<n_bytes; c++)
    {
      if(((c == 0) && ((utf8val[0] == 0xFF) || (utf8val[0] == 0xFE))) || \
         ((c != 0) && ((utf8val[c]  < 0x80) || (utf8val[c]  > 0xBF))))
        {
          PDF_DEBUG_BASE("Invalid UTF-8 character: %.2X:%.2X:%.2X:%.2X",
                         (int)utf8val[0],
                         ((n_bytes>1)?((int)utf8val[1]):0),
                         ((n_bytes>2)?((int)utf8val[2]):0),
                         ((n_bytes>3)?((int)utf8val[3]):0));
          return 0;
        }
    }
  
  /* Load all the bytes of the UTF-8 representation in the UTF-32HE var */
  switch(n_bytes)
  {
    case 1:
      (*p_utf32val).i = (utf8val[0] & 0x7F);            /* 0111 1111 */
      break;
    case 2:
      (*p_utf32val).i = ((utf8val[0] & 0x1F) << 6) +    /* 0001 1111 */
                        (utf8val[1] & 0x3F);          /* 0011 1111 */
      break;
    case 3:
      (*p_utf32val).i = ((utf8val[0] & 0x0F) << 12) +   /* 0000 1111 */
                        ((utf8val[1] & 0x3F) << 6) +  /* 0011 1111 */
                        (utf8val[2] & 0x3F);          /* 0011 1111 */
      break;
    case 4:
      (*p_utf32val).i = ((utf8val[0] & 0x07) << 18) +   /* 0000 1111 */
                        ((utf8val[1] & 0x3F) << 12) +  /* 0000 1111 */
                        ((utf8val[2] & 0x3F) << 6) +   /* 0011 1111 */
                        (utf8val[3] & 0x3F);           /* 0011 1111 */
      break;
    default:
      /* Should never happen! */
      return 0;
  }
  
  return n_bytes;
}

/* Function to convert from UTF-8 to UTF-32HE, lossless */
pdf_status_t
pdf_text_utf8_to_utf32he(const pdf_char_t    *input_data,
                         const pdf_size_t    input_length,
                         pdf_char_t          **p_output_data,
                         pdf_size_t          *p_output_length)
{
  /* Note: PDF Doc Encoding has always 8 bits per character.
   *  This means that, if length of origin string is N bytes, the number of
   *  required bytes for the UTF32 representation of the string is 4N.
   *  (Each PDFDocEncoding byte is expanded to 4 bytes in UTF32. */
  
  extern const unsigned char n_bytes_in_utf8_char [256];
  pdf_size_t new_string_length;
  pdf_size_t new_string_length_worst;
  pdf_size_t bom_bytes;
  int i;  /* index for the origin string data */
  int j;  /* index for the destination string data */
  pdf_size_t delta_in_utf8;

  pdf_char_t *data;
  
  /* Check if BOM is present... and skip it if so */
  bom_bytes = 0;
  if(pdf_text_check_unicode_bom (input_data, input_length, PDF_TEXT_UTF8, 0))
    {
      /* Skip BOM in UTF-8 */
      bom_bytes = 3;
    }
  
  /* Get new string length... */
  new_string_length_worst = 4 * (input_length - bom_bytes);
  
  /* Create destination string with worst size (but empty!) */
  data = (pdf_char_t *)pdf_alloc(new_string_length_worst);
  if(data == NULL)
    {
      return PDF_ENOMEM;
    }
  
  new_string_length = 0;
  for(i = bom_bytes, j = 0; i < input_length; i+=delta_in_utf8, j+=4)
    {
      pdf_text_utf32_char_t utf32val;
      pdf_text_utf8_char_t utf8val[4];

      /* Check number of bytes needed for the UTF-8 char */
      delta_in_utf8 = n_bytes_in_utf8_char[(int)input_data[i]];
      
      /* Check validity of first byte in UTF-8 */
      /* Check if the required bytes are outside the input data stream */
      if((delta_in_utf8 == 0) || \
         ((input_length - i) < delta_in_utf8))
        {
          PDF_DEBUG_BASE("Wrong UTF-8 data received (UTF-8 length: %d, "
                         "Remaining length: %d", delta_in_utf8,
                         (input_length - i));
          pdf_dealloc(data);
          return PDF_EINVAL;
        }
      
      /* Store data in intermediate UTF-8 variable */
      memcpy(&utf8val[0], &input_data[i], delta_in_utf8);
      
      if(pdf_text_utf8_point_to_utf32he_point(utf8val,
                                              delta_in_utf8,
                                              &utf32val) == 0)
        {
          PDF_DEBUG_BASE("Problem decoding UTF-8 string");
          pdf_dealloc(data);
          return PDF_EINVAL;
        }

      /* Copy converted value (in UTF-32HE) to output */
      memcpy(&(data[j]), &(utf32val), 4);
      
      /* Update new string length */
      new_string_length += 4;
    }
  
  /* If everything went ok, set output data */
  *p_output_data = data;
  /* Set final output length of the generated string */
  *p_output_length = new_string_length;
  
  /* If the real required string length is not equal to the initial worst length
   * then update string with correct length. */
  if(new_string_length != new_string_length_worst)
    {
      /* Recreate object with correct smaller size... */
      *p_output_data = (pdf_char_t *)pdf_realloc(*p_output_data,
                                                 new_string_length);
      if(*p_output_data == NULL)
        {
          return PDF_ENOMEM;
        }
    }
  
  return PDF_OK;
}


/************************ UTF-32 to UTF-8 conversions *************************/

/* Static function to convert a given UTF-32HE character to UTF-8. The number
 * of bytes used in the output UTF-8 point is returned (or 0 if the UTF-8 point
 * is not valid */
static pdf_size_t
pdf_text_utf32he_point_to_utf8_point(const pdf_text_utf32_char_t utf32val,
                                     pdf_text_utf8_char_t utf8val[4])
{
  pdf_size_t n_bytes;

  if(utf32val.i < 0x80)
    {
      /* Output is 1 byte */
      n_bytes = 1;
      utf8val[0] = (pdf_text_utf8_char_t) utf32val.i;
    }
  else if(utf32val.i < 0x800)
    {
      /* Output is 2 bytes */
      n_bytes = 2;
      /* Get first byte, using upper 5 bits --> 110xxxxx */
      utf8val[0] = ((pdf_text_utf8_char_t) (utf32val.i >> 6)) | 0xC0;
      /* Get second byte, using lower 6 bits --> 10xxxxxx */
      utf8val[1] = ((pdf_text_utf8_char_t) (utf32val.i & 0x3F)) | 0x80;
    }
  else if(utf32val.i < 0x10000)
    {
      /* Output is 3 bytes */
      n_bytes = 3;
      /* Get first byte, using upper 4 bits --> 1110xxxx */
      utf8val[0] = ((pdf_text_utf8_char_t)(utf32val.i >> 12)) | 0xE0;
      /* Get second byte, using middle 6 bits --> 10xxxxxx */
      utf8val[1] = ((pdf_text_utf8_char_t)((utf32val.i >> 6) & 0x3F)) | 0x80;
      /* Get third byte, using lower 6 bits --> 10xxxxxx */
      utf8val[2] = ((pdf_text_utf8_char_t)(utf32val.i & 0x3F)) | 0x80;
    }
  else if(utf32val.i < 0x0010FFFF)
    {
      /* Output is 4 bytes */
      n_bytes = 4;
      /* Get first byte, using upper 3 bits --> 11110xxx */
      utf8val[0] = ((pdf_text_utf8_char_t)(utf32val.i >> 18)) | 0xF0;
      /* Get second byte, using upper-middle 6 bits --> 10xxxxxx */
      utf8val[1] = (((pdf_text_utf8_char_t)(utf32val.i >> 12)) & 0x3F) | 0x80;
      /* Get second byte, using lower-middle 6 bits --> 10xxxxxx */
      utf8val[2] = (((pdf_text_utf8_char_t)(utf32val.i >> 6)) & 0x3F) | 0x80;
      /* Get third byte, using lower 6 bits --> 10xxxxxx */
      utf8val[3] = ((pdf_text_utf8_char_t)(utf32val.i & 0x3F)) | 0x80;
    }
  else
    {
      /* Invalid input UTF-32 val */
      PDF_DEBUG_BASE("Wrong UTF-32BE value! '0x%.2X 0x%.2X 0x%.2X 0x%.2X'",
                     utf32val.c[0],utf32val.c[1],utf32val.c[2],utf32val.c[3]);
      n_bytes = 0;
    }
  return n_bytes;
}

/* Function to convert from UTF-32HE to UTF-8, lossless */
pdf_status_t
pdf_text_utf32he_to_utf8(const pdf_char_t      *input_data,
                         const pdf_size_t      input_length,
                         pdf_char_t            **p_output_data,
                         pdf_size_t            *p_output_length)
{
  /* Note: UTF-8 has either 8, 16, 24 or 32 bits per character.
   This means that, if length of origin string is 4N bytes, the number of
   required bytes for the UTF-8 representation of the string is 4N in
   the worst case. (When all the UTF-8 representations have 32bits)
   */
  pdf_size_t new_string_length;
  pdf_size_t new_string_length_worst;
  int i;  /* index for the origin string data */
  int j;  /* index for the destination string data */
  pdf_char_t *data;
  pdf_size_t delta_in_utf8;
  
  /* Get new string length (worst case)... */
  new_string_length_worst = input_length;
  /* Create destination string with correct size (but empty!) */
  data = (pdf_char_t *)pdf_alloc(new_string_length_worst);
  if(data == NULL)
    {
      return PDF_ENOMEM;
    }
  
  /* Initiate real string length, without considering marker bytes */
  new_string_length = 0;
  
  for( i = 0, j = 0; i < input_length; i += 4, j += delta_in_utf8 )
    {
      pdf_text_utf32_char_t utf32val;
      pdf_text_utf8_char_t utf8val[4];

      /* Get UTF-32 char, as a direct memory copy from the input array */
      memcpy(&utf32val, &(input_data[i]), 4);
      
      delta_in_utf8 = pdf_text_utf32he_point_to_utf8_point(utf32val,utf8val);
      if(delta_in_utf8 == 0)
        {
          PDF_DEBUG_BASE("Problem encoding UTF-8 string");
          pdf_dealloc(data);
          return PDF_EBADDATA;
        }
      
      /* Store UTF-8 val in output array */
      memcpy(&data[j], &(utf8val[0]), delta_in_utf8);
      
      /* Update new_string_length, depending on the bytes used to represent
       *  this character in UTF-8 */
      new_string_length += delta_in_utf8;
    }
  
  /* If everything went ok, set output data */
  *p_output_data = data;
  /* Set final output length of the generated string */
  *p_output_length = new_string_length;
  
  /* If the real required string length is not equal to the initial worst length
   * then update string with correct length. */
  if(new_string_length != new_string_length_worst)
    {
      /* Recreate object with correct smaller size... */
      *p_output_data = (pdf_char_t *)pdf_realloc(*p_output_data,
                                                 new_string_length);
      if(*p_output_data == NULL)
        {
          return PDF_ENOMEM;
        }
    }
  
  return PDF_OK;
}


/*************************** BOM-related functions ****************************/


inline pdf_text_bom_t
pdf_text_get_unicode_bom(enum pdf_text_unicode_encoding_e unicode_encoding)
{
  extern const pdf_text_bom_t unicode_bom [PDF_TEXT_MAX_UNICODE_ENC];
  return unicode_bom[unicode_encoding];
}


pdf_bool_t
pdf_text_check_unicode_bom (const pdf_char_t *data,
                            const pdf_size_t size,
                            enum pdf_text_unicode_encoding_e enc,
                            int swap)
{
  extern const pdf_text_bom_t unicode_bom [PDF_TEXT_MAX_UNICODE_ENC];

  switch(enc)
  {
    case PDF_TEXT_UTF16_HE:
    case PDF_TEXT_UTF32_HE:
    {
      enc += ((PDF_IS_BIG_ENDIAN ^ swap) ? PDF_TEXT_HE_TO_BE:PDF_TEXT_HE_TO_LE);
    }
    case PDF_TEXT_UTF8:
    case PDF_TEXT_UTF16_BE:
    case PDF_TEXT_UTF16_LE:
    case PDF_TEXT_UTF32_BE:
    case PDF_TEXT_UTF32_LE:
    {
      if((size >= unicode_bom[enc].bom_bytes) && \
         (memcmp(data,unicode_bom[enc].bom_data,unicode_bom[enc].bom_bytes)==0))
        {
          return PDF_TRUE;
        }
    }
    default:
      return PDF_FALSE;
  }
}

/* End of pdf-text-encoding.c */


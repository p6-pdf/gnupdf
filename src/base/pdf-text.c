/* -*- mode: C -*- Time-stamp: ""
 *
 *       File:         pdf-text.c
 *       Date:         Fri Jan 11 21:09:56 2008
 *
 *       GNU PDF Library - Encoded Text handling utilities
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

#include <config.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#ifdef HAVE_MALLOC_H
  #include <malloc.h>
#endif /* HAVE_MALLOC_H */
#include <xalloc.h>
#include <math.h>

#include <pdf-text.h>
#include <pdf-text-encoding.h>
#include <pdf-text-host-encoding.h>
#include <pdf-text-context.h>
#include <pdf-text-filter.h>
#include <pdf-text-ucd.h>


/* Lang/Country Minimum Length, in bytes, of the Lang/Country information within
 *  a UTF16BEstring (2bytes for the first marker, 2 bytes for LANG and 2 bytes
 *  for the last marker). */
#define PDF_TEXT_LCMINL     6

/* Maximum size, in bytes, of the Lang/Country information within a UTF16BE
 *  string (Minimum size + 2 bytes for COUNTRY). */
#define PDF_TEXT_LCMAXL     8


/* ---------------- Static (private) functions prototypes ------------------- */


/* This function receives as input a valid pdf_text_t element, where the
 *  language and country code informations will be stored. In addition to this,
 *  the function receives as input the data string (starting in the first
 *  marker), and stores a pointer to the continuation of the data string, after
 *  having read the language/country information. This function really assumes
 *  that the input data string contains in the first bytes the country/lang
 *  information.
 * Two options are possible:
 *   XXllXX   (6 bytes, XX is the marker, ll the language)
 *   XXllccXX (8 bytes, XX is the marker, ll the language and cc the country)
 */
static pdf_status_t
pdf_text_get_lang_from_utf16be(pdf_text_t element,
                               pdf_char_t **str_out,
                               pdf_size_t *str_out_length,
                               const pdf_char_t *str_in,
                               const pdf_size_t str_in_length);

static pdf_32_t
pdf_text_compare_words(const pdf_char_t *word1,
                       const pdf_size_t size1,
                       const pdf_char_t *word2,
                       const pdf_size_t size2,
                       const pdf_char_t *language1,
                       const pdf_char_t *language2,
                       pdf_status_t *p_ret_code);

/* Clean (destroy and create empty) Word Boundaries list */
static pdf_status_t
pdf_text_clean_word_boundaries_list(pdf_list_t *p_word_boundaries);
/* Fill in the Word Boundaries list using the given data */
static pdf_status_t
pdf_text_fill_word_boundaries_list(pdf_list_t word_boundaries,
                                   const pdf_char_t *data,
                                   const pdf_size_t size);

/* ----------------------------- Public functions ----------------------------*/



pdf_status_t
pdf_text_init(void)
{
  /* Initiate Text module context */
  return pdf_text_context_init();
}


pdf_text_t
pdf_text_new (void)
{
  /* Initialize all contents */
  pdf_text_t element = (pdf_text_t) pdf_alloc (sizeof(struct pdf_text_s));
  if(element != NULL)
    {
      element->data = NULL;
      element->size = 0;
      memset(&(element->lang[0]), 0, PDF_TEXT_CCL);
      memset(&(element->country[0]), 0, PDF_TEXT_CCL);
      /* Create empty word boundaries list */
      if(pdf_text_create_word_boundaries_list(&(element->word_boundaries)) != \
         PDF_OK)
        {
          pdf_dealloc(element);
          element = NULL;
        }
    }

  /* Set output element...*/
  return element;
}


pdf_status_t
pdf_text_destroy (pdf_text_t text)
{
  /* Dealloc memory */
  if(text->data != NULL)
    {
      pdf_dealloc(text->data);
      text->data = NULL;
    }
  
  /* Destroy word boundaries list */
  pdf_text_destroy_word_boundaries_list(&text->word_boundaries);

  /* Finally, clear full structure */
  pdf_dealloc(text);
  
  return PDF_OK;
}


pdf_text_t
pdf_text_dup (const pdf_text_t text)
{
  pdf_text_t element = NULL;

  /* Allocate and initialize element */
  element = pdf_text_new();

  if(element != NULL)
    {
      /* Duplicate size */
      element->size = text->size;
      
      /* Duplicate contents (if size > 0) */
      if(element->size > 0)
        {
          element->data = (pdf_char_t *) pdf_alloc (element->size);
          if(element->data != NULL)
            {
              memcpy(element->data, text->data, (size_t)element->size);
            }
        }
      
      /* Duplicate Language code and Country code (if available) */
      memcpy(element->lang, text->lang, (size_t) PDF_TEXT_CCL);
      memcpy(element->country, text->country, (size_t) PDF_TEXT_CCL);
      
      /* We don't really need to duplicate the contents of the word
       *  boundaries list, as it is a side product */
    }

  /* Set output element...*/
  return element;
}



pdf_status_t
pdf_text_new_from_host (pdf_text_t *text,
                        const pdf_char_t *str,
                        const pdf_size_t size,
                        const pdf_text_host_encoding_t enc)
{
  pdf_text_t element = NULL;
  pdf_status_t ret_code = PDF_ETEXTENC;
  
  if((str == NULL) || \
     (size == 0))
    {
      return PDF_EINVAL;
    }
  
  /* Allocate and initialize element */
  element = pdf_text_new();
  if(element == NULL)
    {
      /* Oops, element creation failed due to memory allocation error... */
      return PDF_ENOMEM;
    }
  
  /* Set Host Encoding contents */
  ret_code = pdf_text_set_host(element, str, size, enc);
  
  if(ret_code == PDF_OK)
    {
      /* Perfect! Set output variable */
      *text = element;
    }
  else
    {
      /* Conversion went wrong... so destroy object contents */
      pdf_text_destroy(element);
    }
  
  /* Return status of the conversion */
  return ret_code;
}                        



pdf_status_t
pdf_text_new_from_pdf_string (pdf_text_t *text,
                              const pdf_char_t *str,
                              const pdf_size_t size,
                              pdf_char_t **remaining_str,
                              pdf_size_t *remaining_length)
{
  pdf_status_t ret_code = PDF_ETEXTENC;
  pdf_text_t element = NULL;
  short bom_found = 0;
  short lang_found = 0;

  if(str == NULL)
    {
      return PDF_EINVAL;
    }
  
  /* Allocate and initialize element */
  element = pdf_text_new();
  
  if(element == NULL)
    {
      /* Oops, element creation failed due to memory allocation error... */
      return PDF_ENOMEM;
    }
  
  /* First of all, check first two bytes to detect UTF-16BE BOM or lang/country
   *  code initializer.
   *  If length of the text is less than 2, then we can assume it is encoded in
   *  PDF Doc Encoding */
  if(size >= 2)
    {
      /* Check Unicode Byte Order Marker encoded in UTF-16BE */
      if(pdf_text_check_unicode_bom(str, size, PDF_TEXT_UTF16_BE, 0))
        {
          bom_found = 1;
          /* Check Lang/Country Code initializer */
          if((size >= 4) && \
             (str[3] == PDF_TEXT_LCI_1) && \
             (str[2] == PDF_TEXT_LCI_0))
            {
              lang_found = 1;
            }
        }
      /* Check Lang/Country Code initializer (if this is the nth call to the
       *  function parsing a single UTF-16BE string.*/
      else if((str[1] == PDF_TEXT_LCI_1) && \
              (str[0] == PDF_TEXT_LCI_0))
        {
          lang_found = 1;
        }
    }
  
  /* If either BOM or Lang Marker are found, process PDF string as encoded 
   *  in UTF16-BE */
  if(bom_found || lang_found)
    {
      pdf_char_t *string_start = (pdf_char_t *)str;
      pdf_size_t string_length = size;

      /* Skip 2-bytes BOM */
      if(bom_found)
        {
          string_start += 2;
          string_length -= 2;
        }

      /* If lang/country code available, obtain and store the information */
      if((lang_found) && \
         (pdf_text_get_lang_from_utf16be(element,
                                         &string_start, &string_length,
                                         string_start, string_length)!=PDF_OK))
        {
          PDF_DEBUG_BASE("Invalid Lang/Code info detected");
          pdf_text_destroy(element);
          return PDF_ETEXTENC;
        }

      /* And finally convert to UTF-32... */
      ret_code = pdf_text_utf16be_to_utf32he(string_start,
                                             string_length,
                                             &(element->data),
                                             &(element->size),
                                             remaining_str,
                                             remaining_length);
    }
  /* Else, process PDF string as encoded in PDF Doc Encoding */
  else
    {
      /* We already know that this string will be fully stored, without
       *  splitting in chunks */
      if(remaining_length != NULL)
        {
          *remaining_length = 0;
        }
      if(remaining_str != NULL)
        {
          *remaining_str = NULL;
        }
      /* And perform the conversion */
      ret_code = pdf_text_pdfdocenc_to_utf32he(str,
                                               size,
                                               &(element->data),
                                               &(element->size));
    }

  /* Only store in the output element if and only if everything went ok */
  if(ret_code == PDF_OK)
    {
      *text = element;
    }
  else
    {
      pdf_text_destroy(element);
    }
  return ret_code;
}


pdf_status_t
pdf_text_new_from_unicode (pdf_text_t *text,
                           const pdf_char_t *str,
                           const pdf_size_t size,
                           const enum pdf_text_unicode_encoding_e enc)
{
  pdf_text_t element = NULL;
  pdf_status_t ret_code = PDF_OK;

  if(str == NULL)
    {
      return PDF_EINVAL;
    }
  
  /* Allocate and initialize element */
  element = pdf_text_new();
  if(element == NULL)
    {
      /* Oops, element creation failed due to memory allocation error... */
      return PDF_ENOMEM;
    }
  
  /* Set Unicode contents */
  if(size > 0)
    {
      ret_code = pdf_text_set_unicode(element, str, size, enc);
    }
  
  if(ret_code == PDF_OK)
    {
      /* Perfect! Set output variable */
      *text = element;
    }
  else
    {
      /* Conversion went wrong... so destroy object contents */
      pdf_text_destroy(element);
    }

  /* Return status of the conversion */
  return ret_code;
}


pdf_status_t
pdf_text_new_from_u32 (pdf_text_t *text,
                       pdf_u32_t number)
{
  /* Longest number to hold in 32bit: 2^32 = 4294967296 (10 chars) */
  pdf_char_t temp[10];
  pdf_size_t n;

  /* Print number in temporal char array, and get number of output chars */
  n = sprintf((char *)&temp[0],"%u",(unsigned int)number);

  /* At least one char should have been printed! */
  if(n > 0)
    {
      /* Treat the generated string as UTF-8 encoded (just numbers in ASCII) */
      return pdf_text_new_from_unicode (text, &temp[0], n, PDF_TEXT_UTF8);
    }
  else
    {
      PDF_DEBUG_BASE("Invalid u32 received: %u", (unsigned int)number);
      return PDF_EBADDATA;
    }
}


/* Return the country associated with a text variable */
const pdf_char_t *
pdf_text_get_country (pdf_text_t text)
{
  return (const pdf_char_t *)text->country;
}

/* Return the language associated with a text variable */
const pdf_char_t *
pdf_text_get_language (pdf_text_t text)
{
  return (const pdf_char_t *)text->lang;
}

/* Associate a text variable (full text) with a country code */
pdf_status_t
pdf_text_set_country (pdf_text_t text,
                      const pdf_char_t *code)
{
  if((code == NULL) || \
     (strlen((char *)code) != (PDF_TEXT_CCL-1)))
    {
      return PDF_EINVAL;
    }
  
  memcpy(&(text->country[0]), code, PDF_TEXT_CCL-1);
  /* Make sure that last byte is NUL */
  text->country[PDF_TEXT_CCL-1] = '\0';
  return PDF_OK; 
}


/* Associate a text variable (full text) with a language code */
pdf_status_t
pdf_text_set_language (pdf_text_t text,
                       const pdf_char_t *code)
{
  if((code == NULL) || \
     (strlen((char *)code) != (PDF_TEXT_CCL-1)))
    {
      return PDF_EINVAL;
    }

  memcpy(&(text->lang[0]), code, PDF_TEXT_CCL-1);
  /* Make sure that last byte is NUL */
  text->lang[PDF_TEXT_CCL-1] = '\0';
  return PDF_OK; 
}


/* Determine if a given text variable is empty (contains no text) */
inline pdf_bool_t
pdf_text_empty_p (pdf_text_t text)
{
  return ((text->size != 0) ? PDF_FALSE : PDF_TRUE);
}


/* Get default system host encoding */
pdf_text_host_encoding_t
pdf_text_get_host_encoding(void)
{
  return pdf_text_context_get_host_encoding();
}


/* Check if host encoding is available */
pdf_status_t
pdf_text_check_host_encoding(const pdf_char_t *encoding_name,
                             pdf_text_host_encoding_t *p_encoding)
{
  /* Check length of host encoding */
  if(strlen((char *)encoding_name) >= PDF_TEXT_HENMAXL)
    {
      PDF_DEBUG_BASE("Encoding name too long!");
      return PDF_EINVAL;
    }
  
  if(pdf_text_host_encoding_is_available(encoding_name) == PDF_OK)
    {
      strcpy((char *)(&(p_encoding->name[0])), (char *)encoding_name);
      p_encoding->name[strlen((char *)encoding_name)-1] = '\0';
      return PDF_OK;
    }
  else
    {
      return PDF_ETEXTENC;
    }
}


pdf_text_host_encoding_t
pdf_text_get_best_encoding (pdf_text_t text,
                            const pdf_text_host_encoding_t preferred_encoding)
{
  pdf_text_host_encoding_t ret_encoding;
  static const pdf_char_t *to_check [3] = {
    (pdf_char_t *) "UTF-8",
    (pdf_char_t *) "UTF-16",
    (pdf_char_t *) "UTF-32"
  };
  int i = 0;
  /* Check for Unicode support as host encoding */
  for(i = 0; i<3; i++)
    {
      if(pdf_text_check_host_encoding(to_check[i], &ret_encoding) == PDF_OK)
        {
          return ret_encoding;
        }
    }
  /* If host does not support any Unicode encoding conversion, return the
   *  preferred one directly */
  return preferred_encoding;
}



pdf_status_t
pdf_text_get_host (pdf_char_t **contents,
                   pdf_size_t *length,
                   const pdf_text_t text,
                   const pdf_text_host_encoding_t enc)
{  
  
  return pdf_text_utf32he_to_host (text->data, text->size, enc,
                                   contents, length);
}


/* Get the contents of a text variable encoded in PDFDocEncoding, as a NUL
 *  terminated string */
pdf_status_t
pdf_text_get_pdfdocenc (pdf_char_t **contents,
                        const pdf_text_t text)
{
  pdf_status_t ret_code;
  pdf_char_t *data = NULL;
  pdf_size_t size = -1;

  ret_code = pdf_text_utf32he_to_pdfdocenc(text->data, text->size,
                                           &data, &size);

  /* Now, if conversion went ok... */
  if(ret_code == PDF_OK)
    {
      /* Add NUL character at the end of the array */
      data = pdf_realloc(data, size+1);
      if(data != NULL)
        {
          data[size] = '\0';
          /* Set output data... */
          *contents = data;
        }
      else
        {
          return PDF_ENOMEM;
        }
    }
  /* else, clear allocated memory, if any */
  else if(data != NULL)
    {
      pdf_dealloc(data);
    }

  return ret_code;
}


pdf_status_t
pdf_text_get_unicode (pdf_char_t **contents,
                      pdf_size_t *length,
                      const pdf_text_t text,
                      const enum pdf_text_unicode_encoding_e enc,
                      const pdf_u32_t options)
{
  pdf_status_t ret_code;
  enum pdf_text_unicode_encoding_e new_enc;
  pdf_char_t *out_data = NULL;
  pdf_size_t out_length = 0;

  /* Check for invalid options... */
  if((options & PDF_TEXT_UTF16BE_WITH_LANGCODE) && \
     (enc != PDF_TEXT_UTF16_BE))
    {
      PDF_DEBUG_BASE("Lang/Country info only available for UTF-16BE");
      /* Not allowed!!! */
      return PDF_EINVAL;
    }

  /* If host endianness required, check it and convert input encoding */
  new_enc = enc;
  if((new_enc == PDF_TEXT_UTF16_HE) || \
     (new_enc == PDF_TEXT_UTF32_HE))
    {
      switch(pdf_text_context_get_host_endianness())
      {
        case PDF_TEXT_BIG_ENDIAN:
          new_enc += PDF_TEXT_HE_TO_BE;
          break;
        case PDF_TEXT_LITTLE_ENDIAN:
          new_enc += PDF_TEXT_HE_TO_LE;
          break;
        default:
          return PDF_EINVAL;
      }
    }

  /* If text is empty, set empty string */
  if((text->data == NULL) || \
     (text->size == 0))
    {
      out_data = NULL;
      out_length = 0;
      ret_code = PDF_OK;
    }
  else
    {
      /* Perform conversion */
      switch(new_enc)
      {
        case PDF_TEXT_UTF8: /* UTF-8 */
          ret_code = pdf_text_utf32he_to_utf8(text->data, text->size,
                                              &out_data, &out_length);
          break;
        case PDF_TEXT_UTF16_LE: /* UTF-16LE */
          ret_code = pdf_text_utf32he_to_utf16le(text->data, text->size,
                                                 &out_data, &out_length);
          break;
        case PDF_TEXT_UTF16_BE: /* UTF-16BE */
          ret_code = pdf_text_utf32he_to_utf16be(text->data, text->size,
                                                 &out_data, &out_length);
          break;
        case PDF_TEXT_UTF32_LE: /* UTF-32LE */
          ret_code = pdf_text_utf32he_to_utf32le(text->data, text->size,
                                                 &out_data, &out_length);
          break;
        case PDF_TEXT_UTF32_BE: /* UTF-32BE */
          ret_code = pdf_text_utf32he_to_utf32be(text->data, text->size,
                                                 &out_data, &out_length);
          break;
        default:
          ret_code = PDF_ETEXTENC;
      }
    }      
  
  /* Check if specific options were requested */
  if(options != PDF_TEXT_UNICODE_NO_OPTION)
    {
      short bom_bytes;
      short lang_bytes;
      pdf_text_bom_t bom  = pdf_text_get_unicode_bom(new_enc);
      
      /* Check if BOM really requested */
      bom_bytes = 0;
      if(options & PDF_TEXT_UNICODE_WITH_BOM)
        {
          bom_bytes = bom.bom_bytes;
        }
      
      /* Check if Lang/Country code really requested (only for UTF16BE!!) */
      lang_bytes = 0;
      if((enc == PDF_TEXT_UTF16_BE) && \
         (options & PDF_TEXT_UTF16BE_WITH_LANGCODE) && \
         (strlen((char *)text->lang) == 2))
        {
          /* At least language is available, but country may also be
           *  available */
          lang_bytes = (strlen((char *)text->country) == 2) ? PDF_TEXT_LCMAXL: \
                                                              PDF_TEXT_LCMINL;
        }
      
      /* Modify data array, if needed, to add Language/Country info and/or
       *  BOM */
      if((lang_bytes + bom_bytes) != 0)
        {
          pdf_char_t *new_out_data;
          pdf_char_t *walker;
          
          new_out_data = (pdf_char_t *)pdf_alloc(out_length + lang_bytes + \
                                                 bom_bytes);
          if(new_out_data == NULL)
            {
              return PDF_ENOMEM;
            }
          walker = new_out_data;
          
          /* Add BOM */
          if(bom_bytes > 0)
            {
              memcpy(walker, bom.bom_data, bom_bytes);       
              /* Update walker */
              walker += bom_bytes;
            }
          
          /* Add Lang/Country */
          if(lang_bytes > 0)
            {
              /* Language and Country */
              if(lang_bytes == PDF_TEXT_LCMAXL)
                {
                  sprintf((char *)walker, "%c%c%2s%2s%c%c",
                          PDF_TEXT_LCI_0,PDF_TEXT_LCI_1,
                          text->lang, text->country,
                          PDF_TEXT_LCI_0,PDF_TEXT_LCI_1);
                }
              /* Language only */
              else
                {
                  sprintf((char *)walker, "%c%c%2s%c%c",
                          PDF_TEXT_LCI_0,PDF_TEXT_LCI_1,
                          text->lang,
                          PDF_TEXT_LCI_0,PDF_TEXT_LCI_1);
                }
              walker += lang_bytes;
            }
          
          /* And copy the contents to the output array */
          if((out_length > 0) && \
             (out_data != NULL))
            {
              memcpy(walker, out_data, out_length);
            }
          
          /* Reset output data array */
          if(out_data != NULL)
            {
              pdf_dealloc(out_data);
            }
          out_data = new_out_data;
          out_length += (lang_bytes + bom_bytes);
        }      
    }
  
  /* Only store in the output element if and only if everything went ok */
  if(ret_code == PDF_OK)
    {
      *contents = out_data;
      *length = out_length;
    }
  else if(out_data != NULL)
    {
      pdf_dealloc(out_data);
    }
  return ret_code;
}


pdf_char_t *
pdf_text_get_hex (const pdf_text_t text,
                  const pdf_char_t delimiter)
{
  int i;
  int j;
  unsigned int new_str_length; 
  pdf_char_t *new_str;
  char new_hex_char [3];

  if(text->size > 0)
    {
      /* Get new string length. If input string has N bytes, we need:
       * - 1 byte for last NUL char
       * - 2N bytes for hexadecimal char representation of each byte...
       * - N-1 bytes for the separator ':'
       * So... a total of (1+2N+N-1) = 3N bytes are needed... */
      new_str_length =  3 * text->size;
      
      /* Allocate memory for new array and initialize contents to NUL */
      new_str = (pdf_char_t *)pdf_alloc(new_str_length);
      if(new_str != NULL)
        {
          memset(new_str, 0, new_str_length);
          
          /* Print hexadecimal representation of each byte... */
          for(i=0, j=0; i<text->size; i++, j+=3)
            {
              /* Clear helper array... */
              memset(&new_hex_char[0], 0, 3);
              /* Print character in helper array... */
              sprintf( new_hex_char, "%02X", (unsigned int)text->data[i]);
              /* Copy to output string... */
              memcpy(&new_str[j],&new_hex_char[0],2);
              /* And if needed, add separator */
              if(i != (text->size-1) )
                {
                  new_str[j+2] = delimiter;
                }
            }
        }
    }
  else
    {
      new_str = (pdf_char_t *)pdf_alloc(1);
      if(new_str != NULL)
        {
          new_str[0] = '\0';
        }
    }
  /* Set output string */
  return new_str;
}


pdf_status_t
pdf_text_set_host (pdf_text_t text,
                   const pdf_char_t *str,
                   const pdf_size_t size,
                   const pdf_text_host_encoding_t enc)
{
  pdf_status_t ret_code;
  pdf_char_t *temp_data;
  pdf_size_t temp_size;

  if(str == NULL)
    {
      return PDF_EINVAL;
    }

  ret_code = pdf_text_host_to_utf32he (str, size, enc,
                                       &temp_data, &temp_size);
  if(ret_code == PDF_OK)
    {
      /* Destroy previous contents of text variable, if any */
      pdf_text_clean_contents(text);
      
      /* Really set contents */
      text->data = temp_data;
      text->size = temp_size;
    }
  return ret_code;
}


/* Set PDF Doc Endoded string */
pdf_status_t
pdf_text_set_pdfdocenc (pdf_text_t text,
                        const pdf_char_t *str)
{
  pdf_status_t ret_code;
  pdf_char_t *temp_data;
  pdf_size_t temp_size;
  
  if(str == NULL)
    {
      return PDF_EINVAL;
    }
  
  ret_code = pdf_text_pdfdocenc_to_utf32he (str, strlen((char *)str),
                                            &temp_data, &temp_size);
  if(ret_code == PDF_OK)
    {
      /* Destroy previous contents of text variable, if any */
      pdf_text_clean_contents(text);
      
      /* Really set contents */
      text->data = temp_data;
      text->size = temp_size;
    }
  return ret_code;
}


pdf_status_t
pdf_text_set_unicode (pdf_text_t text,
                      const pdf_char_t *str,
                      const pdf_size_t size,
                      const enum pdf_text_unicode_encoding_e enc)
{
  pdf_status_t ret_code = PDF_ETEXTENC;
  pdf_char_t *temp_data;
  pdf_size_t temp_size;
  enum pdf_text_unicode_encoding_e new_enc = enc;
  
  if((str == NULL) || \
     (size == 0))
    {
      return PDF_EINVAL;
    }
  
  /* If host endianness required, check it and convert input encoding */
  if((new_enc == PDF_TEXT_UTF16_HE) || \
     (new_enc == PDF_TEXT_UTF32_HE))
    {
      switch(pdf_text_context_get_host_endianness())
      {
        case PDF_TEXT_BIG_ENDIAN:
          new_enc += PDF_TEXT_HE_TO_BE;
          break;
        case PDF_TEXT_LITTLE_ENDIAN:
          new_enc += PDF_TEXT_HE_TO_LE;
          break;
        default:
          return PDF_ETEXTENC;
      }
    }
  
  switch(new_enc)
  {
    case PDF_TEXT_UTF8: /* UTF-8 */
      ret_code = pdf_text_utf8_to_utf32he(str, size,
                                          &temp_data, &temp_size);
      break;
    case PDF_TEXT_UTF16_LE: /* UTF-16LE */
      ret_code = pdf_text_utf16le_to_utf32he(str, size,
                                             &temp_data, &temp_size);
      break;
    case PDF_TEXT_UTF16_BE: /* UTF-16BE */
      ret_code = pdf_text_utf16be_to_utf32he(str, size,
                                             &temp_data, &temp_size,
                                             NULL, NULL);
      break;
    case PDF_TEXT_UTF32_LE: /* UTF-32LE */
      ret_code = pdf_text_utf32le_to_utf32he(str, size,
                                             &temp_data, &temp_size);
      break;
    case PDF_TEXT_UTF32_BE: /* UTF-32BE */
      ret_code = pdf_text_utf32be_to_utf32he(str, size,
                                             &temp_data, &temp_size);
      break;
    default:
      ret_code = PDF_EINVAL;
  }
  
  if(ret_code == PDF_OK)
    {
      /* Destroy previous contents of text variable, if any */
      pdf_text_clean_contents(text);
      
      /* Really set contents */
      text->data = temp_data;
      text->size = temp_size;
    }
  return ret_code;
}


/* Concatenate the two text variables, only if country/lang info is equal */
pdf_status_t
pdf_text_concat (pdf_text_t text1,
                 const pdf_text_t text2,
                 const pdf_bool_t override_langinfo)
{
  if(!override_langinfo)
    {
      /* An error will be returned if 1) one of the lang pointers is NULL and the
       *  other one is not NULL, or 2) if both pointers are valid but the lang
       *  code is different */
      if(((text1->lang != NULL) && (text2->lang == NULL)) || \
         ((text1->lang == NULL) && (text2->lang != NULL)) || \
         ((text1->lang != NULL) && (text2->lang != NULL) && \
          (strcmp((char *)text1->lang, (char *)text2->lang) != 0)))
        {
          return PDF_ETEXTENC;
        }
      
      /* An error will be returned if 1) one of the country pointers is NULL and the
       *  other one is not NULL, or 2) if both pointers are valid but the country
       *  code is different */
      if(((text1->country != NULL) && (text2->country == NULL)) || \
         ((text1->country == NULL) && (text2->country != NULL)) || \
         ((text1->country != NULL) && (text2->country != NULL) && \
          (strcmp((char *)text1->country, (char *)text2->country) != 0)))
        {
          return PDF_ETEXTENC;
        }
    }
  
  /* Ok, so language/country info is equal or non-existent, start
   *  concatenation */
  if(text2->size > 0)
    {
      /* Re-allocate memory in first text element */
      text1->data = (pdf_char_t *)pdf_realloc(text1->data, \
                                              text1->size + text2->size);
  
      /* Copy contents of second element after the first one */
      memcpy(&(text1->data[text1->size]), text2->data, text2->size);
      
      /* Update size of first element */
      text1->size += text2->size;
    }

  return PDF_OK;
}


/* Replace a given pattern in a text object */
/* TODO: Store pointers to found patterns in a pdf_list_t */
pdf_status_t
pdf_text_replace (pdf_text_t text,
                  const pdf_text_t new_pattern,
                  const pdf_text_t old_pattern)
{
  int delta_size;
  long i;
  long n_replacements;
  
  /* Empty old pattern is not allowed */
  if(pdf_text_empty_p(old_pattern))
    {
      return PDF_ETEXTENC;
    }

  /* Well, if text is shorter than the old pattern, just return */
  if(text->size < old_pattern->size)
    {
      return PDF_OK;
    }

  /* Compute the delta in bytes required for each conversion */
  delta_size = (int)(new_pattern->size - old_pattern->size);
  
  /* First, count number of replacements to be done... */
  n_replacements = 0;
  i = 0;
  while(i<= (text->size - old_pattern->size)/4)
    {
      if(memcmp(&(text->data[i*4]), old_pattern->data, old_pattern->size)==0)
        {
          /* Old pattern found! */
          if(delta_size == 0)
            {
              /* If length of old pattern and length of new pattern are equal,
               *  the direct replacement can be done */
              memcpy(&(text->data[i*4]), new_pattern->data, new_pattern->size);
            }
          else
            {
              n_replacements++;
            }
          /* The index must be updated to skip the replacement */
          i += (old_pattern->size/4);
        }
      else
        {
          i++;
        }
    }
    
  /* If delta is equal to zero, replacement has been already done */
  if((delta_size == 0) || \
     (n_replacements == 0))
    {
      return PDF_OK;
    }
  else
    {
      pdf_size_t new_size;
      pdf_char_t *new_data;
      long first_char;
      long j;
      
      /* Compute new size and allocate new memory chunk */
      new_size = text->size + (delta_size * n_replacements);
      new_data = (pdf_char_t *)pdf_alloc(new_size);

      i = 0;
      j = 0;
      first_char = 0;
      while(i<= (text->size - old_pattern->size)/4)
        {
          if(memcmp(&(text->data[i*4]), old_pattern->data, old_pattern->size)==0)
            {
              /* Pattern found, so copy all the contents previous to the pattern
               *  that are equal in both arrays. Doing it this way, we will
               *  do it more efficiently */
              if(i>0)
                {
                  memcpy(&(new_data[j*4]), &(text->data[first_char*4]), \
                         (i-first_char)*4);
                  j += (i-first_char);
                }

              /* New pattern could be empty! */
              if(new_pattern->size > 0)
                {
                  /* Now copy new pattern in output string */
                  memcpy(&(new_data[j*4]), new_pattern->data, \
                         new_pattern->size);
                  j += new_pattern->size/4;
                }
              /* Update indexes */
              i += old_pattern->size/4;
              first_char = i;
            }
          else
            {
              i++;
            }
        }
      /* And finally, copy last contents */
      if(first_char != (text->size/4))
        {
          memcpy(&(new_data[j*4]), &(text->data[first_char*4]), \
                 (i-first_char)*4);
        }

      /* Set correct final size and final content */
      pdf_dealloc(text->data);
      text->data = new_data;
      text->size = new_size;
    }
  
  return PDF_OK;
}


/* Replace a given ASCII-7 pattern in a text object */
pdf_status_t
pdf_text_replace_ascii (pdf_text_t text,
                        const pdf_char_t *new_pattern,
                        const pdf_char_t *old_pattern)
{
  /* Check if patterns are real ASCII-7 valid strings */
  if((!pdf_text_is_ascii7(old_pattern,
                         (pdf_size_t)strlen((char *)old_pattern))) || \
     (!pdf_text_is_ascii7(new_pattern,
                         (pdf_size_t)strlen((char *)new_pattern))))
    {
      PDF_DEBUG_BASE("At least one of the requested patterns is not "
                     "7-bit ASCII");
      return PDF_EINVAL;
    }
  else
    {
      /* Ok, so load ASCII strings as if it were UTF-8 strings */
      pdf_text_t new_pattern_text;
      pdf_text_t old_pattern_text;
      pdf_status_t ret_code;

      /* Create intermediate pdf_text_t variables */
      if(pdf_text_new_from_unicode(&new_pattern_text,
                                   new_pattern,
                                   (pdf_size_t)strlen((char *)new_pattern),
                                   PDF_TEXT_UTF8)!=PDF_OK)
        {
          PDF_DEBUG_BASE("Error creating pdf_text_t from ASCII new pattern");
          return PDF_EBADDATA;
        }
      if(pdf_text_new_from_unicode(&old_pattern_text,
                                   old_pattern,
                                   (pdf_size_t)strlen((char *)old_pattern),
                                   PDF_TEXT_UTF8)!=PDF_OK)
        {
          PDF_DEBUG_BASE("Error creating pdf_text_t from ASCII old pattern");
          return PDF_EBADDATA;
        }

      /* Perform replacement */
      ret_code = pdf_text_replace(text, new_pattern_text, old_pattern_text);
      
      /* Destroy used intermediate variables */
      pdf_text_destroy(new_pattern_text);
      pdf_text_destroy(old_pattern_text);
      
      return ret_code;
    }
}


pdf_status_t
pdf_text_filter (pdf_text_t text,
                 const pdf_u32_t filter)
{
  /* More than one filter at the same time can be requested! But Caution!
   *  UpperCase filter, LowerCase filter and TitleCase filter are mutually
   *  exclusive (at most only one of them must be enabled) */
  
  if((((filter & PDF_TEXT_FILTER_UPPER_CASE) ? 1 : 0) + \
      ((filter & PDF_TEXT_FILTER_LOWER_CASE) ? 1 : 0) + \
      ((filter & PDF_TEXT_FILTER_TITLE_CASE) ? 1 : 0)) > 1)
    {
      PDF_DEBUG_BASE("At most only one case conversion filter can be applied");
      return PDF_EINVAL;
    }

  /* 0x00000001 */
  if((filter & PDF_TEXT_FILTER_LINE_ENDINGS) && \
     (pdf_text_filter_normalize_line_endings(text) != PDF_OK))
    {
      PDF_DEBUG_BASE("Error applying Line Ending normalization filter");
      return PDF_ETEXTENC;
    }

  /* 0x00000010 */
  if((filter & PDF_TEXT_FILTER_UPPER_CASE) && \
     (pdf_text_filter_upper_case(text) != PDF_OK))
    {
      PDF_DEBUG_BASE("Error applying Upper Case filter");
      return PDF_ETEXTENC;
    }
  /* 0x00000100 */
  else if((filter & PDF_TEXT_FILTER_LOWER_CASE) && \
          (pdf_text_filter_lower_case(text) != PDF_OK))
    {
      PDF_DEBUG_BASE("Error applying Lower Case filter");
      return PDF_ETEXTENC;
    }
  /* 0x00001000 */
  else if((filter & PDF_TEXT_FILTER_TITLE_CASE) && \
           (pdf_text_filter_title_case(text) != PDF_OK))
    {
      PDF_DEBUG_BASE("Error applying Title Case filter");
      return PDF_ETEXTENC;
    }

  /* 0x00010000 */
  if((filter & PDF_TEXT_FILTER_REMOVE_AMP) && \
     (pdf_text_filter_remove_amp(text) != PDF_OK))
    {
      PDF_DEBUG_BASE("Error applying Ampersand Removal filter");
      return PDF_ETEXTENC;
    }

  /* 0x00100000 */
  if((filter & PDF_TEXT_FILTER_NORM_WITH_FULL_WIDTH) && \
     (pdf_text_filter_normalize_full_width_ascii(text) != PDF_OK))
    {
      PDF_DEBUG_BASE("Error applying FullWidth Normalization filter");
      return PDF_ETEXTENC;
    }

  /* 0x01000000 */
    if((filter & PDF_TEXT_FILTER_REMOVE_LINE_ENDINGS) && \
       (pdf_text_filter_remove_line_endings(text) != PDF_OK))
    {
      PDF_DEBUG_BASE("Error applying Line Ending Removal filter");
      return PDF_ETEXTENC;
    }

  return PDF_OK;
}


pdf_32_t
pdf_text_cmp (pdf_text_t text1,
              pdf_text_t text2,
              pdf_bool_t case_sensitive,
              pdf_status_t *p_ret_code)
{
  if(p_ret_code != NULL)
    {
      *p_ret_code = PDF_OK;
    }
  
  /* Compare sizes of the texts */
  if(text1->size != text2->size)
    {
      return ((text1->size > text2->size) ? 1 : -1);
    }
  
  if(case_sensitive == PDF_TRUE)
    {
      return memcmp(text1->data, text2->data, text1->size);
    }
  else
    {
      /* Generate word boundaries list, if not already done */
      if((pdf_text_fill_word_boundaries_list(text1->word_boundaries, \
                                             text1->data, \
                                             text1->size) == PDF_OK) && \
         (pdf_text_fill_word_boundaries_list(text2->word_boundaries, \
                                             text2->data, \
                                             text2->size) == PDF_OK))
        {
          pdf_size_t size1;
          pdf_size_t size2;
          
          size1 = pdf_list_size(text1->word_boundaries);
          size2 = pdf_list_size(text2->word_boundaries);
          /* First, compare number of words in each text */
          if(size1 != size2)
            {
              return ((size1 > size2) ? 1 : -1);
            }
          else
            {
              /* Perform a word-per-word lower case comparison! */
              int n;
              
              /* Get word from both texts */
              n = 0;
              while(n < size1)
                {
                  struct pdf_text_wb_s *p_word1;
                  struct pdf_text_wb_s *p_word2;
                  pdf_32_t ret_num;

                  if(pdf_list_get_at(text1->word_boundaries, \
                                     n, \
                                     (const void **)&p_word1) != PDF_OK)
                    {
                      *p_ret_code = PDF_ETEXTENC;
                      PDF_DEBUG_BASE("Error getting word '%d' from text1", n);
                      /* An error happened computing word boundaries! */
                      return -1; 
                    }

                  if(pdf_list_get_at(text2->word_boundaries,
                                     n,
                                     (const void **)&p_word2) != PDF_OK)
                    {
                      *p_ret_code = PDF_ETEXTENC;
                      PDF_DEBUG_BASE("Error getting word '%d' from text2", n);
                      /* An error happened computing word boundaries! */
                      return -1; 
                    }
                  
                  ret_num = pdf_text_compare_words(p_word1->word_start,
                                                   p_word1->word_size,
                                                   p_word2->word_start,
                                                   p_word2->word_size,
                                                   pdf_text_get_language(text1),
                                                   pdf_text_get_language(text2),
                                                   p_ret_code);
                  /* If words are not equal, return the code */
                  if(ret_num != 0)
                    {
                      return ret_num;
                    }
                  ++n; 
                }
              /* If arrived here, the strings are completely equal */
              return 0;
            }
        }
      else
        {
          if(p_ret_code != NULL)
            {
              *p_ret_code = PDF_ETEXTENC;
            }
          PDF_DEBUG_BASE("Problem computing word boundaries. Comparison is not"
                         " valid");
          return -1; /* An error happened computing word boundaries! */
        }
    }
  return 0;
}


/* -------------------------- Private functions ----------------------------- */



static pdf_32_t
pdf_text_compare_words(const pdf_char_t *word1,
                       const pdf_size_t size1,
                       const pdf_char_t *word2,
                       const pdf_size_t size2,
                       const pdf_char_t *language1,
                       const pdf_char_t *language2,
                       pdf_status_t *p_ret_code)
{
  pdf_char_t *lower1;
  pdf_char_t *lower2;
  pdf_size_t new_size1;
  pdf_size_t new_size2;
  pdf_size_t worst_size;
    
  if(p_ret_code != NULL)
    {
      *p_ret_code = PDF_OK;
    }
  
  /* Compare sizes of words */
  if(size1 != size2)
    {
      return ((size1 > size2) ? 1 : -1);
    }
  
  /* Compute new worst word length */
  worst_size = size1 * UCD_SC_MAX_EXPAND;
  
  /* Allocate memory for lowercases */
  lower1 = (pdf_char_t *)pdf_alloc(worst_size);
  lower2 = (pdf_char_t *)pdf_alloc(worst_size);
  if((lower1 == NULL) || \
     (lower2 == NULL))
    {
      PDF_DEBUG_BASE("Unable to compare words");
      if(p_ret_code != NULL)
        {
          *p_ret_code = PDF_ENOMEM;
        }
      if(lower1 != NULL)
        {
          pdf_dealloc(lower1);
        }
      if(lower2 != NULL)
        {
          pdf_dealloc(lower2);
        }
      return -1;
    }

  /* Lowercase words */
  if(pdf_text_ucd_word_change_case(lower1, &new_size1,
                                   UNICODE_CASE_INFO_LOWER_CASE,
                                   word1, size1, language1)!= PDF_OK)
    {
      PDF_DEBUG_BASE("Problem lowercasing word 1");
      pdf_dealloc(lower1);
      pdf_dealloc(lower2);
      *p_ret_code = PDF_ETEXTENC;
      return -1;
    }
  if(pdf_text_ucd_word_change_case(lower2, &new_size2,
                                   UNICODE_CASE_INFO_LOWER_CASE,
                                   word2, size2, language2)!= PDF_OK)
    {
      PDF_DEBUG_BASE("Problem lowercasing word 2");
      pdf_dealloc(lower1);
      pdf_dealloc(lower2);
      *p_ret_code = PDF_ETEXTENC;
      return -1;
    }
                                           
  /* Compare NEW sizes of words */
  if(new_size1 != new_size2)
    {
      pdf_dealloc(lower1);
      pdf_dealloc(lower2);
      return ((new_size1 > new_size2) ? 1 : -1);
    }
  else
    {
      /* Compare contents of words */
      pdf_32_t ret_val;
      ret_val = memcmp(lower1, lower2, new_size1);
      pdf_dealloc(lower1);
      pdf_dealloc(lower2);
      return ret_val;
    }
}


/* Function to clean all contents of a given pdf_text_t variable */
void
pdf_text_clean_contents(pdf_text_t text)
{
  /* Clear all contents */
  if(text->data != NULL)
    {
      pdf_dealloc(text->data);
      text->data = NULL;
    }

  /* Clean list of word breaks (destroy and create empty) */
  pdf_text_clean_word_boundaries_list(&(text->word_boundaries));

  /* Clean country and language info */
  memset(&(text->lang[0]), 0, PDF_TEXT_CCL);
  memset(&(text->country[0]), 0, PDF_TEXT_CCL);
  /* Reset data size */
  text->size = 0;
}



static pdf_status_t
pdf_text_get_lang_from_utf16be(pdf_text_t element,
                               pdf_char_t **str_out,
                               pdf_size_t *str_out_length,
                               const pdf_char_t *str_in,
                               const pdf_size_t str_in_length)
{
  /* Country code is optional */
  short country_available = PDF_FALSE;
  pdf_char_t aux[PDF_TEXT_CCL];
  
  /* Check first code marker and MINIMUM length of array */
  if((str_in_length < PDF_TEXT_LCMINL) || \
     (str_in[1] != PDF_TEXT_LCI_1) || \
     (str_in[0] != PDF_TEXT_LCI_0))
    {
      return PDF_EINVAL;
    }
  
  /* Check last code marker position and MAXIMUM length of array. 
   *  Additionally, set `str_out' and `str_out_length' */
  if((str_in[5] != PDF_TEXT_LCI_1) || \
     (str_in[4] != PDF_TEXT_LCI_0))
    {
      /* Check last marker in bytes 6 and 7... */
      if((str_in_length >= PDF_TEXT_LCMAXL) && \
         (str_in[7] == PDF_TEXT_LCI_1) && \
         (str_in[6] == PDF_TEXT_LCI_0))
        {
          country_available = PDF_TRUE;
          *str_out = (pdf_char_t *)str_in + PDF_TEXT_LCMAXL;
          *str_out_length = str_in_length - PDF_TEXT_LCMAXL;
        }
      else
        {
          /* Either size is too short or last marker not found. This is a
           *  problem in the input data string */
          return PDF_EINVAL;
        }
    }
  else
    {
      /* There is no optional country code info */
      *str_out = (pdf_char_t *)str_in + PDF_TEXT_LCMINL;
      *str_out_length = str_in_length - PDF_TEXT_LCMINL;
    }
  
  
  /* Store 2-bytes ISO 639 language code */
  memcpy(&aux[0], &str_in[2], PDF_TEXT_CCL-1);
  aux[PDF_TEXT_CCL-1] = '\0';
  if(pdf_text_set_language(element, (pdf_char_t *)aux) != PDF_OK)
    {
      return PDF_ETEXTENC;
    }
  
  /* If optional country code is also available, store it... */
  if(country_available)
    {
      memcpy(&aux[0], &str_in[4], PDF_TEXT_CCL-1);
      /* Store 2-bytes ISO 3166 country code */
      if(pdf_text_set_country(element, (pdf_char_t *)aux) != PDF_OK)
        {
          return PDF_ETEXTENC;
        }
    }
  
  return PDF_OK;
}



pdf_bool_t
pdf_text_is_ascii7(const pdf_char_t *utf8data, const pdf_size_t size)
{
  pdf_size_t i;
  for(i=0; i<size; ++i)
    {
      /* Just check the MSB. In ASCII-7 it must be 0 */
      if(utf8data[i] & 0x80)
        {
          return PDF_FALSE;
        }
    }
  return PDF_TRUE;
}


/* Generate Word Boundaries list from text object */
pdf_status_t
pdf_text_generate_word_boundaries(pdf_text_t text)
{
  if(pdf_list_size(text->word_boundaries) == 0)
    {
      return pdf_text_fill_word_boundaries_list(text->word_boundaries,
                                                text->data, text->size);
    }
  else
    {
      /* List already created */
      return PDF_OK;
    }
}

pdf_status_t
pdf_text_destroy_word_boundaries_list(pdf_list_t *p_word_boundaries)
{
  /* Destroy list */
  pdf_list_destroy(*p_word_boundaries);
  return PDF_OK;
}


/* Create empty Word Boundaries list */
pdf_status_t
pdf_text_create_word_boundaries_list(pdf_list_t *p_word_boundaries)
{
  pdf_list_t temp_list;
  /* Initialize word boundaries list */
  if(pdf_list_create (NULL, NULL, PDF_TRUE, &temp_list) != PDF_OK)
    {
      return PDF_ETEXTENC;
    }
  
  /* Set output if everything went ok */
  *p_word_boundaries = temp_list;
  return PDF_OK;
}

/* Clean (destroy and create empty) Word Boundaries list */
static pdf_status_t
pdf_text_clean_word_boundaries_list(pdf_list_t *p_word_boundaries)
{
  /* Only destroy+create if list is not empty! */
  if(pdf_list_size(*p_word_boundaries) != 0)
    {
      /* Destroy list */
      pdf_text_destroy_word_boundaries_list(p_word_boundaries);
      /* Create empty list */
      return pdf_text_create_word_boundaries_list(p_word_boundaries);
    }
  else
    {
      /* List is already empty */
      return PDF_OK;
    }
}


/* Fill in the Word Boundaries list using the given data */
static pdf_status_t
pdf_text_fill_word_boundaries_list(pdf_list_t word_boundaries,
                                   const pdf_char_t *data,
                                   const pdf_size_t size)
{
  struct pdf_text_wb_s word;
  
  /* Perform a basic check of data length */
  if(size % 4 != 0)
    {
      return PDF_EINVAL;
    }
  
  /* Only try to find word boundaries if length is greater than 0! */
  if(size > 0)
    {
      pdf_char_t *walker;
      pdf_size_t n_bytes_left;
      
      /* Initialize walker and number of bytes left */
      walker = (pdf_char_t *)data;
      n_bytes_left = size;
      
      /* RULE WB1: Break at the start of text ( SOT % ) */
      word.word_start = walker;
      
      while(n_bytes_left > 0)
        {
          if(pdf_text_ucd_wb_detect_next(walker,
                                         n_bytes_left,
                                         &(word.word_stop),
                                         &n_bytes_left)!= PDF_OK)
            {
              return PDF_ETEXTENC;
            }
          
          /* Compute word size in bytes */
          word.word_size = (word.word_start - word.word_stop) + 4;
          
          /* Add new word boundary to list */
          pdf_list_add_last(word_boundaries, &word);
        }
    }
  
  return PDF_OK;
}


/* End of pdf-text.c */

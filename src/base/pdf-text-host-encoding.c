/* -*- mode: C -*- Time-stamp: "08/08/29 01:39:33 jemarch"
 *
 *       File:         pdf-text-host-encoding.c
 *       Date:         Fri Jan 11 21:09:23 2008
 *
 *       GNU PDF Library - Encoded Text handling utilities - Host Encodings
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

#include <pdf-types.h>
#include <pdf-text.h>
#include <pdf-text-context.h>
#include <pdf-text-encoding.h>
#include <pdf-text-host-encoding.h>

#include <string.h>

#ifdef PDF_HOST_WIN32
 #include <windows.h>
#else
 #include <iconv.h>
 #include <errno.h>
#endif


/*
 * ICONV API
 * -----------
 * iconv_t iconv_open (const char* tocode, const char* fromcode)
 *
 * size_t iconv (iconv_t cd,
 *               const char **inbuf, size_t *inbytesleft,
 *               char **outbuf, size_t *outbytesleft);
 *
 * int iconv_close (iconv_t cd);
 */


/* 
 * WINDOWS API
 * -------------
 * int MultiByteToWideChar(UINT CodePage, 
 *                         DWORD dwFlags,         
 *                         LPCSTR lpMultiByteStr, 
 *                         int cbMultiByte,       
 *                         LPWSTR lpWideCharStr,  
 *                         int cchWideChar);
 *
 * int WideCharToMultiByte(UINT CodePage, 
 *                         DWORD dwFlags, 
 *                         LPCWSTR lpWideCharStr,
 *                         int cchWideChar, 
 *                         LPSTR lpMultiByteStr, 
 *                         int cbMultiByte,
 *                         LPCSTR lpDefaultChar,    
 *                         LPBOOL lpUsedDefaultChar);
 * 
 * UINT GetACP(void);
 *
 */

#ifdef PDF_HOST_WIN32
static DWORD
pdf_text_get_dwflags_for_cp(UINT CodePage, DWORD def_dwflags)
{
  /* dwFlags has some restrictions */
  switch(CodePage)
  {
    case 50220:
    case 50221:
    case 50222:
    case 50225:
    case 50227:
    case 50229:
    case 52936:
    case 54936:
    case 57002:
    case 57003:
    case 57004:
    case 57005:
    case 57006:
    case 57007:
    case 57008:
    case 57009:
    case 57010:
    case 57011:
    case 65000:
    case 42:
      return 0;
    default:
      return def_dwflags;
  }
}


pdf_status_t
pdf_text_convert_encoding_name_to_CP(const pdf_char_t *encoding_name,
                                     UINT *pCP)
{
  UINT CodePage;
  char end_char;

  /* In windows, the charset name stored in the pdf_text_host_encoding_t
   *  element will be in the following format: "CPn", where 'n' is the
   *  code page number (unsigned integer) obtained with GetACP() */
    
  /* So first of all, check windows host encoding */
  if((strlen((char *)encoding_name) < 3) || \
     (strncmp((char *)encoding_name,"CP",2) != 0))
    {
      PDF_DEBUG_BASE("Host encoding received seems not valid");
      return PDF_ETEXTENC;
    }

  /* Get codepage as unsigned integer. `strtol' will return 0 if it was not
   *  able to correctly parse the string. BTW, 0 is not a valid code page. */
  CodePage = (UINT) strtol ((char *)&encoding_name[2],
                            &end_char,
                            10);
  if(CodePage == 0)
    {
      PDF_DEBUG_BASE("Problem converting input CP value '%s'",
                     encoding_name);
      return PDF_ETEXTENC;
    }
  else
    {
      *pCP = CodePage;
      return PDF_OK;
    }
}

#endif


pdf_status_t
pdf_text_host_encoding_is_available(const pdf_char_t *encoding_name)
{
#ifdef PDF_HOST_WIN32
  {
    UINT CodePage;

    if(pdf_text_convert_encoding_name_to_CP(encoding_name, &CodePage) != PDF_OK)
      {
        PDF_DEBUG_BASE("Invalid windows encoding name received...");
        return PDF_ETEXTENC;
      }

    /* Check given code page in the system */
    return ((IsValidCodePage(CodePage)) ? PDF_OK : PDF_ETEXTENC);
  }
#else
  {
    iconv_t check;

    /* Check conversion from Host Encoding to UTF-32HE */
    check = iconv_open((char *)encoding_name, \
                       (PDF_IS_BIG_ENDIAN ? "UTF-32BE" : "UTF-32LE"));
    if(check == (iconv_t)-1)
      {
        PDF_DEBUG_BASE("Conversion from '%s' to UTF-32HE not available",
                       encoding_name);
        return PDF_ETEXTENC;
      }
    iconv_close(check);

    /* Check conversion from UTF-32HE to Host Encoding */
    check = iconv_open((PDF_IS_BIG_ENDIAN ? "UTF-32BE" : "UTF-32LE"), \
                       (char *)encoding_name);
    if(check == (iconv_t)-1)
      {
        PDF_DEBUG_BASE("Conversion from UTF-32HE to '%s' not available",
                       encoding_name);
        return PDF_ETEXTENC;
      }
    iconv_close(check);
    
    return PDF_OK;
  }
#endif
}


#ifdef PDF_HOST_WIN32
static pdf_status_t
pdf_text_utf32he_to_host_win32(const pdf_char_t      *input_data,
                               const pdf_size_t      input_length,
                               const pdf_text_host_encoding_t enc,
                               pdf_char_t            **p_output_data,
                               pdf_size_t            *p_output_length)
{
  pdf_status_t ret_code;
  pdf_char_t *temp_data;
  pdf_size_t  temp_size;
  UINT CodePage;
  /* Firstly, convert from UTF-32HE to UTF-16LE */
  ret_code = pdf_text_utf32he_to_utf16le(input_data,
                                         input_length,
                                         &temp_data,
                                         &temp_size);
  if(ret_code != PDF_OK)
    {
      PDF_DEBUG_BASE("Couldn't convert from UTF-32HE to UTF-16LE");
      return PDF_ETEXTENC;
    }

  /* In windows, the charset name stored in the pdf_text_host_encoding_t
   *  element will be in the following format: "CPn", where 'n' is the
   *  code page number (unsigned integer) obtained with GetACP() */

  /* So check windows host encoding */
  if(pdf_text_convert_encoding_name_to_CP(enc.name, &CodePage) != PDF_OK)
    {
      PDF_DEBUG_BASE("Invalid windows encoding name received...");
      pdf_dealloc(temp_data);
      return PDF_ETEXTENC;
    }
  else
    {
      DWORD dwFlags;
      int output_nmbyte;
      BOOL default_used = 0;

      /* Get dwFlags value */
      dwFlags = 0;

      /* First of all, query the length of the output string */
      SetLastError(0);
      output_nmbyte =  WideCharToMultiByte(CodePage,     /* CodePage */
                                           dwFlags,      /* dwFlags */
                                           (LPCWSTR)temp_data, /* lpWideCharStr */
                                           (temp_size/sizeof(WCHAR)), /* cbWideChar */
                                           NULL,         /* lpMultiByteStr */
                                           0,            /* ccMultiByte */
                                           NULL,            /* lpDefaultChar */
                                           &default_used); /* lpUsedDefaultChar */

      /* Check if we got an error with the call to WideCharToMultiByte */
      if(output_nmbyte == 0 || default_used)
        {
#ifdef HAVE_DEBUG_BASE
          switch(GetLastError())
            {
              case ERROR_INVALID_FLAGS:
                PDF_DEBUG_BASE("Invalid data to convert to Host Encoding:"
                               " 'Invalid flags'");
                break;
              default:
                PDF_DEBUG_BASE("Invalid data to convert to Host Encoding");
                break;
            }
#endif
          pdf_dealloc(temp_data);
          return PDF_EBADTEXT;
        }

      /* Allocate memory for output buffer */
      *p_output_length = output_nmbyte;
      *p_output_data = (pdf_char_t *)pdf_alloc(*p_output_length);
      if(*p_output_data == NULL)
        {
          pdf_dealloc(temp_data);
          return PDF_ENOMEM;
        }

      /* Launch the conversion to host encoding */
      SetLastError(0);
      default_used = 0;
      if((WideCharToMultiByte(CodePage,     /* CodePage */
                              dwFlags,      /* dwFlags */
                              (LPCWSTR)temp_data, /* lpWideCharStr */
                              (temp_size/sizeof(WCHAR)), /* cbWideChar */
                              (char *)*p_output_data, /* lpMultiByteStr */
                              *p_output_length, /* ccMultiByte */
                              NULL,            /* lpDefaultChar */
                              &default_used) != output_nmbyte) || \
         (default_used))
        {
          PDF_DEBUG_BASE("Problem performing the host encoding conversion");
          pdf_dealloc(*p_output_data);
          pdf_dealloc(temp_data);
          return PDF_ETEXTENC;
        }
      else
        {
          /* Check last byte... could be NUL and we don't want it */
          if((*p_output_data)[*p_output_length -1] == '\0')
            {
              pdf_char_t *temp;
              temp = pdf_realloc((*p_output_data), (*p_output_length -1));
              if(temp != NULL)
                {
                  *p_output_data = temp;
                  *p_output_length = *p_output_length -1;
                }
            }
          pdf_dealloc(temp_data);
          return PDF_OK;
        }
    }
}

#else

static pdf_status_t
pdf_text_utf32he_to_host_iconv(const pdf_char_t      *input_data,
                               const pdf_size_t      input_length,
                               const pdf_text_host_encoding_t enc,
                               pdf_char_t            **p_output_data,
                               pdf_size_t            *p_output_length)
{
  iconv_t to_host;
  size_t n_conv;
  char *in_str;
  size_t n_in;
  size_t n_out;
  pdf_char_t *new_data;
  char *out_str;
  pdf_size_t worst_length;
  pdf_size_t new_length;

    /* Check if conversion is available. If we just specify "UTF-32" as the
  *  input encoding requested, iconv will expect the BOM by default, and
  *  we don't want it, so we specify directly the endianness required in the
    *  name of the encoding, depending on the host endianness */
  to_host = iconv_open((char *)enc.name, \
      (PDF_IS_BIG_ENDIAN ? "UTF-32BE" : "UTF-32LE"));
  if(to_host == (iconv_t)-1)
    {
      PDF_DEBUG_BASE("Conversion from UTF-32 to '%s' not available: '%s'",
                     enc.name, strerror(errno));
      return PDF_ETEXTENC;
    }

  /* Prepare lengths and locations.
   *  The worst length is computed as having one single output byte for each 
   *  input single byte */
  worst_length = input_length+4;
  new_data = (pdf_char_t *)pdf_alloc(worst_length);
  if(new_data == NULL)
    {
      iconv_close(to_host);
      return PDF_ENOMEM;
    }
  n_out = worst_length;
  in_str = (char *)input_data;
  out_str = (char *)new_data;
  n_in = input_length;

  while(n_in > 0)
    {
      /* Convert */
      n_conv = iconv(to_host, &in_str, &n_in, &out_str, &n_out);

      /* Check conversion output status. We check errno to see if the problem
       *  is that more buffer is needed in the output. If this is the case,
       *  we just give a second try to the worst length and reallocate memory.
       *  There is no problem to use errno in multi-threaded applications
       *  if the library is compiled with -D_REENTRANT */
      if(n_conv == (size_t)-1)
        {
          if(errno == E2BIG)
            {
              pdf_size_t n_bytes_generated = 0;
              /* Compute the number of bytes actually generated in the
               *  output buffer. */
              n_bytes_generated = (pdf_size_t) (worst_length - n_out);

              /* We need more output buffer */
              worst_length += (n_in);

              PDF_DEBUG_BASE("Reallocating to '%lu'. "
                             "'%lu' bytes are already generated",
                             (unsigned long) worst_length,
                             (unsigned long) n_bytes_generated);
              /* Reallocate buffer with greater size */
              new_data = (pdf_char_t *)pdf_realloc(new_data,worst_length);
              if(new_data == NULL)
                {
                  return PDF_ENOMEM;
                }

              /* The re-allocated new data does not have to be in the same
               *  memory place as the original one, so the `out_str' pointer
               *  must be reset */
              out_str = (char *) &new_data[n_bytes_generated];

              /* The number of bytes available in the buffer must also be
               *  reset */
              n_out = (worst_length - n_bytes_generated);
            }
          else
            {
              iconv_close(to_host);
              PDF_DEBUG_BASE("Invalid data to convert to Host Encoding: '%s'",
                             strerror(errno));
              return PDF_EBADTEXT;
            }
        }
    }

  /* Compute new final length */
  new_length = worst_length - n_out;

  /* Finally, reset the buffer length to its correct size */
  if(new_length != worst_length)
    {
      new_data = (pdf_char_t *)pdf_realloc(new_data,new_length);
      if(new_data == NULL)
        {
          iconv_close(to_host);
          return PDF_ENOMEM;
        }
    }

  /* And set the output values */
  *p_output_data = new_data;
  *p_output_length = new_length;

  iconv_close(to_host);
  return PDF_OK;
}
#endif



pdf_status_t
pdf_text_utf32he_to_host(const pdf_char_t      *input_data,
                         const pdf_size_t      input_length,
                         const pdf_text_host_encoding_t enc,
                         pdf_char_t            **p_output_data,
                         pdf_size_t            *p_output_length)
{
#ifdef PDF_HOST_WIN32
  return pdf_text_utf32he_to_host_win32(input_data,
                                        input_length,
                                        enc,
                                        p_output_data,
                                        p_output_length);
#else
  return pdf_text_utf32he_to_host_iconv(input_data,
                                        input_length,
                                        enc,
                                        p_output_data,
                                        p_output_length);
#endif
}

#ifdef PDF_HOST_WIN32
static pdf_status_t
pdf_text_host_to_utf32he_win32(const pdf_char_t      *input_data,
                               const pdf_size_t      input_length,
                               const pdf_text_host_encoding_t enc,
                               pdf_char_t            **p_output_data,
                              pdf_size_t            *p_output_length)
{
  UINT CodePage;

  /* In windows, the charset name stored in the pdf_text_host_encoding_t
   *  element will be in the following format: "CPn", where 'n' is the
   *  code page number (unsigned integer) obtained with GetACP() */

  /* So first of all, check windows host encoding */
  if(pdf_text_convert_encoding_name_to_CP(enc.name, &CodePage) != PDF_OK)
    {
      PDF_DEBUG_BASE("Invalid windows encoding name received...");
      return PDF_ETEXTENC;
    }
  else
    {
      DWORD dwFlags;
      int output_nwchars;
      pdf_char_t *temp_data;
      pdf_size_t temp_size;

      /* Get dwFlags value */
      dwFlags = pdf_text_get_dwflags_for_cp(CodePage, MB_ERR_INVALID_CHARS);

      /* For ASCII-7, check MSB... MultiByteToWideChar doesn't do it, and the
       *  behaviour should be equal to that of iconv() */
      if(CodePage == 20127) /* ASCII-7 code point */
        {
          if(pdf_text_is_ascii7(input_data, input_length) == PDF_FALSE)
            {
              PDF_DEBUG_BASE("Invalid data to convert from Host Encoding:"
                             " Not ASCII-7");
              return PDF_EBADTEXT;
            }
        }

      /* First of all, query the length of the output string */
      SetLastError(0);
      output_nwchars =  MultiByteToWideChar(CodePage,     /* CodePage */
                                            dwFlags,      /* dwFlags */
                                            (char *)input_data, /* lpMultiByteStr */
                                            input_length, /* cbMultiByte */
                                            NULL,         /* lpWideCharStr */
                                            0);           /* cchWideChar */

      /* Check if we got an error with the call to MultiByteToWideChar*/
      if(output_nwchars == 0)
        {
#ifdef HAVE_DEBUG_BASE
          switch(GetLastError())
            {
              case ERROR_INVALID_FLAGS:
                PDF_DEBUG_BASE("Invalid data to convert from Host Encoding:"
                               " 'Invalid flags'");
                break;
              case ERROR_NO_UNICODE_TRANSLATION:
                PDF_DEBUG_BASE("Invalid data to convert from Host Encoding:"
                               " 'No Unicode Translation'");
                break;
              default:
                PDF_DEBUG_BASE("Invalid data to convert from Host Encoding");
                break;
            }
#endif
          return PDF_EBADTEXT;
        }

      /* Allocate memory for output buffer */
      temp_size = output_nwchars * sizeof(WCHAR);
      temp_data = (pdf_char_t *)pdf_alloc(temp_size);
      if(temp_data == NULL)
        {
          return PDF_ENOMEM;
        }

      /* Launch the conversion to UTF-16LE */
      SetLastError(0);
      if(MultiByteToWideChar(CodePage,           /* CodePage */
                             dwFlags,            /* dwFlags */
                             (char *)input_data, /* lpMultiByteStr */
                             input_length,       /* cbMultiByte */
                             (LPWSTR)temp_data,  /* lpWideCharStr */
                             output_nwchars) != output_nwchars) /* cchWideChar */
        {
          PDF_DEBUG_BASE("Problem performing the host encoding conversion");
          return PDF_ETEXTENC;
        }
      else
        {
          pdf_status_t ret_code;

          /* Finally, convert to UTF-32HE */
          ret_code = pdf_text_utf16le_to_utf32he(temp_data,
                                                 temp_size,
                                                 p_output_data,
                                                 p_output_length);

          pdf_dealloc(temp_data);
          return ret_code;
        }
    }
}

#else

static pdf_status_t
pdf_text_host_to_utf32he_iconv(const pdf_char_t      *input_data,
                               const pdf_size_t      input_length,
                               const pdf_text_host_encoding_t enc,
                               pdf_char_t            **p_output_data,
                               pdf_size_t            *p_output_length)
{
  iconv_t from_host;
  size_t n_conv;
  char *in_str;
  size_t n_in;
  size_t n_out;
  pdf_char_t *new_data;
  char *out_str;
  pdf_size_t worst_length;
  pdf_size_t new_length;

  /* Check if conversion is available. If we just specify "UTF-32" as the
   *  output encoding requested, iconv will insert the BOM by default, and
   *  we don't want it, so we specify directly the endianness required in the
   *  name of the encoding, depending on the host endianness */
  from_host = iconv_open((PDF_IS_BIG_ENDIAN ? "UTF-32BE" : "UTF-32LE"),
                          (char *)enc.name);
  if(from_host == (iconv_t)-1)
    {
      PDF_DEBUG_BASE("Conversion from '%s' to UTF-32 not available",
                     enc.name);
      return PDF_ETEXTENC;
    }

  /* Prepare lengths and locations.
   *  The worst length is computed as having 4 output bytes for each input
   *  single byte, taking into account that iconv adds an extra 32-bit NUL
   *  value (4 bytes equal to 0) at the end of the converted string. */
  worst_length = (input_length+1)*4;
  new_data = (pdf_char_t *)pdf_alloc(worst_length);
  if(new_data == NULL)
    {
      iconv_close(from_host);
      return PDF_ENOMEM;
    }
  n_out = worst_length;
  in_str = (char *)input_data;
  out_str = (char *)new_data;
  n_in = input_length;

  while(n_in > 0)
    {
      /* Convert */
      n_conv = iconv(from_host, &in_str, &n_in, &out_str, &n_out);

      /* Check conversion output status. We check errno to see if the problem
       *  is that more buffer is needed in the output. If this is the case,
       *  we just give a second try to the worst length and reallocate memory.
       *  There is no problem to use errno in multi-threaded applications
       *  if the library is compiled with -D_REENTRANT */
      if(n_conv == (size_t)-1)
        {
          if(errno == E2BIG)
            {
              pdf_size_t n_bytes_generated = 0;
              /* Compute the number of bytes actually generated in the
               *  output buffer. `n_out' stores the number of bytes still
               *  available in the output buffer. As the number of bytes
               *  allocated is multiple of four, and UTF-32 always uses 4
               *  bytes for each character, this value should always be 0 */
              n_bytes_generated = (pdf_size_t) (worst_length - n_out);

              /* We need more output buffer */
              worst_length += (n_in * 4);
              PDF_DEBUG_BASE("Reallocating to '%lu'. "
                             "'%lu' bytes are already generated",
                             (unsigned long) worst_length,
                             (unsigned long) n_bytes_generated);
              /* Reallocate buffer with greater size */
              new_data = (pdf_char_t *)pdf_realloc(new_data,worst_length);
              if(new_data == NULL)
                {
                  iconv_close(from_host);
                  return PDF_ENOMEM;
                }

              /* The re-allocated new data does not have to be in the same
               *  memory place as the original one, so the `out_str' pointer
               *  must be reset */
              out_str = (char *) &new_data[n_bytes_generated];

              /* The number of bytes available in the buffer must also be
               *  reset */
              n_out = (worst_length - n_bytes_generated);
            }
          else
            {
              iconv_close(from_host);
              pdf_dealloc(new_data);
              PDF_DEBUG_BASE("Invalid data to convert from Host Encoding:"
                             "'%s'",strerror(errno));
              return PDF_EBADTEXT;
            }
        }
    }

  /* Compute new final length */
  new_length = worst_length - n_out;

  /* Remove from the length the bytes related to the 4-byte NUL UTF-32 char */
  if((new_data[new_length-1] == '\0') && \
     (new_data[new_length-2] == '\0') && \
     (new_data[new_length-3] == '\0') && \
     (new_data[new_length-4] == '\0'))
    {
      new_length -= 4;
    }

  /* Finally, reset the buffer length to its correct size */
  if(new_length != worst_length)
    {
      new_data = (pdf_char_t *)pdf_realloc(new_data,new_length);
      if(new_data == NULL)
        {
          return PDF_ENOMEM;
        }
    }

  /* And set the output values within pdf_text_t */
  *p_output_data = new_data;
  *p_output_length = new_length;

  iconv_close(from_host);
  return PDF_OK;
}
#endif


pdf_status_t
pdf_text_host_to_utf32he(const pdf_char_t      *input_data,
                         const pdf_size_t      input_length,
                         const pdf_text_host_encoding_t enc,
                         pdf_char_t            **p_output_data,
                         pdf_size_t            *p_output_length)
{
#ifdef PDF_HOST_WIN32
  return pdf_text_host_to_utf32he_win32(input_data,
                                        input_length,
                                        enc,
                                        p_output_data,
                                        p_output_length);
#else
  return pdf_text_host_to_utf32he_iconv(input_data,
                                        input_length,
                                        enc,
                                        p_output_data,
                                        p_output_length);
#endif
}



/* End of pdf-text-host-encoding.c */

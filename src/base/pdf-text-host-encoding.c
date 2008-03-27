/* -*- mode: C -*- Time-stamp: ""
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

#if defined _WIN32 || defined __WIN32__
 #define WIN32_NATIVE
#endif


#ifdef WIN32_NATIVE
 #include <windows.h>
#else
 #include <iconv.h>
 #include <errno.h>
 #include <string.h>
#endif


#include <pdf-text.h>
#include <pdf-text-context.h>
#include <pdf-text-host-encoding.h>

/*
 * ICONV API
 * -----------
 * iconv_t iconv_open (const char* tocode, const char* fromcode)
 * size_t iconv (iconv_t cd,
 *               const char **inbuf, size_t *inbytesleft,
 *               char **outbuf, size_t *outbytesleft); 
 * int iconv_close (iconv_t cd);
 */


/* TODO: Windows host encoding support */
pdf_status_t
pdf_text_host_encoding_is_available(const pdf_char_t *encoding_name)
{
#ifdef WIN32_NATIVE
  {
    
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


/* TODO: Windows host encoding support */
pdf_status_t
pdf_text_utf32he_to_host(const pdf_char_t      *input_data,
                         const pdf_size_t      input_length,
                         const pdf_text_host_encoding_t enc,
                         pdf_char_t            **p_output_data,
                         pdf_size_t            *p_output_length)
{  
#ifdef WIN32_NATIVE
  {
    
  }
#else
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
                return PDF_EBADDATA;
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
  }
#endif
  
  return PDF_OK;
}


/* TODO: Windows host encoding support */
pdf_status_t
pdf_text_host_to_utf32he(const pdf_char_t      *input_data,
                         const pdf_size_t      input_length,
                         const pdf_text_host_encoding_t enc,
                         pdf_char_t            **p_output_data,
                         pdf_size_t            *p_output_length)
{
#ifdef WIN32_NATIVE
  {
    /* TODO */
  }
#else
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
                PDF_DEBUG_BASE("Invalid data to convert from Host Encoding:"
                               "'%s'",strerror(errno));
                return PDF_EBADDATA;
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
  }
#endif
  
  return PDF_OK;  
}



/* End of pdf-text-host-encoding.c */

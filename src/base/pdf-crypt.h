/* -*- mode: C -*-
 *
 *       File:         pdf-crypt.c
 *       Date:         Fri Feb 22 21:05:05 2008
 *
 *       GNU PDF Library - Header file for encryption module
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

#ifndef PDF_CRYPT_H
#define PDF_CRYPT_H

#include <config.h>

#include <string.h>

#include <pdf-types.h>
#include <pdf-error.h>
#include <pdf-alloc.h>
#include <pdf-crypt-c-aesv2.h>
#include <pdf-crypt-c-v2.h>

/* BEGIN PUBLIC */

enum pdf_crypt_md_algo_e
{
    PDF_CRYPT_MD_MD5
};

enum pdf_crypt_cipher_algo_e
{
  PDF_CRYPT_CIPHER_ALGO_AESV2,
  PDF_CRYPT_CIPHER_ALGO_V2
};

typedef struct pdf_crypt_cipher_s *pdf_crypt_cipher_t;
typedef struct pdf_crypt_md_s *pdf_crypt_md_t;

/* END PUBLIC */


struct pdf_crypt_cipher_algo_s
{
  pdf_status_t (*new) (void ** cipher);

  pdf_status_t (*setkey) (void * cipher, pdf_char_t *key, pdf_size_t size);

  pdf_status_t (*encrypt) (void * cipher,
			   pdf_char_t *out, pdf_size_t out_size,
			   pdf_char_t *in,  pdf_size_t in_size,
			   pdf_size_t *result_size);

  pdf_status_t (*decrypt) (void * cipher,
			   pdf_char_t *out, pdf_size_t out_size,
			   pdf_char_t *in,  pdf_size_t in_sizem,
			   pdf_size_t *result_size);

  pdf_status_t (*destroy) (void * cipher);
};

typedef struct pdf_crypt_cipher_algo_s *pdf_crypt_cipher_algo_t;

struct pdf_crypt_cipher_s
{
  pdf_crypt_cipher_algo_t algo;
  void * raw;
};

struct pdf_crypt_md_s
{
  void * raw;
};


#if !defined HAVE_INLINE && !defined COMPILING_PDF_CRYPT


pdf_status_t pdf_crypt_init (void);


/* BEGIN PUBLIC */

pdf_status_t pdf_crypt_nonce (pdf_char_t * buffer, pdf_size_t size);

pdf_status_t pdf_crypt_cipher_new (enum pdf_crypt_cipher_algo_e algorithm,
				   pdf_crypt_cipher_t *cipher);

pdf_status_t pdf_crypt_cipher_setkey (pdf_crypt_cipher_t cipher,
				      pdf_char_t *key,
				      pdf_size_t size);

pdf_status_t pdf_crypt_cipher_encrypt (pdf_crypt_cipher_t cipher,
				       pdf_char_t *out,
				       pdf_size_t out_size,
				       pdf_char_t *in,
				       pdf_size_t in_size,
				       pdf_size_t *result_size);

pdf_status_t pdf_crypt_cipher_decrypt (pdf_crypt_cipher_t cipher,
				       pdf_char_t *out,
				       pdf_size_t out_size,
				       pdf_char_t *in,
				       pdf_size_t in_size,
				       pdf_size_t *result_size);

pdf_status_t pdf_crypt_cipher_destroy (pdf_crypt_cipher_t cipher);


pdf_status_t pdf_crypt_md_new (enum pdf_crypt_md_algo_e algo, pdf_crypt_md_t *md);

pdf_status_t pdf_crypt_md_write (pdf_crypt_md_t md,
                                 pdf_char_t *in,  pdf_size_t in_size);

pdf_status_t pdf_crypt_md_read (pdf_crypt_md_t md,
				pdf_char_t *out, pdf_size_t out_size);


pdf_status_t pdf_crypt_md_destroy (pdf_crypt_md_t hd);



/* END PUBLIC */

#else

#if defined COMPILING_PDF_CRYPT
# define STATIC_INLINE
#else
# define STATIC_INLINE static inline
#endif /* COMPILING_PDF_CRYPT */


#include <gcrypt.h>


STATIC_INLINE pdf_status_t
pdf_crypt_init (void)
{
  gcry_check_version (GCRYPT_VERSION);
  return PDF_OK;
}


STATIC_INLINE pdf_status_t
pdf_crypt_nonce (pdf_char_t * buffer, pdf_size_t size)
{
  gcry_create_nonce (buffer, size);
  return PDF_OK;
}



STATIC_INLINE pdf_status_t
pdf_crypt_cipher_new (enum pdf_crypt_cipher_algo_e algorithm,
		      pdf_crypt_cipher_t *cipher)
{
  pdf_status_t status;
  pdf_crypt_cipher_algo_t cipher_algo;
  
  *cipher = pdf_alloc (sizeof(struct pdf_crypt_cipher_s));
  cipher_algo = pdf_alloc (sizeof(struct pdf_crypt_cipher_algo_s));

  switch (algorithm)
    {
    case PDF_CRYPT_CIPHER_ALGO_AESV2:
      {
        cipher_algo->new = pdf_crypt_cipher_aesv2_new;
        cipher_algo->setkey = pdf_crypt_cipher_aesv2_setkey;
        cipher_algo->encrypt = pdf_crypt_cipher_aesv2_encrypt;
        cipher_algo->decrypt = pdf_crypt_cipher_aesv2_decrypt;
        cipher_algo->destroy = pdf_crypt_cipher_aesv2_destroy;
        break;
      }
    case PDF_CRYPT_CIPHER_ALGO_V2:
      {
        cipher_algo->new = pdf_crypt_cipher_v2_new;
        cipher_algo->setkey = pdf_crypt_cipher_v2_setkey;
        cipher_algo->encrypt = pdf_crypt_cipher_v2_encrypt;
        cipher_algo->decrypt = pdf_crypt_cipher_v2_decrypt;
        cipher_algo->destroy = pdf_crypt_cipher_v2_destroy;
        break;
      }
    default:
      {
        /* Not reached, but makes stupid compilers happy */
        return PDF_ERROR;
        break;
      }
    }

  if (cipher_algo->new (&(*cipher)->raw) == PDF_OK)
    {
      (*cipher)->algo = cipher_algo;
      status	      = PDF_OK;
    }
  else
    status = PDF_ERROR;
  
  return status;
}


STATIC_INLINE pdf_status_t
pdf_crypt_cipher_setkey (pdf_crypt_cipher_t cipher,
			 pdf_char_t *key, pdf_size_t size)
{
  return cipher->algo->setkey (cipher->raw, key, size);
}


STATIC_INLINE pdf_status_t
pdf_crypt_cipher_encrypt (pdf_crypt_cipher_t cipher,
			  pdf_char_t *out, pdf_size_t out_size,
			  pdf_char_t *in,  pdf_size_t in_size,
			  pdf_size_t *result_size)
{
  return cipher->algo->encrypt (cipher->raw, out, out_size, in, in_size, result_size);
}


STATIC_INLINE pdf_status_t
pdf_crypt_cipher_decrypt (pdf_crypt_cipher_t cipher,
			  pdf_char_t *out, pdf_size_t out_size,
			  pdf_char_t *in,  pdf_size_t in_size,
			  pdf_size_t *result_size)
{
  return cipher->algo->decrypt (cipher->raw, out, out_size, in, in_size, result_size);
}


STATIC_INLINE pdf_status_t
pdf_crypt_cipher_destroy (pdf_crypt_cipher_t cipher)
{
  pdf_status_t ret;

  ret = cipher->algo->destroy (cipher->raw);
  pdf_dealloc (cipher->algo);
  pdf_dealloc (cipher);
  return ret;
}


/* Hashing functions */


STATIC_INLINE pdf_status_t
pdf_crypt_md_new (enum pdf_crypt_md_algo_e algo, pdf_crypt_md_t *_md)
{
  pdf_crypt_md_t md;
  gcry_md_hd_t * raw;
  pdf_status_t ret;
  
  md = pdf_alloc (sizeof(struct pdf_crypt_md_s));

  if (algo == PDF_CRYPT_MD_MD5)
    {
      raw = pdf_alloc (sizeof(gcry_md_hd_t));

      if (gcry_md_open (raw, GCRY_MD_MD5, 0) == GPG_ERR_NO_ERROR)
        {
          md->raw = raw;
          *_md = md;
          ret = PDF_OK;
        }
      else
        {
          gcry_md_close (*raw);
          ret = PDF_ERROR;
        }
    }
  else
    {
      ret = PDF_EBADDATA;
    }

  return ret;
}



STATIC_INLINE pdf_status_t
pdf_crypt_md_write (pdf_crypt_md_t md,
                    pdf_char_t *in,  pdf_size_t in_size)
{
  gcry_md_hd_t * gcry_md = md->raw;
  gcry_md_write (*gcry_md, in, in_size);
  return PDF_OK;
}


STATIC_INLINE pdf_status_t
pdf_crypt_md_read (pdf_crypt_md_t md,
		   pdf_char_t *out, pdf_size_t out_size)
{
  gcry_md_hd_t * gcry_md   = md->raw;
  pdf_size_t required_size = gcry_md_get_algo_dlen (GCRY_MD_MD5);
  
  if (out_size < required_size)
    return PDF_EBADDATA;
  
  if (gcry_md_final (*gcry_md) != GPG_ERR_NO_ERROR)
    {
      return PDF_ERROR;
    }
  else
    {
      void * hash;

      hash = gcry_md_read  (*gcry_md, GCRY_MD_MD5);

      if (hash == NULL)
        return PDF_ERROR;
      else
        {
          memcpy (out, hash, required_size);
        }

      gcry_md_reset (*gcry_md);
      return PDF_OK;
    }
}



STATIC_INLINE pdf_status_t
pdf_crypt_md_destroy (pdf_crypt_md_t md)
{
  pdf_dealloc (md->raw);
  pdf_dealloc (md);
  return PDF_OK;
}



#endif /* COMPILING_PDF_CRYPT */
#endif /* PDF_CRYPT_H */

/* End of pdf-crypt.h */

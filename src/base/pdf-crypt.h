/* -*- mode: C -*- Time-stamp: "08/09/19 00:55:33 jemarch"
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
#include <pdf-types.h>
#include <pdf-error.h>
#include <pdf-alloc.h>
#include <pdf-crypt-c-aesv2.h>
#include <pdf-crypt-c-v2.h>


/* BEGIN PUBLIC */

struct pdf_crypt_cipher_algo_s
{
  pdf_status_t (*new) (void ** cipher);

  pdf_status_t (*setkey) (void * cipher, pdf_char_t *key, pdf_size_t size);

  pdf_size_t (*encrypt_size) (void * cipher, pdf_char_t *in, pdf_size_t in_size);

  pdf_size_t (*decrypt_size) (void * cipher, pdf_char_t *in, pdf_size_t in_size);

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

typedef struct pdf_crypt_cipher_algo_s pdf_crypt_cipher_algo_t;


struct pdf_crypt_cipher_s
{
  pdf_crypt_cipher_algo_t * algo;
  pdf_char_t * key;
  pdf_size_t key_size;
  void * raw;
};

typedef struct pdf_crypt_cipher_s pdf_crypt_cipher_t;


enum pdf_crypt_md_algo_e
{
    PDF_CRYPT_MD_MD5
};

typedef enum pdf_crypt_md_algo_e pdf_crypt_md_algo_t;



struct pdf_crypt_md_s
{
  void * raw;
};

typedef struct pdf_crypt_md_s pdf_crypt_md_t;



/* END PUBLIC */



#if !defined (HAVE_INLINE) && !defined (COMPILING_PDF_CRYPT)


/* BEGIN PUBLIC */

pdf_status_t pdf_crypt_init (void);

pdf_status_t pdf_crypt_cipher_new (pdf_crypt_cipher_algo_t * algorithm,
				   pdf_crypt_cipher_t *cipher);

pdf_status_t pdf_crypt_cipher_setkey (pdf_crypt_cipher_t *cipher,
				      pdf_char_t *key,
				      pdf_size_t size);

pdf_size_t pdf_crypt_cipher_encrypt_size (pdf_crypt_cipher_t cipher,
					  pdf_char_t *in, pdf_size_t in_size);

pdf_size_t pdf_crypt_cipher_decrypt_size (pdf_crypt_cipher_t cipher,
					  pdf_char_t *in, pdf_size_t in_size);

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


pdf_status_t pdf_crypt_md_new (pdf_crypt_md_t *md, pdf_crypt_md_algo_t algo);

pdf_status_t pdf_crypt_md_hash (pdf_crypt_md_t md,
				pdf_char_t *out, pdf_size_t out_size,
				pdf_char_t *in,  pdf_size_t in_size);

pdf_status_t pdf_crypt_md_destroy (pdf_crypt_md_t hd);



/* END PUBLIC */

#else


#if __GNUC__ == 4 && __GNUC_MINOR__ >= 3 && defined(__GNUC_STDC_INLINE__)
#  define FUCKING_C99_SEMANTICS
#endif

/* Apple's gcc build > 5400 (since Xcode 3.0) doesn't support GNU
   inline in C99 mode */
#if __APPLE_CC__ > 5400 && ! defined(FUCKING_C99_SEMANTICS) && __STDC_VERSION__ >= 199901L
#  define FUCKING_C99_SEMANTICS
#endif

#if defined(COMPILING_PDF_CRYPT)
#  if defined(FUCKING_C99_SEMANTICS)
/* force exported copy */
#    define EXTERN_INLINE extern inline
#  else
#    define EXTERN_INLINE
#  endif
#else
/* For gcc >= 4.1 not working in C99 inline semantics by default
   (including the annoying 4.2 warnings about the extern inline
   hack) */
#  if defined(__GNUC_STDC_INLINE__) || defined(__GNUC_GNU_INLINE__)
#    define EXTERN_INLINE  static inline
#  else
#    define EXTERN_INLINE extern inline
#  endif
/* For gcc >= 4.3 with C99 semantics */
#  if defined(FUCKING_C99_SEMANTICS)
/* either inline or link to extern version at compiler's choice */
#    undef EXTERN_INLINE
#    define EXTERN_INLINE inline
#  endif
#endif /* COMPILING_PDF_CRYPT */


#include <gcrypt.h>


EXTERN_INLINE pdf_status_t
pdf_crypt_init (void)
{
  gcry_check_version (GCRYPT_VERSION);
  return PDF_OK;
}


EXTERN_INLINE pdf_status_t
pdf_crypt_cipher_new (pdf_crypt_cipher_algo_t *algorithm,
		      pdf_crypt_cipher_t *cipher)
{
  pdf_status_t status;
  
  if (algorithm->new (&cipher->raw) == PDF_OK)
    {
      cipher->algo     = algorithm;
      cipher->key      = NULL;
      cipher->key_size = 0;
      status	       = PDF_OK;
    }
  else
    status = PDF_ERROR;
  
  return status;
}


EXTERN_INLINE pdf_status_t
pdf_crypt_cipher_setkey (pdf_crypt_cipher_t * cipher,
			 pdf_char_t *key, pdf_size_t size)
{
  pdf_status_t status;

  status = cipher->algo->setkey (cipher->raw, key, size);

  if (status == PDF_OK)
    {
      cipher->key = pdf_alloc (size);
      cipher->key_size = size;
      memcpy (cipher->key, key, size);
    }

  return status;
}


EXTERN_INLINE pdf_size_t
pdf_crypt_cipher_encrypt_size (pdf_crypt_cipher_t cipher,
			       pdf_char_t *in, pdf_size_t in_size)
{
  return cipher.algo->encrypt_size (cipher.raw, in, in_size);
}


EXTERN_INLINE pdf_size_t
pdf_crypt_cipher_decrypt_size (pdf_crypt_cipher_t cipher,
			       pdf_char_t *in, pdf_size_t in_size)
{
  return cipher.algo->decrypt_size (cipher.raw, in, in_size);
}


EXTERN_INLINE pdf_status_t
pdf_crypt_cipher_encrypt (pdf_crypt_cipher_t cipher,
			  pdf_char_t *out, pdf_size_t out_size,
			  pdf_char_t *in,  pdf_size_t in_size,
			  pdf_size_t *result_size)
{
  cipher.algo->setkey (cipher.raw, cipher.key, cipher.key_size);
  return cipher.algo->encrypt (cipher.raw, out, out_size, in, in_size, result_size);
}


EXTERN_INLINE pdf_status_t
pdf_crypt_cipher_decrypt (pdf_crypt_cipher_t cipher,
			  pdf_char_t *out, pdf_size_t out_size,
			  pdf_char_t *in,  pdf_size_t in_size,
			  pdf_size_t *result_size)
{
  cipher.algo->setkey (cipher.raw, cipher.key, cipher.key_size);
  return cipher.algo->decrypt (cipher.raw, out, out_size, in, in_size, result_size);
}


EXTERN_INLINE pdf_status_t
pdf_crypt_cipher_destroy (pdf_crypt_cipher_t cipher)
{
  pdf_dealloc (cipher.key);
  return cipher.algo->destroy (cipher.raw);
}



/* Hashing functions */


EXTERN_INLINE pdf_status_t
pdf_crypt_md_new (pdf_crypt_md_t *md, pdf_crypt_md_algo_t algo)
{
  if (algo == PDF_CRYPT_MD_MD5)
    {
      md->raw = pdf_alloc (sizeof(gcry_md_hd_t));

      if (gcry_md_open (md->raw, GCRY_MD_MD5, 0) == GPG_ERR_NO_ERROR)
	{
	  return PDF_OK;
	}
      else
	{
	  pdf_dealloc (md->raw);
	  return PDF_ERROR;
	}
    }
  else
    {
      return PDF_EBADDATA;
    }
}



EXTERN_INLINE pdf_status_t
pdf_crypt_md_hash (pdf_crypt_md_t md,
		   pdf_char_t *out, pdf_size_t out_size,
		   pdf_char_t *in,  pdf_size_t in_size)
{
  gcry_md_hd_t * gcry_md = md.raw;
  register pdf_size_t i;
  
  for (i = 0; i < in_size; i++)
    {
      gcry_md_putc (*gcry_md, in[i]);
    }

  if (gcry_md_final (*gcry_md) != GPG_ERR_NO_ERROR)
    {
      return PDF_ERROR;
    }
  else
    {
      gcry_md_read  (*gcry_md, GCRY_MD_MD5);
      gcry_md_close (*gcry_md);
      return PDF_OK;
    }
}


EXTERN_INLINE pdf_status_t
pdf_crypt_md_destroy (pdf_crypt_md_t md)
{
  pdf_dealloc (md.raw);
  return PDF_OK;
}



#endif /* COMPILING_PDF_CRYPT */
#endif /* PDF_CRYPT_H */

/* End of pdf-crypt.h */

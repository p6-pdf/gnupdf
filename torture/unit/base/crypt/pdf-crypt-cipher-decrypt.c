/* -*- mode: C -*- Time-stamp: "2008-09-09 04:13:42 david"
 *
 *       File:         pdf-crypt-cipher-decrypt.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_crypt_cipher_decrypt
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

#include <stdbool.h>
#include <stdio.h>
#include <pdf.h>
#include <check.h>


/*
 * Test: pdf_crypt_cipher_decrypt_001
 * Description:
 *   Decrypt an ciphered empty buffer (AESV2).
 * Success condition:
 *   Returns 0.
 */

START_TEST (pdf_crypt_cipher_decrypt_001)
{
  pdf_crypt_cipher_t cipher;
  pdf_char_t *plain;
  pdf_size_t plain_size;

  pdf_char_t ciphered[] = {
    0xa6, 0x14, 0x2e, 0x8a,
    0x51, 0x84, 0x84, 0x2a,
    0xf0, 0x4d, 0xc8, 0x37,
    0x23, 0x8b, 0xc6, 0xf0,
    0x3a, 0x30, 0x0f, 0xcc,
    0xb1, 0x04, 0x05, 0x47,
    0x99, 0xf4, 0x0e, 0xed,
    0x6f, 0xdf, 0xc5, 0x20
  };
  
  pdf_char_t key[] = 
    {
      0x29, 0xfd, 0x08, 0x08,
      0x05, 0x00, 0x00, 0x00,
      0x18, 0x92, 0xe7, 0xbf,
      0xab, 0x17, 0x08, 0x08
    };
			     
  pdf_crypt_cipher_new (PDF_CRYPT_CIPHER_ALGO_AESV2, &cipher);
  pdf_crypt_cipher_setkey (&cipher, key, sizeof(key));

  plain_size = pdf_crypt_cipher_decrypt_size (cipher, ciphered, sizeof(ciphered));
  plain = pdf_alloc (plain_size);
  
  fail_if (pdf_crypt_cipher_decrypt (cipher,
				     plain, plain_size,
				     ciphered, sizeof(ciphered),
				     &plain_size) != PDF_OK);
  fail_if (plain_size != 0);

  pdf_dealloc (plain);
  pdf_crypt_cipher_destroy (cipher);
}
END_TEST



/*
 * Test: pdf_crypt_cipher_decrypt_002
 * Description:
 *   Decrypt an ciphered empty buffer (V2).
 * Success condition:
 *   Returns 0.
 */

START_TEST (pdf_crypt_cipher_decrypt_002)
{
  pdf_crypt_cipher_t cipher;
  pdf_char_t *plain;
  pdf_size_t plain_size;

  pdf_char_t * ciphered;
  pdf_size_t   ciphered_size;
			     
  ciphered_size = 0;

  pdf_crypt_cipher_new (PDF_CRYPT_CIPHER_ALGO_V2, &cipher);
  pdf_crypt_cipher_setkey (&cipher, NULL, 0);

  plain_size = pdf_crypt_cipher_decrypt_size (cipher, ciphered, ciphered_size);
  plain = pdf_alloc (plain_size);
  
  fail_if (pdf_crypt_cipher_decrypt (cipher,
				     plain, plain_size,
				     ciphered, ciphered_size,
				     &plain_size) != PDF_OK);
  fail_if (plain_size != 0);

  pdf_dealloc (plain);
  pdf_crypt_cipher_destroy (cipher);
}
END_TEST




/*
 * Test case creation function
 */
TCase *
test_pdf_crypt_cipher_decrypt (void)
{
  TCase *tc = tcase_create("pdf_crypt_cipher_decrypt");
  tcase_add_test(tc, pdf_crypt_cipher_decrypt_001);
  tcase_add_test(tc, pdf_crypt_cipher_decrypt_002);
  return tc;
}


/* End of pdf-crypt-cipher-decrypt.c */


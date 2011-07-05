/* -*- mode: C -*-
 *
 *       File:         pdf-crypt-cipher-encrypt.c
 *       Date:         Wed Mar  12 12:43:00 2008
 *
 *       GNU PDF Library - Unit tests for pdf_crypt_cipher_encrypt
 *
 */

/* Copyright (C) 2008-2011 Free Software Foundation, Inc. */

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

#include <pdf-test-common.h>

/*
 * Test: pdf_crypt_cipher_encrypt_001
 * Description:
 *   Encrypt a buffer (V2).
 * Success condition:
 *   Encrypted data should be correct.
 */
START_TEST (pdf_crypt_cipher_encrypt_001)
{
  pdf_crypt_cipher_t *cipher;
  pdf_char_t out[14];
  pdf_size_t out_size;
  pdf_char_t in[14] = "Attack at dawn"; /* not trailing '\0' */
  pdf_size_t in_size;
  pdf_char_t key[6] = "Secret"; /* not trailing '\0' */
  pdf_error_t *error = NULL;

  pdf_char_t ciphered[] =
    {
      0x45, 0xA0, 0x1F, 0x64, 0x5F, 0xC3, 0x5B,
      0x38, 0x35, 0x52, 0x54, 0x4B, 0x9B, 0xF5
    };

  in_size  = sizeof (in);
  out_size = in_size;

  cipher = pdf_crypt_cipher_new (PDF_CRYPT_CIPHER_ALGO_V2, &error);
  fail_unless (cipher != NULL);
  fail_if (error != NULL);

  fail_unless (pdf_crypt_cipher_set_key (cipher,
                                         key,
                                         sizeof (key),
                                         &error) == PDF_TRUE);
  fail_if (error != NULL);

  fail_unless (pdf_crypt_cipher_encrypt (cipher,
                                         out,
                                         out_size,
                                         in,
                                         in_size,
                                         &out_size,
                                         &error) == PDF_TRUE);
  fail_if (error != NULL);
  fail_unless (memcmp (out, ciphered, out_size) == 0);

  pdf_crypt_cipher_destroy (cipher);
}
END_TEST


/*
 * Test: pdf_crypt_cipher_encrypt_002
 * Description:
 *   Encrypt an ciphered buffer (AEV2).
 * Success condition:
 *   The ouput data should be correct.
 */

START_TEST (pdf_crypt_cipher_encrypt_002)
{
  pdf_crypt_cipher_t *cipher;
  pdf_error_t *error = NULL;
  pdf_char_t out[80];

  pdf_char_t key[16] =
    {
      0x56, 0xe4, 0x7a, 0x38,
      0xc5, 0x59, 0x89, 0x74,
      0xbc, 0x46, 0x90, 0x3d,
      0xba, 0x29, 0x03, 0x49
    };

  pdf_char_t plain[] =
    {
      0x8c, 0xe8, 0x2e, 0xef, 0xbe, 0xa0, 0xda, 0x3c, /* iv vector */
      0x44, 0x69, 0x9e, 0xd7, 0xdb, 0x51, 0xb7, 0xd9,

      0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
      0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
      0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
      0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
      0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
      0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
      0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
      0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf
    };

  pdf_char_t ciphered[] =
    {
      0x8c, 0xe8, 0x2e, 0xef, 0xbe, 0xa0, 0xda, 0x3c, /* iv vector */
      0x44, 0x69, 0x9e, 0xd7, 0xdb, 0x51, 0xb7, 0xd9,

      0xc3, 0x0e, 0x32, 0xff, 0xed, 0xc0, 0x77, 0x4e,
      0x6a, 0xff, 0x6a, 0xf0, 0x86, 0x9f, 0x71, 0xaa,
      0x0f, 0x3a, 0xf0, 0x7a, 0x9a, 0x31, 0xa9, 0xc6,
      0x84, 0xdb, 0x20, 0x7e, 0xb0, 0xef, 0x8e, 0x4e,
      0x35, 0x90, 0x7a, 0xa6, 0x32, 0xc3, 0xff, 0xdf,
      0x86, 0x8b, 0xb7, 0xb2, 0x9d, 0x3d, 0x46, 0xad,
      0x83, 0xce, 0x9f, 0x9a, 0x10, 0x2e, 0xe9, 0x9d,
      0x49, 0xa5, 0x3e, 0x87, 0xf4, 0xc3, 0xda, 0x55,
    };

  cipher = pdf_crypt_cipher_new (PDF_CRYPT_CIPHER_ALGO_AESV2, &error);
  fail_unless (cipher != NULL);
  fail_if (error != NULL);

  fail_unless (pdf_crypt_cipher_set_key (cipher,
                                         key,
                                         sizeof (key),
                                         &error) == PDF_TRUE);
  fail_if (error != NULL);

  fail_unless (pdf_crypt_cipher_encrypt (cipher,
                                         out,
                                         sizeof (out),
                                         plain,
                                         sizeof (plain),
                                         NULL,
                                         &error) == PDF_TRUE);
  fail_if (error != NULL);

#if 0
  {
    pdf_char_t *out_hex;
    pdf_char_t *ciphered_hex;

    out_hex = pdf_test_get_hex (out, sizeof (out), ':');
    ciphered_hex = pdf_test_get_hex (ciphered, sizeof (out), ':');

    printf ("pdf_crypt_cipher_encrypt_002;\n"
            "\tout:  '%s'\n"
            "\tciph: '%s'\n",
            out_hex,
            ciphered_hex);
    pdf_dealloc (out_hex);
    pdf_dealloc (ciphered_hex);
  }
#endif

  fail_unless (memcmp (out, ciphered, sizeof (out)) == 0);

  pdf_crypt_cipher_destroy (cipher);
}
END_TEST

/*
 * Test: pdf_crypt_cipher_encrypt_003
 * Description:
 *   Encrypt an ciphered buffer incrementally (AEV2).
 * Success condition:
 *   The ouput data should be correct.
 */

START_TEST (pdf_crypt_cipher_encrypt_003)
{
  pdf_crypt_cipher_t *cipher;
  pdf_error_t *error = NULL;
  pdf_char_t out[80];
  pdf_u32_t i;

  pdf_char_t key[16] =
    {
      0x56, 0xe4, 0x7a, 0x38,
      0xc5, 0x59, 0x89, 0x74,
      0xbc, 0x46, 0x90, 0x3d,
      0xba, 0x29, 0x03, 0x49
    };

  pdf_char_t plain[] =
    {
      0x8c, 0xe8, 0x2e, 0xef, 0xbe, 0xa0, 0xda, 0x3c, /* iv vector */
      0x44, 0x69, 0x9e, 0xd7, 0xdb, 0x51, 0xb7, 0xd9,

      0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
      0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
      0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
      0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
      0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
      0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
      0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
      0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf
    };

  pdf_char_t ciphered[] =
    {
      0x8c, 0xe8, 0x2e, 0xef, 0xbe, 0xa0, 0xda, 0x3c, /* iv vector */
      0x44, 0x69, 0x9e, 0xd7, 0xdb, 0x51, 0xb7, 0xd9,

      0xc3, 0x0e, 0x32, 0xff, 0xed, 0xc0, 0x77, 0x4e,
      0x6a, 0xff, 0x6a, 0xf0, 0x86, 0x9f, 0x71, 0xaa,
      0x0f, 0x3a, 0xf0, 0x7a, 0x9a, 0x31, 0xa9, 0xc6,
      0x84, 0xdb, 0x20, 0x7e, 0xb0, 0xef, 0x8e, 0x4e,
      0x35, 0x90, 0x7a, 0xa6, 0x32, 0xc3, 0xff, 0xdf,
      0x86, 0x8b, 0xb7, 0xb2, 0x9d, 0x3d, 0x46, 0xad,
      0x83, 0xce, 0x9f, 0x9a, 0x10, 0x2e, 0xe9, 0x9d,
      0x49, 0xa5, 0x3e, 0x87, 0xf4, 0xc3, 0xda, 0x55,
    };

  cipher = pdf_crypt_cipher_new (PDF_CRYPT_CIPHER_ALGO_AESV2, &error);
  fail_unless (cipher != NULL);
  fail_if (error != NULL);

  fail_unless (pdf_crypt_cipher_set_key (cipher,
                                         key,
                                         sizeof (key),
                                         &error) == PDF_TRUE);
  fail_if (error != NULL);


  for (i = 0; i < 80; i += 16)
    {
      fail_unless (pdf_crypt_cipher_encrypt (cipher,
                                             out + i,
                                             16,
                                             plain + i,
                                             16,
                                             NULL,
                                             &error) == PDF_TRUE);
      fail_if (error != NULL);
    }

  fail_unless (memcmp (out, ciphered, sizeof (out)) == 0);

  pdf_crypt_cipher_destroy (cipher);
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_crypt_cipher_encrypt (void)
{
  TCase *tc = tcase_create ("pdf_crypt_cipher_encrypt");

  tcase_add_test (tc, pdf_crypt_cipher_encrypt_001);
  tcase_add_test (tc, pdf_crypt_cipher_encrypt_002);
  tcase_add_test (tc, pdf_crypt_cipher_encrypt_003);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-crypt-cipher-encrypt.c */

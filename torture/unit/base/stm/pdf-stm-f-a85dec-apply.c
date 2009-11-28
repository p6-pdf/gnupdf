/* -*- mode: C -*- Time-stamp: "2009-08-05 21:58:15 davazp"
 *
 *       File:         pdf-stm-f-a85dec-apply.c
 *       Date:         Sat Sep 20 15:20:17 2008
 *
 *       GNU PDF Library - Unit tests for pdf_stm_f_a85dec_apply
 *
 */

/* Copyright (C) 2009 Free Software Foundation, Inc. */

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

#include <string.h>
#include <check.h>
#include <pdf.h>


/*
 * The following is the spec from the gnupdf site for the ASCII85 filter
 * I have placed within this text requirement identifiers in the form **(#n)**
 * to identify requirements coverage in the test cases.
 *  
 * From gnupdf.org:
 *
 * The ASCII85Decode filter decodes data that has been encoded in ASCII 
 * base-85 encoding and produces binary data. The following paragraphs 
 * describe the process for encoding binary data in ASCII base-85; the 
 * ASCII85Decode filter reverses this process.
 *
 * The ASCII base-85 encoding uses the characters ! through u and the 
 * character z, with the 2-character sequence ~> as its EOD marker. The
 * ASCII85Decode filter ignores all white-space characters **(#1)**. Any other 
 * characters **(#2)**, and any character sequences that represent impossible 
 * combinations in the ASCII base-85 encoding **(#3)**, cause an error.
 *
 * Specifically, ASCII base-85 encoding produces 5 ASCII characters for
 * every 4 bytes of binary data. Each group of 4binary input bytes, 
 * `(b1 b2 b3 b4)`, is converted to a group of 5 output bytes, 
 * `(c1 c2 c3 c4 c5)`, using the relation:
 *
 * (b1 * 256^3) + (b2 * 256^2) + (b3 * 256^1) + b4 =
 *
 * (c1 * 85^4) + (c2 * 85^3) + (c3 * 85^2) + (c4 * 85^1) + c5
 *
 * In other words, 4 bytes of binary data are interpreted as a base-256
 * number and then converted to a base-85 number. The five bytes of the
 * base-85 number are then converted to ASCII characters by adding 33 
 * (the ASCII code for the character !) to each. The resulting encoded 
 * data contains only printable ASCII characters with codes in the 
 * range 33 (!) to 117 (u). As a special case, if all five bytes are 0,
 * they are represented by the character with code 122 (z) **(#4)** instead of 
 * by five exclamation points (!!!!!).
 *
 * If the length of the binary data to be encoded is not a multiple of 
 * 4 bytes, the last, partial group of 4 is used to produce a last, 
 * partial group of 5 output  characters. Given n (1, 2 or 3) bytes of 
 * binary data, the encoder first appends 4 - n zero bytes to make a 
 * complete group of 4. It then encodes this group in the usual way, 
 * but without applying the special z case **(#5)**. Finally, it writes only the
 * first n + 1 characters of the resulting group of 5 **(#6)**. These characters
 * are immediately followed by the ~> EOD marker.
 *
 * The following conditions (which never occur in a correctly encoded 
 * byte sequence) cause errors during decoding:
 *
 *  * The value represented by a group of 5 characters is >= 2^32 **(#7)**.
 *  * A z character occurs in the middle of a group. **(#8)**
 *  * A final partial group contains only one character. (**(#9)**
 *
 */

/*
 * Test: pdf_stm_f_a85dec_apply_001
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition: Requirement **(#4)**
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_f_a85dec_apply_001)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded = "\x00\x00\x00\x00";
  pdf_char_t *encoded = "z~>";


  /* Create the buffers */
  buf_size = 20;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 3, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 4, &read_bytes);
  fail_if (read_bytes != 4);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 4) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_002
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it - test requirement **(#1)** - ignore whitespace.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_f_a85dec_apply_002)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded = "\x00\x00\x00\x00";
  pdf_char_t *encoded = " z\t~\n>       ";


  /* Create the buffers */
  buf_size = 20;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 13, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 4, &read_bytes);
  fail_if (read_bytes != 4);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 4) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_003
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition: Requirement **(#6)**
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_f_a85dec_apply_003)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded = "\x00";
  pdf_char_t *encoded = "!!~>";


  /* Create the buffers */
  buf_size = 20;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 4, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 1, &read_bytes);
  fail_if (read_bytes != 1);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 1) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_004
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_f_a85dec_apply_004)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded = "\x00\x00";
  pdf_char_t *encoded = "!!!~>";


  /* Create the buffers */
  buf_size = 20;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 5, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 2, &read_bytes);
  fail_if (read_bytes != 2);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 2) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_005
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it. Requirement **(#5)** **(#6)**
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_f_a85dec_apply_005)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded = "\x00\x00\x00";
  pdf_char_t *encoded = "!!!!~>";


  /* Create the buffers */
  buf_size = 20;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 6, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 3, &read_bytes);
  fail_if (read_bytes != 3);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 3) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_006
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_f_a85dec_apply_006)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47~>";


  /* Create the buffers */
  buf_size = 20;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);
  fail_if (read_bytes != 16);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 16) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_007
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_f_a85dec_apply_007)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47\x21\x2D~>";


  /* Create the buffers */
  buf_size = 30;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 20, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 17, &read_bytes);
  fail_if (read_bytes != 17);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 17) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_008
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_f_a85dec_apply_008)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "\t!!\x2A\x2D\x27z\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47~>\t\n\n\n";


  /* Create the buffers */
  buf_size = 30;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 23, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);
  fail_if (read_bytes != 16);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 16) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_009
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_f_a85dec_apply_009)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x22\x39\n\n\n\x65\x75\x37\x23\x52\x4C\x68\x47\n~\n>";


  /* Create the buffers */
  buf_size = 30;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 23, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);
  fail_if (read_bytes != 16);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 16) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_010
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_f_a85dec_apply_010)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47\x21\n\x2D\n~\n>";


  /* Create the buffers */
  buf_size = 30;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 23, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 17, &read_bytes);
  fail_if (read_bytes != 17);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 17) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_011
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * Success condition:
 *   The decoded data should be correct.
 */
START_TEST (pdf_stm_f_a85dec_apply_011)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47\x21\x45\x3A~>";


  /* Create the buffers */
  buf_size = 30;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 21, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 18, &read_bytes);
  fail_if (read_bytes != 18);

  /* Close the stream */
  pdf_stm_destroy (stm);

  /* Check the result of the operation */
  fail_if (memcmp (ret_buf, unencoded, 18) != 0);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_012
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * **FAIL** condition: Requirement **(#8)**
 *   The decoding should fail.
 */
START_TEST (pdf_stm_f_a85dec_apply_012)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2Dz\x27\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47~>";
  /* Note that ^ this z is in an invalid position */

  /* Create the buffers */
  buf_size = 20;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_013
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * **FAIL** condition:
 *   The decoding should fail.
 */
START_TEST (pdf_stm_f_a85dec_apply_013)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2Az\x2D\x27\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47~>";
  /* Note  ^ this z is in an invalid position */

  /* Create the buffers */
  buf_size = 20;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_014
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * **FAIL** condition:
 *   The decoding should fail.
 */
START_TEST (pdf_stm_f_a85dec_apply_014)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!z\x2A\x2D\x27\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47~>";
  /*   ^ Note: preceeding z is in an invalid position */

  /* Create the buffers */
  buf_size = 20;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_015
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * **FAIL** condition:
 *   The decoding should fail.
 */
START_TEST (pdf_stm_f_a85dec_apply_015)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!z!\x2A\x2D\x27\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47~>";
  /*  ^ Note that preceeding z is in an invalid position */

  /* Create the buffers */
  buf_size = 20;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_016
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it. Requirement **(#9)**
 * **FAIL** condition:
 *   The decoding should fail.
 */
START_TEST (pdf_stm_f_a85dec_apply_016)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x22\x39\x65\x75\x37\x23\x52\x4C\x68\x47\x48~>";
  /* Note that terminating group is only one byte */

  /* Create the buffers */
  buf_size = 20;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_017
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it. Requirement **(#7)**
 * **FAIL** condition:
 *   The decoding should fail.
 */
START_TEST (pdf_stm_f_a85dec_apply_017)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x73\x75\x75\x75\x75\x23\x52\x4C\x68\x47~>";
  /*  Note that Third group is above 2^32 limit */

  /* Create the buffers */
  buf_size = 20;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_018
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * **FAIL** condition:
 *   The decoding should fail.
 */
START_TEST (pdf_stm_f_a85dec_apply_018)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x74\x21\x21\x21\x21\x23\x52\x4C\x68\x47~>";
  /*  Note that Thirnd group is above 2^32 limit */

  /* Create the buffers */
  buf_size = 20;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_019
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it. Requirement **(#7)**
 * **FAIL** condition:
 *   The decoding should fail.
 */
START_TEST (pdf_stm_f_a85dec_apply_019)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x73\x38\x57\x2D\x22\x23\x52\x4C\x68\x47~>";
  /*  Note that Third group equals 2^32 limit */

  /* Create the buffers */
  buf_size = 20;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_020
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * **FAIL** condition:
 *   The decoding should fail.
 */
START_TEST (pdf_stm_f_a85dec_apply_020)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x73\x38\x57\x2D\x23\x23\x52\x4C\x68\x47~>";
  /*  Note that Third group is above 2^32 limit */

  /* Create the buffers */
  buf_size = 20;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_021
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * **FAIL** condition:
 *   The decoding should fail.
 */
START_TEST (pdf_stm_f_a85dec_apply_021)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x73\x38\x57\x2E\x21\x23\x52\x4C\x68\x47~>";
  /*  Note that Third group is above 2^32 limit */

  /* Create the buffers */
  buf_size = 20;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_022
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * **FAIL** condition:
 *   The decoding should fail.
 */
START_TEST (pdf_stm_f_a85dec_apply_022)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x73\x38\x58\x2D\x21\x23\x52\x4C\x68\x47~>";
  /*  Note that Third group is above 2^32 limit */

  /* Create the buffers */
  buf_size = 20;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_023
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * **FAIL** condition:
 *   The decoding should fail.
 */
START_TEST (pdf_stm_f_a85dec_apply_023)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x73\x39\x57\x2D\x21\x23\x52\x4C\x68\x47~>";
  /*  Note that Third group is above 2^32 limit */

  /* Create the buffers */
  buf_size = 20;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_024
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * **FAIL** condition: Requirement **(#2)** - invalid character
 *   The decoding should fail.
 */
START_TEST (pdf_stm_f_a85dec_apply_024)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x63\x39\x57\x2D\x21\x23\x52\x4C\x68\x76~>";
  /*  Note that Last character is out of allowed range */

  /* Create the buffers */
  buf_size = 20;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test: pdf_stm_f_a85dec_apply_025
 * Description:
 *   Create a memory-based writing stream and attach an A85 filter
 *   decoder to it.
 * **FAIL** condition: Requirement **(#3)** - invalid character
 *   The decoding should fail.
 */
START_TEST (pdf_stm_f_a85dec_apply_025)
{
  pdf_status_t ret;
  pdf_stm_t stm;
  pdf_char_t *ret_buf;
  pdf_size_t buf_size;
  pdf_size_t read_bytes;
  pdf_hash_t a85dec_filter_parameters;

  pdf_char_t *unencoded =
    "\x00\x01\x02\x03\x00\x00\x00\x00\x04\x05\x06\x07\x08\x09\x0A\x0B";
  pdf_char_t *encoded =
    "!!\x2A\x2D\x27z\x63\x39\x57\x2D\x21\x23\x52\x4C\x68\x00~>";
  /*  Note that Last character is out of allowed range */

  /* Create the buffers */
  buf_size = 20;
  pdf_init ();

  ret_buf = pdf_alloc (buf_size);
  fail_if (ret_buf == NULL);


  /* Create the stream */
  ret = pdf_stm_mem_new (encoded, 18, 0,	/* Use the default cache size */
			 PDF_STM_READ, &stm);
  fail_if (ret != PDF_OK);

  /* Install an A85 decoder in the stream */
  ret = pdf_hash_new (NULL, &a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  ret = pdf_stm_install_filter (stm,
				PDF_STM_FILTER_A85_DEC,
				a85dec_filter_parameters);
  fail_if (ret != PDF_OK);

  /* Read some data from the stream */
  ret = pdf_stm_read (stm, ret_buf, 16, &read_bytes);

  fail_if (ret != PDF_ERROR);

  /* Close the stream */
  pdf_stm_destroy (stm);


  /* Destroy data */
  pdf_hash_destroy (a85dec_filter_parameters);
  pdf_dealloc (ret_buf);

}
END_TEST


/*
 * Test case creation function
 */
  TCase * test_pdf_stm_f_a85dec_apply (void)
{
  TCase *tc = tcase_create ("pdf_stm_f_a85dec_apply");

  tcase_add_test (tc, pdf_stm_f_a85dec_apply_001);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_002);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_003);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_004);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_005);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_006);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_007);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_008);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_009);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_010);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_011);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_012);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_013);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_014);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_015);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_016);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_017);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_018);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_019);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_020);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_021);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_022);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_023);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_024);
  tcase_add_test (tc, pdf_stm_f_a85dec_apply_025);

  return tc;
}

/* End of pdf-stm-read.c */

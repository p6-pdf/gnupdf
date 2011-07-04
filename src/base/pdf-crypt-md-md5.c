/* -*- mode: C -*-
 *
 *       File:         pdf-crypt-md-md5.c
 *       Date:         Mon Jul 04 20:13:59 2011
 *
 *       GNU PDF Library - Encryption module, MD5 message digest
 *
 */

/* Copyright (C) 2011 Free Software Foundation, Inc. */

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

#include <stdlib.h>
#include <string.h>
#include <gcrypt.h>

#include <pdf-alloc.h>
#include <pdf-types.h>
#include <pdf-error.h>
#include <pdf-crypt-md-md5.h>

struct pdf_crypt_md_md5_s {
  /* Implementation */
  const struct pdf_crypt_md_s *parent;
  /* Implementation-specific private data */
  gcry_md_hd_t hd;
};

static pdf_bool_t
md5_write (pdf_crypt_md_t    *md,
           const pdf_char_t  *in,
           pdf_size_t         in_size,
           pdf_error_t      **error)
{
  struct pdf_crypt_md_md5_s *md5 = (struct pdf_crypt_md_md5_s *)md;

  gcry_md_write (md5->hd, in, in_size);
  return PDF_TRUE;
}

static pdf_bool_t
md5_read (pdf_crypt_md_t  *md,
          pdf_char_t      *out,
          pdf_size_t       out_size,
          pdf_error_t    **error)
{
  struct pdf_crypt_md_md5_s *md5 = (struct pdf_crypt_md_md5_s *)md;
  pdf_uchar_t *hash;
  gcry_error_t gcry_error;
  pdf_size_t required_size;

  /* Check required size for output */
  required_size = gcry_md_get_algo_dlen (GCRY_MD_MD5);
  if (out_size < required_size)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_ENCRYPTION,
                     PDF_EBADDATA,
                     "cannot read message digest: "
                     "need at least %lu bytes in output (%lu given)",
                     (unsigned long)required_size,
                     (unsigned long)out_size);
      return PDF_FALSE;
    }

  /* Check if we can finalize before getting the hash */
  gcry_error = gcry_md_final (md5->hd);
  if (gcry_error != GPG_ERR_NO_ERROR)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_ENCRYPTION,
                     PDF_ENOMEM,
                     "cannot read message digest: '%s/%s'",
                     gcry_strsource (gcry_error),
                     gcry_strerror (gcry_error));
      return PDF_FALSE;
    }

  hash = gcry_md_read (md5->hd, GCRY_MD_MD5);
  if (!hash)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_ENCRYPTION,
                     PDF_ENOMEM,
                     "cannot read message digest: "
                     "MD5 algorithm not enabled");
      return PDF_FALSE;
    }

  /* Copy hash to output buffer */
  memcpy (out, hash, required_size);
  gcry_md_reset (md5->hd);

  return PDF_TRUE;
}

static void
md5_destroy (pdf_crypt_md_t *md)
{
  struct pdf_crypt_md_md5_s *md5 = (struct pdf_crypt_md_md5_s *)md;

  gcry_md_close (md5->hd);
  pdf_dealloc (md5);
}

/* Implementation of the message digest module */
static const struct pdf_crypt_md_s implementation = {
  .write   = md5_write,
  .read    = md5_read,
  .destroy = md5_destroy
};

pdf_crypt_md_t *
pdf_crypt_md_md5_new (pdf_error_t **error)
{
  gcry_error_t gcry_error;
  struct pdf_crypt_md_md5_s *md;

  md = pdf_alloc (sizeof (struct pdf_crypt_md_md5_s));
  if (!md)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_ENCRYPTION,
                     PDF_ENOMEM,
                     "cannot create new MD5 message digest object: "
                     "couldn't allocate %lu bytes",
                     (unsigned long)sizeof (struct pdf_crypt_md_md5_s));
      return NULL;
    }

  /* Set implementation API */
  md->parent = &implementation;

  /* Initialize message digest object */
  gcry_error = gcry_md_open (&(md->hd), GCRY_MD_MD5, 0);
  if (gcry_error != GPG_ERR_NO_ERROR)
    {
      pdf_set_error (error,
                     PDF_EDOMAIN_BASE_ENCRYPTION,
                     PDF_ENOMEM,
                     "cannot initialize MD5 message digest: '%s/%s'",
                     gcry_strsource (gcry_error),
                     gcry_strerror (gcry_error));
      pdf_dealloc (md);
      return NULL;
    }

  return (pdf_crypt_md_t *)md;
}

/* End of pdf-crypt-md-md5.c */

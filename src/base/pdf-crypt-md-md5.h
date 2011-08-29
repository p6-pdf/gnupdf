/* -*- mode: C -*-
 *
 *       File:         pdf-crypt-md-md5.h
 *       Date:         Mon Jul 04 20:43:35 2011
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

#ifndef PDF_CRYPT_MD_MD5_H
#define PDF_CRYPT_MD_MD5_H

#include <pdf-crypt.h>

pdf_crypt_md_t *pdf_crypt_md_md5_new (pdf_error_t **error);

#endif /* PDF_CRYPT_MD_MD5_H */

/* End of pdf-crypt-md-md5.h */

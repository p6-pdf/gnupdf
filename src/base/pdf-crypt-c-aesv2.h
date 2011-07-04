/* -*- mode: C -*-
 *
 *       File:         pdf-crypt-c-aesv2.h
 *       Date:         Fri Feb 22 21:05:05 2008
 *
 *       GNU PDF Library - Header file for AESV2 backend encryption module
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


#ifndef PDF_CRYPT_C_AESV2_H
#define PDF_CRYPT_C_AESV2_H

#include <pdf-crypt.h>

pdf_crypt_cipher_t *pdf_crypt_cipher_aesv2_new (pdf_error_t **error);

#endif	/* PDF_CRYPT_C_AESV2_H */

/* End of pdf-crypt-c-aesv2.h */

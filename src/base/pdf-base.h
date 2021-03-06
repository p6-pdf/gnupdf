/* -*- mode: C -*-
 *
 *       File:         pdf-base.h
 *       Date:         Fri Jul  6 20:22:05 2007
 *
 *       GNU PDF Library - Header file for the Base Layer
 *
 */

/* Copyright (C) 2007, 2008, 2009, 2010 Free Software Foundation, Inc. */

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

/*
 * The purpose of this file is to provide the symbols exported from
 * the base modules to other library layers.  By using this header we
 * can change the structure of modules without impacting files in
 * other layers.
 */

#ifndef PDF_BASE_H
#define PDF_BASE_H

#include <config.h>

#include <pdf-alloc.h>
#include <pdf-error.h>
#include <pdf-types.h>
#include <pdf-types-buffer.h>
#include <pdf-token.h>
#include <pdf-token-writer.h>
#include <pdf-token-reader.h>
#include <pdf-time.h>
#include <pdf-crypt.h>
#include <pdf-list.h>
#include <pdf-hash.h>
#include <pdf-stm.h>
#include <pdf-fp.h>
#include <pdf-text.h>
#include <pdf-fsys.h>

#endif /* !PDF_BASE_H */

/* End of pdf_base.h */

/* -*- mode: C -*- Time-stamp: "08/07/24 22:57:03 jemarch"
 *
 *       File:         pdf-alloc.h
 *       Date:         Fri Feb 22 20:54:18 2008
 *
 *       GNU PDF Library - Memory De/Allocation Module
 *
 */

/* Copyright (C) 2007, 2008 Free Software Foundation, Inc. */

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

#ifndef PDF_ALLOC_H
#define PDF_ALLOC_H

#include <pdf-types.h>

/*
 * Functions
 */

/* BEGIN PUBLIC */

void *pdf_alloc (const pdf_size_t size);
void pdf_dealloc (const void *pointer);
void *pdf_realloc (const void *pointer, const pdf_size_t size);

/* END PUBLIC */

#endif /* !PDF_ALLOC_H */

/* End of pdf-alloc.h */


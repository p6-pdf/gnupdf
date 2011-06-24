/* -*- mode: C -*-
 *
 *       File:         pdf-fsys-disk.h
 *       Date:         Thu May 22 18:22:59 2008
 *
 *       GNU PDF Library - Disk Filesystem Implementation
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

#ifndef PDF_FSYS_DISK_H
#define PDF_FSYS_DISK_H

#include <pdf-fsys.h>

/* BEGIN PUBLIC */

#define PDF_FSYS_DISK_ID "disk"

#define PDF_FSYS_DISK pdf_fsys_get (PDF_FSYS_DISK_ID)

/* END PUBLIC */

pdf_bool_t pdf_fsys_disk_init (pdf_error_t **error);
void pdf_fsys_disk_deinit (void);

#endif /* !PDF_FSYS_DISK_H */

/* End of pdf-fsys-disk.h */

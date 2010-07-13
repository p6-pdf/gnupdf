/* -*- mode: C -*- Time-stamp: ""
 *
 *       File:         pdf-utils.h
 *       Date:         Mon Jul  5 16:40:36 2010
 *
 *       GNU PDF Library - Common definitions used by the PDF
 *                         utilities.
 *
 */

/* Copyright (C) 2010 Free Software Foundation, Inc. */

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

#ifndef PDF_UTILS_H
#define PDF_UTILS_H

#define PDF_UTILS_VERSION "trunk"

#define PDF_UTILS_COPYRIGHT_DOC(NAME)                        \
  char *pdf_utils_version_msg =                              \
    NAME " (GNU PDF) " PDF_UTILS_VERSION "\n\
Copyright (C) 2010 Free Software Foundation.\n\
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n\
This is free software: you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law."

#define PDF_UTILS_HELP_FOOTER_DOC(NAME)         \
  "Report " NAME " bugs to pdf-devel@gnu.org\n\
GNU PDF home page: <http://www.gnupdf.org/>\n\
General help using GNU software: <http://www.gnu.org/gethelp/>"

#endif /* pdf-utils.h */

/* End of pdf-utils.h */

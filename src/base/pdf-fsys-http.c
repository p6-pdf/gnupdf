/* -*- mode: C -*- Time-stamp: "10/02/20 15:32:39 jemarch"
 *
 *       File:         pdf-fsys-http.c
 *       Date:         Thu Dec 17 18:27:35 2009
 *
 *       GNU PDF Library - HTTP Filesystem Implementation
 *
 */

/* Copyright (C) 2009, 2010 Free Software Foundation, Inc. */

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
#include <curl/curl.h>

#include <pdf-fsys-http.h>






/******************************************************************************/
/************   Private Function Prototypes   *********************************/
/******************************************************************************/




/******************************************************************************/
/************   Private Functions   *******************************************/
/******************************************************************************/






/* ****************************************************************************/
/* **********   Public Functions    *******************************************/
/* ****************************************************************************/

/* ----------   Filesystem interface   -------------------------------------- */






/* ----------   File interface   -------------------------------------------- */

pdf_status_t 
pdf_fsys_http_file_set_size (pdf_fsys_file_t file, pdf_size_t size)
{
  /* Read Only filesystem - Not able to resize */
  return PDF_EBADPERMS;
}

/* End of pdf-fsys-http.c */

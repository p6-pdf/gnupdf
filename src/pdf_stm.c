/* -*- mode: C -*- Time-stamp: "07/07/08 18:11:04 jemarch"
 *
 *       File:         pdf_stm.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Fri Jul  6 18:43:15 2007
 *
 *       GNU PDF Library - streams
 *
 */

/* Copyright (C) 2007 Jose E. Marchesi */

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


#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <xalloc.h>
#include <pdf_stm.h>

#ifdef HAVE_FSEEK
#define fseek fseeko
#endif /* HAVE_FSEEK */

static int pdf_stm_file_close (pdf_stm_file_t file);
static pdf_stm_size pdf_stm_file_size (pdf_stm_file_t file);
static int pdf_stm_file_seek (pdf_stm_file_t file, pdf_stm_pos_t pos,enum pdf_stm_seek_mode mode);
static pdf_stm_size pdf_stm_file_tell (pdf_stm_file_t file);
static size_t pdf_stm_file_read (pdf_stm_file_t file, char *buf, size_t bytes);
static size_t pdf_stm_file_write (pdf_stm_file_t file, char *buf, size_t bytes);

static int pdf_stm_buffer_close (pdf_stm_buffer_t buffer);
static pdf_stm_size pdf_stm_buffer_size (pdf_stm_buffer_t buffer);
static int pdf_stm_buffer_seek (pdf_stm_buffer_t buffer, pdf_stm_pos_t pos, enum pdf_stm_seek_mode mode);
static pdf_stm_size pdf_stm_buffer_tell (pdf_stm_buffer_t buffer);
static size_t pdf_stm_buffer_read (pdf_stm_buffer_t buffer, char *buf, size_t bytes);
static size_t pdf_stm_buffer_write (pdf_stm_buffer_t buffer, char *buf, size_t bytes);


pdf_stm_t 
pdf_stm_open_file (char *filename, 
                   enum pdf_stm_file_mode_t mode)
{
  pdf_stm_t new_stm;

  new_stm = (pdf_stm_t) xmalloc (sizeof(struct pdf_stm_s));
  new_stm->type = PDF_STM_FILE;
  new_stm->data.file.mode = mode;
  new_stm->data.file.filename = strdup (filename);
  new_stm->data.file.stm = NULL;

  switch (mode)
    {
    case PDF_STM_FILE_READ:
      {
        new_stm->data.file.stm = 
          fopen (filename, "rb");
        break;
      }
    case PDF_STM_FILE_WRITE:
      {
        new_stm->data.file.stm =
          fopen (filename, "wb");
        break;
      }
    case PDF_STM_FILE_APPEND:
      {
        new_stm->data.file.stm =
          fopen (filename, "ab");
        break;
      }
    }

  if (new_stm->data.file.stm == NULL)
    {
      free (new_stm);
      return NULL;
    }

  return new_stm;
}

int
pdf_stm_close (pdf_stm_t stm)
{
  if (fclose (stm->data.file.stm))
    {
      return PDF_ERROR;
    }

  free (stm);
  return PDF_OK;
}

static int
pdf_stm_seek (pdf_stm_t stm,
              pdf_stm_pos_t pos,
              int seek_mode)
{
  if (!fseek (stm->data.file.stm,
              pos,
              seek_mode))
    {
      return PDF_ERROR;
    }
  
  return PDF_OK;
}

INLINE int
pdf_stm_seek_beg (pdf_stm_t stm,
                  pdf_stm_pos_t pos)
{
  if (pdf_stm_seek (stm, pos, SEEK_SET) == -1)
    {
      return PDF_ERROR;
    }
  
  return PDF_OK;
}

INLINE int
pdf_stm_seek_cur (pdf_stm_t stm,
                  pdf_stm_pos_t pos)
{
  if (pdf_stm_seek (stm, pos, SEEK_CUR) == -1)
    {
      return PDF_ERROR;
    }

  return PDF_OK;
}

INLINE int
pdf_stm_seek_end (pdf_stm_t stm,
                  pdf_stm_pos_t pos)
{
  if (pdf_stm_seek (stm, pos, SEEK_END) == -1)
    {
      return PDF_ERROR;
    }

  return PDF_OK;
}

INLINE pdf_stm_pos_t
pdf_stm_tell (pdf_stm_t stm)
{
  return ftell (stm->data.file.stm);
}

INLINE size_t
pdf_stm_read (pdf_stm_t stm,
              char *buf,
              size_t bytes)
{
  if (stm->data.file.mode != PDF_STM_FILE_READ)
    {
      return -1;
    }

  return fread (buf, 1, bytes, stm->data.file.stm);
}

INLINE size_t
pdf_stm_write (pdf_stm_t stm,
               char *buf,
               size_t bytes)
{
  if (stm->data.file.mode == PDF_STM_FILE_READ)
    {
      return -1;
    }

  return fwrite (buf, 1, bytes, stm->data.file.stm);
}

INLINE pdf_stm_pos_t
pdf_stm_size (pdf_stm_t stm)
{
  struct stat file_stats;

  if (fstat (fileno(stm->data.file.stm), &file_stats) != 0)
    {
      return -1;
    }

  return file_stats.st_size;
}

/* End of pdf_stm.h */

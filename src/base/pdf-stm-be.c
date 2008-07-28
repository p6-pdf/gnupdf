/* -*- mode: C -*- Time-stamp: "08/07/27 18:26:57 jemarch"
 *
 *       File:         pdf-stm-be.c
 *       Date:         Wed Jun 18 21:15:16 2008
 *
 *       GNU PDF Library - Stream backend
 *
 */

#include <config.h>
#include <string.h>
#include <pdf-stm-be.h>

/* Forward declarations */

static pdf_size_t pdf_stm_be_mem_read (pdf_stm_be_t be,
                                       pdf_char_t *buffer,
                                       pdf_size_t bytes);
static pdf_size_t pdf_stm_be_mem_write (pdf_stm_be_t be,
                                        pdf_char_t *buffer,
                                        pdf_size_t bytes);
static pdf_size_t pdf_stm_be_file_read (pdf_stm_be_t be,
                                        pdf_char_t *buffer,
                                        pdf_size_t bytes);
static pdf_size_t pdf_stm_be_file_write (pdf_stm_be_t be,
                                         pdf_char_t *buffer,
                                         pdf_size_t bytes);

/*
 * Public functions
 */

pdf_stm_be_t
pdf_stm_be_new_file (pdf_fsys_file_t file,
                     pdf_off_t pos)
{
  pdf_stm_be_t new;

  /* Allocate a new structure */
  new = (pdf_stm_be_t) pdf_alloc(sizeof(struct pdf_stm_be_s));

  /* Initialization */
  new->type = PDF_STM_BE_FILE;
  new->data.file.file = file;
  new->data.file.pos = pos;

  return new;
}

pdf_stm_be_t
pdf_stm_be_new_mem (pdf_char_t *buffer,
                    pdf_size_t size,
                    pdf_size_t pos)
{
  pdf_stm_be_t new;

  /* Allocate a new structure */
  new = (pdf_stm_be_t) pdf_alloc(sizeof(struct pdf_stm_be_s));
  
  /* Initialization */
  new->type = PDF_STM_BE_MEM;
  new->data.mem.buffer = buffer;
  new->data.mem.size = size;
  new->data.mem.pos = pos;

  return new;
}

pdf_status_t
pdf_stm_be_destroy (pdf_stm_be_t be)
{
  pdf_dealloc (be);
  /* Note that we are not deallocating the internal buffer (in the
     case of a memory backend) nor the open file (in the case of a
     file backend) */
  return PDF_OK;
}

pdf_size_t
pdf_stm_be_read (pdf_stm_be_t be,
                 pdf_char_t *buffer,
                 pdf_size_t bytes)
{
  pdf_size_t readed_bytes;

  switch (be->type)
    {
    case PDF_STM_BE_MEM:
      {
        readed_bytes = pdf_stm_be_mem_read (be,
                                            buffer,
                                            bytes);
        break;
      }
    case PDF_STM_BE_FILE:
      {
        readed_bytes = pdf_stm_be_file_read (be,
                                             buffer,
                                             bytes);
        break;
      }
    default:
      {
        /* Uh oh */
        readed_bytes = 0;
        break;
      }
    }

  return readed_bytes;
}

pdf_size_t
pdf_stm_be_write (pdf_stm_be_t be,
                  pdf_char_t *buffer,
                  pdf_size_t bytes)
{
  pdf_size_t written_bytes;

  switch (be->type)
    {
    case PDF_STM_BE_MEM:
      {
        written_bytes = pdf_stm_be_mem_write (be,
                                              buffer,
                                              bytes);
        break;
      }
    case PDF_STM_BE_FILE:
      {
        written_bytes = pdf_stm_be_file_write (be,
                                               buffer,
                                               bytes);
        break;
      }
    default:
      {
        /* Uh oh */
        written_bytes = 0;
        break;
      }
    }

  return written_bytes;
}

/*
 * Private functions
 */

static pdf_size_t
pdf_stm_be_mem_read (pdf_stm_be_t be,
                     pdf_char_t *buffer,
                     pdf_size_t bytes)
{
  pdf_size_t free_bytes;
  pdf_size_t readed_bytes;
  
  if ((bytes == 0) || (bytes < 0))
    {
      return 0;
    }
  
  /* How many bytes can we read from the buffer? */
  free_bytes = be->data.mem.size - be->data.mem.pos;
  if (bytes < free_bytes)
    {
      readed_bytes = bytes;
    }
  else
    {
      readed_bytes = free_bytes;
    }

  if (readed_bytes != 0)
    {
      /* Copy the data */
      strncpy ((char *) buffer,
               (char *) be->data.mem.buffer,
               readed_bytes);
    }

  /* Ok, bye bye */
  return readed_bytes;
}

static pdf_size_t
pdf_stm_be_mem_write (pdf_stm_be_t be,
                      pdf_char_t *buffer,
                      pdf_size_t bytes)
{
  pdf_size_t free_bytes;
  pdf_size_t written_bytes;

  /* Check boundaries of the buffer */
  if ((bytes == 0) || (bytes < 0))
    {
      return 0;
    }

  /* How many bytes can we write into the buffer? */
  free_bytes = be->data.mem.size - be->data.mem.pos;
  if (bytes < free_bytes)
    {
      written_bytes = bytes;
    }
  else
    {
      written_bytes = free_bytes;
    }

  if (written_bytes != 0)
    {
      /* Copy the data */
      strncpy ((char *) be->data.mem.buffer,
               (char *) buffer,
               written_bytes);
    }

  /* Ok, bye bye */
  return written_bytes;
}

static pdf_size_t
pdf_stm_be_file_read (pdf_stm_be_t be,
                      pdf_char_t *buffer,
                      pdf_size_t bytes)
{
  pdf_size_t readed_bytes;
  pdf_size_t current_pos;

  /* Seek in the file */
  pdf_fsys_file_get_pos (be->data.file.file,
                         &current_pos);
  if (pdf_fsys_file_set_pos (be->data.file.file,
                             be->data.file.pos) == PDF_ERROR)
    {
      /* Bad position */
      return 0;
    }

  /* Read the requested number of bytes */
  readed_bytes = pdf_fsys_file_read (be->data.file.file,
                                     1, bytes,
                                     (void *) buffer);

  /* Restore the file position and update the position of the
     stream */
  be->data.file.pos = be->data.file.pos + readed_bytes;
  if (pdf_fsys_file_set_pos (be->data.file.file,
                             current_pos) == PDF_ERROR)
    {
      /* This should not happen, but just in case */
      return 0;
    }

  return readed_bytes;
}

static pdf_size_t
pdf_stm_be_file_write (pdf_stm_be_t be,
                       pdf_char_t *buffer,
                       pdf_size_t bytes)
{
  pdf_size_t written_bytes;
  pdf_size_t current_pos;

  /* Seek in the file */
  pdf_fsys_file_get_pos (be->data.file.file,
                         &current_pos);
  if (pdf_fsys_file_set_pos (be->data.file.file,
                             be->data.file.pos) == PDF_ERROR)
    {
      /* Bad position */
      return 0;
    }
  
  /* Write the requested number of bytes */
  written_bytes = pdf_fsys_file_write (be->data.file.file,
                                       1, bytes,
                                       buffer);
  
  /* Restore the file position and update the position of the
     stream */
  be->data.file.pos = be->data.file.pos + written_bytes;
  if (pdf_fsys_file_set_pos (be->data.file.file,
                             current_pos) == PDF_ERROR)
    {
      /* This should not happen, but just in case */
      return 0;
    }
                                       
  return written_bytes;
}


/* End of pdf-stm-be.c */

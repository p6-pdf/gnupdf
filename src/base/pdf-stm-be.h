/* -*- mode: C -*- Time-stamp: "09/09/05 22:50:42 jemarch"
 *
 *       File:         pdf-stm-be.h
 *       Date:         Wed Jun 18 20:53:27 2008
 *
 *       GNU PDF Library - Stream backend
 *
 */

#ifndef PDF_STM_BE_H
#define PDF_STM_BE_H

#include <config.h>

#include <stdio.h>

#include <pdf-types.h>
#include <pdf-fsys.h>

/* Backend types */
enum pdf_stm_be_type_e
{
  PDF_STM_BE_MEM = 0,
  PDF_STM_BE_FILE,
  PDF_STM_BE_CFILE
};

/* Backend data type */
struct pdf_stm_be_file_s
{
  pdf_fsys_file_t file;
  pdf_off_t pos; /* Current position */
};

struct pdf_stm_be_cfile_s
{
  FILE*      file;
  pdf_off_t  pos;
};

struct pdf_stm_be_mem_s
{
  pdf_char_t *buffer;  /* Buffer contents */
  pdf_size_t size;     /* Size of the buffer in octects */
  pdf_size_t pos;      /* Current position into the buffer */
};

struct pdf_stm_be_s
{
  enum pdf_stm_be_type_e type;

  union
  {
    struct pdf_stm_be_mem_s mem;
    struct pdf_stm_be_file_s file;
    struct pdf_stm_be_cfile_s cfile;
  } data;
};

typedef struct pdf_stm_be_s *pdf_stm_be_t;

/* 
 * Public API
 */

pdf_stm_be_t pdf_stm_be_new_cfile (FILE* file,
				   pdf_off_t pos);
pdf_stm_be_t pdf_stm_be_new_file (pdf_fsys_file_t file,
                                  pdf_off_t pos);
pdf_stm_be_t pdf_stm_be_new_mem (pdf_char_t *buffer,
                                 pdf_size_t size,
                                 pdf_size_t pos);
pdf_status_t pdf_stm_be_destroy (pdf_stm_be_t be);

pdf_size_t pdf_stm_be_read (pdf_stm_be_t be,
                            pdf_char_t *buffer,
                            pdf_size_t bytes);
pdf_size_t pdf_stm_be_write (pdf_stm_be_t be,
                             pdf_char_t *buffer,
                             pdf_size_t bytes);

pdf_off_t pdf_stm_be_seek (pdf_stm_be_t be,
                           pdf_off_t pos);

pdf_off_t pdf_stm_be_tell (pdf_stm_be_t be);

#endif /* !PDF_STM_BE_H */

/* End of pdf-stm-be.h */

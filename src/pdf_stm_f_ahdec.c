/* -*- mode: C -*- Time-stamp: "07/07/13 17:12:27 jemarch"
 *
 *       File:         pdf_stm_f_ahdec.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Fri Jul 13 17:08:41 2007
 *
 *       GNU PDF Library - ASCII Hex decoder filter
 *
 */

#include <config.h>
#include <malloc.h>
#include <xalloc.h>
#include <string.h>
#include <pdf_stm_f_ahdec.h>

int
pdf_stm_f_ahdec_init (void **filter_data,
                      void *conf_data)
{
  pdf_stm_f_ahdec_data_t *data;
  data = (pdf_stm_f_ahdec_data_t *) filter_data;

  /* Create the private data storage */
  *data =
    (pdf_stm_f_ahdec_data_t) xmalloc (sizeof(struct pdf_stm_f_ahdec_data_s));
  (*data)->dummy = 0;

  return PDF_OK;
}

int 
pdf_stm_f_ahdec_apply (void *filter_data,
                       pdf_char_t *in, pdf_stm_pos_t in_size,
                       pdf_char_t **out, pdf_stm_pos_t *out_size)
{
  pdf_stm_f_ahdec_data_t data;
  data = (pdf_stm_f_ahdec_data_t) filter_data;

  /* Do nothing */
  *out = (pdf_char_t *) xmalloc (in_size);
  memcpy (*out,
          in,
          in_size);
  *out_size = in_size;

  return PDF_OK;
}

int
pdf_stm_f_ahdec_dealloc (void **filter_data)
{
  pdf_stm_f_ahdec_data_t *data;
  
  data = (pdf_stm_f_ahdec_data_t *) filter_data;
  free (*data);

  return PDF_OK;
}

/* End of pdf_stm_f_ahdec.c */

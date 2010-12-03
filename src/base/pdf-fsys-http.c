/* -*- mode: C -*-
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
#include <sys/select.h>
#include <unistd.h>
#include <curl/curl.h>

#include <pdf-fsys-http.h>


/******************************************************************************/
/************  Interface structure  *******************************************/
/******************************************************************************/

const struct pdf_fsys_impl_s pdf_fsys_http_implementation =
  {
    pdf_fsys_http_init,
    pdf_fsys_http_cleanup,
    pdf_fsys_http_create_folder,
    pdf_fsys_http_get_folder_contents,
    pdf_fsys_http_get_parent,
    pdf_fsys_http_remove_folder,
    pdf_fsys_http_get_item_props,
    pdf_fsys_http_get_free_space,
    pdf_fsys_http_item_p,
    pdf_fsys_http_item_readable_p,
    pdf_fsys_http_item_writable_p,
    pdf_fsys_http_file_get_url,
    pdf_fsys_http_file_open,
    pdf_fsys_http_file_open_tmp,
    pdf_fsys_http_file_read,
    pdf_fsys_http_file_write,
    pdf_fsys_http_file_flush,
    pdf_fsys_http_file_can_set_size_p,
    pdf_fsys_http_file_get_size,
    pdf_fsys_http_file_set_size,
    pdf_fsys_http_file_get_pos,
    pdf_fsys_http_file_set_pos,
    pdf_fsys_http_file_get_mode,
    pdf_fsys_http_file_set_mode,
    pdf_fsys_http_file_same_p,
    pdf_fsys_http_file_request_ria,
    pdf_fsys_http_file_has_ria,
    pdf_fsys_http_file_cancel_ria,
    pdf_fsys_http_file_close,
    pdf_fsys_http_file_reopen
  };






/******************************************************************************/
/************   Private Function Prototypes   *********************************/
/******************************************************************************/
static pdf_status_t
__pdf_fsys_http_valid ( pdf_fsys_file_t file );

static void *
__pdf_fsys_http_proc (void *datum);

static void
__pdf_fsys_http_multi_perform (pdf_fsys_http_data_t data);

static pdf_status_t
__pdf_fsys_http_get_host_path (pdf_text_t path, pdf_char_t **host_path,
                               pdf_size_t *host_path_size);

static void 
__pdf_fsys_http_init_props (struct pdf_fsys_item_props_s *item_props);

static void
__pdf_fsys_http_add_file_to_list (pdf_fsys_http_data_t data, 
                                  pdf_fsys_file_t file);

static pdf_fsys_http_file_t
__pdf_fsys_http_get_file_by_easy (pdf_fsys_http_data_t data, CURL *easy);

static size_t
__pdf_fsys_http_readlim (pdf_fsys_http_file_t http_file, size_t bytes);

static size_t
__pdf_fsys_http_libcurl_writefunc (void *ptr, size_t size, size_t nmemb, 
                                   void *stream);

/******************************************************************************/
/************   Private Functions   *******************************************/
/******************************************************************************/


/* This function is used to check that file represents an http filesystem file
 * that is valid and safe to work with */
static pdf_status_t
__pdf_fsys_http_valid ( pdf_fsys_file_t file )
{
  pdf_status_t rv = PDF_OK;

  if ( NULL == file )
    {
      rv = PDF_ERROR;
    }

  if ( PDF_OK == rv )
    {
      if ( NULL == file->fs )
        {
          rv = PDF_ERROR;
        }
    }

  if ( PDF_OK == rv )
    {
      if ( NULL == file->data )
        {
          rv = PDF_ERROR;
        }
    }

  if ( PDF_OK == rv )
    {
      if ( &pdf_fsys_http_implementation != file->fs->implementation )
        {
          // not this http filesystem implementation
          rv = PDF_ERROR;
        }
    }

  return rv;
}



/* thread for handling background downloading, ria, etc will run in this func */
static void *
__pdf_fsys_http_proc (void *datum)
{
  pdf_fsys_http_data_t data;
  CURLMcode            crv;
  fd_set               read_fd_set;
  fd_set               write_fd_set;
  fd_set               exc_fd_set;
  int                  max_fd;
  int                  srv;
  struct timeval       timeout;

  data = (pdf_fsys_http_data_t)datum;

  while (!data->terminate)
    {

      __pdf_fsys_http_multi_perform (data);

      FD_ZERO (&read_fd_set);
      FD_ZERO (&write_fd_set);
      FD_ZERO (&exc_fd_set);

      pthread_mutex_lock (&(data->mutex));
      {
        crv = curl_multi_fdset (data->curlm, &read_fd_set, &write_fd_set, 
                                &exc_fd_set, &max_fd); 
      }
      pthread_mutex_unlock (&(data->mutex));


      if (CURLE_OK == crv)
        {
          if (-1 == max_fd)
            {
              // no descriptors put into set.
              sleep (1); // try again in 1 second.
            }
          else
            {
              timeout.tv_usec = 0;
              timeout.tv_sec = 1;

              srv = select (max_fd+1, &read_fd_set, &write_fd_set, &exc_fd_set,
                            &timeout);

              if (-1 == srv)
                {
                  // error in select.
                }
              else if (0 == srv)
                {
                  // nothing to do
                }
              else if (srv > 0)
                {
                  // something to do
                  __pdf_fsys_http_multi_perform (data);
                }
              else
                {
                  // unexpected error condition
                }
            }
        }
      else
        {
          // failure of curl_multi_fdset.
          sleep (1); // try again in a second.
        }
      
    }

  pthread_exit (NULL);

}


// __pdf_fsys_http_multi_perform is called by the worker thread
static void
__pdf_fsys_http_multi_perform (pdf_fsys_http_data_t data)
{
  CURLMcode   cmc;
  CURLMsg    *cmsg;
  int         pre_run;
  int         running;
  int         msgs;
  pdf_bool_t  done;
  CURL       *pc;
  CURLcode    cc;

  pdf_fsys_http_file_t http_file;

  do 
    {
      pthread_mutex_lock (&(data->mutex));
      {
        pre_run = data->running_handles;
        cmc = curl_multi_perform (data->curlm, &running);
      }
      pthread_mutex_unlock (&(data->mutex));

#warning "for some reason handles continue to run"
      if (running < pre_run)
        {
          done = PDF_FALSE;
          while (!done)
            {
              cmsg = NULL;

              pthread_mutex_lock (&(data->mutex));
              {
                cmsg = curl_multi_info_read (data->curlm, &msgs);
              }
              pthread_mutex_unlock (&(data->mutex));
              if (NULL == cmsg)
                {
                  done = PDF_TRUE;
                }
              else
                {
                  if (CURLMSG_DONE == cmsg->msg)
                    {
                      pthread_mutex_lock (&(data->mutex));
                      {
                        data->running_handles--;
                      }
                      pthread_mutex_unlock (&(data->mutex));

                      pc = cmsg->easy_handle;
                      cc = cmsg->data.result;

                      http_file = __pdf_fsys_http_get_file_by_easy (data, pc);

                      if (NULL != http_file)
                        {
                          http_file->downloading = PDF_FALSE;
                        }

                      // curl_easy_getinfo called here if more data desired.
                      if (CURLE_OK == cc)
                        {
                          // everything finished successfully
                          // *!*!*! wrap up successful operation

#warning "There is probably wrap-up work to be done here."
                        }
                      else
                        {
                          // there was some type of error.
                          // **!*!*!*! cleanup after failed operation
#warning "There is probably clean-up work to be done here."
                        }

                    }
                  else
                    {
                      // unknown message returned.
                    }
                }
            } // end of while loop for curl_multi_read

        }
      else
        {
          // running handle count was unchanged.
        }

    }
  while (CURLM_CALL_MULTI_PERFORM == cmc);

  return;
}


static pdf_status_t
__pdf_fsys_http_get_host_path (pdf_text_t path, pdf_char_t **host_path,
                               pdf_size_t *host_path_size)
{
#ifdef PDF_HOST_WIN32
  /* For W32, we will always use widechar functions, so Windows' wchar_t
   * implementation should be used (UTF-16LE) */
  pdf_char_t *data = NULL;
  pdf_size_t size = 0;

  if (pdf_text_get_unicode (&data,
                            &size,
                            path,
                            PDF_TEXT_UTF16_LE,
                            PDF_TEXT_UNICODE_WITH_NUL_SUFFIX) == PDF_OK)
    {
      *host_path = data;
      *host_path_size = size;
      return PDF_OK;
    }
  else
    return PDF_ERROR;
#else
  /* Call the pdf_text module to get a host-encoded version of the
   *  given path */
  pdf_char_t *padded = NULL;
  pdf_size_t padded_size = 0;

  if (pdf_text_get_host (&padded,
                         &padded_size,
                         path,
                         pdf_text_get_host_encoding ()) == PDF_OK)
    {
      *host_path = pdf_realloc (padded, padded_size+2);
      if (*host_path != NULL)
        {
          *host_path_size = padded_size + 1;
          (*host_path)[(*host_path_size)-1] = '\0'; 
          return PDF_OK;
        }
      else
        return PDF_ENOMEM;
    }
  else
    return PDF_ERROR;
#endif
}


static void 
__pdf_fsys_http_init_props (struct pdf_fsys_item_props_s *item_props)
{
  memset (item_props, 0, sizeof (struct pdf_fsys_item_props_s));
  
  return;
}


static void
__pdf_fsys_http_add_file_to_list (pdf_fsys_http_data_t data, 
                                  pdf_fsys_file_t file)
{
  if ((NULL == data) ||
      (NULL == file))
    {
      return;
    }

  pthread_mutex_lock (&(data->mutex));
  {
    pdf_list_add_last (data->file_list, file, NULL);
  }
  pthread_mutex_unlock (&(data->mutex));

  return;
}

static pdf_fsys_http_file_t
__pdf_fsys_http_get_file_by_easy (pdf_fsys_http_data_t data,
                                  CURL *easy)
{
  pdf_size_t           i;
  pdf_size_t           n;
  pdf_fsys_file_t      file;
  pdf_fsys_http_file_t http_file;
  pdf_fsys_http_file_t rv_http_file = NULL;

  pthread_mutex_lock (&(data->mutex));
  {

    n = pdf_list_size (data->file_list);
    
    for (i=0; i<n; i++)
      {
        if (pdf_list_get_at (data->file_list, i, (const void**)&file)
            == PDF_OK)
          {
            http_file = NULL;

            if (NULL != file)
              {
                http_file = (pdf_fsys_http_file_t)file->data;
              }

            if (NULL != http_file)
              {
                if ((NULL != http_file->curl) && (easy == http_file->curl))
                  { // if easy handle matches then this is our file
                    rv_http_file = http_file;
                  }
              }
          }
      }
    
  }
  pthread_mutex_unlock (&(data->mutex));

  return rv_http_file;
}


static size_t
__pdf_fsys_http_readlim (pdf_fsys_http_file_t http_file, size_t bytes)
{
  size_t rv;
  size_t avail;

#warning "This will have to be significantly reworked to support RIA."

  avail = http_file->write_idx - http_file->read_idx;
  
  rv = (avail < bytes) ? avail : bytes;

  return rv;
}


static size_t
__pdf_fsys_http_libcurl_writefunc (void *ptr, size_t size, size_t nmemb, 
                                   void *stream)
{
  pdf_size_t written = 0;
  pdf_size_t to_write;
  pdf_status_t wrv;

  /* This function gets called by libcurl when there is received data */

  pdf_fsys_http_file_t http_file = NULL;

  if ((NULL == ptr) || (NULL == stream))
    {
      return 0;
    }
  
  http_file = (pdf_fsys_http_file_t) stream;

  // write to http_file->temp_file

  /*  *!*!*!*!*  Need to ensure that size * nmemb does not exceed pdf_size_t */
#warning "potential overflow. additional safeties required."
  to_write = size * nmemb;

  pthread_mutex_lock (&(http_file->mutex));
  {

    pdf_fsys_file_set_pos (http_file->temp_file, http_file->write_idx);

    wrv = pdf_fsys_file_write (http_file->temp_file, ptr, 
                               to_write, &written);

    http_file->write_idx += written;

  }
  pthread_mutex_unlock (&(http_file->mutex));

#warning "Must track which parts of file have been written so that set_pos"
#warning "can be safely implemented - especially needed for RIA"

  return written;

}



/* ****************************************************************************/
/* **********   Public Functions    *******************************************/
/* ****************************************************************************/

/* ----------   Filesystem interface   -------------------------------------- */

/* Filesystem initialization and cleanup */


pdf_status_t
pdf_fsys_http_init (void **data)
{
  int prv;
  pdf_status_t ret_code = PDF_OK;
  pdf_fsys_http_data_t pdf_fsys_http_data;
  

  if (NULL == data)
    {
      return PDF_ERROR;
    }

  pdf_fsys_http_data = 
    (pdf_fsys_http_data_t) pdf_alloc (sizeof (struct pdf_fsys_http_data_s));

  if (NULL != pdf_fsys_http_data)
    {
      *data = (void *)pdf_fsys_http_data;
    }

  pdf_fsys_http_data->worker = 0;
  pdf_fsys_http_data->curlm = NULL;
  pdf_fsys_http_data->terminate = PDF_FALSE;
  pdf_fsys_http_data->curlm = curl_multi_init();

  // docs say "always returns 0", so don't bother catching rv
  pthread_mutex_init (&(pdf_fsys_http_data->mutex), NULL);

  if (NULL == pdf_fsys_http_data->curlm)
    {
      ret_code = PDF_ERROR;
    }
  else
    {
      ret_code = pdf_list_new (NULL, NULL, PDF_TRUE, 
                               &(pdf_fsys_http_data->file_list) );
    }

  if (PDF_OK == ret_code)
    {
      // create worker thread.

      prv = pthread_create (&(pdf_fsys_http_data->worker), NULL, 
                            __pdf_fsys_http_proc, pdf_fsys_http_data);
    }

  pdf_fsys_http_data->init_stat = ret_code;

  return ret_code;
}


pdf_status_t
pdf_fsys_http_cleanup (void *data)
{
  pdf_fsys_http_data_t pdf_fsys_http_data;
  CURLMcode            crv;
  pdf_size_t           i;
  pdf_size_t           n;
  pdf_fsys_http_file_t http_file;

  pdf_fsys_http_data = (pdf_fsys_http_data_t)data;

  // instruct worker thread to quit:
  pdf_fsys_http_data->terminate = PDF_TRUE;

  // wait for worker thread to quit:
  pthread_join (pdf_fsys_http_data->worker, NULL);

  pdf_fsys_http_data->init_stat = PDF_ERROR;

  // cleanup any remaining curl resources: 
  n = pdf_list_size (pdf_fsys_http_data->file_list);

  for (i=0; i<n; i++)
    {
      if (pdf_list_get_at (pdf_fsys_http_data->file_list, i, (const void**)&http_file)
          == PDF_OK)
        {
          // worker thread is dead, so no need to grab mutex here.

          if (NULL != http_file->curl)
            {
              // Remove any leftover easy handles from multi handle
              curl_multi_remove_handle (pdf_fsys_http_data->curlm, 
                                        http_file->curl);
              // and cleanup each easy handle
              curl_easy_cleanup (http_file->curl);
            }
          // close temp file if open
          pdf_fsys_file_close (http_file->temp_file);
          
          if (NULL != http_file->host_path)
            {
              // delete host_path
              pdf_dealloc (http_file->host_path);
            }
          // delete http_file
          pdf_dealloc (http_file);
        }
    }

  // blow away the list
  pdf_list_destroy (pdf_fsys_http_data->file_list);

  crv = curl_multi_cleanup (pdf_fsys_http_data->curlm);

  pthread_mutex_destroy (&(pdf_fsys_http_data->mutex));

  // free filesystem resources
  pdf_dealloc (pdf_fsys_http_data);

  return PDF_OK;
}

/* Filesystem implementation */


/* Create folder - function must exist for interface - not supported - RO */
pdf_status_t
pdf_fsys_http_create_folder (void *data,
                             pdf_text_t path_name)
{
  /* Read Only filesystem - Not able to create */
  return PDF_EBADPERMS;
}


/* Get folder contents (list files) */
pdf_status_t
pdf_fsys_http_get_folder_contents (void *data,
                                   pdf_text_t path_name,
                                   pdf_list_t item_list)
{
  /* Not possible AFAIK to get a standard directory listing, even if listings */
  /* are allowed for a particular directory */
  /* Perhaps if WebDAV is implemented, this could be done. */

  return PDF_EBADPERMS;
}


/* Get path name of the father of a given item in the filesystem */
pdf_status_t
pdf_fsys_http_get_parent (void *data,
                          pdf_text_t path_name,
                          pdf_text_t parent_path)
{
  // No clear meaning for URL request.
  return PDF_ERROR;
}


/* Remove folder - function must exist for interface - not supported - RO */
pdf_status_t
pdf_fsys_http_remove_folder (void *data,
                             pdf_text_t path_name)
{
  /* Read Only filesystem - Not able to delete */
  return PDF_EBADPERMS;
}


/* Get item properties */
pdf_status_t
pdf_fsys_http_get_item_props (void *data,
                              pdf_text_t path_name,
                              struct pdf_fsys_item_props_s *props)
{
  // Use HTTP HEAD request to retrieve content length, and verify readability

  CURL   * curl;
  CURLcode res;

  long     http_res_code;
  double   file_len;
  double   temp_d;

  pdf_char_t* host_path = NULL;
  pdf_size_t host_path_len = 0;

  pdf_status_t ret_code = PDF_EBADDATA;


  curl = curl_easy_init ();
  
  if ((curl) && (NULL != props))
    {
      /* convert path_name into host-encoding for curl */
      ret_code = __pdf_fsys_http_get_host_path (path_name,
                                                &host_path,
                                                &host_path_len);
    }

  if (PDF_OK == ret_code)
    {
      res = curl_easy_setopt (curl, CURLOPT_URL, host_path);

      if (CURLE_OK != res)
        {
          ret_code = PDF_ERROR;
        }
      else
        { /* This is the head request (NO BODY = HEAD) */
          res = curl_easy_setopt (curl, CURLOPT_NOBODY, 1L);
          
          if (CURLE_OK != res)
            {
              ret_code = PDF_ERROR;
            }
        }
    }

  if (PDF_OK == ret_code)
    {
      
      res = curl_easy_perform (curl);
          
      if (CURLE_OK == res)
        {
          res = curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, 
                                   &http_res_code);
          
          if (CURLE_OK == res)
            {
              
              if (200 == http_res_code)
                {
                  __pdf_fsys_http_init_props (props);

                  props->is_readable = PDF_TRUE;
                  props->is_writable = PDF_FALSE;
                  memset (&(props->creation_date), 0, sizeof (pdf_time_t));
                  memset (&(props->modification_date), 0, 
                          sizeof (pdf_time_t));
                  props->file_size_high = (pdf_u32_t)UINT32_MAX;
                  props->file_size_low = (pdf_u32_t)UINT32_MAX;
                  props->folder_size = (pdf_u32_t)0;
                  
                  res = curl_easy_getinfo (curl, 
                                           CURLINFO_CONTENT_LENGTH_DOWNLOAD, 
                                           &file_len);
                  
                  if ((CURLE_OK == res) && (file_len >= 0.0))
                    {
                      // convert double into file_size_high and file_size_low

#define TWO_RAISED_TO_32_AS_DOUBLE (4294967296.0)

                      // divide by 2^32 to get high-32-bits in temp_d
                      temp_d = file_len / TWO_RAISED_TO_32_AS_DOUBLE;
                      // cast temp_d to assign high
                      props->file_size_high = (pdf_u32_t)temp_d;
                      // move back to double domain to calculate low
                      temp_d = file_len - (((double)props->file_size_high) 
                                           * TWO_RAISED_TO_32_AS_DOUBLE);
                      // cast temp_d to assign low
                      props->file_size_low = (pdf_u32_t)temp_d;
                    }
                  else
                    {
                      // download size not available.
                    }
                }
              else
                {
                  props->is_readable = PDF_FALSE;                      
                }
            }
          else
            {
              ret_code = PDF_ERROR;
            }
        }
      else
        {
          ret_code = PDF_ERROR;
        }
    }

  if (NULL != curl)
    {
      curl_easy_cleanup (curl);
      curl = NULL;
    }

  return ret_code;
}


/* Get the free storage space in the volume containing path_name */
pdf_i64_t
pdf_fsys_http_get_free_space (void *data,
                              pdf_text_t path_name)
{
  /* Read-Only filesystem - no free space */
  return (pdf_i64_t)0;
}


/* Check if file exists */
pdf_bool_t
pdf_fsys_http_item_p (void *data,
                      pdf_text_t path_name)
{
  pdf_bool_t rv;
  
  /* In HTTP filesystem an item exists if it is readable */
  rv = pdf_fsys_http_item_readable_p (data, path_name);

  return rv;
}


/* Check if file is readable */
pdf_bool_t
pdf_fsys_http_item_readable_p (void *data,
                               pdf_text_t path_name)
{
  struct pdf_fsys_item_props_s item_props;
  pdf_status_t ret_code;
  pdf_bool_t rv = PDF_FALSE;


  ret_code = pdf_fsys_http_get_item_props (data, path_name, &item_props);

  if (PDF_OK == ret_code)
    {
      if (item_props.is_readable)
        {
          rv = PDF_TRUE;
        }
    }

  return rv;
}


/* Check if file is writable - Read Only (RO) filesystem - op. not supported */
pdf_bool_t
pdf_fsys_http_item_writable_p (void *data,
                               pdf_text_t path_name)
{
  return PDF_FALSE;
}

/* --------------------- File interface ------------------------------------- */

pdf_status_t
pdf_fsys_http_file_read (pdf_fsys_file_t file, 
                         pdf_char_t *buf,
                         pdf_size_t bytes, 
                         pdf_size_t *read_bytes)
{
  pdf_status_t rv = PDF_OK;
  pdf_fsys_http_file_t http_file = NULL;

  // opened file is handed to worker thread which downloads and writes to
  // temp file.  The "read" function can pass read requests to the disk
  // filesystem on which the temp file has been opened.

  rv = __pdf_fsys_http_valid ( file );

  if ( PDF_OK == rv )
    { // now we know we have a file we can work with.
      
      http_file = (pdf_fsys_http_file_t) file->data;

#warning "Will have to check read request against what parts of file have been downloaded before handing read command off to file_read on temp_file."

      pthread_mutex_lock (&(http_file->mutex));
      {
        pdf_fsys_file_set_pos ( http_file->temp_file, http_file->read_idx );

        // limit the read request to the number of bytes currently available
        // bytes = __pdf_fsys_http_readlim (http_file, bytes);

        rv = pdf_fsys_file_read ( http_file->temp_file, buf, bytes, read_bytes);
        
        // we are not really at the EOF until downloading is complete.
        if ((PDF_EEOF == rv) && (http_file->downloading))
          {
            rv = PDF_OK;
          }

        http_file->read_idx += *read_bytes;
      }
      pthread_mutex_unlock (&(http_file->mutex));

    }

  return rv;
}


/* write data to file - Read Only filesystem - Not able to write */
pdf_status_t
pdf_fsys_http_file_write (pdf_fsys_file_t file, 
                          pdf_char_t *buf,
                          pdf_size_t bytes, 
                          pdf_size_t *written_bytes)
{
  /* Read Only filesystem - Not able to write */
  return PDF_EBADPERMS;
}

/* flush cached data - Read Only filesystem - Not able to write/flush */
pdf_status_t
pdf_fsys_http_file_flush (pdf_fsys_file_t file)
{
  /* Read Only filesystem - Not able to write/flush */
  return PDF_EBADPERMS;
}


/* Read-Only filesystem - cannot set size */
pdf_bool_t
pdf_fsys_http_file_can_set_size_p (pdf_fsys_file_t file,
                                   pdf_size_t pos)
{
  return PDF_FALSE;
}

pdf_status_t
pdf_fsys_http_file_open (void *data,
                         pdf_text_t path_name,
                         enum pdf_fsys_file_mode_e mode,
                         pdf_fsys_file_t *p_file)
{
  struct pdf_fsys_item_props_s item_props;
  pdf_fsys_http_data_t pdf_fsys_http_data;
  pdf_fsys_http_file_t http_file;
  pdf_status_t      rv = PDF_OK;
  pdf_fsys_file_t file = NULL;
  pdf_fsys_t      fsys = NULL;
  pdf_bool_t len_avail = PDF_FALSE;

  CURLcode res;
  CURLMcode cmc;

  pdf_fsys_http_data = (pdf_fsys_http_data_t)data;

  // if anything other than read-only is given as mode then return error
  if (PDF_FSYS_OPEN_MODE_READ != mode)
    {
      rv = PDF_EBADPERMS;
    }
  else if (NULL == p_file)
    {
      rv = PDF_EBADDATA;
    }

  if (PDF_OK == rv)
    { // get properties so that we know size and readability, etc.
      rv = pdf_fsys_http_get_item_props (data, path_name, &item_props);
    }

  if (PDF_OK == rv)
    { // we now have item props:
      
      if (!item_props.is_readable)
        {
          rv = PDF_EBADPERMS;
        }

      if ((UINT32_MAX == item_props.file_size_high)
          && (UINT32_MAX == item_props.file_size_low))
        { // we need to receive content_len from server to set up rx buffer
          len_avail = PDF_FALSE;
        }
      else
        {
          len_avail = PDF_TRUE;
        }
    }

  if (PDF_OK == rv)
    { // all conditions are in place to initiate download of requested item.
      
      // allocate a pdf_fsys_http_file_s and assign path items
      http_file = NULL;
      http_file = pdf_alloc (sizeof (struct pdf_fsys_http_file_s));

      file = pdf_alloc (sizeof (struct pdf_fsys_file_s));

      fsys = pdf_alloc (sizeof (struct pdf_fsys_s));

      if ((NULL == file)
          || (NULL == fsys)
          || (NULL == http_file))
        {
#warning "memory leak likely here if some alloc succeeded."
          rv = PDF_ENOMEM;
        }
      else
        {
          memset (http_file, 0, sizeof (struct pdf_fsys_http_file_s));

          __pdf_fsys_http_add_file_to_list (pdf_fsys_http_data, file);

          *p_file = file;
        }
    }
     
  if (PDF_OK == rv)
    {
      // docs say "always returns 0", so don't bother catching rv
      pthread_mutex_init (&(http_file->mutex), NULL);
    }

  if (PDF_OK == rv)
    {
      
      rv = __pdf_fsys_http_get_host_path (path_name,
                                          &(http_file->host_path),
                                          &(http_file->host_path_size));
      
      fsys->implementation = &pdf_fsys_http_implementation;
      file->fs = fsys;
      file->data = http_file;
      
      // now http_file is partially filled-out
      
      // open a temporary file on disk for storage of downloaded data
      rv = pdf_fsys_file_open_tmp (NULL,
                                   &http_file->temp_file);

    }

  if (PDF_OK == rv)
    { /* temp file is open - now to set the size: */
      
      if (len_avail)
        { /* content len was available - set size now */
          
          // test if requested file is too large for this build:
          if ((sizeof (pdf_size_t) <= 4)
              && (0 != item_props.file_size_high))
            {
              // 32-bit size_t: Unable to handle file > 4GB
              // A 64-bit build should not fail here.
              rv = PDF_EOVERFLOW;
            }
          else
            {
              // Always use the low 32-bits from libcurl 
              http_file->temp_file_size = item_props.file_size_low;
              
              // if we're 64-bit on size_t then assign size_high also
              if ((sizeof (pdf_size_t) == 8)
                  && (0 != item_props.file_size_high))
                {
                  // cast _high to 64-bit then left shift 32 bits.
                  http_file->temp_file_size |= 
                    (((pdf_size_t)item_props.file_size_high) << 32);
                }
              
              // set temp file size
              rv = pdf_fsys_file_set_size (http_file->temp_file, 
                                           http_file->temp_file_size);
              
              // set http_file->temp_file_sized
              if (PDF_OK == rv)
                {
                  http_file->temp_file_sized = PDF_TRUE;
                }
            }
        }
      else // len not available.
        { 
          // file size will be set if an RIA request is successful
          http_file->temp_file_sized = PDF_FALSE;
        }

    }

  if (PDF_OK == rv)
    { /* Initiate download */

      http_file->curl = curl_easy_init ();

      if (http_file->curl)
        {
          res = curl_easy_setopt (http_file->curl, 
                                  CURLOPT_URL, http_file->host_path);
          
          if (CURLE_OK != res)
            {
              rv = PDF_ERROR;
            }

          // set the callback function that will do the writing to temp file:
          res = curl_easy_setopt (http_file->curl,
                                  CURLOPT_WRITEFUNCTION, 
                                  __pdf_fsys_http_libcurl_writefunc);

          if (CURLE_OK != res)
            {
              rv = PDF_ERROR;
            }

          // set the additional data item that will be available to writeback fn
          res = curl_easy_setopt (http_file->curl,
                                  CURLOPT_WRITEDATA, http_file);
          if (CURLE_OK != res)
            {
              rv = PDF_ERROR;
            }
          
          http_file->downloading = PDF_TRUE;

          if (PDF_OK == rv)
            {
              // pass the new curl off to the worker thread for processing:
              pthread_mutex_lock (&(pdf_fsys_http_data->mutex));
              {
                pdf_fsys_http_data->running_handles++;
                
                cmc = curl_multi_add_handle (pdf_fsys_http_data->curlm, 
                                             http_file->curl);

                if (CURLE_OK != cmc)
                  {
                    rv = PDF_ERROR;
                  }
              }
              pthread_mutex_unlock (&(pdf_fsys_http_data->mutex));

            }
          else
            {
              // there was a problem.
              curl_easy_cleanup (http_file->curl);
              http_file->curl = NULL;
#warning "open failed - cleanup is needed to avoid leaks."
            }
        }
      else
        {
          // curl_easy_init returned NULL - nothing good can come of this
        }
    }

  return rv;
}

pdf_status_t
pdf_fsys_http_file_open_tmp (void *data,
                             pdf_fsys_file_t *p_file)
{
  // Does not make sense for a read-only filesystem.
  return PDF_ERROR;
}


/* return file size information if known */
pdf_size_t
pdf_fsys_http_file_get_size (pdf_fsys_file_t file)
{
  // !*!*!*!*! Implement me! !*!*!*!*
#warning "_get_size interface _must_ change to return pdf_status_t"
  // This will allow us to return "I do not know"
  pdf_size_t rv = 0;

  if (PDF_OK == __pdf_fsys_http_valid ( file ) )
    {
      // file is ok to work with

      // Size can be returned if the host provided it,
      // or if the transfer is complete

    }

  // return information about file size

  return rv;
}

pdf_status_t
pdf_fsys_http_file_set_size (pdf_fsys_file_t file,
                             pdf_size_t pos)
{
  /* Read Only filesystem - Not able to resize */
  return PDF_EBADPERMS;
}


/* Get pos */
pdf_status_t
pdf_fsys_http_file_get_pos (pdf_fsys_file_t file,
                            pdf_size_t *pos)
{
  pdf_status_t rv = PDF_ERROR;
  pdf_fsys_http_file_t http_file = NULL;


  if (PDF_OK == __pdf_fsys_http_valid ( file ) )
    {
      // ok to work with

      http_file = (pdf_fsys_http_file_t) file->data;

      rv = http_file->read_idx;

    }


  return rv;
}


/* Set pos */
pdf_status_t
pdf_fsys_http_file_set_pos (pdf_fsys_file_t file,
                            pdf_size_t pos)
{
  pdf_status_t rv = PDF_ERROR;
  pdf_fsys_http_file_t http_file = NULL;

  if (PDF_OK == __pdf_fsys_http_valid ( file ) )
    {
      // ok to work with

      /*
       * Need to check position specified by pos against which parts of the
       * file have been downloaded to verify that the pos is currently valid.
       * Only if valid can this command be passed on to the temp_file
       */

#warning "set_pos must validate the requested position against available data."

      http_file = (pdf_fsys_http_file_t) file->data;

      http_file->read_idx = pos;

      rv = PDF_OK;
    }

  return rv;
}

enum pdf_fsys_file_mode_e
pdf_fsys_http_file_get_mode (pdf_fsys_file_t file)
{
  enum pdf_fsys_file_mode_e rv = PDF_FSYS_OPEN_MODE_INVALID;

  if (PDF_OK == __pdf_fsys_http_valid ( file ) )
    {
      rv = PDF_FSYS_OPEN_MODE_READ;
    }

  return rv;
}


/* Set file mode info */
pdf_status_t
pdf_fsys_http_file_set_mode (pdf_fsys_file_t file,
                             enum pdf_fsys_file_mode_e new_mode)
{
  // read only filesystem - this makes no sense.

  return PDF_ERROR;
}


/* Get URL from file */
pdf_text_t
pdf_fsys_http_file_get_url (pdf_fsys_file_t file)
{
  // return URL that initially pointed to file
  pdf_text_t text_rv = NULL;

  pdf_fsys_http_file_t phttp_file;
  pdf_fsys_t pfsys;

  pdf_status_t rs;
  pdf_text_host_encoding_t encoding;


  if (PDF_OK != __pdf_fsys_http_valid ( file ) )
    {
      // not ok to work with
      return NULL;
    }


  phttp_file = (pdf_fsys_http_file_t)file->data;
  pfsys = file->fs;
  
  if ( &pdf_fsys_http_implementation == pfsys->implementation )
    {
      if ( NULL != phttp_file )
        {
          rs = pdf_text_new_from_host ( phttp_file->host_path, 
                                        phttp_file->host_path_size,
                                        encoding,
                                        &text_rv );

          if ( PDF_OK != rs )
            {
              // something bad happened
              text_rv = NULL;
            }
          else
            {
              // text_rv was initialized successfully and will be returned.
            }
        }
      else
        { // error - no http_file data associated with file
          text_rv = NULL;
        }
    }
  else
    {
      // file isn't from HTTP filesys - no URL.
      text_rv = NULL;
    }

  return text_rv;
}


/* Check if paths are same/equivalent */ 
pdf_bool_t
pdf_fsys_http_file_same_p (pdf_fsys_file_t file,
                           pdf_text_t path_name)
{
  // !*!*!*!*! Implement me! !*!*!*!*
#warning "pdf_fsys_http_file_same_p is not yet implemented."
  // check path against url for file

  if (PDF_OK == __pdf_fsys_http_valid ( file ) )
    {
      // ok to work with
    }



  return PDF_FALSE;
}

pdf_status_t
pdf_fsys_http_file_request_ria (pdf_fsys_file_t file,
                                pdf_size_t offset,
                                pdf_size_t count)
{
  // !*!*!*!*! Implement me! !*!*!*!*

#warning "pdf_fsys_http_file_request_ria is not yet implemented."

  if (PDF_OK == __pdf_fsys_http_valid ( file ) )
    {
      // ok to work with
    }



  return PDF_ERROR;
}

pdf_bool_t
pdf_fsys_http_file_has_ria (pdf_fsys_file_t file)
{
#warning "HTTP fs RIA must eventually be implemented!"
  /* This must eventually return PDF_TRUE */

  if (PDF_OK == __pdf_fsys_http_valid ( file ) )
    {
      // ok to work with
    }



  return PDF_FALSE;
}

pdf_status_t
pdf_fsys_http_file_cancel_ria (pdf_fsys_file_t file)
{
  // !*!*!*!*! Implement me! !*!*!*!*
#warning "pdf_fsys_http_file_cancel_ria is not yet implemented."
  return PDF_ERROR;
}

pdf_status_t
pdf_fsys_http_file_close (pdf_fsys_file_t file)
{
  pdf_fsys_http_file_t http_file;
  pdf_fsys_t pfsys;

  pdf_status_t rv = PDF_ERROR;

  if (PDF_OK != __pdf_fsys_http_valid ( file ) )
    {
      // not ok to work with
      return PDF_ERROR;
    }

  http_file = (pdf_fsys_http_file_t)file->data;
  pfsys = file->fs;
  
  if ( &pdf_fsys_http_implementation == pfsys->implementation )
    { // we have a file reference from this http filesystem. continue

      pthread_mutex_lock (&(http_file->mutex));
      {
        if ( http_file->open )
          {
            rv = pdf_fsys_file_close ( http_file->temp_file );
            http_file->open = PDF_FALSE;
            
            // file is now closed.

#warning "Probably the file resources need to be deallocated now."
            
          }
      }
      pthread_mutex_unlock (&(http_file->mutex));

    }

  return rv;
}

pdf_status_t
pdf_fsys_http_file_reopen (pdf_fsys_file_t file,
                           enum pdf_fsys_file_mode_e mode)
{
  // makes no sense for r/o filesystem - there is only one mode!
  return PDF_EBADPERMS;
}


/* End of pdf-fsys-http.c */

/* -*- mode: C -*- Time-stamp: "07/09/14 23:03:18 jemarch"
 *
 *       File:         pdf.c
 *       Author:       Jose E. Marchesi (jemarch@gnu.org)
 *       Date:         Thu Jul  5 21:07:10 2007
 *
 *       GNU PDF Library - Main implementation file
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

#include <config.h>

#include <string.h>
#include <pdf.h>

/* Global variables */

char* gnupdf_version = "1.3";
struct pdf_globals_s pdf_globals;

static void pdf_alloc_names (void);
static void pdf_dealloc_names (void);


/* Library initialization routine */
int
pdf_init (void)
{
  pdf_alloc_names ();

  return PDF_OK;
}


/* Library finalization routine */
int
pdf_finish (void)
{
  pdf_dealloc_names ();
  
  return PDF_OK;
}

/* Private functions */

#define PDF_ALLOC_NAME(num,str)                                         \
  pdf_globals.names[(num)] = pdf_create_name((unsigned char*) (str), strlen((str)));

static void
pdf_alloc_names (void)
{
  int i;

  i = 0;
  PDF_ALLOC_NAME(i++, PDF_S_LENGTH);
  PDF_ALLOC_NAME(i++, PDF_S_FILTER);
  PDF_ALLOC_NAME(i++, PDF_S_SUBFILTER);
  PDF_ALLOC_NAME(i++, PDF_S_DECODE);
  PDF_ALLOC_NAME(i++, PDF_S_F);
  PDF_ALLOC_NAME(i++, PDF_S_F_FILTER);
  PDF_ALLOC_NAME(i++, PDF_S_F_DECODE_PARMS);           
  PDF_ALLOC_NAME(i++, PDF_S_DL);                       
  PDF_ALLOC_NAME(i++, PDF_S_ASCII_HEX_DECODE);         
  PDF_ALLOC_NAME(i++, PDF_S_ASCII85_DECODE);           
  PDF_ALLOC_NAME(i++, PDF_S_LZW_DECODE);               
  PDF_ALLOC_NAME(i++, PDF_S_FLATE_DECODE);             
  PDF_ALLOC_NAME(i++, PDF_S_RUN_LENGTH_DECODE);        
  PDF_ALLOC_NAME(i++, PDF_S_CCITT_FAX_DECODE);         
  PDF_ALLOC_NAME(i++, PDF_S_JBIG2_DECODE);             
  PDF_ALLOC_NAME(i++, PDF_S_DCT_DECODE);               
  PDF_ALLOC_NAME(i++, PDF_S_JPX_DECODE);               
  PDF_ALLOC_NAME(i++, PDF_S_CRYPT);                    
  PDF_ALLOC_NAME(i++, PDF_S_PREDICTOR);                
  PDF_ALLOC_NAME(i++, PDF_S_COLORS);                   
  PDF_ALLOC_NAME(i++, PDF_S_BITS_PER_COMPONENT);       
  PDF_ALLOC_NAME(i++, PDF_S_COLUMNS);                  
  PDF_ALLOC_NAME(i++, PDF_S_EARLY_CHANGE);             
  PDF_ALLOC_NAME(i++, PDF_S_K);                        
  PDF_ALLOC_NAME(i++, PDF_S_END_OF_LINE);              
  PDF_ALLOC_NAME(i++, PDF_S_ENCODED_BYTE_ALIGN);       
  PDF_ALLOC_NAME(i++, PDF_S_ROWS);                     
  PDF_ALLOC_NAME(i++, PDF_S_END_OF_BLOCK);             
  PDF_ALLOC_NAME(i++, PDF_S_BLACKIS1);                 
  PDF_ALLOC_NAME(i++, PDF_S_DAMAGED_ROWS_BEFORE_ERROR);
  PDF_ALLOC_NAME(i++, PDF_S_JBIG2_GLOBALS);            
  PDF_ALLOC_NAME(i++, PDF_S_COLOR_TRANSFORM);          
  PDF_ALLOC_NAME(i++, PDF_S_TYPE);                     
  PDF_ALLOC_NAME(i++, PDF_S_SUBTYPE);                  
  PDF_ALLOC_NAME(i++, PDF_S_NAME);                     
  PDF_ALLOC_NAME(i++, PDF_S_PREV);                     
  PDF_ALLOC_NAME(i++, PDF_S_ROOT);                     
  PDF_ALLOC_NAME(i++, PDF_S_ENCRYPT);                  
  PDF_ALLOC_NAME(i++, PDF_S_ENCRYPT_METADATA);         
  PDF_ALLOC_NAME(i++, PDF_S_INFO);                     
  PDF_ALLOC_NAME(i++, PDF_S_ID);                       
  PDF_ALLOC_NAME(i++, PDF_S_FIRST);                    
  PDF_ALLOC_NAME(i++, PDF_S_EXTENDS);                  
  PDF_ALLOC_NAME(i++, PDF_S_INDEX);                    
  PDF_ALLOC_NAME(i++, PDF_S_W);                        
  PDF_ALLOC_NAME(i++, PDF_S_XREF_STM);                 
  PDF_ALLOC_NAME(i++, PDF_S_V);                        
  PDF_ALLOC_NAME(i++, PDF_S_CF);                       
  PDF_ALLOC_NAME(i++, PDF_S_CFM);                      
  PDF_ALLOC_NAME(i++, PDF_S_NONE);                     
  PDF_ALLOC_NAME(i++, PDF_S_V2);                       
  PDF_ALLOC_NAME(i++, PDF_S_AES_V2);                   
  PDF_ALLOC_NAME(i++, PDF_S_AUTH_EVENT);               
  PDF_ALLOC_NAME(i++, PDF_S_IDENTITY);                 
  PDF_ALLOC_NAME(i++, PDF_S_STM_F);                    
  PDF_ALLOC_NAME(i++, PDF_S_STR_F);                    
  PDF_ALLOC_NAME(i++, PDF_S_EFF);                      
  PDF_ALLOC_NAME(i++, PDF_S_R);                        
  PDF_ALLOC_NAME(i++, PDF_S_O);                        
  PDF_ALLOC_NAME(i++, PDF_S_U);                        
  PDF_ALLOC_NAME(i++, PDF_S_P);                        
  PDF_ALLOC_NAME(i++, PDF_S_RECIPIENTS);               
  PDF_ALLOC_NAME(i++, PDF_S_FUNCTION_TYPE);            
  PDF_ALLOC_NAME(i++, PDF_S_DOMAIN);                   
  PDF_ALLOC_NAME(i++, PDF_S_RANGE);                    
  PDF_ALLOC_NAME(i++, PDF_S_SIZE);                     
  PDF_ALLOC_NAME(i++, PDF_S_BITS_PER_SAMPLE);          
  PDF_ALLOC_NAME(i++, PDF_S_ORDER);                    
  PDF_ALLOC_NAME(i++, PDF_S_ENCODE);                   
  PDF_ALLOC_NAME(i++, PDF_S_DECODE);                   
  PDF_ALLOC_NAME(i++, PDF_S_C0);                       
  PDF_ALLOC_NAME(i++, PDF_S_C1);                       
  PDF_ALLOC_NAME(i++, PDF_S_N);                        
  PDF_ALLOC_NAME(i++, PDF_S_FUNCTIONS);                
  PDF_ALLOC_NAME(i++, PDF_S_BOUNDS);                   
  PDF_ALLOC_NAME(i++, PDF_S_VERSION);                  
  PDF_ALLOC_NAME(i++, PDF_S_PAGES);                    
  PDF_ALLOC_NAME(i++, PDF_S_PAGE_LABELS);              
  PDF_ALLOC_NAME(i++, PDF_S_NAMES);                    
  PDF_ALLOC_NAME(i++, PDF_S_DESTS);                    
  PDF_ALLOC_NAME(i++, PDF_S_VIEWER_PREFERENCES);       
  PDF_ALLOC_NAME(i++, PDF_S_PAGE_LAYOUT);              
  PDF_ALLOC_NAME(i++, PDF_S_PAGE_MODE);                
  PDF_ALLOC_NAME(i++, PDF_S_OUTLINES);                 
  PDF_ALLOC_NAME(i++, PDF_S_THREADS);                  
  PDF_ALLOC_NAME(i++, PDF_S_OPEN_ACTION);              
  PDF_ALLOC_NAME(i++, PDF_S_AA);                       
  PDF_ALLOC_NAME(i++, PDF_S_URI);                      
  PDF_ALLOC_NAME(i++, PDF_S_METADATA);                 
  PDF_ALLOC_NAME(i++, PDF_S_STRUCT_TREE_ROOT);         
  PDF_ALLOC_NAME(i++, PDF_S_MARK_INFO);                
  PDF_ALLOC_NAME(i++, PDF_S_LANG);                     
  PDF_ALLOC_NAME(i++, PDF_S_SPIDER_INFO);              
  PDF_ALLOC_NAME(i++, PDF_S_OUTPUT_INTENTS);           
  PDF_ALLOC_NAME(i++, PDF_S_PIECE_INFO);               
  PDF_ALLOC_NAME(i++, PDF_S_OC_PROPERTIES);            
  PDF_ALLOC_NAME(i++, PDF_S_PERMS);                    
  PDF_ALLOC_NAME(i++, PDF_S_LEGAL);                    
  PDF_ALLOC_NAME(i++, PDF_S_REQUIREMENTS);             
  PDF_ALLOC_NAME(i++, PDF_S_COLLECTION);               
  PDF_ALLOC_NAME(i++, PDF_S_NEEDS_RENDERING);          
  PDF_ALLOC_NAME(i++, PDF_S_PARENT);                   
  PDF_ALLOC_NAME(i++, PDF_S_KIDS);                     
  PDF_ALLOC_NAME(i++, PDF_S_COUNT);                    
  PDF_ALLOC_NAME(i++, PDF_S_LAST_MODIFIED);            
  PDF_ALLOC_NAME(i++, PDF_S_RESOURCES);                
  PDF_ALLOC_NAME(i++, PDF_S_MEDIA_BOX);                
  PDF_ALLOC_NAME(i++, PDF_S_CROP_BOX);                 
  PDF_ALLOC_NAME(i++, PDF_S_BLEED_BOX);                
  PDF_ALLOC_NAME(i++, PDF_S_TRIM_BOX);                 
  PDF_ALLOC_NAME(i++, PDF_S_ART_BOX);                  
  PDF_ALLOC_NAME(i++, PDF_S_BOX_COLOR_INFO);           
  PDF_ALLOC_NAME(i++, PDF_S_CONTENTS);                 
  PDF_ALLOC_NAME(i++, PDF_S_ROTATE);                   
  PDF_ALLOC_NAME(i++, PDF_S_GROUP);                    
  PDF_ALLOC_NAME(i++, PDF_S_THUMB);                    
  PDF_ALLOC_NAME(i++, PDF_S_B);                        
  PDF_ALLOC_NAME(i++, PDF_S_DUR);                      
  PDF_ALLOC_NAME(i++, PDF_S_TRANS);                    
  PDF_ALLOC_NAME(i++, PDF_S_ANNOTS);                   
  PDF_ALLOC_NAME(i++, PDF_S_STRUCT_PARENTS);           
  PDF_ALLOC_NAME(i++, PDF_S_PZ);                       
  PDF_ALLOC_NAME(i++, PDF_S_SEPARATION_INFO);          
  PDF_ALLOC_NAME(i++, PDF_S_TABS);                     
  PDF_ALLOC_NAME(i++, PDF_S_TEMPLATE_INSTANTIATE);     
  PDF_ALLOC_NAME(i++, PDF_S_PRES_STEPS);               
  PDF_ALLOC_NAME(i++, PDF_S_USER_UNIT);                
  PDF_ALLOC_NAME(i++, PDF_S_VP);                       
  PDF_ALLOC_NAME(i++, PDF_S_AP);                       
  PDF_ALLOC_NAME(i++, PDF_S_JAVASCRIPT);               
  PDF_ALLOC_NAME(i++, PDF_S_TEMPLATES);                
  PDF_ALLOC_NAME(i++, PDF_S_IDS);                      
  PDF_ALLOC_NAME(i++, PDF_S_URLS);                     
  PDF_ALLOC_NAME(i++, PDF_S_EMBEDDED_FILES);           
  PDF_ALLOC_NAME(i++, PDF_S_ALTERNATE_PRESENTATIONS);  
  PDF_ALLOC_NAME(i++, PDF_S_RENDITIONS);               
  PDF_ALLOC_NAME(i++, PDF_S_EXT_G_STATE);              
  PDF_ALLOC_NAME(i++, PDF_S_COLOR_SPACE);              
  PDF_ALLOC_NAME(i++, PDF_S_PATTERN);                  
  PDF_ALLOC_NAME(i++, PDF_S_SHADING);                  
  PDF_ALLOC_NAME(i++, PDF_S_XOBJECT);                  
  PDF_ALLOC_NAME(i++, PDF_S_FONT);                     
  PDF_ALLOC_NAME(i++, PDF_S_PROC_SET);                 
  PDF_ALLOC_NAME(i++, PDF_S_PROPERTIES);               
  PDF_ALLOC_NAME(i++, PDF_S_LIMITS);                   
  PDF_ALLOC_NAME(i++, PDF_S_NUMS);                     
  PDF_ALLOC_NAME(i++, PDF_S_FS);                       
  PDF_ALLOC_NAME(i++, PDF_S_F);                        
  PDF_ALLOC_NAME(i++, PDF_S_UF);                       
  PDF_ALLOC_NAME(i++, PDF_S_DOS);                      
  PDF_ALLOC_NAME(i++, PDF_S_MAC);                      
  PDF_ALLOC_NAME(i++, PDF_S_UNIX);                     
  PDF_ALLOC_NAME(i++, PDF_S_EF);                       
  PDF_ALLOC_NAME(i++, PDF_S_RF);                       
  PDF_ALLOC_NAME(i++, PDF_S_DESC);                     
  PDF_ALLOC_NAME(i++, PDF_S_CI);                       
  PDF_ALLOC_NAME(i++, PDF_S_PARAMS);                   
  PDF_ALLOC_NAME(i++, PDF_S_CREATION_DATE);            
  PDF_ALLOC_NAME(i++, PDF_S_MOD_DATE);                 
  PDF_ALLOC_NAME(i++, PDF_S_CHECKSUM);                 
  PDF_ALLOC_NAME(i++, PDF_S_CREATOR);                  
  PDF_ALLOC_NAME(i++, PDF_S_RES_FORK);                 
  PDF_ALLOC_NAME(i++, PDF_S_D);                        
  PDF_ALLOC_NAME(i++, PDF_S_ACROFORM);                 
}

static void
pdf_dealloc_names (void)
{
  int i;

  for (i = 0; i < PDF_NAMES; i++)
    {
      pdf_destroy_obj (pdf_globals.names[i]);
    }
}

/* End of pdf.c */

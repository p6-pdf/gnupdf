/* -*- mode: C -*- Time-stamp: ""
 *
 *       File:         pdf-text-generate-ucd.c
 *       Date:         Sat Feb 02 12:46:05 2008
 *
 *       GNU PDF Library - Generate Unicode Character Database source file
 *
 */

/* Copyright (C) 2008 Free Software Foundation, Inc. */

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

#include <stdio.h>
#include <string.h>
#include <math.h>
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#else
#include <stdlib.h>
#endif /* HAVE_MALLOC_H */


/*
 http://www.unicode.org/Public/UNIDATA/UCD.html#UCD_File_Format
 */

#ifndef pdf_u32_t
 #define pdf_u32_t int32_t
#endif


int ascii_hex_to_decimal(pdf_u32_t *out, const char *ascii_hex)
{
  int length;
  pdf_u32_t val;
  char *start;
  int i;
  int power;
  int numeric_value;
  
  if(strlen(ascii_hex) == 0)
    {
      /* Couldn't parse anything */
      return -1;
    }
  
  /* Skip '0' at the beginning or even '0x000..' */
  start = (char *)ascii_hex;
  while((*start == '0') || (*start == 'x') || (*start == 'X'))
    {
      start++;
    }
  
  if(strlen(start) == 0)
    {
      /* Then we got 0x0000 at the input... */
      *out = 0;
      return 0;
    }
  
  /* Example: 0x159D is converted to...
   *    num = (D*16^0) + (9*16^1) + (5*16^2) + (1*16^3)
   */
  val = 0;
  for(i=(strlen(start)-1), power = 0; i>=0; i--, power++)
    {
      if((start[i] >='0') && (start[i] <= '9'))
        {
          numeric_value = start[i] - '0';
        }
      else if((start[i] >='A') && (start[i] <= 'F'))
        {
          numeric_value = start[i] - 'A' + 10;
        }
      else if((start[i] >='a') && (start[i] <= 'f'))
        {
          numeric_value = start[i] - 'a' + 10;
        }
      else
        {
          fprintf(stderr, "\nUnexpected char '%c' in ascii-hex string\n",
                  start[i]);
          return -1;
        }
      val += (pow(16,power)) * numeric_value;
    }
  
  /* Set output value */
  *out = val;
  return 0;
}



/* ------------------------- PARSING UnicodeData.txt -------------------------*/


#define MAX_LINE_LENGTH 1000

#define UNICODE_POINT_LENGTH 10
#define NAME_LENGTH 255
#define GENERAL_CATEGORY_LENGTH 3
#define CANONICAL_COMB_CLASS_LENGTH 5
#define BIDI_CLASS_LENGTH 4
#define DECOMP_TYPE_AND_MAP_LENGTH 200
#define NUMERIC_FIELD_LENGTH 10
#define BIDI_MIRRORED_LENGTH 3
#define UNICODE_1_NAME_LENGTH 255
#define ISO_COMMENT_LENGTH 255
#define CASE_MAPPING_LENGTH 10

typedef struct _ucd_entry {
  char unicode_point [UNICODE_POINT_LENGTH];
  char name [NAME_LENGTH];
  char general_category [GENERAL_CATEGORY_LENGTH];
  char canonical_combining_class [CANONICAL_COMB_CLASS_LENGTH];
  char bidi_class [BIDI_CLASS_LENGTH];
  char decomposition_type_and_mapping [DECOMP_TYPE_AND_MAP_LENGTH];
  char numeric_field_6 [NUMERIC_FIELD_LENGTH];
  char numeric_field_7 [NUMERIC_FIELD_LENGTH];
  char numeric_field_8 [NUMERIC_FIELD_LENGTH];
  char bidi_mirrored [BIDI_MIRRORED_LENGTH];
  char unicode_1_name [UNICODE_1_NAME_LENGTH];
  char iso_comment [ISO_COMMENT_LENGTH];
  char simple_uppercase_mapping [CASE_MAPPING_LENGTH];
  char simple_lowercase_mapping [CASE_MAPPING_LENGTH];
  char simple_titlecase_mapping [CASE_MAPPING_LENGTH];
} ucd_entry;



int parse_UnicodeData_line_field(char *place_to_store, int max_field_length,
                         const char *fieldstart, char **field_end)
{
  /* Fields are separated by ';' or '\n' (or '\r') */
  char *walker;
  int length;
  
  /* Look for field separator */
  walker = (char *)fieldstart;
  while((walker != NULL) && \
        (*walker != ';') && \
        (*walker != '\n') && \
        (*walker != '\r'))
    {
      walker ++;
    }

  /* Compute field length */
  length = walker - fieldstart;

  if(length > max_field_length)
    {
      printf("[Error] '%d' bytes are required for the field, "
             "but only '%d' are available\n", length, max_field_length);
      return -1;
    }
  
  /* Copy line contents */
  memcpy(place_to_store, fieldstart, length);

  /* Set last NUL char just in case */
  place_to_store[length] = '\0';
  
  /* Set next field start */
  *field_end = walker+1;
  
  return 0;
}



int parse_UnicodeData_line(ucd_entry *p_entry, const char *line)
{
  char *stop;
  char *start;
  
  start = (char *)line;

  /* Get entry fields! */
  if(parse_UnicodeData_line_field(p_entry->unicode_point, \
                                  UNICODE_POINT_LENGTH-1, start, &stop)!=0)
      return -1;
  start = stop;
  if(parse_UnicodeData_line_field(p_entry->name, \
                                  NAME_LENGTH-1, start, &stop)!=0)
      return -1;
  start = stop;
  if(parse_UnicodeData_line_field(p_entry->general_category, \
                                  GENERAL_CATEGORY_LENGTH-1, start, &stop)!=0)
    return -1;
  start = stop;
  if(parse_UnicodeData_line_field(p_entry->canonical_combining_class, \
                                  CANONICAL_COMB_CLASS_LENGTH-1, start, &stop)!=0)
    return -1;
  start = stop;
  if(parse_UnicodeData_line_field(p_entry->bidi_class, \
                                  BIDI_CLASS_LENGTH-1, start, &stop)!=0)
    return -1;
  start = stop;
  if(parse_UnicodeData_line_field(p_entry->decomposition_type_and_mapping, \
                                  DECOMP_TYPE_AND_MAP_LENGTH-1, start, &stop)!=0)
    return -1;
  start = stop;
  if(parse_UnicodeData_line_field(p_entry->numeric_field_6, \
                                  NUMERIC_FIELD_LENGTH-1, start, &stop)!=0)
    return -1;
  start = stop;
  if(parse_UnicodeData_line_field(p_entry->numeric_field_7, \
                                  NUMERIC_FIELD_LENGTH-1, start, &stop)!=0)
    return -1;
  start = stop;
  if(parse_UnicodeData_line_field(p_entry->numeric_field_8, \
                                  NUMERIC_FIELD_LENGTH-1, start, &stop)!=0)
    return -1;
  start = stop;
  if(parse_UnicodeData_line_field(p_entry->bidi_mirrored, \
                                  BIDI_MIRRORED_LENGTH-1, start, &stop)!=0)
    return -1;
  start = stop;
  if(parse_UnicodeData_line_field(p_entry->unicode_1_name, \
                                  UNICODE_1_NAME_LENGTH-1, start, &stop)!=0)
    return -1;
  start = stop;
  if(parse_UnicodeData_line_field(p_entry->iso_comment, \
                                  ISO_COMMENT_LENGTH-1, start, &stop)!=0)
    return -1;
  start = stop;
  if(parse_UnicodeData_line_field(p_entry->simple_uppercase_mapping, \
                                  CASE_MAPPING_LENGTH-1, start, &stop)!=0)
    return -1;
  start = stop;
  if(parse_UnicodeData_line_field(p_entry->simple_lowercase_mapping, \
                                  CASE_MAPPING_LENGTH-1, start, &stop)!=0)
    return -1;
  start = stop;
  if(parse_UnicodeData_line_field(p_entry->simple_titlecase_mapping, \
                                  CASE_MAPPING_LENGTH-1, start, &stop)!=0)
    return -1;
  start = stop;
  
  return 0; 
}


/* ------------------- CREATING UnicodeDataCaseInfo.c ------------------------*/

#define OUTPUT_UNICODEDATA_CASE_INFO "UnicodeDataCaseInfo.c"

#define MAX_CASE_ENTRIES 2500 /* Much more than required in Unicode 5.0,
*  should be enough for future updates of the
*  standard */


int print_UnicodeData_CASE_info(const ucd_entry *p_entry)
{
  if((strlen(p_entry->simple_uppercase_mapping) > 0) || \
     (strlen(p_entry->simple_lowercase_mapping) > 0) || \
     (strlen(p_entry->simple_titlecase_mapping) > 0))
    {
      printf("point:'%s' upper:'%s' lower:'%s' title:'%s'\n",
             p_entry->unicode_point,
             (strlen(p_entry->simple_uppercase_mapping) > 0) ? \
             p_entry->simple_uppercase_mapping:p_entry->unicode_point,
             (strlen(p_entry->simple_lowercase_mapping) > 0) ? \
             p_entry->simple_lowercase_mapping:p_entry->unicode_point,
             (strlen(p_entry->simple_titlecase_mapping) > 0) ? \
             p_entry->simple_titlecase_mapping:p_entry->unicode_point);
    }
  else
    {
      printf("No case info for point '%s'\n", p_entry->unicode_point);
    }
  return 0;
}

int create_UnicodeData_CASE_header(FILE *pf)
{
  if(pf != NULL)
    {
      fprintf(pf, "\n\n\n"
              "typedef struct _unicode_case_info_s {\n"
              "  pdf_u32_t case_info[4];\n"
              "} unicode_case_info_t;\n\n"
              "static unicode_case_info_t unicode_case_info[] = {\n");
    }
  return 0;
}

int create_UnicodeData_CASE_INTERVAL_header(FILE *pf)
{
  if(pf != NULL)
    {
      fprintf(pf, "\n\n\n"
              "typedef struct _unicode_case_interval_s {\n"
              "  pdf_u32_t interval_start;\n"
              "  pdf_u32_t interval_stop;\n"
              "  long delta;\n"
              "} unicode_case_interval_t;\n\n"
              "static unicode_case_interval_t unicode_case_interval[] = {\n");
    }
  return 0;
}

int create_UnicodeData_CASE_trailer(FILE *pf)
{
  if(pf != NULL)
    {
      fprintf(pf, "};\n\n\n");
    }
  return 0;
}

int create_UnicodeData_CASE_INTERVAL_trailer(FILE *pf)
{
  if(pf != NULL)
    {
      fprintf(pf, "};\n\n\n");
    }
  return 0;
}

int add_entry_to_UnicodeData_CASE(FILE *pf, const ucd_entry *p_entry)
{
  if(pf != NULL)
    {
      if((strlen(p_entry->simple_uppercase_mapping) > 0) || \
         (strlen(p_entry->simple_lowercase_mapping) > 0) || \
         (strlen(p_entry->simple_titlecase_mapping) > 0))
        {
          fprintf(pf, "\t{ 0x%s, 0x%s, 0x%s, 0x%s },\n",
                  p_entry->unicode_point,
                  (strlen(p_entry->simple_uppercase_mapping) > 0) ? \
                  p_entry->simple_uppercase_mapping:p_entry->unicode_point,
                  (strlen(p_entry->simple_lowercase_mapping) > 0) ? \
                  p_entry->simple_lowercase_mapping:p_entry->unicode_point,
                  (strlen(p_entry->simple_titlecase_mapping) > 0) ? \
                  p_entry->simple_titlecase_mapping:p_entry->unicode_point);
          return 0;
        }
    }
  return -1;
}


int add_entry_to_UnicodeData_CASE_INTERVAL(FILE *pf,
                                           pdf_u32_t interval_start,
                                           pdf_u32_t interval_stop,
                                           long delta)
{
  if(pf != NULL)
    {
      fprintf(pf, "  { 0x%.6X, 0x%.6X, %ld },\n",
              interval_start, interval_stop, delta);
    }
  return 0;
}


int compute_UnicodeData_CASE(const char *UnicodeDataFile)
{
  FILE *pf;
  FILE *pf_out;
  ucd_entry entry;
  long count;
  long count_intervals;
  long index;
  char line [MAX_LINE_LENGTH];
  pdf_u32_t array_entries[MAX_CASE_ENTRIES];
  long deltaToIndex = 0;
  long oldDeltaToIndex = 0;
  pdf_u32_t prev_point = 0;
  pdf_u32_t interval_start = 0;
  pdf_u32_t interval_stop = 0;
  
  /* Create Standard Casing source file, involving not only the specific
   *  case changes, but also the intervals of Unicode points with casing
   *  info. */
  
  pf = fopen(UnicodeDataFile, "r");
  if(pf == NULL)
    {
      fprintf(stderr,"\nFile '%s' can't be opened for reading\n", \
              UnicodeDataFile);
      return -2;
    }
  
  pf_out = fopen(OUTPUT_UNICODEDATA_CASE_INFO, "w");
  if(pf_out == NULL)
    {
      fprintf(stderr,"\nFile '%s' can't be opened for writting\n", \
              OUTPUT_UNICODEDATA_CASE_INFO);
      return -3;
    }
  
  
  /* Set header */
  create_UnicodeData_CASE_header(pf_out);
  
  memset(&entry, 0, sizeof(entry));
  memset(&line[0], 0, MAX_LINE_LENGTH);
  count = 0;
  while(fgets(line, MAX_LINE_LENGTH-1, pf)!=NULL)
    {
      /* Parse line */
      parse_UnicodeData_line(&entry, line);
      
      /* Print in screen */
      print_UnicodeData_CASE_info(&entry);
      
      /* Add to output file */
      if(add_entry_to_UnicodeData_CASE(pf_out, &entry) >=0)
        {
          /* If added correctly to output source file, add it to the internal
           *  array, so that intervals can be directly computed. */
          pdf_u32_t unicode_point;
          
          /* Transform unicode point value from ascii-hex to 32bit integer */
          if(ascii_hex_to_decimal(&unicode_point, entry.unicode_point)!=0)
            {
              fprintf(stderr, "\nError converting ASCII-HEX to integer...\n");
              return -1;
            }
          if(count == MAX_CASE_ENTRIES)
            {
              fprintf(stderr,
                      "\nError: Maximum value of case entries reached\n");
              return -2;
            }
          /* If conversion went ok, add to array of entries */
          array_entries[count] = unicode_point;
          /* And update count of elements in array */
          count++;
        }
      
      /* Clear structure  and line contents */
      memset(&entry, 0, sizeof(entry));
      memset(&line[0], 0, MAX_LINE_LENGTH);
    }
  
  /* Set trailer for the case changes list */
  create_UnicodeData_CASE_trailer(pf_out);
  
  /* Input file can be closed now */
  fclose(pf);
  
  
  /*------ Compute intervals array ----------*/
  
  create_UnicodeData_CASE_INTERVAL_header(pf_out);
  
  /* Start first interval */
  interval_start = array_entries[0];
  oldDeltaToIndex = 0;
  count_intervals = 0;
  for(index = 0; index < count; index++)
    {
      /* Compute the deltaToIndex value, which is the correction to be applied
       *  to the unicode point value to get the index of the array previously
       *  created */
      deltaToIndex = array_entries[index] - index;
      
      /* Check if a new interval started. This means that there was a gap
       *  between unicode points or a difference between consecutive delta to
       *  index values... */
      if((index > 0) && \
         ((array_entries[index] != (array_entries[index-1] +1) || \
           (oldDeltaToIndex != deltaToIndex))))
        {
          /* Set the previous point as last interval point */
          interval_stop = array_entries[index-1];
          
          /* Add interval */
          add_entry_to_UnicodeData_CASE_INTERVAL(pf_out,
                                                 interval_start,
                                                 interval_stop,
                                                 oldDeltaToIndex);
          count_intervals++;
          
          /* Set new interval start */
          interval_start = array_entries[index]; 
        }
      
      oldDeltaToIndex = deltaToIndex;
    }
  
  /* Last interval... */
  interval_stop = array_entries[index-1];
  add_entry_to_UnicodeData_CASE_INTERVAL(pf_out,
                                         interval_start,
                                         interval_stop,
                                         oldDeltaToIndex);
  count_intervals++;
  
  create_UnicodeData_CASE_INTERVAL_trailer(pf_out);
  
  /* Close output file and exit */
  fclose(pf_out);
  
  printf("A total of '%ld' entries added in the case info array\n", count);
  printf("A total of '%ld' intervals added\n", count_intervals);
  
  return 0;
}




/* ------------------- CREATING UnicodeDataGenCatInfo.c ------------------------*/

#define OUTPUT_UNICODEDATA_GENCAT_INFO "UnicodeDataGenCatInfo.c"

#define MAX_GENCAT_ENTRIES 25000 /* Much more than required in Unicode 5.0,
                                *  should be enough for future updates of the
                                *  standard */


int print_UnicodeData_GENCAT_info(const ucd_entry *p_entry)
{
  if(strlen(p_entry->general_category) > 0)
    {
      printf("point:'%s' general category:'%s'\n",
             p_entry->unicode_point,
             p_entry->general_category);
    }
  else
    {
      printf("No general category info for point '%s'\n",
             p_entry->unicode_point);
    }
  return 0;
}

int create_UnicodeData_GENCAT_header(FILE *pf)
{
  if(pf != NULL)
    {
      fprintf(pf, "\n\n\n"
                  "typedef enum {\n"
                  "  UNICODE_GENCAT_Lu, /* Letter, Uppercase */\n" 
                  "  UNICODE_GENCAT_Ll, /* Letter, Lowercase */\n"
                  "  UNICODE_GENCAT_Lt, /* Letter, Titlecase */\n"
                  "  UNICODE_GENCAT_Lm, /* Letter, Modifier */\n"
                  "  UNICODE_GENCAT_Lo, /* Letter, Other */\n"
                  "  UNICODE_GENCAT_Mn, /* Mark, Nonspacing */\n"
                  "  UNICODE_GENCAT_Mc, /* Mark, Spacing Combining */\n"
                  "  UNICODE_GENCAT_Me, /* Mark, Enclosing */\n"
                  "  UNICODE_GENCAT_Nd, /* Number, Decimal Digit */\n"
                  "  UNICODE_GENCAT_Nl, /* Number, Letter */\n"
                  "  UNICODE_GENCAT_No, /* Number, Other */\n"
                  "  UNICODE_GENCAT_Pc, /* Punctuation, Connector */\n"
                  "  UNICODE_GENCAT_Pd, /* Punctuation, Dash */\n"
                  "  UNICODE_GENCAT_Ps, /* Punctuation, Open */\n"
                  "  UNICODE_GENCAT_Pe, /* Punctuation, Close */\n"
                  "  UNICODE_GENCAT_Pi, /* Punctuation, Initial quote */\n"
                  "  UNICODE_GENCAT_Pf, /* Punctuation, Final quote */\n"
                  "  UNICODE_GENCAT_Po, /* Punctuation, Other */\n"
                  "  UNICODE_GENCAT_Sm, /* Symbol, Math */\n"
                  "  UNICODE_GENCAT_Sc, /* Symbol, Currency */\n"
                  "  UNICODE_GENCAT_Sk, /* Symbol, Modifier */\n"
                  "  UNICODE_GENCAT_So, /* Symbol, Other */\n"
                  "  UNICODE_GENCAT_Zs, /* Separator, Space */\n"
                  "  UNICODE_GENCAT_Zl, /* Separator, Line */\n"
                  "  UNICODE_GENCAT_Zp, /* Separator, Paragraph */\n"
                  "  UNICODE_GENCAT_Cc, /* Other, Control */\n"
                  "  UNICODE_GENCAT_Cf, /* Other, Format */\n"
                  "  UNICODE_GENCAT_Cs, /* Other, Surrogate */\n"
                  "  UNICODE_GENCAT_Co, /* Other, Private use */\n"
                  "  UNICODE_GENCAT_Cn  /* Other, Not assigned */\n"
                  "} unicode_gencat_info_enum;\n\n");
      
      fprintf(pf, "\n"
                  "typedef struct _unicode_gencat_info_s {\n"
                  "  pdf_u32_t unicode_point;\n"
                  "  unicode_gencat_info_enum gencat;\n"
                  "} unicode_gencat_info_t;\n\n"
                  "static unicode_gencat_info_t unicode_gencat_info[] = {\n");
    }
  return 0;
}

int create_UnicodeData_GENCAT_INTERVAL_header(FILE *pf)
{
  if(pf != NULL)
    {
      fprintf(pf, "\n\n\n"
              "typedef struct _unicode_gencat_interval_s {\n"
              "  pdf_u32_t interval_start;\n"
              "  pdf_u32_t interval_stop;\n"
              "  long delta;\n"
              "} unicode_gencat_interval_t;\n\n"
              "static unicode_gencat_interval_t unicode_gencat_interval[] = {\n");
    }
  return 0;
}

int create_UnicodeData_GENCAT_trailer(FILE *pf)
{
  if(pf != NULL)
    {
      fprintf(pf, "};\n\n\n");
    }
  return 0;
}

int create_UnicodeData_GENCAT_INTERVAL_trailer(FILE *pf)
{
  if(pf != NULL)
    {
      fprintf(pf, "};\n\n\n");
    }
  return 0;
}

int add_entry_to_UnicodeData_GENCAT(FILE *pf, const ucd_entry *p_entry)
{
  if(pf != NULL)
    {
      if(strlen(p_entry->general_category) > 0)
        {
          fprintf(pf, "\t{ 0x%s, UNICODE_GENCAT_%.2s},\n",
                  p_entry->unicode_point,
                  p_entry->general_category);
          return 0;
        }
    }
  return -1;
}


int add_entry_to_UnicodeData_GENCAT_INTERVAL(FILE *pf,
                                           pdf_u32_t interval_start,
                                           pdf_u32_t interval_stop,
                                           long delta)
{
  if(pf != NULL)
    {
      fprintf(pf, "  { 0x%.6X, 0x%.6X, %ld },\n",
              interval_start, interval_stop, delta);
    }
  return 0;
}

                                               
int compute_UnicodeData_GENCAT(const char *UnicodeDataFile)
{
  FILE *pf;
  FILE *pf_out;
  ucd_entry entry;
  long count;
  long count_intervals;
  long index;
  char line [MAX_LINE_LENGTH];
  pdf_u32_t array_entries[MAX_GENCAT_ENTRIES];
  long deltaToIndex = 0;
  long oldDeltaToIndex = 0;
  pdf_u32_t prev_point = 0;
  pdf_u32_t interval_start = 0;
  pdf_u32_t interval_stop = 0;

  /* Create Standard Casing source file, involving not only the specific
   *  case changes, but also the intervals of Unicode points with casing
   *  info. */
  
  pf = fopen(UnicodeDataFile, "r");
  if(pf == NULL)
    {
      fprintf(stderr,"\nFile '%s' can't be opened for reading\n", \
              UnicodeDataFile);
      return -2;
    }
  
  pf_out = fopen(OUTPUT_UNICODEDATA_GENCAT_INFO, "w");
  if(pf_out == NULL)
    {
      fprintf(stderr,"\nFile '%s' can't be opened for writting\n", \
              OUTPUT_UNICODEDATA_GENCAT_INFO);
      return -3;
    }
  
  
  /* Set header */
  create_UnicodeData_GENCAT_header(pf_out);

  memset(&entry, 0, sizeof(entry));
  memset(&line[0], 0, MAX_LINE_LENGTH);
  count = 0;
  while(fgets(line, MAX_LINE_LENGTH-1, pf)!=NULL)
    {
      /* Parse line */
      parse_UnicodeData_line(&entry, line);
      
      /* Print in screen */
      print_UnicodeData_GENCAT_info(&entry);
      
      /* Add to output file */
      if(add_entry_to_UnicodeData_GENCAT(pf_out, &entry) >=0)
        {
          /* If added correctly to output source file, add it to the internal
           *  array, so that intervals can be directly computed. */
          pdf_u32_t unicode_point;
          
          /* Transform unicode point value from ascii-hex to 32bit integer */
          if(ascii_hex_to_decimal(&unicode_point, entry.unicode_point)!=0)
            {
              fprintf(stderr, "\nError converting ASCII-HEX to integer...\n");
              return -1;
            }
          if(count == MAX_GENCAT_ENTRIES)
            {
              fprintf(stderr,
                      "\nError: Maximum value of case entries reached\n");
              return -2;
            }
          /* If conversion went ok, add to array of entries */
          array_entries[count] = unicode_point;
          /* And update count of elements in array */
          count++;
        }
      
      /* Clear structure  and line contents */
      memset(&entry, 0, sizeof(entry));
      memset(&line[0], 0, MAX_LINE_LENGTH);
    }
  
  /* Set trailer for the gencat changes list */
  create_UnicodeData_GENCAT_trailer(pf_out);
  
  /* Input file can be closed now */
  fclose(pf);

  
  /*------ Compute intervals array ----------*/

  create_UnicodeData_GENCAT_INTERVAL_header(pf_out);
  
  /* Start first interval */
  interval_start = array_entries[0];
  oldDeltaToIndex = 0;
  count_intervals = 0;
  for(index = 0; index < count; index++)
    {
      /* Compute the deltaToIndex value, which is the correction to be applied
       *  to the unicode point value to get the index of the array previously
       *  created */
      deltaToIndex = array_entries[index] - index;
  
      /* Check if a new interval started. This means that there was a gap
       *  between unicode points or a difference between consecutive delta to
       *  index values... */
      if((index > 0) && \
         ((array_entries[index] != (array_entries[index-1] +1) || \
          (oldDeltaToIndex != deltaToIndex))))
        {
          /* Set the previous point as last interval point */
          interval_stop = array_entries[index-1];
          
          /* Add interval */
          add_entry_to_UnicodeData_GENCAT_INTERVAL(pf_out,
                                                   interval_start,
                                                   interval_stop,
                                                   oldDeltaToIndex);
          count_intervals++;

          /* Set new interval start */
          interval_start = array_entries[index]; 
        }
      
      oldDeltaToIndex = deltaToIndex;
    }
  
  /* Last interval... */
  interval_stop = array_entries[index-1];
  add_entry_to_UnicodeData_GENCAT_INTERVAL(pf_out,
                                           interval_start,
                                           interval_stop,
                                           oldDeltaToIndex);
  count_intervals++;

  create_UnicodeData_GENCAT_INTERVAL_trailer(pf_out);

  /* Close output file and exit */
  fclose(pf_out);
  
  printf("A total of '%ld' entries added in the GENCAT array\n", count);
  printf("A total of '%ld' intervals added\n", count_intervals);
  
  return 0;
}

/* ------------------- CREATING UnicodeDataCombClassInfo.c ------------------------*/

#define OUTPUT_UNICODEDATA_COMBCLASS_INFO "UnicodeDataCombClassInfo.c"

#define MAX_COMBCLASS_ENTRIES 25000 /* Much more than required in Unicode 5.0,
*  should be enough for future updates of the
*  standard */


int print_UnicodeData_COMBCLASS_info(const ucd_entry *p_entry)
{
  if(strlen(p_entry->canonical_combining_class) > 0)
    {
      printf("point:'%s' combining class:'%s'\n",
             p_entry->unicode_point,
             p_entry->canonical_combining_class);
    }
  else
    {
      printf("No combining class info for point '%s'\n",
             p_entry->unicode_point);
    }
  return 0;
}

int create_UnicodeData_COMBCLASS_header(FILE *pf)
{
  if(pf != NULL)
    {
      fprintf(pf, "\n"
              "typedef struct _unicode_combclass_info_s {\n"
              "  pdf_u32_t unicode_point;\n"
              "  pdf_u8_t combining_class;\n"
              "} unicode_combclass_info_t;\n\n"
              "static unicode_combclass_info_t unicode_combclass_info[] = {\n");
    }
  return 0;
}

int create_UnicodeData_COMBCLASS_INTERVAL_header(FILE *pf)
{
  if(pf != NULL)
    {
      fprintf(pf, "\n\n\n"
              "typedef struct _unicode_combclass_interval_s {\n"
              "  pdf_u32_t interval_start;\n"
              "  pdf_u32_t interval_stop;\n"
              "  long delta;\n"
              "} unicode_combclass_interval_t;\n\n"
              "static unicode_combclass_interval_t "
              "unicode_combclass_interval[] = {\n");
    }
  return 0;
}

int create_UnicodeData_COMBCLASS_trailer(FILE *pf)
{
  if(pf != NULL)
    {
      fprintf(pf, "};\n\n\n");
    }
  return 0;
}

int create_UnicodeData_COMBCLASS_INTERVAL_trailer(FILE *pf)
{
  if(pf != NULL)
    {
      fprintf(pf, "};\n\n\n");
    }
  return 0;
}

int add_entry_to_UnicodeData_COMBCLASS(FILE *pf, const ucd_entry *p_entry)
{
  if(pf != NULL)
    {
      if(strlen(p_entry->canonical_combining_class) > 0)
        {
          fprintf(pf, "\t{ 0x%s, %s},\n",
                  p_entry->unicode_point,
                  p_entry->canonical_combining_class);
          return 0;
        }
    }
  return -1;
}


int add_entry_to_UnicodeData_COMBCLASS_INTERVAL(FILE *pf,
                                                pdf_u32_t interval_start,
                                                pdf_u32_t interval_stop,
                                                long delta)
{
  if(pf != NULL)
    {
      fprintf(pf, "  { 0x%.6X, 0x%.6X, %ld },\n",
              interval_start, interval_stop, delta);
    }
  return 0;
}


int compute_UnicodeData_COMBCLASS(const char *UnicodeDataFile)
{
  FILE *pf;
  FILE *pf_out;
  ucd_entry entry;
  long count;
  long count_intervals;
  long index;
  char line [MAX_LINE_LENGTH];
  pdf_u32_t array_entries[MAX_COMBCLASS_ENTRIES];
  long deltaToIndex = 0;
  long oldDeltaToIndex = 0;
  pdf_u32_t prev_point = 0;
  pdf_u32_t interval_start = 0;
  pdf_u32_t interval_stop = 0;
  
  /* Create Standard Casing source file, involving not only the specific
   *  case changes, but also the intervals of Unicode points with casing
   *  info. */
  
  pf = fopen(UnicodeDataFile, "r");
  if(pf == NULL)
    {
      fprintf(stderr,"\nFile '%s' can't be opened for reading\n", \
              UnicodeDataFile);
      return -2;
    }
  
  pf_out = fopen(OUTPUT_UNICODEDATA_COMBCLASS_INFO, "w");
  if(pf_out == NULL)
    {
      fprintf(stderr,"\nFile '%s' can't be opened for writting\n", \
              OUTPUT_UNICODEDATA_COMBCLASS_INFO);
      return -3;
    }
  
  
  /* Set header */
  create_UnicodeData_COMBCLASS_header(pf_out);
  
  memset(&entry, 0, sizeof(entry));
  memset(&line[0], 0, MAX_LINE_LENGTH);
  count = 0;
  while(fgets(line, MAX_LINE_LENGTH-1, pf)!=NULL)
    {
      /* Parse line */
      parse_UnicodeData_line(&entry, line);
      
      /* Print in screen */
      print_UnicodeData_COMBCLASS_info(&entry);
      
      /* Add to output file ONLY if COMB CLASS is NOT 0 */
      if((strcmp(entry.canonical_combining_class, "0")!=0) && \
         (add_entry_to_UnicodeData_COMBCLASS(pf_out, &entry) >=0))
        {
          /* If added correctly to output source file, add it to the internal
           *  array, so that intervals can be directly computed. */
          pdf_u32_t unicode_point;
          
          /* Transform unicode point value from ascii-hex to 32bit integer */
          if(ascii_hex_to_decimal(&unicode_point, entry.unicode_point)!=0)
            {
              fprintf(stderr, "\nError converting ASCII-HEX to integer...\n");
              return -1;
            }
          if(count == MAX_COMBCLASS_ENTRIES)
            {
              fprintf(stderr,
                      "\nError: Maximum value of case entries reached\n");
              return -2;
            }
          /* If conversion went ok, add to array of entries */
          array_entries[count] = unicode_point;
          /* And update count of elements in array */
          count++;
        }
      
      /* Clear structure  and line contents */
      memset(&entry, 0, sizeof(entry));
      memset(&line[0], 0, MAX_LINE_LENGTH);
    }
  
  /* Set trailer for the gencat changes list */
  create_UnicodeData_COMBCLASS_trailer(pf_out);
  
  /* Input file can be closed now */
  fclose(pf);
  
  
  /*------ Compute intervals array ----------*/
  
  create_UnicodeData_COMBCLASS_INTERVAL_header(pf_out);
  
  /* Start first interval */
  interval_start = array_entries[0];
  oldDeltaToIndex = 0;
  count_intervals = 0;
  for(index = 0; index < count; index++)
    {
      /* Compute the deltaToIndex value, which is the correction to be applied
       *  to the unicode point value to get the index of the array previously
       *  created */
      deltaToIndex = array_entries[index] - index;
      
      /* Check if a new interval started. This means that there was a gap
       *  between unicode points or a difference between consecutive delta to
       *  index values... */
      if((index > 0) && \
         ((array_entries[index] != (array_entries[index-1] +1) || \
           (oldDeltaToIndex != deltaToIndex))))
        {
          /* Set the previous point as last interval point */
          interval_stop = array_entries[index-1];
          
          /* Add interval */
          add_entry_to_UnicodeData_COMBCLASS_INTERVAL(pf_out,
                                                      interval_start,
                                                      interval_stop,
                                                      oldDeltaToIndex);
          count_intervals++;
          
          /* Set new interval start */
          interval_start = array_entries[index]; 
        }
      
      oldDeltaToIndex = deltaToIndex;
    }
  
  /* Last interval... */
  interval_stop = array_entries[index-1];
  add_entry_to_UnicodeData_COMBCLASS_INTERVAL(pf_out,
                                              interval_start,
                                              interval_stop,
                                              oldDeltaToIndex);
  count_intervals++;
  
  create_UnicodeData_COMBCLASS_INTERVAL_trailer(pf_out);
  
  /* Close output file and exit */
  fclose(pf_out);
  
  printf("A total of '%ld' entries added in the COMBCLASS array\n", count);
  printf("A total of '%ld' intervals added\n", count_intervals);
  
  return 0;
}






/* ------------------- CREATING WordBreakProperty.c --------------------------*/

#define OUTPUT_UNICODEDATA_WORDBREAK "WordBreakProperty.c"

#define MAX_WORDBREAK_ENTRIES 2000 /* Much more than required in Unicode 5.0,
                                    *  should be enough for future updates of 
                                    *  the standard */

#ifndef UNICODE_POINT_LENGTH
#define UNICODE_POINT_LENGTH 10
#endif
#define WORDBREAK_PROPERTY_LENGTH  40
#define WORDBREAK_COMMENT_LENGTH  100
typedef struct _ucd_wordbreak_entry {
  char unicode_point_start   [UNICODE_POINT_LENGTH];
  char unicode_point_stop    [UNICODE_POINT_LENGTH];
  char property   [WORDBREAK_PROPERTY_LENGTH];
  char comment   [WORDBREAK_COMMENT_LENGTH];
} ucd_wordbreak_entry;


short should_line_be_parsed_WordBreak(const char *line)
{
  if(line == NULL)
    {
      return 0;
    }

  /* Check if line is commented */
  if(*line == '#')
    {
      return 0;
    }
  /* Check if line is empty */
  else
    {
      char *walker = (char *)line;
      int count = 0;
      while((walker != NULL) && \
            (*walker != '\r') && \
            (*walker != '\n'))
        {
          if(*walker == ';')
            {
              count++;
            }
          walker++;
        }
      /* Check number of fields available in the line */
      if(count < 1)
        {
          return 0;
        }
    }
  return 1;
}


/*
 <code1>..<code2>    ; format   # <comment>
 <code1>             ; format   # <comment>
  */
int parse_WordBreak_line_field(char *place_to_store, int max_field_length,
                               const char *fieldstart, char **field_end)
{
  char *walker;
  int length;
  
  /* Look for field separator (can be either ';'  or end of line) */
  walker = (char *)fieldstart;
  while((walker != NULL) && \
        (*walker != ';') && \
        (*walker != '#') && \
        (*walker != '\n') && \
        (*walker != '\r'))
    {
      walker ++;
    }
  
  if(walker == NULL)
    {
      return -1;
    }
    
  /* Compute field length */
  length = walker - fieldstart;
  
  if(length > max_field_length)
    {
      printf("[Error] '%d' bytes are required for the field, "
             "but only '%d' are available\n", length, max_field_length);
      return -1;
    }
  
  /* Copy line contents */
  memcpy(place_to_store, fieldstart, length);
  
  /* Set last NUL char */
  place_to_store[length] = '\0';
  
  /* If stopped due to ';' delimiters, then look for next field start
   *  point */
  if((*walker == ';') || (*walker == '#'))
    {
      /* Skip ';' */
      walker++;
      while((walker != NULL) && \
            (*walker == ' '))
        {
          walker++;
        }
      *field_end = walker;
    }
  else
    {
      /* End of line reached, set null as output field end */
      *field_end = NULL;
    }
  
  return 0;
}

/*
 <code1>..<code2>    ; format   # <comment>
 <code1>             ; format   # <comment>
 */
int parse_WordBreak_line(ucd_wordbreak_entry *p_entry, const char *line)
{
  char temporal [20];
  char *stop;
  char *start;
  char *aux;
  start = (char *)line;
  
  memset(&temporal[0], 0, sizeof(char)*20);
  memset(&p_entry->unicode_point_start,0,UNICODE_POINT_LENGTH);
  memset(&p_entry->unicode_point_stop,0,UNICODE_POINT_LENGTH);

  /* Get entry fields! */
  if(parse_WordBreak_line_field(temporal, 19, start, &stop)!=0)
    return -1;
  /* Store interval start */
  aux = strstr(temporal,"..");
  if(aux == NULL)
    {
      aux = strstr(temporal,"   ");
      strncpy(p_entry->unicode_point_start, temporal, (aux - temporal));
      strncpy(p_entry->unicode_point_stop, temporal, (aux - temporal));
    }
  else /* Both interval start and stop are available */
    {
      int i;
      strncpy(p_entry->unicode_point_start, temporal, (aux - temporal));
      strncpy(p_entry->unicode_point_stop, aux+2, (&temporal[19]-(aux+2)));
      /* Skip trailing white spaces in stop point */
      i = 0;
      while(p_entry->unicode_point_stop[i] != ' ')
        {
          i++;
        }
      if(p_entry->unicode_point_stop[i] == ' ')
        {
          p_entry->unicode_point_stop[i] = '\0';
        }
    }

  start = stop;
  if(parse_WordBreak_line_field(p_entry->property, \
                                WORDBREAK_PROPERTY_LENGTH-1, start, &stop)!=0)
    return -1;
  start = stop;
  if(parse_WordBreak_line_field(p_entry->comment, \
                                WORDBREAK_COMMENT_LENGTH-1, start, &stop)!=0)
    return -1;
  
  return 0;
}

int print_WordBreak_info(const ucd_wordbreak_entry *p_entry)
{
      printf("interval_start:'%s' interval_stop:'%s' property:'%s' "
             "comments:'%s'\n",
             p_entry->unicode_point_start,
             (strlen(p_entry->unicode_point_stop)>0) ? \
              p_entry->unicode_point_stop:"<None>", 
             p_entry->property,
             (strlen(p_entry->comment)>0) ? p_entry->comment:"<None>");
  return 0;
}

int create_WordBreak_header(FILE *pf)
{
  if(pf != NULL)
    {
      fprintf(pf, "\n\n\n"
              "typedef struct _unicode_wordbreak_info_s {\n"
              "  pdf_u32_t interval_start;\n"
              "  pdf_u32_t interval_stop;\n"
              "  enum pdf_text_ucd_wb_property_e wb_property;\n"
              "} unicode_wordbreak_info_t;\n\n"
              "static unicode_wordbreak_info_t "
              "unicode_wordbreak_info[] = {\n");
    }
  return 0;
}

int create_WordBreak_trailer(FILE *pf)
{
  if(pf != NULL)
    {
      fprintf(pf, "};\n\n\n");
    }
  return 0;
}



int add_entry_to_WordBreak(FILE *pf, const ucd_wordbreak_entry *p_entry,
                           int index)
{
  if(pf != NULL)
    {
      const char *start = p_entry->unicode_point_start;
      const char *stop = p_entry->unicode_point_stop;
      const char *property = p_entry->property;

      fprintf(pf, "  { 0x%s, 0x%s, PDF_TEXT_UCD_WBP_%s }, /* %d */\n",
              p_entry->unicode_point_start,
              p_entry->unicode_point_stop,
              p_entry->property,
              index);
    }
  return 0;
}


int compute_WordBreak(const char *WordBreakFile)
{
  FILE *pf;
  FILE *pf_out;
  ucd_wordbreak_entry entry;
  long count;
  char line [MAX_LINE_LENGTH];
  
  /* Create Standard Casing source file, involving not only the specific
   *  case changes, but also the intervals of Unicode points with casing
   *  info. */
  
  pf = fopen(WordBreakFile, "r");
  if(pf == NULL)
    {
      fprintf(stderr,"\nFile '%s' can't be opened for reading\n", \
              WordBreakFile);
      return -2;
    }
  
  pf_out = fopen(OUTPUT_UNICODEDATA_WORDBREAK, "w");
  if(pf_out == NULL)
    {
      fprintf(stderr,"\nFile '%s' can't be opened for writting\n", \
              OUTPUT_UNICODEDATA_WORDBREAK);
      return -3;
    }
  
  
  /* Set header */
  create_WordBreak_header(pf_out);
  
  memset(&entry, 0, sizeof(entry));
  memset(&line[0], 0, MAX_LINE_LENGTH);
  count = 0;
  while(fgets(line, MAX_LINE_LENGTH-1, pf)!=NULL)
    {
      if(should_line_be_parsed_WordBreak(line))
        {
          /* Parse line */
          parse_WordBreak_line(&entry, line);
      
          /* Print in screen */
          print_WordBreak_info(&entry);
      
          /* Add to output file */
          if(add_entry_to_WordBreak(pf_out, &entry,count) >=0)
            {
              count++;
            }
        }      
      /* Clear structure  and line contents */
      memset(&entry, 0, sizeof(entry));
      memset(&line[0], 0, MAX_LINE_LENGTH);
    }
    
  /* Set trailer for the case changes list */
  create_WordBreak_trailer(pf_out);
  
  /* Input file can be closed now */
  fclose(pf);
  
  printf("A total of '%ld' entries added in the WordBreak info array\n",
         count);
  
  return 0;
}


/* ------------------- CREATING SpecialCasingFile.c --------------------------*/

#define OUTPUT_UNICODEDATA_SPECIAL_CASING "SpecialCasing.c"

#define MAX_SPECIAL_CASE_ENTRIES 200 /* Much more than required in Unicode 5.0,
*  should be enough for future updates of 
*  the standard */

#ifndef UNICODE_POINT_LENGTH
#define UNICODE_POINT_LENGTH 10
#endif
#define SPECIALCASE_POINT_LENGTH 40
#define CONDITION_LIST_LENGTH 255
typedef struct _ucd_special_case_entry {
  char unicode_point   [UNICODE_POINT_LENGTH];
  char lowercase_point [SPECIALCASE_POINT_LENGTH];
  char titlecase_point [SPECIALCASE_POINT_LENGTH];
  char uppercase_point [SPECIALCASE_POINT_LENGTH];
  char condition_list  [CONDITION_LIST_LENGTH];
} ucd_special_case_entry;



short should_line_be_parsed_SpecialCasing(const char *line)
{
  if(line == NULL)
    {
      return 0;
    }
  
  /* Check if line is commented */
  if(*line == '#')
    {
      return 0;
    }
  /* Check if line is empty */
  else
    {
      char *walker = (char *)line;
      int count = 0;
      while((walker != NULL) && \
            (*walker != '\r') && \
            (*walker != '\n'))
        {
          if(*walker == ';')
            {
              count++;
            }
          walker++;
        }
      /* Check number of fields available in the line */
      if(count < 4)
        {
          return 0;
        }
    }
  return 1;
}


/*
 <code>; <lower> ; <title> ; <upper> ; (<condition_list> ;)? # <comment>
 */
int parse_SpecialCasing_line_field(char *place_to_store, int max_field_length,
                                   const char *fieldstart, char **field_end)
{
  char *walker;
  int length;
  
  /* Look for field separator (can be either ';' or end of line */
  walker = (char *)fieldstart;
  while((walker != NULL) && \
        (*walker != ';') && \
        (*walker != '\n') && \
        (*walker != '\r'))
    {
      walker ++;
    }
  
  /* Compute field length */
  length = walker - fieldstart;
  
  if(length > max_field_length)
    {
      printf("[Error] '%d' bytes are required for the field, "
             "but only '%d' are available\n", length, max_field_length);
      return -1;
    }
  
  /* Copy line contents */
  memcpy(place_to_store, fieldstart, length);
  
  /* Set last NUL char */
  place_to_store[length] = '\0';
  
  /* If stopped due to ';' delimiter, then look for next field start point */
  if(*walker == ';')
    {
      /* Skip ';' */
      walker++;
      while((walker != NULL) && \
            ((*walker == ' ') || (*walker == '#')))
        {
          walker++;
        }
      *field_end = walker;
    }
  else
    {
      /* End of line reached, set null as output field end */
      *field_end = NULL;
    }
  
  return 0;
}

int parse_SpecialCasing_line(ucd_special_case_entry *p_entry, const char *line)
{
  char *stop;
  char *start;
  start = (char *)line;
  
  /* Get entry fields! */
  if(parse_SpecialCasing_line_field(p_entry->unicode_point, \
                                    UNICODE_POINT_LENGTH-1, start, &stop)!=0)
    return -1;
  start = stop;
  if(parse_SpecialCasing_line_field(p_entry->lowercase_point, \
                                    SPECIALCASE_POINT_LENGTH-1, start, &stop)!=0)
    return -1;
  start = stop;
  if(parse_SpecialCasing_line_field(p_entry->titlecase_point, \
                                    SPECIALCASE_POINT_LENGTH-1, start, &stop)!=0)
    return -1;
  start = stop;
  if(parse_SpecialCasing_line_field(p_entry->uppercase_point, \
                                    SPECIALCASE_POINT_LENGTH-1, start, &stop)!=0)
    return -1;
  start = stop;
  if(parse_SpecialCasing_line_field(p_entry->condition_list, \
                                    CONDITION_LIST_LENGTH-1, start, &stop)!=0)
    return -1;
  
  if(stop == NULL)
    {
      /* This means that the last read field is not the condition list, but
       *  the name of the unicode point, so we must reset the condition list
       * field */
      memset(p_entry->condition_list, 0, CONDITION_LIST_LENGTH);
    }
  
  return 0;
}

int print_SpecialCasing_info(const ucd_special_case_entry *p_entry)
{
  printf("point:'%s' upper:'%s' lower:'%s' title:'%s' condition:'%s'\n",
         p_entry->unicode_point,
         p_entry->uppercase_point,
         p_entry->lowercase_point,
         p_entry->titlecase_point,
         (strlen(p_entry->condition_list)>0) ? \
         p_entry->condition_list:"<None>");
  return 0;
}

int create_SpecialCasing_header(FILE *pf)
{
  if(pf != NULL)
    {
      fprintf(pf, "\n\n\n"
              "typedef struct _unicode_special_case_info_s {\n"
              "  pdf_u32_t unicode_point;\n"
              "  pdf_u32_t lowercase_point[3];\n"
              "  pdf_u32_t uppercase_point[3];\n"       
              "  pdf_u32_t titlecase_point[3];\n"
              "  const char *condition_list;\n"
              "} unicode_special_case_info_t;\n\n"
              "static unicode_special_case_info_t "
              "unicode_special_case_info[] = {\n");
    }
  return 0;
}

int create_SpecialCasing_INTERVAL_header(FILE *pf)
{
  if(pf != NULL)
    {
      fprintf(pf, "\n\n\n"
              "typedef struct _unicode_special_case_interval_s {\n"
              "  pdf_u32_t interval_start;\n"
              "  pdf_u32_t interval_stop;\n"
              "  long delta;\n"
              "} unicode_special_case_interval_t;\n\n"
              "static unicode_special_case_interval_t "
              "unicode_special_case_interval[] = {\n");
    }
  return 0;
}

int create_SpecialCasing_trailer(FILE *pf)
{
  if(pf != NULL)
    {
      fprintf(pf, "};\n\n\n");
    }
  return 0;
}

int create_SpecialCasing_INTERVAL_trailer(FILE *pf)
{
  if(pf != NULL)
    {
      fprintf(pf, "};\n\n\n");
    }
  return 0;
}


int cut_field_in_chunks_SpecialCasing(const char *field,
                                      char **chunk_1,
                                      char **chunk_2,
                                      char **chunk_3)
{
  char *_chunk_1 = NULL;
  char *_chunk_2 = NULL;
  char *_chunk_3 = NULL;
  char *walker;
  
  _chunk_1 = (char *)field;
  
  if(_chunk_1 == NULL)
    {
      *chunk_1 = _chunk_1;
      *chunk_2 = _chunk_2;
      *chunk_3 = _chunk_3;
      return -1;
    }
  
  /* Skip heading white spaces */
  while(*_chunk_1 == ' ')
    _chunk_1++;
  
  /* Look for end of term */
  walker = _chunk_1;
  while((*walker != ' ') && (*walker != '\0'))
    walker++;
  
  /* Do we have more terms? */
  if(*walker == ' ')
    {
      *walker = '\0'; /* End _chunk_1 string */
      
      _chunk_2 = walker+1;
      
      /* Skip heading white spaces */
      while(*_chunk_2 == ' ')
        _chunk_2++;
      
      /* Look for end of term */
      walker = _chunk_2;
      while((*walker != ' ') && (*walker != '\0'))
        walker++;
      
      /* Do we have more terms? */
      if(*walker == ' ')
        {
          *walker = '\0'; /* End _chunk_2 string */
          
          _chunk_3 = walker+1;
          
          /* Skip heading white spaces */
          while(*_chunk_3 == ' ')
            _chunk_3++;
          
          /* Look for end of term */
          walker = _chunk_3;
          while((*walker != ' ') && (*walker != '\0'))
            walker++;
          
          if((*walker == ' '))
            {
              *_chunk_3 = '\0'; /* End _chunk_3 string */                  
            }
        }
    }
  
  /* Set output chunks */
  *chunk_1 = _chunk_1;
  *chunk_2 = _chunk_2;
  *chunk_3 = _chunk_3;
  return 0;
}

int add_entry_to_SpecialCasing(FILE *pf, const ucd_special_case_entry *p_entry)
{
  if(pf != NULL)
    {
      char *walker;
      char *lower_1 = NULL;
      char *lower_2 = NULL;
      char *lower_3 = NULL;
      char *upper_1 = NULL;
      char *upper_2 = NULL;
      char *upper_3 = NULL;
      char *title_1 = NULL;
      char *title_2 = NULL;
      char *title_3 = NULL;
      
      cut_field_in_chunks_SpecialCasing(p_entry->lowercase_point,
                                        &lower_1, &lower_2, &lower_3);
      cut_field_in_chunks_SpecialCasing(p_entry->uppercase_point,
                                        &upper_1, &upper_2, &upper_3);
      cut_field_in_chunks_SpecialCasing(p_entry->titlecase_point,
                                        &title_1, &title_2, &title_3);
      
      fprintf(pf, "  {\n"
              "    0x%s,\n"
              "    { 0x%s, 0x%s, 0x%s }, /* lowercase */ \n" 
              "    { 0x%s, 0x%s, 0x%s }, /* uppercase */ \n"
              "    { 0x%s, 0x%s, 0x%s }, /* titlecase */ \n"
              "    \"%s\" /* conditions */ \n"
              "  },\n",
              p_entry->unicode_point,
              ((lower_1 != NULL) && (strlen(lower_1)>0)) ? lower_1 : "0000",
              ((lower_2 != NULL) && (strlen(lower_2)>0)) ? lower_2 : "0000",
              ((lower_3 != NULL) && (strlen(lower_3)>0)) ? lower_3 : "0000",
              ((upper_1 != NULL) && (strlen(upper_1)>0)) ? upper_1 : "0000",
              ((upper_2 != NULL) && (strlen(upper_2)>0)) ? upper_2 : "0000",
              ((upper_3 != NULL) && (strlen(upper_3)>0)) ? upper_3 : "0000",
              ((title_1 != NULL) && (strlen(title_1)>0)) ? title_1 : "0000",
              ((title_2 != NULL) && (strlen(title_2)>0)) ? title_2 : "0000",
              ((title_3 != NULL) && (strlen(title_3)>0)) ? title_3 : "0000",
              p_entry->condition_list);
    }
  return 0;
}


int add_entry_to_SpecialCasing_INTERVAL(FILE *pf,
                                        pdf_u32_t interval_start,
                                        pdf_u32_t interval_stop,
                                        long delta)
{
  if(pf != NULL)
    {
      fprintf(pf, "  { 0x%.6X, 0x%.6X, %ld },\n",
              interval_start, interval_stop, delta);
    }
  return 0;
}

int compute_SpecialCasing(const char *SpecialCasingFile)
{
  FILE *pf;
  FILE *pf_out;
  ucd_special_case_entry entry;
  long count;
  long count_intervals;
  long index;
  char line [MAX_LINE_LENGTH];
  pdf_u32_t array_entries[MAX_SPECIAL_CASE_ENTRIES];
  long deltaToIndex = 0;
  long oldDeltaToIndex = 0;
  pdf_u32_t prev_point = 0;
  pdf_u32_t interval_start = 0;
  pdf_u32_t interval_stop = 0;
  
  /* Create Standard Casing source file, involving not only the specific
   *  case changes, but also the intervals of Unicode points with casing
   *  info. */
  
  pf = fopen(SpecialCasingFile, "r");
  if(pf == NULL)
    {
      fprintf(stderr,"\nFile '%s' can't be opened for reading\n", \
              SpecialCasingFile);
      return -2;
    }
  
  pf_out = fopen(OUTPUT_UNICODEDATA_SPECIAL_CASING, "w");
  if(pf_out == NULL)
    {
      fprintf(stderr,"\nFile '%s' can't be opened for writting\n", \
              OUTPUT_UNICODEDATA_SPECIAL_CASING);
      return -3;
    }
  
  
  /* Set header */
  create_SpecialCasing_header(pf_out);
  
  memset(&entry, 0, sizeof(entry));
  memset(&line[0], 0, MAX_LINE_LENGTH);
  count = 0;
  while(fgets(line, MAX_LINE_LENGTH-1, pf)!=NULL)
    {
      if(should_line_be_parsed_SpecialCasing(line))
        {
          /* Parse line */
          parse_SpecialCasing_line(&entry, line);
          
          /* Print in screen */
          print_SpecialCasing_info(&entry);
          
          /* Add to output file */
          if(add_entry_to_SpecialCasing(pf_out, &entry) >=0)
            {
              /* If added correctly to output source file, add it to the internal
               *  array, so that intervals can be directly computed. */
              pdf_u32_t unicode_point;
              
              /* Transform unicode point value from ascii-hex to 32bit integer */
              if(ascii_hex_to_decimal(&unicode_point, entry.unicode_point)!=0)
                {
                  fprintf(stderr, "\nError converting ASCII-HEX to integer...\n");
                  return -1;
                }
              /* If conversion went ok, add to array of entries */
              array_entries[count] = unicode_point;
              /* And update count of elements in array */
              count++;
            }
        }
      
      /* Clear structure  and line contents */
      memset(&entry, 0, sizeof(entry));
      memset(&line[0], 0, MAX_LINE_LENGTH);
    }
  
  /* Set trailer for the case changes list */
  create_SpecialCasing_trailer(pf_out);
  
  /* Input file can be closed now */
  fclose(pf);
  
  /*------ Compute intervals array ----------*/
  
  create_SpecialCasing_INTERVAL_header(pf_out);
  
  /* Start first interval */
  interval_start = array_entries[0];
  oldDeltaToIndex = 0;
  count_intervals = 0;
  for(index = 0; index < count; index++)
    {
      /* Compute the deltaToIndex value, which is the correction to be applied
       *  to the unicode point value to get the index of the array previously
       *  created */
      deltaToIndex = array_entries[index] - index;
      
      /* Check if a new interval started. This means that there was a gap
       *  between unicode points or a difference between consecutive delta to
       *  index values... */
      if((index > 0) && \
         ((array_entries[index] != (array_entries[index-1] +1) || \
           (oldDeltaToIndex != deltaToIndex))))
        {
          /* Set the previous point as last interval point */
          interval_stop = array_entries[index-1];
          
          /* Add interval */
          add_entry_to_SpecialCasing_INTERVAL(pf_out,
                                              interval_start,
                                              interval_stop,
                                              oldDeltaToIndex);
          count_intervals++;
          
          /* Set new interval start */
          interval_start = array_entries[index]; 
        }
      
      oldDeltaToIndex = deltaToIndex;
    }
  
  /* Last interval... */
  interval_stop = array_entries[index-1];
  add_entry_to_SpecialCasing_INTERVAL(pf_out,
                                      interval_start,
                                      interval_stop,
                                      oldDeltaToIndex);
  count_intervals++;
  
  create_SpecialCasing_INTERVAL_trailer(pf_out);
  
  /* Close output file and exit */
  fclose(pf_out);
  
  printf("A total of '%ld' entries added in the special case info array\n",
         count);
  printf("A total of '%ld' intervals added\n", count_intervals);
  
  return 0;
}





/* ---------------------------------- MAIN -----------------------------------*/

int print_usage(const char *program)
{
  fprintf(stderr,
          "\n"
          "[Wrong number of arguments]\n"
          "Usage:\n"
          "   $> %s <UnicodeData.txt> CASE\n\n"
          "   $> %s <UnicodeData.txt> GENCAT\n\n"
          "   $> %s <UnicodeData.txt> COMBCLASS\n\n"
          "   $> %s <SpecialCasing.txt>\n\n"
          "   $> %s <WordBreakProperty.txt>\n\n",
          program, program, program, program, program);
  return 0;
}


int main (int argc, char **argv)
{

  if ((argc == 3) && \
      (strstr(argv[1], "UnicodeData.txt")!=NULL) && \
      (strstr(argv[2], "CASE")!=NULL))
    {
      /* Compute CASE source file from UnicodeData.txt */
      compute_UnicodeData_CASE(argv[1]);
      printf("UnicodeData parsed and CASE file created\n"); 
    }
  else if ((argc == 3) && \
      (strstr(argv[1], "UnicodeData.txt")!=NULL) && \
      (strstr(argv[2], "GENCAT")!=NULL))
    {
      /* Compute GENCAT source file from UnicodeData.txt */
      compute_UnicodeData_GENCAT(argv[1]);
      printf("UnicodeData parsed and GENCAT file created\n"); 
    }
  else if ((argc == 3) && \
           (strstr(argv[1], "UnicodeData.txt")!=NULL) && \
           (strstr(argv[2], "COMBCLASS")!=NULL))
    {
      /* Compute COMBCLASS source file from UnicodeData.txt */
      compute_UnicodeData_COMBCLASS(argv[1]);
      printf("UnicodeData parsed and COMBCLASS file created\n"); 
    }
  else if ((argc == 2) && \
           (strstr(argv[1], "SpecialCasing.txt")!=NULL))
    {
      /* Compute Special CASE source file from SpecialCasing.txt */
      compute_SpecialCasing(argv[1]);
      printf("SpecialCasing parsed and CASE file created\n"); 
    }
  else if ((argc == 2) && \
           (strstr(argv[1], "WordBreakProperty.txt")!=NULL))
    {
      /* Compute Special CASE source file from SpecialCasing.txt */
      compute_WordBreak(argv[1]);
      printf("WordBreakProperty parsed and WORDBREAK file created\n"); 
    }
  else
    {
      print_usage(argv[0]);
      return -1;
    }
  return 0;
}

/* End of pdf-text-generate-ucd.c */


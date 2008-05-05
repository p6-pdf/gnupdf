/* -*- mode: C -*- Time-stamp: "08/02/10 21:34:21 jemarch"
 *
 *       File:         pdf-types.c
 *       Date:         Sun Feb 10 21:33:44 2008
 *
 *       GNU PDF Library - Basic Types Module
 *
 */

/* Copyright (C) 2008 Free Software Foundation, Inc */

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

#include <pdf-types.h>


pdf_i64_t  pdf_i64_new(signed high, unsigned low)
{
  pdf_i64_t pdfint;
  pdfint.high = high;
  pdfint.low = low;
  return pdfint;
}/*end pdf_i64_new*/



pdf_status_t pdf_i64_assign(pdf_i64_t *bignum, signed high, unsigned low)
{

  if (bignum == NULL)
    {
      return PDF_ERROR;
    }
  bignum->high = high;
  bignum->low = low;
  return PDF_OK;
    
}/*end pdf_i64_assign*/



pdf_status_t pdf_i64_assign_quick(pdf_i64_t *bignum, signed value)
{
  if (bignum == NULL)
    {
      return PDF_ERROR;
    }
  if (value < 0)
    {
      bignum->high = 0xFFFFFFFF;
    }
  else
    {
      bignum->high = 0;
    }
  bignum->low = value;
  return PDF_OK;
}/*end pdf_i64_assign_quick*/


pdf_status_t	pdf_i64_copy(pdf_i64_t orig, pdf_i64_t *copy)
{
  if (copy == NULL)
    {
      return PDF_ERROR;
    }
  copy->high = orig.high;
  copy->low = orig.low;
  return PDF_OK;
   
}/*end pdf_i64_copy*/


pdf_status_t pdf_i64_add(pdf_i64_t *dest, pdf_i64_t addend1, pdf_i64_t addend2)
{

  unsigned carry = 0; /*carry*/
  unsigned low1, low2, high1, high2;
  
  if (dest == NULL)
    {
      return PDF_ERROR;
    }

  /*We divide the unsigned 32 bit number and the signed 32 bit numbers in two 16 bit ones each 
    to be able to manage the intermediate storage. Hence, low1 and low2 will save
    the two 16 bit numbers. Method based on algorithm in Knuth volume 2*/

  /*First 16 bit*/
  low1 = ( (addend1.low & (0x0000FFFF)) + (addend2.low & (0x0000FFFF)) ) % PDF_U16_DIV;
  carry = ( (addend1.low & (0x0000FFFF)) + (addend2.low & (0x0000FFFF))) / PDF_U16_DIV;
   
  /*Second 16 bit*/
  low2 = ( carry + 
	   ((addend1.low & 0xFFFF0000)>>16) + ((addend2.low & 0xFFFF0000)>>16) ) % PDF_U16_DIV;
  carry = ( carry +  
	   ((addend1.low & 0xFFFF0000)>>16) + ((addend2.low & 0xFFFF0000)>>16) )/ PDF_U16_DIV;
  dest->low = low1 + (low2<<16);

  high1 = (carry + (addend1.high & (0x0000FFFF)) + (addend2.high & (0x0000FFFF)) ) % PDF_U16_DIV;
  carry = (carry + (addend1.high & (0x0000FFFF)) + (addend2.high & (0x0000FFFF))) / PDF_U16_DIV;
  high2 = ( carry + 
	   ((addend1.high & 0xFFFF0000)>>16) + ((addend2.high & 0xFFFF0000)>>16) ) % PDF_U16_DIV;
  carry = ( carry +  
	   ((addend1.high & 0xFFFF0000)>>16) + ((addend2.high & 0xFFFF0000)>>16) )/ PDF_U16_DIV;
  dest->high = high1 + (high2<<16);
  /*
  if (carry != 0)
    {
      return PDF_ERROR;
    }
  else
    {
      return PDF_OK;
      }*/
  return PDF_OK;
}/*end pdf_i64_add*/

int      pdf_i64_cmp(pdf_i64_t number_1, pdf_i64_t number_2)
{
  if (number_1.high > number_2.high)
    {
      return 1;
    }
  else if (number_1.high < number_2.high)
    {
      return -1;
    }
  else
    {
      if (number_1.low > number_2.low)
	{
	  return 1;
	}
      else if (number_1.low < number_2.low)
	{
	  return -1;
	}
      else
	{
	  return 0;
	}
    }
}/*end pdf_i64_cmp*/

pdf_status_t      pdf_i64_abs(pdf_i64_t *dest, pdf_i64_t number)
{
 
  pdf_i64_t temp, one;

  if (dest == NULL)
    {
      return PDF_ERROR;
    }

  if (number.high < 0)
    {
      pdf_i64_assign(&temp,number.high,number.low);
      pdf_i64_assign(&one,0,1);
      pdf_i64_assign(dest,0,0);
      temp.high = ~temp.high;
      temp.low  = ~temp.low;
      pdf_i64_add(dest, temp, one);
    }
  else
    {
      pdf_i64_assign(dest, number.high, number.low);
    }
  return PDF_OK;
}/*end pdf_i64_abs*/

pdf_status_t      pdf_i64_neg(pdf_i64_t *dest, pdf_i64_t number)
{

  if (dest == NULL)
    {
      return PDF_ERROR;
    }

  pdf_i64_t tempo, one;
  pdf_i64_assign(&tempo,0,0);
  pdf_i64_assign(&one,0,1);
  tempo.high = ~number.high;
  tempo.low  = ~number.low;
  pdf_i64_add(dest, tempo, one);
  return PDF_OK;
}/*end pdf_i64_neg*/

pdf_status_t      pdf_i64_subtraction(pdf_i64_t *dest, pdf_i64_t minuend, pdf_i64_t subtrahend)
{
 
  if (dest == NULL)
    {
      return PDF_ERROR;
    }

  pdf_i64_t neg_subtrahend;
  pdf_i64_neg(&neg_subtrahend, subtrahend);
  pdf_i64_add(dest, minuend, neg_subtrahend);
  return PDF_OK;
}/*end pdf_i64_subtraction*/



void shift_right(pdf_i64_t *dest)
{
  dest->low >>= 1;
    if( (0x00000001 & dest->high) == 1)
    { 
      if ( (0x80000000 & dest->low) == 0)
	{
	  dest->low = ( dest->low | 0x80000000);
	}
    }
  else 
    {
      if ( (0x80000000 & dest->low) == 1)
	{
	  dest->low = (dest->low & 0x7FFFFFFF);
	}
    }
  dest->high >>= 1;
}/*end shift_right*/

void shift_right_long(pdf_i64_t *dest_high, pdf_i64_t *dest_low)
{
  shift_right(dest_low);
  if( (0x00000001 & dest_high->low) == 1)
    { 
      if ( (0x80000000 & dest_low->high) == 0)
	{
	  dest_low->high = ( dest_low->high | 0x80000000);
	}
    }
  else 
    {
      if ( (0x80000000 & dest_low->high) == 1)
	{
	  dest_low->high = (dest_low->high & 0x7FFFFFFF);
	}
    }
  shift_right(dest_high);
}/*end shift_right_long*/
  
void add_long(pdf_i64_t *P1, pdf_i64_t *P2, pdf_i64_t *P3, pdf_i64_t A1, pdf_i64_t A2, pdf_i64_t A3)
{
  pdf_i64_t carry;
  pdf_i64_add(P3, *P3, A3);
  if (P3->low == 2)
    {
      pdf_i64_t one;
      pdf_i64_assign(&one, 0, 1);
      P3->low = 1;
      pdf_i64_add(P2, *P2, one);
    }
    if (pdf_i64_add(P2, *P2, A2) == PDF_ERROR)
      {
	pdf_i64_assign(&carry, 0 , 1);
      }
    pdf_i64_add(P3, *P3, carry);
    pdf_i64_add(P3, *P3, A3);
}//end add_long

pdf_status_t      pdf_i64_mult(pdf_i64_t *dest, pdf_i64_t factor_1, pdf_i64_t factor_2)
{ 
  
  
  /*Knuth vol 2 method*/
  
  int i, j;
  /*variables follow the nomenclature of the algorithm
    presented in Knuth vol 2.
    k is the carry bit, t is a variable used
    to save partial results, the mask is used to tranfer data
    from arrays to pdf_i64_t types, and cont is a simple
    counter*/
  unsigned mask, t , k;
  pdf_i64_t multiplier, multiplicand;
  pdf_i64_abs(&multiplicand, factor_1);
  pdf_i64_abs(&multiplier, factor_2);
  /*the result is stored in w*/
  unsigned w[] ={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  /*v is an array where the multiplier is stored, and u is where the
    multiplicand is stored*/
  unsigned v[8], u[8];
  int result_sign = 1;/*used to carry the sign of the result till the end*/
  pdf_i64_t zero; /*used to store zero value in pdf_i64_t format*/
  pdf_i64_t temp; /*variable used to save partial results*/
  

  if (dest == NULL)
    {
      return PDF_ERROR;
    }


  /*Now check the signs of multiplier and multiplicand*/
  pdf_i64_assign(&zero,0,0);
  if (pdf_i64_cmp(multiplier,zero) < 0)
    {
      result_sign = -1;
      pdf_i64_abs(&multiplier,multiplier);
    }
  if (pdf_i64_cmp(multiplicand,zero) < 0)
    {
      pdf_i64_abs(&multiplicand,multiplicand);
      if (result_sign == -1)
	{
	  result_sign = 1;
	}
      else
	{
	  result_sign = -1;
	}
    }

  mask = 0xFF000000;
  for (i = 0; i < 8;i++)
    {
      if (i <= 3)
	{
	  v[i] = (    ( multiplier.high & (mask >> (i * 8)) ) >> ( (3 - i) * 8) );
	}
      else
	{
	  v[i] = (    ( multiplier.low & (mask >> ( (i - 4) * 8)) ) >> ( (7 - i) * 8) );
	}
    }

  for (i = 0; i < 8;i++)
    {
      if (i <= 3)
	{
	  u[i] = (    ( multiplicand.high & (mask >> (i * 8)) ) >> ( (3 - i) * 8) );
	}
      else
	{
	  u[i] = (    ( multiplicand.low & (mask >> ( (i - 4) * 8)) ) >> ( (7 - i) * 8) );
	}
    }

  

  t = 0;
  for (j = 7; j >= 0;j--)
    {
      k = 0;
      for(i = 7; i >= 0;i--)
	{
	  t = u[i] * v[j] + w[i + j + 1] + k;
	  w[i + j + 1] = t % PDF_U8_DIV;
	  k = t / PDF_U8_DIV;
	}
    }

  
  pdf_i64_assign(&temp, 0, 0);
  for (i = 0; i < 8;i++)
    {
      if (i <=3)
	{
	  temp.high = temp.high + ( w[i + 8] << (8 * (7 - i)));
	}
      else
	{
	  temp.low = temp.low + ( w[i + 8] << (8 * (7 - i)));
	}
    }

  /*Accomodate final result to the sign of the multiplicand and 
    multiplier*/
  if (result_sign == -1)
    {
      pdf_i64_neg(dest, temp);
    }
  else
    {
      pdf_i64_assign(dest, temp.high, temp.low);
    }

  return PDF_OK;

}//end of pdf_i64_mult

void mult_long(unsigned *w, pdf_i64_t factor_1, pdf_i64_t factor_2)
{

  
  /*Knuth vol 2 method*/
  
  int i, j;
  unsigned mask, t , k;
  pdf_i64_t multiplier, multiplicand;
  pdf_i64_abs(&multiplicand, factor_1);
  pdf_i64_abs(&multiplier, factor_2);
  unsigned v[8], u[8];
  
  for (i = 0;i < 16;i++)
    {
      w[i] = 0;
    }

  mask = 0xFF000000;
  for (i = 0; i < 8;i++)
    {
      if (i <= 3)
	{
	  v[i] = (    ( multiplier.high & (mask >> (i * 8)) ) >> ( (3 - i) * 8) );
	}
      else
	{
	  v[i] = (    ( multiplier.low & (mask >> ( (i - 4) * 8)) ) >> ( (7 - i) * 8) );
	}
    }

  for (i = 0; i < 8;i++)
    {
      if (i <= 3)
	{
	  u[i] = (    ( multiplicand.high & (mask >> (i * 8)) ) >> ( (3 - i) * 8) );
	}
      else
	{
	  u[i] = (    ( multiplicand.low & (mask >> ( (i - 4) * 8)) ) >> ( (7 - i) * 8) );
	}
    }

  
  t = 0;
  for (j = 7; j >= 0;j--)
    {
      k = 0;
      for(i = 7; i >= 0;i--)
	{
	  t = u[i] * v[j] + w[i + j + 1] + k;
	  w[i + j + 1] = t % PDF_U8_DIV;
	  k = t / PDF_U8_DIV;
	}
    }


}//end of mult_long


pdf_status_t     pdf_i64_div(pdf_i64_t *dest, pdf_i64_t dividend, pdf_i64_t divisor)
{
  /*Knuth method*/
  
  int i, j; /*counters*/
  /*k is used to find the first non-zero digit in the divisor, q_bar
   is where the partial result of the division is stored, z is used to find
   the highest non-zero digit in the dividend*/
  /*See Knuth for definitions of m and n*/
  unsigned k, q_bar, m , n, z;
  /*divisor_nor stores normalised divisor, d_pdf is used to 
   normalise divisor and dividend, v_pdf is the internal pdf_i64_t 
   version of divisor, q_bar_pdf is the pdf_i64_t version of the
   partial division result, temp is a temporary variable used int 
   the procedures*/
  pdf_i64_t divisor_nor, d_pdf,v_pdf, q_bar_pdf, temp;
  unsigned v[8]; /*where divisor is stored*/
  /*q is where result is stored, u is where the dividend is stored,
    and temporal is a temporal vector used in the different procedures*/
  unsigned q[8] = {0, 0, 0, 0, 0, 0, 0, 0}, u[16], temporal[8];
  unsigned d; /*used to normalise divisor and dividend*/
  unsigned b = PDF_U8_DIV; /*base of the division*/
  unsigned mask = 0xFF000000; /*mask used to pass info from pdf_i64_t types and arrays*/
  pdf_i64_t zero = pdf_i64_new(0 , 0); /*zero in pdf_i64_t format*/
  int result_sign = 1; /*used to carry the sign of the result till the end*/

  z = 0;
  k = 0;

  if (dest == NULL)
    {
      return PDF_ERROR;
    }

  /*Check first if divisor != 0*/
  if (pdf_i64_cmp(zero,divisor) == 0)
    {
      return PDF_ERROR;
    }
  /*Now check the signs fo divisor and dividend*/
  if (pdf_i64_cmp(divisor,zero) < 0)
    {
      result_sign = -1;
      pdf_i64_abs(&divisor,divisor);
    }
  if (pdf_i64_cmp(dividend,zero) < 0)
    {
      pdf_i64_abs(&dividend,dividend);
      if (result_sign == -1)
	{
	  result_sign = 1;
	}
      else
	{
	  result_sign = -1;
	}
    }

  /*We check if abs(divisor) > abs(dividend)*/
  if (pdf_i64_cmp(divisor, dividend) == 1)
    {
      pdf_i64_assign(dest,0,0);
      return PDF_OK;
    }


  /*we store divisor in array*/
  for (i = 0; i < 8;i++)
    {
      if (i <= 3)
	{
	  v[i] = (  ( divisor.high & (mask >> (i * 8)) ) >> ( (3 - i) * 8) ) ;
	}
      else
	{
	  v[i] = (    ( divisor.low & (mask >> ( (i - 4) * 8)) ) >> ( (7 - i) * 8) );
	}
    }
  
  /*we store dividend in temporal array*/
  for (i = 0; i < 8;i++)
    {
      if (i <= 3)
	{
	  temporal[i] = (  ( dividend.high & (mask >> (i * 8)) ) >> ( (3 - i) * 8) ) ;
	}
      else
	{
	  temporal[i] = (    ( dividend.low & (mask >> ( (i - 4) * 8)) ) >> ( (7 - i) * 8) );
	}
    }
  /*With z we'll find the highest non-zero 8 bit number in the dividend*/
  for (i = 7; i >= 0; i--)
    {
      if (temporal[i] != 0)
	{
	  z = i;
	}
    }

  
  /*with k we'll find the highest non-zero 8 bit number in the divisor*/
  for (i = 7; i >= 0; i--)
    {
      if (v[i] != 0)
	{
	  k = i;
	}
    }
  
  /*With z and k the relative sizes of the different numbers are found.
    See Knuth algorithm for more details on n and m.*/

  n = 8 - k; /*n = size of non zero v*/
  m = k - z;

  
  /*d is used to normalise divisor and dividend; see Knuth for details*/
  d = b/(v[k] + 1);


  /*we store d in pdf_i64_t type*/
  d_pdf = pdf_i64_new(0, d);

  /*Here u and v are multiplied by d to normalise them*/

  /*we use a extra long version of pdf_i64_t multiplication to avoid 
    unwanted overflows*/
  /*normalised dividend is stored in variable u*/
  mult_long(u, dividend, d_pdf);
  /*normalised divisor is stored in varialbe divisor_d*/
  pdf_i64_mult(&divisor_nor, divisor, d_pdf);

  /*We add up 8 to z to adapt it to the new 16 bit array, but 
    1 is subtracted due to the inclusion of Uo. See Knuth vol2
    for more details on Uo*/

  z = z + 7;

 

  /*normalised divisor is stored in array v*/
  for (i = 0; i < 8;i++)
    {
      if (i <= 3)
	{
	  v[i] = (    ( divisor_nor.high & (mask >> (i * 8)) ) >> ( (3 - i) * 8) );
	}
      else
	{
	  v[i] = (    ( divisor_nor.low & (mask >> ( (i - 4) * 8)) ) >> ( (7 - i) * 8) );
	}
    }

 

  /*With u and v normalised we start the algorithm*/
  /*Always add z when going through u array to get rid of non-useful zeros*/
  /*The different steps follow the algorithm described in Knuth vol 2*/
  for (j = 0;j <= m; j++)
    {
      /*Step D3*/
      if (u[j + z] == v[k])
	{
	  q_bar = b - 1;
	}
      else
	{
	  q_bar = (u[j + z] * b + u[j + z + 1])/v[k];
	}
      if (k < 7)
	{
	  while (v[k + 1] * q_bar >
		 (  ( u[j + z] * b + u[j + z + 1] - q_bar * v[k] ) * b + u[j + z + 2] ) )
	    {
	      q_bar = q_bar - 1;
	    }
	}

      /*Step D4*/
      /*We put q_bar into a i64 type to be able to multiply it by the normalised divisor*/
      q_bar_pdf = pdf_i64_new(0, q_bar);
      pdf_i64_mult(&v_pdf, divisor_nor, q_bar_pdf);
      /*put u[j]...u[j + n] into a pdf_i64_t type and subtract v_pdf*/
      temp = pdf_i64_new(0,0);
      for (i = j + n; i >= j; i--)
	{
	  int kk = j + n -3;
	  if (i >= kk)
	    {
	      temp.low = temp.low + ( u[i + z] << (8 * (j + n - i)));
	    }
	  else
	    {
	      temp.high = temp.high + ( u[i + z] << (8 * (j + n - i)));
	    }
	}
     
      pdf_i64_subtraction(&temp, temp, v_pdf);
    
      /*We finally put q_bar in the results array*/
      q[j] = q_bar;
      /*If the remainder is less than zero then we re-add the divisor and subtract one from q
       Step D6*/
      if (pdf_i64_cmp(temp, zero) < 0)
	{
	  q[j] = q[j] - 1;
	  pdf_i64_add(&temp, divisor_nor, temp);
	}
      /*Step D5; uj...uj+n is substituted with the uj...uj+n obtained above*/

      for (i = 0; i < 8;i++)
	{
	  if (i <= 3)
	    {
	      temporal[i] = (  ( temp.high & (mask >> (i * 8)) ) >> ( (3 - i) * 8) ) ;
	    }
	  else
	    {
	      temporal[i] = (    ( temp.low & (mask >> ( (i - 4) * 8)) ) >> ( (7 - i) * 8) );
	    }
	}

      for (i = 7; i >= 0;i--)
	{
	  u[z + j + n - 7 + i] = temporal[i];
	}
    }/*end of for in j*/
  
  
  pdf_i64_assign(&temp,0,0);

  for (i = m; i >= 0; i--)
    {
      if (m > 3)
	{
	  if (i >= (m -3))
	    {
	      temp.low = temp.low + ( q[i] << (8* (m - i)));
	    }
	  else
	    {
	      temp.high = temp.high + ( q[i] << (8* (m - i - 4)));
	    }
	}
      else
	{
	  temp.low = temp.low + ( q[i] << (8* (m - i)));
	}
    }

  if (result_sign == -1)
    {
      pdf_i64_neg(dest, temp);
    }
  else
    {
      pdf_i64_assign(dest, temp.high, temp.low);
    }
  if (dest == NULL)
    {
      return PDF_ERROR;
    }
  else
    {
      return PDF_OK;
    }
}

pdf_status_t pdf_i64_mod(pdf_i64_t *dest, pdf_i64_t dividend, pdf_i64_t divisor)
{
  /*Knuth method*/
 
  int i, j; /*counters*/
  /*k is used to find the first non-zero digit in the divisor, q_bar
   is where the partial result of the division is stored, z is used to find
   the highest non-zero digit in the dividend*/
  /*See Knuth for definitions of m and n*/
  unsigned k, q_bar, m , n, z;
  /*divisor_nor stores normalised divisor, d_pdf is used to 
   normalise divisor and dividend, v_pdf is the internal pdf_i64_t 
   version of divisor, q_bar_pdf is the pdf_i64_t version of the
   partial division result, temp is a temporary variable used int 
   the procedures*/
  pdf_i64_t divisor_nor, d_pdf,v_pdf, q_bar_pdf, temp;
  unsigned v[8]; /*where divisor is stored*/
  /*q is where result is stored, u is where the dividend is stored,
    and temporal is a temporal vector used in the different procedures*/
  unsigned q[8] = {0, 0, 0, 0, 0, 0, 0, 0}, u[16], temporal[8];
  unsigned d; /*used to normalise divisor and dividend*/
  unsigned b = PDF_U8_DIV; /*base of the division*/
  unsigned mask = 0xFF000000; /*mask used to pass info from pdf_i64_t types and arrays*/
  pdf_i64_t zero = pdf_i64_new(0 , 0); /*zero in pdf_i64_t format*/
  int result_sign = 1; /*used to carry the sign of the result till the end*/

  z = 0;
  k = 0;

  if (dest == NULL)
    {
      return PDF_ERROR;
    }

  /*Check first if divisor != 0*/
  if (pdf_i64_cmp(zero,divisor) == 0)
    {
      return PDF_ERROR;
    }
  /*Now check the signs fo divisor and dividend*/
  if (pdf_i64_cmp(divisor,zero) < 0)
    {
      result_sign = -1;
      pdf_i64_abs(&divisor,divisor);
    }
  if (pdf_i64_cmp(dividend,zero) < 0)
    {
      pdf_i64_abs(&dividend,dividend);
      if (result_sign == -1)
	{
	  result_sign = 1;
	}
      else
	{
	  result_sign = -1;
	}
    }

  /*We check if abs(divisor) > abs(dividend)*/
  if (pdf_i64_cmp(divisor, dividend) == 1)
    {
      pdf_i64_assign(dest,0,0);
      return PDF_OK;
    }


  /*we store divisor in array*/
  for (i = 0; i < 8;i++)
    {
      if (i <= 3)
	{
	  v[i] = (  ( divisor.high & (mask >> (i * 8)) ) >> ( (3 - i) * 8) ) ;
	}
      else
	{
	  v[i] = (    ( divisor.low & (mask >> ( (i - 4) * 8)) ) >> ( (7 - i) * 8) );
	}
    }
  
  /*we store dividend in temporal array*/
  for (i = 0; i < 8;i++)
    {
      if (i <= 3)
	{
	  temporal[i] = (  ( dividend.high & (mask >> (i * 8)) ) >> ( (3 - i) * 8) ) ;
	}
      else
	{
	  temporal[i] = (    ( dividend.low & (mask >> ( (i - 4) * 8)) ) >> ( (7 - i) * 8) );
	}
    }
  /*With z we'll find the highest non-zero 8 bit number in the dividend*/
  for (i = 7; i >= 0; i--)
    {
      if (temporal[i] != 0)
	{
	  z = i;
	}
    }

  
  
  
  /*with k we'll find the highest non-zero 8 bit number in the divisor*/
  for (i = 7; i >= 0; i--)
    {
      if (v[i] != 0)
	{
	  k = i;
	}
    }
  
  /*With z and k the relative sizes of the different numbers are found.
    See Knuth algorithm for more details on n and m.*/

  n = 8 - k; /*n = size of non zero v*/
  m = k - z;
  
  /*d is used to normalise divisor and dividend; see Knuth for details*/
  d = b/(v[k] + 1);

  /*we store d in pdf_i64_t type*/
  d_pdf = pdf_i64_new(0, d);

  /*Here u and v are multiplied by d to normalise them*/

  /*we use a extra long version of pdf_i64_t multiplication to avoid 
    unwanted overflows*/
  /*normalised dividend is stored in variable u*/
  mult_long(u, dividend, d_pdf);
  /*normalised divisor is stored in varialbe divisor_d*/
  pdf_i64_mult(&divisor_nor, divisor, d_pdf);

  /*We add up 8 to z to adapt it to the new 16 bit array, but 
    1 is subtracted due to the inclusion of Uo. See Knuth vol2
    for more details on Uo*/

  z = z + 7;


  /*normalised divisor is stored in array v*/
  for (i = 0; i < 8;i++)
    {
      if (i <= 3)
	{
	  v[i] = (    ( divisor_nor.high & (mask >> (i * 8)) ) >> ( (3 - i) * 8) );
	}
      else
	{
	  v[i] = (    ( divisor_nor.low & (mask >> ( (i - 4) * 8)) ) >> ( (7 - i) * 8) );
	}
    }

 

  /*With u and v normalised we start the algorithm*/
  /*Always add z when going through u array to get rid of non-useful zeros*/
  /*The different steps follow the algorithm described in Knuth vol 2*/
  for (j = 0;j <= m; j++)
    {
      /*Step D3*/
      if (u[j + z] == v[k])
	{
	  q_bar = b - 1;
	}
      else
	{
	  q_bar = (u[j + z] * b + u[j + z + 1])/v[k];
	}
      while (v[k + 1] * q_bar >(  ( u[j + z] * b + u[j + z + 1] - q_bar * v[k] ) * b + u[j + z + 2] ) )
	{
	  q_bar = q_bar - 1;
	}
      /*Step D4*/
      /*We put q_bar into a i64 type to be able to multiply it by the normalised divisor*/
      q_bar_pdf = pdf_i64_new(0, q_bar);
      pdf_i64_mult(&v_pdf, divisor_nor, q_bar_pdf);
      /*put u[j]...u[j + n] into a pdf_i64_t type and subtract v_pdf*/
      temp = pdf_i64_new(0,0);
      for (i = j + n; i >= j; i--)
	{
	  int kk = j + n -3;
	  if (i >= kk)
	    {
	      temp.low = temp.low + ( u[i + z] << (8 * (j + n - i)));
	    }
	  else
	    {
	      temp.high = temp.high + ( u[i + z] << (8 * (j + n - i)));
	    }
	}
     
      pdf_i64_subtraction(&temp, temp, v_pdf);
      /*We finally put q_bar in the results array*/
      q[j] = q_bar;
      /*If the remainder is less than zero then we re-add the divisor and subtract one from q
       Step D6*/
      if (pdf_i64_cmp(temp, zero) < 0)
	{
	  q[j] = q[j] - 1;
	  pdf_i64_add(&temp, divisor_nor, temp);
	}
      /*Step D5; uj...uj+n is substituted with the uj...uj+n obtained above*/

      for (i = 0; i < 8;i++)
	{
	  if (i <= 3)
	    {
	      temporal[i] = (  ( temp.high & (mask >> (i * 8)) ) >> ( (3 - i) * 8) ) ;
	    }
	  else
	    {
	      temporal[i] = (    ( temp.low & (mask >> ( (i - 4) * 8)) ) >> ( (7 - i) * 8) );
	    }
	}

      for (i = 7; i >= 0;i--)
	{
	  u[z + j + n - 7 + i] = temporal[i];
	}
    }/*end of for in j*/
  
  pdf_i64_assign(&temp,0,0);

  for (i = 0; i < 8;i++)
    {
      if (i <=3)
	{
	  temp.high = temp.high + ( u[i + 8] << (8 * (7 - i)));
	}
      else
	{
	  temp.low = temp.low + ( u[i + 8] << (8 * (7 - i)));
	}
    }
  

  pdf_i64_assign(dest,0,0);
  pdf_i64_div(dest,temp,d_pdf);
  
  
  if (dest == NULL)
    {
      return PDF_ERROR;
    }
  else
    {
      return PDF_OK;
    }


}/*end of pdf_i64_mod*/



/* End of pdf-types.c */

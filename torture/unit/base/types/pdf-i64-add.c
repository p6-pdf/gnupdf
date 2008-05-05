
#include <config.h>
#include <stdio.h>
#include <string.h>
#include <pdf.h>
#include <check.h>



#define INTERACTIVE_DEBUG 0

/*
 * Test: pdf_i64_add_001
 * Description:
 *   Checks if the carry number is properly carried
 * Success conditions:
 * The call should not produce an error
 */
START_TEST(pdf_i64_add_001)
{

  pdf_i64_t add1,add2,dest;
  add1 = pdf_i64_new(0,4294967295);
  add2 = pdf_i64_new(2147483646,1);

  


  fail_if(pdf_i64_add(&dest,add1,add2) != PDF_OK);
  
  fail_if(dest.low != 0);
  fail_if(dest.high != 2147483647);
  

}
END_TEST


/*
 * Test: pdf_i64_add_002
 * Description:
 *   Checks if function checks the proper initialisation of target
 * Success conditions:
 * The call should produce an error
 */

START_TEST(pdf_i64_add_002)
{
  
  pdf_i64_t add1,add2;
  pdf_i64_t *dest;
  add1 = pdf_i64_new(0,4);
  add2 = pdf_i64_new(5,0);

  


  fail_if(pdf_i64_add(dest,add1,add2) != PDF_ERROR);

  
  
}
END_TEST

/*
 * Test: pdf_i64_add_003
 * Description:
 *   Adds a positive and negative number
 * Success conditions:
 * The call should not produce an error
 */
START_TEST(pdf_i64_add_003)
{

  pdf_i64_t add1, add2, dest;
  add1 = pdf_i64_new(0xFFFFFFFF,0xFFFFFFFE); /*-2*/
  add2 = pdf_i64_new(0,3);

  
  fail_if(pdf_i64_add(&dest, add1, add2) != PDF_OK);
  
  fail_if(dest.low != 1);
  fail_if(dest.high != 0);
  

}
END_TEST



/*
 * Test: pdf_i64_add_004
 * Description:
 *   Adds two negative numbers
 * Success conditions:
 * The call should not produce an error
 */
START_TEST(pdf_i64_add_004)
{

  pdf_i64_t add1, add2, dest;
  add1 = pdf_i64_new(0xFFFFFFFF,0xFFFFFFFE); /*-2*/
  add2 = pdf_i64_new(0xFFFFFFFF,0xFFFFFFFE); /*-2*/

  
  fail_if(pdf_i64_add(&dest, add1, add2) != PDF_OK);
  
  fail_if(dest.low !=  0xFFFFFFFC); /*-4*/
  fail_if(dest.high != 0xFFFFFFFF);
  

}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_i64_add (void)
{
  TCase *tc = tcase_create("pdf_i64_add");
  tcase_add_test(tc, pdf_i64_add_001);
  tcase_add_test(tc, pdf_i64_add_002);
  tcase_add_test(tc, pdf_i64_add_003);
  tcase_add_test(tc, pdf_i64_add_004);
  return tc;
}

/* End of pdf-i64-add.c */

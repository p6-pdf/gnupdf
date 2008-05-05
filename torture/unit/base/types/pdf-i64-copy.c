
#include <config.h>
#include <stdio.h>
#include <string.h>
#include <pdf.h>
#include <check.h>



#define INTERACTIVE_DEBUG 0

/*
 * Test: pdf_i64_copy_001
 * Description:
 *   Checks if the the pdf_i64_copy copies the values of one 
 * pdf_i64_t variable into another
 */
START_TEST(pdf_i64_copy_001)
{

  pdf_i64_t orig;
  pdf_i64_t copy;
  
  orig = pdf_i64_new(4,5);


  fail_if(pdf_i64_copy(orig,&copy) != PDF_OK);
  
  fail_if(copy.low != 5);
  fail_if(copy.high != 4);
  

}
END_TEST


/*
 * Test: pdf_i64_copy_001
 * Description:
 *   Checks if the the pdf_i64_copy function gets an error if
 *  the target pointer is not properly initialised
 */

START_TEST(pdf_i64_copy_002)
{
  
  pdf_i64_t orig;
  pdf_i64_t *copy;
  
  orig = pdf_i64_new(4,5);
  

  fail_if(pdf_i64_copy(orig,copy) != PDF_ERROR);
  
  
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_i64_copy (void)
{
  TCase *tc = tcase_create("pdf_i64_copy");
  tcase_add_test(tc, pdf_i64_copy_001);
  tcase_add_test(tc, pdf_i64_copy_002);
  return tc;
}

/* End of pdf-i64-copy.c */

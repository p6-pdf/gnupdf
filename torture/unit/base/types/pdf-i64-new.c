
#include <config.h>
#include <stdio.h>
#include <string.h>
#include <pdf.h>
#include <check.h>



#define INTERACTIVE_DEBUG 0

/*
 * Test: pdf_i64_new_001
 * Description:
 *   Checks if the the pdf_i64_new function properly assigns
 *  the high and low values to a pdf_i64_t type
 */
START_TEST(pdf_i64_new_001)
{

  pdf_i64_t k;

  k = pdf_i64_new(4,5);
  
  fail_if(k.low != 5);
  fail_if(k.high != 4);
  

}
END_TEST




/*
 * Test case creation function
 */
TCase *
test_pdf_i64_new (void)
{
  TCase *tc = tcase_create("pdf_i64_new");
  tcase_add_test(tc, pdf_i64_new_001);
  return tc;
}

/* End of pdf-i64-new.c */


#include <config.h>
#include <stdio.h>
#include <string.h>
#include <pdf.h>
#include <check.h>



#define INTERACTIVE_DEBUG 0

/*
 * Test: pdf_i64_assign_001
 * Description:
 *   Checks if the the pdf_i64_assign function properly assigns
 *  the high and low values to a pdf_i64_t type
 * Success conditions:
 * The call should not produce an error
 */
START_TEST(pdf_i64_assign_quick_001)
{

  pdf_i64_t k;

  fail_if(pdf_i64_assign_quick(&k,5) != PDF_OK);
  
  fail_if(k.low != 5);
  fail_if(k.high != 0);
  

}
END_TEST

/*
 * Test: pdf_i64_assign_002
 * Description:
 *   Checks if the the pdf_i64_assign function properly assigns
 *  the high and low values to a pdf_i64_t type
 * Success conditions:
 * The call should produce an error
 */
START_TEST(pdf_i64_assign_quick_002)
{

  pdf_i64_t* k;

  fail_if(pdf_i64_assign_quick(k,5) != PDF_ERROR);
  
  
}
END_TEST



/*
 * Test case creation function
 */
TCase *
test_pdf_i64_assign_quick (void)
{
  TCase *tc = tcase_create("pdf_i64_assign_quick");
  tcase_add_test(tc, pdf_i64_assign_quick_001);
  tcase_add_test(tc, pdf_i64_assign_quick_002);
  return tc;
}

/* End of pdf-i64-assign-quick.c */

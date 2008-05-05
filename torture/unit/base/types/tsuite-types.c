
#include <check.h>

extern TCase *test_pdf_i64_new (void);
extern TCase *test_pdf_i64_assign (void);
extern TCase *test_pdf_i64_assign_quick (void);
extern TCase *test_pdf_i64_copy (void);
extern TCase *test_pdf_i64_add (void);

Suite *
tsuite_types ()
{
  Suite *s;
  
  s = suite_create("types");

  suite_add_tcase (s, test_pdf_i64_new());
  suite_add_tcase (s, test_pdf_i64_assign());
  suite_add_tcase (s, test_pdf_i64_assign_quick());
  suite_add_tcase (s, test_pdf_i64_copy());
  suite_add_tcase (s, test_pdf_i64_add());
  
  return s;
}


/* End of tsuite-types.c */

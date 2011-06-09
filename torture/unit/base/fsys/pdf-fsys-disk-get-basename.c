/* -*- mode: C -*-
 *
 *       File:         pdf-fsys-disk-get-basename.c
 *       Date:         Wed Jun 07 22:51:54 2011
 *
 *       GNU PDF Library - Unit tests for pdf_fsys_get_basename with the
 *                         Disk filesystem
 *
 */

/* Copyright (C) 2011 Free Software Foundation, Inc. */

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
#include <tortutils.h>
#include <check.h>

#include <pdf.h>
#include <pdf-test-common.h>

struct test_paths_s {
  /* Test path given */
  pdf_char_t *path;
  /* Expected basename */
  pdf_char_t *basename;
  /* Expected error? */
  pdf_bool_t  error_expected;
};

static const struct test_paths_s test_paths[] = {
  /* Absolute paths */
#ifdef PDF_HOST_WIN32
  { "c:\\", NULL, PDF_FALSE },
  { "C:\\Documents", "Documents", PDF_FALSE },
  { "C:\\Documents\\", "Documents", PDF_FALSE },
  { "C:\\Documents\\PDFs", "PDFs", PDF_FALSE },
  { "C:\\Documents\\PDFs\\", "PDFs", PDF_FALSE },
  { "C:\\Documents\\\\\\\\PDFs\\\\\\\\", "PDFs", PDF_FALSE },
#else
  { "/", NULL, PDF_FALSE },
  { "/home", "home", PDF_FALSE },
  { "/home/user", "user", PDF_FALSE },
  { "/home/user/", "user", PDF_FALSE },
  { "/home//////user///////", "user", PDF_FALSE },
  { "/home/user/subdir/../../aaa", "aaa", PDF_FALSE },
  { "/home//user///../user/../user///////subdir/../../bbb", "bbb", PDF_FALSE },
  { "/home/user/./../other/subdir", "subdir", PDF_FALSE },
#endif
  { NULL, NULL, PDF_FALSE },
};

/*
 * Test: pdf_fsys_disk_get_basename_001
 * Description:
 *   Get the basename of a given path.
 * Success condition:
 *   The basename of the given path is the expected one.
 */
START_TEST (pdf_fsys_disk_get_basename_001)
{
  pdf_u32_t i;

  for (i = 0; test_paths[i].path != NULL; i++)
    {
      pdf_error_t *error = NULL;
      pdf_text_t *path;
      pdf_text_t *basename;

#ifdef FSYS_MODULE_ADDITIONAL_TEST_TRACES
      {
        printf ("[%u]\n"
                "  Path:              '%s'\n"
                "  Expected basename: '%s'\n"
                "  Expected error:     %s\n",
                i,
                test_paths[i].path,
                test_paths[i].basename ? test_paths[i].basename : "",
                test_paths[i].error_expected ? "yes" : "no");
      }
#endif /* FSYS_MODULE_ADDITIONAL_TEST_TRACES */

      path = pdf_text_new_from_unicode (test_paths[i].path,
                                        strlen (test_paths[i].path),
                                        PDF_TEXT_UTF8,
                                        &error);
      fail_unless (path != NULL);
      fail_if (error != NULL);

      basename = pdf_fsys_get_basename (PDF_FSYS_DISK,
                                        path,
                                        &error);

#ifdef FSYS_MODULE_ADDITIONAL_TEST_TRACES
      {
        pdf_char_t *basename_utf8;

        basename_utf8 = (basename ?
                         pdf_text_get_unicode (basename,
                                               PDF_TEXT_UTF8,
                                               PDF_TEXT_UNICODE_WITH_NUL_SUFFIX,
                                               NULL,
                                               &error) :
                         NULL);
        printf ("  Basename:          '%s'\n"
                "  Error:           '%s'\n",
                basename_utf8 ? basename_utf8 : "",
                error ? pdf_error_get_message (error) : "");

        pdf_dealloc (basename_utf8);
      }
#endif /* FSYS_MODULE_ADDITIONAL_TEST_TRACES */

      if (test_paths[i].error_expected)
        {
          fail_unless (basename == NULL);
          fail_if (error == NULL);
        }
      else if (test_paths[i].basename == NULL)
        {
          fail_unless (basename == NULL);
          fail_if (error != NULL);
        }
      else
        {
          pdf_char_t *basename_utf8;

          fail_unless (basename != NULL);
          fail_if (error != NULL);

          basename_utf8 = pdf_text_get_unicode (basename,
                                                PDF_TEXT_UTF8,
                                                PDF_TEXT_UNICODE_WITH_NUL_SUFFIX,
                                                NULL,
                                                &error);
          fail_unless (basename_utf8 != NULL);
          fail_if (error != NULL);

          fail_unless (strlen (test_paths[i].basename) == strlen (basename_utf8));
          fail_unless (strcmp (test_paths[i].basename, basename_utf8) == 0);

          pdf_dealloc (basename_utf8);
        }
    }
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_fsys_disk_get_basename (void)
{
  TCase *tc = tcase_create ("pdf_fsys_disk_get_basename");

  tcase_add_test (tc, pdf_fsys_disk_get_basename_001);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-fsys-disk-get_basename.c */

/* -*- mode: C -*-
 *
 *       File:         pdf-fsys-disk-get-parent.c
 *       Date:         Wed Jun 07 22:51:54 2011
 *
 *       GNU PDF Library - Unit tests for pdf_fsys_get_parent with the
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
  /* Expected parent */
  pdf_char_t *parent;
  /* Expected error? */
  pdf_bool_t  error_expected;
};

static const struct test_paths_s test_paths[] = {
  /* Absolute paths */
#ifdef PDF_HOST_WIN32
  { "c:\\", NULL, PDF_FALSE },
  { "C:\\Documents", "C:\\", PDF_FALSE },
  { "C:\\Documents\\", "C:\\", PDF_FALSE },
  { "C:\\Documents\\PDFs", "C:\\Documents", PDF_FALSE },
  { "C:\\Documents\\PDFs\\", "C:\\Documents", PDF_FALSE },
  { "C:\\Documents\\\\\\\\PDFs\\\\\\\\", "C:\\Documents", PDF_FALSE },
#else
  { "/", NULL, PDF_FALSE },
  { "/home", "/", PDF_FALSE },
  { "/home/user", "/home", PDF_FALSE },
  { "/home/user/", "/home", PDF_FALSE },
  { "/home//////user///////", "/home", PDF_FALSE },
  { "/home/user/subdir/../..", "/", PDF_FALSE },
  { "/home//user///../user/../user///////subdir/../..", "/", PDF_FALSE },
  { "/home/user/./../other/subdir", "/home/other", PDF_FALSE },
#endif
  { NULL, NULL, PDF_FALSE },
};

/*
 * Test: pdf_fsys_disk_get_parent_001
 * Description:
 *   Get the parent of a given path.
 * Success condition:
 *   The parent of the given path is the expected one.
 */
START_TEST (pdf_fsys_disk_get_parent_001)
{
  pdf_u32_t i;

  for (i = 0; test_paths[i].path != NULL; i++)
    {
      pdf_error_t *error = NULL;
      pdf_text_t *path;
      pdf_text_t *parent;

#ifdef FSYS_MODULE_ADDITIONAL_TEST_TRACES
      {
	  printf ("[%u]\n"
		  "  Path:            '%s'\n"
		  "  Expected parent: '%s'\n"
		  "  Expected error:   %s\n",
		  i,
		  test_paths[i].path,
		  test_paths[i].parent ? test_paths[i].parent : "",
		  test_paths[i].error_expected ? "yes" : "no");
      }
#endif /* FSYS_MODULE_ADDITIONAL_TEST_TRACES */

      path = pdf_text_new_from_unicode (test_paths[i].path,
					strlen (test_paths[i].path),
					PDF_TEXT_UTF8,
					&error);
      fail_unless (path != NULL);
      fail_if (error != NULL);

      parent = pdf_fsys_get_parent (PDF_FSYS_DISK,
				    path,
				    &error);

#ifdef FSYS_MODULE_ADDITIONAL_TEST_TRACES
      {
	  pdf_char_t *parent_utf8;

	  parent_utf8 = (parent ?
			 pdf_text_get_unicode (parent,
					       PDF_TEXT_UTF8,
					       PDF_TEXT_UNICODE_WITH_NUL_SUFFIX,
					       NULL,
					       &error) :
			 NULL);
	  printf ("  Parent:          '%s'\n"
		  "  Error:           '%s'\n",
		  parent_utf8 ? parent_utf8 : "",
		  error ? pdf_error_get_message (error) : "");

	  pdf_dealloc (parent_utf8);
      }
#endif /* FSYS_MODULE_ADDITIONAL_TEST_TRACES */

      if (test_paths[i].error_expected)
	{
	  fail_unless (parent == NULL);
	  fail_if (error == NULL);
	}
      else if (test_paths[i].parent == NULL)
	{
	  fail_unless (parent == NULL);
	  fail_if (error != NULL);
	}
      else
	{
	  pdf_char_t *parent_utf8;

	  fail_unless (parent != NULL);
	  fail_if (error != NULL);

	  parent_utf8 = pdf_text_get_unicode (parent,
					      PDF_TEXT_UTF8,
					      PDF_TEXT_UNICODE_WITH_NUL_SUFFIX,
					      NULL,
					      &error);
	  fail_unless (parent_utf8 != NULL);
	  fail_if (error != NULL);

	  fail_unless (strlen (test_paths[i].parent) == strlen (parent_utf8));
	  fail_unless (strcmp (test_paths[i].parent, parent_utf8) == 0);

	  pdf_dealloc (parent_utf8);
	}
    }
}
END_TEST

/*
 * Test case creation function
 */
TCase *
test_pdf_fsys_disk_get_parent (void)
{
  TCase *tc = tcase_create ("pdf_fsys_disk_get_parent");

  tcase_add_test (tc, pdf_fsys_disk_get_parent_001);
  tcase_add_checked_fixture (tc,
                             pdf_test_setup,
                             pdf_test_teardown);
  return tc;
}

/* End of pdf-fsys-disk-get_parent.c */

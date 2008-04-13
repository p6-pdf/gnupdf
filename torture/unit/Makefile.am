# torture/unit Makefile.am
# GNU PDF Library

# Copyright (C) 2007, 2008 Free Software Foundation, Inc.

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

if CHECK

# Check for external GNU libiconv library
if ICONV
 ICONV_LIBS = -liconv
endif #ICONV

noinst_PROGRAMS = runtests

LDADD = -lcheck $(top_srcdir)/src/libgnupdf.la $(INTL_MACOSX_LIBS) $(ICONV_LIBS)

AM_CPPFLAGS = -I$(top_srcdir)/lib \
              -I$(top_srcdir)/src -I$(top_srcdir)/src/base \
              -I$(top_srcdir)/src/object
AM_CFLAGS = -g

TESTS = runtests

# Unit tests for the Stream Module test suite
TEST_SUITE_STM = base/stm/pdf-create-file-stm.c

TEST_SUITE_LIST = base/list/pdf-list-add-first.c \
                  base/list/pdf-list-add-at.c \
                  base/list/pdf-list-add-last.c \
                  base/list/pdf-list-create.c \
                  base/list/pdf-list-get-at.c \
                  base/list/pdf-list-indexof.c \
                  base/list/pdf-list-indexof-from.c \
                  base/list/pdf-list-indexof-from-to.c \
                  base/list/pdf-list-iterator.c \
                  base/list/pdf-list-iterator-from-to.c \
                  base/list/pdf-list-iterator-next.c \
                  base/list/pdf-list-next-node.c \
                  base/list/pdf-list-node-value.c \
                  base/list/pdf-list-previous-node.c \
                  base/list/pdf-list-remove-at.c \
                  base/list/pdf-list-remove.c \
                  base/list/pdf-list-remove-node.c \
                  base/list/pdf-list-search.c \
                  base/list/pdf-list-search-from.c \
                  base/list/pdf-list-search-from-to.c \
                  base/list/pdf-list-set-at.c \
                  base/list/pdf-list-size.c 

# Unit tests for the Encoded Text Module test suite
TEST_SUITE_TEXT = base/text/pdf-text-test-common.h \
                  base/text/pdf-text-test-common.c \
                  base/text/pdf-text-test-data.c \
                  base/text/pdf-text-init.c \
                  base/text/pdf-text-new-destroy.c \
                  base/text/pdf-text-dup.c \
                  base/text/pdf-text-new-from-host.c \
                  base/text/pdf-text-new-from-pdf-string.c \
                  base/text/pdf-text-new-from-unicode.c \
                  base/text/pdf-text-new-from-u32.c \
                  base/text/pdf-text-get-country.c \
                  base/text/pdf-text-get-language.c \
                  base/text/pdf-text-set-country.c \
                  base/text/pdf-text-set-language.c \
                  base/text/pdf-text-empty-p.c \
                  base/text/pdf-text-get-host-encoding.c \
                  base/text/pdf-text-check-host-encoding.c \
                  base/text/pdf-text-get-best-encoding.c \
                  base/text/pdf-text-get-host.c \
                  base/text/pdf-text-get-pdfdocenc.c \
                  base/text/pdf-text-get-unicode.c \
                  base/text/pdf-text-get-hex.c \
                  base/text/pdf-text-set-host.c \
                  base/text/pdf-text-set-pdfdocenc.c \
                  base/text/pdf-text-set-unicode.c \
                  base/text/pdf-text-concat.c \
                  base/text/pdf-text-replace.c \
                  base/text/pdf-text-replace-ascii.c \
                  base/text/pdf-text-filter.c \
                  base/text/pdf-text-cmp.c

TEST_ENVIRONMENT = CHARSETALIASDIR=$(top_srcdir)/lib

TEST_FILES = $(TEST_SUITE_STM) \
             $(TEST_SUITE_LIST) \
             $(TEST_SUITE_TEXT)

TSUITE_FILES = base/alloc/tsuite-alloc.c \
               base/stm/tsuite-stm.c \
               base/list/tsuite-list.c \
               base/text/tsuite-text.c


runtests_SOURCES = runtests.c $(TSUITE_FILES) $(TEST_FILES)

check_PROGRAMS = runtests

clean-local:
	-rm -f temporal-test-file
	-rm -f ut.log

endif # CHECK

# End of Makefile.am

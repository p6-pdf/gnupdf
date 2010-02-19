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

#Only generate unit tests if Check framework is available or No-Check utility
# is requested...
if COMPILE_UT


#Check for Check and No-Check :-)
if NOCHECK
  NOCHECK_SRC = nocheck/check.h \
                nocheck/check.c
  NOCHECK_CPPFLAGS = -Inocheck/
else
  CHECK_LIBS = -lcheck
endif #NOCHECK



# Check for external GNU libiconv library
if ICONV
 ICONV_LIBS = -liconv
endif #ICONV

EXTRA_DIST=runtests.sh

noinst_PROGRAMS = runtests

LDADD = $(CHECK_LIBS) \
        $(top_builddir)/src/libgnupdf.la \
        $(INTL_MACOSX_LIBS) \
        $(ICONV_LIBS)

AM_CPPFLAGS = -I$(top_srcdir)/lib \
              -I$(top_srcdir)/src \
              -I$(top_srcdir)/src/base \
              -I$(top_srcdir)/src/object \
              -I$(top_srcdir)/torture/tortutils \
              -DTEST_DATA_PATH=\"$(top_srcdir)/torture/testdata\" \
              $(NOCHECK_CPPFLAGS)
AM_CFLAGS = -g

TESTS = runtests.sh
TESTS_ENVIRONMENT = top_srcdir=$(top_srcdir) \
                    top_builddir=$(top_builddir) \
                    PDF_HOST=$(PDF_HOST) \
                    WINE=$(WINE)

TEST_SUITE_ERROR = base/error/pdf-error.c \
		   base/error/pdf-perror.c

# Unit tests for the Stream Module test suite
TEST_SUITE_STM = base/stm/pdf-stm-mem-new.c \
                 base/stm/pdf-stm-file-new.c \
                 base/stm/pdf-stm-read.c \
                 base/stm/pdf-stm-read-char.c \
                 base/stm/pdf-stm-peek-char.c \
                 base/stm/pdf-stm-bseek.c \
                 base/stm/pdf-stm-btell.c \
                 base/stm/pdf-stm-tell.c \
                 base/stm/pdf-stm-write.c \
		 base/stm/pdf-stm-flush.c \
                 base/stm/pdf-stm-get-mode.c

TEST_SUITE_HASH = base/hash/pdf-hash-new.c \
		  base/hash/pdf-hash-destroy.c \
		  base/hash/pdf-hash-add.c \
		  base/hash/pdf-hash-add-time.c \
		  base/hash/pdf-hash-add-hash.c \
		  base/hash/pdf-hash-add-list.c \
		  base/hash/pdf-hash-add-stm.c \
		  base/hash/pdf-hash-add-text.c \
		  base/hash/pdf-hash-remove.c \
		  base/hash/pdf-hash-size.c \
		  base/hash/pdf-hash-get.c \
		  base/hash/pdf-hash-key-p.c \
		  base/hash/pdf-hash-rename.c \
		  base/hash/pdf-hash-iterator-new.c \
		  base/hash/pdf-hash-iterator-next.c \
		  base/hash/pdf-hash-iterator-destroy.c

TEST_SUITE_LIST = base/list/pdf-list-add-first.c \
                  base/list/pdf-list-add-at.c \
                  base/list/pdf-list-add-last.c \
                  base/list/pdf-list-size.c \
                  base/list/pdf-list-new.c \
                  base/list/pdf-list-destroy.c \
                  base/list/pdf-list-get-at.c \
                  base/list/pdf-list-indexof.c \
                  base/list/pdf-list-indexof-from.c \
                  base/list/pdf-list-indexof-from-to.c \
                  base/list/pdf-list-iterator.c \
                  base/list/pdf-list-iterator-from-to.c \
                  base/list/pdf-list-iterator-next.c \
                  base/list/pdf-list-iterator-free.c \
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
                  base/list/pdf-list-sorted-add.c \
                  base/list/pdf-list-sorted-remove.c \
                  base/list/pdf-list-sorted-search.c \
                  base/list/pdf-list-sorted-search-from-to.c \
                  base/list/pdf-list-sorted-indexof.c \
                  base/list/pdf-list-sorted-indexof-from-to.c

# Unit tests for the Encoded Text Module test suite
TEST_SUITE_TEXT = base/text/pdf-text-test-common.h \
                  base/text/pdf-text-test-common.c \
                  base/text/pdf-text-test-data.c \
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

# Unit tests for the Types Module test suite
TEST_SUITE_TYPES = base/types/pdf-i64-new.c \
		   base/types/pdf-i64-assign.c \
		   base/types/pdf-i64-assign-quick.c \
		   base/types/pdf-i64-copy.c \
		   base/types/pdf-i64-add.c \
		   base/types/pdf-i64-abs.c \
		   base/types/pdf-i64-neg.c \
		   base/types/pdf-i64-cmp.c \
		   base/types/pdf-i64-mult.c \
		   base/types/pdf-i64-div.c \
		   base/types/pdf-i64-mod.c \
		   base/types/pdf-i64-subtraction.c

TEST_SUITE_TIME =  base/time/pdf-time-new.c \
		   base/time/pdf-time-dup.c \
		   base/time/pdf-time-destroy.c \
		   base/time/pdf-time-copy.c \
		   base/time/pdf-time-clear.c \
		   base/time/pdf-time-set-from-u32.c \
		   base/time/pdf-time-set-from-i64.c \
		   base/time/pdf-time-add-cal-span.c \
		   base/time/pdf-time-sub-cal-span.c \
		   base/time/pdf-time-add-span.c \
		   base/time/pdf-time-sub-span.c \
		   base/time/pdf-time-get-local-cal.c \
		   base/time/pdf-time-get-utc-cal.c \
		   base/time/pdf-time-from-cal.c \
		   base/time/pdf-time-set-local-offset.c \
		   base/time/pdf-time-from-string.c \
		   base/time/pdf-time-span-new.c \
		   base/time/pdf-time-span-dup.c \
		   base/time/pdf-time-span-destroy.c \
		   base/time/pdf-time-span-set.c \
                   base/time/pdf-time-span-set-from-i32.c \
                   base/time/pdf-time-span-negate.c \
                   base/time/pdf-time-span-add.c \
                   base/time/pdf-time-span-cmp.c \
                   base/time/pdf-time-span-copy.c \
                   base/time/pdf-time-span-diff.c \
                   base/time/pdf-time-span-to-secs.c \
		   base/time/pdf-time-testdata.c \
		   base/time/pdf-time-test-common.c \
		   base/time/pdf-time-diff-cal.c \
		   base/time/pdf-time-set-to-current-local-time.c \
		   base/time/pdf-time-set-to-current-utc-time.c \
		   base/time/pdf-time-diff.c \
		   base/time/pdf-time-to-string.c \
		   base/time/pdf-time-cal-span-cmp.c \
		   base/time/pdf-time-cal-span-diff.c \
		   base/time/pdf-time-add-cal-span-with-base.c \
		   base/time/pdf-time-w32-set-from-filetime.c \
                   base/time/pdf-time-test-common.h

TEST_SUITE_CRYPT = base/crypt/pdf-crypt-cipher-new.c \
		   base/crypt/pdf-crypt-cipher-setkey.c \
		   base/crypt/pdf-crypt-cipher-destroy.c \
		   base/crypt/pdf-crypt-cipher-encrypt.c \
		   base/crypt/pdf-crypt-cipher-decrypt.c \
		   base/crypt/pdf-crypt-md-new.c \
		   base/crypt/pdf-crypt-md-write.c \
		   base/crypt/pdf-crypt-md-read.c \
		   base/crypt/pdf-crypt-md-destroy.c

TEST_SUITE_ALLOC = base/alloc/pdf-alloc.c \
                   base/alloc/pdf-realloc.c \
                   base/alloc/pdf-dealloc.c

TEST_SUITE_FP = base/fp/pdf-fp-func-eval.c \
                base/fp/pdf-fp-func-4-new.c

TEST_SUITE_FSYS = base/fsys/pdf-fsys-get-free-space.c \
                  base/fsys/pdf-fsys-file-open.c \
                  base/fsys/pdf-fsys-file-open-tmp.c \
                  base/fsys/pdf-fsys-file-close.c


TEST_SUITE_TOKEN = base/token/pdf-token-read.c

TEST_ENVIRONMENT = CHARSETALIASDIR=$(top_srcdir)/lib

TEST_FILES = $(TEST_SUITE_LIST) \
             $(TEST_SUITE_TEXT) \
             $(TEST_SUITE_HASH) \
             $(TEST_SUITE_ERROR) \
             $(TEST_SUITE_TYPES) \
             $(TEST_SUITE_TIME) \
             $(TEST_SUITE_CRYPT) \
             $(TEST_SUITE_ALLOC) \
             $(TEST_SUITE_STM) \
             $(TEST_SUITE_FP) \
             $(TEST_SUITE_FSYS) \
             $(TEST_SUITE_TOKEN)

TSUITE_FILES = base/alloc/tsuite-alloc.c \
               base/list/tsuite-list.c \
               base/text/tsuite-text.c \
               base/hash/tsuite-hash.c \
               base/error/tsuite-error.c \
               base/types/tsuite-types.c \
               base/time/tsuite-time.c \
               base/crypt/tsuite-crypt.c \
               base/stm/tsuite-stm.c \
               base/fp/tsuite-fp.c \
               base/fsys/tsuite-fsys.c \
               base/token/tsuite-token.c

TORTUTILS_FILES = $(top_srcdir)/torture/tortutils/tortutils.h \
                  $(top_srcdir)/torture/tortutils/tortutils.c

runtests_SOURCES = $(NOCHECK_SRC) \
                   $(TORTUTILS_FILES) \
                   $(TSUITE_FILES) \
                   $(TEST_FILES) \
                   runtests.c

check_PROGRAMS = runtests

clean-local:
	-rm -f temporal-test-file
	-rm -f ut.log
	-rm -f tmp.test

endif # COMPILE_UT

# End of Makefile.am

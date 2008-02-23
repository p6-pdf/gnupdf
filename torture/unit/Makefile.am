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

noinst_PROGRAMS = runtests

LDADD = -lcheck $(top_srcdir)/src/libgnupdf.la

AM_CPPFLAGS = -I$(top_srcdir)/lib \
              -I$(top_srcdir)/src -I$(top_srcdir)/src/base \
              -I$(top_srcdir)/src/object
AM_CFLAGS = -g

TESTS = runtests

TEST_FILES = base/stm/pdf-create-file-stm.c
TSUITE_FILES = base/alloc/tsuite-alloc.c base/stm/tsuite-stm.c

runtests_SOURCES = runtests.c $(TSUITE_FILES) $(TEST_FILES)

check_PROGRAMS = runtests

clean-local:
	-rm -f temporal-test-file
	-rm -f ut.log

endif # CHECK

# End of Makefile.am

# Torture directory Makefile.am
# GNU PDF Library

# Copyright (C) 2007 Free Software Foundation, Inc.

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

noinst_PROGRAMS = testall

LDADD = -lcheck $(top_srcdir)/src/libgnupdf.la

AM_CPPFLAGS = -I$(top_srcdir)/lib \
              -I$(top_srcdir)/src -I$(top_srcdir)/src/base \
              -I$(top_srcdir)/src/object
AM_CFLAGS = -g

TESTS = testall

TEST_FILES = test-stm-module.c

testall_SOURCES = testall.c $(TEST_FILES)

check_PROGRAMS = testall

testall.c: $(TEST_FILES)
	$(srcdir)/generate-testall $(TEST_FILES) > testall.c

clean-local:
	-rm -f testall.c

endif # CHECK

# End of Makefile.am

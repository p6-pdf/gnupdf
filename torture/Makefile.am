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

AM_CPPFLAGS = -I ../lib
AM_CFLAGS = -g

TESTS = testall

testall_SOURCES = testall.c \
	test-base.c \
	test-date.c \
	test-libinit.c \
	test-obj_array.c \
	test-obj_createdestroy.c \
	test-obj_dict.c \
	test-obj_dupequality.c \
	test-stm_openclose.c \
	test-rectangle.c

check_PROGRAMS = testall

endif # CHECK

# End of Makefile.am

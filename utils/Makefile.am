# Utils directory Makefile.am
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


# Check for external GNU libiconv library
if ICONV
 ICONV_LIBS = -liconv
endif #ICONV

bin_PROGRAMS = pdf-filter pdf-tokeniser

LDADD = $(top_builddir)/src/libgnupdf.la \
        $(ICONV_LIBS)

AM_CPPFLAGS = -I$(top_srcdir)/lib -I$(top_srcdir)/src/ \
              -I$(top_srcdir)/src/base -I $(top_srcdir)/src/object

pdf_filter_SOURCES = pdf-filter.h pdf-filter.c
pdf_tokeniser_SOURCES = pdf-tokeniser.c

# End of Makefile.am

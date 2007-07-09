# Source directory Makefile.am
# GNU PDF Library

# Copyright (C) 2007 Jose E. Marchesi

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

lib_LTLIBRARIES = libgnupdf.la
libgnupdf_la_SOURCES = pdf.c \
                       pdf_stm.c pdf_stm.h \
                       pdf_stm_file.c pdf_stm_file.h \
                       pdf_stm_mem.c pdf_stm_mem.h \
                       pdf_stm_f_null.c pdf_stm_f_null.h \
                       pdf_obj.c pdf_obj.h \
                       pdf_xref.c pdf_xref.h

include_HEADERS = pdf.h
libgnupdf_la_LIBADD = $(top_srcdir)/lib/libgnu.la

AM_CFLAGS = -Wall -fno-strict-aliasing
AM_CPPFLAGS = -I$(top_builddir)/lib




# End of Makefile.am

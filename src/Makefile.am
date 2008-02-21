# Source directory Makefile.am
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


# libgnupdf.so is the main library to build
lib_LTLIBRARIES = libgnupdf.la


# Base Layer sources

TYPES_MODULE_SOURCES = base/pdf-types.c base/pdf-types.h

STM_MODULE_SOURCES = base/pdf-stm-f-a85.c base/pdf-stm-f-a85.h \
                     base/pdf-stm-f-ahex.c base/pdf-stm-f-ahex.h \
                     base/pdf-stm-f-fax.c base/pdf-stm-f-fax.h \
                     base/pdf-stm-f-lzw.c base/pdf-stm-f-lzw.h \
                     base/pdf-stm-f-null.c base/pdf-stm-f-null.h \
                     base/pdf-stm-f-pred.c base/pdf-stm-f-pred.h \
                     base/pdf-stm-f-rl.c base/pdf-stm-f-rl.h \
                     base/pdf-stm-file.c base/pdf-stm-file.h \
                     base/pdf-stm-mem.c base/pdf-stm-mem.h \
                     base/pdf-stm.c base/pdf-stm.h
if ZLIB
  STM_MODULE_SOURCES += base/pdf-stm-f-flate.c base/pdf-stm-f-flate.h
endif

BASE_LAYER_SOURCES = base/pdf-base.c base/pdf-base.h \
                     $(TYPES_MODULE_SOURCES) \
                     $(STM_MODULE_SOURCES)

# Library sources

libgnupdf_la_SOURCES = pdf-global.c pdf-global.h

if COMPILE_BASE_LAYER
  libgnupdf_la_SOURCES += $(BASE_LAYER_SOURCES)
endif

include_HEADERS = pdf.h
libgnupdf_la_LIBADD = $(top_srcdir)/lib/libgnu.la

AM_CFLAGS = -Wall -fno-strict-aliasing
AM_CPPFLAGS = -I$(top_builddir)/lib -I$(srcdir)/base \
              -I$(srcdir)/object -I$(srcdir)/document


# Public header generation

PUBLIC_HDR_HDR = "/* This file has been automatically generated */"

PUBLIC_HDRS = pdf-global.h \
              base/pdf-types.h \
              base/pdf-stm-f-a85.h \
              base/pdf-stm-f-ahex.h \
              base/pdf-stm-f-fax.h \
              base/pdf-stm-f-flate.h \
              base/pdf-stm-f-lzw.h \
              base/pdf-stm-f-pred.h \
              base/pdf-stm-f-rl.h \
              base/pdf-stm-file.h \
              base/pdf-stm.h

pdf.h : $(PUBLIC_HDRS)
	chmod +x $(top_srcdir)/src/extract-public-hdr
	echo $(PUBLIC_HDR_HDR) > $@
	cat $(PUBLIC_HDRS) | $(top_srcdir)/src/extract-public-hdr >> $@


# End of Makefile.am

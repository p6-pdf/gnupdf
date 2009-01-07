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

LIST_MODULE_SOURCES = base/pdf-list.c base/pdf-list.h

HASH_MODULE_SOURCES = base/pdf-hash.c base/pdf-hash.h \
                      base/pdf-hash-helper.c base/pdf-hash-helper.h

ALLOC_MODULE_SOURCES = base/pdf-alloc.c base/pdf-alloc.h

TYPES_MODULE_SOURCES = base/pdf-types.c base/pdf-types.h

ERROR_MODULE_SOURCES = base/pdf-error.c base/pdf-error.h

FILESYSTEM_MODULE_SOURCES = base/pdf-fsys.c base/pdf-fsys.h \
                            base/pdf-fsys-def.h \
                            base/pdf-fsys-disk.c base/pdf-fsys-disk.h

STM_MODULE_SOURCES = base/pdf-stm.c base/pdf-stm.h \
                     base/pdf-stm-be.c base/pdf-stm-be.h \
                     base/pdf-stm-buffer.c base/pdf-stm-buffer.h \
                     base/pdf-stm-filter.h base/pdf-stm-filter.c \
                     base/pdf-stm-f-null.h base/pdf-stm-f-null.c \
                     base/pdf-stm-f-ahex.h base/pdf-stm-f-ahex.c \
                     base/pdf-stm-f-rl.h base/pdf-stm-f-rl.c \
                     base/pdf-stm-f-v2.h base/pdf-stm-f-v2.c \
                     base/pdf-stm-f-aesv2.h base/pdf-stm-f-aesv2.c \
                     base/pdf-stm-f-md5.h base/pdf-stm-f-md5.c

if ZLIB
  STM_MODULE_SOURCES += base/pdf-stm-f-flate.c base/pdf-stm-f-flate.h
endif

if JBIG2DEC
  STM_MODULE_SOURCES += base/pdf-stm-f-jbig2.c base/pdf-stm-f-jbig2.h
endif
if LIBJPEG
  STM_MODULE_SOURCES += base/pdf-stm-f-dct.c base/pdf-stm-f-dct.h
endif

TEXT_MODULE_SOURCES = base/pdf-text-context.c base/pdf-text-context.h \
                      base/pdf-text-encoding.c base/pdf-text-encoding.h \
                      base/pdf-text-host-encoding.c base/pdf-text-host-encoding.h \
                      base/pdf-text-filter.c base/pdf-text-filter.h \
                      base/pdf-text-ucd-case.c base/pdf-text-ucd-case.h \
                      base/pdf-text-ucd-combclass.c base/pdf-text-ucd-combclass.h \
                      base/pdf-text-ucd-wordbreak.c base/pdf-text-ucd-wordbreak.h \
                      base/pdf-text-ucd-gencat.c base/pdf-text-ucd-gencat.h \
                      base/pdf-text-ucd-proplist.c base/pdf-text-ucd-proplist.h \
                      base/pdf-text-ucd.h \
                      base/pdf-text.c base/pdf-text.h

TIME_MODULE_SOURCES = base/pdf-time-context.c base/pdf-time-context.h \
                      base/pdf-time-string.c base/pdf-time-string.h \
                      base/pdf-time.c base/pdf-time.h

CRYPT_MODULE_SOURCES = base/pdf-crypt.c base/pdf-crypt.h \
                       base/pdf-crypt-c-aesv2.c base/pdf-crypt-c-aesv2.h \
                       base/pdf-crypt-c-v2.c base/pdf-crypt-c-v2.h

FP_MODULE_SOURCES = base/pdf-fp.h base/pdf-fp.c \
                    base/pdf-fp-func.h base/pdf-fp-func.c

BASE_LAYER_SOURCES = base/pdf-base.c base/pdf-base.h \
                     $(ALLOC_MODULE_SOURCES) \
                     $(TYPES_MODULE_SOURCES) \
                     $(FP_MODULE_SOURCES) \
                     $(FILESYSTEM_MODULE_SOURCES) \
                     $(STM_MODULE_SOURCES) \
                     $(ERROR_MODULE_SOURCES) \
                     $(LIST_MODULE_SOURCES) \
                     $(TEXT_MODULE_SOURCES) \
                     $(HASH_MODULE_SOURCES) \
                     $(TIME_MODULE_SOURCES) \
                     $(CRYPT_MODULE_SOURCES)


# Library sources

libgnupdf_la_SOURCES = pdf-global.c pdf-global.h

if COMPILE_BASE_LAYER
  libgnupdf_la_SOURCES += $(BASE_LAYER_SOURCES)
endif

include_HEADERS = pdf.h
libgnupdf_la_LIBADD = $(top_builddir)/lib/libgnu.la $(LIBGCRYPT_LIBS)
# We want a DLL if compiling for Windows
if COMPILE_W32_SYSTEM
  libgnupdf_la_LDFLAGS = -no-undefined
endif

AM_CFLAGS = -Wall -fno-strict-aliasing $(LIBGCRYPT_CFLAGS)
if USE_COVERAGE
  AM_CFLAGS += -fprofile-arcs -ftest-coverage
endif

AM_CPPFLAGS = -I$(top_srcdir)/lib -I$(srcdir)/base \
              -I$(srcdir)/object -I$(srcdir)/document


# Public header generation

PUBLIC_HDRS = pdf-global.h \
              base/pdf-error.h \
              base/pdf-types.h \
              base/pdf-fp.h \
              base/pdf-alloc.h \
              base/pdf-list.h \
              base/pdf-hash.h \
              base/pdf-time.h \
              base/pdf-text.h \
              base/pdf-fsys.h \
              base/pdf-stm-buffer.h \
              base/pdf-stm-filter.h \
              base/pdf-stm.h \
              base/pdf-hash-helper.h \
              base/pdf-crypt.h \
              base/pdf-fp-func.h

pdf.h : $(PUBLIC_HDRS)
	chmod +x $(top_builddir)/src/extract-public-hdr
	cat $(srcdir)/header-autogen > $@
	echo "#ifndef PDF_H" >> $@
	echo "#define PDF_H" >> $@
	cat $^ | $(top_builddir)/src/extract-public-hdr >> $@
	echo "#endif /* !PDF_H */" >> $@
	echo "/* end of pdf.h */" >> $@

clean-local:
	rm -f $(srcdir)/pdf.h
	rm -f $(srcdir)/*.gcno

# End of Makefile.am

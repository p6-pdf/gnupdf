dnl Configuration for autoconf
dnl GNU PDF Library

dnl Please process this file with autoconf to get a `configure' script

dnl Copyright (C) 2007, 2008 Free Software Foundation, Inc.

dnl This program is free software: you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation, either version 3 of the License, or
dnl (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program.  If not, see <http://www.gnu.org/licenses/>.

AC_INIT(src/pdf-global.h)
AM_INIT_AUTOMAKE(gnupdf, 0.1)
AM_CONFIG_HEADER(src/config.h)

gl_EARLY

dnl Libtool
AC_PROG_LIBTOOL

dnl Compiler issues
AC_PROG_CC

dnl System
AC_CANONICAL_HOST
canonical=$host

gl_INIT

dnl Search for headers
AC_CHECK_HEADERS(malloc.h)

dnl Search for data types
AC_CHECK_TYPE(size_t, unsigned)
AC_CHECK_TYPE(off_t, long)

dnl Search for functions
AC_FUNC_FSEEKO

dnl Search for required libraries
CHECK_ZLIB
AM_CONDITIONAL([ZLIB], [test "x$have_zlib" = "xyes"])
AC_CHECK_LIB(m,exp,,,)
AC_CHECK_LIB([check],[suite_create],[have_check=yes],[have_check=no],)
AM_CONDITIONAL([CHECK], [test "x$have_check" = "xyes"])

AC_CHECK_LIB([iconv],[iconv_open],[have_iconv=yes],[have_iconv=no],)
AM_CONDITIONAL([ICONV], [test "x$have_iconv" = "xyes"])

dnl Search for required programs for building the sources
AC_PROG_AWK
if test x$AWK = "xno"; then
AC_MSG_ERROR([cannot find a suitable `awk' program. Please install it and rerun `configure'.])
fi
AC_PATH_PROG([PATH_TO_AWK],$AWK)
AC_SUBST(PATH_TO_AWK)

dnl Search for programs for building documentation
AC_CHECK_PROG([have_makeinfo], [makeinfo], [yes], [no],,)
if test "x$have_makeinfo" = "xno"; then
AC_MSG_ERROR([cannot find the `makeinfo' program. Please install the texinfo package and rerun `configure'.])
fi

AC_CHECK_PROG([have_texi2html], [texi2html], [yes], [no],,)
AM_CONDITIONAL([TEXI2HTML], [test "x$have_texi2html" = "xyes"])
if test "x$have_texi2html" = "xyes" ; then
texihtmlprogram=texi2html
else
texihtmlprogram=makeinfo
fi

AC_CHECK_PROG([have_taskjuggler], [taskjuggler], [yes], [no],,)
AM_CONDITIONAL([TASKJUGGLER], [test "x$have_taskjuggler" = "xyes"])

dnl Manage the compilation level
dnl The GNU PDF library is composed by several layers. The following
dnl `compilation levels' are defined allowing the user to compile a
dnl library composed by a specified subset of these layers.
dnl
dnl level\layer | base | object | document | page |
dnl ------------+------+--------+----------+------+
dnl      0      |  *   |        |          |      |
dnl      1      |  *   |    *   |          |      | 
dnl      2      |  *   |    *   |    *     |      |
dnl      3      |  *   |    *   |    *     |   *  |
dnl 
dnl Note that the base layer is always included in the library
dnl regardless the compilation level.

AC_ARG_VAR(PDFLIB_LEVEL,[A level code specifying the library layers to be compiled (default is 3=all layers)])
dnl The default compilation level is 3.
if test "x$PDFLIB_LEVEL" = "x" ; then
PDFLIB_LEVEL=3
fi

AM_CONDITIONAL([COMPILE_BASE_LAYER], [test "true" = "true"])
AM_CONDITIONAL([COMPILE_OBJECT_LAYER], [test "x$PDFLIB_LEVEL" = "x1" -o \
                                             "x$PDFLIB_LEVEL" = "x2" -o \
                                             "x$PDFLIB_LEVEL" = "x3"])
AM_CONDITIONAL([COMPILE_DOCUMENT_LAYER], [test "x$PDFLIB_LEVEL" = "x2" -o \
                                               "x$PDFLIB_LEVEL" = "x3"])
AM_CONDITIONAL([COMPILE_PAGE_LAYER], [test x$PDFLIB_LEVEL = "x3"])


dnl Project management resources
prmgt_mode=no
AC_ARG_ENABLE([prmgt], AS_HELP_STRING([--enable-prmgt],
                                      [build project management resources (default is NO)]), 
              [prmgt_mode=yes], [prmgt_mode=no])

AM_CONDITIONAL([PRMGT],[test "x$prmgt_mode" = "xyes"])
if test "x$prmgt_mode" = "xyes" ; then 

if test "x$have_taskjuggler" = "xyes" ; then
   AC_CHECK_PROG([have_dot], [dot], [yes], [no],,)
   if test "x$have_dot" = "xno"; then
   AC_MSG_ERROR([cannot find the `dot' program. Please install the graphviz package and rerun `configure'.])
   fi
   AC_CHECK_PROG([have_fdp], [fdp], [yes], [no],,)
   if test "x$have_fdp" = "xno"; then
   AC_MSG_ERROR([cannot find the `fdp' program. Please install the graphviz package and rerun `configure'.])
   fi
fi

AC_CHECK_PROG([have_pmccabe], [pmccabe], [yes], [no],,)
if test "x$have_pmccabe" = "xno"; then
AC_MSG_ERROR([cannot find the `pmccabe' program. Please install it and rerun `configure'.])
fi

fi # prmgt_mode

# debug_*_mode
debug_base_mode=no
debug_object_mode=no
debug_document_mode=no
debug_page_mode=no
PDFLIB_DEBUG_DESCR=""
AC_ARG_ENABLE([debug-base], AS_HELP_STRING([--enable-debug-base],
                                      [debug mode at the base layer (default is NO)]), 
              [debug_base_mode=yes], [debug_base_mode=no])

AC_ARG_ENABLE([debug-object], AS_HELP_STRING([--enable-debug-object],
                                      [debug mode at the object layer (default is NO)]), 
              [debug_object_mode=yes], [debug_object_mode=no])

AC_ARG_ENABLE([debug-document], AS_HELP_STRING([--enable-debug-document],
                                      [debug mode at the document layer (default is NO)]), 
              [debug_document_mode=yes], [debug_document_mode=no])

AC_ARG_ENABLE([debug-page], AS_HELP_STRING([--enable-debug-page],
                                      [debug mode at the page layer (default is NO)]), 
              [debug_page_mode=yes], [debug_page_mode=no])

if test "x$debug_base_mode" = "xyes"; then
  AC_DEFINE([HAVE_DEBUG_BASE], [1], [Debugging Base Layer])
  PDFLIB_DEBUG_DESCR="base, "
fi 
if test "x$debug_object_mode" = "xyes"; then
  AC_DEFINE([HAVE_DEBUG_OBJECT], [1], [Debugging Object Layer])
  PDFLIB_DEBUG_DESCR="${PDFLIB_DEBUG_DESCR}object, "
fi
if test "x$debug_document_mode" = "xyes"; then
  AC_DEFINE([HAVE_DEBUG_DOCUMENT], [1], [Debugging Document Layer])
  PDFLIB_DEBUG_DESCR="${PDFLIB_DEBUG_DESCR}document, "
fi
if test "x$debug_page_mode" = "xyes"; then
  AC_DEFINE([HAVE_DEBUG_PAGE], [1], [Debugging Page Layer])
  PDFLIB_DEBUG_DESCR="${PDFLIB_DEBUG_DESCR}page"
fi

if test "x$PDFLIB_DEBUG_DESCR" = "x"; then
  PDFLIB_DEBUG_DESCR="all disabled"
fi

# end debug_*_mode

dnl Generate output files
AC_OUTPUT(Makefile lib/Makefile src/Makefile torture/Makefile torture/unit/Makefile doc/Makefile utils/Makefile prmgt/Makefile prmgt/pmccabe2html src/extract-public-hdr)

dnl Report configuration results

if test "x$PDFLIB_LEVEL" = "x3"; then
   PDFLIB_LEVEL_DESCR="build all layers"
fi
if test "x$PDFLIB_LEVEL" = "x2"; then
   PDFLIB_LEVEL_DESCR="build base, object and page layers"
fi
if test "x$PDFLIB_LEVEL" = "x1"; then
   PDFLIB_LEVEL_DESCR="build base and object layers"
fi
if test "x$PDFLIB_LEVEL" = "x0"; then
   PDFLIB_LEVEL_DESCR="build base layer"
fi

echo "
Configured for \`${canonical}'.

  Compilation level:                        ${PDFLIB_LEVEL} (${PDFLIB_LEVEL_DESCR})
  Using FlateDecode filter?                 ${have_zlib}
  With unit tests support?                  ${have_check}
  Program to build html manuals             ${texihtmlprogram}
  Build project management resources?       ${prmgt_mode}
  Layers with debug mode enabled            ${PDFLIB_DEBUG_DESCR}
"

dnl Report warnings

if test "x$have_zlib" = "xno" ; then
echo "
WARNING: you are going to build a library without FlateDecode support. 
         That means you wont be able to process many PDF files."
fi

if test "x$have_check" = "xno" ; then
echo "
WARNING: you are going to build the library without unit testing support.
         If you want to be able to perform the unit testing you should
         install the 'check' library."
fi

dnl End of configure.ac

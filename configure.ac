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

dnl Libtool
AC_PROG_LIBTOOL

dnl Compiler issues
AC_PROG_CC

dnl System
AC_CANONICAL_HOST
canonical=$host

dnl gnulib macros
gl_EARLY
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

AC_CHECK_PROG([have_taskjuggler], [taskjuggler], [yes], [no],,)
if test "x$have_taskjuggler" = "xno"; then
AC_MSG_ERROR([cannot find the `taskjuggler' program. Please install it and rerun `configure'.])
fi
AC_CHECK_PROG([have_dot], [dot], [yes], [no],,)
if test "x$have_dot" = "xno"; then
AC_MSG_ERROR([cannot find the `dot' program. Please install the graphviz package and rerun `configure'.])
fi
AC_CHECK_PROG([have_fdp], [fdp], [yes], [no],,)
if test "x$have_fdp" = "xno"; then
AC_MSG_ERROR([cannot find the `fdp' program. Please install the graphviz package and rerun `configure'.])
fi
AC_CHECK_PROG([have_pmccabe], [pmccabe], [yes], [no],,)
if test "x$have_pmccabe" = "xno"; then
AC_MSG_ERROR([cannot find the `pmccabe' program. Please install it and rerun `configure'.])
fi

fi # prmgt_mode

dnl Generate output files
AC_OUTPUT(Makefile lib/Makefile src/Makefile torture/Makefile doc/Makefile utils/Makefile prmgt/Makefile prmgt/pmccabe2html src/extract-public-hdr)

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

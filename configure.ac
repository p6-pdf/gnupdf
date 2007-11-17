dnl Configuration for autoconf
dnl GNU PDF Library

dnl Please process this file with autoconf to get a `configure' script

dnl Copyright (C) 2007 Free Software Foundation, Inc.

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

AC_INIT(src/pdf.h)
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

dnl Search for programs
AC_CHECK_PROG([have_texi2html], [texi2html], [yes], [no],,)
AM_CONDITIONAL([TEXI2HTML], [test "x$have_texi2html" = "xyes"])
if test "x$have_texi2html" = "xyes" ; then
texihtmlprogram=texi2html
else
texihtmlprogram=makeinfo
fi
AC_CHECK_PROG([have_taskjuggler], [taskjuggler], [yes], [no],,)
AM_CONDITIONAL([TASKJUGGLER], [test "x$have_taskjuggler" = "xyes"])

dnl Generate output files
AC_OUTPUT(Makefile lib/Makefile src/Makefile torture/Makefile doc/Makefile utils/Makefile prmgt/Makefile)

dnl Report configuration results

echo "
Configured for \`${canonical}'.

  Using FlateDecode filter?                 ${have_zlib}
  With unit tests support?                  ${have_check}
  Program to build html manuals             ${texihtmlprogram}
  Build project management resources?       ${have_taskjuggler}
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

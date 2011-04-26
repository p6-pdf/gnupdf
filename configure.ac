dnl Configuration for autoconf
dnl GNU PDF Library

dnl Please process this file with autoconf to get a `configure' script

dnl Copyright (C) 2007, 2008, 2009, 2010 Free Software Foundation, Inc.

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

AC_INIT([GNU PDF Library],[trunk],[pdf-devel@gnu.org],[libgnupdf])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE(libgnupdf, trunk)
AC_CONFIG_HEADERS([src/config.h src/pdf-config.h])
AC_CONFIG_MACRO_DIR([m4])

dnl Autobuild
AB_INIT

dnl Compiler issues
AC_PROG_CC

gl_EARLY

dnl Libtool
AC_PROG_LIBTOOL

dnl System
AC_CANONICAL_HOST
canonical=$host

gl_INIT


dnl OS-based compilation options
compile_w32_system=no
AC_SUBST([WINE])
AC_SUBST([PDF_HOST])
case "${host}" in
    *-mingw32*)
        compile_w32_system=yes
        if test "${host}" != "${build}"
        then
           # Cross-compilation: use wine for testing
           WINE=wine
        else
           WINE=""
        fi

        AC_DEFINE([PDF_HOST_WIN32], [1], [Compiling for mingw32 systems])
        PDF_HOST=PDF_HOST_WIN32
        ;;
    *-bsd*)
        AC_DEFINE([PDF_HOST_BSD], [1], [Compiling for bsd systems])
        PDF_HOST=PDF_HOST_BSD
        ;;
    *)
        AC_DEFINE([PDF_HOST_GNU], [1], [Compiling for gnu systems])
        PDF_HOST=PDF_HOST_GNU
        ;;
esac

dnl Search for headers
AC_CHECK_HEADERS(malloc.h)

dnl Search for data types
AC_CHECK_TYPE(size_t, unsigned)
AC_CHECK_TYPE(off_t, long)

dnl Search for functions
AC_FUNC_FSEEKO


dnl Search for required libraries
dnl
dnl For each required library:
dnl
dnl   - The gnulib AC_LIB_HAVE_LINKFLAGS macro is used to define the
dnl     HAVE_LIB${NAME} variable and the $(LTLIBFOO} variables
dnl     substitued in the Makefile.am.  There are some exceptions
dnl     where an specific macro is used.
dnl
dnl   - If the presence of the library determines the compilation of
dnl     conditionally compiled code, an AM_CONDITIONAL is used to
dnl     define the corresponding automake conditional, based on the
dnl     result of AC_LIB_HAVE_LINKFLAGS.
dnl
dnl   - The HAVE_LIB${NAME} defined by AC_LIB_HAVE_LINKFLAGS cannot be
dnl     used in the public pdf.h header.  For that purpose we define a
dnl     PDF_HAVE_LIB${NAME} variable.
dnl
dnl Some of the libraries are mandatory, and some are optional.  See
dnl the section "Report errors" below.

dnl libuuid
AC_LIB_HAVE_LINKFLAGS([uuid])

dnl libpthread
AC_LIB_HAVE_LINKFLAGS([pthread])

dnl libzlib
CHECK_ZLIB
AM_CONDITIONAL([ZLIB], [test "x$have_zlib" != "xno"])
if test "x$have_zlib" != "xno"; then
  AC_DEFINE([PDF_HAVE_LIBZ], [1], [Define to 1 if you have the `z' library])
fi

dnl libjpeg
AC_LIB_HAVE_LINKFLAGS([jpeg])
AM_CONDITIONAL([LIBJPEG], [test "x$HAVE_LIBJPEG" != "xno"])
if test "x$HAVE_LIBJPEG" = "xyes"; then
  AC_DEFINE([PDF_HAVE_LIBJPEG], [1], [Define to 1 if you have the `jpeg' library])
fi

dnl libjbig2dec
AC_LIB_HAVE_LINKFLAGS([jbig2dec])
AM_CONDITIONAL([JBIG2DEC], [test "x$HAVE_LIBJBIG2DEC" != "xno"])
if test "x$HAVE_LIBJBIG2DEC" = "xyes"; then
  AC_DEFINE([PDF_HAVE_LIBJBIG2DEC], [1], [Define to 1 if you have the `jbig2dec' library])
fi

dnl libm
AC_LIB_HAVE_LINKFLAGS([m])

dnl libgpg-error
AC_LIB_HAVE_LINKFLAGS([gpg-error])

dnl libgcrypt
AC_LIB_HAVE_LINKFLAGS([gcrypt],[gpg-error],[#include <gpg-error.h>])

dnl libcheck
AC_LIB_HAVE_LINKFLAGS([check])
dnl We need a recent version of libcheck supporting 'srunner_run'.
AC_CHECK_LIB([check],[srunner_run],[HAVE_SRUNNER_RUN=yes],[HAVE_SRUNNER_RUN=no])
AM_CONDITIONAL([CHECK], [test "x$HAVE_CHECK" = "xyes"])

dnl libiconv
AC_LIB_HAVE_LINKFLAGS([iconv])
AM_CONDITIONAL([ICONV], [test "x$HAVE_LIBICONV" = "xyes"])

dnl Search for required programs for building the sources
AWK=""
AC_PROG_AWK
AC_PATH_PROG([PATH_TO_AWK],$AWK)
AC_SUBST(PATH_TO_AWK)
AC_PATH_PROG([CHMOD],[chmod])

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
AM_CONDITIONAL([COMPILE_OBJECT_LAYER], [test "x$PDFLIB_LEVEL" = "x1" || \
                                        test "x$PDFLIB_LEVEL" = "x2" || \
                                        test "x$PDFLIB_LEVEL" = "x3"])
AM_CONDITIONAL([COMPILE_DOCUMENT_LAYER], [test "x$PDFLIB_LEVEL" = "x2" || \
                                          test "x$PDFLIB_LEVEL" = "x3"])
AM_CONDITIONAL([COMPILE_PAGE_LAYER], [test x$PDFLIB_LEVEL = "x3"])


dnl Project management resources
AC_ARG_ENABLE([prmgt], AS_HELP_STRING([--enable-prmgt],
                                      [build project management resources (default is NO)]),
              [prmgt_mode=$enableval], [prmgt_mode=no])

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

AC_PATH_PROG([TCLSH], [tclsh], [no],)
if test "x$TCLSH" = "xno"; then
AC_MSG_ERROR([cannot find a suitable tcl interpreter. Please install it and rerun `configure'.])
fi

fi # prmgt_mode

# debug_*_mode
PDFLIB_DEBUG_DESCR=""
AC_ARG_ENABLE([debug-base],
              AS_HELP_STRING([--enable-debug-base],
                             [debug mode at the base layer (default is NO)]),
              [debug_base_mode=$enableval], [debug_base_mode=no])

AC_ARG_ENABLE([debug-object],
               AS_HELP_STRING([--enable-debug-object],
                              [debug mode at the object layer (default is NO)]),
              [debug_object_mode=$enableval], [debug_object_mode=no])

AC_ARG_ENABLE([debug-document],
               AS_HELP_STRING([--enable-debug-document],
                              [debug mode at the document layer (default is NO)]),
              [debug_document_mode=$enableval], [debug_document_mode=no])

AC_ARG_ENABLE([debug-page],
              AS_HELP_STRING([--enable-debug-page],
                             [debug mode at the page layer (default is NO)]),
              [debug_page_mode=$enableval], [debug_page_mode=no])


dnl Note that these symbols are defined in both the public (pdf-config.h)
dnl   and private (config.h) configuration headers
if test "x$debug_base_mode" = "xyes"; then
  AC_DEFINE([PDF_HAVE_DEBUG_BASE], [1], [Debugging Base Layer])
  PDFLIB_DEBUG_DESCR="base, "
fi

if test "x$debug_object_mode" = "xyes"; then
  AC_DEFINE([PDF_HAVE_DEBUG_OBJECT], [1], [Debugging Object Layer])
  PDFLIB_DEBUG_DESCR="${PDFLIB_DEBUG_DESCR}object, "
fi

if test "x$debug_document_mode" = "xyes"; then
  AC_DEFINE([PDF_HAVE_DEBUG_DOCUMENT], [1], [Debugging Document Layer])
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

dnl gcov compilation
AC_ARG_ENABLE([coverage],
              AS_HELP_STRING([--enable-coverage],
                             [Compile the library with code coverage support (default is NO)]),
              [use_gcov=$enableval], [use_gcov=no])
AM_CONDITIONAL([USE_COVERAGE], [test "x$use_gcov" = "xyes"])

AC_ARG_ENABLE([fsys-http],
               AS_HELP_STRING([--enable-fsys-http],
                              [Include the HTTP filesystem implementation in the build (default is NO)]),
              [fsys_http=$enableval], [fsys_http=no])

http_implementation=no
if test "x$fsys_http" = "xyes"; then
   AC_LIB_HAVE_LINKFLAGS([curl])
   if test "x$HAVE_LIBCURL" = "xyes"; then
     http_implementation=yes
     AC_DEFINE([PDF_FSYS_HTTP],[1],[Include the HTTP filesystem in the build.])
  fi
fi

AM_CONDITIONAL([FSYS_HTTP],
               [test "x$fsys_http" != "xno" && test "x$HAVE_LIBCURL" = "xyes"])

dnl nocheck (simple replacement of Check framework when this is not available)
AC_ARG_ENABLE([nocheck],
              AS_HELP_STRING([--enable-nocheck],
                             [Use built-in nocheck instead of Check (default is NO)]),
              [use_nocheck=$enableval], [use_nocheck=no])
if test "x$use_nocheck" = "xyes"; then
  AC_DEFINE([HAVE_NOCHECK], [1], [Use No-Check instead of Check])
fi
AM_CONDITIONAL([NOCHECK], [test "x$use_nocheck" = "xyes"])



dnl Check if Unit testing support is available with Check or No-Check
ut_support=no
AM_CONDITIONAL([COMPILE_UT], [test "x$use_nocheck" = "xyes" || test "x$ac_cv_lib_check_srunner_run" = "xyes"])
if test "x$use_nocheck" = "xyes"; then
  ut_framework="(builtin No-Check)"
  ut_support=yes
else
  if test "x$ac_cv_lib_check_srunner_run" = "xyes"; then
    ut_framework="(Check)"
    ut_support=yes
  fi
fi

dnl Get system endianness information
AC_C_BIGENDIAN()
if test "x$ac_cv_c_bigendian" = "xno"; then
  AC_DEFINE([PDF_IS_BIG_ENDIAN], [0], [Big endian?])
else
  AC_DEFINE([PDF_IS_BIG_ENDIAN], [1], [Big endian?])
fi


dnl Specific W32 needs of compilation
AM_CONDITIONAL([COMPILE_W32_SYSTEM], [test "x$compile_w32_system" = "xyes"])

GNUPDF_VERSION=$VERSION
AC_SUBST(GNUPDF_VERSION)


dnl Some additional stuff we need to have properly defined in the pdf.h
dnl Note that gl_cv_c_inline_effective comes from m4/inline.m4
if test $gl_cv_c_inline_effective = yes; then
   AC_DEFINE([PDF_HAVE_INLINE], [1], [System has inline?])
fi

dnl Generate output files
AC_CONFIG_FILES([Makefile
                 libgnupdf.pc
                 build-aux/Makefile
                 build-aux/generate-tsd.pl
                 lib/Makefile
                 src/Makefile
                 src/extract-public-hdr
                 torture/Makefile
                 torture/testdata/Makefile
                 torture/unit/Makefile
                 utils/Makefile
                 prmgt/Makefile
                 prmgt/apic2wiki
                 prmgt/testlog2wiki
                 prmgt/docfuncs
                 prmgt/tsdfuncs
                 prmgt/check-api-doc-consistency.pl
                 doc/Makefile])

AC_OUTPUT

dnl Report errors

missing_programs=""
missing_libs=""
outdated_libs=""

dnl Please update the variables above to achieve this visual
dnl appearance:
dnl
dnl The following blah blah:
dnl
dnl    lib1 (> 2.3.4)
dnl    lib2 (> 1.2.3)
dnl    lib3 (svn required)

if test "x$HAVE_LIBM" != "xyes"; then
   missing_libs="
   libm"
fi
if test "x$HAVE_LIBPTHREAD" != "xyes"; then
   missing_libs="$missing_libs
   libpthread"
fi
if test "x$HAVE_LIBUUID" != "xyes"; then
   missing_libs="$missing_libs
   libuuid"
fi
if test "x$HAVE_LIBGPG_ERROR" != "xyes"; then
   missing_libs="$missing_libs
   libgpg-error"
fi
if test "x$HAVE_LIBGCRYPT" != "xyes"; then
   missing_libs="$missing_libs
   libgcrypt"
fi
if test "x$HAVE_LIBCHECK" != "xyes"; then
   missing_libs="$missing_libs
   libcheck"
fi
if test "x$fsys_http" = "xyes" && test "x$HAVE_LIBCURL" = "xno"; then
   missing_libs="$missing_libs
   libcurl"
fi
if test "x$HAVE_LIBCHECK" = "xyes" && test "x$HAVE_SRUNNER_RUN" != "xyes"; then
   outdated_libs="$outdated_libs
   libcheck (svn required)"
fi

if test "x$AWK" = "x"; then
   missing_programs="$missing_programs
   awk"
fi

if test -n "$missing_libs"; then
   echo ""
   echo "The following required libraries are not available:"
   echo "$missing_libs"
   echo ""
   echo "Please install them and rerun 'configure'."
fi

if test -n "$outdated_libs"; then
   echo ""
   echo "The following required libraries are too old:"
   echo "$outdated_libs"
   echo ""
   echo "Please update them and rerun 'configure'."
fi

if test -n "$missing_programs"; then
   echo ""
   echo "The following required programs are missing:"
   echo "$missing_programs"
   echo ""
   echo "Please install them and rerun 'configure'."
fi

if test -n "$missing_libs" || test -n "$outdated_libs" || test -n "$missing_programs"; then
   exit 1
fi

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
  Using JBIG2 decoder filter?               ${HAVE_LIBJBIG2DEC}
  Using DCT filter?                         ${HAVE_LIBJPEG}
  With http filesystem support?             ${http_implementation}
  With unit tests support?                  ${ut_support} ${ut_framework}
  Program to build html manuals             ${texihtmlprogram}
  Build project management resources?       ${prmgt_mode}
  Layers with debug mode enabled            ${PDFLIB_DEBUG_DESCR}
  Generate code coverage information        ${use_gcov}
"
dnl Report warnings

if test "x$have_zlib" = "xno" ; then
echo "
WARNING: you are going to build a library without FlateDecode support.
         That means you will not be able to process many PDF files."
fi

if test "x$HAVE_LIBJBIG2DEC" = "xno" ; then
echo "
WARNING: you are going to build a library without JBIG2Decode support.
         That means you will not be able to process many PDF files."
fi

if test "x$HAVE_LIBJPEG" = "xno" ; then
echo "
WARNING: you are going to build a library without JPEG support.
         That means you will not be able to process many PDF files."
fi

if test "x$ut_support" = "xno" ; then
echo "
WARNING: you are going to build the library without unit testing support.
         If you want to be able to perform the unit testing you should
         either install the 'check' library or run the configure with
         the --enable-nocheck option."
fi

dnl End of configure.ac

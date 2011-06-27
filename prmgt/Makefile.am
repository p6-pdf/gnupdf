# prmgt directory Makefile.am
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

if PRMGT

TARGETS = cyclo.html apic.html ut.html

cyclo.html:
	chmod +x ${top_srcdir}/build-aux/pmccabe2html
	$(PMCCABE) ${top_srcdir}/src/base/*.[ch] \
                | sort -nr \
                | $(AWK) -f ${top_srcdir}/build-aux/pmccabe2html    \
                         -v lang=html -v name="GNU PDF"             \
                         -v vcurl="%FILENAME%"                      \
                         -v css=${top_srcdir}/build-aux/pmccabe.css \
                         -v url="http://www.gnupdf.org"             \
                  > cyclo.html

apic.html: apic2html
	chmod +x ${builddir}/apic2html
	chmod +x ${builddir}/check-api-doc-consistency.pl
	${builddir}/check-api-doc-consistency.pl -p -t |    \
          $(AWK) -f ${builddir}/apic2html                   \
                 -v name="GNU PDF"                          \
                 -v css=${top_srcdir}/build-aux/pmccabe.css \
                 -v url="http://www.gnupdf.org"             \
          > apic.html

ut.html: testlog2html
	chmod +x ${builddir}/testlog2html
	if test -f ${top_srcdir}/torture/unit/ut.log ; then   \
          cat ${top_srcdir}/torture/unit/ut.log |             \
            $(AWK) -f ${builddir}/testlog2html                \
                   -v name="GNU PDF"                          \
                   -v css=${top_srcdir}/build-aux/pmccabe.css \
                   -v url="http://www.gnupdf.org"             \
            > ut.html;                                        \
        fi

all-local: ${TARGETS}

clean-local:
	-rm -f ${TARGETS}
	-rm -f month-calendar.html status.html \
               week-calendar.html resources.html tasks.html
	-rm -f libgnupdf.tjx
	-rm -f *_fn.txt

endif # PRMGT

# End of Makefile.am

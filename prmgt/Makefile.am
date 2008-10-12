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

TARGETS = cyclo.html cyclo.wiki apic.wiki ut.wiki

cyclo.html:
	chmod +x ${top_srcdir}/build-aux/pmccabe2html
	$(PMCCABE) ${top_srcdir}/src/base/*.[ch] \
                | sort -nr \
                | $(AWK) -f ${top_srcdir}/build-aux/pmccabe2html \
                         -v lang=html -v name="GNU PDF" \
                         -v vcurl="%FILENAME%" \
                         -v url="http://www.gnupdf.org" \
                  > cyclo.html

cyclo.wiki:
	chmod +x ${top_srcdir}/build-aux/pmccabe2html
	$(PMCCABE) ${top_srcdir}/src/base/*.[ch] \
                | sort -nr \
                | $(AWK) -f ${top_srcdir}/build-aux/pmccabe2html \
                         -v lang=wiki -v name="GNU PDF" \
                  > cyclo.wiki

apic.wiki:
	chmod +x ${srcdir}/apic2wiki
	${srcdir}/check-api-doc-consistency.pl -p | ${srcdir}/apic2wiki > apic.wiki

ut.wiki:
	chmod +x ${srcdir}/testlog2wiki
	if test -f ${top_srcdir}/torture/unit/ut.log ; then cat ${top_srcdir}/torture/unit/ut.log | ${srcdir}/testlog2wiki > ut.wiki; fi

if TASKJUGGLER

TARGETS += libgnupdf-dot.png \
           libgnupdf-fdp.png

libgnupdf.tjx : libgnupdf.tjp
	taskjuggler libgnupdf.tjp

libgnupdf-dot.png: libgnupdf.tjx
	zcat libgnupdf.tjx | xsltproc --novalid pertdot.xsl - | dot -T png > libgnupdf-dot.png

libgnupdf-fdp.png: libgnupdf.tjx
	zcat libgnupdf.tjx | xsltproc --novalid pertfdp.xsl - | fdp -T png > libgnupdf-fdp.png

endif # TASKJUGGLER


all-local: ${TARGETS}

clean-local:
	-rm -f ${TARGETS}
	-rm -f month-calendar.html status.html \
               week-calendar.html resources.html tasks.html
	-rm -f libgnupdf.tjx
	-rm -f *_fn.txt

endif # PRMGT

# End of Makefile.am

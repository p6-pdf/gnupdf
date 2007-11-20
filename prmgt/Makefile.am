# prmgt directory Makefile.am
# GNU PDF Library

# Copyright (C) 2007 Free Software Foundation, Inc.

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

TARGETS = libgnupdf-dot.png \
          libgnupdf-fdp.png  \
          cyclo.html

libgnupdf-dot.png: libgnupdf.tjx
	zcat libgnupdf.tjx | xsltproc --novalid pertdot.xsl - | dot -T png > libgnupdf-dot.png

libgnupdf-fdp.png: libgnupdf.tjx
	zcat libgnupdf.tjx | xsltproc --novalid pertfdp.xsl - | fdp -T png > libgnupdf-fdp.png

cyclo.html:
	chmod +x ${srcdir}/pmccabe2html
	pmccabe ${top_srcdir}/src/*.[ch] | sort -nr | ${srcdir}/pmccabe2html > cyclo.html


libgnupdf.tjx : libgnupdf.tjp
	taskjuggler libgnupdf.tjp

all-local: ${TARGETS}

clean-local:
	-rm -f ${TARGETS}
	-rm -f month-calendar.html status.html \
               week-calendar.html resources.html tasks.html
	-rm -f libgnupdf.tjx
	-rm -f *_fn.txt
	-rm -f pmccabe2html

endif # PRMGT

# End of Makefile.am

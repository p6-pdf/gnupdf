# Documentation directory Makefile.am
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

noinst_TEXINFOS = gnupdf-tsd.texi gnupdf-hg.texi gnupdf-arch.texi
info_TEXINFOS = gnupdf.texi gnupdf-utils.texi
gnupdf_TEXINFOS = fdl.texi

EXTRA_DIST = gnupdf-arch-figures gnupdf-figures gnupdf-hg-figures

AM_MAKEINFOHTMLFLAGS = --css-include=manuals.css

gnupdf-tsd.texi: generated-tsd.texi
generated-tsd.texi:
	perl $(srcdir)/generate-tsd.pl > $(srcdir)/generated-tsd.texi

manuals: gnupdf-tsd.texi
	../build-aux/gendocs.sh -o gnupdf-manual \
                                --email  pdf-devel@gnu.org \
                                gnupdf "GNU PDF Reference Manual"
	../build-aux/gendocs.sh -o gnupdf-tsd-manual \
                                --email  pdf-devel@gnu.org \
                                gnupdf-tsd "GNU PDF Test Specification Document"
	../build-aux/gendocs.sh -o gnupdf-arch-manual \
                                --email  pdf-devel@gnu.org \
                                gnupdf-arch "GNU PDF Architecture Guide"
	../build-aux/gendocs.sh -o gnupdf-hg-manual \
                                --email  pdf-devel@gnu.org \
                                gnupdf-hg "GNU PDF Hacker's Guide"
	../build-aux/gendocs.sh -o gnupdf-utils-manual \
                                --email  pdf-devel@gnu.org \
                                gnupdf-utils "GNU PDF Utilities Manual"

if TEXI2HTML

wiki: gnupdf.wiki gnupdf-utils.wiki gnupdf-tsd.wiki gnupdf-hg.wiki

gnupdf.wiki: gnupdf.texi
	texi2html --no-menu --init-file=gnupdf-manual-wiki.init gnupdf.texi

gnupdf-utils.wiki: gnupdf-utils.texi
	texi2html --no-menu --init-file=gnupdf-manual-wiki.init gnupdf-utils.texi

gnupdf-tsd.wiki: generate-tsd gnupdf-tsd.texi
	texi2html --no-menu --init-file=gnupdf-manual-wiki.init gnupdf-tsd.texi

gnupdf-hg.wiki: gnupdf-hg.texi
	texi2html --no-menu --init-file=gnupdf-manual-wiki.init gnupdf-hg.texi

endif # TEXI2HTML

clean-local:
	-rm -f gnupdf.info gnupdf-utils.info gnupdf-hg.info gnupdf-tsd.info
	-rm -f gnupdf.wiki gnupdf-utils.wiki gnupdf-hg.wiki gnupdf-tsd.wiki
	-rm -f gnupdf.html gnupdf-utils.html gnupdf-utils.dvi
	-rm -f gnupdf-arch.info gnupdf-arch.html gnupdf-arch.wiki
	-rm -r gnupdf-arch-manual
	-rm -r gnupdf-hg-manual
	-rm -r gnupdf-manual
	-rm -r gnupdf-utils-manual
	-rm -r gnupdf-tsd-manual

# End of Makefile.am

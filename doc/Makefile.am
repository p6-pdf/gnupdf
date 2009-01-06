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

AM_MAKEINFOHTMLFLAGS = --css-include=manuals.css

if TEXI2HTML

wiki: gnupdf.wiki gnupdf-utils.wiki gnupdf-tsd.wiki gnupdf-hg.wiki

gnupdf.wiki: gnupdf.texi
	texi2html --no-menu --init-file=gnupdf-manual-wiki.init gnupdf.texi

gnupdf-utils.wiki: gnupdf-utils.texi
	texi2html --no-menu --init-file=gnupdf-manual-wiki.init gnupdf-utils.texi

gnupdf-tsd.wiki: gnupdf-tsd.texi
	texi2html --no-menu --init-file=gnupdf-manual-wiki.init gnupdf-tsd.texi

gnupdf-hg.wiki: gnupdf-hg.texi
	texi2html --no-menu --init-file=gnupdf-manual-wiki.init gnupdf-hg.texi

clean-local:
	-rm -f gnupdf.info gnupdf-utils.info gnupdf-hg.info gnupdf-tsd.info
	-rm -f gnupdf.wiki gnupdf-utils.wiki gnupdf-hg.wiki gnupdf-tsd.wiki
	-rm -f gnupdf.html gnupdf-utils.html

endif # TEXI2HTML

# End of Makefile.am

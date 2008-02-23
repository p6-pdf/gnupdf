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

EXTRA_DIST = pdf_filter.1

noinst_TEXINFOS = gnupdf-tsd.texi
info_TEXINFOS = gnupdf.texi gnupdf-utils.texi
gnupdf_TEXINFOS = fdl.texi

man_MANS = pdf_filter.1

AM_MAKEINFOHTMLFLAGS = --no-headers --no-split

if TEXI2HTML

html-local:
	texi2html --init-file=gnupdf-manual.init --css-include=manuals.css gnupdf.texi
	texi2html --init-file=gnupdf-manual.init --css-include=manuals.css gnupdf-utils.texi
	texi2html --init-file=gnupdf-manual.init --css-include=manuals.css gnupdf-tsd.texi

wiki: gnupdf.wiki gnupdf-utils.wiki gnupdf-tsd.wiki

gnupdf.wiki: gnupdf.texi
	texi2html --no-menu --init-file=gnupdf-manual-wiki.init gnupdf.texi

gnupdf-utils.wiki: gnupdf-utils.texi
	texi2html --no-menu --init-file=gnupdf-manual-wiki.init gnupdf-utils.texi

gnupdf-tsd.wiki: gnupdf-tsd.texi
	texi2html --no-menu --init-file=gnupdf-manual-wiki.init gnupdf-tsd.texi

clean-local:
	-rm -f gnupdf.wiki gnupdf-utils.wiki
	-rm -f gnupdf.html gnupdf-utils.html

endif # TEXI2HTML

# End of Makefile.am

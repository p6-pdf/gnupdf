# Documentation directory Makefile.am
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

EXTRA_DIST = pdf_filter.1

info_TEXINFOS = gnupdf.texi gnupdf-utils.texi
gnupdf_TEXINFOS = fdl.texi

man_MANS = pdf_filter.1

if TEXI2HTML

MAKEINFOHTML = texi2html --init-file=gnupdf-manual.init --css-include=manuals.css

endif # TEXI2HTML

# End of Makefile.am

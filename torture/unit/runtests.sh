#!/bin/sh

# Copyright (C) 2009 Free Software Foundation, Inc.
#
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

# Prologue
chmod a-r ${top_srcdir}/torture/testdata/TD00004
chmod a-w ${top_srcdir}/torture/testdata/TD00005

# Run the test suite
if test "x$PDF_HOST" = "xPDF_HOST_WIN32"
then
    CK_FORK=no \
    CK_VERBOSITY=normal \
    ${WINE} ${srcdir}/runtests.exe
else
    ${srcdir}/runtests
fi

# Epilog
chmod 644 ${top_srcdir}/torture/testdata/TD00004
chmod 644 ${top_srcdir}/torture/testdata/TD00005

# End of runtests.sh

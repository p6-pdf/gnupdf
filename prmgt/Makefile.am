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

if TASKJUGGLER

REPORTS = month-calendar.html \
	status.html \
	week-calendar.html \
	resources.html \
	tasks.html

all-local: libgnupdf.tjp
	taskjuggler libgnupdf.tjp

clean-local:
	-rm -f ${REPORTS}

endif # TASKJUGGLER

# End of Makefile.am

#!/usr/bin/env perl

# Copyright (C) 2008 Free Software Foundation, Inc.

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

use warnings;
use strict;

##
# This script traverses every directory in the root and looks for a
# "MANIFEST.wiki" file, from which the directory information is then extracted.
###

my $TOPDIR="..";
my $DOCFILE = "MANIFEST.wiki";

sub print_docfile
{
    my $str = shift;
    open (FILE, $str);
    print <FILE>;
    print "\n"; # in case it's not at end of FILE.
    close(FILE);
}



##
# MAIN PROGRAM
###

my @dirs;

opendir (DIR, $TOPDIR) or die "Couldn't open $TOPDIR\n";
@dirs = grep { !m/^\./ && -d "$TOPDIR/$_" } readdir(DIR);
closedir(DIR);

# push these by hand
push (@dirs, ""); # for TOPDIR
push (@dirs, "src/base");
push (@dirs, "src/object");
push (@dirs, "src/document");
push (@dirs, "src/page");

foreach my $dir (sort(@dirs)){
    my $file = $TOPDIR . '/' . $dir . '/' . $DOCFILE;
    if (-e $file and -r $file){
        print_docfile ($file);
    }
}

0;

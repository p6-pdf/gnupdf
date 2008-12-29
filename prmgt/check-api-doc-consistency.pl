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
# This script takes every procedure documented on the texinfo document "gnupdf.texi"
# and match it with the procedures implemented in the header files.
###

my $TOPDIR="..";

my $SRCDIR = $TOPDIR."/src";
my $INFODIR = $TOPDIR."/doc";
my $MAKEFILE = $SRCDIR."/Makefile.am";
my $INFODOC = $INFODIR ."/gnupdf.texi";

my @ALLSRCS = ();

my (@IMPLSIGS, @DOCSIGS);


sub clean_docsigs
{
    my $str = shift;
    my $novars = shift;

    # remove newlines
    $str =~ tr/\n/ /;
    # remove info syntax
    $str =~ s/(\@deftypefun |\@var)//g;
    # remove curly brackets
    $str =~ s/\s*(\{|\})\s*/ /g;
    # remove comma spaces
    $str =~ s/\s*,\s*/,/g;
    # remove pointer spaces
    $str =~ s/\s*\*\s*/\*/g;
    # remove bracket spaces
    $str =~ s/\s*(\(|\))\s*/$1/g;
    # remove word spaces
    $str =~ s/\s*(\w+)\s+/$1 /g;
    # remove square brackets spaces
    $str =~ s/\s*(\[\s*\])\s*/[]/g;

    if ($novars == 1){
        # remove variable names
        $str =~ s/(\w+)(,|\))/$2/g;
    }
#    print $str . "\n";
    return $str;
}


sub clean_implsigs
{
    my $fd = shift;
    my $novars = shift;
    my ($nextchar, $char, $lines);

    # remove comments
    $lines ="";
  READCHAR:
    while (read ($fd, $char, 1))
    {
        if ($char eq "/")
        {
            if (read ($fd, $nextchar, 1) && $nextchar eq "*")
            {
              EATCOMMENT:
                while (read ($fd, $nextchar, 1) && $nextchar ne "*" )
                {
                    # shh...
                }
                if (read ($fd, $nextchar, 1) && $nextchar eq "/" )
                {
                    goto READCHAR;
                }
                goto EATCOMMENT;
            }else{
                $char .= $nextchar;
            }

        }
        
        $lines .= $char;
    }

    # remove newlines
    $lines =~ tr/\n/ /;
    # remove comma spaces
    $lines =~ s/\s*,\s*/,/g;
    # remove pointer spaces
    $lines =~ s/\s*\*\s*/\*/g;
    # remove bracket spaces
    $lines =~ s/\s*(\(|\))\s*/$1/g;
    # remove word spaces
    $lines =~ s/\s*(\w+)\s+/$1 /g;
    # remove square brackets spaces
    $lines =~ s/\s*(\[\s*\])\s*/[]/g;

    if ($novars == 1){
        # remove variable names
        $lines =~ s/(\w+)(,|\))/$2/g;
    }
#    print "\n\n\n";
#    print $lines . "\n\n\n";
    return ($lines);
}


sub read_implsigs
{
    my $file = shift; 
    my $novars = shift;
    my $fd;

    if (open ($fd, $file))
    {
        push (@IMPLSIGS, clean_implsigs ($fd,$novars));
        close($fd);
    }else{
        print "Couldn't open $file\n";
    }

}

sub read_docsigs
{
    my $file = shift;
    my $novars = shift;

    open (FILE, $file); 
    while (<FILE>)
    {
        if (m/^\@deftypefun/)
        {
            push (@DOCSIGS, clean_docsigs($_,$novars));
        }
    }
    close (FILE);
}


sub read_filenames
{
    my $file = shift;
    open (MAKE, $file);
    my ($found, $path);
    $found=0;
    while (<MAKE>)
    {
        if (m/^PUBLIC_HDRS +=/)
        {
            $found = 1;
            last;
        }       
    }

    if ($found)
    {
        $path = $_;
        chop $path;
        $path =~ s/^PUBLIC_HDRS.+(pdf.+\.h).*/$1/;
        push (@ALLSRCS,$path);
      READMORE:
        while (<MAKE>)
        {
            $path = $_;
            chop $path;
            $path =~ s/^ *(\w+\/pdf.+\.h).*/$1/;
            push (@ALLSRCS,$path);
            
            if (m/\\/)
            {
                goto READMORE;
            }else{
                last;
            }
        }
    }
    close (MAKE);
}


sub show_help
{
    print "\nUusage: $0 [OPT]\n";
    print "OPT is one of:\n";
    print "\t -h:\tprint this help.\n";
    print "\t -f:\tshow failing checks only.\n";
    print "\t -i:\tignore variable names.\n";
    print "\t -p:\tplain mode: do not print a resume.\n\n";
}

##
# MAIN PROGRAM
###
my ($reg, $matched, $fails, $total, $showok,$plainmode, $ignorevars);


$showok = 1;
$plainmode = 0;
$ignorevars = 0;
if (grep ($_ eq '-h', @ARGV)){
    show_help ();
    exit(0);
}
if (grep ($_ eq '-f', @ARGV)){
    $showok = 0;
}
if (grep ($_ eq '-p', @ARGV)) {
    $plainmode = 1;
}
if (grep ($_ eq '-i', @ARGV)) {
    $ignorevars = 1;
}


# fill @ALLSRCS
read_filenames ($MAKEFILE);
# fill @DOCSIGS
read_docsigs($INFODOC, $ignorevars);

foreach my $file (@ALLSRCS)
{
    # fill @IMPLSIGS
    read_implsigs (join('/',$SRCDIR,$file), $ignorevars);
}


$fails = 0;
$total = 0;
foreach my $docsig (@DOCSIGS)
{
    $matched=0;
    $reg = quotemeta($docsig);
#    print " >>>> $reg" . "\n";
    foreach my $implsig (@IMPLSIGS){
        if ($implsig =~ m/$reg/){
            $matched = 1;
            last;
        }
    }
    
    $docsig =~ s/.*[* ]+(\w+)\(.*/$1/;
    if ($matched)
    {
        if ($showok){
            print "$docsig() ... ok\n";
        }
    }else{
        $fails++;
        print "$docsig() ... failed\n";
    }
    $total++;
}

if ($plainmode == 0)
{
    print "\n--- Failed $fails of $total function signatures ---\n";
}

0;

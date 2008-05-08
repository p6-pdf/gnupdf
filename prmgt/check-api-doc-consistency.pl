#!/usr/bin/env perl
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
    # remove newlines
    $str =~ tr/\n/ /;
    # remove info syntax
    $str =~ s/(\@deftypefun |\@var)//g;
    # remove curly brackets
    $str =~ s/ *(\{|\}) */ /g;
    # remove comma spaces
    $str =~ s/ *, */,/g;
    # remove pointer spaces
    $str =~ s/ *\* */\*/g;
    # remove bracket spaces
    $str =~ s/ *(\(|\)) */$1/g;
    # remove word spaces
    $str =~ s/ *(\w+) +/$1 /g;

    return $str;
}


sub clean_implsigs
{
    my $fd = shift;
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
    $lines =~ s/ *, */,/g;
    # remove pointer spaces
    $lines =~ s/ *\* */\*/g;
    # remove bracket spaces
    $lines =~ s/ *(\(|\)) */$1/g;
    # remove word spaces
    $lines =~ s/ *(\w+) +/$1 /g;

    return ($lines);
}


sub read_implsigs
{
    my $file = shift; 
    my $fd;

    if (open ($fd, $file))
    {
        push (@IMPLSIGS, clean_implsigs ($fd));
        close($fd);
    }else{
        print "Couldn't open $file\n";
    }

}

sub read_docsigs
{
    my $file = shift;

    open (FILE, $file); 
    while (<FILE>)
    {
        if (m/^\@deftypefun/)
        {
            push (@DOCSIGS, clean_docsigs($_));
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
    print "\t -f:\tshow failing checks only.\n\n";
}

##
# MAIN PROGRAM
###
my ($reg, $matched, $fails, $total, $showok);


$showok = 1;
if (@ARGV == 1){
    if ($ARGV[0] eq '-h'){
        show_help ();
        exit(0);
    }elsif ($ARGV[0] eq '-f'){
        $showok = 0;
    }
}

read_filenames ($MAKEFILE);
read_docsigs($INFODOC);

foreach my $file (@ALLSRCS)
{
    read_implsigs (join('/',$SRCDIR,$file));
}


$fails = 0;
$total = 0;
foreach my $docsig (@DOCSIGS)
{
    $matched=0;
    $reg = quotemeta($docsig);
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

print "\n--- Failed $fails of $total function signatures ---\n";

0;

#!/bin/bash

# Copyright (C) 2009,2011 Free Software Foundation, Inc.
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


# This first part of this shell script takes a series of test inputs and  
# runs these inputs through the pdf-filter utility through the ASCII85 
# encoder and decoder.  These test inputs are pushed through the filters
# with varying cache sizes to further stress test the filter implementations.
#
# The second portion of this shell script pushes all the files from
# /torture/testdata/ through the encoder and then decoder and verifies
# that the end-result file matches the starting file by using md5sum.
# This is a convenient method of pushing large binary files through the
# filter and automatically testing the results - just copy the test files
# into the /torture/testdata/ directory and execute this script.
#
# This shell script was inspired by a simple test that jemarch executed
# using pdf-filter.  It went like this:
#
#$ echo -ne "one\ntwo\nthree" | ./utils/pdf-filter --cache=1 --a85enc --a85dec
#
# 

FILTER_EXEC=../../utils/pdf-filter

# Define the test data as A85_TD_x where x is a digit having no leading zeroes.
# Be certain to update the inner for-loop below to reflect the number of input
# data items defined. 

A85_TD_0="1"
A85_TD_1="12"
A85_TD_2="123"
A85_TD_3="1234"
A85_TD_4="1\n2"
A85_TD_5="1\n2\t3"
A85_TD_6="\t1\n2\t3\n4"
A85_TD_7="1234\n"
A85_TD_8="12345\t"
A85_TD_9="123456"
A85_TD_10="1234567"
A85_TD_11="12345678"
A85_TD_12="123456789"
A85_TD_13="1234567890"
A85_TD_14="12345678901"
A85_TD_15="123456789012"
A85_TD_16="\n1\n2\n3\n4\n5\n6\n7\n8\n\n9\n\n\n1011121314151617181920"
A85_TD_17="\n1\t2\n3\n4\t5\n6\t7\n8\t\n9\t\n\t1011121314151617181920a"
A85_TD_18="\t1\t2\n3\t4\t5\n6\t7\t8\n\t9\t\n\t1011121314151617181920ab"
A85_TD_19="\t1\t2\n3\t4\t5\t6\n7\t8\t\t9\n\t\t1011121314151617181920abc"
A85_TD_20="\t1\n2\t3\t4\t5\t6\n7\t8\t\t9\t\n\t1011121314151617181920abcd"
A85_TD_21="\t1\t2\t3\t4\n5\t6\t7\n8\t\n9\t\n\t1011121314151617181920abcde"
A85_TD_22="abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ~>~>!@#$\',.%&*:\\^=(){}?/-+<~<~,.~>"

# The outer for loop controls the cache size

for ((i=1; i<=10; i++)) ; do { 

  echo -ne "\nTesting --cache=$i\n"  

  for k in A85_TD_{0..22} ; do {

   if echo -ne ${!k} | $FILTER_EXEC --cache=$i --a85enc --a85dec > test_out ;
     then {
       l=$(< test_out)
       echo -ne ${!k} > test_in
       m=$(< test_in)
       if test "$l" = "$m" ; 
         then {
           echo -ne " Pass\n"
         } ; 
         else {
           echo -ne "\n\nFailure Detected. --cache=$i \n\n"
           echo "$k:\"$m\""
           echo "File:\"$l\""
           exit 1
         } ; 
       fi
     } ;
     else {
      echo -ne "\n\nFailure #2 Detected\n"
      exit 1 
     } ;
   fi

  }; done

}; done


# This portion of the script pushes files from torture/testdata/ through
# the filter - automatically checking the results.

echo -ne "\n\nTesting encoding and decoding of test data files\n\n"

for tdf in ../testdata/* ; do {
  echo -ne "$tdf ... "
  $FILTER_EXEC -i $tdf -o test.a85 --a85enc
  $FILTER_EXEC -i test.a85 -o test.dec --a85dec
  md5sum $tdf | awk -- '{print $1}' > in.md5
  n=$(< in.md5)
  md5sum test.dec | awk -- '{print $1}' > out.md5
  o=$(< out.md5)
  if test "$n" = "$o" ;
    then {
      echo -ne "Pass\n"
    } ;
    else {
      echo -ne "Failure\n\n"
      exit 1
    } ;
  fi
} ; done

echo -ne "\n\nAll tests PASS!\n\n"

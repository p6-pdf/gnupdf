# !/bin/bash

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


BASE_UCD_URL=http://www.unicode.org/Public/UNIDATA/

check_file ()
{
  if [ ! -f `basename $1` ]; then
      echo ""
      echo ""
      echo "Dowloading $1 file from... $BASE_UCD_URL"
	  echo "************************************************************"
      echo ""
      wget $BASE_UCD_URL/$1
	  if [ $? -ne 0 ]; then
        echo "An error happened when downloading $1"
		exit 1;
	  fi
	  echo "File $1 correctly downloaded!"
	  echo "************************************************************"
      echo ""
      echo ""
  fi
}


#Generate Casing Info file...
check_file UnicodeData.txt
check_file SpecialCasing.txt
if [ -f UnicodeDataCaseInfo.c ]; then
  rm UnicodeDataCaseInfo.c
fi
./pdf-text-generate-ucd UnicodeData.txt SpecialCasing.txt CASE


#Generate General Category file...
check_file UnicodeData.txt
if [ -f UnicodeDataGenCatInfo.c ]; then
  rm UnicodeDataGenCatInfo.c
fi
./pdf-text-generate-ucd UnicodeData.txt GENCAT


#Generate Combining Class file...
check_file UnicodeData.txt
if [ -f UnicodeDataCombClassInfo.c ]; then
  rm UnicodeDataCombClassInfo.c
fi
./pdf-text-generate-ucd UnicodeData.txt COMBCLASS


#Generate Combining Class file...
check_file auxiliary/WordBreakProperty.txt
if [ -f WordBreakProperty.c ]; then
  rm WordBreakProperty.c
fi
./pdf-text-generate-ucd WordBreakProperty.txt


#Generate Property List file...
check_file PropList.txt
if [ -f PropList.c ]; then
  rm PropList.c
fi
./pdf-text-generate-ucd PropList.txt



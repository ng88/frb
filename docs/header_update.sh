#! /bin/sh

# Author: ng <ng@ngsoft-fr.com>

#Parameters : header_update sharp|star files

htype="$1"

shift

echo Mode is $htype.

for i in $*
do
  echo Processing $i...
  if [ "$htype" == "star" ]
      then
      sed -i  -e 's/Copyright (C) 2006, 2007 by GUILLAUME Nicolas /Copyright (C) 2006 - 2010 by GUILLAUME Nicolas/g' $i
      #sed -i  -e 's/Copyright (C) 2006 - 2010 by GUILLAUME Nicolas/Copyright (C) 2006, 2007 by GUILLAUME Nicolas /g' $i
  fi

  if [ "$htype" == "sharp" ]
      then
      sed -i  -e 's/Copyright (C) 2006, 2007 by GUILLAUME Nicolas /Copyright (C) 2006 - 2010 by GUILLAUME Nicolas/g' $i
  fi

done


echo Done.


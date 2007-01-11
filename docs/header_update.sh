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
      sed -i -e '/\*   (at your option) any later version.                                   \*/d' -e 's/either version 2 of the License, or     \*/version 2 of the License only.          */g' -e 's/Copyright (C) 2006 by GUILLAUME Nicolas      /Copyright (C) 2006, 2007 by GUILLAUME Nicolas/g' $i
  fi

  if [ "$htype" == "sharp" ]
      then
      sed -i -e '/#   (at your option) any later version.                                   #/d' -e 's/either version 2 of the License, or     \*/version 2 of the License only.          */g' -e 's/Copyright (C) 2006 by GUILLAUME Nicolas      /Copyright (C) 2006, 2007 by GUILLAUME Nicolas/g' $i
  fi

done


echo Done.


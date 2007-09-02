#!/bin/bash

###########################################################################
#   This file is part of the FrB project                                  #
#                                                                         #
#   FrB project                                                           #
#                                                                         #
#   Copyright (C) 2006, 2007 by GUILLAUME Nicolas                         #
#   ng@ngsoft-fr.com                                                      #
#                                                                         #
#   http://frb.ngsoft-fr.com/                                             #
#                                                                         #
#   This program is free software; you can redistribute it and/or modify  #
#   it under the terms of the GNU General Public License as published by  #
#   the Free Software Foundation; either version 2 of the License, or     #
#   (at your option) any later version.                                   #
#   See the COPYING file.                                                 #
###########################################################################

# Execute all the test in the folder passed as argument
# for each .frb file in the directory, a .frb.result file
# must exists (containing the expected result)

if ! [ -d $1 ]
then
    echo usage: $0 folder >&2
    exit 1
fi

cd $1

for i in *.frb
do
    if [ -r $i -a -r $i.result ]
    then
    else
        echo "warning: can't read $1/$i or $1/$i.result" >&2
    fi
done


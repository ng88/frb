#!/bin/bash

###########################################################################
#   This file is part of the FrB project                                  #
#                                                                         #
#   FrB project                                                           #
#                                                                         #
#   Copyright (C) 2006 - 2010 by GUILLAUME Nicolas                        #
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

# Create the .result file from the .frb test file



# USAGE
# type ./make_test_result file1.frb ... fileN.frb
#



[ "$FRB_PATH" = "" ] && FRB_PATH=../parser/frbc

if ! [ -x "$FRB_PATH" ]
then
    echo "unable to execute FrB ($FRB_PATH), please set the FRB_PATH environment variable." >&2
    exit 1
fi


for i in $*
do
    if [ -r $i ]
    then
	echo -n "Making result for $i: "

	"$FRB_PATH" --file $i 2>&1 | sed -e 's/0x[0-9a-fA-F][0-9a-fA-F]*/@ADDR@/g' > $i.result
	if [ $? -eq 0 ]
	then
	    echo success
	else
	    echo FAILED
	    exit 3
	fi

    else
        echo "warning: can't read $i" >&2
    fi
done

echo Done.
